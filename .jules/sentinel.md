## 2024-03-08 - Stack buffer overflow in UNIX socket path
**Vulnerability:** A `strcpy` into `sockaddr_un::sun_path` without bounds checking in `Rconnection::connect` allowed arbitrary length host strings to overflow the stack buffer.
**Learning:** Network-facing inputs, even local socket paths, must always be bounds-checked when copying into fixed-size structures.
**Prevention:** Use `strncpy` with proper size calculation `sizeof(sau.sun_path) - 1` and manual null termination, or `snprintf`.
