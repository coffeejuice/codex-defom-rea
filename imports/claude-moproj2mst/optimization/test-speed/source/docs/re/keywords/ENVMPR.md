# ENVMPR

Environment coefficient ENVMPR.

* registry id 81 (`0x51`), group 1, flag 5; handler columns 0x1402f7590 / 0x1403d75a0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x3b (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xd78`.

## Model fields

`M+0xd78` (`S+0x628`) double.

## Default

1.26e-9 (UNIT ≠ 2) or 3.19e-8 (UNIT = 2); reset by UNIT.

## KEY writer

`FUN_1403d75a0` `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d75a0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
