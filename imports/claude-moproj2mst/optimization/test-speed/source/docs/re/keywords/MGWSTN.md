# MGWSTN (id 189, group 2, case 0x2d)

Mesh-density weighting factor for strain (name: MG weight STN). One double per object, stored at `obj+0x2d30`; written by the registry
write handler `FUN_1402b4750` as line 7 of the mesh-settings block.

## KEY grammar (read)
    MGWSTN <objNo:%i> <value:%Flf>

One line; the value is the rest of the keyword line (`FUN_140414990`, commas count as spaces,
extra text ignored, a non-numeric token gives 0). Same in 2D and 3D.

## Model storage
`value → obj+0x2d30` (double). No side effects. Its initial value comes from the
object constructor (not analysed here).

## Read errors and validation
* objNo EOF / < 1 → 2 (common prologue).
* `FUN_140414990` returns 1 (EOF, or the rest of the line is empty/blank, e.g. the value on the
  next line) → 2; the field has already been set to 0 by the helper.
* No range check. Return-code handling: `_common.md` §1.

## KEY writer
Registry handler `FUN_1402b4750(f, model, ref)`: looks the object up (`FUN_14017aaf0(model+0x1380, objNo)`),
prints `obj+0x3f24` instead of objNo when > 0, then

    fprintf("%-6s %7i %20.10E\n", "MGWSTN", objNo, obj+0x2d30)

and returns 0. It is only reached through `FUN_1402a9180` (line 7; conditions and full order in
[MGNELM.md](MGNELM.md), "Position in the mesh-settings writer"). Always written, in 2D and 3D,
whenever the block is written. Round-trips (`%20.10E` keeps 11 significant digits).

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x2d — reader; `FUN_140414990` — one-`%Flf` rest-of-line reader.
* `FUN_1402b4750` — write handler (registry handler2 of id 189).
* `FUN_1402a9180` — mesh-settings block writer (caller of the handler).

## Corpus check
`key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY`
(2D, writer format): `MGWSTN       1    2.5000000000E-001` — parses. 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY` (older
`%15.7E` width): `MGWSTN       1  2.5000000E-001` — parses.

## Open questions
None for the grammar. Semantics of the value only from the name (see first line).
