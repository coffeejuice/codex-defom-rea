# PNTEDG `FUN_1404d41f8` (FTN_geoutl, 2071 B): nearest edge of a polygon

Source: listing `deform-rea-work/out/p673/L_extra.txt` (1404d41f8..1404d4a0e), decompile
`out/p673/extra.c`. Name from its error text ("Program stopped in Sub. PNTEDG") **[fact]**.
Port: `def::rt::geoutl::fun_1404d41f8` in `port/defrt/src/ftn/geoutl_d41f8.cpp`.

Callers: the drop solver's projection fall-backs (`FUN_140374252` quad, `FUN_140374954`
triangle, OBJPOS_drop_solver.md §5.5.2), `FUN_1404d4a0f` (GENCTC_3d_search.md §10.4),
`FUN_14007ff21`.

## Signature

`PNTEDG(X, P, XI, ETA, Q, DMIN)` = `fun_1404d41f8(double* x, double* p, double* xi, double* eta,
double* q, double* dmin)`; `x(3,4)` node coordinates, `p(3)` the point; out: `q(3)` the nearest
point on the polygon boundary, `dmin` its distance, `(xi, eta)` natural coordinates of `q`
**[fact]** (registers RCX, RDX, R8, R9, stack +0x28, +0x30). The decompile shows
`DAT_140bb2760` as the first argument of `FUN_1404e33b7`; the listing passes `RCX = x`
unchanged (1404d422f/1404d4270) **[fact]**.

## Algorithm **[fact]**

```
k = 0; best = 1.0e10 (DAT_140bb2760)
FUN_1404e33b7(x(:,1), x(:,4), &same)        ! same = 1 when |x1-x4| <= tol per component
n = (same == 1) ? 3 : 4
for i = 1..n:
    j = i + 1; if (n < j) j = 1
    FUN_1404d6b13(x(:,i), x(:,j), p, qt, &t, dmin)   ! PNTLIN: foot qt, t in [0,1], *dmin = distance
    tpar(i) = t
    if (best > *dmin): best = *dmin; q = qt; k = i     ! COMISD best,d; JP/JBE: strict, NaN never wins
if (k == 0): WRITE failure text (below); FUN_1404edb2f(-1) (bare RET) -> continues with k = 0
k2 = k + 1; *dmin = best; if (n < k2) k2 = 1
n == 4: t = tpar(k); omt = 1.0 - t
        xi  = omt*C(1,k) + C(1,k2)*t
        eta = omt*C(2,k) + t*C(2,k2)          ! products then one add, in this order
n == 3: m = k - 1; if (m < 1) m = 3
        FUN_1404e29fa(x, xi)                   ! *xi = area(x1,x2,x3) (Heron), used as scratch
        FUN_1404e2c31(x(:,m), q, x(:,k2), &a)  ! area of (x_m, q, x_k2)
        r = a / *xi; s = 1.0 - r
        k == 1: (xi, eta) = (r, s);  k == 2: (0.0, r);  else: (s, 0.0)
```

`C(2, 0:4)` is the .rdata table at `0x140643ac0`: entry 0 `(-1, 1)` (read only when k = 0),
then the quad corners 1..4 `(-1,-1), (1,-1), (1,1), (-1,1)` **[fact]** (bytes of the exe).
In the triangle case `(xi, eta)` are the area coordinates of nodes 1 and 2 (`FUN_1404e34eb`
uses weights `xi, eta, (1-xi)-eta`): on edge k the weight of the node opposite the edge is 0
**[inferred]** from the formulas.

SAVE state `0x140730d88..0x140730e20` (k, best, same, n, loop indices, qt, t, tpar(0:4), k2,
area, r, s; `0x140730e08` written 0.0, never read): every variable is written before it is read
in one call, except `tpar(0)` (`0x140730dd8`), which no routine writes (only reference in the
binary's decompiles is this routine) and which is read only in the k = 0 path, so it is 0.0;
the port keeps them as locals **[fact]**. The quad result does not depend on `p` beyond `t`.

## Failure path (k = 0) **[fact]**, not exercised

Reached only when every edge distance is NaN or >= 1e10. Three Fortran WRITEs
(`FUN_1404b5ef4`) on the unit held in `DAT_140757a84`: `" Program stopped in Sub. PNTEDG. It seems
a node failed to"` (0x3a chars, str 140bb27a0), `"  a polygon it should rest on."` (0x1e,
str 140bb27e0), and when `DAT_140757aa8 > 0`, `"Please check Node"` (0x11, str 140bb2800) with
the integer `DAT_140757aa8`; then `FUN_1404edb2f(-1)` returns and the code goes on with k = 0
(quad: `xi = (1-tpar(0))*C(1,0) + C(1,1)*tpar(0) = -1`, `eta = 1`; triangle: m = 3, k2 = 1, `q`
not written by PNTEDG). The port calls the hook `fun_1404d41f8_failure_write()` (stub,
`DEF_NOT_PORTED`) for the WRITEs: the I/O descriptors (format, unit variable) are not decoded
**[open]**.

## Verification

`deform-rea-work/scripts_p673/c/d41f8diff.cpp` (+ `callimg.asm`, `mk_d41f8.cmd`): image mapped at
0x140000000; the port with its four callees forwarded to the original routines vs the original
PNTEDG: 300018 calls (fixed branch cases + random quads, 3D quads, triangles, collinear
triangles, near-coincident node 4), 0 mismatches over xi, eta, q, dmin. Zero-length edges are
not generated (PNTLIN's WRITE). Note: the Intel Fortran routines clobber xmm6-15 without saving
them; the harness calls them through `call_img`, which saves them. Golden table (42 cases) in
`port/defrt/tests/test_geoutl_d41f8.cpp`, compare guarded until the callees are ported.
