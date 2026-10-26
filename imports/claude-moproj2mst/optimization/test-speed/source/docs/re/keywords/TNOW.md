# TNOW

Current time / stroke state.

* registry id 31 (`0x1f`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x9 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%Flf %Flf %Flf %Flf` (`FUN_140414e40`) → a, b, c, d. Then: a > -1e20 → `M+0xd90`; b > -1e20 → `M+0xd98`; `M+0xda0` = (int)c (truncation); d > -1e20 → `M+0xda8`. Missing fields are 0 and therefore stored. The helper status is the handler status.

## Model fields

`M+0xd90`, `M+0xd98` double; `M+0xda0` int; `M+0xda8` double (`S+0x640..0x658`).

## Default

all 0.

## KEY writer

`"%-6s %20.10E %20.10E %7i %20.10E\n"` with `M+0xd90, M+0xd98, M+0xda0, M+0xda8`; when write option bit 8 (`M+0x1744 & 8`) is set the first two are replaced by -1e21 and 0.

## Helpers

`FUN_140414e40`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 2223 occurrences in 427 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
