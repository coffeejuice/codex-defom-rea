# CP91 evidence: bounded multi-value double-reader state

## Reachable callers and candidate gap

Original Extra3D `FUN_140ec8110` reads two doubles from `NBC.DAT` with `FUN_1410da510(..., 2)` at source line 46 and three doubles from `STRETCH.DAT` with the same helper at line 94. The literal bindings are listed in `wave2-save-diffs-300-extra3d-keywords.json`. Candidate `fillExtra3D` has matching spans: `NBC.DAT` count 2 and `STRETCH.DAT` count 3. `readDoubles` currently sends both through the existing ASCII-only decoder; it admits the recognized BOMs only for one-value `B_WEIGHT.DAT`. Therefore, for example, a valid UTF-8-BOM `NBC.DAT` row is still refused by the candidate although the original default QTextStream decodes it.

## Original loop, extraction and partial-value behavior

The retained source decompilation `wave2-save-diffs-300-support-1410da510.c` establishes the wrapper order:

1. Initialize every requested double destination to 0.0.
2. Open the QFile ReadOnly and construct one default `QTextStream`.
3. Before each requested extraction, call `QTextStream::atEnd()`; if false, call `operator>>(double&)` into the current destination.
4. Increment the element index after the extraction call without testing its conversion result or QTextStream status.
5. Close and return success after successful open.

Retained QtCore is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The relevant primary captures are `wave2-save-diffs-qt-textstream-getreal-complete.asm`, `wave2-save-diffs-qt-textstream-read-double-complete.asm`, and `wave2-save-diffs-qt-textstream-atend-complete.asm`.

- `operator>>(double&)` always calls getReal at `0x670C6300` when the stream buffer/device has data; it does not gate that call on the existing QTextStream status field. On getReal failure it writes 0.0, computes internal status 1 when `atEnd()` is true or 2 otherwise, and assigns the status only if the current status is zero (`0x670C6321–634A`). Success stores the produced double (`0x670C6305–6316`) and does not clear a previously set status.
- `FUN_1410da510` ignores both the operator result and status and advances its loop index after every attempted extraction. Consequently a prior conversion failure does not stop later calls from attempting another value.
- `atEnd()` at `0x670C2660` first checks the QTextStream buffer position/length. If a buffer exists it compares those indices; otherwise it checks whether the device is at end. It does not consult QTextStream's conversion status. With a short local file fully buffered, a character ungot by getReal remains available to the next extraction and keeps `atEnd()` false.
- `getReal` (`0x670C59B0`) skips leading QChar whitespace, maps the ASCII real grammar through the retained table at `0x6722C880`, and ungets the first character that transitions to stop. It then passes the token to the Qt locale conversion path; default stream construction selects `QLocale::C` and no caller `setLocale` appears in the helper.

For ASCII examples, the retained transitions and wrapper establish these destination sequences (these are static deductions, not original-process observations):

| File bytes, requested count 2 | Public destinations | Reason |
|---|---|---|
| `1.5\r\n2.5\r\n` | `[1.5, 2.5]` | Both conversions succeed; CR/LF is skipped before the second getter. |
| empty | `[0, 0]` | `atEnd` is true before extraction; initialized values remain. |
| `1.5` | `[1.5, 0]` | First extraction succeeds at EOF; second loop check ends. |
| `x 2.5` | `[0, 0]` | Invalid first character is ungot; first operator writes zero/status 2; next iteration sees the same `x`, fails again, and writes zero. |
| `1e+ 2.5` | `[0, 2.5]` | Incomplete exponent token fails conversion; delimiter remains available; next extraction skips whitespace and parses the second number. |

The last two rows characterize PE-visible getter/operator/reader control flow but should remain outside the next implementation's admitted BOM profile until root chooses to include failure cursor behavior. Candidate `readReal` already continues its bounded loop after failure and does not carry an operator status field; this is sufficient for ordinary destination values only when its cursor and conversion result match each getter branch. It is not a complete reusable QTextStream state object.

## Smallest safe public profile

The smallest advancing public profile is an all-valid ASCII numeric row in the existing two-value `NBC.DAT` request, optionally beginning with one of the already identified BOMs, for example UTF-8 BOM plus `1.5\r\n2.5\r\n` → `[1.5, 2.5]`. This exercises exactly two source-qualified getter invocations in the original loop and the candidate's existing two-value loop; no malformed value, sticky-error continuation, Unicode numeric character, locale fallback, or partial trailing row is needed. The corresponding three-value STRETCH case is a direct later extension, not needed for the first bounded test.

Before implementation, keep admission limited to the exact path/count (`NBC.DAT`, two doubles) and decoded ASCII payload. Retain guards for malformed/non-ASCII code units, BOM-less high bytes, spring counts, other paths, and count zero. Tests should include a valid BOM-bearing two-number row; a missing second value (first value plus EOF) to confirm initialized trailing zero; and one invalid-first-character sequence only if root agrees to implement the verified repeated-unget failure output. Do not claim arbitrary malformed rows from the table without a token-level differential review.

## Remaining boundaries

The PE evidence source-qualifies the caller count, default stream codec selection, per-extraction order, operator failure output/status mapping, atEnd check, and retry after failure. It is not a fresh original-process run. Code-point handling beyond ASCII and malformed UTF codecs remain unqualified. The C-locale converter implementation and the candidate `qt::toDouble` wrapper share a source description but are not dynamically compared for edge rounding, non-finite spelling, overflow, or underflow. Keep this recommendation to short, ordinary finite ASCII decimals with whitespace delimiters. No source, test, build, GUI, or REA change was made in this investigation.
