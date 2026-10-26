# FP audit of the Phase 6-7 command specs (listing check of floating-point formulas)

Why: the Ghidra decompiler prints multi-operand floating-point expressions with the wrong grouping
(it drops the parentheses of a right-hand operand, so `a + (b + c)` is printed as `a + b + c`). This
audit checked every floating-point formula with 3 or more operands, mixed `*`/`+`, a division or a
sqrt in the specs listed below against the instruction listing. Method: the whole function was
dumped (`FnList.java`, which also prints the value of every referenced `DAT_` constant), then a
tracer rebuilt the expression tree of every SSE2 store and compare (`tr2.py`: symbolic arguments
`a1..aN`, Fortran statics `M<addr>`, stack `S<off>`). Wherever the tracer's linear walk could be
misled by branches, the raw listing was read. Files: `deform-rea-work/out/p67/fp-audit/` (`L1.txt`,
`L2.txt`, `L4.txt`, `L5.txt` listings, `tr2.py`, `disp.tr` dispatcher trace).

Results: **ok** = the spec's grouping and operand order match the listing; **corrected** = the spec
was changed (marked "(corrected by FP audit)" there); **n/a** = no formula of that kind in the spec.
`+` and `*` with two operands are commutative in IEEE arithmetic, so only grouping (and the operand
order of `-` and `/`) is reported.

## 1. Corrections

| # | Spec | Formula | Function | Spec had | Listing (now in the spec) |
|---|---|---|---|---|---|
| 1 | GENCTC.md §4.6 | 2D element check sum | `FUN_14032e34e` (`0x14032e544`, `0x14032e548`) | `((A + B) + C) * 0.125` | **`(A + (B + C)) * 0.125`** (A, B, C themselves were right) |
| 2 | OBJPOS_interference.md §5.9 | quad tangent `T1` | `FUN_1404cccfb` (`0x1404ccd70..0x1404cce06`) | `(((V3 + V2) - V1) - V4) * 0.25` | **`((V3 + (V2 - V1)) - V4) * 0.25`** (general: `(((1+η)V3 + ((1-η)V2 + (η-1)V1)) - (1+η)V4) * 0.25`) |
| 3 | OBJPOS_interference.md §5.9 | quad tangent `T2` | `FUN_1404cccfb` (`..0x1404cce2a`) | `((V4 + ((-V1) - V2)) + V3) * 0.25` | **`(V4 + (((-V1) - V2) + V3)) * 0.25`** (general: `((1-ξ)V4 + ((V1(ξ-1) - V2(ξ+1)) + V3(ξ+1))) * 0.25`) |
| 4 | OBJPOS_interference.md §5.9 | cross-product length | `FUN_1404cce5e` (`0x1404cced4`, `0x1404cced8`) | `sqrt((n3*n3 + n2*n2) + n1*n1)` | **`sqrt(n3*n3 + (n2*n2 + n1*n1))`** |
| 5 | OBJPOS_interference.md §5.12 | Heron product | `FUN_1404e29fa` (`0x1404e2b93..0x1404e2bd5`) | `(((s - L3)*(s - L2))*(s - L1))*s` | **`(s - L3) * ((s - L2) * ((s - L1) * s))`** |
| 6 | OBJPOS_interference.md §5.11 | bilinear shape function `N2` | `FUN_1404d6974` (`0x1404d69c6`, `0x1404d69f1`) | `(0.25*(ξ+1))*(1-η)` | **`0.25*((ξ+1)*(1-η))`** (bit-identical in practice: `*0.25` is exact unless subnormal) |
| 7 | OBJPOS_interference.md §7.4 | 2D boundary area (shoelace) | `FUN_14033fdb8` (`0x14033fded..0x14033fe85`) | `(((Σ x_k y_{k-1} ...)) * 0.5` (incomplete) | **`S = 0; for k = 2..n: S = ((x_k*y_{k-1}) + S) - (x_{k-1}*y_k); S = ((y_n*x_1) + S) - (x_n*y_1); return S*0.5`**, area `= -(S*0.5)` |

Same helper used elsewhere (not my files, for their owners): `FUN_1404cce5e` (correction 4) is the
cross product of `GENCTC_3d_decide.md` §4.8 / helper table (which gives `len = sqrt(s)` without the
order) and of the FTN_geoutl helpers listed in `GENCTC_3d_search.md`; `FUN_1404cccfb` and
`FUN_1404e29fa` are also in that list.

## 2. Full table

| Spec | Formula / location | Function, address | Result |
|---|---|---|---|
| TCYCLE.md §4 | `span = tLast - tFirst`; `total = (span + d2) + d3`; `dt = ((1.0 + (double)n) * T - total) / (double)k` | dispatcher `0x14037db4e..0x14037dbf8` | ok |
| SVSCAL.md §4 | `r = (x - X[i]) / (X[i+1] - X[i])`; `F = Y[i] + r * (Y[i+1] - Y[i])` | `FUN_140132a60` `0x140132bca`, `FUN_140132bf0` `0x140132c94` | ok |
| DOESCL.md §5 | `v * factor` / plain store | `FUN_140136220`, `FUN_14021d9c0`, `FUN_1401954d0` | n/a (one `MULSD`, nothing to group) |
| GENAXS.md §4 | spring `(-t2) * dir_i`; `c4 = (lo + hi) * 0.5`; `cD = 0.5*(Dhi + Dlo)`; `fabs(cD - c4x)`; `h = fabs(hi.y - lo.y)`; `len = sqrt((x*x + y*y) + z*z)`, divide; `u * (-h)`; `+0x40 = 0.5*h` | `FUN_1401af680`, `FUN_14023b610`, `FUN_14023b720`, `FUN_14023b1f0`, `FUN_14023b2d0` | ok |
| GENBCC.md §4 step 5 | `len = sqrt(dx*dx + dy*dy)`; `tol = 0.05 * hmin` | `FUN_1403087a0`, `FUN_1403087e0`, `FUN_1403089b0`, `FUN_1403046a0`, `FUN_140206d50` | ok |
| GENSUB.md | float `%f` widening, `c[4] - c[0]` (int) | `FUN_14037e5c0` | n/a |
| GENCTC.md §3 | `tol = minEdge * 0.05` (2D, 3D); 2D edge `sqrt(dx*dx + dy*dy)`; 3D box `max(xmax-xmin, ...)`; Fortran `sqrt(v) * 0.0500000007`, `<= 0 → 1e-10` | `FUN_1401a2e90`, `FUN_1401a6c80`, `FUN_1403046a0`, `FUN_140202e00`, `FUN_14003b304` `0x14003b404..0x14003b48b` | ok |
| GENCTC.md §4.3 | `lim = |tol*100|`; `moved = sqrt(ey*ey + ex*ex)`; `tol*(-100) <= d` | `FUN_140350f12` | ok |
| GENCTC.md §4.4 | nearest-segment `|a - b|`, `|b - a|` tests | `FUN_140436a9e` | ok |
| GENCTC.md §4.4 | line kernel: `L = sqrt(dy*dy + dx*dx)`, `s = uy*py + ux*px`, `h = uy*px - ux*py`, `dA`, `dB`, `P = (s*ux + Ax, uy*s + Ay)` | `FUN_1404345c0` | ok |
| GENCTC.md §4.4 | arc kernel: `R`, `rq` (orders added), end point `rot`, `h = (rq - R)·sign`, foot `(qx/rq)*R + Cx`, angle (cross/dot order added), span test `2π + α` | `FUN_14043574f`, `FUN_14043556b`, `FUN_140435625`, `FUN_140435caa` | ok (orders made explicit) |
| GENCTC.md §4.5 | meshed master | `FUN_14034fc56` | n/a (compares only; `FUN_140434952` summarised in the spec, not audited) |
| GENCTC.md §4.6 | element check sum | `FUN_14032e34e` | **corrected** (#1) |
| INICTC.md | – | `FUN_1402049b0` | n/a (integer only) |
| CPYOBJ.md | – | copies | n/a |
| WRTOBJ.md | – | KEY writer | n/a |
| OBJPOS_interference.md §4 step 7 | `|v| = sqrt((x*x + y*y) + z*z)` (was [inferred]) | `FUN_140239a30`, `FUN_1402399b0` | ok |
| OBJPOS_interference.md §4.2 / §6 | dot `((x*x') + (y*y')) + (z*z')` | `FUN_14023b3a0` (also in dispatcher auto search) | ok |
| OBJPOS_interference.md §5.1 | normalise; `dist = sqrt((r0*r0 + r1*r1) + r2*r2)` | `FUN_1401a3aa0` `0x1401a3d1e..0x1401a3d46`, `0x1401a3e11..` | ok |
| OBJPOS_interference.md §5.2 / §5.3 | `w = u / l`, `l = sqrt((x*x + y*y) + z*z)`; `out_i = w_i * d` | `FUN_1401a3ea0`, `FUN_1401a5470` | ok |
| OBJPOS_interference.md §5.4 | `shift = zmax - zmin`; `d = (fabs(tol) + d) - shift` | `FUN_1403539a1` `0x140353f7b` | ok |
| OBJPOS_interference.md §5.5 | rotation matrix R (both branches) | `FUN_1404cf5c0` | ok |
| OBJPOS_interference.md §5.6 | `y_i = ((0.0 + R*t1) + R*t2) + R*t3` | `FUN_1404d0939` | ok |
| OBJPOS_interference.md §5.8 | `e`, edge `q`, `tp`, box `p.z + dcur` | `FUN_1403541fc` | ok |
| OBJPOS_interference.md §5.9 | quad tangents `T1`, `T2` | `FUN_1404cccfb` | **corrected** (#2, #3) |
| OBJPOS_interference.md §5.9 | triangle `a`, `b` | `FUN_1404d3d43` | ok |
| OBJPOS_interference.md §5.9 | cross product `n1..n3` | `FUN_1404cce5e` | ok |
| OBJPOS_interference.md §5.9 | cross-product length | `FUN_1404cce5e` | **corrected** (#4) |
| OBJPOS_interference.md §5.10 | vertex/edge lengths `sqrt(dy*dy + dx*dx)`, `g = fabs(dmin - l0)`, reflex vectors | `FUN_1404cc289` | ok |
| OBJPOS_interference.md §5.10 | point–segment: `len`, `ê`, `t = (êy*wy) + (êx*wx)`, `sd = wx*êy - (êx*wy)`, `t/len`, foot point | `FUN_1404d064d` | ok |
| OBJPOS_interference.md §5.11 | bilinear coefficients `c, b, a, d` (x and y), point-match `ey*ey + ex*ex` | `FUN_1404ccf58` | ok |
| OBJPOS_interference.md §5.11 | `N1`, `N3`, `N4`, `P_k` | `FUN_1404d6974` | ok |
| OBJPOS_interference.md §5.11 | `N2` | `FUN_1404d6974` | **corrected** (#6) |
| OBJPOS_interference.md §5.12 | `b_i = A_i / A`, `(b1 + b2) + b3` | `FUN_1404e2d44` | ok |
| OBJPOS_interference.md §5.12 | Heron `s2`, `s` | `FUN_1404e29fa` | ok |
| OBJPOS_interference.md §5.12 | Heron product `a` | `FUN_1404e29fa` | **corrected** (#5) |
| OBJPOS_interference.md §5.12 | `w3 = (1.0 - b1) - b2`, `P_k` | `FUN_1404e34eb` | ok |
| OBJPOS_interference.md §7.1 | `dist = sqrt(r0*r0 + r1*r1)` | `FUN_1401a0c70` | ok |
| OBJPOS_interference.md §7.2 | `θ = atan(1)*2 - atan2(dy/l, dx/l)`; `ref` box product; `Atol = (ref*tol)/100`; `s0`; `dxs`; `step`; `S_tot`; `out = (0*cos + S*sin, (-0)*sin + S*cos)`; sample abscissae of both samplers | `FUN_1401a1370`, `FUN_14033ffe0`, `FUN_140340273` | ok |
| OBJPOS_interference.md §7.3 | area term `((m1 + m2)*dx)*0.5 + acc`; `f`, `w`, `s = hi - f*w`, relative test, Illinois halving | `FUN_14033fe8e`, `FUN_14033f8d4` | ok |
| OBJPOS_interference.md §7.4 | rotation `x' = cθ*x - sθ*y`, `y' = x*sθ + y*cθ`; `θ·57.29577951308232` | `FUN_14033f2bf`, `FUN_1401a28c0` | ok |
| OBJPOS_interference.md §7.4 | shoelace area | `FUN_14033fdb8` | **corrected** (#7, made exact) |
| OBJPOS_interference.md §8.2 | `tout = ta + (d/len)*(tb - ta)`; path speed→time `t + len/v`, `len = sqrt(acc + Δ²)` | `FUN_14019ffc0`, `FUN_140134de0` | ok |
| OBJPOS_interference.md §8.3 | `P_c = p_i + f*(p_{i+1} - p_i)`; `δ·δ` | `FUN_1401351c0`, `FUN_1402007d0` | ok |
| OBJPOS_interference.md §9.2 | `e = d/|d|`; `out = (b - a)*e_i` | `FUN_1401952e0`, `FUN_140239b40`, `FUN_140239ea0` | ok |
| OBJPOS_interference_kernels.md §4.3 | segment kernel: `A`, `B`, `LA`, `LB`, `e`, `q1`, `q2`, `c1`, `c2`, `a`, `b`, `c`, `Ls`, `Lp`, `d0`, `t`, `v`, quadratic `disc`, `r1`, `r2`, positions | `FUN_14030bba0` | ok |
| OBJPOS_interference_kernels.md §4.4 | arc kernel: `dP`, `Lp`, `Ld`, `R`, `u`, `dh`, `q`, `F`, `h`, `X1`, `X2`, `s`, `g`, tangent test | `FUN_1403407d7` | ok |
| OBJPOS_interference_kernels.md §4.4 | span test `|‖b‖ - ‖a‖|`, `π·2 + α`; angle `atan2` arguments; rotation | `FUN_14032bd2a`, `FUN_14032beef`, `FUN_1404d71d7` | ok |
| OBJPOS.md §5.2 | `L` (3D, 2D); `normalize`; `(p3 - p2)*u_i` | `FUN_140239a30`, `FUN_1403087e0`, `FUN_1401952e0` | ok |
| OBJPOS.md §6.1 step 4 | CNTRAX `+= d`; projection `dot(normalize(dir), d)`; `STROKE += dir_raw[j]*s` | `FUN_14021dba0`, `FUN_140194f90`, `FUN_1403091c0`, `FUN_1403090e0` | ok |
| OBJPOS.md §6.2 / §6.4 | `rad = (angle*pi)/180`; axis `sqrt((x*x + y*y) + z*z)`; the 9 entries of R (`k = 1.0 - c` each time); `M = (A*R)*C`; 4x4 product and apply with `0.0 +` accumulation | `FUN_1402fb550`, `FUN_1402214c0`, `FUN_140221cd0`, `FUN_140221a90`, `FUN_14027f460` | ok |
| OBJPOS.md §6.3, §6.5, §6.6a | already marked listing checked in the spec | – | skipped |
| OBJPOS.md §6.6b | 2D mirror at a line: `n`, `u`, `t = w.x*u.x + w.y*u.y`, `q`, `r = q - w`, `2.0*r`, `P + s` | `FUN_140309640`, `FUN_140308810`, `FUN_140308a90`, `FUN_140308940`, `FUN_140308a20`, `FUN_1403089b0` | ok |
| OBJPOS.md §6.7 | tensor rotation | – | skipped (covered by `OBJPOS_geo_kernels.md` §1, listing based) |
| OBJPOS.md §7.5 | `C = (bmin + bmax)*0.5`; `my`, `mz` `= (max + min)/2.0`; `diag`; `B + D*(val*diag)`; `G = B - Pc`; `proj` | dispatcher `0x14037b29b..0x14037b73c`, `FUN_14023b8f0`, `FUN_14023b110`, `FUN_14023bd90` | ok |
| OBJPOS.md §7.6 | centres `(min + max)*0.5`; `v = cB - cA`; `h = rmax.z - rmin.z`; `v * (-h)` | dispatcher `0x14037b9ba..0x14037bde9` | ok |
| OBJPOS.md §8.5 | `C_i = (min_i + max_i) / 2.0` | `FUN_1401ff4f0` | ok |

About 100 functions were listed and traced.

## 3. Notes

* Not audited: GENCTC.md §5 (3D, other specs), `FUN_140434952` (summarised in GENCTC §4.5), the
  bilinear solver `FUN_1404cd2e1` / `FUN_1404ce05b` and the arc refinement `FUN_140341100` /
  `FUN_14034151e` (in `OBJPOS_interference_kernels.md` §3 / §4.5, not assigned), the CRT and
  Fortran sin/cos/atan bodies.
* All corrections are in Fortran (FTN_geoutl-style, PathScale `-O2`) routines, where the compiler
  keeps partial sums in registers and the decompiler then re-associates them; the C++ (`-O0`
  style) routines all matched their decompiles.
