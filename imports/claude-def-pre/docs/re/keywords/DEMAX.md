# DEMAX

Maximum strain increment per step.

* registry id 47 (`0x2f`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7af0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x19 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0x9b0`.

## Model fields

`M+0x9b0` (`S+0x260`) double.

## Default

0.1 in 2D, 0 in 3D (`FUN_1401e8650`).

## KEY writer

`FUN_1403d7af0`: `"%-6s %20.10E\n"` (after STPDEF).

## Helpers

`FUN_140414990`, `FUN_1403d7af0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 323 occurrences in 323 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
