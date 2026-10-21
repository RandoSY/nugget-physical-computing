# UART: publish measurements

**Concept:** USCI_A0 serial transmit.  
**Hardware:** USB backchannel, 9600 8N1.  
**Expected:** Terminal displays a growing event number every second.

Create a fresh CCS project for target MSP430G2553 and add `main.c` and `edu430.h` from this directory. Do not add multiple labs to one project because every lesson has its own `main()`.

## Inquiry challenge

Predict the outcome of changing a threshold, count, or delay before you run it. Record actual behavior and reconcile it with the registers shown in `edu430.h`.

**Discussion:** Which instructions describe intent? Which instructions modify a physical bit? What stays the same when moving from the original LaunchPad to the G2ET?

## What to inspect

Open `main.c` and read the `TEACHING NOTES` block. Translate at least one macro to `P1OUT`, `P1DIR`, `P1IN`, or the indicated peripheral register. Demonstrate behavior before and after one controlled parameter change.
