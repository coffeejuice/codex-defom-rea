# Checkpoint 60: exact `0x10` Extra3D increment

## Implementation

Extended the bounded `readDecimalInt` profile to accept exactly lowercase `0x10`, followed by end-of-input or ASCII whitespace, as value 16. Existing exact `0x2` and `010` cases are unchanged. Broader hexadecimal forms, uppercase prefixes, binary syntax, other octal forms, signed prefixes, overflow, and malformed/partial-row status remain guarded.

The candidate-only `SW2SP.DAT` test now reads `0x10\r\n` through a one-element integer span and asserts 16. The stale unsupported expectations for `0x10` were removed. Related unsupported syntax remains covered.

## Qualification

`analysis/resume60-numeric-next.md` binds this branch to the original Extra3D `SW2SP.DAT` integer read and the SHA-256-qualified QtCore getter. The original getter chooses base 16 after lowercase `0x`, then shifts by four bits and accumulates hexadecimal digits; its `0x10` result is 16. This implementation is intentionally a single exact token rather than a general hex parser.

No test executable or build was run; root owns validation. No GUI, REA, mutable fixture, shared/CMake, or other source files were changed.

## Freeze hashes

SHA-256 after these edits:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `E5BF259A05E74182BD8462020513DBBD0171BF6115BC3825CF6453B716D94FE4`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `8634295D52F5BB861464A3DAACA174EDCD21D274743A33D7A444F3515EF66AF6`
