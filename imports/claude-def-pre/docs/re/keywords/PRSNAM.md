# PRSNAM (id 257, group 2, case 0x71)

Per-object name of the press/equipment, a string at the start of the movement sub-structure
`obj+0x2160`. The corpus values look like `Equipment (3)`.

## KEY grammar (read)
    PRSNAM <objNo:%i> [ignored rest of line]
    <name line>
Read by the string reader `FUN_140413ab0(f, obj+0x2160, 0x50)` (group2-helpers.md, "String reader"):
1. The rest of the keyword line is discarded.
2. The next line is the name. It is cut at `'\n'` and at most 79 characters are kept; trailing blanks
   and CR are removed, while leading blanks and tabs are kept.

An empty line gives an empty name. 2D and 3D are identical.

## Model storage
* `obj+0x2160` char[0x50] = the name (NUL-terminated within 80 bytes).
* Default (`FUN_14018b500`): empty string (byte 0 = 0).

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* EOF on either `fgets` → 2.
* There is no length error: longer names are truncated to 79 characters.

## KEY writer
This is the first line of the movement-block writer `FUN_1402a7c80`. Registry handler2 is the no-op.
`FUN_1402a7c80` runs for type-5 objects (group2-writer-map.md step 5b), and for other types (not 7)
when `!(model+0x1744 & 0x40000)` (step 18). It is always written:

    fprintf(f, "%-6s %7i\n%s\n", "PRSNAM", N, (char*)(obj+0x2160));

`N` is the caller's number, or `obj+8` when `N < 1`. An empty name gives an empty second line.

It round-trips, except that leading blanks are kept while trailing blanks are lost.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x71: reader.
* `FUN_140413ab0`: string reader.
* `FUN_1402a7c80`: writer (movement block: PRSNAM, MOVCTL, SPDLMT, PRSTRE, STROKE, FORCE, ANGMOV,
  [2D ANGMVY], CNTRAX, [3D ANGMO2, CNTRA2], OBJUPD, OBJSTP).
* `FUN_14018b500`: reset.

## Corpus check
`2D/SI/MultipleOperations/New_MO_Release_Examples_SI/MO2_Operation_Cycles2/Equipments/Equipment00006.KEY`
parses exactly:

    PRSNAM       3
    Equipment (3)

Of the corpus second lines, 825 are empty and 410 hold a single blank (older writers); both read
as an empty name. The longest name is 14 characters.

## Open questions
None.
