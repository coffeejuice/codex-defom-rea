# ACVCOF

Coefficient with three table forms (var 3, 12 or 1).

* registry id 80 (`0x50`), group 1, flag 7; handler columns 0x1402f7590 / 0x1403d7350 / 0x1403d7530.
* Read: `FUN_14039aa90` case 0x3a (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %Flf` → t, v (failure → 2). Table `T = M+0xd00` reset to template 0x1d. t = 0 → `ndim` 0, const (`M+0xd08`) = v; t = 1 → `ndim` 1, var 3, cnt = (int)v; t = 2 → `ndim` 1, var 12, cnt = (int)v; t = 3 → `ndim` 1, var 1, cnt = (int)v. Then the table reader always runs (for t = 0 it reads nothing); its status is the handler status. Other t: reset only (const 0), status of the reader (0).

## Model fields

Table `M+0xd00` (`S+0x5b0`): const `+8`, var `+0x10`, cnt `+0x1c`.

## Default

2.5e-4 (UNIT ≠ 2) or 1e-5 (UNIT = 2).

## KEY writer

`FUN_1403d7350`, after ENVATM: ndim 0 → `"%-6s %7i %20.10E\n"` (0, const); ndim 1 and var 3 → `"%-6s %7i %7i\n"` (1, count); ndim 1 and var 12 → `"%-6s %7i %7i\n"` (2, count); ndim 1 and var 1 → `"%-6s %7i %7i\n"` (3, count); then `FUN_1402f3130` rows `" x value\n"`. Any other state → return 3 (KEY write aborted).

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403d7350`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
