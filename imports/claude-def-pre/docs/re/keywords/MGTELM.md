# MGTELM (id 185, group 2, case 0x29)

Mesh-generation element type/topology code (name: "MG type of element"); an integer stored at
`obj+0x2d84`, read as a double and truncated.

## KEY grammar (read)
    MGTELM <objNo:%i> <value:%Flf>

One line, rest of the keyword line via `FUN_140414990` (commas = spaces). The value is parsed as a
double, so `4`, `4.0` and `4.9` are all accepted. Same in 2D and 3D.

## Model storage
`obj+0x2d84 = (int)value` (C truncation toward zero; int32). No side effects.

## Read errors and validation
* objNo EOF / < 1 → 2.
* Helper returns 1 (EOF or empty/blank rest of line) → 2; the field is still assigned
  `(int)0.0 = 0` before the error is returned.
* No range check (values outside int range are undefined behaviour of the conversion).

## KEY writer
Inline in `FUN_1402a9180`, line 10 of the mesh-settings block (order and conditions:
[MGNELM.md](MGNELM.md)):

    fprintf("%-6s %7i %7i\n", "MGTELM", objNo, obj+0x2d84)

2D and 3D identical; always written with the block. Round-trips for integer values.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x29 — reader; `FUN_140414990` — one-double reader.
* `FUN_1402a9180` — writer.

## Corpus check
2D `key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\MeshSettings00001.KEY`: `MGTELM       1       4`; 3D `key\3D\EN\Diestress\3D_Shrinkfit.KEY`: `MGTELM       1       0` — both parse.

## Open questions
Meaning of the codes (0, 4 seen in the corpus): check the mesher consumers of `obj+0x2d84`.
