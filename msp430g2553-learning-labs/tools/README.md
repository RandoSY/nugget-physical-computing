# Offline host syntax check

`python3 tools/check_sources.py` uses GCC and a deliberately incomplete *mock* `msp430.h`. It can catch malformed C string literals, missing semicolons, and many syntax errors. It does **not** link/flash MSP430 binaries and does not check actual device registers, ISR vector mappings, compiler-specific pragmas, flash/RAM usage, physical wiring, or peripheral timings. Do not confuse this with hardware qualification.
