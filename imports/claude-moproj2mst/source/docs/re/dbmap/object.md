# Object class (keyword group 2): IIOBJ / LLOBJ / LLBND / LLWIN / LLWRZ <-> keyword model

Scope: per-object scalar and record data of the object list m+0x1380 (LLOBJ_DATA). Node/element/face arrays are in `objarrays.md`.
Source: decompiled DEF_PRE_64 v11.0.2 only (`loader_ann.c`, `gendb_ann.c`, `kfw_ann.c`, plus a Ghidra listing of
`FUN_140186f30`). Double constants were read from the image. Word numbers are 0-based inside a record; IIOBJ record k =
MAR[start(IIOBJ)-1 + width*k ..], LLOBJ record k = A[start(LLOBJ)-1 + 100*k ..]. "obj+X" = object field; "w N" = word N.
Layout is the v >= 11000 one; version branches are named where they exist (windows, movement type 2, DYNARY).

Counts: 137 group-2 keywords = 58 arrays (objarrays) + 65 mapped here (DB-backed, some partly computed: SYMSUF, BCCFNC, ROTSYM read-only) + 12 unreached (§3.3) + CUSVAR (unreached, user-variable list) + TPLOGY (KEY-only, rebuilt from 2D geometry).

## 1. Read and write paths

### 1.1 DBREAD (per object, `FUN_1401d2230`, step version v >= 11000 layout; dims from `FUN_1400015e0` = is 2D, `FUN_1400015f0` = is 3D)

| Order | Function | Object part | DB source |
|---|---|---|---|
| 1 | `FUN_14016caf0` | obj+0xc type, mesh flag/counts, name, LLOBJ[48..49] function | IIOBJ record (width 50 3D / 20 2D), LLOBJ record (width 100) |
| 2 | `FUN_140186f30` | sub-object obj+0x1ea0 (process/output scalars, REFPOS, TRGVOL, WPAXIS, OPSTOP, RSEDEF...) | IIOBJ, LLOBJ, IIFNC/LLFNC |
| 3 | `FUN_140195ff0` | sub-object obj+0x2e68 (BC windows WIN*/LOC*) | LLOBJ ranges, LLWIN/LLWRZ (see §6) |
| 4 | `FUN_14018a180` | sub-object obj+0x2d10 (remeshing MG*/RM*) | LLOBJ, LLWIN (MGUSER windows) |
| 5 | `FUN_140174e40` | boundary records -> obj+0x3008 (raw copy of first record), boundary face lists, rigid geometry (DIEGEO), movement `FUN_14018d3c0`(obj+0x2160), ROTSYM | LLBND (width 200 3D), LLGRY, IIBDY/IIBEL/IIBND, IIDOD |

Steps 2-5 are skipped for an empty slot (IIOBJ[0] = 0 -> obj+0xc = 7) **[fact]** `FUN_1401d2230`.
Later in ReadArrays `FUN_1401d2be0`: `FUN_140177ba0` (3D, and 2D v>=7000 path) / `FUN_140178250` rebuild BCCFNC and SYMSUF items from node BC function references **[fact]**; `FUN_1401d25d0` -> `FUN_14016d300` (3D fold data FLDGEO/FLWNET, only when m+0x1734 != 0); `FUN_1401d2760` DYNARY (v > 10999); `FUN_1401d63c0` -> `FUN_1402059a0` regenerates SYMSUF planes from die geometry **[fact]**.

### 1.2 GENDB (`FUN_1401c4d90`)

`FUN_1401c4d90` calls `FUN_14017cb60` when `FUN_1400015f0` (is 3D) is false and `FUN_14017b370` otherwise. Region slots used inside them confirm it: `FUN_14017cb60` addresses MAR slots 4/5 (2D IIBDY/IIBEL), `FUN_14017b370` slots 13-16 (3D IIDOD/IIBDY/IIBEL/IIBND). **So `FUN_14017cb60` = 2D and `FUN_14017b370` = 3D (the brief has them swapped)** **[fact]**.

Per object (not for obj+0xc == 7), IIOBJ record = MAR[start(IIOBJ)-1 + width*k], LLOBJ record likewise (k = 0-based list position) **[fact]**:

| 3D `FUN_14017b370` | 2D `FUN_14017cb60` |
|---|---|
| boundary records `FUN_140175db0` (LLBND, incl. movement `FUN_1401912a0`) | boundary records `FUN_140176830` |
| node/element/face arrays (objarrays) | same |
| `FUN_14016cf10` WriteGeneralInfoToDB (IIOBJ 0-9,12,13; LLOBJ 4-11) | same |
| `FUN_14016da70` fold data (IIOBJ 15-18, LLFLD, IIFLD) if obj not rigid | - |
| `FUN_140187a60` sub obj+0x1ea0 | (called last) |
| `FUN_14018a760` sub obj+0x2d10 (remesh) | same |
| `FUN_140196840` sub obj+0x2e68 (windows) | same |
| `FUN_14016d200` LLOBJ[48..49] | same, then `FUN_140187a60` |

Functions referenced from object data are appended to IIFNC/LLFNC by `FUN_1401bb3d0` (FNCLOC): n = ++count; the LLOBJ word receives n; IIFNC[2n-1] (0-based) = owner tag (object id obj+8, or list index); IIFNC[2n] = IIFNC[2n-2] + length; data written at LLFNC[IIFNC[2n-2]-1] **[fact]**. Reader `FUN_1401309b0(fn, IIFNC, LLFNC, n, flag)`: data at LLFNC[IIFNC[2n-2]-1 ..], length IIFNC[2n]-IIFNC[2n-2], fn+0x54 = IIFNC[2n-1] **[fact]**. Function shape (dims, kind) is preset by the caller before the read.

Constants used below (read from the image): DAT_14051bed8 = 0.0, DAT_14051bed0 = 1e-8, DAT_140509c90 = 1.0, DAT_140509fb8 = 2.0, DAT_14051f1c8 = 3.0, DAT_14051f248 = 4.0, DAT_14051bee8 = 10.0, DAT_14051f1f0 = 20.0 **[fact]**.

## 2. Record tables

### 2.1 IIOBJ (ints, 0-based word; width 50 in 3D, 20 in 2D)

| Word | 3D | 2D | Read (`FUN_14016caf0` unless noted) | Write | Status |
|---|---|---|---|---|---|
| 0 | slot state | same | 0 -> obj+0xc = 7 (empty, rest skipped); 1 -> obj+0x87c = 1 (has mesh); 2 -> obj+0x87c = 0 | 0 if obj+0xc==7; 1 if obj+0x87c!=0 and mesh written, else 2 (`FUN_14016cf10`) | **[fact]** |
| 1 | OBJTYP code | same | 0->obj+0xc 0 (rigid), 1->1 (plastic), 2->3 (elastic), 3->4 (elasto-plastic), 4->6 | inverse of read; obj+0xc 2 (porous) writes word1 = 1, word2 = 1; obj+0xc 5 writes 1 | **[fact]** |
| 2 | porous flag | same | ==1 -> obj+0xc = 2 | 1 when obj+0xc == 2 | **[fact]** |
| 3 | sim flag | same | not read | 1, or 0 when m+0x854 == 0 (sim-ctrl field) | **[fact]** write only |
| 4, 5 | first/last global node | same | obj+0x3984 = w4; obj+0x880 = max(0, w5-w4+1); 0 if no mesh | running node counter (`FUN_140199ce0`) | **[fact]** |
| 6, 7 | first/last global element | same | obj+0x3988 = w6; obj+0x884 = max(0, w7-w6+1) | running element counter | **[fact]** |
| 8, 9 | first/last LLBND record | same | obj+0x3588/0x358c; obj+0x888 = count (>100 -> "NoBdry ... greater than maximum", object not read further) | w8 = last-count+1, w9 = last; also stored back into obj+0x3588/0x358c | **[fact]** |
| 10 | OBJTYP 2nd value (obj+0x2154) | flag obj+0x890 | 3D: `FUN_140186f30` obj+0x2154 = w10. 2D: obj+0x890 = w10 (0/1 only) | 3D: obj+0x2154. 2D: always 0 (`FUN_140187a60`) | **[fact]**; 2D value never survives GENDB |
| 11 | - | - | not read | 2D: 0; 3D: not written | **[fact]** |
| 12 | nodes per element (8 brick, 4 tet) | sim value | 3D: 4 -> `FUN_140165500(obj,2)`, 8 -> (obj,4), else 2 | 3D: 8, or 4 when element type is tet; 2D: `FUN_1401f3c50(m+0x750)` | **[fact]** |
| 13 | 1-based offset of connectivity in IINOD | 0 | 3D: used by objarrays | 3D: running IINOD position; 2D 0 | **[fact]** |
| 14 | CRPTIM (obj+0x1f40) | same | `FUN_140186f30` | `FUN_140187a60` | **[fact]** |
| 15, 16 | fold nodes first/last (LLFLD rows) | WPAXIS first/last function number | 3D `FUN_14016d300`; 2D `FUN_140186f30` | 3D `FUN_14016da70`; 2D `FUN_140187a60` | **[fact]** |
| 17, 18 | fold faces first/last (IIFLD rows, 4 ints) | OPSTOP first/last function number | as above | as above | **[fact]** |
| 19, 20 | WPAXIS first/last function number | - | `FUN_140186f30` | `FUN_140187a60` (0,0 when list empty) | **[fact]** |
| 21, 22 | OPSTOP first/last function number | - | same | same | **[fact]** |
| 23..49 | not used | - | - | - | **[fact]** no access found |

### 2.2 LLOBJ (reals, 0-based word; width 100 both dims)

"fn n" = function number in IIFNC/LLFNC (§1.2). int = stored as real, read with (int) truncation, written as (double)int. Object offset = obj+X.

| Word | 3D meaning | 2D meaning | Object field | Read / write transform | Fn |
|---|---|---|---|---|---|
| 0 | REFTMP | same | 0x1eb8 | copy | `FUN_140186f30` / `FUN_140187a60` |
| 1 | AVGSTR | same | 0x1ea8 | copy | same |
| 2 | LMTSTR | same | 0x1ea0 | copy | same |
| 3 | PENVOL real | same | 0x1eb0 | copy | same |
| 4..11 | OBJNAM, 64 chars | same | 0x828 | read: strncpy 64, then `FUN_140324020`(codepage m+0x748), `FUN_1403239f0` (trim); write `FUN_140323bf0` | `FUN_14016caf0` / `FUN_14016cf10` |
| 12..16 | - | - | - | not used | |
| 17 | - | PENVOL int | 0x1f20 | int | 0x1ea0 sub |
| 18, 19 | - | ZSTR int, real | 0x1f30, 0x1f28 | int, copy | 0x1ea0 sub |
| 20 | RMDPTH | same | 0x2dc8 | copy | `FUN_14018a180` / `FUN_14018a760` |
| 21 | RMSTRK | same | 0x2dd0 | copy | same |
| 22 | RMTIME | same | 0x2de0 | copy | same |
| 23 | RMSTEP | same | 0x2dd8 | copy | same |
| 24 | MGWUSR 1st real | - | 0x2d40 | copy | same |
| 25, 26 | MGUSER window first/last (LLWIN rows) | written 0 | 0x2d50 list | int range; write only windows with win+0x370 > 0 | same |
| 27 | MGNELM 4th int | - | 0x2d94 | int | same |
| 28, 29 | MGWUSR 2nd real, int | - | 0x2d98, 0x2da0 | copy, int | same |
| 30 | - | MGNELM 1st int | 0x2d80 | int | same |
| 31 | MGSIZR | MGTELM | 3D 0x2d20 (read: if <= 1e-8 then 1.0); 2D 0x2d84 int | | same |
| 32 | MGWTMP | MGSIZR | 3D 0x2d28; 2D 0x2d20 (no default) | copy | same |
| 33 | MGWSTN | MGWCUV | 3D 0x2d30; 2D 0x2d48 | copy | same |
| 34 | MGWSTR | MGWTMP | 3D 0x2d38; 2D 0x2d28 | copy | same |
| 35 | MGWCUV | MGWSTN | 3D 0x2d48; 2D 0x2d30 | copy | same |
| 36 | - | MGWSTR | 2D 0x2d38 | copy | same |
| 37 | MGTELM int | MGWUSR real | 3D 0x2d84; 2D 0x2d40 | | same |
| 38, 39 | MGNELM 1st, 2nd int | - | 0x2d88, 0x2d8c | int | same |
| 40, 41 | MGGRID 2 ints | same | 0x2dc0, 0x2dc4 | int | same |
| 42 | REFPOS type | MGERR 1st | 3D 0x1ec8 int; 2D 0x2db0 | | 0x1ea0 / 0x2d10 sub |
| 43..45 | REFPOS x,y,z (type 1) or point/node number in 43 (type 2; 44,45 written 0) | 43: MGERR 2nd; 44,45: MGUSER window first/last | 3D 0x1ed0..0x1ee0 / 0x1ee8 int; 2D 0x2db8, 0x2d50 list | | |
| 46, 47 | TRGVOL int, real | - | 0x2098, 0x2090 | read: if real == 0.0 then int = -1. write: int < 0 -> 0, and real forced to 0 in the object when it was != 0 | 0x1ea0 sub |
| 48 | count n (no keyword) | same | 0x3590 | int; 0 -> nothing more | `FUN_14016caf0` / `FUN_14016d200` |
| 49 | fn number of function obj+0x3598 (kind 0x7c = 124, n values) | same | 0x3598 | read `FUN_1401309b0`; write `FUN_1401bb3d0` tag = object id, `FUN_140131210` | same |
| 50, 51 | MGERR 2 reals | REFPOS x, y (type 1) or number in 50 (type 2) | 3D 0x2db0/0x2db8; 2D 0x1ed0/0x1ed8 / 0x1ee8 | | |
| 52 | FRCMTH int | same | 0x1f3c | int | 0x1ea0 sub |
| 53 | FRCSTP int | same | 0x1f34 | int | same |
| 54 | FRCNEL int | same | 0x1f38 | int | same |
| 55 | MGNELM 3rd int | REFPOS type | 3D 0x2d90; 2D 0x1ec8 | | |
| 56, 57 | HDNEST ints (2nd, 1st printed field order: KEY prints 0x1f48 then 0x1f44) | same | 0x1f48, 0x1f44 | int | 0x1ea0 sub |
| 58, 59 | HDNEST reals | same | 0x1f50, 0x1f58 | copy | same |
| 60, 61 | WINTMP window range | same | list 0x2ee0 | see §6 | `FUN_140195ff0` / `FUN_140196840` |
| 62, 63 | WINATM window range | same | list 0x2f10 | | same |
| 64 | VOLCRG code | same | 0x1fd8 kind, 0x1fe4 is-function | read: v<2 -> kind 0, <12 -> 1, <22 -> 2; function when v not in {0,10,20}. write: 0/10/20 for kind 0/1/2, +1.0 when function | 0x1ea0 sub |
| 65 | VOLCRG value or fn n | same | 0x1fe8 value / fn obj+0x1fe0 (kind 1) | function read `FUN_1401309b0`, written `FUN_1401bb3d0` + `FUN_140131360` (tag = object id) | same |
| 66 | ELPSOL int | same | 0x1f18 | int | same |
| 67 | CSFREQ code: 0 const, 1/2/3 function of kind 1 / 0x1c / 0x1d | same | 0x1f64 flag, 0x1f70 kind | write 0, 1.0, 2.0, 3.0 | same |
| 68 | CSFREQ value or fn n | same | 0x1f68 / fn obj+0x1f60 | | same |
| 69, 70 | OTPRNG reals | same | 0x1ef8, 0x1f00 | copy | same |
| 71 | TMPLMT | same | 0x1f10 | copy | same |
| 72 | OTPRNG code | same | 0x1eec, 0x1ef0 | read: v < 0 -> 0x1eec = 3, 0x1ef0 = abs(v) (`FUN_140484ab8`); else 0x1eec = v. write: 3 -> -0x1ef0 | same |
| 73 | USRSUB int | same | 0x2150 | int | same |
| 74 | OTPRNG last int | same | 0x1f08 | int | same |
| 75, 76 | LOCTMP window range | same | list 0x2f40 | §6 | 0x2e68 sub |
| 77, 78 | LOCATM window range | same | list 0x2f70 | | |
| 79, 80 | LOCDEF window range | same | list 0x2fa0 | written only for windows with win+0x174 != 0 | |
| 81 | OBJUPD int | WINPRS first | 3D 0x1f1c; 2D list 0x2eb0 | | 3D 0x1ea0 sub; 2D 0x2e68 sub (v > 8999) |
| 82 | WINPRS first | WINPRS last | list 0x2eb0 | 3D read only for v > 9999 | 0x2e68 sub |
| 83 | WINPRS last | OBJUPD int | 3D list; 2D 0x1f1c | | |
| 84, 85 | - | TRGVOL int, real | 2D 0x2098, 0x2090 | as words 46/47 | 0x1ea0 sub |
| 86 | RSEDEF kind (only 8 is written; read: any value != 0) | same | 0x2118 | int | same |
| 87 | RSEDEF fn n | same | fn obj+0x20d8 (values at fn+0x48 = obj+0x2120) | read `FUN_1401309b0`; write `FUN_1401bb3d0` tag = object id | same |
| 88, 89 | WINRSE window range (v > 10999) | same | list 0x2fd0 | | 0x2e68 sub |
| 90..99 | - | - | - | not used | **[fact]** no access found |

All rows **[fact]** from `FUN_140186f30`, `FUN_140187a60`, `FUN_14018a180`, `FUN_14018a760`, `FUN_140195ff0`, `FUN_140196840`, `FUN_14016caf0`, `FUN_14016cf10`, `FUN_14016d200`; keyword names from the KEY writers listed in §3.
Words 25/26 in 2D, 81/82 in 2D and 82/83 in 3D are zeroed by the writer before the window lists are written.

### 2.3 Function-table records owned by objects

| Keyword | Count/range | LLFNC record | Read / write |
|---|---|---|---|
| WPAXIS (list obj+0x20a0) | IIOBJ 19..20 (3D) / 15..16 (2D), consecutive fn numbers, tag = 1-based list index | 12 reals: [6] = axis type 0..7; types 0-2: [0..2] vector A (`FUN_140258ed0`), [3..5] vector B, [7] int, [8..11] reals; 3: A, B, [7] int; 4: [0..2], [3..8], [9..] (`FUN_140258fd0/9060/90f0`), [7] int, [8] real; 5: B and [8]; 7: B at [0..2], A at [3..5], [7] real, [8],[9] ints, [10],[11] reals | `FUN_1402589a0` / `FUN_140258c40` **[fact]** (the call passes LLFNC + IIFNC[2k-2]-1; the decompiler dropped the 2nd argument, confirmed in assembly at 0x14018763f / 0x1401878d1) |
| OPSTOP (list obj+0x20b8) | IIOBJ 21..22 (3D) / 17..18 (2D) | 20 reals per item, filled by item vtable +0x18, read by +0x10 | layout not followed **[fact]** for the plumbing, layout unreached |
| VOLCRG, CSFREQ, RSEDEF, LLOBJ[49] function | one fn number in LLOBJ | generic function layout of `FUN_1401309b0` | |

## 3. Keyword table (group 2 "object", 137 keywords)

KEY writer: object class `FUN_14029e250` (kfw_ann.c) and the sub-writers named per row. "M" = DB-mapped, "K" = KEY-only, "U" = unreached, "A" = see objarrays.

### 3.1 Scalars and object sub-objects

| Id | Keyword | KEY writer (fields) | DB location | Read / write notes | |
|---|---|---|---|---|---|
| 144 | OBJNAM | `FUN_1402a2530` (obj+0x828) | LLOBJ 4..11 | §2.2 | M **[fact]** |
| 145 | OBJTYP | `FUN_1402a25a0`: code = obj+0xc mapped 0..6 -> 1..7, 7 -> 0; 2nd value obj+0x2154 | IIOBJ 0..2; 3D also IIOBJ 10 | obj+0xc 5 has no DB code (written as 1 = plastic, read back as 1) | M **[fact]**; 2D 2nd value K |
| 146 | AVGSTR | `FUN_1402af420` (0x1ea8) | LLOBJ 1 | copy | M **[fact]** |
| 147 | LMTSTR | `FUN_1402af4f0` (0x1ea0) | LLOBJ 2 | copy | M **[fact]** |
| 148 | ZSTR | `FUN_1402af5c0` (2D only; 0x1f30 int, 0x1f28) | 2D LLOBJ 18, 19 | | M **[fact]** |
| 149 | PENVOL | 3D inline (0x1eb0); 2D `FUN_1402b4400` (0x1eb0, 0x1f20 int) | LLOBJ 3; 2D int LLOBJ 17 | | M **[fact]** |
| 150 | REFTMP | `FUN_1402af350` (0x1eb8) | LLOBJ 0 | | M **[fact]** |
| 158 | DIEGEO | 2D `FUN_1402aaa70`, 3D `FUN_1402a27a0` (+ `FUN_1402ab210/2b0/530`) | LLBND records + LLGRY + IIDOD (§5) | rebuilt from boundary records | M **[fact]** (layout §5) |
| 161 | BCCFNC | `FUN_1402b4050` (items of list obj+0x2e68: +0x54 tag, +0x1c) | IIFNC/LLFNC functions referenced by node BC arrays | rebuilt in ReadArrays by `FUN_140177ba0`: every fn number used in the node DOF function array (obj+0x998) or face/edge function arrays becomes an item, loaded by `FUN_1401309b0` (kind 1/1); the node array value is replaced by IIFNC[2n-1] | M **[fact]**, function data only |
| 180-183 | RMDPTH, RMSTRK, RMTIME, RMSTEP | `FUN_1402b49d0`, `FUN_1402b4aa0`, `FUN_1402b4b70`, inline in `FUN_1402a9180` | LLOBJ 20, 21, 22, 23 | copy | M **[fact]** |
| 184 | MGNELM | `FUN_1402a9180` 3D: 0x2d88, 0x2d8c, 0x2d90, 0x2d94; 2D: 0x2d80, 0x2d8c, 0x2d94 | 3D LLOBJ 38, 39, 55, 27; 2D LLOBJ 30 | 2D fields 0x2d8c, 0x2d94 are KEY-only (not in the 2D record) | M **[fact]** |
| 185 | MGTELM | 0x2d84 | 3D LLOBJ 37; 2D 31 | int | M **[fact]** |
| 186-190 | MGSIZR, MGWCUV, MGWTMP, MGWSTN, MGWSTR | `FUN_1402b44e0` 0x2d20, `FUN_1402b45b0` 0x2d48, `FUN_1402b4680` 0x2d28, `FUN_1402b4750` 0x2d30, `FUN_1402b4820` 0x2d38 | 3D 31, 35, 32, 33, 34; 2D 32, 33, 34, 35, 36 | 3D MGSIZR <= 1e-8 read as 1.0 | M **[fact]** |
| 191 | MGWUSR | 3D 0x2d40, 0x2d98, 0x2da0 int; 2D 0x2d40 | 3D LLOBJ 24, 28, 29; 2D 37 | | M **[fact]** |
| 192 | MGGRID | 0x2dc0, 0x2dc4 | LLOBJ 40, 41 | int | M **[fact]** |
| 193 | MGERR | `FUN_1402b48f0` 0x2db0, 0x2db8 | 3D LLOBJ 50, 51; 2D 42, 43 | | M **[fact]** |
| 194 | MGUSER | `FUN_1402a9180` windows of list obj+0x2d50 (only win+0x370 > 0) | LLOBJ 25..26 (3D) / 44..45 (2D) + LLWIN/LLWRZ | §6 | M **[fact]** |
| 198 | CUSVAR | `FUN_1402a4a80` code 0x4e via `FUN_1402b7310`, per item of obj+0xb58 | user-variable list; no access in the object reader | objarrays / sim-ctrl owner (IIUVL/LLUVL) **[inferred]** | U (see objarrays) |
| 199 | FRCSTP | inline 0x1f34 | LLOBJ 53 | int | M **[fact]** |
| 200 | FRCNEL | inline 0x1f38 | LLOBJ 54 | int | M **[fact]** |
| 201 | FRCMTH | inline 0x1f3c | LLOBJ 52 | int | M **[fact]** |
| 202 | TMPLMT | `FUN_1402af6a0` 0x1f10 | LLOBJ 71 | | M **[fact]** |
| 212 | CRPTIM | inline 0x1f40 | IIOBJ 14 | | M **[fact]** |
| 215 | REFPOS | inline: type 0x1ec8; 1: 0x1ed0..0x1ee0; 2: int 0x1ee8 | 3D LLOBJ 42..45; 2D 55, 50, 51 | KEY skipped when type 0; type not 1/2 aborts the KEY object | M **[fact]** |
| 218 | HDNEST | `FUN_1402b4c40` 0x1f48, 0x1f44, 0x1f50, 0x1f58 | LLOBJ 56, 57, 58, 59 | | M **[fact]** |
| 226-229 | WINPRS, WINTMP, WINATM, WINRSE | inline / `FUN_1402ab880` | LLOBJ ranges + LLWIN/LLWRZ | §6 | M |
| 230-232 | LOCTMP, LOCATM, LOCDEF | `FUN_1402ae590`, `FUN_1402aeb40`, `FUN_1402aef30` (via `FUN_1402ac560`) | LLOBJ 75..80 + LLWIN/LLWRZ | §6 | M |
| 234 | ELPSOL | inline 0x1f18 | LLOBJ 66 | int | M **[fact]** |
| 235 | CSFREQ | `FUN_1402af830` 0x1f64, 0x1f70, 0x1f68 / fn 0x1f60 | LLOBJ 67, 68 | §2.2 | M **[fact]** |
| 236 | VOLCRG | `FUN_1402afb10` 0x1fd8, 0x1fe4, 0x1fe8 / fn 0x1fe0 | LLOBJ 64, 65 | §2.2 | M **[fact]** |
| 240 | OTPRNG | `FUN_1402abd50` (via `FUN_1402af770`) | LLOBJ 72, 69, 70, 74 | sign coding §2.2 | M **[fact]** |
| 253 | USRSUB | inline 0x2150 (only when != 0) | LLOBJ 73 | int | M **[fact]** |
| 258 | ROTSYM | inline (3D) 0x2058, 0x2078..0x2088, 0x2060..0x2070 | 3D first LLBND record words 11, 44..46, 47..49 | read only (`FUN_140174e40`); written by the boundary writer (§4) | M **[fact]** read |
| 259 | TRGVOL | inline 0x2098 int, 0x2090 | 3D LLOBJ 46, 47; 2D 84, 85 | 3D GENDB may first replace 0x2090 by the value from `FUN_14017b230` ("Target volume of object %d") | M **[fact]** |
| 262 | SYMSUF | `FUN_1402a3880` (3D; list obj+0x2e80) | not stored: items created by `FUN_140177ba0` for node BC code 6 (id = IIFNC[2n-1]); plane data regenerated from geometry by `FUN_1401d63c0` -> `FUN_1402059a0` | computed | M (ids) / computed **[fact]** |
| 264 | WPAXIS | `FUN_1402a39e0` (list obj+0x20a0) | IIOBJ 19..20 (3D) / 15..16 (2D) + LLFNC 12-real records | §2.3 | M **[fact]** |
| 265 | OPSTOP | `FUN_1402a4020` (list obj+0x20b8; item +0xc == 1, value +0x18) | IIOBJ 21..22 / 17..18 + LLFNC 20-real records | §2.3 | M **[fact]** (record layout unreached) |
| 266 | DYNARY | `FUN_1402a41c0` (list obj+0xba8): index, class, type, rows, components; type 3 adds default value and writes only rows differing from it | dynamic-array descriptor (v >= 11000): row words 5 owner object, 6 class, 7 type, 8 components | read `FUN_1401d2760` -> `FUN_14025bdc0` (real) / `FUN_14025bd10` (int); write `FUN_14025bbc0`, `FUN_14025be70` (gendb) | M **[fact]** (format in spec §5.4) |
| 267 | OBJUPD | `FUN_1402a7c80` 0x1f1c | 3D LLOBJ 81; 2D 83 | int | M **[fact]** |
| 269 | TPLOGY | `FUN_1402a9f90` (2D, die topology vertex/edge/loop/face of geometry obj `FUN_140197f40(obj,0)`) | not stored; the topology object is rebuilt from the 2D boundary geometry (`FUN_14027fe20` in `FUN_140174e40` when first record word 0 is 1 or 2) | computed **[inferred]** | K/computed |
| 289, 290 | FLDGEO, FLWNET | `FUN_1402a2b70` (3D, obj+0x3328 / 0x3458 surface meshes) | IIOBJ 15..16 -> LLFLD rows (3 reals), IIOBJ 17..18 -> IIFLD rows (4 ints); face word 4 > 0 = FLDGEO, < 0 = FLWNET (stored relative to first node, sign kept) | read `FUN_14016d300` only when m+0x1734 != 0 and widths of LLFLD/IIFLD > 0; write `FUN_14016da70` (3D, non-rigid) | M **[fact]** |
| 302 | RSEDEF | `FUN_1402b7140` (only when 0x2118 == 8; 8 reals of obj+0x2120) | LLOBJ 86, 87 + function | | M **[fact]** |

Movement keywords (MOVCTL, MOVANG, STROKE, SPDLMT, ANGMOV, CNTRAX, ANGMVY, PRSTRE, PRSNAM, ANGMO2, CNTRA2, OBJSTP, FORCE): §4.

### 3.2 Arrays: see objarrays

RZ, DRZ, BCCDEF, BCCDFN, URZ, FRZ, PRZ, BCCTMP, BCCTFN, NDTMP, NDHEAT, NDFLUX, ELMCON, MTLGRP, STRAIN, STNCMP, DENSTY, DAMAGE, STRESS, USRNOD, USRELM, VOLFC, DVOLF, TICF, GRAIN, GRAIN2, SIZESH, TXTODF, VOTAGE, DATOM, VOLFS, HDNOBJ, BCCCRB, BCCRHT, CRBFLX, RHTFLX, TIMERC, ERECVL, NRECVL, YLDS, ZEFI, ZMFI, CURRNT, ECCDEF, ECCTMP, ECCATM, ECDEFN, ECTMFN, ECATFN, ECPRES, ECHFLX, ECAFLX, ECCRHT, ECRHFN, ECRFLX, MATAXI, NWEAR, DRMESH (58 keywords; all printed by `FUN_1402a4a80`, `FUN_1402a2e90/2fc0` (RZ), `FUN_1402a30c0` (ELMCON)). CUSVAR is printed by the same array writer but its data is a per-object list, not a node/element array.

### 3.3 Not written to KEY by the object writer and not accessed by the object DB code

MOVANG (152), BCCANG (164), MGSIZM (195), OBJPHS (204), CSPMTX (270), CRDSYS (282), GEO23 (283), GEOSEC (284), MSHSEC (285), POLYTP (299), DIEPNT (300), DIEEDG (301): no `FUN_1402f7720(id)` reference anywhere in the KEY writer closure kfw_ann.c and their registry write handler is the generic `0x1402f75d0`; no object field could be tied to them in the loader or GENDB closures. Classified **U** (reader-only or obsolete keywords; a deeper KEY reader trace would be needed) **[fact]** for the absence, **[inferred]** for the reason.

## 4. Movement (OMV_DATA at obj+0x2160) in the first LLBND record

Read: `FUN_140174e40` -> `FUN_14018d3c0`(obj+0x2160, LLBND record IIOBJ[8], IIFNC, LLFNC, m+0x748) = OMV_DATA::ReadFromDB, first record only.
Write: `FUN_140175db0` (3D) / `FUN_140176830` (2D) -> `FUN_1401912a0` = OMV_DATA::WriteToDB, called for **every** boundary record of the object; no version branch (always the v >= 11000 layout) **[fact]**.
Only version branch on read: MOVCTL type 2 (v < 11000 takes the curve from the generic control words). After the switch: SPDLMT flag set and type 1 -> type 8 **[fact]**.
Notation: `mv+X` = obj+0x2160+X; `fn@X` = embedded function object; "xcode" = x-variable code stored in the DB; xt = in-memory code. Writer constants 1.0..11.0 and -1.0 are confirmed from the image; DAT_140522fd8 = 57.2957795 (180/pi).

### 4.1 MOVCTL (id 151 / 0x97), KEY writer FUN_1402b1400. KEY line: `MOVCTL obj TYPE CTL ...`

| KEY field / meaning | mv offset | 3D word | 2D word | read (FUN_14018d3c0) | write (FUN_1401912a0) |
|---|---|---|---|---|---|
| PRSNAM text (64 ch) | mv+0x00 (char[0x50]) | w82..w89 (+w90,w91 read) | w56..w63 (+w64,w65 read) | strncpy 0x50 B, FUN_140324020 fix-up, FUN_1403239f0 rtrim [fact] | FUN_140323bf0: strncpy 0x40 B, space-pad to 64 (FUN_140323b90) [fact] |
| direction X,Y(,Z) (3D KEY: 3 reals; 2D KEY: angle deg = atan2(Y,X)*k, FUN_14018c810) | mv+0x50,0x58,(0x60) | w12,w13,w14 | w10,w11 (mv+0x60 := 0) | copy [fact] | 3D normalised by FUN_14018c940; 2D components within tol of 0/1 snapped (FUN_14018c850) [fact] |
| TYPE (0..11) | mv+0x68 int | w15 | w7 | (int) cast; switch [fact] | constant double = type [fact] |
| type 0 speed const | mv+0x78 | w17 (w16=0) | w9 (w12=0) | copy | fn@0x70.kind==0 -> ctl word 0 [fact] |
| type 0 speed curve | fn@0x70 | w16=fn#, w17=xcode | w12=1, w13=xcode, w14=fn# | xcode 1->xt 2, 2->xt 1; kind=1 [fact] | xt 1->2.0, 2->1.0 [fact] |
| type 1 (force) const | mv+0xf0 | w17 (w16=0) | w9 (w12=0) | copy | when mv+0x160==0 and fn@0xe8.kind==0 |
| type 1 curve | fn@0xe8 | w16=fn#, w17=xcode | w12=1,w13,w14=fn# | xcode 1->xt2, 2->xt1, 4->xt5 [fact] | xt 1->2, 2->1, 5->4 |
| type 1 "1000+n" (KEY CTL=1000+mv+0x160, value mv+0x168) | mv+0x160 int, mv+0x168 | w16 = -(n+1000), w17=val | w12=2, w14=n, w9=val | 3D: if -2000<w16<-1000: n=-1000-w16 [fact] | 3D -(n+1000); 2D w12=2.0 [fact] |
| type 2 (KEY order 0x1f0,0x1f8,0x1e8,0x200) | mv+0x1e8,0x1f0,0x1f8,0x200(int) | 0x1f0=w10, 0x1f8=w41, 0x1e8=w42, 0x200=(int)w43 | 0x1f0=w9, 0x1e8=w15, 0x1f8=w16, 0x200=(int)w17 | [fact] | [fact] same words |
| type 2 curve (KEY xt=4) | fn@0x208 | v>=11000: w103=fn#, w104=xcode; v<11000: w16=fn#, w17 | v>=11000: w69=fn#, w70; v<11000: w12==1,w13,w14 | xcode 4 -> xt 5; other -> prints "Need to check OMV_DATA::ReadFromDB()" [fact] | always w103/w104 (3D), w69/w70 (2D); xt5 -> 4.0 [fact] |
| type 3 4 reals | mv+0x280,0x288,0x290,0x298 | w10,w41,w42,w43 | w9,w16,w15,w17 | copy [fact] | same [fact] |
| type 3 curve | fn@0x2a0 | w103/w104 | w69/w70 | xcode 4 -> xt5 (no version test) [fact] | [fact] |
| type 4 3 reals | mv+0x318,0x320,0x328 | w42,w43,w41 | w15,w17,w16 | copy | same |
| type 4 curve (flag char mv+0x330) | fn@0x338 | w16=fn#, w17=3.0 | w12=1, w14=fn#, w13=3.0 | ctl>0 -> kind 1, xt 10; ctl<1 clears [fact] | [fact] |
| type 5 const | mv+0x3b8 | w43 (w16=0) | w17 (w12=0) | copy | [fact] |
| type 5 curve | fn@0x3b0 | w16=fn#, w17=1 | w12=1,w13=1,w14=fn# | needs xcode==1; xt := 19 [fact] | xcode 1.0 |
| type 5 extras | mv+0x428,0x430,0x438,0x440i,0x444i | w76,w42,w63,(int)w62,(int)w64 | w50,w15,w48,(int)w47,(int)w49 | [fact] | [fact] |
| type 6 die-follow (KEY CTL=-abs(mv+0x514)) | mv+0x510 flag, mv+0x514 | w16 = -n | w12 = -1, w14 = n | 3D w16<0, 2D w12==-1 -> flag=1, n=abs [fact] | [fact] |
| type 6 curve | fn@0x448 (type 0x7a 3D / 0x79 2D) | w16=fn#, w17=2 or 5 | w12=1,w13=2 or 5,w14=fn# | kind=1, xt=1; xcode 5 -> FUN_140134b00 [fact] | xt==1 -> 2.0 else 5.0 + FUN_140134de0 transform [fact] |
| type 6 ints (KEY prints 0x4c0,xt,0x4c4,0x464) | mv+0x464,0x4c0,0x4c4 | w41,w42,w43 (int) | 0x464=w15, 0x4c4=w16 (0x4c0 not in 2D) | [fact] | [fact] |
| type 6 3D frame (3x3) | mv+0x4c8..0x508 | LLFNC header of fn#: [0]=10.0 (offset to data), [1..9]=mv+0x4c8..0x508 | n/a | FUN_140130920 raw ptr; if 0x4c0==2 apply FUN_140194e30 [fact] | [fact] |
| type 7 const / curve | mv+0x520 / fn@0x518 | w17 / w16,w17 | w9 / w12,w13,w14 | as type 0 [fact] | as type 0 |
| type 7 reals | mv+0x608,0x610 | w42,w43 | w15,w16 | | |
| type 8 const / curve | mv+0x598 / fn@0x590 | w17 / w16,w17 | w9 / w12..w14 | as type 0 | |
| type 8/9 real | mv+0x608 | w42 | w15 | | |
| type 10 reals | mv+0x608,0x618 | w42,w43 | w15,w16 | | |
| type 11 3 reals | mv+0x188,0x190,0x198 | w21,w22,w23 | w20,w21 (0x198 := 0) | generic copy (all types) | written only for type 11 (and type-1 const, see sec 2) |
| follow other object (KEY CTL=-abs(mv+0x624), types 0,1,7,8) | mv+0x620 flag, mv+0x624 | w16 in (-1000,0): n=abs(w16) | w12<0: n=abs(w14) | after switch, all types [fact] | 3D w16=-n; 2D w12=-1.0, w14=n (overrides ctl) [fact] |

### 4.2 Other movement keywords

| KEY (id, writer) | KEY fields -> mv offset | 3D word | 2D word | read / write notes |
|---|---|---|---|---|
| STROKE (153, FUN_1402b0350) | 3D: 0x170,0x178,0x180; 2D: 0x170,0x178 | w18,w19,w20 | w18,w19 (0x180:=0) | copy both ways [fact]; KEY skipped if obj+0xc==5 |
| FORCE (303, FUN_1402b04b0) | 3D: 0x1a0,0x1a8,0x1b0; 2D: 0x1a0,0x1a8 | w24,w25,w26 | w22,w23 (0x1b0:=0) | read copy [fact]; write: ONLY for type0-const = mv+0x78*dir (see sec 2) [fact] |
| SPDLMT (155, FUN_1402b11e0) | flag char 0x8b8, fn@0x8c0 (npts 0x8dc), 0x938, 0x940 | w75=fn#, w80, w81 | w8=fn#, w54, w55 | fn#!=0 -> flag=1 [fact]; KEY: if flag==0 and 0x940>0 writes `0 0 val` |
| PRSTRE (256, FUN_1402afd70) | 0x948,0x950,0x958 | w77,w78,w79 | w51,w52,w53 | copy; KEY only if 0x948>0 [fact] |
| ANGMOV (156, FUN_1402b0610) mode | 0x628 (0/1) | w27 | w30 | [fact] |
| ANGMOV mode0 const / curve | 0x668 / fn@0x660 | w38 / w28=xcode,w29=fn# | w32 / w35,w36 | 3D xcode 1->xt1, 2->xt2, 3->xt10; 2D 1->1, 2->10. KEY xt 1->1, 10->2 [fact] |
| ANGMOV mode1 const / curve | 0x6e0 / fn@0x6d8 | w37 / w28,w29 | w31 / w35,w36 | same [fact] |
| CNTRAX (157, FUN_1402b1000) center | 3D 0x630,0x638,0x640; 2D 0x630,0x638 | w33,w34,w35 | w33,w34 | [fact] |
| CNTRAX axis (3D) | 0x648,0x650,0x658 | w30,w31,w32 | n/a | write normalised (FUN_14018ca10) [fact] |
| CNTRAX last field | 0x750 | w36 | w37 | [fact] |
| ANGMO2 (260, FUN_1402afe80, 3D) mode | 0x758 | w50 | n/a | [fact] |
| ANGMO2 const / curve | mode0 0x798 / fn@0x790; mode1 0x810 / fn@0x808 | w61 / w60; ctl w51, fn# w52 | n/a | xcode 1/2/3 <-> xt 1/2/10; KEY only xt 1,10 [fact] |
| CNTRA2 (261, FUN_1402b01e0, 3D) | 0x760,0x768,0x770 ; 0x778,0x780,0x788 ; 0x880 | w56,w57,w58 ; w53,w54,w55 ; w59 | n/a | axis normalised on write (FUN_14018cb10) [fact] |
| ANGMVY (203, FUN_1402b0a40, 2D) present | 0x960 (-1 = none) | n/a | w38 (0 -> 0x960=-1), w39 = mode | write only if ctx+0x850==3 or (==1 and 0x960==1) [fact] |
| ANGMVY mode0/1 | 0x970/fn@0x968 ; 0x9e8/fn@0x9e0 | n/a | w40=ctl(0,1,2 -> xt 1,10), w41=const or fn# | [fact] |
| ANGMVY mode2 | 0xa58,0xa60,0xa68 | n/a | w41,w42,w43 | [fact] |
| ANGMVY last | 0xa70 | n/a | w46 | [fact] |
| OBJSTP (268, in FUN_1402a7c80) | 3D 0x888..0x8b0 (6); 2D 0x888,0x890,0x8a0,0x8a8 | w91..w96 | w65..w68 | copy [fact]; KEY gated by FUN_140198c80 (obj+0xc!=0 and norm(mv+0x8a0..) > tol) |
| PRSNAM (257, FUN_1402a7c80) | mv+0 string | w82.. | w56.. | see 1a |
| ROTSYM (258, in FUN_14029e250, 3D) | obj+0x2058 ; obj+0x2078,0x2080,0x2088 ; obj+0x2060,0x2068,0x2070 | w11 ; w44,w45,w46 ; w47,w48,w49 | n/a | read FUN_140174e40 end; write FUN_140175db0 after movement [fact]. Outside OMV_DATA |


Derived and side writes:


- type 0 with const speed: 3D w24..26 = mv+0x78*dir(x,y,z); 2D w22,23 = mv+0x78*dir(x,y). On read these land in mv+0x1a0.. (FORCE slots).
- type 1 with const force: 3D w21..23 = mv+0xf0*dir; 2D w20,21 = mv+0xf0*dir. On read these land in mv+0x188.. (type-11 slots).
- 2D only: before w24..29 are written, FUN_1401d6030 copies ctx+0x8d0,0x8d8,0x8e8,0x8f0,0x900,0x908 into mv+0x1b8..0x1e0; reader copies w24..29 back into mv+0x1b8..0x1e0.


First-record words used by movement with no KEY keyword [fact]:
- 2D w24..w29 (mv+0x1b8..0x1e0): values come from ctx (sim-control) block, no movement keyword prints them.
- 3D w21..23 / 2D w20,21 for type 1 const (force*dir), 3D w24..26 / 2D w22,23 for type 0 const (speed*dir): derived vectors, KEY prints the scalar + direction instead.
- 3D type-6 LLFNC header (10.0 + 3x3 frame mv+0x4c8..0x508): KEY prints it only when 0x4c0==2 (two lines after MOVCTL).
- Control/xcode words (3D w16,w17; 2D w12..w14) carry fn numbers: KEY instead inlines the curve via FUN_1402f3130 (npts from fn+0x1c).
- Name tail bytes 64..79 (3D w90,w91; 2D w64,w65) are read into the name buffer but never written as name.

KEY-only / not in DB:
- MOVANG (id 152): registry writer is the default stub FUN_1402f75d0; no movement code reads or writes a MOVANG field [fact] -> no DB word (obsolete/alias) [inferred].
- SPDDEF: not in keyword_registry.tsv [fact]; not found.
- 2D MOVCTL angle field: computed (FUN_14018c810 = atan2(mv+0x58, mv+0x50)*DAT_140522fd8) from w10,w11 [fact]; DAT_140522fd8 = 180/pi [inferred].
- Curve point counts (npts at fn+0x1c, e.g. mv+0x8c, 0x104, 0x224, 0x2bc, 0x354, 0x3cc, 0x534, 0x5ac, 0x67c, 0x8dc): live in IIFNC/LLFNC, not LLBND [fact].
- 3D: ANGMVY (mv+0x960..0xa70) not in 3D record. 2D: ANGMO2/CNTRA2 (mv+0x758..0x880), CNTRAX z/axis (0x640..0x658), OBJSTP 0x898/0x8b0, type-6 0x4c0 not in 2D record [fact].
- FORCE (mv+0x1a0..) is not written from the FORCE value itself; see sec 2 [fact].


## 5. LLBND boundary records (non-movement words) and rigid geometry (DIEGEO)

Object k owns LLBND records IIOBJ[8]..IIOBJ[9] (count obj+0x888, max 100). Record r = A[start(LLBND)-1 + W*(r-1) ..], W = LLBND width (200 in 3D). On read the first 100 reals of the first record are also copied raw to obj+0x3008 (`memcpy 800`) **[fact]** `FUN_140174e40`. Writers: 3D `FUN_140175db0`, 2D `FUN_140176830`, called once per record with a record type (3D: 3 = deformable boundary, 2 for a rigid object for which `FUN_140203980` is true; geometry-only records of rigid objects use type 1) **[fact]** `FUN_14017b370`, `FUN_14017cb60`.

| Word | 3D | 2D | Read (`FUN_140174e40`) | Write | |
|---|---|---|---|---|---|
| 0 | record type | same | 1.0 or 2.0 in the first record: 3D `FUN_1401985e0(obj,1)`; 2D builds the die geometry `FUN_14027fe20` | = type | **[fact]** |
| 1 | boundary node count (obj+0xbe0+0x30i) | - | when obj has mesh | written when type > 1 | **[fact]** |
| 2 | first IIBND node row (1-based) | boundary node count | 2D: count | running counter + 1 | **[fact]** |
| 3 | boundary face count (+0xbf0) | first IIBDY face (1-based) | 3D faces x 4 ints read from IIBDY at (w4-1)*4, IIBEL at w4-1, both stored minus first node/element + 1 | | **[fact]** |
| 4 | first IIBDY/IIBEL face (1-based); first record value kept in obj+0x398c (used for element-face BCs) | 2D geometry: point count (geo+0x14; geo+0x10 := 2) | | | **[fact]** |
| 5 | geometry node count | 2D: first LLGRY word (count x components) | 3D only when `FUN_140160d90(obj)`: `FUN_1402fcd90(geo, w5, w7)` | | **[fact]** |
| 6 | first LLGRY row (x,y,z) | 2D: 1.0 -> `FUN_1402763d0(geo,1)` | 3D: 3*w5 reals copied | | **[fact]** |
| 7 | geometry face count | - | | | **[fact]** |
| 8 | first IIDOD word for faces (4 ints/face) | - | | | **[fact]** |
| 10 | 4.0 (brick) or 3.0 (tet): nodes per boundary face | - | not read | written when type > 1 | **[fact]** |
| 11, 44..49 | ROTSYM | - | §3.1 | | **[fact]** |
| 39 | 1.0 when polygonal geometry is written | - | not read (reader uses `FUN_140160d90`) | | **[fact]** |
| 97 | first IIDOD word of per-face ints (0 = none -> `FUN_1402f9af0`) | - | | | **[fact]** |
| 98, 99 | first/last fn number, bearing-surface items (`FUN_1402fd020`, `FUN_1402f8850`) | - | | `FUN_1401bb3d0` | **[fact]** |
| 100, 101 | first/last fn number, second item list (`FUN_1402fd490`, `FUN_1402f8de0`) | - | | | **[fact]** |
| 102 | entity count check ("Info : during bearing surface loading...") | - | compared with `FUN_1402fcfe0` | | **[fact]** |

Movement words (3D w12..w38, w41..w43, w50..w96, w103/w104; 2D w7..w70) are in §4. The 2D geometry layout of LLGRY (segment kinds, 3 or 6 reals per point) follows the KEY DIEGEO writer `FUN_1402aaa70` **[inferred]**.

## 6. Windows (LLWINDEF_DATA) in LLWIN / LLWRZ

Read: `FUN_140195ff0` (lists in obj+0x2e68) and `FUN_14018a180` (remesh list obj+0x2d50, presets win+0x174 = 2) create window k = 1..n (win+0x170 = k, regenerated) and fill it by `FUN_14025de80`(win, LLWIN + W*(first-1+i), LLWRZ, IIFNC, LLFNC) = ReadFromDBArrays. Write: `FUN_140196840` / `FUN_14018a760` -> `FUN_140260ea0` = WriteToDBArrays (sub-writers `FUN_140262250`, `FUN_140262cf0`, `FUN_140263040`, `FUN_140262c00`) **[fact]**.

### 6.1 Cursors and gating
Cursors [fact]: model+0x134 = next free LLWIN record (1-based), model+0x138 = next free LLWRZ row (1-based)
(`FUN_14019a5a0/5c0`, `FUN_14019a5e0/600`); model+0x10c = last function number used (`FUN_1401bb3d0`).
Per list: start = cursor; after writing, if any record was produced, LLOBJ[first] = start, LLOBJ[last] = cursor-1, else both 0.0 [fact].

Write gating flags (3D `FUN_14017b370` / 2D `FUN_14017cb60`) [fact]:
- flagA (WINTMP, WINATM, LOCTMP, LOCATM, and remesh windows): 0 if obj+0xc == 0 and (`FUN_1401f3d90(simctrl)` or object has no own mesh: 3D obj+0x884 < 1, 2D no boundary/mesh written); else 1.
- flagLOCDEF: `FUN_1401f3c30(simctrl)` (simctrl+0x108) != 0 and object has mesh (3D obj+0x884 >= 1 / 2D mesh written). LOCDEF also skips windows with win+0x174 == 0.
- flagWINPRS: 2D `FUN_14017cb60`: obj+0xc != 0 and mesh written; 3D `FUN_14017b370`: always 1.
- WINRSE: no flag, always written; read only if version > 10999 [fact].

### 6.2 LLWIN record (one row of width W_LLWIN, doubles)

Window type word0 [fact]: read `type = {1,2,3,4,5,6,7,8,10}[word0-1]` (word0 1..9); write is the inverse (type 10 -> 9.0).

#### 6.2a Common header (types 1,2,3,4,8,10 — geometry types; types 5,6,7 have no geometry)

| Word | 3D field | 2D field | Read | Write |
|---|---|---|---|---|
| 0 | +0x174 type | same | see above | see above |
| 1 | first LLWRZ row (1-based) | same | used as row index | = curWrz [fact] |
| 2 | last LLWRZ row | same | n = word2-word1 (+1) | = curWrz-1+nRows |
| 3 | +0x350 | fn/obj encoding -> +0x368/+0x36c | double | |
| 4 | +0x358 | +0x350 | double | |
| 5 | +0x360 | +0x358 | double | |
| 14 | shape +0x210 | (none) | 0/1->1 cube, 2->2 cylinder, 3->3 mesh, 11/12/13->11 polyline [fact] | 1->1.0, 2->2.0, 3->3.0, 11/12/13 -> `DAT_140522fe0` (11.0, read from the image); else 0.0 + error |
| 15 | encoding +0x368/+0x36c | (none) | 1.0 -> +0x368=0; <0 -> +0x368=2, +0x36c=-word; other: unchanged | +0x368==0 -> 1.0; ==2 -> -abs(+0x36c) (`FUN_140484ab8`); ==1 -> word not written |

2D special [fact]: if type==1 and +0x368==2 and +0x36c==0, word3 = -param_10 (callers pass 0).
Meaning of +0x350..+0x360 and +0x368/+0x36c is not named in code; KEY prints them as the second line of each window
(3 doubles 3D, 2 doubles 2D) and the encoded int (1 / 2 / -n) [fact]. Possibly velocity + "follow object n" [inferred].

#### 6.2b LLWRZ geometry (rows of 3 doubles in 3D, 2 doubles in 2D; row r at LLWRZ + 8*stride*(r-1)) [fact]

| Shape | Rows | Content | Window fields |
|---|---|---|---|
| 2D (any) | n = word2-word1+1 | (x,y) per point | +0x178 = n, +0x180 = points |
| 1 cube | 8 (read errors if word2-word1 != 7) | xyz corners | +0x178=8, +0x180; then `FUN_140267760` builds faces |
| 2 cylinder | 3 | rows 1-2 = axis points (xyz); row 3 = (+0x1b8, +0x1c0, unused) | +0x178=2 |
| 3 mesh | nNodes + ceil(4*nElem/3) | nodes xyz, then 4 ints/elem stored as doubles, packed continuously | word7 = nNodes, word8 = nElem; object at +0x220 |
| 11 polyline | n = word2-word1+1 | xyz | +0x178=n; read: word7 == 0.0 or 1.0 -> +0x378=1; write: +0x378 := 1, word7 = 1.0 |

3D write quirk [fact]: types 3 and 4 with shape 0 return right after writing word0 and do NOT advance the LLWIN cursor.

#### 6.2c Type-specific words (word -> field). "fnsel" = 0 value / 1 function etc.

| Type (list/KEY) | Words | Read (FUN_14025de80) | Write |
|---|---|---|---|
| 1 (storeWindefFric; no object list found) | 6 -> +0x8 (1..4); 9 sel: <0 -> +0xc=1,+0x10=-sel,+0x28=w7; 0 -> +0x28=w7; 1..4 -> fn var (1,4,3,0x15) fn#=w8; 5 -> 3-var fn (4,3,0x15) ints w10..12; 6/7/8 -> pairs w10,w11 -> +0x110/+0x120/+0x130. If +0x8==4: 3D w16 sel, w13 value/fn#, w17..19; 2D w14 sel, w13, w15..17 -> +0x98 block | `FUN_140262250` mirror [fact] |
| 2 MGUSER | 6 -> +0x370 (element size [inferred]) | [fact] | w6 = +0x370 |
| 3 WINTMP | 6/7 -> +0x380 (0 value +0x388 / else fn var1); 8/9 -> +0x3f8 (0 value, 1 fn var1, 2 fn var3, 3 2-var (1,3) ints w10,w11 -> +0x414,+0x418); 12/13 -> +0x470 (0,1 var1,2 var3); 16/17 -> +0x4e8 (0 value, 1 fn var1) | [fact] | `FUN_140262cf0` + w16/17 |
| 4 WINATM | 6/7 -> +0x5e0 (0,1 var1,2 var3); 8..11 -> +0x658 (0, 1 var1, 2 var 0xc, 3 2-var (1,0xc) ints w10,w11 -> +0x674,+0x678) | [fact] | `FUN_140263040` |
| 5 LOCTMP | 1 -> +0x838; 6..13 as type 3; 14/15 -> +0x4e8; 16 -> +0x560 (>0) / +0x568 (1 value +0x570, 2 fn var1, 3 fn var4), 17 value/fn# | [fact] | w1=+0x838, w2=0, w3=1.0, w4=w5=0, `FUN_140262cf0`, w14/15, w16/17 |
| 6 LOCATM | 1 -> +0x838; 6..11 as type 4; 12/13 -> +0x6d0 (0 value +0x6d8, 1 fn var1) | [fact] | header as type 5, `FUN_140263040`, w12/13 |
| 7 LOCDEF | 1 -> +0x838; 6/7 -> +0x748 (0 value +0x750, 1 fn var1); 13 -> +0x8 (1..3 else 0); 8 sel -> +0x18 (0 value w9 -> +0x28; 1..4 fn var 1,4,3,0x15; 5 3-var ints w10..12), w9 fn# | [fact] | header as type 5, `FUN_140262c00`, `FUN_140262250` (sel at w8, value/fn at w9, +0x8 at w13; sel rebuilt from +0x24/+0x30) |
| 8 WINPRS | 6/7 -> +0x748 (0 value +0x750, 1 fn var1); 8/9 -> +0x7c0 (0 value +0x7c8, 1 fn var1) | [fact] | `FUN_140262c00` + w8/9 |
| 10 WINRSE | nothing beyond header/geometry | [fact] | nothing |

Function references [fact]: a record word holds a 1-based global function number n. IIFNC is an int array
`[start1, tag1, start2, tag2, ...]`: LLFNC data of n = LLFNC[IIFNC[2n-2]-1 .. IIFNC[2n]-2]; IIFNC[2n-1] = tag -> fnobj+0x54.
Writer: n = ++model+0x10c, IIFNC[2n-1] = *obj+8 (object number), IIFNC[2n] = IIFNC[2n-2] + size, IIFNC[0] = 1 for n=1.
Data layout: 1-D table (fn+4==1): rows of (x, y1..yk) with k = fn[0x10]; code 0x7c: k values only; multi-var: optional dims, axes then values (`FUN_1401309b0` / `FUN_140131360`).


### 6.3 Per keyword

| KEY (id) | List | LLOBJ words (0-based, first..last) | Window type | KEY prints (kfw) | DB-backed | KEY-only / lost |
|---|---|---|---|---|---|---|
| WINPRS (0xe2) | obj+0x2eb0 | 3D 82..83 (read v>9999), 2D 81..82 (read v>8999) | 8 | `FUN_14029e250`/`FUN_1402ab880`: obj#, +0x170, 3D +0x210 / 2D +0x178, +0x74c!=0, +0x7c4!=0, enc(+0x368); +0x350,+0x358(,+0x360); geometry `FUN_1402f4890`; `FUN_1402f3130` +0x748, +0x7c0 | all printed fields | +0x170 regenerated; enc 2 (+0x368==1) not stored |
| WINTMP (0xe3) | obj+0x2ee0 | 60..61 | 3 | +0x170, shape/npts, +0x384!=0, sel(+0x3fc,+0x408) 0..3, sel(+0x474,+0x480), +0x4ec!=0, enc; header doubles; geometry; fns +0x380,+0x3f8,+0x470,+0x4e8 | yes | KEY writer first deletes windows without geometry (`FUN_1402632d0`, `FUN_1402696f0`) |
| WINATM (0xe4) | obj+0x2f10 | 62..63 | 4 | +0x170, shape/npts, +0x5e4!=0, sel(+0x65c,+0x668), enc; doubles; geometry; fns +0x5e0,+0x658 | yes | KEY flattens +0x5e4 to 0/1 (DB keeps var1 vs var3) |
| WINRSE (0xe5) | obj+0x2fd0 | 88..89 (read v>10999; read requires first > 0) | 10 [inferred] | +0x170, 3D +0x210 / 2D +0x178, geometry | geometry + header | — |
| LOCTMP (0xe6) | obj+0x2f40 | 75..76 | 5 | `FUN_1402ae590`: obj#, +0x170, +0x838, +0x384!=0, sel 0x3fc, sel 0x474, +0x4ec!=0, +0x560 sel (0..3); fns +0x380,+0x3f8,+0x470,+0x4e8,(+0x568) | yes | — |
| LOCATM (0xe7) | obj+0x2f70 | 77..78 | 6 | `FUN_1402aeb40`: +0x170, +0x838, +0x5e4!=0, sel 0x65c, +0x6d4!=0; fns +0x5e0,+0x658,+0x6d0 | yes | — |
| LOCDEF (0xe8) | obj+0x2fa0 | 79..80 | 7 | `FUN_1402aef30`: +0x170, +0x838, +0x74c!=0, +0x8 (1..3), sel(+0x24,+0x30) 0..5; fns +0x748, +0x20 | yes | LOC* printed via `FUN_1402ac560` |
| MGUSER (0xc2) | obj+0x2d50 (in obj+0x2d10) | 3D 25..26, 2D 44..45 | 2 | `FUN_1402a9180`, only if +0x370 > 0: 3D seq#, +0x370, +0x350..+0x360, shape (12/13 -> 11), enc, (+0x378 if shape 11); 2D +0x170, +0x178, +0x370, +0x350, +0x358, enc; geometry | yes (only if +0x370 > bed8) | 3D number is a running count; +0x378=0 lost (write forces 1) |
| BCCFNC (0xa1) | obj+0x2e68 itself (fn objects, next at +0x60; `FUN_140136860`) | none (not LLOBJ) | — (function objects, not windows) | `FUN_1402b4050`: obj#, fn+0x54 (tag), fn+0x1c (npts), table if fn+4==1 && fn+0x10==1 | yes, NOT in `FUN_140195ff0` | see below |

BCCFNC detail [fact]: read in `FUN_1401d2be0` ("ReadBdryCndFuncFromDB") -> `FUN_140177ba0(obj, IIFNC, LLFNC, nFunc, ...)`:
clears the list, collects function numbers referenced by the object's BC arrays (obj+0x998 [+0x990 code 6 -> kind 7], +0x9a8,
+0xaf0, +0xb08, +0xb20, +0xb38), replaces each array entry by IIFNC[2n-1] (the tag), and for each referenced n (kind != 7)
appends a 1-D table fn (`FUN_140136670`, fn+4=1, fn+0x10=1, `FUN_1401309b0`); kind 7 (3D only) goes to obj+0x2e80 (`FUN_140257490`).
The alternative `FUN_140178250` (ReadNodeBdryCndFuncFromDB) builds no list and is dead because `FUN_1401d2b70` returns 1 [fact per decompile].
Write: `FUN_140178460` registers each list fn whose tag is referenced (tag = list index, 1-based) via `FUN_1401bb3d0` and
rewrites the BC arrays (e.g. into IIBFD) with the new global numbers [fact].


## 7. DB words with no keyword (internal / computed)

| Word | Meaning | |
|---|---|---|
| IIOBJ 0, 3, 4..9, 12, 13 | slot state, sim flag (m+0x854), node/element/boundary ranges, element kind, IINOD offset | **[fact]** |
| IIOBJ 15..18 (3D) | fold mesh ranges (carry FLDGEO/FLWNET) | **[fact]** |
| IIOBJ 19..22 / 15..18 (2D) | WPAXIS / OPSTOP function-number ranges | **[fact]** |
| LLOBJ 25/26, 44/45, 60..63, 75..83, 88/89 | window ranges (first/last LLWIN row) | **[fact]** |
| LLOBJ 48, 49 | count and function number of an internal function obj+0x3598 (kind 0x7c = 124); no KEY writer references obj+0x3590/0x3598 | **[fact]**, meaning unknown |
| LLOBJ 65, 68, 87 when function | function numbers (FNCLOC) | **[fact]** |
| LLBND w0..w10, w39, w97..w102 | boundary record type, boundary node/face ranges, geometry ranges, nodes per face, bearing-surface items | **[fact]** |
| LLBND 3D w21..26 / 2D w20..23 | speed x direction (type 0) or force x direction (type 1) written by the movement writer; read back into the FORCE / type-11 slots | **[fact]** |
| LLBND 2D w24..29 | sim-control values copied into mv+0x1b8..0x1e0 (`FUN_1401d6030`) | **[fact]** |
| first LLBND record, words 0..99 | raw copy kept in obj+0x3008 | **[fact]** |
| LLWIN w1/w2 | LLWRZ row range | **[fact]** |

## 8. What `deformdb` needs for this class

1. Object record view: IIOBJ (width from IIAD1) and LLOBJ decoded per §2 with dimension switch (2D/3D differ from word 10 and from LLOBJ 17 on), including the int-as-real convention and the encodings (OBJTYP map, OTPRNG sign, VOLCRG 0/10/20(+1), CSFREQ 0..3, TRGVOL -1 rule, MGSIZR default 1.0).
2. A generic function-table reader/writer for IIFNC/LLFNC (FNCLOC: start/tag pairs, `FUN_1401309b0` layouts: 1-D table, 0x7c value list, multi-variable with dims) - shared with sim ctrl and materials.
3. LLBND record access per object (IIOBJ 8..9), the movement decoder of §4 (first record only on read; writer repeats it in every record) and the ROTSYM / geometry words of §5.
4. LLWIN/LLWRZ window decoder (§6): type map word0 1..9 -> 1..8,10; shape/encoding words; LLWRZ stride 3 (3D) / 2 (2D); per-keyword LLOBJ ranges and version gates (WINPRS v > 9999 3D / v > 8999 2D, WINRSE v > 10999).
5. Lists rebuilt from other data on read: BCCFNC and SYMSUF ids from node BC function references (`FUN_140177ba0`), SYMSUF planes from geometry, TPLOGY from 2D geometry; WPAXIS (12-real) and OPSTOP (20-real) LLFNC records.
6. Writer side (byte-exact GENDB): running cursors for nodes, elements, boundary records, LLWIN (m+0x134), LLWRZ (m+0x138), functions (m+0x10c), and the write-only quirks: IIOBJ[10] = 0 in 2D, IIOBJ[12] = sim value in 2D, LLOBJ 25/26 zeroed in 2D, TRGVOL real cleared when int < 0, window gating flags, 3D GENDB TRGVOL override.

## 9. Open questions

1. The brief's GENDB labels are swapped: `FUN_14017cb60` is the 2D writer and `FUN_14017b370` the 3D writer (region slots and the `FUN_1400015f0` test in `FUN_1401c4d90`).
2. OPSTOP 20-real LLFNC record layout (item vtable +0x10 / +0x18) not followed.
3. LLOBJ 48/49 function (kind 0x7c) has no keyword in the object KEY writer; owner unknown.
4. 2D IIOBJ 10 (obj+0x890) is read but GENDB always writes 0; files written by other DEFORM programs may carry a value.
5. Movement: FORCE value itself is never written (only speed x direction for type 0); OBJSTP / name tail words overlap (reader takes 80 name bytes, writer 64); type 2 v < 11000 reads the curve only when fn number > 1; xt code 26 of type 6 is lost.
6. Windows: no object list holds type-1 windows (friction windows, `FUN_140262250`); 3D types 3/4 with shape 0 do not advance the LLWIN cursor; 2D remesh writer zeroes LLOBJ 25/26 and leaves 44/45 untouched when no window is written; meaning of win+0x350..0x36c.
7. Keywords of §3.3 (MOVANG, BCCANG, MGSIZM, OBJPHS, CSPMTX, CRDSYS, GEO23, GEOSEC, MSHSEC, POLYTP, DIEPNT, DIEEDG): no writer or DB access found at the analysed depth; a KEY-reader trace would show whether they are accepted and dropped.
8. CUSVAR (list obj+0xb58) is printed through the array writer; its DB source (user-variable regions) belongs to objarrays / sim ctrl and was not followed here.

## Addendum (thread lead): POLYTP, DIEPNT, DIEEDG resolved

Followed after the main pass (`out/dbmap/geo_kw.c`). All three are **KEY-only, computed from the rigid
geometry object** (`geo = obj+0x3c00[k]`, `FUN_140198620`), never read from or written to the DB:

* POLYTP (299) `FUN_1402ab210`: `POLYTP obj n1 n2`, n1 = number of 16-byte items in the vector at `geo+0x18`
  (`FUN_140003500`), n2 = count of the list at `geo+0x68` (`FUN_140199fa0`). Written by the 3D geometry writer
  for meshed rigid objects. After DBREAD both lists are empty, so the export shows `POLYTP n 0 0`
  (checked: COG step 25 objects 2-4) **[fact]**.
* DIEPNT (300) `FUN_1402ab2b0` and DIEEDG (301) `FUN_1402ab530`: print the point / edge lists of the same
  geometry only when they are non-empty; empty after DBREAD **[fact]**.
* TPLOGY is the 2D counterpart (`FUN_1402a9f90`, see §3.3).

The remaining §3.3 keywords (MOVANG, BCCANG, MGSIZM, OBJPHS, CSPMTX, CRDSYS, GEO23, GEOSEC, MSHSEC, CUSVAR)
have no KEY writer call and no DB access, and none appears in any KEY export of the 816 cached DB steps:
they are KEY-input keywords outside the DB mapping.
