# ODMAX

ODMAX (3D only).

* registry id 60 (`0x3c`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x26 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D only: line 0 `%Flf` into a local, then `FUN_14039d2b0(S, v)` stores it at `S+0xbc8` (`M+0x1318`). 2D: nothing read.

## Model fields

`M+0x1318` (`S+0xbc8`) double.

## Default

0 (`FUN_1401e8fb0`).

## KEY writer

3D only, after DPLEN: value from `FUN_1403d8490(S)`, `"%-6s %20.10E\n"`.

## Helpers

`FUN_140414990`, `FUN_14039d2b0`, `FUN_1403d8490`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 168 occurrences in 168 corpus files consumed exactly up to the next keyword/comment; e.g. `key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Operations/Task00001/SimCtrl.KEY`.
