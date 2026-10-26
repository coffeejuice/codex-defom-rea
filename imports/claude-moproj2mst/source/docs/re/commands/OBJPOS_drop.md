# OBJPOS drop positioning (3D modes 7, 8, 9)

> **Follow-up**: the settling solver `FUN_14035b861` and its kernels are specified in
> [OBJPOS_drop_solver.md](OBJPOS_drop_solver.md). Corrections from it: the direction length is
> `sqrt(z² + (y² + x²))`; the mode-9 vectors are divided by their squared length (a non-unit vector
> scales the allowed rotation by 1/L⁴); with nk = 3, `K(:,1..2)` are uninitialised stack and the
> solver still reads `K(4:6,1)`; the solver moves the object's live coordinates in place; the initial
> drop moves by gap + p15/2; a `NO_DIEGEO.DAT` in the working directory disables the contact search
> for the rest of the process.

Part of the OBJPOS spec ([OBJPOS.md](OBJPOS.md): grammar, mode table, other modes, coupled-object
loop). Interference modes: [OBJPOS_interference.md](OBJPOS_interference.md). Shared mechanics:
[_common.md](_common.md).

Source: `DEF_PRE_64.exe` v11.0.2, `case 7` of `FUN_140378e00`, switch cases 6, 7, 8 of the 3D
branch (`out/bm_cmd.c` lines 847-879), decompiles `out/p67/objpos-ifd/ifd_deep.c` and `drop.c`.
**[fact]** = read in code, **[inferred]** = deduced.

## 1. Summary

"Drop" moves a 3D object rigidly (translation **and** rotation) along a direction until it rests on
the other objects, like a part dropped into a die under gravity [inferred from the solver's
"mass", "center", "VELOC", "contact point" debug output]. The result is a 4×4 transform that is
applied to the object and then to each coupled object. There is no 2D drop: modes 7-9 in 2D fall
outside the 2D switch (cases 0-5, 10, 11, ...) and do nothing besides the shared coupled loop
**[fact: no 2D case 6/7/8]**.

| Mode | case | Difference |
|---:|---|---|
| 7 | 6 | constraints from the object's symmetry windows, or its rotational-symmetry axis |
| 8 | 7 | `*param_4 = 1`: windows ignored; one axis constraint through a point computed from the object's surface, axis = drop direction |
| 9 | 8 | as 7, plus an extra vector (tokens 6-8) passed to the solver |

## 2. Argument grammar (3D line, tokenizer as in OBJPOS.md)

| Token | Variable | Meaning |
|---:|---|---|
| 0 | `local_5708` | object to drop |
| 1 | `local_5704` | mode 7/8/9 |
| 2 | `local_5d18` → `local_5468` | solver parameter `p15` (passed by value); `<= 0` → 0.002 (`DAT_140baf920`) inside the solver [fact]; [inferred] a gap/contact tolerance in length units |
| 3-5 | `local_5d10..5d00` | drop direction (normalised by the solver; zero → solver returns without result, §4) |
| 6-8 | `local_5cf8..5ce8` | mode 9 only: vector `v` copied to `local_5460..5450`; modes 7/8: zeros |
| 9 | `local_5ce0` | coupled count, list on the next line (OBJPOS.md) |

The second 3-vector after `v` (`local_5448..5438`) is always 0 **[fact]**.

## 3. Dispatcher (cases 6, 7, 8)

```
p15 = tok2; v = (0,0,0); v2 = (0,0,0)
FUN_140240fc0(M)                     // M = 4x4 identity (row-major double[16], M[i*4+j])
f8 = (mode == 8); if mode == 9: v = (tok6, tok7, tok8)
n = 0
dropped = FUN_1401a7d60(model, A, &dir(tok3..5), &f8, &v /*6 doubles*/, p15, M, &n, NULL)
printf(dropped ? "Object %d has been dropped successfully.\n"
               : "Failure in drop-positioning object %d.\n", A+8)
```
Then the shared code after the switch (OBJPOS.md): no STROKE update for 7/8/9, `moved = 0`; in the
coupled loop each usable coupled object C gets, when `dropped`:
`FUN_1401fe240(C, M)` and `printf("Object %d has been moved by drop positioning.\n", C+8)` **[fact]**.
Return value 0 (also on failure); 3 only from the moving-object checks before the switch.

## 4. `FUN_1401a7d60(model, A, dir, &f8, v, p15, M, &n, list)` (3322 B)

1. `A == NULL` → return 0. `useSurf = (A.type == 0) ? A.geom : (A.mesh == 0)` (`geom` = byte
   `+0x3990`). Moving data **[fact]**:
   * mesh (`!useSurf`): `nn = A+0x880`, `nf = A+0xbf0`, coords `FUN_1401808d0(A+0x8a0)`,
     faces `A+0xbf8` (pointers to the live arrays, no copy);
   * surface: DIEGEO surface `S = FUN_140198620(A,&0)`: `FUN_1400034d0`, `FUN_140003500`,
     `FUN_140006410` (coords), `FUN_140005510` (faces).
2. `FUN_14016c780(A)` (**side effect**): in 3D, if A has BC windows (`A+0x2e80` list), removes every
   window whose index is not referenced by a node BC code 6 (`A+0x990` codes, `A+0x998` window
   indices) **[fact]**.
3. Fixed surface: `U = FUN_1402f9020()` (empty surface). Candidates: `n > 0 && list` ? the listed
   positions : every position 1..`model+0x1378` (here always all objects, `n = 0`). For each
   present, non-empty object `C != A` (by number) with `geom || mesh`: if `!C.geom`: build a
   temporary surface from the mesh boundary (`FUN_1401fc730(C, &1)`: 3D copies `C+0x8a8` nodes
   and `C+0xbf8` faces into a new DIEGEO surface, `FUN_1402fc1b0`); `FUN_1402fa880(U, surface)`
   (append); temporary surface removed again (`FUN_1401ffed0`) **[fact]**.
   No fixed surface at all → the solver is not called → failure.
4. Constraints `K[3][6]` (point, normal per slot) and code `nk`:
   * `f8 == 0` (modes 7, 9):
     * `w = FUN_140195b00(A+0x2e68)` = window count. `w == 0`: if `FUN_140186bc0(A+0x1ea0)`
       (3D and `A+0x2058 >= 1e-20`, `DAT_140522fa0`) and `FUN_140203fa0(A)` (some node BC code
       7 or 8): `nk = 3`, `K[2] = (A+0x2060..+0x2070, A+0x2078..+0x2088)`; else `nk = 0`.
     * `w > 0`: for each window i (list `A+0x2e80`, link `+0x38`) whose vector at `+0x20` has
       `sqrt(x²+y²+z²) >= 1e-12`: `K[m] = (win+8..+0x18, win+0x20..+0x30)`, `m++`; `nk = m`.
       `m > 3` writes past `K` (stack overflow) **[fact]**; `m == 3` is read as the axis code.
   * `f8 == 1` (mode 8): `FUN_1401fe7d0(A, c, a)` (surface of A, temporary one for a mesh object;
     `FUN_1403e8510` 8989 B computes `c`, `a` [inferred: centroid and principal axis]);
     `nk = 3`, `K[2] = (c, dir)` (the axis is the drop direction; `a` is discarded) **[fact]**.
5. Solver: `FUN_140359a70(&2, &nn, &nf, coords, faces, &nU, &nUf, Ucoords, Ufaces, dir, &nk, K,
   v, &0.5, &p15, &200, &0, t[3], R[12], &status)` (§5). Status starts at 1.
6. `status == 0` → success **[fact]**:
   ```
   T  = I with row 3 = t           (local_c8)
   Rm = I with Rm[i*4+j] = R[i*3+j] (i,j < 3)        (local_1d8)
   C- = I with row 3 = -R[9..11]   (local_148)
   C+ = I with row 3 = +R[9..11]   (local_258)
   M = I; M = C+·M; M = Rm·M; M = C-·M; M = T·M      // FUN_140241110(M, X): M := X·M
   if !useSurf (mesh): if A.geom: FUN_1401fe290(A, M) (all DIEGEO surfaces); FUN_1401fe3a0(A, M)
   else (surface):     if A.mesh: FUN_1401fe310(A, M) (mesh nodes A+0x8a8, then FUN_1401fe3a0)
   FUN_1401fd730(A, M)
   ```
   `FUN_140241110(A, B)`: `out[c][a] = Σ_{k=0..3} B[c][k]·A[k][a]`, summed from 0.0 in k order
   (`((((0 + B[c][0]A[0][a]) + B[c][1]A[1][a]) + ...)`), result copied back (`FUN_140241070`).
   Points use the row-vector convention `FUN_140241270(M, p)`: `q_j = Σ_{k=0..3} M[k][j]·h_k`,
   `h = (p, 1.0)`, summed from 0.0 in k order; `p = q[0..2]` **[fact]**. So
   `p' = ((p + t − c)·R) + c` up to rounding [inferred].
   The representation used by the solver is not transformed here (mesh object: mesh nodes; surface
   object: surface nodes): the solver receives the live coordinate arrays and is expected to move
   them itself **[inferred; open item 2]**.
   `FUN_1401fe3a0`: the extra surfaces at `A+0x3328` / `A+0x3458` when present
   (`FUN_140160e50`/`FUN_140160eb0`), `FUN_1402fccf0`. `FUN_1401fd730`: remesh `A+0x2d10`
   (`FUN_14021f730`), windows `A+0x2e68` (`FUN_14021f8d0`), movement `A+0x2160` (`FUN_14021dfa0`),
   process `A+0x1ea0` (`FUN_14021f550`), `A+0x2be8` (`FUN_1402202a0`), each item of the
   `FUN_1401983e0` list (`FUN_140281b80`), and the owning model's per-object data
   (`FUN_14019f4a0(model, M, A.number)`) **[fact; callees not followed]**.
   *(corrected after oracle run)* `FUN_14021dfa0(mv, M)` (445 B, decompile `out/p67/ov-ifd/dfa0.c`):
   when `*(int*)(*(mv+0xa80) + 0x1c) != 2` (3D [inferred]) it transforms the point `mv+0x630` with its direction `mv+0x648`
   and the point `mv+0x760` with its direction `mv+0x778` — each pair **only when the direction
   is not all |·| < 1e-12** (`FUN_14023ace0` against (0,0,0), `DAT_14051f260`): point `p := p·M`,
   direction `a := unit((a·M) − (0·M))` (`FUN_140241270` on both, `FUN_14023b1f0` subtract,
   `FUN_14023b720` normalise); otherwise `FUN_14030a170(M, mv+0x630)` **[fact]**. These pairs are the
   CNTRAX / CNTRA2 axis point and vector [inferred from the oracle]: with a zero axis vector (all
   corpus seeds used here) a drop leaves CNTRAX / CNTRA2 unchanged although the nodes move
   (`OBJPOS/7-drop-3d`: CNTRAX 2 stays at z 3 after the lift, nodes end at -0.001;
   `OBJPOS/7-drop-coupled-3d`: CNTRAX 1 stays at z 10, die nodes at 6.999), whereas the
   translations of modes 1/2 move CNTRAX unconditionally.
7. `FUN_1402f91f0(U)` (destroy); return `status == 0`.

`FUN_1401fe240(C, M)` for coupled objects = `FUN_1401fe290` + `FUN_1401fe310` + `FUN_1401fd730`,
i.e. both the surfaces and the mesh nodes **[fact]**.

## 5. Solver `FUN_140359a70` (7665 B) and `FUN_14035b861` (30040 B), Fortran

Arguments `(nb=2, nn, nf, X, F, nU, nUf, XU, FU, dir, nk, K, v, g=0.5, p15, itmax=200,
verbose=0, t, R, status)` **[fact]**.

`FUN_140359a70`:
```
status = 1
if nb != 2: return
if g <= 0: g = 0.5 (DAT_140baf918); if p15 <= 0: p15 = 0.002 (DAT_140baf920)
allocate body tables (20 ints / body; 200 doubles / body ...), body 1 = moving, body 2 = fixed
l = sqrt((d3*d3 + d2*d2) + d1*d1)            // note: z² first
if l <= 0: return (status 1)                  // zero direction → failure
d_i = d_i / l
tol = 1e-8 (DAT_140baf940)
if nk == 1 or 2:
    c = |K[0].n · d| (FUN_140367f0b(K+0x18, d, &c, &3), fabs); if c > 1e-8 → fail (status 1)
    if nk == 2: c = |K[1].n · d|;               if c > 1e-8 → fail
elif nk == 3:
    c = | |K[2].n · d| + (-1.0) |               // axis must be parallel to the direction
    if c > 1e-8 → fail
(fail prints "moving direction and symmetry plane normal vector" only when verbose > 0)
v1 = v[0..2], v2 = v[3..5]: each normalised if its length > 0 (FUN_140367f0b = dot / length
     [inferred]); nv = number of non-zero vectors (DAT_1407047e0)
FUN_14035b861(..., dir, &nk, K, &nv, v, &g, &p15, &itmax, &verbose, t, R, &status)
free
```
**[fact]** (orders: `l` as written; the dot helper `FUN_140367f0b` was not listed).

`FUN_14035b861` **[fact, summarised]**: checks that exactly one body is free and at least one is
fixed (else returns with status 1); builds the bucket grid (`FUN_140225640(&10, &verbose)`);
iterates a rigid-body settling loop (at most `itmax` = 200 iterations, `DAT_140704cc0`) with
contact detection `FUN_1403704c9`, `FUN_140368889`, `FUN_140369c19`, `FUN_140371339`,
`FUN_14036d3fd`, mass properties `FUN_140362db9`, `FUN_1403668de`, step/convergence using `g`
(`param_28`) and `p15` (`param_29`); output `t = DAT_140704ec8..ed8`,
`R[0..8]` from `DAT_140b6cca0[6..14]` (3×3), `R[9..11] = DAT_140b6cca0[3..5]` (rotation centre),
`status = 0` **[fact]**. With `verbose = 0` it prints nothing and writes no file (the
`WKP2.GEO` debug file is written only when `verbose == 2`) **[fact]**.

The settling algorithm itself (contact search, step control, stopping rule) was **not
transcribed**; a bit-exact port needs it (open item 1).

## 6. Constants

| Address | Value | Use |
|---|---|---|
| `DAT_14051f240` | 0.5 | `g` passed by the wrapper |
| `DAT_140baf918` | 0.5 | `g` default |
| `DAT_140baf920` | 0.002 | `p15` default when token 2 <= 0 |
| `DAT_140baf928`, `DAT_140baf930`, `DAT_140baf938` | 3.0, 1.0, 2.0 | body table seeds |
| `DAT_140baf940` | 1e-8 | direction/constraint test |
| `DAT_140baf950` | -1.0 | axis test |
| `DAT_14051f260` | 1e-12 | window normal test |
| `DAT_140522fa0` | 1e-20 | rotational-symmetry test |
| `DAT_140509c90` | 1.0 | identity, homogeneous 1 |

## 7. Messages

| Text | When |
|---|---|
| `Object %d has been dropped successfully.\n` | status 0 |
| `Failure in drop-positioning object %d.\n` | any failure (zero direction, no fixed surface, constraint mismatch, solver failure) |
| `Object %d has been moved by drop positioning.\n` | each usable coupled object after a success |
| `ERROR  : Object %d Undefined\n`, `ERROR  : No Mesh or Die Defined for Object %d\n` | moving object (return 3) or a coupled object (skipped) |

## 8. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140240fc0` | 4×4 identity | 166 | read fully |
| `FUN_140241110`, `FUN_140241270`, `FUN_140241070` | matrix product, point transform, copy | – | read fully |
| `FUN_1401a7d60` | drop wrapper | 3322 | read fully |
| `FUN_14016c780` | prune BC windows | 537 | read fully |
| `FUN_1402f9020`, `FUN_1402fa880`, `FUN_1402f91f0` | surface union ctor/append/dtor | – | not followed |
| `FUN_1401fc730`, `FUN_1401ffed0` | temporary surface from mesh / remove | 496, 136 | read |
| `FUN_140195b00`, `FUN_140258090`, `FUN_140186bc0`, `FUN_140203fa0` | windows, symmetry tests | – | read fully |
| `FUN_1401fe7d0`, `FUN_1403e8510` | mode 8 point | 639, 8989 | wrapper read, kernel not followed |
| `FUN_140359a70` | solver setup | 7665 | read |
| `FUN_14035b861` | settling solver | 30040 | summarised |
| `FUN_1401fe240`, `FUN_1401fe290`, `FUN_1401fe310`, `FUN_1401fe3a0`, `FUN_1401fd730` | apply M | – | read (sub-object callees not followed) |

## 9. Open items

1. `FUN_14035b861` and its kernels (`FUN_140362db9`, `FUN_1403668de`, `FUN_140365e0b`,
   `FUN_1403704c9`, `FUN_140368889`, `FUN_140369c19`, `FUN_140371339`, `FUN_14036d3fd`,
   `FUN_140366ac3`, `FUN_140367fe2`, `FUN_1403750cf`, about 60 KB) are not transcribed. Without
   them the drop result cannot be reproduced bit-exactly.
2. Whether the solver writes the moved coordinates back into the arrays it receives (the wrapper's
   choice of which representation to transform suggests so) must be confirmed in `FUN_14035b861`.
3. The meaning of `p15` (token 2) and of the mode 9 vectors inside the solver is inferred only.
4. `A+0x2058..+0x2088` (process sub-object `+0x1b8..+0x1e8`) are not in the reference specs; the
   rotational-symmetry reading is inferred.

## 10. Tests

`tests/commands/OBJPOS/ifd_m{7,8,9}_*.KEY` in `cases_ifd.json`: plain drop, default token 2,
tilted part, mode 8, mode 9 with and without a vector, coupled die, rigid mover, upward drop,
zero direction, missing object, and mode 7 in 2D (no-op). The solver is now transcribed in
OBJPOS_drop_solver.md. Oracle results (each header has an `ORACLE` line; verdicts in
`oracle/ov-ifd.md`): plain drop, default token 2, modes 8 and 9 (with and without a vector) all
move the workpiece by (0,0,-3.001) = -(gap + p15/2); the coupled die gets the same transform; the
tilted part is rotated back exactly flat (pure translation (0, -0.0190265, -0.001)); the rigid die
(centre of mass outside the workpiece top) tips over and falls until the iteration limit, still
"dropped successfully"; the upward drop of a part that touches die 3 does **not** move (contact at
the first detection, geometric equilibrium test; lifted first it moves (0,0,7.001),
`drop_v_up_lifted_3d`); zero direction → `Failure in drop-positioning object 2.`; mode 7 in 2D
prints nothing and changes nothing.
