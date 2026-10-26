# TMPBWD

Thermal bandwidth option.

* registry id 68 (`0x44`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x2e (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` (`FUN_140413de0`) → `M+0xa70`. Helper failure → 2.

## Model fields

`M+0xa70` (`S+0x320`) int.

## Default

1.

## KEY writer

Section *Iteration Controls*. `"%-6s %7i\n"`, value `M+0xa70`.

## Helpers

`FUN_140413de0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
