# Simulation control (keyword group 1) <-> .DB

Class: simulation-control sub-object `S = m + 0x750` of `_DB_STEP_DATA m`. Offsets below are `S+x`
(the KEY writer `FUN_1403d4990` addresses the same fields as `m + 0x750 + x`). `S+0xbc0` is a back pointer
to `m`. Word indexes are **0-based** inside the region record. `I[k]` = IISIM word k (int32),
`L[k]` = LLSIM word k (real64), `U` = IIUVL (int32 words used as text). "2D"/"3D" = `FUN_1400015e0(m)`
true/false (`FUN_1400015e0` is "is 2D": with it false the object count is read from IISIM[23], which
matches the tested 3D layout in db-format-spec §8.1).

Sources: DBREAD `loader_ann.c`, GENDB `gendb_ann.c`, KEY writer `kfw_ann.c`, KEY reader
`FUN_14039aa90` (`out/kw13/grp1.c`, switch on `id-0x16`). All rows are **[fact]** (read in the cited
function) unless marked **[inferred]**.

## 1. Read and write paths

### Read (DBREAD)

| Step | Function | What |
|---|---|---|
| 1 | `FUN_1401cca80` ReadStepHeader | CURSIM, OPRNAM, SIMNAM from the step header and directory; TITLE from the DB header (§2.1) |
| 2 | `FUN_1401cf4a0` ReadSimCtrl | sets `m+0x2220` = &IIFNC, `m+0x2228` = &LLFNC, `m+0x2230` = &LLWIN, `m+0x2238` = &LLWRZ (from the decoded MAR/A arrays); takes IISIM, LLSIM, IIUVL pointers; copies model counts (§4.1); calls `FUN_1401eb870(S, IISIM, LLSIM, IIUVL, IIFNC, LLFNC)`; then `FUN_1401cf930` (UNNAME) and `FUN_1401cfc30` (UENAME) |
| 3 | `FUN_1401eb870` | copies every IISIM/LLSIM field into S; two complete branches, 3D and 2D (§2) |
| 4 | `FUN_1401d0320` | model-level derived widths from STNOUT/ELMNOD/TIMEID... (§4.2) |

Old-version branch: `FUN_1401d2bb0` is `return 1` and `FUN_1401d2b70` returns 1 for every model, so
the `FUN_1401cf040` path (re-read a single MAR region via `FUN_1401cf220`, then `free`) and the
`FUN_1401d4400` paths inside `FUN_1401cf930/cfc30` are dead code **[fact]**. `FUN_1401eb870` has no
version test: steps < 11000 are read with the same word indexes after the region remap of spec §6.2
**[fact]**. The only version branch is in `FUN_1401cca80` (OPRNAM/CURSIM, §2.1).

### Write (GENDB)

| Step | Function | What |
|---|---|---|
| 1 | `FUN_1401bc230` -> `FUN_1401bea40` -> `FUN_1401ee650` | counts sim-ctrl functions into `m+0x88` (NFN) and LLFNC length `m+0x8c` (§5.5) |
| 2 | `FUN_1401bffb0` DB header | TITLE (S+0) -> DB header words 1-20; STYPE (forced 1 in 3D) -> ISMTYP; MESHNO, NSTART, STPINC, CURSIM[0] -> `FUN_1403b3f73` (directory entry) |
| 3 | `FUN_1401c4950(m, IISIM, LLSIM, LLCPU, LLFNC, IIFNC, IIUVL)` | calls `FUN_1401effc0(S, IISIM, LLSIM, IIUVL, IIFNC, LLFNC)`. **LLCPU is passed in but dropped**: nothing in the sim-ctrl writer touches LLCPU (width 50, `FUN_1401c4560`) |
| 4 | `FUN_1401effc0` | mirror of `FUN_1401eb870`, 3D and 2D branches; allocates function ids with `FUN_1401bb3d0` |
| 5 | `FUN_1401c4a30`, `FUN_1401c4be0` (from `FUN_1401c4d90`) | UNNAME / UENAME name blocks in the A array |
| 6 | `FUN_1401c5020` -> `FUN_1401e2180` step header | H[1] PDIE, H[2],H[3] TNOW, H[4] CURSIM[0], H[10],H[11] CURSIM[1..2], H[16..23] OPRNAM, H[24..33] SIMNAM |

KEY writer: `FUN_1403d4990` with helpers `FUN_1403d6580` (STNOUT), `FUN_1403d6620` (ELMNOD),
`FUN_1403d7720`..`FUN_1403d8420` (one per keyword, see the registry handler2 column).

## 2. Keyword table

### 2.1 Header / directory keywords

| KEY | Field | DB location | Read | Write | Notes |
|---|---|---|---|---|---|
| TITLE | S+0x000 char[81] | DB header record 2, bytes 0-79 | `FUN_1401cca80`: strcpy from DB object +0xa50 (filled when the DB header is read) | `FUN_1401bffb0`: if strlen>0 copy 80 chars to DB +0xa50, blank padded by `FUN_140323bf0` | not in any step array |
| CURSIM v1 | S+0xe4 int | directory entry word 6 (SimNo); step header H[4] | `FUN_1401cca80`: = dir entry +0x14 | `FUN_1401e2180` H[4]=(double); `FUN_1401bffb0` passes it to the directory | |
| CURSIM v2 | S+0xe8 int | H[10] (real) | `(int)H[10]` (v<11000: = v1) | H[10]=(double) | |
| CURSIM v3 | S+0xec int | H[11] (real) | `(int)H[11]` | H[11]=(double) | `FUN_1401cca80` sets v2/v3 again from H[10]/H[11] at the end for every version |
| OPRNAM | S+0x51 char[65] | H[16..23], 64 chars | v>=11000: 64 chars, text swap `FUN_140324020(.., m+0x748, 8, 0)`, trailing blanks trimmed `FUN_1403239f0`; v<11000: copy of SIMNAM (64) | `FUN_140323bf0(H+16, S+0x51, 64)` blank padded | |
| SIMNAM | S+0x92 char[81] | H[24..33], 80 chars | swap (10 words) + trim; 3D v<3000: forced "Operation" | `FUN_140323bf0(H+24, S+0x92, 80)` | |

`m+0x748` (text platform code) = `(int)H[8]` when dir entry +0x1c == 2, else `(int)H[5]` (`FUN_1401cca80`).
`FUN_140324020` swaps 4-byte words (and then 8-byte pairs when the last argument is 0) only when
host*100 + file code is in {1,2,3,100,200,300} **[fact]**.

### 2.2 IISIM / LLSIM keywords

Column "3D" / "2D" gives the DB word. "-" = not stored for that dimension.

| KEY | Field(s) | 3D | 2D | Read transform (`FUN_1401eb870`) | Write transform (`FUN_1401effc0`) / notes |
|---|---|---|---|---|---|
| SMODE | S+0xf4 | I[0] | I[0] | copy | copy. KEY writer recomputes it first (`FUN_1401f3f80`): TRANS[0]&&TRANS[1] -> 3, TRANS[0] only -> 1, else 2 |
| STYPE | S+0xf8 | I[49] | I[1] | 3D: `<1 -> 1` | copy |
| GEOTYP | S+0x100 | - | I[2] | copy | 2D only (KEY too). GEOTYP==3 -> 3 vector components (§4.2) |
| MESHNO | S+0xf0 | I[1] | I[3] | copy | copy. KEY skipped when `m+0x1744 & 4` |
| UNIT | S+0xfc | I[18] | I[25] | copy | copy |
| TNOW 1 (global time) | S+0x640 | L[0] | L[0] | copy | copy; also H[2], H[3]. KEY writes -1e21 when `m+0x1744 & 8` |
| TNOW 2 | S+0x648 | L[50] | L[42] | copy | copy |
| TNOW 3 (int) | S+0x650 | L[51] | L[43] | `(int)` | `(double)` |
| TNOW 4 | S+0x658 | L[56] | L[39] | copy | copy (KEY: 0 when `m+0x1744 & 8`) |
| TRANS 1..4 | S+0x104..0x110 | I[44..47] | I[44..47] | copy | copy |
| TRANS 5 | S+0x114 | I[48] | I[50] | copy | copy |
| HTMTHD | S+0x118 | I[43] | I[43] | copy | copy |
| STNOUT 1..7 | S+0x11c..0x134 | I[50..56] | I[52..58] | copy; after both branches: if all 7 are 0 -> STNOUT 5 (S+0x12c) = 1 | copy |
| ELMNOD 1..10 | S+0x138..0x15c | I[57..66] | I[59..68] | copy | copy |
| NSTART | S+0x160 | I[2] | I[4] | copy | copy. KEY skipped when `m+0x1744 & 1` |
| STPINC | S+0x164 | I[5] | I[7] | copy | copy |
| PDIE 1 | S+0x168 | I[7] | I[9] | copy | copy; also H[1] |
| PDIE 2 | S+0x9b4 | I[35] | I[36] | copy | copy |
| OSCTRL 1..4 | S+0x9b8, 0x9bc, 0x9c0, 0x9c4 | - | I[41], I[42], I[48], I[49] | copy | 2D only |
| NSTEP | S+0x16c | I[4] | I[6] | copy | copy |
| EMAX | S+0x170 | L[10] | L[20] | copy | copy |
| TMAX | S+0x178 | L[9] | L[21] | copy | copy |
| SMAX 1..3 | S+0x180..0x190 | L[11..13] | - | copy | 3D only; 2D KEY writes 2 values that are never stored |
| VMIN 1..3 | S+0x198..0x1a8 | L[14..16] | - | copy | as SMAX |
| LMAX 1..3 | S+0x1b0..0x1c0 | L[17..19] | - | copy | as SMAX |
| STPDEF | S+0x1c8 | I[6] | I[8] | copy | copy |
| DEMAX | S+0x260 | L[5] | L[4] | copy | copy |
| DSMAX | S+0x1e0 | L[6] | L[5] | `FUN_1401f4390` (store) | copy. KEY written only if no DTTYPE function and S+0x240 == 0 |
| DTMAX 1,2 | S+0x1d0, S+0x1d8 | L[3], L[55] | L[2], L[9] | copy | copy. KEY: both if S+0x240==1, else `0 DTMAX2` if DTMAX2>0 |
| DTTYPE | S+0x1e8 FUNCTION_DATA, S+0x240 mode | I[67] type, I[68] fid | I[69], I[70] | type 0: ndim=0 (S+0x1ec), S+0x1f0=0. type 1..4 (other -> 1): ndim=1, xvar S+0x1f8 = 1,2,1,2, mode S+0x240 = 0,0,1,1, curve from fid (§5.3). Then `FUN_1401ee510`: no curve -> S+0x240 = 1 if DSMAX <= 0 else 0 | ndim 0 -> type 0, fid 0; else mode 0: xvar 1 -> 1, 2 -> 2; mode 1: xvar 1 -> 3, 2 -> 4; other xvar -> 1. fid from `FUN_1401bb3d0`, len = 2·npts·ncomp |
| SLDERR | S+0x268 | L[4] | L[3] | copy | copy |
| DTPMAX 1..3 | S+0x270, 0x2a0, 0x2a8 | L[38], L[39], L[40] | L[6], L[26], L[27] | copy | copy |
| DTPMAX 4,5 | S+0x2b0, 0x2b8 | - | L[47], L[48] | copy | 2D only (KEY writes 5 values in 2D, 3 in 3D) |
| DVMAX 1,2 | S+0x278, 0x280 | L[7], L[8] | L[24], L[25] | copy | copy |
| MDSOBJ 1..3 | S+0x288..0x290 int | L[34..36] | L[28..30] | `(int)` | `(double)` |
| MDSOBJ 4 | S+0x298 | L[37] | L[31] | copy | copy |
| CNTERR | S+0x9c8 int, S+0x9d0 | - | L[44] (int), L[45] | `(int)` / copy | 2D only |
| VFACTR mode | S+0x9d8 | I[80] | I[51] | copy | copy |
| VFACTR list | S+0x9dc count, FUNCTION_DATA S+0x9e0 (ncomp S+0xa20, values ptr S+0xa28) | I[81] count, I[82] fid | - | count -> S+0x9dc and S+0xa20; if mode==1 and count>0: read fid with `FUN_1401309b0` | 3D only. Written only if mode==1 && count>0: I[81]=ncomp, I[82]=fid, len `FUN_140130290` = ncomp. Reader skips a 1-value list (needs ncomp>1, §5.3) |
| DTSUB | S+0x2c0 | L[33] | L[49] | copy | clamped and stored back into S: 0<=x<0.5 kept; 0.5<=x<=1 -> 1; x==2 -> 2; else 3D 1.0, 2D 0.0 |
| DPLEN | S+0x2c8 | L[52] | - | copy | 3D only (KEY too) |
| ODMAX | S+0xbc8 | L[54] | - | copy | 3D only (KEY via `FUN_1403d8490`) |
| CVGERR 1,2 | S+0x2d0, 0x2d8 | L[20], L[21] | L[7], L[8] | copy | copy |
| ITRMTH | S+0x2e0 | I[9] | I[16] | copy; 2D: 3 -> 1 | copy |
| SOLMTD | S+0x2e4 + 4 params S+0x2f0..0x308 (real) | I[32]; params I[72..75] (type 2) or L[61..64] (type 6) | I[15]; same param words | type 2: `(double)` of ints, S+0x2e8=4; type 6: copy, S+0x2e8=4 | type 2: `(int)`; type 6: copy |
| ITRMXD | S+0x310 | I[10] | I[17] | copy | copy |
| DEFBWD | S+0x314 | I[11] | I[19] | copy | copy |
| SOLMTT | S+0x318 | I[15] | I[22] | copy | copy |
| ITRMXT | S+0x31c | I[16] | I[23] | copy | copy |
| TMPBWD | S+0x320 | I[17] | I[24] | copy | copy |
| LATENT | S+0x324 | I[13] | I[40] | copy | copy |
| FRQNCY | rows S+0x328, cols S+0x32c, FUNCTION_DATA S+0x330 (ncomp S+0x370, values S+0x378) | I[76], I[77], I[78] fid | same | rows,cols>=1: ncomp=rows·cols, read fid; else `FUN_1401e9850` default 6x1 = {1,0,100,20,1,1} (type 0xb5) | I[78]=0 unless rows,cols>0 -> fid, len = ncomp. KEY written only if rows·cols == ncomp |
| GEOERR 1,2 | S+0x660, 0x668 | L[23], L[24] | L[12], L[13] | copy | copy |
| GEOERR 3,4 | S+0x670, 0x678 | L[59], L[60] | L[50], L[51] | copy | copy |
| PENINF | S+0x3a8 | L[22] | L[10] | copy | copy |
| UNTE2H | S+0x3b0 | L[27] | L[16] | copy | copy |
| TINTGF | S+0x3b8 | L[26] | L[15] | copy | copy |
| BLZMAN | S+0x4b8 | L[32] | L[22] | copy | copy |
| ENVTMP | FUNCTION_DATA S+0x3c8 (ndim S+0x3cc, const S+0x3d0, xvar S+0x3d8, npts S+0x3e4); furnace flag S+0x3c0 | 3D: L[57] = 0 or 10 (furnace); L[28] fid, L[29] const | 2D: L[23] fid, L[17] const | 3D L[57]==0 / 2D: fid==0 -> ndim 0, const; else ndim 1, xvar 1, curve. 3D L[57]==10 -> S+0x3c0=1, furnace record fid L[58] (§5.4). Other L[57] values: nothing set | mirror; furnace: L[57]=10.0, L[58]=fid. KEY: `ENVTMP 0 c`, `ENVTMP 1 npts`+curve, or `ENVTMP 10 nzones` |
| FRNNAM | furnace object S+0x440 (name at +0) | furnace record words 1..8 | - | `FUN_1403759b0` | `FUN_140375bb0`; 3D only; KEY only when S+0x3c0 |
| CNVCOF | FUNCTION_DATA S+0x4c0 (ndim 0x4c4, const 0x4c8, xvar 0x4d0=3, npts 0x4dc) | L[30] fid, L[31] const | L[18], L[19] | fid==0 -> const, else curve with xvar 3 | mirror |
| ENVATM | FUNCTION_DATA S+0x538 (0x53c, 0x540, xvar 0x548=1, 0x554) | L[41] flag, L[42] const/fid | L[35], L[36] | `(int)flag==0` -> const; else fid=(int)L[42] | flag written 0.0 or 1.0 |
| ACVCOF | FUNCTION_DATA S+0x5b0 (0x5b4, 0x5b8, xvar 0x5c0, 0x5cc) | L[43] type, L[44] const/fid | L[37], L[38] | type 0 const; 1/2/3 -> curve with xvar 3/12/1 | type = 1.0/2.0/3.0 from xvar 3/12/1 |
| ENVMPR | S+0x628 | L[48] | L[40] | copy | copy |
| ENVMPT | S+0x630 | L[49] | L[41] | copy | copy |
| EHRATE | S+0x638 | L[53] | L[46] | copy | copy |
| TIMEID | FUNCTION_DATA S+0xa58 (npts S+0xa74, ncomp S+0xa98) | I[71] fid | I[71] | fid>0 -> `FUN_140130dc0` (self-describing, §5.2) | npts<1 -> 0, else `FUN_140131600` |
| ERECID | S+0xad0 (npts 0xaec, ncomp 0xb10) | I[83] | I[83] | as TIMEID | as TIMEID |
| NRECID | S+0xb48 (npts 0xb64, ncomp 0xb88) | I[84] | I[84] | as TIMEID | as TIMEID |
| USRDEF | S+0x688 + 81·i, i=0..9 | IIUVL bytes 80·i..80·i+79 (words 20·i..) | same | strncpy 80, swap ints only `FUN_140324020(.,.,10,1)`, trim; all-blank -> "" | blank fill 80, copy strlen (max 80). KEY count = last non-empty index+1 |
| UNNAME | model `m+0x33a0` char*[W], W=`m+0x3398` = width(LLND1) | 3D: A[ASTART(LLND1)-1 + NUMNP·W], 8 words (64 chars) per name; 2D: A slot 57 start (fixed `m+0x1ea4`) | same | `FUN_1401cf930`: W = AWIDTH(LLND1); for each object block i (stride W·64 bytes) fill names still empty; swap 8 words + trim | `FUN_1401c4a30` writes one block (blank fill 64, copy ≤64); W = max(2, n) if n≥3 else 2 (`FUN_1401be9d0`) |
| UENAME | `m+0x33b0`, W=`m+0x33a8` = width(LLET1) | 3D: A[ASTART(LLET1)-1 + NUMEL·W]; 2D: A slot 56 start (`m+0x1ea0`) | same | `FUN_1401cfc30`, as UNNAME | `FUN_1401c4be0`, `FUN_1401be960` |

The 2D name slots 56/57 are hard-coded A-table offsets (0x1ea0/0x1ea4); `region_globals.tsv` names
2D slots 56/57 LLZMI/LLODF, so the 2D region name is uncertain **[inferred]**.

## 3. KEY-only, input-only, unreached

| KEY | Class | Reason |
|---|---|---|
| CVNAME | KEY-only | written from model container `m+0x33b8` (`FUN_14019a4e0`, `FUN_14032f1e0`...); KEY reader case 0x44 fills it; no reference to `m+0x33b8` in the DBREAD or GENDB closures |
| DSMAX_NSTEP | input-only | KEY reader case 0x1b converts it: DSMAX (`m+0x930`) = (stroke − PDIE object +0x22e0) / nsteps; not written to KEY or DB |
| INIGES | input-only | KEY reader case 0x28 reads one int into a local and drops it; menu `FUN_140005520` shows it as "N/A" |
| SMAX, VMIN, LMAX (2D) | KEY-only in 2D | 2D branches of reader/writer do not touch S+0x180..0x1c0 |

No group-1 keyword is unreached.

## 4. DB words with no keyword

### 4.1 Model counts in IISIM (written by `FUN_1401effc0`, read by `FUN_1401cf4a0`)

| Model field | 3D word | 2D word | Read into | Meaning |
|---|---|---|---|---|
| `m+0x68` (`FUN_1401f45b0`) | I[3] | I[5] | - | unknown count, not read back |
| `m+0x6c` | I[19] | I[26] | `m+0x13e8` | NUMNP (all objects) |
| `m+0x70` | I[20] | I[27] | `m+0x13ec` | NUMEL |
| `m+0x74` | I[21] | I[28] | - | unknown |
| `m+0x78` | I[22] | I[29] | `m+0x1320` | number of materials |
| `m+0x7c` | I[23] | I[30] | `m+0x1378` | number of objects |
| `m+0x80` | I[24] | I[31] | `m+0x13f0` | total boundary count N (LLBND records; LLBRT is N x N records, see interobj.md); checked on COG step 25: 7 |
| `m+0x84` | I[25] | I[32] | - | unknown |
| `m+0x88` | I[26] | I[33] | `m+0x13fc` | NFN = number of functions in IIFNC |
| `m+0x8c` | I[27] | I[34] | - | LLFNC length (words) |
| `m+0x90, 0x94` | I[28], I[29] | - | - | unknown (3D) |
| `m+0x98` | I[30] | - | - | unknown (3D) |
| `m+0x9c` | I[31] | - | - | unknown (3D) |
| `m+0xbc` | I[33] | I[37] | `m+0x13f4` | number of inter-material relations ("NoRelations") |
| `m+0xc0` | I[34] | I[39] | `m+0x13f8` from **I[39] in both dims** | number of windows ("NoWindows") |

3D mismatch **[fact]**: the 3D writer puts `m+0xc0` in I[34] and never writes I[39], but the reader
takes NoWindows from I[39] (byte 0x9c) for both dimensions.
Constants written: 3D I[8]=I[12]=I[14]=0; 2D I[11]=0, I[12]=1, I[13]=1, I[18]=1.

### 4.2 LLSIM words without keyword

| Word | Content |
|---|---|
| 3D L[2] | `FUN_1401f3bb0`: sqrt of the sum of squares of PDIE object `+0x22d0,+0x22d8,+0x22e0` (2D: first two) via `FUN_14021e170`; 0 if no PDIE object. Not read back. **[inferred]** die stroke/velocity magnitude |
| 3D L[45..47], 2D L[32..34] | constants 1.0, 2.0, 3.0 |
| 2D L[14] | S+0x680 (no keyword in the KEY writer; copied both ways) |
| unused | 3D L[1], L[25], L[65..99]; 2D L[1], L[11], L[52..60], L[65..99]; 3D I[36..42], I[69], I[70], I[79], I[85..99]; 2D I[10], I[14], I[20], I[21], I[35], I[38], I[79..82], I[85..99] |

IIUVL words 200..499 are not used. LLCPU (50 words) is neither read nor written by this class.

### 4.3 Model-level derived fields (`FUN_1401d0320`, after all classes are read)

`m+0x15f8` is a 79-int table of component counts per dynamic variable (`D[i]`, passed to the object
readers; `FUN_1401d64f0(m,i)` tests `D[i]>0`). dim = `FUN_140158e90(m)`.

| Item | Rule |
|---|---|
| D[all] | 1, then D[0..6], D[18], D[19], D[25] = dim; 3D: D[46] (`m+0x16b0`) = 0 or 9 if `FUN_1401d64f0(m,0x2e)` |
| D[24] | 5 if `FUN_1401d64f0(m,0x18)` (always true) |
| D[38], D[26], D[39], D[57] | 2·dim |
| D[40] (stress output width) | STNOUT6 + STNOUT7 + max(1, ΣSTNOUT1..5) · 2·dim |
| D[47], D[56], D[58] | 2; D[59] = 2 (2D) / 3 (3D) |
| D[49..52] | nmat, nmat, inter-material count, same (if D[49]>0) else 0 |
| D[53], D[29], D[54], D[55] | counts returned by `FUN_1401d64f0` for ids 0x35, 0x1d, 0x36, 0x37 (material grain / phase data) |
| D[17] / D[60] | UNNAME count `m+0x3398` / UENAME count `m+0x33a8` |
| 2D | D[37] = 4, D[62] = dim; GEOTYP==3: D[2], D[3], D[4], D[18], D[19] = 3; D[38], D[26], D[39], D[57] = 6; D[40] = ΣSTNOUT·6 + STNOUT6+7 |
| 3D | D[37] = 0, D[62] = dim |
| ELMNOD1 (S+0x138) ∈ {0,3} | D[28] = 0, else `m+0x33c8` |
| ELMNOD2 / ELMNOD3 ∈ {0,3} | D[27] = 0 / D[26] = 0 |
| D[45] | `m+0x33d8` |
| D[34], D[63], D[35] | TIMEID npts, ERECID npts, NRECID npts |

The same routine exists on the GENDB side (gendb_ann.c line ~13880). The meaning of each D index is
owned by the object arrays **[inferred]**.

## 5. Function table (IIFNC / LLFNC)

Shared by all classes. Pointers: read `m+0x2220` (IIFNC), `m+0x2228` (LLFNC); write `m+0x50`/`m+0x58`
buffers at the region starts. Region widths: IIFNC 0, LLFNC 2 (`FUN_1401c4560`). Lengths: IIFNC
2 + 2·NFN, LLFNC = `m+0x8c`.

### 5.1 IIFNC directory (allocator `FUN_1401bb3d0`, GENDB)

Function ids are 1-based and allocated in write order (counter `m+0x10c`, reset in `FUN_1401baf30`).

| IIFNC word (0-based) | Content |
|---|---|
| 0 | 1 (start of function 1) |
| 2k−2 | 1-based start of function k in LLFNC |
| 2k−1 | attribute of function k (caller-supplied; 0 for sim-ctrl curves, zone index for furnace zones) |
| 2k | start of function k+1 = start(k) + length(k) |
| 2·NFN+1 | not written |

`FUN_1401bb3d0(m, &len, IIFNC, &attr, &fid_out(double), &start_out)` warns "FNCLOC Func # %d exceeds
allocated number" when the id > `m+0x88` and "FNCLOC address exceeds allocated size" when the end > `m+0x8c`.
Data is then written at `LLFNC + start − 1`. The id is stored in the owning record as an int (IISIM)
or real (LLSIM).

Reader: length(k) = IIFNC[2k] − IIFNC[2k−2]; data = LLFNC[IIFNC[2k−2] − 1 ...];
`fd.attr (+0x54) = IIFNC[2k−1]`. A function with length ≤ 0 or id ≤ 0 is skipped.

### 5.2 FUNCTION_DATA object (ctor `FUN_14012d2a0(fd, type)`)

| Offset | Field |
|---|---|
| +0x00 | type code (e.g. 0x7c, 0xa7..0xb5); ctor sets ndim/xvar/ncomp per type |
| +0x04 | ndim: 0 constant / list, 1 curve, 2 or 3 table |
| +0x08 | constant value (double) |
| +0x10, +0x14, +0x18 | x-variable code per dimension |
| +0x1c, +0x20, +0x24 | number of points per dimension |
| +0x28, +0x30, +0x38 | x arrays (double*) |
| +0x40 | ncomp (values per point, `FUN_14012fc00` forces ≥1) |
| +0x48 | y values (double*, ncomp·Πnpts; `FUN_14012ff20`) |
| +0x54 | attribute from IIFNC |

### 5.3 Plain function layout (writer `FUN_140131360` = WriteFuncToDBArray, reader `FUN_1401309b0`)

No header: ndim, ncomp and the x-variable codes are **not stored**; the reader relies on the object
being pre-shaped by its owner (e.g. S+0x1ec=1 and xvar set before the call).

| ndim | LLFNC words |
|---|---|
| 0 | only for types 0xa7, 0xa8, 0xaa, 0xab, 0xad, 0xb3, 0xb5: ncomp values y[0..ncomp−1]. Reader needs ncomp > 1 |
| 1 | per point: x, y[0..ncomp−1] (stride ncomp+1). Reader: if ncomp == 1, npts = length/2. Type 0x7c: ncomp values, x[0] = 0 |
| 2, 3 | x arrays of each dimension back to back (npts_d words each), then the y block (ncomp·Πnpts). The reader can take the dims from a 2/3-word real prefix when its flag argument is set; every sim-ctrl call passes 0 |

Length used by the sim-ctrl writer: curves `2·FUN_14012ff20` (= 2·npts·ncomp); FRQNCY/VFACTR
`FUN_140130290` (ndim 0: ncomp; ndim 1: `FUN_140130200` = 2·npts or ncomp for 0x7c; ndim 2/3:
Σnpts + Πnpts·ncomp).

### 5.4 Self-describing layout (TIMEID/ERECID/NRECID and furnace zones)

Writer `FUN_140131600`, reader `FUN_140130dc0`, length `FUN_140130380` = data + 10.

| Word | Content |
|---|---|
| 0 | tag 10.0 (reader requires it) |
| 1 | ndim |
| ndim 0: 2, 3 | ncomp, constant |
| ndim 1: 2, 3, 4 | xvar, npts, ncomp (if npts ≤ 0: constant instead of ncomp) |
| ndim 2: 2..6 | xvar1, xvar2, npts1, npts2, ncomp |
| 10... | data: ndim 1 interleaved as §5.3 (`FUN_1401317c0`, length npts·(ncomp+1) rounded up to even); ndim 2 as §5.3 (`FUN_140131fa0`) |

ndim 3 writes only words 0-1; the reader ignores it.

Furnace (ENVTMP 10, 3D; writer `FUN_140375bb0`, reader `FUN_1403759b0`): a 20-word record
(`FUN_140375e70` = 20) at fid F: [0] = 20.0, [1..8] FRNNAM (64 chars, swapped 8 words on read),
[9] first zone fid, [10] last zone fid. Zone functions are allocated right after F, one per zone,
self-describing, with IIFNC attribute = zone number. Reader: nzones = last − first + 1.

### 5.5 Sizing (`FUN_1401ee650`, adds to NFN and LLFNC length)

Each present curve (ENVTMP, CNVCOF, ENVATM, ACVCOF, DTTYPE): +1, +2·npts·ncomp; furnace: +1, +20
and per zone +1, +len+10; TIMEID/ERECID/NRECID (only if npts>0 and ncomp>0): +1, +len+10; VFACTR
(mode 1, count>0) and FRQNCY (rows, cols > 0): +1, +`FUN_140130290`.
Note: the writer emits TIMEID-type records when npts ≥ 1 even if ncomp = 0 (the sizing skips them).

## 6. What `deformdb` needs for this class

1. A typed `SimCtrl` view with two index maps (3D/2D) for IISIM/LLSIM as in §2.2, plus header/directory
   items (CURSIM, OPRNAM, SIMNAM) and the DB title.
2. A function-table reader/writer: IIFNC directory (§5.1), plain curves with caller-supplied
   shape (§5.3), self-describing records (§5.4), furnace record. Writer must allocate ids in the
   GENDB order and maintain `m+0x88`/`m+0x8c` (IISIM NFN / LLFNC length).
3. Computed values on write: SMODE from TRANS (KEY only), DTTYPE code from (xvar, mode), DSMAX/DTMAX
   mode flag S+0x240, DTSUB clamp, FRQNCY default, STNOUT5 default, model counts (§4.1), L[2] die
   magnitude, constants (§4.1, §4.2).
4. USRDEF text in IIUVL (10×80, int-word swap only) and UNNAME/UENAME blocks at the tail of
   LLND1/LLET1 (3D) or fixed A slots 57/56 (2D).
5. Derived component table D[] (§4.3) if object arrays are decoded.
6. Reproduce the 3D NoWindows quirk (reader I[39], writer I[34]) or flag it.
