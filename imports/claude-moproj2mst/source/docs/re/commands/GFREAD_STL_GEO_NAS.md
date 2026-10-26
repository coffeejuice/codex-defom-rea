# GFREAD in-process readers: STL, GEO (3D and 2D), NASTRAN

Porting-depth spec of the GFREAD readers that run inside DEF_PRE_64.exe v11.0.2 (no external
program). Dispatch, the extension → type table and the 2D/3D tables are in [GFREAD.md](GFREAD.md);
DXF/IGES are in [DXF_IGES_READERS.md](DXF_IGES_READERS.md). KEY writers referred to below are
specified in the group 2 keyword specs (`DIEGEO.md`, `TPLOGY.md`, `POLYTP.md`, `RZ.md`,
`ELMCON.md`, branch `claude/project-thread-j4c7t9`).

Decompiles: `deform-rea-work/out/p89/c/gf3d.c gf2d.c gfnas.c gfmisc.c` and
`out/p89f/stl/{a..f}.c`, listings `out/p89f/stl/l_*.txt` (Ghidra copy `ghidra_p89f1`).
Constants were read from the PE file. A Python model of the STL path that reproduces the
original's DIEGEO byte for byte is `tests/commands/GFREAD/data/gfread_stl_model.py`; the
synthetic inputs are made by `tests/commands/GFREAD/data/gfread_stl_geo_nas_gen.py`.

## 1. Summary

| Ext. type | Object | Reader | Store | KEY result after KFWRIT | GFREAD message |
|---|---|---|---|---|---|
| 3 STL | 3D | `FUN_140244fa0` (detect `FUN_140245b90`, ASCII `FUN_140245cc0`, binary `FUN_140245ee0`, merge + orientation) | `FUN_140246ad0` → surface 0 | `DIEGEO N 1 nn ne` (triangles as `a b c a`), `POLYTP N 0 0` | Done / Failed; **all-degenerate file → DEF_PRE exits with code 1** |
| 2 GEO | 3D | `FUN_1402484c0` (pre-check `FUN_1402488a0`) | `FUN_14024be40` → surface 0, then `FUN_1402059a0(obj,0)` | `DIEGEO N 1 nn ne` (facets verbatim), `POLYTP N 0 0` | Done / Failed |
| 2 GEO | 2D | pass 1 `FUN_1402484c0` + `FUN_14024bb30` (never changes anything); pass 2 `FUN_1402478f0(…, 4, obj)` (pre-check `FUN_140248390`), which stores via `FUN_14024bb30` and reads an optional topology (`FUN_140249700`) | curves of geometry 0 | curve `DIEGEO N t n` (+ `TPLOGY`, see §5.3) | always Done |
| 10 NAS | 3D | type scan `FUN_140250280`, then mode 1 `FUN_14024f270` / mode 2 `FUN_14024fa50` → `FUN_14024f6a0` / mode 4 `FUN_14024fe90` | mode 1 surface `FUN_140251be0`; modes 2/4 mesh `FUN_140251d60` | CTETRA: mesh keywords (`RZ`, `ELMCON N ne 4`, …) **and** a surface `DIEGEO` of the boundary faces; CHEXA: mesh keywords only; CTRIA3: `DIEGEO` (but message Failed) | Done / Failed |

All three formats are 3D-only except GEO. In a 2D object the 2D table does nothing for STL and
NAS and reports success (GFREAD.md §3).

## 2. STL (3D object)

Call: `FUN_140243a70` case 3: STL object `FUN_140244ea0` (`stl[0] = 1` → tolerant locator,
`stl[1] = 0` binary flag, counts/pointers 0), `tol = (float)loader[0] = (float)1.0E-5`
(`DAT_140522f98`), `flag = 1`; `ok = FUN_140244fa0(stl, file, &tol, &flag)`; if ok
`ok = FUN_140246ad0(stl, obj)`; scaling (`FUN_1401fffa0`) is skipped because
`loader[1] = 1.0` (`|1.0-1.0| > 1E-6` false). Destructor `FUN_140244f00`.

All STL arithmetic is **single precision** (`float`) until the final copy to doubles.

### 2.1 ASCII / binary detection `FUN_140245b90`

* `fopen(file, "rt")` (**text mode**: CRLF → LF, a byte 0x1A ends the data); open failure →
  returns 0, which `FUN_140244fa0` ignores (flag stays 0 → ASCII reader, which fails to open
  too; unreachable from GFREAD because the handler tested the file first).
* `n = fread(buf, 1, 255, f)` into a 256-byte stack buffer that is **not** zeroed or
  NUL-terminated; `fclose`.
* For i in 0..n-1: if `buf[i] > 0x7F` → binary (`stl[1] = 1`), return. Else `buf[i] = tolower`.
* Then `strstr(buf, "facet normal")`: found → ASCII (`stl[1] = 0`), else binary.
  (The search may run past byte n into stale stack bytes; for a port, search the n bytes and
  stop at the first NUL.)
* Consequences: an ASCII file with any byte > 0x7F in its first 255 bytes (e.g. UTF-8 in the
  `solid` name) is read as binary; a binary file whose first 255 bytes are all ≤ 0x7F and do
  not contain `facet normal` is binary (the default).

### 2.2 Raw lists

`FUN_140244fa0` first sets up the raw point list `stl+0x20` (`FUN_140410fd0`: count 0,
capacity 5000, growth 10000, floats ×3) and the raw cell list `stl+0x50` (`FUN_1404050b0`:
capacity 10000, growth 10000, 3 ints per cell). `FUN_1404115d0`/`FUN_1404113b0` append a point
(3 floats) and return its **1-based** index; `FUN_140405540` appends a cell. Points are never
de-duplicated here: every vertex of every facet is a new raw point.

### 2.3 ASCII reader `FUN_140245cc0`

```
f = fopen(file, "rt")             fail → fprintf(stdout, "\tError opening file %s\n", file); return 0
fgets(line, 255, f)               the "solid ..." line (a longer first line leaves its rest to fscanf)
loop:
  r = fscanf(f, "%*s %*s %f %f %f\n", &v0, &v1, &v2)      "facet normal nx ny nz" (normal discarded)
  if r == -1: break
  fgets(line, 255, f)                                     "outer loop"
  3 times: fscanf(f, "%*s %f %f %f\n", &v0, &v1, &v2)     "vertex x y z"; result ignored
           id_k = add_point(v0, v1, v2)                  values left unchanged by a failed scan
  fgets(line, 255, f); fgets(line, 255, f)                "endloop", "endfacet"
  add_cell(id_0, id_1, id_2)
fclose; return 1
```

* `v0..v2` is one float[3] reused for the normal and the three vertices; a failed conversion
  leaves the previous values.
* The keywords are not checked (`%*s`), so any word works; only the token count matters.
* **End of file**: the MSVC CRT returns -1 only if EOF comes before the first directive matches.
  For `endsolid name` and also for a bare `endsolid` the first `%*s` matches, so `fscanf`
  returns 0 and **one more facet is read**: three points equal to the last vertex read and a
  cell (k+1, k+2, k+3). The next `fscanf` returns -1. This extra facet always collapses in the
  merge and is dropped, but it counts in the raw totals of the bounds message
  (cases `gfread_stl_bigbounds`, `gfread_stl_bigbounds_noname`: "9 points and 3 triangles" for 2
  facets). A file that ends right after `endfacet` gets no extra facet.

### 2.4 Binary reader `FUN_140245ee0`

```
f = fopen(file, "rb")             fail → "\tError opening file %s\n", return 0
fread(hdr, 1, 80, f); fread(&cnt, 1, 4, f)     cnt is ignored (FUN_140246980 is an empty byte-swap hook)
while fread(rec, 48, 1, f) >= 1:               12 little-endian floats: normal, v1, v2, v3
    fread(attr, 2, 1, f)                       result ignored
    a = add_point(rec[3..5]); b = add_point(rec[6..8]); c = add_point(rec[9..11])
    add_cell(a, b, c)
fclose; return 1
```
Trailing bytes shorter than 48 are ignored; a last record without its 2 attribute bytes still
counts.

### 2.5 Bounds check

`FUN_1404116a0(points)` computes min/max per axis over the raw points (initial min =
`(float)1E29` `DAT_1405646a4`, max = `-1E29f` `DAT_1405646a8`; `if p < min: min = p`,
`if max < p: max = p`). Then for each axis `d = (float)fabs((double)(max - min))` (float
subtraction). If any `(double)d > 1.0E10` (`DAT_14051f5f0`):

```
fprintf(stdout, "\tRead file with %d points and %d triangles\n", nRawPoints, nRawCells);
fprintf(stdout, "\tBounds for data in the file\n\n\tdx : %g\n\tdy : %g\n\tdz : %g\n\n"
                "\tLimits greater than max tolerance of 1E10. This may result from\n"
                "\ta file with bad data or incorrect format. Please check the file\n"
                "\tor CAD system for output options and try using ASCII STL files\n"
                "\tto minimize these problems.\n\n");
```
**The second `fprintf` passes no arguments** (verified in the listing at `0x140245180`), so
dx/dy/dz print whatever is in the argument registers (observed: `1.#QNAN`, `2.65249e-314`,
`0`). The port cannot reproduce these values; print the real extents or a fixed text and treat
the three numbers as don't-care in comparisons. Both raw lists are freed, return 0 → Failed.
A file with no facet has min/max ±1E29, extent `inf` → this message with 0 points and 0
triangles.

### 2.6 Vertex merge (point locator)

After the check: the raw lists are trimmed (`FUN_140411360`, `FUN_1404053b0`); a merged point
list `new` (capacity nRaw/2) and the result cell list `out` (capacity nRawCells, 3 ints) are
created; locator `L` (stack, `FUN_14040ce80(L, tol)`):

| Field | Value |
|---|---|
| `dims[3]` (`L[0..2]`) | 100, 100, 100 |
| `L[4]` | 5 (bucket list growth) |
| `L[7]` | 0, the merged point count |
| `tol` `L[8]` | `(float)1E-5` |
| `tol2` `L[9]` | `tol * tol` (float) |

`FUN_14040d270(L, new, bounds, 0)` (bounds = the raw min/max of §2.5):
* per axis: `min[i]`, `max[i]` copied; if `max[i] <= min[i]`: `max[i] = min[i] + 1.0f`
  (`DAT_14053176c`), so a flat surface gets a unit range.
* `nb = 100*100*100` buckets; two calloc'd arrays of 1 000 000 int lists (only the first, at
  `L+0x58`, is used).
* `h[i] = (max[i] - min[i]) / 100.0f` (float). `minh` = smallest `h[i]` (start `1E29f`).
* `level = (int)ceil((double)tol / (double)minh)`, then `level = min(level, 100)`
  (100 = largest dim). Example: a 10-unit part, `h = 0.1`, `level = 1`.
* If `min + 1.0f == min` in float (flat surface with |coordinate| ≥ 2^24) `h` is 0, `level`
  becomes INT_MIN and the bucket index divides by zero: undefined; not reachable with sane data.

Lookup `FUN_14040d750(L, p, 0)` for each vertex (the exact-match variant `FUN_14040dc00` is
used only when `stl[0] == 0`, never here):
```
for i in 0..2:                                    exact operation order (listing 0x14040d7c1..)
    t = (double)(p[i] - min[i]) * 0.999            float subtraction, then double (DAT_140522f68 = 0.999)
    t = t / (double)(max[i] - min[i])              float subtraction, double division
    idx[i] = (int)trunc((float)t * (float)dims[i]) float multiply, CVTTSS2SI
home = idx[0] + idx[1]*100 + idx[2]*10000
for r in 0..level:
    neighbours = FUN_14040dfd0(L, idx, dims, r)
    for b in neighbours (in order):
        for id in bucket[b] (insertion order):
            q = new[id]
            d2 = ((p0-q0)*(p0-q0) + (p1-q1)*(p1-q1)) + (p2-q2)*(p2-q2)    all float (FUN_140411880)
            if d2 <= tol2: return id                   FIRST match wins, not the nearest
++count; append count to bucket[home]; append p to new; return count
```
`FUN_14040dfd0` neighbours of ring r: r = 0 → only `idx`. r > 0: for each axis
`lo = idx-r` (0 if `idx-r < 1`), `hi = min(idx+r, dims-1)`; loop x from lo..hi, then y, then z
(z innermost); keep (x,y,z) if any coordinate equals the **unclipped** `idx±r` (the shell of
the cube). The new point keeps the coordinates of the vertex that created it.

Then for each raw cell in order: map its three raw ids (vertex 0, 1, 2) through the lookup;
keep the cell only if the three merged ids are pairwise different (`FUN_140405540(out, 3, ids)`).
No area test: only repeated ids drop a triangle. Raw lists are freed.

### 2.7 DEF_STL.OUT and the orientation pass

Because `flag == 1`:
`f = fopen("DEF_STL.OUT", "wt")`; **failure → return 0** (Failed; lists leak). Otherwise the
surface checker runs on (`new`, `out`) and **changes `out`**:

```
FUN_14040e620(S, new, out, smallArea = (float)1E-9 DAT_140534cc0)   depth limit S+0xc0 = 1000
FUN_14040f4e0(S, f, 0, 1)        progress text + patches + orientation
FUN_14040e980(S, f, 1)           summary
FUN_14040e810(S); fclose(f); FUN_1401b85d0("DEF_STL.OUT")   delete
```

Orientation (`FUN_14040f4e0` → `FUN_14040fde0`, `FUN_14040f9f0`, `FUN_14040fa70`,
`FUN_14040ff60`, `FUN_140405800`) — must be ported exactly, it decides the stored node order:

* `links[v]` = cells containing node v, in cell order (`FUN_14040fde0`).
* `patch[c]` = 0 for all cells. `p = 1`. While a cell with `patch == 0` exists (lowest index s):
  `pending = []`, `depth = 0`, `visit(s, p)`; then for each entry e of `pending` (the list may
  grow while it is walked): `depth = 0`, `visit(e, p)`; `p += 1`.
* `visit(c, p)`:
  ```
  patch[c] = p; depth += 1
  if depth > 1000: pending.append(c); exceed += 1; return        (depth is NOT decremented)
  for k in 0..2:
      a = c[k]; b = c[(k+1) % 3]                                 (c re-read each time)
      if a == b: continue
      L = [n in links[a] if n != c and b in n], without duplicates, in links order
      if len(L) >= 2: invalid.append(c)                          (non-manifold edge; c may repeat)
      elif len(L) == 1 and patch[L[0]] == 0:
          n = L[0]; j = position of b in n
          if n[(j+1) % 3] != a:                                   n does not run b→a
              reversed.append(n); reverse n: swap n[0] and n[2]  (FUN_140405800)
          visit(n, p)
  depth -= 1
  ```
  The seed of each patch keeps its orientation; neighbours are flipped to agree with it. Because
  an exceeded call returns without `depth -= 1`, once the limit is hit every later call in the
  same walk is deferred to `pending`; port it literally.
* Then the area pass (`FUN_140410760`, Heron per triangle in double from float edge lengths;
  cells with area < 1E-9 are listed), an empty redundancy check (`FUN_140410750`) and patch
  boundary extraction (`FUN_140410130`/`FUN_140410260`); none of them changes `new` or `out`.

File text (deleted at once, so it is never visible; the times come from `GetTickCount` and
differ per run, so the file is not reproducible anyway). The port may skip writing it, but must
keep the fopen-failure → Failed rule if it writes it:
```
"\t...Removing extra points   \n"
"\t...Building surface links  "  "(%g ms)\n"
"\t...Finding surface patches  " "(%g ms)\n"
"\t...Calculating area "         "(%g ms)\n"
"\t...Checking for redundant polygons  " "(%g ms)\n"
"\t...Extracting patch boundaries  "     "(%g ms)\n"
"\n\tSurface information\n"
"\tNumber of points              : %d\n"   "\tNumber of polys               : %d\n"
"\tNo of surfaces                : %d\n"   "\tNo of free edges              : %d\n"
"\tPoly with invalid edges       : %d\n"   "\tPoly with invalid orientation : %d\n"
"\tPoly with small area          : %d\n"   "\tSurface area                  : %g\n"
"\tMin polygon area              : %g\n"   "\tMax polygon area              : %g\n"
"\tMin edge length               : %g\n"   "\tMax edge length               : %g\n"
"\tRecursion depth limit         : %d\n"   "\tNo. exceed max depth          : %d\n"   "\n"
[if invalid]  "\nPolygons have edges shared by more than one other polygon \n" + "%d " per id
[if reversed] "\nBadly oriented polygons which were reversed\n" + "%d " per id
[if small]    "\nPolygons with area less than %g\n" + "%d " per id
              (each list: "\n" after the 1st, 11th, 21st … id, then "\n")
"\n" "Surf patches\n" + per patch "Patch %6d  NoCurves %6d  NoPoly  %6d\n" + "\n\n"
```

### 2.8 Result of the read

`FUN_1402469a0(stl, nNew, nOut, &new[1], &out[1])`: `stl+8 = nNew`, `stl+0xc = nOut`,
`stl+0x10` = calloc'd doubles (each float widened, so 0.2f prints `2.0000000298E-001`),
`stl+0x18` = copy of the 3·nOut ints. Return 1.

**Empty result**: if no cell survives, `FUN_140405720(out, 1)` (taken for the copy) fails its
range check and calls `FUN_14039d450`: stdout gets
`"defCELL_GetCellData : 1 not valid, Max 0\n"` + `"\n"` + `"\tThe above error should not have
happened\n\tIf this error occurs repeatedly,\n\tPlease contact SFTC and report the conditions\n\tunder
which it occurs.\n"` and the process calls `exit(1)` (`FUN_14039d2d0`). DEF_STL.OUT was
already written and deleted. No OUT.KEY is written (case `gfread_stl_alldegen`). The port
must end the run the same way.

### 2.9 Store `FUN_140246ad0(stl, obj)`

* `obj+0x39c0 == 2` (2D object) → `"*** _DEF_STL::setDataToObjectGeo() ***  not in 2D\n"`, 0
  (unreachable from GFREAD).
* `stl+0x10` or `+0x18` NULL → 0 (only possible after an allocation failure).
* `FUN_140163340(obj, 1)`: in a 3D model deletes surfaces 1.. of `obj+0x3c00[]` and sets
  `obj+0x3994 = 1`; `FUN_1402009f0(obj, nn, ne, 0)` → `FUN_1402fcd90(surf0, &nn, &ne)` sizes
  the node (3 doubles), facet (4 ints), `+0x20` and POLYTP (`+0x68`, zeroed) arrays (DIEGEO.md).
* nodes: `memcpy` of nn·3 doubles; facets: for each triangle `(a, b, c, a)`.
* `obj+0x3990 = 1` (`FUN_1401985e0`), return 1. GFREAD then sets `obj+0x3a40 = 3`.
* The surface's previous POLYTP values are lost (reset to 0). Curve geometry, mesh and other
  object data are untouched.

## 3. 3D GEO

### 3.1 Pre-check `FUN_1402488a0`

`fopen "rt"`; `fgets(buf, 2000)` twice (line 1 is not looked at; if there is no line 2 the
buffer still holds line 1); `strtok(buf, " ,\t\n\r")`; true only if the first token exists,
`atol(token) == 1` and the line has exactly **4** tokens. So line 2 must be the first node,
`1 x y z`.

### 3.2 Reader `FUN_1402484c0(geo, file)` (`_DEF_AMGGEO`, fields `+0x20` nn, `+0x28` double*,
`+0x30` ne, `+0x38` int*)

```
if !precheck: return 0
f = fopen(file, "rt")
fscanf(f, "%d\n", &nn);   if nn < 1 or (double)nn >= 1.0E7 (DAT_14051f1a8): fclose, return 0
nodes = calloc(nn*3, 8)
i = 0; while i < nn:
    r = fscanf(f, "%*d %lf %lf %lf\n", &x, &y, &z)
    if r < 3: fclose, return 0                    (EOF -1 is < 3 too)
    nodes[i] = (x, y, z); i += 1
ok = 1
fscanf(f, "%d\n", &ne);   if ne < 1 or ne >= 1.0E7: ok = 0      (nodes stay read)
else: facets = calloc(ne*4, 4)
      j = 0; while j < ne:
          r = fscanf(f, "%*d %d %d %d %d\n", &n1, &n2, &n3, &n4)
          if r < 4: fclose, return 0
          facets[j] = (n1, n2, n3, n4); j += 1
fclose; return ok
```
Free format: a record may span lines; the leading index is skipped, records are stored in file
order. Values are full doubles (`%lf`). Node numbers are not checked.

### 3.3 Store `FUN_14024be40(geo, obj)`

`+0x28` or `+0x38` NULL → 0. Else exactly as STL §2.9 but the facets are copied **verbatim**
(4 ints, whatever the 4th is: `n1`, `0`, …). Returns 1. The 3D table then calls
`FUN_1402059a0(obj, 0)` whenever the read succeeded (even if the store failed).

### 3.4 Post-processing `FUN_1402059a0(obj, 0)` (planar facet groups)

Only if `obj+0x3990` is set. `surf = obj+0x3c00[0]`:
1. Clear the plane list at `surf+0x28` (`FUN_1402581b0`). (`param_2 = 0`: the copy from
   `obj+0x2e80`, `FUN_140257c80`, is skipped.)
2. `diag` = bounding-box diagonal of the surface nodes (vtable slot 9, `FUN_1402fb120`: min
   start 1E30, max start -1E30, `sqrt(Σ(max-min)²)`; 0 if no node or facet).
3. For each facet i with **4th node < 1** (triangles written as `a b c 0`; facets with
   `n4 = n1` are skipped): A, B, C = nodes n1, n2, n3;
   `n = (C-B) × (A-B)` (`FUN_14023e050`: `nx = (Cy-By)(Az-Bz) - (Cz-Bz)(Ay-By)`,
   `ny = (Cz-Bz)(Ax-Bx) - (Cx-Bx)(Az-Bz)`, `nz = (Cx-Bx)(Ay-By) - (Cy-By)(Ax-Bx)`), divided by
   its length unless the length is 0. If `|n| >= 1E-6`: search the planes in list order for one
   with `|n - P.normal|² < 0.01` (`DAT_14051f258`) and
   `fabs(P.normal · (A - P.point)) < 0.001 * diag` (`DAT_14051f1e8`). None → new plane
   (`FUN_140257e90`) with id = smallest unused id ≥ 1 (`FUN_140258260`), point A, normal n.
   `surf+0x20[i] = plane id + 600`.
4. Neither `surf+0x20` nor the plane list is written by KFWRIT (case `gfread_geo3d_small`:
   three `… 0` facets, the OUT.KEY has only DIEGEO and `POLYTP 4 0 0`). The port needs this only
   if something else reads `surf+0x20`.

## 4. NASTRAN (3D object)

Object `FUN_14024f0a0`: `mode = 1`, `npe = 4`, `nn = ne = 0`, `+0x10` vector<double> nodes,
`+0x18` vector<int> connectivity. `FUN_14024f1d0`: `if FUN_140250280(nas, file)` then run the
reader for `mode` 1, 2 or 4; **if the scan fails the function still returns 1** with no data.

### 4.1 Type scan `FUN_140250280`

`fopen "rt"`; failure → `"*** failed to open file %s  ***\n"`, return 0. Loop:
`tok = FUN_140411ad0(rdr, f, 2, buf, -5000)` (token reader, §4.6: next non-skipped line,
first token, **upper-cased**; at EOF `buf = "DONE"`), then
`CTETRA` → mode 2, npe 4; `CTRIA3` → mode 1, npe 3; `CHEXA` → mode 4, npe 8 (full-string
`strncmp(…, 80)`; the **last** matching line wins), until `buf == "DONE"` (a line whose first
token is `DONE` also ends the scan). None found →
```
"*** Supported mesh type not found           ***\n"
"*** Current supported two mesh types are    ***\n"
"*** CTRIA3 --> triangle surface mesh        ***\n"
"*** CHEXA -->  hex solid mesh               ***\n"
```
and return 0. Only the first token of a line is looked at: large-field `CTETRA*` and
`GRID*` are not recognised; lower-case `ctetra` is (upper-cased) but the readers below use
case-sensitive `strstr`, so such a file yields no elements.

### 4.2 Fixed-field readers (8-column fields)

Common frame of `FUN_14024f270` (mode 1), `FUN_14024f6a0` (mode 2 fallback) and
`FUN_14024fe90` (mode 4):
```
f = fopen(file, "rt")                       fail → 0
while fgets(line, 255, f):                  one 256-byte buffer reused for all lines
    if strstr(line, "$"): continue          '$' anywhere skips the line
    if strstr(line, "GRID"):  node line     (checked first; substring anywhere)
    elif strstr(line, ELEM):  element line  ELEM = "CTRIA3" / "CTETRA" / "CHEXA"
fclose
nn = nodes.size / 3;   ne = conn.size / {4 (mode 1), 4 (mode 2), 8 (mode 4)}
return 1
```
Field k = bytes `line[8k .. 8k+7]` copied into its own 9-byte NUL-terminated buffer, **without
regard to the line length**: bytes after the line's `\n\0` are stale data from earlier, longer
lines (a port must keep one persistent 256-byte buffer that `fgets` overwrites only up to the
NUL to get the same values; a field that starts at or after the NUL is usually `atof` 0).

| Line | Fields used | Conversion | Stored |
|---|---|---|---|
| `GRID` (all modes) | 3, 4, 5 (cols 25–48: X1 X2 X3) | mode 1: `atof` → double; modes 2/4: `(float)atof`, then widened (0.1 → `1.0000000149E-001`) | push x, y, z |
| `CTRIA3` (mode 1) | 3, 4, 5 (G1 G2 G3) | `atol` | push G1, G2, G3, then G1 again |
| `CTETRA` (mode 2) | 3, 4, 5, 6 (G1..G4) | `atol` | push 4 ids |
| `CHEXA` (mode 4) | 3..8 of the line (G1..G6), then `fgets(next, 255)` and its fields 1, 2 (cols 9–24: G7 G8) | `atol` | push 8 ids |

Number parsing is plain CRT `atof`/`atol`: NASTRAN short exponents such as `1.0-3` or `1.0+3`
parse as **1.0**, `1.0D-3` also as 1.0, `1.0E-3` correctly (case `gfread_nas_tet_fixed`: GRID
`1.0-3` → `1.0000000000E+000`). The GRID id (field 1) and CP are ignored and **nodes are not
renumbered**: they are stored in file order and element ids are used as 1-based indices into
that order, so the file must number GRIDs 1..n in order. The CHEXA continuation line is always
taken as the next physical line, whatever it contains; its marker fields are not checked.
CTETRA with more than 4 nodes (10-node) keeps only G1..G4; CTETRA/CTRIA3 continuation lines are
not read (they are skipped unless they contain `GRID`/the element name).

### 4.3 Mode 2: free-field first `FUN_14024fa50`, fallback `FUN_14024f6a0`

`FUN_14024f620`: `ok = FUN_14024fa50(nas, file)`; if 0, `ok = FUN_14024f6a0(nas, file)`.
The vectors are **not cleared** between the two: whatever the free-field pass pushed before it
failed stays and the fixed pass appends to it. In a purely fixed-field file the first GRID line
fails at once, so nothing is duplicated.

`FUN_14024fa50`, same frame (`fgets 255`, `$`, `GRID`, `CTETRA`):
* GRID: positions of all `,` in the line (an int[4] followed by other locals; more than 5
  commas overwrite neighbouring locals). Fewer than 2 commas → `fclose`, return 0. x, y, z =
  `(float)atof(line + comma[k] + 1)` for k = 2, 3, 4 (`GRID,ID,CP,X1,X2,X3`). With 2–4 commas
  the missing positions are stale.
* CTETRA: comma positions (int[10]); fewer than 2 → return 0. `npe = commas - 2`;
  ids `atol(line + comma[k] + 1)` for k = 2 .. commas-1, all pushed. `nas+4 = npe`.
* End: `nn = size/3`, `ne = conn.size / npe` (npe of the last CTETRA line).
* A trailing comma or a 10-node CTETRA gives npe ≠ 4; the store then uses element kind 4
  (brick) and copies `ne*npe` ints into an 8-per-element array: undefined. Only 4-node
  `CTETRA,EID,PID,G1,G2,G3,G4` lines are safe.

### 4.4 Mode 1 store `FUN_140251be0` (CTRIA3) — and why GFREAD says Failed

2D object → `"*** _DEF_NASTRAN::setDataToObjectGeo() *** not implemented in 2D\n"`, 0.
`mode != 1`, `nn == 0` or `ne == 0` → 0. Else like STL §2.9: surface 0 gets nn nodes (doubles,
not rounded to float) and the `(G1,G2,G3,G1)` facets, `obj+0x3990 = 1`, return 1.
The 3D table **ignores** this result and returns `FUN_140251d60`'s, which is 0 for mode 1. So a
CTRIA3 file prints `Loading geometry - Failed`, `obj+0x3a40` is not set, **but the surface is
stored and KFWRIT writes it** (case `gfread_nas_tria`).

### 4.5 Mesh store `FUN_140251d60(nas, obj, comp)` (modes 2 and 4)

`comp = FUN_140158f50(obj) + 0x15f8` (the model's per-variable component table; `comp[0]` = 3
coordinates). `mode ∉ {2,4}`, `nn == 0` or `ne == 0` → 0. Else:
* `k = (npe == 4) ? 2 : 4`; `FUN_140202700(obj, nn, ne, k, comp, 0)`:
  `obj+0x39c0` = model dimension (`FUN_140199af0`), material-group probe `FUN_1401fb270`,
  `obj+0x87c = 1`, `FUN_140165500(obj, k)` (element kind fields, ELMCON.md),
  `FUN_1401652b0(obj, nn, comp)` (node count, node block, temperatures 20.0 SI / 68.0 English),
  `FUN_1401653b0(obj, ne, comp)` (element block and defaults); if `obj+0x3980 > 0` the material
  group is applied (`FUN_1402027e0`).
* `memcpy` nn·`comp[0]` doubles to the coordinate array (`FUN_1401808d0(obj+0x8a0)`, i.e.
  `obj+0x8a8`) and ne·npe ints to the connectivity (`FUN_140183cb0(obj+0x9e0)`, `obj+0x9e8`).
* `FUN_1401fc290(obj)`: boundary build (the same routine ELMCON's reader calls; 3D boundary
  faces into `obj+0xbf8/+0xc00` by the Fortran `FL3_FNDBND` `FUN_140354ed0`). Return 1.

Then in `FUN_140243a70`:
* **mode 2 (CTETRA)**: callback `FUN_140244e60` (no callback → 1), `FUN_140251d60`, and if
  that succeeded `FUN_1401fc290(obj)` again and `FUN_1401fc730(obj, &1)`. In a 3D model
  `FUN_1401fc730` makes the boundary the object's surface: `FUN_140163340(obj, obj+0xbd8)`,
  surface 0 sized to (all nn nodes, `obj+0xbf0` faces), nodes copied from `obj+0x8a8`, faces
  (4 ints, triangles as `a b c a`) from `obj+0xbf8`, `obj+0x3990 = 1`, then
  `FUN_1402fc1b0(surf)` compacts the nodes: nodes referenced by a face are renumbered 1..m in
  ascending old index, others dropped (no change when every node is on the boundary).
* **modes 1 and 4**: `FUN_140251be0` (surface; only mode 1 stores) then `FUN_140251d60`
  decides the result. CHEXA therefore gets the mesh but **no surface DIEGEO** and no
  `FUN_1401fc730`.

### 4.6 Token reader `FUN_140411ad0(rdr, f, type, out, pos)` (used by the type scan and the 2D GEO reader)

Globals: line buffer `DAT_14070c490` (2000), token table (`DAT_14070c088` = token 1, …),
token count `DAT_14070c070`, next index `DAT_140629420`.
* `type == 12`: `fgets` one line (no skipping at all), cut at the first `\r`/`\n`, copy it to
  `out`, reset the table (count 1, next 2) → the next `pos 0` read starts a new line.
  EOF → `out = "DONE"`, -2.
* `pos == -5000` (or `pos == 0` when the current line is used up): read lines until one has a
  first token that does not start with `*`; per line: `fgets(2000)`, if `rdr[0] == 1` cut at
  column 63 (never: `rdr[0] = 0`), cut at the first `//` (except type 4), `strtok` with
  `" ,:=\n\t\r"` (so blank lines, `*` comment lines and `//` comments are skipped; commas,
  colons and `=` separate tokens). Returns the first token; with -5000 the return value is the
  token count of the line. EOF → -2 (type 2 also sets `out = "DONE"`).
* `pos == 0` with tokens left: next token of the current line, return 0. `pos > 0`: token
  `pos` of the current line or -1.
* Conversions: 2 upper-case string; 3, 4, 12 string; 5 `atol` → int; 6, 7 `atoi`-like → int;
  8 `strtoul(…,10)` → float; 9 `atof` → float after `D`/`d` → `E`; 10 `strtod` → double after
  `D`/`d` → `E` (so Fortran `1.0D-3` works, `1.0-3` reads as 1.0); 1 `atol != 0` → bool.

## 5. 2D GEO (2D object)

### 5.1 The first pass never changes anything

`FUN_140244010` case 2 runs `FUN_140246f70(g1, file, &0, 3, 0)` → the 3D reader of §3.2, and if
that succeeded `FUN_14024bac0(g1, obj, 2)` → `FUN_14024bb30`. `FUN_14024bb30` requires the
curve-start array `g1+0x10`, which only the 2D reader allocates (the constructor
`FUN_140246d70` zeroes it), so it returns 0 at once. Moreover the 3D pre-check (4 tokens on
line 2) and the 2D pre-check (2 tokens on line 2) exclude each other, so for any file at most
one of the two passes can read data. Case `gfread_geo2d_from3d`: a 3D-format GEO given to a 2D
object passes the 3D reader, changes nothing, fails the 2D pre-check, and GFREAD still prints
Done.

### 5.2 Reader `FUN_1402478f0(geo, file, &close = 0, 4, obj)`

Pre-check `FUN_140248390`: like §3.1 but true when line 2 has exactly **2** tokens (no value
test). Then `fopen "rt"`; `FUN_140246e40` resets the curve fields; `start = calloc(100,4)`,
`bc = calloc(100,4)`; `nc = 0`, `nP = 0`, `prev = -1`, `grow = 1`.

```
loop:
    if tok(INT, &type, -5000) < 1: break            new line, its first token; EOF ends
    if type == 99: topo = 1; break
    cur = type
    tok(INT, &npts, 0); tok(INT, &code, 0)           same line or following lines
    skip = !(prev < 0 or prev == cur)
    if not skip: pts = realloc(pts, (nP + npts) * (cur == 3 ? 6 : 3) doubles)
                 (with the close flag and code >= 0 and cur != 3: one more point; never here)
    for each of npts points (types 1..4 only; another type reads no points):
        if skip: tok(LINE, buf, -5000)               consumes exactly one physical line
        else:   tok(INT, &idx, -5000)                new line; idx ignored
                type 1: x, y = tok(DBL, 0) ×2; z = 0
                type 2: x, y, z = tok(DBL, 0) ×3
                type 3: v0..v4 = tok(DBL, 0) ×5; v5 = tok(DBL, 0) * π/180   (6 values/point)
                type 4: x, y, z read, then z = 0
                stored at point nP + i (stride 3, or 6 for type 3)
    if skip: cur = prev
    if code >= 0 and not skip:
        start[nc] = nP + 1; bc[nc] = code; nc += 1
        if nc > 99: grow += 1; start, bc = realloc(nc * grow ints)
        nP += npts; prev = cur
t = cur - 1;  geo+8 (curve type) = 1 if t == 0 or t == 3 else t
ok = FUN_14024bac0(geo, obj, 4)                      → FUN_14024bb30, §5.3
if topo: FUN_140249700(geo, f, obj)                  §5.4
fclose; return ok                                    (the 2D table ignores it)
```
Notes:
* File types 1, 2 and 4 become curve type 1 (3 values per point: x, y, third value); file
  type 3 becomes curve type 2 (6 values); a file type 5 or more gives curve type type-1
  without points.
* A curve with `code < 0` is read but not kept; its points are overwritten by the next curve.
* A record whose type differs from the first kept curve's type is skipped line by line, and
  `prev` stays.
* Values may wrap to the next line (each point starts on a new line through `idx`), commas,
  colons and `=` are separators, `//` starts a comment, `*` lines and blank lines are skipped
  (not inside a skipped record).
* A missing value at EOF leaves that slot as realloc left it (uninitialised); the loop then ends.
* The per-curve boundary code is stored in the curve (`+0xc`) but **no KEY keyword writes it**.

### 5.3 Store `FUN_14024bb30(geo, obj)`

`geo+0x28` (points) or `geo+0x10` NULL, or obj NULL → 0. (Zero points in total: the realloc
helper returns NULL for size 0, so nothing is stored.) Else:
`geom = FUN_140197f40(obj, 0)`, created by `FUN_140198270` if missing; `FUN_14027fa30(geom)`
deletes all topology and curves; `FUN_140198230(obj, nc)` creates nc curves; curve i gets
`+0x10 = geo+8` (type), `+0x14` = `start[i+1] - start[i]` (last: `nP - start[i] + 1`), its point
array is reallocated (vtable +0x18) and filled from point `start[i]-1` with stride 3 or 6,
`+0xc = bc[i]`. Returns 1. The 3D surface (`obj+0x3c00`) is not touched.

KEY output (DIEGEO.md, TPLOGY.md): `DIEGEO N type total` with a `-1 0 0 0` separator line
between curves; TPLOGY: with one curve and no faces the writer builds the topology itself
(existing case `gfread_2d_geo`: 2 vertices, 1 edge, 1 loop, 1 face); with more than one curve
and no topology **no TPLOGY is written** (case `gfread_geo2d_multi`); with a `99` section the
file's topology is written (case `gfread_geo2d_topo`).

### 5.4 Topology after `99` (`FUN_140249700`)

The rest of the `99` line is ignored. The remaining lines use the KEY line readers (`fgets`,
commas → spaces, `sscanf`, see common.md) and follow the TPLOGY record grammar (TPLOGY.md) with
one extra line in front:
```
<any line>                                  FUN_140413c60 (200 bytes)
<nV> <nE> <nL> <nF>                          FUN_140414100
<comment line>  then nV vertex records       "%i %i %Flf %Flf" + k edge-id lines
<comment line>  then nE edge records         "%i %i %i %i" (id, k, v1, v2) + k loop-id lines
<comment line>  then nL loop records         "%i %i %i" (id, ke, kf) + ke edge ids + kf face ids
<comment line>  then nF face records         "%i %i %i" (id, k, attr) + k loop ids
```
Storage is that of the TPLOGY reader (`FUN_140284690` clear, `FUN_1402847d0(geom, 0)`, create
nV vertices, nL loops, nF faces; records address entities by index id-1, links match by id).
Differences: every read error just returns (no status, the curves stay); the geometry must
exist (`FUN_14024ae10`: `FUN_1401983e0(obj) > 0`); edge and face indexes are not range-checked
(out of range dereferences NULL).
Full specification from the listing (where exactly the original faults, record by record):
[GEO_TOPOLOGY.md](GEO_TOPOLOGY.md).

## 6. Messages (stdout)

| Text | When |
|---|---|
| `\tError opening file %s\n` | STL reader cannot open the file (unreachable after the handler's test) |
| `\tRead file with %d points and %d triangles\n` + bounds text (§2.5) | STL extent > 1E10; dx/dy/dz undefined |
| `defCELL_GetCellData : 1 not valid, Max 0\n\n\tThe above error …\n` + exit(1) | STL with no surviving triangle |
| `*** failed to open file %s  ***\n` | NAS type scan cannot open the file |
| `*** Supported mesh type not found           ***` + 3 lines | NAS without CTETRA/CTRIA3/CHEXA |
| `*** _DEF_STL::setDataToObjectGeo() ***  not in 2D\n`, `*** _DEF_NASTRAN::setDataToObjectGeo() *** not implemented in 2D\n` | stores called for a 2D object (unreachable from GFREAD) |
| `ERROR: iBdryNo = %d, larger than NoGeoBdry = %d\n` (printf) | `FUN_1402009f0` with no surface slot (not seen) |
| `INFO   : Loading geometry - Done\n` / `- Failed\n` | handler, after every load (GFREAD.md) |

## 7. KEY output after KFWRIT (group 2 keywords of object N)

| Input | Keywords that change | Observed |
|---|---|---|
| STL | `DIEGEO N 1 nn ne` + nn `" %7i %20.10E %20.10E %20.10E"` node lines (float values widened) + ne `" %7i %7i %7i %7i %7i"` lines `i a b c a`; `POLYTP N 0 0` | `gfread_3d_stl` (corpus die, 900 raw → 152 nodes, 300 facets) and the synthetic cases match the model byte for byte |
| 3D GEO | same two keywords, facets verbatim | `gfread_geo3d_small` |
| 2D GEO | curve `DIEGEO`, `TPLOGY` (see §5.3) | `gfread_2d_geo`, `gfread_geo2d_*` |
| NAS CTETRA | surface `DIEGEO` of the boundary faces + `POLYTP N 0 0`, then the mesh block: `RZ N nn`, `DRZ`, `DRMESH`, `BCCDEF`, `BCCDFN`, `URZ`, `FRZ`, `PRZ`, `BCCTMP`, `BCCTFN`, `NDTMP N 0 2.0E+001`, `NDHEAT`, `NDFLUX`, `USRNOD`, `ELMCON N ne 4`, `MTLGRP`, `STRAIN`, `STNCMP`, `DENSTY`, `DAMAGE`, `STRESS`, `YLDS`, … `DATOM`, `HDNOBJ`, `BCCCRB`, …, `NWEAR` (the standard writers of a meshed object, all with default values) | `gfread_nas_tet_fixed`, `gfread_nas_tet_free` (identical OUT.KEY) |
| NAS CHEXA | the same mesh block with `ELMCON N 1 8`, no DIEGEO | `gfread_nas_hex` |
| NAS CTRIA3 | `DIEGEO N 1 nn ne` (`a b c a`), `POLYTP N 0 0` — although GFREAD reports Failed | `gfread_nas_tria` |
| any failure | nothing for N (a new object keeps only its defaults, e.g. `OBJTYP N 1 0`) | `gfread_stl_bigbounds`, `gfread_geo3d_*`, `gfread_nas_none` |

The boundary-face order of the CTETRA DIEGEO comes from `FL3_FNDBND`; in the 2-tet case it
is (1 3 2), (1 2 4), (1 4 3), (2 3 5), (2 5 4), (3 4 5) — outward faces, the shared face
removed, ordered by ascending node ids.

## 8. Oracle cases

`tests/commands/GFREAD/gfread_{stl,geo,nas}_*.json`, inputs in `tests/commands/GFREAD/data/`
(3D cases: `UPSET_Brick.KEY`, `GFREAD 4` = new object; 2D cases: `RING_SI.KEY`, `GFREAD 1`).
All ran on 2026-10-21 against the install.

| Case | Confirms |
|---|---|
| `gfread_stl_ascii_merge` | ASCII detection; merge with tolerance 1E-5 (P4' merged, P6 at 2E-5 kept); **first match, not nearest** (C → A); degenerate triangle dropped; inconsistent triangle **reversed** (`4 3 5` → `5 3 4`); float→double node values. DIEGEO equal to the model |
| `gfread_stl_binary_merge` | binary detection, 50-byte records, count ignored, 10 trailing bytes ignored; same DIEGEO |
| `gfread_stl_bucket_edge` | points in buckets 49 and 50 merge through the ring-1 search; flat z range handled; matches the model |
| `gfread_stl_bigbounds` | bounds message (garbage dx/dy/dz), raw counts include the stale `endsolid` facet (9 points, 3 triangles), Failed, no DIEGEO |
| `gfread_stl_bigbounds_noname` | a bare `endsolid` also makes `fscanf` return 0 (again 9 points / 3 triangles) |
| `gfread_stl_alldegen` | empty result → `defCELL_GetCellData` text and **exit code 1**, no OUT.KEY |
| `gfread_3d_stl` (existing) | corpus binary STL: 900 raw points → 152 nodes, 300 facets; the model reproduces all 453 DIEGEO lines |
| `gfread_geo3d_small` | `%lf` nodes, facets verbatim incl. 4th = 0; the plane ids of §3.4 do not reach the KEY |
| `gfread_geo3d_badcheck` | pre-check: 3 tokens on line 2 → Failed |
| `gfread_geo3d_noelem` | `ne = 0` → Failed, nothing stored |
| `gfread_geo2d_multi` | `code < 0` curve dropped, mismatched-type record skipped, comma separator, `D` exponent, `//` comment, value on the next line, `*` line skipped, type 2 → curve type 1, no TPLOGY with 2 curves |
| `gfread_geo2d_topo` | `99` section read as TPLOGY records (2 vertices, 2 edges, 1 loop, 1 face written back) |
| `gfread_geo2d_from3d` | first 2D pass changes nothing; object keeps its KEY geometry; Done |
| `gfread_nas_tet_fixed` | type scan, free-field fallback, fixed columns, `1.0-3` → 1.0, float rounding of GRID values, mesh keywords + boundary DIEGEO |
| `gfread_nas_tet_free` | free-field reader; OUT.KEY identical to the fixed case |
| `gfread_nas_hex` | CHEXA + continuation fields 2–3, mesh only, no DIEGEO |
| `gfread_nas_tria` | CTRIA3 surface stored (doubles not rounded) while the message is Failed |
| `gfread_nas_none` | "Supported mesh type not found" text, Failed |

## 9. Helpers

| Address | Role |
|---|---|
| `FUN_140244ea0` / `FUN_140244f00` | STL object ctor (`stl[0] = 1`) / dtor |
| `FUN_140244fa0` | STL read: detect, read, bounds, merge, DEF_STL.OUT, result copy |
| `FUN_140245b90` | ASCII/binary detection |
| `FUN_140245cc0`, `FUN_140245ee0` | ASCII / binary readers |
| `FUN_140246980` | byte-swap hook (empty) |
| `FUN_140410fd0`, `FUN_1404113b0`/`FUN_1404115d0`, `FUN_140411300`, `FUN_140411360`, `FUN_140411640`, `FUN_140411620`, `FUN_140411080` | float point list: init, append (1-based id), grow, trim, get, count, free |
| `FUN_1404116a0` / `FUN_140411850` | point-list bounds (min/max per axis) |
| `FUN_1404050b0`, `FUN_140405540`, `FUN_140405350`, `FUN_1404053b0`, `FUN_140405720`, `FUN_1404056c0`, `FUN_140405150`, `FUN_140405800` | int cell list: init, append, grow, trim, get (range check → fatal), count, free, reverse |
| `FUN_14039d450`, `FUN_14039d570`, `FUN_14039d2d0` | fatal error: message + "The above error…" + `exit(1)` |
| `FUN_14040ce80`, `FUN_14040d270`, `FUN_14040d750`, `FUN_14040dc00`, `FUN_14040dfd0`, `FUN_140411880`, `FUN_14040cf80` | point locator: ctor, init buckets, tolerant insert, exact insert (unused), ring neighbours, float distance², dtor |
| `FUN_14040e620`, `FUN_14040f4e0`, `FUN_14040fde0`, `FUN_14040f9f0`, `FUN_14040fa70`, `FUN_14040ff60`, `FUN_140410760`, `FUN_140410d40`, `FUN_140410130`, `FUN_140410260`, `FUN_14040e980`, `FUN_14040e810` | surface checker (DEF_STL.OUT): ctor, run, links, next seed, orientation walk, edge neighbours, area, triangle area, patch boundaries, summary, dtor |
| `FUN_1402469a0` | STL result copy (float → double) |
| `FUN_140246ad0` | STL store |
| `FUN_140163340`, `FUN_1402009f0`, `FUN_1402fcd90`, `FUN_140198620`, `FUN_140006410`, `FUN_140005510`, `FUN_1401985e0` | surface slot count, size surface 0, sizes, slot, node data, facet data, "has surface" flag |
| `FUN_140246d00`, `FUN_140246d70`, `FUN_140246dd0`, `FUN_140246e40`, `FUN_140246ef0` | `_DEF_AMGGEO` ctor, init curve fields, init surface fields, reset curves, reset surface |
| `FUN_140246f70`, `FUN_14024bac0` | GEO read / store dispatch by mode |
| `FUN_1402488a0`, `FUN_1402484c0`, `FUN_14024be40` | 3D GEO pre-check, reader, store |
| `FUN_1402059a0`, `FUN_1402fb120`, `FUN_14023e050`, `FUN_14023b850`, `FUN_14023c770`, `FUN_1402581b0`, `FUN_140258260`, `FUN_140257e90`, `FUN_140258090` | 3D GEO plane grouping: main, bbox diagonal, unit normal, distance², plane distance, clear, free id, append, n-th |
| `FUN_140248390`, `FUN_1402478f0`, `FUN_14024bb30`, `FUN_140249700`, `FUN_14024ae10` | 2D GEO pre-check, reader, store, topology, geometry 0 |
| `FUN_140411ad0`, `FUN_140411920`, `FUN_140411a60`, `FUN_140411970` | token reader, its ctor, upper-case, `D`→`E` |
| `FUN_14024f0a0`, `FUN_14024f110`, `FUN_14024f1d0`, `FUN_14024f1b0` | NAS ctor, dtor, read dispatch, mode getter |
| `FUN_140250280` | NAS type scan |
| `FUN_14024f270`, `FUN_14024f620`, `FUN_14024fa50`, `FUN_14024f6a0`, `FUN_14024f670`/`FUN_14024fe90` | CTRIA3, CTETRA (free then fixed), CHEXA readers |
| `FUN_140220810`, `FUN_140256800`, `FUN_1400030b0`, `FUN_140003090` | push int / push double, int / double vector sizes |
| `FUN_140251be0`, `FUN_140251d60` | NAS surface store, mesh store |
| `FUN_140202700`, `FUN_140165500`, `FUN_1401652b0`, `FUN_1401653b0`, `FUN_1401808d0`, `FUN_140183cb0` | mesh allocation (ELMCON.md / RZ.md) |
| `FUN_1401fc290`, `FUN_140354ed0` | boundary build, Fortran FL3_FNDBND |
| `FUN_1401fc730`, `FUN_1402fc1b0` | boundary → surface 0, node compaction |
| `FUN_14047b6c0` = `atol`, `FUN_14048754c` = `atof`, `FUN_1404880dc` = `strtod`, `FUN_140484558` = `fabs`, `FUN_140484644` = `sqrt`, `FUN_140486e8c` = `ceil` | CRT |

## 10. Open questions

* `FL3_FNDBND` (`FUN_140354ed0` with `FUN_140355cbf`/`FUN_14035672f`) is not specified: the
  boundary-face set and order of the CTETRA DIEGEO are only observed (2-tet case). It is the same
  routine ELMCON reading uses, so it belongs to the mesh-boundary spec.
* STL bounds message: the three `%g` values are undefined (missing arguments); the port has to
  choose what to print.
* The STL orientation pass with the 1000-deep recursion limit was not exercised by a test (needs
  a connected patch of > 1000 triangles in one DFS chain); the rule is from the code only.
* `surf+0x20` plane ids (3D GEO) and the per-curve boundary codes (2D GEO) are stored but not
  written to the KEY; who reads them (contact, remeshing) is not known.
* NASTRAN: lines longer than 254 characters, large-field (`GRID*`) and free-field files with
  more than 5 commas on a GRID line were not tested; behaviour is from the code (undefined in
  the last case).
* 2D GEO file types other than 1–4 and the close flag (`*param_3 != 0`, never set by GFREAD)
  were not tested.
