# CP155 complete unsigned SPRING2 binary implementation

Added a count-one complete-token route for unsigned ASCII binary flags `0[bB][01]+`. It folds bits into a `uint32_t` by shift-left-one/add and returns the signed interpretation of the low dword. It requires at least one bit and ASCII whitespace/EOF after the run; malformed suffixes and missing digits do not enter the route.

The existing exact `0B10`, invalid-first-bit `0B2`, and valid-prefix `0b12` handlers remain ahead of the general binary path. Count-two general binary tokens remain guarded; the exact `0b12` multi-row case remains separate. The existing ASCII/BOM/codec gate is unchanged. This follows the Qt prefix/digit loop and low-dword store bound in `analysis/resume156-spring-next.md`; no original-process comparison is claimed.

## Focused coverage

- `0b1\r\n` -> 1.
- `0b10\r\n` -> 2.
- `0B1010\t` -> 10.
- `0b10000000000000000000000000000000` -> `INT32_MIN` at EOF.
- `0B11111111111111111111111111111111\r\n` -> -1.
- 33-bit binary value `0B1` + 31 zeroes + `1` -> 1, proving low-dword wrap.
- All accepted cases assert byte-for-byte preservation of both files.
- Existing `0B10`, `0B2`, and `0b12` tests remain; malformed suffix, invalid-bit, and count-two complete-token cases are refused.

## Static preflight and freeze

- `git diff --check` passed for the owned source/test files.
- Inspected handler order and gates: exact status/cursor cases precede the new parser; the new path is strictly count-one and consumes only complete binary digits; all other parsers and codecs remain unchanged.
- No build or CTest was run, per assignment. Root owns integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `0870C34A0C18DCBA3A2026BA562AC6A2E3C8AE575A100BEE67A3919B6396745D`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `5779941FCFE83FBA867E3655E8D98021203DD29BE1CCEEDAC23FE72860FF768F`
