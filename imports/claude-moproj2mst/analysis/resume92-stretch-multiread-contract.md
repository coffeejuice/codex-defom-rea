# CP92: STRETCH three-double stream contract

## Caller and read ordering

Original `FUN_140ec8110` calls `FUN_1410da690(..., 1)` for the `STRETCH.DAT` integer at decompile line 79. If that QFile read succeeds, it makes a separate `FUN_1410da510(..., 3)` call for three doubles at line 94. The keyword map binds the former literal `0x142b14e90` and the latter `0x142b14ea0` to `STRETCH.DAT`. This is two separate file opens and QTextStream instances, so the double reader restarts at byte zero; it does not continue from the integer extraction cursor. Candidate `fillExtra3D` follows the same order: one integer `readInts`, then a separate count-three `readDoubles` on `STRETCH.DAT`, and uses double destinations 1 and 2 for the displayed stretch values.

The original double wrapper `FUN_1410da510` is the same function reviewed for NBC in `resume92-nbc-multiread-contract.md`. Its destination initialization, per-value `atEnd` check, getReal/operator extraction, unconditional index increment, file-open success return, and lack of status inspection are independent of the requested count. The QtCore PE is the same SHA-qualified image and same `getReal`, `operator>>(double&)`, `atEnd`, QLocale-C and `ungetChar` branches cited in that report. No new native function witness is needed to derive a third extraction.

## Three-value predictions from source and current candidate

For the exact ASCII stream below, each extraction is a fresh well-formed decimal token under the same QTextStream:

| `STRETCH.DAT` bytes | Original three destinations | Candidate status |
|---|---|---|
| `1\r\n0.25\r\n0.5\r\n` | `[1, 0.25, 0.5]` | Existing `readReal` loop returns the same values. |
| `1\r\n0.25\r\n` | `[1, 0.25, 0]` | Third slot is initialized zero; original attempts a third read after skipping CR/LF, fails at EOF, writes zero; candidate consumes trailing whitespace then returns zero. |
| empty file | `[0, 0, 0]` | Both open-success paths skip extraction and keep initialized zeros. |

These outcomes follow from the retained sidecar loop plus QTextStream parser/operator sequence. Ordinary finite ASCII values and CR/LF delimiters use the already inspected getReal state table, QLocale-C numeric conversion, and unget delimiter branch. They do not require another original-process capture. The current candidate already supports this ASCII count-three path; a dedicated test would mainly pin the third value and partial EOF behavior from a public sidecar caller.

## Status and failure-cursor interaction

The generic double operator always invokes getReal even if status is already nonzero. On failure it writes 0.0 and sets status only if no earlier status exists; on success it stores the result but does not clear an earlier status. `FUN_1410da510` ignores both operator return and status, increments the index, and checks `atEnd` again. Thus status does not stop the next extraction; getReal's cursor/unget result controls which token that extraction sees.

For example, if the second token is a failed but consumed numeric prefix such as `1e+` followed by whitespace and then `0.5`, the next extraction can still skip the delimiter and parse the third token even though status remains set. If the second token begins with an invalid character that getReal ungets, the third extraction sees and fails on that same character again; later bytes are not reached within the fixed count. The candidate's `readReal` + fixed-count loop has the corresponding cursor effect. These edge inputs are useful tests if error continuation becomes the selected scope, but the next admission should start with valid rows and EOF tail only.

## Next safe test boundary and remaining guard

The smallest additional public regression is a candidate-only `STRETCH.DAT` fixture containing `1\r\n0.25\r\n0.5\r\n`, call `readDoubles` with exactly three values, and assert all three results. A second fixture `1\r\n0.25\r\n` asserts the initialized zero tail. These are ASCII inputs; no source change appears necessary because the current generic `readDoubles` loop already follows the source order.

Do not generalize the CP90 BOM decoder from B_WEIGHT count one to STRETCH count three based on this numeric loop review alone. Qt's BOM selection is common and statically established, but that would be a distinct decoder-admission change and should retain a separate exact path/count test plus malformed/non-ASCII guards. Current STRETCH count-three BOM files remain unsupported. Non-ASCII numeric code points, malformed BOM conversion, device read errors, long getReal boundary behavior, and process runtime comparison remain outside this contract.

No source or test was changed for CP92; no build, original-process session, GUI, or mutable REA action occurred.
