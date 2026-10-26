# OBJPOS drop / GENCTC 3D: ray-quad children and ray fall-backs

Closes OBJPOS_drop_solver.md open item 2 and GENCTC_3d_search.md open item 2 (children of the
ray-quad `FUN_140064f58`, and the parallel-ray fall-back of the ray-triangle `FUN_1400647a0`).
Source: `DEF_PRE_64.exe` v11.0.2; listings `out/p673/L_extra.txt` (1400647a0, 14007ff21,
1400653e5, 140063ab3), `out/p673/b/L_b.txt` (140065ef8, 14007f8c4, 140066698, 140066902,
14007a47d, and the drop routines 140373650..140371339). Port: `port/defcmd/src/genctc3d_ray.cpp`.
**[fact]** = read in the listing, **[inferred]** = deduced. Notation as in OBJPOS_drop_solver.md:
every expression is in the listing's operation order, fully parenthesised; `|x|` = ANDPD with
`DAT_140bad200`, `-x` = XORPD with `-0.0`. X(:,k) = X + 3(k-1). Comparisons are COMISD; "ordered"
conditions are false for NaN unless stated.

Constants: `DAT_140bade80` 1e-11, `DAT_140bad888` 1e-6, `DAT_140bad3c0` 1e-6f
(9.999999974752427e-07), `DAT_140bad800` 0.9f (0.8999999761581421), `DAT_140bad6e8` 1e10,
`DAT_140badea0` 1e9, `DAT_140bad3c8` 1e5, `DAT_140bad808` 1e8, `DAT_140bada38` 0.25,
`DAT_140badea8` 1e-9, `DAT_140badeb0` −1e-8, `DAT_140badeb8` 1.00000001, `DAT_1405c8850` 1/3,
corner table `DAT_1405c80e0` (stride 3 doubles): V1 = (1, 0), V2 = (0, 1), V3 = (0, 0).

## 1. Plane intersection `FUN_140063ab3(P, dir, n, X1, Q, &d, &ok)` (390 B) **[fact]**

```
den = ((dir2*n2) + (dir1*n1)) + (dir3*n3)
ok = 0
if not (1e-11 >= |den|):                      // NaN goes on
    ok = 1
    a  = -(((n2*X1_2) + (n1*X1_1)) + (n3*X1_3))
    b  = ((n2*P2) + (n1*P1)) + (n3*P3)
    tt = (a + b) / den
    d  = -tt
    Q_c = P_c - (dir_c * tt)                  // c = 3, 2, 1 (store order)
```
Q and d are not written when ok = 0. GENCTC_3d_search §10.2 matches.

## 2. Ray-triangle `FUN_1400647a0` entry (correction of GENCTC_3d_search §10.2) **[fact]**

* `*bad == 1` (11th argument): `FUN_140065ef8(P, dir, n, X, &d, &s, &t, Q, &hit, &bad)` and
  return; `hit` is not cleared first.
* else `140063ab3(P, dir, n, X(:,1), Q', &dist, &ok)`; `dn = ((dir2*n2)+(dir1*n1))+(dir3*n3)`;
  `hit = 0`; `ok == 0` (tested as `(float)ok == 0.0f`) → `bad = 0`, `FUN_140065ef8(..., &bad)`,
  return. So `FUN_140065ef8` is the parallel-ray fall-back, entered with flag 1 only when the
  caller passes bad = 1 (degenerate normal), with flag 0 for a parallel ray.
* mode 1: return (hit 0) if `dn > 0` or `|dist| > |d|`; mode 2: if `|dist| > |d|`.
* The in-triangle test of §10.2 follows; `0.0 >= u → u = 0.0` (also turns −0 into +0),
  `u >= 1.0 → u = 1.0`, the same for w; outputs `Q = Q'`, `hit = 1`, `d = dist`, `t = u`,
  `s = (1.0 - u) - w`. A miss sets hit = 0 and leaves d, s, t, Q.

## 3. Ray-quad `FUN_140064f58` (OBJPOS_drop_solver §5.6, completed) **[fact]**

`hit = 0`; `bad == 1` → `1400653e5`. Else `N = 1404d3d43(X)`, `T = 1404cf5c0(N)`,
`z4 = (1404ded0c(X(:,4) → y, origin X(:,1), T, &1, &1))_3`; not `1e-6 > |z4|` → `1400653e5`.
`140063ab3(P, dir, n, X(:,1), Q', &dist, &ok)` (n = the caller's normal, not N); ok = 0 →
`1400653e5`. Mode tests as §2. `14007ff21(X, Q', &s, &t, &in, &0)`; `in < 1` → `Q = Q'`,
`hit = 1`, `d = dist`.

## 4. Point in quad `FUN_14007ff21(X, P, &s, &t, &in, &mode)` (1678 B) **[fact]**

```
N = 1404d3d43(X)
if 1 <= mode <= 6:                              // zero components, renormalise
    mode 1..3: N(mode) = 0;  4: N2 = N1 = 0;  5: N3 = N2 = 0;  6: N1 = N3 = 0
    sq = (N1*N1) + 0.0;  sq = sq + (N2*N2);  sq = (N3*N3) + sq
    r = 1.0 / sqrt(sq);  N_c = N_c * r          // SQRTSD even for sq = 0
T  = 1404cf5c0(N)
Q0 = 1404d6974(X, 0.0, 0.0)                    // bilinear centre
P' = 1404ded0c(P, Q0, T, &1, &1)                // argument order (x, y, origin, T, np, mode)
V' = 1404ded0c(X, Q0, T, &4, &1)
dmin = 1e8
1404cc289(V', P', near, &dmin, &edge, &4, &in, &1, idx, &0, &3)
if in < 1 or (in == 1 and 1e-6 >= dmin):
    if in == 1: in = 0
    1404ccf58(V', P', xe, &k, &err)
    if err == 0: (s, t) = xe
    else: V'(:,4) = V'(:,1);  1404d41f8(V', P', &s, &t, Q0, &dmin)   // see §9
    Q = 1404d6974(V', s, t)                    // into the `near` buffer
    if |Q3 - P'3| > 1e-6: in = 1
```
(`in` > 1 from 1404cc289, or `in == 1` with `dmin > 1e-6`, returns with that `in`.)

## 5. Point in triangle `FUN_14007f8c4(X, P, &s, &t, &in, &mode)` (1629 B) **[fact]**

As §4 with three nodes: the same mode/normalisation block; then
`Q0 = 1404e34eb(X, &1/3, &1/3, &3)` **before** `T = 1404cf5c0(N)`; `V' = ded0c(X, ..., &3, &1)`;
`1404cc289(V', P', near, &dmin, &edge, &3, &in, &1, idx, &0, &3)`; the same in-test;
`1404e2d44(V', P', &s, &t, &b3, &bad2)`; `bad2 ≠ 0` → `V'(:,4) = V'(:,1)`,
`1404d41f8(V', P', &s, &t, Q0, &dmin)`; `Q = 1404e34eb(V', s, t, &3)`; `|Q3 − P'3| > 1e-6` →
`in = 1`. V'(:,4) is a static slot written only before 1404d41f8 [the port keeps it global].

## 6. Non-planar quad `FUN_1400653e5(P, dir, n, X, &d, &s, &t, Q, &hit, &mode)` (1960 B) **[fact]**

The third argument is never read. Fan of four triangles (edge k, centroid):
```
kt = 0; kf = 0                                          // DAT_1406a1a3c / 1406a1a40
C = ((((X1x + X2x) + X3x) + X4x) * 0.25, same y, 0.25 * (((X1z + X2z) + X3z) + X4z))
best = 1e10
for k = 1..4: k2 = k+1 (5 -> 1); tri = [X(:,k), X(:,k2), C]
    nt = 1404d3d43(tri);  sa = (|nt2| + |nt1|) + |nt3|;  flag = 0
    if sa >= 0.9f:                                       // NaN -> fall-back
        flag = 1
        140063ab3(P, dir, nt, tri, Q', &dist, &ok)
        if ok != 0:
            test = true
            if mode >= 1: dn = ((dir2*nt2)+(dir1*nt1))+(dir3*nt3)
                mode 1: test = not (dn > 0 or |dist| > |d|);  mode 2: test = not (|dist| > |d|)
            if test: 14007f8c4(tri, Q', &s, &t, &in, &0)
                     if in <= 0: kt = k; hit = 1; if best > dist: best = dist; Q = Q'
            next k
    // fall-back (sa < 0.9f, or ok == 0 with flag = 1):
    140065ef8(P, dir, nt, tri, &dist2, &s, &t, Q', &hit, &flag)
    if 1e-6 > |(1.0 - s) - t| and best > dist2 and hit == 1: best = dist2; Q = Q'; kf = k
if 1e9 > best and (kt > 0 or kf > 0):
    14007ff21(X, Q, &s, &t, &in, &0);  if in <= 0: d = best
if in == 1: hit = 0                    // `in` (DAT_1406a1b30) may still hold an earlier call's value
```
`s, t` are the caller's: the sub-triangle calls overwrite them; the final 14007ff21 recomputes
them as quad coordinates of Q. In the fall-back the test `(1 − s) − t ≈ 0` accepts only hits on
the edge opposite the triangle's third node (the quad edge k) [inferred]. dist2, Q' and in are
statics that a call can read before writing them (140065ef8 with flag 1 can return without any
write) **[fact]**.

## 7. In-plane ray `FUN_140065ef8(P, dir, n, X, &dist, &s, &t, Q, &hit, &flag)` (1952 B) **[fact]**

```
if flag == 1:                                  // frame from an edge x dir
    for i = 1, 2, 3 (j = i+1, 4 -> 1) until found:
        (e, len) = 1404d68dd(X(:,j), X(:,i))   // e = unit(X(:,i) - X(:,j))
        if len > 1e-6f: 1404cce5e(e, dir, c, &len)  // R9 still &len: len := |e x dir|
                        if len > 1e-6f: T = 1404cf5c0(c); found
    not found: return (nothing written, hit unchanged)
else: T = 1404cf5c0(n)
Y = 1404d0939(X, X, T, &1, &3) (3 nodes in the frame about X(:,1)); Y(1:2,4) = Y(1:2,1)
Z = 1404d0939([P, P + dir], X, T, &1, &2)      // P + dir as dir_c + P_c
hit = 0
if |Z(3,1) - Y(3,1)| > 1e-6f: return           // P off the triangle's plane
m = 0; best = 1e5
for edge e = 1..3 (B = Y(:,e), Y(:,e+1)):
    140066698(Z, B, out_e, par_e, &3, &ok); 140066902(par_e(1), par_e(2), &in_e)
    e = 1: if in_e and ok: m = 1; best = par_1(1)
    e > 1: if ok and in_e and |best| > |par_e(1)|: best = par_e(1); m = e
if m == 0: return
a = par_m(1); b = par_m(2); hit = 1
Q_c = (dir_c * a) + P_c                         // c = 3 written as (a*dir3) + P3
m2 = m+1 (4 -> 1)
s = ((1-b) * Vx(m)) + (Vx(m2) * b);  t = ((1-b) * Vy(m)) + (b * Vy(m2))
dist = 14007a47d(P, Q)
```
Note `best` starts at 1e5 but edge 1 replaces it unconditionally (no comparison) **[fact]**.

## 8. Small helpers **[fact]**

* `FUN_140066698(A, B, out, par, &n, &ok)`: column stride `max(n, 0)`; `a1 = A(1,2)−A(1,1)`,
  `b1 = B(1,1)−B(1,2)`, `a2 = A(2,2)−A(2,1)`, `b2 = B(2,1)−B(2,2)`,
  `det = (b2*a1) − (b1*a2)`; `ok = 0`; `1e-9 >= |det|` → return. `c1 = B(1,1)−A(1,1)`,
  `c2 = B(2,1)−A(2,1)`, `r = ((c1*b2) − (c2*b1))/det`, `q = ((c2*a1) − (c1*a2))/det`;
  `par = (r, q)`, `ok = 1`; `out(i) = (r * D(i)) + A(i,1)` for i = 1..n with the static list
  `D = (a1, a2, b1, b2, det, ...)`, so `out(3) = (r*b1) + A(3,1)` **[fact; looks like an
  unintended third component, harmless: only x, y are used]**.
* `FUN_140066902(&a, &b, &ok)`: `ok = 0`; `0 > a > −1e-8 → a = 0.0`, else `1 < a < 1.00000001 →
  a = 1.0`; the same for b; `ok = 1` iff `0 <= a <= 1` and `0 <= b <= 1` (a, b written back).
* `FUN_14007a47d(A, B, &d)`: `e = B − A`; `sq = ((e2*e2) + (e1*e1)) + (e3*e3)`; `d = sq`;
  `sq != 0` (or NaN) → `d = sqrt(sq)`.

## 9. Corrections to OBJPOS_drop_solver.md §5.5.2 **[fact]**

* `FUN_140374252` / `FUN_140374954` (and 14007ff21 / 14007f8c4): before the 1404d41f8
  fall-back the binary copies `V'(:,1)` into `V'(:,4)` — for the quad this **overwrites node 4**
  (listing 1403746fc, 1400803a9), so PNTEDG sees a triangle (node 4 == node 1) and the
  following `1404d6974(V', ξ, η)` uses the modified node 4.
* Tolerances: `tol = b84720`, and `tol == 0 → tol = b84730 * 1e-5` (`DAT_140bafcd0`); off-plane
  limit `b84728`, `== 0 → b84730 * 0.5` (`DAT_140baf918`) (listing 140374613, 140374812).
* The mode argument (6th) zeroes normal components 1..6 as §4 (mode 0 in the drop).
* 140373650 indexes FID, NRM, XE and SW by `k = flags(n)` (the drop's flags are 1..nn, so k = n)
  and BC, X by n; 140370a26 / 140371339 index NRM, FID, XE, FN by the position i in the node list
  and BC, X, VN, GAP by `n = nodes(i)` **[fact]**.
* 140370a26 / 140371339 loop over every body b with `IT(2,b) > 0`, RT groups `IT(9..10,b)` and
  nodes `int(RT(3,g)) .. int((RT(2,g) − RT(3,g)) + 1.0)` (cvttsd2si) **[fact]**.
* `FUN_14001f54f` reads 9 arguments (the 10th, `&0`, is never read). Its box index list
  `DAT_1405c1800` = {1, 2} (INTEGER).

## 10. Statics read before written (port globals) **[fact]**

14001f54f: EI2/ER1/Xp/conn buffers and the kernels' s, t, Q, hit (`DAT_140649460..580`);
140064f58: `DAT_1406a1a38`; 1400653e5: `DAT_1406a1b30`, `1406a1b10`, `1406a1ae0`; 140065ef8:
`DAT_1406a1e40` (140066902 clamps them in place even when 140066698 failed); 14007f8c4 /
140374954: V'(:,4); 140373650: NI4, ER1, Xp, conn, ξ, η, bad (`DAT_140706720..818`).

## 11. Open items

1. The list-directed layout of the CNTINF lines (140373650) uses defrt `fio` (PathScale layout as
   checked by p67-7); not reached by any corpus case.
2. Golden checks against the binary exist for 140063ab3, 1400647a0 (non-fall-back paths),
   140066698, 140066902, 14007a47d; the routines that call FTN_geoutl kernels are unverified
   until those kernels are ported.
