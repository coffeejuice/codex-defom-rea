# PDIE

Primary die object and a second integer.

* registry id 38 (`0x26`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x10 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` (`FUN_140413ee0`) → `M+0x8b8`, `M+0x1104`.

## Model fields

`M+0x8b8` (`S+0x168`), `M+0x1104` (`S+0x9b4`) int.

## Default

0, 0.

## KEY writer

`"%-6s %7i %7i\n"`.

## Notes

PDIE[0] is also used by DSMAX_NSTEP to find the primary die object.

## Helpers

`FUN_140413ee0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 500 occurrences in 486 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
