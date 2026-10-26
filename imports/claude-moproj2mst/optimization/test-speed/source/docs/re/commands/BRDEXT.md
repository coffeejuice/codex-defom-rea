# BRDEXT — extract an object's border from the DB as geometry (`AMG_DXT` / `AMG_DXT3`)

Decompiles: `deform-rea-work/out/p89/brdext.c` (handler, depth 2), `out/p89/b/brd2.c`
(`FUN_14013e230`, `FUN_14013d230`, `FUN_14013cc10`, GEO readers), `out/p89/b/l_*.txt`
(instruction listings used to fix argument order of `FUN_14013dba0`/`FUN_14013d230`).
Shared mechanics: [common.md](common.md).

## 1. Summary

| | |
|---|---|
| Registry id | 292 (`BRDEXT`) |
| Dispatcher case | `0x11f+5` in `FUN_140378e00` |
| Handler | `FUN_1401a9ac0(model, &objId)` → `FUN_14013eb80(mg)` → `FUN_14013dba0` → `FUN_14013e230` |
| 2D object (obj+0x39c0 == 2) | external **`AMG_DXT`** (2D folder) |
| otherwise | external **`AMG_DXT3`** (3D folder) |

**Not in-process**: it launches the external border-extraction program, which reads the problem's
database (step = model+0x744) and writes `<problem>.GEO` (+ `.RES`, `NBCD.DAT`, inferred). DEF_PRE
reads the GEO back and replaces the object's **geometry** (2D boundary curves / 3D surface) with
it. The mesh is not touched. The 2D/3D choice is per object (obj+0x39c0), not per model.

## 2. Argument grammar

```
BRDEXT <objId>
```

`FUN_140413de0(f, &objId, 0)` (rest of the keyword line, `%i`); reader failure → 2; object not
found → 2; otherwise the handler runs, its result is ignored, the case returns 0.

KEY example (`PROBID` set so the DB lookup is predictable; `PROBID` reads the line after the keyword):

```
DBREAD 0
HAMMER_SI.DB
PROBID
HAMMER_SI
BRDEXT 1
KFWRIT
OUT.KEY
```

## 3. Model / object fields used

| Field | Use |
|---|---|
| model+0x340 PROBID | via mg+0x3f60: prefix for the DB search, INI line 1, `<name>.GEO/.RES` |
| model+0x1748 DB name (DBREAD) | fallback DB file name for the existence check |
| model+0x744 step | must be != 0; INI line 2; DB selection |
| obj+8 | object number (INI line 3) |
| obj+0xc | object type; 0 → "Invalid object type" |
| obj+0x39c0 | object dimension: 2 → 2D program, 3 → 3D + DB version check (`FUN_140158d00` ==2, `FUN_140158ca0` ==3) |
| obj+0x3f21 (byte) | set to 1 on success, 0 on failure (`FUN_140158e20`) (inferred: "border extracted" flag) |
| obj geometry | replaced (§6) |
| globals `DAT_1406e2d78` / `DAT_1406e2d7c` | set to 0 / 0xd at start; `DAT_1406e2d78` = 1 on success, 0 on failure |

`FUN_14013eb80` uses a 1-byte local object (`local_117`, initialised 0 by `FUN_140158e40`); in
`FUN_14013e230` `FUN_140158d40(this)` reads it → 0 → `USRWIN.DAT` is deleted.

## 4. Sequence and files

`FUN_14013dba0(this, prob = PROBID, db = model+0x1748, &step = model+0x744, obj)`:

1. **Cleanup** `FUN_14013d900(this, PROBID, &step)`: if PROBID is non-empty, run the DB lookup
   (step 4) on a copy, then `remove("<name>.GEO")`, `remove("<name>.RES")`, `remove("NBCD.DAT")`
   (name = looked-up problem name, or PROBID if the lookup failed).
2. `obj == NULL` → `ERROR  : Border extraction failed.` (unreachable).
   `obj+0xc == 0` → `ERROR  : Border extraction failed. Invalid object type`.
   `step == 0` → `ERROR  : Border extraction failed. Invalid step number`. Each → failure.
3. `cwd = _getcwd(…, 0x104)` (or `"."` on failure).
4. **DB lookup** `FUN_14013d230(&step, prob, cwd, &db, &prob)` (return value ignored):
   * List `cwd + "\" + "*.DB"` with `FindFirstFileA`/`FindNextFileA` (directories skipped).
   * `FUN_14013cc10`: list again (`cwd\` + `\` + `*.DB`); for every file take the stem
     (basename without the last extension, `FUN_1401b1a30`); keep it if it **starts with PROBID**
     (case-sensitive `strncmp`) **and is not equal** to PROBID; record the suffix
     (stem minus the PROBID prefix). `qsort` the suffixes with `FUN_14013ca60` (§4a). Then walk
     the whole array (one slot per listed file; slots past the matches are `""`) and return the
     first slot whose text is non-empty and `abs(%d value) >= abs(step)`, else -1.
   * Found: `prob = prob + suffix`, `db = cwd + "\" + prob + ".DB"`. Not found: both unchanged
     (db stays the DBREAD name, relative to cwd).
   So a DB named exactly `<PROBID>.DB` is never selected by the search; it is used only through
   the fallback (DBREAD name).
5. `FUN_1401b17f0(db)`: `fopen_s(db, "r")` fails → `ERROR  : Border extraction failed.` +
   `Database [%s] does not exist.` (db) → failure.
6. 3D object only: open the DB with the DB reader (`FUN_1401dbfe0` ctor, `FUN_1401dc3f0(db, 0)`,
   `FUN_1401dd0d0`); if the DB version field (`local_103c` in the decompile = the info object's
   +0xabc, `Cls_1401dbfe0::uabc`, set by `FUN_1401dc3f0`; `FUN_1401dd0d0` re-reads the header and
   its result is ignored) < 0x8fd (2301) → the "V2.3 or earlier" error (verbatim in §7) →
   failure; close (`FUN_1401dc1c0`).

### 4a. The suffix comparator `FUN_14013ca60` (task T8-4, from the listing)

`qsort(arr, nmatch, 0x28, FUN_14013ca60)` passes pointers to the 0x28-byte `std::string` slots of
the `new[]` array built in `FUN_14013d230`. The listing (14013ca8f / 14013caa2) passes each
pointer **unchanged** to `FUN_1400066a0`, the `std::string(const char*)` constructor, so the key
text is the C string at the slot's own address, not the slot's text. The VS2010 string layout is
+0x00 the allocator (8 bytes, written by no constructor or assignment: `FUN_140005040` writes
+0x08/+0x18/+0x20 only, the swap `FUN_1401b5d30` moves +0x08..+0x27), +0x08 the 16-byte buffer
or heap pointer, +0x18 size, +0x20 capacity. The 8 bytes come from the heap block of `new[]`
(element 0 sits at block+8 after the count). Each side: `v = abs(FUN_1401b0870(copy))`
(`sscanf "%d"` into a 0-initialised int, `FUN_140484ab8` = `abs`); result -1 if `va < vb`, 0 if
equal, 1 otherwise. qsort is the CRT's (`140484bd0`), which swaps the 0x28 bytes of two slots.

Consequence: in practice every pair compares equal (the bytes rarely spell a number), and the
order of two or more matching suffixes is the permutation MSVC's qsort applies to equal
elements; for up to 8 matches that is shortsort: each pass swaps the first slot with the last
(`[1 2 3 X]` → `[X 2 3 1]` → `[3 2 X 1]` → `[2 3 X 1]`; with PROBID `P` and step 1 the
search then picks `P2`, not `P1`). The port zeroes the 8 bytes
(key `""`, value 0) and runs the CRT qsort port (`def::rt::crt_qsort`); bytes that happen to
read as digits, and the read into +0x08 when the 8 bytes contain no NUL, are not reproducible.
Tier-1 test: `port/defcmd/tests/test_p8_05_brdext_lookup.cpp`.
7. `FUN_14013e230(this, prob, &step, obj)` (returns 0 = success, 1 = failure):
   1. `remove DEF_BRD.OUT` (open-test + remove); 3D object: also `CHK_VOL.INI`.
   2. 2D: delete then create **`AMG_DXT.INI`** ("wt"); else **`AMG_DXT3.INI`** ("wt"). Open
      failure → return 1. Contents:
      ```
      "%s\n"   prob (looked-up problem name)
      "%d\n"   step (model+0x744)
      "%d\n"   object number (obj+8)
      ```
   3. Launch (§5).
   4. `remove DEF_BRD.OUT`. `geo = "<prob>.GEO"`, `res = "<prob>.RES"` (`sprintf "%s.GEO"`,
      `"%s.RES"`).
   5. `geo` not readable → delete `res`, `NBCD.DAT`; return 1.
   6. Read the GEO (§6). Failure → delete `geo`, `res`, `NBCD.DAT`; return 1.
   7. 3D: apply it to the object (`FUN_14024bac0(geo, obj, 3)`); result 0 → return 1.
   8. Delete `USRWIN.DAT`; delete `AMG_DXT.INI` / `AMG_DXT3.INI`. Return 0.
      On success `<prob>.GEO`, `<prob>.RES`, `NBCD.DAT` are **left in the directory**.
8. Return 0 → `         Success\n`, true. Non-zero code → message (§7), false.

Back in `FUN_14013eb80`: success → `DAT_1406e2d78 = 1`, obj+0x3f21 = 1. Failure →
`DAT_1406e2d78 = 0`, obj+0x3f21 = 0 and the cleanup of step 1 again (deletes `.GEO`, `.RES`,
`NBCD.DAT`).

## 5. Launch

| | 2D object | other |
|---|---|---|
| format | `sprintf("AMG_DXT > %s", "DEF_BRD.OUT")` | `sprintf("AMG_DXT3 > %s", "DEF_BRD.OUT")` |
| command line | `AMG_DXT > DEF_BRD.OUT` | `AMG_DXT3 > DEF_BRD.OUT` |
| wrapper | `FUN_140224100(1, …)` dirKind 2 (`2D\`) | `FUN_140224130(1, …)` dirKind 3 (`3D\`) |

`system()` via `cmd`, waits, exit status ignored; success = `<prob>.GEO` exists and parses. The
program reads `AMG_DXT.INI` / `AMG_DXT3.INI` from the working directory (inferred; no stdin
redirection) and the DB `<prob>.DB` (inferred from the INI content).

## 6. Read-back

`_DEF_AMGGEO` object (`FUN_140246d00`), `FUN_140246f70(geo, file, &flag, kind, obj)`:

* **3D** (kind 3 → `FUN_1402484c0(geo, file)`; precheck `FUN_1402488a0(file)`; `fopen` mode at
  0x140532848):
  ```
  "%d\n"                 nnodes      (must be >= 1 and < double at 0x14051f1a8, else fail)
  nnodes x "%*d %lf %lf %lf\n"   x y z  (fewer than 3 fields → fail)
  "%d\n"                 nfaces      (out of range → fail after the nodes)
  nfaces x "%*d %d %d %d %d\n"   4 node ids (fewer than 4 → fail)
  ```
  Applied by `FUN_14024be40(geo, obj)`: `FUN_140163340(obj, 1)`;
  `FUN_1402009f0(obj, nnodes, nfaces, 0)` (allocate the object's surface geometry);
  container = `FUN_140198620(obj, {0})` (NULL → fail); copy nnodes×3 doubles and nfaces×4 ints;
  `FUN_1401985e0(obj, 1)`. Inferred: the object's 3D geometry (GEO surface, quads) is replaced by
  the extracted border.
* **2D** (kind 4, flag 1 → `FUN_1402478f0(geo, file, &1, 4, obj)`): fixed-width record reader
  (`FUN_140411ad0`: int width 5, real width 10, skip width 12). Repeated header `type, count, id`
  (I5 each); `type == 99` ends the curves. Point records per type: 1 = `I5 x y` (z = 0),
  2 = `I5 x y z`, 3 = `I5` + 6 reals (6th × constant at 0x140522fd0; arc data, inferred),
  4 = `I5 x y z` with z forced 0. With flag 1 and `id >= 0` each curve is closed by repeating its
  first point. Curve start indices go to geo+0x10, ids to geo+0x18, count geo+0xc; geo+8 = curve
  kind. Applied by `FUN_14024bac0(geo, obj, 4)` → `FUN_14024bb30`: the object's 2D geometry
  container (`FUN_140197f40(obj,0)`, created by `FUN_140198270` if absent) is cleared
  (`FUN_14027fa30`), sized to the curve count (`FUN_140198230`), and each curve's points copied;
  ids applied with `FUN_1402567e0`. If type 99 was seen, the rest of the file is read by
  `FUN_140249700(geo, f, obj)` (not decompiled here). This reader is the DEFAMG 2D GEO reader.

## 7. Messages

stdout:

| Text | When |
|---|---|
| `INFO   : Extracting border - Object %d...\n` | start |
| `ERROR  : Border extraction failed.\n` | obj NULL; DB missing (followed by next line) |
| `Database [%s] does not exist.\n` | DB file not openable |
| `ERROR  : Border extraction failed. Invalid object type\n` | obj+0xc == 0 |
| `ERROR  : Border extraction failed. Invalid step number\n` | model+0x744 == 0 |
| `ERROR  : Border extraction does not work with DEFORM-3D V2.3 or earlier databases.\nGenerate a new DEFORM-3D database using the existing data, read in the new database \nand perform border extraction again.\n` | 3D, DB version < 0x8fd |
| `         Success\n` | `FUN_14013e230` returned 0 |
| `ERROR  : %s (Code %d)\n` | `FUN_14013e230` returned non-zero; `%s` = `An error occurred when trying to extract the border for the object.\n\n` for code 1 (the only code it returns); 0x2711 → `Border extraction does not work with DEFORM-3D V2.3 or earlier databases. \nGenerate a new DEFORM-3D database using the existing data, read in \nthe new database and perform border extraction.\n\n`; 0x2712 → `Could not open the current database. Make sure the\ndatabase exists in the current directory.\n\n` (both unreachable) |
| `INFO   : Extracting border - Done\n` | always at the end |

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401a9ac0` | BRDEXT handler (mg setup, messages) |
| `FUN_14013eb80` | border extraction driver; flags; cleanup on failure |
| `FUN_14013dba0` | checks, DB lookup, DB version check, calls the worker |
| `FUN_14013d900` | delete `<name>.GEO`, `<name>.RES`, `NBCD.DAT` |
| `FUN_14013d230` | DB lookup: `*.DB` in cwd matching PROBID + step |
| `FUN_14013cc10` | collect/sort suffixes, pick by step |
| `FUN_14013ca60` | qsort comparator (abs of `%d` value) |
| `FUN_1401b1630` | directory listing (FindFirstFileA) |
| `FUN_1401b1a30` | basename without extension |
| `FUN_1401b0940` | prefix compare (flag 1 = case-sensitive) |
| `FUN_1401b0870` | `sscanf("%d")` of a string |
| `FUN_1401b1770` | `_getcwd` or `"."` |
| `FUN_1401b17f0` | file exists (`fopen_s "r"`) |
| `FUN_14013e230` | write INI, launch AMG_DXT/AMG_DXT3, read GEO |
| `FUN_140246f70` / `FUN_1402484c0` / `FUN_1402478f0` | GEO readers (dispatch / 3D / 2D) |
| `FUN_14024bac0` / `FUN_14024be40` / `FUN_14024bb30` | apply GEO to the object (dispatch / 3D / 2D) |

## 9. Open questions

* Settled (p8-05): `FUN_14013ca60` (§4a) and the version field of step 6 (`Cls_1401dbfe0`
  +0xabc). `FUN_140249700` (2D topology after type 99, which writes the `TPLOGY` records of the
  object) is specified by p8-09 in `GEO_TOPOLOGY.md` / `GFREAD_STL_GEO_NAS.md` §5.4.
* What AMG_DXT/AMG_DXT3 write: the oracle runs leave `<prob>.GEO` and `<prob>.RES` for 2D
  (no `NBCD.DAT`); for 3D also `NBCD.DAT`, `CHK_VOL.INI`, `DRZ.DAT`, `DSTPAR.DAT`, `NODLST.DAT`
  (written by AMG_DXT3, not deleted by DEF_PRE).
