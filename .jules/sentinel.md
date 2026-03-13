## 2024-03-13 - [Buffer Overflow Risk in Rserve Login]
**Vulnerability:** In `Rconnection.cpp`'s `login` function, the `authbuf` buffer size is fixed at `strlen(user)+strlen(pwd)+22`, which is too small because the result of `crypt(pwd, salt)` may be longer than `strlen(pwd) + 22` or `pwd` length isn't considered properly. The result of `crypt` might overflow `authbuf` and segfault if `crypt` returns NULL.
**Learning:** `crypt()` is used for Rserve authentication and its return value length may exceed the allocated buffer or return `NULL`.
**Prevention:** Dynamically allocate `authbuf` *after* checking `crypt` results or use a safely sized buffer and `strncpy`/safe concatenation and check for `NULL` from `crypt()`.
