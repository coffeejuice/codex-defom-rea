# CNTERR

Contact error control (2D only).

* registry id 56 (`0x38`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d81d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x22 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

2D only: line 0 `%i %Flf` (`FUN_1404154c0`) → `M+0x1118`, `M+0x1120`. 3D: nothing read (rest of line reaches the main loop).

## Model fields

`M+0x1118` int, `M+0x1120` double (`S+0x9c8/0x9d0`).

## Default

0, 0 (2D models).

## KEY writer

2D only, right before VFACTR: `FUN_1403d81d0` `"%-6s %7i %20.10E\n"`.

## Helpers

`FUN_1404154c0`, `FUN_1403d81d0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 305 occurrences in 305 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
