#include <cassert>
#include <cstring>
#define Win32
#define MAIN
#define SOCK_ERRORS
#define USE_SNPRINTF
#include "../sisocks.h"

int main() {
    struct { char buf[8]; char canary[8]; } value;
    std::memset(&value, 'X', sizeof(value));
    assert(snprintf(value.buf, 0, "%s", "abcdefghij")==10);
    assert(value.buf[0]=='X');
    assert(snprintf(value.buf, 1, "%s", "abcdefghij")==10);
    assert(value.buf[0]==0 && value.buf[1]=='X');
    assert(snprintf(value.buf, 8, "%s", "1234567")==7);
    assert(std::strcmp(value.buf, "1234567")==0);
    assert(snprintf(value.buf, 8, "%s", "123456789")==9);
    assert(std::strcmp(value.buf, "1234567")==0 && value.canary[0]=='X');
    assert(snprintf(NULL, 0, "%d", 1234)==4);
    assert(sockerrorchecks(NULL, 0, -1)==-1);
    WSASetLastError(12345);
    sockerrorchecks(value.buf, 8, -1);
    assert(value.buf[7]==0 && value.canary[0]=='X');
    puts("PASS real Windows snprintf lengths, return values, canaries and socket bounds");
}
