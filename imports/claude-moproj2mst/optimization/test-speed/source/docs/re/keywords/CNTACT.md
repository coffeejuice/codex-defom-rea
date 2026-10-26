# CNTACT

Declares a contact pair (master, slave, direction) or clears all pairs.

* registry id 271 (`0x10f`), group 3, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_140388fb0`. Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

The keyword line is tokenised (`FUN_140413900`, up to 3 tokens split on space/comma/tab/CR/LF, converted with `strtol` base 10; helper status = handler status):
* 1 token v: if v == 0 all pairs are deleted (`FUN_1401fade0`); return 0 (no mirroring).
* 3 tokens m, s, d: find/append pair (m,s); d = 0 → direction `+0x170` = 0. 3D: d = 1 → (`+0x170`,`+0x174`) = (1,1); 2 → (2,1); 3 → (1,0); 4 → (2,0). 2D: d = 1 → `+0x170` = 1; 2 → 2. Any other d leaves the pair as it was. Then mirroring.
* 2 tokens: nothing.

<a id="mirroring"></a>**Mirroring** (end of `FUN_140388fb0`, every group 3 keyword except CNTACT with 1 token): if m ≠ s, pair (s,m) is found or appended; its direction becomes 2 if (m,s) has direction 1, 1 if 2, else 0; then `FUN_1401f9e80` copies all data of (m,s) into (s,m) via `FUN_1401f9900`, restoring the target's own `+0x168/+0x16c/+0x170`. Copied: `+0x174` (3D only), SEPRES `+0x178/+0x180`, friction and IHTCOF blocks (`FUN_1401f9d50`, `FUN_1401f9e10`), SEPDEN `+0x238` and FRCROT table `+0x240` (2D only), the window list `+0x188` (`FUN_140269340`), the parameter doubles `+0x108..+0x160`, INTRST table `+0x2b8`, CNTMTH, WMODEL and RGDCNT fields.

### Mirror copy in detail

`FUN_1401f9e80(dst, src)` saves `dst+0x168/+0x16c/+0x170`, calls `FUN_1401f9900(dst, src)`, then restores them (the direction was set just before from the source's direction). `FUN_1401f9900` copies, in this order:

1. `+0x168`, `+0x16c`, `+0x170` (restored afterwards), `+0x174` only if the model is 3D (`FUN_1400015f0(dst+0x398)`), `+0x178` (int) and `+0x180` (double): SEPRES.
2. `FUN_1401f9d50(dst, src)`, the friction block header and tables: `+0x0` mode (int), `+0x4` and `+0x5` fixed bytes, `+0x8`, `+0xc` ids, `+0x10`, `+0x14` types, table `+0x18` (friction 1) and table `+0x90` (friction 2) by `FUN_14012eff0`. It does not copy the parameter pairs (step 6 does).
3. `FUN_1401f9e10(dst, src)`, the IHTCOF block: table `+0x1b8` by `FUN_14012eff0`, `+0x230` id, `+0x234` id flag.
4. 2D only (`FUN_1400015e0`): `+0x238` (SEPDEN, double) and table `+0x240` (FRCROT).
5. `FUN_140269340(dst+0x188, src+0x188)`: the window list. It clears the target list (`FUN_1402699c0`), copies list `+0x18` and `+0x28`, then appends one new window per source window (`FUN_1402694a0`) and copies it with `FUN_14025c9b0` (all window data: `+0x8` friction block, `+0x170/+0x174`, dimension `+0x898`, `+0x890`, points, shape `+0x210`, the `+0x220` mesh, `+0x188..+0x200`, `+0x1b8` (3D window), `+0x1c0`, `+0x350..+0x360`, `+0x368/+0x36c`), with the new window's link `+0x840` = 0.
6. The friction parameter doubles `+0x108..+0x160` (twelve, in the order 108, 110, 138, 140, 118, 120, 148, 150, 128, 130, 158, 160; the order does not matter).
7. Table `+0x2b8` (INTRST), `+0x330`, `+0x334` (int), `+0x338` (double), `+0x340` (int), `+0x348..+0x370` (six doubles), `+0x378`, `+0x37c`, `+0x380`, `+0x384` (int), `+0x388` (double): the CNTMTH, WMODEL and RGDCNT fields.

Not copied: `+0x390` (list link), `+0x398` (model pointer), and `+0x174` in 2D, `+0x238`/`+0x240` in 3D.

Table copy `FUN_14012eff0(dst, src)` (0x78-byte table, [_common.md](_common.md) §3): copies the template id `+0x0` (and `+0x58` only when the template is `0x80`), the constant `+0x8`, `+0x54`, `ndim` `+0x4`, `ncomp` `+0x40`, `+0x50`, `var[0..2]` and `cnt[0..2]`; then `FUN_14012fad0(dst)` frees (`FUN_14012f480`), clamps and re-allocates the arrays for these sizes; then copies the abscissa arrays (`cnt[i]` doubles each, for `i < ndim`, stopping at the first null pointer) and the value array (`FUN_14012ff20` doubles) when both pointers are non-null. `+0x60` keeps the target's value. For a port this is a deep copy of the table that leaves `+0x60` alone.

## Model fields

Pair `+0x168` master, `+0x16c` slave, `+0x170` direction (0 none, 1, 2), `+0x174` 3D sub-flag (1/0).

## Default

A new pair (`FUN_1401f46e0`): direction 0, `+0x174` = 1, IHTCOF table template 0x17, FRCROT table template 0x16, friction template 0x15 (mode 1), all other values 0, CNTMTH `+0x334` = 1.

## KEY writer

Section banner `Inter-Object Data`, then for every pair in list order (`FUN_1403c9ad0` → `FUN_1403c8560`): skipped if m = 0 or s = 0, if s < m, or if m = s with direction 1. Code: direction 0 → 0; 2D: 1 → 1, 2 → 2; 3D: (1,1) → 1, (2,1) → 2, (1,0) → 3, (2,0) → 4; any other combination → return 3 (KEY write aborted). Record: `"%-6s %7i %7i %7i\n"` (m, s, code). With direction 0 nothing else is written for the pair. Otherwise the pair's records follow in this order: FRCFAC, FRCROT (2D), IHTCOF, FRCWIN (each window), SEPRES, SEPDEN (2D), INTRST, CNTMTH (3D), WMODEL, RGDCNT. The first non-zero return aborts.

## Notes

Because only pairs with m ≤ s are written and every keyword mirrors its data, a KEY round trip writes each contact once, from the lower-numbered object. Data given only for (s,m) with s > m still reaches the written (m,s) pair through the mirror copy.

## Helpers

`FUN_140413900`, `FUN_140413a50`, `FUN_14047b6c0`, `FUN_1401fade0`, `FUN_1401facf0`, `FUN_1401faaa0`, `FUN_1401f46e0`, `FUN_1401f9e80`, `FUN_1401f9900`, `FUN_1403c9ad0`, `FUN_1403c8560`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 2430 occurrences in 470 corpus files consumed exactly up to the next keyword/comment; e.g. `key/2D/EN/Creep/CREEP_Example1_EN.KEY`.
