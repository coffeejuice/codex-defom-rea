# OBJPOS interference kernels (boundary faces, bucket grid, bilinear solve, 2D gaps, path backup)

Closes the open items of [OBJPOS_interference.md](OBJPOS_interference.md) (§13 items 1-6) and
OBJPOS.md §12 item 4 (3D mirror boundary rebuild). Section numbers like "IF §5.8" refer to
OBJPOS_interference.md. Shared mechanics: [_common.md](_common.md).

Source: `DEF_PRE_64.exe` v11.0.2, headless Ghidra (`ghidra_p67a`) decompiles and listings in
`C:/Users/alext/Projects/deform-rea-work/out/p67/ifd-kernels/` (`*.c` decompiles, `L_*.txt`
listings, `c_*.txt` / `d_*.txt` constant dumps). **[fact]** = read in code (decompile, listing
where stated), **[inferred]** = deduced. All arrays below are Fortran style (1-based, column major)
unless written `[0..]`.

## 1. Boundary faces of a 3D mesh: `FUN_140354ed0` (FL3 `FNDBND`)

### 1.1 Interface

`FUN_140354ed0(&nelem, conn, &npe, &timing, &nFace, faces, elemOf)` **[fact]**

| Arg | Meaning |
|---|---|
| `nelem` | element count (`obj+0x884`) |
| `conn` | ELMCON, `npe` ints per element, 1-based node ids |
| `npe` | nodes per element (`obj+0x894`): 4 tet, 8 brick, 10 (quadratic tet, see 1.6) |
| `timing` | 1 prints `1000 FL3 FNDBND  TIME (ms)` timings (Fortran WRITE); callers pass 0 |
| `nFace` | out: number of boundary faces |
| `faces` | out: `F(4, nFace)`, 1-based node ids; triangles have `F(4) = F(1)` |
| `elemOf` | out: element of each face (see 1.4 for the hex caveat) |

Callers allocate `faces` = `calloc(facesPerElem·nelem·4, 4)` and `elemOf` =
`calloc(facesPerElem·nelem, 4)` with `facesPerElem = obj+0x898` (4 tet, 6 brick; `FUN_140165500`)
**[fact]** (zero-filled, which matters for 1.3).

Descriptor `DAT_140703fb0` = (nodes per face `npf`, faces per element `nfe`, table selector `ty`)
**[fact]**: `npe == 8` → (4, 6, 3); `npe == 10` → (6, 4, 2); otherwise → (3, 4, 1).
Three work arrays of `n = nfe·nelem` records are allocated (Fortran ALLOCATE `FUN_1404b62c4`):
`T` (`DAT_140703ef0`, 4 ints per face), `E` (`DAT_140703f50`, 2 ints) and `K` (`DAT_140703e90`,
2 ints: key, face id). An allocation failure sets `DAT_140703fcc = 11` and returns with `nFace`
untouched **[fact]**.

`npe == 8` → `FUN_14035672f` (hex, 1.3); else `FUN_140355cbf` (tet, 1.2); then the run sort of
1.4 **[fact]** (call arguments checked in the listing `L_354ed0.txt`).

### 1.2 Tet path `FUN_140355cbf(nelem, conn, npe, &npf, &nfe, &nFace, T, K, faces, elemOf, table, timing)`

Face table = `0x140605090 + (ty-1)·0x60`, `npf` ints per face (dump `tab_faces.txt`) **[fact]**:

| ty | table | faces (local node numbers) |
|---|---|---|
| 1 (tet4) | `0x140605090` | (1,3,2) (1,2,4) (1,4,3) (2,3,4) |
| 2 (tet10) | `0x1406050f0` | 6 per face: (1,6,5,4,3,2) (5,9,10,8,3,4) (10,9,5,6,1,7) (3,8,10,7,1,2) |

```
id = 0
for e = 1..nelem: for f = 1..nfe:
    id += 1
    a = conn(tab(1,f), e); b = conn(tab(2,f), e); c = conn(tab(3,f), e)
    d = (npf == 4) ? conn(tab(4,f), e) : 0
    T(:,id) = (a, b, c, d); K(:,id) = (a + b + c + d, id)        // int sum
qsort(K, id, 8, cmp)              // FUN_14043c760: VS2008 CRT qsort, cmp FUN_14043c7f0 on K(1) only (1.5)
nFace = 0
for i = 1..id:
    f = K(2,i); if T(1,f) == 0: continue                         // removed as a duplicate
    if i != id:
        j = i+1; while j <= id and K(1,i) >= K(1,j): j++          // run end: first larger key
        for k = i+1 .. j-1:
            g = K(2,k)
            if all m = 1..npf: T(m,f) ∈ {T(1,g), T(2,g), T(3,g), (npf == 4 ? T(4,g) : -)}:
                T(1,g) = 0; goto next_i                           // f is internal: drop f and g
    nFace += 1; faces(1..3, nFace) = T(1..3, f); if npf == 4: faces(4,nFace) = T(4,f)
    elemOf(nFace) = (f-1)/nfe + 1
  next_i:
if npf == 3: for k = 1..nFace: faces(4,k) = faces(1,k)
```
**[fact]**. Only the first matching later face is removed. The membership test compares against
`T(1,g)` too, so a face already removed (`T(1,g) = 0`) can still match when its other nodes
cover `f` [inferred: impossible for a valid mesh, where every internal face occurs exactly twice].

### 1.3 Hex path `FUN_14035672f(nelem, conn, npe, &npf, &nfe, &nFace, T, E, K, faces, elemOf, hexTab, tetTab, wedgeTab, timing)`

Tables (all 1-based local node numbers of the 8-node ELMCON record) **[fact]**:

| table | address | stride | faces |
|---|---|---:|---|
| hex | `0x140604fd0` (`0x140604f10 + (3-1)·0x60`) | 4 | (1,4,3,2) (5,6,7,8) (1,2,6,5) (2,3,7,6) (3,4,8,7) (4,1,5,8) |
| tet-in-hex | `0x140604f10` | 3 | (1,3,2) (1,2,4) (1,4,3) (2,3,4) (0,0,0) (0,0,0) |
| wedge | `0x140605030` | 4 | (1,3,2,1) (4,5,6,4) (1,2,5,4) (2,3,6,5) (3,1,4,6) (3,3,6,6) |

Per element `e` (three per-element arrays `KIND`, `NPF`, `NFE`, Fortran allocated) **[fact]**:
```
zeros = #{j : conn(j,e) == 0}
mult  = max_k #{j : conn(j,e) == conn(k,e)}                 // multiplicity incl. itself
KIND, NPF, NFE = 8, npf(4), nfe(6); table = hex; fpf = 4      // fpf = nodes taken per face
if zeros == 2 || mult == 2: KIND = 6, NFE = 5, table = wedge      (NPF stays 4)
elif zeros == 4 || mult == 4: KIND = 4, NPF = 3, NFE = 4, table = tet-in-hex, fpf = 3
for f = 1..6 (always nfe = 6 of the argument, not NFE):
    id += 1; E(:,id) = 0 initially in K
    a,b,c = conn(table(1..3,f), e); d = (fpf == 4) ? conn(table(4,f), e) : 0
    T(:,id) = (a,b,c,d)
    z = #{m ∈ 1..4 : T(m,id) == 0}; p = #{m < q ≤ 4 : T(m,id) == T(q,id)}
    if p < 2 && z < 2: K(:,id) = (a+b+c+d, id); E(:,id) = (e, e)
    else: id -= 1                                            // degenerate face dropped, id reused
```
Notes **[fact]**: the "(0,0,0)" tet-in-hex faces read `conn(0, e)` = the previous element's 8th
node (for `e = 1` four bytes before the array); the face is always dropped (three equal values).
A pyramid (one node repeated four times) is treated as a tet of its first four nodes.

Then `qsort(K, id)` and the same duplicate removal as 1.2, with `npf_f = NPF(E(1,f))` nodes of `f`
compared and the 4th node of `g` only when `npf_f == 4` and `NPF(E(1,g)) == 4`. Output: `faces(1..3)`,
`faces(4)` only when `NPF(e) == 4`; `E(2, nFace) = e`; `elemOf(nFace) = (f-1)/NFE(e) + 1`.
Final pass over the output **[fact]**:
```
for k: e = E(2,k)
    if NPF(e) == 3: faces(4,k) = faces(1,k)
    elif KIND(e) == 6: compact the positive entries of faces(1..4,k) to the front (order kept);
                       if fewer than 4: faces(4,k) = faces(1,k)
```
**Caveat [fact]**: `elemOf` divides the *face id* by the element's own `NFE`; ids are only
`(e-1)·6 + f` when no earlier face was dropped, so `elemOf` is correct for pure brick meshes and
wrong after the first degenerate element (`E(2,·)` holds the right element but is not returned).

### 1.4 Run sort in `FUN_140354ed0` (after either path)

```
for i = 1..nFace:
    s_i = F(1,i)+F(2,i)+F(3,i) (+ F(4,i) when npe == 8)
    for j = i+1..nFace:
        s_j = same for j; if s_j != s_i: break
        if F(1,j) < F(1,i) || (F(1,j) == F(1,i) && F(2,j) < F(2,i)):
            swap F(1..4,i) <-> F(1..4,j); swap elemOf(i) <-> elemOf(j)
```
**[fact]** (comparison order checked in the listing, `0x140355961..0x1403559da`). Within each
run of equal node sums this is an exchange sort on `(F1, F2)`; faces equal in both keep the
qsort order. For tets the sort key used by qsort included `d = 0`, while this pass uses
`F1+F2+F3` (same value); for tet-in-hex faces in an 8-node mesh qsort used `a+b+c+0` but this
pass adds `F(4) = F(1)`, so such runs need not be contiguous (inner loop then stops early).

### 1.5 The CRT qsort (needed for the face order)

`FUN_14043c760` = `qsort(K, n, 8, FUN_14043c7f0)`; comparator: `K1(a) > K1(b) → 1, == → 0,
else -1` (the face id is not compared, so ties are resolved by the algorithm) **[fact]**. Ghidra
identifies `qsort`/`shortsort` as Visual Studio 2008 CRT; the decompile matches that `qsort.c`
exactly **[fact]**:
```
qsort(lo, num): if num < 2 return; hi = lo + (num-1)·w; sp = 0
recurse: size = (hi-lo)/w + 1
  if size <= 8: shortsort(lo, hi)          // selection: for hi down to lo+w: max = first p in lo..hi with
                                           //   cmp(p, max) > 0 updating; swap(max, hi)
  else:
    mid = lo + (size/2)·w
    if cmp(lo,mid) > 0: swap(lo,mid); if cmp(lo,hi) > 0: swap(lo,hi); if cmp(mid,hi) > 0: swap(mid,hi)
    lg = lo; hg = hi
    loop:
      if mid > lg: do lg += w while lg < mid && cmp(lg,mid) <= 0
      if mid <= lg: do lg += w while lg <= hi && cmp(lg,mid) <= 0
      do hg -= w while hg > mid && cmp(hg,mid) > 0
      if hg < lg: break
      swap(lg,hg); if mid == hg: mid = lg
    hg += w
    if mid < hg: do hg -= w while hg > mid && cmp(hg,mid) == 0
    if mid >= hg: do hg -= w while hg > lo && cmp(hg,mid) == 0
    if hg - lo >= hi - lg: { if lo < hg: push(lo,hg); if lg < hi: { lo = lg; goto recurse } }
    else:                  { if lg < hi: push(lg,hi); if lo < hg: { hi = hg; goto recurse } }
  pop: if --sp >= 0: (lo,hi) = stack[sp]; goto recurse
```
A port must use this algorithm (not `std::sort`) to reproduce the face numbering.

### 1.6 Orientation and which faces face the ray

For a positively oriented element (tet: `((x2-x1)×(x3-x1))·(x4-x1) > 0`; brick: nodes 1-4 bottom
counter-clockwise seen from 5-8) every table face is ordered so that its right-hand normal points
**outward** (checked on the unit tet and unit cube) [inferred from the tables]. The 3D mirror
keeps this by swapping element nodes before the rebuild (OBJPOS.md §6.5 step 3). DIEGEO surface
faces keep the order of the KEY file (triangles `(a,b,c,-a)`, 4th entry passed through `abs`).

In the ray test (IF §5.8) the vertices are taken in reverse order `(F4,F3,F2,F1)`, so the
computed normal is the **inward** one and `e = N3 > 1e-8` keeps the faces whose outward normal
has a negative z component in the rotated frame, i.e. the faces turned towards the incoming
travelling node (which moves in +z) **[fact: code; geometric reading inferred]**. Faces with an
outward normal perpendicular to the ray (`|N3| < 1e-8`) or facing away are ignored.

`npe == 10`: the tet routine is used with `npf = 6` but stores only 3 nodes per face (and `d = 0`),
while the duplicate test runs `m = 1..6` over `T(m,f)` (reading into the next record) — not
meaningful; DEFORM 3D meshes are tet4/brick8 [inferred].

### 1.7 Use by the 3D mirror: `FUN_1401fc290(obj)` (OBJPOS.md §6.5 step 4)

Runs when `obj+0x87c` (mesh), `obj+0x9e8` (ELMCON) and the owning model are set **[fact]**:
* 3D: `FUN_140354ed0(&obj+0x884, obj+0x9e8, &obj+0x894, &0, &n, F, EL)` (the element nodes were
  already swapped by `FUN_1401fe410`); `FUN_140163300(obj, 1)` (boundary count `obj+0xbd8 = 1`,
  capped at 100); `FUN_140200a70(obj, n, 0)`: record 0 `+0x10` (`obj+0xbf0`) = n and
  `FUN_14017e8a0` reallocates its arrays (`+0x18` = `calloc(4n,4)` faces, `+0x20` = `calloc(n,4)`
  elements); `memcpy(obj+0xbf8, F, 16n)`, `memcpy(obj+0xc00, EL, 4n)`; free F, EL.
* 2D: `FUN_14032d65e` (FL2 FNDBND, loops) — not used by the 3D mirror.
* Then `FUN_140165d30`: `obj+0x88c = Σ FUN_14017ebc0(loop)` (3D: loop `+0x10` = face count) and
  `FUN_1401631e0`: `FUN_1401632d0` → `FUN_1401856e0(obj+0xae8)` (frees the face-data arrays),
  `FUN_1401858d0(obj+0xae8, n, -1)`: 12 new zero-filled arrays of `n` (ints / doubles in the
  pattern int,int,double ×4) **[fact]**.

So the new face numbering is exactly the FNDBND order of 1.2-1.5. The face data restore of
OBJPOS.md §6.5 step 6 then maps each old face to a new index **[fact]**:
* `FUN_140201fb0(backupLoop)` (3D): looks only at the **first** face: if `F(1,1) == abs(F(4,1))`
  every face gets nodes 2 and 3 swapped, else nodes 2 and 4 (xor swaps) — re-orientation only.
* `FUN_140200ff0(obj, n0, n1, n2, n3)`: if the first new face has `F(1) != F(4)` (quad mesh) the
  match is "all 4 nodes of new face k ∈ {n0..n3}", else "all 3 first nodes ∈ {n0,n1,n2}"; returns
  the first such k (0-based) or -1. Matching is set inclusion, so orientation does not matter.

### 1.8 Use by the interference kernel and order dependence

`FUN_1401a3ea0`/`FUN_1401a5470` call FNDBND on copies of `obj+0x9e0` with the same argument
pattern **[fact]**. The travelling body's faces only select which nodes are cast (`work(n)` flags,
IF §5.4), but the order matters: the bucket query box of each node uses the running minimum `d`
(2.4), and once `d < 0` later nodes look further *below* themselves. Face order (= node order)
can therefore change the result in overlapping cases **[inferred from 2.4]**.

## 2. Bucket search grid (`cfnCUBE_*`, used by IF §5.4 / §5.8)

Global table `DAT_1406fe4f0` of `DAT_1406fe4f8` cube objects (0x1a0 bytes each, `calloc`, ids
`(-1,-1)`), `DAT_1406fe4fc` used **[fact]**. Cube layout: `+0` object id, `+4` sub id, `+8`
locator (2.2), `+0xf8` polygon set (count `+0xf8`, nodes per poly `+0x100`, data `+0x110`),
`+0x118` point set (count, data `+0x158`, dirty flag `+0x124`, bounds `+0x128..`), `+0x160`
result list **[fact]**.

### 2.1 Calls made by `FUN_1403539a1`

1. `FUN_140225640(&1, &0)` CreateCubes_Obj: frees an existing table (`FUN_140225820`), allocates 1.
2. `FUN_140225ff0(&1,&1,&1, &nN1, P1, &nF1, &4, F1, &1, &0)` → `FUN_140226150(id 1, sub 1, type 1,
   &nPts, pts, &nPoly, &4, polys, &offset 1, &create 1, &verbose 0)` (2.2).
3. Per node: `FUN_140228c60(&1,&1, box, &nc, &exact 0, &0)` (2.4) and
   `FUN_140229f30(&1,&1, list, &0)` (copy + `qsort` ascending ints `FUN_140231d00`; duplicates
   are kept; order does not matter for the minimum).
4. `FUN_140225820(&0)` CleanCubes_AllObj.

### 2.2 Build `FUN_140226150` (type 1 = polygons)

```
divisions per axis  bc = nPoly < 10 ? 5 : nPoly < 100 ? 10 : nPoly < 1000 ? 20
                       : nPoly < 10000 ? (int)(0.0033·(double)(nPoly-1000)) + 20
                       : (int)((double)(nPoly-10000)·0.0005) + 50      // DAT_140530da0, DAT_140530d98
FUN_1403fed00(loc, 1, (double)1e-6f, offset)    // tol 9.99999997e-07 (DAT_140530da8 float); 50^3 default
bounds B = FUN_140406360(point set): min/max over ALL nPts points (start ±1e29, strict compares)
for i in x,y,z: lo = B.min_i; hi = B.max_i
    if hi <= lo: hi = lo + 1.0 ; if fabs(hi - lo) < 0.001: hi = hi + 0.001      // DAT_14051f1e8
    a_i = fabs(hi - lo)
sp = pow(fabs((a_x·a_y)·a_z), 0.3333333) / (double)bc      // FUN_140483f60 = CRT pow; DAT_140530d90 = 0.3333333 (not 1/3)
n_i = (int)(a_i / sp); if n_i < 6: n_i = 5; n_i = min(n_i, nPoly)
if bc > 49: while 1.1 < (double)((n_x·n_y·n_z) / bc³) [int division]: n_i = max5(n_i - 1) (all three)
FUN_1403ff890: n_i clamped to [1, 100]
FUN_1403ff970: locator box = the adjusted (lo, hi) per axis (same two fixes), H_i = (hi-lo)/(double)n_i,
               cell lists allocated (n_x·n_y·n_z cells, 32 B each)
for each polygon k = 1..nPoly with first node > 0: FUN_140401050(loc, poly k, 4, k)       (2.3)
```
**[fact]** (decompile; `(int)` is truncation). The box is the bounding box of all nodes of the
fixed body (all mesh nodes, not only boundary nodes), with no padding.

### 2.3 Polygon insertion `FUN_140401050(loc, nodes, np = 4, id)`

```
for each of the 4 entries (abs(node), point = pts(node - offset + 1)):
    for axis i: c = coordinate
        idx_i = (1e-6 < fabs(c - hi_i)) ? (int)(((c - lo_i) / (hi_i - lo_i)) · (double)n_i)
                                        : (int)((((c - lo_i) · 0.999) / (hi_i - lo_i)) · (double)n_i)
    track min/max idx per axis (min starts at n_x·n_y·n_z, max at 0)
clamp min to >= 0, max to <= n_i - 1
N = normalised Σ_{k=2..np-1} (v_{k-1} - v_0) × (v_k - v_0)  (FUN_140240d80, FUN_140239b40: 0 if |·| <= 1e-12)
if !(all |N_i| < 1e-4) && (span_x > 2 || span_y > 2 || span_z > 2):        // FUN_14023aa90, DAT_14051f250
    for ix, iy, iz in the index box:
        w_k = ((x_k - (lo_x + ((double)ix + 0.5)·H_x)) / (1.01·H_x), same for y, z)   // DAT_14051f240, DAT_140562a90
        n = FUN_140454110(4, w)                // get_polygon_normal: Σ_{k=2..3} (w_{k-1}-w_0)×(w_k-w_0) (unnormalised)
        if FUN_1404537f0(4, w, n, 0, 0) == 1: append id to cell (ix,iy,iz)
else: append id to every cell of the index box
```
**[fact]**. Cell index `ix + iy·n_x + iz·n_x·n_y` **[fact]**. `FUN_1404537f0` is
`polygon_intersects_cube` of Green & Hatch, "Fast polygon-cube intersection testing" (Graphics
Gems V, `pcube.c`): `FUN_140453010` trivial_vertex_tests (face planes `±0.5`, edge bevels
`|a±b| <= 1.0`, corner bevels `|±x±y±z| <= 1.5`; returns 1 if a vertex is inside, 0 if all
vertices are outside one plane, else -1), `FUN_140453e80` (edge test `FUN_140453c40`
segment_intersects_cube; then, unless `n == 0`, `t = (n·w_0)/(n·sign(n))`, and if
`(t+0.5)·(t-0.5) <= 0` the winding number `FUN_140453850` polygon_contains_point_3d of
`t·sign(n)`) **[fact; matching the published code by structure and constants 0.5, 1.0, 1.5]**.
Only the return value exactly 1 registers the cell (a winding number of -1 would not).
Triangles are passed as 4 points with `v_3 = v_0` [fact].

### 2.4 Query `FUN_140228c60(&1,&1, box, &nc, &0, &0)`

`box = (p.x, p.y, p.z, p.x, p.y, p.z + dcur)` (corner 0, corner 1; IF §5.8).
```
reset result list (FUN_140405150)
if !FUN_140240b40(box, locatorBox): nc = 0; return
    // FUN_140240b40: per axis "overlap" iff a.min ∈ [b.min,b.max] or a.max ∈ [b.min,b.max] or
    //   b.min ∈ [a.min,a.max] or b.max ∈ [a.min,a.max] (inclusive); all three axes required
for corner c in (0, 1): for axis i:
    v = (lo_i <= c_i) ? ((c_i <= hi_i) ? c_i : hi_i) : lo_i                  // clamp
    idx = (1e-8 < fabs(v - hi_i)) ? (int)(((v - lo_i)/(hi_i - lo_i))·(double)n_i)
                                  : (int)((((v - lo_i)·0.999)/(hi_i - lo_i))·(double)n_i)   // DAT_14051bed0 = 1e-8
    track per-axis min/max over the two corners
for ix, iy, iz in [min,max]: append every id stored in the cell (no box test: exact flag 0)
nc = result count
```
**[fact]**. Consequences **[inferred from the code]**:
* The two corners are min/max-ed, so a negative `dcur` gives the range `[p.z + dcur, p.z]`.
* A face is a candidate iff it is registered in the node's `(ix, iy)` column at some `iz`
  between the cells of `p.z` and `p.z + dcur`. Every face whose hit lies in `[p.z, p.z+dcur]`
  is found; a face *below* the node (negative `h`) is found only if it reaches the node's own cell
  (or, once `dcur < 0`, the cells down to `p.z + dcur`).
* Nodes outside the box in x or y, or with the whole query segment outside the box in z (with
  the inclusive test above), get no candidates (`dn = 1e5`).
* A brute-force port (all faces) differs from the original only through faces behind the node;
  test `ik_m102_bucket_3d` (section 7) is built to show this.
* Oracle (ov-ifd): `OBJPOS/102-overlap-3d` (die 1 lowered 2, mode 102, same set-up as
  `ik_m102_bucket2_3d`) moves die 1 by (0,0,2) back to contact: the die bottom face behind the top
  workpiece nodes is returned (shared cell) and gives `d = -2`, as predicted here.

## 3. Bilinear inverse map (IF §5.11)

Equations from `FUN_1404ccf58` (IF §5.11): row x `A1 ξη + B1 ξ + C1 η + D1 = 0` with
`(A1,B1,C1,D1) = (c_x, b_x, a_x, d_x)` (`DAT_14072ff70/78/80/88`), row y `(A2..D2) =
(c_y, b_y, a_y, d_y)` (`DAT_14072ff90..a8`) **[fact]**. `tol = 1e-9` (`DAT_1406438d8`).

### 3.1 Analytic solve `FUN_1404cd2e1(&A1,&B1,&C1,&D1,&A2,&B2,&C2,&D2, XE, &k, &err)`

Roots are collected as `R_r = (ξ_r, η_r)`, `r = 1, 2` (`DAT_140730020 + 16(r-1)`), count `m`
(`DAT_140730040`). `k = 1` on entry. `neg(x)` = sign-bit flip. Division results are stored in
the caller's coefficient globals (they are modified) **[fact]**.
```
if |A1| >  tol: (B1,C1,D1) = (B1/A1, C1/A1, D1/A1); A1 = 1.0
if |A2| >= tol: (C2,D2,B2) = (C2/A2, D2/A2, B2/A2); A2 = 1.0           // note >= here
if A2 == A1:                                   // both normalised (or equal): subtract
    E = (B2-B1, C2-C1, D2-D1); X = row 1
    if |E0| > tol: if |E1| <= tol: XI(q = E2/E0) else QUAD
    else: ETA(q = E2/E1)
elif |A1| <= tol:                              // row 1 is linear
    E = (B1, C1, D1); X = row 2
    if |B1| <= tol: ETA2(q = D1/C1)            // uses row 2
    elif |C1| <= tol: XI2(q = D1/B1)
    else: QUAD with qa = (B1·A2)/C1, qb = ((A2·D1)/C1 - B2) + (B1·C2)/C1, qc = D2 - (C2·D1)/C1
else:                                          // row 2 is linear
    E = (B2, C2, D2); X = row 1
    if |E0| > tol: if |E1| <= tol: XI(q = E2/E0) else QUAD
    else: ETA(q = E2/E1)
XI(q):  ξ = neg(q); m = 1; den = C1 - q·A1; if den == 0: den = 1e-9; η = neg((D1 - q·B1)/den)
ETA(q): η = neg(q); m = 1; den = B1 - q·A1; (0 → 1e-9);           ξ = neg((D1 - q·C1)/den)
XI2(q): ξ = neg(q); m = 1; den = C2 - q·A2; (0 → 1e-9);           η = neg((D2 - q·B2)/den)
ETA2(q):η = neg(q); m = 1; den = B2 - q·A2; (0 → 1e-9);           ξ = neg((D2 - q·C2)/den)
QUAD (with X = row 1 unless stated): qa = (A1·E0)/E1, qb = ((A1·E2)/E1 - B1) + (E0·C1)/E1,
      qc = D1 - (C1·E2)/E1;  b = neg(qb), a = neg(qa), c = qc; m = 0
    if |a| > tol:
        disc = b·b - (a·4.0)·c;  if disc < 0: err = 1; return             // k stays 1
        s = sqrtsd(disc); ξ1 = (s - b)/(a·2.0); ξ2 = neg(b + s)/(a·2.0)
        η2 = neg((E0·ξ2 + E2)/E1); η1 = neg((ξ1·E0 + E2)/E1)
        if -1.001 <= ξ1 <= 1.001 && -1.001 <= η1 <= 1.001: m = 1, R_1 = (ξ1, η1)
        if -1.001 <= ξ2 <= 1.001 && -1.001 <= η2 <= 1.001: m += 1, R_m = (ξ2, η2)
    else: m = 1; ξ = neg(c/b); η = neg((E2 - (c/b)·E0)/E1)
accept: if m == 0: err = 1; return
    n = 0
    for j = 1..m:
        if R_m inside [-1.001, 1.001]² :                    // NOTE: tests R_m (the last root), not R_j
            n += 1; for i = 1,2: XE(i,n) = R_j(i); if R_m(i) > 1.0: XE(i,n) = 1.0; if R_m(i) < -1.0: XE(i,n) = -1.0
    if n == 0: err = 1; return
    err = 0; k = n
```
**[fact]**; the "last root" indexing of the accept loop was confirmed in the listing
(`0x1404cd743..0x1404cdff9`: the bounds test and the clamp compare `[0x140730020 + 16·m - 16]`
for every `j`). 1.001 is `DAT_140bb26a0/a8 = ∓1.0010000467300415`. The `disc` order
`b·b - (a·4)·c` and the root formulas were checked in the listing (`0x1404cdab2..0x1404cdc2a`).

`FUN_1404ccf58` then (IF §5.11): if `err != 1` and `twist` (both ξη coefficients `> 1e-5`
before normalisation) → done with `XE(:,1)`; else if `err != 1`, `Q = map(XE(:,1))`,
`(P1-Q1)²... (ey·ey + ex·ex) < 1e-6 && k == 1` → done; otherwise `k = 1` and the fallback
3.2 **[fact]**. So two accepted roots (k = 2) always go to the fallback when not "twisted".


> Ported (geoutl_poly, bit-exact): the accept loop also lets a NaN root component pass the bounds
> test (comparisons with NaN are false on the reject branch).

### 3.2 Iterative fallback `FUN_1404ce05b(V, P, XE, &err)` (bisection on η)

`V(3,4)` the quad (rotated, relative to the node), `P = (0,0)`; only x, y are used.
```
sx = (((V11 + 0.0) + V12) + V13) + V14 ; sy = same for row 2          // listing order
Cx = V11 - sx·0.25 ; Cy = V21 - sy·0.25
σ = (Cx·(V22 - V21) - Cy·(V12 - V11) < 0) ? -1 : 1                   // orientation of the quad
W = V (3x4 copy); lo = -1.0; hi = 1.0; it = 0
loop:
    t = (lo + hi)·0.5
    for c = 1,2: for i = 1..3: acc = 0; for k = 1..4: acc = ((s_c·s_k) + 1.0)·W(i,k) + acc
                 M(i,c) = acc·0.25            // s = (-1, 1, 1, -1) (DAT_1406438e0); M1 = mid(W1,W4), M2 = mid(W2,W3)
    if σ == 1: base = M(:,1); d = (M(1,2) - M(1,1), M(2,2) - M(2,1))
    else:      base = M(:,2); d = (M(1,1) - M(1,2), M(2,1) - M(2,2))
    θ = ATAN2F(d.y, d.x)                      // FUN_1404b3878 (3.3)
    sθ = SINF(θ); cθ = COSF(θ)                // FUN_1404b3b30 → FUN_1404b3ab0 / FUN_1404b3ab8 (3.3)
    dx = P1 - base.x; dy = P2 - base.y
    cr = cθ·dy - sθ·dx
    if fabs(cr) <= 1e-6:                                     // DAT_140bb2680
        err = 0; XE(2) = t
        pr = sθ·dy + cθ·dx ; len = d.y·sθ + cθ·d.x
        if len < pr: pr = len ; if pr < 0: pr = 0
        XE(1) = ((pr·2.0)/len + (-1.0))·(double)σ
        return
    if cr >= 0: lo = t; W(:,1) = M(:,1); W(:,2) = M(:,2)     // keep the upper half
    else:       hi = t; W(:,4) = M(:,1); W(:,3) = M(:,2)     // keep the lower half
    it += 1; if it >= 50: err = 1; return                   // DAT_140643900; XE unchanged
```
**[fact]** (listing `L_ce05b.txt`; the products `s_c·s_k + 1` are exactly 0 or 2, so
`M(:,1) = (2·W1 + 2·W4)·0.25` bit-equals `(W1+W4)/2`). At most 50 midpoints are evaluated. The
caller (`FUN_1403541fc`) ignores `err`: on failure the analytic `XE(:,1)` (or whatever was left)
is used by `FUN_1404d6974` [fact: no test of the flag at the call site].

### 3.3 Fortran runtime atan2 / atan / sin / cos (PathScale libm, Cody-Waite)

> Naming: ATAN2F / SINF / COSF below mean the Fortran-runtime **double** routines `ps_atan2`
> (1404b3878) and `ps_sincos` (1404b3b30) of `defrt/stdlib/libm.hpp`, not single-precision functions.


`FUN_1404b4898(x)` = fabs (and-mask). Constants are listed in section 6. **[fact, listings
`L_b3878.txt`, `L_atan.txt`, `L_sin.txt`]**.

`ATAN2F(y, x)` (`FUN_1404b3878`):
```
if x == 0: return (y == 0) ? 0.0 : (y < 0 ? -π/2 : π/2)          // DAT_14056f0a8, DAT_14051f238
r = ATANF(fabs(y / x)); if x < 0: r = π - r                        // DAT_14051f230
if signbit(y): r = -0.0 - r ; return r
```
`ATANF(x)` (`FUN_1404b3f48`):
```
neg = x < 0; f = neg ? -0.0 - x : x; N = 0
if f > 1.0: f = 1.0 / f; N = 2
if f > 0.2679491924311227: N += 1; f = (((f·0.7320508075688773 - 0.5) - 0.5) + f) / (f + 1.7320508075688772)
if fabs(f) > 1.05367121277235e-08:
    g = f·f
    p = (((-8.494624035132068 - g·0.8375829936815006)·g - 20.505855195861653)·g - 13.688768894191927)·g
    q = (((g + 15.024001160028575)·g + 59.57843614259735)·g + 86.15734959713025)·g + 41.06630668257578
    f = f + (p / q)·f
if N > 1: f = -0.0 - f
f += {N=1: 0.5235987755982989, N=2: 1.5707963267948966, N=3: 1.0471975511965979}
return neg ? -0.0 - f : f
```
`SINF(x)` / `COSF(x)` (`FUN_1404b3960(x, cos)`, entries `FUN_1404b3ab0` cos = 0 and
`FUN_1404b3ab8` cos = 1):
```
y = fabs(x); sgn = (!cos && x < 0); Y = cos ? y + π/2 : y
if Y > 210828714.0: return 0.0
XN = (int)(Y·0.3183098861837907 + 0.5)        // cvttsd2si
if XN odd: sgn = !sgn
xn = (double)XN; if cos: xn = xn + (0.0 - 0.5)
f = (y - xn·3.1416015625) - xn·(-8.908910206761538e-06)
if fabs(f) >= 1.05367121277235e-08:
    g = f·f
    r = ((((((((g·r8 - r7)·g + r6)·g - r5)·g + r4)·g - r3)·g + r2)·g - r1)
    f = f + (r·f)·g
return sgn ? -0.0 - f : f
```
with `r1..r8` = 0.16666666666666666, 0.008333333333333165, 1.984126984120184e-4,
2.7557319210152756e-6, 2.5052106798274583e-8, 1.605893649037159e-10, 7.642917806891047e-13,
2.7204790957888847e-15 (`0x140572440..78`). These are also the sin/cos of 2D `FUN_14033f2bf`
(via `FUN_1404b3b30`) and of the 2D arc kernel (4.4), resolving IF §13 item 5 for those calls.
The 2D kernel's θ still uses the CRT `atan`/`atan2` (`FUN_1404865ac`, `FUN_140485d6c`, not
transcribed).

## 4. 2D gap kernels (IF §7.2)

Context set before the sampling (IF §7.2) **[fact]**: `FUN_1403460c2(model+0xdb0, +0xdb8)`
stores GEOERR 1/2 in `DAT_140b74428/30`; `FUN_1403460e7(&ext)` sets `DAT_140b84730 = ext`
(`max(|xS.max-xS.min|, |yS.max-yS.min|)`, overriding the value stored by `FUN_14034c209`),
`DAT_140b84720 = GEOERR1`, `DAT_140b84728 = GEOERR2`; `FUN_14033ffb8(&flag)`: `DAT_1405f6000 =
(flag == 0)`. Tolerances used below: `tolE = 1e-10` (`DAT_1405f1eb8`), `tolG = DAT_140b84748` if
non-zero else `ext·GEOERR2` (`DAT_140b84748` is 0.0 in the image; writers not searched).

The ray direction is `dir = (0.0, 1.0)` for the target S and `(0.0, -1.0)` for the mover T
(`local_2c0/2b8` in `FUN_1401a1370`); the query point of sample i is `Q = (x_i, y_ref)`.

### 4.1 Mesh boundary sampler `FUN_14033ffe0(-, pts, &np, &yref, &x0, &x1, &n, dir, g)`

`pts` = rotated boundary loop as `np` points (2 doubles), segments `k = 1..np-1` from point k to
k+1 **[fact]**:
```
for i = 0..n: x = ((x1 - x0)/(double)n)·(double)i + x0
    best = 1e10
    for k = 1..np-1:
        gap = best; FUN_14030bba0(&pts(k), zeros4 (DAT_1405f6030), &(x, yref), dir, &gap, &flag)
        if DAT_1405f6000 != 1 || gap >= -1e-15:
            if fabs(gap) < fabs(best): best = gap
            if best == 0.0: break
    g(i+1) = g(i+1) + best
```
### 4.2 Die curve sampler `FUN_140340273(name, recs, &nrec, &yref, &x0, &x1, &n, dir, g, &err)`

Curve records are 6 doubles: `[0]` kind (1 = line, else arc), `[1..2]` start S, `[3..4]` second
point (line: end point; arc: centre C), `[5]` arc sweep angle φ (radians, signed) **[fact for the
layout, sweep meaning from 4.4]**.
```
tolA = DAT_140b84748 != 0 ? DAT_140b84748 : DAT_140b84730·DAT_140b84728
err = 0
for each line record: L = sqrt((x1-x2)² + (y1-y2)²) [as (dx·dx) + (dy·dy)];
    if L < 1e-6 (DAT_140baec98, = (double)1e-6f): Fortran WRITE "1001 4X WARNING: Zero length en..."
    with the record number, err = 1
if err == 1: return                                           // caller: kernel fails (IF §7.2)
dx = (x1 - x0)/(double)n
for i = 0..n: x = (double)i·dx + x0
    best = 1e10
    for each record: gap = best
        line: FUN_14030bba0(&rec[1], zeros4 (DAT_1405f6210), &(x,yref), dir, &gap, &f)
        arc : FUN_1403407d7(&rec[1], &rec[3], &rec[5], &(x,yref), dir, &gap, &tolA, &f)
        if DAT_1405f6000 != 1 || gap >= -1e-15:
            if gap < best: best = gap ; if best == 0.0: break
    g(i+1) = g(i+1) + best
```
**[fact]**. Mesh loops keep the gap of smallest magnitude, die curves the smallest signed gap.

### 4.3 Segment kernel `FUN_14030bba0(seg, vel, Q, dir, &gap, &flag)`

General swept-segment routine (end-point velocities `vel(1..2)`, `vel(3..4)`); here `vel = 0`.
`gap` is only written on a hit; `flag` is not read by the samplers **[fact]**.
```
A = (vel1 - dir1, vel2 - dir2); B = (vel3 - dir1, vel4 - dir2)          // here A = B = -dir
LA = sqrt(Ay·Ay + Ax·Ax); LB = sqrt(By·By + Bx·Bx)
if !(LA > tolE || LB > tolE): return
if LA <= tolE: A = B ; if LB <= tolE: B = A
p1 = seg(1..2), p2 = seg(3..4)
e = ((p1x - p2x)·tolE, (p1y - p2y)·tolE); q1 = (e.x + p1x, e.y + p1y); q2 = (p2x - e.x, p2y - e.y)
c1 = ((Qy - q1y)·Ax - (Qx - q1x)·Ay) · ((q2y - q1y)·Ax - (q2x - q1x)·Ay)
c2 = ((Qy - q2y)·Bx - (Qx - q2x)·By) · ((q1y - q2y)·Bx - (q1x - q2x)·By)
if !(c1 >= 0 && c2 >= 0): return                       // Q not in the band swept by the segment
dPx = Qx - p1x; dPy = Qy - p1y; sx = p2x - p1x; sy = p2y - p1y
Ls = sqrt(sy·sy + sx·sx); Lp = sqrt(dPx·dPx + dPy·dPy)
a = Bx·Ay - Ax·By                                      // 0.0 exactly when A == B
b = (((By - Ay)·dPx - (Bx - Ax)·dPy) - Ax·sy) + Ay·sx
c = sy·dPx - sx·dPy
d0 = (Ls >= 1e-10) ? c / Ls : Lp                       // signed distance of Q from the segment line
if d0 < neg(tolG): return                              // back side: ignored
ad = fabs(d0)
if ad <= tolE:                                         // Q on the line
    if fabs(a) >= tolE || fabs(b) >= tolE:
        t = Lp/Ls; vx = t·Bx + (1.0 - t)·Ax; vy = t·By + (1.0 - t)·Ay
        if vy·dPx - vx·dPy < 0: gap = 0.0; flag = 1
    return
if fabs(a) <= tolE:                                    // linear (always the case here)
    if fabs(b) <= tolE: flag = -1; return
    t = neg(c / b)
    if d0 <= 0: { if ad <= tolG && t < 0: gap = 0.0; flag = 1 }
    else:       { if t > 0: gap = t; flag = 1 }
    return
quadratic (moving segments; not reached with vel = 0): disc = b·b - (a·4.0)·c; < 0 → flag -1;
    r1 = ((sqrt(disc) - b)/a)·0.5; r2 = (neg(sqrt(disc) + b)/a)·0.5; positions p1 + r2·A, p2 + r2·B;
    t = r1, or r2 when |(|q2-q1| - |Q-q1|) - |Q-q2|| <= tolE; d0 <= 0: (ad >= tolG → flag -1)
    t < 0 → gap = t; d0 > 0: t >= 0 → gap = t
```
**[fact: decompile]** [listing-checked by the FP audit: every expression above matches `FUN_14030bba0`'s listing].
With `dir = (0, ±1)`: `Ax = 0.0 - 0.0`, `Ay = 0.0 - (±1)`, so `b = Ay·sx` (plus signed zeros)
and the gap is `t = neg(c / b)` — the vertical distance from Q to the segment along `dir`
[inferred]. Only segments with `d0 > 0` (Q on the positive side of the segment direction) give
a positive gap; segments seen from behind are skipped unless within `tolG`, where a
non-positive `t` returns gap 0 (contact) **[fact]**.

### 4.4 Arc kernel `FUN_1403407d7(S, C, &φ, Q, dir, &gap, &tolA, &flag)`

```
g1 = g2 = *gap                                         // incoming best
dP = Q - C; Lp = sqrt(dPy·dPy + dPx·dPx); Ld = sqrt(dir2·dir2 + dir1·dir1)
R = sqrt((Sy - Cy)·(Sy - Cy) + (Sx - Cx)·(Sx - Cx))
u = (dPx/Lp, dPy/Lp); dh = (dir1/Ld, dir2/Ld)
n = FUN_1404d71d7(dh, 3.1415926535898·0.5)            // rotate: (cθ·x - sθ·y, sθ·x + cθ·y), SINF/COSF of 3.3
q = n.y·dPy + n.x·dPx
if fabs(q) > R: return                                 // the line through Q misses the circle
if q <= 0: q = neg(q); n = -n (sign flips)
F = (n.x·q + Cx, q·n.y + Cy); h = sqrt(R·R - q·q)
X1 = (h·dh.x + F.x, h·dh.y + F.y)
if FUN_14032bd2a(S, C, φ, X1): e = X1 - Q; s = e.y·dh.y + e.x·dh.x
    g1 = (s >= 0) ? fabs(sqrt(e.y·e.y + e.x·e.x)/Ld) : neg(fabs(...))
X2 = (F.x - h·dh.x, F.y - h·dh.y); same → g2
g = (fabs(g2) < fabs(g1)) ? g2 : g1
if fabs(g) <= 1e-7:                                    // DAT_140baeca0: touching at Q
    if φ > 0: u = -u
    if dh.y·u.y + dh.x·u.x <= 1e-5: return             // DAT_140baeca8
    g = *gap; FUN_140341100(S, tmp, C, &φ, zeros2, Q, dir, &tolA, &g, &f2)   // 4.5
if fabs(g) < fabs(*gap): *gap = g; flag = 1
```
**[fact: decompile]** [listing-checked by the FP audit]. The arc's own endpoints/sweep enter only through the span test:

`FUN_14032bd2a(S, C, &φ, X, &in)`: `a = S - C`, `b = X - C`; requires
`|sqrt(b.y² + b.x²) - sqrt(a.y² + a.x²)| <= ext·GEOERR2`; `α = FUN_14032beef(a, b)`; if `φ < 0`:
`α = -α`, `φ = -φ`; in if `-0.001 <= α <= 0.001 + φ`, else with `α += 3.1415926535898·2.0` the
same test **[fact]** [listing-checked]. `FUN_14032beef(a, b, &α)`: if `|a| > 1e-10 && |b| > 1e-10` (lengths as
`sqrt(y·y + x·x)`): `α = ATAN2F(((a.x·b.y - a.y·b.x)/|a|)/|b|, ((b.y·a.y + b.x·a.x)/|a|)/|b|)`
else 0 **[fact]** [listing-checked].

### 4.5 Arc contact refinement `FUN_140341100` / `FUN_14034151e` (summarised)

Reached only when the best arc hit is within 1e-7 of the sample point and the direction is not
tangent. `FUN_140341100` computes the arc end point `E = rot(S - C, φ) + C` (`FUN_1404d71d7`
with the angle φ, R9 = the sweep pointer in the listing), expresses S, E and C in the frame of
`dir` relative to Q (dot/cross with `dir/|dir|`), orders the end points (flag 2 and negated sweep
when swapped) and calls `FUN_14034151e` (4576 B), which solves the ray/arc contact in angle
space with Fortran `ACOS` (`FUN_1404b3d50` → `FUN_1404b3ba4`), sorts candidate angles
(`FUN_1404d93ae`), ±π periodic copies, tolerance 1e-4 (`DAT_1405f6290`) and π =
3.1415926535898, then returns `g = (|C_y/ sinα|·cosα + C_x)/|dir|`, rejected (status 0) when
negative **[fact, summarised; not transcribed — open item 2]**.

### 4.6 Area function and root finder

`FUN_140437b3b(g, &1, &(n+1), &1, &gmin, &gmax, &1)`: start `gmin = 1e20`, `gmax = -1e20`
(`DAT_140bb15c8/d0`), strict updates **[fact]**. `FUN_14033fe8e`: `A(s) = Σ_{i=1..n}
((max(0, s - g_i) + max(0, s - g_{i+1}))·dx)·0.5 + acc` with `acc` from 0, `max` done as
`v = s - g; if v < 0: v = 0` **[fact]**. The root finder is IF §7.3; its NaN handling was
checked in the listing (`L_3f8d4.txt`: `comisd` + `jp` → an unordered relative test counts as
"not converged") **[fact]**. Consequence **[inferred from the code]**: with `tol = 0` (area 0 %)
`Atol = 0`, the first bracket has `Alo = 0` at `lo = gmin`, every false-position step gives
`f = 1`, `s = lo`, `A(s) = 0`, the relative test is NaN and `|w| = step >= 0.01`; the loop runs
100 times and returns `err = 1` → "interference positioning failed". **Confirmed by the oracle
run** (seven 2D cases with token 2 = 0 fail, the 1 % variants succeed; details and the listing
addresses in OBJPOS_interference.md §7.3).

## 5. Path branch backup (IF §8.1, `FUN_14019fb50`)

```
B = FUN_14015ec80(local 16 KB object, 0)        // _OBJ constructor
FUN_14015ef60(B, model)                         // B+15000 = model, sub-object owners set
FUN_1401613a0(B, A, model+0x15f8, f4=1, f5=1, f6=1, f7=1, f8=0, f9=1)       // backup
... FUN_14019ffc0 moves A along its path (FUN_1402007d0 → FUN_14021d0b0(A, δ, &0, &1)) ...
failure: FUN_1401613a0(A, B, model+0x15f8, same flags)                       // restore
FUN_14015f790(B)                                // destructor, on both paths
```
**[fact]** (flag order from the listing of the stack stores). `FUN_1401613a0(dst, src, ctx,
&f4..&f9)` copies **[fact]**: `FUN_140158f70(dst)` reset; `+8` number only if `f8` (so not
here); `+0xc` type; name `+0x828` (64 chars, `f7`); `+0x87c..+0x88c`, `+0x3984/88/8c`,
`+0x3588/8c`, `+0x3980`; element kind `FUN_140165500` (`+0x890..+0x89c`); 3D die geometry
`FUN_1401618b0` (`+0x3990` flag, item count, each `+0x3c00[k]` by its vtable assign) or 2D
`FUN_1401619c0`; FLDGEO/FLWNET `+0x3328`, `+0x3458`; boundary loops `+0xbd8`, `+0xbe0[k]`;
node arrays `+0x8a0` (`FUN_140180390`, `f5`); ELMCON `+0x9e0` (`FUN_140183900`, `f4, f6, f9`);
face data `+0xae8`; process `+0x1ea0`; movement `+0x2160` (`FUN_14018bd30`: direction `+0x50`,
STROKE `+0x170`, `+0x188`, `+0x1a0`, path position `+0x630`, `+0x648` and `FUN_14018beb0`:
type, tables, ...); remesh `+0x2d10`; BC windows `+0x2e68`; edge list `+0x3998`; `+0x3aa0..`.

Not copied: the sub-object `+0x2be8` and the inter-object windows owned by A
(`model+0x13c8` entries, moved by `FUN_14019f100`). `FUN_14021d0b0` moves both (OBJPOS.md §6.1
step 4), so after a **failed** path search they stay displaced by (last tried path position −
original position) while the rest of A is restored **[fact: code; effect inferred]**. On
success nothing is restored; the dispatcher's `FUN_14019ed10` puts every path object at the new
time (coupled objects included, also those whose own search failed).

## 6. Constants (new in this file)

| Address | Value | Use |
|---|---|---|
| `0x140604f10` / `fd0` / `0x140605030` / `090` / `0f0` | tables | 1.2, 1.3 |
| `DAT_140530da8` | 1e-6f | locator tolerance |
| `DAT_140530d90` | 0.3333333 | pow exponent (spacing) |
| `DAT_140530da0`, `DAT_140530d98` | 0.0033, 0.0005 | division count slopes |
| `DAT_140530d88` | 1.1 | bucket count limit |
| `DAT_14051f1e8` | 0.001 | minimum box extent |
| `DAT_140509c90` | 1.0 | degenerate box extent |
| `DAT_14051f1b8`, `DAT_14051bed0` | 1e-6, 1e-8 | "at max" index eps (insert, query) |
| `DAT_140522f68` | 0.999 | "at max" index factor |
| `DAT_14051f250` | 1e-4 | polygon normal ~0 |
| `DAT_14051f240`, `DAT_14056aed0` | 0.5, -0.5 | cell centre, pcube face planes |
| `DAT_140562a90` | 1.01 | cell scale for pcube |
| `DAT_14051f1d0`, `DAT_14056aed8` | 1.5, -1.5 | pcube corner bevels |
| `DAT_14052c598`, `DAT_14052c5a0` | 1e29, -1e29 | bounds start |
| `DAT_1406438d8` | 1e-9 | bilinear coefficient tol |
| `DAT_1406438e0..f8` | -1, 1, 1, -1 | ξ signs of the quad nodes |
| `DAT_140bb2650/58/60/98/b0` | -1, 1, 0.25, 2, 0.5 | 3.1, 3.2 |
| `DAT_140bb2680` | 1e-6 | fallback line distance |
| `DAT_140643900` | 50 | fallback iterations |
| `DAT_14051f230/38`, `DAT_14056f0a8`, `DAT_14056ef60` | π, π/2, -π/2, -0.0 | ATAN2F |
| `0x140572440..0x1405725b0` | Cody-Waite coefficients | 3.3 |
| `DAT_1405f1eb8`, `DAT_140bae858` | 1e-10, 1e-10 | segment tolE, min length |
| `DAT_140bae860`, `DAT_140bae840`, `DAT_140bae870` | 4, 0.5, 1 | segment quadratic |
| `DAT_1405f6030`, `DAT_1405f6210`, `DAT_1405f6240` | zeros | end-point velocities |
| `DAT_140baec98` | (double)1e-6f | zero-length line |
| `DAT_140baeca0`, `DAT_140baeca8` | 1e-7, 1e-5 | arc contact, tangency |
| `DAT_1405f6238`, `DAT_1405f3be0`, `DAT_1405f6298` | 3.1415926535898 | π in the arc code (not exact π) |
| `DAT_1405f6230`, `DAT_140baec30/38` | 1, 1, -1 | signs |
| `DAT_1405f3bd8` | 0.001 | arc angle tolerance |
| `DAT_140baeb58` | 1e-10 | angle vector length |
| `DAT_140bb15c8/d0` | 1e20, -1e20 | gmin/gmax start |
| `DAT_140b84748` | 0.0 | 2D tolerance override |

## 7. Tests

`tests/commands/OBJPOS/ik_*.KEY`, listed in `tests/commands/OBJPOS/cases_ik.json` (labels
`OBJPOS/ik-*`). Expected values come from this analysis, not from an oracle run.

| File | Exercises | Prediction |
|---|---|---|
| `ik_mirror_brick_3d.KEY` | 3D mirror of the brick workpiece, FNDBND hex path + face re-mapping, GENDB | face order of 1.3-1.5 |
| `ik_mirror_tet_3d.KEY` | same on the tet seed (FNDBND tet path) | face order of 1.2, 1.4, 1.5 |
| `ik_m102_bucket_3d.KEY` | die 1 lowered 5 into the workpiece, mode 102 | bucket grid 8×10×11 over the die surface; the die bottom (z=10, cell 0) is found only from the side nodes at z=11, not from the top nodes at z=15 (cell 1): d = -1, die 1 moved by (0,0,1), final bottom at z = 11 (a brute-force port gives 15) |
| `ik_m102_bucket2_3d.KEY` | same with the die lowered 2 | top nodes share cell 0 with the die bottom: d = -2, die back at z = 15 |

## 8. Helper table

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140354ed0` | FL3 FNDBND driver + run sort | 3567 | read fully + listing |
| `FUN_140355cbf` | tet faces | 2672 | read fully |
| `FUN_14035672f` | hex/wedge/tet-in-hex faces | 6655 | read fully |
| `FUN_14043c760`, `FUN_14043c7f0`, `qsort`, `shortsort` | VS2008 qsort, key compare | – | read fully |
| `FUN_1404b62c4`, `FUN_1404b6d74`, `FUN_1404b5ef4`, `FUN_140235710` | Fortran ALLOCATE/DEALLOCATE/WRITE, timer | – | not followed |
| `FUN_1401fc290` | 3D/2D boundary rebuild | 1153 | read fully |
| `FUN_140163300`, `FUN_140200a70`, `FUN_14017e8a0`, `FUN_140165d30`, `FUN_14017ebc0`, `FUN_1401631e0`, `FUN_1401632d0`, `FUN_1401858d0` | loop/face bookkeeping | – | read |
| `FUN_140200ff0`, `FUN_140201fb0` | face re-mapping after mirror | 526, 680 | read fully |
| `FUN_140225640`, `FUN_140225ff0`, `FUN_140225820`, `FUN_140231b10`, `FUN_140225af0` | cube create/build wrapper/clean | – | read (clean summarised) |
| `FUN_140226150` | BuildBucket | 5395 | read fully (types other than 1 summarised) |
| `FUN_1403fed00`, `FUN_1403ff890`, `FUN_1403ff970`, `FUN_140406360`, `FUN_140406300`, `FUN_140405c80` | locator init, divisions, box, bounds | – | read fully |
| `FUN_140401050` | insert polygon | 3037 | read fully |
| `FUN_140240d80`, `FUN_14023aa90` | polygon normal, near-zero test | – | read |
| `FUN_140454110`, `FUN_1404537f0`, `FUN_140453010`, `FUN_140453e80`, `FUN_1404531b0`, `FUN_140453300`, `FUN_1404535e0`, `FUN_140453850`, `FUN_140453c40` | pcube (Green & Hatch) | – | read (matched to pcube.c) |
| `FUN_140228c60`, `FUN_140240b40` | SearchPoly, box overlap | 2746, 569 | read fully |
| `FUN_140229f30`, `FUN_140405540`, `FUN_140405720`, `FUN_1404056c0`, `FUN_140405150` | result list | – | read |
| `FUN_1404cd2e1` | bilinear analytic | 3450 | read fully + listing (accept loop, disc) |
| `FUN_1404ce05b` | bilinear fallback | 2433 | read fully + listing |
| `FUN_1404b3878`, `FUN_1404b3f48`, `FUN_1404b3960`, `FUN_1404b3ab0/ab8`, `FUN_1404b3b30`, `FUN_1404b4898` | Fortran atan2/atan/sin/cos/fabs | – | listing, transcribed |
| `FUN_14033ffe0`, `FUN_140340273` | 2D samplers | 659, 1380 | read fully |
| `FUN_14030bba0` | segment kernel | 3271 | read fully (decompile) |
| `FUN_1403407d7`, `FUN_14032bd2a`, `FUN_14032beef`, `FUN_1404d71d7` | arc kernel, span, angle, rotate | – | read fully |
| `FUN_140341100`, `FUN_14034151e`, `FUN_1404b3d50`, `FUN_1404d93ae` | arc contact refinement | 1054, 4576 | summarised (open 2) |
| `FUN_14033ffb8`, `FUN_1403460c2`, `FUN_1403460e7`, `FUN_140437b3b`, `FUN_14033fe8e`, `FUN_14033f8d4` | 2D context, min/max, area, root | – | read fully (+ listing of the root test) |
| `FUN_1401613a0`, `FUN_14015ec80`, `FUN_14015ef60`, `FUN_14015f790`, `FUN_140158f70`, `FUN_1401618b0`, `FUN_1401619c0`, `FUN_140161ac0`, `FUN_140180390`, `FUN_14018bd30`, `FUN_14018beb0`, `FUN_140165500` | object backup/restore | – | read (sub-copies summarised) |

About 95 functions followed.

## 9. Open items

1. ~~pcube (`FUN_140453010`..`FUN_140453c40`) matched to `pcube.c` by structure only~~ — closed:
   transcribed from the decompile and the listing in `port/defcmd/src/ifd/cube/pcube.cpp`
   (p67-2). Structure and constants as in 2.3; FP order from the listing. For NaN inputs the
   listing differs from the decompile in three places (sign of a NaN normal component is +1 in
   `FUN_140453e80`/`FUN_140453c40`, axis choice in `FUN_140453850`); the port follows the
   listing. Also from the listing: the query clamp `v = lo > c ? lo : (c > hi ? hi : c)` keeps a
   NaN coordinate (2.4), and the normalisation in `FUN_140239b40` zeroes a NaN vector.
2. 2D arc contact refinement `FUN_140341100`/`FUN_14034151e` (4.5) and the Fortran `ACOS`
   `FUN_1404b3ba4`, sort `FUN_1404d93ae`: only reached when an arc hit is within 1e-7 of a sample
   point; not transcribed (decompile in `arc2.c`).
3. ~~`FUN_14030bba0`, `FUN_1403407d7`: operation order taken from the decompile only~~ — closed: the FP
   audit (`fp-audit.md`) checked both in the listing; every grouping in 4.3 / 4.4 matches.
4. CRT `atan`/`atan2` (`FUN_1404865ac`, `FUN_140485d6c`) used for the 2D rotation angle θ, and CRT
   `pow` (`FUN_140483f60`, bucket spacing — only its truncated integer use matters there).
5. `DAT_140b84748` (2D tolerance override) assumed 0.0; writers not searched.
6. Element numbers of FNDBND for meshes with degenerate bricks (1.3 caveat) — whether DEFORM ever
   stores such meshes in the 8-node layout was not checked in the corpus.

## 10. Note for the parent (do not lose)

* OBJPOS_interference.md: §5.7 should link to section 1 here (and its "3-node faces padded with
  0" is wrong: padded with `F(1)`); §13 items 1-6 are closed by sections 1, 2, 3, 4, 3.3 (sin/cos
  part of item 5) and 5; the 2D quirk of 4.6 (area 0 % always fails) contradicts the prediction
  in `tests/commands/OBJPOS/ifd_m2_2d.KEY` ("touch") — that header (and any other 2D case with
  token 2 = 0) should expect "Object %d to Object %d interference positioning failed." unless an
  oracle run says otherwise. §5.10 / §5.11 "open item 3" can point to section 3.
* OBJPOS.md: §6.5 step 4 and open item 4 can link to 1.7 (face order = FNDBND + VS2008 qsort;
  re-mapping rule); `FUN_14032d65e` is the 2D counterpart (not followed).
* `FUN_1404b3960` sin/cos and `FUN_1404b3878` atan2 are the Fortran runtime versions used by many
  Fortran modules (also `FUN_14033f2bf`, `FUN_1404d71d7`); worth a shared C++ helper.
