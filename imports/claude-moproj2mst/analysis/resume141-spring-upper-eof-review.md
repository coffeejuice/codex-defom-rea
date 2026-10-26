# CP141 independent review: one-row SPRING2 uppercase `0X` at EOF

## Result

No blocker found. The new admission requires count-one/first-flag permission and full companion equality to `0X`. It returns public zero and advances to EOF at offset 2. Retained Qt evidence supports the same EOF result as lowercase `0x` because Qt case-folds the prefix before base selection. The candidate does not model the internal EOF conversion status 1, as the implementation receipt states.

## Frozen hashes

The reviewed files match the CP141 receipt:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `ad1633cd791dcb541f33bb070741f72b4df73b54b2bbd706323264832c37bb22` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `ee84fbf08d6bf6bd9e9718434e593e00543a5c696ac7bcf08d6b434122c7f444` |

## Findings

- `exactUpperBareHexEofZero` is gated by the one-row token permission and exact whole-file bytes `0X`; its branch sets cursor to `start+2`, so for the exact input the cursor is EOF at byte offset 2. The existing lowercase EOF and uppercase/lowercase CRLF branches remain distinct.
- The lower-case `bareHexPrefix` guard does not match uppercase `0X`, but the explicit uppercase predicates handle only exact one-row `0X` and `0X\r\n`. Other uppercase forms fall through to the bounded parser and remain unsupported; tests explicitly refuse `0X` and `0X\r\n` at count two, `0XG\r\n`, and spaced one-row variants. The one-row focused case asserts success, flag zero, and byte-exact preservation of both files.
- Qt's `QChar::toLower` before prefix selection makes `X` equivalent to `x`. The retained lowercase `0x` table records prefix consumption, EOF at offset 2, getter failure, destination zero and fresh status 1. Applying that same case-folded path to uppercase is a static derivation rather than a separate captured table row or original-process comparison.
- The helper begins extraction because the input is initially nonempty, copies the operator's zero value, ignores conversion status, and returns true after its one requested flag. Thus public zero is supported; internal status remains unmodeled by candidate code and test.

## Validation boundary

No source/test changes, build/test execution, GUI/original application interaction, or mutable REA operation occurred. Root owns integrated validation.
