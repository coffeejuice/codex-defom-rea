# TMPLMT (id 202, group 2, case 0x3a)

One double per object at `obj+0x1f10` (property sub-structure `obj+0x1ea0`, field `+0x70`;
default 0). Name suggests a temperature limit; the code shows only a plain double.

## KEY grammar (read)

    TMPLMT <objNo:%i> <value:%Flf>

`FUN_140414990` (rest of line, commas = spaces, missing → 0.0). 2D and 3D identical.

## Model storage

`obj+0x1f10` double, default 0.0 (`FUN_140186200`). No side effects.

## Read errors and validation

Blank/EOF rest of line → **2**. Non-numeric → 0.0, returns 0.

## KEY writer

`FUN_1402af6a0` (handler2), called by `FUN_14029e250` right after REFTMP (objects of internal
type ≠ 5, 7), always:

    fprintf("%-6s %7i %20.10E\n", "TMPLMT", objNo, obj+0x1f10)

Returns 0. (Next in `FUN_14029e250`: 3D only ROTSYM, id 258.)

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 0x3a, `FUN_140414990`, `FUN_1402af6a0`, `FUN_140186200`.

## Corpus check

431 files / 1056 records, all 3 tokens. Example (CREEP_Example1_EN.KEY:139):
`TMPLMT       1    0.0000000000E+000`.

## Open questions

None.
