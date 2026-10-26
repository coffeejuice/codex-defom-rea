# OBJUPD (id 267, group 2, case 0x7b)

Per-object integer "object update" option, in the 0x1ea0 sub-structure. The code only stores and prints it. It is stored at `obj+0x1f1c` (int).

## KEY grammar (read)
    OBJUPD <objNo:%i> <value:%i>
The value is the rest of the keyword line, read by `FUN_140413de0(f, obj+0x1f1c, 0)` (group2-helpers.md,
variants). That reader does one `fgets`, replaces commas with blanks, then `sscanf "%i"`, so hex
`0x…` and leading-zero octal are accepted. The blank-line skip flag is 0, so a keyword line that
ends right after the object number is **not** continued on the next line. Extra tokens are ignored,
and a non-numeric token gives 0 with success. The record is this one line. 2D and 3D are identical.

## Model storage
* `obj+0x1f1c` (int32) = value.
* Default `0`. It is set by the reset of the `obj+0x1ea0` sub-structure, `FUN_140186200`, which the
  object reset `FUN_14015f0b0` calls when the object is created.
* No side effects.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 → 2 "Reading Error". This happens on EOF, or when the rest of the line is empty or blank.
  The field has already been set to 0.
* No range check.

## KEY writer
In the movement-block writer `FUN_1402a7c80`, after `CNTRA2` (3D) or `CNTRAX`, and before `OBJSTP`. It is written only if all the handlers before it in that function returned 0 (MOVCTL, SPDLMT, PRSTRE, STROKE, FORCE, ANGMOV, [2D: ANGMVY], CNTRAX, [3D: ANGMO2, CNTRA2]). `FUN_1402a7c80` itself runs for type-5 objects (step 5b) and, for other types, when `!(model+0x1744 & 0x40000)` (step 18). There is no value condition. The object number printed is `N` (the caller's renumbered number); when `N < 1`, `obj+8` is used.

    fprintf(f, "%-6s %7i %7i\n", "OBJUPD", N, *(int*)(obj+0x1f1c));

The writer has no status of its own. The value round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x7b: reader dispatch.
* `FUN_140413de0`: rest-of-line `%i` reader.
* `FUN_1402a7c80`: writer (inline).
* `FUN_140186200`: `obj+0x1ea0` sub-structure reset (default).

## Corpus check
`2D/SI/MultipleOperations/New_MO_Release_Examples_SI/MO2_Operation_Cycles2/Equipments/Equipment00006.KEY`: the line `OBJUPD       3       0` parses exactly. 1313 lines across 568 files: 1312 × `0`, 1 × `2`.

## Open questions
* What the value means is not visible in the pre-processor; reading the solver code would settle it.
