# Modern Code Composer Studio setup (CCS 21.x)

Official CCS: https://www.ti.com/tool/CCSTUDIO (TI listed 21.0.1 in October 2026). CCS 21 uses the Theia-based interface, so older CCS 12 screenshots/tutorial import dialogs may look different.

1. Install CCS with the **MSP430 target support/compiler components**.
2. Attach your MSP-EXP430G2ET or original MSP-EXP430G2 over USB; check Windows Device Manager.
3. Choose **File / New Project** (or equivalent CCS New Project command), select **MSP430G2553** as the actual **device** and create a new **empty C** project. When the IDE offers board/debug connection, select **MSP-EXP430G2ET / eZ-FET** for newer board or your connected legacy debugger for original board. Do not choose a different MSP430 device.
4. Copy exactly two source files from ONE lesson folder into the project: `main.c` and `edu430.h` (or add them as source and project-local include). Avoid importing all lessons at once because each defines `main()`.
5. Build, flash and debug. If project creation cannot find the device support/linker file, install the MSP430 package/toolchain through CCS's component manager. The project should use the **MSP430G2553 target-specific linker command file** supplied by the installed device package.
6. For UART lessons, open a Windows serial terminal (e.g. PuTTY, Tera Term), select the LaunchPad virtual COM port, **9600 baud, 8 data bits, no parity, one stop bit, no flow control**.
7. On the original G2, configure RXD/TXD jumpers for **hardware UART** as marked on the PCB (older revisions vary), and note that legacy eZ430 debugger support is not equivalent across current operating systems/CCS versions. If its old on-board debugger is unsupported, use a supported MSP430 external programmer/debugger; the C firmware remains compatible.

This package intentionally does **not** include legacy Eclipse `.cproject`/`.ccsproject` files: fixed old compiler tool versions can prevent import into CCS 21. It provides **CCS-ready source projects** (one small folder per program), not IDE-generated project files. Test project import on your installed CCS configuration before classroom deployment.

Device header: `<msp430.h>` is supplied by the CCS MSP430 compiler/device support, NOT a file to download from this package. The 1 MHz examples depend on factory calibration constants; erased calibration must be repaired before running timed experiments.


## Lesson 11 — ready-to-import ProjectSpec candidates

Unlike the original source-only labs, the four Lesson 11 stage directories also contain a TI `*.projectspec` file for quick project creation in CCS. Import each ProjectSpec independently (or create an empty target project as above). **ProjectSpec XML was checked, but its import and MSP430 linking could not be tested on an installed CCS workstation here.** See [Lesson 11 CCS Projects](../lessons/11-tiny-multitasker/CCS_PROJECTS.md).
