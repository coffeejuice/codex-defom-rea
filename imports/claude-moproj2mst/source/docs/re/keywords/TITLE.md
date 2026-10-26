# TITLE

Problem title (one text line).

* registry id 22 (`0x16`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x0 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0 is ignored; the next line is the title (`FUN_140413ab0(f, M+0x750, 0x51)`): cut at the first `\n`, at most 80 characters kept, trailing spaces/CR removed. EOF on either line → 2.

## Model fields

`M+0x750` (`S+0x000`) char[0x51].

## Default

`"SIMULATION"` (`FUN_1401e7e90`).

## KEY writer

Section *Process Definition*, first record: `"%-6s\n%s\n"` → `TITLE` on its own line, then the string.

## Helpers

`FUN_140413ab0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 482 occurrences in 482 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
