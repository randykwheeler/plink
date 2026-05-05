## 2025-05-05 - [Insecure PRNG Seed in PLINK]
**Vulnerability:** Weak PRNG seed using `time(0)` for `CRandom::srand` in `plink.cpp` and `plinklibhandler.cpp`.
**Learning:** `time(0)` provides predictable seeds (1-second resolution), which is dangerous for cryptography or sensitive randomizations.
**Prevention:** Always use a non-deterministic seed source like `std::random_device{}()` from `<random>` in modern C++ to initialize PRNGs.
