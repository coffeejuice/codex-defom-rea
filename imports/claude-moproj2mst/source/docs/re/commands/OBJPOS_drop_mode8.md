# OBJPOS drop, mode 8: axis point `FUN_1401fe7d0` / `FUN_1403e8510`

Packet p67-3 (worker C). Called from the drop wrapper (OBJPOS_drop.md §4 step 4, `f8 == 1`):
`FUN_1401fe7d0(A, c, a)`; the drop uses `K[2] = (c, dir)` and discards `a` (OBJPOS_drop.md).
The same wrapper is OBJPOS mode 6's own centre (OBJPOS.md 7.4, which uses only `c`).
Integration rules: OBJPOS_geo_kernels.md §2.2-2.4 (CENTER part, already specified); this file
adds the AXIS part, the moment integrands, the eigen solver and the module data.

Sources: decompile `deform-rea-work/out/p673/extra.c`, `out/p673/c/m8.c`; listings
`out/p673/c/L_m8.txt` (FnList form `F_m8.txt`, traced with `out/p67/fp-audit/tr2.py`).
Port: `port/defcmd/src/drop/mode8_{axis,massprop,linalg}.cpp`, header `defcmd/drop/mode8.hpp`,
block `defcmd/drop/globals_c.hpp`.

## 1. `FUN_1401fe7d0(obj, c, a)` → bool **[fact]**

```
made = false
if obj+0x87c != 0 and !FUN_140160d90(obj) (byte +0x3990):  FUN_1401fc730(obj, &1); made = true
elif obj+0x87c == 0 and !FUN_140160d90(obj):  return false
s = FUN_140198620(obj, &0)                      ! surface 0; null -> return false (temp stays)
nP = FUN_1400034d0(s); nF = FUN_140003500(s); X = FUN_140006410(s); F = FUN_140005510(s)
F' = new int[4*nF] (thunk 1404797d8), F'(i) = abs(F(i)) (140484ab8), freed after the call
FUN_1403e8510(&nP, &nF, X, F', c', a', &err, &0)
c = c'; a = a'; if made: FUN_1401ffed0(obj)
return err == 0
```
`+0x87c` is the mesh flag (p67-1 `has_mesh` 140160bf0) [inferred from p67-1's naming].

## 2. `FUN_1403e8510(nP, nF, X, F, c, a, err, dbg)` **[fact]**

1. `type = 3`; for every face `F(1,f) != |F(4,f)|` → `type = 4`; `*err = 1`.
2. `*dbg > 0`: list-directed debug WRITEs (`NUMSN`, `RZ`, `NUMSE`, `NBDRY`, later `Principal`,
   `vec 1..3`, `CENTER`, `AXIS`). The only caller passes 0, so no I/O is reachable.
3. ALLOCATE `nrm(3,nF)` (0x140708120), `Y(3,nP)` (0x140708070), `area(nF)` (0x1407080d0)
   (`FUN_1404b62c4`); Gauss tables `FUN_1403ee538(gp, gw)`.
4. Pass 1: `FUN_1403ea82d(X, F, &1, &nF, &type, nrm, area, &0, 0x1407082a0, dbg, gp, gw)`;
   `c = (B19, B20, B21)` (block §4).
5. `M = [[B0, B3, B6], [B3, B4, B7], [B6, B7, B8]]` (symmetric, from the upper triangle);
   `FUN_1403f0b89(M, &3, &3, d, V, &nrot)`; `V1..V3` = columns of `V` (0x1407083e0).
6. `w = V1 x V2` (`FUN_1403eef06`), `s = V3.w` (`FUN_1403f0590`, from 0.0); `0.0 > s` (strict,
   NaN no) → `V1 = -V1` (sign-bit XOR, also on zeros).
7. `Y(j,i) = ((0.0 + (X(1,i)-B19)*V(1,j)) + (X(2,i)-B20)*V(2,j)) + (X(3,i)-B21)*V(3,j)`.
8. Pass 2: `FUN_1403ea82d(Y, F, ...)` (same arguments otherwise); DEALLOCATE the three arrays.
9. `a = 0`; `d12 = |B0-B4|`, `d31 = |B8-B0|`, `d23 = |B4-B8|` (pass-2 moments);
   `d23 > d12 && d31 > d12` → `a = V3`; `d12 > d23 && d31 > d23` → `a = V1`;
   `d12 > d31 && d23 > d31` → `a = V2`; each sets `*err = 0` (strict, NaN false). Ties: `a = 0`,
   `err = 1`, and `1401fe7d0` returns false.

So `c` is the volume centroid and `a` the principal axis whose moment differs from the two
others (the symmetry axis of a body of revolution) [inferred]. Note the test uses the pass-2
moments, computed in the principal frame, where the products of inertia are near zero.

## 3. `FUN_1403ea82d` (mode 0) **[fact]**

Faces `i = 1..nF` (face `first+i-1`): `nrm(:,i) = 0`, node numbers `F(1..type)` copied,
`FUN_1403ec3cc` normal and area. Then `k = 1..10`: `I_k` by `FUN_1403ed5af` (type 4) or
`FUN_1403ed06d` (type 3), and: `B18 = I1` (V), `B19..21 = I2..4 / B18`, `B0 = I5`, `B4 = I6`,
`B8 = I7`, `B3 = I8`, `B6 = I9`, `B7 = I10`. After the loop `B5 = B7`, `B2 = B6`, `B1 = B3`,
`B9..B17 = 0.0`, `B17 = B9 = B13 = B18`. Modes 1..3 (symmetry-plane variants, `FUN_1403ee161`,
`FUN_1403ee24b` and their callees `FUN_1403f0667`, `FUN_1403f096f`) are unreachable: the only
caller passes `mode = 0` (`DAT_140708180`, set to 0 in `1403e8510`); not ported.

`FUN_1403ec3cc` type 4 (not in OBJPOS_geo_kernels.md): corners `m = 1..4`, `next = F(m+1)`
(F(1) for m = 4), `prev = F(m-1)` (F(4) for m = 1); `a = X(next)-X(cur)`, `b = X(prev)-X(cur)`,
`c0 = a1*b2 - b1*a2`, `c1 = a2*b0 - b2*a0`, `c2 = b1*a0 - a1*b0`, `L = sqrt((c0² + c1²) + c2²)`;
`L > 0` → `c = c/L`; `n = c/double(type) + n`; `area = L + area`; finally `area = area*0.25`.

## 4. Integrands `FUN_1403edb4f(x, y, z, out, n, k)` and the block **[fact]**

For `k > 4`: `x' = x - B19`, `y' = y - B20`, `z' = z - B21`. `F = 0`, then
| k | F |
|---|---|
| 1..4 | see OBJPOS_geo_kernels.md §2.2 |
| 5 | `F2 = ((y'y')y')/3.0`, `F3 = ((z'z')z')/3.0` |
| 6 | `F1 = ((x'x')x')/3.0`, `F3 = ((z'z')z')/3.0` |
| 7 | `F1 = ((x'x')x')/3.0`, `F2 = ((y'y')y')/3.0` |
| 8 | `F1 = ((x'x')y')*-0.5` |
| 9 | `F1 = ((x'x')z')*-0.5` |
| 10 | `F2 = ((y'y')z')*-0.5` |
`*out = (F1*n1 + F2*n2) + F3*n3` for every k (listing: `(n3*F3) + ((n2*F2) + (F1*n1))`);
k outside 1..10: `*out` unchanged. With the divergence theorem these are `I11 = ∫(y'²+z'²)dV`,
`I22`, `I33`, `I12 = -∫x'y' dV`, `I13`, `I23` [inferred].

Block `0x140b44d80` (22 doubles, `g_massprop`): `B0..B8` I(3,3) column-major, `B9..B17` M(3,3)
= V·identity, `B18` V, `B19..B21` centroid. Read by `1403edb4f` within the same pass; whether
other code reads it was not searched (open item).

## 5. `FUN_1403f0b89(a, n, np, d, v, nrot)`: cyclic Jacobi **[fact]**

The Numerical Recipes `jacobi` scheme: `v = I`, `b = d = diag(a)`, `z = 0`, `nrot = 0`; sweeps
1..50: `sm = Σ_{p<q} |a(p,q)|` (from 0.0, q inner) — `sm == 0` returns; `tresh =
(sm*0.2)/double(n*n)` for sweeps 1..3, else 0.0; for `p < q`: `g = |a(p,q)|*100.0`; sweep > 4
and `|d_p| == g + |d_p|` and `|d_q| == g + |d_q|` → `a(p,q) = 0`; else if `|a(p,q)| > tresh`:
`h = d_q - d_p`; `|h| == |h| + g` → `t = a/h`, else `θ = (h*0.5)/a`, `t = 1.0/(sqrt(θθ + 1.0) +
|θ|)`, `0.0 > θ` → `t = -t`; `c = 1.0/sqrt(tt + 1.0)`, `s = t*c`, `τ = s/(1.0 + c)`, `h = t*a`;
`z_p -= h`, `d_p = d_p - h`, `z_q = h + z_q`, `d_q = h + d_q`, `a(p,q) = 0`; rotations
`g' = g - ((g*τ) + h)*s`, `h' = ((g - τ*h)*s) + h` over `(a(j,p), a(j,q))` j < p,
`(a(p,j), a(j,q))` p < j < q, `(a(p,j), a(q,j))` j > q, `(v(j,p), v(j,q))` all j; `nrot++`.
End of sweep: `b = b + z`, `d = b`, `z = 0`. Work vectors b, z: SAVE arrays of 500
(0x140708fd8, 0x140709f78), set before use. Eigenvalues are not sorted.

Small routines: `FUN_1403eef06` `c = a x b` (`c1 = a2*b3 - b2*a3`, `c3 = a1*b2 - b1*a2`,
`c2 = a3*b1 - b3*a1`); `FUN_1403f0590` `r = Σ a_i*b_i` from 0.0 (`r = a_i*b_i + r`).

## 6. Statics

All SAVE scratch of these routines (0x140708070..0x14070af70) is written before it is read in
every call (checked per routine), so the port keeps locals; only the block 0x140b44d80 is a
global. 1403ed5af writes the integrand result into the slot of the metric's F (0x1407086f8),
harmless.

## 7. Verification

`deform-rea-work/scripts_p673/c/m8diff.cpp` (`mk_m8.cmd`, `callimg12.asm`): image mapped at
0x140000000; the port vs the original `1403ea82d` (mode 0, both rules, all 10 integrals, normals,
areas: boxes, triangulated and rotated boxes, prisms with mixed faces, jitter), `1403f0b89`
(60 000 symmetric matrices, n = 2..4, diagonal, near-diagonal, scaled), `1403ee538`,
`1403ee73e`, `1403edb4f` (k = 0..11), `1403ec3cc` (types 3, 4, 5, degenerate), `1403eef06`,
`1403f0590`: 296 002 compares, 0 mismatches. `1403e8510` itself allocates (Fortran ALLOCATE),
so it was not run in the image; tier-1 test `port/defcmd/tests/test_drop_mode8.cpp` has the
image golden tables and hand-checked driver cases (cube: no axis; box and prism: axis z).
`1401fe7d0` is not unit-tested (needs an object with a surface; p67-1 helpers are stubs).

## 8. Open items

1. `FUN_1401fc730` / `FUN_1401ffed0` are p67-1's (stubs in `src/drop/p671_pending.cpp`).
   p67-1's worktree has an own partial port of `1401fe7d0`/`1403e8510` (`objpos::centre_3d`,
   `objpos::mp::*`, CENTER only, its own `@data 140b44d80`); the merge should keep one copy
   (this one is complete: AXIS, k = 5..10, eigen solver).
2. Other readers of the block 0x140b44d80 not searched.
3. Debug WRITEs (`dbg > 0`) not ported (`DEF_NOT_PORTED`), unreachable.
