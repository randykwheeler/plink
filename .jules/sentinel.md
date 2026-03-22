## 2024-05-09 - [Heap Buffer Overflow in Rconnection.cpp login()]
**Vulnerability:** In `Rconnection.cpp::login()`, memory is allocated with a hardcoded `malloc(strlen(user)+strlen(pwd)+22)`, but the output of `crypt(pwd, salt)` may be longer than `strlen(pwd)+21`, potentially causing a heap buffer overflow when `strcpy` writes the crypted string into `authbuf`.
**Learning:** Hardcoded buffer sizes added to string lengths can be unsafe when handling variable-length outputs from functions like `crypt()`.
**Prevention:** Always dynamically size buffers based on the actual length of the output string (e.g., using `strlen(crypt(pwd, salt))`), and handle possible `NULL` returns from functions like `crypt()` securely.
