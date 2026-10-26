# CURSIM

Current simulation counters (three integers).

* registry id 23 (`0x17`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i %i` (`FUN_140413fb0`, no count output) → `M+0x834`, `M+0x838`, `M+0x83c`.

## Model fields

`M+0x834/0x838/0x83c` (`S+0xe4/0xe8/0xec`) int.

## Default

1, 0, 0.

## KEY writer

`"%-6s %7i %7i %7i\n"` with the three ints.

## Helpers

`FUN_140413fb0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 2431 occurrences in 507 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
