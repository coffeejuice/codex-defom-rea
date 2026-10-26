# CP159 SPRING2 next source-ready parser slice

## Selected increment

Extend the CP158 `q`/`?` unread-marker cursor profile by one independent ASCII-letter witness, `z`, under the same primary row-count 1/2 gates. This is a narrow test of whether the admitted behavior follows the actual first unread ASCII non-digit rather than a marker spelling; it does not generalize to arbitrary punctuation, high bytes, malformed numeric families, or count >= 3. Keep q/? behavior unchanged.

Exact fixtures to admit privately:

| Rows | SPRING2.INI bytes | Flags | Cursor/decision |
|---:|---|---|---|
| 1 | `1z\r\n` | `[1]` | decimal extractor stops before z, ungets it; count limit ends |
| 2 | `1z\r\n` | `[1,0]` | first extraction returns 1 with z unread; retry at z fails to zero and leaves z unread |
| 2 | `z 2\r\n` | `[0,0]` | failed extraction ungets z; retry sees same z; trailing 2 is unreachable |

Keep `1z?`, count-three `z 2`, any high-byte marker and numeric/radix malformed tokens guarded. Preserve the existing file byte-immutability assertions.

## Original source and cursor evidence

Original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The relevant original caller is `FUN_1410da690` in `analysis/wave2-save-diffs-300-support-1410da690.c`: it zeroes the destination array, checks `!atEnd()` and row bound before each `QTextStream >> int`, increments the row index regardless of stream status, and returns success after a successful file open. `analysis/resume119-spring2-normal-path.md` qualifies that loop and its default QTextStream construction.

For the decision/cursor, `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` decimal path `0x670C5609–5790` accumulates digits, then ungets a non-digit other than the consumed C-locale grouping comma at `0x670C5780–578B`; `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows failed extraction writes zero (`0x670C6125–6133`). `analysis/wave2-save-diffs-qt-textstream-atend-complete.asm` shows unread buffered input keeps `atEnd()` false. ASCII `z` is not a decimal digit, sign, whitespace, or C-locale grouping comma. Therefore the table follows directly from the same original decisions as CP158. This is immutable PE plus caller static evidence, not a fresh original-process differential.

CP158 review `analysis/resume158-spring-cursor-repair-review.md` verified that the private reader already preserves the actual marker cursor for q/? and that z (`1z`, `1z?`) remains refused. Its accepted-scope test seam is the private SPRING2 reader exercised through the focused `test_simcontrol300_spring_reader.cpp`; implementation point is `readSpringFlagInt` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`, called from the SPRING2 flag loop around lines 627–633. Extend the marker predicate there and add three focused private-reader cases with byte-preservation checks; retain the public Save output contract through existing integration coverage (nonzero flags serialize using the established decimal writer). Do not change the public ABI or relax the public path/count gates.

## Limits and admission

This is an extension of the same cursor behavior with an independent marker witness, not a new Qt parser algorithm. If the checkpoint needs an independent behavior rather than an additional alphabet witness, there is no separately established source-ready branch identified in the retained receipts: comma grouping already has its own bounded CP122 implementation; BOM and numeric families are already covered by earlier receipts. A broader generic ASCII non-digit admission would need an explicit profile decision and tests beyond this small slice. No source/build/test/fixture/GUI/mutable-REA changes were made in this evidence pass.
