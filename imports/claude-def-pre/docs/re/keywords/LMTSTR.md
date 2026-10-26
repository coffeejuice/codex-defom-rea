# LMTSTR (id 147, group 2, case 0x03)

One double per object at `obj+0x1ea0` (first field of the property sub-structure at
`obj+0x1ea0`; default 0.01). Name and default suggest the "limiting strain rate"; the code shows
only a plain double.

## KEY grammar (read)

    LMTSTR <objNo:%i> <value:%Flf>

`FUN_140414990` (rest of line, commas = spaces, missing → 0.0). 2D and 3D identical.

## Model storage

`obj+0x1ea0` double (default `DAT_14051f258` = 0.01). No side effects.

## Read errors and validation

Blank/EOF rest of line → **2**. Non-numeric → 0.0, returns 0.

## KEY writer

`FUN_1402af4f0` (handler2), called by `FUN_14029e250` right after AVGSTR (objects of internal
type ≠ 5, 7), always:

    fprintf("%-6s %7i %20.10E\n", "LMTSTR", objNo, obj+0x1ea0)

(`objNo` renumbered by `+0x3f24` when > 0.) Returns 0.

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 3, `FUN_140414990`, `FUN_1402af4f0`, `FUN_140186200` (defaults).

## Corpus check

454 files / 1096 records, all 3 tokens. Example (CREEP_Example1_EN.KEY:135):
`LMTSTR       1    5.0000000000E-004`.

## Open questions

None.
