# Shared geometry/math library for phases 6-9 (`port/defrt`)

Header: `#include <defrt/ftn/geoutl.hpp>` (namespace `def::rt::geoutl`); CRT additions in
`#include <defrt/stdlib/libm.hpp>` (namespace `def::rt::libm`). API contract in the header:
name `fun_<address>`, the binary's argument list in order (Fortran: all by pointer), return in
xmm0 -> `double`, eax -> `std::int32_t`, else `void`.

Status: `api` = name fixed, body not yet in the branch; `ported` = body in; `bit-exact` = compared
with the original routine (mapped copy of the exe) on random and spec-derived inputs, 0 mismatches,
golden table in the tier-1 test.

| Address | C++ name | Header | Role (from the specs) | Status |
|---|---|---|---|---|
| 1404cce5e | `geoutl::fun_1404cce5e` | geoutl_basic | c = a x b, normalised when norm(c) > 1e-16 (REAL*4 constant) or NaN, else c raw and len = 0 | bit-exact (1M calls) |
| 1404d68dd | `geoutl::fun_1404d68dd` | geoutl_basic | u = q - p; len = norm(u), u normalised unless norm(u)^2 == 0 exactly | bit-exact (1M calls) |
| 1404cf5c0 | `geoutl::fun_1404cf5c0` | geoutl_basic | 3x3 frame of a direction (zero vector: flag, m untouched); `defmodel::topo::direction_matrix` forwards here | bit-exact (1M calls) |
| 1404ded0c | `geoutl::fun_1404ded0c` | geoutl_basic | transform points R(a-c) (mode < 2) / R^T a + c; `topo::transform_points` forwards here | bit-exact (500k calls) |
| 1404d71d7 | `geoutl::fun_1404d71d7` | geoutl_basic | rotate a 2D point by an angle (ps_sincos); `topo::rotate_2d` forwards here | bit-exact (1M calls) |
| 1404d0939 | `geoutl::fun_1404d0939` | geoutl_basic | b(:,i) = R (a(:,i) - c), b(3,i) negated unless *flag > 0 (6 args: a, c, R, b, flag, n) | bit-exact (500k calls) |
| 1404cccfb | `geoutl::fun_1404cccfb` | geoutl_basic | unit normal of bilinear quad x(3,4) at (xi, eta): dX/dxi x dX/deta via 1404cce5e | bit-exact (1M calls) |
| 1404d3d43 | `geoutl::fun_1404d3d43` | geoutl_basic | unit normal of triangle x(3,3) via 1404cce5e; len in `g_tri_normal_len` (140757aa0) | bit-exact (1M calls) |
| 1404e33b7 | `geoutl::fun_1404e33b7` | geoutl_basic | same = 1 when \|a(k) - b(k)\| <= 1e-5 for all k (NaN counts as equal) | bit-exact (1M calls) |
| 1404edb2f | `geoutl::fun_1404edb2f` | geoutl_basic | bare RET (emptied error routine; callers pass INTEGER -1) | bit-exact (trivial) |
| 1404cc289 | `geoutl::fun_1404cc289` | geoutl_poly | point in polygon: nearest vertex/edge of a 2D polygon to p, side -1/+1/0 | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404d064d | `geoutl::fun_1404d064d` | geoutl_poly | point-segment 2D: length, signed distance, parameter, optional nearest point | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404ccf58 | `geoutl::fun_1404ccf58` | geoutl_poly | bilinear inverse map (s,t) of p in a quad: analytic, else bisection | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404cd2e1 | `geoutl::fun_1404cd2e1` | geoutl_poly | bilinear analytic solve of the two st/s/t rows (accept loop tests the last root) | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404ce05b | `geoutl::fun_1404ce05b` | geoutl_poly | bilinear bisection fallback on the mid line, 50 halvings, ps_atan2/ps_sincos | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404d6974 | `geoutl::fun_1404d6974` | geoutl_poly | bilinear interpolation N(s,t)·x(3,4) | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404d6647 | `geoutl::fun_1404d6647` | geoutl_poly | line search (step halving) of 1404d5a1d | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404d6b13 | `geoutl::fun_1404d6b13` | geoutl_poly | PNTLIN: point to 3D segment, clamped projection and distance | bit-exact (200k random calls, outputs + SAVE, NaN payloads); zero-length WRITE path by listing |
| 1404d5a1d | `geoutl::fun_1404d5a1d` | geoutl_poly | Newton projection (s,t) on a quadratic patch, 31 iterations | bit-exact (200k random calls, outputs + SAVE, NaN payloads) |
| 1404e29fa | `geoutl::fun_1404e29fa` | geoutl_tri | triangle area of X(3,3), Heron, product `(s-L3)*((s-L2)*((s-L1)*s))` | bit-exact |
| 1404e2d44 | `geoutl::fun_1404e2d44` | geoutl_tri | planar (x, y) barycentric coordinates by Heron area ratios; flag area < 1e-16 or sum >= 1.000001f | bit-exact |
| 1404e34eb | `geoutl::fun_1404e34eb` | geoutl_tri | point X(n,3) . (a, b, (1-a)-b) | bit-exact |
| 1404e2c31 | `geoutl::fun_1404e2c31` | geoutl_tri | triangle area of three points (copies, then 1404e29fa) | bit-exact |
| 1404ca2cb | `geoutl::fun_1404ca2cb` | geoutl_tri | signed tetrahedron volume (n . (x4-x1)) * area / 3 | bit-exact |
| 1404d37d7 | `geoutl::fun_1404d37d7` | geoutl_tri | 2D orientation of a 3/4-gon (right-turn count), optional vertex reversal | bit-exact |
| 1404d3094 | `geoutl::fun_1404d3094` | geoutl_tri | project a point onto a triangle: foot of the normal or nearest edge point, signed distance, edge/vertex code | bit-exact |
| 1404e2914 | `geoutl::fun_1404e2914` | geoutl_tri | 1404d3094 then barycentric (xi, eta) by 1404e305a; dist 1e8 when degenerate | bit-exact |
| 1404e305a | `geoutl::fun_1404e305a` | geoutl_tri | barycentric coordinates in the triangle frame with out-of-plane tolerance (COMMON 140b84728/30) | bit-exact |
| 1404d203b | `geoutl::fun_1404d203b` | geoutl_proj | project P on the bilinear quad X(3,4): (s,t), Q, unsigned dist (planar map, Newton, edge-triangle and triangle-split fall-backs, then the location code) | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run, own file and end to end with the library; golden rows in test_ftn_geoutl_proj) |
| 1404d275e | `geoutl::fun_1404d275e` | geoutl_proj | Newton fall-back: nearest point on the 4 (corner k, corner k+1, centroid) triangles, then (s,t) by the inverse bilinear map in a frame along Q-P; code k*10000 for a degenerate triangle | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404d41f8 | `geoutl::fun_1404d41f8` | geoutl_proj (body in src/ftn/geoutl_d41f8.cpp) | nearest edge/corner fall-back | ported (body from p67-3, in this library since 6cdff60); pntedg_golden in test_geoutl_d41f8, reached by the 8.7M-call proj sweep (paths 1, 5, 7) |
| 1404d4a0f | `geoutl::fun_1404d4a0f` | geoutl_proj | planar test (abs(z_k - z_1) <= 1e-6 in the frame of the normal at (0,0)) and planar projection: point in polygon, inverse bilinear map, else 1404d41f8 | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404ddeba | `geoutl::fun_1404ddeba` | geoutl_proj | (s,t) of a point in the quad by the inverse bilinear map in the frame at (0,0), ierr when outside/off the surface by more than the COMMON 140b84728 tolerance | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404d3e13 | `geoutl::fun_1404d3e13` | geoutl_proj | quad location code of (s,t): 0 inside, edge k, -corner | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 1.5M calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404e37c4 | `geoutl::fun_1404e37c4` | geoutl_proj | triangle location code of (s,t,1-s-t): -vertex, edge, 0 | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 1.5M calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404d1b1c | `geoutl::fun_1404d1b1c` | geoutl_proj | box of the listed points X(:,idx(1..n)) widened by a margin | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run (with 1404d1d05); golden rows in test_ftn_geoutl_proj) |
| 1404d1ed7 | `geoutl::fun_1404d1ed7` | geoutl_proj | overlap of two boxes (closed intervals, NaN overlaps) | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 1.5M calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404d1d05 | `geoutl::fun_1404d1d05` | geoutl_proj | box of the points X(:,1..n) widened by a margin | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run (with 1404d1b1c); golden rows in test_ftn_geoutl_proj) |
| 1404d93ae | `geoutl::fun_1404d93ae` | geoutl_proj | bubble sort of the columns of a(ld,n) by row key, ascending (order 1) or descending (callers: 2D arc contact refinement) | bit-exact (projdiff: 0 differences in outputs and SAVE, NaN payloads strict, 300k calls per run; golden rows in test_ftn_geoutl_proj) |
| 1404865ac | `libm::crt_atan` | libm | MSVC CRT atan (AMD LibM): 4 reduction bands up to 2.4375 + rational in r^2, abs(x)>4.86e18 -> +-pi/2, NaN quieted; errno not reproduced | bit-exact (1.2M calls) |
| 140485d6c | `libm::crt_atan2` | libm | MSVC CRT atan2(y, x) (y in xmm0): 241-entry table (14056e000/14056e790) + poly on min/max ratio, quadrant fix with split pi; tiny-pair rescale; errno not reproduced | bit-exact (1.3M calls) |
| 140483f60 | `libm::crt_pow` | libm | MSVC CRT pow(x, y): IEEE special cases + y parity for x<0, magnitude from x87 FYL2X/F2XM1/FSCALE at PC=53 (libm_crt2_x87.asm, host-CPU dependent like the original); errno not reproduced | bit-exact (1.5M calls) |

Roles are provisional where the specs only summarise the routine; the workers porting them fill
in exact roles and change the status column.
