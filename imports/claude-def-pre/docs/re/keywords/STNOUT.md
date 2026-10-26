# STNOUT

State-variable output switches (7 integers).

* registry id 34 (`0x22`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0xc (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: nine `%i` (`FUN_140414600`); the first seven go to `M+0x86c..0x884`, the 8th and 9th are discarded. Then `FUN_1401d0fc0(M, 0)` (layout rebuild, as GEOTYP).

## Model fields

`M+0x86c..0x884` (`S+0x11c..0x134`) int[7].

## Default

0, 0, 0, 0, 1, 0, 0.

## KEY writer

`FUN_1403d6580`: `"%-6s %7i %7i %7i %7i %7i %7i %7i\n"`.

## Notes

These counts feed `FUN_1401d09c0` (the sum of the first five and the last two size an output array).

## Helpers

`FUN_140414600`, `FUN_1401d0fc0`, `FUN_1403d6580`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 544 occurrences in 544 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
