## 2024-05-15 - [Initial Sentinel Setup]
**Vulnerability:** Initializing sentinel logs
**Learning:** Sentinel guidelines established
**Prevention:** Follow boundaries and guidelines
## 2024-05-15 - [sisocks.h Buffer Overflow Mitigation]
**Vulnerability:** snprintf emulation used vsprintf ignoring buffer limits, and sockerrorchecks used strncpy which could leave strings unterminated. Also used unbounded sprintf on non-Unix platforms.
**Learning:** Legacy C++ headers for system-independent socket logic (e.g. sisocks.h) often contain unsafe string operations like vsprintf or strncpy that need standardizing to snprintf / _vsnprintf to guarantee null termination and buffer safety.
**Prevention:** Always use snprintf with correct buffer lengths instead of strncpy/sprintf, and ensure snprintf emulations (like on Windows) use bounded alternatives like _vsnprintf with explicit null termination of the final byte.
