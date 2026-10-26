# ZSTR (id 148, group 2, case 0x04)

2D-only object setting: an integer flag and a double at `obj+0x1f30` / `obj+0x1f28` (property
sub-structure `obj+0x1ea0`, fields `+0x90` / `+0x88`; both defaulted to 0 only in 2D). The name
suggests a z-direction (out-of-plane) strain setting for plane problems; the code shows only an
int and a double.

## KEY grammar (read)

2D (`FUN_1400015e0(model)` true):

    ZSTR <objNo:%i> <flag:%i> <value:%Flf>

`FUN_1404154c0` (`"%i %Flf"`, rest of line, commas = spaces, missing → 0).

3D (any non-2D model): **nothing is read after the object number** and the case returns 0. The
rest of the line stays in the stream, so the KEY loop reads its next token as a keyword: for
`ZSTR 1 0 0.0` it reads `0`, does not find it and prints
`WARNING: Keyword 0 not found. Skip <rest of line>` (stdout, `_common.md` §1), then continues
normally. Net effect: the record is ignored, with one warning.

## Model storage

2D: `obj+0x1f30` int = flag, `obj+0x1f28` double = value (stored directly by the helper).
3D: nothing.

## Read errors and validation

2D: blank/EOF rest of line → **2**. Otherwise 0, no checks. 3D: always 0 (object is still
created by the object-number lookup).

## KEY writer

`FUN_1402af5c0` (handler2), called by `FUN_14029e250` **only in 2D**, after LMTSTR and before
PENVOL (objects of internal type ≠ 5, 7):

    fprintf("%-6s %7i %7i %20.10E\n", "ZSTR", objNo, obj+0x1f30, obj+0x1f28)

Always written in 2D (no default suppression). Returns 0; a nonzero return would end the object
with that code. Never written in 3D.

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 4, `FUN_1404154c0`, `FUN_1402af5c0`, `FUN_140186200` (2D defaults),
`FUN_1400015e0` (2D test).

## Corpus check

247 files / 622 records, all in 2D files, all 4 tokens. Example (CREEP_Example1_EN.KEY:136):
`ZSTR         1       0    0.0000000000E+000`. No 3D file uses it.

## Open questions

The registry's handler3 column for ZSTR holds `0x100000000` (not a code address; probably a
table-dump artefact or a real bad pointer). Nothing in the KEY path uses handler3.
