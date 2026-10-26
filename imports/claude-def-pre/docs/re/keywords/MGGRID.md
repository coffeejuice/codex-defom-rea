# MGGRID (id 192, group 2, case 0x30)

Mesh-generation grid resolution (name: "MG grid"): two integers stored at `obj+0x2dc0` and
`obj+0x2dc4`, read as doubles and truncated.

## KEY grammar (read)
    MGGRID <objNo:%i> <g1:%Flf> <g2:%Flf>

One line, rest of the keyword line via `FUN_140414a40` (commas = spaces; a missing `g2` is 0).
Same in 2D and 3D.

## Model storage
`obj+0x2dc0 = (int)g1`, `obj+0x2dc4 = (int)g2` (truncation toward zero, int32).

## Read errors and validation
* objNo EOF / < 1 → 2.
* Helper returns 1 (EOF or empty/blank rest of line) → 2; both fields are assigned 0 first.
* No range check.

## KEY writer
Inline in `FUN_1402a9180`, line 12 of the block ([MGNELM.md](MGNELM.md) for order/conditions):

    fprintf("%-6s %7i %7i %7i\n", "MGGRID", objNo, obj+0x2dc0, obj+0x2dc4)

2D and 3D identical. Round-trips for integer values.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x30 — reader; `FUN_140414a40` — two-double reader.
* `FUN_1402a9180` — writer.

## Corpus check
2D `key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY` and 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY`: `MGGRID       1      25      25` — parses.

## Open questions
None for the grammar.
