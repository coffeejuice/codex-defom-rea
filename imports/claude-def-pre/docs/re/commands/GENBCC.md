# GENBCC (id 295, group 6, high switch `case 8` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1961-1973, dispatcher
listing `0x14037d376..0x14037d3fa`, closures `out/p67/genbcc.c` and `out/p67/gencmds/genbcc2.c`,
listings of `FUN_1402069a0`, `FUN_140206d50`, `FUN_1403087a0`, `FUN_140305520`). Shared mechanics:
[_common.md](_common.md). Facts are marked **[fact]**, deductions **[inferred]**.

## 1. Summary

Regenerates the default boundary conditions of one **2D** object: clears its thermal and
deformation BC arrays, then

* axisymmetric models (GEOTYP 1 or 3): fixes the radial velocity of the nodes on the symmetry
  axis (`BCCDEF = 1`, value 0) and sets heat exchange with the environment (`ECCTMP = 2`) on every
  boundary edge that does not lie on the axis;
* other 2D models: `ECCTMP = 2` on every boundary edge, no velocity BC.

For a **3D object it does nothing** (`obj+0x39c0 == 3` → return 0) **[fact]**. It is C++ code;
no Fortran (`genbcc2.f`) routine is called on this path **[fact]**. Not used in the corpus.

```
GENBCC <objNo>
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the GENBCC line | `FUN_140413de0(f, &objNo, 0)` | `fgets(0x400)`, `,` → space, `sscanf("%i")`; result **stored** in the dispatcher status |

* Read failure (EOF, blank line) → return **2** ("Reading Error") **[fact]**.
* `obj = FUN_1401d6ea0(model, objNo)`; NULL → return **2** **[fact]**.
* Then `FUN_1401ac6c0(model, &objNo)` (looks the object up again, calls `FUN_1402066e0(obj)`); its
  result is ignored and the case returns 0 **[fact]**. One line consumed.

## 3. Fields (2D object `obj`, model `m = obj+15000`)

| Field | Meaning | GENBCC |
|---|---|---|
| `obj+0x39c0` | object dimension, copied from `m+0x1c` at construction (`FUN_140199af0`) | 3 → no-op |
| `obj+0x880`, `obj+0x884`, `obj+0x88c` | nodes, elements, boundary edge count NBE, recomputed by `FUN_140165d30` inside the NBF helper: 2D = sum of (`cnt_i` − 1) over the loops (`FUN_14017ebc0`), 3D = sum of the loop face counts `+0x10` (corrected after oracle run) | read |
| `obj+0x8a8` | node coordinates (2 per node in 2D) | read |
| `obj+0x9e8` | element connectivity, 4 ints per element | read (edge length) |
| `obj+0xbd8`, `obj+0xbe0` | boundary loop count, loop records (0x30 bytes: `+0` count, `+8` node list) | read |
| `obj+0x8b8` URZ, `obj+0x8c0` FRZ | node velocity / force BC values (widths `m+0x1600`, `m+0x1604`) | URZ written |
| `obj+0x990` BCCDEF, `obj+0x998` BCCDFN | node BC code / function number (widths `m+0x1640`, `m+0x1644`) | written |
| `obj+0x9a0` BCCTMP, `obj+0x9a8` BCCTFN | node thermal BC code / function | cleared |
| `obj+0xae8` ECCDEF, `obj+0xaf0` ECDEFN | edge deformation BC code / function | cleared |
| `obj+0xb00` ECCTMP, `obj+0xb08` ECTMFN | edge thermal BC code / function | cleared, then set |
| `obj+0x2f40` | LOCTMP window list | emptied |
| `m+0x850` | GEOTYP (2D) | read |

Names from `objarrays.md` / `object.md` (branch `claude/project-thread-bz5k5e`).

## 4. Algorithm

`FUN_1402066e0(obj)`: `if (obj+0x39c0 == 3) return 0;` else `FUN_1402069a0(obj)` **[fact]**.

`FUN_1402069a0(obj)` **[fact]**:

1. `A = calloc(NBE, 4)`, `B = calloc(NBE, 4)` (`FUN_140256dc0`, NULL when `NBE == 0`);
   `nA = nB = 0`. Widths `wD = m+0x1640`, `wF = m+0x1644`, `wU = m+0x1600`, `wR = m+0x1604`.
2. Thermal reset `FUN_140204d20`: for every node `i`: `if (BCCTMP[i] > 0) BCCTFN[i] = 0;
   BCCTMP[i] = 0`. If ECCTMP and ECTMFN exist: for every edge slot `j < NBE`:
   `if (ECCTMP[j] > 0) ECTMFN[j] = 0; ECCTMP[j] = 0`. Empty the LOCTMP list `obj+0x2f40`
   (`FUN_1402699c0`: destroy every item, count 0).
3. Deformation reset `FUN_140204180` (2D branch, components `c = 0, 1` only):
   ```
   code = BCCDEF[i*wD + c]
   if (code == 0) { if (fabs(FRZ[i*wR + c]) > 1e-6) BCCDFN[i*wF + c] = 0; }
   else           { BCCDFN[i*wF + c] = 0; if (code == 1) URZ[i*wU + c] = 0.0; }
   if (code >= 0) BCCDEF[i*wD + c] = 0;     // negative codes are kept
   ```
   then `ECCDEF[j] = ECDEFN[j] = 0` for `j < NBE` (no NULL test). The 3D branch of this helper
   (3 components, stride 3, negative codes also cleared) is not reachable from GENBCC.
4. GEOTYP `g = m+0x850` (`FUN_140212420` → `FUN_1401d6d90`):
   * `g == 1` or `g == 3` → **axis search** `FUN_140206d50` (step 5);
   * otherwise `FUN_140206f60`: `B = 1, 2, ..., NBE`, `nB = NBE`.
5. Axis search `FUN_140206d50` **[fact]**:
   * `hmin = FUN_1403046a0(obj)` (2D): 0 if no mesh / no elements / no connectivity / no
     coordinates; else start `1e29` and over every element `e` and its edges `k -> k+1`
     (`k = 0..3`, edge 3 → 0; when **element 0** has `node0 == node3` (triangle stored as quad)
     the edge `3 -> 0` is skipped for **all** elements):
     `d = P[a] - P[b]` (`FUN_1403089b0`, `a` = node k, `b` = node k+1; `dx = ax - bx`,
     `dy = ay - by`), `len = sqrt(dx*dx + dy*dy)` (`FUN_1403087a0`: `MULSD`, `MULSD`, `ADDSD`;
     then CRT `sqrt`), `if (len <= hmin) hmin = len`.
   * `tol = 0.05 * hmin` (constant `0x140524ce0`, `MULSD`). [listing-checked]
   * `prevOnAxis = curOnAxis = false`. For `l = 1 .. NBE`:
     `slot = FUN_1403054e0(obj, &l)`; `(n1, n2) = FUN_140200e20(obj, slot)`;
     `p1 = FUN_140163490(obj, n1)`, `p2 = FUN_140163490(obj, n2)` (coordinate pointers, NULL for
     node numbers outside `1..obj+0x880` or no mesh). If both are non-NULL:
     ```
     prevOnAxis = curOnAxis; curOnAxis = false;
     if (l == 1 && fabs(p1[0]) < tol) { prevOnAxis = true; A[nA++] = n1; }
     if (fabs(p2[0]) < tol)           { curOnAxis  = true; A[nA++] = n2; }
     if (!prevOnAxis || !curOnAxis)     B[nB++] = l;            // edge not on the axis
     ```
     The test of `n1` is done only for the first edge; later edges reuse the previous edge's
     `n2` result (assumes a consecutive loop) **[fact]**. Comparisons are strict (`<`), on the
     first coordinate (r). Nodes can enter `A` twice (harmless, see step 6).
6. If `nA > 0`: `FUN_140206fd0(obj, A, nA, BCCDEF, BCCDFN, URZ, dof 0, code 1, fn 0, value 0.0,
   &wD, &wF, mode 0, ...)` → `FUN_140207250` (2D path) for every node `n` of `A`
   (`row = n - 1`) **[fact]**:
   * if `BCCDEF[row*wD+0] == 3`, `== 4` or `< 0`, or `BCCDFN[row*wF+0] < 0`: move the existing
     code to the first component `j != 0` with `BCCDEF == 0` and `BCCDFN >= 0`
     (`BCCDEF[row*wD+j] = BCCDEF[row*wD+0]`, and `BCCDFN[row*wF+j] = BCCDFN[row*wF+0]` when the
     latter is negative). After step 3 only negative (kept) codes can trigger this.
   * `BCCDEF[row*wD+0] = 1`, `BCCDFN[row*wF+0] = 0`, `URZ[row*wD+0] = 0.0` (the URZ index uses
     the BCCDEF width `wD`) **[fact]**.
7. If `nB > 0`: `FUN_1402084c0(obj, 2, B, nB, ECCTMP, 0, 0, 2, 0, 0, mode 0)` → `FUN_1402086c0`
   2D path → `FUN_140305d70(obj, ECCTMP, B, &nB, &2)`: for each `l` of `B`: if `l < 1` or
   `l > NBE` print to **stderr** `"EdgeNo(%d) is out of range[1 -- %d]\n"` (l, NBE) and skip; else
   `slot = FUN_1403054e0(obj, &l)`; if `slot < 0` or `slot > NBF` (`FUN_140165170`) print to
   stderr `"ECC array index(%d) is out of range[0 -- %d]\n"` (slot, NBF) — **and still store**
   `ECCTMP[slot] = 2` **[fact]**.
8. Free `A`, `B`; return 1 (ignored).

Edge → slot (`FUN_140305520(&l, &nLoops, loops)`) **[fact]**: `first = 1`; for loop `i`:
`end = first - 1 + cnt_i`; if `first <= l < end` return `l - 1 + 2*i`; else `first = end`.
No loop → **-1**. Slot → nodes (`FUN_140200ec0`): `start = 0`; for loop `i`: if
`start <= slot < start + cnt_i` return `nodes_i[slot-start]`, `nodes_i[slot-start+1]`; else
`start += cnt_i + 1`; none → (0, 0). Loop `i` has `cnt_i` nodes with the first node repeated
at the end, i.e. `cnt_i - 1` edges, and `NBE = sum (cnt_i - 1)` (`FUN_140165d30`: 2D
`FUN_14017ebc0` returns `cnt - 1`) **[fact]**. So `l = 1..NBE` maps onto every real edge slot
(loop `i` occupies slots `sum_{j<i}(cnt_j + 1) ..`, two unused slots between loops; NBF =
`NBE - 2 + 2*nLoops` in 2D) and **no edge number gives slot -1**: the range messages of step 7
are not printed for a consistent boundary (corrected after oracle run: the earlier claim
"`NBE = sum cnt_i`, the last `nLoops` edges give slot -1, stderr `ECC array index(-1)` and a
write to `ECCTMP[-1]`" was wrong; `GENBCC/plane` has an empty stderr and every ECCTMP slot of
object 1 set to 2, KEY line `ECCTMP 1 0 2`). The out-of-range store in `FUN_140305d70` remains
a latent bug for inconsistent data; a port must not write out of bounds (the p67-5 port prints the
message and skips the store). Note also that step 2 clears only the first `NBE` ECCTMP/ECTMFN
slots, not the `2*nLoops - 1` further ones of a multi-loop object.

## 5. Outputs

* stdout: nothing. stderr: nothing for a consistent boundary (the step 7 range messages need an
  edge number outside the loops; corrected after oracle run).
* KEY view: when every slot ends with code 2 (non-axisymmetric path, or an object without axis
  nodes) the ECCTMP writer uses its uniform form `ECCTMP <n> 0 2` (ECCTMP.md)
* Object arrays as described; return code 0, or 2 for a read error / missing object.
* 3D object: no change at all.

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140413de0` | `%i` reader | | group2-helpers.md |
| `FUN_1401ac6c0`, `FUN_1402066e0`, `FUN_140158ca0` | entry, 3D test (`obj+0x39c0 == 3`) | 63 / 70 / 51 | read fully |
| `FUN_1402069a0` | 2D body | 923 | read fully (listing for the call arguments) |
| `FUN_140204d20` | thermal reset | 283 | read fully |
| `FUN_140204180` | deformation reset | 840 | read fully |
| `FUN_140212420`, `FUN_1401d6d90` | GEOTYP `m+0x850` | 50 / 17 | read fully |
| `FUN_140206f60` | all edges list | 103 | read fully |
| `FUN_140206d50` | axis node / edge search | 510 | read fully, FP from listing |
| `FUN_140202a20`, `FUN_1403046a0`, `FUN_1403089b0`, `FUN_1403087e0`/`FUN_1403087a0` | min element edge length (2D) | | read fully |
| `FUN_1403054e0`/`FUN_140305520`, `FUN_140200e20`/`FUN_140200ec0` | edge → slot → nodes | | read fully |
| `FUN_140165170`, `FUN_140165d30`, `FUN_14017ebc0` | NBF; NBE recompute (2D `cnt - 1` per loop, 3D face count) | 184 / 120 / 83 | read fully (after oracle run) |
| `FUN_140163490` | node coordinate pointer | 166 | read fully |
| `FUN_140206fd0`, `FUN_140207250` | set node BC code (with code relocation) | 632 / 3312 | read fully for this call (2D, code 1, fn 0) |
| `FUN_1402084c0`, `FUN_1402086c0`, `FUN_140305d70` | set edge code | 503 / 593 / 280 | read fully for this call |
| `FUN_1401809c0`, `FUN_1401808d0` | URZ / coordinate accessors (`FUN_14017ffe0` allocate-if-missing, `FUN_1401d48d0` layout sync) | | summarised; `FUN_1401d48d0` (3392 bytes) not followed |
| `FUN_1402699c0` | empty a window list | 165 | read fully |

## 7. Open items

* `FUN_1401d48d0` (called by the URZ accessor) is not followed; it is assumed to have no visible
  effect on a consistent model.
* ~~The exact meaning of loop `cnt`~~ settled: `cnt` includes the repeated closing node and
  `NBE = sum (cnt - 1)` (`FUN_140165d30`, `FUN_14017ebc0`); no slot -1 (oracle `GENBCC/plane`,
  empty stderr).
* `GEOTYP` changed after the mesh was read (as `torsion.KEY` does with `GEOTYP 3`) changes the
  BCCDEF width `m+0x1640` from 2 to 3 without re-laying out the arrays: the KEY writer then shows
  re-interpreted rows and GENBCC indexes `i*3+c` past the 2-wide arrays (heap garbage appeared in
  object 2's BCCDEF in `GENBCC/torsion` but not in the `GEOTYP 3`-only control
  `GENBCC/v_torsion_ctl`). This is a GEOTYP/width issue, not GENBCC logic; a GEOTYP 3 test needs a
  seed read with GEOTYP 3 (3-component BCCDEF rows).
* The physical meaning of ECCTMP code 2 (heat exchange with environment) and BCCDEF code 1
  (fixed velocity) is taken from the GUI conventions **[inferred]**.

## 8. Tests

`tests/commands/GENBCC/` (2D seed `SPIKE_SI.KEY`, GEOTYP 1, three meshed objects; 3D seed
`UPSET_Brick.KEY`): `axisym.KEY` (object 1, axis search), `axisym_obj2.KEY` (object 2),
`plane.KEY` (`GEOTYP 2` first: all edges, stderr range message), `torsion.KEY` (`GEOTYP 3`: axis
search), `brick3d.KEY` (3D: no change), `missing.KEY` (object 9 → Reading Error),
`empty.KEY` (no number → Reading Error), and `gendb2d.KEY` (axisymmetric + GENDB).

Oracle results (2026-10-21, `docs/re/commands/oracle/ov-misc.md`): `axisym` leaves OUT.KEY equal
to the seed (SPIKE_SI already carries exactly the GENBCC defaults for object 1: 31 axis nodes with
BCCDEF x = 1, 56 ECCTMP edges); `axisym_obj2` gives `ECCTMP 2 0 2` (object 2 has no node with
|r| < tol, every edge code 2); `plane` clears the 31 x = 1 codes (negative codes kept) and sets
all ECCTMP slots to 2, no stderr. Controls `v_plane_ctl` / `v_torsion_ctl`
(`cases_v_ov-misc.json`) run the GEOTYP line alone.
