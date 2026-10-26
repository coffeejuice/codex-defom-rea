# OBJPOS geometric kernels: tensor rotation, own-centre, sin/cos, 2D mirror parts

Companion to [OBJPOS.md](OBJPOS.md) (worker geo-kernels). It closes the open items 1, 2, 3, 5 and 6
of OBJPOS.md section 12 so that the geometric modes can be ported bit-exactly. The 3D mirror
boundary rebuild (`FUN_1401fc290` / `FUN_140354ed0`, open item 4) is covered by another worker.
Source: `DEF_PRE_64.exe` v11.0.2, headless Ghidra (copy `ghidra_p67e`); working files
(decompiles, listings, constant dumps, a listing simulator and a Python transcription of the CRT
sin/cos) are in `deform-rea-work/out/p67/geo-kernels/`. **[fact]** = read in the code (listing where
the FP order matters), **[inferred]** = deduced.

Notation: `fl(a op b)` is one IEEE double operation; every formula below is written with explicit
parentheses in the executed association. Additions and multiplications are commutative in IEEE,
so only the grouping matters, not the operand order. Fortran arrays are 1-based and column-major:
`T(i,j)` is at `base + 8*((i-1) + 3*(j-1))`.

## 1. Tensor rotation (STRESS, STNCMP, YLDS)

### 1.1 Caller `FUN_140184ac0(elm, origin, dir, angle_deg)` (from the rotate, OBJPOS.md 6.2 step 2)

Element container `elm = obj+0x9e0`, element count `n = elm+0xe8`, the flag passed to the
kernels is always `1` (`local_5c`), the angle is passed in **degrees** **[fact]**. 3D needs
`dir != NULL`. Width of each array from the layout table `elm+0xf0 -> +0x15f8` (`+0x98` STRESS,
`+0xa0` STNCMP, `+0xe4` YLDS); `FUN_14019a480(elm)` = 2D (`elm+0xf8 == 2`), `FUN_14019a450` = 3D.

| Array | 3D | 2D |
|---|---|---|
| STRESS `elm+0x20` (kind 1) | `FUN_140325884(arr, &n, &1, dir, &a, &1)` in place, **width not checked** (the kernel assumes 6 doubles per element) | only when width == 4: `FUN_14032e585(arr, &n, &1, &a, &1)` |
| STNCMP `elm+0x30` (kind 2) | `blocks = w / 6`, `rem = w % 6`, `m = n*blocks`; `rem == 0`: kernel in place over `m` consecutive 6-tuples; `rem > 0`: the first `6*blocks` values of every element copied into a `calloc` buffer, rotated as `m` tuples, copied back (the `rem` tail values untouched) | skipped when `w != 4` and GEOTYP (`model+0x850`) == 3; `w == 4`: in place; otherwise for each block `b < w/4`: components `4b..4b+3` of all elements copied to a buffer, rotated, copied back |
| YLDS `elm+0xb8` (kind 1) | as STRESS | as STRESS |
| MATAXI `elm+0x60` | OBJPOS.md 6.2 | OBJPOS.md 6.2 |

### 1.2 3D kernel `FUN_140325884(S, &n, &kind, D, &a, &flag)` (Fortran, statics at `0x140701810..`)

Listing `l_325884.txt`, element block simulated symbolically (`sim.py`, output
`sim_325884_blockB.txt`); the two copies of the element block (kind 1 at `0x140326389`, kind 2 /
other at `0x140325ede`) are instruction-identical **[fact]**.

**Frame T** (`0x140701840`, column-major), with `a, b, c = D.x, D.y, D.z` (raw, not normalised):

```
r1 = sqrt(a*a + c*c)
r2 = sqrt((b*b + a*a) + c*c)
s  = r1 / r2 ;  t = b / r2
if (!(r1 >= 1e-12))       // DAT_140baea68; JP/JC: NaN also takes this branch
    T = [1, 0, 0,  0, s, t,  0, -t, s]                       (T11,T21,T31, T12,T22,T32, T13,T23,T33)
else
    ca = a / r1 ; cc = c / r1
    T = [cc, -(ca*t), ca*s,  +0.0, s, t,  -ca, -(cc*t), cc*s]
```

`-x` is a sign flip (`xorpd` with `DAT_140baea70` = -0.0). No zero-axis test (unlike the point
kernel 6.3, which keeps the previous frame): `D = 0` gives `s = t = 0/0 = NaN`, the degenerate
branch, and every tensor becomes NaN **[fact]**. OBJPOS does pass `D = 0` (mode 3 with a zero
direction and a non-zero angle): `OBJPOS/v-dir0_3d` gives NaN (KEY `-1.#IND000000E+000`) in
STRESS, STNCMP and YLDS (the zero-filled YLDS array is then written with all 4978 rows), and all
nodes at the origin (point kernel, OBJPOS.md 6.3) (corrected after oracle run: the earlier text
named `OBJPOS/3-zero_3d`, which has `D = (0,0,1)` and angle 0 and never reaches the kernel).

**Angle and R** (`0x1407018a0`):
`rad = (a * 3.141592653589793) / 180.0` (`DAT_1405f3140`, `DAT_140baea78`; multiply then divide).
`FUN_1404b3b30(rad, &sn, &cs)` = Fortran SIN then COS (section 3.2). `s' = (flag == 1) ? -sn : sn`.
`R(1,1) = cs, R(2,1) = -s', R(3,1) = 0, R(1,2) = s', R(2,2) = cs, R(3,2) = 0, R(1,3) = R(2,3) = 0,
R(3,3) = 1.0`. With `flag = 1`: `R = [[cs, -sn, 0], [sn, cs, 0], [0, 0, 1]]` (row form) **[fact]**.

**Q = T^T R T** (two triple loops, `W` at `0x140701910`, `Q` overwrites `R` in place):

```
for i=1..3, j=1..3:  W(i,j) = ((0.0 + R(i,1)*T(1,j)) + R(i,2)*T(2,j)) + R(i,3)*T(3,j)
for i=1..3, j=1..3:  Q(i,j) = ((0.0 + T(1,i)*W(1,j)) + T(2,i)*W(2,j)) + T(3,i)*W(3,j)
```

(The `0.0 +` is executed: it only turns a `-0.0` first product into `+0.0`.) This is the same
matrix `T^T R T` the point kernel applies (OBJPOS.md 6.3), so tensors and nodes rotate the same
way, except for the different angle conversion and frame threshold **[fact]**.

**Per element** `e = 1..n` (record of 6 doubles at `S + 48*(e-1)`, `kind` re-read every element):
component order is `[xx, yy, zz, xy, yz, zx]` (`S(1,1)=s0, S(2,2)=s1, S(3,3)=s2, S(1,2)=s3,
S(2,3)=s4, S(1,3)=s5`, deduced from the index pattern) **[fact]**.

```
load s0..s5 from the element; kind == 2: s3 = e3*0.5, s4 = e4*0.5, s5 = e5*0.5   (DAT_140baea80)
V(i,j) = (Q(i,1)*S(1,j) + Q(i,2)*S(2,j)) + Q(i,3)*S(3,j)                    i,j = 1..3
S'(i,l) = (Q(l,1)*V(i,1) + Q(l,2)*V(i,2)) + Q(l,3)*V(i,3)
s0' = S'(1,1), s1' = S'(2,2), s2' = S'(3,3), s3' = S'(2,1), s4' = S'(3,2), s5' = S'(3,1)
kind == 2: s3' *= 2.0, s4' *= 2.0, s5' *= 2.0                                (DAT_140baea88)
store s0'..s5' back
```

All 9 `V` and all 6 `S'` sums have the `(t1 + t2) + t3` grouping (listing) and read only the
original `s` values **[fact]**. Confirmed bit-exact by oracle run: every STRESS and STNCMP double
(2 x 29868) written by GENDB in `OBJPOS/gk-tensor-y_3d`, `gk-tensor-obl_3d` and `v-bits-rot_3d`
equals this transcription with the Fortran SIN/COS of 3.2 (with the AMD libm sin/cos at 40.6
degrees only ~44% match); using `Q^T` instead of `Q` fails, so the convention is pinned. So `S' = Q S Q^T`. A `kind` other than 1 or 2 reuses the static
buffer of the previous element (stale) and writes it into the element; `FUN_140184ac0` only
passes 1 and 2 **[fact]**.

### 1.3 2D kernel `FUN_14032e585(S, &n, &kind, &a, &flag)` (listing `l_32e585.txt`)

`rad = (a * 3.141592653589793) / 180.0` (`DAT_1405f4350`, `DAT_140baebf8`); `FUN_1404b3b30` -> `sn,
cs`; `s' = (flag == 1) ? -sn : sn` (`DAT_140baec00`). Precomputed once **[fact]**:
`cc = cs*cs`, `ss = s'*s'`, `sc = s'*cs`, `d = cc - ss`.

Per element (4 doubles `[rr, zz, hoop, rz]` = `e0..e3`):

```
S3 = (kind == 2) ? e3*0.5 : e3                 // DAT_140baeb90
h  = (S3*2.0)*sc                                // DAT_140baebc0
e0' = ((e1*ss) + (e0*cc)) + h
e1' = ((e1*cc) + (e0*ss)) - h
e3' = ((e1*sc) - (e0*sc)) + (S3*d)
kind == 2: e3' = e3'*2.0
e2 (hoop) is not written
```

**[fact]** (listing; the decompile shows the same grouping). Oracle: `OBJPOS/gk-tensor_2d` (30
degrees) and `v-tensor_2d` (40.6) match on all 1265 STRESS and STNCMP rows at KEY precision
(GENDB refuses that seed, so no bit check). With `flag = 1` this is
`sigma' = R sigma R^T` for a counter-clockwise rotation by `a` **[inferred]**.

## 2. Own centre for 3D mode 6: `FUN_1403e8510` (Fortran mass properties)

Called by `FUN_1401fe7d0(obj, &C, &axis)` with: `nP` = point count, `nF` = face count, the point
array of geometry 0 (3 doubles each; a temporary surface built from the mesh when the object has a
mesh and no rigid geometry), a copy of the face array with **every one of the 4 node numbers made
positive** (`abs`), outputs `CENTER` (3), `AXIS` (3), `ok` flag, debug flag `= 0` (so none of the
`NUMSN`, `RZ`, `CENTER`, `AXIS`, `Principal`, `vec n` Fortran writes run) **[fact]**. Only `CENTER`
is used by OBJPOS.

### 2.1 Driver

1. Face type: `type = 3`; for every face, if `f[0] != abs(f[3])` then `type = 4` **[fact]**. So a
   surface is processed with the triangle rule only when *every* face is a triangle (4th node equal
   to the first); one quad sends all faces, triangles included, through the quad rule (a triangle
   becomes a quad with node 4 = node 1).
2. Allocate work arrays (normals `3*nF`, areas `nF`), Gauss tables `FUN_1403ee538`, then
   `FUN_1403ea82d(P, F, &1, &nF, &type, normals, areas, &mode=0, ...)` **[fact]**.
3. `CENTER = (m[0x13], m[0x14], m[0x15])` of the module block at `0x140b44d80`, i.e.
   `0x140b44e18/e20/e28`, copied right after this first call **[fact]**. Everything after
   (principal axes, second call) cannot change `CENTER`.

### 2.2 `FUN_1403ea82d` (mode 0)

1. For each face `f = 1..nF`: `FUN_1403ec3cc` computes the unit normal `n_f` and area (section
   2.3). Normals are used only by the triangle rule.
2. For `k = 1..10`: integral `I_k` by `FUN_1403ed06d` (type 3) or `FUN_1403ed5af` (type 4), each
   starting from `0.0` and running over all faces in order. Then **[fact]**:
   `V = I_1` (`0x140b44e10`), `Cx = I_2 / V`, `Cy = I_3 / V`, `Cz = I_4 / V` (`divsd`), `k = 5..10`:
   moments about the centre (need `C` of the same call).

Integrands `FUN_1403edb4f(x, y, z, &f, n, &k)`, with `F` a vector whose unused components are
`+0.0` and the listing grouping of `F.n` **[fact]**:

| k | F | `f` exactly |
|---|---|---|
| 1 | `(x, 0, 0)` | `(n2*0) + ((n1*0) + (x*n0))` |
| 2 | `((x*0.5)*x, 0, 0)` | `((F0*n0) + (n1*0)) + (n2*0)` |
| 3 | `(0, (y*0.5)*y, 0)` | `((n0*0) + (F1*n1)) + (n2*0)` |
| 4 | `(0, 0, (z*0.5)*z)` | `(F2*n2) + ((n1*0) + (n0*0))` |
| 5-7 | `((x'^3)/3.0 ...)` with `x' = x - Cx` etc. | inertia (only for AXIS) |
| 8-10 | `((x'*x')*y')*(-0.5)` etc. | products (only for AXIS) |

Each `f` equals `F_k * n_k` except for the sign of a zero result, which cannot change a non-zero
sum. So `V = sum x n_x dA` and `C_x = (sum (x^2/2) n_x dA) / V` (divergence theorem; the surface
must be closed, `V = 0` gives an infinite or NaN centre) **[inferred]**.

### 2.3 Triangle rule (`type 3`)

Normal `FUN_1403ec3cc` (once per face, before the integrals) **[fact]**:
`a = P[f1] - P[f0]`, `b = P[f2] - P[f0]` (componentwise),
`c0 = (a1*b2) - (b1*a2)`, `c1 = (a2*b0) - (b2*a0)`, `c2 = (b1*a0) - (a1*b0)`,
`L = sqrt((c1*c1 + c0*c0) + c2*c2)`; if `L > 0` (not NaN): `n_i = c_i / L`, else `n = (0,0,0)`.

Integral `FUN_1403ed06d`, for each face `f` (outer) and quadrature point `q = 1..3` (inner)
**[fact]**:

```
a  = P[f0] - P[f2] ; b = P[f1] - P[f2]          // note: a different edge pair than the normal
c0 = (a1*b2) - (b1*a2) ; c1 = (a2*b0) - (b2*a0) ; c2 = (b1*a0) - (a1*b0)
A  = sqrt((c1*c1 + c0*c0) + c2*c2) * 0.5        // DAT_140bb10c8
w  = A * 0.3333333333333333                      // DAT_140bb10d0 = 0x3fd5555555555555
L1 = T1[q], L2 = T2[q], L3 = (1.0 - L1) - L2
X_j = ((0.0 + P[f0]_j*L1) + P[f1]_j*L2) + P[f2]_j*L3      j = x, y, z
I  = (w * f_k(X, n_f)) + I
```

Tables (`0x140620860`, `0x140620880`): `T1 = (0x3fe5555555555556, 0x3fc5555555555557,
0x3fc5555555555557)`, `T2 = (0x3fc5555555555557, 0x3fe5555555555556, 0x3fc5555555555557)`
(2/3 and 1/6 rounded **up**, not the nearest doubles); hence `L3 = 0x3fc5555555555551,
0x3fc5555555555550, 0x3fe5555555555554` for `q = 1, 2, 3` **[fact]** (computed).

### 2.4 Quad rule (`type 4`): 2x2 Gauss on the bilinear face

`FUN_1403ed5af`, for each face `f` (outer), `i = 1..2` (xi), `j = 1..2` (eta, inner) **[fact]**:
`xi = G[i]`, `eta = G[j]` with `G = (-0.577350269189626, +0.577350269189626)` =
`0xbfe279a74590331e`, `0x3fe279a74590331e` (not the exact `1/sqrt(3)`), weights `W = (1.0, 1.0)`
(`FUN_1403ee538`, `DAT_140bb1150/1158`, `DAT_140bb1070`).

* Shape functions `FUN_1403ee73e(xi, eta, zeta = 1.0)`: the 8-node brick functions, face node `m`
  (1..4) = brick node `m+4` with `(xi_m, eta_m) = (-1,-1), (1,-1), (1,1), (-1,1)`, `zeta_m = 1`.
  With `p = xi*xi_m + 1.0`, `q = eta*eta_m + 1.0`, `r = 1.0 + 1.0*zeta_m` (= 2.0):
  `N = ((0.125*p)*q)*r`, `dN/dxi = ((xi_m*0.125)*q)*r`, `dN/deta = ((eta_m*0.125)*p)*r` **[fact]**.
* Tangents `FUN_1403eed50`: `t_k(j) = (((0.0 + P[f0]_j*dN_1k) + P[f1]_j*dN_2k) + P[f2]_j*dN_3k) +
  P[f3]_j*dN_4k`, `k = xi, eta`.
* Metric `FUN_1403eebc7` (recomputes the same tangents): `E = (t1x*t1x + t1y*t1y) + t1z*t1z`,
  `F = (t1x*t2x + t1y*t2y) + t1z*t2z`, `G = (t2x*t2x + t2y*t2y) + t2z*t2z`;
  `dA = sqrt((E*G) - (F*F))`; `wt = (W_i*W_j)*dA`.
* Point `X_j = (((0.0 + P[f0]_j*N_1) + P[f1]_j*N_2) + P[f2]_j*N_3) + P[f3]_j*N_4`.
* Normal (at the Gauss point, **no zero test**): `c0 = (t1y*t2z) - (t1z*t2y)`,
  `c1 = (t1z*t2x) - (t2z*t1x)`, `c2 = (t2y*t1x) - (t2x*t1y)`,
  `L = sqrt((c1*c1 + c0*c0) + c2*c2)`, `n = c / L`.
* `I = (wt * f_k(X, n)) + I`.

The per-face normals of `FUN_1403ec3cc` are not used by the quad rule **[fact]**. The UPSET_Brick
geometry is all quads, so the existing `OBJPOS/6-basic_3d` case runs this rule.

### 2.5 AXIS (not used by OBJPOS; summary)

`k = 5..10` give the inertia tensor about `C`; `FUN_1403f0b89` (Jacobi-type eigen solver, 3 KB,
not followed) gives the principal vectors, made right-handed (`FUN_1403eef06` cross product,
`FUN_1403f0590` sign test, flip of vector 1); the points are transformed into the principal frame
and `FUN_1403ea82d` runs again; with `d12 = |I11-I22|`, `d31 = |I33-I11|`, `d23 = |I22-I33|` the axis
is principal vector 3 if `d12` is strictly smallest, vector 1 if `d23` is, vector 2 if `d31` is,
and `ok = 0` then; on ties `AXIS = 0` and `ok = 1` **[fact]**. OBJPOS ignores both.

## 3. sin / cos

### 3.1 Die-geometry rotation: `FUN_140bb3000` = CRT `sin`, `FUN_140bb3490` = CRT `cos`

* Which is which **[fact]**: their NaN/Inf handlers `FUN_1404a0abc` / `FUN_1404a0ae4` call
  `FUN_1404a0934` with the names `"sin"` (`0x1405708bc`) / `"cos"` (`0x1405708b8`); small
  arguments return `x - x^3/6` resp. `1 - x^2/2`.
* What it is: statically linked, not an import. Both live in a separate executable section named
  `text` (`0x140bb3000-0x140bb3fff`, constants in `data`/`.rdata` at `0x14056dc80..`), written in
  assembly. Constants are those of AMD's x86-64 libm (`piby2_1 = 0x3ff921fb54400000`,
  `piby2_1tail = 0x3dd0b4611a626331`, `piby2_2 = 0x3dd0b4611a600000`,
  `piby2_2tail = 0x3ba3198a2e037073`, `twobypi = 0x3fe45f306dc9c883`, limit `5e5`, Payne-Hanek
  `FUN_1404a0430` above it); the CRT is the static Visual Studio 2008 x64 CRT (Ghidra matches
  `calloc` etc. to "Visual Studio 2008 Release"), whose x64 `sin`/`cos` come from AMD
  **[inferred]**. There is no CPU dispatch (no FMA variant).
* Not correctly rounded: a Python transcription of the listing (`amdsc.py`) is within 1 ulp of a
  high-precision reference on 70 000 random arguments and correctly rounded on 96.8% of them; over
  the 102 858 OBJPOS-style arguments `(a*pi)/180`, `a = -360..360` step 0.007, it differs from the
  host (Windows UCRT) `sin` in 1.1% and `cos` in 1.3% of the cases **[fact]** (computed). A port
  must therefore transcribe this routine (or link the VS2008 x64 CRT); `std::sin` is not
  bit-exact. Confirmed by oracle run (`OBJPOS/v-bits-rot_3d`, 40.6 degrees, die object 2): with
  `amdsc.py` and the exact 4x4 sequence of OBJPOS.md 6.4 all 234 DIEGEO doubles of the GENDB
  output match; with the host UCRT or the Fortran kernel 63 of 234.

Algorithm (listing `l_bb3.txt`; `S1..S6` = `0x14056dd80..ddd0` for sin and `0x14056df00..df50`
for cos, same values `-1/6, 0x3f81111111110bb3, 0xbf2a01a019e83e5c, 0x3ec71de3796cde01,
0xbe5ae600b42fdfa7, 0x3de5e0b2f9a43bb8`; `C1..C6` = `1/24, 0xbf56c16c16c16967, 0x3efa01a019f4ec91,
0xbe927e4fa17f667b, 0x3e21eeb690382eec, 0xbda907db47258aa7`) **[fact]**:

```
Inf/NaN: handler (Inf -> NaN 0xfff8..., NaN -> quiet NaN)
|x| <= pi/4 (bits <= 0x3fe921fb54442d18):
  sin: |x| < 2^-27: x;  |x| < 2^-13: x - ((x*x)*x)*(1/6)
       else x2=x*x; p=(S6*x2+S5)*x2+S4; q=(S3*x2+S2)*x2+S1; x6=(x2*x2)*x2; x + (x*x2)*(p*x6+q)
  cos: |x| < 2^-27: 1.0;  |x| < 2^-13: 1.0 - (x*x)*0.5
       else x2=x*x; x4=x2*x2; x8=x4*x4; A=(C2*x2+C1)*x4; B=(C4*x2+C3)*x8; Cc=(x4*x8)*(C6*x2+C5)
            h=x2*(-0.5); t=h+1.0; (((1.0-t)+h) + ((A+B)+Cc)) + t
otherwise (x := |x|; sin remembers neg = (x was < 0)):
  |x| < 5e5: npi2 = (int)trunc(x*twobypi + 0.5); n = (double)npi2
             rhead = x - piby2_1*n; rtail = piby2_1tail*n; r = rhead - rtail
             if (exp(x) - exp(r) > 15): t = rhead; rtail = piby2_2*n; rhead = t - rtail
                    rtail = piby2_2tail*n - ((t - rhead) - rtail); r = rhead - rtail
             rr = (rhead - r) - rtail
  else       FUN_1404a0430(x) -> r, rr, npi2 (not followed)
  with x2 = r*r:
    SINP(r) = small: exp(r) <= 0x3de: r;   exp(r) < 0x3f2: r - ((1/6)*r)*x2
              else sin_piby4 = (rr + ((r*x2)*(p*x6+q) - (x2*0.5)*rr)) + r      (p, q, x6 as above)
    COSP(r) = small: exp(r) <= 0x3de: 1.0; exp(r) < 0x3f2: 1.0 - x2*0.5
              else cos_piby4 = ((Q + P*x6)*x4 + (((((0.5*x2)-1.0)+1.0) - x2*0.5) - r*rr))
                               - ((x2*0.5) - 1.0)
              with x4=x2*x2, x6=x4*x2, P=(C6*x2+C5)*x2+C4, Q=(C3*x2+C2)*x2+C1
  sin: v = (npi2 even) ? SINP : COSP;  negate (0.0 - v) when bit1(npi2) != neg
  cos: v = (npi2 even) ? COSP : SINP;  negate (0.0 - v) when ((npi2 + 1) & 2) != 0
```

(`exp(.)` = biased exponent field. The final cos sign test is from the decompile, the rest from
the listing.) Used by `FUN_140221cd0` (die-geometry 4x4, OBJPOS.md 6.4) and 30 other callers.

### 3.2 Fortran SIN/COS used by both tensor kernels and the point kernels

Both tensor kernels call `FUN_1404b3b30(x, &s, &c)` (listing), which calls `0x1404b3ab0` (sin) then
`0x1404b3ab8` (cos), both entries of `FUN_1404b3960` (flag 0 / 1) **[fact]**: the Cody-Waite
routine already transcribed in OBJPOS.md 6.3.1 (`C1 = 3.1416015625`, `C2 = -8.908910206761538e-06`,
`YMAX = 210828714`, degree-8 odd polynomial). Its constants and structure are Cody & Waite's
*Software Manual for the Elementary Functions* SIN/COS, as in the SGI/Open64 math library that the
PathScale runtime inherits **[inferred]**. Same kernel for `FUN_14043658f` (section 4.2).

## 4. 2D mirror sub-parts (`FUN_1403066a0`)

### 4.1 Boundary-face arrays `FUN_140305180` (listing checked)

For each boundary loop `k < obj+0xbd8` (only when `0 < count < 100`), with `cnt = obj+0xbe0+0x30k`
and `off` as in OBJPOS.md 6.6 step 3, the first `cnt-1` entries from `off` are reversed in place
(`FUN_140305360` 4-byte, `FUN_140305420` 8-byte; `i <-> cnt-2-i`, exact copies) in this order
**[fact]** (names: objarrays.md): `+0xae8` ECCDEF, `+0xaf0` ECDEFN, `+0xaf8` ECPRES (8 B),
`+0xb00` ECCTMP, `+0xb08` ECTMFN, `+0xb10` ECHFLX (8 B), `+0xb18` ECCATM, `+0xb20` ECATFN, then
`+0xb10` ECHFLX **again** (the listing loads `[RAX+0xb10]` twice). Net effect: ECHFLX keeps its old
order and **ECAFLX (`+0xb28`) is never reversed**, so after a 2D mirror ECHFLX and ECAFLX stay on
the old face numbering while the other seven follow the new one. Reproduce as is. Confirmed by
oracle run (`OBJPOS/v-ecflx_2d` vs `v-ecflx-ctrl_2d`, values 5 and 7 set on faces (1,2), (2,3) of
SPIKE_SI object 1): after the mirror ECPRES is on faces (2,1), (3,2) while ECHFLX and ECAFLX are on
faces 1 and 2 of the new numbering, nodes (207,219), (219,224).

### 4.2 Edge re-ordering after the mirror: `FUN_1402788a0(edge)`

Called by `FUN_140278740` once per mirrored edge **[fact]**. Edge fields: `+0x10` type, `+0x14`
record count, `+0x18` records; width `FUN_140276090(type)`: type 1 -> 3, type 2 -> 6, else 0
**[fact]**. Type 1 records are points `(x, y, v)`; type 2 records are `[kind, x1, y1, x2|cx, y2|cy,
angle]` with `kind` 1.0 = line, 2.0 = arc (start, centre, sweep in radians) **[inferred]** from
the operations below.

```
if (count < 1 || records == NULL) return
edge+0x28 = 1                                     // FUN_14027f410, "modified" flag [inferred]
buf = copy of the records
if (type == 2): buf[r][5] *= 57.29577951308232    // DAT_140522fd8, every record
FUN_14041f590(&w, &count, buf)                    // Fortran
copy buf back into the edge
```

`FUN_14041f590(&w, &n, R)` **[fact]** (listing):

1. Only when `w == 6`: `FUN_14045a140(R, &n)`: for every record, `r5 = (r0 == 2.0) ?
   r5*0.017453292519943295 : 0.0` (`DAT_140bb2250`, `DAT_140bb2258`). Then per record: copy it to
   a static; `kind = cvttsd2si(r0)` (truncation);
   * `kind == 1`: swap the end points: `(r1, r2, r3, r4) = (old r3, old r4, old r1, old r2)`;
   * otherwise (arc): `FUN_14043658f(&r1, &r3, &r5, &E)` computes the arc end point
     `d = (r1 - r3, r2 - r4)`, `sn, cs = SIN, COS(r5)` (Fortran, 3.2),
     `E.x = ((cs*d.x) + r3) - (sn*d.y)`, `E.y = (d.y*cs) + ((d.x*sn) + r4)` (listing; the decompile
     groups `E.y` differently); then `r1, r2 = E`, `r5 = -r5` (sign flip). The centre is kept.
2. For all widths: reverse the record order, `i = 1..n/2` (integer division) swapped with
   `n+1-i` through a static buffer (exact copies).

Confirmed by oracle run: `gk-4-type2_2d` (line record `(-10,-1.77)-(50,8.825)` becomes
`(-50,8.825)-(10,-1.77)`, `r5 = 0`) and the type-1 point lists of `4-basic_2d` / `4-oblique_2d`
(points mirrored and their order reversed).

So the mirror (OBJPOS.md 6.6 step 1: points mirrored, `r5` negated) followed by this routine turns
each contour around: line records swap their ends, arcs start at their old end with the opposite
sweep, and the record list is reversed. The angle goes through `(r5 * 57.29577951308232) *
0.017453292519943295` (not an identity in floating point) before the sine/cosine and is stored
negated in that rounded form; line records of type-2 edges end with `r5 = 0.0`; a record whose
`r0` is neither exactly 2.0 nor truncates to 1 is treated as an arc with sweep 0 (end = start)
**[fact]**.

### 4.3 2D window mirror `FUN_140268890(w, A, B)` and orientation `FUN_140265bf0`

Used for remesh windows, BC window lists `+0x78` / `+0xa8` (OBJPOS.md 6.6 step 4) and the
inter-object windows `FUN_14019f830` **[fact]**:

1. Skipped when `A` or `B` is NULL. Each point `w+0x180 + 16*i`, `i < w+0x178`, mirrored with
   `FUN_140309640` (OBJPOS.md 6.6b).
2. `FUN_140265bf0(w, &reverse = 1)`: nothing when the point count `<= 0`. 2D windows
   (`w+0x898 == 2`, `FUN_14025c930`): signed area `FUN_140309550`: `A = 0.0`; for `i = 0..n-1`
   (`j = i+1`, wrapping to 0): `A = A + 0.5*((x_i*y_j) - (y_i*x_j))` (decompile grouping; only the
   sign is used). If `A >= 0.0` (`DAT_14051bed8`) return; else reverse the point order (exact
   copies). A counter-clockwise window therefore stays counter-clockwise after the mirror; a
   window with area exactly 0 is not reversed. (3D windows use a normal test instead; not used by
   the 2D mirror.)

`FUN_14019f830(m, A, B, o#)`: for every usable object whose number is `o#` (normally exactly one),
for every inter-object entry `E` with `E+0x188` windows and (`E+0x170 == 2`, master `E+0x168 ==
o#`) or (`E+0x170 == 1`, slave `E+0x16c == o#`): each window `FUN_140268890` **[fact]**.

### 4.4 2D own-centre bbox (mode 6): `FUN_140279040(edge, min, max, &zmax)`

Returns false when `count < 1` or no records **[fact]**. Comparisons: min takes `v` when
`v <= cur`, max takes `v` when `cur <= v` (a NaN never replaces the current value) **[fact]**.

* type 1: `min = max = (r0, r1)` of record 0 and `zmax = r2`; records `1..n-1` update with
  `(r0, r1)` and `zmax` with `r2`.
* type 2 (and anything not 1): `min = max = (r1, r2)` of record 0, `zmax = 0.0`; records `0..n-1`
  update with `(r1, r2)` then `(r3, r4)`; records with `cvttsd2si(r0) == 2` update `zmax` with `r5`.
  So arc **centres** enter the box and arc bulges do not; the 2D mode-6 centre of a die with arcs is
  the centre of this end-point/centre box, not of the true outline **[fact]**.

`FUN_1401ff600` unions the boxes of all edges (`FUN_140198310` count, `FUN_1401982c0`): the first
edge with a box sets min/max (x, y only), later edges replace a component only when strictly
smaller / larger; `zmax` is dropped **[fact]**. Then OBJPOS.md 8.5: `C = (min + max) / 2.0`.

## 5. Sub-object `obj+0x2be8` (GEO23 block)

It is the GEO23 structure of `docs/re/keywords/GEO23.md` (`G23`, mode at `+0`, back pointer
`+0x120` = `obj+0x3a98`); offsets `+0x38/+0x39/+0x3c/+0x40/+0x50` are MSHSEC/GEOSEC/CRDSYS
(`obj+0x2c20..0x2c38`) **[fact]**. The parts moved by OBJPOS are two optional frames
**[fact]** (constructor `FUN_1401889f0`, called via `FUN_1401889a0` from `FUN_14015ec80`
**[inferred]** object constructor):

| Offset | Init | Translate `FUN_140220080` | Rotate `FUN_140220190` | Mirror `FUN_14021f940` | 4x4 `FUN_1402202a0` (drop) |
|---|---|---|---|---|---|
| `+0x81` byte | 0 | gate | gate | gate | gate |
| `+0x88` vec | (1,0,0) | - | as point | as point (float) | transformed + normalised |
| `+0xa0` pt | (0,0,0) | += d | as point | as point | transformed |
| `+0xb8` pt | (0,0,1) | += d | as point | as point | transformed |
| `+0xd0` byte | 0 | gate | gate | gate | gate |
| `+0xd8` vec | (1,0,0) | - | as point | as point | transformed + normalised |
| `+0xf0` vec | (0,0,1) | - | as point | as point | transformed + normalised |
| `+0x108` pt | (0,0,0) | += d | as point | as point | transformed |

So `+0x88`, `+0xd8`, `+0xf0` are directions and the rotate/mirror treat them as points (wrong for
an origin other than 0) **[inferred]**. The two flags are never set in DEF_PRE: a scan of every
instruction for byte stores to `[reg + 0x81]` / `[reg + 0xd0]` finds only the constructor's zero
stores (the others are stack slots), there is no keyword or DB reader for these fields, and the
equality test `FUN_140188f50` ignores them **[fact]**; hence these branches are dead in batch mode
and a port can skip them **[inferred]** (a store through an indexed address would escape the scan).
Side note for the GEOSEC owner: the same constructor sets `+0x3c` (GEOSEC count) to 20 and then
calls `FUN_140189200(G23, 210.0)`, which overwrites it with `(int)(210.0/3.0) = 70` and sets
`+0x10 = (180.0 - 210.0)/2.0 = -15.0` because `+0x80 = 1` **[fact]** (decompile; whether this is
the default seen by KFREAD was not checked).

## 6. Tests

`tests/commands/OBJPOS/cases_gk.json` (5 cases, labels `OBJPOS/gk-*`):

| Case | Seed | Exercises |
|---|---|---|
| `gk-tensor-y_3d` | `key/3D/EN/Forging/Spike_ForgingBlow2.KEY` (object 1: tet mesh, STRESS and STNCMP 4978x6 non-zero, no geometry) | 1.2 with axis y (degenerate frame branch, `r1 = 0`), GENDB |
| `gk-tensor-obl_3d` | same | 1.2 with axis (1,1,1), full frame, GENDB |
| `gk-6-tri_3d` | same | 2.3 triangle rule on the mesh-derived surface (that the temporary surface faces are all triangles is **[inferred]**) |
| `gk-tensor_2d` | `key/2D/SI/ResistanceSpotWelding/Resistance_Spot_Welding_elstoplastic_model_with_phase_transformation_Hold_cycle_SI.KEY` (STRESS, STNCMP 1265x4) (seed replaced after the oracle run: DEF_BOOLEAN_OUT.KEY fails KFREAD with `ERROR: Keyword ELMCON Reading Error.`, so the old case never rotated anything) | 1.3 (KEY only; GENDB refuses this seed) |
| `gk-4-type2_2d` | `key/2D/SI/HeatTreat/HT_QUENCH1_SI.KEY` (object 2: type-2 edge, one line record) | 4.2 line branch |

The quad rule (2.4) is already run by `OBJPOS/6-basic_3d` (UPSET_Brick geometry is all quads).
No corpus 2D die has arc records (all type-2 edges found hold one line record), so the arc branch
of 4.2 has no test.

## 7. Helper table

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140184ac0` | element tensor dispatch | 2596 | read fully |
| `FUN_140325884` | 3D tensor rotation (Fortran) | 4161 | listing, simulated |
| `FUN_14032e585` | 2D tensor rotation (Fortran) | 1121 | listing |
| `FUN_1404b3b30`, `FUN_1404b3960` | Fortran SINCOS, SIN/COS | 58, 336 | listing (OBJPOS.md 6.3.1) |
| `FUN_1401fe7d0` | mode-6 3D centre driver | 639 | read |
| `FUN_1403e8510` | mass properties driver (CENTER/AXIS) | 8989 | read (debug writes skipped) |
| `FUN_1403ea82d` | integrals k=1..10 | 7071 | mode 0 read, listing for divisions |
| `FUN_1403ec3cc` | face normal/area | 2044 | triangle part listing |
| `FUN_1403ed06d` | triangle 3-point rule | 1346 | listing |
| `FUN_1403ed5af` | quad 2x2 Gauss rule | 1440 | listing |
| `FUN_1403edb4f` | integrands (jump table `0x140bb10e0`) | 1554 | listing cases 1-4, decompile 5-10 |
| `FUN_1403ee73e`, `FUN_1403eed50`, `FUN_1403eebc7`, `FUN_1403ee538` | shape functions, tangents, metric, Gauss tables | 735, 438, 393, 518 | listing (`ee538` decompile) |
| `FUN_1403f0b89`, `FUN_1403eef06`, `FUN_1403f0590` | eigen solver, cross, sign | 3028, 93, 215 | not followed (AXIS only) |
| `FUN_140bb3000`, `FUN_140bb3490` | CRT sin, cos (AMD libm) | 1089, 1094 | listing, transcribed |
| `FUN_1404a0abc`, `FUN_1404a0ae4`, `FUN_1404a0934` | sin/cos NaN/Inf handlers | 18, 18, 218 | read |
| `FUN_1404a0430` | Payne-Hanek reduction (`|x| >= 5e5`) | 1281 | not followed |
| `FUN_140305180`, `FUN_140305360`, `FUN_140305420` | 2D face-array reversal | 459, 185, 174 | listing / read |
| `FUN_140278740`, `FUN_1402788a0`, `FUN_140276090`, `FUN_14027f410` | 2D edge mirror and re-order | 340, 329, 60, 23 | read |
| `FUN_14041f590`, `FUN_14045a140`, `FUN_140437fc0`, `FUN_14043658f` | Fortran edge re-order, angle conversion, copy, arc end point | 662, 145, 135, 253 | listing / read |
| `FUN_140268890`, `FUN_140265bf0`, `FUN_14025c930`, `FUN_140309550`, `FUN_14019f830` | 2D window mirror, orientation, area, inter-object windows | 153, 1253, 47, 224, 448 | read |
| `FUN_140279040`, `FUN_1401ff600` | 2D edge bbox, union | 2315, 479 | read |
| `FUN_1401889f0`, `FUN_140188f50`, `FUN_140189200`, `FUN_1401892e0`, `FUN_140220080`, `FUN_140220190`, `FUN_14021f940`, `FUN_1402202a0` | GEO23 block | - | read |

About 45 functions followed (plus constant/jump-table dumps and two instruction scans).

## 8. Open items

1. `FUN_1404a0430` (sin/cos for `|x| >= 5e5` rad, i.e. angles above ~2.9e7 degrees) not followed.
2. ~~CRT sin/cos not validated against DEF_PRE~~: validated bit-exact on one angle (3.1); the
   Payne-Hanek branch and the cos sign rule for other quadrants are not separately exercised.
3. ~~Temporary surface of a tet mesh~~: `gk-6-tri_3d` prints origin (0.500461, 0.502164,
   -0.21001); the triangle rule of 2.3 over the FNDBND boundary faces of the tet mesh gives
   C = (0.50046083, 0.50216408, -0.21000991) and the rotated RZ then agrees with the KEY on 1268 of
   1269 rows (one last-digit difference on a 2e-6 value), consistent with all-triangle faces.
4. AXIS chain (`FUN_1403f0b89` etc.) not followed; not needed by OBJPOS.
5. No corpus case for the arc branch of 4.2.

## 9. Note for the parent (OBJPOS.md)

Suggested edits to OBJPOS.md (not made here): in 6.7 replace "The exact summation order ... open
item 1" by a link to section 1 here and add the STNCMP block handling (1.1); in 6.4 change
"(which is cos/sin is [inferred])" to [fact] and link section 3.1 (AMD libm, must be transcribed);
in 7.4 replace "`FUN_1403e8510` FP order not followed (open item 2)" by a link to section 2; in 6.6
steps 1, 3, 4 link sections 4.2, 4.1 (add "ECAFLX never reversed") and 4.3; in 8.5 link 4.4; in
6.1/6.2/6.5 name `obj+0x2be8` as the GEO23 block and link section 5 (dead in batch); in the helper
table move `FUN_140bb3490/3000`, `FUN_140325884/14032e585`, `FUN_1403e8510`, `FUN_1402788a0`,
`FUN_140268890`, `FUN_140279040` to "read" with a link; in section 12 close items 1, 2, 3, 5, 6 and
keep 4 (other worker), 7, 8; in section 11 mention `cases_gk.json`.
