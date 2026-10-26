# GEOERR

Geometry tolerances (four doubles).

* registry id 71 (`0x47`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d82f0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x31 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` ×4 → `M+0xdb0`, `M+0xdb8`, `M+0xdc0`, `M+0xdc8`.

## Model fields

`M+0xdb0..0xdc8` (`S+0x660..0x678`) double[4].

## Default

1e-6, 1e-4, 0, 0. (2D models additionally set `M+0xdd0` = 100.0, not part of this keyword.)

## KEY writer

Section *Processing Conditions*, first: `FUN_1403d82f0` four `%20.10E`.

## Helpers

`FUN_140414e40`, `FUN_1403d82f0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
