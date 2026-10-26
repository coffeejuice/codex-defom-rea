# DIEGEO (id 158, group 2, case 0x0e)

The object's boundary geometry. It has three record forms:
* **curve geometry**: polylines or arc chains. Each run of points is stored as one curve, and
  each curve is an edge (`_DEF_EDGE` wrapping an `_OBJ_GEO2D`) of geometry 0 in the object's
  geometry list `obj+0x3998`. This form is used in 2D, and in 3D for the 2D section of an object.
* **3D surface mesh**: nodes and quadrilateral/triangular facets in the surface object
  `obj+0x3c00[0]`.
* **3D "type 3"**: curve geometry with optional header lines that carry a 4×4 placement matrix.

The topology of the curves is in TPLOGY and the matrix in CSPMTX. All three are written together
with DIEGEO.

## KEY grammar (read)

Each `%i`/`%Flf` line below is read with `fgets` + `sscanf`, so one record is one line and
commas are separators, unless the line says `fscanf`. Helpers are described in group2-helpers.md.

**2D model** (`FUN_1400015e0` true):

    DIEGEO <obj> <type:%i> <npts:%i>                      FUN_140413ee0 (rest of keyword line)
    then point lines, read by FUN_14039a200 with fscanf (free format, not line based):
      type 1:  <mark:%i> <v0> <v1> <v2>                    3 × %Flf
      type 2:  <mark:%i> <v0> <v1> <v2> <v3> <v4> <v5>     6 × %Flf

* `type` must be 1 or 2, otherwise the keyword returns **2**. Nothing more is read, so the point
  lines that follow are then parsed as keywords.
* `npts` = number of **real** points summed over all curves. A line whose `mark == -1` is a
  separator: its values are read and dropped, it does not count toward `npts`, and it ends the
  current curve. Any other `mark` value is ignored; points are stored in file order. The last real
  point also ends the current curve. Curves are numbered 0, 1, … in order.
* Two consecutive separators, or a separator first, produce a curve with 0 points.
* Values are `fscanf("%Flf")`, so a record may span lines. Commas are not accepted. The writer
  prints `v0` of type 2 as an integer, which reads back fine as a double.

**3D model** (all other dimensions). Line 0 is `FUN_140413fb0(f, &a, &b, &c, &nconv)`
(`"%i %i %i"`, conversions counted):

1. `a == 3` → **type-3 curve form**, `FUN_140399e60(model, f, obj, b, c)`:

        DIEGEO <obj> 3 <total:%i> <nhdr:%i>
        [2 lines  "%i %Flf %Flf %Flf"     read and discarded]     if nhdr == 2 or 6
        [4 lines  "%i %Flf %Flf %Flf %Flf" = matrix rows 0..3]    if nhdr == 4 or 6
        then (total - nhdr) type-1 points as in 2D (FUN_14039a200(type 1, total-nhdr))

   It requires `total ≥ 1`, `nhdr ≥ 1` and `nhdr < total`, otherwise it returns **2**. For
   `nhdr` = 1, 3 or 5 no header line is read, but `nhdr` is still subtracted from the point
   count. In each matrix line the leading index is ignored and the 4 values go to row i, columns
   0..3; missing values are 0 (FUN_140415990 zeroes its outputs).
2. else if `nconv == 2` (exactly two integers, `a b`) → **curve form**: `a` must be 1 or 2
   (otherwise **2**) and `b` = npts; points as in 2D.
3. else (3 integers, or fewer than 2) → **surface form**:

        DIEGEO <obj> <a:%i> <nnode:%i> <nelem:%i>
        nnode lines  "<i:%i> <x:%Flf> <y:%Flf> <z:%Flf>"           FUN_140415880
        if a == 1:  nelem lines  "<i:%i> <n1> <n2> <n3> <n4>"     FUN_140414200, %i×5,
                                                                  blank lines not skipped

   Indices are ignored and records are stored in order. With `a != 1` the facet array is resized
   but not read, so it stays zero. The writer always writes `a = 1`. The node loop runs over the
   **current** size of the node vector: when `nnode < 1` the vector is not resized, so the old
   count is read.

## Model storage

Curve geometry (`FUN_14039a200`):
* `geom = FUN_140197f40(obj, 0)`: entry 0 of the geometry list `obj+0x3998`. The list is
  created at object construction (`FUN_14015f0b0`); `FUN_140197e40` creates it on demand
  (type 3).
* Curve i is `FUN_1401982c0(obj, &i)`, which is edge i of `geom+0x18` →
  `FUN_14027ff20` → `_OBJ_GEO2D`. Missing curves are appended with `FUN_14027fce0(geom)`. That
  function allocates a 0x30-byte curve and a 0x38-byte edge, sets the edge id `+0x30` to
  edge count + 1, appends the edge to `geom+0x18` and sets the dirty flag `geom+0xb8 = 1`
  (`FUN_1402847d0`). Curve fields: `+0x10` type (1 or 2), `+0x14` npts, `+0x18` `double*`
  points (3 or 6 values per point, `FUN_140276090(type)`). When a curve is closed the reader sets
  `+0x14`, then `+0x10 = type`, calls vtable `+0x18` (reallocate) and copies the points.
* Clearing before the read: in 3D (curve form), when `FUN_140160d20(obj)` (geometry has ≥1 edge)
  and geometry 0 exists, `FUN_14027fa30(geom)` frees all faces, loops, edges and vertices. In
  type-3 form the geometry is always cleared and one curve is pre-created. **In 2D nothing is
  cleared**: curves 0..k-1 are overwritten in place and higher-numbered existing curves are kept.
* Type 2: after the read, `v5` of **every point of every curve** in geometry 0 is multiplied by
  π/180 (`DAT_140522fd0` = 0.017453292519943295). The loop uses stride 6 whatever each curve's
  type is.
* Type 3: the 4×4 matrix (row stride 0x20) is copied to `geom+0x38` (`FUN_14039aa60`).

Surface form:
* `surf = obj+0x3c00[0]` (`FUN_140198620(obj, &0)`; the array holds 100 surface pointers).
* `FUN_1402fcd90(surf, &nnode, &nelem)`:
  * resizes `surf+0x10` (vector<double>, 3 per node) and `surf+0x18` (vector<int>, 4 per
    facet), zero-filling each, but only when the count is ≥ 1;
  * callocs `surf+0x20` (one int per facet);
  * resizes `surf+0x68` (one int per facet, the POLYTP array).
  * Node count = `FUN_1400034d0` (size/3), facet count = `FUN_140003500` (size/4).
* Afterwards `obj+0x3994 = 1` (`FUN_140198600`) and the byte `obj+0x3990 = 1`
  (`FUN_1401985e0`, the "has surface" flag).

## Read errors and validation

* Line 0 EOF or blank (helper returns 1) → **2**.
* 2D: `type` ∉ {1,2} → **2**. `FUN_14039a200`: `fscanf` EOF → 2, a non-numeric token
  (`fscanf == 0`) → 3. The parser maps both to **2**.
* 3D curve form: `a` ∉ {1,2} → **2**. **The status of `FUN_14039a200` is ignored** (the case
  returns 0), so short or garbled point data is not reported.
* 3D type 3: any check in `FUN_140399e60` fails, or a header line hits EOF, or the geometry or
  curve cannot be obtained → **2**. Point-data errors (2/3) → **2**.
* 3D surface: node or facet line EOF/blank → **2**.
* No message is printed by the reader. The interpreter prints the generic Reading Error
  (see `_common.md` §1).

## KEY writer

Call sites: steps 19–20 of group2-writer-map.md. All return codes from the sub-writers are
ignored.

**Curve writer `FUN_1402aaa70(model, f, obj, N)`** (needs curve 0):
1. `FUN_140304e40(obj, 1)` drops degenerate curves: those with 0 points, type 1 with fewer than
   2 points, or type 2 with 1 point whose `(v1,v2)` equals `(v3,v4)` within 1e-12. If no curve
   is degenerate nothing changes; otherwise the degenerate ones are deleted
   (`FUN_140304e00`).
2. `type` = type of curve 0, `total` = sum of `+0x14` over all curves.
   `"%-6s %7i %7i %7i\n"` → `DIEGEO`, N, type, total.
3. For each curve: `FUN_1402763d0(curve, type)` converts it to the header type when the types
   differ (2→1 is discretised by `FUN_14032ac75`, 1→2 is fitted by `FUN_14032b34b` after
   `FUN_1402765e0` removes consecutive duplicate points; the algorithms are not documented here).
   Before every curve except the first, a separator line is written:
   * type 1: `" %7i %20.10E %20.10E %20.10E\n"` with -1, 0, 0, 0 (zeros printed as
     `0.0000000000E+000`);
   * type 2: `" %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"` with -1, 0, 0, …
   Points are numbered from 1 continuously over all curves:
   * type 1: `" %7i %20.10E %20.10E %20.10E\n"` with k, v0, v1, v2;
   * type 2: `" %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"` with k, `(int)v0`, v1, v2,
     v3, v4, v5 × 180/π (`DAT_140522fd8` = 57.29577951308232).

**Dispatcher `FUN_1402a26c0(model, f, obj, N)`**: `d = obj+0x39c0` (`FUN_140158ce0`).
* 3D model → `FUN_1402a27a0` (always).
* 2D model with `d == 3` → `FUN_1402a27a0`; otherwise → `FUN_1402aaa70`.

**Surface writer `FUN_1402a27a0(model, f, obj, N)`**:
* 3D only, and only if geometry 0 has ≥1 edge (`FUN_140159160`): `FUN_1402ab160`, which writes
  the curve DIEGEO again (`FUN_1402aaa70`), then TPLOGY (`FUN_1402a9f90`), then CSPMTX
  (`FUN_1402aaf40`) and stops at the first nonzero status.
* Then, if `FUN_140203a20(obj)` (byte `+0x3990`, nnode > 0, nelem > 0) and `obj+0x3994 > 0`:
  * `"%-6s %7i %7i %7i %7i\n"` → `DIEGEO`, N, 1, nnode, nelem;
  * nnode lines `" %7i %20.10E %20.10E %20.10E\n"` with i (1-based), x, y, z;
  * nelem lines `" %7i %7i %7i %7i %7i\n"` with i, n1..n4;
  * then POLYTP (`FUN_1402ab210`), DIEPNT (`FUN_1402ab2b0`, if `FUN_140160db0`) and DIEEDG
    (`FUN_1402ab530`, if `FUN_140160e00`). Those keywords are documented in their own specs.

**Emission in `FUN_14029e250`:**
* 3D: step 19 writes curves when `FUN_140198050(obj, 0)`. Step 20 runs when byte `+0x3990` is
  set (`FUN_140160d90`). An object with both curves and a surface therefore gets the **curve
  DIEGEO twice** (the corpus shows this, e.g. `Cogging_Multiple_Ops_Lab1/Objects/Object00001.KEY`
  lines 34 and 41). On reading, the second copy clears and replaces the first.
* 2D: step 20 runs when the geometry has ≥1 edge and curve 0 has type ≠ 0.

The header type 3 is never written: type-3 input comes back out as type 1 curves plus CSPMTX.

Round-trip notes:
* A 2D model with `obj+0x39c0 == 3` gets the surface form (`DIEGEO N 1 nn ne`), which the 2D
  reader takes as type 1 with nn points. It does not round-trip.
* Points of a curve with 0 points are not written, so the curve disappears on re-read.

Other writer: `FUN_1402a1df0` (prints a `"*\n*  Data for Object #     %i\n*\n"` banner and
writes curves inline, without degenerate removal) is a separate single-object geometry dump and is
not on the `FUN_14029e250` path.

## Unit conversion

Type-2 `v5` (an angle) is in degrees in the file and in radians in memory. Nothing else is
converted.

## Helpers

| Address | Role |
|---|---|
| `FUN_14039a200` | curve point reader (type 1/2, separators, deg→rad) |
| `FUN_140399e60` | type-3 form: header lines, matrix, then `FUN_14039a200(1, total-nhdr)` |
| `FUN_140415990` | `"%i %Flf %Flf %Flf %Flf"` line (matrix row) |
| `FUN_14039aa60` / `FUN_14026a3e0` | copy a 4×4 matrix to / from `geom+0x38` (`FUN_140220670`) |
| `FUN_140220540` | 4×4 matrix init to identity (`FUN_140220570`) |
| `FUN_140197e40` / `FUN_140197f40` | create / get the geometry list `obj+0x3998`, entry i |
| `FUN_1401982c0`, `FUN_14027ff20` | curve i of geometry 0 |
| `FUN_140198310`, `FUN_140159190` | curve (edge) count |
| `FUN_14027fce0` | append a new curve+edge (id = count+1) |
| `FUN_14027fa30` | free all topology entities of a geometry |
| `FUN_140198620` | `obj+0x3c00[i]` surface object |
| `FUN_1402fcd90` (`FUN_1402f9600`, `FUN_1402f96a0`, `FUN_1402f9970`, `FUN_1402f9a50`) | size the surface arrays |
| `FUN_1400034d0` / `FUN_140003500` / `FUN_140006410` / `FUN_140005510` | node count / facet count / node data / facet data |
| `FUN_140198600`, `FUN_1401985e0` | set `obj+0x3994` / byte `obj+0x3990` |
| `FUN_1402aaa70` | curve writer |
| `FUN_140304e40` | degenerate-curve removal |
| `FUN_1402763d0` | convert a curve to another type |
| `FUN_1402a26c0`, `FUN_1402a27a0`, `FUN_1402ab160` | dispatcher / surface writer / 3D curve+TPLOGY+CSPMTX group |
| `FUN_140160d20`, `FUN_140198050`, `FUN_140160d90`, `FUN_140203a20`, `FUN_1401985a0` | writer conditions |

## Corpus check

* 2D type 1: `2D\SI\MultipleOperations\MO2_Boolean_Oprations\Operations\Task00004\CUT_OBJECT.KEY`
  parses: `DIEGEO       4       1       5` followed by 5 lines `       1  2.5000000E+001 -5.0000000E+000  0.0000000E+000`.
* 2D type 2: `2D\EN\HeatTreat\HT_CARB1_EN.KEY` line 2792:
  `DIEGEO 2 2 1` / `1 1 -3.937E-001 -6.968E-002 1.968E+000 3.474E-001 0.0E+000`.
* 3D type 3: `3D\SI\FlowForming\Implicit_ALE\Operations\Task00001\Flowform2DGeo.KEY`:
  `DIEGEO 1 3 9 4`, 4 identity matrix rows, then 5 points.
* 3D curve + TPLOGY + CSPMTX + surface: `3D\EN\Cogging\Cogging_Multiple_Ops_Lab1\Objects\Object00001.KEY`
  (curve DIEGEO at lines 34 and 41, TPLOGY at 48, CSPMTX at 62,
  `DIEGEO 1 1 4440 8876` at 67, POLYTP at 13384).

A line-count simulation of the grammar over every DIEGEO in the corpus `*.KEY` files
(dimension taken from the 2D/3D folder) checked these forms:

| Form | Records |
|---|---:|
| 2D type 1 | 417 |
| 2D type 2 | 4 |
| 3D curve | 60 |
| 3D surface | 326 |
| 3D type 3 | 4 |

Every record ends exactly before a keyword or comment line. Separator lines (`-1`) are
handled as specified.

## Open questions

* Meaning of the type-1 third value and of the type-2 fields v0..v4. The 1e-12 test suggests
  `(v1,v2)` and `(v3,v4)` are two points; disassembling `FUN_14032ac75` would settle it.
* When the reader runs in 2D, curves beyond those read are kept, and an earlier type-2 curve
  gets its angle converted a second time. This could matter only when the same object has two
  DIEGEO keywords in 2D.
