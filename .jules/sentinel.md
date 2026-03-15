## 2024-05-24 - Buffer Overflow in Crypt Authentication
**Vulnerability:** Heap buffer overflow and potential segfault in `Rconnection::login` due to fixed-size buffer allocation for `crypt()` output and missing NULL check.
**Learning:** `crypt()` output length can exceed the original password length and can return NULL on failure. Fixed-size buffers based on input length are unsafe for cryptographic outputs.
**Prevention:** Dynamically allocate buffers based on the actual length of cryptographic outputs and always check for NULL return values from functions like `crypt()`.
