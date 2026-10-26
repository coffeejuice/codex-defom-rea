# OSCTRL

Four control integers (2D only).

* registry id 39 (`0x27`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x11 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

2D only: line 0 `%i %i %i %i` (`FUN_140414100`) → `M+0x1108..0x1114`. In 3D nothing is read (status 0; the rest of the line reaches the main loop, see GEOTYP).

## Model fields

`M+0x1108..0x1114` (`S+0x9b8..0x9c4`) int[4].

## Default

0 ×4 (set only for 2D models).

## KEY writer

2D only: `"%-6s %7i %7i %7i %7i\n"` after PDIE.

## Helpers

`FUN_140414100`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 305 occurrences in 305 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
