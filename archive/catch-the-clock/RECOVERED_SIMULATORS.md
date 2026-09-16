# Catch-the-Clock recovered simulators

This directory preserves surviving historical Catch-the-Clock source and browser applications recovered from the Intellectual Estate Library.

## Directly browsable source

- `catch_the_clock_observer_extended_metrics.gcb.txt` — earlier Great Cow BASIC observer/metrics source.
- `catch_the_clock_observer_reviewed.gcb.txt` — reviewed PIC16F18424 observer/reporter. The external 555/74LS00 timing, logic, and latch remain authoritative; the PIC observes and reports rather than replacing them.
- `static_spst_555_7400_pic_simulator.html` — previously recovered runnable browser simulator.

## Lossless gzip-preserved HTML

- `static_pushbutton_555_7400_pic_simulator.html.gz`
- `gsa_555_7400_pic_blackbox_sim.html.gz`

These `.gz` files are byte-preserving compressed copies of the recovered standalone HTML applications. Decompress with a normal gzip tool to restore the original `.html` file.

The compression is an archival transport choice only; it does not imply that the browser applications themselves were originally distributed compressed.

## Validation boundary

These are historical recovered artifacts. Their presence here proves recovery of the source/application files, not new hardware validation. Bench-dependent polarity, PIC/GCBASIC toolchain behavior, and physical timing should be reconfirmed before treating a historical configuration as a current validated release.
