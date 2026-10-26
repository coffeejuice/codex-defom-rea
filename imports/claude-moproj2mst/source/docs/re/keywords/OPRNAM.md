# OPRNAM

Operation name (one text line).

* registry id 24 (`0x18`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x2 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

As TITLE: line 0 ignored, next line → `M+0x7a1`, buffer 0x41 (at most 64 characters kept).

## Model fields

`M+0x7a1` (`S+0x51`) char[0x41].

## Default

`"OPERATION 1"`.

## KEY writer

`"%-6s\n%s\n"`.

## Helpers

`FUN_140413ab0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 1937 occurrences in 127 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY`.
