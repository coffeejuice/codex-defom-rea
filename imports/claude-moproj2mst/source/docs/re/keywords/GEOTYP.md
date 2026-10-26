# GEOTYP

Geometry type (2D only).

* registry id 28 (`0x1c`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x6 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Only when the model is 2D: line 0 `%i` → `M+0x850`, then `FUN_1401d0fc0(M, 0)` (rebuilds the state-variable layout table `M+0x15f8`, see notes). In 3D the handler reads **nothing** (status 0): the rest of the keyword line is left for the main loop, whose next `%39s` token will be the value and will produce `WARNING: Keyword … not found`.

## Model fields

`M+0x850` (`S+0x100`) int.

## Default

1.

## KEY writer

2D only: `"%-6s %7i\n"` after STYPE.

## Notes

`FUN_1401d0fc0` recomputes the per-variable component counts (`FUN_1401d09c0` logic: GEOTYP 3 in 2D switches several entries to 3 components) and re-maps object state arrays; it belongs to the model skeleton (Phase 1) and is only referenced here.

## Helpers

`FUN_140413de0`, `FUN_1401d0fc0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 356 occurrences in 356 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
