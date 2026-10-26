# PENVOL (id 149, group 2, case 0x05)

Object penalty constant (double at `obj+0x1eb0`, default 1.0E+6 — the typical volume-constancy
penalty value), plus in 2D an integer at `obj+0x1f20` (default 1 in 2D).

## KEY grammar (read)

2D (`FUN_1400015e0(model)` true):

    PENVOL <objNo:%i> <penalty:%Flf> <k:%Flf>

`FUN_140414a40` (`"%Flf %Flf"`); the second value is read as a double and truncated to int.

3D (any non-2D model):

    PENVOL <objNo:%i> <penalty:%Flf>

`FUN_140414990`. Further tokens on the line are ignored (the whole line is consumed).

Commas count as spaces; missing values → 0.

## Model storage

* `obj+0x1eb0` double = penalty (default `DAT_140522f60` = 1.0E+6).
* 2D only: `obj+0x1f20` int = `(int)k` (C truncation toward zero; default 1). A missing `k`
  stores 0.

## Read errors and validation

Blank/EOF rest of line → **2**. In 2D the int is stored even when the helper failed (it is then
0). No other checks.

## KEY writer

In `FUN_14029e250`, after LMTSTR (objects of internal type ≠ 5, 7), always written:

* 3D: inline in `FUN_14029e250`:
  `fprintf("%-6s %7i %20.10E\n", "PENVOL", objNo, obj+0x1eb0)`.
* 2D: after ZSTR, through `FUN_1402b4400` (handler2):
  `fprintf("%-6s %7i %20.10E %7i\n", "PENVOL", objNo, obj+0x1eb0, obj+0x1f20)`.
  A nonzero return would end the object (never happens).

`objNo`: the inline 3D line uses `FUN_14029e250`'s object number (`param_4`, or `obj+8` when
0); the handler uses `obj+0x3f24` when > 0 — the same value, since `FUN_14029e250` stores its
number in `+0x3f24` first. Round-trips (the int is written with `%7i`, read back with `%Flf`).

## Unit conversion

None.

## Helpers

`FUN_14038b8a0` case 5, `FUN_140414990`, `FUN_140414a40`, `FUN_1402b4400` (2D writer),
`FUN_14029e250` (3D writer), `FUN_140186200` (defaults).

## Corpus check

454 files / 1096 records: 2D files 4 tokens, 3D files 3 tokens, as above. Examples:
`PENVOL       1    1.0000000000E+006       1` (CREEP_Example1_EN.KEY:137, 2D),
`PENVOL       1    1.0000000000E+006` (3D Cogging_Multiple_Ops_Lab1/_TEMP_STEP.KEY:191).

## Open questions

Meaning of the 2D integer `+0x1f20`.
