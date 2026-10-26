# DB map: materials (LLMPR) and inter-material / phase transformation (LLTFM)

Scope: keyword group 4 "material" (42 keywords, ids 92-133) and group 5 "phase transformation"
(10 keywords, ids 134-143) of DEF_PRE_64.exe v11.0.2. Everything below comes from decompiled code.
**[fact]** = read in code at the cited address; **[inferred]** = deduction.
Word offsets are **0-based inside the record**; "w23" = word 23. All record words are reals (doubles);
"int" means the real holds an integer (written `(double)i`, read `(int)x`, i.e. truncation).
Material object offsets are **bytes** in `LLMAT_DATA` (size 0x2218); the reader indexes it as `int*`,
so reader index k = byte 4k.

## 1. Read and write paths

### 1.1 Read (DBREAD)

| Step | Function | What it does |
|---|---|---|
| 1 | `FUN_1401cff20` ReadMatlInfo | `FUN_1401c7d40(dim)` selects region index tables; base = `A + (ASTART(LLMPR)-1)`; for i = 0..nMat-1 (nMat = `m+0x1320`, from IISIM) appends a new material (`FUN_14029c060`, ctor `FUN_14028c6f0`) and calls `FUN_14028f300(mat, base + i*AWIDTH(LLMPR), IIFNC*, LLFNC*, stepVersion)`; remembers `num[i] = mat+0` **[fact]** |
| 2 | same, 2nd loop | for every material with `+0x1cc8 == 1` (mixture master, MSTMTR) replaces each list entry e (`+0x1cd0[k]`) by `num[e-1]`: the DB stores **DB slot numbers**, the model keeps **material numbers** **[fact]** |
| 3 | `FUN_1401d6250` | for each master, each listed component material gets `+0x1cc8 = 2` and its own list freed **[fact]** |
| 4 | `FUN_1401d0180` ReadInterMatlInfo | base = `A + ASTART(LLTFM)-1`; for i (from) and j (to) in 0..nMat-1 where both materials exist (`FUN_1401d7090` = i-th material in list order): new object (`FUN_140339df0`, ctor `FUN_140333f60`, size 0x780), `+0 = num(from)`, `+4 = num(to)`, `FUN_140336740(obj, base + AWIDTH(LLTFM)*(j*nMat + i), IIFNC*, LLFNC*)` **[fact]**. nMat² objects are created, also for empty records |

IIFNC* = `m+0x2220` = `&MAR[MARSTART(IIFNC)-1]`, LLFNC* = `m+0x2228` = `&A[ASTART(LLFNC)-1]` (`loader_ann.c` l.13815) **[fact]**.
Version branches inside `FUN_14028f300`: EXPAND reference temperature w88 only if (3D and v ≥ 5000) or
(2D and v ≥ 8000); words 66, 150-162 only if v ≥ 11000 **[fact]**. 2D/3D changes word positions of ANISO and
PMEAB (§2.3) **[fact]**. No other version branch. Older steps reach this code after the slot remap of
spec §6.2, with the stored width.

### 1.2 Write (GENDB)

| Step | Function | What it does |
|---|---|---|
| 0 | `FUN_1401c03f0` | widths: W(LLMPR) = 500, W(LLTFM) = 50, W(LLFNC) = 2, W(IIFNC) = 0 (2D and 3D) **[fact]** |
| 0 | `FUN_1401c1980` (3D) | start(LLMPR) = 1 + len(LLFNC) (`m+0x8c`); len(LLMPR) = NMAT·500; len(LLTFM) = NMAT²·50; len(IIFNC) = 2 + 2·NFN (`m+0x88`) **[fact]** |
| 0 | `FUN_1401bc230` → `FUN_1401be510(m, m+0x78)` → `FUN_14029c490`, `FUN_14029c5c0` | mixture components inherit the master's "used" flag (`+4`) and group flags (`+8`); used materials get dense slots 1..NMAT in increasing material number, stored in `mat+4`; NMAT → `m+0x78` **[fact]**. "Used" is set by objects that reference the material (`FUN_14015ff30`) **[inferred: element material groups]** |
| 1 | `FUN_1401c49b0` → `FUN_14029bdf0(list, LLMPR base, IIFNC, LLFNC, &W)` | builds number→slot table (`FUN_14029ba10`); for number 1..maxNo, if material exists and `+4 ≠ 0`: `FUN_140294ae0(mat, base + (slot-1)·W, IIFNC, LLFNC, &slot, table)` **[fact]** |
| 2 | `FUN_1401c4fa0` → `FUN_14033a5a0` | for from = 1..maxNo, to = 1..maxNo, both used: record `base + W·((slot_to-1)·NMAT + slot_from-1)` (NMAT = `m+0x78` via `FUN_140199c10`); finds the pair object (`FUN_14033a040`), if present `FUN_140337f50(obj, rec, IIFNC, LLFNC, &counter)` **[fact]** |

Unused materials are not written; their numbers vanish from the DB. Material numbers in w0 can be
non-contiguous; slots are dense **[fact]**. On read, list position = slot **[inferred]** (records are read in order).

### 1.3 KEY writers (names)

`FUN_1403cdca0` (material, skipped if byte `+0x2210` set) calls per-keyword handler2s from the registry
plus inline MSTMTR, HDNRUL, MATDEN (3D only), SIZEMD, TXTURE, and GBENGY `FUN_1403d4400`, GBMOBI
`FUN_1403d44a0`, NUCSIZ `FUN_1403d4550`. `FUN_1403cbf00` (inter-material, skipped if `+0xc == 0`) writes
TTTN, PHASTF inline, then TTTD, PHASLH, PHASVL, TRNSFP, PHEQVF, COARSN, IPSNRG handlers and VARSEL inline **[fact]**.

## 2. Tabular data: property functions in IIFNC / LLFNC

### 2.1 Function object (0x78 bytes, kind code set by `FUN_14012d2a0`)

| Byte | Field |
|---|---|
| 0x00 | kind code (fixes nvar, variable codes and ncomp defaults, `FUN_14012d2a0`) |
| 0x04 | nvar: 0 = constant, 1, 2 or 3 independent variables |
| 0x08 | constant value (nvar 0) |
| 0x10/0x14/0x18 | variable codes of var 1..3 (3 is used for the temperature-like first variable everywhere **[inferred]**; others seen: 1, 6, 7, 8, 9, 0xc, 0x14, 0x17, 0x18, 0x1e, 0x1f, 0x21) |
| 0x1c/0x20/0x24 | n1, n2, n3 (grid sizes) |
| 0x28/0x30/0x38 | grid arrays |
| 0x40 | ncomp (values per grid point, ≥ 1) |
| 0x48 | values (n1·n2·n3·ncomp) |
| 0x54 | owner word copied from IIFNC on read |

### 2.2 IIFNC directory and LLFNC packing

Function k (1-based) **[fact]** (`FUN_1401bb3d0` write, `FUN_1401309b0` read):

* IIFNC[2k-2] = 1-based start in LLFNC; IIFNC[2k-1] = owner (material **slot** for material functions,
  0 for inter-material); IIFNC[2k] = start of k+1, so length = IIFNC[2k] − IIFNC[2k-2]; IIFNC[0] = 1.
* The record word holds k as a real; 0 = no function. The counter is `m+0x10c`, shared by all classes,
  incremented in writer call order **[fact]**.

LLFNC data of one function (`FUN_140131360` write / `FUN_1401309b0` read) **[fact]**:

| Form | Packing |
|---|---|
| nvar 1 | n records `x, y1..y_ncomp`. Reader: if ncomp = 1 then n = length/2, else n comes from the record word. Writer length = 2n (`FUN_140130200`) or n·(ncomp+1) rounded up to even (`FUN_140130230`) |
| nvar 1, kind 0x7c | ncomp values only |
| nvar 0, kind ∈ {0xa7,0xa8,0xaa,0xab,0xad,0xb3,0xb5} | ncomp values |
| nvar 2/3 | [header n1, n2(, n3) as reals — only BURGRS/ALPHA/NDISFM/RECVRY], grid1 (n1), grid2 (n2), [grid3], then n1·n2·(n3)·ncomp values copied as one block (`memcpy`). n1..n3 otherwise come from record words |
| self-describing (`FUN_140131600` / `FUN_140130dc0`) | 10-word header: [0] = 10.0 (magic, checked on read), [1] = nvar; nvar 0: [2] = ncomp, [3] = constant; nvar 1: [2] = var code, [3] = n, [4] = ncomp (n ≤ 0: [4] = constant); nvar 2: [2],[3] var codes, [4],[5] n1,n2, [6] ncomp; data from word 10. Length = data + 10 |

ncomp and the variable codes of most tables are **not stored**: they come from the kind code set by the
constructors (`FUN_14028c6f0`, `FUN_140333f60`) **[fact]**; the DB keeps only a type selector.

### 2.3 Common "property" pattern P(fn, const, type, n1, n2)

Writer `FUN_14029b0d0`, reader inline in `FUN_14028f300` **[fact]**:
nvar 0 → w[fn] = 0, w[const] = value; nvar 1 → w[type] = 0 (var 3), 1 (var 9), 2 (var 0xc);
nvar 2 → w[type] = 3, w[n1], w[n2] (read back as vars (3, 0xc)); then w[fn] = function number.
KEY type = DB type + 1 for nvar ≥ 1 (KEY `FUN_1402f3aa0`: var 3→1, 9→2, 0xc→3, (3,0xc)→4) **[fact]**.
KEY types 6 (var 8) and 7 ((3,4)) have no DB type code **[inferred: lossy]**.

## 3. LLMPR record (W = 500), one per used material

### 3.1 Keyword table, group 4

| KEY (id) | Model field (byte) | DB words | Read / write transform | Notes / cites |
|---|---|---|---|---|
| (number) | `+0` | w0 | int | material number, not slot **[fact]** `FUN_14028f300`, `FUN_140294ae0` |
| MTNAME (92) | `+0xc` char[64] | w2..w9 | 64 bytes copied; read: conditional byte swap by platform code `m+0x748` (`FUN_140324020`, 8 words) and trailing-blank trim (`FUN_1403239f0`); write: strncpy + pad (`FUN_140323bf0`) | **[fact]** |
| FRAE2H (93) | `+0x1ec8` double | w10 | copy | default 0.9 (`FUN_14028cdc0`) **[fact]** |
| FSTRES (94) | `+0x50` type, data `+0x58..0x628` | w11 + w12..w22, w78..w80 | w11 = KEY FSTRES code (see 3.2); internal type = code−1 except code 0 → −1, 2/3 → 1, 7/8 → 5, 9..16 → 6..13, negative → 14 with `+0x660` = −code | **[fact]** both sides |
| YOUNG (95) | fn `+0x670` | P(33, 32, 23, 24, 25) | §2.3 | **[fact]** |
| POISON (96) | fn `+0x6e8` | P(35, 34, 26, 27, 28) | §2.3 | **[fact]** |
| EXPAND (97) | fn `+0x760`; ref. temp `+0x7d8` | P(37, 36, 29, 30, 31); w88 | w88 read only 3D v ≥ 5000 / 2D v ≥ 8000, always written | **[fact]** |
| THRCND (98) | fn `+0x7e0` | P(39, 38, 47, 84, 85) | §2.3 | **[fact]** |
| HEATCP (99) | fn `+0x858`; int `+0x8d0` | P(41, 40, 48, 86, 87); w99 | int | `+0x8d0` is the extra KEY integer (`FUN_1402f40f0`) **[fact]** |
| EMSVTY (100) | fn `+0x8d8` | w46 fn / w45 const | w46 = 0 → const w45; else fn (var 3, 1-D) | **[fact]** |
| TMPSOL (102) | `+0x1ed8` double | w42 | copy | KEY read only (`FUN_140384a50` case 10), not KEY-written **[fact]** |
| TMPLIQ (103) | `+0x1ee0` double | w43 | copy | idem, case 0xb **[fact]** |
| FPERV (104) | fn A `+0x1ef0`, fn B `+0x1f68` | A: w50 flag, w49; B: w148 flag, w147 | flag 0 → w49/w147 = const; flag 1 → 1-D fn, var code 1, fn number in w49/w147 | writer sets flag 1 only if var code = 1 **[fact]** |
| FRCMOD (105) | `+0x1ad0` type, `+0x1ad8..0x1af8`, `+0x1b00`, `+0x1b08`, `+0x1b88`, fn `+0x1b90`, fn `+0x1c08`, 9 reals `+0x1bb0..0x1bf0` | w70 code, w71, w72, w73, w74, w149 | see 3.3 | **[fact]** |
| CREEP (106) | `+0xac8` type, fns `+0xad0 .. +0xe98`, `+0xda0` | w57 code, w58..w60, w61..w64 | see 3.4 | **[fact]** |
| DIFCOE (107) | `+0x950` type, fns `+0x958`, `+0x9d0`, `+0xa48`, `+0xac0` | w53..w56 | w53: 0 → const w56; 1 → 2-D fn n1=w54 n2=w55; 2,3 → 1-D fn n=w54 (writer w55 = ncomp+1); negative → type 4, `+0xac0` = abs; fn number w56 | **[fact]** |
| GRNDAT (108) | `+0x17e8` mode, `+0x1078..0x17e0` | w109..w130 | see 3.5 | **[fact]** |
| BURGRS (110) | fn `+0x18f0` | w139 code, w140 | 0 → const w140; 1/2/3 → 1-D, var 3 / 6 / 0x18; 4/5/6 → 2-D (3,6) / (3,0x18) / (6,0x18); 7 → 3-D (3,6,0x18); 2-D/3-D tables carry an n1,n2(,n3) header in LLFNC; fn number w140 | **[fact]** |
| ALPHA (111) | fn `+0x1968` | w141, w142 | 0 const; 1 var 3; 2 var 8; 3 2-D (3,8) with header | **[fact]** |
| NDISFM (112) | fn `+0x19e0` | w143, w144 | as ALPHA | **[fact]** |
| RECVRY (113) | fn `+0x1a58` | w145, w146 | as ALPHA | **[fact]** |
| HDNPHA (114) | fn `+0xf10` | w89 code, w90, w91, w92 | 0 const w90; 1 var 0xc; 2 var 3; 3 var 9; 4 2-D (3,0xc) n1=w91 n2=w92; fn number w90; DB code = KEY code | **[fact]** |
| ELRST (115) | fn `+0x1cd8` | w51, w52 | 0 const w52; 1 var 3; 2 var 9; other ≠0 → var 3; write: var 3 → 1 else 2 | **[fact]** |
| MSTMTR (116) | `+0x1cc8` role, `+0x1ccc` count, `+0x1cd0` int list | w93 count, w94 fn number of list | list = DB slots as reals (function length = count, not rounded); read maps slots → numbers (§1.1) | **[fact]** |
| HDNTIM (117) | fn `+0xf88` | w95 | 1-D fn; written only if n ≠ 0 | **[fact]** |
| JOMINY (118) | fn `+0x1000` | w96 n1, w97 n2, w98 | written only if n1 > 0 and n2 > 0 | **[fact]** |
| UTSDAT (119) | fn `+0x1b10` | w107 flag, w108 | 0 → const w108; else 1-D var 3, fn number w108 | **[fact]** |
| HDNRUL (120) | `+0x668` int | w104 | write 1.0 if ≠ 0 else 0; read (int)≠0 → 1 | **[fact]** |
| PMEAB (121) | fn `+0x1d50` | w100 type, w101; n1,n2 at w137,w138 (3D) / w131,w132 (2D) | 0 const w101; 1 var 3; 2 var 9; 4 2-D (3,0x14); fn number w101 | **[fact]** |
| PMITT (122) | fn `+0x1dc8` | w102, w103 | 0 const; 1 var 3; 2 var 9 | **[fact]** |
| ANISO (123) | `+0x1e40` model, fn `+0x1e48`, `+0x1ec0` | B..B+5, B = 131 (3D) / 133 (2D) | w[B]: 0 none; 1..4 → fn kind 0x68/0x69/0x67/0x65; negative → 5 with `+0x1ec0` = abs. w[B+2] = ncomp; w[B+1] sub: 1 → var 3, n = w[B+4]; 2 → var 7, n = w[B+3]; 3 → (7,3) n1=w[B+3] n2=w[B+4]; 4 → (3,0x1e) same; fn number w[B+5] | **[fact]** |
| MATDEN (124) | `+0x1ee8` double | w1 | copy (KEY written in 3D only) | **[fact]** |
| MASDEN (125) | fn `+0x1fe0` | P(82, 81, 83, -, -) | type 0/1/2 → var 3/9/0xc. Writer passes word 1 as n1 and n2: a 2-D MASDEN would overwrite w1 (MATDEN) | **[fact]** (`FUN_140294ae0`) |
| COARSE (126) | `+0x20d0` count, fn `+0x20d8` | w65 = fn number of a 20-word header function | header: [0] count, [1] data fn number, [2] 0.0 (const) or 5.0 (2-D (3,8)), [3] const or n1, [4] n2 | **[fact]** (kind 0xad) |
| SIZEMD (127) | `+0x2150` int | w66 | int; read v ≥ 11000 | **[fact]** |
| TXTURE (128) | `+0x2154..0x215c` ints | w150..w152 | int; v ≥ 11000 | **[fact]** |
| DIFBND (129) | fn `+0x2160` | w153 | self-describing fn, always written; v ≥ 11000 | **[fact]** |
| SINTER (130) | `+0x21d8` model, `+0x21e0..0x21f8` | w154, w155..w158 | int + 4 reals; v ≥ 11000 | **[fact]** |
| GBENGY (131) | fn `+0x17f0` | w159 | self-describing, always written; v ≥ 11000 | **[fact]** |
| GBMOBI (132) | `+0x1868`, `+0x1870` | w160, w161 | copy; v ≥ 11000 | **[fact]** |
| NUCSIZ (133) | fn `+0x1878` | w162 | self-describing, always written; v ≥ 11000 | **[fact]** |

### 3.2 FSTRES data words (w11 = code)

| Code | Internal `+0x50` | DB words → model | Writer size |
|---|---|---|---|
| 0 | −1 | none | |
| 1 | 0 | w12..w15 → `+0x58..+0x70` | |
| 2 / 3 | 1, `+0xc8` = 1 / 0 | fn `+0x78`: n1..n3 = w12..w14, fn number w15 | `FUN_14012fff0` = n1+n2+n3+n1·n2·n3 |
| 4 | 2 | w12..w16 → `+0xf0..+0x110` | |
| 5 | 3 | w12..w15 → `+0x118..+0x130` | |
| 6 | 4 | fn `+0x1b0` (code w12, n w14[,w15], fn w18) and fn `+0x138` (code w13, n w16[,w17], fn w19); codes: 0 const (w14 / w16), 1 var 3, 2 var 0xc, 3 2-D (3,0xc) | |
| 7 / 8 | 5, `+0x278` = 1 / 0 | fn `+0x228`: n1..n3 = w12..w14, fn w15 | |
| 9 | 6 | w12..w22 → `+0x2a0..+0x2f0`; w78..w80 → `+0x2f8..+0x308` | |
| 10 | 7 | w12..w17 → `+0x310..+0x338` | |
| 11 | 8 | w12..w16 → `+0x340..+0x360` | |
| 12 | 9 | fn `+0x400`: code w12 (0 const w14, 1 var 3, 2 var 8, 3 2-D (3,8) n1=w14 n2=w15), fn w18 | |
| 13 / 15 | 10 / 12 | fn `+0x480` (n = w12 → `+0x49c`, fn w15), fn `+0x4f8` (ncomp = w13, fn w16), fn `+0x570` (ncomp = w14, fn w17) | n·(ncomp+1), even |
| 14 | 11 | w12..w15 → `+0x368..+0x380`; w16 = 0 → const w17 (`+0x390`), else self-describing fn `+0x388` number w16 | |
| 16 | 13 | fn `+0x5e8`: n1..n3 = w12..w14, ncomp = w15, fn w16 | `FUN_140130090` |
| < 0 | 14 | `+0x660` = −code (user routine) | |

All **[fact]** (`FUN_14028f300` switch, `FUN_140294ae0` switch on type+1, KEY `FUN_1403d0c90`).

### 3.3 FRCMOD

Write: `FUN_14028e030` → `FUN_14028e140` gives (code, p0..p4); w70 = code, w74 = p0 = `+0x1b00`,
w71 = p1, w72 = p2, w73 = p3 (always 0), w149 = p4 = `+0x1b88` **[fact]**.

| w70 code | Internal `+0x1ad0` | Extra |
|---|---|---|
| 0, 1, 3, 6, 8, 9, 10, 16 | same | none |
| 2 / 4 / 5 / 15 | 2 / 4 / 5 / 15 | w71 → `+0x1ad8` / `+0x1ae0` / `+0x1ae8` / `+0x1ae8` |
| 7 | 7 | w71, w72 → `+0x1af0`, `+0x1af8` |
| 11 | 12 | w71 = 9, w72 = fn number of 9 reals → `+0x1bb0,+0x1bb8,+0x1bd0..+0x1bf0,+0x1bc0,+0x1bc8` (`FUN_14028c420` / `FUN_14028c340`) |
| 12 | 13 | w71 = fn number, fn `+0x1b90` (1-D, var 0x1f); KEY prints its n |
| 17 | 17 | w71 = n (`+0x1c24`), w72 = fn number of fn `+0x1c08` |
| < 0 | 11, `+0x1b08` = code | KEY prints the negative code |

w74 → `+0x1b00` and w149 → `+0x1b88` always read **[fact]**.

### 3.4 CREEP (w57)

| w57 | `+0xac8` | Data |
|---|---|---|
| 0 | 0 | none |
| 1..4 | 1..4 | 1-D fn `+0xbc0` / `+0xc38` / `+0xcb0` / `+0xd28`, n = w58, fn number w64; writer size n·(ncomp+1) even, ncomp from kind |
| 5 / 6 | 5 / 6 | 3-D fn `+0xb48` / `+0xad0`, n1..n3 = w58..w60, fn w64 |
| 7 / 8 | 7 / 8 | fn `+0xda8` (n = w58 → `+0xdc4`, fn w61), fn `+0xe20` (ncomp = w59, fn w62), fn `+0xe98` (ncomp = w60, fn w63) |
| < 0 | 9 | `+0xda0` = abs(w57) |

### 3.5 GRNDAT (w109 = mode)

| w109 | `+0x17e8` | Data |
|---|---|---|
| 0 | 0 | none |
| 1 | 1 | one function (number w130) holds 9 consecutive 1-D tables (x, y·ncomp), sizes from words: (type,n) pairs (w110,w111) `+0x1088`; (w112,w113) `+0x1100`; (w114,w115) `+0x1178`, w114 = 0 → flag `+0x1078` = 0; (w116,w117) `+0x11f0`; (w118,w119) `+0x1268`, flag `+0x107c`; (w120,w121) `+0x12e0`; (w122,w123) `+0x1358`, flag `+0x1080`; (w124,w125) `+0x13d0`; (w126,w127) `+0x1448`, flag `+0x1084`. type 1 → var 3, 2 → var 8, 3 → var 0x21. w128 → `+0x14c8`, w129 → `+0x14c0`. Writer length = Σ n·(ncomp+1) (`FUN_14028b990`) |
| 2 | 2 | mesoscale: w110 → `+0x14d0`; w111 = consistency count (`FUN_14028b780`, read only for a warning); w112 = fn number of the main grain record (`FUN_14028a740` / `FUN_14028aa70`: [0] n, [1..n] reals, [23] real, [24] int, [25..35] sub-table descriptors, data from [37]); five lists (count, fn number) at (w113,w114) `+0x16f8`, (w115,w116) `+0x16e0`, (w117,w118) `+0x16c8`, (w119,w120) `+0x1710`, (w121,w122) `+0x1728`; each list function holds one fn number per entry, each pointing to a grain record (`FUN_14028b3b0` / `FUN_14028b4a0`) |
| 4 | 4 | 2-D fn `+0x1740` vars (3,8), n1 = w110, n2 = w111, fn w112 |
| 5 | 5 | w110..w115 → `+0x17b8..+0x17e0` |
| < 0 | −1 | `+0x17ec` = abs |

All **[fact]**. Writer bug: the 9th mode-1 table selects code 2 from `+0x13e0` (8th table's var) instead of
its own `+0x1458` (`FUN_140294ae0`) **[fact]**.

## 4. LLTFM record (W = 50), one per ordered material pair

Record (from slot i, to slot j, 0-based) at `ASTART(LLTFM)-1 + 50·(j·NMAT + i)` **[fact]**.
Object `+0`/`+4` = from/to material numbers (not stored in the record).

| KEY (id) | Model (byte) | DB words | Read / write | Notes |
|---|---|---|---|---|
| PHASTF (134) | `+0x8` type, `+0xc` active | w0 | int; `+0xc` = (w0 ≠ 0); w0 = 0 → nothing else read or written | KEY: from, to, type **[fact]** |
| TTTD (135) | `+0x10`, `+0x14` model, see 4.1 | w1, w6..w13, w49 | | **[fact]** |
| TTTN (136) | `+0x18` char[80] | w18..w27 | read: conditional swap (10 words) + trim; write: blank-filled, strncpy ≤ 80 | KEY writer prints a generated "Transformation from a -> b", not this field **[fact]** `FUN_1403cbf00` |
| PHASLH (137) | fn `+0x478` | w2 type, w3 | 0 const w3; 1 var 3; 2 var 0xc; fn number w3 | **[fact]** |
| PHASVL (138) | fn `+0x4f0` | w4, w5, w28, w29 | 0 const w5; 1 var 3; 2 var 0xc; 3 2-D (3,0xc) n1=w28 n2=w29; fn w5 | **[fact]** |
| TRNSFP (139) | `+0x5e4` flag, fn `+0x5e8` | w14, w15, w16, w31, w32 | w14 ≠ 0 → flag; w15 as PHASVL, const/fn w16, n1,n2 w31,w32 | **[fact]** |
| PHEQVF (140) | `+0x5e0` code, fn `+0x568` | w33..w36 | code 0/4 → const w34; 1/5 var 3; 2/6 var 0xc; 3/7 2-D (3,0xc) n1=w35 n2=w36; fn w34. Written only when type = 2, read always | KEY only when type = 2 **[fact]** |
| COARSN (141) | `+0x660` flag, `+0x668`, `+0x670`, fn `+0x678` | w37, w38, w41, w42, w43 | w37 ≠ 0 → flag; w38: 0 → const w41, 1 → var 3, fn w41; w42, w43 copied | **[fact]** |
| IPSNRG (142) | fn `+0x6f0` | w44..w47 | 0 const w47; 1 var 3; 2 var 0x17; 3 2-D (3,0x17) n1=w45 n2=w46; fn w47 | **[fact]** |
| VARSEL (143) | `+0x768` int | w48 | int | **[fact]** |

Inter-material functions get owner 0 in IIFNC **[fact]** (`FUN_140337f50`).

### 4.1 TTTD (w6 = KEY model code)

| w6 | `+0x14` | Data |
|---|---|---|
| 1 | w13 (0..3) | fn `+0x78` kind 0x2c (3-D; vars (0xc,6,3) for w13 = 0/2, (3,7,0xe) for 1/3); n1..n3 = w9..w11; ncomp = w8 (`+0xb8`, copied to `+0x6c`); fn number w7; w8 = 1 → w12 → `+0x70`; w8 = 2 → last two LLFNC words of the function → `+0x170`, `+0x178`; w49 → `+0xf0` |
| 2 | 4 | fn `+0x1f8`, ncomp w8 → `+0x238`, n1..n3 w9..w11, fn w7 |
| 3 | 5 | function w7 holds 11 reals → `+0x270..+0x2c0`; w8 = 11 |
| 4 | 6 | function w7: 4 reals → `+0x340..+0x358`, then (x,y) table `+0x2c8`, n = (w8−4)/2 |
| 5 | 7 | 6 reals → `+0x360..+0x388`; w8 = 6 |
| 6 | 8 | 4 reals → `+0x390..+0x3a8`; w8 = 4 |
| 7 | 9 | 7 reals → `+0x3b0..+0x3e0`; w8 = 7 |
| 8 | 10 | (x,y) table `+0x3f8`, n = w8/2 |
| 9 | 11 (only if w13 = 0, else the type stays at its default 0) | as code 1 with fn `+0xf8`, ncomp `+0x138`, tail → `+0x3e8`, `+0x3f0` |
| 10..13 | 12..15 | w7 → `+0x1c0` (length), fn `+0x180` number w8 |
| < 0 | 16 | `+0x470` = abs |

w1 → `+0x10` (0/1/2), printed as the 4th integer of the TTTD line **[fact]**. Writer size for codes 1/9
= n1·n2·n3·ncomp + n1+n2+n3 + ncomp (`FUN_140336300`) **[fact]**.

## 5. KEY-only and unreached keywords

| Keyword | Status | Reason |
|---|---|---|
| LTHEAT (101) | not stored | KEY reader `FUN_140384a50` case 9 reads one int into a local and drops it; no KEY write, no DB **[fact]** |
| GRNMDL (109) | not stored | case 0x11: int read into a local and dropped **[fact]** |

No group-4 or group-5 keyword is KEY-written but missing from the DB. TMPSOL and TMPLIQ are the opposite:
in the DB (w42, w43) but never KEY-written **[fact]**.

## 6. DB words without a keyword

| Where | Meaning |
|---|---|
| LLMPR w0 | material number (identifies the record) |
| LLMPR w44 | `+0x1ed0` double, copied both ways, no KEY reader or writer (default 0) **[fact]** |
| LLMPR w73 | FRCMOD p3, always written 0, never read **[fact]** |
| LLMPR w111 (mode 2) | GRNDAT mesoscale element count, a check value only **[fact]** |
| LLMPR w67-69, 75-77, 105, 106, 163-499 | not referenced by reader or writer **[fact]** |
| LLTFM w30 | running count of active pairs with type 1 or 2 (`*param_5 += 1`), written only, never read **[fact]** |
| LLTFM w17, w39, w40 | not referenced **[fact]** |
| IIFNC odd words | function owner (material slot or 0) → fn `+0x54` **[fact]** |

## 7. What deformdb needs for this class

1. Material and pair counts: NMAT from IISIM[22] (3D) / IISIM[29] (2D); stride = stored widths of
   LLMPR and LLTFM (500 / 50 when GENDB wrote them).
2. A function-table decoder for IIFNC/LLFNC: directory (start, owner, next start) and the four packings of §2.2,
   including the self-describing 10-word form and the n1/n2 header used by BURGRS/ALPHA/NDISFM/RECVRY.
3. A table of kind codes → (nvar, var codes, ncomp) for the constructors (`FUN_14012d2a0` switch), because
   the DB does not store ncomp or most variable codes.
4. Per-material typed view keyed by the table in §3 with the 2D/3D word shifts (ANISO base 131/133, PMEAB
   n1/n2 at 137-138 / 131-132) and version gates (w88, w66, w150-162).
5. Slot ↔ number mapping: list order = slot; w0 = number; MSTMTR lists hold slots.
6. Pair view: record (i, j) = from slot i, to slot j; skip records with w0 = 0.
7. Text: MTNAME (8 words) and TTTN (10 words) need the same byte-order handling as the step header text.
8. For writing: renumber used materials densely, allocate functions in the order of `FUN_140294ae0` and
   `FUN_140337f50` and keep the shared function counter across classes.

## 8. Open questions

* Exact meaning of the variable codes (only 3 = temperature is evident from KEY usage) **[inferred]**.
* Ordering inside the nvar 2/3 value block (copied as one block; index order is that of the model array,
  not visible in this code).
* What sets `+0x2210` (material skipped by the KEY writer) and the mesoscale grain record fields (§3.5, mode 2)
  were not followed into `FUN_14028a950` / `FUN_14028acb0`.
* Lossy combinations: YOUNG-type properties with var 8 or (3,4); FPERV with var ≠ 1; TTTD code 9 with
  w13 ≠ 0; MASDEN 2-D overwriting w1 **[inferred from code paths]**.
