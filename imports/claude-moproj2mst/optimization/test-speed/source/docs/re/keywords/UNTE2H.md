# UNTE2H

Unit conversion factor (energy to heat).

* registry id 73 (`0x49`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d8420 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x33 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0xb00`.

## Model fields

`M+0xb00` (`S+0x3b0`) double.

## Default

1.0 (UNIT ≠ 2) or 0.107 (UNIT = 2); reset by UNIT.

## KEY writer

`FUN_1403d8420` `"%-6s %20.10E\n"`.

## Notes

No conversion is applied when reading or writing; the factor is just stored.

## Helpers

`FUN_140414990`, `FUN_1403d8420`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
