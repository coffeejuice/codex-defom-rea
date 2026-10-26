# SOLMTD

Deformation solver, with optional parameters on a second line.

* registry id 70 (`0x46`), group 1, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_14039aa90` case 0x30 (`id - 0x16`). Write: `FUN_1403d4990`.

## Read grammar

Line 0: `%i %i` → m, k (status of this line is the handler status unless a second line is read). Valid if 1 ≤ m ≤ 6 (`FUN_1401ea570`); invalid: `SOLMTD %d - ignored\n`, nothing changed. Valid: `M+0xa34` = m, `M+0xa38` = 0, and
* m = 2, k = 4: `M+0xa38` = 4; the **next line** `%i %i %i %i` (`FUN_140414100`) gives S1..S4, stored as doubles at `M+0xa40, 0xa48, 0xa50, 0xa58`; status = that line.
* m = 2, k ≠ 4: `M+0xa38` = 4, S1..S4 = 2, 4, 1, 1; prints `SOLMTD 2 - parameters set to default values\n` then `S1:%d;  S2:%d;  S3:%d;  S4:%d\n`. The disassembly (`0x14039c06c`) takes S4 from `(int)*(double*)(M+0xa60)` — the 8 bytes of ITRMXD/DEFBWD read as a double — not from `M+0xa58`; with integer ITRMXD/DEFBWD this is a denormal and prints 0. Reproduce it bit-exactly.
* m = 6, k = 4: `M+0xa38` = 4; next line `%Flf` ×4 → `M+0xa40..0xa58`.
* m = 6, k ≠ 4: prints `SOLMTD 6 - missing parameters\n`, parameters unchanged.

## Model fields

`M+0xa34` solver, `M+0xa38` parameter count, `M+0xa40..0xa58` double[4].

## Default

solver 1, count 0, parameters 0.

## KEY writer

After ITRMTH. m = 2: `"%-6s %7i %7i\n"` (2, 4) then `" %7i %7i %7i %7i\n"` (parameters truncated to int); m = 6: `"%-6s %7i %7i\n"` (6, 4) then `" %20.10E %20.10E %7i %7i\n"` — the last two are written as **ints** (truncated) although read as doubles; other m: `"%-6s %7i\n"`. The header always says 4, even when `M+0xa38` is 0.

## Helpers

`FUN_140413ee0`, `FUN_1401ea570`, `FUN_140414100`, `FUN_140414e40`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 487 occurrences in 485 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
