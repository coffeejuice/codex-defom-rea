# DEFAMG (registry 293): generate a new mesh for one object

Addresses are DEF_PRE_64.exe v11.0.2. Decompiles: `deform-rea-work/out/p89/defamg.c` (handler, depth 2)
and `deform-rea-work/out/p89/a/*.c` (`amg2d.c`, `amg3d.c`, `vt.c`, `geo2d.c`, `misc.c`, `m2.c`, `m3.c`,
disassembly `dis1.txt`). Shared mechanics (dispatch, readers, launcher `FUN_140223790`) are in
[common.md](common.md).

## 1. Summary

`DEFAMG <obj>` runs the automatic mesh generator on object `<obj>` and replaces the object's
mesh with the result.

* Dispatcher case `0x11f+6` of `FUN_140378e00`. Handler **`FUN_1401a9be0(model, &obj)`**.
* The handler builds a stack mesh-generator object (`_MeshGenBase`, ctor `FUN_14013e790`, dtor
  `FUN_14013e840`, 0x4be0 bytes), binds it to the object (`FUN_140148840`), and branches on the
  model dimension `model+0x1c` (`FUN_1400015f0`: `== 3`):
  * **2D** (`model+0x1c != 3`): one external run of **DEF_AMG** (2D folder), which writes a KEY
    file `<prob>_<obj:%03d>.KEY` that DEF_PRE reads back with its own KEY interpreter.
  * **3D**: **DEF_AMG3_GUI** (3D folder) builds a surface mesh (`MESH3D.INP`, read back), then
    **MESH3D_GUI** builds the volume mesh; its `mesh.out` is converted to `SCRATCH.KEY` by
    **BRKMSH** (optionally checked by **CHKVOL**), or, when `mesh.out` is missing, a second
    DEF_AMG3_GUI run produces `SCRATCH.KEY`. DEF_PRE parses the `RZ`/`ELMCON` blocks of
    `SCRATCH.KEY` with a private reader.
* The dispatcher ignores the handler's result: the case returns 0 whenever the object id was
  read and the object exists (see §2).
* After any branch: `model+0x2f89 = 1` (the "mesh changed -> MESHNO+1 at GENDB" flag), success is
  judged only by `obj+0x87c != 0` (object has a mesh), and the target volume (TRGVOL) may be
  recomputed (§6.4).

Programs **not** reachable from DEFAMG (checked by xref, `out/p89/a/callers.txt`):
`DEF_COG_AMG` (`FUN_140158160`), `DEF_LOCREM3` (`FUN_140158870/a30/b50`) have no code callers at
all (only `.pdata`/unwind references). `FIXGEO`: DEFAMG only deletes `FIXGEO.DAT` (§4.3);
`FIXGEO.EXE` is launched from geometry tools (`FUN_14027ece0`, `FUN_1402e4340`, ...) not on this
path. `AMG_DXT/AMG_DXT3` belong to BRDEXT (reached by REMESH, see [REMESH.md](REMESH.md)).
`COATINGMESH.EXE` is on the 2D path but gated by a coating-layer count `obj+0x2de8 > 0` whose only
writer (`FUN_140141050`) has no code callers, so it is not reached in batch (inferred, §5.2).

## 2. Argument grammar

```
DEFAMG <obj>
```

`FUN_140413de0(f, &obj, 0)`: rest of the keyword line, `%i` (so `0x1`, `01` are accepted).
* Reader returns 1 (EOF or empty rest of line) -> case returns 2 ("Reading Error").
* `FUN_1401d6ea0(model, obj)` (object list `model+0x1380`) returns NULL -> 2. A non-numeric field
  leaves `obj = 0`, normally not an object -> 2.
* Otherwise `FUN_1401a9be0` is called and the case returns 0, whatever happened.

KEY example:
```
DEFAMG       1
```

## 3. Model / object fields used

Mesh generator `G` (stack object): `G+8` = object, `G+0x10` = `G+0x18` = a copy of the object
(`FUN_14015ef60` + `FUN_1401613a0`, flags 1,1,1,1,0,1; used by DEFINT, not by DEFAMG),
`G+0x3f48` = model, `G+0x3f60` = problem name copied from **`model+0x340`** (the name derived from
the DB path by DBREAD, ctor default `"DEFORM"`), below written `<prob>`. `G+0x4760` byte = object
had a mesh; `G+0x4761`/`G+0x4768` = "uniform node temperature" flag/value (`FUN_140144810`).

Object fields (keyword names from `claude-deform-rea-grp2/docs/re/keywords`):

| Field | Meaning | Use |
|---|---|---|
| `obj+8` | object number | INI files, file names |
| `obj+0xc` | object type (0 rigid, 2 porous, ...) | 3D NBCD.OUT: BCs cleared for type 0; type 2 -> porous density refresh `FUN_1401444f0` |
| `obj+0x87c` | has mesh | gates, success test `FUN_140160bf0` |
| `obj+0x880`, `obj+0x884` | node / element count | |
| `obj+0x8a0` container | node coords (`FUN_1401808d0`), node temperature (`FUN_140180c40`) | |
| `obj+0x9e0` container | connectivity (`FUN_140183cb0`), element group (`FUN_140183d00`) | |
| `obj+0x990/0x998` | node BC code / BC function-or-plane per DOF (3 per node) | NBCD.OUT read-back |
| `obj+0xbf0/0xbf8` | surface polygon count / polygons (4 ints) | 3D surface mesh |
| `obj+0x3998` | geometry list (DIEGEO etc.); entry 0 via `FUN_140197f40(obj,0)` | 2D curves, 3D surface (`FUN_140198620`) |
| `obj+0x3990` byte | object has surface geometry (3D, `FUN_140160d90`) | |
| `obj+0x3994` | surface geometry present count (`FUN_1401985a0`) | 3D gate |
| `obj+0x3948` byte | (`FUN_140158ea0`) when set, a 3D surface is NOT rebuilt from the mesh | |
| `obj+0x3980` | object material group id (`FUN_1401fb270`) | -> global `DAT_1406e2d80` |
| `obj+0x39c0` | object geometry/dimension kind (`FUN_140158ce0`; see RZ.md) | ==2: USRWIN.DAT point format; ==3: TRGVOL recompute variant |
| `obj+0x3f21` byte | set by BRDEXT (`FUN_140158e20`) when AMG_DXT succeeded; ctor 0 | 1 keeps `NBCD.DAT` at cleanup |
| `obj+0x2d10` block | mesh-generation parameters (see below) | |
| `obj+0x2090`, `obj+0x2098` | TRGVOL real / int | §6.4, CHK_VOL.INI |
| `obj+0x2be8`, `+0x2c08` | GEO23 mode / k | `FUN_140155340` (no-op here, §4.1) |
| `obj+0x2de8/0x2df0/0x2df8/0x2e00` | coating layer count / thicknesses / ids / flag | COATINGMESH (2D), in-process coating (3D) |

Mesh-generation block `obj+0x2d10` (`FUN_140189790` defaults):

| Offset | Keyword | Default 2D / 3D | Notes |
|---|---|---|---|
| `0x2d18` | none found | 0 | ==1 selects the alternative 2D `DEF_MGN.ACT` |
| `0x2d20` | MGSIZR | 3.0 / 2.0 | |
| `0x2d28` | MGWTMP | 0 / 0 | normalised, see below |
| `0x2d30` | MGWSTN | 1.0 / 0.25 | normalised |
| `0x2d38` | MGWSTR | 1.0 / 0.25 | normalised |
| `0x2d40` | MGWUSR (1st real) | 0 / 0 | normalised |
| `0x2d48` | MGWCUV | 1.0 / 0.5 | normalised |
| `0x2d50` list | MGUSER windows (`LLWINDEF_DATA`, count `FUN_140189580`) | | |
| `0x2d80` | MGNELM (2D n) | 1000 | |
| `0x2d84` | MGTELM | 4 | |
| `0x2d88` | MGNELM (3D a) | 8000 | 3D: forced to `max(1, value)` (§4.2) |
| `0x2d8c`, `0x2d90`, `0x2d94` | MGNELM b, c, flag | 32000, 1000 / 100, 0 | not used by DEFAMG |
| `0x2d98`, `0x2da0` | MGWUSR 3D 2nd real, int | 1.0, 1 | 0x2d98 rewritten (§4.2) |
| `0x2db0`, `0x2db8` | MGERR | 0.01, 32.0 | |
| `0x2dc0`, `0x2dc4` | MGGRID | 25, 25 | |

(Defaults from `FUN_140189790`, values 2D / 3D; constants `DAT_140522fc0` = 0.25,
`DAT_14051f240` = 0.5, `DAT_14051f258` = 0.01, `DAT_140522fb0` = 32.0. They only matter when
the KEY does not set the keyword.)

**Weight normalisation `FUN_140144200(G, obj+0x2d10)`, written back into the object.**
2D (`FUN_1400015e0`: model dimension 2): no MGUSER windows (count < 1) -> MGWUSR = 0. 3D: if
`|MGWUSR| > 0.001` and the window count is 0, MGWUSR = 0 (corrected from the disassembly at
0x14014422b..0x140144285; an earlier version had the two branches swapped).
`s = MGWCUV + MGWTMP + MGWSTN + MGWSTR + MGWUSR` (this order, 0x140144294); if `|s| <= 1e-10` (2D) /
`<= 1e-8` (3D) then `s = 1`; each of the five is divided by `s`. A following KFWRIT therefore
prints normalised weights.

Globals (persist between commands): `DAT_1406e2d78` = AMG_DXT succeeded (set by BRDEXT, else 0
from program start), `DAT_1406e2d80` = material group id, `DAT_1406e2d74` = mesher state
(0 start, 1 surface mesh read, 2 volume mesh read, 3 coating done, 4 2D running), `DAT_1406e2d7c`
progress code (passed to empty virtuals).

Model fields: `model+0x1c` dim, `model+0x340` problem name, `model+0x84c` UNIT (1 = English),
`model+0x850` GEOTYP, `model+0x1328` material list, `model+0x15f8` component-count table
(`[0]` = coordinates per node), `model+0x1618`, `model+0x16ec` per-node / per-element counts.

`_MeshGenBase` vtable `0x14051f148`: +0x08 `FUN_14013ee00` (3D surface step), +0x10
`FUN_140140110` (3D volume step), +0x18 `FUN_14013f010` (read MESH3D.INP), +0x30 returns 1,
+0x58 returns 0x14, +0x28/+0x40/+0x48/+0x50 empty (progress/UI hooks). No subclass is used here.

## 4. Flow and files written

### 4.0 Common start (`FUN_1401a9be0`)

1. Print `"INFO   : Generating mesh - Object %d...\n"` (stdout).
2. `FUN_14013e790` ctor, `FUN_140148840(G, obj, model, model+0x340)`.
3. Prepare `FUN_140144620(G)`:
   * the object needs geometry (2D: `FUN_140160d20` geometry list count > 0; 3D: `FUN_140160cc0`)
     or a mesh (`obj+0x87c`); neither -> return 0: the handler destroys G and returns **without
     further messages and without setting `model+0x2f89`**.
   * `FUN_140144810`: node temperature uniform? If no mesh: uniform = REFTMP (`obj+0x1eb8`); else
     uniform iff every node temperature is within 0.001 of the first (value = first).
   * `G+0x4760 = obj+0x87c != 0`; `FUN_1401fb270(obj, &n, &DAT_1406e2d80)` (id = `obj+0x3980` if
     >= 1, else from element groups).
   * no geometry but a mesh (3D additionally needs `obj+0x3948 == 0`): `FUN_1401fc730(obj, &1)`
     builds the geometry from the mesh boundary.
4. Cleanup `FUN_14013e880(G, obj+0x3f21 == 0)` deletes (`FUN_1401b85d0`, silently, if present):
   `DEF_AMG.INI DSTPAR2.DAT USRWIN.DAT DEF_MGN.ACT DEF_MGN.OUT mesh.out NEWMSH.GEO NBCD.OUT
   NBCD.LST NEWNOD.DAT` [`NBCD.DAT` if flag], `DEF_AMG3.INI DSTPAR.DAT BLOCK3.DAT DEF_MGN.ACT
   DEF_MGN.OUT MESH3D.INP mesh.out MESH3D.MSG MESH3D.SCR BRKMSH.INI BRKMSH.KEY BRKMSH.MSG
   SCRATCH.KEY SCRATCH.DAT NEWMSH.GEO NBCD.OUT NBCD.LST CHK_VOL.INI CHK_VOL.MSG crossed.faces.0
   DEF_AMG.DST NEWNOD.DAT` [`NBCD.DAT` if flag], `DEF_INT.ACT DEF_REM.INI REM_SCR.SCR NEW.MSH
   STEP.DAT DEF_FRZ2.OUT DMESH.DAT FRZ1.INI DEF_FRZI.KEY DEF_FRZI.TMP DEF_REM3.INI SCRATCH.KEY
   NEWMESH.KEY DEF_INT.OUT` (in this order).
5. If still no geometry: `FUN_1401fc730(obj, &1)` again.

### 4.1 2D: `FUN_140144a70(G, &0)` -> `FUN_140144de0(G, &0)`

`DAT_1406e2d74 = 4`. Checks in `FUN_140144de0` (return code in brackets; any non-zero code ->
`FUN_140145480` cleanup, mesh unchanged):
* `G+8 == NULL` [0x14]; no geometry entry or curve count `FUN_140198310 < 1` [0xe].
* exactly one curve: `FUN_1402789f0(curve)` = signed area of the polygon (arcs subdivided in 4)
  `>= 0`; when negative the curve is reversed (curve vtable +0x28) [0x11 only if vtable +0x30
  returned 0, never here].
* `FUN_140158ec0(geom)`: the geometry has >= 1 region with >= 1 loop. If not: with one curve the
  topology is built (`FUN_140198520(obj, &0)`) and re-checked [0x12]; with several curves
  DEFAMG passes `0` so it returns [0x12] immediately (inferred effect: multi-curve 2D geometry
  without topology is not meshed).

Files, in write order:

1. **`DEF_MGN.ACT`** (`"wt"`), `fprintf("%s", s)`, the mesher's interactive answers:
   `obj+0x2d18 == 1` -> `"\n4\n3\nN\n\n\ne\n5\n6\n\ne\n"`, else `"6\n\n5\n6\n\ne\n"`
   (`DAT_14051dc68` / `DAT_14051dc58`). [0xd if it cannot be opened]
2. **`DEF_AMG.INI`**: `"%s\n"` `<prob>`; `"%6d\n"` `obj+8`. [0xd]
3. weight normalisation (§3).
4. **`USRWIN.DAT`** `FUN_140145ce0(G, obj+0x2d10, obj+0x39c0 == 2)`: first deleted; written only
   if `MGWUSR > 1e-6` and the window count >= 1:
   `"%1f\n"` MGWUSR (sic, `%1f`); per window i = 1..n (`FUN_140269900(list, i)`, skipped if NULL):
   `"%6d %15.7e\n"` npts (`win+0x178`), density (`win+0x370`); then points:
   * `obj+0x39c0 == 2`: npts lines `"%15.7e %15.7e\n"` from `win+0x180` (2 doubles per point);
   * otherwise only windows with shape `win+0x210` in {11, 12, 13}: `FUN_140268590(win, 1)`
     converts to a polygon, then npts lines `"%15.7e %15.7e\n"` x, y (3 doubles per point).
     Other shapes get the header line only.
   [0xd if it cannot be opened; the function returns 0 when nothing is written]
5. `FUN_140155340(G, obj, *(double*)(G+0x4770))`: `G+0x4770` is 0 from the ctor -> no-op.
6. **`<geo>.GEO`** `FUN_140143920(G, 0, 0, 0)`; `<geo>` = `<prob>` with a trailing `".DB"`
   removed. 2D writer `FUN_140248bf0(_DEF_AMGGEO, file, obj, &1, model+0x1c)` ->
   `FUN_14024ae90`: for every region of geometry 0 and every loop of the region:
   `"%d\n"` curve type (type, or type+1 when type is 2 or some loop of type 1 fails
   `FUN_14027f140`); `"%d %d\n"` npts and, for the first loop, the region material
   (`FUN_1402f7fb0`, else the object's group id, else 99999), 0 for the other loops (an inner loop
   with positive area is reversed and gets 0); points: type 1 `"%10d %20.10E %20.10E %20.10E\n"`
   (i, 3 doubles), type 2 `"%10d %10d %20.10E %20.10E %20.10E %20.10E %7.2lf\n"` (i, (int)v0,
   v1..v4, v5 x 57.29577951308232). Return value ignored in 2D.
7. **`DSTPAR2.DAT`** [0xd]:
   `"%6d %6d %15.7e %15.7e\n"` MGGRID `0x2dc0`, `0x2dc4`, MGERR `0x2db0`, `0x2db8`;
   `"%6d %15.7e %6d %15.7e %15.7e %15.7e %15.7e\n"` MGNELM `0x2d80`, MGSIZR `0x2d20`, MGTELM
   `0x2d84`, MGWCUV `0x2d48`, MGWSTR `0x2d38`, MGWTMP `0x2d28`, MGWSTN `0x2d30` (order verified in
   the disassembly at 0x140145272 / 0x140145304).
8. **`DEFORM_UIRUNREMESH.DAT`** `FUN_1400b9130(name, "2D")`: file containing exactly `2D` (no
   newline).

### 4.2 3D: `FUN_1401559d0`, `FUN_14013ee00(G, 0)`, `FUN_140140110(G, 1)`

**Size scaling `FUN_1401559d0(obj, -1.0, 0, &1)`** (modifies the object):
`A` = surface area of geometry 0 (`FUN_140155600`; object surface polygons when there is no
geometry); `n = max(1, obj+0x2d88)` and **`obj+0x2d88 = n`**; `h = sqrt(4*(A/n)/sqrt(3))`
(`FUN_140483f60(3.0, 0.5)`, `FUN_140484644`; CRT names inferred); `r = 2h / (1 + 1/MGSIZR)`;
`r = 1/r` if `|r| > 1e-8` else 1.0. Then if `obj+0x2d98 < 1e-8` it becomes 1.0; every MGUSER
window with `|win+0x370| > 1e-12` gets `win+0x370 = win+0x370 * r / obj+0x2d98`; finally
`obj+0x2d98 = r`.

**Surface step `FUN_14013ee00(G, 0)`**: `DAT_1406e2d74 = 0`, then `FUN_14013f410(G, 0, 0)`:

* no nodes (`obj+0x880 < 1`) and no surface geometry (`obj+0x3994 < 1`) -> return 0x14.
1. **`DEF_AMG3.INI`**: `"%s\n"` `<prob>`; `"%6d\n"` `obj+8`. [3]
2. **`<geo>.GEO`** `FUN_140143920` 3D form, from surface geometry 0 (`FUN_140198620(obj,&0)`):
   `"%10d\n"` nnode; nnode lines `"%10d %15.7f %15.7f %15.7f \n"`; `"%10d\n"` triangle count
   (a polygon with `p[0] == |p[3]|` counts 1, a quad 2); per triangle
   `"%10d %10d %10d %10d %10d\n"`: k, p0, p1, p2, |p3| (triangle) or k, p0, p1, p2, p0 then
   k+1, p0, p2, |p3|, p0 (quad split) [3 when the file or geometry is missing].
3. **`NBCD.DAT`** `FUN_140142240`, only if `DAT_1406e2d78 == 0` and some surface polygon has a
   non-zero BC code (`FUN_1402062b0`, codes `FUN_140158e80(geom)`): per node up to three distinct
   codes of the polygons using it (filled from slot 2 down). If at least one polygon has a code:
   `"%7d %7d %7d\n"` 0, 0, nnode; nnode lines `"%7d %7d %7d %7d\n"` i, c0, c1, c2;
   `"%7d\n"` m (symmetry planes, `FUN_140158e70(FUN_140158e60(geom))`); per plane j:
   `"%7d\n"` 601 + j (`j + 0x259`, j 0-based), `"%15.7g %15.7g %15.7g\n"` plane +8,+0x10,+0x18,
   `"%15.7g %15.7g %15.7g\n"` +0x20,+0x28,+0x30; then `"%7d\n"` 0 twice.
4. **`DSTPAR.DAT`** [4] after the weight normalisation (order verified at 0x14013f5dc..0x14013f718):
   `"%lf %lf %lf\n"` MGWSTN `0x2d30`, MGWSTR `0x2d38`, MGWTMP `0x2d28`;
   `"%lf %lf %d\n"` MGWCUV `0x2d48`, MGSIZR `0x2d20`, `0x2d88` (8000 is substituted while
   printing if 0, but the size scaling has already made it >= 1);
   `"%d %d\n"` 0, 0; `"%d %lf\n"` `0x2da0`, `0x2d98`.
5. **`BLOCK3.DAT`** `FUN_14013f8f0` (first deleted): only when window count > 0 and MGWUSR
   `0x2d40 > 0`: `"%lf\n"` `0x2d40`; `"%lf\n"` `0x2d98`; `"%d\n"` count; per window
   `"%d %lf\n"` shape `win+0x210`, density `win+0x370`, then by shape:
   * 1 or 2: `win+0x178` lines `"%5d %15.7E %15.7E %15.7E\n"` (i, points `win+0x180`); shape 2
     adds `"%15.7E %15.7E\n"` `win+0x1b8`, `win+0x1c0`;
   * 3 (surface window, geometry `win+0x220`): `"%d\n"` nnode, lines
     `"%10d %15.7f %15.7f %15.7f\n"`; `"%d\n"` npoly, lines `"%10d %10d %10d %10d %10d\n"`;
   * 11, 12, 13: axis = WPAXIS item 1 (`FUN_140259bc0(obj+0x20a0, 1)`): `"%d\t"` axis type
     (+4); `"%lf\t%lf\t%lf\n"` (+8,+0x10,+0x18); type 1: unit vector of |(+0x20..+0x30) -
     (+8..+0x18)| per component as `"%lf\t%lf\t%lf\n"` (or `"0\t0\t0\t\n"` for zero length),
     else `"%lf\t%lf\t%lf\n"` (+0x20,+0x28,+0x30); `"%d\n"` npts; npts lines
     `"%15.7f %15.7f %15.7f\n"`.
6. **`DEF_MGN.ACT`**: `"5\nN\n7\ne\nY\n"` (`DAT_14051d520`) [1].
7. delete `DEF_MGN.OUT`, `MESH3D.INP`, `FIXGEO.DAT`; `DEFORM_UIRUNREMESH.DAT` = `3D`.

**Volume step `FUN_140140110(G, 1)`** (runs even when the surface step failed):
* no surface flag (`obj+0x3990 == 0`) but a mesh: `FUN_1401fc730(obj, &1)`.
* `CHK_VOL.INI`: if it exists, `obj+0x2098 != -1` and TRGVOL `obj+0x2090 > 0`: read `"%lf\n"`;
  if `|v - TRGVOL| > 1e-6` rewrite it with `"%g\n"` TRGVOL.
* delete `crossed.faces.0`; then `FUN_140141d50(G)`:
  1. cleanup `FUN_140142680`: `MESH3D.SCR MESH3D.INP MESH3D.MSG BRKMSH.INI SCRATCH.KEY
     SCRATCH.DAT DEF_MGN.OUT BRKMSH.INI <prob>_<obj:%03d>.KEY <prob>.GEO <prob>.DST <prob>.RES`.
  2. **`DEF_MGN.ACT`** `"5\nN\n6\ne\nY\n"` (`DAT_14051d830`) [6].
  3. **`MESH3D.SCR`**: `"0\n"`, `"MESH3D.INP\n"`, `"0\n"` [6].
  4. **`MESH3D.INP`** `FUN_1401427e0` [6]: state 1 (surface mesh was read): object nodes used by
     the surface polygons `obj+0xbf8`, renumbered in order of first use (`obj+0x880 == 0` -> 7);
     state 0: the surface geometry, quads split into (a,b,c,a) and (a,c,d,a) (4th index negated
     when the original was negative). Format: `"%10d\n"` nnode; `"%10d %15.7f %15.7f %15.7f \n"`;
     `"%10d\n"` npoly; `"%10d %10d %10d %10d %10d\n"` (k, 4 node numbers).
  5. `DEFORM_UIRUNREMESH.DAT` = `3D`; launch `MESH3D_GUI`; delete `DEFORM_UIRUNREMESH.DAT`.
  6. `mesh.out` **absent**: `FUN_14013f410(G, 1, 0)` again (same files as the surface step except
     `DEF_MGN.ACT`, which keeps `"5\nN\n6\ne\nY\n"`), then `SCRATCH.KEY` must exist [7].
  7. `mesh.out` **present**: delete `MESH3D.MSG`; **`BRKMSH.INI`** `"%10d %10d\n"` 4, `obj+8`;
     `"%s\n"` `"BRKMSH.KEY"` [6]; launch BRKMSH; delete `BRKMSH.KEY`, `BRKMSH.MSG`;
     `SCRATCH.KEY` must exist [8]; `CHK_VOL.INI` `"%lf"` > 0 -> launch CHKVOL, delete
     `CHK_VOL.MSG`; `SCRATCH.KEY` must exist [9, after `FUN_140142680`].
  8. read `SCRATCH.KEY` (§6.2) [0xc on failure].

## 5. Launch

All through `FUN_140223790` mode 1 (`cmd /c`, waits, exit code ignored; [common.md](common.md)).

| Step | Command line (format, args) | Wrapper / dirKind |
|---|---|---|
| 2D | `sprintf("DEF_AMG < %s > %s", "DEF_MGN.ACT", "DEF_MGN.OUT")` | `FUN_140224100`, 2 |
| 2D, model dim not 2 (unreachable from DEFAMG) | `"DEF_AMG3_GUI < %s > %s"`, same args | `FUN_140224130`, 3 |
| 3D surface | `sprintf("DEF_AMG3_GUI < %s > %s", "DEF_MGN.ACT", "DEF_MGN.OUT")` | `FUN_140224130`, 3 |
| 3D volume | `"MESH3D_GUI"` | `FUN_140224130`, 3 |
| 3D convert | `sprintf("BRKMSH > %s", "BRKMSH.MSG")` | `FUN_140224130`, 3 |
| 3D volume check | `sprintf("CHKVOL > %s", "CHK_VOL.MSG")` | `FUN_140224130`, 3 |
| 2D coating (not reached) | `"COATINGMESH.EXE"` | `FUN_140224100`, 2 |

Success is judged only from files: 2D `<prob>_<obj:%03d>.KEY`; 3D `MESH3D.INP` (surface),
`mesh.out` (which branch), `SCRATCH.KEY`.

Inputs of each program (as far as DEF_PRE prepares them): DEF_AMG: `DEF_MGN.ACT` (stdin),
`DEF_AMG.INI`, `<geo>.GEO`, `DSTPAR2.DAT`, `USRWIN.DAT`, `DEFORM_UIRUNREMESH.DAT`, and
`NBCD.DAT` when kept from BRDEXT. DEF_AMG3_GUI: `DEF_MGN.ACT`, `DEF_AMG3.INI`, `<geo>.GEO`,
`DSTPAR.DAT`, `BLOCK3.DAT`, `NBCD.DAT`, `DEFORM_UIRUNREMESH.DAT`. MESH3D_GUI: `MESH3D.SCR`,
`MESH3D.INP` (inferred: the command line has no arguments). BRKMSH: `BRKMSH.INI`, `mesh.out`
(inferred).

After DEF_AMG (2D) DEF_PRE removes (`remove`): `DEFORM_UIRUNREMESH.DAT DEF_MGN.ACT DEF_AMG.INI
DEF_MGN.OUT USRWIN.DAT DSTPAR2.DAT`. After DEF_AMG3_GUI: `DEFORM_UIRUNREMESH.DAT <prob>.GEO
<prob>.DST DEF_AMG3.INI DEF_MGN.ACT DSTPAR.DAT BLOCK3.DAT DEF_AMG.DST`. (Note: these deletions use
the unstripped `<prob>`; `<geo>.GEO` differs only when `<prob>` ends in `.DB`.)

## 6. Read-back

### 6.1 2D: `FUN_1401455b0(G, &1)`

* `name = sprintf("%s_%03d.KEY", <prob>, obj+8)`; missing -> return 0 (mesh unchanged).
* `FUN_1401b5f00(model, name, &0, &0)`: generic KEY loader: `model+0x2f8a = name`,
  `model+0x1740 = 0`, `FUN_1401b6520(model, f)` (the KEY interpreter: every keyword in the file
  is applied to the model, so the file's RZ/ELMCON/BC keywords replace the object's data;
  contents are produced by DEF_AMG and were not analysed), `model+0x2f8a = " "` (str 0x140525634, one space).
  Error: `"ERROR: Keyword file %s open error!\n"`. Interpreter error -> delete the file, return 0.
* delete the KEY; if `obj+0x884 < 1` or `obj+0x880 < 1`: `FUN_1401ffd80` clears the mesh
  (`obj+0x87c = 0`), return 0.
* `FUN_1402027e0(obj, DAT_1406e2d80, 0, 0)` (store the group id in the object); node temperature
  reset `FUN_1401443c0`: all `nnode * model+0x1618` values = uniform temperature if known, else
  20.0 when `model+0x84c` (UNIT, read as model+0x750+0xfc) `== 1`, else 68.0 (68.0 also without a model; DAT_14051f1f0 = 20.0, DAT_14051f1f8 = 68.0; corrected); `FUN_1401444f0` (porous objects only:
  `obj+0xc == 2`, refreshes the density text via `FUN_14020ae70`/`FUN_14020b0f0`, `"%.12g"`).
* Back in `FUN_140144a70`: if the group id was < 1, the element group array (first `obj+0x884`
  entries) is set to 0. Then `FUN_140145810` (coating, skipped when `obj+0x2de8 <= 0`),
  `DAT_1406e2d74 = 3`, cleanup `FUN_140145480`: `DEF_MGN.ACT DEF_MGN.OUT NEWNOD.DAT NEWMSH.GEO
  NBCD.OUT NBCD.LST mesh.out <prob>_<obj:%03d>.KEY <prob>.GEO <prob>.DST`.
* If the read-back failed, `FUN_140145480` is **not** called (those files may remain).

### 6.2 3D surface: `FUN_14013f010` (vtable +0x18)

Reads `MESH3D.INP` (`"rt"`): `"%d\n"` n (1 <= n < 1e20); n x `"%*d %lf %lf %lf\n"`; `"%d\n"` m
(1 <= m < 1e7); m x `"%*d %d %d %d %d\n"`. On n, m > 0: `FUN_140202700(obj, n, 0, 2,
model+0x15f8, 0)` (nodes only), coordinates copied, `obj+0x87c = 1`, `FUN_140163300(obj, 1)`,
`FUN_140200a70(obj, m, 0)` + copy into `obj+0xbf8` (surface polygons), `FUN_140165d30(obj)`.
Returns 0 only if the file cannot be opened or a count is out of range -> step result 0x14.
Success -> `DAT_1406e2d74 = 1`. Then `FUN_14013efb0` deletes `DEF_MGN.ACT DEF_MGN.OUT
NEWNOD.DAT NEWMSH.GEO NBCD.LST mesh.out`.

### 6.3 3D volume: `FUN_140142f10` reads `SCRATCH.KEY`

Lines are read with `FUN_1401b83d0` (500 chars), trimmed (`FUN_1401b1060`), and searched for a
substring: first line containing `RZ` -> `sscanf("%*s %*d %d\n")` nnode (1..1e20), nnode lines
`"%*d %lf %lf %lf\n"`; next line containing `ELMCON` -> `sscanf("%*s %*d %d %d\n")` nelem,
nodes per element (4 -> `"%*d %d %d %d %d\n"`, 8 -> 8 ints, other -> fail). Then
`FUN_140202700(obj, nnode, nelem, 2 (tet) | 4 (brick), model+0x15f8, 0)`, coordinates and
connectivity copied (`obj+0x894` ints per element), `FUN_1401445f0` (group id to all elements,
`FUN_1402027e0(obj, DAT_1406e2d80, 0, 1)`), `FUN_1401fc290` (rebuild surface polygons from the
mesh). `DAT_1406e2d74 = 2`. Nothing else of SCRATCH.KEY is used.

After a successful `FUN_140141d50` (`FUN_140140110`): delete `crossed.faces.0`, `MESH3D.MSG`;
coating `FUN_140140310` (in-process, `FUN_1401403c0`, only if `obj+0x2de8 > 0`); if
`DAT_1406e2d78 == 0`: **`NBCD.OUT`** read by `FUN_140141580`: `"%d\n"` count, then lines
`"%lf %lf %lf %d %d %d %d %d\n"` (x, y, z, cx, cy, cz, 2 unused ints); for each, the nearest node
within distance^2 <= `FUN_140202a20(obj)`^2 gets per DOF: code c with c/100 == 6 -> BC 6 and
plane c-600, else BC c (`obj+0x990/0x998`); boundary faces whose 3 nodes share no 6/7/8 code
get flags `obj+0xb00/0xb18` = 2; a rigid object (`obj+0xc == 0`) has all node BCs cleared.
Then node temperature reset and porous density refresh as in 2D.

### 6.4 Common tail (`FUN_1401a9be0`)

`model+0x2f89 = 1`. If `obj+0x87c == 0`: `"INFO   : Generating mesh - Failed\n"`. Else
`"         Success\n"`, `"INFO   : Generating mesh - Done\n"`, and if TRGVOL `obj+0x2090 > 0`:
`v = FUN_140163540(obj, obj+0x39c0 == 3 ? 0 : 1)` (2D models: `FUN_140306940` area/volume by
GEOTYP; 3D: volume enclosed by the new surface, or by the geometry for 0); if `v > 0`:
`obj+0x2090 = v` and `"INFO   : New target volume (%g) has been assigned.\n"`.

## 7. Messages and return codes

All on stdout, verbatim: `"INFO   : Generating mesh - Object %d...\n"`,
`"INFO   : Generating mesh - Failed\n"`, `"         Success\n"`,
`"INFO   : Generating mesh - Done\n"`, `"INFO   : New target volume (%g) has been assigned.\n"`,
and from the read-back `"ERROR: Keyword file %s open error!\n"` plus whatever the KEY interpreter
prints. Internal step codes (0x14, 0xe, 0x11, 0x12, 0xd, 1, 3, 4, 6-9, 0xc) are never printed;
they only skip the remaining steps. Case return: 2 for a missing/unknown object, else 0.

Note: "Success" only means the object has a mesh afterwards; when meshing fails but the old mesh
is still there (e.g. 3D surface step failed and no SCRATCH.KEY), DEF_PRE prints Success with the
old mesh (inferred from the code paths).

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401a9be0` | DEFAMG handler |
| `FUN_14013e790` / `FUN_14013e840` | `_MeshGenBase` ctor / dtor |
| `FUN_140148840` | bind object, model, problem name; copy object into G+0x18 |
| `FUN_140144620` | prepare (geometry from mesh, temperature, group id) |
| `FUN_140144810` | uniform node temperature detection |
| `FUN_1401fb270` | object group id / distinct element groups |
| `FUN_1401fc730` | build geometry (2D curves / 3D surface) from the mesh |
| `FUN_14013e880`, `FUN_14013e960`, `FUN_14013ea10` | start cleanup |
| `FUN_140144a70` | 2D driver |
| `FUN_140144de0` | 2D input files + DEF_AMG launch |
| `FUN_140144200` | weight normalisation |
| `FUN_140145ce0` | USRWIN.DAT |
| `FUN_140143920` | `<geo>.GEO` (3D inline, 2D via `FUN_140248bf0` -> `FUN_14024ae90`) |
| `FUN_1402789f0` | signed area of a 2D curve |
| `FUN_1401455b0` | 2D read-back of `<prob>_<obj>.KEY` |
| `FUN_1401b5f00` / `FUN_1401b6520` | KEY file loader / KEY interpreter |
| `FUN_140145810`, `FUN_1401458d0` | COATINGMESH launch / `AddCoating.INI` + object KEY (`FUN_14029e250`) |
| `FUN_140145480`, `FUN_14013efb0`, `FUN_140142680` | cleanups |
| `FUN_1401559d0`, `FUN_140155600` | 3D size scaling, surface area |
| `FUN_14013ee00` | 3D surface step |
| `FUN_14013f410` | DEF_AMG3.INI, .GEO, NBCD.DAT, DSTPAR.DAT, BLOCK3.DAT, DEF_AMG3_GUI launch |
| `FUN_140142240` | NBCD.DAT |
| `FUN_14013f8f0` | BLOCK3.DAT |
| `FUN_14013f010` | MESH3D.INP reader (surface mesh) |
| `FUN_140140110` | 3D volume step driver |
| `FUN_140141d50` | MESH3D_GUI / BRKMSH / CHKVOL / second DEF_AMG3_GUI |
| `FUN_1401427e0` | MESH3D.INP writer |
| `FUN_140142f10` | SCRATCH.KEY reader (RZ, ELMCON) |
| `FUN_140141580` | NBCD.OUT reader |
| `FUN_140140310`, `FUN_1401403c0` | 3D in-process coating |
| `FUN_1401443c0`, `FUN_1401444f0` | temperature reset, porous density |
| `FUN_140163540` | new target volume |
| `FUN_1400b9130` | write a small text file (`DEFORM_UIRUNREMESH.DAT`) |
| `FUN_140202700` | allocate mesh arrays (nodes, elements, type) |

## 9. Open questions

* Contents and keyword set of DEF_AMG's `<prob>_<obj>.KEY` and of `SCRATCH.KEY` beyond RZ/ELMCON
  (produced by the external programs; check with an oracle run).
* `obj+0x2d18` has no writer found (displacement scan of the whole image); its default is 0. If it
  is set through a pointer elsewhere, the alternate `DEF_MGN.ACT` becomes reachable.
* Coating (`obj+0x2de8`): settled by p8-06 (T8-5), never > 0 in batch, so COATINGMESH and the
  3D in-process coating `FUN_1401403c0` (with the DEF_MHCK mesh check `FUN_14026a4f0`) are not
  reached. The count is block `obj+0x2d10` + 0xd8; its only writers are `FUN_140189ad0` / the
  defaults `FUN_140189790` (0) and the setter `FUN_14018af80`, called only by the block copy
  `FUN_140189b70` (source block's count) and the uncalled `FUN_140141050`; KEY readers and the DB
  reader `FUN_14018a180` never write it (full proof in `docs/re/status/p8-06.tsv`, row 1401403c0).
* CHKVOL (T8-5, p8-06): not reachable from DEFAMG (and REMESH) or CNVT3D: both run the start
  cleanup `FUN_14013e880` (`FUN_14013ea10` deletes `CHK_VOL.INI`) before the 3D steps, and only
  DEF_AMG3_GUI runs in between. Oracle runs with a `CHK_VOL.INI` input
  (`tests/commands/DEFAMG/defamg_3d_chkvol.json`, `tests/commands/CNVT3D/cnvt3d_chkvol_tet_obj2.json`)
  show it deleted and no CHKVOL. The only caller of the volume step without that cleanup is
  `FUN_140002690`, menu 5 > object > 5 die geometry > 7 Generate Mesh (packet p10-02); there a
  `CHK_VOL.INI` with a value > 0 launches `CHKVOL > CHK_VOL.MSG` after BRKMSH (capture run of
  `tests/commands/MENU5/menu5_die_generate_mesh_3d_chkvol.json`).
* Exact `AddCoating.INI` argument order (decompiler only, not verified; unreachable here):
  `"%s\n"` KEY name twice, `"%f %d %d %d\n"`, per layer `"%g  "`, `"\n%d  %d\n%d  %d\n%d\n"`,
  per layer `"%d\n"`.
* Identity of the CRT helpers `FUN_140483f60` (pow) / `FUN_140484644` (sqrt) /
  `FUN_140484558` (fabs) is inferred from use.
* Defaults `DAT_140522fc0`, `DAT_140522fb8`, `DAT_140522fb0`, `DAT_14051f258` were not dumped.
