# CP73: next SW2SP grouping/delimiter boundary

## Selected exact token: comma is not grouping in the default stream

For a one-value `SW2SP.DAT`, the next useful exact fixture is bytes `1,234\r\n`. The original C-locale `QTextStream` parses the decimal prefix `1`, leaves the comma unread, and completes the integer extraction successfully with value 1. This is a token-boundary case, not a thousands-grouped value.

The control flow is source-qualified:

1. CP67 established that `QTextStreamPrivate` constructs its locale as `QLocale::c()` and does not call `setLocale` in the original sidecar reader.
2. In this QtCore image, `QLocale::c()` and the default stream's QLocale are both built from language 1/country 0; that constructor sets the QLocale number-options word at object `+2` to zero (`analysis/resume67-qlocale-c-factory.asm`, `analysis/resume67-qlocale-language-country-ctor.asm`, `analysis/resume67-qtextstream-private-ctor.asm`).
3. `QLocale::numberOptions()` returns that word (`analysis/resume73-qlocale-numberoptions.asm`, `0x6704A340–0x6704A349`). When the decimal getter sees a non-digit, it constructs a C-locale QLocale and compares number options at `0x670C5712–575C`. Both are zero, so it jumps directly to the unget path at `0x670C5780–578B`; it does **not** reach the conditional `groupSeparator()` comparison at `0x670C575E–577A`.
4. The already collected digit count is nonzero, so the getter takes the successful conversion path (`0x670C5790` onward). `operator>>(int&)` stores the integer and does not set error status (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6125–6127`, `0x670C6166–616A`).

Although the C-locale data record's group separator is comma, the default stream's zero number-options field makes the token a delimiter at this branch. The comma remains unread. `FUN_1410da690` requests just one SW2SP integer, ignores stream status, completes after this successful extraction, and the Extra3D fill formats the resulting 1 as decimal for the control (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`, `analysis/wave2-save-diffs-300-support-140ec8110.c`). Save later writes the control text (`analysis/wave2-save-diffs-300-support-140ec9040.c`).

The current candidate's integer scanner rejects `1,234` because comma is not ASCII whitespace. This is a concrete narrow discrepancy beyond the exact overflow tokens. If implemented, start with a one-value SW2SP fixture asserting `readInts(...) == true` and value 1 for exact bytes `1,234\r\n`; keep the cursor immediately after `1` with comma unread.

## Guard scope

Do not interpret comma as an accepted group separator from the locale symbol alone. The branch accepts/consumes grouping only when the stream's number-options differ from the C default; the original reader does not override them. Keep non-C number-options/group-size cases, subsequent extraction after the comma, malformed tokens, and multi-value SW2SP requests outside this one-value contract. The call site used in Extra3D Open passes count one; SPRING's count-controlled allocation path remains separate.

Evidence is static in retained QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, plus the original Extra3D decompilations cited above. No original runtime comparison is claimed. No source, test, build, GUI, or mutable REA state was changed.
