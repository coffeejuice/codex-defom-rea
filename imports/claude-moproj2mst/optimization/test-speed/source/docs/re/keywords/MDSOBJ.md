# MDSOBJ

Object-related step control: three ints and a double.

* registry id 55 (`0x37`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d8130 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x21 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i %i %Flf` (`FUN_1404164e0`) → `M+0x9d8`, `M+0x9dc`, `M+0x9e0`, `M+0x9e8`.

## Model fields

`M+0x9d8/0x9dc/0x9e0` int, `M+0x9e8` double (`S+0x288..0x298`).

## Default

0, 0, 0, 0.

## KEY writer

`FUN_1403d8130`: `"%-6s %7i %7i %7i %20.10E\n"`.

## Notes

The corpus has several MDSOBJ lines per file (3 651 occurrences); each overwrites the previous one, so only the last survives.

## Helpers

`FUN_1404164e0`, `FUN_1403d8130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 3651 occurrences in 494 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
