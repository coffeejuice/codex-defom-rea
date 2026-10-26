# IHTCOF

Interface heat-transfer coefficient: constant, table, or fixed id.

* registry id 273 (`0x111`), group 3, flag 7; handler columns 0x1402f7590 / 0x1403cb3e0 / 0x1403cb520.
* Read: `FUN_140388fb0` case 0x1 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %Flf` → t, v (failure → 2). Table `pair+0x1b8` reset to template 0x17. t = 0 → const (`+0x1c0`) = v; t = 1 → `ndim` 1, var 1, cnt (int)v, table reader; t = 2 → var 4, table reader; t < 0 → `+0x230` = −t, `+0x234` = 1 (table stays const 0); otherwise `+0x230` = `+0x234` = 0. Other positive t: reset only.

## Model fields

Table `pair+0x1b8`, `+0x230` id, `+0x234` id flag.

## Default

template 0x17 (const 0), id 0.

## KEY writer

`FUN_1403ca830`, after FRCROT: id flag set → `"%-6s %7i %7i %7i %20.10E\n"` (m, s, −id, 0.0); `ndim` 0 → (m, s, 0, const); `ndim` 1 with var 1 or 4 → `"%-6s %7i %7i %7i %7i\n"` (m, s, 1 or 2, cnt) + rows; else return 3. Nothing is written if m or s is 0.

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403ca830`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 993 occurrences in 378 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
