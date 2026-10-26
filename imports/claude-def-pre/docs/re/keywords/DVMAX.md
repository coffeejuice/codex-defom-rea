# DVMAX

Two velocity-change limits.

* registry id 54 (`0x36`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d80a0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x20 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf %Flf` → `M+0x9c8`, `M+0x9d0`.

## Model fields

`M+0x9c8`, `M+0x9d0` (`S+0x278/0x280`) double.

## Default

0, 0.

## KEY writer

`FUN_1403d80a0`: `"%-6s %20.10E %20.10E\n"`.

## Helpers

`FUN_140414a40`, `FUN_1403d80a0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
