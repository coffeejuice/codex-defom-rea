# DTPMAX

Maximum time-step limits.

* registry id 53 (`0x35`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7f70 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1f (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

3D: line 0 `%Flf %Flf %Flf` with `FUN_140414d10` → `M+0x9c0`, `M+0x9f0`, `M+0x9f8`; if exactly one number is present all three are set to 0. 2D: `%Flf` ×5 (`FUN_140414f50`) → `M+0x9c0`, `M+0x9f0`, `M+0x9f8`, `M+0xa00`, `M+0xa08`.

## Model fields

`M+0x9c0`, `M+0x9f0`, `M+0x9f8`, `M+0xa00`, `M+0xa08` double.

## Default

all 0.

## KEY writer

`FUN_1403d7f70`: 3D `"%-6s %20.10E %20.10E %20.10E\n"`; 2D five `%20.10E` values.

## Helpers

`FUN_140414d10`, `FUN_140414f50`, `FUN_1403d7f70`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 486 occurrences in 484 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
