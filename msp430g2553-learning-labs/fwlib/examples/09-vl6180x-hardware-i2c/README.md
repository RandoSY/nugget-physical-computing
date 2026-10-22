# Example 09: ST VL6180X on hardware USCI_B0 I2C

This is a classroom prototype, not a hardware-qualified sensor driver.

1. Select **MSP430G2553** in TI Code Composer Studio.
2. Import `EDU430_09_VL6180X_HW_I2C.projectspec`, or create a device project and add only the five C/header files in this directory.
3. Check the breakout’s 3.3-V I/O specification. Connect SDA→P1.7, SCL→P1.6, and common GND. Add pullups to **3.3 V** if needed, and remove the LaunchPad **LED2 jumper** on P1.6.
4. Set the original G2's TX/RX jumpers to hardware UART where required. Open the Windows virtual COM port at **9600-8N1**.
5. Flash, reset, and observe model verification and `distance_mm` lines.
6. Expect ~0–100 mm of useful range in suitable conditions; log surface reflectivity and ambient light effects.
7. See `../../docs/ST_VL6180X_EXEMPLAR.md` for the complete bus register breakdown and verification protocol.

**Status:** Host compiler and simulated sensor tests passed; no TI compiler/hardware verification yet.
