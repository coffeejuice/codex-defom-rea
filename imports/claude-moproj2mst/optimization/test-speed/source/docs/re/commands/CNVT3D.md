# CNVT3D (registry 287): convert 2D objects to a 3D keyword file

Decompiles: `deform-rea-work/out/p89/cnvt3d.c` (depth 2 from `FUN_1401467f0`), plus
`out/p89/d/{init,surf,mesh3d,solid,interp,geo,fx,f8f70,fc0a0,amg2d}.c` (Ghidra copy `ghidra_p89d`).
Shared mechanics (argument readers, launcher `FUN_140223790`): [common.md](common.md).

## 1. Summary

| | |
|---|---|
| Registry id / keyword | 287 `CNVT3D`, group 6 (command) |
| Dispatcher | `FUN_140378e00`, `case 0x11f+0` |
| Handler | `FUN_1401467f0(cvt, model+0x1748, model, ids, n)` on a stack converter object `cvt` (class `_MeshGenBase`, 0x5128 bytes on the dispatcher stack) |
| Converter ctor / init / dtor | `FUN_14013e790(cvt)` (ctor), `FUN_14013e880(cvt, 1)` (deletes scratch files, §4.1), `FUN_14013e840(cvt)` (dtor) |

CNVT3D builds a **new, separate 3D model** (`_DB_STEP_DATA`, `FUN_1401c8950(new, 3)`) from 2D
objects of the current model and **writes it as a KEY file** to the file name stored by `ACTOUT`
(`model+0x140`). The current (2D) model is **not** replaced by the 3D model; the 3D model is
destroyed at the end. However, the 2D source objects in the current model **are modified** on the
mesh paths (§6.3), so a following `KFWRIT`/`GENDB` sees changed 2D objects.

Per object the conversion is controlled by four object keywords that only exist for this
purpose (see `claude-deform-rea-grp2/docs/re/keywords/{CRDSYS,GEO23,GEOSEC,MSHSEC}.md`):

| Keyword (object) | Field | Meaning in CNVT3D (names from the log texts in `FUN_1401467f0`) |
|---|---|---|
| `CRDSYS` (282) | `obj+0x2c38` | `== 2` → flag `c2` ("coordinate system 2"); changes the axis mapping (§6.4). Meaning of the values not named in code. |
| `GEO23` (283) mode | `obj+0x2be8` | 1 = **revolve**, 2 = **extrude**, 0 = none (object skipped) |
| `GEO23` mode 1 a, b, c | `+0x2bf0`, `+0x2bf8`, `+0x2c00` | a = `RevolveAngle` (deg), b = `RevolveStartAngle` (deg), c → `cvt+0x4770` (passed to the generators; meaning not named) |
| `GEO23` mode 2 a, b | `+0x2c10`, `+0x2c18` | a = `ExtrudeLength`, b = `ExtrudeStartLocation` |
| `GEO23` mode 1 `k` | `+0x2c08` | not read by CNVT3D |
| `GEOSEC n [f]` | `+0x2c24` (n, default 20), `+0x2c21` | `NoGeometrySections`; `+0x2c21` not read by CNVT3D |
| `MSHSEC n [f]` | `+0x2c28` (n, default 21), `+0x2c20` | number of mesh sections; `+0x2c20` = `OutputMesh` |
| `MGNELM` 3rd int (2D) | `obj+0x2d94` | 3D mesh kind: 0 → tetrahedral path, 1 → brick (hexahedral sweep) path |
| `MGNELM` 1st int (2D) | `obj+0x2d80` | target 2D element count for the brick path (§6.2) |
| `MGNELM` 2nd int (2D) | `obj+0x2d8c` | passed to the generators (§5) |

(The `MGNELM` field order 0x2d80 / 0x2d8c / 0x2d94 for 2D is from `dbmap/object.md` word 184.)

2D/3D: the command only makes sense on a 2D model (it projects 2D r,z / x,y data to 3D).
Nothing in the handler checks the dimension.

## 2. Argument grammar

```
CNVT3D <n:%i>
<id1> <id2> ... <idn>            (only read when n > 0)
```

* Keyword line: `FUN_140413de0(f, &n, 0)` (`%i`). Reader error → **return 2**.
* `n <= 0`: nothing else is read, nothing is done, return 0.
* Next line: `FUN_140413900(f, &cnt, n)` (tokens split by `" ,\n\t\r"`, at most `n`), reader
  error (EOF / empty line) → **return 2**. `count = min(cnt, n)`; each token is converted with
  `FUN_14047b6c0` (`strtol` base 10, non-numbers → 0) into an `int[count]`.
* Then `FUN_14013e790(cvt)`, `FUN_14013e880(cvt, 1)`, `FUN_1401467f0(cvt, model+0x1748, model,
  ids, count)`, `free(ids)`, `FUN_14013e840(cvt)`. Return **0** whatever the conversion result.

Example (convert objects 1 and 2):

```
ACTOUT 0
CONV3D.KEY
CNVT3D 2
1 2
```

`ACTOUT` (registry 286, `bm_cmd.c` `param_3 == 0x11e`): reads an int on its keyword line
(ignored) and copies the next line, with every `\n` replaced by NUL, to `model+0x140`
(`FUN_140382320`). That string is the output file of CNVT3D. Without `ACTOUT`, `model+0x140`
is empty (inferred: initial value) and the save fails (§6.5).

## 3. Model fields used

Converter object `cvt` (offsets in `cvt`):

| Offset | Set by | Content |
|---|---|---|
| +0x8 | `FUN_140147b70`, `FUN_140148840` | current object pointer (2D source object while generating) |
| +0x10 | `FUN_140148840` | `cvt+0x18` (an embedded object copy of the source, `FUN_1401613a0`) |
| +0x3f48 | handler | source (2D) model |
| +0x3f50 | handler | new 3D model (`operator new(0x3408)`, `FUN_1401c8950(p, 3)`) |
| +0x3f58 | handler | `model+0x3390` (DB object, `FUN_140159010`) |
| +0x3f60 | `FUN_140148840` | copy of `model+0x340` (problem/KEY name, used for `%s.GEO`, `%s_%03d.KEY` and `DEF_AMG3.INI`) |
| +0x4360 | handler | `model+0x1748` (DBREAD path), or `"DEFORM"` when that pointer is NULL or the string empty — the log-file stem (§7) |
| +0x4770 | handler | revolve: `obj+0x2c00`; extrude: 0 |

Source model: `model+0x1380` object list (`FUN_1401d6ea0`), `model+0x750`, `+0x1328`,
`+0x1358`, `+0x13c8` (copied into the new model by `FUN_1401a91d0`: `FUN_1401f36f0`,
`FUN_14029b6c0`, `FUN_140339dc0`, `+0x1320 = FUN_140199c50(new+0x1328)`, `FUN_1401fa150`),
`model+0x744` (step number, `DEF_REM.INI`), `model+0x834` (SimNo, `DEF_CNV23.DAT`),
`model+0x888..` (10 ints, ELMNOD options, `DEF_INT_3DTET.DAT`), `model+0x15f8`.

Source object: `+8` id, `+0xc` type, `+0x828` name (64 chars), `+0x87c` has-mesh,
`+0x880`/`+0x884` node/element counts, `+0x8a0` node container, `+0x9e0` element container,
`+0x1ea0` sub-object, `+0x2090` TRGVOL real, `+0x2160` movement (OMV_DATA), `+0x2be8..+0x2d0f`
GEO23 block, `+0x2d10` mesh-window list, fields of §1.

Objects in the new model are found or created with `FUN_14019ccc0(new, id)` (creates objects up
to `id`, new ones type 7), so the 3D KEY can contain empty type-7 objects for ids that were not
converted.

## 4. Files

### 4.1 Deleted before anything else (`FUN_14013e880(cvt, 1)`)

`FUN_14013e960`: `DEF_AMG.INI`, `DSTPAR2.DAT`, `USRWIN.DAT`, `DEF_MGN.ACT`, `DEF_MGN.OUT`,
`mesh.out`, `NEWMSH.GEO`, `NBCD.OUT`, `NBCD.LST`, `NEWNOD.DAT`, `NBCD.DAT`;
`FUN_14013ea10`: `DEF_AMG3.INI`, `DSTPAR.DAT`, `BLOCK3.DAT`, `DEF_MGN.ACT`, `DEF_MGN.OUT`,
`MESH3D.INP`, `mesh.out`, `MESH3D.MSG`, `MESH3D.SCR`, `BRKMSH.INI`, `BRKMSH.KEY`, `BRKMSH.MSG`,
`SCRATCH.KEY`, `SCRATCH.DAT`, `NEWMSH.GEO`, `NBCD.OUT`, `NBCD.LST`, `CHK_VOL.INI`,
`CHK_VOL.MSG`, `crossed.faces.0`, `DEF_AMG.DST`, `NEWNOD.DAT`, `NBCD.DAT`;
then `DEF_INT.ACT`, `DEF_REM.INI`, `REM_SCR.SCR`, `NEW.MSH`, `STEP.DAT`, `DEF_FRZ2.OUT`,
`DMESH.DAT`, `FRZ1.INI`, `DEF_FRZI.KEY`, `DEF_FRZI.TMP`, `DEF_REM3.INI`, `SCRATCH.KEY`,
`NEWMESH.KEY`, `DEF_INT.OUT` (in this order; `FUN_1401b85d0` = delete).

### 4.2 Status and log files (always)

* `DEF_CNV23.DAT`: deleted at start (`FUN_14013c970(0, 0)`); at the end opened `"w"` and, when
  `model+0x834 > 0`, written as one line `"\tSimNo\t" <SimNo> "\n"` (C++ stream, decimal).
  Otherwise left empty.
* `CONV_STATUS.DAT`: deleted at start (`FUN_14013c320`), then every message is appended (§7).
* `<stem>.LOG`: every message is appended (§7).

### 4.3 Files of the mesh generators

See §5/§6. All names are relative to the working directory.

## 5. Per-object processing and launches

Loop `i = 1..n` (`DAT_1406e2d84 = i`, `DAT_1406e2d88 = n` — used for the `"[%d/%d] "` prefix):

1. `obj = FUN_1401d6ea0(model, id)`; not found → skip. `c2 = (obj+0x2c38 == 2)`.
2. If GEO23 mode (`obj+0x2be8`) `<= 0` → skip (`FUN_140189110`).
3. `kind = 5`. Revolve: `start = +0x2bf8`, `ang = +0x2bf0`, `cvt+0x4770 = +0x2c00`.
   Extrude: `start = +0x2c18`, `ang = +0x2c10` (the length), `cvt+0x4770 = 0`.
4. If the object has a mesh (`+0x87c != 0`):
   * if `+0x2c24 > 0`: if `FUN_140160cc0(obj)` (has geometry, inferred) is false, the boundary
     geometry is first extracted from the mesh (`FUN_1401fc730(obj, &1)`); then
     **geometry generation**: `FUN_140147b70(cvt, id, +0x2c24, +0x2d8c, start, ang, cvt+0x4770, 5)`.
   * if `OutputMesh` (`+0x2c20`) and `+0x2c28 > 0`: `kind = 2` (`+0x2d94 == 0`) or `4`
     (`+0x2d94 == 1`); **mesh generation**: `FUN_140147b70(cvt, id, +0x2c28, +0x2d8c, start, ang,
     cvt+0x4770, kind)`. (Other `+0x2d94` values leave kind 5 and the call runs the geometry
     branch again.)
5. If the object has geometry but no mesh: geometry generation with `+0x2c24` (kind 5).
6. Post-processing of the generated 3D object (§6.4), copy of properties (`FUN_140148750`), axis
   mapping of movement and `+0x1ea0` data.

`FUN_140147b70(cvt, id, nsec, m, start, ang, c, kind)`: `cvt+8 = source object`;
`kd = FUN_1401ee4c0(model+0x750)` overridden by the object's GEO23 mode when it is 1 or 2;
`FUN_140148840` makes `cvt+0x18` a copy of the source object.

| kind | Path | Generators |
|---|---|---|
| 5 | geometry only | "Creating geometry..."; temporary `MeshGenBase`; `FUN_140153710(cvt, kd, start, ang, obj, mg, c)` |
| 2 | tetrahedral mesh | `FUN_140153710` (geometry), `FUN_140153910` (mesh-window z shift for extrude), "Generating surface mesh..." `FUN_14013ee00(mg, 0)`, then if `DAT_1406e2d74 == 1` "Generating solid mesh..." `FUN_140140110(mg, 1)`, "Assigning state variables..." `FUN_140148a20(cvt, mg+8)` |
| 4 | brick mesh (sweep) | if `obj+0x2d80 != obj+0x884`: 2D remesh + interpolation `FUN_140148960`; then revolve `FUN_140148f70` / extrude `FUN_14014c0a0` (in-process sweep of the 2D quad mesh, `nsec` sections) |

### 5.1 Geometry (`FUN_140153710` → `FUN_140153c00` / `FUN_140154270`)

`FUN_140155340` prepares the source; the 3D object is `FUN_14019ccc0(new, id)`. If the object
has `OutputMesh == 0` or no mesh: `FUN_140160cc0(obj)` false → `printf("2D DB is not correct!")`
and return (no newline); else `FUN_140153c00` (revolve/extrude the boundary geometry);
with a mesh and `OutputMesh`: `FUN_140154270`. Mesh windows are copied (`FUN_140189b70`) when
the model has a previous-step model (`FUN_1401d8120`). Revolve: `FUN_140198d30(obj3D, start, ang)`.
Both generators call the primitive builder `FUN_1402ba500`, which can run **FIXGEO** to clean
the swept profile: `FUN_1402ce190` → `FUN_1402d7cd0` → `FUN_1402da250` → `FUN_14027ece0`
(only when a profile point had to be moved, `local_1c8` in `FUN_1402da250`). Files and command
line of that FIXGEO call: [DXF_IGES_READERS.md](DXF_IGES_READERS.md) §5.4
(`FIXGEO.EXE > DEF_GEO.TMP`, dirKind 2).

### 5.2 Tetrahedral path (kind 2) — external programs

Shared with REMESH/DEFAMG 3D (`_MeshGenBase`); file formats of `%s.GEO`, `NBCD.DAT`,
`BLOCK3.DAT`, `MESH3D.INP` are written by `FUN_140143920`, `FUN_140142240`, `FUN_14013f8f0`,
`FUN_1401427e0` (see the DEFAMG/REMESH specs). Sequence:

1. Surface mesh `FUN_14013ee00` → `FUN_14013f410(mg, 0, 0)`:
   * returns 0x14 when the object has no nodes and no geometry;
   * `DEF_AMG3.INI` ("wt"): `"%s\n"` problem name (`cvt+0x3f60`), `"%6d\n"` object id;
   * `%s.GEO` (`FUN_140143920`), `NBCD.DAT` (`FUN_140142240`, when `DAT_1406e2d78 == 0` and
     `FUN_1402062b0(obj)`), `DSTPAR.DAT`:
     `"%lf %lf %lf\n"` (+0x2d28, +0x2d30, +0x2d38), `"%lf %lf %d\n"` (+0x2d48, +0x2d20,
     +0x2d88 or 8000 when 0), `"%d %d\n"` 0 0, `"%d %lf\n"` (+0x2da0, +0x2d98) (all `obj+`);
     `BLOCK3.DAT` (`FUN_14013f8f0`);
   * `DEF_MGN.ACT`: `"5\nN\n7\ne\nY\n"`;
   * delete `DEF_MGN.OUT`, `MESH3D.INP`, `FIXGEO.DAT`; create `DEFORM_UIRUNREMESH.DAT`
     (`FUN_1400b9130(name, "3D")`);
   * launch **`DEF_AMG3_GUI < DEF_MGN.ACT > DEF_MGN.OUT`** (`FUN_140224130`, dirKind 3);
   * delete `DEFORM_UIRUNREMESH.DAT`, `%s.GEO`, `%s.DST`, `DEF_AMG3.INI`, `DEF_MGN.ACT`,
     `DSTPAR.DAT`, `BLOCK3.DAT`, `DEF_AMG.DST`.
   Success (`vtbl+0x18`) sets `DAT_1406e2d74 = 1`; then `FUN_14013efb0` deletes `DEF_MGN.ACT`,
   `DEF_MGN.OUT`, `NEWNOD.DAT`, `NEWMSH.GEO`, `NBCD.LST`, `mesh.out`.
2. Solid mesh `FUN_140140110(mg, 1)`: `CHK_VOL.INI` update (if it exists, TRGVOL int != -1 and
   TRGVOL real > 0: read `"%lf\n"`, rewrite `"%g\n"` TRGVOL when they differ by > 1e-6); delete
   `crossed.faces.0`; `FUN_140141d50`:
   * `FUN_140142680` (deletes `MESH3D.SCR`, `MESH3D.INP`, `MESH3D.MSG`, `BRKMSH.INI`,
     `SCRATCH.KEY`, `SCRATCH.DAT`, `DEF_MGN.OUT`, `BRKMSH.INI`, `%s_%03d.KEY`, `%s.GEO`);
   * `DEF_MGN.ACT` = `"5\nN\n6\ne\nY\n"`; `MESH3D.SCR` = `"0\n" "MESH3D.INP\n" "0\n"`;
     `MESH3D.INP` by `FUN_1401427e0`;
   * `DEFORM_UIRUNREMESH.DAT` created, launch **`MESH3D_GUI`** (dirKind 3), file deleted;
   * if `mesh.out` does **not** exist: `FUN_14013f410(mg, 1, 0)` again; `SCRATCH.KEY` missing →
     7; else read it (`FUN_140142f10`), delete `SCRATCH.KEY`, `MESH3D.MSG`; ok → 0 else 0xc;
   * if `mesh.out` exists: delete `MESH3D.MSG`; `BRKMSH.INI` = `"%10d %10d\n"` (4, object id),
     `"%s\n"` `BRKMSH.KEY`; launch **`BRKMSH > BRKMSH.MSG`** (dirKind 3); delete `BRKMSH.KEY`,
     `BRKMSH.MSG`; `SCRATCH.KEY` missing → 8; `CHK_VOL.INI` `"%lf"` > 0 → launch
     **`CHKVOL > CHK_VOL.MSG`** (dirKind 3), delete `CHK_VOL.MSG`; `SCRATCH.KEY` missing → 9;
     else read it (`FUN_140142f10`) → 0 or 0xc.
   On success: delete `crossed.faces.0`, `MESH3D.MSG`; `FUN_140140310`; **`FUN_140141580`
   (reads `NBCD.OUT`, §6.1)** when `DAT_1406e2d78 == 0`; `FUN_1401443c0`, `FUN_1401444f0`.
3. State variables `FUN_140148a20` (§6.3): launches **`DEF_INT < DEF_INT.ACT > DEF_INT.OUT`**
   (`FUN_140224100`, dirKind 2).

### 5.3 Brick path (kind 4)

* If the 2D element count differs from `MGNELM` 1st int: `FUN_140148960`: boundary extraction,
  `obj+0x2d80 = (int)FUN_140159030(nsec-1, m % (nsec-1))`, 2D remesh `FUN_140144a70(cvt, &0)`
  (2D AMG: `DEF_AMG.INI`, `DSTPAR2.DAT`, `DEF_MGN.ACT`, launches **`DEF_AMG < DEF_MGN.ACT >
  DEF_MGN.OUT`** dirKind 2, or `DEF_AMG3_GUI < ...` dirKind 3 depending on the object, see the
  DEFAMG spec; optional `COATINGMESH.EXE` with `AddCoating.INI`), then `DEF_INT_3DTET.DAT` and the
  DEF_INT interpolation of §6.3, then `DEF_INT_3DTET.DAT` deleted.
* Revolve `FUN_140148f70` / extrude `FUN_14014c0a0`: in-process sweep (no launch). The revolve
  sweep deletes `NBCD.OUT` and, **when the angle is < 360**, writes `NBCD.OUT` ("w", C++ stream,
  left-aligned scientific, precision 8) for the two cut faces:
  * line 1: `"\t" N "\n"`, `N = 2*nnode2D - (number of nodes on the axis)`;
  * per 2D node p (1-based) on the axis (x == 0 and y == 0 of its swept position):
    `"\t" x "\t" y "\t" z "\t0\t602\t601\t0\t" p "\n"`;
  * other nodes: `"\t" x "\t" y "\t" z "\t0\t0\t601\t0\t" p "\n"` (start face) and
    `"\t" x "\t" y "\t" z "\t0\t0\t602\t0\t" (nnode2D*nsec + p) "\n"` (end face).
  Then `FUN_14014c730` builds the 3D mesh.

## 6. Read-back

### 6.1 `NBCD.OUT` → node BCs (`FUN_140141580`, kinds 2 and 4)

Opened `"rt"`; missing → return -1, nothing changed. Line 1 `"%d\n"` = count. Each line:
`"%lf %lf %lf %d %d %d %d %d\n"` (x, y, z, c1, c2, c3, …). For each line the nearest node of the
3D object within `FUN_140202a20(obj)²` (squared distance) gets, per direction j = 0..2: if
`cj / 100 == 6` → BC code `obj+0x990[3*node+j] = 6`, `obj+0x998[...] = cj - 600`
(symmetry plane 1/2); else code `= cj`. Then faces whose three nodes share no code 6/7/8 with
the same sub-id get `obj+0xb00[face] = obj+0xb18[face] = 2`; if the object type `+0xc` is 0
all node codes are cleared again; if it is non-zero `FUN_140257c80(obj+0x2e80, …)` rebuilds the
symmetry-surface list.

### 6.2 Generated meshes

Tetrahedral: the mesher's `SCRATCH.KEY` is read into the temporary object by `FUN_140142f10`
(KEY reader) and moved into the 3D object (`FUN_140143900`). Brick: built in memory.
Kind 2 with extrude also calls `FUN_14021d0b0(obj3D, (0,0,start), …)` (translation).
After meshing (kinds 2, 4, if the 3D object has a mesh and `FUN_1402062b0`): the boundary is
rebuilt (`FUN_1401fc290`, plus `FUN_1401fc730` for kind 4), revolve < 360 for kind 4:
`FUN_140198d30(obj3D, start, ang)`; `FUN_140141580` (§6.1).

### 6.3 State-variable transfer (`FUN_140148a20`, kind 2; `FUN_140148960`, kind 4)

`FUN_140148a20(cvt, tet)`: the tetrahedral nodes are projected to 2D — revolve
`(sqrt(x²+y²), z)`, extrude `(x, y)` — and the **2D source object's mesh is replaced** by this
point set with the tetrahedra as 4-node elements (`FUN_140158f70`, `FUN_140202700(obj,
nn, ne, 0, model+0x15f8, 2)`). Then `FUN_14013d7e0(10, model+0x888)` writes
`DEF_INT_3DTET.DAT` ("w"): 10 lines `"%1d\n"` (ELMNOD options), and `FUN_140146050`:

* `cvt+0x3f40 = 1`; `NEW.MSH` ("wt", `FUN_1401563c0`): `"%10d %10d\n"` (nnode, 1), per node
  `"%10d %15.7f %15.7f\n"`, then `"%10d %\n"` (nelem; the format has a stray `%`), per element
  `"%10d "` id and 4× `"%10d "` and `"\n"`;
* `DEF_INT.ACT` ("wt"): `"\n5\nY\n1\nNEW.MSH\n\n6\n\n"`;
* `DEF_REM.INI` ("wt"): `"%s\n"` stem (`cvt+0x4360`), `"%d\n"` `model+0x744`, `"%d\n"` 2,
  `"%d\n"` object id;
* DB name `"%s.DB"` and KEY name `<stem>_REM.KEY` derived from the stem (text before the last
  `.`; a `/`-suffix is cut); `STEP.DAT` / `DEF_INT_NO_DB.DAT` by `FUN_1402ac8a0` (see DEFINT.md);
* launch `DEF_INT < DEF_INT.ACT > DEF_INT.OUT` (dirKind 2); delete `NEW.MSH`,
  `DEF_INT_NO_DB.DAT`; `<stem>_REM.KEY` missing → "Interpolate failed ...", else read it into a
  temporary 2D model (`FUN_1401b5f00`) and copy the object into `cvt+8` (`FUN_1401613a0`), delete
  it. `DEF_INT_3DTET.DAT` deleted afterwards (`FUN_14013d7c0`).
* `FUN_140151400(cvt, tet)` copies the interpolated values to the 3D object.

The 2D source object keeps the projected mesh after CNVT3D (no restore seen).

### 6.4 Object post-processing (all kinds)

* Wpaxis list `obj3D+0x20a0`, entry index 1 (`FUN_140259bc0(list, 1)`): revolve: `+4 = 0`,
  `+0xb0 = 1` when `ang == 360.0`, else `+0xb0 = 0`, `+0xb8 = ang`, `+0xc0 = start`;
  extrude: `+4 = 1`, vectors A = (0,0,start), B = (0,0,start+length).
* A type-4 entry (`FUN_14025a270`, created if absent) gets vectors (1,0,0), (0,1,0) or
  (0,0,1) and (0,0,0): when (`c2` and revolve) or (`!c2` and extrude) the 3D object is first
  rotated with `FUN_14021d2f0(obj3D, (0,0,0), (1,0,0), +90 extrude / -90 revolve)` and the entry
  is (1,0,0),(0,1,0),(0,0,0); otherwise (1,0,0),(0,0,1),(0,0,0). `+0xb8/+0xc0` = ang/start
  (revolve) or 0/0 (extrude).
* `FUN_140148750`: `FUN_14021d4c0` copies name `+0x828`, type `+0xc`, the 0x128-byte GEO23 block,
  movement (`FUN_14021ef80`), `+0x1ea0` data, TRGVOL handling, mesh windows `+0x2d10` (only when
  mesh and `+0x2d94 != 0`); for `+0x2d94 != 0` also `FUN_140147f90` (mesh-window conversion,
  in-process; may call `FUN_1402ba500`).
* Movement axis (`FUN_14021ef80(obj3D+0x2160, obj+0x2160, m)`, m = 2 if `c2` else 3): when the
  2D direction (`+0x50,+0x58`) is not (0,1) it is copied as (x, y, 0); else m=3 → (0,0,y),
  m=2 → (0,y,0). `+0x170/+0x178` → m=3 (x, 0, y), m=2 (x, y, 0).
* `FUN_14021f160(obj3D+0x1ea0, obj+0x1ea0, c2)`: copies +0x18, +0x28; +0x30..+0x40 copied as is
  (`c2`) or with the 2nd/3rd doubles swapped.

### 6.5 Saving

"Saving data..." then `FUN_1401c5810(new, model+0x140, &0)` = the KEY writer used by menu 8
(`FUN_14000ac60`): empty name → 1 (fail). Otherwise (`FUN_1401c6e40(new)` false for a fresh
model, inferred) `new+0x1744 = 0`, `fopen(name, "wt")` (fail → 1), `FUN_1401c5af0` writes the
whole 3D model as KEY. Result → "Conversion succeed"/"Conversion failed". Then the 3D model is
deleted (`FUN_140158fc0(new, 1)`), "Conversion is done.", `"$END$"`, `DEF_CNV23.DAT` (§4.2).
The returned KEY can be loaded later with KFREAD into a 3D session.

## 7. Messages

`FUN_14013c6b0(msg, stem)` ("log"): append `msg "\n"` to `CONV_STATUS.DAT` ("a"); if that open
fails nothing else happens. Then `stem` minus a trailing `".DB"` (`FUN_1401b0cc0`) + `".LOG"`
is opened "a"; if it opens, `msg "\n"` is appended and `printf("%s\n", msg)` goes to stdout.
`FUN_14013c340` (used inside the generators) is the same but prefixes `"[%d/%d] "` (i, n) when
n > 1, also on stdout.

Stem `cvt+0x4360` (e.g. `X.DB` → `X.LOG`, default `DEFORM.LOG`) is used for: "Conversion has been started...", "Saving data...", "Saving data... done",
"Conversion failed", "Conversion succeed", "Conversion is done.", "Creating geometry...",
"Generating surface mesh...", "Generating solid mesh...", "Assigning state variables...",
"Interpolate SV On 2D Quad Mesh ...", "Interpolate failed ...", "Interpolate succeed ...",
"Load interpolated data failed ...", "Load interpolated data succeed ...".

Stem `" "` (a single space, `DAT_14051ddfc` etc.) → log file **`" .LOG"`** is used for:
`"$BGN$"`, `"$END$"`, `"No. of Objects = %d"`, `"Object = %d"`, `"RevolveStartAngle = %f"`,
`"RevolveAngle = %f"`, `"ExtrudeStartLocation = %f"`, `"ExtrudeLength = %f"`,
`"Generate geometry: "`, `"NoGeometrySections = %d"`, `"Generate geometry - done "`,
`"OutputMesh = %d"`, `"Generate mesh: "`, `"Generate mesh - done "`.
Also `printf("2D DB is not correct!")` (§5.1).

Order for one revolved meshed object (OutputMesh on): `$BGN$`, `Conversion has been
started...`, `No. of Objects = 1`, `Object = 1`, `RevolveStartAngle = …`, `RevolveAngle = …`,
`Generate geometry: `, `NoGeometrySections = 20`, generator messages, `Generate geometry - done `,
`OutputMesh = 1`, `Generate mesh: `, …, `Generate mesh - done `, `Saving data...`,
`Saving data... done`, `Conversion succeed|failed`, `Conversion is done.`, `$END$`.

Return code of the keyword: 2 only for argument errors (§2); otherwise 0.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401467f0` | CNVT3D driver |
| `FUN_14013e790` / `FUN_14013e840` | `_MeshGenBase` ctor / dtor |
| `FUN_14013e880`, `FUN_14013e960`, `FUN_14013ea10` | delete scratch files (§4.1) |
| `FUN_14013c6b0`, `FUN_14013c340` | log to `CONV_STATUS.DAT`, `<stem>.LOG`, stdout |
| `FUN_14013c320`, `FUN_14013c970`, `FUN_14013d760`, `FUN_14013d790` | delete status file, `DEF_CNV23.DAT`, `$BGN$`, `$END$` |
| `FUN_1401c8950` | `_DB_STEP_DATA` ctor (dimension 3) |
| `FUN_1401a91d0` | copy simulation/material/inter-object data to the new model |
| `FUN_140147b70` | generate geometry (5) / tet mesh (2) / brick mesh (4) |
| `FUN_140148840` | bind source object, copy into `cvt+0x18` |
| `FUN_140153710`, `FUN_140153c00`, `FUN_140154270`, `FUN_140155340` | geometry revolve/extrude |
| `FUN_140153910` | extrude: shift mesh-window z by ±0.001·|length| |
| `FUN_14013ee00`, `FUN_14013f410` | 3D surface mesh (DEF_AMG3_GUI) |
| `FUN_140140110`, `FUN_140141d50` | 3D solid mesh (MESH3D_GUI / BRKMSH / CHKVOL) |
| `FUN_140141580` | read `NBCD.OUT` → node BC codes |
| `FUN_140148a20`, `FUN_140146050`, `FUN_1401563c0`, `FUN_140146560`, `FUN_140151400` | state-variable transfer through DEF_INT |
| `FUN_140148960`, `FUN_140144a70` | brick path: 2D remesh + DEF_INT |
| `FUN_140148f70`, `FUN_14014c0a0`, `FUN_14014c730`, `FUN_14014fae0` | revolve / extrude sweep |
| `FUN_140148750`, `FUN_14021d4c0`, `FUN_140147f90`, `FUN_14021ef80`, `FUN_14021f160` | copy object data with axis mapping |
| `FUN_14021d2f0`, `FUN_14021d0b0` | rotate / translate a 3D object |
| `FUN_1401c5810`, `FUN_1401c5af0` | KEY writer |
| `FUN_1402ba500` → … → `FUN_14027ece0` | primitive builder, optional FIXGEO |

## 9. Open questions

The brick sweep, 3D mesh numbering, NBCD.OUT, state transfer, the `FUN_140141580` field map and
`FUN_1401c6e40` are now in [CNVT3D_SWEEP.md](CNVT3D_SWEEP.md); its own open-questions section
lists what remains (GEOPRIM internals of the kind 5/2 geometry, the cos(π/2) constant in
`FUN_14021d2f0`, WPAXIS entry 1 of a partial revolve). The items below are kept for the record.

* `FUN_140153c00`/`FUN_140154270`/`FUN_140148f70`/`FUN_14014c730` geometry and sweep algorithms
  are only summarised (3240 + 1641 + 12572 bytes); node/element numbering of the swept mesh,
  the exact role of `obj+0x2d8c` and `GEO23 c` (`cvt+0x4770`) are not derived.
* Exact sscanf field mapping in `FUN_140141580` (the decompiler lost the last 6 pointers): the 4th
  to 6th `%d` are taken as c1..c3 (inferred from the `local_3c - 4` indexing).
* Which offsets `FUN_1401c6e40` tests (17 bytes; assumed `+0x1b5a` flag cleared by the ctor).
* `model+0x1748` after KFREAD (no DBREAD): assumed empty → stem `DEFORM`.
* `FUN_140144a70` (brick 2D remesh) is shared with DEFAMG 2D; its INI formats are not repeated.
* Whether `" .LOG"` (leading space) is really created on Windows; `fopen(" .LOG", "a")` should
  succeed (leading spaces are legal).
