# ENVMPT

Environment coefficient ENVMPT.

* registry id 82 (`0x52`), group 1, flag 5; handler columns 0x1402f7590 / 0x1403d7620 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x3c (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xd80`.

## Model fields

`M+0xd80` (`S+0x630`) double.

## Default

8.8e-15 (UNIT ≠ 2) or 2.25e-13 (UNIT = 2); reset by UNIT.

## KEY writer

`FUN_1403d7620` `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d7620`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
