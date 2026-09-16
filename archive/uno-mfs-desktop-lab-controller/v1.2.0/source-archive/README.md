# UNO/MFS Desktop Lab Controller v1.2.0 — Exact Recovered Source Archive

This directory preserves an exact deterministic archive of the surviving v1.2.0 source package recovered from the Intellectual Estate Library.

## Contents of reconstructed archive

- `UNO_MFS_Desktop_Lab_Controller_Arduino.ino`
- `uno_lab_bridge.py`
- `ARCHITECTURE(1).md`

## Reconstruct

```bash
cat UNO_MFS_Desktop_Lab_Controller_v1.2.0_recovered.tar.gz.b64.part* > package.b64
base64 -d package.b64 > UNO_MFS_Desktop_Lab_Controller_v1.2.0_recovered.tar.gz
sha256sum UNO_MFS_Desktop_Lab_Controller_v1.2.0_recovered.tar.gz
tar -xzf UNO_MFS_Desktop_Lab_Controller_v1.2.0_recovered.tar.gz
```

Expected archive SHA-256:

`a9ecc3c2d58a8934846be14fc5398719745c19d119682d47ad0562ace7e24d1c`

Recovered source hashes:

- `UNO_MFS_Desktop_Lab_Controller_Arduino.ino` — `fb11157c78b1ed2ba6241b394ca61b6e4d2a678edf71fb347ae68cd3a094f135`
- `uno_lab_bridge.py` — `ae1543e941e117ce4f8aa63b1daf026b4173114cc7d40ceae29f6f44de8e8521`
- `ARCHITECTURE(1).md` — `616fb549c831a848447ec9a1680141433b1647d851b47b15f6fd9d1d2c8ca0a4`

## Validation boundary

The surviving v1.2.0 professional guide records the following status:

- strict host C++ compile with g++: PASS
- strict host C++ compile with clang++: PASS
- ATmega328P AVR-target relocatable object: PASS
- firmware contract for pin map / protocol v2 / persistence / watchdog: PASS
- final Arduino AVR core link / HEX in the documented environment: NOT AVAILABLE
- physical UNO + sensors bench validation: REQUIRED

Recovery preserves that status exactly. It does not promote the package to hardware-validated merely because the source has been recovered.
