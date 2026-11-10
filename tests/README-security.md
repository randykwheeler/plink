# Security regression checks

Run from the repository root in an isolated Linux environment with g++, Python 3 and libcrypt development files:

```sh
sh tests/run-security.sh
```

These checks execute the real Rconnection implementation and socket receive function. The socket check substitutes only the logging object, allowing a local peer to close before data and after a partial reply. Rconnection checks send assignments through a socket pair, validate the wire payload and zero padding around 64 KiB and 1 MiB boundaries, reject lengths that cannot fit a short Rserve parameter and reject allocation-length overflow. An empty expression regression checks safe initialization and destruction. Socket error checks cover zero-length and truncated outputs.

AddressSanitizer and UndefinedBehaviorSanitizer run with undefined behavior treated as failure. LeakSanitizer is disabled because the ephemeral runtime cannot read the process task directory; this suite does not establish absence of leaks. The source still emits pre-existing string-literal conversion warnings.

`security_polyfill.py` compiles the actual polyfill function extracted from sisocks.h with an emulated legacy CRT that returns -1 and leaves no terminator on truncation. It tests lengths 0, 1, exact fit and truncation, canaries and required-length return values. This does not substitute for a real Windows runtime test.

For Windows, build and run the platform-specific test with the supported Windows toolchain. A MinGW cross-build command is:

```sh
x86_64-w64-mingw32-g++ -std=c++98 -static -static-libgcc -static-libstdc++ tests/security_windows.cpp -lwsock32 -o security-windows.exe
```

Run `security-windows.exe` on an isolated Windows runner. The real Windows test was cross-compiled successfully during this change but not executed on Windows. The complete Rconnection source also cross-compiles with `-std=c++98 -DWin32 -DUSE_SNPRINTF` after correcting the existing pointer-to-long diagnostic for Win64.

Before the fix, the assignment test observed AddressSanitizer heap-buffer-overflow: a 65,535-byte write into a 16-byte allocation in Rconnection::assign (exit 1). The peer-close test observed UndefinedBehaviorSanitizer index -1 out of bounds in sockets.cpp (exit 1). The corrected Linux checks passed three consecutive runs, followed by the integrated polyfill check. Full application build, live Rserve interoperability and Windows runtime execution remain separate validation steps.
