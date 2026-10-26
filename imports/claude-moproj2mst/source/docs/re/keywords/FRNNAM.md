# FRNNAM

Name of the environment-region definition (used with ENVTMP 10).

* registry id 76 (`0x4c`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x36 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0 ignored, next line → `M+0xb90` (`FUN_140413ab0`, buffer 0x40, at most 63 characters).

## Model fields

`M+0xb90` char[0x40] (start of the ENVTMP region-list object).

## Default

empty.

## KEY writer

Written only when the ENVTMP region flag `M+0xb10` is set, right after ENVTMP: `"%-6s\n%s\n"`.

## Notes

Reading ENVTMP 10 after FRNNAM keeps the name (the rebuild touches only `+0x40` onwards).

## Helpers

`FUN_140413ab0`. Shared mechanics: [_common.md](_common.md).

## Corpus

No corpus KEY file contains this keyword; grammar is from code only.
