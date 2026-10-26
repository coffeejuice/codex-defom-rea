# CP149 independent review — exact one-row `019\r\n`

## Result

No correctness defect or admission leak found. The reviewed files match the CP149 freeze: `simcontrol300_native_io.cpp` SHA-256 `E8CEF08D79CC152026C432BDB012D0017BCDB10FDCF53859F382AF58046A7759`; `test_simcontrol300_spring_reader.cpp` SHA-256 `4E8D3A49F1392B4C2BCC2A5AED8B01E1448DE4ABEE60F027CAD2991206757811`.

## Exact branch and cursor

The new branch requires `allowSingleRowQualifiedToken` and full input equality with `019\r\n`. In the reader this permission is passed only for `count == 1 && i == 0`. For the exact bytes, initial whitespace scanning leaves `start == 0`; the branch sets `cursor = start + 2`, clears the comma state, and returns 1. It therefore represents the octal prefix `01` while leaving the `9` at offset 2. With count one, the caller does not retry at that cursor. This matches the retained Qt base-zero/octal source decision and caller count loop described in the proposal; it does not implement a general octal parser.

## Test and guard check

The focused one-row fixture supplies a complete `SPRING.INI`, verifies `019\r\n` returns one row with flag 1, and checks both input files byte-for-byte. Adjacent one-row variants `0190\r\n`, `019 \r\n`, leading whitespace, and trailing whitespace still refuse. The same exact `019\r\n` and nearby forms refuse for the complete two-row fixture; the three-row fixture separately refuses `018` and `09`. Existing exact `018` and `09` profiles remain separately tested with their expected values, so this change does not conflate octal `019` with decimal `09` or the count-two retry behavior for `018`.

The output test does not expose or assert a cursor field, but the source directly sets the cursor to offset 2 and the one-row loop makes no second extraction. The public contract has no status/cursor accessor, so this is an implementation inspection limit rather than a behavioral test defect.

No blocker found. No source edit, build/CTest, GUI, original runtime, or mutable REA operation was performed.
