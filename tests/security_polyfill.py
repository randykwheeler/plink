#!/usr/bin/env python3
import pathlib
import subprocess
import tempfile

source = pathlib.Path("sisocks.h").read_text()
start = source.index("int sisocks_snprintf(")
end = source.index("\n#else\nextern int sisocks_snprintf", start)
function = source[start:end]
harness = r"""
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
int _vsnprintf(char *buf, int len, const char *fmt, va_list ap) {
    int n=vsnprintf(buf, len, fmt, ap);
    if (n >= len) {
        memset(buf, 'T', len);
        return -1;
    }
    return n;
}
int _vscprintf(const char *fmt, va_list ap) {
    return vsnprintf(NULL, 0, fmt, ap);
}
""" + function + r"""
int main() {
    struct { char buf[8]; char canary[8]; } value;
    memset(&value, 'X', sizeof(value));
    assert(sisocks_snprintf(value.buf, 0, "%s", "abcdefghij")==10);
    assert(value.buf[0]=='X');
    assert(sisocks_snprintf(value.buf, 1, "%s", "abcdefghij")==10);
    assert(value.buf[0]==0 && value.buf[1]=='X');
    assert(sisocks_snprintf(value.buf, 8, "%s", "1234567")==7);
    assert(strcmp(value.buf, "1234567")==0 && value.canary[0]=='X');
    assert(sisocks_snprintf(value.buf, 8, "%s", "123456789")==9);
    assert(value.buf[7]==0 && value.canary[0]=='X');
    assert(sisocks_snprintf(NULL, 0, "%d", 1234)==4);
    puts("PASS polyfill length/canary/return tests with emulated legacy CRT truncation");
}
"""
with tempfile.TemporaryDirectory() as directory:
    directory = pathlib.Path(directory)
    cpp = directory / "polyfill.cpp"
    cpp.write_text(harness)
    binary = directory / "polyfill"
    subprocess.run(["g++", "-std=c++98", "-O1", "-g", "-fsanitize=address,undefined", str(cpp), "-o", str(binary)], check=True)
    subprocess.run([str(binary)], check=True)
