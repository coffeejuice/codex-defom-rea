# DEF_PRE_64: keyword model ↔ .DB step arrays

How DBREAD turns one .DB step into DEF_PRE's in-memory keyword model, and how GENDB turns the model back into
a step. Recovered from decompiled code of `DEF_PRE_64.exe` v11.0.2 only; the two test DBs (copies under
`DEFORM_corpus\db`) and cached DEF_PRE KEY exports were used afterwards to check findings (§7). The container
itself (records, directory, region tables, RLE) is in [`../db-format-spec.md`](../db-format-spec.md).

Per-class detail, one file each, with a row per keyword and the function address behind every claim:

| File | Scope | Keywords |
|---|---|---|
| [`dbmap/stepdriver.md`](dbmap/stepdriver.md) | DBREAD / GENDB drivers, step header, DB header, directory placement, address tables and region sizes | frame keywords (TITLE, CURSIM, OPRNAM, SIMNAM, NSTART, MESHNO, STYPE, SMODE, …) |
| [`dbmap/simctrl.md`](dbmap/simctrl.md) | simulation control (`m+0x750`) in IISIM / LLSIM / IIUVL; **the shared function table IIFNC / LLFNC** | group 1, 70 |
| [`dbmap/material.md`](dbmap/material.md) | materials (LLMPR, 500 reals each) and material pairs / phase transformation (LLTFM, 50 reals each) | groups 4 + 5, 42 + 10 |
| [`dbmap/object.md`](dbmap/object.md) | per-object records IIOBJ / LLOBJ, movement and rigid geometry in LLBND, remesh and BC windows | group 2 scalars, 79 |
| [`dbmap/objarrays.md`](dbmap/objarrays.md) | node / element / boundary-face arrays, state variables, BC arrays, fold meshes, DYNARY | group 2 arrays, 58 + `*` variants |
| [`dbmap/interobj.md`](dbmap/interobj.md) | inter-object data (LLBRT) and **the shared window table LLWIN / LLWRZ** | group 3, 11 |

Tooling: [`dbmap/region_globals.tsv`](dbmap/region_globals.tsv) (the 182 region-index globals `DAT_1406e8xxx` →
region names, built by `dbmap/region_globals.py` from `FUN_1401c6e60` / `FUN_1401c7080` / `FUN_1401c72a0` /
`FUN_1401c77c0`), `dbmap/annotate.py` (rewrites decompiled C so region accesses read `ASTART(m,LLMPR)`), and
`deformdb/tools/dbraw.cpp` (prints a step's regions exactly as DEF_PRE addresses them).

## 1. Shape

One C++ object `_DB_STEP_DATA` (`m`, ~0x3400 bytes, ctor `FUN_1401c8950` → `FUN_1401c8c00`) is both the keyword
model and the DB codec. Its sub-objects are the keyword classes:

| `m+` | Content | DBREAD | GENDB | KEY writer |
|---|---|---|---|---|
| 0x750 | sim control | `FUN_1401eb870` | `FUN_1401effc0` | `FUN_1403d4990` |
| 0x1320 / 0x1328 | material count / list (LLMAT_DATA) | `FUN_14028f300` per record | `FUN_14029bdf0` → `FUN_140294ae0` | `FUN_1403cdca0` |
| 0x1358 | material-pair list | `FUN_140336740` | `FUN_14033a5a0` → `FUN_140337f50` | `FUN_1403cbf00` |
| 0x1378 / 0x1380 | object count / list (LLOBJ_DATA) | `FUN_14016caf0`, `FUN_140186f30`, `FUN_140195ff0`, `FUN_14018a180`, `FUN_140174e40`, `FUN_1401d2be0` | 3D `FUN_14017b370`, 2D `FUN_14017cb60`, then `FUN_1401c4a30`, `FUN_1401c4be0` | `FUN_14029e250` |
| 0x13c8 | inter-object list | `FUN_1401f58a0` | `FUN_1401fa4f0` → `FUN_1401f7730` | `FUN_1403c9ad0` |
| 0x1b90 / 0x1dc0 | MAR / A address tables (70 / 140 starts, then as many widths) | copied from IIAD1 / IIAD2 (`FUN_1401cd1f0`) | computed (`FUN_1401c03f0`, `FUN_1401c1980` 3D) | – |
| 0x1b78 / 0x1b88 | decoded A / MAR of the step being read | | | |
| 0x50 / 0x58 / 0x60 | MAR / A / header buffers being written | | | |
| 0x2220–0x2238 | pointers to IIFNC, LLFNC, LLWIN, LLWRZ (shared by all classes) | | | |

The region-index globals hold the same region name for 2D and 3D once DEFORM's printed name table is corrected
for 3D slots 47/48 (LLSIZ / LLRFL), which confirms the swap already applied in `deformdb`.

## 2. DBREAD

`DBREAD` keyword / menu 2 → `FUN_1401cc2a0` (first exact step match in the directory) → `FUN_1401cc420`. The KEY
form takes a step number (0 = last step with arrays; otherwise `FUN_1401e3b60` picks the nearest step ≤ N that has
arrays, ordering −s as −s−0.5) and a DB path. Phases, in order (progress labels are DEFORM's own):

| # | Function | Label | Reads | Fills |
|---|---|---|---|---|
| 1 | `FUN_1401cca80` | ReadStepHeader | step header, directory entry, DB title | CURSIM, OPRNAM, SIMNAM, TITLE, fold flag, F/S |
| 2 | `FUN_1401cd1f0` | ReadArrays | MAR / A (or regions), IIAD1 / IIAD2, version remap | address tables |
| 3 | `FUN_1401cf4a0` → `FUN_1401eb870` | ReadSimCtrl | IISIM, LLSIM, IIUVL, IIFNC / LLFNC | group 1, model counts |
| 4 | `FUN_1401cff20` → `FUN_14028f300` | ReadMatlInfo | LLMPR, functions | group 4, slot → material number |
| 5 | `FUN_1401d0180` → `FUN_140336740` | ReadInterMatlInfo | LLTFM (record `50·(j·NMAT+i)`) | group 5 |
| 6 | `FUN_1401d2230` | ReadObjInfo | IIOBJ, LLOBJ, LLBND, LLWIN / LLWRZ | group 2 scalars, movement, windows |
| 7 | `FUN_1401d5c10` → `FUN_1401f58a0` | ReadInterObjInfo | LLBRT (N × N boundary pairs) | group 3 |
| 8 | `FUN_1401d0320` | – | – | expected width `W[id]` of every array (ELMNOD, STNOUT, GEOTYP, counts) |
| 9 | `FUN_1401d2be0` (+ `FUN_1401d5f50` 2D, `FUN_1401d25d0` 3D fold) | ReadArrays | node / element / face regions | group 2 arrays; read **only if stored width = W[id]** |
| 10 | `FUN_1401cd020` | ProcessStepHeader | per-object header blocks | object names / counts check |
| 11 | `FUN_1401d2760` | ReadDynamicArrayFromDB | dynamic arrays (v > 10999) | DYNARY |
| 12 | `FUN_1401d63c0` | genGeoSymPlane | – | symmetry planes, recomputed from die geometry |

Header-only steps (MARSize = ASize = 0) stop after phase 1 with error 2. `FUN_1401d2bb0` / `FUN_1401d2b70` always
return 1 in this build, so every "partial region" and "old layout copy" branch (`FUN_1401cf040`, …) is dead code;
the only version branches that run are in phases 1–2 (address-table remap, OPRNAM / CURSIM < 11000) and the
per-word gates listed in the class files (e.g. LLMPR w88, w66, w150–162; WINPRS; DYNARY).

## 3. GENDB

`FUN_1401b9940` → `FUN_1401bb550(m, path, check, dbopt, signmode)` → `FUN_1401bcbb0`.

* `check`: 0 check only, 1 skip checks, 2 check then write. `dbopt`: 1 old DB (append; error if missing),
  2 new DB (written to `path+"ttmp"`, replaces the old file only after success), < 1 auto. Menu 7 passes
  check = 1, dbopt = 1 / 2, signmode = 0; the KEY GENDB tokens are mapped in `stepdriver.md` §5.
* `signmode` 0: NSTART n > 0 is stored as step −(n+1) (the pre-processor's input step); 2: also decrements
  negative steps. A KEY-file NSTART is negated by `FUN_1401d7f40`.
* Placement (`FUN_1401e1f60`): the new step goes before the first directory entry with |step| ≥ |NSTART|
  (+n after −n); the write (`FUN_1403b7729`, op 4) reuses that entry and sets NRSETS = position, **dropping every
  later step**. Warning when MESHNO is lower than the last step's.

Write order inside `FUN_1401bcbb0`: DB header (`FUN_1401bffb0`: title, version, SMI dates, ISTART = position,
ISMTYP forced 1 for 3D, IPTYPE 1) → region sizes and starts (`FUN_1401c03f0`) → zeroed buffers
(`FUN_1401c4560`; MAR length = start(IIDST) − 1, A length = start(slot 126) − 1) → address tables
(`FUN_1401c4840`) → sim control → materials → objects (+ arrays) → inter-object → material pairs →
`FUN_1401c5260` → step header (`FUN_1401c5020` / `FUN_1401e2180`) → `FUN_1403b5a04` op 4.

### Computed, not copied (summary; details in the class files)

* All counts, region starts / widths / lengths, first node / element / boundary numbers, IINOD offsets, boundary
  face lists (IIBDY / IIBEL / IIBND), LLBND record numbers, fold ranges.
* Material slots: only materials used by an object are written, in increasing material number; LLMPR word 0
  keeps the number, MSTMTR lists and MTLGRP store slots / positions.
* Function numbers: every class re-appends its curves to IIFNC / LLFNC through `FUN_1401bb3d0` in write order
  (counter `m+0x10c`); window rows use cursors `m+0x134` / `m+0x138`.
* Code conversions: OBJTYP, BCCDEF contact codes (−object ↔ −LLBND record, −200 "all directions"), negative
  BCCTMP-type codes written as 2, DTTYPE from (x-variable, mode), DTSUB clamped and stored back, FRQNCY and
  STNOUT defaults, CNTACT mirror records (flag 1 ↔ 2).
* Step header per-object min / max statistics, volume, loads (from the model's arrays); the slots it does not
  compute are 0, because GENDB never carries the previous step's blocks over (`+0xae0 = −1`).
* SimMode = 4 when microstructure, diffusion or grain is active, else SMODE.

## 4. Region ownership

| Region | Owner | Notes |
|---|---|---|
| IIAD1, IIAD2 | frame | address tables |
| IIFNC, LLFNC | all classes | function table: IIFNC pairs (1-based start in LLFNC, owner / tag), next start; payload layouts in `simctrl.md` §5 and `material.md` §2 |
| IISIM, LLSIM, IIUVL | simctrl | USRDEF text in IIUVL; UNNAME / UENAME at the tail of LLND1 / LLET1 (3D) |
| LLCPU | – | passed to the sim-control writer but unused; not read |
| LLMPR, LLTFM | material | |
| IIOBJ, LLOBJ | object | |
| LLBND, IIBND, LLGRY, IIDOD | object + objarrays | movement and rigid geometry in LLBND; boundary links |
| LLWIN, LLWRZ | object + interobj | window table |
| LLBRT | interobj | N × N boundary pairs, N = IISIM[24] (3D) / [31] (2D) |
| IINOD, IIMTR, IIBCD, IIBFD, IIBCT, IIBFT, IIBDY, IIBEL, IIED*, IIET*, IIEA*, IIER*, IIFLD and A node / element regions LLCRZ … LLNRV | objarrays | |
| IIANG | – | sized but never addressed (BCCANG has no reader or writer) |
| IIDST … IILAST, LLHGO … LLAST | – | beyond the stored MAR / A length: solver work space, never stored by GENDB |
| LLDST, LLTRZ, LLTMQ, LLFLW, LLGNF, LLANG | – | no index global: DEF_PRE never addresses them |

## 5. Keyword coverage

Registry groups 1–5 (270 keywords):

| Class | Keywords | DB-mapped | KEY-only (computed) | Not in DB | Notes |
|---|---:|---:|---:|---:|---|
| sim control | 70 | 67 | 1 (CVNAME) | 2 (DSMAX_NSTEP → DSMAX, INIGES) | 2D SMAX / VMIN / LMAX are KEY-written but not stored |
| material | 42 | 40 | – | 2 (LTHEAT, GRNMDL: parsed and dropped) | TMPSOL / TMPLIQ stored but not KEY-written |
| phase transformation | 10 | 10 | – | – | TTTN stored; KEY prints a generated name |
| object scalars | 79 | 65 | 4 (TPLOGY, POLYTP, DIEPNT, DIEEDG) | 10 (MOVANG, BCCANG, MGSIZM, OBJPHS, CSPMTX, CRDSYS, GEO23, GEOSEC, MSHSEC, CUSVAR) | STROKE comes from LLBND w18–20 |
| object arrays | 58 | 58 | – | – | 63 names mapped in `objarrays.md` (incl. BCCFNC, SYMSUF, FLDGEO, FLWNET, DYNARY, shared with `object.md`); `*` comment variants: *DTMP mapped, *FRZB, *PRZB, *HITN, *EPS, *EFSTS, *EFEPS read but never written, *HTGN never transferred (widths never match) |
| inter-object | 11 | 11 | – | – | 2D-only FRCROT, SEPDEN; 3D-only CNTMTH |

"Not in DB" keywords have no DB access in either direction and no KEY-writer call (except the two input-only sim
keywords, which the KEY reader converts or discards); none of them appears in the KEY exports of the 816 cached DB
steps. No keyword that DBREAD reads or GENDB writes is left unmapped.

DB-side keyword list per step: the KEY exports of 35 steps of the two test DBs contain 192 distinct keywords
(59 sim control, 34 material, 93 object, 6 inter-object), against the ~52 array keywords `deformdb` decodes
today (`deform-rea-work/out/dbmap/db_key_keywords.tsv`).

## 6. Round-trip hazards (KEY → DB → KEY not identical)

Found in code; a faithful port reproduces them, a modern API may fix them knowingly:

* FORCE values are not stored (only speed × direction for movement type 0); 2D step-header FORCE x of every
  meshed object is overwritten with 0 (`DAT_1406e85cc` = 0).
* 3D window count written to IISIM[34], read from IISIM[39].
* 3D window with shape 0 and usage 3 / 4 does not advance the LLWIN cursor (next window overwrites it).
* Movement name: reader takes 80 bytes, writer writes 64.
* FRCROT kind-4 function reads back as a constant; window attach code 2 lost; IHTCOF kind 3 cannot be KEY-written.
* Material: YOUNG-type with variable 8 or (3,4), FPERV variable ≠ 1, TTTD code 9 with w13 ≠ 0, 2-D MASDEN
  (writer overwrites w1 = MATDEN), GRNDAT 9th-table code read from the wrong field (`+0x13e0`).
* Face-BC count: reader sums faces over all surfaces, 3D writer writes surface 0 only.
* Element arrays: an 8-point stored width with 1 point per element keeps point 1; GENDB copies element values
  to all 8 points.

## 7. Checks against the test DBs

`dbraw` on the corpus copy of `COG_P1_B1.DB` step 25 against DEF_PRE's own KEY export of that step (oracle cache):

| Class | Checked | Result |
|---|---|---|
| sim control | MESHNO, NSTART, NSTEP, STPINC, PDIE (2 values), ITRMTH, UNIT, TRANS 1–5, HTMTHD, TNOW 1–4 | all equal |
| material | material 1: number (w0), MATDEN (w1), MTNAME text (w2–9), FRAE2H (w10), FSTRES code (w11), POISON (w34/35), EMSVTY (w45/46), YOUNG function 4 → IIFNC start 331, 2 points (20, 217000), (871, 156000) in LLFNC; material 2 number and EMSVTY | all equal |
| object | object 1: OBJTYP (IIOBJ 0–2), REFTMP, AVGSTR, LMTSTR, PENVOL, RMDPTH, RMSTRK, ELPSOL, FRCMTH, TMPLMT (LLOBJ 0–3, 20, 21, 66, 52, 71); STROKE of object 2 = LLBND record 1 w20 = 222.30498 | all equal |
| inter-object | pair (1,2) = LLBRT record 7 (N = 7): contact flag 1, FRCFAC type 1 value 0.696, IHTCOF 1.35; mirror record 1 flag 2 | all equal |
| frame | CURSIM 7 5 0 = H[4], H[10], H[11]; H[8] = 0 (fold flag); object block STROKE z = H[505] | equal |
| object arrays | the 20 array keywords already verified value-by-value (spec §10) | unchanged |

2D structure check on a copy of a DEF_PRE-written 2D DB from the oracle cache (`MO2_Heatup_Rest_Upset_Boolean_EN.DB`,
step −1, mesh 5, 3 objects): MESHNO IISIM[3] = 5 = directory mesh; NMAT IISIM[29] = 2 → LLMPR 2·500, LLTFM 2²·50;
NOBJ IISIM[30] = 3 → IIOBJ 3·20; boundary count IISIM[31] = 3 → LLBRT 3²·50, LLBND 3·100; H1 = 3, S = H10 = 400,
F = H13 = 100. All as documented.

One error found and fixed this way: the inter-object helper had the boundary-count word swapped
(IISIM[24] is 3D, [31] 2D; COG: 7 boundaries = LLBND length / 200).

Not yet checked on data: 2D record contents word by word (only the 2D sizes and counts above; the oracle cache holds
12 DEF_PRE-written 2D DBs), GENDB byte output (Phase 4 test), older-version remaps (no such steps in the corpus).

## 8. What `deformdb` needs

Today it has the container, raw MAR / A, region names and typed access to ~52 node / element keywords. For
Phase 2 (DBREAD → model) and Phase 4 (model → DB) it needs, in this order:

1. **Function table** codec (IIFNC / LLFNC): directory, plain curves (shape supplied by the owner's kind code,
   `FUN_14012d2a0`), self-describing 10-word records, n1 / n2 headers; a writer that allocates numbers in GENDB order.
2. **Typed record views** with 2D / 3D index maps: SimCtrl (IISIM / LLSIM / IIUVL), Material (LLMPR) and pair
   (LLTFM), Object (IIOBJ / LLOBJ), LLBND (movement, rigid geometry), inter-object (LLBRT pair accessor), windows
   (LLWIN / LLWRZ), step header (per-object slot table `FUN_1401c7f80`).
3. **Width rule** of `FUN_1401d0320` (read only if stored width = expected) and the integration-point rule.
4. **Arrays not yet exposed**: element-face BCs, boundary faces (IIBDY / IIBEL / IIBND), fold meshes, user
   variable names, ELMNOD option-2 nodal blocks, `*DTMP`.
5. **Writer** (`deformdb::Writer` today writes raw arrays): region-size generator (`FUN_1401c03f0`,
   `FUN_1401c1980`), cursors and renumbering (materials, functions, windows, boundaries), the code conversions of §3,
   directory placement + truncation, NSTART sign rule, DB-header date / ISTART / ISMTYP rules, header statistics.
6. **Old versions** (< 11000): address-table remap already documented in spec §6.2, not implemented.

## 9. Open questions

* Meaning of several codes: material function variable codes, FRCFAC types 1–4 and negative references,
  window usages 3 / 4 / 8, step-header quantity codes 2600 / 2800 / 5000 / 1600 and 3D slot 115.
* Not followed: OPSTOP 20-real LLFNC record, LLOBJ 48 / 49 function (kind 0x7c, no KEY writer), mesoscale
  GRNDAT sub-records, `FUN_1401bb320` cleanup.
* Setters of `m+0x8` (direct-write flag; when 0 GENDB writes nothing), `m+0xc`, `m+0x10`, `m+0x1b5b`, `m+0x1b5d`,
  `m+0x2f89`; GENDB counts `m+0x74`, `0x84`, `0x90`–`0x9c`.
* CUSVAR: `objarrays.md` sees its list built only in DB code (KEY-only), `object.md` found no source; it is in
  no KEY export of the cached steps.
* Nodal vs element DAMAGE width (`m+0x33c8` vs `m+0x33d8`): offsets diverge if they differ.
