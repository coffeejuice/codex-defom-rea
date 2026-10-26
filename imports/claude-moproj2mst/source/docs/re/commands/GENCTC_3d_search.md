# GENCTC 3D: pipeline, master surface, candidate search and geometric kernels

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (headless Ghidra, project copy `ghidra_p67c`).
Decompiles: `deform-rea-work/out/p67/contact/g3d.c` (split per function into
`out/p67/ctc3d-search/fn/FUN_<addr>.c`); listings of every floating-point kernel quoted here in
`out/p67/ctc3d-search/lst/l_<addr>.txt`, with a symbolic SSE evaluation of each listing in
`out/p67/ctc3d-search/sse/l_<addr>.txt` (tool `out/p67/ctc3d-search/sse.py`); constants in
`out/p67/ctc3d-search/d1.out`, `d2.out`, `d3.out`. Parent spec: [GENCTC.md](GENCTC.md) (driver,
tolerance, 2D). The per-node decision and write-back (`FUN_14005190e`, `FUN_140040d77` and its own
callees) are in the ctc3d-decide worker's file; this file covers everything upstream of it and the
shared search kernel `FUN_14006798b` (MDSPS2_REC) that the decision routine calls.

**[fact]** = read in the code (decompile, checked in the listing where floating point order
matters), **[inferred]** = deduced, not followed to the end.

## 0. Status

| Part | Status |
|---|---|
| Pipeline order of `FUN_1400512aa` and what each stage does for the slave O and for its partners | specified |
| Cube table, cube attribute fields (NI/NR/EI/ER) and their writers | specified (field map §3) |
| Master surface build `FUN_14004f56b` (points, polygons, normals, areas, neighbours, node lists, node normals, dihedral angles) | specified; topology helpers (patches, folding, curves) summarised |
| C locator library (grid sizing, point/polygon insertion, point search, cell radius, box query) | specified with formulas |
| SRCNT stage `FUN_14003b304` and SRCNT2/4/5 | control flow and conditions specified; inactive for a plain GENCTC while no two partners are within tol of each other (oracle: active in `d3d_two_masters`, §12); symmetry planes from fixed-code faces disable partner polygons (§7.2) |
| MDSPS2_REC `FUN_14006798b` (point-to-surface search) | specified step by step |
| Triangle kernel (`FUN_1400702c8`, `FUN_1400647a0`, `FUN_140063ab3`, `FUN_140070bf5`) | specified with operation order |
| Quadrilateral kernel `FUN_1404d203b` (FTN_geoutl bilinear projection, ≈30 routines, 35 KB) | structure, main path, shape functions, inside code and Newton step specified; fall-backs summarised (§9.3, open item 1) |
| Optional files SRCNT2.DAT, SKIP_PNTR.DAT, DIEGEO.DAT / DIE_00k.GEO | specified (§11) |

## 1. Floating-point conventions (read this first)

* **Ghidra prints 3-term Fortran sums in reverse and without the parentheses that matter.** The
  decompile shows `z*z + y*y + x*x`; the listing evaluates `t = y*y; t = t + x*x; s = z*z + t`,
  i.e. `(x*x + y*y) + z*z` (the two additions are commutative, so `(x²+y²)+z²` is the exact
  bit-level form). This was checked in the listing of `FUN_1404d68dd`, `FUN_140066ff1`,
  `FUN_140066a50`, `FUN_140070bf5`, `FUN_14003b0c6`, `FUN_140075036`, `FUN_14006798b` (all dot
  products and norms), `FUN_1404d6974` and `FUN_1404d3e13` **[fact]**. Every dot product
  `a·b` in this file therefore means `((a1*b1 + a2*b2) + a3*b3)` and every `|v|` means
  `sqrt((v1*v1 + v2*v2) + v3*v3)` unless stated otherwise. A 3-factor product written `a*b*c`
  in the Fortran is `((a*b)*c)` in source order **[fact, e.g. FUN_140075036, FUN_140070bf5]**.
* The C cube/locator library (`0x140225000`-`0x140240fff`, `0x1403fe000`-`0x140454fff`) is MSVC
  code; its expressions follow the C source left to right as printed (`FUN_140406540`:
  `((dx*dx + dy*dy) + dz*dz)` **[fact, listing]**).
* Fortran `sqrt` is an inline `SQRTSD`. The C library calls the CRT: `FUN_140484644` = `sqrt`,
  `FUN_140484558` = `fabs`, `FUN_140483f60` = `pow`, `FUN_140486e8c` = `ceil` **[fact]**.
  `FUN_1404b3878` = PathScale `atan2` (used for dihedral angles, §5.8).
* `ABS(x)` is `andpd` with `DAT_140bad200` (0x7fff…); `-|x|` is `xorpd -0.0 (DAT_140bad268)`
  of `|x|`; `sign(|a|, b)`-style assignments are written out where used **[fact]**.
* Several Fortran constants are single-precision literals widened to double; they are given with
  their exact double value (e.g. `0.0500000007450580596923828125`). Float literals in the C
  library: the cube insertion tolerance is the float `1e-6` = 9.999999974752427e-7 **[fact]**.
* Integer conversions: every cell index is `(int)` = `CVTTSD2SI` truncation toward zero
  **[fact, listing of FUN_140226150]**.

## 2. Pipeline

### 2.1 Before the Fortran: cubes for every object (`FUN_1401a7710`, read fully)

`FUN_140225820` frees old cubes, `FUN_140225640(&(3·nobj))` allocates the cube table
(`DAT_1406fe4f0`, records of 0x1a0 bytes, count `DAT_1406fe4fc`, capacity `DAT_1406fe4f8` =
3·`model+0x1378`). Then for object slots 1..`model+0x1378` with a mesh (`+0x87c`) or rigid
geometry (`FUN_140160d90`): `FUN_1401a7980` chooses the source (see GENCTC.md §5.2; for the
UPSET seeds the dies 1 and 3 use the DIEGEO polygons, the workpiece 2 its FE boundary surface 0)
and `FUN_14004f56b(&objno, &1, coords, faces, &4, &1, &npts, &npoly, &0, &flag)` builds the
cube of instance 1 (§5) **[fact]**. All objects get a cube, the slave included.

### 2.2 `FUN_1400512aa(&O, &1, coords, bcc, &ndof, &1, &N, &nface, &1, &0, &0, &0, &tol)` (read fully)

| Step | Call | Works on | Effect (details in §) |
|---:|---|---|---|
| 1 | `FUN_14005c3a7(O,1,nface,bcc,ndof)` | slave O | EI3 per-edge BC class of O's polygons (§6.1) |
| 2 | `FUN_140052f49(O,1,N,bcc,ndof)` | slave O | NI5 node feature class, "Critical Corner Node" (§6.2) |
| 3 | `FUN_140029d47(O,1,N,nface)` | slave O | NI3 on-surface flags, EI2 validity (§5.5) |
| 4 | `FUN_14002cc15(O,1,nface,bcc,ndof)` | slave O | EI2=0 for polygons whose nodes share a positive BC code (§6.3) |
| 5 | `FUN_14003b304(coords,O,1,N,nface,bcc,ndof,&0,&tol,&0,&0)` | partners, O | SRCNT: tolerance, partner prep, SRCNT2/4/5, symmetry, DIEGEO (§7) |
| 6 | `FUN_140088e99()` | partners | NR1[4]=0 for nodes of partner polygons with EI2≠1 (§7.7) |
| 7 | `FUN_14003fadc(O,1,N,nface,&0)` | partners | NI7 edge/feature flags (§8) |
| 8 | `FUN_14005190e(…, &node, …)` for node = 1..N | slave nodes | decision + write-back (ctc3d-decide), which calls MDSPS2_REC (§9) per partner |

`DAT_140676704` (passed as SRCNT's `param_8`) is set to 0 before step 5, so SRCNT takes its full
branch (the `==1` branch would only recompute EI2 with step 4) **[fact]**. The globals
`DAT_14067670c` / `DAT_140676708` hold the node counter of step 8.

Partners: `DAT_140771918[1..DAT_14077867c]` (the list built by the driver; O is not in it),
instance of object k: `DAT_140778684[k]` (=1 for O and all partners) **[fact]**.

## 3. Data model

### 3.1 Cube record (0x1a0 bytes, `DAT_1406fe4f0 + i*0x1a0`) **[fact]**

| Offset | Content |
|---|---|
| +0 / +4 | object number / instance |
| +8 | locator (`defLOCD`, 0xf0 bytes, §4) |
| +0xd0 | index base of polygon node numbers (=1: `locator[0x32]`, from the call's `&1`) |
| +0xf8 | polygon cell data `defCELL`: `[0]` count, `[1]` capacity, `[2]` ints per cell (4), `[4]` grow step, `+0x18` data pointer = the caller's face array (not copied) |
| +0x118 | point data `defDPT`: `[0]` count, `[3]` bounds-dirty, `+0x10..+0x3f` bounds (xmin,xmax,ymin,ymax,zmin,zmax), `+0x40` data pointer = the caller's coordinate array (not copied) |
| +0x160 | scratch cell list (box-query results) |
| +0x180 / +0x190 | node / polygon attribute lists: per entity a linked list of (field id, count, data) |

The cube **aliases the object's coordinate and face arrays**; coordinates moved by the
write-back are seen by later searches of the same GENCTC call **[fact: +0x158/+0x110 store the
caller's pointers]**.

Attribute access (all `(obj, inst, &entity, &field, buf, &count)`): `FUN_140228830` Get NI,
`FUN_140228690` Init NI, `FUN_140228b10` Get NR, `FUN_140228980` Init NR, `FUN_140228260` Get EI,
`FUN_1402280d0` Init EI, `FUN_140228540` Get ER, `FUN_1402283b0` Init ER. "Init" replaces the
field's data (creates it if missing). A Get on a missing field returns count 0 and leaves the
buffer untouched (`FUN_140404b90`/`FUN_140404f70`) **[fact]**. `FUN_140227e00` returns the
polygon's 4 node numbers (minus base, plus 1) and count 4; `FUN_140227fc0` copies a point's
3 coordinates. An unknown (obj, inst) prints `cfnCUBE_…: the bucket of object(%d) …` via
`FUN_14039d3a0` and returns count 0.

### 3.2 Fortran COMMON arrays (index `obj + 100·inst`, i.e. `[inst][100]`) **[fact]**

| Address | Content | Writer |
|---|---|---|
| `DAT_14076b6a8 + (obj*3+inst*300)*4` | number of points of the cube | `FUN_14004f56b`, `FUN_14005110a` |
| `DAT_14076e588 + (same)*4` | number of polygons | same |
| `DAT_140773534 + (obj*2+inst*200)*4` | 4 (nodes per polygon) | `FUN_14005110a` |
| `DAT_14075a9b8 + (obj*6+inst*600)*8` | bounding box `[xmin,ymin,zmin,xmax,ymax,zmax]` of **all** cube points, widened by `DAT_1406766f8 = DAT_140bad4a0 = 1e-10` on every side (`FUN_1404d1d05`, start values ±1e10 `DAT_140bb2760/68`) | `FUN_14005110a` |
| `DAT_140767500 + (obj+inst*100)*8` | `sqrt(min positive NR2 over O's surface nodes)` | SRCNT §7.1 |
| `DAT_14076abb8` | number of symmetry planes (0 in DEF_PRE: its only writers `FUN_14008616f`, `FUN_140083e2d`, `FUN_140082ef0` have no callers) | — |
| `DAT_14075bc80/84` | process partition count/index, reset to 0 by `FUN_140359a70` (called from the DB loader), never set otherwise ⇒ all "partition" branches are dead | — |

### 3.3 Cube attribute map (who writes what)

Nodes (NI/NR, entity = point number):

| Field | Count | Meaning | Writer |
|---|---:|---|---|
| NI1 | ≤4999 | ascending list of polygons (valid by `FUN_140062fb2`) containing the node | `FUN_14007c697` (ND2ELM) §5.7 |
| NI2 | var | ascending list of neighbour nodes (sharing a polygon) | `FUN_14005815c` §5.9 |
| NI3 | 1 | 1 = node lies on ≥1 polygon whose EI2 ∉ {0, −2000} | `FUN_140029d47`, `FUN_14003c506` §5.5 |
| NI4 | 6 | `[cx,cy,cz, px,py,pz]` contact codes and polygon numbers | zeroed for partners in SRCNT; filled only by partner–partner contact `FUN_14003e7f6` §7.4 |
| NI5 | 1 | feature class of slave nodes (0,1,2,3,4 or a contact code) | `FUN_140052f49` §6.2 |
| NI7 | 2 | `[class, object]`, partners: class 2 = node on a boundary/feature edge | `FUN_1400501d7`, `FUN_14003fadc` §8 |
| NR1 | 4 | unit area-weighted node normal + convexity indicator | `FUN_14005815c`, `FUN_14007d97c`, `FUN_140088e99` |
| NR2 | 1 | nodal area Σ area/nvert | `FUN_14005815c` |
| NR5 | 2 | (max, min) cos between node normal and directions to NI2 neighbours | `FUN_14005815c` |
| NR4 | 9 | no writer upstream of the decision; created per slave node by the decision's `FUN_14004946f` (see GENCTC_3d_decide.md) | ctc3d-decide |

Polygons (EI/ER, entity = polygon number):

| Field | Count | Meaning | Writer |
|---|---:|---|---|
| EI1 | 2·nv | `[k]` neighbour polygon across edge k (vertex k→k+1), `[nv+k]` edge-table id | `FUN_1400798bf`, `FUN_14007b1ea`, `FUN_14007aa22` §5.6 |
| EI2 | 1 | status, see below | many |
| EI3 | nv | per-edge BC class 1/2 (slave O only) | `FUN_14005c3a7` §6.1 |
| EI4 | 1 | patch number | `FUN_14007b1ea` |
| EI5, EI6 | var | curve/topology lists (CRVPLY) | `FUN_14005bbf2`→`FUN_14005d848`/`FUN_1400600bb` (summarised) |
| ER1 | 3 | unit normal | `FUN_1400407e7` §5.4 |
| ER2 | 1 | area | `FUN_1400407e7` |
| ER3 | nv | dihedral angle at edge k in (0, 2π]; 0 when no neighbour | `FUN_1400610f2` §5.8 |

EI2 values **[fact]**: `1` valid; `0` disabled (last node < 0 in `FUN_140029d47`; O polygons
with a common positive BC code, `FUN_14002cc15`; SRCNT5 for last node < 0); `−1000` zero area
(ER2 ≤ 1.0000000133514e-10, `FUN_140029d47`) or folded (`FUN_14007d97c`, "Polygon is set to
invisible"); also 0 for partner polygons lying in a symmetry plane of O (`FUN_14003d2ee`, §7.2
step 4, corrected after oracle run); `2` symmetry-plane polygon (`FUN_14003dabe`, needs the
`DAT_14076abb8` table, dead); `−2000`
interference (SRCNT4 `FUN_140033d76`; searches then use 5·tol); `−k` polygon of O in contact
with object k (SRCNT5).

Polygon node list convention `FUN_140062fb2(conn, &n, &flag)`: flag = 1; if `conn[n] < 1` then
flag = −1 and `conn[n] = −conn[n]`; if `conn[1] ≤ 0` then flag = 0 and `conn[1] = −conn[1]`.
Builders skip polygons with flag 0. A 4-node polygon with `|conn1| == |conn4|` (or n = 3) is a
triangle everywhere **[fact]**. `FUN_14004f363` returns the coordinates of the polygon's nodes
with node number > 0 after `abs`, and the count of those.

## 4. C locator library (cube type 3: points + polygons)

### 4.1 Grid sizing (`FUN_140226150`, cfnCUBE_BuildBucket; `FUN_1403fed00`, `FUN_1403ff970`) **[fact]**

With `np` = point count and `nf` = polygon count:

1. Division size `div`: `nf < 10` → 5; `10 ≤ nf ≤ 99` → 10; `100 ≤ nf ≤ 999` → 20;
   `1000 ≤ nf ≤ 9999` → `(int)(0.0033 * (double)(nf−1000)) + 20` (`DAT_140530da0`);
   `nf ≥ 10000` → `(int)((double)(nf−10000) * 5e-4) + 50` (`DAT_140530d98`).
2. Bounds = min/max over **all np points** (`FUN_140406360`, strict `<`/`>` updates from
   ±1e29). Per axis: if `max ≤ min` then `max = min + 1.0`; if `fabs(max−min) < 0.001` then
   `max += 0.001` (`DAT_14051f1e8`); `L = fabs(max−min)`.
3. `h = pow(fabs((Lx*Ly)*Lz), 0.3333333) / (double)div` — the exponent is the decimal literal
   0.3333333 (`DAT_140530d90` = 0x3fd55555318abc87), not 1/3.
4. `n_i = (int)(L_i / h)`; if `n_i < 6` then 5; then `n_i = min(n_i, nf)`.
5. If `div > 49`: while `1.1 < (double)((n0*n1*n2) / div³)` (integer division) decrement each
   `n_i` by 1 (but not below 5).
6. `FUN_1403ff890` clamps each `n_i` to [1, 100]. `FUN_1403ff970` copies the bounds, re-applies
   the two widening rules of step 2, sets cell sizes `H_i = (max_i−min_i)/(double)n_i`, the
   insertion tolerance `tol = (double)1e-6f` and `tol²`, and an "insertion level"
   `min(max n_i, (int)ceil(tol / min H_i))`.

Cell index of a coordinate `x` on axis i (same code in `FUN_140400330`, `FUN_140401050`,
`FUN_14022d770`, `FUN_14022fcd0`, `FUN_140228c60`):

```
if fabs(x - max_i) > EPS:  idx = (int)(((x - min_i) / (max_i - min_i)) * (double)n_i)
else:                      idx = (int)((((x - min_i) * 0.999) / (max_i - min_i)) * (double)n_i)
```

`EPS` = 1e-8 (`DAT_14051bed0`) except in polygon insertion where it is 1e-6 (`DAT_14051f1b8`);
`0.999` = `DAT_140522f68`. Searches clamp `x` into [min_i, max_i] first; insertion does not but
clamps the resulting index range to [0, n_i−1]. Linear cell number `ix + iy*nx + iz*nx*ny`.

### 4.2 Insertion (`FUN_140226150` type 3)

* Points: a byte map is set to 'F' and `FUN_140233ef0` marks 'T' every point referenced by
  `abs(conn)` of any polygon; points 1..np marked 'T' are inserted in order into their cell
  (`FUN_140400330`) **[fact]**. Only surface points are searchable.
* Polygons (`FUN_140401050`, for every polygon with `conn[1] > 0`): the cell range is the
  min/max of the vertex cells (vertices taken with `abs`). A normal is computed
  (`FUN_140240d80`: `e = v1−v0`; for j = 2..n−1: `s += e × (v_j−v0)`, `e = v_j−v0`; unit if
  `|s| > 1e-12`, else 0). If the normal is the zero vector within 1e-4 per component
  (`FUN_14023aa90`), or the range spans ≤ 3 cells on every axis, the polygon goes into **every**
  cell of the range. Otherwise each cell of the range is tested: vertices are mapped to
  `((v − (min + (i + 0.5)·H)) / (1.01·H))` per axis (`DAT_14051f240`, `DAT_140562a90`) and the
  polygon–unit-box test `FUN_140454110` + `FUN_1404537f0` decides (not transcribed, §15 item 4).
  Cell lists keep insertion order (polygon order) **[fact]**.

### 4.3 Nearest surface point `FUN_14022d770` (cfnCUBE_SearchPoint_Obj) **[fact]**

Input point P. If the cube has fewer than 16 points (all points, not only surface points): brute
force over all points 1..np, squared distance `FUN_140406540` = `((dx²+dy²)+dz²)`, strict `<`,
start = point 1. Otherwise:

1. `Pc` = P clamped to the bounds; cell `c` of `Pc`; `R0 = sqrt(Σ max(|Pc_i−min_i|,|Pc_i−max_i|)²)`
   (C order x,y,z); best² = R0²; levels = `min(max n_i, (int)ceil(R0 / min H_i))`.
2. For level L = 0, 1, …: visit cells with `max(|ix−cx|,|iy−cy|,|iz−cz|) = L` inside the grid
   (loop order x outer, y, z inner), every point of the cell: `d² = |Pc − point|²` (note: the
   **clamped** point), keep if `d² < best²` (first found wins ties).
3. After each level: `r = sqrt(best²)`; per axis `lo = min_i + (c_i−L)·H_i`,
   `hi = min_i + (c_i+L)·H_i` (for L = 0: `c_i+1`), `g_i = min(|Pc_i−lo|, |Pc_i−hi|)`;
   `g = min(g_x, g_y, g_z)` evaluated as `t = (g_y ≤ g_x ? g_y : g_x); g = (g_z ≤ t ? g_z : t)`.
   Stop if `r < g`. (The upper face uses `c+L`, not `c+L+1`: a quirk to keep.)
4. Returns the point number and `sqrt(|P − point|²)` with the **unclamped** P. No point found:
   fatal `defCUBE_SearchPoint_Obj : Could not find a min distance point` + exit (`FUN_14039d450`).

### 4.4 Initial search radius `FUN_14022fcd0` (SearchPoint_CellPolygon) **[fact]**

Same set-up as 4.3 (clamped point, R0, level count). Shell by shell (same visiting order), the
**first** cell with a non-empty polygon list ends the search; the radius is
`sqrt(max over the 8 corners of that cell of |Pc − corner|²)` (corners
`(min+ix·H) + {0,H}` per axis, order 0..7 as in the decompile, max with `<`). Returned in
MDSPS2's distance argument. No cell found: fatal error + exit.

### 4.5 Box query `FUN_140228c60` + `FUN_14022a5c0`

`FUN_140228c60(obj,inst,box[6],&n,&exact=0,&dbg)`: no intersection with the cube bounds
(`FUN_140240b40`) → n = 0. Otherwise the cell range of the two clamped box corners is visited and
every polygon id of every cell is appended to the scratch list (duplicates kept); `n` = raw
count. `FUN_14022a5c0` turns the scratch list into the **unique ascending** polygon list (bit
map) and returns its length **[fact]**.

## 5. Master surface construction `FUN_14004f56b` (per object, read fully)

Called with `npts`, `nface`, 4 nodes per polygon. Does nothing when `npts == 0` or `nface == 0`.
Steps (`DAT_1405c16f0` = 0 in the image, so the "== 0" branches run) **[fact]**:

| # | Call | Result |
|---:|---|---|
| 1 | store `DAT_140778684[obj]=inst`, point/polygon counts (§3.2) | |
| 2 | `FUN_140225cd0` + `FUN_140225ff0`→`FUN_140226150` type 3 | cube + locator (§4) |
| 3 | `FUN_14005110a` | counts again, nodes/polygon = 4, bounding box (§3.2) |
| 4 | `FUN_1400407e7(…,&1,&2,…)` | ER1 normal, ER2 area (§5.4) |
| 5 | `FUN_140029d47` | NI3, EI2 (§5.5) |
| 6 | `FUN_14007c697(…,&1,…)` | NI1 (§5.7) |
| 7 | `FUN_1400798bf`, `FUN_1402280d0` (EI4=…), `FUN_14007b1ea` | half-edge matching, EI1 neighbours, EI4 patches; "Info - Multiple patches detected" (unit 1) (§5.6) |
| 8 | `FUN_14007aa22` | edge table, EI1 `[nv+k]` edge ids |
| 9 | `FUN_1400610f2(…,&3,…)` | ER3 dihedral angles; "Warning -- the area of Polygon (…) is Zero!" (unit 1) (§5.8) |
| 10 | `FUN_14005815c` | NR1, NR2, NR5, NI2 (§5.9) |
| 11 | `FUN_14007d97c` | folding check: "Info: Folding Node Identified", "Polygon is set to invisible" ⇒ EI2 = −1000, NR1 update (summarised) |
| 12 | `FUN_14005bbf2` | curves/topology (EI5/EI6, "Info: Error in Topology") (summarised) |
| 13 | `FUN_14003c506` | NI3 = 0 for nodes all of whose polygons have EI2 ∈ {0, −2000} |

### 5.4 Normal and area `FUN_1400407e7` **[fact, listing]**

For polygon p: X = coordinates of its nodes (≤4), conn via `FUN_14004f363`.

* Normal `FUN_140066a50(X, n, &bad)`: `a = X2−X1`, `b = X3−X1`; `FUN_140066ff1(a,b,n,&q)`:
  `n = (a2*b3 − b2*a3, a3*b1 − b3*a1, a1*b2 − b1*a2)`, `q = (n1²+n2²)+n3²`; if
  `|q| > 1.0000000168623835e-16` (`DAT_140badec8`): `q = sqrt(q)`, `n /= q` (component order
  z, y, x — irrelevant), else n = 0, q = 0. If `|q| ≤ 9.999999960041972e-12` (`DAT_140badec0`):
  `a /= |a|`, `b /= |b|` (each only if its length > 0) and the cross product is repeated; still
  ≤ that value → `bad = 1` (normal stays 0). ER1 = n (3 values).
* Area `FUN_140075036(X, &A, conn, &mode)` with mode 0 uses vertices 1, 2, 3: `FUN_1404d68dd`
  gives unit `u = (X1−X2)/|X1−X2|`, `l1 = |X1−X2|` and `w = (X3−X2)/|·|`, `l2`
  (`FUN_1404d68dd(A,B)` = `B−A`... see note); `c = (u1*w1 + u2*w2) + u3*w3`, clamped to
  [−1, 1] (`DAT_140bad880`, `DAT_140bad138`); `A = |((sqrt(1 − c*c) * l1) * l2) * 0.5|`.
  If the polygon has 4 nodes with `conn4 ≠ conn1` and `conn4 > 0`, X2 := X3, X3 := X4 and the
  same formula is added: `A = A + A2`. ER2 = A.

  Note: `FUN_1404d68dd(A, B, v, &l)` returns `v = B − A`, `l = (v1²+v2²)+v3²`, and if `l ≠ 0`:
  `l = sqrt(l)`, `v /= l` **[fact, listing]**. In `FUN_140075036` the calls are
  `(X1, X2)` and `(X3, X2)`, so `u = (X2−X1)/l1`, `w = (X2−X3)/l2`, the angle at vertex 2.

### 5.5 Validity `FUN_140029d47` **[fact]**

All nodes: NI3 = 0. For each polygon: `flag = (conn[n] ≥ 0) ? 1 : 0` (raw sign of the last
entry); `if flag > 0 and ER2 ≤ 1.0000000133514e-10 then flag = −1000`; EI2 = flag; every node of
the polygon (abs) gets NI3 = 1, whatever the flag.

### 5.6 Neighbours `FUN_1400798bf` (read), `FUN_14007b1ea`, `FUN_14007aa22` (read)

Half-edges `(conn_k, conn_{k+1})` of every polygon with flag ≠ 0 and `conn1 ≠ 0`. For each
half-edge (n1,n2), the candidate polygons are the intersection of NI1(n1) and NI1(n2)
(`FUN_14006f089`); among those holding the reversed half-edge (n2,n1) the one with the largest
`ER1(p)·ER1(q)` (start −10.0, strict `>`) is paired (first wins ties, polygon-list order). More
than two matches sets the "multiple patches" flag. `FUN_14007b1ea` then floods patches (EI4) and
writes EI1 neighbours; `FUN_14007aa22` builds the 8-int edge table (n1, n2, p1, k1, p2, k2, 0, …)
and stores the edge id in EI1[nv+k] **[fact for 1400798bf/14007aa22; 14007b1ea summarised]**.

### 5.7 Node → polygon lists `FUN_14007c697` (ND2ELM, read) **[fact]**

For polygons in ascending order with `FUN_140062fb2` flag ≠ 0, every node `abs(conn_k) ≠ 0`
gets the polygon appended once (`FUN_14007e28d` hashed bins of 16 per node, grown by `N` when
full). More than 4999 polygons on one node: "Error: Local Array size exceeded in ND2ELM" and
STOP. NI1 = the list (ascending polygon numbers).

### 5.8 Dihedral angles `FUN_1400610f2` / `FUN_140062fff` / `FUN_14007ea01`

For every edge-table entry with two polygons (both with flag ≠ 0 and area > 1.0000000133514e-10)
the angle is computed in the frame of the shared edge: `FUN_1404d68dd(Xa, Xb)` edge vector and
length; if the length ≤ 1.0000000036274937e-15 the angle is `3.140000104904175`
(`DAT_140bade58`). Else `FUN_1404cf5c0` builds the edge frame and `FUN_1404d0939` maps the
opposite vertex (triangles) or the polygon centroid (quads, plain sum / n) of each polygon into
it; with `(x1,y1)`, `(x2,y2)` the 2D components, `r1 = sqrt(y1² + x1²)`, `r2 = sqrt(y2² + x2²)`,
`s = ((x1*y2 − y1*x2)/r1)/r2`, `c = ((y2*y1 + x2*x1)/r1)/r2`, `angle = atan2(s, c)`
(`FUN_1404b3878`; argument registers not re-checked, §15 item 3); if `angle ≤ 0`:
`angle = 3.1415926535898 * 2.0 + angle` (`DAT_1405c8820`, `DAT_140bad4d0`). ER3[k] of both
polygons = angle; edges without neighbour = 0.0 **[fact]**.

### 5.9 Node normals `FUN_14005815c` (ININDP, read fully) **[fact, listing for sums]**

For node i with NI1 count > 0: over its polygons with flag ≠ 0, `nv` = 3 for a degenerate quad
else the count; the polygon's nodes other than i are appended to a neighbour list (>5000:
"Error: Increase Array Size in ININDP"); with `A` = ER2, `n` = ER1:
`NR2 += A / (double)nv`; `S += A * n` (per component, `S_c = A*n_c + S_c`).
`|S| = sqrt((S1²+S2²)+S3²)`; if `|S| ≤ 1.0000000133514e-10`: message "Potential Problem at node"
(unit 1), NR1 = (0,0,0,0), NR2 = 0. Else `S /= |S|`, NR2 written, NI2 = sorted unique
neighbours (`FUN_14005a6bf`), for each neighbour j: `u = unit(X_j − X_i)`, `c = u·S`,
`cmax = max(cmax, c)` (start −1), `cmin = min(cmin, c)` (start 1). Convexity: over the node's
polygons and their edges k that start at node i and have a neighbour (EI1[k] ≠ 0):
`A_cnt += ER3[k] > 4.398226261138916`, `B_cnt += ER3[k] < 1.8849540948867798`;
`conv = −B_cnt + (A_cnt > 0 ? −0.6000000238418579 : 0)` if `A_cnt < B_cnt`;
`conv = A_cnt + (B_cnt ≠ 0 ? 0.6000000238418579 : 0)` if `B_cnt < A_cnt`; else 0.
NR1 = (S1, S2, S3, conv), NR5 = (cmax, cmin).

## 6. Slave preparation (stages 1, 2, 4)

### 6.1 `FUN_14005c3a7` (stage 1) **[fact, summarised]**

Initialises EI3 of every O polygon to (0,0,0,0), then per polygon p (EI1 neighbours, conn): the
BC codes of p's nodes (`bcc[3·(node−1) + c]`) give the list of distinct codes common along the
polygon (`FUN_14003a580`, which uses `FUN_14003a978`); a polygon whose nodes have a common
**positive** code gets that code; for each edge k with neighbour q the same is done for q and
EI3[k] of p is set to 1 when the codes differ in object, 2 when they are equal (and `> 0`) —
exact rule in the decompile lines 200-330 (keep the decompile as reference for the port).

### 6.2 `FUN_140052f49` (stage 2, read) **[fact]**

NI5 := 0 for all nodes. For each polygon edge k with EI3[k] == 1, the distinct BC codes of its
two nodes (`FUN_14003a580`) are scanned: a code equal to 1, > 600, or in 4..8 sets `w = 1`; a
code < 1 sets `w = DAT_14076abb8[|code|]` (symmetry plane count of that object, 0 in DEF_PRE);
both nodes get `NI5 += w` (`w` persists from the previous code if none matches). Then every node
NI5 := NI5 / 4 (integer, rounding toward zero). A node with NI5 == 2 that has an edge to a node
with NI5 == 1 and fewer than 3 edges to nodes with NI5 == 2 becomes NI5 = 3; these nodes are
listed: `1000(' Info: Critical Corner Node Idenfied - Object Number :',I5)` and
`1001(' Info: Node ',I7)` on unit 1 (NEWEQ.MSG). Finally along edges with EI3 == 2 the common
code of the two nodes (`FUN_14003a978`) is copied into NI5 of nodes still 0 if it is negative,
or 4 if it is positive.

### 6.3 `FUN_14002cc15` (stage 4) **[fact]**

For each O polygon with EI2 == 1: if `FUN_14003a978` finds a common positive BC code of its nodes,
EI2 = 0.

`FUN_14003a978(bcc, conn, &n, &ndof, &code, &dir)` (read fully): node 1's three codes
c = bcc(node1, k), k = 1..3; for the first k with `c ≠ 0`: if `c == 1` count the nodes whose
component k is 1 (dir = k), else count the nodes having `c` in any of their three components;
when every node matches, `code = c` (and `dir = k` when `c == 1`) and return; otherwise try the
next k. Returns code 0 when no component of node 1 is common **[fact]**.

## 7. SRCNT `FUN_14003b304` (read fully)

### 7.1 Tolerance **[fact, listing]**

`m = min{ NR2(i) : node i of O with NI3 ≠ 0, 0 < NR2(i) < m }` with `m` starting at
1e7 (`DAT_140bad0f0`); `r = sqrt(m)` (SQRTSD) is stored in `DAT_140767500[O]`.
`tol' = tol` if `tol > 1.000000013351432e-10`; else `tol' = r * 0.05000000074505806`, and if that
is ≤ 0 then `tol' = 1.000000013351432e-10`. (GENCTC.md §3 calls NR2 "node real value #2": it is
the nodal area.) Note the strict `0 < NR2 < m`. `tol'` (`DAT_140655ac8`) is what SRCNT2/4/5 use;
the decision routine receives the driver's `tol` through `FUN_1400512aa`'s 13th argument.

### 7.2 Order of work

1. `FUN_14003b130`: `DAT_140655a94 = 1` if any polygon of any partner (≠ O, ≠ 0) has a
   negative last node; then steps 6-9 are skipped.
2. `FUN_140232090` cube cell sizes of O (unused afterwards); `FUN_14003b0c6` = diagonal
   `|max−min|` of O's bounding box; `DAT_140655af8 = diag * 2.0`.
3. `FUN_14002a857` (SYMGRP2): symmetry planes of O into `DAT_140655b00…` (point),
   `DAT_140656460…` (unit normal), per-plane tolerance, count `DAT_1406570e0`; "Potential error in
   SYMGRP2", "Delete Symmetry definition in Symgrp2" (unit 1). **(corrected after oracle run)**
   The planes come from O's polygons whose nodes share the **fixed code 1** in direction k
   (`FUN_14003a580` common-code list; the branch tests `code == 1`): if `|ER1·(±e_k)| ≥ 0.99`
   (`DAT_140bad3e0` = 0.9900000095367432, axis table `DAT_1405c63e8`) for the first such polygon
   of that direction and sign, a plane is added with normal ±e_k, point = a node of the polygon and
   tolerance `sqrt(ER2) * 0.10000000149011612` (`DAT_140bad2d0`); at most 100 planes ("Potential
   error in SYMGRP2" beyond). A polygon with more than one common code makes the routine return at
   once. UPSET_Brick / UPSET_Tet get two planes (x = 0 and y = 0, the workpiece's quarter-model
   faces). The earlier reading "BC codes 6xx only, none without code 6" was wrong; the
   `DAT_14076abb8` symmetry table of the decision routine is a different one and stays 0.
4. For every partner P with ≥ 2 points and ≥ 1 polygon: NI4 := 0 (6 ints) for all P's points;
   `FUN_140029d47(P)` again (resets NI3 and EI2 of P — overwrites the −1000 set by the folding
   check only if area > tol); if `DAT_14076abb8[P] == 0`, `FUN_14003d2ee` (symmetry plane
   polygons of P; nothing when `DAT_1406570e0 == 0`); then for every other partner Q ≠ O, P:
   `FUN_14003e7f6(P, Q)` partner–partner contact (§7.4).
   `FUN_14003d2ee` **(corrected after oracle run)**: per plane k, a box around the plane point
   (`FUN_140080a83`, in-plane extent from `DAT_140655af8`) is queried on P's cube; every polygon
   with EI2 == 1, `|ER1·N_k| ≥ 0.9950000047683716` (`DAT_140bad7f0`) and centroid (plain mean of
   its nodes) at plane distance `|(X_k − c)·N_k| ≤ tol_k` gets **EI2 = 0** (disabled). In the
   UPSET seeds the die faces x = 0 and y = 0 (18 of the 54 polygons of each die) are disabled;
   `DIE_001.GEO` / `DIE_003.GEO` of `s3d_diegeo_dat` show exactly these with `n4 < 0`, and a die
   shifted by 0.0009 (`v_d0_x_geo`) still has them disabled (tol_k ≈ 0.45). Since MDSPS2_REC
   with mode 0 skips EI2 = 0 polygons (§9.2), no slave node is ever projected onto them.
5. For every partner P with polygons: SRCNT2 `FUN_14002cdee(P, …)` (§7.3).
6. SRCNT4 `FUN_14002f091` (§7.5), then SRCNT5 `FUN_140030d0e` (§7.6).
7. If `DAT_14076abb8 > 0`: per partner `FUN_14003dabe`, `FUN_14003c506`, `FUN_14005d44f`
   (symmetry; dead in DEF_PRE).
8. Debug flag (`param_10`, always 0 here) else: DIEGEO.DAT handling (§11.3).

### 7.3 SRCNT2 `FUN_14002cdee` **[fact]**

Reads SRCNT2.DAT once per process (§11.1). For every polygon p of partner P with EI2 ≠ 0
(tolerance `t = tol'`, `5·tol'` for EI2 = −2000; `dVar11 = tol' * 20.0`): gathers NI4 of p's
nodes; it acts only when a node has a negative contact code or the polygon has a common negative
code, i.e. **only when partners are already marked as touching another object** (NI4 is zeroed
in §7.2 step 4 and only `FUN_14003e7f6` sets it). Then: polygon box ± `tol'*20` overlapping
O's box, area ≥ 1e-10, nodes without code are re-searched with MDSPS2 on the touching object
(mode 3, edge length + 20·tol' radius) and the slave's nearest node, and `FUN_14002de5d`
subdivides the polygon (parameters from SRCNT2.DAT) and may set EI2 of p; message
`'SRCNT2', poly, obj` (unit 6) when MDSPS2 finds no polygon.

### 7.4 Partner–partner contact `FUN_14003e7f6(A, B)` **[fact]**

For each point of A with NI3 ≠ 0: box ± `tol'` around it, `FUN_140228c60` on B's cube; if any
polygon: MDSPS2_REC(B, point, mode 0, start 1e6 `DAT_140bad320`); if found and
(`|d| < tol'` or `d < 0`): code −B and `polygon·sign(d)` go into the first zero slot of NI4 in the
order z, y, x. Message `1001('Info: Contact Between Object Pair',2I5,'has been Established')`
on unit 1 when anything was set.

### 7.5 SRCNT4 `FUN_14002f091` (interference) **[fact]**

Skipped when SKIP_PNTR.DAT exists (§11.2) or `DAT_1405c16fc == 1` (it is 0). Per partner P whose
widened box overlaps O's: P's points whose NI4 holds a negative code other than −O are
collected; with none (the GENCTC case after step 4) nothing happens. Otherwise it prints
`Interference detected, KOBJ=` (unit 6), queries O's polygons in that box and marks them
EI2 = −2000 (`FUN_140032ce4`, `FUN_140032a3e`, `FUN_140033d76`, `FUN_14003329f`, and
"Info: Checking Invisible definition" + `FUN_140034138`) — summarised.

### 7.6 SRCNT5 `FUN_140030d0e` (on O's polygons) **[fact]**

For each O polygon with EI2 ≠ 0: last node < 0 → EI2 = 0. Else sum the node coordinates and
take the common BC code (`FUN_14003a978`); if it is negative, `k = −code ≠ O` and
`DAT_140778684[k] > 0`: centroid = sum / (double)nv; MDSPS2_REC(k, centroid, mode 0, zero
normal); if `d ≤ t` and `t * (−4.0) ≤ d` and `ER1(p)·ER1(found) ≤ −0.949999988079071`
(`DAT_140bad4f0`) then EI2(p) = −k. Only polygons whose nodes all carry an old contact code
(GENCTC without INICTC) are affected.

### 7.7 `FUN_140088e99` (stage 6) **[fact]**

For each partner, polygons with EI2 ≠ 1: each node's NR1 is re-written with its first three
values and 0 as the fourth.

## 8. Partner feature nodes `FUN_14003fadc` + `FUN_1400501d7` (stage 7, read) **[fact]**

`r = sqrt(min ER2 over O's polygons, start 1e8)`. For each partner P: `FUN_1400501d7` sets
NI7 = (1, 0) for all P's points, builds the edge list of polygons with EI2 < 1 (sorted by
`FUN_14008b65b`), marks edges found twice with a negative polygon id and gives NI7 = (2, k) to
the end nodes of single edges (k = object of the touching node code or |EI2|). Then for P's
polygons with `−1000 < EI2 ≠ 1`: negative EI2 copies |EI2| into NI7[2] of its nodes; EI2 ≥ 0
(i.e. 0) polygons with ≥ 2 class-2 nodes are projected (centroid, MDSPS2 mode 3 against each
other partner whose box overlaps) and the nearest object within `r` is stored in NI7[2] of both
polygons' nodes. With every partner polygon valid (EI2 = 1) this stage only initialises NI7.

## 9. MDSPS2_REC `FUN_14006798b` (read fully)

```
FUN_14006798b(&M, &inst, &erfield, &npoly, P[3], st[2] out, &poly out, &d in/out,
              &status out, Q[3] out, dirn[4], &mode, &dbg)
```

`erfield` is the ER field used for the edge test (3 = dihedral angles in all callers), `dirn` an
optional direction (3 components + a cosine limit), `mode`: > 0 accept every polygon, 0 normal,
< 0 = −(object code) for polygons already in contact. Messages only with `dbg == 1`
("Entering MDSP2S_OBJ", "...factor search = ", "...Found polygon", "...Min distance") on
unit 1 **[fact]**.

### 9.1 Set-up **[fact, listing]**

1. `a3204 = mode; a3208 = 0; if mode < 0: a3204 = 1, a3208 = mode`.
   `useDir = (1.000000013351432e-10 ≤ |dirn[1..3]|)`.
2. `*d = R_cell` from `FUN_14022fcd0` (§4.4) — **the caller's input value is overwritten**.
3. `(dn, inode) = FUN_14022d770(P)` nearest surface node of M (§4.3).
4. If `mode < 0` and NI3(inode) == 0: `dn = sqrt(NR2(inode)) * 1.5`, `*d = dn`.
5. `if dn < *d: *d = dn`.
6. If `*d ≤ 9.999999960041972e-13` (`DAT_140badf00`): `r2 = sqrt(NR2(inode)) * 0.009999999776482582`;
   if `*d < r2` (strict) `*d = r2`; if still `≤ 9.99999996e-13`: `*d = 9.99999993922529e-9`.
7. `base = *d * 1.0099999904632568` (`DAT_140badf08`); `factor = 1.0`; `pass = 0`; `phase = 0`.

### 9.2 Search loop

```
loop:
  if phase == 0: R = base * factor; *d = R
     box = P ± R (FUN_1404d1b1c, one point); FUN_140228c60 → raw count; FUN_14022a5c0 → unique ascending list
  else (phase 1): list = NI1(inode) (more than 7000: "Increase NDATA sz from 7000:" (unit 6) + STOP)
  if *d == 0: *d = 9.999999974752427e-7
  status = 0; if phase == 0: pass += 1
  if mode < 1: FUN_14006e938(list, k = -mode)   // stable partition: EI2 == -k first (only if k > 0), then EI2 > 0, then the rest
  best_prev_poly = 0
  for each polygon p in list order (consecutive duplicates skipped):
     f = EI2(p)
     if f == a3208 and a3208 < 0: f = 1
     elif f < 0 and f > -900 and a3204 == 1: a044 = p            // remembers a contact polygon
     consider p only if mode > 0, or (f not in {0, 2, -1000} and (mode >= 0 or f >= 0))
     X, conn = FUN_14004f363(p); nv = count
     w = 1.0; if f != 1 and inode is one of p's nodes: w = 1.000001 (DAT_140badf50)
     tri = (conn1 == conn4) or nv == 3          (nvert = 3 or 4)
     Rb = |*d| * 1.0099999904632568; Rp = Rb * 9.999999747378752e-5
     if polygon box ± Rp does not overlap P ± Rb (FUN_1404d1ed7): skip
     if ER2(p) <= 9.999999960041972e-13: skip
     N = ER1(p); if (|N2| + |N1|) + |N3| <= 1.000000013351432e-10: skip       // listing order: |y| + |x| first
     if useDir:
        c = dirn·N; lim = dirn[4]; if lim > 0.8999999761581421: lim = 0.8999999761581421
        if lim < -0.05999999865889549: lim = -0.05999999865889549
        if lim < c: skip
     if not tri:  FUN_1404d203b(X, P, &s, &t, Q, &dist, &flag)       // §10.4, dist >= 0
                  dist = dist * w
                  v = P - Q; if v·N < 0: dist = -|dist|
                  inside = FUN_1404d3e13(s, t)                         // §10.5
     else:        dist = |*d|
                  FUN_1400702c8(P, X, Q, &dist, &s, &t, &trimode, &edge, &inside)   // §10.1
                  dist = dist * w; trimode = 1                         // trimode (DAT_1406a3200) is 0 only for the first triangle of the call
     for each of p's nodes: if node == inode and dn < |dist|: dist = (dist >= 0) ? dn : -dn
     if |dist| < |*d|:                                                  // strict: first polygon wins ties
        prev = (*poly, code); *poly = p; code = inside
        if inside > 0:                                                  // foot on edge `inside`
           q = EI1(p)[inside]
           if q < 1: code = 0
           else: g = EI2(q)
                 if (g == a3208 and a3204 == 1 and a3208 < 0) or g > 0:
                     if f < 1: (*poly, code) = prev; continue          // p itself not valid: undo, keep old best
                 if g == 0: code = 0
                 if g == -1000: dist = |dist|; code = 0
        st = (s, t); Q out = Q; status = 1; *d = dist
  if *poly > 0: goto final
  if pass < 6 or (factor <= 1.0 and phase == 0):
     factor = (a044 == 0) ? factor + 0.5 : factor * 10.0
     if pass > 0: useDir = 0                                           // always true here
  else:
     if phase != 0: print "Info: No Polygon Found by Polygon Search Code", "...Object", "...Min distance",
                    "...Number of polygons" (unit 1); *d = |*d|; status = -1; return
     phase = 1
```

So the box grows as `R·1.0, 1.5, 2.0, 2.5, 3.0, 3.5` (six passes, `+ 0.5` = `DAT_140bad260`)
or by ×10 once a contact polygon was seen, then one pass over NI1(inode), then failure
**[fact]**. Note `*d` keeps the last `R` when nothing was accepted, so a later accepted polygon
must be strictly nearer than the current search radius.

### 9.3 Final sign and projection **[fact]**

With the accepted polygon (`*poly > 0`), X re-read, every node must be > 0 else
"MDSPS2_REC Fatal error" (unit 1, no stop). Then:

* quad with `conn1 == conn4` → treated as 3-node; quad: `FUN_1404d6974(X, Q, s, t)` recomputes
  Q from (s, t) (§10.3); 3-node: `FUN_1404e34eb(X, Q, s, t, 3)` (not transcribed).
* If `code ≠ 0` and `erfield > 0`: `E = ER[erfield](poly)`.
  * `code < 0` (foot at vertex `-code`): node `v = conn[-code]`, `NR1(v)`; if
    `|NR1[4]| ≤ 9.99999993922529e-9` (flat/undecided vertex): `u = unit(P − Q)`
    (`FUN_1404d68dd(Q, P)`), `c = u·NR1[1..3]`; if `c ≤ 0`: **re-search** `FUN_14006de2d`
    (§10.7) over the last box list and return; else `*d = |*d|`, return. Otherwise
    `NR1[4] > 0` → `*d = |*d|`; `< 0` → `*d = −|*d|`.
  * `code > 0` (foot on edge k): `e = E[k]`; if `|e| ≤ 0.10000000149011612` or
    `e > 6.179999828338623` → `*d = |*d|`, return. `g = |e − 3.141592653589793|`
    (`DAT_1405c81d0`); if `g ≥ 0.20000000298023224`: `e > 3.1415998935699463` → `*d = |*d|`;
    else if `e < 3.140000104904175` and `e > 0.019999999552965164` → `*d = −|*d|`; else unchanged.
    If `g < 0.2000000030`: `FUN_1400839c6` (§10.6) volume sign: < 0 → `−|*d|`, else `|*d|`.

## 10. Geometric kernels

### 10.1 Triangle `FUN_1400702c8(P, X[3][3], Q, &d, &s, &t, &mode, &found, &code)` **[fact, listing]**

1. `FUN_140066a50(X, n, &bad)` (§5.4 normal of the triangle). If `bad == 0`: ray from P along
   `−n` (`FUN_1400647a0`, §10.2) with `din = *d`. If it hits (`found ≠ 0`): if
   `|din| < |dnew|` and `mode == 1` → `found = 0`, return without outputs; else outputs
   `Q, *d = dnew, s, t`, `code = 0`, return.
2. Otherwise nearest point on the three edges `FUN_140070bf5(P, A, B, foot, r, dist)` for
   (X1,X2), (X2,X3), (X3,X1); best = edge 1, then edge 2 if `|d2| ≤ |dbest|`, then edge 3 if
   `|d3| ≤ |dbest|` (later edge wins ties). Natural coordinates with the vertex table
   `V1=(1,0), V2=(0,1), V3=(0,0)` (`DAT_1405c8300..40`), `r` = edge parameter:
   edge 1: `s = (1−r)*1.0 + r*0.0`, `t = (1−r)*0.0 + r*1.0`; edge 2: `s = 0.0`,
   `t = 1.0*(1−r) + r*0.0`; edge 3: `s = (1−r)*0.0 + r*1.0`, `t = 0.0` (products and sums kept
   as written: `(1−r)*a + r*b`).
3. If `|din| < |dbest|` and `mode == 1`: `found = 0`, return.
4. `code = edge (1,2,3)`; `|1−s| ≤ 9.99999993922529e-9` → `code = −1`; `|1−t| ≤ 1e-8` →
   `−2`; `|s + t| ≤ 1e-8` → `−3` (later tests override). `*d = dbest` (≥ 0).
   **Quirk:** `Q` = the foot point of edge `|code|`, so for a vertex code the foot of the edge
   with that number is used, not necessarily the vertex itself.
5. Sign: `v = unit(Q − P)` (`FUN_1404d68dd(P, Q)`); if `v·n > 0`: `*d = −dbest`.

### 10.2 Ray–triangle `FUN_1400647a0(P, dir, n, X, &d, &s, &t, Q, &hit, &mode, &bad)` **[fact]**

`FUN_140063ab3(P, dir, n, X1, Q, &d, &ok)`: `den = dir·n`; if `|den| > 1e-11` (`DAT_140bade80`):
`ok = 1`, `a = −(n·X1)`, `b = n·P`, `tt = (a + b)/den`, `d = −tt`, `Q = P − dir*tt` (per component
`P_c − dir_c*tt`). With `dir = −n` this gives `d = n·P − n·X1` (signed height, positive on the
normal side) and Q the foot. `ok == 0` → `FUN_140065ef8` (not followed). Mode 1: proceeds only
if `dir·n ≤ 0` and `|d| ≤ |din|`; mode 2: only if `|d| ≤ |din|`.
In-triangle test in the plane dropping the dominant normal axis: `m = argmax |n_c|` (x, then y
if `|n_y| > |n_x|`, then z if `|n_z| > max`), (j, k) = the next two axes cyclically. With
`a1 = Q_j−X1_j, a2 = Q_k−X1_k, b1 = X2_j−X1_j, b2 = X2_k−X1_k, c1 = X3_j−X1_j, c2 = X3_k−X1_k`,
`u = −10.0`:

```
if |b1| <= 1e-11: w = a1 / c1; if -1e-5 <= w <= 1.00001: u = (a2 - w*c2) / b2
else:             w = (b1*a2 - a1*b2) / (b1*c2 - c1*b2); if -1e-5 <= w <= 1.00001: u = (a1 - c1*w) / b1
inside = (u >= -1e-5) and (w >= -1e-5) and (u + w <= 1.00001)
```

(`DAT_140bade90` = −1e-5, `DAT_140bade98` = 1.00001.) Inside: clamp `u` and `w` to [0, 1]
separately, `hit = 1`, `t = u`, `s = (1.0 − u) − w`, outputs Q and d. Else `hit = 0`.

### 10.3 Edge projection and shape functions

`FUN_140070bf5(P, A, B, F, r, dist)`: `e = P − A`, `f = B − A`, `le = |e|`, `lf = |f|`; if
`le ≤ 9.9999998245167e-14` (`DAT_140bae1c8`): `F = P`, `r = 0`, `dist = 0`, return. If
`lf ≤ 9.9999998e-14`: message (unit 1, format `1005(3F15.7)` with A) and `lf = 9.9999998e-14`.
`e /= le`, `f /= lf`; `c = e·f`; (`sqrt(max(0, 1−c²))` computed, unused); `r = (c*le)/lf`,
`r = min(r, 1.0)`, `r = max(r, 0)` (`r ≤ 0 → 0`); `k = r*lf`; `F_c = k*f_c + A_c`;
`dist = |F − P|` via `FUN_14007a47d` (`sqrt` only if the square ≠ 0) **[fact, listing]**.

`FUN_1404d6974(X[4][3], Q, s, t)` bilinear map **[fact, listing]**:
`N1 = ((1−s)*(1−t))*0.25`, `N2 = 0.25*((s+1)*(1−t))`, `N3 = ((s+1)*(t+1))*0.25`,
`N4 = ((t+1)*(1−s))*0.25`; `Q_c = (((0.0 + X1c*N1) + X2c*N2) + X3c*N3) + X4c*N4` (the sum starts
from +0.0, so a −0.0 result becomes +0.0). Corner table `DAT_140643a90`: (−1,−1), (1,−1),
(1,1), (−1,1).

### 10.4 Quadrilateral `FUN_1404d203b(X, P, &s, &t, Q, &dist, &flag)` (FTN_geoutl)

Structure **[fact]**, inner routines summarised:

1. `FUN_1404d4a0f`: degenerate test `FUN_1404e33b7(X1, X4)`; local frame
   (`FUN_1404d3d43` for triangles, `FUN_1404cccfb` else, with `s = t = 0`); Q0 = N(0,0)·X;
   `FUN_1404cf5c0` + `FUN_1404d0939` map the 4 nodes into the frame; the quad is **planar** when
   `|z_k − z_1| ≤ 1e-6` (`DAT_140bb2680`) for k = 2..4. P is mapped; `FUN_1404cc289`
   point-in-polygon in 2D; inside → `FUN_1404ccf58` inverse bilinear map gives (s, t). For a
   planar quad: inside → `Q = N(s,t)·X`, `dist = |P − Q|` (`FUN_1404d68dd(P, Q)`), outside →
   `FUN_1404d41f8` (nearest edge/corner). Returns "non-planar" otherwise.
2. Non-planar: Newton on the bilinear surface. Coefficients per axis (i = 1..3) from the corner
   coordinates `x1..x4` (decompile lines 41-60): `a = ((x4 + (x1+x2)) + x3) − P_i*4.0`… with the
   ten accumulated products; the listing order of these sums was not re-derived (open item 1).
   `FUN_1404d5a1d` iterates: `s += λ·Δs`, `t += λ·Δt` (λ = line-search step from `FUN_1404d6647`,
   start 1.0), 2×2 solve by Cramer with determinant `D = a00*a11 − a01*a10` (`D == 0` →
   `FUN_1404edb2f` error), stop when `sqrt(Δt² + Δs²) < 1e-4` (`DAT_140643ba0`), give up after
   31 iterations or `λ < 0.0010000000474974513` (`flag = 1`).
3. Converged and `−1 ≤ s,t ≤ 1` → `FUN_1404d6974` + `dist = |P − Q|`. Otherwise
   `FUN_1404d275e` (edge/corner fall-back), and for its codes ≥ 10000 a triangle split
   (`FUN_1404e2914`, `FUN_1404ddeba`).
4. Always ends with `FUN_1404d3e13(s, t, &code)` (result discarded inside; MDSPS2 calls it again).

`dist` is unsigned; MDSPS2 applies the sign from `(P − Q)·N`.

### 10.5 Inside code `FUN_1404d3e13(s, t, &code)` **[fact, listing]**

`e = 0.001` (`DAT_140643a80`). `code = 0` when `−0.999 < s < 0.999` and `−0.999 < t < 0.999`
(bounds `−1 + e` and `1 − e`, computed as `e + (−1.0)` and `1.0 − e`). Otherwise for edges
k = 1..4 (corners k and k+1 of the table, edge 4 = corner 4→1): along the edge's constant
coordinate the band is `[1−e, 1+e]` for +1 and `[−1−e, −1+e]` for −1, along the other coordinate
`[−1−e, 1+e]`; count the edges whose band contains (s, t) (inclusive); exactly one → `code = k`.
Zero or several → nearest corner: `code = −k` for the corner with the smallest
`sqrt((t − t_k)² + (s − s_k)²)`, start value 2.0, strict `<` (so a point ≥ 2 away from every
corner keeps code 0).

### 10.6 Flat-edge sign `FUN_1400839c6(P, X, &nv, &edge, &sgn)` **[fact]**

Triangle: the three vertices; quad: centroid `0.25*Σ` (sums in vertex order), vertex `edge` and
vertex `edge+1` (4→1). `V = FUN_1404ca2cb(points, P)` (tetrahedron volume, not transcribed):
`|V| ≤ 1e-10` (`DAT_1405c8890`) → `sgn = 0`; `V < −1e-10` → −1; else 1.

### 10.7 Vertex re-search `FUN_14006de2d` **[fact]**

Among NI1 of the vertex node, the polygon with the largest ER2 having EI2 ≠ 0 (strict `>` from
0); its centroid C (plain average of 3 or 4 nodes, sums in node order); `u = (C − P)/|C − P|`
(return if `|C−P| ≤ 0`). For every other polygon of the last box list (consecutive duplicates
skipped, EI2 ≠ 0): degenerate area (`ER2/Amax < 9.999999747378752e-6` or
`ER2 < 9.99999993922529e-9`) recomputes the normal; ray from P along u
(`FUN_1400647a0` triangle / `FUN_140064f58` quad, mode 2, `din = |C−P|`); the hit with
`0 ≤ d ≤ |C−P|` and the smallest d wins (strict). Default: the largest polygon. Sign:
`u·ER1(winner) > 0` → `*d = −|*d|`, else `*d = |*d|`.

### 10.8 Boxes **[fact, listing]**

`FUN_1404d1b1c(X, box, &margin, &n, idx)`: start (1e10, −1e10) per axis, min with `<`, max with
`>` over the n listed points (`idx[1..n]`), then `min −= margin`, `max += margin`.
`FUN_1404d1ed7(A, B, &ov)`: `ov = 1` unless one of `B.min_c > A.max_c` or `A.min_c > B.max_c`
(inclusive overlap; NaN compares as overlapping). The decompile's `||`/`&&` nesting is wrong
here; the listing is six independent `>` tests.

## 11. Optional working-directory files

### 11.1 SRCNT2.DAT (`FUN_14002cdee`) **[fact]**

`OPEN(UNIT=92, FILE='SRCNT2.DAT', STATUS='OLD', ERR=…)` at the first SRCNT2 call of the process
(`DAT_1405c66b0` set to 1 afterwards, whether the file exists or not). If it opens: one
list-directed `READ(92,*) N1, N2` (INTEGER*4); on success (iostat not 1 or 2) `N1 →
DAT_1405c66b4` and `N2 → DAT_1405c66b8` (defaults 5 and 20 in the data section), then
`CLOSE(92)`. N1 is the maximum number of subdivisions of a partner polygon in `FUN_14002de5d`
(step `0.3333333333/(N1+1)`, `DAT_140bad4c0`), N2 caps a point count there. Effective only when
SRCNT2 reaches `FUN_14002de5d` (partners already carrying contact codes, §7.3), so for a plain
`INICTC`/`GENCTC` the file has no visible effect **[inferred from §7.2]**.

### 11.2 SKIP_PNTR.DAT (`FUN_14002f091`) **[fact]**

`OPEN(UNIT=61, FILE='SKIP_PNTR.DAT', STATUS='OLD', ERR=…)` on every SRCNT4 call. When the open
fails (file absent) SRCNT4 runs; when it succeeds SRCNT4 is skipped entirely (and unit 61 is left
open; no CLOSE on that path). Contents are never read.

### 11.3 DIEGEO.DAT → DIE_00k.GEO (`FUN_14003b304`, `FUN_14003cd08`, `FUN_14002b89c`) **[fact]**

Only when step 1 of §7.2 found no negative partner node. `OPEN(UNIT=23, FILE='DIEGEO.DAT',
STATUS='OLD', ERR=…)`, immediately `CLOSE(23)`; contents are never read. If the open succeeded,
for every partner k the file name buffer `DIE_000.GEO` (static, `s_DIE_000_GEO_1405c68c0`) gets
k written into it by an internal WRITE (`I1` at offset 6 for k < 10, `I2` at offset 5 for
k < 100; for k ≥ 10 the offset-5 digits are reset afterwards; k ≥ 100 keeps the previous name),
so partners 1 and 3 give `DIE_001.GEO` and `DIE_003.GEO`. `FUN_14002b89c` opens unit 81
(`STATUS='UNKNOWN'`), rewinds, and writes:

```
1000(I8)            number of points
1001(I6,3F16.7)     per point: i, x, y, z           (cube coordinates, i.e. the die's current points)
1000(I8)            number of polygons
1002(5I8)           per polygon: j, n1, n2, n3, n4  (n4 = n1 for 3-node polygons; n4 = -|n4| when EI2 != 1)
```

(The `IN OUTDIE` debug line needs the debug polygon argument, 0 here.) The files are written
before the per-node decisions, so they show the dies, not the moved slave.

Oracle check (`s3d_diegeo_dat`): `DIE_001.GEO` and `DIE_003.GEO` (5424 bytes each, 112 lines:
`      56`, 56 point lines such as `     1       0.0000000       0.0000000      15.0009000`,
`      54`, 54 polygon lines such as `       1       1       2       6      -5`) appear next to
OUT.KEY, which is identical to `s3d_gap_in`. The points are the die's current (lifted)
coordinates; `n4 < 0` marks the polygons disabled by the symmetry planes (§7.2 step 4).

## 12. Messages of this part

Unit 1 = NEWEQ.MSG (deleted at exit, GENCTC.md §5.5): "Info: Critical Corner Node Idenfied",
"Info - Multiple patches detected", "Warning -- the area of Polygon () is Zero!",
"ERROR in Topology", "Potential Problem at node", "Check Connectivity around Node",
"Info: Folding Node Identified", "Polygon is set to invisible", "Info: Contact Between Object
Pair … has been Established", "Info: No Polygon Found by Polygon Search Code", "MDSPS2_REC Fatal
error", the 1005 edge warning. Unit 6 (stdout): `SRCNT2` diagnostic, "Interference detected,
KOBJ=", "Increase NDATA sz from 7000:" (+STOP), "Error: Local Array size exceeded in ND2ELM"
(+STOP), the SRCNT4 "In0/In1/In2- SRCNT4" and "After SRCNT4/5/SYMDIE4" lines (debug flag only).
C library fatal errors (`FUN_14039d450`) print the message and "The above error should not have
happened" to stdout (`__iob_func()+0x30`) and call `exit(1)` **[fact]**. A plain GENCTC on the
UPSET seeds prints none of the unit-6 lines (confirmed by the oracle for every case with
`tol` < 15). **(corrected after oracle run)** With `tol` ≥ the gap between two partners
(`d3d_two_masters`, `GENCTC 16`, dies 15 apart) partner–partner contact (§7.4) gives the die
points contact codes, SRCNT2/SRCNT4 then act and stdout shows ` Interference detected, KOBJ=  1`,
` Interference detected, KOBJ=  3`, ` Info: Checking Invisible definition` (unit 6 is
unbuffered: in a captured transcript these lines come before the C++ output of the same
command file). Afterwards 51 of die 1's and 42 of die 3's 54 polygons have EI2 ≠ 1
(`v_two_masters_geo`); with `SKIP_PNTR.DAT` no unit-6 line is printed and die 1's y = 36 face
stays valid (`v_two_masters_skip`), so the bulk of the disabling comes from SRCNT2
(`FUN_14002de5d`, summarised) and the rest from SRCNT4; `SRCNT2.DAT` = `1 1` gives the same
result as the defaults (`v_two_masters_srcnt2`). NEWEQ.MSG (unit 1) is never left in the
working directory.

## 13. Interface to the decision routine

What `FUN_14005190e` / `FUN_140040d77` find when they start (node loop of §2.2 step 8):

| Item | Content when the decision starts |
|---|---|
| cubes | one per object (instance 1), §5; coordinates aliased to the object arrays |
| slave O nodes | NI1, NI2, NI3, NI5 (§6.2), NR1 (normal + convexity), NR2 (nodal area), NR5; NI4 not set (the decision re-initialises it); NI7 = none for O |
| slave O polygons | EI1, EI2 (1 / 0 / −1000; 0 also for common positive BC code, −k from SRCNT5 only with old codes), EI3, EI4, ER1, ER2, ER3 |
| partners | NI1-NI3, NR1 (NR1[4] = 0 at nodes of non-valid polygons), NR2, NR5, NI4 = 0 (unless partner–partner contact), NI7 = (1,0) or (2,k); polygons EI1, EI2 (1 / 0 / −1000), ER1-ER3 |
| COMMON | point/polygon counts and boxes per (object, instance) §3.2, `DAT_140767500[O] = sqrt(min NR2 of O)`, partner list `DAT_140771918`/`DAT_14077867c`, instances `DAT_140778684` |
| tolerance | the driver's `tol` (13th argument of `FUN_1400512aa`); SRCNT's `tol'` (`DAT_140655ac8`) is only used inside SRCNT |
| search kernel | MDSPS2_REC (§9) with outputs `st` (natural coordinates: quad (s,t) ∈ [−1,1]², triangle (weight of vertex 1, weight of vertex 2)), `poly` (> 0 or 0), signed `d` (> 0 outside along the polygon normal, < 0 inside), `status` (1 found / 0 / −1 failed), `Q` (projected point, from the shape functions for quads) |

Values the decision routine reads that no stage of this part writes: NR4 and NI4 of slave nodes
(both initialised per node by the decision's `FUN_14004946f`; `FUN_14004e10c` there also rewrites
NR1[1..3] for some nodes), `DAT_140772728` (0), `DAT_14075bc70` (0) **[fact, xref]**. The
decision routine's own view of these inputs is GENCTC_3d_decide.md §3.

## 14. Helper table

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_1400512aa` | pipeline | 611 | read fully |
| `FUN_1401a7710`, `FUN_1401a7980`, `FUN_1401a7a30` | cube per object | 611/158/227 | read fully |
| `FUN_14004f56b` | master surface build | 3180 | read fully |
| `FUN_140225640/5820/5af0/5cd0/5ff0` | cube table create/clean | | read |
| `FUN_140226150` | cfnCUBE_BuildBucket | 5395 | read fully, listing |
| `FUN_1403fed00`, `FUN_1403ff890`, `FUN_1403ff970` | locator init/divisions | 440/211/1916 | read fully |
| `FUN_140400330`, `FUN_140401050`, `FUN_140240d80`, `FUN_14023aa90`, `FUN_140233ef0` | point/polygon insertion, polygon normal | | read fully |
| `FUN_140454110`, `FUN_1404537f0` (+`140453010…`) | polygon–box overlap | | not followed |
| `FUN_14022d770` | nearest point | 4927 | read fully |
| `FUN_14022fcd0` | initial radius | 4223 | read fully |
| `FUN_140228c60`, `FUN_14022a5c0`, `FUN_140240b40`, `FUN_1404040c0` | box query, unique list | | read (1404040c0 not followed; unused with exact=0) |
| `FUN_140227e00/7fc0/80d0/8260/83b0/8540/8690/8830/8980/8b10`, `FUN_1404045e0…140404f70`, `FUN_140405xxx`, `FUN_140406xxx` | cube accessors, cell/point containers | | read |
| `FUN_14005110a`, `FUN_1404d1d05` | counts, box | 218/466 | read fully |
| `FUN_1400407e7`, `FUN_140066a50`, `FUN_140066ff1`, `FUN_140075036`, `FUN_1404d68dd`, `FUN_14004f363`, `FUN_140062fb2` | normal, area, vectors | | read fully, listing |
| `FUN_140029d47`, `FUN_14003c506`, `FUN_14002cc15`, `FUN_140088e99` | validity flags | | read fully |
| `FUN_14007c697` (+`FUN_14007e28d`) | NI1 | 2942 | read (bins helper summarised) |
| `FUN_1400798bf`, `FUN_14007aa22` | edges, neighbours | 3006/1992 | read fully |
| `FUN_14007b1ea`, `FUN_14007c24c` | patches | 4194 | summarised |
| `FUN_1400610f2`, `FUN_140062fff`, `FUN_14007ea01` | dihedral angles | | read (frame helpers `1404cf5c0`, `1404d0939` not followed) |
| `FUN_14005815c`, `FUN_14005a6bf` | node normals, NI2 | 4396 | read fully |
| `FUN_14007d97c`, `FUN_14005bbf2`, `FUN_14005d848`, `FUN_1400600bb`, `FUN_14005fe15`, `FUN_140060038` | folding, curves | | summarised |
| `FUN_14005c3a7`, `FUN_14003a580`, `FUN_14003a978` | EI3, code lists, common code | 4264/1016/645 | read (14005c3a7 edge rule summarised) |
| `FUN_140052f49` | NI5 corner nodes | 4503 | read fully |
| `FUN_14003b304` | SRCNT | 4610 | read fully |
| `FUN_14003b130`, `FUN_140232090`, `FUN_14003b0c6` | SRCNT set-up | | read fully |
| `FUN_14002a857`, `FUN_14003d2ee`, `FUN_14003dabe`, `FUN_14005d44f` | symmetry | | summarised (inactive) |
| `FUN_14003e7f6` | partner–partner contact | 3206 | read fully |
| `FUN_14002cdee`, `FUN_14002de5d` | SRCNT2 | 4207/3599 | 14002cdee read, 14002de5d summarised |
| `FUN_14002f091` (+`140032ce4`, `140032a3e`, `140033d76`, `14003329f`, `140034138`, `140030ae1`) | SRCNT4 | 6164 | 14002f091 read, callees summarised |
| `FUN_140030d0e` | SRCNT5 | 2564 | read fully |
| `FUN_14003cd08`, `FUN_14002b89c`, `FUN_14003c742` | DIE_00k.GEO, debug print | | read (14003c742 debug only) |
| `FUN_14003fadc`, `FUN_1400501d7`, `FUN_14008b65b` | partner feature nodes | 3339/2802 | read (sort helper not followed) |
| `FUN_14006798b` | MDSPS2_REC | 12455 | read fully, listing |
| `FUN_14006e938` | candidate reorder | 1480 | read fully |
| `FUN_14006de2d` | vertex re-search | 2827 | read fully |
| `FUN_1400702c8`, `FUN_1400647a0`, `FUN_140063ab3`, `FUN_140070bf5`, `FUN_14007a47d` | triangle kernels | | read fully, listing |
| `FUN_140065ef8`, `FUN_140064f58`, `FUN_1400653e5`, `FUN_14007f8c4`, `FUN_14007ff21`, `FUN_140066698`, `FUN_140066902` | ray fall-backs, ray–quad | | not followed |
| `FUN_1400839c6`, `FUN_1404ca2cb` | volume sign | 1127/214 | 1400839c6 read, volume not followed |
| `FUN_1404d203b`, `FUN_1404d4a0f`, `FUN_1404d5a1d`, `FUN_1404d6974`, `FUN_1404d3e13` | quad projection | | read (5a1d sums not re-ordered) |
| `FUN_1404d275e`, `FUN_1404d41f8`, `FUN_1404e2914`, `FUN_1404ddeba`, `FUN_1404cc289`, `FUN_1404ccf58`, `FUN_1404cccfb`, `FUN_1404d3d43`, `FUN_1404cf5c0`, `FUN_1404d0939`, `FUN_1404e33b7`, `FUN_1404e34eb`, `FUN_1404d6647`, `FUN_1404d3094`, `FUN_1404d37d7`, `FUN_1404d6b13`, `FUN_1404e29fa`, `FUN_1404e2c31`, `FUN_1404e2d44`, `FUN_1404e305a`, `FUN_1404cd2e1`, `FUN_1404ce05b`, `FUN_1404cce5e`, `FUN_1404d064d`, `FUN_1404ded0c` | FTN_geoutl quad helpers | ≈35 KB | not followed |
| `FUN_1404d1b1c`, `FUN_1404d1ed7` | boxes | 489/156 | read fully, listing |
| `FUN_14039d450`, `FUN_14039d3a0` | fatal / warning print | | read |
| `FUN_140484644` sqrt, `FUN_140484558` fabs, `FUN_140483f60` pow, `FUN_140486e8c` ceil, `FUN_1404b3878` atan2 | math | | identified |

Followed: about 120 routines (≈95 read fully, the rest summarised); ≈45 listed as not followed.

## 15. Open items

1. **FTN_geoutl quadrilateral projection** (`FUN_1404d203b` fall-backs and the Newton set-up,
   ≈30 routines, 35 KB): the port should transcribe them from the decompile, but every 3-term
   sum must be re-ordered from the listing (§1). `FUN_1404d5a1d`'s coefficient sums and
   `FUN_1404d4a0f`'s frame were not re-derived. Brick seeds (quad faces) depend on it; the test
   `s3d_on_edge` targets its edge logic.
2. Ray–quad `FUN_140064f58` and the degenerate ray fall-back `FUN_140065ef8` (used only by the
   vertex re-search and for zero-normal triangles).
3. `FUN_14007ea01`: the decompile drops the `atan2` arguments; assumed `atan2(s, c)`. Check the
   listing before porting. PathScale `atan2` must match bit for bit (GENCTC.md §9 item 3).
4. Polygon–box overlap `FUN_140454110`/`FUN_1404537f0` (affects which cells hold a large
   polygon; a miss changes the candidate list only when the polygon does not overlap the cell
   by the test but the point search would need it).
5. Topology helpers `FUN_14007b1ea`, `FUN_14007d97c`, `FUN_14005bbf2` and children: only their
   effect on EI1/EI2/EI4 is stated; they matter for non-manifold or folded surfaces.
6. `FUN_14005c3a7` EI3 rule (1 vs 2) given in outline; NI5 depends on it.
7. `FUN_1404b5054` (Fortran OPEN) return 1 is read as "ERR= taken (file missing)"; consistent
   across the three files, not verified inside the runtime.
   (Oracle: consistent — DIEGEO.DAT / SKIP_PNTR.DAT presence changes the outcome as specified.)
8. (added after oracle run) Partner–partner contact case (`tol` ≥ die gap): SRCNT2's
   `FUN_14002de5d` and SRCNT4's callees decide which partner polygons are disabled; they are only
   summarised, so `d3d_two_masters` / `v_two_masters_skip` cannot be predicted from this spec.
   Corpus GENCTC tolerances are far below die gaps, so this matters only for unusual input.

## 16. Note for the parent (GENCTC.md §5)

* §5.3 table can link here: stage roles are §2.2; replace "polygon/element data of O" by
  "EI3 BC edge classes", "node-polygon links" by "NI3/EI2 validity", "surface normals" by
  "EI2 = 0 for polygons with a common positive BC code"; step 6 is "NR1[4] reset", step 7
  "partner feature nodes (NI7)", and MDSPS2_REC is called from step 8, not from step 7.
* §3: "min positive node real value #2" is the nodal area NR2 (§7.1), and the condition is
  `tol ≤ 1.000000013351432e-10` on `tol'` only inside SRCNT; the decision gets the driver tol.
* §5.5 optional files: replace with a link to §11 (DIEGEO.DAT gives `DIE_001.GEO`,
  `DIE_003.GEO`…, not `DIE_000.GEO`; SKIP_PNTR.DAT disables SRCNT4; SRCNT2.DAT is
  `READ(*) N1, N2`, defaults 5/20, read once per process).
* §9 open item 1 is now split into §15 items 1-6 here plus the decision-routine items of the
  ctc3d-decide file.
* Tests: `tests/commands/GENCTC/cases_s3d.json` (10 cases `s3d_*.KEY`, data files `s3d_*.DAT`).

## Quad projection library (ported)

The quadrilateral projection routines this spec left open (`FUN_1404d203b` family) are now ported
bit-exact in `port/defrt` (`#include <defrt/ftn/geoutl_proj.hpp>`, namespace `def::rt::geoutl`,
bodies in `src/ftn/geoutl_proj.cpp`; `fun_1404d41f8` in `src/ftn/geoutl_d41f8.cpp` by p67-3).
Each routine was compared with the original in a mapped copy of the exe over 300k calls, NaN
payloads included, alone and end to end: 0 mismatches. Status per routine:
`docs/re/status/geoutl.md`.

| Routine | What it does |
|---|---|
| `fun_1404d203b` | entry: project P(3) on the quad X(3,4); returns (s, t), Q = N(s,t)·X and \|P − Q\| |
| `fun_1404d4a0f` | planar quads: projection in the frame of the quad normal at (0,0) (triangle normal for degenerate quads) |
| `fun_1404ddeba` | curved quads: (s, t) of P by the bilinear inverse map in the quad's frame (Newton set-up, uses the bilinear solver of `geoutl_poly`) |
| `fun_1404d275e` | fall-back when (s, t) lands outside: nearest point of P on the quad's four triangles |
| `fun_1404d41f8` | PNTEDG, last fall-back: nearest edge or corner (owned by p67-3) |
| `fun_1404d3e13` | location code of (s, t) in a quad: 0 inside (−0.999, 0.999)², k = 1..4 on edge k, corners |
| `fun_1404e37c4` | location code of triangle coordinates (s, t, 1 − s − t): −i near vertex i |
| `fun_1404d1b1c` / `fun_1404d1d05` | bounding box (min xyz, max xyz) of indexed / leading points, widened by a margin |
| `fun_1404d1ed7` | overlap of two boxes (closed intervals; a NaN bound counts as overlap) |
| `fun_1404d93ae` | bubble sort of the columns of a(ld, n) by one row, ascending or descending (whole columns swapped) |

The projection tolerances come from COMMON `140b84720` (`def::rt::geoutl::g_common_140b84720`,
members `u08` at `140b84728` and `u10` at `140b84730`), the single copy shared with defmodel and
the drop solver. PNTEDG's failure message (reached only when every edge distance is NaN or
>= 1e10) is still a `DEF_NOT_PORTED` stub in `geoutl_pending_d41f8.cpp`.

### Algorithm of `fun_1404d203b` (from the listings)

1. `*flag = 0`; `fun_1404d4a0f(X, P, s, t, dist, &nonplanar)`: normal N at (0,0) (`1404cccfb` with
   s = t = 0; for X(:,1) = X(:,4), `1404e33b7`, the triangle normal `1404d3d43`, and then the
   *incoming* s, t are used for the origin), origin Q0 = N(s,t)·X, frame R of N (`1404cf5c0`), the
   corners and P mapped into it (`1404d0939`). Non-planar when some |z_k − z_1| > 1e-6 (a NaN
   difference counts as planar). Point in polygon (`1404cc289`, bound 1e10); inside → (s, t) by
   the inverse bilinear map `1404ccf58`. Planar: inside → Q, dist = |P − Q| (`1404d68dd`), outside
   → PNTEDG `1404d41f8`. **Planar quads return here with Q = N(s,t)·X and no location code.**
2. Non-planar: per axis i, with corners x1..x4 and p = P_i:
   `a = ((x3 + (x1 + x2)) + x4) − p·4`, `b = ((x2 − x1) + x3) − x4`, `c = (x3 − (x1 + x2)) + x4`,
   `d = ((x1 − x2) + x3) − x4`; the two equations accumulate (each `+= ` adds the new product
   first): c1 = (d·d, d·b, (c·2)·d, c·c, (a·d + c1₅) + b·c, a·c),
   c2 = (d·d, c·d, (b·2)·d, (a·d + c2₄) + b·c, b·b, a·b). Newton `1404d5a1d(c1, c2, s, t, X, P)`
   starts from the (s, t) left by step 1.
3. Converged: (s, t) in [−1, 1]² (a NaN counts as inside) → Q = N(s,t)·X, dist = |Q − P|; else
   PNTEDG. Not converged: `1404d275e` — centroid C = Σ 0.25·X_k (from 0.0, corner order); for
   k = 1..4 the triangle (X_k, X_k+1, C) by `1404d3094`; the first degenerate one (code 10000)
   returns code k·10000; the nearest by |dist| (start 1e5, strict) gives Q, dist, code; if P lies
   in a triangle (code 0, dist 0) its normal is the frame direction, else (Q − P)/|Q − P|
   (triangle normal of the last triangle when |Q − P| < 1e-12); codes ±1 and +2 (a quad edge or
   corner) return here. Otherwise dist ×= 1.5 and the corners in the frame at P along that
   direction go to `1404ccf58` with the point (0, 0): each root's N(s,t)·X nearer than dist
   (strict, |.|) replaces Q, dist, s, t.
4. Code ≥ 10000: the three other corners form a triangle, `1404e2914` projects P (sets *flag),
   `1404ddeba(X, Q, st)` maps Q back to (s, t) in the quad frame at (0,0) (its ierr is not read);
   *flag ≠ 0 returns at once. Then the same [−1, 1]² test (and code ≠ 1) chooses Q = N(s,t)·X with
   dist = |Q − P| or PNTEDG, and `1404d3e13(s, t)` stores the location code in its SAVE (the
   value is not returned; MDSPS2 calls it again).

`1404ddeba`'s tolerance is COMMON `140b84728`, or `140b84730 · 0.01f` when that is 0, or `0.02f`;
outside the polygon by more than it, or |z(Q) − z(P)| > 5·tol in the frame → ierr = 1.
Corrections to §10.4: the planar path ends without `1404d3e13`, and so does the triangle split
when `1404e2914` sets *flag; `1404d275e` uses corner–corner–centroid triangles, not edges; the
Newton coefficient order is the one above. The triangle-split path needs a degenerate
corner–corner–centroid triangle in a non-planar quad, which in practice only happens when
rounding makes a quad with two equal corners non-planar (coordinates around 1e6 and larger).
`1404d93ae` is a column sort (OBJPOS_interference_kernels.md "sort"), not an arc routine.
