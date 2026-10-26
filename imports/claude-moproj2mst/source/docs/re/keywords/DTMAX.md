# DTMAX

Time increment per step (and a second value).

* registry id 50 (`0x32`), group 1, flag 0; handler columns 0x1402f7590 / 0x1403d7bf0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x1c (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf %Flf` (`FUN_140414a40`) → `M+0x920`, `M+0x928`. If `M+0x920 > 0`: DSMAX (`M+0x930`) = 0 (`FUN_1401f4390`) and `FUN_1401ee510(S)`.

## Model fields

`M+0x920`, `M+0x928` (`S+0x1d0`, `S+0x1d8`) double.

## Default

0, 0.

## KEY writer

Only when `M+0x93c == 0` (no DTTYPE table), `FUN_1403d7bf0`: if the time flag `M+0x990 == 1`: `"%-6s %20.10E %20.10E\n"` (`M+0x920`, `M+0x928`); else if `M+0x928 > 0`: same format with 0 and `M+0x928`; else nothing. It is followed by the literal `DTTYPE` line `"%-6s %7i %7i\n"` with 0, 0.

## Helpers

`FUN_140414a40`, `FUN_1401f4390`, `FUN_1401ee510`, `FUN_1403d7bf0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 762 occurrences in 337 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
