# ITRMTH

Iteration method.

* registry id 63 (`0x3f`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x29 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → v. Valid if 1 ≤ v ≤ 3 (`FUN_1401ea4d0`, same range in 2D and 3D): store at `M+0xa30`, and in 2D a value of 3 is changed to 1. Invalid: `ITRMTH %d - ignored\n` on stdout, value unchanged. Status = helper status.

## Model fields

`M+0xa30` (`S+0x2e0`) int.

## Default

1 in 2D, 2 in 3D.

Corrected by kw-g1b: `FUN_1401e96b0` tests `FUN_1400015e0` (2D) and takes the first branch when it is false (3D); the earlier text had 2D and 3D swapped. Confirmed by the oracle: an empty KEY round trip writes the 3D values, a 2D one the 2D values (`port/defkey/tests/data/kw_g1b/c2_empty.expected`).

## KEY writer

`"%-6s %7i\n"` after CVGERR.

## Helpers

`FUN_140413de0`, `FUN_1401ea4d0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
