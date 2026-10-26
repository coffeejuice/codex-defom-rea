# PENINF

Penalty factor.

* registry id 72 (`0x48`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d83a0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x32 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xaf8`.

## Model fields

`M+0xaf8` (`S+0x3a8`) double.

## Default

1e9.

## KEY writer

`FUN_1403d83a0` `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d83a0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
