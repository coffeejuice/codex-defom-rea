# NSTART

Starting step number.

* registry id 36 (`0x24`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0xe (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → `M+0x8b0`; on success also sets the byte `M+0x2f88` = 1 ("NSTART given"). With read option bit 4 (`M+0x1740 & 4`) the value goes to a dummy and the byte is untouched.

## Model fields

`M+0x8b0` (`S+0x160`) int; flag byte `M+0x2f88`.

## Default

-1.

## KEY writer

Section *Stopping & Step Controls*; skipped when write option bit 1; `"%-6s %7i\n"`.

## Helpers

`FUN_140413de0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 343 occurrences in 343 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
