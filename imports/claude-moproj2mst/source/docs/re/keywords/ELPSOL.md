# ELPSOL (id 234, group 2, case 0x5a)

Per-object integer option (the name suggests elasto-plastic solution). The code only stores and prints it. It is stored at `obj+0x1f18` (int).

## KEY grammar (read)
    ELPSOL <objNo:%i> <value:%i>
The value is the rest of the keyword line, read by `FUN_140413de0(f, obj+0x1f18, 0)` (group2-helpers.md,
variants). That reader does one `fgets`, replaces commas with blanks, then `sscanf "%i"`, so hex
`0x…` and leading-zero octal are accepted. The blank-line skip flag is 0, so a keyword line that
ends right after the object number is **not** continued on the next line. Extra tokens are ignored,
and a non-numeric token gives 0 with success. The record is this one line. 2D and 3D are identical.

## Model storage
* `obj+0x1f18` (int32) = value.
* Default `2`. It is set by the reset of the `obj+0x1ea0` sub-structure, `FUN_140186200`, which the
  object reset `FUN_14015f0b0` calls when the object is created.
* No side effects.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 → 2 "Reading Error". This happens on EOF, or when the rest of the line is empty or blank.
  The field has already been set to 0.
* No range check.

## KEY writer
Inline in `FUN_14029e250` (group2-writer-map.md step 16). Registry handler2 is the no-op. It is always written for objects whose type is not 5 or 7, right after `TRGVOL` and before `OTPRNG`, provided every status-checked handler before it returned 0.

    fprintf(f, "%-6s %7i %7i\n", "ELPSOL", N, *(int*)(obj+0x1f18));

The writer has no status of its own. The value round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x5a: reader dispatch.
* `FUN_140413de0`: rest-of-line `%i` reader.
* `FUN_14029e250`: writer (inline).
* `FUN_140186200`: `obj+0x1ea0` sub-structure reset (default).

## Corpus check
`2D/EN/MultipleOperations/New_MO_Release_Examples_EN/MO2_Extrusion_and_Boolean_Cycles/Objects/Object00007.KEY`: the line `ELPSOL       4       2` parses exactly. 1082 lines: 660 × `2` (the default), 422 × `0`.

## Open questions
* What the value means is not visible in the pre-processor; reading the solver code would settle it.
