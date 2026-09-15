# nugget-fwlib — canonical lineage registration

**Status:** `validated / active lineage`

**Current source:** [RandoSY/nugget-fwlib](https://github.com/RandoSY/nugget-fwlib)

`nugget-fwlib` is a Great Cow BASIC firmware framework for PIC16F18424 teaching instruments in the REDBOARD / PIC Black Box / Nugget pathway.

Its explicit learning progression is:

`555 logic -> PIC behavior -> serial protocol -> dashboard instrument -> higher-level node`

The library is organized around behavior classes rather than miscellaneous snippets, including GPIO, timing, analog measurement, UART/text protocol, data handling, devices, diagnostics, examples, and host-side tools.

## Why it belongs here

This repository is part of the durable Nugget idea: keep embedded behavior understandable while progressively assembling a useful instrument. It is therefore lineage beneath `nugget-physical-computing`, not a competing top-level program.

## Attribution and licensing

The existing source explicitly credits the Piconomix `px-fwlib` organizational influence and contains a separate attribution file. Its repository license file proposes an MIT license for estate-owned material.

Because attribution boundaries matter, the existing repository remains the source of record until a deliberate consolidation preserves both the license and attribution information.

## Migration disposition

- Keep the existing repository intact as known source/history.
- Migrate selected teaching examples only after confirming their current runnable state.
- Preserve `ATTRIBUTION.md` with any copied library material.
- Do not copy generated build artifacts merely to reproduce history.
- Classify utilities that serve several projects under `embedded-tools` rather than duplicating them.

## Next action

Identify the smallest known-good PIC16F18424 example sequence that demonstrates LED -> input -> timing -> ADC -> UART and package that sequence as the first Nugget reproducibility test.
