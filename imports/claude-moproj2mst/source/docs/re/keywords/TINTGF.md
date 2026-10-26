# TINTGF

Time-integration factor.

* registry id 74 (`0x4a`), group 1, flag 5; handler columns 0x1402f7590 / 0x1403d7250 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x34 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xb08`.

## Model fields

`M+0xb08` (`S+0x3b8`) double.

## Default

0.75.

## KEY writer

`FUN_1403d7250` `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d7250`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
