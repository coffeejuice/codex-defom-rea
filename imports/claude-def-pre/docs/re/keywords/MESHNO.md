# MESHNO

Mesh number.

* registry id 29 (`0x1d`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x7 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → `M+0x840`; if read option bit 2 (`M+0x1740 & 2`) is set the value goes to a dummy.

## Model fields

`M+0x840` (`S+0xf0`) int.

## Default

1.

## KEY writer

Skipped when write option bit 4 (`M+0x1744 & 4`); else `"%-6s %7i\n"`.

## Helpers

`FUN_140413de0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 359 occurrences in 359 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
