## 2024-03-10 - Fix stack buffer overflow in Unix socket connect
**Vulnerability:** Stack buffer overflow in `Rconnection::connect()` when connecting to a Unix socket. The user-provided `host` string was copied into `sau.sun_path` (fixed size buffer) using `strcpy` without checking length.
**Learning:** `struct sockaddr_un.sun_path` has a fixed size (usually 108 bytes). Copying unbounded user input into it can overflow the buffer, potentially allowing arbitrary code execution or denial of service.
**Prevention:** Always verify that the string length is less than the destination buffer size before copying, or use safe string copying functions like `strncpy` or `strlcpy`.
