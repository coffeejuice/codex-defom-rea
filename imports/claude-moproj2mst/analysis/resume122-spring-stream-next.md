# CP122 next bounded `SPRING2.INI` integer-stream case

## Selected case: C-locale comma delimiter after a decimal prefix

The next small source-closed branch after CP121's `x` cases is `1,234`, exercising Qt's C-locale grouping-separator decision. The retained QtCore evidence does **not** parse this as a grouped integer: the default stream number-options path ungets the comma after the decimal prefix. This creates a distinct delimiter branch from CP121's ordinary `x` terminator and can affect the next companion flag extraction.

Candidate `readSpringFlagInt` currently rejects comma-attached forms. That remains a conservative unsupported-input boundary, but it is not behaviorally identical to the original.

## Source evidence and stream result

Original application identity: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtCore identity: `QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/resume73-sw2sp-comma-prefix.md` retains the exact QtCore control-flow qualification for `1,234`: C-locale QTextStream integer extraction consumes the decimal `1`, returns success, and leaves the comma unread. The getter compares the incoming character with locale separators and the stream's number-options flags; with the default flags the comma follows the unget path rather than grouping accumulation. This is the same shipped QtCore `QTextStream >> int` operator used by `FUN_1410da690` for companion flags. Numeric locale C is established independently by `analysis/resume67-sw2sp-locale.md`.

The original companion helper `FUN_1410da690` is in `analysis/wave2-save-diffs-300-support-1410da690.c`. It pre-zeroes the output, then loops while `!atEnd()` and `i < count`; each operator call is followed by an unconditional loop-index increment, and the helper returns true after successful file open without checking conversion status. `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm` shows `atEnd()` tests buffer position/length or the underlying device end state, not conversion status. The operator failure path in `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` stores zero and sets a nonzero status only if no prior status is set; it does not gate another extraction while buffered/device input remains.

## Bounded static deductions

For an ASCII `SPRING2.INI` opened successfully:

| Bytes | Requested count | Original-static flags | Cursor/status consequence |
|---|---:|---|---|
| `1,234` | 1 | `[1]` | First extraction consumes `1`, ungets comma; the loop reaches its count limit, so the comma is not requested again. |
| `1,234` | 2 | `[1,0]` | Second extraction starts at comma, fails with no digits, writes zero, and leaves comma unread. The helper increments to count and returns true. |
| `1,234` | 3 | `[1,0,0]` | Later attempts see the same unread comma, write zero again, and preserve the already-set stream status. |

These are static deductions from the Qt getter/operator plus the original loop. They are not a fresh original-process run. Suffix digits `234` are never reached after the comma remains unread.

## Candidate boundary and smallest follow-up

The candidate currently rejects comma in any requested flag token and clears public output on that guard. This is safer than inventing general grouping behavior, but the CP122 cases above are specific, source-backed mismatches. A future isolated increment could add only the exact comma-stop prefix behavior: parse the leading canonical decimal digit run, leave a following comma unread, and on any subsequent requested extraction at that comma return zero without advancing. Keep the branch bounded to ASCII, signed int32 prefix values, `SPRING2.INI`, the existing `count <= 256` reader profile, and its existing caller extraction count. Do not claim grouped values or allow unrelated punctuation.

Suggested tests: count one with `1,234` -> flag `[1]`; count two with `1,234` -> `[1,0]`; count three -> `[1,0,0]`; retain CP121 `1x`/`x 2` results; reject malformed grouping variants such as `1,,2`, `1,2,3`, comma-first input, overflow, non-ASCII and BOM. Verify both files remain unmodified.

No source, test, build, GUI, original-process, or mutable REA state changed for this read-only audit.
