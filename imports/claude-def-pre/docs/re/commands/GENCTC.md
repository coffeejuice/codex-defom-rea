# GENCTC (registry id 6): generate initial contact boundary conditions

Source: `DEF_PRE_64.exe` v11.0.2, `FUN_140378e00` `case 6` (`out/bm_cmd.c` lines 567-581);
3D driver `FUN_1401a6c80`, 2D driver `FUN_1401a2e90`. Decompiles used (deform-rea-work):
`out/p67/genctc3d.c`, `out/p67/genctc2d.c` (depth 2) and the deeper closures
`out/p67/contact/g3d.c` (600 functions, depth 6) and `out/p67/contact/g2d.c` (324 functions);
call trees `out/p67/contact/tree3d.txt`, `tree2d.txt`; listings `out/p67/contact/l_*.txt`.
Shared mechanics: [_common.md](_common.md). INICTC (clear contact codes): [INICTC.md](INICTC.md).

Status: the C++ drivers (2D and 3D), the tolerance rules, the pair selection, the 2D Fortran
kernels and the BC-code bookkeeping are specified from code. The **3D Fortran contact search**
(≈160 Fortran routines, ≈230 KB of code, the DEFORM-3D solver contact module "SRCNT*") is only
mapped at routine level; see §5 and §9. Bit-exact 3D needs a follow-up packet.

## 1. Summary

`GENCTC tol` finds the boundary nodes of each deforming ("slave") object that touch an object it
is in contact with (inter-object pairs from CNTACT) and marks them in the slave's BCCDEF array
(`obj+0x990`) with the contact code **−(master object number)**. Nodes found within the tolerance
are also **moved onto the master surface** (coordinates change). Corpus usage is always
`[OBJPOS …] / INICTC slave / GENCTC tol` (e.g. `ROLL_EN.MST`, `HAMMER_EN.MST`, 3D `.d3mo`
files; `GENCTC 0` is the most common form, 1021 occurrences).

| | 2D (`FUN_1401a2e90`) | 3D (`FUN_1401a6c80`) |
|---|---|---|
| unit of work | each pair with direction 2 (master ≠ slave) | each object (slave) with its partner list |
| master surface | rigid (type 0): die geometry curves (line/arc segments); otherwise the master's FE boundary loops | bucket ("cube") surface of every object: FE boundary surface 0, or the rigid surface polygons |
| kernels | Fortran 2D (`0x1403xxxxx`, `0x14043xxxx`) | Fortran 3D contact module (`0x14002xxxx`-`0x14008xxxx`) + C cube/locator library (`0x14022xxxx`, `0x1403fxxxx`-`0x14040xxxx`) |
| commit | all-or-nothing per pair, after an element validity check | in place, node by node |
| messages | Fortran unit at `0x140b847a4` (never assigned, 0) → **stderr**, CRLF line ends (corrected after oracle run: `v_spike_push1004`) | mostly unit 1 = `NEWEQ.MSG`, deleted at exit (no `NEWEQ.MSG` left in any oracle run); some on unit 6 = stdout |

## 2. Argument grammar

`FUN_140414990(f, &tol)`: rest of the keyword line, `sscanf("%Flf")` (commas → blanks). EOF or an
empty rest of line → returns 1 → case returns **2** (`ERROR: Keyword GENCTC Reading Error.`).
A non-numeric token → `sscanf == 0` → success with tol = 0 (automatic tolerance) **[fact]**
(reader behaviour from `group2-helpers.md`). One line is consumed. The driver's return value is
ignored; the case always returns 0 after a successful read **[fact]**.

## 3. Tolerance

| | Rule | Constants |
|---|---|---|
| 2D | if `tol < 1e-12`: `tol = FUN_140202a20(slave) * 0.05` [listing-checked] | `DAT_14051f260` = 1e-12, `DAT_140524ce0` = 0.05 (double) |
| 3D | if `tol < 1e-12`: `tol = FUN_140202da0(slave) * 0.05`; if the result `<= 0` the object is skipped [listing-checked] | same |

**Quirk [fact]:** the tolerance variable is the driver's parameter and is overwritten. The
automatic value computed for the first slave/pair is kept for **all later slaves/pairs** of the
same GENCTC call (it is no longer `< 1e-12`). In 3D, after a skipped object (value ≤ 0) the next
object recomputes.

**(clarified after oracle run)** In 3D the value is computed at step 3 of §5.1, *before* the
partner list (step 4), so "first slave" means the first object slot that passes step 2 (found,
type ≠ 7/5, has a mesh), even when it then has no partner and is skipped. In UPSET_Brick and
UPSET_Tet the rigid top die 1 carries a mesh: `GENCTC 0` takes `0.05 × 12 = 0.6` from die 1's
elements (10 × 12 × 10/15 boxes) and uses it for the workpiece 2, not `0.05 × 5 = 0.25` from
the workpiece. Evidence: `v_auto_out` (die lifted 0.26, `GENCTC 0`: accepted) against
`v_explicit025` (same, `GENCTC 0.25`: rejected), `v_auto_lift0.4` accepted, `v_auto_lift1.0`
rejected, `v_auto_lift0.6` accepted for some nodes only (|d| ≈ tol, decided by round-off of the
quad projection). In 2D the first pair's slave is the workpiece (SPIKE_SI: `0.05 × 1.15798 =
0.0579`; `v_spike_auto_in` lift 0.05 accepted, `v_spike_auto_out` lift 0.07 rejected).

Element size functions (both read fully):

* 2D `FUN_1403046a0(obj)` (via `FUN_140202a20`, which dispatches on the object's model
  dimension): 0.0 if no mesh / no elements / no connectivity (`obj+0x9e8`) / no coords
  (`obj+0x8a8`). Else `min` over elements e and edges of `sqrt(dx*dx + dy*dy)` (`FUN_1403087a0`
  = `x*x + y*y`, then `FUN_140484644` = CRT `sqrt`), with dx, dy = node(j) − node(j+1),
  4 nodes per element, edge j = 0..3 (edge 3 closes to node 0). Start value `DAT_14052c598` =
  1e29 [listing-checked]. **Quirk:** if the *first* element has `conn[0] == conn[3]` (triangle stored as a
  degenerate quad), edge 3 is skipped for **every** element.
* 3D `FUN_140202e00(obj)` (via `FUN_140202da0`; in 2D that function prints
  `LLOBJ_DATA::calculateElementBoxMin() need to be checked in 2D.` and returns 0): −1.0
  (`DAT_140522fc8`) if no mesh or `obj+0x884`/`obj+0x894` is 0 or the arrays are missing;
  else `min` over elements of `max(xmax−xmin, ymax−ymin, zmax−zmin)` of the element's
  nodes (start 1e20 `DAT_14051f1b0`; ties: `if (a <= b) b else a` order as decompiled).

Inside the 3D Fortran (`FUN_14003b304`) the value is re-tested: if `tol <= 1e-10`
(`DAT_140bad130`, float-rounded constant 1.000000013351432e-10) it is replaced by
`sqrt(min positive node real value #2 of the object) * 0.0500000007` (`DAT_140bad758`, a
float constant) and by 1e-10 if that is ≤ 0. Since the C++ driver already replaced small values
this only matters for 1e-12 ≤ tol ≤ 1e-10 **[fact]** [listing-checked].

## 4. 2D algorithm

### 4.1 Driver `FUN_1401a2e90(model, tol)` (699 bytes, read fully)

```
for p = 1 .. count(model+0x13c8):                 // pair list, FUN_1401fac90 (next at +0x390)
  pair = p-th pair; skip if NULL or dir (+0x170) == 0
  dir 1: S = +0x168, M = +0x16c ; dir 2: S = +0x16c, M = +0x168 ; other: skip
  skip if S == M
  objS = lookup(S), objM = lookup(M)                // FUN_1401d6ea0
  need objS && objS+0x87c (mesh) && objM && (objM+0x87c || FUN_140160d20(objM))
  if tol < 1e-12: tol = minEdge(objS) * 0.05       (§3)
  FUN_1401a3160(model, objM, objS, pair, tol, &fail, 0)
```

`FUN_140160d20(obj)` = `FUN_1401983a0(obj) > 0` (object has rigid 2D geometry entities
**[inferred]**). The returned −2 / 0 is not used by the dispatcher.

### 4.2 Pair worker `FUN_1401a3160(model, M, S, pair, tol, &fail)` (2198 bytes, read fully, checked in the listing)

1. Returns 0 (nothing) unless `pair+0x170 == 2` **[fact, listing 0x1401a31a8]**. A pair with
   direction 1 is therefore handled only through its mirrored pair (s,m) with direction 2,
   which CNTACT reading always creates (CNTACT.md, mirroring). The object order is the same
   either way: S = the pair's second object (+0x16c).
2. Returns 0 if S type (`+0xc`) is 7 or 5 or S has no mesh, or M type is 7 or 5 **[fact]**.
3. `ndof = model+0x1640` (2). Work copies: `bc3[N*3]` int (BCCDEF codes x, y of S, third int
   0) and `xy[N*2]` double (S coordinates from `FUN_1401808d0(S+0x8a0)` = the RZ array).
   `*fail = 0`, `changed = 0`.
4. For each boundary loop k of S (`S+0xbd8` loops, record `S+0xbe0+0x30k`: `+0` node count
   n_k, `+8` node list): the loop is processed with **n_k − 1** nodes (the last entry repeats the first) **[inferred]**:
   * **M type 0 (rigid)**: for each geometry entity c = 1..`FUN_140198310(M)` of M's 2D
     geometry container (`M+0x3998`, entity list 0): copy the curve into a temporary
     `_OBJ_GEO2D` (`FUN_140275cd0`, `add`), convert it to segment form with
     `FUN_1402763d0(geo, 2)` (§4.7), then `FUN_140350f12` (§4.3) over the loop's nodes.
   * **M of any other type (meshed master)**: for each boundary loop j of M,
     `FUN_14034fc56` (§4.5) with M's coordinates and loop node list.
5. If `changed == 0`: return 0, nothing written.
6. Element check: for every element e = 1..`S+0x884`, `FUN_14032e34e(e, conn, xy, &ok)`
   (§4.6) on the **moved** coordinates. If any element fails, return 0 and nothing is written
   (no message) **[fact]**.
7. Commit: copy `bc3` codes x, y back to `S+0x990` and `xy` back to the RZ array; return 1.

The pairs are visited in list order and every pair works on fresh copies, so a later pair sees
the codes and coordinates committed by earlier ones **[fact]**.

### 4.3 Rigid master: `FUN_140350f12` (1990 bytes, read fully)

Per node i of the loop list (1-based, in list order):

```
lim  = |tol * 100.0|                       (DAT_140baf860 = 100.0; computed once, abs via mask)
P0   = xy[i]                               (saved)
FUN_140436a9e(&nseg, seg, xy[i], &tol, &d, &iseg, &st, ...)   // nearest segment; moves xy[i] if st == 2
moved = (st == 2) ? sqrt(ey*ey + ex*ex) : 0.0     with e = xy[i] - P0
if moved <= lim:
   if tol * (-100.0) <= d:                 (DAT_140baf868 = -100.0)
      if st == 2: changed = 1; update codes (rule below)
   else: print 1002 warning; xy[i] = P0; fail = 1
else:  print 1004 warning; xy[i] = P0; fail = 1
```
[listing-checked] (`lim`, `moved`, the `-100·tol` product).

Code update rule (also used in §4.4), with `c0, c1` = the node's x, y codes and `K = −M`:

| Condition (in order) | Result |
|---|---|
| `c0 == K` or `c1 == K` | unchanged |
| `c1 == 0` | `c1 = K` |
| `c1 < 0` | if `c0 < 1`: `c0 = c1`; then `c1 = K` |
| `c0 < 1` | `c0 = K` |
| otherwise (both > 0) | warning 1001 (`Unable to change BCC`), unchanged |

Messages (Fortran formatted WRITE `FUN_1404b5ef4`, unit variable `0x140b847a4` = 0 → **stderr**,
lines end in CR LF; confirmed by the oracle, corrected after oracle run). Example
(`v_spike_push1004`, die 2 pushed 1.0 into the workpiece, `GENCTC 0.001`, so lim = 0.1):

```
    WARNING: Node  318 pulled to object   2 surface.
             Moved to entity,     5, distance      -0.9743939
             Invalid contact boundary condition on mesh
```

(three such blocks, nodes 318, 327, 332; nothing committed, node 318 loses its `-2`).

* `1001(4X,'WARNING: Unable to change BCC for Node ',I4,';',' BCCDEF = (',2I4,')')` — node, c0, c1
* `1002(4X,'WARNING: Node ',I4,' pulled to object ',I3,' surface.',/,4X,'         Moved to entity,',I6,', distance ',F15.7)` — node, M, iseg, d
* `1004(...same...,/,4X,'         Invalid contact boundary condition on mesh')`

### 4.4 Nearest-segment search `FUN_140436a9e` (1335 bytes, read fully)

Segment records are 6 doubles: `[0]` type (1.0 line, 2.0 arc), `[1..2]` start A, `[3..4]`
end B (line) or centre C (arc), `[5]` sweep angle (arc).

```
best = 1e11 (DAT_140bb1580); ibest = 0
for j = 1..nseg:                                  // mode 1 = measure only
   type 1.0: line(P, A, B, mode 1) -> dj         type 2.0: arc(P, A, C, ang, mode 1) -> dj
   a = |dj|, b = |best|
   if ((|a - b| > 1e-10 or dj >= 0) and a <= b):
       if j == ibest + 1 and |b - a| <= 1e-10:   // consecutive segment, same distance
           dj = a * ((best < 0 and dj < 0) ? -1.0 : 1.0)
       ibest = j; best = dj
if tol < best: status 1 (not in contact), return
status 2: rerun the kernel of segment ibest with mode 2 (moves P onto it)
```
[listing-checked]

Line kernel `FUN_1404345c0` (914 bytes; operation order checked in the listing):

```
dx = Bx-Ax; dy = By-Ay; L = sqrt(dy*dy + dx*dx)
px = Px-Ax; py = Py-Ay
if L >= 1e-10:
   ux = dx/L; uy = dy/L
   s = uy*py + ux*px                    // along the segment
   h = uy*px - ux*py                    // signed normal distance
   if s < 0 or L < s:
      dA = sqrt(px*px + py*py); dB = sqrt((Py-By)^2 + (Px-Bx)^2)
      nearer endpoint E (A if dA <= dB); d = copysign-like: (h >= 0) ? dE : -dE
      move: P = E
   else: d = h; move: P = (s*ux + Ax, uy*s + Ay)
else: d = sqrt(px*px + py*py) (unsigned); move only when mode == 2: P = A
```
[listing-checked] (all of it, incl. `dA = sqrt(px*px + py*py)`, `dB = sqrt((Py-By)*(Py-By) + (Px-Bx)*(Px-Bx))`).

Arc kernel `FUN_14043574f` (1371 bytes) [listing-checked]: r = A − C, R = sqrt(rx*rx + ry*ry), end point
E = C + rot(r, ang) (`FUN_14043556b`: `sincos` via `FUN_1404b3b30`, x' = cos·x − sin·y,
y' = sin·x + cos·y); q = P − C, rq = sqrt(qy*qy + qx*qx); h = (rq − R)·sign(ang); foot
F = (qx/rq·R + Cx, qy/rq·R + Cy). The angle of q from r is `atan2` (`FUN_1404b3878`) of the
normalised cross and dot products (`FUN_140435625`: `atan2((((b.y*a.x) - (b.x*a.y))/|a|)/|b|, (((a.y*b.y) + (a.x*b.x))/|a|)/|b|)`, lengths `sqrt(y*y + x*x)`); `FUN_140435caa` accepts it if it lies in
[−1e-6, |ang| + 1e-6] directly or after adding 2·3.1415926535898. Inside: d = h, move to F.
Outside: d = |P − A| or |P − E| (unsigned, the smaller; A on ties), move to that end point.

### 4.5 Meshed master `FUN_14034fc56` (1790 bytes) / `FUN_140434952` (3097 bytes)

For each slave node: nearest master node of the loop, then the nearest boundary segment
(`FUN_1404345c0` mode 2 on a copy); best |d| kept (start 1e10 `DAT_140baf840`). The node is
in contact when `|d| <= tol` (signed d tested as `d <= tol and (d >= 0 or |d| <= tol)`); then
it is moved with mode 2 and the code rule of §4.3 applies. No 100·tol checks, only warning
1001. The corner and orientation handling inside `FUN_140434952` (neighbour rotations by
±1.5707950592041016, `DAT_140bb1558/60`, convexity test against 0.05000000074505806) is
summarised only; see the decompile.

### 4.6 Element check `FUN_14032e34e` (567 bytes, read fully)

Nodes n1..n4 of element e (4 ints). With xi, yi the moved coordinates:

```
A = ((y1-y4)*(x2-x3) - (y2-y3)*(x1-x4)) * 0.57735026918963
B =  (y2-y4)*(x1-x3) - (y1-y3)*(x2-x4)
C = ((y1-y2)*(x3-x4) - (y3-y4)*(x1-x2)) * (-0.57735026918963)
ok = (A + (B + C)) * 0.125 > 0          // (corrected by FP audit)
```

(constants `DAT_140baebe8` = 0.57735026918963, `DAT_140baebe0` = -0.57735026918963,
`DAT_140baebf0` = 0.125; the sum is evaluated as `A + (B + C)`: `B + C` first (`ADDSD XMM2,XMM3`
at `0x14032e544`), then `A + that` (`0x14032e548`) **[fact, listing; corrected by FP audit — the
decompile's `(A + B) + C` was wrong]** [listing-checked].)

### 4.7 Curve conversion `FUN_1402763d0(geo, 2)`

Converts a DIEGEO point list (type 1: x, y, fillet radius per point) to line/arc segments:
`FUN_1402765e0` drops consecutive duplicate points (|Δ| < 1e-6), `FUN_140345f5b` sets the
model length scale, `FUN_14032b34b` / `FUN_14032bba1` build lines and fillet arcs. Shared with
the 2D geometry code of OBJPOS **[inferred]**; summarised only.

## 5. 3D algorithm

> **Superseded in detail by two follow-up specs**: [GENCTC_3d_search.md](GENCTC_3d_search.md)
> (pipeline stages, master-surface build, locator library, MDSPS2_REC search and kernels) and
> [GENCTC_3d_decide.md](GENCTC_3d_decide.md) (per-node decision `FUN_140040d77`, write-back,
> messages). Corrections to this section from them: the 3D fallback tolerance uses the nodal area
> NR2 (L = sqrt(nodal area), ×0.2); for a plain INICTC/GENCTC the search is done only by
> MDSPS2_REC (`FUN_14006798b`), SRCNT2/4/5 change nothing; `DIEGEO.DAT` is not read, its presence
> writes one `DIE_00k.GEO` per partner (not `DIE_000.GEO`); `SKIP_PNTR.DAT` (contents unread)
> turns SRCNT4 off; `SRCNT2.DAT` holds two integers (defaults 5, 20); the per-object flags
> `DAT_140778684[obj]` are the cube entity number (always 1) and have no effect. 3-term sums
> compile as `(x + y) + z`; Ghidra prints them reversed.
>
> **Corrections after the oracle run** (details in the two follow-up specs and
> [oracle/ov-ctc.md](oracle/ov-ctc.md)): (a) "SRCNT2/4/5 change nothing" holds only while no
> two partners of the slave lie within `tol` of each other; with `tol` ≥ the die gap
> (UPSET_Brick: 15, case `d3d_two_masters` with `GENCTC 16`) partner–partner contact is
> established, SRCNT2 and SRCNT4 act, almost every die polygon is disabled and the result is
> completely different. (b) SYMGRP2 does build symmetry planes for slave faces with a fixed
> code 1 (x = 0 and y = 0 in UPSET_Brick); partner polygons lying in those planes are disabled
> (EI2 = 0) and never used for projection.

### 5.1 Driver `FUN_1401a6c80(model, tol)` (2689 bytes, read fully with listing)

1. `FUN_1401a7710(model)` builds the surface buckets of **all** objects (§5.2).
2. For each object slot O = 1..`count(model+0x1380)` (`FUN_1401d6ed0`): skip unless found,
   type ≠ 7, has mesh, type ≠ 5.
3. Tolerance (§3).
4. Partner list (calloc, `FUN_140256dc0`): for each pair with direction ≠ 0:
   direction 1 and `+0x168 == O` → add `+0x16c`; direction 2 and `+0x16c == O` → add
   `+0x168` (no duplicates, list order). Skip O if the list is empty.
5. Hand the list to Fortran: `FUN_1400511e4` copies it to `DAT_140771918[1..n]`, count to
   `DAT_14077867c`; `FUN_140082ed6` sets the instance flag `DAT_140778684[obj] = 1` for every
   partner and for O. These flags are never cleared **[fact]**.
6. Code-6 encoding: every BCCDEF code equal to 6 becomes `600 + BCCDFN` (`obj+0x998`) for
   the Fortran call, and every code equal to `600 + BCCDFN` afterwards is set back to 6.
7. `FUN_1400512aa(&O, &1, coords obj+0x8a8, bcc obj+0x990, &ndof model+0x1640, &1,
   &N obj+0x880, &nface obj+0xbf0, &1, &0, &0, &0, &tol)` (§5.3).
8. A second pass guarded by a constant `false` (`FUN_140055162` "Establish FEM solid
   Structure", `FUN_14001d99f` "Node inside itself") is dead code **[fact, listing 0x1401a72bd]**.
9. After the loop: `FUN_140225820` frees all buckets.

### 5.2 Buckets `FUN_1401a7710`

`FUN_140225640(3·nobj)` creates the cube table. Each object with a mesh or a rigid surface
(`obj+0x3990`) is inserted by `FUN_14004f56b`. `FUN_1401a7980` picks the source: the FE
boundary surface 0 (coords `obj+0x8a8`, faces `obj+0xbf8`, counts `obj+0x880`/`obj+0xbf0`,
4 nodes per face) when the object is a slave of some pair (and has a mesh), or is non-rigid
with a mesh, or is rigid without a rigid surface; otherwise the rigid surface
`obj+0x3c00[0]` (point and face arrays via `FUN_140005510`/`FUN_140006410`). Only boundary
surface 0 is used in 3D **[fact]**.

### 5.3 Fortran pipeline `FUN_1400512aa` (routine level)

| Order | Routine | Role (from strings and calls) |
|---|---|---|
| 1 | `FUN_14005c3a7` | polygon/element data of O |
| 2 | `FUN_140052f49` | corner nodes (`Info: Critical Corner Node Idenfied`) |
| 3 | `FUN_140029d47` | node-polygon links |
| 4 | `FUN_14002cc15` | surface normals |
| 5 | `FUN_14003b304` | contact search `SRCNT`: tolerance (§3), `SRCNT2` `FUN_14002cdee` per partner, `SRCNT4` `FUN_14002f091`, `SRCNT5` `FUN_140030d0e`, symmetry `FUN_14002a857` |
| 6 | `FUN_140088e99` | node real data |
| 7 | `FUN_14003fadc` | node classification (`MDSPS2_REC` kernel `FUN_14006798b`) |
| 8 | `FUN_14005190e` per node 1..N | final decision and write-back (§5.4) |

### 5.4 Per-node write-back `FUN_14005190e` / `FUN_140040d77`

The decision routine `FUN_140040d77` (14653 bytes, partly read) computes a signed distance d to
the nearest candidate partner polygon and, with mode 0, accepts the node when `|d| < tol`.
For each accepted contact the code `−partner` goes into the first zero component in the
order **z, y, x**. With two contacts within tol of the node it keeps **both codes and does not
move the node** (message `is inside of two Objects`, NEWEQ.MSG); otherwise it keeps the first
accepted partner (GENCTC_3d_decide.md §4.6.4; corrected after oracle run `v_two_masters_12`:
side nodes at z = 4 / 11 with `GENCTC 12` get `-1` in z and `-3` in y, or `-3` in x when y is
fixed, and stay in place; nodes with x and y fixed get only `-1`). For an accepted node
`FUN_14005190e` writes the 3 codes to `bcc` and moves the node to the projected point
(`DAT_1405c16f4` = 0 in initialised data, never written, so the move always happens).

### 5.5 3D messages

The unit at `0x14077a7bc` is set at start-up to 1 and connected to `NEWEQ.MSG` (deleted at
exit, `FUN_14001ce7b` / `FUN_14001d1bf`), so most 3D messages (`Node … Old coord … New
coord`, `is inside Object`, `is inside of two Objects`) are not visible. Unit 6 (stdout) is
used by `FUN_14004ad46` (`Info : Node separation is detected…`, `Contact condition is
changed`, only for nodes that already had contact codes), `FUN_14002f091`
(`Interference detected, KOBJ=`), and the `SRCNT2` diagnostic.

Optional files in the working directory change behaviour: `SRCNT2.DAT` (two integers,
default 5 and 20, read once per process), `SKIP_PNTR.DAT`, `DIEGEO.DAT` (if present the partner
geometry is written to `DIE_00k.GEO`, one file per partner k, e.g. `DIE_001.GEO` and
`DIE_003.GEO`; corrected after oracle run `s3d_diegeo_dat`). The oracle scratch directory has
none of them unless a case supplies them. See GENCTC_3d_search.md §11.

Output order (oracle): unit-6 lines are written unbuffered while the C++ side's stdout is
buffered, so in a captured transcript they appear **before** the C++ lines printed earlier by
the same command file, e.g. right after the `Keyword File = [ ? ] ?` prompt
(`d3d_two_masters`: ` Interference detected, KOBJ=  1`, `... KOBJ=  3`,
` Info: Checking Invisible definition`, then ` *** INFO: Reading Keyword File`).

## 6. Outputs and errors

| Item | 2D | 3D |
|---|---|---|
| BCCDEF `obj+0x990` | −M in x or y slot (§4.3 rule) | −partner in z, y, x order; codes 6 preserved |
| coordinates | slave nodes moved onto the master | slave nodes moved onto the master |
| other arrays | none | none on the C++ side |
| return | 0 (2 on read error) | 0 (2 on read error) |

No contact node list, `CNTINF` or pair data is written.

## 7. Helper table

| Address | Role | Size | Status |
|---|---|---|---|
| `FUN_1401a2e90` | 2D driver | 699 | read fully |
| `FUN_1401a3160` | 2D pair worker | 2198 | read fully |
| `FUN_140350f12` | 2D rigid master | 1990 | read fully |
| `FUN_140436a9e` | nearest segment | 1335 | read fully |
| `FUN_1404345c0` | line kernel | 914 | read fully, listing |
| `FUN_14043574f` | arc kernel | 1371 | read fully |
| `FUN_140435625`, `FUN_140435caa`, `FUN_14043556b` | angle helpers | 298/435/186 | read fully |
| `FUN_14034fc56`, `FUN_140434952` | meshed master | 1790/3097 | summarised |
| `FUN_14032e34e` | element check | 567 | read fully |
| `FUN_1403046a0`, `FUN_140202e00` | element size | 809/1455 | read fully |
| `FUN_1402763d0` and callees | curve conversion | | summarised |
| `FUN_1401a6c80`, `FUN_1401a7710`, `FUN_1401a7980`, `FUN_1401a7a30` | 3D driver, buckets | | read fully |
| `FUN_1400512aa` | 3D pipeline | 611 | read fully |
| `FUN_14003b304`, `FUN_14002cdee`, `FUN_14005190e`, `FUN_140040d77` | 3D search and write-back | | partly read |
| ≈150 further 3D Fortran / cube routines | | | not followed |
| `FUN_1404b3878` atan2, `FUN_1404b3b30` sincos, `FUN_140484644` sqrt | math library | | not followed |

## 8. Tests

`tests/commands/GENCTC/` (cases.json): 3D UPSET_Brick and UPSET_Tet with tolerance 0, 0.0001
and 1.0 after `INICTC 2`; 2D SPIKE_SI with 0, 0.0005, 0.05 and 1e-13 after `INICTC 1`;
GENCTC without INICTC; OBJPOS then INICTC then GENCTC (2D and 3D); a GENDB variant; a
non-numeric argument (automatic tolerance) and an empty argument line (reading error).
Oracle results: all of these reproduce the seed's codes and coordinates exactly (the seeds'
contact nodes lie on the dies), so they check the bookkeeping but not the tolerance values.
`cases_v_ov-ctc.json` adds discriminating cases: automatic tolerance brackets (`v_auto_*`,
`v_explicit025`, `v_spike_auto_*`), 2D warnings (`v_spike_push1004`, `v_spike_push05`), the
corrected OBJPOS line (`v_brick_objpos_fixed`), two masters without partner–partner contact
(`v_two_masters_12`), symmetry-plane polygons (`v_d0_*`), and the optional files with partner
contact (`v_two_masters_geo`, `_skip`, `_srcnt2`). Per-case verdicts:
[oracle/ov-ctc.md](oracle/ov-ctc.md).

## 9. Open items

1. 3D Fortran contact search: exact candidate selection, distance and projection order in
   `FUN_14002cdee`, `FUN_14002f091`, `FUN_140030d0e`, `FUN_14006798b`, `FUN_140040d77`.
   Bit-exact 3D needs a separate packet (≈160 routines).
2. ~~2D message unit `0x140b847a4`: no write found; assumed 0 (stderr). Check with the oracle.~~
   Resolved by the oracle: stderr, CR LF line ends (`v_spike_push1004`).
3. Math library (`atan2`, `sincos`): results must match the PathScale library bit for bit.
4. 2D curve conversion `FUN_14032b34b`: shared with OBJPOS geometry, not specified here.
5. `FUN_140434952` corner rules (meshed master) not transcribed.
