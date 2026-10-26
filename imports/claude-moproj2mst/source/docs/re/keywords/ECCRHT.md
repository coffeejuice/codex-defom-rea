# ECCRHT (id 250, group 2, case 0x6a)

Per boundary edge (2D) / boundary face (3D) int array, the **code** member of the triple
ECCRHT / ECRHFN / ECRFLX (resistance heating), stored in the object's 12-array edge/face block. Same mechanism as **ECCDEF**;
see `ECCDEF.md` for the full grammar, slot lookup, storage, writer logic and round-trip notes.
Only the differences are listed here.

## KEY grammar (read)

    ECCRHT <objNo:%i> <count:%i> [<default:%i>]       (rest of keyword line, sscanf "%i %i", no comma replacement)
    then <count> records (fscanf, free format):
      2D:  <idx:%i> <n1:%i> <n2:%i>                 <value:%i> x m
      3D:  <idx:%i> <n1:%i> <n2:%i> <n3:%i> <n4:%i> <value:%i> x m
    then fgets (rest of the last line)

`m` = `model+0x1720` (state-variable width `W[0x4a]`, normally 1). A header with 2
conversions fills all `NS` slots x m with the default; `count < 1` ends the keyword. `idx` is
ignored; the edge (2D, `FUN_140200c90`, either node order) or face (3D, `FUN_140200ff0`, node-set
containment, tri/quad mode from face 0) selects the slot.
Int reader: slot `< 0` or `>= NS` prints `ERROR in readEdgeFaceVarInt, iECCArrayAdr\n` to
stdout and drops the record (reading continues, status unaffected).

## Model storage

`obj+0xb30` -> `int*` array of `NS = FUN_140165170(obj)` entries, element
`[slot*m + c]`; allocated zero-filled by ELMCON (`FUN_1401fc290` -> `FUN_1401631e0` ->
`FUN_1401858d0`, entry 9 of the block at `obj+0xae8`). Nothing else is changed.

## Read errors and validation

As ECCDEF: status 1 (-> parser **2**, "Reading Error", KEY loop continues) only on EOF or a blank
header line; everything else returns 0. No range validation of values.

## KEY writer

Handler2 `FUN_1402b6f00` -> `FUN_1402a4a80(model, f, obj, 0x4a, N, 0)` -> case 0x4a:
`FUN_1402f6600(f, "ECCRHT", N, obj, obj+0xb30 array, model+0x1720, default 0)`. Called from
`FUN_1402abf20` (writer-map step 54, condition `obj+0x87c != 0 && obj+0xbd8 > 0`), position
10 of 12. Lines:
* uniform: `"%-6s %7i %7i %7i\n"` (name, N, 0, a[0])
* else header `"%-6s %7i %7i %7i\n"` (name, N, cnt, 0) then per slot with `a[i] != 0`:
  2D `" %7i %7i %7i %7i\n"` (k, n1, n2, a[i]); 3D `" %7i %7i %7i %7i %7i %7i\n"` (k, n1..n4, a[i]).
`m >= 2` -> nothing written (status 1, discarded). Only component 0 is written.

## Unit conversion

None (integer).

## Helpers

`FUN_14038b8a0` case 0x6a, `FUN_14041a440`, `FUN_140165170`, `FUN_140200c90`, `FUN_140200ff0`,
`FUN_1401fc290`/`FUN_1401631e0`/`FUN_1401858d0` (allocation), `FUN_1402abf20`, `FUN_1402b6f00`,
`FUN_1402a4a80` case 0x4a, `FUN_1402f6600`, `FUN_140200e20`/`FUN_140200ec0`/`FUN_140201210`; roles in
`ECCDEF.md`.

## Corpus check

All corpus EC blocks parse exactly with this grammar (scan of every `.KEY` under
`DEFORM_corpus\key`, see ECCDEF). Example: key\2D\EN\ResistanceHeating\Resistance_heat2_EN.KEY line 1472: `ECCRHT       1      20       0` / `       1      20      21       3`; 3D: key\3D\EN\Resistance_Heating\3D_Brick_Resistance_heating_EN_Ex2.KEY line 25043 (`       1       1       2     653     652       3`).

## Open questions

See ECCDEF (2D closed-loop assumption, `model+0x1b5c`, width `W[0x4a]` in the KEY path).
Physical meaning of the code is not established by the code.
