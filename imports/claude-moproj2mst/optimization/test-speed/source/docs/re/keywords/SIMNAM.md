# SIMNAM

Simulation name (one text line).

* registry id 25 (`0x19`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x3 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

As TITLE: line 0 ignored, next line → `M+0x7e2`, buffer 0x51 (at most 80 characters).

## Model fields

`M+0x7e2` (`S+0x92`) char[0x51].

## Default

`"SIMULATION 1"`.

## KEY writer

`"%-6s\n%s\n"`. **Side effect right after SIMNAM is written:** `FUN_1401f3f80(S)` recomputes SMODE from TRANS[0..1]: both non-zero → 3, TRANS[0]==0 → 1, otherwise → 2 (see SMODE).

## Helpers

`FUN_140413ab0`, `FUN_1401f3f80`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 2431 occurrences in 507 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
