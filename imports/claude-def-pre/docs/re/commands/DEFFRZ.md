# DEFFRZ — force interpolation from a DB step (`DEF_FRZ` / `DEF_FRZ3`)

Decompiles: `deform-rea-work/out/p89/deffrz.c` (handler, depth 2), `out/p89/b/frz2.c`
(`FUN_140157890`, `FUN_140157be0`, `FUN_1401573c0`), `out/p89/defint.c` (shared writers).
Shared mechanics: [common.md](common.md). The interpolation framework (mg object, mesh writers,
read-back, cleanup) is described in [DEFINT.md](DEFINT.md); only the differences are here.

## 1. Summary

| | |
|---|---|
| Registry id | 296 (`DEFFRZ`) |
| Dispatcher case | `0x11f+9` in `FUN_140378e00` |
| Handler | `FUN_1401aa370(model, &objA, &objB, &value)` |
| 2D (model+0x1c == 2) | `FUN_140157890` → external **`DEF_FRZ`** (2D folder) |
| otherwise (3D) | `FUN_140157be0` → external **`DEF_FRZ3`** (3D folder) |

Interpolates nodal force data of object B from the DB named by the last `DBREAD`
(model+0x1748, step model+0x744) onto the current mesh of object A, then reads the result KEY back
into the model. "Force interpolating" in the messages; mg+0x3f40 = 2 ("force" mode).

Call chain: handler → `FUN_140155c70(mg, 2)` (mg+0x3f40 = 2) → `FUN_140148840(mg, objA, model,
model+0x340)` → `FUN_1401575a0(mg, 1, &objBptr, model+0x1748, model+0x744)` →
`FUN_140157670(mg, value)` (mg+0x4788 = the double) → `FUN_1401576a0(mg, 0)`:
mg+0x4bd0 = 0; if mg+0x4780 == NULL print the `_objectInterpolation` error (unreachable);
save model+0x8b0 (NSTART); mode 2 → `FUN_1401577e0` → 2D/3D routine; restore model+0x8b0.
(The handler's own `model+0x1c == 3` test only chooses between two identical stack buffers.)

## 2. Argument grammar

```
DEFFRZ <objA> <objB> <value>
```

* Dispatcher: `FUN_140415f00(f, &a, &b, &d)` — rest of the keyword line, `%i %i %Flf`
  (commas allowed). Missing fields stay 0 / 0.0. Reader failure (EOF / empty line) → return 2.
* Object A missing → 2; object B missing → 2. Otherwise the handler runs; result ignored; case returns 0.
* objA = target (its current mesh is written out and receives the data); objB = source object in
  the DB (`**(mg+0x4780)+8`). `value` is written with `"%f"` into the INI and used nowhere else
  in DEF_PRE (section 9).

KEY example:

```
DBREAD 0
HAMMER_SI.DB
DEFFRZ 1 1 0.0
KFWRIT
OUT.KEY
```

## 3. Model / object fields used

As DEFINT (§3 there), plus:

| Field | Use |
|---|---|
| mg+0x4778 = 1, mg+0x4780 → {objB} | source object list |
| mg+0x4788 | the double argument |
| mg+0x4bd0 = 0 | 3D INI line 2 |
| model+0x8b0 (NSTART) | saved before, restored after the routine |
| model+0x1604 | per-node component count used by `FUN_1401573c0` (inferred: DOF per node) |
| objA+0x990, objA+0x998 (int arrays), `FUN_140180a40(objA+0x8a0)` (double array, node block +0x20) | cleared by `FUN_1401573c0` before read-back (inferred: nodal BC codes / BC values) |
| mg+0x4b98 … mg+0x4bc8 | 7 doubles read from `DEF_FRZI.TMP` / `SCRATCH.DAT`; mg is destroyed afterwards, so they have no effect on the model (section 9) |

## 4. Files written for the external program

### 2D — `FUN_140157890(mg)`

1. **`FRZ1.INI`** ("wt"; failure → return 0):
   ```
   "%s\n"     DB name (mg+0x4790)
   "%d\n"     step (mg+0x4b90 = model+0x744)
   "%d\n"     objA number (mg+8 → +8)
   "%10d "    number of source objects (mg+0x4778 = 1)
   "%10d  "   objB number           (one per source object)
   "\n"
   "%f\n"     value (mg+0x4788)
   ```
2. **`DMESH.DAT`** — `FUN_1401563c0(mg, "DMESH.DAT")` with mg+0x3f40 = 2 (failure → delete
   `FRZ1.INI`, return 0):
   ```
   "%10d\n"                          nnodes
   nnodes x "%10d %15.7f %15.7f\n"   i+1, x, y
   "%10d %5d\n"                      nelem, 4
   nelem x ( "%10d " i+1, 4 x "%10d " node ids, "\n" )
   ```
3. Delete `DEF_FRZ2.OUT`, `DEF_FRZI.TMP`.

### 3D — `FUN_140157be0(mg)`

1. **`FRZ3.INI`** ("wt"; failure → return 0):
   ```
   "%s\n"   PROBID (mg+0x3f60)
   "%d\n"   mg+0x4bd0 != 0 → 1 else 0   (always 0 from DEFFRZ)
   "%s\n"   DB name
   "%d\n"   step
   "%d\n"   objA number
   "%d\n"   objB number        (model not 2D; the 2D variant "%10d " count + "%10d  " ids + "\n" is unreachable here)
   "%f\n"   value
   ```
2. **`DMESH.DAT`** — `FUN_140156cd0(mg, "DMESH.DAT")` with mode 2 (failure → delete `FRZ3.INI`,
   return 0):
   ```
   "%10d\n"                                 nnodes
   nnodes x "%10d %15.7f %15.7f %15.7f\n"   i+1, x, y, z
   "%10d %5d\n"                             nelem, npe (obj+0x894)
   nelem x ( "%10d " i+1, npe x "%10d ", "\n" )
   MTLGRP block (as in DEFINT.md §4, 3D)
   ```
3. Delete `DEF_FRZ3.OUT`, `SCRATCH.KEY`, `SCRATCH.DAT`.

## 5. Launch

| | 2D | 3D |
|---|---|---|
| format | `sprintf("%s > %s", "DEF_FRZ", "DEF_FRZ2.OUT")` | `sprintf("%s > %s", "DEF_FRZ3", "DEF_FRZ3.OUT")` |
| command line | `DEF_FRZ > DEF_FRZ2.OUT` | `DEF_FRZ3 > DEF_FRZ3.OUT` |
| wrapper | `FUN_140224100(1, …)` dirKind 2 | `FUN_140224130(1, …)` dirKind 3 |

No stdin redirection (the program reads `FRZ1.INI` / `FRZ3.INI` itself, inferred). `system()`, waits,
exit code ignored. Afterwards:

* 2D: delete `DEF_FRZ2.OUT`, `DMESH.DAT`, `FRZ1.INI`; `DEF_FRZI.KEY` not readable → return 0.
* 3D: delete `DEF_FRZ3.OUT`, `DMESH.DAT`, `FRZ3.INI`; `SCRATCH.KEY` not readable → return 0.

Then the handler always runs the cleanup `FUN_14013e880(mg, 1)` (list in DEFINT.md §5; it covers
`DEF_FRZ2.OUT`, `DMESH.DAT`, `FRZ1.INI`, `DEF_FRZI.KEY`, `DEF_FRZI.TMP`, `SCRATCH.KEY`, `SCRATCH.DAT`;
note `FRZ3.INI`, `DEF_FRZ3.OUT` are deleted only by the routine itself).

## 6. Read-back

`FUN_140157830(mg, file)` → 2D `FUN_140156640`, else `FUN_140157040`; with mg+0x3f40 = 2 both do:

1. `FUN_1401573c0(mg)`: for i in 0 … nnodes(objA) × model+0x1604 − 1:
   if `objA+0x990[i] == 0` and `fabs(arr[i]) > 1e-6` (DAT_14051f1b8) then `objA+0x998[i] = 0`;
   then `arr[i] = 0.0`, where `arr = FUN_140180a40(objA+0x8a0)`.
2. `FUN_1401b5f00(model, file, {1}, {0})`: KEY read into the live model with mode mask 0 (all
   keywords, including DBREAD/PROBID/NSTART if present in the file) and flag 1, i.e.
   `FUN_1401d7f10` before (clears model+0x2f88/0x2f89) and `FUN_1401d7f40` after (post-read
   fix-ups: `FUN_1401ee510(model+0x750)`, `FUN_1401d6250`, `FUN_1401d63c0(model,0)` on every
   non-type-7 object; if model+0x2f88 was set by the file, NSTART is made negative and copied to
   model+0x744). (FLDGEO.KEY is not read in this mode.)

Then the KEY file is deleted, and:

* 2D: open `DEF_FRZI.TMP` ("rt"; missing → return 0), `fscanf("%lf\n")` → mg+0x4b98,
  `"%lf %lf\n"` → mg+0x4ba0, mg+0x4ba8, `"%lf %lf\n"` → mg+0x4bb0, mg+0x4bb8; close, delete.
  Return = read-back result (1).
* 3D: open `SCRATCH.DAT` ("rt"; missing → return 0), `"%lf\n"` → mg+0x4b98,
  `"%lf %lf %lf\n"` → mg+0x4ba0/4ba8/4bb0, `"%lf %lf %lf\n"` → mg+0x4bb8/4bc0/4bc8; close, delete.
  Return = read-back result.

## 7. Messages

stdout:

| Text | When |
|---|---|
| `INFO   : Force interpolating - Object %d...\n` (objA) | start |
| `         Skip Force interpolation\n` | model+0x1748 empty |
| `         Force interpolating...%s\n` (DB name) | before running |
| `         Failure\n` / `         Success\n` | routine result |
| `INFO   : Interpolating data - Done\n` | end (same text as DEFINT) |
| `Error in _objectInterpolation: need to set the objects to interpolate from.\n` | mg+0x4780 NULL — unreachable |

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401aa370` | DEFFRZ handler |
| `FUN_140155c70` | set mg+0x3f40 (mode) |
| `FUN_140157670` | set mg+0x4788 (double) |
| `FUN_1401576a0` | `_objectInterpolation`: dispatch by mode, save/restore NSTART |
| `FUN_140157780` | mode 0/1: 2D `FUN_140155c90` / 3D `FUN_140156950` (DEF_INT path, used by REMESH) |
| `FUN_1401577e0` | mode 2: 2D `FUN_140157890` / 3D `FUN_140157be0` |
| `FUN_140157890` | 2D force interpolation (DEF_FRZ) |
| `FUN_140157be0` | 3D force interpolation (DEF_FRZ3) |
| `FUN_140157830` | read-back dispatch (2D `FUN_140156640`, 3D `FUN_140157040`) |
| `FUN_1401573c0` | clear nodal force/BC arrays before read-back |
| others | see DEFINT.md §8 |

## 9. Settled (T8-6, packet p8-08) and open points

* **The double argument** (`DEFFRZ a b value`): in DEF_PRE it is stored at mg+0x4788
  (`FUN_140157670`) and used exactly once, as the last line of `FRZ1.INI` / `FRZ3.INI`
  (`"%f
"`, `FUN_140157890` / `FUN_140157be0`). No other DEF_PRE code reads mg+0x4788 (the mg
  object lives on the handler's stack and is destroyed after the cleanup). Its meaning is
  DEF_FRZ's / DEF_FRZ3's; DEF_PRE only passes it through. The port does the same.
* **The 7 read-back numbers** (`DEF_FRZI.TMP` 2D: 5, `SCRATCH.DAT` 3D: 7): read with
  `fscanf("%lf ...")` into mg+0x4b98..mg+0x4bc8 after the KEY read-back and the file is deleted;
  no DEF_PRE code reads those offsets (the handler destroys mg right after `FUN_14013e880`), so
  they have no effect on the model, the messages or any output. The success result does not
  depend on their values (only on the KEY file existing; a missing `.TMP`/`.DAT` gives Failure).
  DEF_FRZ.exe's own format 1015 (`' *** INFO : Total die force:' ... 'Old (Fr,Fz) =' 2E15.5,
  'New (Fr,Fz) =' 2E15.5`) suggests (not derived: DEF_FRZ was only searched for strings) that the
  file carries one value followed by the old and new die force: (Fr,Fz) pairs in 2D,
  (Fx,Fy,Fz) triples in 3D. Port: `MeshGenBase::u4b98[7]`, read and discarded the same way.
* `FUN_1401573c0` array semantics: obj+0x990 / obj+0x998 are the node set's int arrays at
  +0xf0 / +0xf8 (inferred nodal BC code / flag arrays), the double array is the node set slot
  +0x20 (`FUN_140180a40`, state variable key 3); not matched to keyword names.
* What keywords `DEF_FRZI.KEY` / `SCRATCH.KEY` contain (deleted after reading) is DEF_FRZ's.
