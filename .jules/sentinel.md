## 2024-05-18 - Buffer Overflow in UNIX Socket Connection Path
**Vulnerability:** A `strcpy(sau.sun_path, host)` call in `Rconnection.cpp` could lead to a buffer overflow if the `host` parameter exceeds the length of `sau.sun_path` (usually 108 bytes on many Linux systems).
**Learning:** Hardcoded fixed-size structs like `sockaddr_un` represent silent buffer overflow risks if user input isn't validated against their specific limits, particularly when relying on `strcpy`.
**Prevention:** Always validate string lengths against fixed buffer bounds before copying, or use bounds-checking functions (like `strncpy` but careful of null-termination). Prefer safer abstractions if available.
