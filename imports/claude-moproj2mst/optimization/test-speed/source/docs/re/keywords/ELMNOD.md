# ELMNOD

Element/node output selection (up to 10 integers on a second line).

* registry id 35 (`0x23`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0xd (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i` → n (local). The **next line** is read by `FUN_1404147e0`: up to 10 `%i`, of which the first `min(n,10)` (none if n ≤ 0) are copied to `M+0x888…`. Then `FUN_1401d0fc0(M, 0)`. The status is that of line 0 only (EOF on the second line is ignored).

## Model fields

`M+0x888..0x8ac` (`S+0x138..0x15c`) int[10].

## Default

all 0.

## KEY writer

`FUN_1403d6620`: `"%-6s %7i\n"` with the constant 10, then `FUN_1402f2fe0(f,10,M+0x888,10,0)`: one line of ten `" %7i"` and `"\n"`.

## Notes

Elements 0..2 also gate `FUN_1401d09c0` (value 0 or 3 zeroes some layout entries).

## Helpers

`FUN_140413de0`, `FUN_1404147e0`, `FUN_1401d0fc0`, `FUN_1403d6620`, `FUN_1402f2fe0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 533 occurrences in 533 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
