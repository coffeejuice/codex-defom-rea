# BLZMAN

Stefan–Boltzmann constant.

* registry id 77 (`0x4d`), group 1, flag 5; handler columns 0x1402f7590 / 0x1403d72d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x37 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xc08`.

## Model fields

`M+0xc08` (`S+0x4b8`) double.

## Default

5.669e-11 (UNIT ≠ 2) or 3.306e-15 (UNIT = 2); reset by UNIT.

## KEY writer

`FUN_1403d72d0` `"%-6s %20.10E\n"` after TINTGF.

## Helpers

`FUN_140414990`, `FUN_1403d72d0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
