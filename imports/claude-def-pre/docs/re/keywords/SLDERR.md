# SLDERR

Sliding/velocity error limit.

* registry id 52 (`0x34`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7ef0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1e (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0x9b8`.

## Model fields

`M+0x9b8` (`S+0x268`) double.

## Default

0.

## KEY writer

`FUN_1403d7ef0`: `"%-6s %20.10E\n"` after the DTTYPE block.

## Helpers

`FUN_140414990`, `FUN_1403d7ef0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
