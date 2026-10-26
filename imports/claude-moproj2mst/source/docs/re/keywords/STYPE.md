# STYPE

Simulation type code.

* registry id 26 (`0x1a`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x4 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → `M+0x848`. No validation.

## Model fields

`M+0x848` (`S+0xf8`) int.

## Default

1.

## KEY writer

Written after SMODE: `"%-6s %7i\n"`.

## Helpers

`FUN_140413de0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
