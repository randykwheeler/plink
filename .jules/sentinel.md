## 2024-05-24 - [Heap Buffer Overflow in Rconnection login]
**Vulnerability:** A heap buffer overflow in `Rconnection::login` due to a fixed-size `malloc` of 22 bytes padding for `authbuf` while using the system `crypt()` function, which can return strings larger than the hardcoded padding size (e.g. SHA-512 hashes).
**Learning:** Hardcoded buffer sizes are extremely brittle, especially when dealing with outputs from system functions whose implementation and return lengths can change over time or vary by OS.
**Prevention:** Always dynamically size allocations based on the measured string length of the inputs being copied (`strlen()` on inputs/outputs) and handle potential `NULL` returns from functions like `crypt()` and `malloc()`.
