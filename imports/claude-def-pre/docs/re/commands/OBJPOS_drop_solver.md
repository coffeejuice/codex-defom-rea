# OBJPOS drop: the settling solver `FUN_14035b861` and its kernels

Part of the OBJPOS drop spec ([OBJPOS_drop.md](OBJPOS_drop.md): grammar, wrapper `FUN_1401a7d60`,
setup `FUN_140359a70`, transform composition). This file covers everything from the call of
`FUN_14035b861` to the returned `t[3]`, `R[12]`, `status`. The contact search it calls is the
shared 3D contact engine of GENCTC, specified in [GENCTC_3d_search.md](GENCTC_3d_search.md) and
[GENCTC_3d_decide.md](GENCTC_3d_decide.md); geometric kernels (`FUN_1404cf5c0`, `FUN_1404ded0c`,
`FUN_1404cc289`, `FUN_1404ccf58`, `FUN_1404d6974`, `FUN_1404e2d44`, `FUN_1404e34eb`,
`FUN_1404d3d43`) are in [OBJPOS_interference.md](OBJPOS_interference.md) §5.5–5.12 and
[OBJPOS.md](OBJPOS.md) §6.3. Shared mechanics: [_common.md](_common.md).

Source: `DEF_PRE_64.exe` v11.0.2. Decompiles `out/p67/drop-solver/solver.c` (depth 6 from
`14035b861`, one function per file in `out/p67/drop-solver/f/`), listing of the whole module
`out/p67/drop-solver/L_all.txt` (0x140359a70–0x140375400) and `L_contact.txt` (`FUN_14001f54f`).
**[fact]** = read in code, **[inferred]** = deduced.

**Floating-point order.** The Ghidra decompile drops parentheses on the right operand of `+`/`*`
(for example it prints `wz*I33 + wy*I32 + wx*I31` for `wz*I33 + (wy*I32 + wx*I31)` and
`sqrt(q)*w1*w2` for `sqrt(q)*(w1*w2)`). Every expression with three or more operands below was
re-derived from the listing with a symbolic SSE tracer (`scratchpad/ds/sym.py`, output
`out/p67/drop-solver/sym_multi.txt`) and is written fully parenthesised. All arithmetic is scalar
SSE2 double; `-x` is a sign-bit XOR (`DAT_140baf9e8`), `|x|` an AND (`DAT_140baf948`); `sqrt` is
`SQRTSD`; no `x87`, no FMA. Integer conversions: `nint(x)` = Fortran NINT
(`|x| >= 2^52 ? x : x + (x >= 0 ? 0.5 : -0.5)`, then `cvttsd2si`), `int(x)` = `cvttsd2si`
(truncation).

Notation: Fortran arrays are 1-based and column-major; `X(:,n)` = coordinates of node n
(`X + 24(n-1)`), `F(:,f)` = 4 node numbers of face f (`F + 16(f-1)`). `dot(a,b)` = `FUN_140367f0b`
(§5.15): `s = 0.0; for k: s = a_k*b_k + s`, i.e. `((0.0 + a1b1) + a2b2) + a3b3`.
`cross(a,b)` = `FUN_140366881`: `(a2*b3 - b2*a3, a3*b1 - b3*a1, a1*b2 - b1*a2)`.
`unit(a)`: `l = sqrt(dot(a,a)); if l > 0: a_k = a_k / l` (three divisions).
All locals are static (`DAT_1407xxxxx`); values survive between calls of the solver.

## 1. What the solver does

An explicit rigid-body dynamics loop with penalty contact **[fact; physical reading inferred]**:

1. Mass properties of the moving body (density 1) from its closed surface (divergence theorem).
2. Contact detection of the moving body's nodes against the fixed surface with the GENCTC 3D
   engine in "penetration" mode (a node is in contact when its signed distance is ≤ 0).
3. If no node is in contact: cast rays from the moving nodes along the drop direction `d`; move
   the body rigidly by `dmin + p15/2` (so it penetrates by `p15/2`). If no ray hits, cast from the
   fixed nodes along `-d`; if that hits, move and **stop**; if nothing hits, stop without moving.
4. Equilibrium test (`FUN_140369c19`). If stable, stop.
5. Iterate (at most `itmax − 5` = 195 times): gravity `m·9800·d`, gyroscopic moment, a constant
   normal force per contact node rescaled so that its total along `d` equals the weight, 6×6
   solve for accelerations, velocity update, projection onto the allowed motions (symmetry planes,
   axis, mode-9 vectors), a step-size search that limits the rotation to `g` = 0.5° per trial
   step and bisects until newly touching nodes penetrate by at most `p15`, rigid update of the
   coordinates, mass properties again, contact detection again, equilibrium test.
6. Return the accumulated rigid motion as translation `t` and rotation rows `R[0..8]` about the
   final centre `R[9..11]`, `status = 0`.

The node coordinates `X` of the moving body (the live array of the object, see OBJPOS_drop §4)
are moved **in place** by every rigid update **[fact]** (closes OBJPOS_drop open item 2). The
returned matrix is built from separately tracked points (§4.9), not from `X`.

The only failure after the setup checks is a singular 6×6 system (§5.10). Hitting the iteration
limit, a diverging velocity or "no hit at all" all return `status = 0` **[fact]**.

## 2. Interface and data

### 2.1 Arguments (34, all by reference unless stated)

`FUN_140359a70` (setup) calls (listing 0x14035b3xx, decompile `drop.c` line 690) **[fact]**:

| # | Value | Name used here |
|---:|---|---|
| 1 | moving coordinates `X` (wrapper's live array) | `X` (3×nn) |
| 2 | `DAT_140704410` | `RT` real body table (200 doubles per body) |
| 3 | fixed coordinates `XU` | `XU` (3×nU) |
| 4 | `DAT_1407046d0` | `IT` int body table (20 ints per body) |
| 5 | moving faces `F` | `F` (4×nf) |
| 6 | fixed faces `FU` | `FU` (4×nUf) |
| 7 | `DAT_140704620` | node list `1..nn` |
| 8, 9 | `&nb` (2), copy | |
| 10, 11 | copies of `nn`, `nf` | |
| 12, 13 | `&nf`, `&nn` | |
| 14, 15 | `&nUf`, `&nU` | |
| 16 | `DAT_140704470` | `PT` pair table (20 doubles per body pair) |
| 17 | `DAT_140704390` | `NRM(3,3,nn)` contact normal per node and slot |
| 18 | `DAT_140704730` | `XE(2,3,nn)` (ξ, η) of the contact point per slot |
| 19 | `DAT_1407045c0` | `BC(3,nn)` contact codes (negative = −body) |
| 20 | `DAT_140704670` | `FID(3,nn)` contact face of the fixed body per slot |
| 21 | `DAT_140704570` | node flags `1..nn` (all > 0) |
| 22 | `DAT_1407044f0` | `SW(2,3,nn)` ints, zero, never written (§4.4) |
| 23 | `dir` (unit, normalised by the setup) | `d` |
| 24, 25 | `&nk`, `K(6,3)` | constraints (point, normal) |
| 26, 27 | `&nv`, `v(3,2)` | mode-9 vectors |
| 28 | `&g` (0.5) | max rotation per step in degrees |
| 29 | `&p15` (token 2, default 0.002) | gap/penetration tolerance |
| 30 | `&itmax` (200) | |
| 31 | `&verbose` (0) | |
| 32, 33 | `t[3]`, `R[12]` (out) | |
| 34 | `&status` (out) | |

Body tables written by the setup **[fact]** (body 1 = moving, body 2 = fixed):

| Field | Body 1 | Body 2 | Meaning [inferred] |
|---|---|---|---|
| `IT[0]` | 1 | 2 | body number |
| `IT[1]` | 1 | 0 | > 0 free, 0 fixed |
| `IT[4]`, `IT[5]` | 1, nn | 0, 0 | node range |
| `IT[6]`, `IT[7]` | 0, 0 | – | face-group range (gives one group) |
| `IT[8]`, `IT[9]` | 1, 1 | 2, 2 | `RT` group range |
| `IT[12]` | 4 if `F(4,1) == F(1,1)` else 8 | – | element type → 3 or 4 nodes per face |
| `IT[13]` | 1 | – | |
| `RT[0]` | 3.0 | 1.0 | body type (3 = node set with faces, 1 = rigid surface) |
| `RT[1..4]` | nn, 1.0, nf, 1.0 | – | node count, first node, face count, first face |
| `RT[5..8]` | – | nU, 1.0, nUf, 1.0 | same for the rigid surface |
| `PT(·)[0]` | pair (2,1) = 2.0, pair (1,2) = 1.0 | | 1 = "body 2 is a contact partner of body 1" |

Arrays allocated by the solver (freed at the end) **[fact]**: `DAT_1407049f0` face vectors
(3·nf), `DAT_140704a40` node velocities `VN(3,nn)`, `DAT_140704a90` previous `VN`,
`DAT_140704840` / `DAT_140704890` / `DAT_1407048e0` node lists (nn ints),
`DAT_140704930` per-node contact force/moment (6·nn, written, never read),
`DAT_140704990` coordinate backup (3·nn), `DAT_1407047f0` signed gap per node `GAP(nn)`.

### 2.2 Body state (COMMON at 0x140b6cca0..0x140b6ce78) **[fact]**

| Address | Content |
|---|---|
| `cca0 [0..2]` | `C0`: centre of mass at the start |
| `cca0 [3..5]` | `CT`: tracked centre (moved with every rigid update) |
| `cca0 [6..8], [9..11], [12..14]` | `A1, A2, A3`: tracked points, start `C0 + e_k` |
| `cd40` (9) | inertia tensor `I(3,3)` about the centre |
| `cd88` (9) | mass matrix `m·I3` |
| `cdd0` | mass (= volume) `m` |
| `cdd8..cde8` | centre of mass `c` (recomputed from the geometry) |
| `cdf0..ce00` | rotation increment of the last update (angles, §5.13) |
| `ce20..ce30`, `ce38..ce48` | velocity `v`, angular velocity `w` |
| `ce50..ce78` | acceleration (linear, angular) |
| `b84720`, `b84728`, `b84730` | projection tolerances 1e-4, 0.01, characteristic length (§5.2) |

Solver scalars: `dt = DAT_140704cc8` (0.001, constant), `V(6) = DAT_140704d30` (unprojected
velocity), `|V| = DAT_140704d60`, `W = DAT_140704d68` (weight), `k = DAT_140704d90` (stiffness),
`kc = DAT_140704d78` (stiffness per contact), `damp = DAT_140704d80` (never used), `nc =
DAT_140704d88` (contact node count), `it = DAT_140704cd0`, `stop = DAT_140704d98`,
`shift = DAT_140704d9c`, `redrop = DAT_140704d8c`, `ins = DAT_140704cf0` (0).

## 3. Corrections to OBJPOS_drop.md §5 (setup `FUN_140359a70`)

* Direction length: `l = sqrt((d3*d3) + ((d2*d2) + (d1*d1)))` **[fact, listing 0x14035af3d]**,
  i.e. `x² + y²` first, then `z²` (not "z² first").
* The mode-9 vectors are divided by their **squared** length: `q = dot(v1,v1); if q > 0: v1_k =
  v1_k / q` (same for `v2`) **[fact, listing 0x14035b044–0x14035b0d5]**. A vector of length L
  becomes `v/L²`, and the rotation it allows (§5.11) is scaled by `1/L⁴`. Unit vectors are
  unaffected.
* For `nk = 3` the wrapper fills only `K(:,3)`; `K(:,1)`, `K(:,2)` are uninitialised stack
  (`local_358` is never cleared, `ifd_deep.c` FUN_1401a7d60) **[fact]**. The solver reads
  `K(4:6,1)` in §5.13 also for `nk = 3`; with `w = 0` (§5.11) the result is a zero rotation
  unless the garbage is Inf/NaN **[inferred]**.

## 4. Main routine `FUN_14035b861` (30040 B)

### 4.1 Initialisation **[fact]**

```
zero: A6(6,6) (DAT_140704af0), F6(6) (DAT_140704c10), accPrev(6) (DAT_140704c40),
      Sk(3,3) (DAT_140704c70), angles cdf0..ce00, v, w, acc (ce20..ce78)
itmax' = itmax; time = 0 (DAT_140704cd8, written only); it = 0; dt = 0.001 (DAT_140baf9b0)
count free bodies (IT[1] > 0) and fixed bodies (IT[1] == 0): return (status stays 1) unless
      exactly 1 free and >= 1 fixed                      // always true for the drop
for the free body: d24 = 1 (first face), d28 = nf, npf = (IT[12] == 8) ? 4 : (IT[12] == 4) ? 3
      allocate DAT_1407049f0, a40, a90, 840, 890, 8e0
      b84728 = 0.01 (DAT_140baf9c0); b84720 = 1e-4 (DAT_140baf9b8)
      FUN_1403750cf(X, F, &npf, &1, &b84730)             // §5.2
FUN_140365e0b()                                          // §5.1 Gauss table
allocate 930 (6·nn), 990 (3·nn), 7f0 (nn)
FUN_140362db9(...)                                       // §5.3 mass properties, constraints nk, K
V = (v, w) = 0;  |V| = sqrt(dot6(V,V))
FUN_1403668de(X, VN, &nn)                                // §5.4 (zero)
W = m * 9800.0 (DAT_140baf9c8)                           // weight, mm/s^2 [inferred units]
C0 = c; CT = c; A_k = 0; A1(1) = A2(2) = A3(3) = 1.0; A_k = C0 + A_k   (component add C0_i + A_k,i)
FUN_140225640(&10, &verbose)                             // cube table for 10 cubes (GENCTC_3d_search §2.1)
it = 5                                                   // listing 0x14035eb90: the counter starts at 5
```

### 4.2 First contact detection and initial drop **[fact]**

```
detect(X, flag = 1, tolscale = dt)                       // §5.5; nc = contact nodes
if nc == 0:
    redrop = 0
    initialDrop(...)                                     // §5.6, may move X
    if redrop == 1: goto FINISH
    FUN_140362db9(...)                                   // mass properties after the translation
detect(X, flag = ins (0), tolscale = dt)
FUN_140367fe2(&k, &damp, &W, &p15)                       // §5.7 (v = w = 0 here: k = 5·W)
kc = (nc == 0) ? k : k / (double)nc
FUN_140369c19(...)                                       // §5.12: stop, shift
if stop == 1: goto FINISH
```

### 4.3 Iteration (loop body, `it = it + 1` first) **[fact]**

```
loop:
  it = it + 1
  (verbose > 0: 'Iteration = ' it+1)
  zero A6, F6, Sk;  h = dt (DAT_140704da0 = dt)
  VNprev = VN (3·nn copy, unused afterwards)
  (DAT_140704da8 = (|V| < 1e-10f) || nc == 0 — written only)
  if it > 1: accPrev = acc                               // always true (it >= 6)
  (DAT_140704db0/dd0 = v, w — passed on, never read)
  A6(1:3,1:3) = m·I3 block (cd88); A6(4:6,4:6) = I (cd40); other entries 0
  F6(1:3) = d_k * W                                      // gravity
  gyroscopic (listing 0x14035f932–0x14035fb1d):
     L1 = (wz*I13) + ((wy*I12) + (wx*I11))
     L2 = (wz*I23) + ((wy*I22) + (wx*I21))
     L3 = (wz*I33) + ((wy*I32) + (wx*I31))
     F6(4) = (F6(4) - (wy*L3)) + (L2*wz)
     F6(5) = ((wx*L3) + F6(5)) - (wz*L1)
     F6(6) = (F6(6) - (wx*L2)) + (wy*L1)
     (Sk = skew matrix of L, written, not read)
  if nc > 0: contactForces(F6, W, d, kc)                 // §5.8
  nc = #{n : any BC(:,n) < 0};  kc = (nc == 0) ? k : k / (double)nc
  FUN_140366ac3(A6, F6, &6, &err)                        // §5.10, F6 := solution
  if err == 1: FUN_140225820(&verbose); return           // status stays 1, arrays not freed
  acc = F6
  time = h + time
  velocity (6 components i):
     nc >= 1: V_i = h * acc_i                            // no accumulation while in contact
     nc == 0: V_i = (((accPrev_i * 0.5) + (acc_i * 0.5)) * h) + V_i      // DAT_140baf9f0, ..918
  |V| = sqrt(dot6(V,V));  (v, w) = V
  constraints (§5.11) modify v, w only (V keeps the unprojected values)
  FUN_1403668de(X, VN, &nn)                              // §5.4
  (verbose > 0: ' VELOC(1:3)' VN(:,1))
  if |VN(1,1)| > 1e100 or |VN(2,1)| > 1e100 or |VN(3,1)| > 1e100 (DAT_140bafa10):
      stop = 1; break                                    // only node 1 is checked
  if stop == 1: break
  redrop = 0
  FUN_14036d3fd(...)                                     // §5.13: step search, moves X
  if redrop == 1: break
  (DAT_140704ea0 = c — written, never read)
  FUN_140362db9(...)                                     // mass properties of the moved body
  detect(X, flag = ins, tolscale = h)
  (verbose == 2: WKP2.GEO dump, §6)
  FUN_140369c19(...)                                     // §5.12
  if shift == 1: detect(X, flag = ins, tolscale = h)
  if stop == 1 or it >= itmax or DAT_140704ec4 == 1: break
  goto loop
```

`DAT_140704ec4` is the argument of the empty stub `FUN_14001cd40` and is never written outside
the verbose-2 I/O lists, so it is 0 **[fact]** (an interrupt hook compiled out [inferred]).
`FUN_14001cd20`/`cd30`/`cd40` are 3-byte `RET` stubs; `DAT_140704cd4` (start 20, `+20` when
`it+1 >= cd4-5`) only feeds them **[fact]**.

With `itmax = 200` the loop body runs for `it = 6..200`, at most **195 iterations**, and the
exit by the limit is not an error **[fact]**.

### 4.4 FINISH **[fact]**

```
(verbose == 2: WKP2.GEO)
FUN_140362db9(...)                                       // mass properties (only for the verbose print)
(verbose > 0: 'positioning finished', 'mass   : ' m, 'center : ' c, one line per contact node with
 format '200 ('contact point',I5,1X,4E15...)' : n, X(:,n), GAP(n))
t = (CT_1 - C0_1, CT_2 - C0_2, CT_3 - C0_3)
for k = 1..3: A_k = A_k - CT (componentwise); l = sqrt(dot(A_k, A_k)); A_k = A_k / l (divisions)
(verbose > 0: 'linear transform ' t, 'rotational matrix' A_k (3 lines), 'rotation center  ' CT)
R[0..8] = (A1, A2, A3) (row k = A_k); R[9..11] = CT
status = 0
free 1407049f0, a40, a90, 840, 890, 8e0, 930, 990, 7f0
FUN_140225820(&verbose)                                  // frees all cubes
```

The rows are only normalised, not re-orthogonalised. In the wrapper (OBJPOS_drop §4 step 6)
`p' = ((p + t − CT)·R) + CT` with the row-vector convention, so `R` maps `e_k` to `A_k` **[fact]**.

## 5. Kernels

### 5.1 `FUN_140365e0b` (686 B): Gauss table **[fact]**

Zeroes `GP(4,4)` at `0x140b6cba0` and `W(4,4)` at `0x140b6cc20` and stores the 1- to 4-point
Gauss-Legendre rules (`GP(n,i)`, `W(n,i)`, n = number of points): `GP(1,1) = 0`, `W(1,1) = 2.0`;
`±0.577350269189626` / 1.0; `∓0.774596669241483, 0` / `0.555555555555556, 0.888888888888889`;
`∓0.861136311594053, ∓0.339981043584856` / `0.347854845137454, 0.652145154862546`
(`DAT_140bafba8..140bafc00`, 15 significant digits, not the exact rules). Only the 1-point rule
is used (§5.3.3).

### 5.2 `FUN_1403750cf(X, F, &npf, &ngroups=1, &L)` (799 B) **[fact]**

`b = 1e10` (`DAT_140bafc50`); for group g = 1..ngroups (only g = 1): `max = -1e8`, `min = 1e8`
(`DAT_140bafe48/40`), over the `npf` nodes of **face g** (`F(k,g)`): `min_i = x < min_i`,
`max_i = x > max_i`; `q = ((0 + e1²) + e2²) + e3²` with `e = max − min`; `b = min(b, q)` (strict);
`L = sqrt(b)` → `b84730`. Only the first face is measured. `b84730` is used only when `b84720` or
`b84728` is 0 (§5.5.3), which never happens here, so it has no effect **[fact]**.

### 5.3 Mass properties `FUN_140362db9(X, F, &d24, &d28, &npf, FV, &nk, K)` (5256 B) **[fact]**

```
for i = 1..nf (face f = d24 + i - 1): FUN_140364241(X, F(:,f), FV(:,i), &npf)   // §5.3.1
S = 0
for j = 1..10 (nk == 3: j = 1..7):
    S = (npf == 4) ? FUN_140364e60(... case j) : FUN_1403649bb(... case j)       // §5.3.2/3
    j=1: m = S        j=2..4: c_(j-1) = S / m
    j=5: I11 = S      j=6: I22 = S      j=7: I33 = S
    j=8: I12 = S      j=9: I13 = S      j=10: I23 = S
    nk ∈ {1,2}, j=4:  project c onto plane K(:,1) (FUN_140365a10); nk == 2: also onto K(:,2)
    nk == 3,    j=4:  project c onto the axis K(:,3) (FUN_140365b02)
    nk ∈ {1,2}, j=10: n = K(4:6,1): |n1| == 1.0 → I13 = I12 = 0; |n2| == 1.0 → I23 = I12 = 0;
                      |n3| == 1.0 → I23 = I13 = 0  (exact equality); nk == 2: same with K(4:6,2)
    nk == 3,    j=7:  I12 = I13 = I23 = 0
I21 = I12; I31 = I13; I32 = I23
M = 0 (cd88..cdc8); M11 = M22 = M33 = m
```

Projection helpers (frame `T = FUN_1404cf5c0(n)`; if `n = 0` T keeps its previous value, see
OBJPOS_interference §5.5): `FUN_140365a10(c, p, n)`: `L = ded0c(c, p, T, mode 1)`; `L3 = 0`;
`c = ded0c(L, p, T, mode 2)`. `FUN_140365b02`: same with `L1 = L2 = 0` **[fact]**.

#### 5.3.1 `FUN_140364241(X, nodes, out, &npf)` (1914 B): face vector **[fact, listing]**

* npf = 3: `a = X(:,n2) − X(:,n1)`, `b = X(:,n3) − X(:,n1)`, `q = cross(a,b)`,
  `l = sqrt((q3*q3) + ((q2*q2) + (q1*q1)))`; `out = q / l` if `l > 0`, else `out = 0`.
* npf = 4: `out = 0`; for corner k = 1..4 (next = k+1, previous = k−1, cyclic):
  `a = X(:,next) − X(:,k)`, `b = X(:,prev) − X(:,k)`, `q = cross(a,b)`, `q = q/l` if `l > 0`;
  `out_i = (q_i / 4.0) + out_i` (divisor `(double)npf`). The result is not normalised and only
  the triangle path uses it.

#### 5.3.2 Triangles `FUN_1403649bb` (1189 B) **[fact, listing]**

Per face f (nodes `n1, n2, n3` = `F(1..3,f)`):
`a = X(:,n1) − X(:,n3)`, `b = X(:,n2) − X(:,n3)`, `q = cross(a,b)`,
`A = sqrt((q3*q3) + ((q2*q2) + (q1*q1))) * 0.5`;
centroid `x̄_i = ((0 + X_i(n1)) + X_i(n2)) + X_i(n3)` then `/ 3.0` (`(double)npf`);
`f = FUN_1403653fe(x̄, ȳ, z̄, FV(:,f), case)`; `S = (A * f) + S`.

#### 5.3.3 Quadrilaterals `FUN_140364e60` (1438 B) **[fact, listing]**

One Gauss point (ξ = η = `GP(1,1)` = 0, weight `W(1,1)` = 2.0; listing 0x140364f86: the loops
run for i = j = 1 only). Per face f:

```
(E, Fm, G) = FUN_140366542(ξ, η, F(:,f), X)        // first fundamental form at ζ = 1
J = sqrt((E*G) - (Fm*Fm)) * (W11*W11)              // listing 0x1403651b9: weights multiplied first
(N, dN) = FUN_1403660b9(ξ, η, ζ = 1.0)             // 8-node brick functions, top face
T = FUN_1403666cb(F(:,f), X, dN)                   // tangents t1 = ∂x/∂ξ, t2 = ∂x/∂η
x_i = Σ_{m=1..4} X_i(F(m,f)) * N_{m+4}, summed from 0.0 in m order
q = cross(t1, t2); l = sqrt((q3*q3) + ((q2*q2) + (q1*q1)))
if l > 1e-11 (DAT_140bafb30): q = q / l; f = FUN_1403653fe(x, q, case); S = (J * f) + S
```

`FUN_1403660b9`: node signs (ξk, ηk, ζk) for k = 1..8 = (−,−,−), (+,−,−), (+,+,−), (−,+,−),
(−,−,+), (+,−,+), (+,+,+), (−,+,+); `a = (ξ*ξk) + 1`, `b = (η*ηk) + 1`, `c = 1 + (ζ*ζk)`;
`N_k = ((0.125*a)*b)*c`, `∂N/∂ξ = ((ξk*0.125)*b)*c`, `∂N/∂η = ((ηk*0.125)*a)*c`,
`∂N/∂ζ = ((ζk*0.125)*a)*b`. `FUN_1403666cb`: `T(i,j) = (((0 + x1 D(j,5)) + x2 D(j,6)) + x3 D(j,7))
+ x4 D(j,8)` with `x_m = X_i(F(m,f))`, j = 1 (ξ), 2 (η). `FUN_140366542`:
`E = (t1z*t1z) + ((t1y*t1y) + (t1x*t1x))`, `Fm = (t1z*t2z) + ((t1y*t2y) + (t1x*t2x))`,
`G = (t2z*t2z) + ((t2y*t2y) + (t2x*t2x))` (it calls `FUN_1403660b9` and `FUN_1403666cb` itself).

#### 5.3.4 Integrand `FUN_1403653fe(x, y, z, &f, n, case)` (1554 B) **[fact, listing]**

`P = (0,0,0)`; for case > 4: `x' = x − cx`, `y' = y − cy`, `z' = z − cz` (current centre).
`f = (n3*P3) + ((n2*P2) + (n1*P1))` with

| case | P | integral |
|---:|---|---|
| 1 | (x, 0, 0) | volume |
| 2 | ((x*0.5)*x, 0, 0) | ∫x |
| 3 | (0, (y*0.5)*y, 0) | ∫y |
| 4 | (0, 0, (z*0.5)*z) | ∫z |
| 5 | (0, ((y'*y')*y')/3.0, ((z'*z')*z')/3.0) | ∫(y'²+z'²) |
| 6 | (((x'*x')*x')/3.0, 0, ((z'*z')*z')/3.0) | ∫(x'²+z'²) |
| 7 | (((x'*x')*x')/3.0, ((y'*y')*y')/3.0, 0) | ∫(x'²+y'²) |
| 8 | (((x'*x')*y')*(−0.5), 0, 0) | −∫x'y' |
| 9 | (((x'*x')*z')*(−0.5), 0, 0) | −∫x'z' |
| 10 | (0, ((y'*y')*z')*(−0.5), 0) | −∫y'z' |

Cases 5–10 use the centre of the current call (cases 2–4 have just updated it) **[fact]**.
Density is 1, so `m` is the volume; inward-oriented faces give a negative volume [inferred].

### 5.4 Node velocities `FUN_1403668de(X, VN, &n)` (485 B) **[fact, listing]**

`r = X(:,j) − c`; `VN1 = ((wy*r3) + vx) − (r2*wz)`, `VN2 = ((wz*r1) + vy) − (r3*wx)`,
`VN3 = ((r2*wx) + vz) − (wy*r1)`.

### 5.5 Contact detection `FUN_1403704c9` (1373 B), `detect(X, flag, tolscale)`

```
backup = X (FUN_140375025, 3·nn doubles)
if flag == 1: FUN_1403722fc(mode 1)          // insert the fixed surface (once per solve)
FUN_1403722fc(mode 2)                         // moving body: insert/update, search, decide, extract
X = backup                                    // node moves of the decision are undone
FUN_140370a26(...)                            // §5.5.4 signed gaps, separation release
nc = #{n : any BC(:,n) < 0}
(verbose > 0: 'Number of Contact points = ' nc)
```
**[fact]**

#### 5.5.1 `FUN_1403722fc(..., &mode, ..., &flag)` (4801 B): engine calls **[fact]**

Cube numbering: body 1 (moving) uses **instance 2**, body 2 (fixed) instance 1. Body type
`t = nint(RT[0])`.

* mode 1, fixed body (`t < 3`): `FUN_14004f56b(&2, &1, XU + 3(nint(RT[6])−1), FU + 4(nint(RT[8])−1),
  &4, &1, &nU, &nUf, &0, &flag')` — master surface build of GENCTC_3d_search §5.
* mode 2, moving body (`t > 1`): `flag == 1` → `FUN_14004f56b(&1, &2, X, F + 4(nint(RT[4])−1), &4,
  &IT[4], &nn, &nf, &0, &flag')`; otherwise `FUN_1400407e7(&1, &2, &1, &2, &nn, &nf, &0, &flag')`
  (polygon normals/areas only, GENCTC_3d_search §5.4). The `t == 3` assignments before it are
  overwritten (dead).
* mode 2, `nb > 1`: `DAT_140778684[0] = nb`; for obj = 1..nb, inst = 1..2: zero the cube point
  and polygon counts `DAT_14076b6a8`/`DAT_14076e588 + (obj*3+inst*300)*4`, `DAT_14076aa28
  [obj+100·inst]`, `DAT_140771788[...]`, and `DAT_140778684[obj]` (instance flags) — **after**
  the inserts above **[fact; effect on the engine: open item 3]**. For the free body with `t = 3`:
  `DAT_14076c008[1] = nn`, `DAT_14076eee8[1] = nf`; partner list `DAT_140771918[1..DAT_14077867c]`
  = bodies j with `nint(PT((j−1)·nb + 1)[0]) == 1` (body 2); for a rigid partner
  `DAT_140778684[j] = 1`, `DAT_14076ea38[j] = nUf`, `DAT_14076bb58[j] = nU` (a type-3 partner would
  get instance 2); `PT[1] == 1` would set `DAT_14076abb8[j]` / `DAT_14076ad48[j]` (not here).
  Then
  ```
  FUN_14005150d(&1, &2, X, BC, &3, &1, &nn, &nf, &1, &1, &0, &flag', &tol=0.0, &0)
  for n = IT[4]..IT[5] with flags(n) > 0:
      tol = sqrt((VN3*VN3) + ((VN2*VN2) + (VN1*VN1))) * tolscale       // VN = VN(:,n), listing 0x140372c83
      FUN_14005190e(&1, &2, X(:,IT[4]), BC(:,IT[4]), &3, &(n − IT[4] + 1), &move=1, &mode=1,
                    &dbg=0, &flag', &tol)           // array bases + node index, as in GENCTC
  if mode == 2: FUN_140373650(...)                                  // §5.5.2
  ```
  `FUN_14005150d` (1025 B) = GENCTC's pipeline `FUN_1400512aa` (GENCTC_3d_search §2.2) with one
  more stage after step 2: `FUN_140049361` runs `FUN_14004946f` (constraint rows,
  GENCTC_3d_decide §4.2) for every node. Before step 3 it opens `NO_DIEGEO.DAT` with status
  `OLD` (`DAT_140bad0e0`) once per process (`DAT_1405c76a0`: 0 untested, 1 absent, −1 present);
  **if the file exists every later search is skipped** (no contacts are ever found) **[fact;
  return value 1 = ERR= taken, as in GENCTC_3d_search open item 7]**. SRCNT gets `tol = 0.0`, so
  it uses its automatic `tol'` (GENCTC_3d_search §7.1). The decision runs with **mode 1**:
  a partner is accepted when its signed distance `dd <= 0` (GENCTC_3d_decide §4.6.3), with the
  node tolerance `|VN(:,n)|·tolscale` (`<= 1e-9f` → the node's own length `sqrt(NR2)`). With
  mode ≠ 0 a single accepted contact goes through the general tail (NI4 and NR4 saved).

The cube aliases the coordinate array given at insertion (the real `X`, GENCTC_3d_search §3.1),
and the moving body's cell structure and node normals NR1 are built only at that insertion
**[fact for the calls; consequence inferred]**: later searches see current coordinates but stale
cells and node normals of the moving body.

#### 5.5.2 Result extraction `FUN_140373650` (3074 B) **[fact]**

For n = 1..nn with flags(n) > 0: `NI4 = Get NI4(1, 2, n)` (6 ints: codes c(1..3), polygons
p(1..3)). For slot s = 3, 2, 1 with `c(s) < 0`:

```
if SW(1,s,n) < 1:                                   // always (SW is never written)
    obj = −c(s); poly = p(s)
    if poly < 1: print ' Fatal Error in polygon search in CNTINF', ' ', ' Please report this to SFTC.',
                 ' ', ' ...Object(Boundary)   = ' 1, ' ...Node number        = ' n,
                 ' ...Contact die number = ' obj, ' '      (unconditional, list-directed, unit 6 [inferred])
    else:
        inst = DAT_140778684[obj]; FID(s,n) = poly
        NRM(:,s,n) = −Get ER1(obj, inst, poly)       // minus the polygon's unit normal
        (Xp, conn, cnt) = FUN_14004f363(obj, inst, poly)
        triangle if conn(1) == |conn(4)| → FUN_140374954 else FUN_140374252 (Xp, X(:,n), &ξ, &η, &bad, &0)
        if bad < 1: XE(:,s,n) = (ξ, η)              // else the old (ξ, η) stay
else: (dead) release slots whose code equals −SW(2,s,n)
```

`FUN_140374252` (quad, 1794 B): `N = FUN_1404d3d43(Xp)` (normal of nodes 1–3);
`T = FUN_1404cf5c0(N)`; `Q0 = FUN_1404d6974(Xp, 0, 0)`; `P' = ded0c(P, Q0, T, 1)`;
`V' = ded0c(Xp(:,1..4), Q0, T, 1)`; `dmin = 1e8`; `FUN_1404cc289(V', P', near, &dmin, &edge, &4,
&bad, &1, idx, &0, &3)`; if `bad < 1` or (`bad == 1` and `dmin <= 1e-4` (`b84720`)): `bad = 0`;
`FUN_1404ccf58(V', P', ξη, &k, &err)`; `err == 0` → `(ξ, η)` = result, else
`FUN_1404d41f8(V', P', &ξ, &η, Q0, &dmin)` (not followed); `Q = FUN_1404d6974(V', ξ, η)`;
`if |Q3 − P'3| > 0.01 (b84728): bad = 1`.
`FUN_140374954` (triangle, 1745 B): the same with `Q0 = FUN_1404e34eb(Xp, 1/3, 1/3)`
(`DAT_140605680`), 3 nodes, barycentric `FUN_1404e2d44(V', P', &ξ, &η, &b3, &bad2)`, fallback
`FUN_1404d41f8` when `bad2 ≠ 0`, and `Q = FUN_1404e34eb(V', ξ, η)` (weights ξ, η, (1−ξ)−η)
**[fact]**. The sixth argument (0) skips a component-zeroing branch.

#### 5.5.3 Contact point and signed gap `FUN_140370a26` (2323 B) **[fact, listing]**

For each node n of the free body (via `RT[1..2]`: `n = int(RT[2]) .. int((RT[1] − RT[2]) + 1.0)`):
slot `s` = the **last** negative component of `BC(:,n)` (3 beats 2 beats 1); none → skip.
```
f = FID(s,n); (ξ, η) = XE(:,s,n); N = NRM(:,s,n)
triangle if FU(1,f) == FU(4,f) or FU(4,f) == 0
quad:  a = (1 − ξ)*0.25; b = (ξ + 1)*0.25
       N1 = a*(1 − η); N2 = b*(1 − η); N3 = (η + 1)*b; N4 = (η + 1)*a
tri:   N1 = ξ; N2 = η; N3 = (1 − ξ) − η
Q_i = Σ_k (N_k * XU_i(FU(k,f))) + Q_i, from 0.0 in k order
D = Q − X(:,n); |D| = sqrt(dot(D,D)); dn = dot(D, N)
if dn > 0 and |D| > 1e-5 (DAT_140bafcd0): BC(s,n) = 0            // separated: release
GAP(n) = (dn > 0) ? |D| : −|D|                                     // > 0 outside, < 0 penetrating
```

### 5.6 Initial drop `FUN_140368889` (5008 B) **[fact, listing]**

```
dmin = 1e10 (DAT_140bafc50)
for the free body rb (= 1), node range from RT, partners j with nint(PT((j−1)·nb + rb)[0]) == 1:
    if ins > 0: insert j's surface (not here, ins = 0)
    L = 1e5 (DAT_140bafc60); hits = 0
    for each node n: P = X(:,n)
        nr = Get NR1(rb, 2, n)                         // node normal of the moving body
        if (nr3*d3) + ((nr2*d2) + (nr1*d1)) >= 0:
            ray(j, inst 1, P, d, &Q, &L, &frac, &uv, &face)        // FUN_14001f54f below
            if face > 0 and frac >= 0: dmin = min(dmin, L) (strict <); hits = hits + 1
            L = dmin                                   // after every cast: the next ray is limited to the best so far
redrop = 0
if hits == 0:                                          // reverse cast from the fixed nodes
    redrop = 1
    for each node n of body j (last partner): P = XU(:,n); nr = Get NR1(j, 1, n)
        if dot(nr, −d) (same order) >= 0: ray(rb, inst 2, P, −d, ...) ; same min update, hits2++
    if hits2 == 0: return                              // redrop = 1: solver stops, no motion
if dmin > 0:
    l = sqrt((d3*d3) + ((d2*d2) + (d1*d1)));  s = dmin + (p15 * 0.5)
    Δ_i = (d_i / l) * s
    X(:,IT[4]..IT[5]) += Δ;  c += Δ;  CT += Δ;  A1, A2, A3 += Δ        (x + Δ per component)
```

`Get NR1` on a node without the field (interior mesh nodes) leaves the buffer unchanged, so the
previous node's normal is reused (GENCTC_3d_search §3.1) **[fact]**. After a successful reverse
cast `redrop` stays 1: the main routine moves the body and stops without settling **[fact]**.

`FUN_14001f54f(&obj, &inst, P, d, Q, &L, &frac, uv, &face, &0)` (2521 B, shared engine)
**[fact, listing L_contact.txt]**:

```
for k = 1..3: e_k = d_k * L; Lsq = (e_k*e_k) + Lsq; E_k = P_k + e_k; dd = (d_k*d_k) + dd   (from 0.0)
d̂ = d; if dd > 1e-10f (DAT_140bad130): r = 1.0 / sqrt(dd); d̂_k = r * d_k
len = sqrt(Lsq); pad = len * 1e-4f (DAT_140bad1f8 = 9.999999747378752e-05)
box = FUN_1404d1b1c((P, E), pad); candidates = box query + unique list (GENCTC_3d_search §4.5)
face = 0; if no candidate: return (frac, Q, uv unchanged)
frac = 1.0
for each candidate polygon p (consecutive duplicates skipped) with EI2(p) == 1:
    (Xp, conn, cnt) = FUN_14004f363(p); n = ER1(p)
    if (n3*d̂3) + ((n2*d̂2) + (n1*d̂1)) < 0:
        bad = ((n3*n3) + ((n2*n2) + (n1*n1))) < 1e-10f; din = len
        triangle (conn(1) == conn(4), conn(4) == 0 or cnt == 3): FUN_1400647a0(P, d̂, n, Xp, &din, &s, &t, Q', &hit, &1, &bad)
        else: FUN_140064f58(same)
        if hit == 1: f' = din / len; if |f'| < |frac|: frac = f'; Q = Q'; uv = (s, t); face = p
if face > 0: if frac > 1.0: face = 0;  L = L * frac
```
`FUN_1400647a0`: GENCTC_3d_search §10.2 (mode 1: only `d̂·n ≤ 0` and `|dist| ≤ |din|`).
`FUN_140064f58` (1165 B): `N = FUN_1404d3d43(Xp)`, `T = FUN_1404cf5c0(N)`,
`z4 = (ded0c(Xp(:,4), Xp(:,1), T, 1))_3`; if `|z4| >= 1e-6` (`DAT_140bad888`, non-planar) or the
plane intersection `FUN_140063ab3` fails → `FUN_1400653e5` (not followed); else apply the mode-1
tests of §10.2, then `FUN_14007ff21` (point in quad, not followed): inside (`< 1`) → hit with
Q, `din` = plane distance **[fact for this routine; children open item 4]**.

### 5.7 Stiffness `FUN_140367fe2(&k, &damp, &W, &p15)` (560 B) **[fact, listing]**

```
|v| = sqrt((vz*vz) + ((vy*vy) + (vx*vx)))
Lw = FUN_140367df1(I, w)                       // (Lw)_i = ((0 + I(i,1)w1) + I(i,2)w2) + I(i,3)w3
e = ((Lw3*wz) + ((Lw2*wy) + (Lw1*wx))) * 0.5
k = ((((e + (((m*0.5)*|v|)*|v|)) / p15) / p15) + W) * 5.0      // DAT_140bafc38
if |v| > 0: damp = ((p15*0.1f)*k) / |v|        // DAT_140bafc40 = 0.10000000149011612
elif |v| == 0: damp = sqrt(k*m) * 0.1f
```
Called once, before the loop, with `v = w = 0`: `k = (0/p15/p15 + W)*5.0`. `damp` is passed on but
never used **[fact]**.

### 5.8 Contact forces `FUN_140371339(..., F6, &W, d, &kc, ...)` (4035 B) **[fact, listing]**

```
S = 0 (6)
for each contact node n (slot s as in §5.5.3, Q, D, |D| computed the same way; |D| unused):
    N = NRM(:,s,n); u = VN(:,n); un = dot(N, u)
    ut_i = u_i − (N_i * un); l = sqrt(dot(ut, ut)); t̂ = ut; if l > 0: t̂ = ut / l
    fm = kc * 0.1 (DAT_140bafd50); if fm <= 0: fm = kc * 0.001
    Fn_i = −(N_i * fm)
    Fn_i = Fn_i − ((0.0 * fm) * t̂_i)            // friction coefficient 0.0 hard-coded, executed
    r = X(:,n) − c; Mn = cross(r, Fn)
    FN(1:6,n) = (Fn, Mn)                         // DAT_140704930, not read
    S_j = FN(j,n) + S_j  (j = 1..6)
a = |dot(S(1:3), d)|; sc = 1.0; if a > 0: sc = W / a
F6_j = (S_j * sc) + F6_j   (j = 1..6)
```
Every contact node gets the same magnitude, independent of its penetration; the scaling makes
the total contact force along `d` equal to the weight, so `kc` only matters through rounding
**[fact]**.

### 5.9 Mass matrix and gyroscopic term

In the main routine, §4.3 **[fact, listing]**.

### 5.10 Linear solve `FUN_140366ac3(A, b, &n=6, &err)` (2437 B) **[fact, listing]**

Gaussian elimination with row pivoting on column k, column-major `A(i,j)`:
```
err = 0
for k = 1..n:
    p = k; amax = |A(k,k)|; for i = k+1..n: if |A(i,k)| > amax (strict): amax = |A(i,k)|, p = i
    if amax <= 1e-10 (DAT_140bafc18): err = 1; return
    if p != k: swap A(k,c) and A(p,c) for c = k..n; swap b(k), b(p)
    for i = k+1..n:
        for c = k+1..n: A(i,c) = A(i,c) − ((A(i,k) * A(k,c)) / A(k,k))
        b(i) = b(i) − ((A(i,k) * b(k)) / A(k,k))
b(n) = b(n) / A(n,n)
for i = n−1..1: s = b(i); for c = n down to i+1: s = s − (A(i,c) * b(c)); b(i) = s / A(i,i)
```
(`A(i,k)` below the pivot is not zeroed.) The block structure makes the first three rows exact
divisions by `m`; signed zeros can arise from `0 − (0·x)/p`.

### 5.11 Allowed motions (main routine) **[fact, listing]**

```
nk ∈ {1,2}: n = K(4:6,1); q = dot(n, v); v_i = v_i − (n_i * q); q = dot(n, w); w_i = n_i * q
    nk == 2: n = K(4:6,2); q = dot(n, v); v_i = v_i − (n_i * q); w = 0
nk == 3:    a = K(4:6,3); q = dot(a, v); v_i = a_i * q; w = 0
nv > 0:     q1 = dot(v1, w); q2 = dot(v2, w); w_i = (v1_i * q1) + (v2_i * q2)
```
(with nv = 1 the second vector is 0). `K` points (`K(1:3,·)`) are only used by §5.3.

### 5.12 Equilibrium test `FUN_140369c19` (14308 B) **[fact]**

Outputs `stop` (`DAT_140704d98`) and `shift` (`DAT_140704d9c`), both 0 at entry.

```
list = contact nodes (any BC < 0), cnt = #list  (DAT_140704840)
if cnt == 0: return
if cnt < 3 and nk == 0: return
if cnt < 2 and nk == 1: return
if nk > 1: stop = 1; return                         // two planes or an axis: any contact is enough
T = FUN_1404cf5c0(d)                                // frame with d → +z
u(n) = unit(proj(T·(X(:,n) − c)))   where T·r = ded0c(r, O = 0, T, mode 1), then component 3 := 0
```

**nk = 0 (cnt ≥ 3)**: `ref = u(list1)`; `best = 2.0`; for i = 2..cnt: `q = dot(ref, u_i)`;
`if q < best: best = q, R6 = (u_i, 3 more doubles)`; `ref = R6` (the most opposite direction).
`L = R = false`, `cL = cR = 1.0`; for i = 1..cnt: `z = cross(ref, u_i)_3`, `q = dot(ref, u_i)`;
`if z > 1e-14: L = true, cL = min(cL, q)`; `if z < −1e-14: R = true, cR = min(cR, q)`
(`DAT_140bafc68/70`). (verbose: `MAX COSINE1` cL, `MAX COSINE2` cR.)
`L and R` → `stop = 1`, return; `!L and !R` → return; one side only: `L and cL > cos 179°`
(`DAT_140bafca8` = −0.9998476951563913) → return; `R and cR > cos 179°` → return; else fall
through to the moment test.

**nk = 1 (cnt ≥ 2)**: as nk = 0, but the search for the most opposite direction and the side
test run over the contact nodes **and their mirror images** in the plane `K(:,1)`
(`FUN_140365d0c(p) `= `ded0c` to the plane frame, negate component 3, back), after the direct
nodes 2..cnt (search) resp. 1..cnt (side test); same decisions **[fact]**.

**Moment test** (reached only by the fall-through):

```
gmin = min over list of GAP(n) (start 1e10, strict <, in list order)
if gmin < 0 and |gmin| > p15 * 2.0: return                 // still penetrating too deep
cnt+x, cnt−x, cnt+y, cnt−y, cnt+z, cnt−z = 0; sup = 0; Fm = 0
for each contact node n (slot s = last negative code):
    Fc = −NRM(:,s,n); r = X(:,n) − c; M = cross(r, Fc); M' = T·M (ded0c, O = 0)
    for each axis a: if |M'_a| > 1e-5: count sign (> 0 → cnt+a, < 0 → cnt−a)
    nk == 0: if dot(Fc, d) <= cos 91° (DAT_140bafcd8 = −0.01745240643728351): sup = sup + 1
             Fm_i = (Fc_i / (double)cnt) + Fm_i
    nk == 1: M6 = (M, 3 following doubles) + (K(1:3,1), 3 following doubles) (6 component adds),
             FUN_140365bfc(M6, K(1:3,1), K(4:6,1)) (frame of n, negate components 1 and 2, back),
             M6 = M6 − (same 6 doubles); M' = T·M; count the signs again (mirror moment)
nk == 0 and sup == 0:                                      // no contact supports the weight
    Δ_i = ((d_i + Fm_i) * p15) * 10.0                       // DAT_140bafcf0
    c += Δ; X(:,1..nn) += Δ; A1, A2, A3 += Δ; CT += Δ; shift = 1; return
(verbose: 'chkstop - EQM' and the six counters)
okx = !(cnt+x == 0 and cnt−x >= 1) and !(cnt+x >= 1 and cnt−x == 0); oky likewise
if okx and oky: two = (cnt+x > 0 and cnt−x > 0) + (cnt+y > 0 and cnt−y > 0); if two > 1: stop = 1
```
The z counters are computed but not used **[fact]**.

Consequence (confirmed after the oracle run, `OBJPOS/7-drop-up-3d`): nothing in §4.2 or here
compares the contact normals with `d`. A body that already touches a fixed surface is "in
contact" at the first detection (mode 1 accepts `dd <= 0`) for **any** drop direction, so no
initial drop is made, and with contact nodes on both sides of the centre the test stops at once:
dropping the UPSET workpiece (resting on die 3) along +z towards die 1 leaves it where it is,
with the success message. Lifted 3 first, the same drop moves it by (0,0,7.001)
(`OBJPOS/v-ifd-drop-up-lifted-3d`).

### 5.13 Rigid update `FUN_14036d3fd` (12492 B) **[fact, listing]**

```
old = contact nodes (list 840), cnt
if cnt == 0: initialDrop(...) (§5.6, with ins = 0); return        // lost all contacts: drop again
mode:
  cnt == 1: P1 = X(:,old1); r = c − P1; ax = unit(cross(r, d)); mode = 1
  cnt == 2: P1, P2; ax = unit(P2 − P1); r = c − P1; if dot(ax, cross(r, d)) <= 0: ax = −ax; mode = 2
  cnt >= 3: P1, P2; ax = unit(P2 − P1)
            for i = 3..cnt: e = unit(X(:,old_i) − P1); q = dot(cross(ax, e), cross(ax, e))
                            if |q| > sin²1° (DAT_140bafcf8 = 0.00030458649045213493): mode = 0; goto FREE
            r = c − P1; if dot(ax, cross(r, d)) <= 0: ax = −ax; mode = cnt
  if nk > 0 (n = K(4:6,1), also for nk = 3, see §3):
      mode == 1: q = dot(n, ax); ax_i = n_i * q             // not normalised
      mode > 1:  q = |(|dot(n, ax)|) + (−1.0)|; if q > 1e-10: mode = 0
FREE:
if nv > 0 and mode > 0: q1 = dot(v1, ax); q2 = dot(v2, ax); ax_i = (v1_i*q1) + (v2_i*q2); ax = unit(ax)
allocate Xt (3·nn); wr = w; wmax = 0
if mode > 0: q = |dot(ax, w)|; wr_i = q * ax_i
if |wr1| > 0: wmax = |wr1|;  if |wr2| > wmax: wmax = |wr2|;  if |wr3| > wmax: wmax = |wr3|
if wmax == 0: h0 = dt; w = 0; mode = 0
else (wmax > 0): h0 = (g * 0.017453292519943295) / wmax       // DAT_140bafca0: g degrees per step
(wmax NaN: h0 keeps its value from the previous call — static)
h = h0; hi = 0.0 (upper bracket); lo = 0.0 (lower bracket); ntry = 0
SEARCH:
  ntry = ntry + 1 (verbose: 'ITR' line with ntry, h, hi, lo)
  Xt = X
  mode == 0: O = (v_i * h) + c_i; a = w * h;  move(Xt, from c, to O, a, &nn, &0)
  mode > 0:  a = w * h; q = |dot(ax, a)|; a = q * ax; move(Xt, from P1, to P1, a, &nn, &0)
  detect(Xt, flag = 0, tolscale = 0.0)            // DAT_140706210 = 0: overwrites BC, FID, NRM, XE, GAP
  r = (g * h) / h0
  new = contact nodes of the trial (list 890), cntN
  if r >= 130.0 (DAT_140bafd10): ACCEPT(h, full)
  if cntN > 0:
      fresh = nodes of new not in old (list 8e0), nf
      if nf > 0:
          gm = min GAP over fresh (start 1e10, strict <)
          if gm > 0 or −p15 <= gm: ACCEPT(h, half)
          hi = h; h = (h + lo) * 0.5
          if |h_old − lo| < h0 * 0.001f (DAT_140bafd20 = 0.0010000000474974513): ACCEPT(h, half)
          goto SEARCH
  lo = h
  if hi < h: h = h + h0                                   // no bracket yet: grow
  else: h = (h + hi) * 0.5; if |hi − h_old| < h0 * 0.001f: ACCEPT(h, half)
  goto SEARCH
ACCEPT(hs, kind):
  mode == 0: s = (kind == half) ? hs * 0.5 : hs           // hs = the h of that branch (see below)
             if cntN > 5*cnt: s = h;  if r >= 50.0 (DAT_140bafd28): s = h
             O = (v_i * s) + c_i; ang = w * s; move(X, from c, to O, ang, &nn, &1)
  mode > 0:  ang = |dot(ax, w * h)| * ax;  move(X, from P1, to P1, ang, &nn, &1)
free Xt
```
Exact accept values **[fact, listing 0x14036fc90–0x1403700d5]**: "gm" branches `s = h_cur * 0.5`;
bisection-converged branches `s = h_new * 0.5` with `h_new = (h_cur + lo) * 0.5` resp.
`(h_cur + hi) * 0.5`; the 130° branch `s = h_cur`; the overrides use the current `h` variable
(`DAT_1407061c8`). For `mode > 0` the half factor is not applied (the axis rotation always uses
`h`). `cntN` in the `5·cnt` test is the trial's contact count. `wr` replaces `w` only for the step
size (`wmax`); the rotation uses `w` itself (projected on `ax` when `mode > 0`). `move` with
`&0` is used on the trial copy, with `&1` on `X` (it then also moves the tracked points).

`move = FUN_140367448(X, O_from, O_to, ang, &n, &track)` (2110 B):

```
for k = 1..3: q = ang_k / 3.1415926535898 (DAT_140bafc20, not π); ang_k = ang_k − (int(q) * 3.1415926535898)
             (|q| >= 2^52: q itself)                           // ang is reduced in place
Rx = [1 0 0; 0 c1 −s1; 0 s1 c1], Ry = [c2 0 s2; 0 1 0; −s2 0 c2], Rz = [c3 −s3 0; s3 c3 0; 0 0 1]
     (s_k, c_k) = FUN_1404b3b30(ang_k) = (SIN, COS) of the Fortran runtime (OBJPOS.md §6.3.1)
R = (Rz · Ry) · Rx with FUN_140367c86: C(i,j) = ((0.0 + A(i,1)B(1,j)) + A(i,2)B(2,j)) + A(i,3)B(3,j)
for j = 1..n: y = R·(X(:,j) − O_from) (FUN_140367df1, sums from 0.0); X(:,j) = O_to + y
if track > 0: A_k = O_to + R·(A_k − O_from) (k = 1..3); CT = O_to + R·(CT − O_from)
```
The reduction is modulo 3.1415926535898 (not 2π): an angle beyond ±π' would flip the rotation;
the step search keeps angles near `g`° **[fact; inferred that it never matters]**. The rotation
increments are stored in `cdf0..ce00` (reduced) **[fact]**.

### 5.14 Output

§4.4.

### 5.15 Small helpers **[fact]**

`FUN_140367f0b` dot product, `FUN_140366881` cross product (`a × b`, listing order as in
Notation), `FUN_140375025(a, b, &n)` copy `b = a`, `FUN_1403735bd(a, &n)` zero ints,
`FUN_140367df1(A, x, y, &n)` `y_i = ((0.0 + A(i,1)x1) + A(i,2)x2) + ...`,
`FUN_140367c86(A, B, C, &n)` matrix product (§5.13), `FUN_14001cd20/cd30/cd40` empty stubs,
`FUN_140225640` / `FUN_140225820` cube table create/free (cfnCUBE, verbose prints only).

## 6. Messages and files

| Text (Fortran list-directed unless stated) | When |
|---|---|
| ` Fatal Error in polygon search in CNTINF` + 7 lines (§5.5.2) | a contact code without polygon; **not** gated by verbose |
| `Iteration = `, ` VELOC(1:3)`, `Number of Contact points = `, `MAX COSINE1/2`, `chkstop - EQM`, `ITR` (format at `DAT_140bafd00`), `positioning finished`, `mass   : `, `center : `, `contact point` (format 200), `linear transform `, `rotational matrix`, `rotation center  ` | verbose > 0 only (never from OBJPOS) |
| `WKP2.GEO` (status `UNKNOWN`): nn, coordinates, nf, faces | verbose == 2 only |
| engine messages (GENCTC_3d_decide §5): unit 6 `Info : Node separation is detected...`, `Contact condition is changed`, ...; `NEWEQ.MSG` lines | possible for nodes that already carry a contact code (`FUN_14004ad46`) [inferred: needs an object across a boundary edge, unlikely with one target] |

`NO_DIEGEO.DAT` in the working directory disables the contact search (§5.5.1).

## 7. Status and failure paths

| Path | status | Result |
|---|---:|---|
| `nb ≠ 2`, zero direction, constraint/direction mismatch (setup) | 1 | OBJPOS_drop §5 |
| not exactly 1 free and ≥ 1 fixed body | 1 | cannot happen from OBJPOS |
| singular 6×6 (pivot ≤ 1e-10, e.g. volume ≤ 1e-10) | 1 | arrays leaked, cubes freed; `X` may already be translated by §5.6 |
| no hit in either direction | 0 | no motion, "dropped successfully" |
| reverse-cast hit | 0 | translation only |
| stable at once | 0 | translation of §5.6 only |
| `|VN(:,1)|` component > 1e100 | 0 | current state |
| iteration limit (195 bodies of the loop) | 0 | current state |

## 8. Side effects

* `X` (the object's live coordinate array) holds the final position; the wrapper does not
  transform it again (OBJPOS_drop §4 step 6) **[fact]**.
* Globals `b84720 = 1e-4`, `b84728 = 0.01`, `b84730` stay set after the solver (the only other
  readers in this module are §5.5.2) **[fact; readers elsewhere not searched]**.
* Contact-engine COMMON (`DAT_140771918`, `DAT_14077867c`, `DAT_140778684`, cube counts) keeps
  the drop's values after return; GENCTC rewrites them before use (GENCTC_3d_decide §2) [inferred].
* `NO_DIEGEO.DAT` state and other engine statics persist for the process.

## 9. Constants

| Address | Value | Use |
|---|---|---|
| `DAT_140baf9b0` | 0.001 | time step `dt` |
| `DAT_140baf9b8`, `DAT_140baf9c0` | 1e-4, 0.01 | `b84720`, `b84728` |
| `DAT_140baf9c8` | 9800.0 | gravity |
| `DAT_140baf9e0` | 1.000000013351432e-10 (float) | `|V|` test (written only) |
| `DAT_140baf918`, `DAT_140baf9f0` | 0.5, 0.5 | trapezoid, `p15/2`, bisection |
| `DAT_140bafa10` | 1e100 | divergence test |
| `DAT_140bafb30` | 1e-11 | quad normal |
| `DAT_140bafba0` | −0.5 | products of inertia |
| `DAT_140bafc08`, `DAT_140bafc10` | 0.125, 0.25 | shape functions |
| `DAT_140bafc18` | 1e-10 | pivot, axis test |
| `DAT_140bafc20` | 3.1415926535898 | angle reduction |
| `DAT_140bafc28` | 2^52 | NINT/trunc guard |
| `DAT_140bafc38`, `DAT_140bafc40` | 5.0, 0.1f | stiffness, damping |
| `DAT_140bafc50`, `DAT_140bafc60` | 1e10, 1e5 | minima start, ray length |
| `DAT_140bafc68`, `DAT_140bafc70` | ±1e-14 | side test |
| `DAT_140bafca0`, `DAT_140bafca8` | 1° in rad, cos 179° | step size, one-sided test |
| `DAT_140bafcd0`, `DAT_140bafcd8` | 1e-5, cos 91° | separation, moment, support |
| `DAT_140bafcf0` | 10.0 | EQM shift |
| `DAT_140bafcf8` | sin²1° | collinearity |
| `DAT_140bafd10`, `DAT_140bafd18`, `DAT_140bafd28` | 130, 90, 50 | step limits (90 only for nn < 1) |
| `DAT_140bafd20` | 0.001f | bisection end |
| `DAT_140bafd50` | 0.1 | contact force factor |
| `DAT_140bafe40/48` | ±1e8 | bounding box, `dmin` of 1404cc289 |
| `DAT_140605680` | 1/3 | triangle centre |
| `DAT_140bad130`, `DAT_140bad1f8`, `DAT_140bad888` | 1e-10f, 1e-4f, 1e-6 | ray query, ray–quad |

## 10. Helper table

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_14035b861` | solver main | 30040 | read fully, listing |
| `FUN_140365e0b` | Gauss table | 686 | read fully |
| `FUN_1403750cf` | characteristic length | 799 | read fully, listing |
| `FUN_140362db9` | mass properties | 5256 | read fully |
| `FUN_140364241`, `FUN_1403649bb`, `FUN_140364e60`, `FUN_1403653fe` | face vectors, integrals | 1914/1189/1438/1554 | read fully, listing |
| `FUN_1403660b9`, `FUN_140366542`, `FUN_1403666cb` | shape functions, metric, tangents | 735/393/438 | read fully, listing |
| `FUN_140365a10`, `FUN_140365b02`, `FUN_140365bfc`, `FUN_140365d0c` | plane/axis projection, mirror | 242–272 | read fully |
| `FUN_1403668de` | node velocities | 485 | read fully, listing |
| `FUN_1403704c9`, `FUN_1403722fc` | detection driver, engine calls | 1373/4801 | read fully |
| `FUN_140373650`, `FUN_140374252`, `FUN_140374954` | contact extraction, local coords | 3074/1794/1745 | read fully |
| `FUN_140370a26` | gap | 2323 | read fully, listing |
| `FUN_140368889` | initial drop | 5008 | read fully, listing |
| `FUN_14001f54f` | ray query (engine) | 2521 | read fully, listing |
| `FUN_140064f58` | ray–quad (engine) | 1165 | read; children not followed |
| `FUN_140367fe2` | stiffness | 560 | read fully, listing |
| `FUN_140371339` | contact forces | 4035 | read fully, listing |
| `FUN_140366ac3` | 6×6 solve | 2437 | read fully, listing |
| `FUN_140369c19` | equilibrium test | 14308 | read fully, listing |
| `FUN_14036d3fd`, `FUN_140367448` | step search, rigid move | 12492/2110 | read fully, listing |
| `FUN_140367f0b`, `FUN_140366881`, `FUN_140367df1`, `FUN_140367c86`, `FUN_140375025`, `FUN_1403735bd` | vector helpers | ≤ 363 | read fully, listing |
| `FUN_14005150d`, `FUN_140049361` | engine pipeline (drop variant) | 1025/270 | read fully |
| `FUN_14004f56b`, `FUN_1400407e7`, `FUN_14005190e`, cube Get/Init, `FUN_14004f363`, `FUN_1400647a0` | engine | | see GENCTC_3d_search/decide |
| `FUN_1404d41f8`, `FUN_1400653e5`, `FUN_14007ff21` | projection / ray fall-backs | | not followed |
| `FUN_1404b3b30` | sin, cos | 58 | OBJPOS.md §6.3.1 |

Followed for this file: 46 routines (36 read fully with listing, 10 read or summarised).

## 11. Open items

1. `FUN_1404d41f8` (nearest edge/corner fall-back of §5.5.2) is not specified anywhere; it
   decides `(ξ, η)` for nodes just outside a contact polygon (within 1e-4) and for degenerate
   triangles.
2. Ray–quad children `FUN_14007ff21` (point in quad) and `FUN_1400653e5` (non-planar fall-back)
   are not followed (also GENCTC_3d_search open item 2): needed for brick targets in §5.6.
3. Engine interplay of §5.5.1: the drop zeroes the cube point/polygon counts
   `DAT_14076b6a8`/`DAT_14076e588` after inserting the cubes and fills the parallel arrays
   `DAT_14076c008`/`eee8`/`ea38`/`bb58` instead; which of them SRCNT and the decision read for a
   rigid partner was not checked (owners of GENCTC_3d_search should confirm).
4. Stale engine data for the moving body (cells, NR1 node normals built once at insertion,
   §5.5.1/§5.6) is inferred from the aliasing rules, not traced through SRCNT.
5. The verbose-only output formats (`ITR` line `DAT_140bafd00`, WKP2.GEO layout) are not
   transcribed; OBJPOS always passes verbose 0.
6. Garbage `K(:,1)` for `nk = 3` (§3): harmless unless the stack holds Inf/NaN; a port should
   use zeros and note the difference.

## 12. Tests

`tests/commands/OBJPOS/drop_*.KEY` in `cases_drop.json` (predictions from this spec, UPSET_Brick:
workpiece 2 nodes x 0..12, y 0..15, z 0..15 resting on die 3, no symmetry windows → nk = 0):

| Case | Prediction |
|---|---|
| `drop_gap05_3d` | token 2 = 0.5, lifted 3: §5.6 moves by `−(3 + 0.25)` in z, all 16 bottom nodes in contact around the centre → stable at once: workpiece moved by about (0, 0, −3.25), 'Object 2 has been dropped successfully.' |
| `drop_inplace_3d` | not lifted (bottom on die 3): contacts at the first detection (mode 1 accepts d ≤ 0), stable → no motion, success message (if rounding makes d slightly positive: the initial drop moves by at most about −0.001 in z) |
| `drop_nohit_3d` | lifted 3, drop along +x between the dies: no ray hits in either direction → no motion, success message |

`ifd_m7_3d` of `cases_ifd.json` should give `−(3 + 0.001)`, not −3 (initial drop adds `p15/2`).

Oracle results (ov-ifd): all three as predicted — `drop_gap05_3d` moves by exactly (0,0,-3.25),
`drop_inplace_3d` and `drop_nohit_3d` do not move; `ifd_m7_3d`/`ifd_m7_tol0_3d`/`ifd_m8_3d`/`ifd_m9_3d`
move by (0,0,-3.001). Further observations: `ifd_m7_tilted_3d` (5° tilt) ends exactly flat again,
net translation (0, -0.0190265095, -0.001) = (0, -5(1 - cos 5°), -p15/2) [consistent with ten
rotation steps limited to `g` = 0.5°, inferred]; `ifd_m7_rigid_3d` (die 1 on the smaller workpiece)
tips over and falls past all fixed surfaces until the iteration limit, final box z -174.6..-122.1,
reported as success (§7); `ifd_m7_up_3d`: no motion (§5.12 consequence).

## 13. Note for the parent (OBJPOS_drop.md)

* §5 "`FUN_14035b861` [fact, summarised]" and open items 1–3 can be replaced by a link to this
  file; the summary sentence "iterates a rigid-body settling loop (at most `itmax` = 200
  iterations)" should read "at most 195 iterations (counter starts at 5)".
* §5 pseudo-code: the direction length is `sqrt(z² + (y² + x²))` (x²+y² first); the mode-9
  vectors are divided by their squared length (§3 here).
* §4 step 4: `K(:,1..2)` are uninitialised for `nk = 3`.
* §4 step 6: open item 2 is confirmed — the solver moves `X` itself.
* §10 tests: `ifd_m7_*` drops end `p15/2` below the contact (e.g. −3.001), and a drop with no
  target in the path succeeds without moving.
