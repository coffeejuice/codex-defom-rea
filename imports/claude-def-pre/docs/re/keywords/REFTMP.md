# REFTMP (id 150, group 2, case 0x06)

One double per object at `obj+0x1eb8`, the object's reference temperature (default 20 or 68
depending on `FUN_140235c10()`, i.e. °C vs °F — consistent with a unit-system switch).

## KEY grammar (read)

    REFTMP <objNo:%i> <value:%Flf>

`FUN_140414990` (rest of line, commas = spaces, missing → 0.0). 2D and 3D identical.

## Model storage

`obj+0x1eb8` double. Default (`FUN_140186200`): `FUN_140235c10() == 1` → 20.0
(`DAT_14051f1f0`), otherwise 68.0 (`DAT_14051f1f8`). No side effects.

## Read errors and validation

Blank/EOF rest of line → **2**. Non-numeric → 0.0, returns 0.

## KEY writer

`FUN_1402af350` (handler2), called by `FUN_14029e250` after the PENVOL/ZSTR group and the optional
USRSUB line (id 253, written when `obj+0x2150 != 0`); objects of internal type ≠ 5, 7; always:

    fprintf("%-6s %7i %20.10E\n", "REFTMP", objNo, obj+0x1eb8)

Returns 0.

## Unit conversion

None on read/write (the value is in the file's unit system).

## Helpers

`FUN_14038b8a0` case 6, `FUN_140414990`, `FUN_1402af350`, `FUN_140186200`, `FUN_140235c10`
(unit query, not decompiled).

## Corpus check

440 files / 1082 records, all 3 tokens. Example (CREEP_Example1_EN.KEY:138):
`REFTMP       1    1.2020000000E+003`.

## Open questions

`FUN_140235c10` return values (1 = SI assumed) not checked.
