# DEFINT — interpolate DB state data onto the current mesh (`DEF_INT` / `DEF_REM3_GUI`)

Decompiles: `deform-rea-work/out/p89/defint.c` (handler, depth 2), `out/p89/b/{frz2,misc,kr,kf}.c`.
Shared mechanics (dispatch, readers, launcher `FUN_140223790`): see [common.md](common.md).

## 1. Summary

| | |
|---|---|
| Registry id | 294 (`DEFINT`) |
| Dispatcher case | `0x11f+7` in `FUN_140378e00` |
| Handler | `FUN_1401aa000(model, &objId)` |
| 2D (model+0x1c != 3) | `FUN_140155c90` → external **`DEF_INT`** (2D folder) |
| 3D (model+0x1c == 3) | `FUN_140156950` → external **`DEF_REM3_GUI`** (3D folder) |

The command takes the current mesh of object `objId`, hands it together with the database named
by the last `DBREAD` (model+0x1748) and its step (model+0x744) to the remesh/interpolation program,
which maps the state variables of that DB step onto the mesh and writes a KEY file. DEF_PRE reads
that KEY back into the model (same object, so the object's state arrays are replaced).
No mesh is generated: this is the "data interpolation" half of remeshing. REMESH/DEFAMG
reach the same two routines through `FUN_1401576a0` → `FUN_140157780` (mode 0/1), so this page is
the reference for the DEF_INT interpolation step.

Object/dimension switch: the handler picks the 2D or 3D routine with `FUN_1400015f0(model)`
(`model+0x1c == 3`), i.e. anything that is not 3D goes to the 2D routine.

## 2. Argument grammar

```
DEFINT <objId>
```

* Dispatcher: `FUN_140413de0(f, &objId, 0)` — rest of the keyword line, `%i`. Non-zero reader
  return (EOF / empty line) → return 2 ("Reading Error").
* `FUN_1401d6ea0(model, objId)` == 0 (no such object) → return 2.
* Otherwise the handler runs; its return value is ignored and the case returns 0.
* No further lines are read.

KEY example (2D, data from the DB last read):

```
DBREAD 0
HAMMER_SI.DB
DEFINT 1
KFWRIT
OUT.KEY
```

## 3. Model / object fields used

| Field | Meaning | Use |
|---|---|---|
| model+0x1c | dimension (2/3) | 2D vs 3D routine (`FUN_1400015f0` ==3, `FUN_1400015e0` ==2) |
| model+0x340 | PROBID string | copied to mg+0x3f60 by `FUN_140148840`; names `<PROBID>.RMS/.GEO/.RES` |
| model+0x744 | DB step number (set by DBREAD / DB load) | mg+0x4b90 → INI "step" line |
| model+0x1748 | DB file name (set by DBREAD, `FUN_140378e00` case 2) | mg+0x4790 (strncpy 0x400) and mg+0x4360; empty → "Skip data interpolation" |
| model+0x1740 | KEY-read mode mask (set during `FUN_1401b5f00`) | 0xf during read-back (see §6) |
| obj+8 | object number | INI and KEY lines |
| obj+0x880 / +0x884 | node count / element count | mesh files |
| obj+0x894 | nodes per element (3D) | `SCRATCH.KEY` |
| obj+0x8a0 | node-data block; `FUN_1401808d0` returns coordinates (+8): 2 doubles/node (2D), 3 (3D) | mesh files |
| obj+0x9e0 | element block; `FUN_140183cb0` returns connectivity (+8), `FUN_140183d00` per-element material group (+0x18) | mesh files |

Mesh-generation helper object ("mg", `_MeshGenBase`, 19 424 bytes on the stack):
`FUN_14013e790` constructs it, `FUN_140148840(mg, obj, model, model+0x340)` sets mg+8 = obj,
mg+0x3f48 = model, mg+0x3f60 = PROBID, copies the object into mg+0x18 (`FUN_1401613a0`) and
sets mg+0x10 = mg+0x18. `FUN_1401575a0(mg, 1, &objPtr, model+0x1748, model+0x744)` sets
mg+0x4778 = 1 (number of source objects), mg+0x4780 = pointer to the source-object pointer array
(here the target object itself), mg+0x4790/mg+0x4360 = DB name (`FUN_140157610`), mg+0x4b90 = step.
The routines set mg+0x3f40 = 1 (interpolation mode; 2 = force interpolation, see DEFFRZ.md).
After the routine: `FUN_14013e880(mg, 1)` (cleanup, §5) and `FUN_14013e840(mg)` (destructor).

## 4. Files written for the external program

### 2D — `FUN_140155c90(mg, model+0x1748)`

Order:

1. mg+0x3f40 = 1; mg+0x4360 = DB name (or `"DEFORM"` if the argument is NULL/empty — not reachable
   from DEFINT, the handler already skipped empty names).
2. `rms = sprintf("%s.RMS", PROBID)`.
3. **`DEF_INT.ACT`** (`fopen "wt"`; failure → return 0): `fprintf("%s", ...)` of
   `sprintf("\n5\nY\n1\n%s\n\n6\n\n", rms)`, i.e. the lines
   `""`, `5`, `Y`, `1`, `<PROBID>.RMS`, `""`, `6`, `""` (stdin script for DEF_INT's menu).
4. **`DEF_REM.INI`** (`FUN_1401562f0`, "wt"; failure → return 0):
   ```
   "%s\n"   DB name (mg+0x4790, as given to DBREAD, path included)
   "%d\n"   step (mg+0x4b90 = model+0x744)
   "%d\n"   2            (constant)
   "%d\n"   object number of the source object (**(mg+0x4780))+8 — the DEFINT object
   ```
5. Output name: take the DB name, strip the directory (`find_last_of("/\\")`, substring after it),
   and if it ends in `.DB` (case-insensitive, length > 3, `FUN_1401b0cc0`) replace the last 3
   characters by `_REM.KEY`: `HAMMER_SI.DB` → `HAMMER_SI_REM.KEY`. If the name does not end in
   `.DB` the bare file name itself is used as the output name (quirk: it is then deleted in step 7
   and later read as a KEY file).
6. **`<PROBID>.RMS`** (`FUN_1401563c0(mg, rms)`, "wt"; failure → delete `DEF_INT.ACT`,
   `DEF_REM.INI`, return 0). With mg+0x3f40 = 1 (DEFINT) the format is:
   ```
   "%10d %10d\n"           nnodes (obj+0x880), 1
   nnodes x "%10d %15.7f %15.7f\n"   i+1, x, y   (coords from FUN_1401808d0(obj+0x8a0))
   "%10d %\n"              nelem (obj+0x884)       <- format string verbatim; prints "%10d " + newline, see §9
   nelem x ( "%10d " i+1, then 4 x "%10d " node ids, then "\n" )   (FUN_140183cb0(obj+0x9e0))
   ```
   (With mg+0x3f40 = 2 — DEFFRZ — the header lines are `"%10d\n"` nnodes and `"%10d %5d\n"` nelem, 4.)
   Element ids are always 4 per element (quads; 2D triangles are stored with a repeated node, inferred).
7. Delete `DEF_INT.OUT` and the output KEY name (`<stem>_REM.KEY`).
8. `FUN_1400b9130("DEFORM_UIRUNREMESH.DAT", "2D")`: create the file ("wt") containing the 2 bytes
   `2D` (no newline).

### 3D — `FUN_140156950(mg, ...)` (the second argument is ignored)

1. mg+0x3f40 = 1.
2. **`DEF_INT.ACT`** ("wt"; failure → return 0): `"%s"` of `"\n"` — one empty line.
3. **`DEF_REM3.INI`** (`FUN_140156ba0`, "wt"; failure → return 0):
   ```
   "%s\n"  PROBID (mg+0x3f60)
   "%s\n"  DB name (mg+0x4790)
   "%d\n"  step (mg+0x4b90)
   "%d\n"  source object number (**(mg+0x4780))+8
   "%d\n"  1
   "%d\n"  102 (0x66) when mg+0x3f40 == 1 (DEFINT); 2 when mg+0x3f40 == 0; no line otherwise
   ```
4. **`SCRATCH.KEY`** (`FUN_140156cd0(mg, "SCRATCH.KEY")`, "wt"; failure → delete `DEF_INT.ACT`,
   `DEF_REM3.INI`, return 0). Mode 0/1 format (a KEY fragment):
   ```
   "RZ %10d %10d\n"                    objNo, nnodes
   nnodes x "%10d %15.7f %15.7f %15.7f\n"   i+1, x, y, z
   "ELMCON %10d %10d %5d\n"            objNo, nelem, npe (obj+0x894)
   nelem x ( "%10d " i+1, npe x "%10d " node ids, "\n" )
   MTLGRP block (FUN_1402f4df0, below)
   ```
   (Mode 2 — DEFFRZ — writes `"%10d\n"` nnodes and `"%10d %5d\n"` nelem, npe instead of the two
   keyword header lines; the MTLGRP block is written in every mode.)
5. Delete `DEF_INT.OUT`.

   MTLGRP block: `FUN_1402f4df0(f, FUN_1402f7720(0xae), objNo, mtlgrp, nelem, 1, 1)`;
   `FUN_1402f7720(0xae)` returns the registry name of id 174 = `MTLGRP`
   (`keyword_registry.tsv`); `mtlgrp` = `FUN_140183d00(obj+0x9e0)` (one int per element).
   Nothing is written if nelem == 0 or the array is NULL. If all values equal the first:
   `"%-6s %7i %7i %7i\n"` (`MTLGRP`, objNo, 0, value). Otherwise
   `"%-6s %7i %7i %7i\n"` (`MTLGRP`, objNo, count of elements whose value != 1, 1) followed, for
   each such element, by `" %7i"` element number and `" %7i"` value, then `"\n"`
   (`FUN_1402f2fe0`, 10 values per line, continuation lines start with 8 spaces).

## 5. Launch

| | 2D | 3D |
|---|---|---|
| format | `sprintf("%s < %s > %s", "DEF_INT", "DEF_INT.ACT", "DEF_INT.OUT")` | `sprintf("%s < %s > %s", "DEF_REM3_GUI", "DEF_INT.ACT", "DEF_INT.OUT")` |
| command line | `DEF_INT < DEF_INT.ACT > DEF_INT.OUT` | `DEF_REM3_GUI < DEF_INT.ACT > DEF_INT.OUT` |
| wrapper | `FUN_140224100(1, cmd)` → dirKind 2 (`2D\`) | `FUN_140224130(1, cmd)` → dirKind 3 (`3D\`) |

Mode 1 = `system()` through `cmd`, waits, exit code ignored (common.md). Working directory = DEF_PRE's.
Success is detected only by the presence of the output KEY file.

Deletions after the run:

* 2D: `DEFORM_UIRUNREMESH.DAT`, `<PROBID>.GEO`, `<PROBID>.RES`, `<PROBID>.RMS`, `DEF_INT.ACT`,
  `DEF_INT.OUT`, `REM_SCR.SCR`, `NEW.MSH`, `STEP.DAT`; then `FUN_1401b8560(<stem>_REM.KEY)` — not
  readable → return 0.
* 3D: `<PROBID>.GEO`, `<PROBID>.RES`, `NBCD.DAT`, `SCRATCH.KEY`, `DEF_INT.ACT`, `DEF_INT.OUT`;
  then `NEWMESH.KEY` not readable → return 0.

Final cleanup in the handler (always, success or not): `FUN_14013e880(mg, 1)` deletes (each only
if it can be opened) — via `FUN_14013e960`: `DEF_AMG.INI`, `DSTPAR2.DAT`, `USRWIN.DAT`,
`DEF_MGN.ACT`, `DEF_MGN.OUT`, `mesh.out`, `NEWMSH.GEO`, `NBCD.OUT`, `NBCD.LST`, `NEWNOD.DAT`,
`NBCD.DAT`; via `FUN_14013ea10`: `DEF_AMG3.INI`, `DSTPAR.DAT`, `BLOCK3.DAT`, `DEF_MGN.ACT`,
`DEF_MGN.OUT`, `MESH3D.INP`, `mesh.out`, `MESH3D.MSG`, `MESH3D.SCR`, `BRKMSH.INI`, `BRKMSH.KEY`,
`BRKMSH.MSG`, `SCRATCH.KEY`, `SCRATCH.DAT`, `NEWMSH.GEO`, `NBCD.OUT`, `NBCD.LST`, `CHK_VOL.INI`,
`CHK_VOL.MSG`, `crossed.faces.0`, `DEF_AMG.DST`, `NEWNOD.DAT`, `NBCD.DAT`; then `DEF_INT.ACT`,
`DEF_REM.INI`, `REM_SCR.SCR`, `NEW.MSH`, `STEP.DAT`, `DEF_FRZ2.OUT`, `DMESH.DAT`, `FRZ1.INI`,
`DEF_FRZI.KEY`, `DEF_FRZI.TMP`, `DEF_REM3.INI`, `SCRATCH.KEY`, `NEWMESH.KEY`, `DEF_INT.OUT`.
(`FUN_1401b85d0` = open-test then `remove`.) So after DEFINT none of the exchange files remain.

## 6. Read-back

2D: `FUN_140156640(mg, "<stem>_REM.KEY")`, then the file is deleted.
3D: `FUN_140157040(mg, "NEWMESH.KEY")`, then the file is deleted.

Both readers (returning 1 whenever the file exists):

* mg+0x3f40 == 2 (DEFFRZ only): `FUN_1401573c0(mg)` then
  `FUN_1401b5f00(model, file, {1}, {0})` — see DEFFRZ.md.
* else, if the source object number equals the target object number (always true for DEFINT,
  mg+0x4780 points at the target object): `FUN_1401b5f00(model, file, {0}, {0xf})` — the KEY
  interpreter reads the file into the live model with **model+0x1740 = 0xf** for the duration:
  bit 1 skips `DBREAD` (DB name not changed), bit 8 keeps PROBID (the `PROBID` keyword reads into a
  scratch buffer), bit 2 skips `MESHNO` (model+0x840), bit 4 skips `NSTART` (model+0x8b0)
  (group-1 cases 7 and 0xe = registry ids 29, 36). Flag `{0}`: no pre/post hooks
  (`FUN_1401d7f10`/`FUN_1401d7f40` not called). So the object's keywords in the file (RZ, ELMCON,
  state variables, BCs…) overwrite the object's data in place.
  3D only: afterwards, if `FLDGEO.KEY` is readable, `FUN_1401b5f00(model, "FLDGEO.KEY", {0}, {0})`
  then delete it.
* else (different source object; not reachable from DEFINT): a temporary model
  (`FUN_1401c8950(new 0x3408, dim 2|3)`) copies the model (`FUN_1401d7390`, `FUN_1401d7320`),
  reads the file into it with `({1},{0})`, the target object is reset (`FUN_140158f70`) and
  re-copied from the temporary model's object (`FUN_1401613a0`), its +0xc (type) and +0x828 (name,
  0x40) restored from mg+0x10; the temporary model is destroyed (`FUN_140158fc0`).

The output KEY's exact contents are produced by DEF_INT / DEF_REM3_GUI (outside DEF_PRE).

`DEF_REM3_GUI.com` (0x401240) runs `DEF_REM_64.EXE` when a file `64bit.DAT` exists in the current
directory ("Running 64bit simulation"), else the 32-bit `DEF_REM.EXE` ("Running 32bit simulation");
DEF_AMG3_GUI, MESH3D_GUI and the other 3D .com wrappers test the same file, which the GUI keeps in
the project directory. The oracle runs start DEF_PRE in a scratch directory without it, so they took
the 32-bit path; on Sim2 that has no 32-bit MPI (`mpich.dll`, it does not run on current Windows 11),
so DEFINT 3D and REMESH 3D from DB are recorded as `Failure`. The port's launcher (`launch.cpp`) runs
children with `SetErrorMode(SEM_FAILCRITICALERRORS|...)`, so the missing DLL opens no dialog.

## 7. Messages and return values

All to stdout (`__iob_func()+0x30`):

| Text | When |
|---|---|
| `INFO   : Interpolating data - Object %d...\n` | object found |
| `         Skip data interpolation\n` | model+0x1748 empty (no DBREAD) — nothing else runs except cleanup |
| `         Data interpolation...%s\n` (DB name) | before running |
| `         Failure\n` / `         Success\n` | routine result |
| `INFO   : Interpolating data - Done\n` | always at the end |

Handler returns 0 if the object is missing (unreachable — dispatcher checks first), else 1.
Dispatcher case returns 0 in all cases except reader failure / missing object (2).
DEF_INT's own output goes to `DEF_INT.OUT`, which is deleted.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401aa000` | DEFINT handler |
| `FUN_14013e790` / `FUN_14013e840` | `_MeshGenBase` ctor / dtor |
| `FUN_140148840` | mg init: target object, model, PROBID, object copy |
| `FUN_1401575a0` | set source objects, DB name (`FUN_140157610`), step |
| `FUN_140155c90` | 2D interpolation driver (DEF_INT) |
| `FUN_1401562f0` | write `DEF_REM.INI` |
| `FUN_1401563c0` | write 2D mesh file (`.RMS` / `DMESH.DAT`) |
| `FUN_140156640` | 2D read-back of the result KEY |
| `FUN_140156950` | 3D interpolation driver (DEF_REM3_GUI) |
| `FUN_140156ba0` | write `DEF_REM3.INI` |
| `FUN_140156cd0` | write 3D mesh as KEY (`SCRATCH.KEY` / `DMESH.DAT`) |
| `FUN_140157040` | 3D read-back (`NEWMESH.KEY`, `FLDGEO.KEY`) |
| `FUN_14013e880` | delete all mesher/interpolation exchange files |
| `FUN_1401b0cc0` | case-insensitive "ends with" |
| `FUN_1400b9130` | create file with given text (`DEFORM_UIRUNREMESH.DAT`) |
| `FUN_1402f4df0` / `FUN_1402f2fe0` | write an element-int keyword block (MTLGRP) |
| `FUN_1402f7720` | registry id → keyword name |
| `FUN_1401b5f00` | KEY file reader (sets model+0x1740 = mode; flag → pre/post hooks) |
| `FUN_1401808d0` / `FUN_140183cb0` / `FUN_140183d00` | node coords / connectivity / element material group arrays |
| `FUN_140224100` / `FUN_140224130` | launcher, 2D / 3D folder |

## 9. Open questions

* **`"%10d %\n"`** (0x14051e500), settled (p8-08): DEF_PRE's `fprintf` is the VS2008 static CRT's
  `_output_l` (14048ccb4), which uses the non-secure state table `__lookuptable` (14056fb30,
  referenced at 14048ce9d; only `_output_s_l` / `_output_p_l` use the table with invalid states,
  14056fbb0). `'%'` followed by `'\n'` is state PERCENT with class OTHER -> NORMAL, so the `'\n'`
  is written and the `'%'` is dropped; no argument is consumed and nothing fails. The line is
  therefore `"%10d "` of nelem followed by a newline (the oracle's 2D DEFINT run reports Success).
* 3D (`DEF_REM3_GUI`) cannot run on the development PC (its 32-bit `mpich.dll` is missing):
  `NEWMESH.KEY` never appears and the original prints `Failure` (oracle case
  `DEFINT obj1 3D (DEF_REM3_GUI)`); the port launches the same command line and fails the same
  way. Verify the 3D read-back only on a PC where DEF_REM3_GUI runs.
* What DEF_INT/DEF_REM3_GUI put in the result KEY (which keywords) — check in the oracle output by
  keeping the file (it is deleted; a run with a wrapper that copies it would be needed).
* `DEF_REM3.INI` line 6 values (2, 102) are mode codes for DEF_REM3_GUI; meaning not derived here.
* Whether DEF_INT reads `DEFORM_UIRUNREMESH.DAT` (inferred: tells it it was launched by the UI, 2D).
