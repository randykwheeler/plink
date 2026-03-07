## 2024-05-24 - Buffer overflow in Rconnection.cpp

**Vulnerability:** A buffer overflow vulnerability was found in `Rconnection.cpp` at line 586 when connecting via Unix sockets. The `host` string passed to the constructor, and then directly passed to `strcpy(sau.sun_path, host)`, could overflow the `sun_path` buffer of size typically 108 characters on Unix systems if a large string was passed. This was even marked with a `// FIXME: possible overflow!` comment.

**Learning:** Fixed buffer overflow vulnerabilities due to an unchecked `strcpy` into a fixed-size `sockaddr_un::sun_path` buffer from user-provided input. Using simple boundary checks and returning an error code prevents a buffer overflow crash and potential code execution. `strlen(host) >= sizeof(sau.sun_path)` length check followed by `strcpy` is sufficient and prevents crashes.

**Prevention:** Ensure any strings, specifically file paths or network addresses given by user or calling context, are length-checked against target fixed-size buffers, or use safer variants like `strncpy`/`snprintf` to prevent buffer overflows, especially in C/C++ networking routines like `connect`.
