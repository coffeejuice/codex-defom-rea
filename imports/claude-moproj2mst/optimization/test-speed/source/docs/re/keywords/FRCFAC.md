# FRCFAC

Friction between two objects (one or two friction laws).

* registry id 272 (`0x110`), group 3, flag 7; handler columns 0x1402f7590 / 0x1403cb5f0 / 0x1403cb740.
* Read: `FUN_140388fb0` case 0x0 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

Rest of line: `%i %i %Flf %Flf %Flf` (`FUN_140416100`) → mode, t, d1, d2, d3 (failure → 2). mode 0..4 is stored in the pair's `+0x0`; other values leave the stored mode. If the stored mode is 4 (two frictions), two more lines follow, each `%i %Flf %Flf %Flf` (`FUN_140415880`, failure → 2) = (t, d1, d2, d3) applied to friction 1 and friction 2. Otherwise (t, d1, d2, d3) from the keyword line are applied to friction 1.

**Friction sub-record** (shared by FRCFAC and FRCWIN; layout of `FUN_1401fb020`, offsets relative to the friction block): `+0x0` mode, `+0x4`/`+0x5` "fixed" bytes for friction 1/2, `+0x8`/`+0xc` positive ids, `+0x10`/`+0x14` types, table 1 at `+0x18` (template 0x15; const `+0x20`, var `+0x28..`, cnt `+0x34..`), table 2 at `+0x90` (const `+0x98`), parameter pairs for types 6/7/8 at `+0x108/+0x110`, `+0x118/+0x120`, `+0x128/+0x130` (friction 1) and `+0x138..+0x160` (friction 2).

Applying (type t, numbers d1, d2, d3) to friction k: t < 0 → fixed byte = 1, id = −t, `ndim` 0, const = d1 (no data). Otherwise fixed byte = 0, table reset to template 0x15, type = t and: 0 → const d1; 1 → `ndim` 1, var 1, cnt (int)d1; 2 → var 4; 3 → var 3; 4 → var 21; 5 → `ndim` 3 with vars 4, 3, 21 and counts (int)d1, (int)d2, (int)d3; 6/7/8 → parameter pair (d1, d2), `ndim` 0. If `ndim` ≠ 0 the table reader `FUN_14041a800` follows (its status becomes the handler status). Types > 8 only reset the table.

Writing friction k (`FUN_1403c9be0` and the FRCWIN writer): fixed → `" %7i %20.10E\n"` (−id, const); else by type: 0 → `" %7i %20.10E\n"` (0, const); 1–4 → `" %7i %7i\n"` (t, cnt[0]); 5 → `" %7i %7i %7i %7i\n"` (5, cnt[0..2]); 6–8 → `" %7i %20.10E %20.10E\n"` (t, pair); then the table rows (`FUN_1402f3130`) if `ndim` ≠ 0.

## Model fields

Friction block at pair `+0x0` (see above).

## Default

mode 1, both frictions constant 0 (template 0x15).

## KEY writer

First pair record after CNTACT, `FUN_1403c9be0(M, f, pair, 0, 0)` (m/s from the pair). mode outside 0..4 → return 3. mode 4: `"%-6s %7i %7i %7i\n"` (m, s, 4) then friction 1 and friction 2 as sub-records. Other modes: one header line that carries friction 1: fixed → (m, s, mode, −id, const); type 0 → (m, s, mode, 0, const); 1–4 → (m, s, mode, t, cnt[0]); 5 → (m, s, mode, 5, cnt[0..2]); 6–8 → (m, s, mode, t, pair); then the table rows if `ndim` ≠ 0. (Exact formats: `"%-6s %7i %7i %7i %7i %20.10E\n"`, `"%-6s %7i %7i %7i %7i %7i\n"`, `"%-6s %7i %7i %7i %7i %7i %7i %7i\n"`, `"%-6s %7i %7i %7i %7i %20.10E %20.10E\n"`.)

## Notes

Registry column 2 `FUN_1403cb5f0` passes explicit m, s from its vector (needs 3 elements); column 3 `FUN_1403cb740` returns friction table 1 (`+0x18`) or, for the label `"Fric Frac 2"`, table 2 (`+0x90`).

## Helpers

`FUN_140416100`, `FUN_140415880`, `FUN_14012fc70`, `FUN_14041a800`, `FUN_1403c9be0`, `FUN_1402f3130`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 959 occurrences in 376 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/DieStress/DieStress_Example2_EN.KEY`.
