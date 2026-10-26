# CNVT3D: 2D→3D geometry generation, brick sweep, 3D mesh build and read-back

Companion to [CNVT3D.md](CNVT3D.md) (driver, files, messages). This spec covers the algorithms the
driver calls per object: the brick sweep (kind 4) to porting depth, the NBCD.OUT round trip, the
state-variable transfer, the axis snapping, the geometry generators (structure, parameters and the
FIXGEO trigger) and two small helpers. Addresses: DEF_PRE_64.exe v11.0.2.

Sources: `deform-rea-work/out/p89/cnvt3d.c`, `out/p89/d/{f8f70,fc0a0,sweep,geo,fx}.c`; own Ghidra
copy `ghidra_p89f4`, outputs in `deform-rea-work/out/p89f/cnvt/` (`l_148f70.txt`, `l_14c730.txt`,
`l_141580.txt`, `l_147b70.txt`, `l_1c6e40.txt` listings; `h1.c`, `h2.c`, `rot.c` helper
decompiles). Constants were read from the PE file (table in §12).

Status labels: **[code]** read from the decompile/listing; **[oracle]** confirmed by a case in §11;
**[inferred]** not proven.

## 1. Notation and constants

* `obj` = 2D source object (`cvt+8`), `NN = obj+0x880` nodes, `NE = obj+0x884` elements,
  `XY = obj+0x8a8` (2 doubles per node: r, z), `CONN = obj+0x9e8` (4 ints per element, 1-based).
* `model2 = cvt+0x3f48` (2D model), `model3 = cvt+0x3f50` (new 3D model).
* `PI = 3.141592653589793` (`DAT_14051f230`), `D180 = 180.0` (`DAT_14051f228`),
  `D360 = 360.0` (`DAT_14051f210`), `F09 = 0.9` (`DAT_14051f220`).
* `cos`/`sin` = `FUN_140bb3490`/`FUN_140bb3000`. [oracle] They return the correctly rounded
  values for the tested arguments, for example `cos(1.5707963267948966) = 6.123233995736766e-17`.
  So the CRT `std::cos`/`std::sin` reproduce them.
* `fabs` = `FUN_140484558`, `floor` = `FUN_140485120`, `floorf` = `FUN_140485048`.
* The lists in the sweep (`FUN_140159860` ctor, `FUN_1401599e0` push_back, `FUN_140159a40` front,
  `FUN_140159970` pop_front, `FUN_1401599b0` empty, `FUN_140159ab0` size) behave as
  `std::list<int>`.

## 2. Entry: `FUN_140147b70(cvt, id, nsec, m, start, ang, c, kind)` for kind 4

Argument registers from `l_147b70.txt`: `nsec = [rsp+0x9840]` (MSHSEC n, `obj+0x2c28`),
`m = [rsp+0x9848]` (MGNELM 2nd int, `obj+0x2d8c`), `start`/`ang` (revolve: GEO23 b/a; extrude:
b/a = start/length), `c = cvt+0x4770` (revolve: GEO23 c `obj+0x2c00`; extrude 0).

```
kd = FUN_1401ee4c0(model2+0x750);                       // 1 revolve, 2 extrude
if (obj.geo23mode != kd && 0 < obj.geo23mode && obj.geo23mode < 3) kd = obj.geo23mode;
FUN_140148840(cvt, obj, model2, model2+0x340);         // cvt+0x18 = copy of obj, cvt+0x10 = &cvt+0x18
if (kind == 4) {
    if (obj+0x2d80 != obj+0x884) {                      // MGNELM 1st int != element count
        int q  = m / (nsec - 1);                        // IDIV, C truncation; nsec==1 -> divide fault
        int t2 = (int)floorf((float)q);                 // CVTDQ2PS, floorf, CVTTSS2SI: == q
        FUN_140148960(cvt, t2);                         // §2.1
    }
    if (kd == 1) FUN_140148f70(cvt, nsec, start, ang, c, cvt+8);   // §3
    else if (kd == 2) FUN_14014c0a0(cvt, nsec, start, ang, cvt+8);  // §4
}
```

So `m` is the target **3D** brick element count, and the 2D remesh aims at `m / (nsec-1)` quads.
The divisor is MSHSEC n, not the even-rounded section count used by the revolve (§3).

### 2.1 `FUN_140148960(cvt, target)`: 2D remesh and interpolation

```
FUN_1401fc730(cvt->obj, &true);             // boundary geometry from the mesh
cvt->obj+0x2d80 = target;
FUN_140144a70(cvt, &false);                 // 2D AMG remesh (DEFAMG.md / CNVT3D.md §5.3)
FUN_14013d7e0(10, model2+0x888);            // DEF_INT_3DTET.DAT, 10 lines "%1d\n"
FUN_140146050(cvt);                         // DEF_INT interpolation (CNVT3D.md §6.3)
cvt->obj = FUN_1401d6ea0(model2, (cvt+0x10)->id);   // re-find the object after the reload
FUN_14013d7c0();                            // delete DEF_INT_3DTET.DAT
```

Only these decisions are made here: remesh whenever the element count differs from `+0x2d80`
(the previous target, so a second CNVT3D with the same settings does not remesh again), always
interpolate after a remesh, never interpolate otherwise. Neither return value is tested: a failed
remesh or interpolation still leads to the sweep of whatever mesh the object then has.

## 3. Revolve sweep `FUN_140148f70(cvt, nsec, start, ang, c, obj)` [code, oracle]

```cpp
FUN_140155340(cvt, obj, cvt->c);        // §7 axis snapping of the 2D source (uses cvt+0x4770)
remove("NBCD.OUT");                     // FUN_1401b85d0
bool noAxis = true;                     // local_300
double angE = ang;  if (D360 < ang) angE = D360;      // min(ang, 360)
int n = nsec;  if (n == 0) n = 2;
if (n % 2 == 1) n = n + 1;              // C remainder: odd positive -> even; MSHSEC default 21 -> 22
double dth = ((angE * PI) / D180) / (double)n;        // local_2b8
double th0 = (start * PI) / D180;                     // dVar16
Pt3   *P      = alloc((n+1)*NN);         // {x,y,z} doubles, layer s node p at P[s*NN + p-1]
int   (*E)[8] = alloc(n*NE);             // element e of section s at E[(s-1)*NE + e-1]
int   *axis   = alloc(NN);
char  *scaled = alloc((n+1)*NN);
list<int> keep, node2D, elem2D, unused;  // local_2e8, local_2c0, local_2f8, local_308

for (p = 1; p <= NN; ++p) {                           // layer 0
    double r = XY[2p-2];
    if (c <= r) { axis[p-1] = 0;
        P[p-1].x = XY[2p-2] * cos(th0);
        P[p-1].y = XY[2p-2] * sin(th0); }
    else        { axis[p-1] = 1; P[p-1].x = 0; P[p-1].y = 0; noAxis = false; }
    P[p-1].z = XY[2p-1];
    keep.push_back(p-1);  node2D.push_back(p);
    for (L = 1; L <= n+1; ++L) scaled[NN*(L-1) - 1 + p] = 0;
}
for (s = 1; s <= n; ++s) {
    for (p = 1; p <= NN; ++p) {
        Pt3 &Q = P[s*NN - 1 + p];
        if (P[p-1].x == 0.0 && P[p-1].y == 0.0) { Q.x = 0; Q.y = 0; }
        else { Q.x = XY[2p-2] * cos(dth*(double)s + th0);   // multiply, then add th0
               Q.y = XY[2p-2] * sin(dth*(double)s + th0); }
        Q.z = P[p-1].z;
        if (axis[p-1] == 0) {
            if (D360 <= angE) { if (s < n) { keep.push_back(s*NN - 1 + p); node2D.push_back(p); } }
            else              {             keep.push_back(s*NN - 1 + p); node2D.push_back(p); }
        }
    }
    for (e = 1; e <= NE; ++e) {
        const int *cn = &CONN[4*(e-1)];                    // cn[0..3]
        auto Z  = [&](int nd){ const Pt3 &q = P[s*NN - 1 + nd]; return q.x == 0 && q.y == 0; };
        bool hit = !noAxis && ( Z(cn[3]) || Z(cn[2]) || Z(cn[1])
                                || P[s*NN - 1 + cn[0]].x == 0 );  // 4th test compares x twice [listing]
        int *Ee = E[(s-1)*NE + e-1];
        if (hit) {
            if (s % 2 == 1) { for (i = 0; i < 8; ++i) Ee[i] = 0; continue; }   // dropped, not listed
            int k = 0;                        // first k with cn[k] off axis and cn[k+1] on axis
            while (Z(cn[k%4]) || !Z(cn[(k+1)%4])) ++k;      // never ends if no such k exists
            for (j = 0; j < 4; ++j) {          // order k, k+1, k+2, k+3
                int i = s*NN - 1 + cn[(k+j)%4];
                if (!scaled[i] && (P[i].x != 0 || P[i].y != 0)) {
                    P[i].x = P[i].x * F09;  P[i].y = P[i].y * F09;  scaled[i] = 1; } }
            if (s == n) for (int j : {0, 3}) {   // layer 0, no on-axis test
                int i = cn[(k+j)%4] - 1;
                if (!scaled[i]) { P[i].x *= F09; P[i].y *= F09; scaled[i] = 1; } }
            Ee[0] = (s-1)*NN + cn[k%4];      Ee[1] = s*NN + cn[k%4];
            Ee[2] = s*NN + cn[(k+3)%4];      Ee[3] = (s-1)*NN + cn[(k+3)%4];
            for (j = 0; j < 4; ++j) Ee[4+j] = (s-2)*NN + cn[(k+j)%4];
            for (i = 0; i < 8; ++i) {
                if (P[Ee[i]-1].x == 0 && P[Ee[i]-1].y == 0) Ee[i] = Ee[i] % NN;
                if (Ee[i] == 0) Ee[i] = NN; }
            elem2D.push_back(e);
        } else {
            for (i = 0; i < 4; ++i) Ee[i]   = (s-1)*NN + cn[3-i];   // reversed 2D order
            for (i = 0; i < 4; ++i) Ee[4+i] =  s   *NN + cn[3-i];
            elem2D.push_back(e);
        }
    }
}
if (D360 <= angE)                                     // close the ring
    for (q = (n-1)*NE; q <= n*NE; ++q)                // 1-based q, so 0-based (n-1)*NE-1 .. n*NE-1
        for (i = 0; i < 8; ++i) { int v = E[q-1][i];
            if (n*NN < v) E[q-1][i] = (v % NN == 0) ? NN : v % NN; }
if (angE < D360) write NBCD.OUT (§5);
FUN_14014c730(cvt, obj, noAxis, th0, dth, n, P, E, axis, keep, unused, node2D, elem2D);  // §6
```

Notes:

* **Axis criterion.** A node is an axis node when `r < c` (signed, so every negative r counts as
  well). With `c = 0` (the GEO23 default) no node is an axis node. Nodes at r = 0 then get the
  position (0,0) in every layer and are **duplicated** per layer [oracle `axis_c0`].
* **Sections and layers.** There are n sections, where n is the even-rounded MSHSEC count, and n+1
  node layers. Layer 0 is at `th0`. Layer s is at `dth*s + th0`, computed by multiplication, not
  by accumulation. For 360° the last layer is computed but not kept.
* **Axis elements.** An element that has an axis node in layer s is dropped at odd s. At even s it
  becomes one hexahedron that spans sections s-1 and s. Its corner nodes are in layers s-1 and s.
  The (k+1, k+2) edge collapses onto the layer-0 axis nodes, and nodes `cn[k]`, `cn[k+3]` repeat
  from layer s-2. The off-axis nodes of the element in layer s are pulled to 0.9·(x, y), once per
  node. When s = n, the layer-0 nodes `cn[k]` and `cn[k+3]` are pulled as well. Odd n is
  impossible because of the rounding above, so axis elements always pair up.
* **Hang.** `hit` can be true without any real axis node, through the 4th test (x == 0 alone)
  or a degenerate element with all four nodes on the axis. The `k` search then never ends.
* **No rotation of the 2D order.** Regular elements take the 2D nodes in reverse order, which keeps
  positive volume (r, θ, z right-handed).

## 4. Extrude sweep `FUN_14014c0a0(cvt, nsec, start, len, obj)` [code, oracle]

```cpp
if (nsec <= 1) return;                       // nothing: the 3D object keeps the geometry only
double dz = len / (double)(nsec - 1);        // param_4 overwritten
double *X = alloc(nsec*NN*3);  int *C = alloc((nsec-1)*NE*8);
for (s = 1; s <= nsec; ++s) {
    for (p = 1; p <= NN; ++p) { int i = (s-1)*NN - 1 + p;
        X[3i] = XY[2p-2];  X[3i+1] = XY[2p-1];  X[3i+2] = start + dz*(double)(s-1); }
    if (1 < s) for (e = 1; e <= NE; ++e) { int *c8 = &C[8*((s-2)*NE + e-1)];
        for (j = 0; j < 4; ++j) { c8[j]   = (s-2)*NN + CONN[4(e-1)+j];
                                  c8[4+j] = (s-1)*NN + CONN[4(e-1)+j]; } }   // 2D order kept
}
FUN_14014fae0(cvt, nsec*NN, (nsec-1)*NE, X, C, obj, dz);   // §6.3
```

The extrude has no snapping, no axis handling, no NBCD.OUT write and no odd/even rounding.
`nsec` is the number of node layers here, so MSHSEC 3 gives 2 element layers. The driver later
rotates the object by +90° about x when CRDSYS != 2 (CNVT3D.md §6.4). The observed rotated
coordinates are in §10.

## 5. NBCD.OUT writer (revolve, `angE < 360`) [code, oracle]

`FUN_1401b1880/18f0(f, "NBCD.OUT")` opens it with `fopen(name, "w")` (text mode, so lines end in
CRLF). `FUN_1401b1bd0` wraps that FILE in a C++ `ostream`. Every line ends with `FUN_140007a50`,
which is `put('\n')` followed by `flush`.

```
a  = Σ axis[p];  N = (NN - a) + NN;
os << "\t" << N << endl;                               // default int format
for (p = 1; p <= NN; ++p) {
    os.setf(left); os.precision(8); os.setf(scientific);   // setf(0x40), precision 8, setf(0x1000)
    if (P[p-1].x == 0 && P[p-1].y == 0)
        os << "\t" << x << "\t" << y << "\t" << z << "\t0\t602\t601\t0\t" << p << endl;
    else {
        os << "\t" << x << "\t" << y << "\t" << z << "\t0\t0\t601\t0\t" << p << endl;  // layer 0
        Pt3 &Q = P[n*NN + p-1];
        os << "\t" << Q.x << "\t" << Q.y << "\t" << Q.z << "\t0\t0\t602\t0\t" << (n*NN + p) << endl;
    }
}
```

Doubles come out as `%.8e` with the old MSVC 3-digit exponent: `9.52500000e+002`,
`6.12323400e-016`, `-8.66025404e+000`. The coordinates are the ones after the 0.9 pull. The 2nd
index printed is the **unrenumbered** id `n*NN + p` (it is matched by position, not by id, §8).
With `c = 0` and nodes at r = 0, `N` still counts them twice but only one line is written for each,
so the file has fewer lines than announced [oracle `axis_c0`: header 12, 10 lines].

## 6. 3D mesh build

### 6.1 `FUN_14014c730(cvt, obj, noAxis, th0, dth, n, P, E, axis, keep, unused, node2D, elem2D)`

```cpp
if (!noAxis) {
    int nE = n*NE;  valid[], newE[];
    for (i = 1; i <= nE; ++i) {
        valid[i-1] = (E[i-1][0]+...+E[i-1][7] < 1) ? 0 : 1;
        newE[i-1]  = (i < 2) ? valid[0] : newE[i-2] + valid[i-1]; }
    a   = Σ_{p<NN} axis[p];
    nn3 = keep.size();  ne3 = newE[nE-1];
    map[(n+1)*NN] = {0};
    for (j = 1; j <= nn3; ++j) { int v = keep.front();
        X3[j-1] = P[v];  map[v] = j;  keep.push_back(keep.front()); keep.pop_front(); }
    for (j = 1; !node2D.empty(); ++j) { N2[j-1] = node2D.front(); node2D.pop_front(); }
    for (i = 1; i <= nE; ++i) if (valid[i-1] == 1)
        for (k = 0; k < 8; ++k) C3[8*(newE[i-1]-1) + k] = map[E[i-1][k] - 1];
    for (j = 1; !elem2D.empty(); ++j) { E2[j-1] = elem2D.front(); elem2D.pop_front(); }
    FUN_14014d730(cvt, nn3, ne3, X3, C3, a, th0, dth, N2, E2);
} else {
    nn3 = keep.size();  ne3 = elem2D.size();
    for (j = 0; j < nn3; ++j) { X3[j] = P[j]; N2[j] = node2D.front(); node2D.pop_front(); }
    for (j = 0; j < ne3; ++j) { C3[j] = E[j];  E2[j] = elem2D.front(); elem2D.pop_front(); }
    FUN_14014d730(cvt, nn3, ne3, X3, C3, 0, th0, dth, N2, E2);
}
```

The resulting numbering, confirmed for every revolve case of §11:

* **Nodes.** Layer 0 holds all NN 2D nodes in 2D order, ids 1..NN. Then come layers s = 1..n, or
  s = 1..n-1 for 360°. Each layer holds its off-axis 2D nodes in 2D order. Without axis nodes,
  node `s*NN + p` is 2D node p in layer s. Axis nodes exist once, in layer 0, so with axis nodes
  the ids are compacted in the same (s, p) order.
* **Elements.** They follow (s = 1..n, e = 1..NE), skipping the axis elements dropped at odd s.
  For 360° the last section reuses the layer-0 nodes.
* **Element node order.** Regular hexahedron: `(L(s-1): c4 c3 c2 c1, L(s): c4 c3 c2 c1)`. Axis
  hexahedron: `(L(s-1)ck, L(s)ck, L(s)ck+3, L(s-1)ck+3, L(s-2)ck, A ck+1, A ck+2, L(s-2)ck+3)`,
  where A is the merged axis node. No wedges or degenerate hexahedra are produced. The 8-node
  record is always a full hexahedron.

### 6.2 `FUN_14014d730(cvt, nn3, ne3, X3, C3, a, th0, dth, N2, E2)`: object and state variables (revolve)

```cpp
geotyp = model2+0x850;                                      // FUN_1401d6d90 (GEOTYP)
src = FUN_1401d6ed0(model2, (cvt+0x10)->id);                // the 2D object (after any remesh)
dst = FUN_14019ccc0(model3, (cvt+0x10)->id);
FUN_140202700(dst, nn3, ne3, 4, model3+0x15f8, 3);          // allocate mesh, dst+0x87c = 1
dst+0xc = (cvt+0x10)+0xc;  strcpy(dst+0x828, (cvt+0x10)+0x828);
FUN_1401d21c0(model3, model2+0x163c);  copy USRNOD names (strncpy 64 each, model+0x33a0)
FUN_1401d2090(model3, model2+0x16e8);  copy USRELM names (model+0x33b0)
FUN_1401d1660(model3, id);             // recompute widths W3[] (FUN_1401d0320), resize arrays
memcpy(dst coords, X3, nn3*24);  memcpy(dst conn, C3, ne3*32);
for (i = 1; i <= ne3; ++i) dst.MTLGRP[i-1] = src.MTLGRP[E2[i-1]-1];   // FUN_140183d00 (+0x9f8)
for (t = 1; t <= 0x2e; ++t) transferSV(DAT_1405caa80[t-1]);           // table below
```

SV id table `DAT_1405caa80` (46 ints; ids as in `dbmap/objarrays.md`, hex):
`01 DRZ, 02 URZ, 03 FRZ, 04 *FRZB, 06 *PRZB, 08 NDTMP, 0c *DTMP, 09 NDHEAT, 0a *HITN, 0b NDFLUX,
10 RHTFLX, 0e CRBFLX, 11 USRNOD, 0d DATOM, 0f VOTAGE, 18 NWEAR, 1a STRESS(n), 1b STRAIN(n),
1c DAMAGE(n), 1d GRAIN(n), 22 TIMERC, 23 NRECVL, 26 STRESS, 27 *EPS, 28 STNCMP, 29 *EFSTS,
2a *EFEPS, 2b STRAIN, 2c DENSTY, 30 *HTGN, 2d DAMAGE, 2e MATAXI, 3c USRELM, 31 VOLFC, 32 VOLFS,
33 DVOLF, 34 TICF, 35 GRAIN, 36 SIZESH, 37 TXTODF, 38 HDNOBJ, 39 YLDS, 3a ZEFI, 3b ZMFI, 3e CURRNT,
3f ERECVL`. RZ (00) is not in the table; coordinates come from §3.

`transferSV(id)`:

```cpp
w2 = model2->W[id];  if (w2 == 0) return;                   // W = model+0x15f8+4*id
s = FUN_1401627e0(src, id, &nodal); if (!s) return; d = FUN_1401627e0(dst, id, &nodal); if (!d) return;
cnt = nodal ? nn3 : ne3;   q2 = q3 = 1;                     // integration points
if (id == 0x26 || id == 0x2b || id == 0x2d) {               // STRESS, STRAIN, DAMAGE
    q2 = FUN_140183c20(src elems, j); q3 = FUN_140183c20(dst elems, j);   // j = 2, 1, 0
    if (q3 != 2*q2) q2 = q3 = 1; }
switch: id 0x11 USRNOD (nodal):   d[(i-1)*w2 ..] = s[(N2[i-1]-1)*w2 ..], w2 values, i = 1..cnt
        id 0x3c USRELM, 0x31 VOLFC, 0x32 VOLFS, and 0x28 STNCMP when geotyp == 3:
                                   the same per element with E2
        otherwise: generic below
generic:
  blocks = 1; kind = w2;
  if (id == 0x28 && geotyp != 3) { kind = 4; blocks = 7; }
  for (b = 0; b < blocks; ++b) {
    o2 = o3 = 0;
    if (id == 0x28 && geotyp != 3) {
        tag = {3000, 0xc1c, 0xc80, 0xce4, 0xd48, 0xdac, 0xdac}[b]; if (b >= 5) kind = 1;
        o = FUN_140166200(src, &tag);  o3 = 6*o;  o2 = 4*o;       // STNOUT block offsets
        if (b == 6) { x = FUN_140212460(src, 6); o2 += x; o3 += x; } }
    if (o2 < 0 || o2 >= w2) continue;
    for (i = 1; i <= cnt; ++i) {
      phi = th0;                                            // node of layer 0
      if (!nodal) phi = th0 + dth*(floor((((double)(C3[8i-5] + C3[8i-1] - 2a))
                                          / (double)(src.NN - a)) / 2.0) + 0.5);
      else if (src.NN < i) phi = th0 + dth*floor((double)(i - a) / (double)(src.NN - a));
      w3 = model3->W[id];
      for (g = 1; g <= q2; ++g) switch (kind) {
      case 1:  // scalars
        if (nodal) d[i-1] = s[N2[i-1]-1];
        else if (id == 0x2e && FUN_1401d64f0(model3, 0x2e, 0)) {            // MATAXI 3D
            al = (s[E2[i-1]-1] * PI) / D180;   R = d + 9(i-1);
            R[0] = cos(al)*cos(phi); R[1] = cos(al)*sin(phi); R[2] = sin(al);
            R[3] = cos(phi + PI/2);  R[4] = sin(phi + PI/2);  R[5] = 0;
            R[6] = cos(al + PI/2)*cos(phi); R[7] = cos(al + PI/2)*sin(phi); R[8] = sin(al + PI/2); }
        else if (id == 0x28) d[(i-1)*w3 + o3] = s[(E2[i-1]-1)*w2 + o2];
        else { d[(i-1)*q3 + g-1] = s[(E2[i-1]-1)*q2 + g-1];
               if (q3 == 2*q2) d[(i-1)*q3 + q2 + g-1] = d[(i-1)*q3 + g-1]; }
        break;
      case 2:  // 2D vectors (r, z); src row = nodal ? N2[i-1] : E2[i-1]
        if (w3 == 3) { d[3(i-1)] = s[(row-1)*w2]*cos(phi); d[3(i-1)+1] = s[(row-1)*w2]*sin(phi);
                       d[3(i-1)+2] = s[(row-1)*w2 + 1]; }
        else if (w3 == 2) { d[2i-2] = s[(row-1)*w2]; d[2i-1] = s[(row-1)*w2 + 1]; }
        break;
      case 4:  // 2D tensors (rr, zz, tt, rz), only when id == 0x28 or w3 == 6
        c = cos(phi); sn = sin(phi);
        Rm = {{c, -sn, 0}, {sn, c, 0}, {0, 0, 1}};          // -sn = sin(phi) XOR sign bit
        T  = {{t0, 0, t3}, {0, t2, 0}, {t3, 0, t1}};        // t_k = s[row, comp o2+k, point g]
        if (id == 0x27 || id == 0x28) T /= 2.0 (each entry);
        M  = Rm*T  (each entry a*b + c*d + e*f, left to right);
        out = {M0·R0, M1·R1, M2·R2, M0·R1, M1·R2, M0·R2};   // xx yy zz xy yz xz
        d[(i-1)*w3*q3 + (o3+k)*q3 + g-1] = out[k], k = 0..5;
        if (q3 == 2*q2) copy each to point g + q2;
        if (id == 0x27 || id == 0x28) d[(i-1)*w3 + o3 + k] *= 2.0, k = 0..5;
        break;
      case 6:  // 6-component (GEOTYP 3) tensors: copied per point, not rotated
        d[(i-1)*6*q3 + k*q3 + g-1] = s[(E2[i-1]-1)*6*q2 + k*q2 + g-1], k = 0..5 (+ copy to g+q2)
      } } }
```

Index details of the tensor read: `t0 = s[(o2+0)*q2 + g-1 + (E2-1)*w2*q2]`,
`t3 = s[(o2+3)*q2 …]`, `t2 = s[(o2+2)*q2 …]`, `t1 = s[(o2+1)*q2 …]`. The nodal variant reads
`s[(N2-1)*w2 + o2 + k]`. Widths other than 1, 2, 4 and 6 (NWEAR 5, DVOLF, TICF, grain fields with
other widths) are **not copied**. Their 3D arrays keep the values from `FUN_1401d1660`.

The section angle `phi` reproduces two quirks of the binary [oracle]:

* Element `phi` uses nodes 4 and 8 of the new hexahedron, so it is the mid-angle of section s or of
  section s+1, depending on the node numbers. Revolve 90/n=2: elements 1 and 2 → 22.5°, elements
  3 and 4 → 67.5°.
* Node `phi` uses `floor((i-a)/(NN-a))`, so the **last node of each layer** gets the angle of the
  next layer. Revolve 90/n=2, NN=6: node 12 has DRZ (6cos90°, 6sin90°), node 18 has 135°.

### 6.3 `FUN_14014fae0(cvt, nn3, ne3, X, C, obj, dz)`: object and state variables (extrude)

Object creation, names, width reset and copies as in §6.2. The 2D row is taken modulo the 2D
count instead of from lists: element row `r = i % NE` and node row `r = i % NN`, with `0 → NE`
(`NN`). The rules are the same with `phi` absent:

* kind 1: scalars copied. MATAXI in 3D: `{cos al, sin al, 0, cos(al+PI/2), sin(al+PI/2), 0, 0, 0, 1.0}`.
* kind 2, `w3 == 3`: `(v0, v1, 0)`. `w3 == 2`: `(v0, v1)`.
* kind 4 (`id 0x28` or `w3 == 6`): `{t0, t1, t2, 0, 0, t3}` (xx, yy, zz, xy, yz, xz). There is no
  /2 and ×2 pair.
* kind 6: copied.

MTLGRP: `dst[i-1] = src[(i % NE ? i % NE : NE) - 1]`.

## 7. Axis snapping `FUN_140155340(cvt, obj, c)` and `FUN_140154f20` [code, oracle]

Runs when `c != 0`, `obj` is non-null and GEO23 mode == 1. Called from the revolve sweep and from
`FUN_140153710` (geometry).

```cpp
if (obj.hasMesh) {
    if (obj.geo23k == 0 && obj+0xbd8 > 0) {          // GEO23 k (+0x2c08) and boundary loops
        vector<int> ids;  FUN_140154f20(cvt, c, ids, obj);
        for (id : ids) XY[2*id-2] = 0;
    } else for (p = 1..NN) if (fabs(XY[2p-2]) < c) XY[2p-2] = 0;
}
if (FUN_140160d20(obj))                              // geometry loop 0 exists
    for (j = 1..geo.count) if (fabs(geo.pt[3(j-1)]) < c) geo.pt[3(j-1)] = 0;   // container +0x14/+0x18
```

`FUN_140154f20` (k == 0 path). The loops are at `obj+0xbe0 + 0x30*i` (+0 count, +8 node array).
For every loop and every position j < count-1 whose node has `fabs(r) < c`: the first hit sets
`top = bot = j`. After that, a node with `z > z(node[top])` sets `top = j`, and a node with
`z < z(node[bot])` sets `bot = j`. The indices refer to the current loop's array. Then, for
**every** loop: `L = count-1`; `if (bot < top) bot += L` (cumulative over the loops); push
`node[j % L]` for `j = top..bot`. With a counter-clockwise loop this is the boundary path from the
highest to the lowest near-axis node, which is the axis edge. Interior nodes are not snapped on
this path. They still count as axis nodes in §3 because `r < c` there.

The snapping modifies the **2D source object**, so the KFWRIT after CNVT3D shows r = 0 [oracle
`snap_k0`, `snap_k1`: r = 0.0005 → 0].

## 8. Read-back `FUN_140141580(cvt)`: NBCD.OUT → node BCs [listing]

```cpp
FILE *f = fopen("NBCD.OUT", "rt");  if (!f) return -1;
int best = -1;
double tol = FUN_140202a20(cvt->obj);  tol = tol*tol;
fgets(buf, 0x400, f);  sscanf(buf, "%d\n", &cnt);
for (l = 0; l < cnt; ++l) {
    fgets(buf, 0x400, f);                    // at EOF buf keeps the previous line
    sscanf(buf, "%lf %lf %lf %d %d %d %d %d\n",
           &xyz[0], &xyz[1], &xyz[2],        // rsp+0x4b0, 0x4b8, 0x4c0
           &c[0], &c[1], &c[2],              // rsp+0x4c8, 0x4cc, 0x4d0  -> the 3 BC codes
           &u4, &u5);                        // rsp+0x498, 0x488 -> read, never used
    double dmin = 1e30;  best = -1;          // DAT_140509b50
    for (i = 0; i < obj.NN; ++i) {
        double d2 = (x-X[i].x)^2 + (y-X[i].y)^2 + (z-X[i].z)^2;   // FUN_14023b850, this order
        if (d2 < dmin) { dmin = d2; best = i; } }               // COMISD, strict
    if (tol < dmin || best < 0) continue;                      // JC: dmin <= tol applies
    for (j = 0; j < 3; ++j)
        if (c[j] / 100 == 6) { BCCDEF[3*best+j] = 6; BCCDFN[3*best+j] = c[j] - 600; }
        else                   BCCDEF[3*best+j] = c[j];         // BCCDFN left unchanged
}
fclose(f);
if (obj.type != 0) FUN_140257c80(obj+0x2e80, FUN_140158e60(FUN_140198620(obj, &0)));  // SYMSUF rebuild
for (f = 0; f < FUN_140165170(obj); ++f) {                      // boundary faces
    FUN_140201210(obj, f, &n1, &n2, &n3, &n4);                  // obj+0xbf8[4f..4f+3]
    bool shared = false;
    for (j = 0; j < 3 && !shared; ++j) {
        int code = BCCDEF[3(n1-1)+j];  if (code != 6 && code != 7 && code != 8) continue;
        int sub  = BCCDFN[3(n1-1)+j];
        bool in2 = any i<3: BCCDEF[3(n2-1)+i] == code && BCCDFN[3(n2-1)+i] == sub;  if (!in2) continue;
        bool in3 = any i<3: BCCDEF[3(n3-1)+i] == code && BCCDFN[3(n3-1)+i] == sub;  if (!in3) continue;
        shared = true; }
    if (!shared) { obj+0xb00[f] = 2; obj+0xb18[f] = 2; }        // ECCTMP = ECCATM = 2
}
if (obj.type == 0) for all nodes, j<3: BCCDEF = 0, BCCDFN = 0;
return 0;
```

* The sscanf field map is now exact: the 4th, 5th and 6th numbers are the x, y, z BC codes. The 7th
  and 8th (`0`, node id) are read and ignored, so matching is purely geometric.
* `FUN_140202a20` gives the tolerance before squaring. A 3D model uses `FUN_140202a70`: the
  shortest edge of the boundary faces (`+0xbf0` faces, `+0xbf8` 4-int records). A face is a
  triangle when record 0 has `n0 == n3`. The start value is 1e29 (`DAT_14052c598`), and the
  result is 0 when there is no mesh, no loop (`+0xbd8 < 1`) or no face. A 2D model uses
  `FUN_1403046a0`, the shortest element edge.
* Codes are **overwritten** for all three directions, so a direction coded 0 in NBCD.OUT clears a
  BC the node had before.
* The extrude path also calls this function. NBCD.OUT is deleted only by the command start
  (`FUN_14013e880`) and by the revolve sweep, so an extrude converted after a revolve in the same
  CNVT3D reads the revolve's stale file [inferred from the call order; not run].

The KEY written by the save then shows the result [oracle]: `BCCDEF obj n 0` with
`node 0 0 6` (start/end face) or `node 0 6 6` (axis). `BCCDFN` gives `0 0 1` / `0 0 2` / `0 2 1`.
`ECCTMP`/`ECCATM` list the non-symmetry boundary faces with value 2. Two `SYMSUF` entries appear,
each with a point and a normal. For revolve 90° they are y = 0 with (0,-1,0), and the 90° plane
with (-1, 6.1232339957e-17, 0).

## 9. Driver steps around the sweep (kind 4) [code]

After `FUN_140147b70(…, 4)` returns (`FUN_1401467f0`, lines 238-256 of the decompile):

```cpp
o3 = FUN_14019ccc0(model3, id);
if (o3.hasMesh) {
    saved = cvt->obj;  cvt->obj = o3;
    o3+0x2d94 = 1;                                      // MGNELM flag of the 3D object
    if (o3.hasMesh && o3+0xbf0 == 0) { FUN_1401fc290(o3); FUN_1401fc730(o3, &true); }  // faces, DIEGEO
    if (obj.geo23mode == 1 && ang < 360.0) FUN_140198d30(o3, start, ang);
    FUN_140141580(cvt);                                  // §8
    cvt->obj = saved;
}
```

`FUN_140198d30(o3, start, ang)`:

* `ang >= 360`: only `FUN_1402059a0(o3, 0)`, which rebuilds the symmetry-surface list.
* Otherwise it computes plane normals `n1 = (cos((start+90)·PI/180), sin(…), 0)` and
  `n2 = (cos((start+ang+90)·PI/180), sin(…), 0)` through the origin.
* For every face of the object's geometry (`o3+0x3c00`), when all three first nodes are within
  1e-8 (`FUN_14023c770`, `|n·(p-o)|`) of n1, or all within 1e-8 of n2, the face's 4th entry is
  **negated**. These are the `-n` entries in the DIEGEO face list of the KEY.
* Then `FUN_1402059a0(o3, 0)` runs. Every symmetry surface whose normal is not parallel to n1 or
  n2 (`fabs(fabs(dot) - |a||b|) >= 1e-6`) is removed (`FUN_140257f70`). If one was removed,
  `FUN_140205e40(o3, 0)` runs.

The DIEGEO the KEY shows for a brick object is this boundary of the brick mesh. Its points are all
mesh nodes and its faces are the boundary quadrilaterals. It is not the geometry made by §10.2.

## 10. Geometry generation (kinds 5 and 2) [code; structure only below the primitive]

### 10.1 `FUN_140153710(cvt, kd, start, ang, obj, mg, c)`

```cpp
FUN_140155340(cvt, obj, c);                              // §7
o3 = FUN_14019ccc0(model3, (cvt+0x10)->id);
if (FUN_140160d90(o3)) { FUN_140148840(mg, o3, model3, model2+0x340); return; }  // test not derived
if (!obj) return;
if (obj.outputMesh == 0 || !obj.hasMesh) {
    if (!FUN_140160cc0(obj)) { printf("2D DB is not correct!"); return; }
    FUN_140153c00(cvt, obj, o3, model3);                 // §10.2 from the boundary geometry
} else FUN_140154270(cvt, obj, o3, model3);              // §10.3 from the mesh boundary
if ((prev = FUN_1401d8120(model2)) && (po = FUN_1401d6ea0(prev, id)))
    FUN_140189b70(o3+0x2d10, po+0x2d10, &1, &1);         // mesh windows from the previous step
if (kd == 1) FUN_140198d30(o3, start, ang);              // §9
FUN_140148840(mg, o3, model3, model2+0x340);             // mg now works on the 3D object
```

`FUN_140153910(cvt, kd, start, len, mgObj)` (tet path only) runs when there is a mesh-window list
with `+0x2d40 > 0`. It acts on the windows with `+0x210 == 3`, and only for `kd == 2`. A window
point with `|z - start| < 1e-4` gets `z -= 0.001·|len|`. Otherwise, a point with
`|z - start - len| < 1e-4` gets `z += 0.001·|len|`.

### 10.2 `FUN_140153c00(cvt, obj, o3, model3)`: from the 2D boundary geometry

1. `prim` = GEOPRIM object on the stack (`FUN_1402b7ad0`, freed by `FUN_1402b8090`).
2. `geo = FUN_140197f40(obj, 0)`. The function returns false when `geo` is missing, has no data
   (`FUN_140159160`), or loop 0 (`FUN_14027ff20(geo, 0)`) is missing or has `count < 1`.
3. Loop cleanup:
   * if `FUN_140159110(loop)` is false, `FUN_140277830(loop)`;
   * `FUN_1401590d0(FUN_140159150(loop), 0.01)`, a point merge with tolerance 0.01;
   * `FUN_1402778c0(loop)`;
   * a last point equal to the first is dropped;
   * every point with `fabs(r) <= 0.01·0.1` gets r = 0.
4. Revolve (`mode == 1`): `FUN_1402b8980(prim, 1, 0, &obj.b /*start*/, &obj.a /*ang*/,
   axisDir = (0,0,1), axisPt = (0,0,0), &obj.geosec, &npts, pts, &0, 0, &0, 0, &0, 0)`.
   This sets `prim+0x2330 = 2`, `+0x23a0 = 1`, `+0x23a4 = 0` and the revolve record at
   `prim+0x2358` (0x888 bytes): `+0x858` start, `+0x860` angle, `+0x840` normalised axis direction
   (`FUN_140239800`), `+0x828` axis point, `+0x868` sections. Then `FUN_1402dd0d0`, profile
   `FUN_1402e4080(prim+0x1700, &npts, pts)`, `FUN_1402e9aa0`, `FUN_1402e94d0/9500(prim+0x1720, &1, …)`.
5. Extrude: `FUN_1402b8520(prim, 1, 0, &obj.a /*len*/, &2, pts, &npts, …, dir (0,0,1) default)`.
   This sets `prim+0x2330 = 1` and the extrude record `prim+0x2350` (0x850 bytes): `+0x820` length,
   `+0x828` count 2, `+0x808..+0x818` direction.
6. `FUN_1402ba500(prim, o3, &false, model3)`, §10.4.

### 10.3 `FUN_140154270(cvt, obj, o3, model3)`: from the 2D mesh boundary

* `sec = obj.geosec` if `>= 1`, else `obj.mshsec`.
* Revolve: `FUN_1402b8980(prim, 0, 2, &start, &ang, (0,0,1), (0,0,0), &sec, &0, conn, XY, &NN,
  &NE, &obj+0xbe0 (loops), obj+0xbe8)`.
* Extrude: `FUN_1402b8520(prim, 0, 2, &len, …, XY, &NN, …, conn, &NE, &loops…)`.
* So `prim+0x23a0 = 0`, `+0x23a4 = 2`: `FUN_1402ce190` skips the surface generator and runs
  `FUN_1402ce290` (mesh-boundary sweep, not decompiled). No FIXGEO is reached from here.
* After `FUN_1402ba500`, when `FUN_1401591b0(prim)` holds, the face list `o3+0xbf8` is copied. A
  face with a negative 4th entry is made positive (`FUN_140484ab8`) and flagged -1. Each face
  takes one entry from the GEOPRIM list `prim+0x2518` [listing, T9-3: the decompile's `local_180`
  is RSP+0x2608 = prim+0x2518, the GEOPRIM being at RSP+0xf0 with 0x2520 bytes]. `FUN_1402ce310`
  pushes one int per face (low 4 bytes of the 8-byte slot): 0 for a collapsed (triangle) face
  (+0x24fc set), 1 for a quad (+0x24fd set); the split runs only when both kinds occur
  (`FUN_1401591b0`). Zero entries are copied as-is; others are split with
  `FUN_14023d410(coords, faces, f+1, 4, t1, t2)` into two triangles whose 4th entry is the first
  node, negated for a flagged face. `o3+0xbe0`/`+0xbf0` are rebuilt from it. The builder's 5th
  argument (ctx) is an uninitialised heap block of NN*(sec+1) ints that `FUN_1402ce310` fills with
  the 2D node of every 3D node (p9-05); freed at the end.
* Revolve: `FUN_14021d2f0(o3, (0,0,0), (0,0,1), -90.0)`.
* Then `FUN_1401fc730(o3, &1)`, `FUN_1401ffd80(o3)`, and `o3+0x2d90 = obj+0x2d80` (the 3D
  MGNELM 3rd field = 2D target count; KEY `MGNELM 2 8000 4 2 1` [oracle]).

### 10.4 `FUN_1402ba500(prim, o3, &flag, model3, ctx=0)`: primitive builder

```cpp
if (!model3) { prim+0x23c0 = new _DB_STEP_DATA(3); prim+0x23b8 = 1; }
else { if (!FUN_1400015f0(model3)) return false; prim+0x23c0 = model3; prim+0x23b8 = 0; }
if (!o3) { prim+0x23b0 = new object(0x3f28) bound to the model; prim+0x23a8 = 1; } else prim+0x23b0 = o3;
prim+0x23d8 = ctx;
ok = FUN_1402ba850(prim);
if (prim+0x2330 == 1) ok = FUN_1402baa60(prim);           // extrude
else if (prim+0x2330 == 2) ok = FUN_1402ce190(prim);      // revolve
if (flag && ok) FUN_1402dc560(prim);
free the owned object/model;  return ok;
```

`FUN_1402ce190`:

* If `rev+0x86c == 0`, `FUN_1402d9bb0` classifies the axis into `t`. `t` 0 or 1 sets
  `rev+0x86c = 1`; any other value sets `rev+0x86c = 2` and `prim+0x24db = 1`.
* Then `FUN_1402d7cd0` when `+0x23a0 != 0`, and `FUN_1402ce290` when `+0x23a4 != 0`.

`FUN_1402d7cd0` (revolve surface generator):

* Clamp the angle `prim+0x2378` to ≤ 360. `|ang-360| < 1e-8` means full revolution.
* `sec = prim+0x2380` (< 1 → fail).
* Axis from `FUN_1402d9bb0`, then **`FUN_1402da250`** (below).
* `FUN_1402caac0` decides whether the sections are uniform. When uniform, `step = ang/sec` and
  layer k is at `step*k`. Otherwise `FUN_1402cac30`/`FUN_1402caf40` compute up to four step
  sizes [open].
* Profile points are mapped to 3D according to the axis case `t` (`(0, r, z)`, `(r, z, 0)`, …).
* Points are rotated by the start angle and, when `+0x24db` is set, by -90°. Both use
  `FUN_140325691`, a rotation about the axis that calls `FUN_1404b3b30` (sincos of
  `(a/45)·0.7853981633974483`).
* `FUN_1402da9f0` runs, then `FUN_1402db0d0` marks the profile points on the axis.
* Layers `1..sec` (`sec-1` for a full revolution) are added by rotating the profile.
* Quadrilateral strips come from `FUN_1402db310` (`+0x23a0 == 1`) or `FUN_1402dbb20`.
* End caps, for a partial revolution only, are triangles from `FUN_14041f830`/`FUN_1402e24f0`.
  They are copied once and once more reversed, with ids shifted by `npts·sec` for points not on
  the axis.
* The result is stored by `FUN_1402dbe70`/`FUN_1402dc160`, and the WPAXIS entry 1 of the object is
  filled (`+4 = 0`, axis point and direction).

**`FUN_1402da250(prim, dir, pt)`** (profile crossing the axis; the FIXGEO trigger) [code]:

```cpp
if (prim+0x1700 < 1 || !prim+0x1708) return;
d = normalize(dir);                                  // FUN_14023b720
if (rev && rev+0x86c == 2) d = (dir[1], dir[2], dir[0]);       // raw, not normalised
if (1e-6 <= fabs(d.z)) { a2 = (d.z, d.x); o = (pt[1], pt[2]); }
else { a2 = (d.x, d.y); o = (pt[0], pt[1]);
       if (rev && rev+0x86c == 2) o = (pt[1], pt[2]); }
pos = neg = 0;
for (i = 1..n) { v = P[i] - o; s = v.x*a2.y - a2.x*v.y;      // FUN_140308ad0
                 if (s > 1e-6) ++pos; else if (s < -1e-6) ++neg; }
if (pos && neg) {
    bool negSide = pos < neg;  bool posSide = !negSide;      // move the minority side
    for (i = 1..n) { v = P[i] - o; s = cross(v, a2);
        if ((s > 1e-6 && negSide) || (s < -1e-6 && posSide)) {
            t = v.x*a2.x + v.y*a2.y;                           // FUN_140308a90
            P[i].x = o.x + t*a2.x;  P[i].y = o.y + t*a2.y;  moved = true; } }
    if (moved) {                                             // local_1c8
        geo tmp; FUN_140220390(par); tmp.count = n; tmp.pts = P (2 doubles each, FUN_1402760e0 stride);
        FUN_14027ece0(&tmp, par, &k, 1, 0);                  // FIXGEO, DXF_IGES_READERS.md §5.4
        if (n < tmp.count) FUN_1402e3fa0(prim+0x1700, &tmp.count); else prim+0x1700 = tmp.count;
        copy the points back; FUN_140220420(par); free tmp;
    }
}
```

When the tie `pos == neg` happens, `negSide` is false, so the negative side is moved. FIXGEO only
runs when a profile point really was projected onto the axis. The CNVT3D die case
(`cnvt3d_geometry_die_obj1`) does not create `NEWGEO.GEO`, consistent with this.

### 10.5 Extrude primitive `FUN_1402baa60(prim)` [listing, T9-3]

```cpp
ok = false;
if (prim.mode_geo  /*+0x23a0*/) ok = FUN_1402bb460(prim);          // geometry path
if (prim.mode_mesh /*+0x23a4*/) {
    ok = FUN_1402bbb10(prim);   // == 1: FUN_1402bd210 (shape roll, not reachable in batch)
                                // == 2: FUN_1402bbb60 (mesh boundary); other values: false
    if (strlen(prim+0x1b30)) FUN_1402bc580(prim);                    // never set in batch
}
return ok;
```

Batch callers set exactly one path: `FUN_140153c00` (§10.2) mode_geo 1 / mode_mesh 0,
`FUN_140154270` (§10.3) 0 / 2. The extrude never calls FIXGEO and never reads the section ratio
`+0x2388`.

**Geometry path `FUN_1402bb460`** (2D boundary loop → closed triangulated surface):

1. `len = prim+0x2360` (the setup copies the record length there). `len < 1e-12` (also NaN:
   `COMISD 1e-12, len; JA`), `n = profile.n < 1` or no points → false.
2. `T` = `3·4n` ints (operator new, saturating size); `ntri`:
   * `FUN_1402e7f40(&profile)` false → `ntri = 0` (no caps, side strips only);
   * else `+0x24b9 == 0` (always in batch: only the ctor writes this byte) →
     `ntri = FUN_14041f830(1, &n, pts, T)` (p9-05's cap triangulator);
   * else `ntri = FUN_1402e24f0(prim, n, pts, T)` (minimum-area triangulation, §10.6).
3. Points `X` (`2n` × 3 doubles): `X[i] = (x, y, len)`, `X[n+i] = (x, y, 0)` for profile point
   i+1, i = 0..n-1.
4. Bottom cap: `T[ntri+k] = (T[k].c + n, T[k].b + n, T[k].a + n)` (reversed), `ntri *= 2`.
5. Side strips, i = 0..n-1 (1-based ids):
   * `i < n-1`: `(i+1, i+1+n, i+2+n)`, `(i+1, i+2+n, i+2)`;
   * `i == n-1`: `profile.u10` set (by the check of step 2): `(n, 2n, n+1)`, `(n, n+1, 1)`;
     else nothing.
6. `FUN_1402dc040(prim, &2n, X, &ntri, T)`: each triangle becomes the quad `a b c a`; then
   `FUN_1402dbe70` stores it: every geometry surface of the object is cleared (`FUN_1402f9340`),
   surface count 1, surface flag 1, `FUN_1402009f0(o3, np, nf, 0)` (allocate surface 0), copy the
   points (3 doubles) and faces (4 ints).
7. `FUN_1402c1130(prim, &0)` orients the object to the extrude direction (below).
8. WPAXIS entry 1 (`FUN_140259bc0(o3+0x20a0, 1)`): `+4 = 1`, `+8` = `X[0]`, `+0x20` = `X[n]`
   (values before the orientation). True.

**Orientation `FUN_1402c1130`** (`d = rec+0x808`, dot products `FUN_14023a380` with ex, ey, ez,
origin (0,0,0)): `|d·ez| > 0.9` → nothing. Else `d·ex > 0.9` → rotate +90° about (0,1,0), then
+90° about (1,0,0). Else `d·ex <= -0.9` → the same two rotations, then translate `(-len, 0, 0)`
(`FUN_14021d0b0(o3, v, &0, &1)`). Else `d·ey > 0.9` → rotate -90° about (-1,0,0), then translate
`(0, len, 0)`; `d·ey <= -0.9` → rotate -90° about (-1,0,0); otherwise nothing. Rotations are
`FUN_14021d2f0(o3, origin, axis, angle)` (p67-1). The default direction (0,0,1) transforms
nothing.

**Mesh-boundary path `FUN_1402bbb60`** (2D mesh → closed shell of 4-node faces, the boundary the
3D mesher gets):

1. `+0x23e0..+0x23ec = 0`, `+0x2360 = len = rec+0x820`. `len < 1e-12` → false. `np` = 2D node
   count (`FUN_1402e9b50`), `xy` = 2D coordinates (`FUN_1402e9b90`); `np < 1`, no `xy`, or
   `ns = prim.sections (+0x2380) < 1` → false.
2. No boundary list (`int_lists.n < 1`) → `FUN_1402cc080` builds it (FL2_FNDBND chains of the 2D
   elements, at most 100 lists). Still none → stdout (`__iob_func()+0x30`)
   `"cross section mesh boundary can not be constructed.\n"`, false.
3. Node layers in `X` (allocated for `(ns+1)·np` points): layer 0 = all `np` nodes `(x, y, 0)`,
   `ctx[k] = k` when `ctx` (+0x23d8) is set. `B` = boundary list 1, `nb` its length; a closed
   list (first == last, `FUN_1402e91d0`) drops its last entry **in place** (the list count is
   decremented). Layers s = 1..ns-1: for each `B[j]`, `X[e] = X[B[j]-1]` with
   `z += (len / (double)ns) · (double)s` (division hoisted, then the product, then the add),
   `ctx[e] = B[j]-1`. Last layer: all `np` nodes with `z = z0 + len`, `ctx[e] = k`. Counters:
   `+0x23e0 = np`, `+0x23e4 = np + (ns/2)·nb`, `+0x23e8 = np + ns·nb`, `+0x23ec = nb`. Node total
   `nn = 2·np + (ns-1)·nb`.
4. Faces (`nb·ns + 2·nel` quads). `flip = FUN_1402bc720(prim, X)`: the unit normal of the first
   2D element (`FUN_140240d80` on its 4 nodes: `Σ cross(p_{k-1}-p0, p_k-p0)`, k = 2..3, normalised
   by `FUN_140239b40`) has `n·ez > 0`. `FUN_1402bc980(prim, &ns, X, Q, &flip, &closed = 0)`
   (the boundary list's closed test runs once more, in place):
   * bottom: the 2D elements, `a`/`c` swapped when `flip`;
   * strips s = 1..ns, j = 0..nb-1, `j'` = j+1 (0 for the last), `b1 = B[j]`, `b2 = B[j']`,
     `base = np + (s-1)·nb`: s = 1: `ns == 1` → `(b1, b1+np, b2+np, b2)`, else
     `(b1, np+1+j, np+1+j', b2)`; s = ns > 1: `(base+1+j-nb, b1+base, b2+base, base+1+j'-nb)`;
     otherwise `(base+1+j-nb, base+1+j, base+1+j', base+1+j'-nb)`; `a`/`c` swapped when `flip`.
     (`closed` != 0 would close the last layer on the 2D ids and skip the caps; batch passes 0.)
   * top: the 2D elements + `np + (ns-1)·nb`, `a`/`c` swapped when **not** `flip`.
5. Object: `FUN_140202700(o3, nn, 0, 4, model+0x15f8, 0)`, coordinates copied to
   `FUN_1401808d0(o3+0x8a0)`, `FUN_140163300(o3, 1)` (one boundary), `FUN_140200a70(o3, nq, 0)`,
   the quads copied to `*(o3+0xbf8)` (16 bytes each).
6. WPAXIS entry 1: `+4 = 1`, `+8` = node 1, `+0x20` = first node of the last layer. True. No
   orientation on this path.

`FUN_1402bc580` (only when `prim+0x1b30` holds a file name, never in batch; needs a mesh
`o3+0x87c != 0` and kind 1) writes, mode "wt": `"0\t0\t0\n"`, node 1 as `"%lf\t%lf\t%lf\n"`, the
direction as `"%lf\t%lf\t%lf\n"`, `"0.0\t0.0\n"`.

`FUN_1402ba500` writes `DEF_GEOPRIM.KEY` (`FUN_1402dc560`) only for a true third argument; the
batch callers pass false.

### 10.6 Minimum-area triangulation `FUN_1402e24f0(prim, n, pts, T)` [listing]

Not reached in batch (needs `+0x24b9`); ported for p9-05 and the profile API. Points
`P[0] = (0,0,0)`, `P[i] = (x_i, y_i, 0)`; vertex list `1..n` (a closing `1` is pushed, the solver
reads n entries). The solver (`FUN_140420750`, 0x48 bytes: vertex ids, count, point base, k,
result ints, area, part list) runs `FUN_140420890`:

* `FUN_140421510` splits a polygon of m >= 200 vertices at its shortest diagonal: over
  `k = m/2 .. 3` (descending) and `i = 0..k-1`, the pair `(i, (i+k) % m)` with the smallest squared
  distance (`FUN_14023b850`, strict `<`, start 1e29). Part A = `v[a] .. v[b]`, part B =
  `v[b] .. v[a]` (both keep the diagonal ends); the input list is cleared; recursion on A then B.
  Parts below 200 vertices go to the back of the part list.
* Each part, `FUN_140420a30`: dynamic programme over chords, `d(i,j) = (j-i-1) mod m`, table
  index `d·m + i` (`(m-1)·m` entries `{best, cost}`, initial cost (180, 1e29), best -1). Costs are
  pairs compared lexicographically, strictly (`FUN_1404218a0`); combining takes the max of the
  first members and the sum of the second (`FUN_140421810`). A triangle costs (180, |area|)
  (`FUN_1404211a0`, `FUN_140421210`: `fabs(FUN_14023e670(3, …))`), so the sum of absolute
  triangle areas is minimised and the first strict minimum wins. `d = 0` costs (0, 0); `d = 1` is
  the triangle `(i, i+1, i+2)`; then for `len = 2..m-2`, `i = 0..m-1`, `j = (i+1+len) % m`, middle
  `k = i+1 .. j-1` (mod m): `cost(i,k) ⊕ cost(k,j) ⊕ tri(i,k,j)`. Closing pair: over `i`,
  `j = i+1 .. i-1` (mod m), `cost(i,j) ⊕ cost(j,i)`, first strict minimum `(f, g)`.
* Output `FUN_1404213c0(f, g)` then `(g, f)`, pre-order: for `d > 0` push
  `(v[a], v[best], v[b])`, add the triangle area to +0x30, recurse `(a, best)`, then `(best, b)`.
  All parts append to one list. `T` gets `size/3` triples (1-based point ids); the return value is
  that count (-1 only when the solver returns false, which it never does).

`FUN_1402e7f40(&profile)` caches its answer in `profile.u10` (+0x1710), the byte the geometry
path reads as "closed": a simple profile gets the closing side strip, a failed one does not
(and no caps). It returns true at once when the byte is already set. It tests only the closing edge `p1–pn` against every edge
`p_i–p_{i+1}`, i = 1..n-1, with `FUN_140308120`/`FUN_140308340` (line crossing by signed
distances; parallel → 2) and the box test `FUN_140309960` on the **unsorted** end points (a box
with `x2 < x1` contains nothing). A crossing inside both boxes must lie within 1e-6 (x and y) of
`p_n` for i = n-1, else of `p_1`; two parallel edges whose x ranges overlap strictly fail. n < 3
fails (byte 0). A pass sets the byte to 1.

The rest of 0x140420750-0x140422a40 is the COW vector/list template code of the solver's
containers (push_back, reserve, copy, detach, list insert, destructors); the port uses std
containers there.

## 11. Oracle cases (`tests/commands/CNVT3D/cnvt3d_sweep_*.json`, data in `data/`)

Inputs: `data/sweep_offaxis.KEY` holds RING_SI's header, die and inter-object data, plus an
object 2 of 2 quads over r = 10..30, z = 0..10. It also has NDTMP 101..106, DRZ (i, 0.5),
STRAIN 0.1·e, STRESS (10e, 20e, 30e, 5e) and `MGNELM 2 2 4 1`, so there is no remesh and the brick
path is taken. `sweep_axis.KEY` has the same object over r = 0..20, with nodes 1 and 4 at r = 0.
`sweep_nearaxis.KEY` has those two nodes at r = 0.0005. All 9 cases ran on 2026-10-21 with exit 0.

| Case | Settings | Confirmed |
|---|---|---|
| `rev90_n2` | 90°, n 2, c 0 | node ids s·6+p; element order and reversed node order; NBCD.OUT bytes (`\t12`, `%.8e` 3-digit exponents, CRLF, ids 1..6 / 13..18); BCCDEF/BCCDFN from NBCD; DRZ last-node-of-layer angle quirk; element STRESS rotation at 22.5°/67.5° (xx yy zz xy yz xz) |
| `rev180_n3` | 180°, start 30, n 3 | n rounded to 4 (30 nodes, 8 hex); end-face ids 25..30; θ_s = dθ·s + θ0 |
| `rev360_n4` | 360°, n 4 | 24 nodes (last layer dropped), last section wraps to layer 0; no NBCD.OUT |
| `axis_rev90_n2` | 90°, n 2, c 0.001 | axis nodes once (14 nodes); element 1 dropped at s=1, axis hex `9 13 11 7 5 4 1 2` at s=2; 0.9 pull of layer-2 and layer-0 nodes (9.0, 5.5109106e-16); NBCD count 10 with `0 602 601` lines; BCCDEF `0 6 6` / BCCDFN `0 2 1` on the axis; STRAIN/STRESS taken from the right 2D elements |
| `axis_rev360_n4` | 360°, n 4, c 0.001 | 18 nodes, 6 hex; last axis hex `17 5 2 15 13 4 1 11` (wrap + compaction) |
| `axis_c0_rev90_n2` | as above, c 0 | r=0 nodes duplicated per layer (18 nodes, 4 hex); NBCD header 12, 10 lines |
| `extrude_n3` | len 50, start 5, n 3 | 3 layers z = 5/30/55 before rotation; element nodes in 2D order; tensor `{t0,t1,t2,0,0,t3}`; then the +90° x-rotation (§12 note) |
| `snap_k1_rev90_n2` | GEO23 k 1, c 0.001 | `fabs(r) < c` snap writes r = 0 back into the 2D object (OUT.KEY), 3D result as the axis case |
| `snap_k0_rev90_n2` | GEO23 k 0 | same result through the boundary-loop path |

The earlier `cnvt3d_revolve90_brick_obj2` (RING_SI, remeshed) agrees too. Its NBCD.OUT header is
70 = 2·35, its end ids are 421.. = 35·12 + p (MSHSEC 11 → 12), and
`5.83238038e-014 = 952.5·cos(π/2)`.

KEY lines produced for a converted brick object, in the order the save writes them (`CONV3D.KEY`,
object 2, revolve 90/n 2):

* header and geometry: `OBJTYP 2 2 0`, `OBJNAM`, `ROTSYM`, `DIEGEO 2 1 <nn> <nfaces>` (brick
  boundary, -n marks the symmetry-plane faces), `POLYTP`;
* mesh: `RZ 2 <nn>` with x y z, `DRZ 2 <nn> 0.0` with 3 components, `BCCDEF`/`BCCDFN` (§8),
  `NDTMP 2 <nn>`, `ELMCON 2 <ne> 8`, `MTLGRP 2 0 1`;
* element state: `STRAIN 2 <ne>`, `STNCMP 2 0 6`, `STRESS 2 <ne>` with 6 values (5 on the record
  line plus 1 continuation line);
* mesh settings: `MGNELM 2 8000 <m> <2D target> 1`, …;
* face BCs and symmetry: `ECCTMP`/`ECCATM 2 <nf> 0` with value 2 per face, `SYMSUF 2 1 0` and
  `SYMSUF 2 2 0` (point, normal);
* axes: `WPAXIS 2 1 0 0` (revolve) or `2 1 1 0` (extrude), `WPAXIS 2 2 4 0` followed by the angle.

## 12. Constants and helpers

| Address | Value / role |
|---|---|
| `DAT_14051f210` | 360.0 |
| `DAT_14051f230`, `DAT_14051f228` | π, 180.0 |
| `DAT_14051f220` | 0.9 (axis pull) |
| `DAT_14051f208`, `DAT_14051f200` | 90.0, -90.0 |
| `DAT_14051f238`, `DAT_14051f240`, `DAT_140509fb8` | π/2, 0.5, 2.0 (SV angle) |
| `DAT_14051bed8`, `DAT_140509c90`, `DAT_14051bec0` | 0.0, 1.0, sign mask |
| `DAT_140509b50` | 1e30 (nearest-node start) |
| `DAT_14052c598` | 1e29 (edge-length start) |
| `DAT_14051f1b8`, `DAT_140541478` | 1e-6, -1e-6 |
| `DAT_14051bed0` | 1e-8 (plane test, full revolution) |
| `DAT_14051f258`, `DAT_14051bef8` | 0.01, 0.1 |
| `DAT_14051f1e8`, `DAT_14051f250` | 0.001, 1e-4 (mesh-window shift) |
| `DAT_140baea58`, `DAT_140baea60` | 45.0, π/4 (`FUN_140325691`) |
| `DAT_1405caa80` | 46 SV ids (§6.2), count `FUN_140161fd0` = 0x2e |
| `FUN_140148f70`, `FUN_14014c0a0` | revolve / extrude sweep |
| `FUN_14014c730`, `FUN_14014d730`, `FUN_14014fae0` | compaction; object + SV (revolve / extrude) |
| `FUN_1401627e0(obj, id, &nodal)` | SV array accessor (switch on id-1) |
| `FUN_140183c20(elems, j)` | integration points: 1; 4 (2D) or 8/1 (3D, `+0xec == 8`) when ELMNOD[j] == 3 |
| `FUN_140166200(obj, &tag)` | STNCMP block offset from STNOUT (`model+0x86c..`) |
| `FUN_140202700(o, nn, ne, 4, W, 3)` | allocate a mesh |
| `FUN_140155340`, `FUN_140154f20` | axis snapping |
| `FUN_140141580`, `FUN_140202a20/a70/3046a0`, `FUN_14023b850` | NBCD.OUT read, tolerance, squared distance |
| `FUN_140198d30`, `FUN_1402059a0`, `FUN_14023c770` | symmetry faces/surfaces for partial revolve |
| `FUN_140153710`, `FUN_140153c00`, `FUN_140154270`, `FUN_140153910` | geometry entry points |
| `FUN_1402b7ad0`/`8090`, `FUN_1402b8980`, `FUN_1402b8520`, `FUN_1402ba500` | GEOPRIM ctor/dtor, revolve / extrude setup, builder |
| `FUN_1402ce190`, `FUN_1402d7cd0`, `FUN_1402da250`, `FUN_14027ece0` | revolve dispatch, surface generator, axis projection, FIXGEO |
| `FUN_140325691`, `FUN_1404b3b30` | point rotation, sincos |
| `FUN_14021d2f0` → `FUN_1402002b0`, `FUN_1402214c0`, `FUN_140221cd0` | object rotation, 4×4 matrix, Rodrigues |
| `FUN_1401c6e40(model)` | `return *(char*)(model+0x1b5a)`, §13 |

## 13. `FUN_1401c6e40` [listing]

`MOV AL, byte ptr [RCX + 0x1b5a]; RET`: it returns the byte at `model+0x1b5a`. The
`_DB_STEP_DATA` ctor `FUN_1401c8950` sets `+0x1b59 = 1`, `+0x1b5a = 0`, `+0x1b5b = 0`. Nothing on
the CNVT3D path writes `+0x1b5a`: the only writer seen nearby, `FUN_1401c6e20`, writes `+0x1b59`.
So `FUN_1401c5810` always takes the direct branch for the new 3D model:
`model+0x1744 = *opts` (0), `fopen(name, "wt")`, `FUN_1401c5af0`.

## 14. Open questions

* The extrude's +90° x-rotation (`FUN_14021d2f0`) multiplies by **6.1232011757e-17**, not by
  `cos(fl(π/2)) = 6.123233995736766e-17`. The evidence is node z = 5·c printed as 3.0616005879E-016.
  The Rodrigues code `FUN_140221cd0` uses `FUN_140bb3490`. The node path behind the virtual call
  (`vtbl+0x40` in `FUN_1402002b0`) was not followed, so it is open which routine produces the
  value. It may be `FUN_1404b3b30`'s argument reduction. A port needs it bit-exact only for the
  last digits of the ±90° rotations.
  **Resolved (T9-1, p9-03):** the nodes rotate with `FUN_140325691`, which takes sincos
  (`FUN_1404b3b30`, PathScale libm) of `(a/45)·π/4`; its Cody-Waite reduction gives
  cos = 6.1232011757e-17 for a = 90. `def::model::fun_140325691` (p1-10, `rt::libm::ps_sincos`)
  reproduces 3.0616005879E-016 and 1.8369603527E-015 of `cnvt3d_sweep_extrude_n3` (test
  `t9_1_rotation_constant_is_pathscale_cos`); p67-1's rotate needs no change.
* `FUN_1402ce290` (mesh-boundary sweep for `FUN_140154270`), `FUN_1402d9bb0` (axis
  classification), `FUN_1402caac0/cac30/caf40` (non-uniform section steps), `FUN_1402db310/dbb20`
  (strip faces), `FUN_14041f830` (cap triangulation) are not decompiled; the extrude primitive `FUN_1402baa60`
  is in §10.5. The geometry of kinds 5 and 2 is therefore specified only to the level of §10.
  **Revolve part resolved (p9-05, T9-2):** [GEOPRIM_REVOLVE.md](GEOPRIM_REVOLVE.md). The cap
  triangulation seeds its segment order with srand(time(NULL)): cap faces differ between runs (§7 there).
* §10.3 `local_180`: resolved (prim+0x2518, filled by `FUN_1402ce310`; T9-3).
* WPAXIS entry 1: **resolved (T9-4).** The KEY writer `FUN_1402a39e0` prints, for types 0..2, the
  header int +0x38 and the last line +0x40..+0x58; `+0xb0/+0xb8/+0xc0` are printed only for type 4
  (header +0xb0, last line +0xb8). Entry 1 (type 0 revolve / 1 extrude) therefore always shows
  `0 0 0 0`; the angle is the last line of entry 2 (type 4): 90, 180, 360 in the cached runs.
* `FUN_140144a70` (2D remesh) and `FUN_140146050` (DEF_INT) are as in DEFAMG.md and CNVT3D.md §6.3.
  A failed remesh (case `cnvt3d_sweep_remesh_fail_rev90`, MGNELM target 0 quads) hangs the original
  inside the DEF_INT launch (oracle timeout 900 s); not usable as a case.
* The stale NBCD.OUT for an extrude after a revolve (§8): still not observed. Case
  `cnvt3d_sweep_stale_nbcd_rev90_ext` (and `_swapped`) with a hand-made 3-object seed
  (`data/sweep_two.KEY`) crash the original (0xC0000005) in the set-up of the copied object 3; it
  needs a seed saved by DEF_PRE itself.
