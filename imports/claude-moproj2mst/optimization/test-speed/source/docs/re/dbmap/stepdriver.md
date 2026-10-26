# Step frame: DBREAD / GENDB drivers, step header, directory, DB header, address tables

Scope: the step-level frame around the keyword classes of DEF_PRE_64 (v11.0.2). Recovered from decompiled
code only (`loader_ann.c`, `gendb_ann.c`, `gendb_d3.c`, `kfw_ann.c`, `bm_cmd.c`, `bm_main.c`, plus
`FUN_1401d7f40`, `FUN_1401e3b60`, `FUN_1401e3d40` decompiled for this note). **[fact]** = read in code,
**[inferred]** = interpretation.

Conventions: `m` = model (`_DB_STEP_DATA`), `S` = sim-control sub-object at `m+0x750` (so `S+x` = `m+0x750+x`),
`obj` = object (`FUN_14017aa80(m+0x1380, n)`), `db` = DB object (`m+0x3390`, or a local one in GENDB).
`H[i]` = step-header real, **0-based** (spec §7 numbers it i+1; same convention as `simctrl.md`).
Directory entry in memory (`db+0xad8`, 0x28 bytes, built by `FUN_1401dc3f0`): `+0` MeshNo, `+4` StepNo,
`+8` HSize, `+0xc` MARSize, `+0x10` ASize, `+0x14` SimNo, `+0x18` SimMode, `+0x1c` dim, `+0x20` version,
`+0x24` "has arrays" flag (= MARSize >= 1) **[fact]** (`FUN_1401dc3f0`).
Licence checks met on these paths (dim licence test in `FUN_1401cc420`, header of `FUN_1403b5a04`,
TRANS module checks in `FUN_1401bc230`): licence call, omitted.

---

## 1. Read path (DBREAD)

### 1.1 Entry points and arguments

| Entry | What it passes | Evidence |
|---|---|---|
| KEY command `DBREAD` (registry id 2, case 2 of `FUN_140378e00`) | keyword line: one int = requested step `N`; next line: DB path. Ignored when `m+0x1740 & 1`. Path saved to `m+0x1748`. `db` = `m+0x3390`, opened by `FUN_1401dc3f0(db, path, 0)` | `bm_cmd.c` case 2 **[fact]** |
| | `N == 0`: last directory entry with arrays (`+0x24 != 0`), its StepNo ("Read the last step"); none -> stays 0 and `FUN_1401cc2a0` fails | **[fact]** |
| | `N != 0`: `FUN_1401e3b60(db, N)` scans entries from the last one down and returns the StepNo of the first entry that has arrays and `key(StepNo) <= key(N)`, `key(s) = s` for s >= 1, `key(s) = -s - 0.5` for s < 1 (constants -1.0 / 0.5 at `0x140522fc8` / `0x14051f240`); 0 if none. So `-n` sorts between `n-1` and `n`; prints "Read step %d (<- %d)" when it changed | `FUN_1401e3b60`, `FUN_1401e3d40` **[fact]** |
| | then `FUN_1401cc2a0(m, db, step, 1, 0)`; on success `m+0x340` gets a file name derived from `m+0x1748` | **[fact]** |
| Menu 2 "Database input" `FUN_14000b170` | prompts DB name (default `m+0x340`, ".DB" appended when missing, constant `0x140509204`), opens with `FUN_1401dc3f0(db,path,0)`, prints title, version (`db+0xabc`/1000), release/initial/current dates (`db+0xaa1/0xaa9/0xab1`), NUMRST (`db+0xacc`), StepNo of every entry with arrays; prompt "Step Number = <last> ?"; typed int (no `FUN_1401e3b60` mapping) -> `FUN_1401cc2a0(m, db, step, 1, 0)` | **[fact]** |
| KFWRIT reload branch (`kfwrit_d2.c`) | `FUN_1401cc2a0(tmp, db, m+0x744, 0)` only when `m+0x1b5a` is set; DBREAD always leaves it 0 in this build (§1.4) -> unreached | **[fact]** |

### 1.2 `FUN_1401cc2a0(m, db, step, readHdrCache, keepRaw)` - step lookup

1. If `m+0x1b5d` set: `m+0x3390 = db` **[fact]** (setter not found).
2. Linear search entries `0..NRSETS-1` for the **first** entry with `StepNo == step` (exact, signed). Not found:
   prints "Step %d not found in Database %s." and returns **1** **[fact]**.
   Note: `FUN_1401e3b60` returns the *last* matching entry with arrays, `FUN_1401cc2a0` takes the *first* entry
   with that number; if an earlier header-only entry carries the same number the load fails with 2 **[inferred]**.
3. `FUN_1401cc420(m, db, idx)`; on success and `readHdrCache != 0`: `FUN_1401dd4d0(db, &idx, keepRaw)` (§1.6) **[fact]**.

### 1.3 `FUN_1401cc420(m, db, idx)` - phases and error codes

| Phase | Action | Return / side effect |
|---|---|---|
| 0 | `db->dir == NULL` | **2** |
| 0 | dim-licence test on entry dim | **7** (licence call, omitted) |
| 1 | `FUN_1401d7ef0`: `m+0x2f89 = 0` (clears "mesh changed -> MESHNO+1" flag); `db==NULL` -> `m[0x672]` (=`m+0x3390`); `idx == -1` -> `idx = m+0x740` (reload current entry) | |
| 1 | `idx < 0 || idx >= NRSETS` | **2** |
| 2 | `FUN_1401cb440(m)` free (below) | |
| 3 | vtable slot 0: `(*m->vtbl[0])(m, &dim)` with dim = entry `+0x1c` (2 -> 2, else 3) | sets the model dimension **[inferred]** (`FUN_140158e90` reads it back) |
| 3 | `m+0x740 = idx`, `m+0x744 = entry.StepNo`, `m+0x1b60 = db+0xabc` (DB version, `FUN_1401cc120`), `m+0x1b64 = entry.version` (step version, `FUN_1401cc160`) | |
| 4 | ReadStepHeader `FUN_1401cca80` (§2.1) | 3 (Fortran status 99) / 4 (other); then `m+0x1378 = 0` |
| 5 | entry `+0x24 == 0` (MARSize < 1, header-only step) | **2**; `m+0x1378` (= H[0]) and all header fields stay set |
| 6 | ReadArrays `FUN_1401cd1f0` (§3.1) | 999 no db, 5 alloc/read error, 3 status 99, 1 unsupported version; `m+0x1378 = 0` |
| 7 | `FUN_1401d83b0` = `m+0x1b5b` set ("arrays only") | return **0**: MAR/A and address tables loaded, no class parsing **[fact]**; setter not found |
| 8 | `FUN_140235c10` (getUnitSystem), ReadSimCtrl `FUN_1401cf4a0`, `FUN_140235c30(m+0x84c UNIT)`, ReadMatlInfo, ReadInterMatlInfo, ReadObjInfo, ReadInterObjInfo, `FUN_1401d0320`, ReadArrays `FUN_1401d2be0`, 2D: `FUN_1401d5f50`, 3D: ReadObjFoldInfoFromDB `FUN_1401d25d0` | |
| 9 | ProcessStepHeader `FUN_1401cd020` (§2.2), always 0 | else `m+0x1378 = 0` |
| 10 | free `m+0x1b80` (header), `m+0x1b88` (MAR), `m+0x1b78` (A), `m+0x2240` | |
| 11 | step version > 10999: ReadDynamicArrayFromDB `FUN_1401d2760`; 3D: genGeoSymPlane `FUN_1401d63c0(m,0)`; `m+0x1b5a = 0` (both `FUN_1401d2b70`/`FUN_1401d2bb0` return 1) | **0** |

`FUN_1401cb440(m)` (free): `FUN_1401cb910`, `FUN_1401cb660`, frees `m+0x1b80/0x1b78/0x1b88`, `FUN_1401cb1f0`,
frees `m+0x2240..0x2258`, resets S (`FUN_1401e7da0`), materials (`m+0x1328`), inter-materials (`m+0x1358`),
object lists `m+0x1380`, `m+0x13a8`, `m+0x1378 = 0`, inter-objects (`m+0x13c8`), `m+0x740 = m+0x744 = 0` **[fact]**.

### 1.4 Dead branches in this build
`FUN_1401d2b70` and `FUN_1401d2bb0` always return 1 **[fact]**, so the per-region partial-load code in
`FUN_1401cd1f0` (`FUN_1403dc13f`, `FUN_1401cf190`, `FUN_1401cf220`, the 3D `m+0x2490`/`m+0x2b24` region-length
tables, `FUN_1403dc957`) and the partial branches of `FUN_1401cf4a0` (`FUN_1401cf040`) are never taken.
DBREAD always reads the whole MAR and A with `FUN_1403db9e8` (op 3).

### 1.5 Database open `FUN_1401dc3f0(db, path, headerOnly)` (used by DBREAD, menu 2, GENDB)
Reads record 2 via `FUN_1403d84b0`: title -> `db+0xa50` (80, trimmed), SMI1 -> version
`db+0xabc = (int)(atof(SMI1)*1000 + 0.5)` (constants 1000.0 at `0x1405094e8`, 0.5), SMI2/3/4 -> `db+0xaa1`,
`+0xaa9`, `+0xab1` (release / initial run / current dates per the menu-2 labels), `db+0xac0`, `db+0xac4`,
NRSETS `db+0xac8`, `db+0xad0` (NXTREC; the GENDB header writer reads it back as NXTREC) **[fact]**;
`+0xac0` = ISTART, `+0xac4` = ISMTYP **[inferred]** from word order. Directory via `FUN_1403d96fc`
(5 ints per entry for 3D v<3000, else 10). For DB version >= 10000 dim/version come from entry words 8/9,
otherwise from the file family and DB version; 3D v<3000 entries get SimNo = 1, SimMode = 0.
`db+0xacc` (NUMRST) counts entries with MARSize >= 1. The header cache `db+0xae8` is freed (`= 0`).
Errors: 4 file missing, 1 open failure, 2 bad header / no entries / unknown family, 3 directory read error **[fact]**.

### 1.6 Header cache `FUN_1401dd4d0(db, &idx, keepRaw)` / `FUN_1401dfb80`
Skipped when `db+0x1900` set or no entries. Allocates `db+0xae8` = one 0xe0-byte record per directory entry,
reads the step header of entry `idx` (op 2, `FUN_1403daf3e`) into a temp buffer (kept in `db+0x1908/0x1910`
when `keepRaw`, else freed), closes unit 3, sets `db+0xae0 = idx` **[fact]**. Record fields: `+0` NOBJ = H[0],
`+4` H[1], `+8` H[2], `+0x10` H[3], `+0x1c` platform, `+0x18` H[8] (3D), `+0x30` F = H[12], `+0x34` S = H[9],
`+0x38` H[10] (0 -> dir SimNo, and OPRNAM := SIMNAM), `+0x3c` H[11], `+0x4c` OPRNAM, `+0x8d` SIMNAM,
`+0x20` -> NOBJ blocks of 0xcd0 bytes (first min(S,400) reals of each object block + name at +0xc80),
`+0x28` = |stroke| of object `objIdx = H[1]-1` (sqrt of the squared stroke slots); global min/max of two slot
pairs (table `0x1405d4ac0`/`0x1405d54c0`) in `db+0xaf0` **[fact]**. Used by post-processing displays, not by the
keyword model; GENDB never sees it (§5.6).

---

## 2. Step header

### 2.1 ReadStepHeader `FUN_1401cca80(m, db)`
`FUN_1401e5f00(db, file, &first, idx)` maps the directory index to a data file and its first entry (record
index = idx - first + 1; multi-file databases **[inferred]**). `m+0x1b70 = HSize` (entry `+8`),
`m+0x1b80 = calloc(HSize, 8)`, op 2 read `FUN_1403da0b6`. Then **[fact]**:

| Model field | Source | Note |
|---|---|---|
| `m+0x1378` NOBJ | `(int)H[0]` | overwritten later by IISIM in `FUN_1401cf4a0` |
| `m+0x748` platform | 2D `(int)H[8]`, 3D `(int)H[5]` | selects the text conversion of `FUN_140324020` |
| header cache | `FUN_1401dd4d0(db, m+0x740, 0)` | |
| `S+0xe4` CURSIM v1 | entry SimNo (`+0x14`) | |
| `S+0x92` SIMNAM | 80 chars from H[24..33], `FUN_140324020(.., m+0x748, 10, 0)`, trim | 3D step v<3000: literal "Operation" |
| `S+0xe8`, `S+0x51` | v<11000: = SimNo, OPRNAM = first 64 chars of SIMNAM; v>=11000: `(int)H[10]`, 64 chars from H[16..23] converted (8 words) and trimmed | |
| `S+0x0` TITLE | strcpy from `db+0xa50` (DB header) | |
| `m+0x1734` fold-info flag | 3D: `(int)H[8]`; 2D: 0 | consumed by `FUN_1401d25d0` (`if m+0x1734 != 0`) |
| `S+0xe8`, `S+0xec` CURSIM v2/v3 | `(int)H[10]`, `(int)H[11]` | assigned again at the end for **every** version, so the v<11000 SimNo copy is overwritten |

### 2.2 ProcessStepHeader `FUN_1401cd020(m, db, idx)`
`FUN_1401c7f80(dim)` loads the per-object slot table (globals `DAT_1406e8524..0x1406e8708`, 2D and 3D sets);
`FUN_1401e1de0` gives F/S: dim 2 -> (34,100); 3D v<3000 -> (8,100); else (34,200); v>10999: F = cache `+0x30`
(H[12]) and S = cache `+0x34` (H[9]) when > 0 **[fact]**. For each object `i < cache NOBJ` that exists **[fact]**:

| Object field | Header real | 3D slot | 2D slot |
|---|---|---|---|
| `obj+0x2300/0x2308/0x2310` FORCE x,y,(z) | `H[F + S*i + slot]` | 0,1,2 | 0,1 |
| `obj+0x22e8/0x22f0/0x22f8` MOVCTL type-11 values (current speed) | same | 6,7,8 | 4,5 |

Nothing else of the per-object block reaches the keyword model on read **[fact]** (STROKE `obj+0x22d0..0x22e0`
is never written by the DBREAD closure).

### 2.3 Write: `FUN_1401c5020` -> `FUN_1401e2180` (step header) **[fact]**
`FUN_1401e1f30`: F = 100, S = 400 always. HSize = `100 + 400*NOBJ` (NOBJ = `m+0x7c`, GENDB count). Buffer
`m+0x60` is zeroed first (allocated for 100 objects, §3.3).

| H (0-based) | Value written | Source |
|---|---|---|
| 0 | NOBJ | `m+0x7c` (computed, §5.4) |
| 1 | PDIE | `S+0x168` = `m+0x8b8` |
| 2, 3 | TNOW global time (both) | `S+0x640` = `m+0xd90` |
| 4 | CURSIM v1 (sim no) | `S+0xe4` |
| 5 (3D) / 8 (2D) | platform code = 2 | `FUN_140324000` returns 2 |
| 6, 7 | MAR length, A length (full, before compression) | written by op 4 `FUN_1403b7729` from `m+0x30`/`m+0x34` |
| 8 (3D) | fold-info flag | `FUN_14019a2f0(m)` = `m+0xcc` |
| 9 | S = 400 | |
| 10, 11 | CURSIM v2, v3 | `S+0xe8`, `S+0xec` |
| 12 | F = 100 | |
| 16..23 | OPRNAM, 64 chars blank padded | `FUN_140323bf0(H+16, S+0x51, 64)` |
| 24..33 | SIMNAM, 80 chars blank padded | `FUN_140323bf0(H+24, S+0x92, 80)` |
| others of 0..99 | 0 | |

Per-object block `i` (objects `1..NOBJ` that exist and are not type 7, `FUN_140160f10`); 3D slots / 2D slots:

| Item | 3D | 2D | Source / computation |
|---|---|---|---|
| STROKE x,y,(z) | 3,4,5 | 2,3 | copy `obj+0x22d0/0x22d8/0x22e0` |
| FORCE x,y,(z) | 0,1,2 | 0,1 | copy `obj+0x2300..` |
| MOVCTL-11 speed | 6,7,8 | 4,5 | copy `obj+0x22e8..` |
| object name, 64 chars | 106..113 | 34..41 | `obj+0x828` (OBJNAM) |
| nodes, elements | 88, 89 | 42, 43 | `obj+0x880`, `obj+0x884` |
| min/max (computed) | see list | | only when `obj+0x87c != 0` and `!FUN_140160c20(obj)` |

Computed min/max (`FUN_14020cae0(obj, code, &min, &max, comp, flag)`, `FUN_14020d320` for stress,
`FUN_140211ef0(obj, code, &slot)` writes a component block): code 700 velocity (12,13); 800 displacement (31,32);
600 temperature (14,15); 200 relative density (16,17); 500/0 effective stress (22,23); 500/1 (35,36); 500/4 min
only (37); 500/2 max only (38); 500 components block at 39; 300 effective strain (18,19), recomputed with code 310
(node values) when ELMNOD strain option `m+0x88c` is 1 or 2; 300 components block at 53; 400 strain rate
(20,21), block at 67; 900/1, 900/2 user node vars (79-82); 1000/1, 1000/2 user element vars (83-86); 100 damage
(33,34), code 110 when ELMNOD damage option `m+0x888` is 1/2; 2600 (98,99); 2800/1 (100,101); 2800/2 (102,103);
5000 (104,105); 2D only: code 1600 (56,57); volume `FUN_140163540(obj,1)` -> slot 24 (3D) / 20 (2D);
`FUN_1401638f0(obj,1)` -> slot 115 (3D). Slot numbers are 3D; 2D uses the 2D column of `FUN_1401c7f80` **[fact]**.
Item names follow spec §7.3 labels **[inferred]**.

**Defect [fact]:** in 2D `DAT_1406e85cc = 0`, so the last statement writes `FUN_1401638f0(obj,1)` (returns 0.0
for 2D) into slot 0, overwriting FORCE x of every meshed 2D object; DBREAD then loads FORCE x = 0.

Slots not listed (bounding box 25-30, torque/angle 9-11, 3D 114-124 except 115, ...) are written only by the
carry-over branch: if `db+0xae0 >= 0`, `db+0xae8 != 0`, the cached entry has the same SimNo and `i <` cached
NOBJ, the block starts as a copy of the cached block. In GENDB the DB object is a fresh one
(`FUN_1401dc030` sets `+0xae0 = -1`, `FUN_1401dc3f0` clears `+0xae8`), so the branch is not taken and those
slots are 0 **[fact]**.

`FUN_1401c5020` then also sets the Fortran address tables (`FUN_1403b3bcc/3bfd` 2D, `FUN_1403b3c2e/3c9b` 3D
with I1 = start(IIAD2)-start(IIAD1), I2 = start(IIFNC)-start(IIAD2)) and, 3D, `FUN_1403b973a`/`FUN_1403b9ad6`
(region-table setup for op 4) **[fact]**.

---

## 3. Address tables and sizes

### 3.1 Read `FUN_1401cd1f0` (ReadArrays)
**[fact]** unless marked.
1. `FUN_1401cc1a0`: HSize/MARSize/ASize/version from the entry; `m+0x1b64` re-synced. `db==NULL` -> 999.
2. Region index globals: 3D `FUN_1401c7d10` (`FUN_1401c7080`+`FUN_1401c77c0`), 2D `FUN_1401c7ce0`
   (`FUN_1401c6e60`+`FUN_1401c72a0`), cached in `DAT_1406e81f8`.
3. `m+0x1b68 = (int)H[6]` (MAR length), `m+0x1b6c = (int)H[7]` (A length); if either is 0 both = 2 000 000 with
   "WARNING: The database does not store the sizes ..." `m+0x1b88` = realloc(MAR length ints),
   `m+0x1b78` = realloc(A length reals); failure -> 5.
4. Op 3 `FUN_1403db9e8` reads and decodes MAR/A (2D and 3D). Status 99 -> 3, other -> 5.
5. Step version > 10999: `FUN_1403dd16d` reads the dynamic-array descriptor counts (values discarded here).
6. 3D, version < 6000: decoded lengths must equal H[6]/H[7], else 5.
7. DBMADR `FUN_1403dd950(MAR, NADS1[140], NADS2[280])`: I1 = MAR[2]-MAR[1], I2 = MAR[3]-MAR[2];
   NADS1 = MAR[1..I1], NADS2 = MAR[I1+1..I1+I2] (no bound check against 140/280).
8. Copy into `m+0x1b90` (70 MAR starts + 70 widths) and `m+0x1dc0` (140 A starts + 140 widths):

| Dim / version | MAR | A |
|---|---|---|
| any, >= 11000 | NADS1[0..139] verbatim | NADS2[0..279] verbatim |
| 3D 2300-2999 | 80 words via map `0x1405d6de0` | 116 words via `0x1405d6f20` |
| 3D 3000-3999 | 80 words via `0x1405d70f0` | 140 words via `0x1405d7230` |
| 3D 4000-10999 | n = min((NADS1[1]-NADS1[0])/2, 70) pairs: start k -> slot map[k], width k (NADS1[n+k]) -> slot map[k+40] (`0x1405d7460`) | n = min((NADS2[1]-NADS2[0])/2, 140): map[k], map[k+70] (`0x1405d75a0`) |
| 2D 6000-6999 | 60 words via `0x1405d5f00` | 140 words via `0x1405d5ff0` |
| 2D 7000-10999 | as 3D 4000-10999 with width offset 30 (`0x1405d6220`) | offset 70 (`0x1405d6310`) |
| 3D < 2300, 2D < 6000 | "DB Version No is %d. Direct loading for this version is not supported." in `m+0x1401`, `m+0x1400 = 1`, return 1 | |

Map contents are in spec §6.2. Width defaults (width 0 in the table) are applied by the class readers, not here.
After the copy nothing else is computed on the live path (the 3D region-length tables `m+0x2494`, `m+0x2b28` are on
the dead branch, §1.4).

### 3.2 Write: widths `FUN_1401c03f0`, starts `FUN_1401c1980` (3D) / `FUN_1401c2d10` (2D)
`FUN_1401c03f0` fills the width half of both tables (MAR width slot k at `m+0x1b90+4*(70+k)`, A at
`m+0x1dc0+4*(140+k)`), then calls the start generator. Widths are constants or model counts **[fact]**:

| Width | Value |
|---|---|
| IIAD1, IIAD2, IIFNC, IINOD, IIMTR, IIBCD, IIBFD, IIBFT, IIUVL, IIBDY, IIBEL, IINWN, IILRZ, IIMAX, IICNT, IITIM, IISEP | 0 |
| IIOBJ | 50 (3D) / 20 (2D) |
| IISIM | 100 |
| IIBCT | 3 |
| IIEDE, IIETM, IIEAT, IIEDF, IIETF, IIEAF, IIERH, IIERF | 1 |
| 3D raw slots: W12 IIANG, W13 IIDOD, W16 IIBND, W58 IIDST, W59 IIRBP, W60 IIRBE, W67 IIPPT, W68 IIMSL | 0; W23 IIFLD = 4 if fold flag `m+0xcc == 1` and `m+0xd0 > 0` and `m+0xd4 > 0`, else 0 |
| LLFNC 2, LLMPR 500, LLTFM 50, LLSIM 100, LLCPU 50, LLOBJ 100, LLBND 200 (3D) / 100 (2D), LLBRT 50, LLGRY 3 (3D) / 6 (2D), LLWIN 20 | constants |
| LLWRZ, LLCRZ, LLDRZ, LLURZ, LLPFN, LLCFN, LLCPR, LLSLD | dim |
| LLPPR | 3 (3D) / 0 (2D) |
| LLTMP, LLPHT, LLHIT, LLVLT, LLATM, LLFTS, LLFEP, LLRDT, LLHGN, LLRFL | 1 |
| LLFLX | 3 |
| LLND1 / LLET1 | `m+0xdc` / `m+0xd8` = max(user node / element variable count, 2) (`FUN_1401be9d0`, `FUN_1401be960`) |
| LLWER | 5 if `m+0xe4` else 0 |
| LLSTS | `FUN_1401cafe0(m,2)` x 6 (3D, 2D GEOTYP 3) or x 4 (2D) (integration points x components) |
| LLTEP | `FUN_1401cafe0(m,1)` |
| LLDMG | `m+0x16ac * FUN_1401cafe0(m,0)` |
| LLEPS, LLYLS | 6 (3D) / 4 (2D); 6 for 2D GEOTYP (`m+0x850`) 3 |
| LLTSR | ncomp*(sum of `m+0x86c..0x87c`) + `m+0x880` + `m+0x884` (ELMNOD-dependent) |
| LLVLF, LLVLN / LLDVF, LLICB | `m+0x78` / `m+0xbc` when phase transformation `m+0xc8 > 0`, else 0 |
| LLHDN, LLZEI 2; LLZMI 3 (3D) / 2 (2D); LLCUR 3 (3D) / 2 (2D) | constants |
| LLGSZ | by `FUN_1401d7160` grain mode: 1, 2, 4 or 5 components via `FUN_1401d70c0`, else 0 |
| LLSIZ, LLODF, LLTRC, LLERV, LLNRV | `m+0x16d0`, `m+0x16d4`, `m+0x1680`, `m+0x16f4`, `m+0x1684` (set by `FUN_1401d0320`) |
| LLMTA | `m+0x16b0`, 0 -> 1 |
| LLFCC, LLFDD | 2 (2D) / 0 (3D); LLDUR, LLRSD 2 (2D) / 0 (3D); LLPRE, LLHFL, LLAFL 1 (3D) / 0 (2D) |
| 2D GEOTYP 3 overrides | LLURZ, LLPFN, LLCFN, LLFCC, LLFDD, LLDUR, LLRSD = 3; LLEPS, LLYLS = 6 |
| 3D W14 LLFLD | 3 under the fold condition above, else 0 |
| LLUVL, LLDTM, LLHGO, LLCPA, LLARE, LLTIM, LLAGB, LLEP2 | 0 |

Starts (`FUN_1401c1980`, 3D): slot 1 = 1, start(k+1) = start(k) + count x width (+ object terms), with
IIAD1 = 140 and IIAD2 = 280 words; slots 27-58 are empty (all equal start(26)); A slots 63-126 empty
(= start(62)). Counts are the GENDB fields `m+0x6c` NUMNP, `m+0x70` NUMEL, `m+0x74` NUMNP + objects **[inferred]**,
`m+0x7c` NOBJ, `m+0x78` materials, `m+0x88` NFN, `m+0x8c` function data length, `m+0x94` boundary faces,
`m+0x90`, `m+0x98`, `m+0x9c`, `m+0xa0` (connectivity length), `m+0xc0/0xc4`, `m+0xd0/0xd4` filled by
`FUN_1401bc230` (§5.4) **[fact]**; ELMNOD options `m+0x888/0x88c/0x890` choose NUMEL vs NUMEL+NUMNP rows for
LLDMG, LLTEP, LLSTS. The resulting region lengths are tabulated in spec §6.3 (not repeated here).

### 3.3 Buffers `FUN_1401c4560` and address-table store `FUN_1401c4840` **[fact]**

| Field | 3D | 2D |
|---|---|---|
| `m+0x30` MAR length (stored) | start(MAR slot 58, IIDST) - 1 | start(slot 62) - 1 |
| `m+0x34` A length (stored) | start(A slot 126) - 1 | start(slot 129) - 1 |
| `m+0x38` | start(MAR slot 69, IILAST) (full incl. solver-only regions) | same |
| `m+0x3c` | start(A slot 139, LLAST) + 100 + 400*NOBJ (memory estimate, printed by `FUN_14001cd50`) | same |
| `m+0x40` / `m+0x44` / `m+0x48` | MAR ints / 2*A length / 2*(100 + 400*100) (header for 100 objects) | |
| buffers | `m+0x50` MAR (ints), `m+0x58` A (reals), `m+0x60` header (reals), all zeroed | |

`FUN_1401c4840`: MAR region IIAD1 = copy of `m+0x1b90` (140 ints), IIAD2 = copy of `m+0x1dc0` (280 ints).

---

## 4. Database header and directory entry (write side)

### 4.1 `FUN_1401bffb0` DB header (record 2) **[fact]**

| Word | Value |
|---|---|
| 1-20 TITLE | `S+0` if non-empty, copied into `db+0xa50`; otherwise the existing title; blank padded 80 |
| 21-22 SMI1 | `sprintf("%5.2lf   ", (m+0xc)*10 / 1000.0)` (program version) |
| 23-24 SMI2 | `m+0x10` (8 chars, program release date) -> `db+0xaa1` |
| 25-26 SMI3 | today `"%02d/%02d/%02d"` (`FUN_140235800`, year mod 100) when NSTART == -1 or `db+0xaa9` empty, else kept |
| 27-28 SMI4 | today, always |
| 29 ISTART | directory position `pos` of the step being written (bcbb0 `param_3`) |
| 30 ISMTYP | STYPE `m+0x848`, forced 1 for 3D |
| 31 NRSETS | old NRSETS (0 for a new DB); op 4 sets the final value (= pos) |
| 32 NXTREC | `db+0xad0` |
| 33 IPTYPE | 1 |
| 34 | 0 |

Delivered by `FUN_1403b400b` (Fortran common: `DAT_140b6cb70` ISTART, `..74` ISMTYP, `..78` NRSETS, `..7c`
NXTREC, `..80` IPTYPE, `..84` word 34). `FUN_1403b3f73(&MESHNO m+0x840, &NSTART m+0x8b0, &STPINC m+0x8b4,
&SimNo S+0xe4, &SimMode, &dim, &version)` stores the directory-entry values; SimMode = `FUN_1401c5630` = 4 if
any of `m+0x85c/0x860/0x864` (TRANS microstructure/diffusion/grain) is non-zero, else `m+0x13c` (= SMODE `S+0xf4`
normalised by `FUN_1401f3f80`, set in `FUN_1401bc230`); version = `m+0xc * 10` (GENDB always writes the current
layout, flag `param_7` = 1 from `FUN_1401bb550`).

### 4.2 Op 4 `FUN_1403b5a04` -> `FUN_1403b7729` (directory placement) **[fact]**
Call in `FUN_1401bcbb0`: `FUN_1403b5a04(&3 unit, &4 op, &pos, &NSTART, &HSize, m+0x60, &m+0x30, MAR, &m+0x34, A, &err)`.

* `pos >= 1` (always the case from GENDB): `FUN_1403b44d2` (header/index I/O, not followed); `pos == 1`: fresh
  directory (next record 17, `DAT_140b50da0 = 4`); `pos > NRSETS`: new entry (new 13-record chunk when needed);
  `pos <= NRSETS`: entry `pos` is reused and data are written from its old header record on.
  Then **NRSETS = pos**: every later entry is dropped.
* `pos < 1` (solver path, not used by GENDB): append after the last entry; if the last entry has the same
  StepNo it is overwritten; if `(StepNo-1) mod STPINC != 0` the previous entry is turned into a header-only entry
  (MARSize = ASize = 0) and its array space is reused. This is how header-only entries arise.
* Entry words: MeshNo, StepNo, HSize, MARSize, ASize, SimNo, SimMode, dim, version from the values above;
  H[6]/H[7] (and H[5] or H[8] platform) are written into the header buffer here. Compression: 2D version > 3109
  (whole array, MARSize/ASize become the stored lengths), 3D >= 6000 region tables (`FUN_1403ba54c`).

### 4.3 Directory position `FUN_1401e1f60(db, &NSTART, &nrsets, &pos, &kept, m+0x68, &MESHNO, &lastMesh)` **[fact]**
```
a = |NSTART|; nrsets = NRSETS; pos = 1
while pos <= nrsets and |StepNo(pos)| < a: pos++
if pos <= nrsets and |StepNo(pos)| == a and NSTART > 0 and StepNo(pos) < 0: pos++   # +n goes after -n
kept = pos - 1
j = first entry in 1..pos-1 with MeshNo(j) == MeshNo(pos-1)                       # first step of last kept mesh
m+0x68 = StepNo(j); lastMesh = MeshNo(j)
return -1 if j found and lastMesh > MESHNO and pos <= nrsets else 0
```
-1 prints "WARNING: MESHNO %5d less than MESHNO in the last step of database %5d" (`FUN_14001cd60` code 0x26);
the step is still written. So a new step replaces the entry with the same |step| (and the same sign rule) and
truncates everything after it. `m+0x68` (pre-set to NSTART) has no further reader in the GENDB closure.

---

## 5. GENDB driver

### 5.1 Entry points and argument mapping

| Caller | `FUN_1401bb550(m, path, check, dbopt, signmode)` | Evidence |
|---|---|---|
| KEY command `GENDB` (registry id 4, case 4 of `FUN_140378e00`) | saves NSTART, `FUN_1401d7f40(m)`; `m+0x2f89` -> MESHNO+1 (flag not cleared here); up to 3 ints on the keyword line: token0 -> dbopt (1 or 2, else 0); token1 -> check (0 -> 2, 2 -> 1, anything else -> 2); token2 -> signmode (0..2, else 0; 1 or 2 restore the saved NSTART); next line = DB path. Direct flag `m+8` == 0 -> `FUN_1401b9a40` = stub returning 0 (nothing written) | `bm_cmd.c` **[fact]** |
| Menu 7 `FUN_14000a370` | "Write step %d to: 0 - Quit; 1 - Old DB; 2 - New DB"; asks the DB name (new: ".DB" appended, overwrite confirmation), then `FUN_1401b9940(m, path, m+0x740, 1, 1|2)` -> `FUN_1401bb550(m, path, check=1, dbopt=1|2, signmode=0)` (from the call-site disassembly) | `bm_main.c`, listing at `0x14000a6af`/`0x14000aa0b` **[fact]** |
| `FUN_1401b9940(m, path, curIdx, check, dbopt)` | `m+0x2f89` -> MESHNO+1 and clear; `_chdir(path)` via `FUN_1401b8bf0` (result ignored); direct flag -> `FUN_1401bb550(m, path, check, dbopt, 0)`, else stub `FUN_1401b9a40` | listing **[fact]** |

`FUN_1401d7f40(m)`: `FUN_1401ee510(S)`, `FUN_1401d6250` (resets phase lists of mixtures), `FUN_1401d63c0(m,0)`;
if NSTART was read from a KEY file (`m+0x2f88`, set by the NSTART reader) and NSTART >= 0:
NSTART = -(NSTART+1); `m+0x744 = NSTART` **[fact]**.

### 5.2 `FUN_1401bb550` sequence **[fact]**
1. "Direct DB Writing Scheme On"; version check `FUN_1401b9c00(m, path)`: opens the DB if present and warns
   "The old database was written in a higher version of DEFORM" when its version > `m+0xc*10` (warning only).
2. Input check `FUN_1401bc230` (§5.4); error -> return 1 "DEFORM Database can NOT be Generated".
3. `check` 0 or 2: `FUN_1401b9d50` (four object checks `FUN_1401b9de0`, `FUN_1401b9fe0`, `FUN_1401ba460`,
   `FUN_1401ba5a0`; any false -> return 1) and `FUN_1401bebf0` (checks 1-5 `FUN_1401becf0`, `FUN_1401bfdf0`,
   `FUN_1401bfe60`, `FUN_1401bfed0`, `FUN_1401bff40`, summary `FUN_14001cdc0`); `check == 0` returns 0 here
   (check only). Other values: "Database checking has been skipped."
4. dbopt: < 1 -> 1 if the file exists else 2; == 1 and file missing -> "Old Database NOT Exist", return 1.
5. signmode on NSTART (`m+0x8b0`): 0 -> if > 0 then -(n+1); 2 -> if > 0 then -(n+1), if < 0 then n-1;
   1 -> unchanged. The (possibly negated) value is the StepNo written, and also what the sim-control writer
   stores (it runs later).
6. dbopt 1 (old): `FUN_1401dc3f0(db, path, 0)` (fail -> "Invalid DEFORM Database", 1); `FUN_1401b9810`: version
   >= 10000 always OK, else must be equal or a listed compatible pair (2D 9000->9010/9100, 9010->9100;
   3D 6000->6010/6100, 6010->6100), else "not a valid Version %g database", 1; then `FUN_1401e1f60` (§4.3).
   dbopt 2 (new): if the file exists, "Database already Exists; Overwriting": the output name becomes
   `path + "ttmp"` (`FUN_140484ef0`, string at `0x1405277f0`) and `FUN_1401b85d0` deletes a stale file of that
   name (fopen "rt" test, then remove); `FUN_1401dc3a0` sets `db` name and version (`m+0xc*10`); pos = 1, nrsets = 0.
7. Open unit 3 `FUN_1403ae0b6` (status 1 old / 2 new); error -> "Error in opening %s", 1.
8. `FUN_1401bcbb0` (§5.3), close unit 3. Success: "DEFORM Database Generated"; in the overwrite case the old file
   is deleted (`FUN_1401b85d0(path)`) and `path+"ttmp"` renamed to `path` (2-argument CRT call shown as
   `FID_conflict__wrmdir`, rename **[inferred]**); `FUN_1401c54b0` is an empty stub. Failure: the temporary file
   is deleted, return 1. So "new DB" over an existing file never touches the old file until the write succeeded.

### 5.3 `FUN_1401bcbb0` order **[fact]**
Progress `FUN_1401c54c0(1..8)` (`FUN_14001cdb0` stage messages) between: DB header `FUN_1401bffb0`; widths and
starts `FUN_1401c03f0`; buffers `FUN_1401c4560`; address tables `FUN_1401c4840`; sim ctrl `FUN_1401c4950`
(IISIM, LLSIM, LLCPU, LLFNC, IIFNC, IIUVL); materials `FUN_1401c49b0`; objects `FUN_1401c4d90`; inter-objects
`FUN_1401c4ea0`; inter-materials `FUN_1401c4fa0`; dynamic arrays `FUN_1401c5260` (collects each object's
DYNARY arrays via `obj+0xba8`, fills the tables with `FUN_1403bc2c4`, numbers the slots with `FUN_14025be70`);
step header `FUN_1401c5020`; write op 4; `FUN_1401bb320(m)` (post-write cleanup, not followed).

### 5.4 Counts computed by `FUN_1401bc230` (inputs of §3.2) **[fact]**
`FUN_1401baf30`; `m+0xe8/0xf0 = m+0xdb0/0xdb8`; "No mode has been activated" when TRANS(1..4) all 0; unlicensed
modules switched off (licence, omitted); `m+0x13c = FUN_1401f3f50(S)` (SMODE); max object number
`FUN_1401caf40` > 100 -> error "100 is the maximum allowed number". Then 3D `FUN_1401bd3d0` / 2D
`FUN_1401bdaf0` fill NOBJ `m+0x7c`, NUMNP `m+0x6c`, NUMEL `m+0x70`, `m+0x74`, `m+0x80`, `m+0x84`, 3D `m+0x90..0xb4`,
`m+0xa0`, 2D `m+0xb8`, `m+0x104/0x108`; `m+0x78` materials (`FUN_1401be510`, mixture check for TRANS 3),
`m+0xbc` inter-material relations (`FUN_1401be660`), `m+0xc8` (`FUN_1401be700`, material-group checks),
`m+0xd8`, `m+0xdc` (§3.2), NFN `m+0x88`, LLFNC length `m+0x8c`, `m+0xc0/0xc4` (`FUN_1401bea40..beb90`).

### 5.5 Values GENDB computes instead of copying
DB header: SMI1, SMI3, SMI4, ISTART, NRSETS, NXTREC, IPTYPE, word 34, ISMTYP forced 1 in 3D.
Directory entry: StepNo (sign rule), SimMode, HSize, MARSize, ASize (stored lengths), dim, version, records,
position/truncation, MeshNo+1 after a mesh change. Step header: H[0] NOBJ, H[5]/H[8] platform 2, H[6], H[7],
H[8] (3D fold flag), H[9] = 400, H[12] = 100, all per-object min/max, volume, slot 115 (3D), node/element counts.
Address tables: all starts and widths (§3.2); every count in §5.4. 2D FORCE x forced to 0 (defect, §2.3).

### 5.6 Not reached
Carry-over of previous per-object blocks (§2.3), the solver append path `pos < 1` (§4.2), `FUN_1401b9a40` (stub).

---

## 6. Keyword table (frame keywords)

DB home = DB header (record 2), directory entry, or step header. Keywords also present in IISIM/LLSIM are mapped
there by `simctrl.md`; only the frame location is given here.

| Keyword | Model field | DB location | Read | Write | Notes |
|---|---|---|---|---|---|
| TITLE | `S+0x0` char[81] (`m+0x750`) | record 2 words 1-20 (80 chars) | `FUN_1401cca80`: strcpy `db+0xa50` (trimmed by `FUN_1401dc3f0`) | `FUN_1401bffb0`: if non-empty -> `db+0xa50`, blank-padded 80 | database-wide: the last GENDB with a non-empty TITLE sets it **[fact]** |
| CURSIM v1 | `S+0xe4` (`m+0x834`) | dir word 6 SimNo; H[4] | = entry SimNo (3D v<3000: 1) | dir SimNo and H[4] = (double) | **[fact]** |
| CURSIM v2 | `S+0xe8` (`m+0x838`) | H[10] | `(int)H[10]` for every version | H[10] | v<11000 SimNo copy is overwritten **[fact]** |
| CURSIM v3 | `S+0xec` (`m+0x83c`) | H[11] | `(int)H[11]` | H[11] | **[fact]** |
| OPRNAM | `S+0x51` char[65] | H[16..23] | v>=11000: 64 chars, platform text conversion (8 words), trim; else = SIMNAM[0..63] | blank pad 64 | **[fact]** |
| SIMNAM | `S+0x92` char[81] | H[24..33] | 80 chars, conversion (10 words), trim; 3D v<3000 "Operation" | blank pad 80 | **[fact]** |
| NSTART | `S+0x160` (`m+0x8b0`) | dir word 2 StepNo (+ IISIM) | `m+0x744` = StepNo of the loaded entry; NSTART itself from IISIM | StepNo = NSTART after the sign rule (§5.2.5, `FUN_1401d7f40`); also decides SMI3 (== -1) and the directory position | **[fact]** |
| MESHNO | `S+0xf0` (`m+0x840`) | dir word 1 MeshNo (+ IISIM) | not from the directory | MeshNo = MESHNO (+1 if `m+0x2f89`) | MESHNO-less-than warning **[fact]** |
| STPINC | `S+0x164` (`m+0x8b4`) | no frame home (IISIM, see `simctrl.md`); passed to Fortran `DAT_140b6c9f0` | - | used by op 4 only on the `pos < 1` append path | **[fact]** |
| STYPE | `S+0xf8` (`m+0x848`) | record 2 word 30 ISMTYP (+ IISIM) | model value from IISIM | ISMTYP = STYPE, 3D -> 1 | **[fact]** |
| SMODE | `S+0xf4` (`m+0x844`) | dir word 7 SimMode (+ IISIM) | not from the directory | SimMode = 4 if TRANS(3..5) active else SMODE | **[fact]** |
| PDIE | `S+0x168` (`m+0x8b8`) | H[1] (+ IISIM) | header cache only | H[1] = (double) | **[fact]** |
| TNOW (global time) | `S+0x640` (`m+0xd90`) | H[2], H[3] (+ LLSIM) | header cache only | both = TNOW | **[fact]** |
| FORCE | `obj+0x2300/0x2308/0x2310` | object block slots 0,1,2 (3D) / 0,1 (2D) | `FUN_1401cd020` copy | `FUN_1401e2180` copy; 2D slot 0 then overwritten by 0.0 | defect **[fact]** |
| MOVCTL (type-11 values) | `obj+0x22e8/0x22f0/0x22f8` | slots 6,7,8 (3D) / 4,5 (2D) | copy | copy | other MOVCTL data: object class |
| STROKE | `obj+0x22d0/0x22d8/0x22e0` | slots 3,4,5 (3D) / 2,3 (2D) | not read | copy | write-only in the frame **[fact]** |
| OBJNAM | `obj+0x828` | slots 106..113 (3D) / 34..41 (2D), 64 chars | not read here (LLOBJ, object class) | copy | header copy is redundant |

KEY-only (frame): none of the keywords above. The header KEY writer `FUN_1401c5a20` only prints the banner
"DEFORM-nD V... KEYWORD FILE (Qt)". STPINC has no frame home (IISIM only).
Unreached: STROKE on read (no DBREAD access); all per-object carry-over slots in GENDB.

Counts: 17 keyword rows mapped (14 distinct keywords with a frame home + CURSIM split in 3), 0 KEY-only,
1 unreached-on-read (STROKE).

## 7. DB words without keyword

| Location | Meaning |
|---|---|
| record 2 SMI1-4, ISTART, NRSETS, NXTREC, IPTYPE (=1), word 34 (=0) | §4.1 |
| dir words 3-5, 8-13 | HSize, MARSize, ASize, dim, version, records (spec §4.2) |
| H[0] | object count (read into `m+0x1378` first) |
| H[5] (3D) / H[8] (2D) | platform code 2 (text conversion selector `m+0x748`) |
| H[6], H[7] | full MAR / A lengths (DBREAD allocation sizes, default 2 000 000) |
| H[8] (3D) | fold-info flag (`m+0xcc` write, `m+0x1734` read; gates `FUN_1401d25d0` and the IIFLD/LLFLD widths) |
| H[9], H[12] | S = 400, F = 100 |
| object slots | min/max statistics, volume, node/element counts, slot 115: computed, never read by DBREAD |
| MAR IIAD1/IIAD2 | address tables (§3) |

## 8. What `deformdb` needs for the frame
1. `StepEntry` already has all 13 words; add a typed `StepHeader` view: NOBJ, PDIE, time, CURSIM[3], platform,
   lengths, fold flag, F, S, OPRNAM, SIMNAM (H[24..33]; 3D v<3000 "Operation"; v<11000 OPRNAM = SIMNAM), per-object
   FORCE / speed / STROKE / name / counts, with the 2D/3D slot table of `FUN_1401c7f80`.
2. Header-length rule F/S as in `FUN_1401e1de0`, and the DBREAD default 2 000 000 when H[6]/H[7] = 0.
3. Step selection helpers: exact first-match lookup (`FUN_1401cc2a0`) and DBREAD's "nearest step <= N with arrays"
   (`FUN_1401e3b60`, key -s-0.5 for s < 1); report header-only steps as error 2.
4. Writer: directory placement of `FUN_1401e1f60` + truncation (NRSETS = pos), the NSTART sign rule, SMI3/SMI4 date
   rule, ISMTYP forced 1 for 3D, SimMode rule, H[6]/H[7]/platform filled at write time, F = 100 / S = 400; the
   min/max statistics need the model (cannot be copied); reproduce or knowingly fix the 2D FORCE-x defect.
5. Old-version address-table remap (§3.1) for a typed view of steps < 11000.

Open questions: setters of `m+0x1b5b`, `m+0x1b5d`, `m+0x2f89`, `m+0x8` (direct flag), `m+0xc` (program
version/10), `m+0x10` (release date); exact quantity names of codes 2600/2800/5000/1600 and slot 115; meaning of
`m+0x74/0x80/0x84/0x90/0x98/0x9c` counts; `FUN_1401bb320` post-write cleanup.
