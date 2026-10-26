# SOLMTT

Thermal solver.

* registry id 66 (`0x42`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x2c (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → v. Valid if 1 ≤ v ≤ 5 (`FUN_1401ea610`): store at `M+0xa68`, and in 2D 2 is changed to 1. Invalid: `SOLMTT %d - ignored\n`.

## Model fields

`M+0xa68` (`S+0x318`) int.

## Default

1 in 2D, 2 in 3D.

Corrected by kw-g1b: `FUN_1401e96b0` tests `FUN_1400015e0` (2D) and takes the first branch when it is false (3D); the earlier text had 2D and 3D swapped. Confirmed by the oracle: an empty KEY round trip writes the 3D values, a 2D one the 2D values (`port/defkey/tests/data/kw_g1b/c2_empty.expected`).

## KEY writer

`"%-6s %7i\n"` after DEFBWD.

## Helpers

`FUN_140413de0`, `FUN_1401ea610`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 886 occurrences in 485 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
