# MGERR (id 193, group 2, case 0x31)

Mesh-generation error/tolerance pair (name: "MG error"): two doubles at `obj+0x2db0` and
`obj+0x2db8`.

## KEY grammar (read)
    MGERR <objNo:%i> <e1:%Flf> <e2:%Flf>

One line, rest of the keyword line via `FUN_140414a40` (commas = spaces; missing `e2` = 0).
Same in 2D and 3D.

## Model storage
`e1 → obj+0x2db0`, `e2 → obj+0x2db8` (double, written directly by the helper).

## Read errors and validation
* objNo EOF / < 1 → 2.
* Helper returns 1 (EOF or empty/blank rest of line) → 2; both fields are 0 then.
* No range check.

## KEY writer
Registry write handler `FUN_1402b48f0` (object lookup and `+0x3f24` renumbering as the other
handlers), reached only via `FUN_1402a9180`, line 13 of the block ([MGNELM.md](MGNELM.md)):

    fprintf("%-6s %7i %20.10E %20.10E\n", "MGERR", objNo, obj+0x2db0, obj+0x2db8)

Returns 0. 2D and 3D identical. Round-trips.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x31 — reader; `FUN_140414a40` — two-double reader.
* `FUN_1402b48f0` — write handler (registry handler2 of id 193); `FUN_1402a9180` — caller.

## Corpus check
2D `key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY`: `MGERR        1    1.0000000000E-002    3.2000000000E+001`; 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY`:
`MGERR        1  0.0000000E+000  0.0000000E+000` — both parse.

## Open questions
None for the grammar.
