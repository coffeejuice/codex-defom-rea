# CP116 SPRING multirow parser boundary

## Static result

The retained original decompile for `FUN_1410dab40` qualifies the count-greater-than-one read loop, but does not qualify arbitrary partial rows or allocation failures. The next safe internal parser profile can be count two with two complete supported ASCII records and absent `SPRING2.INI`; public Open→Save still needs the page/list state described below.

Primary retained original image: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Relevant retained decompiles are `analysis/wave2-save-diffs-300-support-1410dab40.c`, `...-1410da800.c`, `...-1410da8f0.c`, `...-1410da690.c`, and `...-1410db030.c`.

## Count and allocation order

`FUN_1410dab40` opens `SPRING.INI` read-only, creates one `QTextStream`, and reads the count through `FUN_1410da800(..., 1)`. That helper zeroes the destination first and returns the value of `atEnd()` captured before any extraction. For a nonempty stream, one integer extraction is attempted and the wrapper returns true even if the getter failed and wrote zero. The reader writes the count output only when that wrapper returns true, then continues only if count is positive.

For positive count `n`, the reader allocates an auxiliary `n * 4`-byte integer array using `FUN_14147489c(n << 2)` and the main `0x40`-byte-stride record array using `FUN_1410db4f0(..., n)`. It tests both returned pointers for null before reading `SPRING2.INI`. Failure of either allocation returns false. The allocator bodies and their overflow/cleanup behavior are not retained here, so do not widen count or allocation-failure behavior.

## Optional `SPRING2.INI`

`FUN_1410da690` first zeroes all `n` auxiliary integers. If the file is missing or cannot be opened, it returns false; the caller then explicitly zeroes all `n` entries again and resumes the main record loop. Thus a missing companion supplies flag zero to every row.

If `SPRING2.INI` opens, the helper constructs a separate stream and reads up to `n` integers while `atEnd()` is false. It ignores extraction status and returns true after the successful open, even if fewer than `n` values are read. The destination was pre-zeroed, so untouched entries remain zero. Exact malformed-token cursor/status effects and cases where a bad token repeats were not independently enumerated here. Keep the supported multirow slice to the absent-companion case; any present companion remains guarded.

## Main loop for each row

For each `i` from zero to `n-1`, the same open main stream is queried in this order:

1. one integer into record offset `0`;
2. two doubles into offsets `8` and `0x10`;
3. one integer into offset `0x18`;
4. three doubles into offsets `0x20`, `0x28`, and `0x30`;
5. the auxiliary integer into offset `0x38`.

The `FUN_1410da800`/`FUN_1410da8f0` wrappers zero each local destination group before reading, call `atEnd()` before extraction, and return only the initial non-end result. If a group starts with data but reaches EOF or fails partway through its extraction, the wrapper can still return true with the remaining destination fields zero. The caller stores fields group by group, so earlier fields stay written. If a later wrapper starts at end, the reader returns false after retaining already-written prefixes. The body does not inspect QTextStream status or roll back the partially filled array. The main loop stops after `n` rows and does not require EOF after the last row.

Because the retained decompile does not establish how `FUN_1410db4f0` initializes unfilled record storage, do not admit a file that ends before all fields of the final row. Also keep partial/malformed rows and extra trailing values outside the next profile.

## Concrete next internal profile

Once the root-owned page/list route can carry multiple `SpringRecord`s, the next bounded reader increment can admit exactly two complete valid ASCII rows with `SPRING2.INI` absent. The native file reader should require count two and a complete, explicitly enumerated two-row byte fixture (including the exact writer-produced whitespace spelling if that spelling is used). It can then return both records with flag zero. Retain guards for count zero/negative behavior beyond the existing cases, counts above two, present companion, allocation failure, partial/malformed rows, trailing extras, BOM/other codec, and non-ASCII payloads.

## Public item/page state that remains required

`FUN_140ec8110` inserts one item per returned record, derives the checkbox from list count, sets the stored selected row to `-1`, then selects row zero. The CP114 local PE disassembly closes that each item owns its `0x40`-byte record at `+0x30` and that `FUN_140ecdee0` saves the current form into the old row before selecting/loading a different row. It matches the selected row by the form's object-ID combos; a missing combo item leaves the prior selection unchanged. Save calls the helper for the current item, then snapshots all rows from their `+0x30` payloads.

For count two, candidate page state must therefore include the ordered record list, spring-enabled/checkbox state, selected-row index, both object-ID combo selections, and the current editable row. On initial Open, row zero is selected. On an unchanged Save, row zero is written back from visible fields (whose doubles were displayed with general format precision 6 and parsed again), while row one remains its item-owned record. Save serializes both records in order. A reader-only count-two success is not public Open→Save support.

The root owns that shared page/list/writer integration and any UI-model contract. The current native reader/test lane should not change the shared page interface or enable Save before that integration exists. No source, test, build, GUI, runtime, or mutable REA change occurred in this evidence review.
