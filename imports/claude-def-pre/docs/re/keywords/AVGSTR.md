# AVGSTR (id 146, group 2, case 0x02)

One double per object at `obj+0x1ea8` (object property sub-structure at `obj+0x1ea0`, defaults
set by `FUN_140186200`; default 1.0). Name and default suggest the "average strain rate" used
for rigid-plastic objects; the code shows only that it is a plain double.

## KEY grammar (read)

    AVGSTR <objNo:%i> <value:%Flf>

`FUN_140414990` (`%Flf`, rest of line, commas = spaces, missing → 0.0). 2D and 3D identical.

## Model storage

`obj+0x1ea8` double (default `DAT_140509c90` = 1.0). No side effects.

## Read errors and validation

Blank/EOF rest of line → helper 1 → **2**. Non-numeric value → 0.0, returns 0.

## KEY writer

`FUN_1402af420` (handler2), called by `FUN_14029e250` right after OBJNAM for objects of internal
type ≠ 5, 7; always written:

    fprintf("%-6s %7i %20.10E\n", "AVGSTR", objNo, obj+0x1ea8)

`objNo` is replaced by `obj+0x3f24` when that is > 0. Returns 0; `FUN_14029e250` stops the object
on a nonzero return (never happens).

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 2 (reader), `FUN_140414990` (line reader), `FUN_1402af420` (writer),
`FUN_140186200` (defaults).

## Corpus check

454 files / 1096 records, all `AVGSTR n value` (3 tokens), 2D and 3D. Example
(CREEP_Example1_EN.KEY:134): `AVGSTR       1    5.0000000000E-003`.

## Open questions

None.
