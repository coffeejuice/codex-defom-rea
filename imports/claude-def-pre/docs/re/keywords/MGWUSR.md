# MGWUSR (id 191, group 2, case 0x2f)

Mesh-density weighting for user-defined windows (name: "MG weight user"); see MGUSER for the
windows themselves. 2D: one double; 3D: two doubles and an integer.

## KEY grammar (read)
3D:

    MGWUSR <objNo:%i> <w:%Flf> <d2:%Flf> <n:%Flf>          (FUN_140414c20)

2D:

    MGWUSR <objNo:%i> <w:%Flf>                             (FUN_140414990; extra text ignored)

One line; commas = spaces; missing trailing values = 0.

## Model storage
* `w → obj+0x2d40` (double), both dims.
* 3D only: `d2 → obj+0x2d98` (double), `obj+0x2da0 = (int)n` (truncation, int32).
  In 2D these fields are not touched.

## Read errors and validation
* objNo EOF / < 1 → 2.
* Helper returns 1 (EOF or empty/blank rest of line) → 2 (3D: `obj+0x2da0` is still set to 0).
* No range check.

## KEY writer
Inline in `FUN_1402a9180`, line 14 of the block ([MGNELM.md](MGNELM.md)):

* 3D: `"%-6s %7i %20.10E %20.10E %7i\n"` name, objNo, `+0x2d40`, `+0x2d98`, `+0x2da0`.
* 2D: `"%-6s %7i %20.10E\n"` name, objNo, `+0x2d40`.

Round-trips.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x2f — reader; `FUN_140414c20` (3 doubles), `FUN_140414990` (1 double).
* `FUN_1402a9180` — writer.

## Corpus check
2D `key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY`: `MGWUSR       1    0.0000000000E+000`; 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY`:
`MGWUSR       1  0.0000000E+000  1.0000000E+000       1` — both parse.

## Open questions
Meaning of the 3D `d2` and integer `n` (corpus: 1.0 and 1).
