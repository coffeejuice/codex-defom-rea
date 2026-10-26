# Object arrays (keyword group 2): per-node, per-element, per-face and per-object array keywords <-> MAR/A

Scope: the array keywords of an object (nodal / element / boundary-face fields, BCs, connectivity, fold meshes,
DYNARY) and how DEF_PRE_64 v11.0.2 moves them between the keyword model and a .DB step. Scalars, LLBND movement
words, windows and rigid geometry records: see [`object.md`](object.md). Model-level widths derived from STNOUT /
ELMNOD: [`simctrl.md`](simctrl.md) (its `D[i]` is `W[i]` here).
All facts are from decompiled code (`loader_ann.c`, `gendb_ann.c`, `kfw_ann.c`); region slots are 0-based
(2D/3D) as in `region_globals.tsv`. "dim" = 2 or 3. NUMNP / NUMEL = totals over all objects.

## 0. Key structures

| Item | Meaning | Source |
|---|---|---|
| `W[id]` = m+0x15f8+4·id, id 0..0x4e | expected width per **object variable id** (`eObjVarTyp`, the switch of `KWD_writeObjStateVariables` `FUN_1402a4a80`) | init `FUN_1401d0320` (DBREAD, after ReadObjInfo) **[fact]** |
| obj+0x8a0 | node-field container; arrays at obj+0x8a8..0x968 (table §2); `FUN_140180310(c,id,w)` resizes one field | `FUN_140162ff0`, `FUN_14016dfb0` **[fact]** |
| obj+0x9e0 | element-field container; arrays at obj+0x9e8..0xb40; `FUN_140183840(c,id,w)` resizes | `FUN_140163090`, `FUN_140170ed0` **[fact]** |
| obj+0x880 / 0x884 | node / element count = IIOBJ[5]−IIOBJ[4]+1 / IIOBJ[7]−IIOBJ[6]+1 (0 if no mesh) | `FUN_14016caf0` **[fact]** |
| obj+0x3984 / 0x3988 | first global node / element (IIOBJ[4] / IIOBJ[6]) | `FUN_14016caf0` **[fact]** |
| obj+0x87c | 1 = object has mesh (IIOBJ[0]==1); every array reader returns if 0 | `FUN_14016caf0` **[fact]** |
| obj+0x894 | nodes per element (3D 4/8; 2D 4) | `FUN_140174130`, `FUN_140163090` **[fact]** |
| obj+0x3588, 0x888 | first LLBND record, number of records (surfaces / boundary loops, ≤100) | `FUN_14016caf0` **[fact]** |
| obj+0xbe0 + 0x30·k | boundary surface k: +0 node count, +8 node list, +0x10 face count, +0x18 face nodes (4/face), +0x20 face elements | `FUN_140174e40` **[fact]** |
| obj+0x398c | first boundary face (1-based, global) of surface 0 | `FUN_140174e40` **[fact]** |
| obj+0x88c | 3D: Σ face counts of all surfaces (`FUN_140165d30`); 2D: Σ loop node counts | **[fact]** |
| NBF(obj) `FUN_140165170` | boundary faces of the object: 3D obj+0x88c; 2D obj+0x88c − 2 + 2·obj+0xbd8 (0xbd8 = loop count) | **[fact]** |
| p(k) `FUN_140183c20` | integration points for ELMNOD option k (k=0 damage m+0x888, 1 strain m+0x88c, 2 stress m+0x890): option 3 → 3D 8 if nodes/elem==8 else 1, 2D 4; otherwise 1 | **[fact]** (`FUN_14019a450` = is-3D **[inferred]**) |

Model fields used (m = `_DB_STEP_DATA`): ELMNOD options 1..5 at m+0x888,+0x88c,+0x890,+0x894,+0x898 (1 damage, 2 strain,
3 stress, 5 grain); STNOUT 1..7 at m+0x86c..0x884; GEOTYP m+0x850 (2D, 3 = three velocity components); TRANS[3]
m+0x85c, TRANS[5] m+0x864; NUMNP m+0x13e8, NUMEL m+0x13ec, NBND (LLBND records) m+0x13f0, phase relations m+0x13f4,
NFN m+0x13fc; nmat m+0x1320; user node/element variable counts m+0x3398 / m+0x33a8 (= stored widths of LLND1 / LLET1,
`FUN_1401cf930`, `FUN_1401cfc30`) **[fact]**.

## 1. Read path (DBREAD) and write path (GENDB)

**DBREAD** (`FUN_1401cc420`): ReadObjInfo `FUN_1401d2230` (per object `FUN_14016caf0`, …, `FUN_140174e40` boundary
records/faces) → ReadInterObjInfo → **`FUN_1401d0320` (fills W[])** → ReadArrays `FUN_1401d2be0` → 2D `FUN_1401d5f50` /
3D `FUN_1401d25d0` (fold) → ProcessStepHeader → `FUN_1401d2760` (DYNARY, v>10999) → 3D `FUN_1401d63c0` (genGeoSymPlane) **[fact]**.

ReadArrays `FUN_1401d2be0`, per object with obj+0xc ≠ 7 **[fact]**:
1. ReadNodeInfoFromDB `FUN_14016dfb0` (A node fields), `FUN_140170410` (MAR node fields).
2. ReadElemInfoFromDB `FUN_140170ed0` (A element fields), `FUN_140174130` (IINOD, IIMTR).
3. ReadEdgeFaceInfoFromDB `FUN_140176df0` (face BCs), ReadBdryCndFuncFromDB `FUN_140177ba0` (function numbers → BCCFNC / SYMSUF).
4. After all objects: "Boundary Conversion" table rec→object, MTLGRP index→material number, PDIE record→object,
   NBCD (BCCDEF contact codes) conversion, LLBND attach-object conversion (obj+0x21c8==5 → obj+0x25a0) **[fact]**.

`FUN_1401d2b70` and `FUN_1401d2bb0` always return 1 (whole MAR/A in memory), so the region-by-region branches
(`FUN_1401d5820`, `FUN_140178250` node-only BC functions, "LLGRY/LLCRZ only" path) are dead in this build **[fact]**.

**GENDB** `FUN_1401c4d90`: `FUN_1400015f0` (is 3D) false → `FUN_14017cb60` = **2D**; true → `FUN_14017b370` = **3D**
(the brief has them swapped; slot use confirms it) **[fact]**. Per meshed object, 3D order **[fact]** (`FUN_14017b370`):
WriteNodeInfoToDB `FUN_14016f8e0` → WriteElemIntegerInfoToDB `FUN_140174cd0` → IIBND/IIBDY/IIBEL (inline) →
WriteEdgeFaceInfoToDB `FUN_1401774f0` → WriteElemInfoToDB `FUN_140172c90` → WriteNodeIntegerInfoToDB `FUN_1401707e0` →
WriteBdryCndFuncToDB `FUN_140178460` → WriteBdryInfoToDB `FUN_140175db0` (LLBND) → general info `FUN_14016cf10` (IIOBJ/LLOBJ)
→ fold `FUN_14016da70` (non-rigid, fold flag) → movement / windows / BC lists. 2D `FUN_14017cb60`: LLBND records
(`FUN_140176830`, one per loop) first, then node, elem-int, edge-face, elem, node-int, BC-func writers.
After all objects: PDIE record (`IISIM`), BCCDEF contact conversion `FUN_14017e190`, MTLGRP material renumbering **[fact]**.
Then `FUN_1401c4a30` / `FUN_1401c4be0` (user variable names, §8) and `FUN_1401c5260` (DYNARY, §9).

Running offsets (GENDB model context, `FUN_140199ce0`/`FUN_140199d20`/`FUN_14019a0c0`/`FUN_14019a1c0`/`FUN_14019a220`):
node offset +0x114, element offset +0x118, IIBND offset +0x11c, boundary-face offset +0x130, LLBND record counter +0x110;
NUMNP +0x6c, NUMEL +0x70 **[fact]**. So firstNode/firstElem/first face/first record are **computed on write** (object order,
meshed objects only), never copied from the read step.

### 1.1 Generic rules

Read (A node field id, region R): `if W[id] == width(R)` → copy `A[start(R)−1 + (firstNode−1)·W[id] + i]`, i < nodes·W[id].
Element fields use firstElem / elements. **A width mismatch silently skips the field** (array stays as allocated)
**[fact]** `FUN_14016dfb0`, `FUN_140170ed0`. Exceptions are listed per row ("adopt" = W[id] := stored width, field resized).
Write: `A[start(R)−1 + offset·W[id] + i] = field` with W[id] of the writing model, unconditionally unless noted;
the address-table widths are set independently by `FUN_1401c03f0` (§3) **[fact]** `FUN_14016f8e0`, `FUN_140172c90`.

## 2. Keyword map (eObjVarTyp id = argument of `FUN_1402a4a80`)

Region = MAR/A slot (2D/3D, 0-based). W init = `FUN_1401d0320`; G3 = 2D with GEOTYP 3. "rd"/"wr" conditions and
transforms; all **[fact]** in the cited functions unless marked.

### 2.1 Node fields (A, rows = nodes; read `FUN_14016dfb0`, write `FUN_14016f8e0`, KEY `FUN_1402a4a80`)

| id | KEY | obj | Region | W init | Read | Write | Notes |
|---|---|---|---|---|---|---|---|
| 00 | RZ | +0x8a8 | LLCRZ 11/11 | dim | strict | copy | KEY via `FUN_1402a2db0` (2D non-G3 `FUN_1402a2fc0`) |
| 01 | DRZ | +0x8b0 | LLDRZ 12/12 | dim | strict | copy | |
| 02 | URZ | +0x8b8 | LLURZ 13/13 | dim (G3 3) | strict | copy | |
| 03 | FRZ | +0x8c0 | LLPFN 15/15 | dim (G3 3) | strict | copy | |
| 04 | *FRZB | +0x8c8 | LLCFN 16/16 | dim (G3 3) | strict | **not written** | KEY comment line "*FRZB" |
| 05 | PRZ | +0x8d0 | LLPPR 17/17 | dim | 3D only; strict or stored 0 | 3D only | 2D table width 0 |
| 06 | *PRZB | +0x8d8 | LLCPR 18/18 | dim | strict | **not written** | |
| 08 | NDTMP | +0x8e0 | LLTMP 20/20 | 1 | strict | copy | 2D KEY writes pairs (T−ΔT, T) using id 0x0c |
| 09 | NDHEAT | +0x8e8 | LLPHT 21/21 | 1 | strict | copy | |
| 0a | *HITN | +0x8f0 | LLHIT 22/22 | 1 | strict | **not written** | |
| 0b | NDFLUX | +0x8f8 | LLFLX 23/23 comp 1 | 1 | stored 1, 0 or 3 | copy at comp offset 0 | component-major, see 0x10/0x0e |
| 10 | RHTFLX | +0x920 | LLFLX comp 2 | 1 | only if stored width 3: `A[start−1+NUMNP+firstNode−1+n]` | always written at +NUMNP | |
| 0e | CRBFLX | +0x910 | LLFLX comp 3 | 1 | stored width 3: +2·NUMNP | always written at +2·NUMNP | |
| 0c | *DTMP | +0x900 | LLDTM 58/55 | 1 | 2D only; start(LLDTM) < A length and (strict or stored 0) | 2D only | temperature increment |
| 0d | DATOM | +0x908 | LLATM 26/26 | 1 | strict | copy | |
| 0f | VOTAGE | +0x918 | LLVLT 25/25 | 1 | strict | copy | |
| 11 | USRNOD | +0x968 | LLND1 24/24 | m+0x3398 (= stored width) | strict | if field ≠ null | names after the data, §8 |
| 18 | NWEAR | +0x928 | LLWER 27/27 | 5 | strict on 5; row stride = stored width | if field ≠ null and table width > 0 | 3D `FUN_1401d2230` sets W[0x18] = stored width but `FUN_1401d0320` runs later and resets 5 **[fact]** order `FUN_1401cc420` |
| 19 | DRMESH | +0x930 | A slot 19 (3D LLDRM) | dim | 3D; strict (literal slot 0x13) | 3D | |
| 1d | GRAIN (nodal) | +0x950 | LLGSZ 45/46 node rows | `FUN_1401d64f0(0x1d)` | ELMNOD[5]==1: adopt stored width; copy if > 0 | ELMNOD[5]==1 | active when a material has grain model +0x17e8 == 2 (then ELMNOD[5] := 1); KEY GRAIN2 (id 209) when `FUN_1401d7160` == 2 |
| 22 | TIMERC | +0x958 | LLTRC 61/58 | m+0x11c4 | v>10999; adopt; copy if > 0 | if W>0 and field ≠ null | |
| 23 | NRECVL | +0x960 | LLNRV 63/61 | m+0x12b4 | v>10999; adopt | if W>0 and field ≠ null | |

ELMNOD option 2 nodal blocks (rows NUMEL + global node, after the element rows of the same region):

| id | KEY | obj | Region | W init | Read | Write |
|---|---|---|---|---|---|---|
| 1a | STRESS (nodes) | +0x948 | LLSTS 29/29 | 2·dim (G3 6); 0 if ELMNOD[3] ∈ {0,3} | strict: `A[start−1+(NUMEL+firstNode−1)·W]` | ELMNOD[3]==2: `A[start−1+NUMEL·W[0x26]+nodeOff·W[0x1a]]` |
| 1b | STRAIN (nodes) | +0x938 | LLTEP 34/34 | 1; 0 if ELMNOD[2] ∈ {0,3} | strict | ELMNOD[2]==2, element stride W[0x2b] |
| 1c | DAMAGE (nodes) | +0x940 | LLDMG 38/38 | m+0x33c8; 0 if ELMNOD[1] ∈ {0,3} | strict, else if ELMNOD[1]==2 and W < stored: adopt | ELMNOD[1]==2, element stride W[0x2d] |

Reader and writer agree only when the element width (W[0x26]/W[0x2b]/W[0x2d]·p) equals the node width; for damage
W[0x2d]=m+0x33d8 and W[0x1c]=m+0x33c8 may differ **[inferred]**. KEY: `FUN_1402f60c0` writes element + node rows when
the nodal field exists.

### 2.2 Node integer fields (MAR; read `FUN_140170410`, write `FUN_1401707e0`)

| id | KEY | obj | Region | W init | Read | Write transform |
|---|---|---|---|---|---|---|
| 12 | BCCDEF | +0x990 | IIBCD 9/7 | dim (G3 3) | strict or stored 0 | copy; then contact codes converted (§6) |
| 13 | BCCDFN | +0x998 | IIBFD 10/8 | dim (G3 3) | strict or stored 0 | function numbers validated and replaced by **global** function numbers (§5) |
| 14 | BCCTMP | +0x9a0 | IIBCT 11/9 comp 1 | 1 | stored 1, 0 or 3; stride 1 | copy, **negative → 2** |
| 17 | BCCRHT | +0x9b8 | IIBCT comp 2 (+NUMNP) | 1 | only if stored width 3 | copy at +NUMNP, negative → 2 |
| 16 | BCCCRB | +0x9b0 | IIBCT comp 3 (+2·NUMNP) | 1 | only if stored width 3 | copy at +2·NUMNP, negative → 2 |
| 15 | BCCTFN | +0x9a8 | IIBFT 12/10 | 1 | strict or stored 0 | global function numbers (§5) |

On read, function references are mapped back to the object's local list by `FUN_140177ba0` (§5) and BCCDEF contact
codes in the "NBCD conversion" of `FUN_1401d2be0` (§6). 3D read only: a node whose 3 BCCDEF codes are equal and
negative becomes (c−200, 0, 0) **[fact]**.

### 2.3 Element fields (A, rows = elements; read `FUN_140170ed0`, write `FUN_140172c90`)

| id | KEY | obj | Region | W init | Read | Write |
|---|---|---|---|---|---|---|
| 26 | STRESS | +0xa00 | LLSTS 29/29 | 2·dim (G3 6) | stored>0: stored == p(2)·W → copy p·W per elem; else if p==1 and stored/W == 8 → take integration point 1 | stored == p·W → copy; else if p==1 and stored/W==8 → **replicate** to 8 points |
| 27 | *EPS | +0xa08 | LLEPS 30/30 | 2·dim (G3 6) | strict | **not written** |
| 28 | STNCMP | +0xa10 | LLTSR 31/31 | STNOUT6+STNOUT7 + max(1, ΣSTNOUT1..5)·2·dim | strict | copy W[0x28] |
| 29 | *EFSTS | +0xa18 | LLFTS 32/32 | 1 | strict | **not written** |
| 2a | *EFEPS | +0xa20 | LLFEP 33/33 | 1 | strict | **not written** |
| 2b | STRAIN | +0xa28 | LLTEP 34/34 | 1 | as STRESS with p(1) | as STRESS |
| 2c | DENSTY | +0xa30 | LLRDT 35/35 | 1 | strict | copy |
| 2d | DAMAGE | +0xa38 | LLDMG 38/38 | m+0x33d8 | as STRESS with p(0) | as STRESS |
| 2e | MATAXI | +0xa40 | LLMTA 28/28 | 3D 9 if `FUN_1401d64f0(0x2e)` else 0; 2D 1 | strict | if W ≠ 0 and field ≠ null |
| 30 | *HTGN | +0xa50 | LLHGN 37/37 | 2 | strict (never matches: table width 1, region sized NUMNP) | **not written** |
| 31 | VOLFC | +0xa58 | LLVLF 40/41 | nmat or 0 (`FUN_1401d64f0(0x31)`) | strict | if obj+0x3590>0: copy, or remapped to phase count `FUN_140199c10` when ≠ nmat (`FUN_140160980`, `FUN_1401607f0`) |
| 32 | VOLFS | +0xa60 | LLVLN 42/43 | nmat or 0 | strict | as VOLFC |
| 33 | DVOLF | +0xa68 | LLDVF 41/42 | m+0x13f4 or 0 | strict | if obj+0x3590>0, table width>0 and equal; mismatch → "Field width of DVOLF doesn't match" in DEF_PRE.ERR |
| 34 | TICF | +0xa70 | LLICB 44/45 | m+0x13f4 or 0 | strict | as DVOLF ("TICF") |
| 35 | GRAIN | +0xa78 | LLGSZ 45/46 elem rows | `FUN_1401d64f0(0x35)` | ELMNOD[5]==0 and strict | ELMNOD[5]==0, equal width else DEF_PRE.ERR |
| 36 | SIZESH | +0xa80 | LLSIZ 46/47 | `FUN_1401d6c60` | v>10999; adopt | if W>0 and field ≠ null |
| 37 | TXTODF | +0xa88 | LLODF 60/57 | `FUN_1401d6b30` | v>10999; adopt | if W>0 and field ≠ null |
| 38 | HDNOBJ | +0xa90 | LLHDN 43/44 | 2 | strict | copy |
| 39 | YLDS | +0xa98 | LLYLS 50/49 | 2·dim (G3 6) | strict | copy |
| 3a | ZEFI | +0xaa0 | LLZEI 51/50 | 2 | strict | copy |
| 3b | ZMFI | +0xaa8 | LLZMI 59/56 | 3D 3, 2D 2 | v>10999; strict | copy |
| 3c | USRELM | buffer `FUN_140184a30`(obj+0x9e0) | LLET1 39/40 | m+0x33a8 (= stored width) | strict | if buffer ≠ null; names §8 |
| 3e | CURRNT | +0xab0 | LLCUR 47/60 | dim | 2D always; 3D v>10999; strict | copy |
| 3f | ERECVL | +0xab8 | LLERV 62/59 | m+0x123c | v>10999; adopt | if W>0 and field ≠ null |

KEY: ids 0x26/0x2b with ELMNOD option 3 print each integration point (8 / 1 for 3D brick/tet, 4 in 2D); DAMAGE uses
`FUN_1402f58d0` with the same point counts **[fact]** `FUN_1402a4a80`.

### 2.4 Element integer fields (MAR; read `FUN_140174130`, write `FUN_140174cd0`)

| id | KEY | Region | Read | Write |
|---|---|---|---|---|
| 25 | ELMCON (obj+0x9e8) | IINOD 6/4 | 3D: `MAR[start−1 + IIOBJ[13]−1 + i]`, i < elems·obj+0x894; 2D: `MAR[start−1 + (firstElem−1)·4 + i]`, i < elems·4; value − firstNode + 1 (global → local); no width check | at running offset (start(IINOD) + Σ previous elems·nodes/elem); value + nodeOffset; 3D IIOBJ[13] = offset − start + 1 (`FUN_14016cf10`) **computed** |
| 3d | MTLGRP (+0x9f8) | IIMTR 7/5 | strict (W 1) or stored 0; then DB value = **position in material list** → material number (`FUN_1401d2be0`, error "converting MTLGRP") | copy at elemOffset, then all NUMEL entries renumbered material number → position (`FUN_14029ba10` table) **computed** |

W[0x25] = 2D 4, 3D 0 (connectivity width comes from obj+0x894) **[fact]** `FUN_1401d0320`.

### 2.5 Element-face boundary conditions (per boundary face; read `FUN_140176df0`, write `FUN_1401774f0`)

| id | KEY | obj | Region | Read (MAR: stored width 1; A: 1 or 0) | Write |
|---|---|---|---|---|---|
| 41 | ECCDEF | +0xae8 | IIEDE 14/17 | copy | copy |
| 44 | ECDEFN | +0xaf0 | IIEDF 17/20 | copy, then → local function (§5) | global function number where ECCDEF ≠ 9 |
| 43 | ECPRES | +0xaf8 | LLPRE 53/51 | copy | copy |
| 42 | ECCTMP | +0xb00 | IIETM 15/18 | copy | copy |
| 45 | ECTMFN | +0xb08 | IIETF 18/21 | → local fn | global fn where ECCTMP ≠ 4 |
| 46 | ECHFLX | +0xb10 | LLHFL 54/52 | copy | copy |
| 47 | ECCATM | +0xb18 | IIEAT 16/19 | copy | copy |
| 48 | ECATFN | +0xb20 | IIEAF 19/22 | → local fn | global fn where ECCATM ≠ 4 |
| 49 | ECAFLX | +0xb28 | LLAFL 55/53 | copy | copy |
| 4a | ECCRHT | +0xb30 | IIERH 20/24 | 2D any; 3D v>10999 | only if `FUN_1401f3cb0`(sim ctrl) > 0 |
| 4b | ECRHFN | +0xb38 | IIERF 21/25 | same, → local fn | same; global fn where ECCRHT ≠ 4 |
| 4c | ECRFLX | +0xb40 | 2D LLRFL 48; 3D A slot 48 (`R_LLRFL`, named LLSIZ in DEFORM's table, spec §6.1) | same (width 1 only) | same |

Read: version gate 3D v ≥ 4000, 2D v ≥ 7000; rows `start−2+obj+0x398c+i`, i < NBF(obj); arrays sized NBF by
`FUN_1401631e0` **[fact]**. Write: offset = 3D model face counter (+0x130), 2D LLBND[3] of the object's first loop − 1;
count = 3D faces of surface 0 only (obj+0xbf0), 2D obj+0x88c − 2 + 2·loops **[fact]** `FUN_14017b370`, `FUN_14017cb60`.
For a 3D object with several boundary surfaces the reader expects NBF = Σ faces while the writer writes surface 0's
count **[inferred]** (normally one surface). Table widths: 3D 1 for all; 2D 0 for LLPRE/LLHFL/LLAFL (reader accepts 0)
**[fact]** `FUN_1401c03f0`. KEY writer `FUN_1402abf20` (only if mesh and boundary loops exist), lines via
`FUN_1402f6600` (MAR) / `FUN_1402f6a10` (A).

## 3. Widths written to the address table (GENDB `FUN_1401c03f0`) vs W[]

| Region | Table width written | Reader expects (W) | Consequence |
|---|---|---|---|
| IIBCD, IIBFD, IIBFT, IINOD, IIMTR | 0 | dim / 1 | reader accepts 0 |
| IIBCT | 3 | 1 (+3 accepted) | 3 components always written |
| LLFLX | 3 | 1 (+0/3 accepted) | 3 components always written |
| LLWER | 5 if m+0xe4 ≠ 0 else 0 | 5 | |
| LLSTS / LLTEP / LLDMG | p(2)·6 (2D: p·4, G3 p·6) / p(1) / W[0x2d]·p(0) | p·W | p from `FUN_1401cafe0` |
| LLTSR | 2·dim·ΣSTNOUT1..5 + STNOUT6 + STNOUT7 | same with max(1,Σ) | differs only if Σ = 0 (sim ctrl forces STNOUT5 = 1 then, see simctrl.md) |
| LLHGN | 1 (region sized NUMNP) | 2 (element) | *HTGN never transferred |
| LLND1 / LLET1 | m+0xdc / m+0xd8 (user var counts) | adopted | regions NUMNP·w + 8·w·NOBJ / NUMEL·w + 8·w·NOBJ (`FUN_1401c1980`) |
| LLVLF, LLVLN / LLDVF, LLICB | m+0x78 / m+0xbc if m+200 > 0, else 0 | nmat / relations | |
| LLGSZ | `FUN_1401d70c0` by grain model (`FUN_1401d7160`) | `FUN_1401d64f0` | |
| LLSIZ, LLODF, LLTRC, LLERV, LLNRV | W[0x36], W[0x37], W[0x22], W[0x3f], W[0x23] of the writing model | adopted | |
| LLMTA | W[0x2e], or 1 if 0 | W[0x2e] | |
| LLPPR | 3D 3, 2D 0; A slot 19: 3D 3 (LLDRM), 2D 0 | | |
| LLCUR / LLZMI | 3D 3, 2D 2 | dim / 3D 3, 2D 2 | |
| IIEDE..IIERF, LLRFL | 1 | 1 | |
| LLPRE, LLHFL, LLAFL | 3D 1, 2D 0 | 1 or 0 | |
| 3D LLFLD (A 14) / IIFLD (MAR 23) | 3 / 4 when fold flag==1 and fold counts > 0, else 0 | — | §7 |
| 2D slots 56/57 (LLUEN/LLUNN) | 8 | — | user variable names §8 |

Region sizes: `FUN_1401c1980` (3D) / `FUN_1401c2d10` (2D). ELMNOD-dependent rows: option 0 or 3 → NUMEL, option 2 →
NUMEL+NUMNP, option 1 → 0 rows (not handled) **[fact]**; LLGSZ rows NUMNP if ELMNOD[5]==1 else NUMEL.
A slot 47 (3D, `R_LLSIZ`) is sized NUMEL·w and slot 48 (`R_LLRFL`) NBF·w, confirming spec §6.1 **[fact]**.

## 4. Boundary faces: IIBDY, IIBEL, IIBND (LLBND links)

LLBND record words (0-based reals; full record in [object.md §5](object.md)) that link to face arrays **[fact]**
(read `FUN_140174e40`, write 3D `FUN_140175db0`, 2D `FUN_140176830`):

| 3D word | Content | 2D word | Content |
|---|---|---|---|
| 0 | record type (1 geometry only, 2 rigid mesh+geometry, 3 deformable) | 0 | type |
| 1 | surface node count (obj+0xbe0) | 1 | 1 for the loop returned by `FUN_1401ffa90` (outer loop **[inferred]**), else 0 |
| 2 | first IIBND word (1-based) — **written, not read** | 2 | loop node count |
| 3 | face count (obj+0xbf0) | 3 | first IIBDY word (1-based); also face offset of the face BCs |
| 4 | first face (1-based) = IIBDY face / IIBEL index; record 0 → obj+0x398c | | |
| 10 | nodes per face: 3.0 if nodes/elem==4 else 4.0 | | |

| Array | 3D layout | 2D layout | Read | Write |
|---|---|---|---|---|
| IIBDY 4/14 | 4 global node numbers per face, face f at `4(f−1)` | one global node per loop entry (node list + closing node) | value − firstNode + 1 into surface face list (3D) / node list (2D) | local + nodeOffset |
| IIBEL 5/15 | element per face | element per edge (`FUN_140167900`, min 1) | 3D: value − firstElem + 1; 2D: **not read** | element + elemOffset |
| IIBND 16 (3D) | boundary node list of the surface | — | **not read** | surface node list + nodeOffset |

Face numbering, counts and IIBND/IIBEL contents are **computed on write** from the in-memory boundary (3D faces from
the surface lists; 2D edges from the loop node lists, closing node repeated) **[fact]**. 3D rigid geometry (LLGRY 3
reals/point, IIDOD 4 ints/facet + 1 int/facet) and 2D geometry (6-real segments in LLGRY) are in object.md §5.

## 5. BC functions: BCCFNC, SYMSUF and the function-number fields

Read `FUN_140177ba0` (after the face BCs) **[fact]**: for each entry f > 0 (≤ NFN) of BCCDFN, BCCTFN, ECDEFN
(ECCDEF ∉ {0,9}), ECTMFN, ECATFN, ECRHFN (code ∉ {0,4}) the field becomes `IIFNC[2f−1]` (int at 0-based index 2f−1 of
the IIFNC region, the per-object function number) and function f is marked with a kind (1 BCCDFN, 7 BCCDFN with BCCDEF
== 6, 2 BCCTFN, 3..6 face kinds). Then the object's BCCFNC list (obj+0x2e68) and, in 3D, the SYMSUF list (obj+0x2e80)
are rebuilt: kind 7 → symmetry plane object with id `IIFNC[2f−1]` (3D only), other kinds → new function loaded from
global record f by `FUN_1401309b0(func, IIFNC, LLFNC, f)` **[fact]**.

Write `FUN_1401707e0` / `FUN_1401774f0` / `FUN_140178460` **[fact]**: references above the object's function capacity
(`FUN_140165230` = max(NBF, nodes) + 1000 slots) are zeroed; used local functions are flagged; each flagged BCCFNC
function is appended to the global table (`FUN_1401bb3d0` allocates the global number and LLFNC position,
`FUN_140131360` writes its data); IIBFD (BCCDEF ≠ 6), IIBFT, IIEDF, IIETF, IIEAF, IIERF are then overwritten with the
**global** numbers. 3D: flagged symmetry planes are written as function type 6 (`FUN_1402574d0`) and IIBFD entries
with BCCDEF == 6 receive their global numbers. KEY BCCFNC: `FUN_1402b4050` per list entry; SYMSUF: `FUN_1402a3880`.

## 6. Computed conversions of codes

* **BCCDEF contact codes** (negative = contact with an object). Memory: −object (3D also −(200+object) = all DOF).
  DB: −LLBND record. Write `FUN_14017e190` over all NUMNP·dim codes: single-record object → −first record; 2D
  multi-loop → −(first + loop containing the node, `FUN_140166e60` on LLCRZ x,y); 3D multi-surface → −first record
  whose type word < 3.0; 3D −(200+obj) → −(200+record) and the node's other components equal to −obj are zeroed.
  Read (`FUN_1401d2be0`): record → object via the record→object table; values outside 1..NBND → 0 with message
  "Object %d Node %d has wrong NBCD Code(%d). Initialized." **[fact]**
* **BCCTMP/BCCRHT/BCCCRB**: negative values are written as 2 (information lost) **[fact]** `FUN_1401707e0`.
* **MTLGRP**: §2.4. **PDIE** and LLBND "attached object" words use the same record↔object conversion (object.md).

## 7. Fold meshes: FLDGEO / FLWNET (3D)

Read `FUN_1401d25d0` only when m+0x1734 ≠ 0; m+0x1734 = (int)H[9] (step header real 9, 1-based) for 3D entries
(`FUN_1401cca80`), else 0 **[fact]** — so H[9] in 3D is the fold flag (written from model+0xcc via `FUN_1401e2180`
**[inferred]**). Per object `FUN_14016d300` with IIOBJ words 15/16 (first/last fold node) and 17/18 (first/last
fold face), requiring both ranges and the LLFLD and IIFLD table widths > 0 **[fact]**:

* nodes: `A[start(LLFLD)−1 + (first−1)·3 + i]`, 3 reals per node (A slot 14);
* faces: `MAR[start(IIFLD)−1 + (first−1)·4 + i]`, 4 ints per face (MAR slot 23); words 1–3 − (firstFoldNode−1);
  word 4: ≤ 0 → + offset, > 0 → − offset;
* faces with word 4 > 0 → FLDGEO (obj+0x3328), < 0 → FLWNET (obj+0x3458); mixed → split by `FUN_1402fa100` **[fact]**
  (sign meaning **[inferred]**: marker of the flow-net part).

Write `FUN_14016da70` (3D, object not rigid, model fold flag ≠ 0): IIOBJ[15..18] = running fold node/face ranges
(computed), FLDGEO and FLWNET merged into one mesh (`FUN_1402fa4f0`), node numbers + offset, word 4 sign-preserving.
KEY: FLDGEO/FLWNET by `FUN_1402a2b70` (ids 289/290) **[fact]**.

## 8. User variable names (USRNOD / USRELM)

Names (64 chars = 8 reals each) follow the data: 3D at `A[start(LLND1)−1 + NUMNP·nUsrNod]` and
`A[start(LLET1)−1 + NUMEL·nUsrElm]`; 2D in A slots 57 (node) / 56 (element) **[fact]** `FUN_1401c4a30`,
`FUN_1401c4be0`, `FUN_1401cf930`, `FUN_1401cfc30`. Writer: one block, blank-padded, truncated to 64. Reader: walks one
block per object but keeps the first non-empty name per variable (byte-order fix `FUN_140324020`). Stored in
m+0x33a0 / m+0x33b0 (model-level, not per object).

## 9. DYNARY (dynamic arrays)

Read `FUN_1401d2760` (v>10999): real table rows → `FUN_14025bdc0`, int rows → `FUN_14025bd10` on obj+0xba8 of the
object number in row word 5 (`FUN_1401d6ea0`); item = (class w6, type w7, rows = w1/w8, components w8) **[fact]**.
Write `FUN_1401c5260`: `FUN_1401c53d0` collects items of all objects (obj+0xc ≠ 7), descriptor `FUN_1403bc2c4`,
then per object `FUN_14025be70` → `FUN_14025b780`: length `FUN_14025b410`, owner = object number (obj+8),
class/type/components = item+0xc/+0x18/+0x28 → words 6/7/8; int items through `FUN_1403bcaa6`, real items through
`FUN_1403bcda9` (first argument = operation: 1 read, 2 write); slot numbers = write order, counted separately for int
and real **[fact]**. All values are copied from the in-memory items (nothing recomputed but slot numbers and lengths).
KEY: `FUN_1402a41c0` (class 1 real type 1 coordinates 2/3 comps; int type 2 connectivity 3/4/8; real type 3 state
variables).

## 10. Other loader steps in scope

* `FUN_1401d5f50` (2D): copies 6 values from the primary-die object (obj+0x2318..0x2340) into m+0x8d0..0x908; no DB
  access **[fact]**.
* `FUN_1401d63c0` genGeoSymPlane (3D): `FUN_1402059a0` per rigid-geometry object rebuilds the geometry's symmetry
  plane list from the facets (planar test); no DB access, **computed** **[fact]**.

## 11. Keyword status

**Mapped (read and written)** **[fact]**: RZ, DRZ, URZ, FRZ, PRZ (3D), DRMESH (3D), NDTMP, NDHEAT, NDFLUX, RHTFLX,
CRBFLX, USRNOD, VOTAGE, DATOM, NWEAR, TIMERC, NRECVL, BCCDEF, BCCDFN, BCCTMP, BCCTFN, BCCRHT, BCCCRB, STRESS, STNCMP,
STRAIN, DENSTY, DAMAGE, MATAXI, USRELM, VOLFC, VOLFS, DVOLF, TICF, GRAIN, GRAIN2, HDNOBJ, ZEFI, YLDS, CURRNT, SIZESH,
ZMFI, TXTODF, ERECVL, ELMCON, MTLGRP, ECCDEF, ECDEFN, ECCTMP, ECTMFN, ECCATM, ECATFN, ECCRHT, ECRHFN, ECPRES, ECHFLX,
ECAFLX, ECRFLX, BCCFNC, SYMSUF, FLDGEO, FLWNET, DYNARY (63), plus pseudo-keyword *DTMP (2D).

**Read only (not written by GENDB)**: *FRZB, *PRZB, *HITN, *EPS, *EFSTS, *EFEPS (KEY comment lines); the fields keep
whatever the region holds **[fact]** (absent from `FUN_14016f8e0` / `FUN_140172c90`).

**Never transferred**: *HTGN (width mismatch 2 vs 1 and not written) **[fact]**.

**KEY-only** **[fact]**: CUSVAR (id 0x4e, obj+0xb58 list; only constructed in DB code, `FUN_1402b7310`).

**Unreached**: BCCANG (id 164: no case in `FUN_1402a4a80`, no KEY writer, no DB code uses region IIANG); TPLOGY,
POLYTP, DIEPNT, DIEEDG (geometry helpers `FUN_1402ab160/210/2b0/530`, `FUN_1402a9f90` not decompiled at this depth;
geometry storage itself is in object.md §5); HDNEST, WPAXIS, OPSTOP, RSEDEF are not arrays (object.md).

## 12. DB words with no keyword

| Location | Meaning | Status |
|---|---|---|
| IIOBJ 4–9, 13, 15–18 | node/element/LLBND/connectivity/fold ranges, recomputed per write | **[fact]** |
| IIBND (3D) | surface boundary node lists (not read back) | **[fact]** |
| IIBEL (2D) | edge → element (not read back) | **[fact]** |
| LLBND 1–4, 10 | boundary node/face counts and first indices, nodes per face | **[fact]** |
| H[9] (3D) | fold info flag | **[fact]** read / **[inferred]** write |
| LLND1/LLET1 tails, 2D A 56/57 | user variable names | **[fact]** |
| IIFNC/LLFNC entries appended per object | BC functions and symmetry planes (type 6) | **[fact]** |

## 13. What `deformdb` needs for this class

1. W[id] logic of `FUN_1401d0320` (ELMNOD, STNOUT, GEOTYP, material/phase counts) to reproduce DEFORM's
   "read only if widths match" behaviour, or at least report mismatches; adopt rules for SIZESH, TXTODF, ERECVL,
   TIMERC, NRECVL, nodal DAMAGE/GRAIN.
2. Integration-point handling: p from ELMNOD 3 + nodes/elem; 8→1 point read (point 1) and 1→8 replication on write.
3. Component-major IIBCT / LLFLX (3 comps: BCCTMP/BCCRHT/BCCCRB, NDFLUX/RHTFLX/CRBFLX).
4. Boundary model: LLBND words 1–4/10, IIBDY/IIBEL (3D faces, 2D loops), NBF formula, face-BC arrays indexed by face.
5. Function references: DB stores global IIFNC/LLFNC numbers; local = IIFNC[2f−1]; writer must rebuild the global
   function table and renumber (incl. symmetry planes, BCCDEF == 6).
6. Code conversions: BCCDEF contact −record ↔ −object (incl. −200 form), MTLGRP position ↔ material number,
   BCCTMP-type negatives → 2 on write.
7. Writer must compute all offsets (firstNode/Elem, faces, IIBND, LLBND record numbers, IINOD offset, fold ranges),
   region widths per §3 and names blocks; fold meshes (3D, H[9]) and DYNARY items with owner object number.
8. Not yet in deformdb: element-face BCs, boundary faces, fold meshes, user variable names, ELMNOD option-2 nodal
   blocks for strain/damage, *DTMP.

## 14. Corrections to db-format-spec.md §8.4

* HTGN: LLHGN is read as an **element** field of width 2 (W[0x30]); the region is sized NUMNP with table width 1, so
  the read never matches and GENDB never writes it.
* NWEAR default width 5 (W[0x18]); FRZB/PRZB/HITN/EPS/EFSTS/EFEPS are read but not written.
* LLGSZ node rows are KEY GRAIN2 (id 209), element rows GRAIN.
* `FUN_14017b370` is the 3D object writer, `FUN_14017cb60` the 2D one.
