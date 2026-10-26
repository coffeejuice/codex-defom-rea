# LMAX

Stop criterion LMAX.

* registry id 45 (`0x2d`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7a00 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x17 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D: line 0 `%Flf %Flf %Flf` (`FUN_140414c20`) → `M+0x900`, `+0x908`, `+0x910`. 2D: `%Flf %Flf` (`FUN_140414a40`) → the first two only.

## Model fields

`M+0x900` double[3] (`S+0x1b0`).

## Default

0, 0, 0.

## KEY writer

`FUN_1403d7a00`: 3D `"%-6s %20.10E %20.10E %20.10E\n"` (three values); 2D `"%-6s %20.10E %20.10E\n"` (two).

## Helpers

`FUN_140414c20`, `FUN_140414a40`, `FUN_1403d7a00`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 486 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
