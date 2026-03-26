## 2024-05-24 - [Heap Buffer Overflow in Rserve Interface Login]
**Vulnerability:** A heap buffer overflow occurs in `Rconnection::login` when allocating `authbuf`. The buffer size is calculated using the plaintext password length, but the `crypt(pwd, salt)` result (which can be significantly longer) is later copied into it using `strcpy`.
**Learning:** System APIs like `crypt()` that return variable-length strings should always have their outputs captured, validated (for NULL), and measured *before* being used to allocate fixed-size buffers for concatenation.
**Prevention:** Always pre-calculate required buffer sizes using the exact strings that will be copied. Add NULL checks for any system API that could potentially fail.
