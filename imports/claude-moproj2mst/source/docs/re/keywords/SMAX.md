# SMAX

Stop criterion SMAX (stroke).

* registry id 43 (`0x2b`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7820 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x15 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D: line 0 `%Flf %Flf %Flf` (`FUN_140414c20`) → `M+0x8d0`, `+0x8d8`, `+0x8e0`. 2D: `%Flf %Flf` (`FUN_140414a40`) → the first two only.

## Model fields

`M+0x8d0` double[3] (`S+0x180`).

## Default

0, 0, 0.

## KEY writer

`FUN_1403d7820`: 3D `"%-6s %20.10E %20.10E %20.10E\n"` (three values); 2D `"%-6s %20.10E %20.10E\n"` (two).

## Helpers

`FUN_140414c20`, `FUN_140414a40`, `FUN_1403d7820`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 486 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
