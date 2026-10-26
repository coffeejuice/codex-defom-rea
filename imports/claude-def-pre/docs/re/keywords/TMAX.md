# TMAX

Stop criterion TMAX (time).

* registry id 42 (`0x2a`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d77a0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x14 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0x8c8`.

## Model fields

`M+0x8c8` (`S+0x178`) double.

## Default

0.

## KEY writer

`FUN_1403d77a0`: `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d77a0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 504 occurrences in 494 corpus files consumed exactly up to the next keyword/comment (14 misses, all in GUI project files `.d2mo` where the next token is `KWD_FOR_MST_END`, not a KEY keyword); e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
