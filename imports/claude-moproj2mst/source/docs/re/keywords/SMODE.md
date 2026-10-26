# SMODE

Simulation mode (deformation / heat transfer flags).

* registry id 27 (`0x1b`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x5 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` into a local, then `FUN_1401f3ff0(S, v)`: stores `v` at `M+0x844` and, for v = 3 → TRANS[0]=1, TRANS[1]=1; v = 2 → TRANS[0]=1, TRANS[1]=0; v = 1 → TRANS[0]=0, TRANS[1]=1; other values change only `M+0x844`. (TRANS[0] = `M+0x854` via `FUN_1401f3e90`, TRANS[1] = `M+0x858` via `FUN_1401f3eb0`.)

## Model fields

`M+0x844` (`S+0xf4`) int; also TRANS[0], TRANS[1].

## Default

1 (TRANS defaults 0, 1, consistent with mode 1).

## KEY writer

Written after SIMNAM: the value is first recomputed by `FUN_1401f3f80` from TRANS[0..1] (both ≠0 → 3; TRANS[0]=0 → 1; else 2), then `"%-6s %7i\n"` with `M+0x844`. A file with an out-of-range SMODE therefore round-trips to 1/2/3.

## Helpers

`FUN_140413de0`, `FUN_1401f3ff0`, `FUN_1401f3f80`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 490 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
