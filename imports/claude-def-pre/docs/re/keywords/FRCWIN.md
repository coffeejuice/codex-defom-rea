# FRCWIN

Friction window: a region of the contact with its own friction.

* registry id 278 (`0x116`), group 3, flag 0; handler columns 0x1402f7590 / 0x1402f75d0 / 0x1402f7610.
* Read: `FUN_140388fb0` case 0x6 (`id - 0x110`). Write: `FUN_1403c9ad0` → `FUN_1403c8560` (per pair).

## Read grammar

Every group 3 keyword except CNTACT starts with `fscanf(f, "%i %i")` on the keyword line → master m, slave s (fscanf -1 → 2). The pair `(m,s)` is found (`FUN_1401facf0`) or appended (`FUN_1401faaa0`, with `+0x168` = m, `+0x16c` = s). The handler then `fgets` **the rest of the same line**. After the handler, if m ≠ s the pair is mirrored into `(s,m)` (see [CNTACT.md](CNTACT.md#mirroring)).

The rest of the line is tokenised (`FUN_140413900`; 6 tokens in 3D, 8 in 2D; ints by `strtol`, doubles by `atof`). Common tokens: w (window number), g (3D shape / 2D point count), k (friction kind 1..4), t (friction type), d (friction value), q (orientation flag). The window list `pair+0x188` is extended to w entries (`FUN_140269bc0`; new windows get their list index in `+0x170`) and window w is reset (`FUN_14025d1a0(win,1)`, which keeps `+0x170` and the link `+0x840`); `win+0x8` = k when 1..4.

* 3D: next line `%Flf %Flf %Flf` → `win+0x350..0x360` (failure → 2). g = 1 → `win+0x210` = 1, g = 2 → 2, else return 2. Then `FUN_14041b7d0(f, win, 1)`: shape 1 → 8 lines `%i %Flf %Flf %Flf` (index must equal 1..8); shape 2 → 2 lines `%Flf %Flf %Flf` then one line `%Flf %Flf` → `win+0x1b8`, `win+0x1c0`. Points go to `win+0x180` (3 doubles each, count `win+0x178`). Then `FUN_140267760(win)` derives the window faces.
* 2D: tokens 6 and 7 → `win+0x350`, `win+0x358`; then `FUN_14041b6a0(f, win, g, 1)`: g lines `%i %Flf %Flf` (index must equal 1..g; 2 doubles per point).
Any read failure or index mismatch → 2.

q: 0 → `win+0x368` = 2, `win+0x36c` = −(s if the pair direction is 1, else m); 1 → `win+0x368` = 0; 2 → 1; q < 0 → 2 and `win+0x36c` = |q|; q > 2 → return 2.

Friction (block at `win+0x8`): k = 4 → two lines `%i %Flf %Flf %Flf` exactly as FRCFAC mode 4. Otherwise (t, d) from the tokens: t < 0 → fixed, id −t, const d; 0 → const d; 1–4 → table, cnt (int)d; 5 → **one more line** `%i %i %i` with the three counts; 6–8 → **one more line** `%Flf %Flf` with the parameter pair; then the table reader if `ndim` ≠ 0.

**Friction sub-record** (shared by FRCFAC and FRCWIN; layout of `FUN_1401fb020`, offsets relative to the friction block): `+0x0` mode, `+0x4`/`+0x5` "fixed" bytes for friction 1/2, `+0x8`/`+0xc` positive ids, `+0x10`/`+0x14` types, table 1 at `+0x18` (template 0x15; const `+0x20`, var `+0x28..`, cnt `+0x34..`), table 2 at `+0x90` (const `+0x98`), parameter pairs for types 6/7/8 at `+0x108/+0x110`, `+0x118/+0x120`, `+0x128/+0x130` (friction 1) and `+0x138..+0x160` (friction 2).

Applying (type t, numbers d1, d2, d3) to friction k: t < 0 → fixed byte = 1, id = −t, `ndim` 0, const = d1 (no data). Otherwise fixed byte = 0, table reset to template 0x15, type = t and: 0 → const d1; 1 → `ndim` 1, var 1, cnt (int)d1; 2 → var 4; 3 → var 3; 4 → var 21; 5 → `ndim` 3 with vars 4, 3, 21 and counts (int)d1, (int)d2, (int)d3; 6/7/8 → parameter pair (d1, d2), `ndim` 0. If `ndim` ≠ 0 the table reader `FUN_14041a800` follows (its status becomes the handler status). Types > 8 only reset the table.

Writing friction k (`FUN_1403c9be0` and the FRCWIN writer): fixed → `" %7i %20.10E\n"` (−id, const); else by type: 0 → `" %7i %20.10E\n"` (0, const); 1–4 → `" %7i %7i\n"` (t, cnt[0]); 5 → `" %7i %7i %7i %7i\n"` (5, cnt[0..2]); 6–8 → `" %7i %20.10E %20.10E\n"` (t, pair); then the table rows (`FUN_1402f3130`) if `ndim` ≠ 0.

## Model fields

Window list `pair+0x188` (elements `LLWINDEF_DATA`, 0x920 bytes, link `+0x840`, count at list `+0x14`); per window: `+0x8` friction block, `+0x170` index, `+0x178` point count, `+0x180` points, `+0x1b8/+0x1c0` two extra doubles, `+0x210` shape, `+0x350..+0x360` three doubles, `+0x368/+0x36c` orientation.

## Default

No windows.

## KEY writer

For each window of a pair with a direction (`FUN_1403c8560`), after IHTCOF. Kind code = `win+0x8` (1..4, else return 3); 3D shape code = `win+0x210` (1/2, else return 3); orientation code: `+0x368` 0 → 1, 1 → 2, 2 → −|`+0x36c`|.
* k = 4, 3D: `"%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n"` (m, s, w, shape, 4, 0, 0, orient) then `" %20.10E %20.10E %20.10E\n"` (`+0x350..0x360`). 2D: `"%-6s %7i %7i %7i %7i %7i %7i %7i %7i %15.7E %15.7E\n"` (m, s, w, point count, 4, 0, 0, orient, `+0x350`, `+0x358`). Then the geometry (`FUN_1402f4890`) and the two friction sub-records.
* other k: the friction type and value go into the header: fixed → (…, k, −id, value as `%20.10E`, orient) in 3D, `%15.7E` in 2D; type 0 → (…, k, 0, const, orient); types 1–4 → (…, k, t, cnt, orient) all `%7i`; 5–8 → (…, k, t, 0, orient). 3D formats: `"%-6s %7i %7i %7i %7i %7i %7i %20.10E %7i\n"` / `"%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n"`; 2D formats append `%15.7E %15.7E` (`+0x350`, `+0x358`) and use `%15.7E` for the value. 3D then writes `" %20.10E %20.10E %20.10E\n"`. Then the geometry, then for type 5 `" %7i %7i %7i\n"` (counts), for 6–8 `" %20.10E %20.10E\n"`, then the table rows.

Geometry `FUN_1402f4890(f, win, 0)`: 2D windows (`win+0x898 == 2`): per point `" %7i"` index + `" %20.10E"` ×2; 3D shape 1: per point index + 3 values; shape 2: per point 3 values (no index) then `" %20.10E %20.10E\n"` (`+0x1b8`, `+0x1c0`); other shapes see below.

**Shapes 3 and 11–13 are never written by FRCWIN.** The KEY reader cannot create them (shape g other than 1/2 → status 2), but a DB can. In 3D, `FUN_1403c8560` maps `win+0x210` 1 → 1, 2 → 2 and returns **3** for any other value before printing anything for that window. The pair's earlier records (CNTACT, FRCFAC, IHTCOF, earlier windows) are already written; nothing more of the pair and no later pair follows (`FUN_1403c9ad0` stops at the first non-zero status), and Inter-Object Data is the last section of `FUN_1401c5af0`. KFWRIT (`FUN_1401c5810`) closes the file as it is and returns 3. In 2D the shape is not checked; the geometry form follows the window's own dimension (`win+0x898 == 2` → point list).

The shape 3 and 11–13 branches of `FUN_1402f4890` are reached through the other callers of the geometry writer (`FUN_1402a9180`, `FUN_1402ab880`, called from the object writer `FUN_14029e250`). For reference:
* shape 3: `"%d\n"` point count (`FUN_1400034d0(win+0x220)`: size of the double vector at `+0x220`→`+0x10`, / 3), then per point `" %7i %20.10E %20.10E %20.10E\n"` (1-based index, x, y, z; always indexed); then `"%d\n"` face count (`FUN_140003500`: int vector at `+0x220`→`+0x18`, 4 ints per face), then per face `" %7i %7i %7i %7i %7i\n"` (1-based index, four node numbers).
* shapes 11, 12, 13: `"%d\n"` (`win+0x178`), then per point `" %7i"` index (only when the mode argument is 0) and `" %20.10E"` ×3 + `"\n"` from `win+0x180`.

## Notes

The 8-point window of shape 1 is a hexahedron (`FUN_140267760` builds six faces from point pairs 7/2, 8/1, 8/3, 5/2, 7/5, 3/1); shape 2 is two points plus two scalars (a cylinder-like window). Exact geometric meaning belongs to the window module and is not needed for KEY I/O.

## Helpers

`FUN_140413900`, `FUN_140413a50`, `FUN_140269bc0`, `FUN_14025d1a0`, `FUN_14025c350`, `FUN_140414c20`, `FUN_14041b7d0`, `FUN_14041b6a0`, `FUN_140415680`, `FUN_140263cb0`, `FUN_140267760`, `FUN_140415880`, `FUN_140413fb0`, `FUN_140414a40`, `FUN_14041a800`, `FUN_1402f4890`, `FUN_1402f2e80`, `FUN_1403c8560`. Shared mechanics: [_common.md](_common.md).

## Corpus

Checked with `tools/check_group13_grammar.py`: 22 occurrences in 6 corpus files consumed exactly up to the next keyword/comment; e.g. `key/3D/EN/Extrusion/ALE/European_Benchmark_2005_ALE.KEY`.
