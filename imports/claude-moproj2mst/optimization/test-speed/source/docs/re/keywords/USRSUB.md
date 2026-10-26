# USRSUB (id 253, group 2, case 0x6d)

Per-object integer that selects a user subroutine (by its name). The code only stores and prints it. It is stored at `obj+0x2150` (int).

## KEY grammar (read)
    USRSUB <objNo:%i> <value:%i>
The value is the rest of the keyword line, read by `FUN_140413de0(f, obj+0x2150, 0)` (group2-helpers.md,
variants). That reader does one `fgets`, replaces commas with blanks, then `sscanf "%i"`, so hex
`0x…` and leading-zero octal are accepted. The blank-line skip flag is 0, so a keyword line that
ends right after the object number is **not** continued on the next line. Extra tokens are ignored,
and a non-numeric token gives 0 with success. The record is this one line. 2D and 3D are identical.

## Model storage
* `obj+0x2150` (int32) = value.
* Default `0`. It is set by the reset of the `obj+0x1ea0` sub-structure, `FUN_140186200`, which the
  object reset `FUN_14015f0b0` calls when the object is created.
* No side effects.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 → 2 "Reading Error". This happens on EOF, or when the rest of the line is empty or blank.
  The field has already been set to 0.
* No range check.

## KEY writer
Inline in `FUN_14029e250` (group2-writer-map.md step 10). It is written **only when `obj+0x2150 != 0`**, for objects whose type is not 5 or 7. It comes after the `PENVOL` (3D) or `ZSTR`/`PENVOL` (2D) lines and before `REFTMP`, provided the AVGSTR/LMTSTR (and, in 2D, ZSTR/PENVOL) handlers returned 0. Negative values are written as they are.

    fprintf(f, "%-6s %7i %7i\n", "USRSUB", N, *(int*)(obj+0x2150));

The writer has no status of its own. The value round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x6d: reader dispatch.
* `FUN_140413de0`: rest-of-line `%i` reader.
* `FUN_14029e250`: writer (inline).
* `FUN_140186200`: `obj+0x1ea0` sub-structure reset (default).

## Corpus check
`2D/EN/viewfactor/Viewfactor2_EN.KEY`: the line `USRSUB       1       3` parses exactly. 86 lines across 26 files: values -1, 1, 2, 3 (8 / 24 / 46 / 8). A value of 0 never appears, which matches the writer condition.

## Open questions
* What the value means is not visible in the pre-processor; reading the solver code would settle it.
