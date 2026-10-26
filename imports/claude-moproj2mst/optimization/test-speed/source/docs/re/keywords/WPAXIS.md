# WPAXIS (id 264, group 2, case 0x78)

Work-piece axes: a numbered list of axis definitions (0xe8-byte records in the linked list
`obj+0x20a0`). Each has a type 0..7 that selects which point, vector and parameter fields
follow.

## KEY grammar (read)

    WPAXIS <obj> <id:%i> <type:%i> <sub:%i>     FUN_140413fb0 (nconv not used)

* **2D model**: only `type` 5 or 6 is accepted. Any other type → the case returns 0 **without
  reading the data lines** (they would then be parsed as keywords). 3D accepts all types.
* `type < 0` or `type > 7` is stored as 0 and read as type 0.

Data lines (each line is one `fgets` + `sscanf`; commas allowed, missing values 0). Fields are
byte offsets in the record `e`:

| type | header `sub` → | line 1 | line 2 | line 3 | line 4 |
|---|---|---|---|---|---|
| 0, 1, 2 | `e+0x38` (clamped: not 0/1 → 0) | 3d → `+0x08,+0x10,+0x18` | 3d → `+0x20,+0x28,+0x30` | 4d → `+0x40,+0x48,+0x50,+0x58` | – |
| 3 | `e+0x60` | 3d → `+0x08..+0x18` | 3d → `+0x20..+0x30` | – | – |
| 4 | `e+0xb0` | 3d → `+0x68,+0x70,+0x78` | 3d → `+0x80,+0x88,+0x90` | 3d → `+0x98,+0xa0,+0xa8` | 1d → `+0xb8` |
| 5 | ignored | 3d → `+0x20..+0x30` (zeroed first) | 1d → `+0xb8` | – | – |
| 6 | ignored | (no data lines) | | | |
| 7 | ignored | 3d → `+0x20..+0x30` | 3d → `+0x08..+0x18` | 5d `FUN_140414f50` → `+0x40`, `(int)` → `+0xc8`, `(int)` → `+0xcc`, `+0xd0`, `+0xd8` | – |

Type 7 line 3 is read as 5 doubles; values 2 and 3 are truncated to int.
"3d" = `FUN_140414c20`, "4d" = `FUN_140414e40`, "1d" = `FUN_140414990`.

## Model storage

* `e = FUN_140259bc0(obj+0x20a0, id)`. While the list count is < id it appends records
  (`FUN_140259950(list, 0)`: 0xe8 bytes, init `FUN_140258580`, `+0 = new count`), then returns
  the id-th record **by position** (`FUN_140259b60`, 1-based, `+0xe0` = next).
  * Ids are therefore positions. `WPAXIS 1 3 …` creates records 1..3 and fills record 3.
  * id ≤ 1 returns the first record. id ≤ 0 on an empty list returns NULL, and the following
    write to `+4` would crash. A port should reject it.
* `e+4 = type` (after clamping). The other fields as in the table.

## Read errors and validation

* Header EOF/blank → **2**.
* Types 0–2 and 3: only the **last** data line's status is checked (EOF/blank → **2**); the
  statuses of earlier lines are overwritten.
* Type 4: only line 4 is checked. Type 5: only line 2. Type 7: only line 3.
* No messages.

## KEY writer

`FUN_1402a39e0(model, f, obj, N)`. 2D: group2-writer-map.md step 57; 3D: step 57′, after
SYMSUF. For p = 1..count, `e = FUN_140259b60(list, p)`; NULL is skipped; switch on `e+4`:

* 0, 1, 2: `"%-6s %7i %7i %7i %7i\n"` → WPAXIS, N, `e+0`, type, `e+0x38`; then
  `" %20.10E %20.10E %20.10E\n"` (+0x08..+0x18), the same for +0x20..+0x30, then
  `" %20.10E %20.10E %20.10E %20.10E\n"` (+0x40..+0x58).
* 3: header with `e+0x60`; the two 3-vector lines.
* 4: header with `e+0xb0`; 3-vector lines +0x68.., +0x80.., +0x98..; then `" %20.10E\n"` +0xb8.
* 5: `"%-6s %7i %7i %7i\n"` → WPAXIS, N, id, type; then 3-vector +0x20..; then `" %20.10E\n"` +0xb8.
* 6: `"%-6s %7i %7i %7i\n"` only.
* 7: `"%-6s %7i %7i %7i\n"`; then 3-vector +0x20..; then 3-vector +0x08..; then
  `" %20.10E %7i %7i %20.10E %20.10E\n"` → +0x40, `+0xc8`, `+0xcc`, +0xd0, +0xd8.
* Other types: nothing.

It returns 0 (3 if `f`/`obj` NULL). The id printed is `e+0`, which equals the position for
records created by the reader.

Round-trip notes: in 2D, records of types 0–4 and 7 are written but the 2D reader ignores
them, and their data lines then reach the keyword loop as unknown keywords.

## Unit conversion

None. Type 7's `+0xd0`/`+0xd8` look like angles in degrees (corpus: -90, -80); they are not
converted.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413fb0` | header `%i %i %i` |
| `FUN_140414990`, `FUN_140414c20`, `FUN_140414e40`, `FUN_140414f50` | 1/3/4/5 double lines |
| `FUN_140259bc0`, `FUN_140259950`, `FUN_140259b60`, `FUN_14019a550` | get-or-create by position, append, nth, count |
| `FUN_1402a39e0` | writer |

## Corpus check

* `3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY` line 303: `WPAXIS 1 1 1 0` + 3 lines and
  `WPAXIS 1 2 4 0` + 4 lines parse.
* Type 7: `3D\EN\Cogging\Cogging_Multiple_Ops_Lab2\_TEMP_STEP.KEY` line 7324:
  `    9.9571056374E+000       6       7   -9.0000000000E+001   -8.0000000000E+001` parses as
  5 doubles.
* Types used in the corpus: 0 (49), 1 (64), 4 (22), 7 (1). All occurrences are in 3D files.

## Open questions

* The semantics of the types and of the `sub` flag. Strings in the GUI module would settle
  them; this does not matter for I/O.
* The defaults set by `FUN_140258580` for fields that the type's lines do not set.
