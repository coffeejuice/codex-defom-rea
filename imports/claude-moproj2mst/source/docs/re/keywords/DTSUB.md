# DTSUB

Sub-step time control.

* registry id 58 (`0x3a`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x24 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf` (`FUN_140414990`) → `M+0xa10`. Helper failure → 2.

## Model fields

`M+0xa10` (`S+0x2c0`) double.

## Default

1.0 in 3D, 0 in 2D.

## KEY writer

Section *Stopping & Step Controls*. `"%-6s %20.10E\n"`, value `M+0xa10`. Written after VFACTR.

## Helpers

`FUN_140414990`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 416 occurrences in 414 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
