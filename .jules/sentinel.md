## 2024-05-24 - [Fix buffer overflow in Rconnection Unix socket path]
**Vulnerability:** A buffer overflow vulnerability in `Rconnection.cpp` where `strcpy(sau.sun_path, host)` was used without checking if the length of `host` exceeds the size of `sau.sun_path`.
**Learning:** Hardcoded fixed-size structs like `sockaddr_un` are susceptible to buffer overflows when copying user-controlled strings (like hostnames or paths) without length checks.
**Prevention:** Always use `strncpy` and ensure manual null-termination, or proactively check string lengths against the destination buffer's `sizeof()` before copying.
