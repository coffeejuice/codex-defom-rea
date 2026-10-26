# EMAX

Stop criterion EMAX.

* registry id 41 (`0x29`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7720 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x13 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0x8c0`.

## Model fields

`M+0x8c0` (`S+0x170`) double.

## Default

0.

## KEY writer

Registry column-2 writer `FUN_1403d7720`: `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_1403d7720`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
