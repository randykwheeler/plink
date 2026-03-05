## 2024-05-18 - Buffer Overflow in UNIX Domain Socket Path
**Vulnerability:** A `strcpy` was used to copy the `host` variable into `sau.sun_path`, which is a fixed-size buffer inside `struct sockaddr_un`. An attacker could provide a `host` path larger than the buffer, leading to a buffer overflow.
**Learning:** Fixed-size buffers from system headers, like `sun_path` in `sockaddr_un`, are easily overlooked. Never use `strcpy` with an uncontrolled input length, even if it seems safe because it represents a 'path' or 'host'.
**Prevention:** Always validate the length of input strings against the destination buffer's capacity using `sizeof()` before copying. Use `strncpy` and explicitly null-terminate the buffer for defense-in-depth.
