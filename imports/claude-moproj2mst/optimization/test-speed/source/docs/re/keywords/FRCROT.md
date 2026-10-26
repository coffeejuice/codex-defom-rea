# FRCROT

Rotational friction factor: constant or table (var 1 or 4).

* registry id 276 (`0x114`), group 3, flag 7; handler columns 0x1402f7590 / 0x1403cab90 / 0x1403cac80.
* Read: `FUN_140388fb0` case 0x4 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line `%i %Flf` → t, v (failure → 2). Table `pair+0x240` reset to template 0x16; t = 0 → const (`+0x248`) = v; t = 1 → `ndim` 1, var 1, cnt (int)v; t = 2 → var 4. Table reader always runs. Read in 2D and 3D.

## Model fields

Table `pair+0x240` (mirrored only in 2D).

## Default

template 0x16 (const 0).

## KEY writer

2D only, right after FRCFAC: `FUN_1403caa40`: `ndim` 0 → `"%-6s %7i %7i %7i %20.10E\n"` (m, s, 0, const); `ndim` 1 with var 1/4 → `"%-6s %7i %7i %7i %7i\n"` (m, s, 1/2, cnt) + rows; else return 3.

## Helpers

`FUN_1404154c0`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403caa40`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 629 occurrences in 247 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
