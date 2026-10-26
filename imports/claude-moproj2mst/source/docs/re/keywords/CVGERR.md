# CVGERR

Convergence error limits (velocity, force).

* registry id 61 (`0x3d`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d8260 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x27 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf %Flf` → `M+0xa20`, `M+0xa28`.

## Model fields

`M+0xa20`, `M+0xa28` (`S+0x2d0/0x2d8`) double.

## Default

2D 0.001, 0.01; 3D 0.005, 0.05.

Corrected by kw-g1b: `FUN_1401e96b0` tests `FUN_1400015e0` (2D) and takes the first branch when it is false (3D); the earlier text had 2D and 3D swapped. Confirmed by the oracle: an empty KEY round trip writes the 3D values, a 2D one the 2D values (`port/defkey/tests/data/kw_g1b/c2_empty.expected`).

## KEY writer

Section *Iteration Controls*, first: `FUN_1403d8260` `"%-6s %20.10E %20.10E\n"`.

## Helpers

`FUN_140414a40`, `FUN_1403d8260`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 490 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
