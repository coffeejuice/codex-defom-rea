# CP116 SPRING two-row writer contract

## Source binding

The retained target is `evidence/gui_mo/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Relevant evidence is `analysis/wave2-save-diffs-300-support-1410db030.c` (writer `FUN_1410db030`), `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` (writer `FUN_1410da9e0`), and `analysis/wave2-save-diffs-300-extra3d-write-values.json` (the four separator literals).

The literal at `0x142c143dc` after the count, `0x142c143e0` after each first object, `0x142c143e4` after each direction[0], and `0x142c143e8` after each direction[2] are all ASCII LF (`0x0a`). The writer streams each QString list element; it appends a tab after every element except an element equal to that LF literal, and appends the final ASCII CRLF (`0x0d 0x0a`). The record fields use base-10 integer formatting and `QString::number(double, 'g', 6)`. The example values below are ASCII, so the QTextStream encoding choice does not alter their bytes.

## Two-row flag-zero example

Use distinct rows:

- `(10, 0.5, 1.5, 20, [0, 1, 0], flag 0)`
- `(30, -2.25, 4, 40, [1, 0, -1], flag 0)`

The `SPRING.INI` item sequence is:

`2, LF, 10, LF, 0.5, 1.5, 20, 0, LF, 1, 0, LF, 30, LF, -2.25, 4, 40, 1, LF, 0, -1, LF`

Applying the writer's tab rule and final CRLF gives these exact bytes (escapes shown, no BOM):

```text
2\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n30\t\n-2.25\t4\t40\t1\t\n0\t-1\t\n\r\n
```

Equivalently, the exact hex bytes are:

`32 09 0a 31 30 09 0a 30 2e 35 09 31 2e 35 09 32 30 09 30 09 0a 31 09 30 09 0a 33 30 09 0a 2d 32 2e 32 35 09 34 09 34 30 09 31 09 0a 30 09 2d 31 09 0a 0d 0a`

## Flag and count branches

Before constructing the output, the writer scans each record's flag at record offset `+0x38`; any nonzero flag sets a single Boolean. Every row's flag is appended to a separate `SPRING2` QStringList, not to the primary list. It then attempts `SPRING.INI` first.

For all-zero flags, including the two rows above, the writer calls the existing readable-open/delete helper for `SPRING2.INI` after the primary write attempt, regardless of that attempt's Boolean return. It never writes a `SPRING2.INI` list on this path. The helper is the `fopen("rt")`-then-close/remove behavior qualified by the retained sidecar notes: an absent file remains absent; a readable existing companion is removed.

Count zero returns false before building either list or attempting either sidecar. No additional explicit count-overflow guard appears in this function. The row loop uses signed `int`, and computes each record address after widening the index to 64 bits before multiplying by `0x40`; it does not perform a narrow 32-bit byte-offset multiplication. Negative count is not rejected locally, but normal caller list counts are nonnegative; it would serialize the decimal negative count with no rows, then follow the all-zero-flag cleanup branch. Do not generalize this bounded contract to arbitrary invalid native pointers or unbounded caller-provided counts.

No source, test, build, GUI, mutable REA database, or original runtime changes were made.
