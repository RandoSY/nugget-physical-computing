# Edu430 — ST VL6180X Hardware I²C Exemplar

**Extension to Edu430 FwLib v0.2**. Teaches a reusable hardware-I²C transaction
and a separate optical range sensor driver on the MSP430G2553 LaunchPad.

- `include/edu430_i2c_reg16.h`: USCI_B0 master write/read transaction with
  7-bit addressing, 8/16-bit register support, repeated START, bounded
  polling, and distinct NACK / timeout / bus-busy errors.
- `include/edu430_vl6180x.h`: driver for the **VL6180X specifically**, with
  sensor identity check, ST startup settings, single-shot range in mm, and
  user-visible status codes; transport-independent through callbacks.
- `examples/09-vl6180x-hardware-i2c/`: self-contained example including
  a `.projectspec` for Code Composer Studio and UART output.
- `tests/`: host syntax and deterministic sensor behavior tests.
- `docs/ST_VL6180X_EXEMPLAR.md`: wiring, engineering explanations, exercises,
  register-to-macro expansions, failure modes, and validation protocol.

This is deliberately an **additive module**, preserving all existing lessons,
and it is **NOT hardware qualified**. Follow the documented 3.3 V breakout
requirements and the old/new LaunchPad P1.6 LED jumper instructions.
