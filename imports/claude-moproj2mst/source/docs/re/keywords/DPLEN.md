# DPLEN

DPLEN (3D only).

* registry id 59 (`0x3b`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x25 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D only: line 0 `%Flf` → `M+0xa18`; 2D: nothing read.

## Model fields

`M+0xa18` (`S+0x2c8`) double.

## Default

0.5.

## KEY writer

3D only: `"%-6s %20.10E\n"` after DTSUB.

## Helpers

`FUN_140414990`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 177 occurrences in 177 corpus files consumed exactly up to the next keyword/comment; e.g. `key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Operations/Task00001/SimCtrl.KEY`.
