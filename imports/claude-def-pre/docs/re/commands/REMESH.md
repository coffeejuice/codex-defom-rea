# REMESH (registry 291): border extraction + new mesh + data interpolation

Addresses are DEF_PRE_64.exe v11.0.2. Decompile `deform-rea-work/out/p89/remesh.c` (depth 2),
helpers in `deform-rea-work/out/p89/a/`. Shared mechanics: [common.md](common.md).

## 1. Summary

`REMESH <obj>` is the sequence of the three remeshing commands on one object, sharing nothing but
the model, the object and a few globals:

```
FUN_1401a9a10(model, &obj):
    obj == NULL -> return 0
    print "INFO   : Remeshing - Object %d...\n"
    FUN_1401a9ac0(model, &obj)   = BRDEXT handler   (border extraction, AMG_DXT/AMG_DXT3)
    FUN_1401a9be0(model, &obj)   = DEFAMG handler   (mesh generation)
    FUN_1401aa000(model, &obj)   = DEFINT handler   (interpolation of state data, DEF_INT/DEF_REM3_GUI)
    print "INFO   : Remeshing - Done\n"
    return 1
```

* Dispatcher case `0x11f+4` of `FUN_140378e00`; handler `FUN_1401a9a10`.
* Results of the three steps are ignored; each step runs even if the previous one failed.
* 2D/3D differences are entirely inside the three steps (`model+0x1c == 3` -> 3D). See
  [BRDEXT.md](BRDEXT.md), [DEFAMG.md](DEFAMG.md), [DEFINT.md](DEFINT.md) for the full
  specifications; this file documents the composition and the hand-over between the steps.

## 2. Argument grammar

```
REMESH <obj>
```

Same as DEFAMG: `FUN_140413de0(f, &obj, 0)` reads `%i` from the rest of the keyword line;
reader failure (EOF / empty rest of line) -> 2; `FUN_1401d6ea0(model, obj)` NULL -> 2; otherwise
the case returns 0 regardless of the handler result.

KEY example:
```
DBREAD 0
PROBLEM.DB
REMESH       1
KFWRIT
OUT.KEY
```
(DBREAD sets `model+0x1748`, the DB path; without it the interpolation step is skipped, §4.3.)

## 3. Model fields and hand-over between the steps

| Field | Set by | Used by |
|---|---|---|
| `model+0x340` problem name `<prob>` | DBREAD (from the DB path) / ctor `"DEFORM"` | all three: file names `<prob>.GEO`, `<prob>.RES`, `<prob>.DST`, `<prob>.RMS`, INI first lines |
| `model+0x1748` DB path | DBREAD | BRDEXT (name passed to AMG_DXT), DEFINT (empty -> skip) |
| `model+0x744` step number | DBREAD | BRDEXT (`FUN_14013dba0(..., model+0x744)`), DEFINT (`FUN_1401575a0` arg) |
| `obj+0x3f21` byte | BRDEXT: 1 if AMG_DXT succeeded, else 0 (`FUN_140158e20`) | DEFAMG start cleanup: when 1, `NBCD.DAT` is **kept** for the mesher |
| global `DAT_1406e2d78` | BRDEXT: 1 if AMG_DXT succeeded, else 0 | DEFAMG 3D: when 1, no `NBCD.DAT` is written and `NBCD.OUT` is not read back |
| object mesh / geometry | BRDEXT (border geometry, see BRDEXT.md), DEFAMG (new mesh) | DEFINT builds its generator from the object **after** DEFAMG, i.e. from the new mesh |
| `model+0x2f89` | DEFAMG (= 1 when the generator ran) | next GENDB: MESHNO + 1 |

## 4. Steps, files and launches

### 4.1 BRDEXT (`FUN_1401a9ac0`)

Prints `"INFO   : Extracting border - Object %d...\n"`, builds a `_MeshGenBase` (`FUN_14013e790`,
`FUN_140148840(G, obj, model, model+0x340)`), runs `FUN_14013eb80(G)`, prints
`"INFO   : Extracting border - Done\n"`. `FUN_14013eb80` -> `FUN_14013dba0` ->
`FUN_14013e230`, which writes `AMG_DXT.INI` (2D) / `AMG_DXT3.INI` (3D) and launches

* 2D: `sprintf("AMG_DXT > %s", "DEF_BRD.OUT")`, `FUN_140224100` (dirKind 2)
* 3D: `sprintf("AMG_DXT3 > %s", "DEF_BRD.OUT")`, `FUN_140224130` (dirKind 3)

and checks `<prob>.GEO` / `<prob>.RES` / `NBCD.DAT`. Failure -> `DAT_1406e2d78 = 0`,
`obj+0x3f21 = 0`, fallback `FUN_14013d900`; success -> both 1. Details: [BRDEXT.md](BRDEXT.md).

### 4.2 DEFAMG (`FUN_1401a9be0`)

Exactly as [DEFAMG.md](DEFAMG.md) §4–6: 2D `DEF_AMG < DEF_MGN.ACT > DEF_MGN.OUT`;
3D `DEF_AMG3_GUI < DEF_MGN.ACT > DEF_MGN.OUT`, `MESH3D_GUI`, `BRKMSH > BRKMSH.MSG`,
`CHKVOL > CHK_VOL.MSG`. Differences caused by the BRDEXT step:

* start cleanup `FUN_14013e880(G, obj+0x3f21 == 0)`: after a successful AMG_DXT run the
  `NBCD.DAT` produced by the border extraction survives and is read by DEF_AMG / DEF_AMG3_GUI
  (inferred from the file name; the meshers were not analysed);
* 3D: `FUN_14013f410` writes its own `NBCD.DAT` only when `DAT_1406e2d78 == 0`, and
  `FUN_140140110` reads `NBCD.OUT` back into the node BCs only when `DAT_1406e2d78 == 0`;
* `<prob>.GEO` written by AMG_DXT is overwritten by DEFAMG's own `<geo>.GEO` (`FUN_140143920`)
  and deleted after DEF_AMG3_GUI (3D) / by `FUN_140145480` (2D).

### 4.3 DEFINT (`FUN_1401aa000`)

Prints `"INFO   : Interpolating data - Object %d...\n"`; builds a new generator from the object
(new mesh), `FUN_1401575a0(G, 1, &obj, model+0x1748, model+0x744)`.

* `strlen(model+0x1748) == 0` -> `"         Skip data interpolation\n"`.
* else `"         Data interpolation...%s\n"` (DB path), then
  * 2D `FUN_140155c90(G, model+0x1748)`: writes `DEF_INT.ACT` =
    `sprintf("\n5\nY\n1\n%s\n\n6\n\n", "<prob>.RMS")`, `DEF_REM.INI` (`FUN_1401562f0`),
    `<prob>.RMS` (`FUN_1401563c0`), `DEFORM_UIRUNREMESH.DAT` (`2D`), launches
    `sprintf("%s < %s > %s", "DEF_INT", "DEF_INT.ACT", "DEF_INT.OUT")` via `FUN_140224100`
    (dirKind 2), and reads back `<db base>_REM.KEY` with `FUN_140156640` (`<db base>`: the DB path after
    its last `/` or `\`, with a trailing `.DB` removed; `_REM.KEY` is appended only when the
    name ended in `.DB`).
  * 3D `FUN_140156950(G)`: `DEF_INT.ACT` = `"\n"`, `DEF_REM3.INI` (`FUN_140156ba0`),
    `SCRATCH.KEY` (`FUN_140156cd0`), launches
    `sprintf("%s < %s > %s", "DEF_REM3_GUI", "DEF_INT.ACT", "DEF_INT.OUT")` via `FUN_140224130`
    (dirKind 3), reads back `NEWMESH.KEY` with `FUN_140157040`.
  * prints `"         Success\n"` or `"         Failure\n"`.
* `FUN_14013e880(G, 1)` (same cleanup list as DEFAMG §4.0, `NBCD.DAT` included), prints
  `"INFO   : Interpolating data - Done\n"`.

File formats and read-back of this step: [DEFINT.md](DEFINT.md).

## 5. Launch summary (in order, a successful 3D run with a DB)

| # | Command line | dirKind | Step |
|---|---|---|---|
| 1 | `AMG_DXT3 > DEF_BRD.OUT` (2D: `AMG_DXT > DEF_BRD.OUT`) | 3 (2) | BRDEXT |
| 2 | `DEF_AMG3_GUI < DEF_MGN.ACT > DEF_MGN.OUT` (2D: `DEF_AMG < DEF_MGN.ACT > DEF_MGN.OUT`) | 3 (2) | DEFAMG |
| 3 | `MESH3D_GUI` | 3 | DEFAMG 3D |
| 4a | `BRKMSH > BRKMSH.MSG` (+ `CHKVOL > CHK_VOL.MSG` if `CHK_VOL.INI` > 0) when `mesh.out` exists | 3 | DEFAMG 3D |
| 4b | `DEF_AMG3_GUI < DEF_MGN.ACT > DEF_MGN.OUT` when `mesh.out` is missing | 3 | DEFAMG 3D |
| 5 | `DEF_REM3_GUI < DEF_INT.ACT > DEF_INT.OUT` (2D: `DEF_INT < DEF_INT.ACT > DEF_INT.OUT`) | 3 (2) | DEFINT |

All mode 1 (`cmd /c`, waits, exit code ignored).

## 6. Read-back

None of its own; see the three step specs: BRDEXT (border geometry), DEFAMG 2D
`<prob>_<obj:%03d>.KEY` through the KEY interpreter / 3D `MESH3D.INP` then `SCRATCH.KEY`
(`RZ`, `ELMCON`) and `NBCD.OUT`, DEFINT `<db base>_REM.KEY` (2D) / `NEWMESH.KEY` (3D).

## 7. Messages (stdout, in order)

```
INFO   : Remeshing - Object %d...
INFO   : Extracting border - Object %d...
INFO   : Extracting border - Done
INFO   : Generating mesh - Object %d...
         Success                                  | INFO   : Generating mesh - Failed
INFO   : Generating mesh - Done                   |
[INFO   : New target volume (%g) has been assigned.]
INFO   : Interpolating data - Object %d...
         Skip data interpolation                  | Data interpolation...%s + Success/Failure
INFO   : Interpolating data - Done
INFO   : Remeshing - Done
```

When DEFAMG's preparation fails (object without geometry and mesh) only its first line is
printed. Messages printed by the KEY interpreter during read-backs come in between. Case
return: 2 for missing/unknown object, else 0.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401a9a10` | REMESH handler |
| `FUN_1401a9ac0` | BRDEXT handler |
| `FUN_1401a9be0` | DEFAMG handler |
| `FUN_1401aa000` | DEFINT handler |
| `FUN_14013eb80`, `FUN_14013dba0`, `FUN_14013e230` | border extraction, AMG_DXT launch |
| `FUN_140158e20` | set `obj+0x3f21` |
| `FUN_140155c90`, `FUN_140156950` | 2D / 3D interpolation drivers |
| `FUN_14013e880` | cleanup shared by DEFAMG and DEFINT |

## 9. Open questions

* Which AMG_DXT outputs DEF_AMG / DEF_AMG3_GUI consume (NBCD.DAT is kept on purpose; whether the
  meshers also read `<prob>.RES`) — needs an oracle run with file listing between the steps.
* REMESH on a model loaded by KFREAD only: `model+0x1748` may be non-empty if a previous DBREAD
  ran in the same session; the interpolation then uses that DB.
