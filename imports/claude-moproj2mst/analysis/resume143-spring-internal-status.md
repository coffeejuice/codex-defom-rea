# CP143: SPRING2 EOF status and reachable callers

## Finding

For the exact readable companion bytes `0X` with no trailing byte, the retained Qt 4 parser predicts flag value `0`, cursor at EOF (byte offset 2), and fresh `QTextStream` integer-extraction status `ReadPastEnd` (numeric status 1). The converter's reachable `SPRING2.INI` caller does not inspect that status. It checks `atEnd()` before each requested extraction, copies the integer destination after extraction, increments its loop index, and returns success after the file opened. For a one-row SPRING file, the failed extraction's zero is therefore the only externally relevant value. A private candidate model of `(value=0, nextByte=2)` is sufficient for this closed case; adding sticky stream status to public state is not required.

## Source qualification

The Qt evidence is the original `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, with PE-byte-checked captures and interpretation recorded in `analysis/resume83-sw2sp-parser-independent-review.md`. Its `getInteger` path case-folds before recognizing the `0X` prefix. With no following character, it consumes the prefix and reaches the EOF failure branch. The same capture's table gives `0x` value 0, next offset 2/EOF, fresh operator status 1; uppercase `0X` is a deduction from the recorded `toLower()` call before prefix selection, not an independently captured runtime row. The exact token's expected Qt state is therefore statically inferred.

The original `FUN_1410da690` decompile is retained at `analysis/wave2-save-diffs-300-support-1410da690.c`. It zero-initializes `count` output integers, opens the `QFile`, constructs a `QTextStream`, and loops on `!atEnd() && i < count`. In the body it passes the current output slot to `operator>>(int&)`, then unconditionally increments `i`. It never calls `QTextStream::status()` and has no branch on the operator's return value or status. When `0X` is initially nonempty, one extraction is attempted; the operator writes zero, the stream reaches EOF, and for count one the helper exits and returns true after successful open.

The candidate's `NativeExtra3DIo::readSprings` performs the companion extraction within the declared count and stores its returned integer in `SpringRecord::flag`. Its exact `0X` admission advances the private cursor by two and returns zero. `fillExtra3DImpl` only consumes `readSprings`' Boolean and returned records: it rejects a false result paired with nonempty output, and processes nonempty successful records. It does not receive a QTextStream status. Thus the reachable Open projection also has no status-dependent branch. The helper Boolean is an open/read-profile result, not QTextStream conversion status.

The original Save path is separately qualified in `analysis/resume120-spring2-writer-order.md`: `FUN_1410db030` serializes the stored integer flag and chooses write/remove behavior based on whether any row flag is nonzero. It does not recover a prior stream status or use it as a Save decision. This does not extend the CP141 input qualification to Save or assert all file-error behavior.

## State representation and boundary

For current one-row `0X` admission, keep the private reader contract as parsed `flag=0` plus its internal cursor advancement to EOF. Do not add `QTextStream` status to `SpringRecord`, `Extra3DIo`, `IterationPage`, or public session state: no reachable consumer branches on it, and the original helper returns success despite this extraction failure.

If later multi-extraction cases need modeled status transitions, keep that state internal to a parser result (for example, getter result, next unread position, and a private fresh/sticky status enum). That is a future parser requirement, not a requirement for this one-value token. In particular, status cannot replace the existing `atEnd()` cursor test: the retained `FUN_1410da690` loop is driven by input position and requested count, so a non-EOF failure with an unread character can be followed by another extraction even after status becomes nonzero. The `0X` EOF case has no remaining byte to trigger such a retry in a one-row file.

No source, build, GUI, original-process runtime, or mutable REA work was performed. Original process ACP is irrelevant for this exact ASCII token; the report does not claim runtime comparison.
