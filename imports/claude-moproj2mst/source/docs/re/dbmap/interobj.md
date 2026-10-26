# Inter-object data (keyword group 3) <-> .DB, and the shared window table

Class: inter-object entry `E` (0x3a0 bytes, ctor `FUN_1401f46e0`), kept in the list at `m+0x13c8`
(append `FUN_1401faaa0`, find by (master, slave) `FUN_1401facf0`, get n-th `FUN_1401fac90`,
count `FUN_1400015d0`; next pointer `E+0x390`, model pointer `E+0x398`). Offsets below are byte offsets
`E+x`. DB storage: real region **LLBRT** (A slot 8, index global `R_LLBRT` = 7), one record of
`W = AWIDTH(LLBRT)` reals (GENDB writes W = 50) per **ordered pair of boundaries**, plus functions in
IIFNC/LLFNC and windows in **LLWIN** (A slot 10, W = 20) / **LLWRZ** (A slot 11, W = dimension).
Word indexes are **0-based** inside a record. `r[k]` = LLBRT record word k (real64).
"2D"/"3D" = `FUN_1400015e0(m)` (`+0x1c == 2`) / `FUN_1400015f0(m)` (`+0x1c == 3`) **[fact]**.

Sources: DBREAD `loader_ann.c`, GENDB `gendb_ann.c`, KEY writer `kfw_ann.c`, KEY reader
`FUN_140388fb0` (`out/kw13/grp3.c`, switch on `id-0x110`). Keyword ids printed via `FUN_1402f7720(id)`:
0x10f CNTACT, 0x110 FRCFAC, 0x111 IHTCOF, 0x112 SEPRES, 0x113 INTRST, 0x114 FRCROT, 0x115 SEPDEN,
0x116 FRCWIN, 0x117 CNTMTH, 0x118 WMODEL, 0x119 RGDCNT (= registry ids 271-281). All rows are
**[fact]** unless marked **[inferred]**. Constants read from the binary: `DAT_140509c90` = 1.0,
`DAT_140509fb8` = 2.0, `DAT_14051f1c8` = 3.0, `DAT_14051f248` = 4.0, `DAT_140522f48..` = 5.0, 6.0, 7.0,
8.0, 9.0, `DAT_140522fe0` = 11.0, `DAT_14051bed8` = 0.0.

## 1. Read and write paths

### 1.1 Record addressing (both directions)

Records are indexed by **boundary**, not by object. `N` = total number of boundaries:
DBREAD `m+0x13f0` = IISIM[24] (3D) / IISIM[31] (2D) (`FUN_1401cf4a0`: 3D branch reads byte 0x60, 2D branch 0x7c; checked on COG step 25: IISIM[24] = 7 = LLBND length / 200); GENDB `m+0x80` (also sizes LLBND).
Object o owns boundaries `obj+0x3588 .. obj+0x358c` (IIOBJ words 8, 9; count `obj+0x888` =
last - first + 1, set in `FUN_14016caf0`).

    rec(bm, bs) = A[start(LLBRT) - 1 + W * ((bs - 1) * N + (bm - 1))]     bm = master boundary, bs = slave boundary

Region length = N^2 * W (`FUN_1401c1980`/`FUN_1401c2d10`: start(slot 9) = start(slot 8) + N*N*W) **[fact]**.

### 1.2 Read (DBREAD) — `FUN_1401d5c10` ReadInterObjInfo

| Step | Function | What |
|---|---|---|
| 1 | `FUN_1401d5c10` | for every ordered pair (i, j) of objects with `obj+0xc != 7` (7 = empty slot, IIOBJ word 0 = 0), **including i == j**: append a new E (`FUN_1401faaa0`), E+0x168 = i, E+0x16c = j (object numbers `obj+8`) |
| 2 | same | for bm over i's boundaries (outer), bs over j's boundaries (inner): if `bm,bs` outside 1..N print "ERROR in _DB_STEP_DATA::ReadInterObjInfoFromDB()!"; else **if E+0x170 == 0** call `FUN_1401f58a0(E, rec(bm,bs), IIFNC, LLFNC, LLWIN, W_LLWIN, LLWRZ)`. So the first boundary pair whose word 0 is non-zero wins; later pairs are skipped |
| 3 | `FUN_1401f58a0` | decodes one record (§2); two complete branches 2D / 3D; functions via `FUN_1401309b0`; windows via `FUN_1402694a0` (append to list `E+0x188`) + `FUN_14025de80` (§6) |

No step-version test in `FUN_1401d5c10`/`FUN_1401f58a0` and none at the call site in `FUN_1401cc420`;
older steps go through the address-table remap only **[fact]**. Entries are created for every pair even
when no record is non-zero (then E+0x170 = 0).

### 1.3 Write (GENDB) — `FUN_1401c4ea0` -> `FUN_1401fa4f0`

| Step | Function | What |
|---|---|---|
| 0 | `FUN_1401beb90` -> `FUN_1401fa170` -> `FUN_1401f49c0` per pair i <= j | counts function slots into `m+0x88` / LLFNC length `m+0x8c`, windows into `m+0xc0` / LLWRZ rows `m+0xc4` (`FUN_14025d3b0`); sets `m+0xe4` = 1 if any WMODEL is set (`FUN_1401fb1e0`) |
| 0' | `FUN_1401bff40` -> `FUN_1401fa310` -> `FUN_1401f4ce0` | check; if > 1 object and no relation defined: "WARNING: No Inter-Object Relation Defined" |
| 1 | `FUN_1401c03f0` | widths: W_LLBRT = 50, W_LLWIN = 20, W_LLWRZ = 2 (2D) / 3 (3D) |
| 2 | `FUN_1401c4560` | A buffer zero-filled (`FUN_140478d30(buf,0,..)`): every word not written stays 0 |
| 3 | `FUN_1401bcbb0` order | objects `FUN_1401c4d90` (object windows first), then `FUN_1401c4ea0(m, LLBRT, LLFNC, IIFNC, LLWIN, LLWRZ, LLBND)` |
| 4 | `FUN_1401fa4f0(list, LLBRT, &N, IIFNC, LLFNC, LLWIN, LLWRZ, &W_LLBRT, &W_LLWIN, LLBND, &W_LLBND)` | for objects i = 1..n and j = i..n (non-empty), find E(i,j) (`FUN_1401facf0`); only if found and E+0x170 != 0 |
| 5 | `FUN_1401fa840` | choose the boundary pair (bm in i, bs in j): an object with one boundary (`+0x3588 == +0x358c`) uses it; otherwise a 2D geometric search (`FUN_1401667e0` point of the other boundary, `FUN_140166b40` containing/nearest boundary, `FUN_140166980` closest pair, direction by E+0x170) **[fact]**; if nothing is found the index stays 0 and the record address is out of range (no check) **[fact]** |
| 6 | `FUN_1401f7730(E, &i, &j, rec(bm,bs), IIFNC, LLFNC, LLWIN, LLWRZ, &W_LLWIN)` | encodes the record (§2) |
| 7 | `FUN_1401fa4f0` | if bm != bs: **mirror record** rec(bs,bm) = copy of words 1..W-1, word 0 swapped 1.0 <-> 2.0 (0 stays 0) |

Pairs i > j are never written from their own entry; their DB record is the mirror of (j, i).
On read the mirror gives E(j,i) with the master/slave flag swapped — the same rule the KEY reader uses
(`FUN_140388fb0` end: creates E(j,i), flag 1<->2, `FUN_1401f9e80` copy) **[fact]**.

### 1.4 KEY writer — `FUN_1403c9ad0` -> `FUN_1403c8560` per entry

Skips an entry if master or slave is 0, if slave < master, or if master == slave and E+0x170 == 1.
Prints `CNTACT m s code` for every remaining entry, **also when there is no contact (code 0)** and
then stops for that entry. Otherwise: FRCFAC `FUN_1403c9be0`; 2D only FRCROT `FUN_1403caa40`;
IHTCOF `FUN_1403ca830`; FRCWIN per window; SEPRES `FUN_1403cad60`; 2D only SEPDEN `FUN_1403caf20`;
INTRST `FUN_1403cb0a0`; 3D only CNTMTH `FUN_1403cbce0`; WMODEL `FUN_1403cb840`; RGDCNT `FUN_1403cbb10`.
Because DBREAD creates entries for all ordered pairs, a KEY export after DBREAD lists `CNTACT i j 0`
for every non-contacting pair i <= j, in creation order (i outer, j inner) **[fact]**.

## 2. LLBRT record layout

| Item | 2D word | 3D word | Model field |
|---|---|---|---|
| contact flag (master/slave) | 0 | 0 | E+0x170 |
| 3D contact option | — | 1 | E+0x174 |
| friction type | 2 | 2 | E+0x0 |
| friction 1: value / function no / code | 3 / 4 / 5 | 3 / 4 / 5 | E+0x20 / desc E+0x18 / E+0x10 |
| friction 1: extra 3 words | 0x18-0x1a | 0xe-0x10 | E+0x34..0x3c (ints) or pairs E+0x108..0x130 |
| IHTCOF value / function no / kind or -ref | 6 / 7 / 8 | 6 / 7 / 8 | E+0x1c0 / desc E+0x1b8 / E+0x1c8, E+0x230 |
| SEPRES code / value | 0x10 / 0x11 | 0xc / 0xd | E+0x178 / E+0x180 |
| FRCROT kind flag / value or fn | 0xd / 0xe | — | E+0x244, E+0x250 / E+0x248, desc E+0x240 |
| window-present flag (write only) | 0xf | — | — |
| SEPDEN | 0x12 | — | E+0x238 |
| INTRST kind / value or fn | 0x13 / 0x14 | 0x13 / 0x14 | E+0x2bc, E+0x2c8 / E+0x2c0, desc E+0x2b8 |
| first / last LLWIN record (1-based) | 0x16 / 0x17 | 0x24 / 0x25 | list E+0x188 |
| RGDCNT 3 ints + real | 0x1b-0x1d, 0x1e | 0x26-0x28, 0x29 | E+0x37c, 0x380, 0x384, 0x388 |
| CNTMTH flag / int / real | — | 0x1f / 0x20 / 0x21 | E+0x330 / 0x334 / 0x338 |
| WMODEL code / function no | 0x1f / 0x20 | 0x22 / 0x23 | E+0x340, data E+0x348..0x378 |
| friction 2 value / fn / code | 0x21 / 0x22 / 0x23 | 0x2a / 0x2b / 0x2c | E+0x98 / desc E+0x90 / E+0x14 |
| friction 2 extra 3 words | 0x24-0x26 | 0x2d-0x2f | E+0xac..0xb4 or pairs E+0x138..0x160 |
| unused (never read or written) | 1, 9-0xc, 0x15, 0x27-0x31 | 9-0xb, 0x11, 0x12, 0x15-0x1e, 0x30, 0x31 | — |

(`FUN_1401f58a0` read, `FUN_1401f7730` write.)

## 3. Keyword table

`fn(k)` = function number k: start `IIFNC[2k-2]`, length `IIFNC[2k] - IIFNC[2k-2]`, IIFNC[2k-1] = type
word (0 for every inter-object function), data in LLFNC from the start (1-based) — decoded by the shared
`FUN_1401309b0`, allocated on write by `FUN_1401bb3d0` (counter `m+0x10c`, writes k into the record
word) and filled by `FUN_140131360`. Function numbers are **computed** on write (allocation order),
never copied.

| Keyword | Model field | DB location (2D / 3D) | Read transform (`FUN_1401f58a0`) | Write transform (`FUN_1401f7730`) | Notes |
|---|---|---|---|---|---|
| CNTACT | E+0x168 master, E+0x16c slave (object numbers), E+0x170 flag 0/1/2, E+0x174 (3D) | pair -> record index (§1.1); r[0]; 3D also r[1] | (int)r[0]: 0 -> 0 (nothing else read), 1 -> 1, 2 -> 2; 3D: E+0x174 = (r[1] == 0) ? 1 : 0 | r[0] = 0/1.0/2.0; 3D r[1] = (E+0x174 == 0) ? 1.0 : 0 | KEY code: 2D = flag; 3D = 1/2 when E+0x174 = 1, 3/4 when 0 (flag 1/2) (`FUN_1403c8560`, KEY reader same). Master/slave numbers are not stored: they come from which boundary pair the record sits at. Mirror record swaps 1/2 |
| FRCFAC | E+0x0 type 1-4; set 1: neg flag byte E+0x4, ref E+0x8, code E+0x10, value E+0x20, function desc E+0x18 (nvar E+0x1c, kinds E+0x28.., dims E+0x34..0x3c), model pairs E+0x108/0x110, 0x118/0x120, 0x128/0x130; set 2 (type 4 only): byte E+0x5, E+0xc, E+0x14, E+0x98, desc E+0x90, dims E+0xac..0xb4, pairs E+0x138..0x160 | type r[2]; set 1 r[3] value, r[4] fn, r[5] code, extras 2D r[0x18..0x1a] / 3D r[0xe..0x10]; set 2 2D r[0x21..0x26] / 3D r[0x2a..0x2f] | type = (int)r[2] (1-4, 0 leaves default). code = (int)r[5]: < 0 -> neg flag, ref = -code, value = r[3]; 0 -> constant r[3]; 1/2/3/4 -> 1-variable function fn((int)r[4]) with variable kind 1/4/3/21; 5 -> 3-variable function (kinds 4, 3, 21), dims = (int)extras; 6/7/8 -> two reals extras[0], extras[1] into the matching pair (no function) | mirror: r[2] = type as real; r[5] = code (or -ref); code 0 -> r[3] = value; 5 -> extras = dims; 6-8 -> extras = pair; extras zeroed first; if nvar != 0 allocate fn and write r[4]. Negative: r[5] = -ref, r[3] = value | KEY FRCFAC: types 1-3 one line `m s type code value|npts|...`, type 4 header + two sub-lines (`FUN_1403c9be0`). Meaning of the negative reference and of types 1-4 not recovered (type 4 is the only one with a 2nd coefficient set **[fact]**; "hybrid" **[inferred]**) |
| IHTCOF | E+0x1bc (0 const / 1 function), E+0x1c0 value, desc E+0x1b8 (kind E+0x1c8, npts E+0x1d4), neg ref E+0x230 + flag E+0x234 | r[6] value, r[7] fn, r[8] kind or -ref | r[7] == 0 -> constant r[6]; else function fn(r[7]) with kind r[8]: 1 -> 1, 2 -> 4, 3 -> 3. Then if r[8] < 0: E+0x230 = -r[8], E+0x234 = 1 | const: r[7] = 0, r[6] = value; function: r[8] = 1.0 / 2.0 / 3.0 for kind 1 / 4 / 3, r[7] = fn; if E+0x234: r[8] = -E+0x230 (overrides) | KEY codes 0 const, 1 kind 1, 2 kind 4, negative = ref with value 0 (`FUN_1403ca830`). Kind 3 exists in the DB but the KEY writer returns error 3 for it **[fact]** |
| SEPRES | E+0x178 code 0/1/2, E+0x180 value | 2D r[0x10], r[0x11]; 3D r[0xc], r[0xd] | (int) code, value copied | same, code as real | KEY `SEPRES m s code value` (`FUN_1403cad60`) |
| SEPDEN | E+0x238 | 2D r[0x12]; 3D none | copy | copy | 2D only in KEY (`FUN_1403caf20`), DB and reader |
| INTRST | E+0x2bc (0/1), E+0x2c0 value, desc E+0x2b8 (kind E+0x2c8, npts E+0x2d4) | r[0x13] mode, r[0x14] value or fn (2D and 3D) | r[0x13] == 0 -> constant r[0x14]; 1 -> function fn(r[0x14]) kind 3; 2 -> kind 4 | const: r[0x13] = 0, r[0x14] = value; function: r[0x13] = 1.0 (kind 3) / 2.0 (kind 4), r[0x14] = fn | KEY codes 1/2 = kind 3/4 (`FUN_1403cb0a0`) |
| FRCROT | E+0x244 (0/1), E+0x248 value, desc E+0x240 (kind E+0x250, npts E+0x25c) | 2D r[0xd] mode, r[0xe] value or fn; 3D none | r[0xd] == 0 -> constant r[0xe]; else function fn(r[0xe]) kind 1 | const: r[0xd] = 0, r[0xe] = value; function: r[0xd] = 1.0 **only for kind 1**, r[0xe] = fn | **Lossy**: a kind-4 function (KEY code 2) leaves r[0xd] = 0, so it reads back as a constant equal to the function number **[fact]**. 2D only |
| FRCWIN | windows in list E+0x188 (usage 1) | range 2D r[0x16..0x17] / 3D r[0x24..0x25]; records in LLWIN/LLWRZ (§6) | n = r[last] - r[first] + 1 if (int)r[first] != 0; window k: E+0x170 = k+1, read LLWIN record r[first]-1+k | windows written at the global window cursor `m+0x134` / LLWRZ cursor `m+0x138`; range = (old cursor, new cursor - 1), or 0/0 if none; 2D r[0xf] = 1.0 when windows exist | One `FRCWIN` line set per window (`FUN_1403c8560`) |
| CNTMTH | E+0x330 flag, E+0x334 int, E+0x338 real | 3D r[0x1f], r[0x20], r[0x21]; 2D none | flag = r[0x1f] != 0; if set int = (int)r[0x20], real = r[0x21] | r[0x1f] = 0 or 1.0, then int, real | 3D only (`FUN_1403cbce0`) |
| WMODEL | E+0x340 mode 0-3; mode 1: 4 reals E+0x348..0x360; mode 2: 2 reals E+0x368, 0x370; mode 3: ref E+0x378 | code 2D r[0x1f] / 3D r[0x22]; fn 2D r[0x20] / 3D r[0x23] | (int)code: 0 -> 0; < 0 -> mode 3, ref = -code; 1 -> 4 reals copied from LLFNC[IIFNC[2*fn-2]-1 ..]; 2 -> 2 reals likewise | mode 1/2: code 1.0/2.0, allocate an LLFNC slot of 4 / 2 words (type word 0), write fn, memcpy the reals; mode 3: code = -ref | The constants travel as a pseudo-function in LLFNC **[fact]**. KEY `WMODEL m s 1 4` + 4 reals, `m s 2 2` + 2 reals, `m s -ref` (`FUN_1403cb840`) |
| RGDCNT | E+0x37c, 0x380, 0x384 (ints), E+0x388 (real) | 2D r[0x1b..0x1e]; 3D r[0x26..0x29] | (int) casts, copy | (double) casts, copy | KEY printed only if E+0x37c > 0; KEY reader accepts first int 0..4 (`FUN_140388fb0` case 9) |

## 4. KEY-only and unreached keywords

All 11 group-3 keywords are stored in LLBRT (or its windows/functions); none is KEY-only and none is
unreached. Dimension limits are the same in KEY, DB read and DB write: FRCROT and SEPDEN 2D only,
CNTMTH 3D only (§3) **[fact]**.

Round-trip losses seen in code (KEY -> DB -> KEY):
* FRCROT kind 4 -> constant (above).
* IHTCOF kind 3 can be read from the DB but not written to KEY.
* FRCWIN attach code 2 (window field +0x368 = 1) is written as 0 and reads back as code 1 (§6.3).
* Pair records with E+0x170 = 0 are not written; their records stay 0.

## 5. DB words without a keyword (computed / internal)

| Word | Meaning |
|---|---|
| record position | boundary pair (bm, bs); master/slave object numbers are recovered from the boundary ranges of IIOBJ |
| r[0] in the mirror record | the master/slave flag as seen from the other object (computed: 1 <-> 2) |
| function numbers r[4], r[7], r[0xe] (2D), r[0x14], r[0x20]/r[0x23], r[0x22]/r[0x2b] | allocation order of the function table (computed on write) |
| window range | LLWIN record numbers (computed from the global window cursor) |
| 2D r[0xf] | 1.0 when the pair has windows; written only, never read |
| LLWIN w[1], w[2] | LLWRZ row range of the window geometry (computed) |

## 6. Windows (shared: LLWIN, LLWRZ, class LLWINDEF_DATA)

Window object `V` (vtable `LLWINDEF_DATA::vftable`, ctor `FUN_14025c100`, init `FUN_14025c350`:
V+0x174 = usage, V+0x368 = V+0x36c = 0, V+0x370 = 0, V+0x378 = 0, V+0x838 = 0; V+0x888 = model).
Lists: append `FUN_1402694a0`, get `FUN_140269900`, count `FUN_14019a590`. Read
`FUN_14025de80` (LLWINDEF_DATA::ReadFromDBArrays), write `FUN_140260ea0` (WriteToDBArrays), friction
part of the write `FUN_140262250` (storeWindefFric). `w[k]` = LLWIN record word k (real64, record width
W_LLWIN, written as 20); `P(p)` = LLWRZ row p (1-based), **stride fixed 3 in 3D and 2 in 2D** (the
reader ignores the stored LLWRZ width; GENDB writes width = dimension).

### 6.1 Usage type (w[0])

| w[0] | V+0x174 | Used for | Data words |
|---|---|---|---|
| 1 | 1 | inter-object friction window (FRCWIN), also LOCDEF-style friction block | w[6..0x13] (§6.4) |
| 2 | 2 | mesh window: V+0x370 = w[6] (element size; KEY MGUSER `FUN_1402a9180`, list obj+0x2d50, LLOBJ words 25/26 3D, 44/45 2D) | w[6] |
| 3 | 3 | object window list (FUN_140195ff0) — heat/atom type BC window **[inferred]** | w[6..0x11] |
| 4 | 4 | idem **[inferred]** | w[6..0xb] |
| 5 | 5 | LOCTMP (`FUN_1402ae590`, list obj+0x2f40, LLOBJ 75/76) | w[1], w[6..0x11] |
| 6 | 6 | LOCATM (`FUN_1402aeb40`, list obj+0x2f70, LLOBJ 77/78) | w[1], w[6..0xd] |
| 7 | 7 | LOCDEF (`FUN_1402aef30`, list obj+0x2fa0, LLOBJ 79/80) | w[1], w[6..0xd] |
| 8 | 8 | object window list **[inferred]** | w[6..9] |
| 9 | 10 | no data | — |
| other | — | "ERROR in LLWINDEF_DATA::ReadFromDBArrays(): not implemented windef type" | — |

Object window ranges (first/last LLWIN record, LLOBJ real words, `FUN_140195ff0`): list +0x48:
3D [82,83] (version > 9999), 2D [81,82] (version > 8999); +0x78 [60,61]; +0xa8 [62,63]; +0x168 [88,89]
(version > 10999); +0xd8 [75,76]; +0x108 [77,78]; +0x138 [79,80] (lists relative to obj+0x2e68).
The usage is taken from w[0], not from the list (the caller's preset is overwritten) **[fact]**.

### 6.2 Geometry (usage not 5, 6, 7)

| Item | 3D | 2D |
|---|---|---|
| LLWRZ range | w[1] first row, w[2] last row | same |
| shape (V+0x210) | w[0xe]: 0 or 1 -> 1 box, needs w[2]-w[1] = 7 (8 points); 2 -> 2 cylinder, needs w[2]-w[1] = 2: V+0x180 gets 2 points P(w[1]), P(w[1]+1), V+0x1b8, V+0x1c0 = x, y of P(w[1]+2); 3 -> 3 surface mesh: w[7] points, w[8] quads, points P(w[1]..), then 4·w[8] node numbers stored as reals packed right after the points (ceil(4·w[8]/3) rows); 11, 12, 13 -> 11 point list, n = w[2]-w[1]+1, V+0x378 = 1 if w[7] is 0 or 1 | polyline of n = w[2]-w[1]+1 points (x, y) |
| shape errors | "cubic window nNoPoints != 8", "cylindrical window nNoPoints != 3": window left half-read | — |
| vector V+0x350, 0x358, 0x360 | w[3], w[4], w[5] | V+0x350, 0x358 = w[4], w[5] |
| attach mode V+0x368 / V+0x36c | w[0xf] | w[3] |
| derived | box/cylinder faces built by `FUN_140267760` (computed, not stored) | — |

Write (`FUN_140260ea0`): w[1] = LLWRZ cursor `m+0x138` (`FUN_14019a5e0`), w[2] = w[1] - 1 + rows;
rows = 8 box, 3 cylinder, nPts + ceil(4·nQuads/3) mesh, V+0x178 points for shapes 11-13 (w[0xe] = 11.0,
w[7] = 1.0); unknown shape -> w[0xe] = 0 + "not implemented WindefShape type". 3D with V+0x210 == 0 and
usage 3 or 4: only w[0] is written and the window counter is **not** advanced (next window overwrites
it) **[fact]**. Cursors are updated with `FUN_14019a5c0` (`m+0x134` windows) / `FUN_14019a600`
(`m+0x138` rows) after each pair/object.

### 6.3 Attach mode (V+0x368)

| DB word (3D w[0xf], 2D w[3]) | V+0x368, V+0x36c | KEY code (`FUN_1403c8560`) |
|---|---|---|
| 1.0 | 0 | 1 |
| 0 (also what is written for V+0x368 = 1) | unchanged = 0 (ctor) | 1 |
| -n | 2, n | -n |
| (V+0x368 = 1 in memory) | written as nothing | 2 — **lost** on DB round trip |

2D only: usage 1 with V+0x368 = 2 and V+0x36c = 0 writes w[3] = -(object id), the id being the slave
when E+0x170 = 1, else the master (`FUN_1401f7730` passes it as `param_10`). The KEY reader stores
attach code 0 as V+0x368 = 2, V+0x36c = -(other object) (`FUN_140388fb0` case 6). V+0x350.. and the
attach mode describe window motion (velocity, follow object) **[inferred]**.

### 6.4 Usage-specific data (read `FUN_14025de80`, write `FUN_140260ea0`)

Friction block (usage 1; usage 7 uses the same writer `FUN_140262250` with other word numbers):

| Item | usage 1 | usage 7 | Field |
|---|---|---|---|
| friction type 1-4 | w[6] | w[0xd] (0 if not 1-3) | V+0x8 |
| value | w[7] | w[9] | V+0x28 |
| function no | w[8] | w[9] | desc V+0x20 |
| code (0-8, < 0 = ref) | w[9] | w[8] (0-5, no negative) | V+0x18, V+0xc, V+0x10 |
| extras (dims / pairs) | w[10..12] | w[10..12] (code 5 dims only) | V+0x3c..0x44, V+0x110..0x138 |
| type 4 second set: value or fn / code / extras | 3D w[0xd] / w[0x10] / w[0x11..0x13]; 2D w[0xd] / w[0xe] / w[0xf..0x11] | — | V+0xa0, desc V+0x98, V+0x1c, V+0xb4.., V+0x140..0x168 |

Code decoding is identical to FRCFAC (§3): 0 constant, 1-4 one-variable function of kind 1/4/3/21,
5 three-variable (kinds 4, 3, 21; dims from the extras), 6-8 two reals.

Other usages (mode word, then value or function number; "fn" = function via `FUN_1401309b0`):

| Usage | Words -> fields |
|---|---|
| 3 | w[6]: 0 const w[7] -> V+0x388, else fn(w[7]) kind 1 (V+0x380); w[8]: 0 const w[9] -> V+0x400, 1 fn kind 1, 2 fn kind 3, 3 two-variable fn (kinds 1, 3; dims w[10], w[11]) (desc V+0x3f8); w[0xc]: 0 const w[0xd] -> V+0x478, 1 fn kind 1, 2 fn kind 3 (V+0x470); w[0x10]: 0 const w[0x11] -> V+0x4f0, 1 fn kind 1 (V+0x4e8) |
| 4 | w[6]: 0 const w[7] -> V+0x5e8, 1 fn kind 1, 2 fn kind 3 (V+0x5e0); w[8]: 0 const w[9] -> V+0x660, 1 fn kind 1, 2 fn kind 12, 3 two-variable (kinds 1, 12; dims w[10], w[11]) (V+0x658) |
| 5 (LOCTMP) | as usage 3 for w[6..0xd]; w[0xe]: 0 const w[0xf] -> V+0x4f0, else fn (V+0x4e8); w[0x10] >= 1 -> V+0x560 = 1; w[0x10]: 1 const w[0x11] -> V+0x570, 2 fn kind 1, 3 fn kind 4 (V+0x568); V+0x838 = w[1] |
| 6 (LOCATM) | as usage 4 for w[6..0xb]; w[0xc]: 0 const w[0xd] -> V+0x6d8, else fn kind 1 (V+0x6d0); V+0x838 = w[1] |
| 7 (LOCDEF) | w[6]: 0 const w[7] -> V+0x750, else fn kind 1 (V+0x748); friction block above; V+0x838 = w[1] |
| 8 | w[6]: 0 const w[7] -> V+0x750, else fn (V+0x748); w[8]: 0 const w[9] -> V+0x7c8, else fn (V+0x7c0) |

Usages 5, 6, 7 write w[1] = V+0x838, w[2] = 0, w[3] = 1.0, w[4] = w[5] = 0 and have no LLWRZ rows
**[fact]**. Writer asymmetries: usage 3 writes w[0x10] = 1.0 only for kind 1 (else 0, i.e. reads back
as constant); usage 5 w[0x10] = 1.0 const, 2.0 kind 1, 3.0 kind 4; mode words for functions are
written as 1.0 (`FUN_140260ea0`) **[fact]**. The usage-3/4/8 sub-writers `FUN_140262cf0`,
`FUN_140263040`, `FUN_140262c00` were not read in detail **[unreached detail]**; their word numbers above
come from the reader.

## 7. What `deformdb` needs for this class

* Total boundary count N (IISIM[24] 3D / IISIM[31] 2D) and per-object boundary ranges (IIOBJ words 8, 9).
* An LLBRT pair accessor `rec(bm, bs)` and an object-pair view that takes, per ordered object pair,
  the first boundary pair (master boundaries outer, slave inner) with r[0] != 0; 2D and 3D layouts of §2.
* Shared function table reader (IIFNC pairs + LLFNC payload, the `FUN_1401309b0` rules) — inter-object
  uses type word 0, and WMODEL stores plain constants in an LLFNC slot.
* Window decoder for LLWIN/LLWRZ (§6), shared with objects; derived box/cylinder faces are not stored.
* Writer: zeroed LLBRT of N^2·W, mirror record with flag 1<->2, function-number and window/row cursors
  continuing after the objects' windows, and a boundary choice for multi-boundary objects (keep the
  pair found on read instead of DEFORM's geometric search).
* KEY parity: emit `CNTACT i j 0` for every non-contacting pair i <= j of non-empty objects.

## 8. Open questions

* Meaning of the negative FRCFAC / IHTCOF / WMODEL references, of FRCFAC types 1-4 and of the
  function variable kinds 1, 3, 4, 12, 21.
* Meaning of the 3D contact option r[1] (E+0x174) and of the RGDCNT ints.
* Usages 3, 4, 8 of the window table: which object keywords they carry (object class doc).
