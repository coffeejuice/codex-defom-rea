# DSMAX

Stroke increment per step.

* registry id 48 (`0x30`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7b70 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1a (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` → `M+0x930`. If the value > 0: `M+0x920` (DTMAX[0]) = 0 and `FUN_1401ee510(S)` (recompute the time/stroke flag `M+0x990`, see `_common.md` §4).

## Model fields

`M+0x930` (`S+0x1e0`) double.

## Default

0.

## KEY writer

`FUN_1403d7b70`: `"%-6s %20.10E\n"`, written only when no DTTYPE table (`M+0x93c == 0`) **and** the time flag `M+0x990 == 0`.

## Helpers

`FUN_140414990`, `FUN_1401ee510`, `FUN_1403d7b70`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 576 occurrences in 157 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/ElastoPlastic/Extrusion-EP_EN.KEY`.
