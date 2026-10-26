# ECRHFN (id 251, group 2, case 0x6b)

Per boundary edge (2D) / boundary face (3D) int array, the **function number** member of the triple
ECCRHT / ECRHFN / ECRFLX (resistance heating), stored in the object's 12-array edge/face block. Same mechanism as **ECCDEF**;
see `ECCDEF.md` for the full grammar, slot lookup, storage, writer logic and round-trip notes.
Only the differences are listed here.

## KEY grammar (read)

    ECRHFN <objNo:%i> <count:%i> [<default:%i>]       (rest of keyword line, sscanf "%i %i", no comma replacement)
    then <count> records (fscanf, free format):
      2D:  <idx:%i> <n1:%i> <n2:%i>                 <value:%i> x m
      3D:  <idx:%i> <n1:%i> <n2:%i> <n3:%i> <n4:%i> <value:%i> x m
    then fgets (rest of the last line)

`m` = `model+0x1724` (state-variable width `W[0x4b]`, normally 1). A header with 2
conversions fills all `NS` slots x m with the default; `count < 1` ends the keyword. `idx` is
ignored; the edge (2D, `FUN_140200c90`, either node order) or face (3D, `FUN_140200ff0`, node-set
containment, tri/quad mode from face 0) selects the slot.
Int reader: slot `< 0` or `>= NS` prints `ERROR in readEdgeFaceVarInt, iECCArrayAdr\n` to
stdout and drops the record (reading continues, status unaffected).

## Model storage

`obj+0xb38` -> `int*` array of `NS = FUN_140165170(obj)` entries, element
`[slot*m + c]`; allocated zero-filled by ELMCON (`FUN_1401fc290` -> `FUN_1401631e0` ->
`FUN_1401858d0`, entry 10 of the block at `obj+0xae8`). Nothing else is changed.

## Read errors and validation

As ECCDEF: status 1 (-> parser **2**, "Reading Error", KEY loop continues) only on EOF or a blank
header line; everything else returns 0. No range validation of values.

## KEY writer

Handler2 `FUN_1402b6fc0` -> `FUN_1402a4a80(model, f, obj, 0x4b, N, 0)` -> case 0x4b:
`FUN_1402f6600(f, "ECRHFN", N, obj, obj+0xb38 array, model+0x1724, default 0)`. Called from
`FUN_1402abf20` (writer-map step 54, condition `obj+0x87c != 0 && obj+0xbd8 > 0`), position
11 of 12. Lines:
* uniform: `"%-6s %7i %7i %7i\n"` (name, N, 0, a[0])
* else header `"%-6s %7i %7i %7i\n"` (name, N, cnt, 0) then per slot with `a[i] != 0`:
  2D `" %7i %7i %7i %7i\n"` (k, n1, n2, a[i]); 3D `" %7i %7i %7i %7i %7i %7i\n"` (k, n1..n4, a[i]).
`m >= 2` -> nothing written (status 1, discarded). Only component 0 is written.

## Unit conversion

None (integer).

## Helpers

`FUN_14038b8a0` case 0x6b, `FUN_14041a440`, `FUN_140165170`, `FUN_140200c90`, `FUN_140200ff0`,
`FUN_1401fc290`/`FUN_1401631e0`/`FUN_1401858d0` (allocation), `FUN_1402abf20`, `FUN_1402b6fc0`,
`FUN_1402a4a80` case 0x4b, `FUN_1402f6600`, `FUN_140200e20`/`FUN_140200ec0`/`FUN_140201210`; roles in
`ECCDEF.md`.

## Corpus check

All corpus EC blocks parse exactly with this grammar (scan of every `.KEY` under
`DEFORM_corpus\key`, see ECCDEF). Example: only the uniform form occurs (583 lines, all `ECRHFN <N> 0 0`), e.g. key\2D\EN\plane stress\Pla_PlaneStress_EN.KEY.

## Open questions

See ECCDEF (2D closed-loop assumption, `model+0x1b5c`, width `W[0x4b]` in the KEY path).
Physical meaning of the function number is not established by the code.
