# GENCTC 3D: SRCNT4 (interference) subtree

Read from the listings `deform-rea-work/out/p677/lst/L_<addr>.txt` (FP order) and the decompiles
(control flow). Port: `port/defcmd/src/ctc3d/srcnt4/`. Refines GENCTC_3d_search.md §7.5.
Notation: `add(a,b)` etc. are SSE ops with the listing's dest first; `|v|` = `sqrt(add(z*z, add(y*y, x*x)))`.
EI2/NI4/ER1/NR1/NR2/NI2/EI1/EI6 are cube attributes (GENCTC_3d_search.md §3.3).

## 14002f091 SRCNT4(O, inst, bcc, ndof, N, nface, X, tol', p9, dbg, dobj, dinst, dpoly) **[fact]**

1. `OPEN(61,'SKIP_PNTR.DAT',STATUS='OLD',ERR=)`; opened → return (unit stays open). Return also
   when `DAT_1405c16fc == 1`. `nface` and `p9` are not used.
2. Box of O: `FUN_1404d1b1c(COMMON box of O (2 points), boxO, tol', 2, (1,2))`; centre
   `cO = (mul(add(min,max),0.5)` for x, y; `mul(0.5, add(min,max))` for z).
3. Per partner P = `DAT_140771918[1..DAT_14077867c]` (count read once): box of P the same way,
   centre cP; if boxes overlap (`FUN_1404d1ed7`):
   * box B of P's points whose NI4 contact code (first 3 entries) is negative and ≠ −O
     (`140030ae1`, init 1e4/−1e4); `cnt` counts **all** negative codes, −O included.
   * `cnt = 0` → next partner. Else B widened by `mul(tol',0.5)` (mode −1 call; it re-adds the last
     fetched point, `DAT_14064f460`, which is stale when only −O codes were seen).
   * `WRITE(6,*) 'Interference detected, KOBJ=', P` when `DAT_14075bc84 == 0` (always).
   * `dir = cP − centre(B)`; if `|dir| ≤ 9.999999717180685e-10` (float 1e-9) then `dir = cP − cO`;
     if that is also ≤ it → next partner (NaN goes on). `dir /= |dir|`.
   * Box query of **P's** cube with boxO (`FUN_140228c60` + `FUN_14022a5c0`): unique list L.
   * `140032ce4` filters L; `140032a3e` → `start`; L freed; `start = 0` → next partner.
   * `140033d76`: EI2 := −2000 by flood fill from `start`.
   * `tol2 = mul(DAT_140767500[O], 2.0)`; `14003329f` re-validates; if it re-validated anything:
     `WRITE(6,*) 'Info: Checking Invisible definition'` and `140034138` (EI2 := 1 flood fill).
   * Debug lines `In0/In1/In2- SRCNT4` only when `dbg == 1 && dobj == P` (never in GENCTC).

## Helpers **[fact]**

* `140030ae1(X, box, margin, n, idx, mode)`: mode 1 → box = (1e4,1e4,1e4, −1e4,−1e4,−1e4).
  Else for k = |idx(i)| > 0: min if `min > v`, max if `v > max` (NaN never replaces); mode ≠ 0:
  `min = sub(min, m)`, `max = add(m, max)`.
* `140032ce4(P, inst, L, n, boxO, tol')`: for each L(i) with EI2 = 1: nodes nv (last dropped when
  `|conn(nv)| == |conn(1)|`), sum `add(p, sum)`, NI4 count `nneg` of nodes with a negative code
  among NI4(1..3); centroid `div(sum, (double)nv)`; box ± tol'; L(i) = 0 if `nneg < 2` or no
  overlap with boxO. Then L is compacted.
* `140032a3e(P, inst, L, n, start, npoly, flag)`: flag(L(i)) = 1 if EI2 = 1; start = first j with
  flag(j) = 0 and EI2(j) = 1 (0 if none). So the fill starts **outside** the interference list.
* `140033d76(P, inst, npoly, start, dir, value, flag)`: flag(j) = 1 for EI2 ≠ 1; flag(start) = 1;
  stack loop: EI2(start) := value (−2000), `140039ef9(mode 1)`, pop.
* `140039ef9(P, inst, poly, S, cnt, flag, dir, mode)`: EI1 (neighbours), conn (nv−1 when
  `conn(nv) == conn(1)`, no abs), EI6, ER1. For edge k with neighbour nb ≠ 0, flag(nb) ≠ 1:
  EI2(nb) ∈ {0, −1000} or ≥ 600 → flag; mode 1 and EI2 < 0 → flag; mode 0 → push if EI6 entry
  `EI6(2k−1)` is 0 else flag; otherwise (mode 1, EI2 ∈ [1,599]) `dot = add(d3·n3, add(d2·n2, d1·n1))`
  with ER1(nb); `dot < −0.10000000149011612` → skip (not flagged), else (NaN too) push. The push
  path also fetches NI4 of the edge's two nodes and calls `14003a4be`, and computes ER1·ER1, but
  the results are not used. Push: `cnt += 1; flag(nb) = 1; S(cnt) = nb`.
* `14003329f(P, inst, O, Oinst, X, npoly, N, tol2, boxO, found)`: for polygons j of P with
  EI2 = −2000 and last node ≥ 0: box of its nodes ± tol2; if it overlaps boxO: centroid
  `add(div(p, (double)(float)nv), c)`; for each O node i with `NR1(i)·n_j < −0.1`:
  `d = X(i) − (cz, cz, cz)` (**the binary subtracts the centroid's z from all three components**,
  140033c3f-140033c49), `cos = div(d·n_j, |d|)`; `cos > 0.10000000149011612` → EI2(j) := 1, next
  polygon. Then (overlap or not) per vertex v: nearest O point q (`FUN_14022d770`); for each NI2
  neighbour r of q: `u = unit(X(r) − v)` (`FUN_1404d68dd`), `u·n_j > 0` → EI2(j) := 1, next
  polygon; then if `sqrt(NR2(q)) > dist(v,q)` and `NR1(q)·n_j < −0.5` → EI2(j) := 1 (loop goes on).
  `found` = last re-validated polygon.
* `140034138(P, inst, npoly, O, Oinst, N, dir, value, flag, bcc, ndof)`: flag := 0; first O node
  whose BC codes bcc(1..3, i) hold exactly one negative code and it is −P: MDSPS2_REC on P (third
  argument 3, start distance 1e5, direction `DAT_1405c6850`, mode 1); hit polygon p with flag 0:
  EI2 := value (1) by flood fill with `140039ef9(mode 0)`, then return (only one node is used).
* `14003a4be(a, b, found)`: found = 1 if some a(i) = b(j) < 0 (i, j ≤ 3).

## What SRCNT4 writes

EI2 of the **partner** P only (never O): −2000 on the region reached from the first valid
polygon outside the filtered interference list (fill bounded by EI2 ≠ 1 polygons, the listed
polygons and faces turned more than ~96° against `dir`), then 1 again for −2000 polygons facing
O (14003329f) and for the edge-connected region (EI6 = 0 edges) around the MDSPS2 hit of the first
O node carrying the code −P. NI4/NR*/coordinates are only read. Unit 6 gets the two messages.

## Corrections to GENCTC_3d_search.md §7.5

* The box query and all EI2 changes are on **P's** cube (query box = O's box), not O's polygons.
* `cnt > 0` also counts codes −O; the box B then comes from the stale `DAT_14064f460`.
* 14003329f uses `X(i) − cz` for all three components [fact, 140033c3f-140033c49] and divides by
  `(double)(float)nv` [fact, 140033a3e].
