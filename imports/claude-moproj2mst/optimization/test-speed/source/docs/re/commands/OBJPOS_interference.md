# OBJPOS interference positioning (modes 2, 12, 102, 112)

Part of the OBJPOS spec ([OBJPOS.md](OBJPOS.md): grammar, mode table, other modes, coupled-object
loop, stroke update). Shared mechanics: [_common.md](_common.md). Drop modes 7/8/9:
[OBJPOS_drop.md](OBJPOS_drop.md).

Source: `DEF_PRE_64.exe` v11.0.2, `case 7` of `FUN_140378e00` (`out/bm_cmd.c` lines 582-1433),
headless Ghidra decompiles (`out/p67/objpos-ifd/ifd3.c`, `ifd_deep.c`) and listings
(`out/p67/objpos-ifd/L_*.txt`). **[fact]** = read in code, **[inferred]** = deduced.

## 1. Summary

Interference positioning moves object A (the "moving" object, token 0) along a direction until it
touches object B (the "target") and then pushes it further by an amount set by token 2. The
displacement is stored in the global `DAT_140706bb0..bc0` and applied to every node of A (and of
the coupled objects). The move (`FUN_14021d0b0`, OBJPOS.md) also translates the rotation-axis
points CNTRAX / CNTRA2 of each moved object, whatever their axis vector (oracle: `OBJPOS/2-depth-3d`
CNTRAX 1 z = -0.5 after +5 and -5.5; `OBJPOS/2-area-2d` CNTRAX 2 y = -2.84155) **[fact, oracle]**.

| Mode | switch case | `flag` (`local_5558`) | STROKE update | meaning [inferred from code] |
|---:|---|---|---|---|
| 2 | 1 | 1 | no | "resolve overlap first": A is backed off behind B before the distance is measured |
| 12 | 0xb | 1 | yes (`local_5684`) | as 2, then `STROKE += displacement` for A and the coupled objects |
| 102 | 0x65 | 0 | no | measure from the current position, no back-off |
| 112 | 0x6f | 0 | yes | as 102 plus STROKE update |

The flag reaches the geometric kernel: 3D `FUN_1403539a1` argument 14 (back-off shift, section
5.4), 2D `FUN_1401a1370` argument 8 (back-off shift and the "ignore negative gaps" switch, section
7). 12/112 differ from 2/102 **only** in the STROKE update after the switch **[fact]**; 102/112
differ from 2/12 **only** in `flag` **[fact]**.

3D and 2D use completely different kernels:

* 3D: rotate both objects so the direction becomes +z, then for every boundary node of the
  travelling object cast a ray along +z onto the faces of the other object (bucket search + exact
  quad/triangle hit). Distance = smallest hit + |token 2| (an interference **depth**, length units).
* 2D: rotate both boundaries so the direction becomes +y, sample the vertical gap between them on
  1001 abscissae, and solve for the shift that gives an overlap **area** equal to
  `token2 % × reference area` (secant / Illinois iteration).

## 2. Argument grammar

All tokens come from one line read by the tokenizer `FUN_140413900(f, &cnt, 10)`; token `i` is
`FUN_140413a50(i)`; missing tokens convert to 0 **[fact]** (see _common.md §3). Tokens 0, 1:
`strtol` (`FUN_14047b6c0`); the others `atof` (`FUN_14048754c`) **[fact]**.

| Token | 3D | 2D |
|---:|---|---|
| 0 | moving object number `local_5708` | same |
| 1 | mode `local_5704` | same |
| 2 | `local_5d18` = **interference depth** `tol` (absolute value is used, §5.4) | `local_5d18` = **interference area in percent** of a reference area (§7.3) |
| 3 | `local_5d10` = dx | `local_5d10` = target object number |
| 4 | `local_5d08` = dy | `local_5d08` = dx |
| 5 | `local_5d00` = dz | `local_5d00` = dy |
| 6 | `local_5cf8` = target object number, **0 = automatic search** | `local_5cf8` = number of coupled objects |
| 7, 8 | read, unused by these modes | – |
| 9 | `local_5ce0` = number of coupled objects | – |

* Target number: `abs((int)(double)token)` (`FUN_140484ab8` = abs) then `FUN_1401d6ea0` (lookup
  by object number) **[fact]**. In 2D there is **no** automatic search: target 0 is looked up as
  object 0, which is not found → the command returns 3 ("Data Error") without a message **[fact]**.
* Coupled count `n = (int)(double)token` (3D token 9, 2D token 6). If `n > 0` the **next line** is
  read with `FUN_140413900(f, &cnt, 100)` (failure → return 2); `n = min(cnt, n)`; the list
  `local_5cb8[i] = (int)atof(token i)` **[fact]**.
* Direction tokens equal to 0 within 1e-12 (all components) mean "use the object's MOVCTL
  direction" (§4.1).

## 3. Model / object fields

| Field | Use |
|---|---|
| `obj+8` number, `+0xc` type (0 rigid, 1 plastic, 2 porous, 3 elastic, 4 elasto-plastic, 5, 6, 7 empty; object.md) | filters, kernel choice |
| `obj+0x87c` has-mesh, `+0x880` nnode, `+0x884` nelem, `+0x890` element kind (2 = tet → 4 nodes/4 faces, 4 = brick → 8 nodes/6 faces) | 3D mesh boundary |
| `obj+0x8a0` node array object (`FUN_1401808d0` = coordinate pointer), `obj+0x9e0` connectivity (`FUN_140183cb0`) | 3D mesh |
| `obj+0x3990` byte: DIEGEO surface present (`FUN_140160d90`); surface via `FUN_140198620(obj,&0)`: nnode `FUN_1400034d0`, nface `FUN_140003500`, coords `FUN_140006410`, faces `FUN_1401b06e0` + `FUN_140005100` (4 ints/face) | 3D surfaces |
| `obj+0xbd8` boundary loop count, `obj+0xbe0 + 0x30·k` loop records (`+0` node count, `+8` node list) | 2D mesh boundary |
| 2D die curves: count `FUN_140198310`, curve `FUN_1401982c0(obj,&i)` (`+0x10` curve kind, `+0x14` point count) | 2D geometry |
| movement `obj+0x2160`: `+0x21b0..+0x21c0` direction, `+0x21c8` movement type (4 = mech. press, 6 = path), `+0x25a8` type-6 path table, `+0x2790` current path position, `+0x22d0..+0x22e0` STROKE | §4, §8, §9 |
| `model+0x750` sim-control: time `+0x640` / `+0x648` selected by `+0x650` (`FUN_1401f3ae0` get, `FUN_1401f3b30` set) | path branch |
| `model+0xdb0`, `+0xdb8` (GEOERR values 1, 2) | 2D kernel tolerances (§7) |
| `model+0x1378` object count, `model+0x1380` object list (`FUN_14017aa80` = n-th in list, `FUN_14017aaf0` = by number) | searches |

Note: the coupled-list lookups inside the kernels (`FUN_14019ec30`, `FUN_1401a0300`,
`FUN_14019fb50`) use `FUN_14017aa80(model+0x1380, abs(id))` = **n-th object in the list**, while
the dispatcher uses `FUN_1401d6ea0` = **by number**. They agree when objects are numbered 1..n in
list order **[fact]**.

## 4. Dispatcher flow (3D, `cVar3 == 0` branch)

1. Lookup moving object `A = FUN_1401d6ea0(model, token0)`; NULL → return 3 silently; `FUN_1401611f0(A)`
   false (prints `ERROR  : Object %d Undefined` or `ERROR  : No Mesh or Die Defined for Object %d`)
   → return 3 **[fact]**.
2. `local_5638 = (int)token6`.
   * `!= 0`: `B = FUN_1401d6ea0(model, abs(token6))`; NULL → return 3; `B+0xc == 7` →
     `printf("ERROR  : Object %d Undefined\n", B+8)`, return 3; `!FUN_140160cc0(B) && B+0x87c == 0` →
     `printf("ERROR  : No Mesh or Die Defined for Object %d\n", B+8)`, return 3 **[fact]**.
   * `== 0`: automatic search (§6). **No** validity checks on the result.
3. `flag = (mode == 2 || mode == 12)` **[fact]**.
4. `t_old = FUN_1401f3ae0(model+0x750)`; `one = 1`;
   `ok = FUN_14019fa00(model, A, B, &dir(tok3..5), &tol(tok2), &dist, &DAT_140706bb0, flag,
   &ncoupled, coupled, &timeChanged, &one)` **[fact]**.
5. Messages / state (3D):
   * `!ok`: `"Object %d to Object %d interference positioning failed.\n"` (A+8, B+8); `moved = 0`.
     If `B == NULL` (automatic search found nothing) the `B+8` read is a NULL dereference
     **[fact: code; crash inferred]**.
   * ok, `timeChanged == 0`: `"Object %d has been interference positioned w.r.t. Object %d.\n"`;
     `moved = 1`.
   * ok, `timeChanged == 1`: `t_new = FUN_1401f3ae0`; `FUN_14019ed10(model)` (§8.3);
     `"Object %d has been interference positioned w.r.t. Object %d.\n"` then
     `"       Current time has been changed to %g from %g\n"` (t_new, t_old); `moved = 0`.
6. After the switch (shared code, see OBJPOS.md): `strokeUpd = (mode ∈ {11, 12, 112}) && moved`,
   cleared when `A+0x21c8 == 4`; if set, `A.STROKE[0..2] += DAT_140706bb0[0..2]` (one `addsd` each).
   `rotFlag = (mode ∈ {2,12,102,112}) && FUN_14023ace0(tok3..5, (0,0,0), 1e-12)` (all |d_i| < 1e-12).
7. Coupled loop: for each id: `C = FUN_1401d6ea0(abs(id))`, `FUN_1401611f0(C)` (prints the two
   errors, skips); if `moved`: `d = DAT_140706bb0`; if `rotFlag`:
   `d = FUN_1401952e0(C+0x2160, 0.0, |DAT_140706bb0|)` (§9.2: the length of the displacement along
   C's own MOVCTL direction); `FUN_14021d0b0(C, &d, &1, &1)` (move); `printf("Object %d has been moved
   by (%g, %g, %g).\n", C+8, d0, d1, d2)`; if `strokeUpd`: `C.STROKE += d` **[fact]**.
   `|v|` here is `FUN_140239a30` = `sqrt((x*x + y*y) + z*z)` (`FUN_1402399b0` sum of squares) **[fact]** [listing-checked].

The 2D branch (§7.1) is the same with tokens shifted and these differences **[fact]**:
target from token 3, no automatic search, target geometry test `FUN_140203980` instead of
`FUN_140160cc0`; success message without time change is
`"Object %d is interference positioned w.r.t. Object %d.\n"`; the 2D stroke flag is set for modes
11/12/112 **without** testing `moved` (so a failed or path-type 12/112 adds the stale
`DAT_140706bb0` to STROKE x, y); coupled message `"Object %d(coupled) has been moved by (%g, %g).\n"`;
`rotFlag` uses `FUN_140308d60` (2 components, tokens 4, 5).

The dispatcher returns `local_56d8._4_4_` (0) → status 0 even when positioning failed **[fact]**.

### 4.1 `FUN_14019fa00` (320 B): router

`*timeChanged = 0`; if `FUN_14019ec30(model, A, &n, list)` → `*timeChanged = 1`,
`return FUN_14019fb50(...)` (path branch, §8); else `return FUN_1401a0300(...)` **[fact]**
(the decompiler shows `void`; the listing tail-returns the callee's AL).

`FUN_14019ec30`: true iff `FUN_14021e470(A+0x2160)` and every coupled object found (by list
position) and not empty also satisfies `FUN_14021e470` **[fact]**.
`FUN_14021e470(mv)`: `mv+0x68 == 6` (movement type 6) **and** `!FUN_14012fd80(mv+0x448)`
(table T@25a8 not all zero: ndim 0 → constant ≠ 0; else any abscissa or value ≠ 0) **[fact]**.

### 4.2 `FUN_1401a0300` (1793 B): direction choice and coupled minimum

```
zeroDir = 2D ? (|dx|<1e-12 && |dy|<1e-12) [then the 3-comp test too] : all 3 |d_i| < 1e-12
if !zeroDir: D = dir (3D: 3 comps, 2D: x, y, D.z = 0)
else: if !FUN_14018cd10(A+0x2160) return 0        // MOVCTL direction is all exact 0.0
      D = A+0x21b0.. (2D: x, y)
if ncoupled == 0:                                  // (list pointer is never NULL)
    return FUN_1401a0a10(model, A, B, &D, tol, dist, disp, flag, &one)
            // 3D FUN_1401a3a00 / 2D FUN_1401a0bd0: kernel, then on success
            // FUN_14019eb60 → FUN_14021d0b0(A, disp, &one, &1) moves A
dm = 0; dispm = 0
if !FUN_1401a0b00(model, A, B, &D, tol, &dm, dispm, flag) return 0
if dot(D, dispm) < 0: dm = -dm                     // FUN_14023b3a0: (x*x'+y*y')+z*z'
for each coupled id > 0 (ids <= 0 skipped here):
    C = n-th object; skip NULL / empty
    DC = (!zeroDir || !FUN_14018cd10(C+0x2160)) ? D : C+0x21b0..
    if FUN_1401a0b00(model, C, B, &DC, tol, &dc, dispc, flag):
        if dot(DC, dispc) < 0: dc = -dc
        if dc < dm:
            dm = dc
            if !zeroDir || !FUN_14018cd10(A+0x2160): dispm = dispc
            else: dispm = FUN_1401952e0(A+0x2160, 0.0, fabs(dm))   // along A's MOVCTL dir
*dist = fabs(dm); disp = dispm
FUN_14021d0b0(A, disp, &one, &1)                   // move A
return 1
```
**[fact]**. `FUN_1401a0b00` dispatches to `FUN_1401a3aa0` (3D, §5) or `FUN_1401a0c70` (2D, §7) and
does **not** move anything. Dot products `FUN_14023b3a0` = `((x*x') + (y*y')) + (z*z')` [listing-checked].

## 5. 3D kernel

### 5.1 `FUN_1401a3aa0(model, A, B, D, &tol, &dist, disp, flag)` (1002 B)

```
if A==0 || B==0: return 0
u = D; FUN_14023b720(u)        // L = (x*x+y*y)+z*z; L==0 ? 0 : sqrt(L); if L!=0: u_i = u_i / L
if (B.type==0 && !B.geom) || (A.type==0 && !A.geom): return 0       // geom = byte +0x3990
if B.type!=0 && !B.mesh && !B.geom: return 0 ; same for A
if B.type==0 && B.geom && !B.mesh && A.type==0 && A.geom && !A.mesh:
     FUN_1401a5470(model, B, A, u, tol, r, &err, flag)     // rigid surface vs rigid surface
else FUN_1401a3ea0(model, B, A, u, tol, r, &err, flag)
if err == 0: dist = sqrt((r0*r0 + r1*r1) + r2*r2); disp = r; return 1
u = u * -1.0  (FUN_14023b2d0, DAT_140522fc8)               // retry backwards
FUN_1401a3ea0(model, B, A, u, tol, r, &err, flag)          // always a3ea0 on retry
if err == 0: r = r * -1.0; dist = sqrt(...); disp = r; return 1
dist = 0.0; return 0
```
**[fact]**. `sqrt` is the CRT `FUN_140484644` (sqrtsd semantics). Note the early `return 0` paths do
**not** write `dist`; the retry failure writes `dist = 0.0` (matters for §6). [listing-checked]

### 5.2 `FUN_1401a3ea0(model, B, A, u, tol, out, &err, flag)` (5569 B)

Inside, "S" = B (target), "T" = A (moving) **[fact]**.

1. `u` is normalised **again**: `l = sqrt((x*x + y*y) + z*z)`, `w_i = u_i / l` (no zero check).
2. `R = FUN_1404cf5c0(w)` (§5.5) (R also starts as identity, `DAT_140509c90` = 1.0).
3. For each of S and T, source of geometry:
   * **surface** if `(type==0 && geom) || (type!=0 && geom && !mesh)`: nodes/faces of the DIEGEO
     surface; face k = 4 ints `f[4k..4k+3]`, the 4th passed through `abs` **[fact]**;
   * else **mesh**: coords `obj+0x8a0` (nnode·3), connectivity `obj+0x9e0`
     (nelem·{4 | 8}); boundary faces from `FUN_140354ed0` (FL3 `FNDBND`, §5.7).
     `obj+0x890` other than 2/4 leaves the per-element counts uninitialised **[fact]**.
4. Rotation (origin (0,0,0), `FUN_1404d0939`, §5.6):
   * T deformable (`T.type != 0`): rotate S and T by `R(w)`. Travelling = T, fixed = S.
   * T rigid (`T.type == 0`): rotate both by `R(-w)` (exact sign flips of `w`). Travelling = **S**,
     fixed = T (the target's nodes are cast backwards onto the moving rigid object).
5. `FUN_1403539a1(fixedRot, travRot, &tol, fixedFaces, travFaces, &nFixF, &nTravF, &nFixN,
   &nTravN, work[nTravN], &d, &(1 or -1), &err, &flag)` (§5.4). The `±1` argument is unused.
6. `err = (err_k == 1)`; `out_i = w_i * d` (i = 0..2) **[fact]** [listing-checked].

### 5.3 `FUN_1401a5470` (6148 B): both objects are rigid surfaces

Same loading as 5.2 with `w` normalised the same way. Four rotations: S, T by `R(w)` and S, T by
`R(-w)`. Pass 1: `FUN_1403539a1(S_w, T_w, ...)` (T nodes onto S faces) → `d1`, `e1`. Pass 2:
`FUN_1403539a1(T_-w, S_-w, ...)` (S nodes onto T faces) → `d2`, `e2`. Result
`d = (!e1 && !e2) ? (fabs(d2) < fabs(d1) ? d2 : d1) : (!e1 ? d1 : (!e2 ? d2 : err=1))`;
`out_i = w_i * d` **[fact]** [listing-checked].

### 5.4 `FUN_1403539a1(P1 fixed, P2 trav, &tol, F1, F2, &nF1, &nF2, &nN1, &nN2, work, &d, -, &err, &flag)` (2139 B, Fortran [inferred posdie2.f/position.f family])

Arrays are Fortran: `P(3, n)` column-major, faces `F(4, n)` 1-based node ids **[fact]**.
```
FUN_140225640(&1, &0)                 // cfnCUBE_CreateCubes_Obj(1 object, verbose 0)
FUN_140225ff0(&1,&1,&1, &nN1, P1, &nF1, &4, F1, &1, &0)   // bucket grid of the fixed faces
zmax = -1e5 (DAT_140baf898); for k=1..nN2: if zmax <= P2(3,k): zmax = P2(3,k); work(k)=0
zmin =  1e5 (DAT_140baf8a0); for k=1..nN1: if P1(3,k) <= zmin: zmin = P1(3,k)
shift = 0.0
if flag == 1 && zmin < zmax: shift = zmax - zmin; for k: P2(3,k) = P2(3,k) - shift
d = 1e5
for f = 1..nF2: for j = 1..4:
    n = abs(F2(j,f)); if work(n) >= 1: continue
    FUN_1403541fc(P2(:,n), P1, F1, -, (0,0,1) DAT_140604eb0, -, &dn, &d)     // §5.8
    if dn < d: d = dn
    work(n) = 1
if d == 1e5: err = 1
d = ((fabs(tol) + d) - shift)          // andpd with DAT_140baf8a8, then addsd, subsd
FUN_140225820(&0)                     // cfnCUBE_CleanCubes_AllObj
```
**[fact]** [listing-checked]. Consequences: a hit at exactly 1e5 or beyond counts as "no hit"; only boundary-face
nodes of the travelling object are cast (all nodes enter zmax); the fixed object's nodes are never
cast in 5.2 (one-sided test). With `flag = 1` (modes 2/12) the travelling body is first moved
`shift` back so that its highest point is level with the fixed body's lowest point; the shift is
subtracted again at the end, so `d` is the real signed travel **[fact]**.

Consequence of the back-off [inferred from the code; confirmed by the oracle: `OBJPOS/2-auto-behind-3d`
moves the workpiece by (0,0,-56) w.r.t. Object 3, `OBJPOS/2-coupled-movctldir-3d` moves die 1 by
(0,0,-55), and the 2D analogue `OBJPOS/v-ifd-area1-coupled-2d` picks die 3's (0, 81.9779)]: with flag 1 a target lying anywhere along the line (also entirely
*behind* the mover) is approached from its far side: the travelling body is put below the
target's lowest point and cast forward, so `d` becomes large and negative (the mover jumps
through to the other side). Without the flag (102/112) such a target gives no forward hit and
the reversed retry of §5.1 is used instead.

### 5.5 `FUN_1404cf5c0(a, R)` (538 B): rotation that maps `a` to +z (listing checked)

```
flag DAT_140730310 = 0
if a1==0 && a2==0 && a3==0: DAT_140730310 = 1; return       // R untouched
r = sqrt(a1*a1 + a3*a3)           // XMM8 = a1*a1; XMM8 += a3*a3
L = sqrt((a2*a2 + a1*a1) + a3*a3)
s1 = r / L ; s2 = a2 / L
if r < 1e-9 (DAT_140643930) [or NaN]:
    R = [1.0, 0, 0,  0, s1, s2,  0, -s2, s1]         // R[0..8], column-major R(i,j)=R[(i-1)+3(j-1)]
else:
    ca = a1 / r ; sc = a3 / r
    R = [sc, -(s2*ca), s1*ca,  0, s1, s2,  -ca, -(sc*s2), sc*s1]
```
Negation is a sign-bit xor (`DAT_140bb2688`) **[fact]** [listing-checked]. Then `R·a = (0, 0, |a|)` [inferred].

### 5.6 `FUN_1404d0939(X, O, R, Y, &k, &n)` (793 B)

For each node j: `t_i = X(i,j) - O_i`; `y_i = ((0.0 + R(i,1)*t1) + R(i,2)*t2) + R(i,3)*t3`;
`Y(:,j) = y`; `Y(3,j) = Y(3,j) * (k > 0 ? 1.0 : -1.0)` (always `k = 1` here) **[fact]** [listing-checked]. The leading
`0.0 +` turns a `-0.0` product into `+0.0`.

### 5.7 `FUN_140354ed0` (3567 B, FL3 `FNDBND`): boundary faces of a 3D mesh

Arguments `(&nelem, conn, &nodesPerElem, &timing(0), &nFaceOut, faces, work)`. Nodes per element
4 → 4 faces of 3 nodes, 8 → 6 faces of 4, 10 → 4 faces of 6 **[fact]**. All element faces are
generated (`FUN_140355cbf` tet / `FUN_14035672f` hex, face tables in data), keyed by the sum of
their node ids, sorted/compared pairwise; faces occurring once are boundary faces, 4 ints each
(3-node faces padded with `F(4) = F(1)`, not 0) **[fact, summarised]**. Full transcription, face-node tables and
orientation (which faces pass the `e > 0` test in §5.8): [OBJPOS_interference_kernels.md §1](OBJPOS_interference_kernels.md#1-boundary-faces-of-a-3d-mesh-fun_140354ed0-fl3-fndbnd).

### 5.8 `FUN_1403541fc(p, P1, F1, -, z=(0,0,1), -, &dn, &dcur)` (3269 B): ray from node p along +z

```
box = (p.x, p.y, p.z,  p.x, p.y, p.z + dcur)            // dcur = running minimum (1e5 at first)
FUN_140228c60(&1,&1, box, &nc, &0, &0)                 // bucket query (OBJPOS_interference_kernels.md §2)
FUN_140229f30(&1,&1, list, &0)                          // candidate face ids, qsort'ed
dn = 1e5
for each candidate face f:
    for s = 1..4: V(:,s) = P1(:, abs(F1(5-s, f))) - p   // vertices in REVERSE order (4,3,2,1)
    nv = (abs(F1(1,f)) == abs(F1(4,f))) ? 3 : 4
    cx = #{s<=nv : V(1,s) < 0}, cy = #{s<=nv : V(2,s) < 0}     // strict (<=0 and !=0)
    if cx==0 || cx==nv: skip ; if cy==0 || cy==nv: skip       // ray outside the xy box
    N = (nv==4) ? quadNormal(V) : triNormal(V)                // §5.9
    e = ((0.0 + z1*N1) + z2*N2) + z3*N3                       // = N3 here
    if e <= 0 || fabs(e) < 1e-8 (DAT_140604ed8): skip
    m = 0; for s=1..nv: t = s+1 (nv+1→1);
        q = (V(2,s)-V(2,t))*(V(2,s)-V(2,t)) + (V(1,s)-V(1,t))*(V(1,s)-V(1,t))
        if q < 1e-15 (DAT_140604ee0): m++
    if m > 1: skip                                            // degenerate face in projection
    FUN_1404cc289(V, (0,0), nearest, &dmin=1e8, &edge, &nv, &inout, &1, -, &0, &3)   // §5.10
    if inout >= 1: skip                                       // outside
    tp = (((0.0 + V(1,1)*N1) + V(2,1)*N2) + V(3,1)*N3) / e    // plane distance
    if nv == 4:
        FUN_1404ccf58(V, (0,0), xi_eta, &k, &err)              // §5.11 inverse bilinear
        FUN_1404d6974(V, P, xi, eta)                           // P = Σ N_s V(:,s)
        h = P3
    else:
        FUN_1404e2d44(V, (0,0,0), &b1, &b2, &b3, &bad)        // §5.12 (bad is ignored)
        FUN_1404e34eb(V, P, &b1, &b2, &3)                      // weights (b1, b2, (1-b1)-b2)
        h = (fabs(b1) > 1.001 || fabs(b2) > 1.001 || fabs(b3) > 1.001) ? tp : P3
                       // DAT_140baf8c0 = 1.0010000467300415
    if h < dn: dn = h
free list
```
**[fact]** (dot-product and edge-length orders checked in `L_1403541fc.txt`) [listing-checked]. There is no `h >= 0`
test: a face behind the node that the bucket query returns gives a negative `dn`.

### 5.9 Normals

* `FUN_1404cccfb(V, &xi=0, &eta=0, N)` quad: for k = 1..3 (corrected by FP audit, listing
  `0x1404ccd70..0x1404cce2a`):
  `T1_k = (((1+eta)*V(k,3) + ((1-eta)*V(k,2) + (eta-1)*V(k,1))) - (1+eta)*V(k,4)) * 0.25`,
  `T2_k = ((1-xi)*V(k,4) + ((V(k,1)*(xi-1) - V(k,2)*(xi+1)) + V(k,3)*(xi+1))) * 0.25`;
  with `xi = eta = 0` the factors are exact (`eta-1` is `eta + (-1.0)`), so
  `T1_k = ((V(k,3) + (V(k,2) - V(k,1))) - V(k,4)) * 0.25`,
  `T2_k = (V(k,4) + (((-V(k,1)) - V(k,2)) + V(k,3))) * 0.25`;
  then `FUN_1404cce5e(T1, T2, N, &len)` **[fact]** [listing-checked].
* `FUN_1404d3d43(V, N)` triangle: `a = V(:,2) - V(:,1)`, `b = V(:,3) - V(:,1)`, `FUN_1404cce5e(a, b, N)` [listing-checked].
* `FUN_1404cce5e(a, b, n, &len)`: `n1 = a2*b3 - b2*a3`, `n2 = a3*b1 - b3*a1`, `n3 = a1*b2 - b1*a2`;
  `len = sqrt(n3*n3 + (n2*n2 + n1*n1))` (corrected by FP audit: `n2*n2 + n1*n1` is added first,
  listing `0x1404cced4`); if `len > DAT_140bb2668` (1.0000000168623835e-16) or NaN:
  `inv = 1.0 / len; n_i = n_i * inv` (multiply by the reciprocal); else `len = 0` and `n` is left
  unnormalised **[fact]**.

### 5.10 `FUN_1404cc289` (2674 B): point (0,0) against the projected polygon

Arguments `(X stride 3, P, Q out(2), &dmin in/out, &edge out, &nv, &inout out, &closed=1, idx, &useIdx=0, &stride=3)`.
```
inout = 1; edge = 0
i0 = 1; l0 = sqrt(dy*dy + dx*dx) of (X(:,1) - P)         // dx = x - Px, dy = y - Py
for v = 2..nv: l = sqrt(dy*dy + dx*dx); if l < l0: i0 = v, l0 = l     // nearest vertex
ne = closed ? nv : nv-1
for i = 1..ne:
    j = i+1 (→1 past nv); skip while sqrt(dy*dy+dx*dx) of X(:,i)-X(:,j) < DAT_140bb2640 (1.0000000133514320e-10)
    FUN_1404d064d(P, X(:,i), X(:,j), &len, &sd, &t, &2)      // signed distance to segment
    if fabs(sd) <= fabs(dmin): Q = P; edge = i; dmin = fabs(sd); (t, sd saved)
if edge != 0:
    g = fabs(dmin - l0); if g < 1e-10: edge = i0
    if dmin == 0.0: inout = 0; edge = abs(edge); return       // on the boundary
    if fabs(sd_saved) < l0 && g > 1e-10: inout = (sd_saved < 0) ? -1 : 1; return
    // nearest feature is the vertex v = i0: test both adjacent edges
    FUN_1404d064d(P, X(:,v-1), X(:,v), ..., &s1, ..., &1); FUN_1404d064d(P, X(:,v), X(:,v+1), ..., &s2, ..., &1)
    inout = 1; if s1 < 0 && s2 < 0: inout = -1
    if (s1 <= 0 || s2 <= 0) && inout != -1:
        a = X(:,v+1) - X(:,v) (2D, z = 0), b = X(:,v-1) - X(:,v); n = FUN_1404cce5e(a, b)
        if n3 <= 0: inout = -1                                   // reflex vertex
```
**[fact, condensed from the decompile; the exact branch/assignment order is in `ifd_deep.c`]**.
`FUN_1404d064d(P, A, B, &len, &sd, &t, &mode)`: `e = B - A`, `len = sqrt(ey*ey + ex*ex)`,
`ê = e / len` (two divisions), `w = P - A`, `t = (êy*wy) + (êx*wx)`, `sd = wx*êy - (êx*wy)`,
`*t_out = t / len`; if `t < 0 || t > len`: distance to the nearer end point, sign of `sd`
(`l_B = sqrt(dy*dy+dx*dx)` of `P - B` vs `l_A` of `w`; `|l_A| <= |l_B|` → A), and with mode != 1
P is moved to that end point; else `sd` and (mode != 1) `P = A + t·ê` (`P.y = êy*t + A.y`,
`P.x = t*êx + A.x`) **[fact]** [listing-checked] (also the vertex distances `sqrt(dy*dy + dx*dx)`, `g = fabs(dmin - l0)` and the reflex-vertex vectors of `FUN_1404cc289`). The loop (mode 2) passes a copy of the query point
(`DAT_14072fe00`, reset to (0,0) before every edge), so the caller's point is not changed; the
projected point of the best edge is what `Q` receives **[fact]**. Only `inout` is used by §5.8.

### 5.11 Quad: `FUN_1404ccf58`, `FUN_1404cd2e1`, `FUN_1404ce05b`, `FUN_1404d6974`

`FUN_1404ccf58(V, P=(0,0), xi_eta, &k, &err)` sets up the bilinear map
`x(ξ,η) = a0 + a1 ξ + a2 η + a3 ξη` per axis (constants `DAT_140bb2660 = 0.25`,
`DAT_140bb2670 = -0.25`):
```
err = 0
c_x = ((V(1,3) + (V(1,1) - V(1,2))) - V(1,4)) * 0.25          // ξη coefficient   DAT_14072ff70
b_x = (((V(1,1) - V(1,2)) - V(1,3)) + V(1,4)) * -0.25         // ξ  coefficient   DAT_14072ff78
a_x = (((V(1,2) + V(1,1)) - V(1,3)) - V(1,4)) * -0.25         // η  coefficient   DAT_14072ff80
d_x = ((((V(1,2) + V(1,1)) + V(1,3)) + V(1,4)) * 0.25) - P1   // constant − P     DAT_14072ff88
c_y = 0.25 * (((V(2,1) - V(2,2)) + V(2,3)) - V(2,4))          // DAT_14072ff90
b_y = -0.25 * (((V(2,1) - V(2,2)) - V(2,3)) + V(2,4))         // DAT_14072ff98
a_y = (((V(2,2) + V(2,1)) - V(2,3)) - V(2,4)) * -0.25         // DAT_14072ffa0
d_y = ((((V(2,2) + V(2,1)) + V(2,3)) + V(2,4)) * 0.25) - P2   // DAT_14072ffa8
twist = fabs(c_x) > 1e-5 && fabs(c_y) > 1e-5                  // DAT_140bb2678
FUN_1404cd2e1(&c_x,&b_x,&a_x,&d_x, &c_y,&b_y,&a_y,&d_y, xi_eta, &k, &err)   // analytic solve
if err != 1:
    if twist: return
    Q = FUN_1404d6974(V, ·, ξ, η); ex = P1 - Q1; ey = P2 - Q2
    if (ey*ey + ex*ex) < 1e-6 (DAT_140bb2680) && k == 1: return
k = 1; FUN_1404ce05b(V, P, xi_eta, &err)                            // iterative fallback
```
[listing-checked] (all eight coefficients and the `ey*ey + ex*ex` test).
(node k of the face sits at (ξ,η) = (−1,−1), (1,−1), (1,1), (−1,1) for k = 1..4 [inferred from
the shape functions of `FUN_1404d6974`]).
`FUN_1404cd2e1` (3450 B) normalises each equation by its `ξη` coefficient when
`|coef| > 1e-9` (`DAT_1406438d8`), eliminates, solves the resulting quadratic
`b² − 4ac` (`DAT_140bb2690 = 4`, `DAT_140bb2698 = 2`) and accepts roots with
`-1.001 <= ξ,η <= 1.001` (`DAT_140bb26a0/a8 = ∓1.0010000467300415`), clamping accepted values to
[-1, 1] **[fact; full branch tree in `ifd_deep.c`, not re-typed here]**.
`FUN_1404ce05b` (2433 B) is a bisection on the quad (max `DAT_140643900 = 50` steps, tolerance
1e-6) **[fact, summarised]**; transcription of `FUN_1404cd2e1` / `FUN_1404ce05b`: [OBJPOS_interference_kernels.md §3](OBJPOS_interference_kernels.md#3-bilinear-inverse-map-if-511).
`FUN_1404d6974(V, P, &ξ, &η)`: `N1 = ((1-ξ)*(1-η))*0.25`, `N2 = 0.25*((ξ+1)*(1-η))` (corrected by FP audit; `*0.25` is exact, so only the order differs),
`N3 = ((ξ+1)*(η+1))*0.25`, `N4 = ((η+1)*(1-ξ))*0.25`; `P_k = (((0.0 + V(k,1)N1) + V(k,2)N2) + V(k,3)N3) + V(k,4)N4`
**[fact]**.

### 5.12 Triangle: `FUN_1404e2d44`, `FUN_1404e29fa`, `FUN_1404e34eb`

`FUN_1404e2d44(V, Q=(0,0,0), &b1, &b2, &b3, &bad)`: `A = area(V1,V2,V3)`; `bad = A < 1e-16`
(`DAT_140bb28c8`); `A1 = area(Q,V2,V3)`, `A2 = area(Q,V3,V1)`, `A3 = area(Q,V2,V1)`;
`b1 = A1/A`, `b2 = A2/A`, `b3 = A3/A`; `bad |= ((b1 + b2) + b3 >= 1.0000009536743164)`
(`DAT_140bb28d0`) **[fact]** [listing-checked]. ~~Q is the node itself (3D, off the face plane), so these are 3D Heron
areas, not projected ones.~~ (corrected by the geoutl port) The areas are **planar x,y** ones:
`FUN_1404e2d44` copies only rows 1-2 into its work triangles at `140644700`, whose row 3 stays at
its DATA value 0, so the node's height off the face is ignored **[fact, port bit-exact]**.
`FUN_1404e29fa(M, &area)` (Heron): for edges j = 1..3 (`j+1` wraps):
`s2 = ((0.0 + d1*d1) + d2*d2) + d3*d3` with `d = M(:,j) - M(:,j+1)`, `s2 < 0 → 0`, `L_j = sqrt(s2)`;
`s = (((0.0 + L1) + L2) + L3) * 0.5`; `a = (s - L3) * ((s - L2) * ((s - L1) * s))` (corrected by FP audit, listing `0x1404e2b93..0x1404e2bd5`);
`area = a >= 0 ? sqrt(a) : 0` **[fact]**.
`FUN_1404e34eb(V, P, &b1, &b2, &3)`: `w = (b1, b2, (1.0 - b1) - b2)`;
`P_k = ((0.0 + V(k,1)w1) + V(k,2)w2) + V(k,3)w3` **[fact]** [listing-checked].

## 6. Automatic target search (3D, token 6 = 0)

```
FUN_140378c80(model, &A.number, coupled, ncoupled, &list)       // std::vector<obj*>
best = 1e29 (DAT_14052c598); Bsel = NULL
for i = 0 .. size-1 (FUN_1403823e0 size, FUN_140382400 at):
    Ci = list[i]
    FUN_1401a0b00(model, A, Ci, &tok3..5, &tok2, &d, &DAT_140706bb0, flag_init = 1)   // result ignored
    a = normalize(tok3..5); b = normalize(DAT_140706bb0)      // FUN_14023b050 copy, FUN_14023b720
    if ((a0*b0 + a1*b1) + a2*b2) < 0: d = -d                   // xor DAT_14051bec0
    if d < best: best = d; Bsel = Ci
B = Bsel
```
**[fact]** [listing-checked]. Candidate filter `FUN_140378c80`: for every list position k = 1..`model+0x1378`:
object present, type != 7, number != A's, type != 5, number not in the coupled list (raw value
compare, `FUN_140378c20`), and `(type != 0 && mesh) || (type == 0 && (mesh || FUN_140160cc0))`,
and `(A.mesh || C.mesh)` **[fact]**.

Quirks (all **[fact]** from the code paths):
* The kernel runs with flag 1 during the search whatever the mode; the final call uses the mode's flag.
* No movement-direction fallback in the search: a zero direction gives `0/0` in §5.2 step 1
  (NaN) [inferred: every candidate then fails].
* The return value is ignored. A candidate that fails in both directions sets `d = 0.0`, which
  beats every positive distance; a candidate rejected by the early checks of §5.1 leaves `d` and
  `DAT_140706bb0` from the previous candidate (uninitialised stack for the first one).
* A displacement opposite to the direction (backward retry, or the flag-1 back-off of §5.4 for a
  candidate lying behind the mover) makes the distance negative, and it wins over any forward hit.
  The same signed comparison decides the coupled minimum in §4.2.
* `DAT_140706bb0` holds the last candidate's displacement; the final `FUN_14019fa00` overwrites it.
* No candidate selected (`best` never improved): `B = NULL`, §4 step 5 dereferences it.

## 7. 2D kernel

### 7.1 `FUN_1401a0c70(model, A, B, D, &tol, &dist, disp, flag)` (1780 B)

1. For B, then A: if `type == 0` and it has die curves and curve 0 has kind `+0x10 == 2`: copy all
   curves (array of 0x30-byte curve objects, vtable `+0x20` = assign) and run
   `FUN_140306a70(obj)` (OR of `FUN_14027e990(curve)` over the curves) **[fact]**.
2. `rc = FUN_1401a1370(model, B, A, D, tol, 1000, r, flag)`.
3. Restore the curves that were copied (`FUN_14019a620`), free the copies.
4. `rc == 0`: `dist = sqrt(r0*r0 + r1*r1)`, `disp = r` (2 doubles); success iff `dist < 1e9`
   (`DAT_140522f50`), else `dist = 0`, fail. `rc != 0`: `dist = 0`, fail **[fact]** [listing-checked].

### 7.2 `FUN_1401a1370(model, S=B, T=A, D, tol, n=1000, out, flag)` (5447 B)

```
for X in (S, T): if FUN_1401ffa90(X, &loop): copy loop node count·2 doubles, copy X coords (nnode·2)
(dx, dy) = D
θ  = FUN_1404865ac(1.0) * 2.0 - FUN_140485d6c(dy/l, dx/l)     // l = sqrt(dx*dx + dy*dy); atan, atan2 (CRT)
FUN_1401a28c0(model, S, curveS, coordsS, rotS, side=1, &kindS, &θ, &nS, &xS, &yS, &areaS)   // §7.4
FUN_1401a28c0(model, T, curveT, coordsT, rotT, side=2, &kindT, &θ, &nT, &xT, &yT, &areaT)
ref = (kindS==2 && kindT==2) ? (areaT <= areaS ? areaT : areaS)
    : kindS==2 ? areaS : kindT==2 ? areaT
    : (fabs(xS.max - xS.min) * fabs(yS.max - yS.min)) (== 0 → 10.0, DAT_14051bee8)
Atol = (ref * tol) / 100.0                                      // DAT_140524cd8
if flag == 1:
    y0 = yS.min; s0 = yS.min - yT.max
    T moved by (0, s0): curve kind 1 → every curve point y += s0 (points 1..4 of each 6-double
    record: x+0, y+s0 alternately); kind 2 → rotated node y += s0
else: y0 = yT.max; s0 = 0
x0 = max(xS.min, xT.min); x1 = min(xS.max, xT.max)
if !(x0 < x1) || fabs(x1 - x0) < 1e-12: return 1
g[0..n] = 0
FUN_1403460c2(model+0xdb0, model+0xdb8); FUN_1403460e7(&max(fabs(xS.max-xS.min), fabs(yS.max-yS.min)))
FUN_14033ffb8(&flag)                       // DAT_1405f6000 = (flag == 0)
side +1: kindS==1 ? FUN_140340273(S, curve, ...) : FUN_14033ffe0(S, poly, ...)   (y0, x0, x1, n, g, &err)
if err >= 1: return 1
side -1: same for T with its own reference y (local_2f8 = y0) 
if err >= 1: return 1
(flag==0: g_i -= (y_T_ref - y0) = 0.0)     // no-op as written
dxs = (x1 - x0) / n
step = max(yS.max - yS.min, yT.max - yT.min) * 0.1, at least 0.01      // DAT_14051bef8, DAT_14051f258
FUN_14033f8d4(&step, &Atol, g, &n, &dxs, &s, &err)                       // §7.3
if err == 1: return 1
S_tot = s0 + s
out = (0.0*cos θ + S_tot*sin θ, (-0.0)*sin θ + S_tot*cos θ)              // FUN_140bb3490 cos, FUN_140bb3000 sin
return 0
```
**[fact]** (sum orders as in the decompile) [listing-checked]. The sampling routines add, for each abscissa
`x_i = ((x1 - x0)/n)*i + x0` (FUN_14033ffe0) or `i*((x1-x0)/n) + x0` (FUN_140340273), the
smallest |gap| (`FUN_14033ffe0`) or smallest gap (`FUN_140340273`) between the reference line and
the object's boundary pieces; with `DAT_1405f6000 == 1` (modes 102/112) gaps below `-1e-15` are
skipped **[fact]**. Pieces: line segments `FUN_14030bba0` (3271 B), arcs `FUN_1403407d7` (2345 B,
uses π = `DAT_1405f6238`) — transcribed in [OBJPOS_interference_kernels.md §4](OBJPOS_interference_kernels.md#4-2d-gap-kernels-if-72).

### 7.3 `FUN_14033f8d4(&step, &Atol, g, &n, &dx, &s, &err)` (1252 B): overlap-area root

```
gmin = min_i g_i  (FUN_140437b3b, start 1e20); if gmin >= 1e10: err = 1; return
A(s) = Σ_{i=1..n} ((max(0, s-g_i) + max(0, s-g_{i+1})) * dx) * 0.5 + acc   // FUN_14033fe8e, acc from 0
lo = gmin, Alo = 0; s = step + gmin
loop: A = A(s); if Atol == A: return; if Atol <= A: break; lo = s; Alo = A; s = lo + step
hi = s; Ahi = A; side = -1
repeat (max 100, else err = 1):
    f = (Ahi - Atol) / (Ahi - Alo); w = hi - lo; s = hi - f*w; A = A(s)
    if fabs((Atol - A)/Atol) <= 1e-3 || fabs(w) <= 1e-5: return        // DAT_1405f6018, DAT_1405f6020
    σlo = (Alo - Atol >= 0) ? 1 : -1 ; σ = (A - Atol >= 0) ? 1 : -1
    if σ != σlo: hi = s; Ahi = A; if side == σ: Alo = (Atol + Alo)*0.5; side = σ
    else:        lo = s; Alo = A; if side == σ: Ahi = (Atol + Ahi)*0.5; side = σ
```
**[fact]** (Illinois-type false position) [listing-checked]. With `tol = 0` (area 0 %) the iteration never converges:
every step gives `s = lo`, `A = 0`, the relative test is NaN (unordered → not converged) and `|w| = step >= 0.01`,
so it fails after 100 steps (`err = 1` → "interference positioning failed"), not "touch"
([OBJPOS_interference_kernels.md §4.6](OBJPOS_interference_kernels.md#46-area-function-and-root-finder)).
**Confirmed after the oracle run** (all seven 2D cases with token 2 = 0 print
`Object 2 to Object 1 interference positioning failed.`, the same cases with 1 % succeed) and in the
listing `L_3f8d4.txt`: the counter test is `CMP EAX,0x64; JG 0x14033fd1c` at `0x14033fade` (fail when
the 101st step would start, i.e. after 100 steps); the relative test `0x14033fb9d..fbb7` is
`|(Atol - A)/Atol|` against 1e-3 with `JP` → not converged. Precisely: `s = hi - 1.0*(hi - lo)` is
`lo` only up to rounding, so `A(s)` is 0 or a tiny positive value; the relative test is then NaN or
±Inf, never converged; `σ = σlo = +1` on every step (both `A >= 0 = Atol`), so `hi` never moves,
only `Ahi` is halved, `f` stays 1 and `|w| = step >= 0.01`. A tiny positive area (1e-6 %,
`OBJPOS/v-ifd-areatiny-2d`) converges at touch plus a small overlap.
Numeric check: `OBJPOS/2-area-2d` (1 %) moves die 2 by (1.57e-15, -12.8415477900); an independent
evaluation of `A(s)` from the SPIKE geometry (reference area 25.4·57.15, `Atol` = 14.516, root by
bisection) gives -12.839, inside the 1e-3 area tolerance; mode 102 (`OBJPOS/v-ifd-area1-102-2d`, flag 0,
other bracket start) gives -12.8380134961, so the exact value depends on the iteration path.

### 7.4 `FUN_1401a28c0` (1482 B): rotated boundary of one object

* No die curves (`!FUN_140160d20`): kind 2, outer boundary loop (`FUN_1401ffa90`, the loop whose
  bounding box contains the running box; score 1111 test via `pow(10, c)` = `FUN_1402204a0`)
  rotated by `FUN_14033f2bf` **[fact]**:
  `(sθ, cθ) = FUN_1404b3b30(θ)` [inferred sin, cos]; per node `x' = cθ*x - sθ*y`,
  `y' = x*sθ + y*cθ` [listing-checked]; tracks x-min/max (side-dependent tie rule with 1e-6), y-min/max (start ±1e11),
  `area = -(S * 0.5)` with `FUN_14033fdb8`: `S = 0.0; for k = 2..n: S = ((x_k*y_{k-1}) + S) - (x_{k-1}*y_k)`; then `S = ((y_n*x_1) + S) - (x_n*y_1)`; returns `S * 0.5` (exact order given by FP audit, listing `0x14033fded..0x14033fe85`) and reorders the
  loop to start at the extreme node.
* Die curves: kind 1; one curve → copy it; several → `FUN_1401ff7f0` picks one; a curve with
  kind != 2 is converted (`FUN_1402763d0(curve, 2)`); rotate by `θ·57.29577951308232`
  (`DAT_140522fd8`, vtable `+0x40`), bounding box `FUN_140279960`, `FUN_14034c209` stores it and
  `DAT_140b84730 = max(ymax-ymin, xmax-xmin)`. No usable curve but a mesh → kind 2 as above
  **[fact]**.

## 8. Path branch (movement type 6, "arbitrary path")

### 8.1 `FUN_14019fb50(model, A, B, -, &tol, ...)` (1130 B)

```
t0 = FUN_1401f3ae0(model+0x750)
fprintf(stdout, "  Arbitrary path has been detected.\n")
fprintf(stdout, "  Current TNow is %lf.\n", t0)
backup = copy of A (FUN_14015ec80, FUN_14015ef60, FUN_1401613a0(backup, A, model+0x15f8, flags))
tnew = 0
ok = FUN_14019ffc0(model, A, B, &tol, &d, disp, &tnew)
if !ok: fprintf(stdout, "  Failed in interference positioning\n"); restore A from backup; return 0
for each coupled id (n-th object, not empty):
    if FUN_14019ffc0(model, C, B, &tol, &d, disp, &tc) && tc < tnew: tnew = tc
fprintf(stdout, "  Calculated new time = %lf\n", tnew)
FUN_1401f3b30(model+0x750, &tnew)              // set current time
return 1
```
**[fact]**. The direction tokens, `flag`, the distance and `DAT_140706bb0` are not used/written
in this branch. The dispatcher then calls `FUN_14019ed10` (8.3).

### 8.2 `FUN_14019ffc0(model, X, B, &tol, &d, disp, &tout)` (825 B)

Requires `FUN_14021e470(X+0x2160)`. Builds the path curve `FUN_14021ee40` (copy of T@25a8; if
`+0x458 != 1` converted from speed to time by `FUN_140134de0`: `t_k = t_{k-1} + len_k / v_{k-1}`,
`len = sqrt(Σ Δ²)`; `+0x4c0 == 2` adds an offset `FUN_140194d40`/`FUN_140135bc0`). For segments
k = 0 .. npts-2: `ta = t_k`, `tb = t_{k+1}`, `Pa`, `Pb` (`FUN_1401350e0`, 3 comps when ncomp 4
else 2); `seg = Pb - Pa`; `len = FUN_14023b720(seg)` (normalises `seg`); if `len > 0`:
`FUN_1402007d0(X, &ta)` (put X at the start of the segment), `FUN_1401a0b00(model, X, B, &seg,
&tol, &d, disp, flag = 0)`; if it succeeds and `d <= len`:
`tout = ta + (d / len) * (tb - ta)`, `FUN_1402007d0(X, &tout)`, return 1 **[fact]** [listing-checked].

### 8.3 Repositioning at a time

`FUN_14019ed10(model)`: `t = FUN_1401f3ae0`; for every object position k with
`FUN_14021e470`: `FUN_1402007d0(obj, &t)` **[fact]**.
`FUN_1402007d0(X, &t)`: only movement type 6 with a non-zero path; `P = FUN_1401351c0(path, t)`
(interval search `FUN_140132a60` → index i, fraction f; `P_c = p_i,c + f*(p_{i+1},c - p_i,c)`);
`cur = FUN_140194cb0(mv)` (= `mv+0x630` for type 6, else `mv+0x170`); `δ = P - cur`; if
`sqrt(δ·δ) > 1e-12`: `FUN_14021d0b0(X, δ, &0, &1)`; then `STROKE (X+0x22d0, +0x22d8, 3D +0x22e0) = *cur`
**[fact]**.

## 9. Small shared helpers

### 9.1 `FUN_14021d0b0(obj, v, &a, &b)` (move by a vector; also OBJPOS.md)
If `v != NULL` and not all |v_i| < 1e-30 (`DAT_14051bee0`; 2D tests 2 comps): if `*b`:
`FUN_1402000a0(obj, v)`; `FUN_140200610(obj, v)`; `FUN_14021d190(obj, v, a)` **[fact]**.

### 9.2 `FUN_1401952e0(mv, a, b, out)` (476 B)
`e = FUN_140239b40(mv+0x50)` (direction / |direction|, zero when `|len| <= 1e-12`);
if `fabs(|e|) > 1e-6` and `fabs(b) > 1e-6` (`DAT_14051f1b8`): `out = (b - a) * e` (`FUN_140239ea0`:
`(b-a)*e_i`); else `out = 0` **[fact]** [listing-checked].

### 9.3 `FUN_14018cd10(mv)`: MOVCTL direction present = any of `mv+0x50, +0x58 (, +0x60 in 3D)` != 0.0 **[fact]**.

## 10. Constants

| Address | Value | Use |
|---|---|---|
| `DAT_14051f260` | 1e-12 | zero-direction tests |
| `DAT_14051bee0` | 1e-30 | move-vector zero test |
| `DAT_14051f1b8` | 1e-6 | `FUN_1401952e0` |
| `DAT_14052c598` | 1e29 | auto-search start |
| `DAT_140522fc8` | -1.0 | reverse direction / result |
| `DAT_14051bec0`, `DAT_140bb2688` | sign bit | negation by xor |
| `DAT_140baf8a8`, `DAT_140bb2638`, `DAT_140baec70` | abs mask | fabs by and |
| `DAT_140baf898` / `DAT_140baf8a0` | -1e5 / 1e5 | zmax start / zmin start, "no hit" distance |
| `DAT_140643930`, `DAT_1406438d8` | 1e-9 | rotation degeneracy; bilinear coefficient |
| `DAT_140604eb0` | (0, 0, 1) | ray direction |
| `DAT_140604ed8` | 1e-8 | face facing test |
| `DAT_140604ee0` | 1e-15 | degenerate edge (squared) |
| `DAT_140604ef0` | (0, 0, 0) | query point |
| `DAT_140baf8b0` | 1e8 | point-in-polygon start distance |
| `DAT_140baf8c0`, `DAT_140bb26a8` | 1.0010000467300415 | barycentric / ξη acceptance |
| `DAT_140bb2640` | 1.0000000133514320e-10 | polygon edge length / vertex tie |
| `DAT_140bb2668` | 1.0000000168623835e-16 | cross-product length |
| `DAT_140bb2660`, `DAT_140bb2670` | 0.25, -0.25 | bilinear |
| `DAT_140bb2678`, `DAT_140bb2680` | 1e-5, 1e-6 | bilinear degeneracy, point match |
| `DAT_140bb28c8`, `DAT_140bb28d0` | 1e-16, 1.0000009536743164 | triangle area, barycentric sum |
| `DAT_140522f50` | 1e9 | 2D max distance |
| `DAT_140524cd8` | 100.0 | 2D percent |
| `DAT_14051bee8` | 10.0 | 2D reference-area fallback |
| `DAT_14051bef8`, `DAT_14051f258` | 0.1, 0.01 | 2D step |
| `DAT_140baec80` | 1e10 | 2D "no gap" |
| `DAT_140baec90` | -1e-15 | 2D negative-gap cut (modes 102/112) |
| `DAT_1405f6018`, `DAT_1405f6020` | 1e-3, 1e-5 | 2D iteration tolerances |
| `DAT_140522fd8` | 57.29577951308232 | rad → deg |

## 11. Messages (stdout)

| Text | When |
|---|---|
| `Object %d to Object %d interference positioning failed.\n` | kernel returned 0 (3D and 2D) |
| `Object %d has been interference positioned w.r.t. Object %d.\n` | 3D success; 2D success after a time change |
| `Object %d is interference positioned w.r.t. Object %d.\n` | 2D success without time change |
| `       Current time has been changed to %g from %g\n` | path branch success (new, old) |
| `  Arbitrary path has been detected.\n`, `  Current TNow is %lf.\n` | path branch start (fprintf stdout) |
| `  Failed in interference positioning\n` | path branch failure |
| `  Calculated new time = %lf\n` | path branch success |
| `Object %d has been moved by (%g, %g, %g).\n` / `Object %d(coupled) has been moved by (%g, %g).\n` | each coupled object (3D / 2D) |
| `ERROR  : Object %d Undefined\n`, `ERROR  : No Mesh or Die Defined for Object %d\n` | target or coupled object checks (return 3 for the target) |

Return: 2 when a tokenizer call fails; 3 when the moving or target object is missing/unusable
(message only for the usable-object checks); otherwise 0 (also after "failed") **[fact]**.

## 12. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140378c80` | candidate list | 366 | read fully |
| `FUN_140378c20` | id in list | 76 | read fully |
| `FUN_140382350/3c0/3e0/400/440/4e0` | vector ctor/dtor/size/at/push/clear | – | read |
| `FUN_14019fa00` | router | 320 | read fully + listing |
| `FUN_14019ec30` | all-path test | 214 | read fully |
| `FUN_14021e470` | path movement test | 59 | read fully |
| `FUN_14012fd80`, `FUN_14012ff20` | table all-zero / value count | 264, 200 | read |
| `FUN_1401a0300` | direction + coupled minimum | 1793 | read fully |
| `FUN_1401a0a10`, `FUN_1401a3a00`, `FUN_1401a0bd0`, `FUN_14019eb60` | kernel + move wrappers | – | read fully |
| `FUN_1401a0b00` | 2D/3D kernel switch | 188 | read fully |
| `FUN_1401a3aa0` | 3D kernel driver + retry | 1002 | read fully |
| `FUN_1401a3ea0` | 3D loader/rotator | 5569 | read fully |
| `FUN_1401a5470` | rigid–rigid 3D | 6148 | read fully |
| `FUN_1403539a1` | 3D distance driver | 2139 | read fully + listing |
| `FUN_1403541fc` | node ray vs faces | 3269 | read fully + listing |
| `FUN_1404cf5c0` | rotation matrix | 538 | listing |
| `FUN_1404d0939` | rotate points | 793 | read fully |
| `FUN_1404cccfb`, `FUN_1404d3d43`, `FUN_1404cce5e` | normals | – | read fully |
| `FUN_1404cc289`, `FUN_1404d064d` | point in polygon, point–segment | 2674, 748 | read, condensed |
| `FUN_1404ccf58`, `FUN_1404d6974` | inverse/forward bilinear | 905, 415 | read fully |
| `FUN_1404cd2e1` | bilinear analytic solve | 3450 | read, summarised |
| `FUN_1404ce05b` | bilinear fallback | 2433 | summarised (open 3) |
| `FUN_1404e2d44`, `FUN_1404e29fa`, `FUN_1404e34eb` | barycentric | – | read fully |
| `FUN_140354ed0`, `FUN_140355cbf`, `FUN_14035672f` | boundary faces | 3567+ | summarised (open 1) |
| `FUN_140225640/ff0/820`, `FUN_140226150`, `FUN_140228c60`, `FUN_140229f30` | bucket grid | – | summarised (open 2) |
| `FUN_1401a0c70` | 2D driver | 1780 | read fully |
| `FUN_1401a1370` | 2D kernel | 5447 | read fully |
| `FUN_1401a28c0`, `FUN_14033f2bf`, `FUN_1401ffa90`, `FUN_14034c209` | 2D boundaries | – | read |
| `FUN_14033ffe0`, `FUN_140340273`, `FUN_14033fe8e`, `FUN_14033fdb8`, `FUN_140437b3b` | 2D sampling, area | – | read |
| `FUN_14033f8d4` | 2D root finder | 1252 | read fully |
| `FUN_14030bba0`, `FUN_1403407d7`, `FUN_140341100`, `FUN_14032bd2a` | segment/arc gaps | – | summarised (open 4) |
| `FUN_14019fb50`, `FUN_14019ffc0`, `FUN_14019ed10`, `FUN_1402007d0` | path branch | – | read fully |
| `FUN_14021ee40`, `FUN_1401350e0`, `FUN_1401351c0`, `FUN_140134de0`, `FUN_140194cb0` | path table | – | read |
| `FUN_14015ec80/ef60/f790`, `FUN_1401613a0` | object backup/restore | – | not followed |
| `FUN_1401952e0`, `FUN_140239b40/9a30/9ea0`, `FUN_14018cd10` | movement-direction helpers | – | read fully |
| `FUN_14021d0b0` | move object | 211 | read (callees in OBJPOS.md) |
| `FUN_1401f3ae0`, `FUN_1401f3b30` | time get/set | – | read fully |

About 75 functions followed.

## 13. Open items

Items 1-6 are covered by [OBJPOS_interference_kernels.md](OBJPOS_interference_kernels.md): 1 → §1, 2 → §2,
3 → §3, 4 → §4, 5 → §3.3 (sin/cos part; the CRT atan/atan2 of the 2D θ stay open there), 6 → §5.


1. Face-node tables of `FUN_140355cbf`/`FUN_14035672f` (FNDBND): they fix the orientation of
   mesh boundary faces and therefore which faces pass `e > 0` in §5.8. Not dumped.
2. Bucket grid (`FUN_140226150` build, `FUN_140228c60` query): only decides which faces are
   tested. A port that tests every face gives the same minimum when no face lies below the node
   in the ray's column; overlapping 102/112 cases may differ [inferred].
3. `FUN_1404ce05b` (bilinear bisection fallback) and the full branch tree of `FUN_1404cd2e1` need a
   listing-level transcription.
4. 2D gap kernels `FUN_14030bba0`, `FUN_1403407d7`, `FUN_140341100`, `FUN_14032bd2a`,
   `FUN_14027e990`, `FUN_1402763d0` and the curve virtuals were not transcribed.
5. `FUN_1404b3ab0/ab8` (sin/cos of the Fortran runtime) and `FUN_1404865ac`/`FUN_140485d6c`
   (CRT atan/atan2) were identified by call pattern only [inferred].
6. Object backup/restore of the path branch (`FUN_1401613a0` with 6 flag bytes) not followed.

## 14. Tests

`tests/commands/OBJPOS/ifd_m{2,12,102,112}_*.KEY`, listed in `tests/commands/OBJPOS/cases_ifd.json`
(labels `OBJPOS/<mode>-<variant>`): explicit target, depth sign, MOVCTL-direction fallback,
oblique direction, overlap with flag 1 vs 0, deformable and tet movers, automatic target (incl.
the target-behind case and coupled exclusion), coupled objects, failures (no MOVCTL direction, no
hit, missing mover/target), 2D area tolerance, 2D target 0 and the 2D failed-stroke quirk.
Expected values in the file headers came from this analysis; after the oracle run each header has
an `ORACLE` line with the observed result (verdicts: `oracle/ov-ifd.md`). The 2D cases with token
2 = 0 all fail (§7.3); their intended checks are repeated with 1 % in `ifd_v_*_2d.KEY`
(`cases_v_ov-ifd.json`). The automatic search with a zero direction and the NULL-target crash path
are not scripted.
