## 2024-03-14 - [Heap Buffer Overflow]
**Vulnerability:** A heap buffer overflow and potential denial-of-service in `Rconnection::login` due to a statically sized buffer `malloc(strlen(user)+strlen(pwd)+22)` being used for `crypt()` output, and missing `NULL` check for `crypt()`.
**Learning:** `crypt()` hashing algorithms (like SHA-512) can produce strings longer than historical assumptions (22 bytes) and can fail, returning `NULL`.
**Prevention:** Calculate buffer sizes dynamically based on exact string lengths (e.g., `strlen(crypt_res)`) and always check for `NULL` returns from functions like `crypt()`.
