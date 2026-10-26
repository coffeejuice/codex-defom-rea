# TRANS

Five process flags (TRANS[0..4]).

* registry id 32 (`0x20`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0xa (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i %i %i %i` (`FUN_140414200`, no blank skipping). Only if the helper succeeded: each value `>= 0` is stored at `M+0x854 + 4i`; negative values leave the old flag. Missing fields read as 0 and are stored.

## Model fields

`M+0x854..0x864` (`S+0x104..0x114`) int[5]. TRANS[0], TRANS[1] are tied to SMODE.

## Default

0, 1, 0, 0, 0.

## KEY writer

`"%-6s %7i %7i %7i %7i %7i\n"` with the 5 ints.

## Helpers

`FUN_140414200`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 906 occurrences in 487 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
