# CCS 21.x setup — Lesson 11

Each stage includes a **TI `.projectspec` file** declaring the MSP430G2553 and the exact files to copy. This is intended to create an importable CCS project without pinning a legacy compiler version. TI documents ProjectSpec support in CCS; **these particular four specs have been XML-validated but not imported into a running CCS installation**. For maximum reliability, the empty-project instructions below remain available. If you have Codex-generated CCS projects from your local machine, you can replace **only the program source files** with these files and retain the verified project configurations.

## Option A: import a TI ProjectSpec (recommended trial)

In CCS choose **Project > Import CCS Projects** or the current equivalent. Browse to the folder containing a stage-specific `.projectspec`, select it, and import. Check that the device is **MSP430G2553**, the current installed MSP430 compiler is selected, and the four source sets remain separate. If your CCS version does not detect the spec, use Option B below; do not force an old `.cproject`.

## Option B: make four empty projects

1. In CCS, choose **Create New Project**, device **MSP430G2553**, an MSP430 C empty project, and the debugger corresponding to the connected LaunchPad (G2ET eZ-FET preferred).
2. Name the first project `MSP430_L11_01_Blocking`. Add `01-blocking/main.c` and `01-blocking/edu430.h` to the project root.
3. Repeat for `MSP430_L11_02_ExplicitTimer`, `MSP430_L11_03_RunEvery`, `MSP430_L11_04_EnergyTrace`, using files only from the matching stage folder. The last two also need `tiny_scheduler.h` and `button_filter.h`.
4. Build/flash **one at a time**. No Arduino, Energia, vendor RTOS, or external libraries are required. The TI device header `<msp430.h>` comes from installed MSP430 device support.
5. Open the USB virtual COM port with a terminal, **9600 baud, 8N1, no flow control**. Close any other serial monitor holding that port. Stage 4 defaults to **no UART traffic** to avoid biasing EnergyTrace.
6. On older G2, select hardware UART jumper orientation if available, check legacy debugger compatibility. MCU code is common to both boards.
7. Compile Stage 4 with `#define IDLE_LOW_POWER 0` and record an EnergyTrace run; rebuild with `1` and repeat. Keep `POWER_TEST_LED` and `POWER_TEST_UART` unchanged across both runs.

If your installed CCS import format differs, use source-only projects rather than importing legacy Eclipse metadata. The 1MHz factory calibration values must be present (not erased). Link with the correct MSP430G2553 linker script supplied by CCS.

## Debugging tips

- If UART prints garbage, verify target DCO calibration, terminal baud, and both RXD/TXD jumpers.
- If Timer_A seems stuck, confirm `TIMER0_A0_VECTOR`, `CCIE`, `TA0CCR0`, `TASSEL_2`, and global interrupts.
- If the CPU stays in LPM0, confirm `__bic_SR_register_on_exit(LPM0_bits)` is inside the Timer ISR in the low-power build.
- A held button must not increase the reported press count every 10ms; only new, debounced transitions increment.
- Keep target current consumption separate from the red/green board LED current when interpreting EnergyTrace.
