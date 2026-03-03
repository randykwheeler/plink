## 2025-03-03 - Stack Buffer Overflow in UNIX Domain Socket Initialization
**Vulnerability:** A critical stack buffer overflow was discovered in `Rconnection.cpp` where user-controlled `host` data was copied into `sau.sun_path` using the unsafe `strcpy` function.
**Learning:** Fixed-size structures like `sockaddr_un` (whose `sun_path` is strictly ~108 bytes depending on OS) must never be populated using unbounded string copying functions like `strcpy` when handling external input.
**Prevention:** Always use bounds-checking functions like `strncpy` combined with manual null-termination (`str[size-1] = '\0';`) to ensure destination buffers do not overflow.
