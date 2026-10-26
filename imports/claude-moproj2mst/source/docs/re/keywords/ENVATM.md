# ENVATM

Environment atmosphere value: constant or table against time (var 1).

* registry id 79 (`0x4f`), group 1, flag 7; handler columns 0x1402f7590 / 0x1403d70c0 / 0x1403d71e0.
* Read: `FUN_14039aa90` case 0x39 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %Flf` → t, v (failure → 2). Table `T = M+0xc88` reset to template 0x1e. t = 0 → `ndim` 0, const (`M+0xc90`) = v; t = 1 → `ndim` 1, var 1, cnt = (int)v. Then the table reader always runs (for t = 0 it reads nothing); its status is the handler status. Other t: reset only (const 0), status of the reader (0).

## Model fields

Table `M+0xc88` (`S+0x538`): const `+8`, var `+0x10`, cnt `+0x1c`.

## Default

1.69 (both unit systems).

## KEY writer

`FUN_1403d70c0`, after EHRATE: ndim 0 → `"%-6s %7i %20.10E\n"` (0, const); ndim 1 and var 1 → `"%-6s %7i %7i\n"` (1, count); then `FUN_1402f3130` rows `" x value\n"`. Any other state → return 3 (KEY write aborted).

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403d70c0`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
