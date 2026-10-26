# INTRST

Interface stress coefficient: constant or table against var 3 or 4.

* registry id 275 (`0x113`), group 3, flag 7; handler columns 0x1402f7590 / 0x1403cb1f0 / 0x1403cb300.
* Read: `FUN_140388fb0` case 0x3 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %Flf` → t, v (failure → 2). Table `pair+0x2b8` reset to template 0x4e; t = 0 → const (`+0x2c0`) = v; t = 1 → `ndim` 1, var 3, cnt (int)v; t = 2 → var 4. The table reader always runs (status).

## Model fields

Table `pair+0x2b8`.

## Default

template 0 (const 0).

## KEY writer

`FUN_1403cb0a0`, after SEPDEN: `ndim` 0 → `"%-6s %7i %7i %7i %20.10E\n"` (m, s, 0, const); `ndim` 1 with var 3/4 → `"%-6s %7i %7i %7i %7i\n"` (m, s, 1/2, cnt) + rows; otherwise return 3.

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403cb0a0`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 927 occurrences in 374 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
