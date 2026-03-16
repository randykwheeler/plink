## 2024-03-16 - [Buffer Overflow in Rconnection::login]
**Vulnerability:** A heap-based buffer overflow existed in `Rconnection::login` because memory was allocated based on the plaintext password length, but then overwritten with the potentially much longer output of `crypt()`.
**Learning:** Always calculate memory allocation size *after* the final string length is determined, especially when transforming data using functions like `crypt()` that produce fixed or variable-length hashes independent of the input size.
**Prevention:** Calculate the final string to be copied before allocating memory, and always check the return value of functions like `crypt()` for NULL before using them in string operations like `strcpy()`.
