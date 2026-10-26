# GENCTC 3D, part 2: per-node decision and write-back

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only. Decompiles `out/p67/contact/g3d.c`
(deform-rea-work), listings and xrefs in `out/p67/ctc3d-decide/` (`l_<addr>.txt`,
`xrefs*.txt`, `callers.c`). Parent spec: [GENCTC.md](GENCTC.md) (§5). Upstream stages
(surface build, cube attributes, contact search, the projection kernel `FUN_14006798b`):
[GENCTC_3d_search.md](GENCTC_3d_search.md) (written in parallel by the ctc3d-search
worker; this file links to it for every routine it owns).

Scope: the loop `for n = 1..N: FUN_14005190e(n)` at the end of `FUN_1400512aa` (GENCTC.md
§5.3, step 8) and everything it calls, except `FUN_14006798b` (and its callees),
`FUN_14003a978`, `FUN_14004f363`, `FUN_1400702c8`, `FUN_1404d203b` and the cfnCUBE
library, which are specified in GENCTC_3d_search.md.

Facts are marked **[fact]** (read in code; floating-point order checked in the listing where
stated), deductions **[inferred]**.

## 1. Summary

For each node n (1..N, all nodes of the slave O, not only surface nodes) `FUN_14005190e`:

1. builds the node's constraint matrix and code/polygon table from its BCCDEF codes
   (fixed code 1 and symmetry codes 600+k), `FUN_14004946f`;
2. re-checks contacts the node already has (negative codes): keeps them and projects the node
   onto that partner again, moves the contact to an adjacent object, detects separation, or
   releases the code ("large distance"), `FUN_140049b2d`;
3. for a node with exactly two contact codes, may switch the contact polygon,
   `FUN_14004522c`;
4. writes the (possibly re-projected) position back to the coordinates;
5. decides new contacts with every partner object, `FUN_140040d77`: a partner is accepted when
   the signed distance d to its nearest polygon satisfies `|d| < tol`; the code `-partner`
   goes into the first zero component in the order **z, y, x**;
6. if at least one contact is accepted: corrects the projected point for fixed/symmetry
   directions (`FUN_14005266d`), writes the three codes to BCCDEF and moves the node.

The only results visible after GENCTC are BCCDEF `obj+0x990` (through the `bcc` pointer)
and the coordinates `obj+0x8a8` (through `coords`); everything else is cube scratch data
freed after the call (GENCTC.md §5.1 step 9) **[fact]**.

## 2. Call interface

`FUN_1400512aa` calls, for n = 1..N (`DAT_140676708` loop counter, `DAT_14067670c` = n):

```
FUN_14005190e(&O, &ent, coords, bcc, &ndof, &n, &move, &mode, &dbg, &flag, &tol)
```

| Arg | Driver value (`FUN_1401a6c80`) | Use |
|---|---|---|
| `&O` | `obj+8` (object number) | slave object number for the cube calls |
| `&ent` | `&1` (driver local `local_94[1]`) | cube entity (surface) number; always 1 |
| `coords` | `obj+0x8a8` (N×3 double, node n at `coords+24(n-1)`) | read and written |
| `bcc` | `obj+0x990` (N×ndof int, node n at `bcc+4·ndof·(n-1)`) | read and written; code 6 already encoded as `600+BCCDFN` |
| `&ndof` | `model+0x1640` (3 in 3D) | row stride of `bcc` |
| `&move` | `&1` | 1 = write coordinates (always) |
| `&mode` | `&0` | decision mode `p12` of `FUN_140040d77`; 0 in GENCTC |
| `&dbg` | `&0` | debug flag; set to 0 at entry and never set to 1 in this path, so the `Before IODIE2` / `After IODIE2` dumps are dead **[fact]** (14006798b receives it as its last argument; per GENCTC_3d_search.md it does not set it) |
| `&flag` | `&0` | set to 0 at entry, to −1 when a contact is written; not read by the driver **[fact]** |
| `&tol` | driver tolerance (GENCTC.md §3) | contact tolerance |

Other callers of `FUN_14005190e` (not GENCTC): `FUN_1403722fc` (mode `DAT_1407066c8`),
`FUN_140087c32` / `FUN_140086598` (mode −1; no static callers) **[fact, callers.c]**.

All Fortran routines here keep their locals in static storage (`DAT_14066xxxx`); values
survive between nodes and between GENCTC calls. Where a stale value can reach an output it
is said below.

## 3. Inputs from the search stage

Everything below is produced upstream (stages 1-7 of `FUN_1400512aa`, GENCTC_3d_search.md
§3 "Cube attribute map") or by the C++ driver. Cube access: `NI`/`NR` = node int/real data,
`EI`/`ER` = polygon int/real data, `EC` = polygon connectivity (`FUN_140227e00`, 1-based node
numbers, 4 entries, degenerate quad `|c1| == |c4|` = triangle), node coordinates
`FUN_140227fc0`. A Get on a field that was never initialised returns count 0 and leaves the
buffer untouched.

### 3.1 Slave node data (object O, entity 1, node n)

| Field | Size | Read by | Meaning used here |
|---|---|---|---|
| NI1 | list | 140049b2d (via 14004ce7a) | polygons containing the node |
| NI2 | list + count | 140040d77 | neighbour nodes (sharing a polygon); count 0 ⇒ node not on the surface ⇒ skipped |
| NI4 | 6 int | all | `code[1..3]`, `poly[1..3]`; **re-initialised** for every node by 14004946f (§4.1), then read/written here |
| NI5 | 1 int | 14004e10c | feature class (1, 3 tested) |
| NR1 | 4 dbl | all | unit node normal `nrm` + [4] convexity value; 14004e10c may rewrite it |
| NR2 | 1 dbl | 14005190e | nodal area; `L = sqrt(NR2)` is the node length scale |
| NR4 | 9 dbl | all | 3 rows (one per direction) of constraint / contact normals; created by 14004946f (no upstream writer) |
| NR5 | 2 dbl | 140049b2d | (max cos, min cos) of normal vs. neighbour directions |

### 3.2 Partner object data (object p, entity `e = DAT_140778684[p]`)

| Field | Use |
|---|---|
| EC | polygon nodes (edge/corner tests) |
| EI1 | `[k]` = neighbour polygon across edge k |
| EI2 | polygon status: 1 valid, 0 disabled, −1000 zero area/invisible, 2 symmetry polygon, −2000 interference, −k in contact with object k |
| EI5, EI6 | topology lists (14005ee88, 1400446b4, 14004522c) |
| ER1 | unit polygon normal |
| ER2 | polygon area (weights in 14004ce7a) |
| node NR1 | normal of a partner node (14004ad46 path) |
| node NI1, NI5, NI7 | NI7 = 2 ints: `[0] == 2` feature/boundary-edge node, `[1]` object across that edge |
| coordinates | `FUN_140227fc0` |

### 3.3 Fortran COMMON / globals

| Address | Meaning | Writer |
|---|---|---|
| `DAT_14077867c`, `DAT_140771918[1..n]` | partner count and list (pair order, GENCTC.md §5.1 step 4) | `FUN_1400511e4` (driver) |
| `DAT_140778684[obj + 100·?]` | cube entity of an object; GENCTC writes 1 for O and every partner (`FUN_140082ed6`), bucket insert writes the same; never cleared. Since the value is always 1 the persistence has **no effect** between GENCTC calls **[fact]** | driver, 14004f56b |
| `DAT_140772728[ent·100 + p]` | partner skipped when == 1 | none found (0) |
| `DAT_14076b6a8[p·3 + ent·300]` | partner node count, must be > 0 | search stage |
| `DAT_14076e588[same]` | partner polygon count, must be ≠ 0; passed to 14006798b (arg 4) | search stage |
| `DAT_14075a9b8 + 8·(ent·600 + p·6)` | partner bounding box `[xmin,ymin,zmin,xmax,ymax,zmax]` | search stage |
| `DAT_14075bc70`, `DAT_14075bc74`, `DAT_14075bc5c` | node→object pointer table; only read, never written ⇒ disabled **[fact, xrefs1.txt]** | — |
| `DAT_14076abb8`, `DAT_14076aa24[k]`, `DAT_14076a0a8+8(c+3k)`, `DAT_140769750+24k` | symmetry planes: count, code (600+k), unit normal, point. Written only by `FUN_14008616f`/`FUN_140082ef0`/`FUN_140083e2d`, which have no static callers; 0 planes in DEF_PRE per GENCTC_3d_search.md **[inferred]** | — |
| `DAT_14077a7bc` | message unit (1 = `NEWEQ.MSG`, GENCTC.md §5.5) | start-up |

### 3.4 Projection kernel `FUN_14006798b` as called here

`FUN_14006798b(&p, &e, &3, &npoly, P, lc, &poly, &d, &st, Q, dir, &flag, &dbg)` —
specified in GENCTC_3d_search.md. Outputs used: `lc` = local coordinates (ξ, η) of the foot
point in the polygon, `poly` (> 0 found, 0 none), `d` = signed distance (the input value is
ignored by the kernel), `st` (1 found, 0 not in this pass, −1 nothing found), `Q` = projected
point. Inputs: `dir` = preferred direction (zeros `DAT_1405c6c30` / `DAT_1405c70b0` /
`DAT_1405c70d0`, or the node normal + offset in 140049b2d), `flag` = the kernel's `mode`
(GENCTC_3d_search.md §9.2): **0** → candidates are polygons with EI2 ∉ {0, 2, −1000}, i.e. valid
(1), interference (−2000) and in-contact (−k) polygons; **−O** → only EI2 ≥ 0 polygons that are
not 0/2/−1000 (so EI2 = 1) plus those with EI2 == −O. (corrected after oracle run: the earlier
"polygons with EI2 == flag are also valid" read flag 0 as admitting disabled polygons; it does
not.)

## 4. Algorithm

### 4.1 `FUN_14005190e` (3423 bytes, read fully with listing)

```
P := coords[n]                                    (DAT_140676720..730)
FUN_14004946f(P, O, ent, bcc, ndof, n, P, dbg, flag)        §4.2
L  := sqrt(NR2(n))                                (SQRTSD, DAT_140676750)
FUN_140049b2d(P, O, ent, bcc, ndof, n, P, &L, dbg, flag, &sep)   §4.3 (may move P, change bcc)
tolS := tol;  if !(1e-9f < tol): tolS := L        (DAT_140bad530 = 9.999999717180685e-10; NaN also → L)
tol2 := tolS * 0.2f                               (DAT_140bada60 = 0.20000000298023224)
FUN_14004522c(O, ent, coords[n] (old position), bcc[n], n, &f, &tol2)   §4.5
if move == 1: coords[n] := P
FUN_140040d77(P, O, ent, bcc, ndof, n, dbg, flag, &tolS, Pn, codes[3], mode, &nacc, &sep)  §4.6
if move == 1 and nacc != 0:
    FUN_14005266d(O, ent, coords[n], Pn, codes, n)          §4.7 (corrects Pn)
    flag := -1
    bcc[n] := (codes[1], codes[2], codes[3])                (stored x, z, y order; no effect)
    WRITE(unit 7a7bc, 1001) n, coords[n] (x,y,z), Pn (x,y,z)
    if DAT_1405c16f4 == 0 (always, only read): coords[n] := Pn
```

Notes [fact]: `sep` (`DAT_140676758`) is set to 0 by 14004ad46 and receives the object of a
detected separation (§4.4); 140040d77 then skips that partner. `tolS` is per node: with
`tol <= 1e-9` every node uses its own `L` (the driver normally passes a positive tol, see
GENCTC.md §3). Message 1001: `1001('Node ',I7,' Old coord. ',3F15.7,/,12X,' New coord. ',3F15.7)`
on unit `DAT_14077a7bc` (NEWEQ.MSG, invisible).

### 4.2 `FUN_14004946f`: constraint rows (1726 bytes, read fully, listing)

```
M := 0 (3x3, row r = direction r); I := 0 (6 ints)
Init NR4(n) := M (9), Init NI4(n) := I (6)               // always, for every node
nrm := NR1(n)
if |c1|+|c2|+|c3| != 0 (c = bcc[n]):
  any := 0
  for r = 1..3:
    if c[r] == 1:    s := (e_r·nrm < 0) ? +1.0 : -1.0;  M[r] := s·e_r;  I.code[r] := 1; any := 1
                     (e_r = row r of the identity table DAT_1405c7020)
    elif c[r] > 600: any := 1; for k = 1..DAT_14076abb8 with DAT_14076aa24[k] == c[r]:
                        I.code[r] := c[r]; M[r] := -N_k (sign flip by XOR); I.poly[r] := k
  Init NR4(n) := M, Init NI4(n) := I
  if any: FUN_14004e10c(O, ent, n, bcc[n])
```

`FUN_14004e10c` (1230 bytes, read fully): `cls := NI5(n)`; if cls == 0 return. `nrm := NR1`,
`M := NR4`. cls 1: k := last r with `c[r] >= 600` or `c[r] == 1`; `t := cross(nrm, M[k])`
then `nrm := cross(M[k], t)`, both with `FUN_1404cce5e` (§4.8). cls 3: if exactly two
constrained directions a < b: `t := cross(M[a], M[b])` (1404cce5e),
`s := t·nrm`, `nrm := (s < 0 ? -1 : +1)·t`. Then `nrm[r] := 0` for every r with `c[r] == 1`;
`Init NR1(n) := nrm` (first 3 values; count 1 means one record of 4). Dot products as in
§4.8.

### 4.3 `FUN_140049b2d`: existing contacts (4633 bytes, read fully, listing)

Runs only if some `bcc[n][r] < 0` **[fact]**. Normally not reached after `INICTC` (codes
cleared); it matters for GENCTC without INICTC and for repeated GENCTC calls.

```
M := NR4(n); I := NI4(n); nrm := NR1(n) (4 dbl); (a, b) := NR5(n)
off := (a + b) * 0.5;  if !(0 < off): off := 0          (NaN kept)
nfix := #r with c[r]==1 or c[r]>600 (ifix := last such r, m := -M[ifix]); nneg := #r with c[r]<0
Pout := P
for r = 1..3 with c[r] < 0:
   FUN_14004c429(c, &dupObj, &ndup, &r, &pos)       // pairs (x,y),(y,z),(x,z) with equal negative codes
   if ndup > 0 and pos == 1: rDup := r
   if ndup < 1 or pos < 2:                            // first (or only) direction of this object
      p := -c[r];  if p == O: next r;  e := DAT_140778684[p];  if e <= 0: next r
      if nneg > 1: nrm[1..3] := 0; off := -0.06f (DAT_140bad9b0 = -0.05999999865889549)
      FUN_14004ce7a(O, ent, bcc, ndof, &p, NI1(n), &cnt, nrm)     // normal from polygons sharing code -p
      nrm[4] := off
      FUN_14006798b(&p,&e,&3,&npoly(p), Pout, lc, &poly, &d, &st, Q, nrm, &(-O), dbg)
      if |d| > 2.0·L:                                 (DAT_140bad4d0 = 2.0)
          FUN_14006798b(... lc2, &poly2, &d2, &st, Q2, zeros DAT_1405c70b0, &0, dbg)
          if |d2| < |d|: (lc, poly, d, Q) := (lc2, poly2, d2, Q2)
      if |d| <= 5.0·L:                                (DAT_140bad498 = 5.0)
          FUN_14004ad46(&p, e, O, ent, Pout, Q, lc, &poly, bcc, ndof, &r, n, &L, &sep)   §4.4
          if p != 0:
             M[r] := normal(p, poly) (1400483ed: ER1(poly), or NR1(node -poly) if poly < 0)
             if nneg == 1 and nfix == 0: nrm := -normal(p, poly); Init NR1(n) := nrm
             I.code[r] := -p; I.poly[r] := poly
             Pout[k] := Q[k] for k = x, y, z unless bcc[n][k] == 1
             if p == dupObj and r == rDup: lcDup := lc
             if ifix > 0 and c[ifix] > 600:
                if nfix == 1: k := I.poly[ifix]; v := Pout - X_k; s := v·m; Pout := Pout - m·s
                if nfix == 2: v := Pout - P;  s := v·nrm;  Pout := nrm·s + P
      else:                                            // release
          I.code[r] := 0; I.poly[r] := 0; bcc[n][r] := 0
          WRITE(unit 7a7bc,*) 'Release Contact Boundary Condition of', n, 'from die ', p,
                              ' because of large distance'
   else:                                              // second direction with the same object
      FUN_14004c564(O, ent, &dupObj, &e, &rDup, &r, lcDup, I, M)
Init NR4(n) := M; Init NI4(n) := I
```

Operation order [fact, listing l_140049b2d.txt]: `s = (vx·mx + vy·my) + vz·mz`;
nfix 1: `Pout_x = Pout_x − m_x·s` (each component); nfix 2: `Pout_x = nrm_x·s + P_x`.

`FUN_14004ce7a` (881 bytes, read fully): for each polygon q of NI1(n): common code of its
nodes (`FUN_14003a978`, see search spec); if it equals −p: `acc += ER1(q)·ER2(q)`
(`acc_k = n_k·area + acc_k`) else the list entry is negated. `len = sqrt((ax²+ay²)+az²)`;
if `len > 9.99999993922529e-09` (`DAT_140bad7f8`): `nrm = acc / len` (three divisions),
else nrm unchanged.

`FUN_14004c429` (315 bytes, read fully): with `c1,c2,c3` the codes: pair (1,2) equal and < 0
→ count 1, obj −c1, pos 1 for r=1 / 2 for r=2; pair (2,3) → +1, pos 1 for r=2 / 2 for r=3;
pair (1,3) → +1, pos 1 for r=1 / 2 for r=3; if count == 2: pos := r.

`FUN_14004c564` (1057 bytes, read fully): duplicate contact with the same object in
directions rDup < r. `q := I.poly[rDup]`; locator (§4.8) on `lcDup` for q (quad if 4
distinct nodes, else triangle) → code k. k == 0 (inside): `I.code[r] := 0, I.poly[r] := 0`
(BCCDEF not changed). k > 0 (edge): `I.code[r] := -p`, `I.poly[r] := EI1(q)[k]`,
`M[r] := ER1(that polygon)`. k < 0 (corner): `FUN_14005ee88` (CRVPLY, 2118 bytes, summarised:
walks the polygons around corner node `|conn[-k]|` via EI5/EI6, may print `SKIPPING CRV #:`
`IN CRVPLY` on unit 7a7bc) returns a new polygon q' and edge k'; then
`I.code[rDup] := -p, I.poly[rDup] := q', M[rDup] := ER1(q')`,
`I.code[r] := -p, I.poly[r] := EI1(q')[k'], M[r] := ER1(EI1(q')[k'])`.

### 4.4 `FUN_14004ad46`: separation / change of contact object (3923 bytes, read fully)

Called from 140049b2d for direction r of contact object p with projection (Q, lc, poly).

```
sep := 0
scan z, y, x: if a second negative code is found → return          // only single-contact nodes
(u, len) := unit(Q - P)                                  (1404d68dd)
conn := EC(p, poly); k := locator(lc) (quad 1404d3e13, or triangle 1404e37c4 if 3 nodes or |c1|==|c4|)
if k < 0: FUN_1400446b4(p, e, &poly, conn, &k, NI7a, NI7b)  (corner → edge of an adjacent polygon)
if k < 0: FUN_14004bc99(p, e, |conn[-k]|, &k, &poly, conn)   (second attempt)
if k < 1: return                                         // foot inside the polygon: keep contact
a, b := |conn[k]|, |conn[k+1]| (wrap); NI7(a), NI7(b): both [0] must be 2, else return
t := unit(b - a) (1404d68dd);  w := cross(t, ER1(poly)) normalised (140066ff1);  s := u·w
if NI7(a)[1] != NI7(b)[1] or NI7(a)[1] == 0: return
q := NI7(a)[1]  (object across the edge);  e2 := DAT_140778684[q]
FUN_14006798b(&q, &e2, &3, &npoly(q), P, lc2, &poly2, &d2 (preset 1e6), &st, Q2, zeros, &0, &0)
if poly2 > 0 and |ER1(poly)·ER1(q,poly2)| < 0.5: return
if (d2 < len and d2 > 1e-8) or (d2 < L·0.3f and s < -0.5):     (DAT_140bada20 = 1e-8, DAT_140bada28 = 0.30000001192092896, DAT_140bad628 = -0.5)
    WRITE(6,1000) n;  bcc[n][r] := 0;  sep := p;  p := 0;  return
if d2 > 1e-8: return
WRITE(6,1001) n;  WRITE(6,1002) poly, p, poly2, q
if -q occurs in bcc[n][1..3]: WRITE(6,1003) n, -p;  bcc[n][r] := 0
else: bcc[n][r] := -q
p := q; lc := lc2; poly := poly2; Q := Q2
```

`FUN_1400446b4` (2936 bytes) and `FUN_14004bc99` (1936 bytes): topology walks over NI1 /
EI2 / EC / NI7 of the partner to turn a corner hit into an edge (k > 0) of a polygon whose
edge nodes both have `NI7[0] == 2`; NI5 == 3 of the corner node negates k. **Summarised
only** (control flow read, not transcribed).

### 4.5 `FUN_14004522c`: contact polygon switch for two-contact nodes (4677 bytes, read fully)

Runs only if exactly two of `bcc[n]` (old values) are negative.

```
I := NI4(n), M := NR4(n); the two contact directions give (obj_j, poly_j, nrm_j), j = 1, 2
if |nrm_1·nrm_2| < 0.35f: return          (DAT_140bad930 = 0.3499999940395355)
if obj_1 == obj_2: return
for j = 1, 2: over the edges k of poly_j with EI6(poly_j)[k] > 0 and EI2(EI1(poly_j)[k]) == 1:
     dist_jk := point-segment distance (1404d6b13) from X (old position) to edge k;
     keep the minimum (start 1e4, strict <) → (k_j, dist_j)
if no edge found for either: return
if dist_1 <= tol2 or dist_2 <= tol2:
   choose j with the smaller dist (tie → j = 2); nb := EI1(poly_j)[k_j]
   dot := ER1(poly_j)·ER1(nb);  if |dot| < 1.0: use nb, val := dot  else val := 1.0
   if |val| < 0.35f: for r with I.code[r] == -obj_j: I.poly[r] := nb, M[r] := ER1(nb)
        Init NI4, NR4; WRITE(unit 7a7bc,*) 'Contact Poly for Object', obj_j,
        'has been changed for Node:', n;  f := 1
```

`FUN_1404d6b13` (1017 bytes, listing): `t = ((p−a)·(b−a)) / |b−a|²` clamped to [0, 1],
foot `a + t(b−a)`, distance `sqrt((dx²+dy²)+dz²)`; `|b−a|² == 0` prints
`1001( ' ERROR IN PNTLIN ',/,3F20.7)` on unit 6 and calls `FUN_1404edb2f(-1)`, which is a
bare `RET`, so the division by zero proceeds **[fact]**.

### 4.6 `FUN_140040d77`: decision (14653 bytes, read fully, key tests checked in the listing)

Parameters: `P` (current position), `codes` (out), `Pn` (out), `nacc` (out), `tolS`, `mode`
(0), `sep`. Static state used below: `best` (`DAT_14066ccb4` object, `DAT_14066ccc0` signed
distance), `evict` (`DAT_14066ccb8`), slot tables `Q[1..3]` (`DAT_14066cd48 + 24·slot`),
`dirOfSlot[1..3]` (`DAT_14066d08c + 4·slot`).

```
nacc := 0
if all three bcc[n][k] != 0: return                                       (nothing written)
Pn := P
nb := NI2(n); if count == 0: return                                       (not a surface node)
FUN_14003abfd(bcc, [n, nb...], count+1, ndof, &shared, &dirShared)
if shared != 0: return                                                    (§4.6.1)
codes := bcc[n]
r := max(9.999999717180685e-10, tolS)        (strict: r := tolS only if 1e-9f < tolS)
box := FUN_1404d1b1c(Pn, r)                  (x−r, y−r, z−r, x+r, y+r, z+r)
I := NI4(n); M := NR4(n); nrm := NR1(n)
negC := last negative code in x, y, z order (z wins), negDir its direction; posDir := last positive direction
best := (0, 1e5); bestAbs2 := 1e5; evict := 0; nOld := 0 (3 dbl)
if negC != 0:
    best.obj := |negC|; best.d := tolS · 0.05f (DAT_140bad758 = 0.05000000074505806)
    if negC != -O: nOld := ER1(|negC|, I.poly[negDir])
for i = 1..DAT_14077867c:
    p := DAT_140771918[i]
    skip if p == O or p == 0 or (sep != 0 and p == sep) or some bcc[n][k] == -p
    e := DAT_140778684[p]
    skip if DAT_140772728[e·100+p] == 1 or DAT_14076b6a8[p·3+e·300] <= 0
    skip if FUN_14006790a(p, e, box) finds no polygon (cfnCUBE_SearchPoly_Obj count 0)
    skip if DAT_14076e588[...] == 0 or box does not overlap the partner box (1404d1ed7, closed intervals)
    s := nacc + 1
    FUN_14006798b(&p,&e,&3,&npoly, P, lc, &poly, &d (preset 1e5), &st, Q[s], zeros, &0 (flag), dbg)
    if poly == 0: WRITE(6,*) 'Invalid Contact search at Node ', n, ' wrt Die No. ', p;  next i
    §4.6.2 classification → distance dd (signed) and sign factor sg, or skip
    §4.6.3 acceptance
after the loop: §4.6.4
```

#### 4.6.1 `FUN_14003abfd` (763 bytes, read fully)

For directions k = 1..3 with `c = bcc[n][k] != 0` (x first): count the neighbours j that
share it — for `c == 1`: neighbour code in the same direction k is 1; otherwise: c appears
in any direction of the neighbour. If the count equals the number of neighbours, return
`shared := c` (and `dirShared := k` for c == 1) at once. (A second test against a counter
that starts at the neighbour count and grows with every nonzero code can never be met.)
Effect: a node whose fixed code (or contact code) is shared by all its neighbours is not
examined **[fact]**.

#### 4.6.2 Classification of the candidate (poly > 0)

With `conn := EC(p, poly)`, `nv` = 4 or 3 (`nv := 3` when `|c1| == |c4|`):

1. `k := locator(lc)`: quad (nv 4) `FUN_1404d3e13`, triangle `FUN_1404e37c4` (§4.8):
   0 inside, k > 0 edge k, k < 0 corner −k.
2. k < 0: `FUN_1400446b4(p, e, &poly, conn, &k, A, B)`; still k < 0 → **skip partner**; if
   `A[2] > 0 and B[2] > 0` → `posDir := 0`.
   k > 0 and posDir != 0: edge nodes a, b; if `EI2(poly) != 1` and `NI7(a)[0] == 2` and
   `NI7(b)[0] == 2` and both `[1] > 0` → `posDir := 0`.
3. k == 0 → `ad := |d|` → **(R)**.
4. k > 0 (edge): `nbP := EI1(poly)[k]`;
   `nbP < 0` → `ad := |d|` → (R); `nbP > 0` and `EI2(nbP) != 0` → `ad := |d|` → (R);
   posDir != 0 → `ad := |d|` → (R). Otherwise (neighbour disabled or missing and no
   positive code): `t := unit(a − b)` (`FUN_1404d68dd(b, a)` gives `a − b`),
   `w := cross(t, ER1(poly))` normalised (1404cce5e); `v := Q[s] − P`;
   if `v·w > 0`: `d := |d|`; `ad := |d|`. If `nbP == 0` → (R). Else project P onto the
   neighbour polygon: `FUN_14006fb9b(xyz(nbP), ER1(nbP), tri(nbP), P, lc, ..., 5.0·ad, &d2, ...)`
   (quad: `FUN_1404d203b`, d2 negative when `(P − foot)·n < 0`; triangle: `FUN_1400702c8`;
   both in GENCTC_3d_search.md). If `ad > |d2|` and `nOld·ER1(nbP) <= -0.8`
   (`DAT_140bad748`, double) and `best.d > |d2|`: `best := (p, |d2|)`, `dd := −|d2|` →
   **(A)** directly (Q[s] stays the projection onto `poly`). Else → (R).

**(R)** — `ad` = unsigned distance, `d` signed:

```
if ad < |best.d| or (ad == |best.d| and d < 0):           [listing 140042a0b]
    best := (p, d)                                         (signed d stored)
    if evict != 0 and nacc > 0:
        FUN_140046625(&evict, I, codes, dirOfSlot, &nacc, Q)   // un-accept evict
        WRITE(unit 7a7bc,*) ' Info: Intialize Contact at Node : ', n, ' With Object : ', |evict|
st2 := EI2(poly)       (poly > 0 always per the search spec)
st2 == -1000 → skip partner
st2 == 2 → st2 := 1
st2 == 0 or -2000: if best.obj == p: evict := p (else skip partner)
other st2 (1, -k, ...): if ad > tolS → skip partner
sg := (d < 0) ? -1 : +1
if k == 0:
   if st2 == 0 or -2000: sg := -1
   if st2 < 0 and st2 != -O: sg := -1
   if st2 >= 1: dd := ad·sg → (T)
else (edge): if st2 != 1: nb := EI1(poly)[k]; nb < 1 → (posDir == 0 ? skip partner : continue);
             else st3 := EI2(nb); st3 == 2 → st3 := 1; elif st3 == st2 and st3 < 1: sg := -1
   if st2 > 0: dd := ad·sg → (T)
(st2 <= 0 here:)
if st2 == 0 or -2000:
   FUN_140046471(p, e, poly, &cc)     // common NI4 code of the polygon's nodes (FUN_14003a978)
   if cc < 1: ad := 0; sg := -1
elif -1000 < st2 < 0:                 // polygon is in contact with object k = -st2
   if negC != st2:
      FUN_14006798b(&k, &e_k, &3, &npoly(p), P, ..., &poly3, &d3, ..., zeros, &0, dbg)
      if poly3 == 0: skip partner
      if ad > |d3| and |d3| > tolS: skip partner
      if ad <= tolS and |d3| <= tolS and EI2(k,poly3) == -p and st2 == -k: sg := -1
      else sg := FUN_140046a93(p, poly, k, poly3)
   else:
      sg := FUN_140046a93(p, poly, |negC|, I.poly[dir of negC])
dd := ad·sg
if st2 < 0 and st2 == -O: st2 := 1
(T): if mode < 0 and dd >= tolS: skip partner          (mode 0: never)
if sg <= 0:
    FUN_14006798b(&p,&e,&3,&npoly, P, lc, &poly, &d4, &st, Q[s], zeros, &(-O), dbg)
    if st == -1: WRITE(unit 7a7bc,*) 'Potential DIEGEO interference at Node', n;  skip partner
    if st2 < 1: d4 := |dd|
    dd := |d4|·sg
```

`FUN_140046a93` (340 bytes, listing): `sg := (ER1(a)·ER1(b) >= -0.8999999761581421) ? 1.0 : -1.0`
(`DAT_140bad8c0`, float constant). `FUN_140046471` (436 bytes): collects NI4 codes of the
polygon's nodes and returns their common code via `FUN_14003a978` (search spec).

Note [fact]: in the `st2 == 0 / -2000` branch with no common code, `ad := 0`, so `dd = -0.0`
and the node is accepted regardless of its distance (only the search box limits it). For
rigid dies EI2 is 1 (valid) and the plain path applies: `ad <= tolS` then `dd = d`; nodes
behind the die surface (`d < 0`) get a third kernel call with flag −O and `dd := −|d4|`.

Oracle findings (added after oracle run):

* `st2 == 0` (and 2) cannot occur here: the first kernel call uses mode 0, which never returns
  EI2 = 0/2 polygons (§3.4). The die faces disabled by the symmetry planes
  (GENCTC_3d_search.md §7.2 step 4) are therefore simply invisible: `v_d0_x` / `v_d0_y` (die
  shifted 0.0009 sideways and up, its x = 0 / y = 0 face still disabled, `GENCTC 0.001`) reject
  the nodes on that side (distance 0.00127 > tol) exactly like the control `v_d0_far`.
* The `−2000` / `−k` "distance 0" acceptance **does** occur once SRCNT has marked partner
  polygons (partner–partner contact, `tol` ≥ die gap): in `d3d_two_masters` (`GENCTC 16`) every
  examined node of the workpiece gets `−1` whatever its distance (up to 15) and is moved to the
  third kernel call's projection onto the nearest **valid** polygon — the bottom edge of die 1's
  x = 30 face, e.g. node 2 (4,0,0) → (30,0,15); x-fixed nodes keep x: node 17 (0,5,0) → (0,5,15).
  Die 3 is then skipped for every node (`best.obj ≠ p` in the st2 = 0/−2000 branch), so no `−3`
  is written anywhere and the bottom nodes lose their `−3`. Unexplained: the fixed x code 1 of
  nodes 5, 9 and 13 (codes `1 1 0`) is cleared (`0 1 -1`), node 1 keeps it; same with
  `SKIP_PNTR.DAT`. Not followed further (unusual input).

#### 4.6.3 Acceptance (listing 1400432ab)

| mode | accept when |
|---|---|
| 0 (GENCTC) | `|dd| < tolS` (strict) |
| < 0 | `dd <= 0` and `|dd| < tolS` |
| > 0 | `dd <= 0` |

On acceptance:

```
WRITE(unit 7a7bc,1001) n, O, p     1001(' Info: Node ',I7,' of Object ',I6,' is inside Object ',I6)
nacc := nacc + 1
if codes[3] == 0:   codes[3] := -p; dirOfSlot[nacc] := 3; I.code[3] := -p; I.poly[3] := poly; M[3] := normal(p, poly)
elif codes[2] == 0: same with direction 2
elif codes[1] == 0: same with direction 1
(else: nacc is incremented but no code is written; dirOfSlot[nacc] keeps a stale value)
```

`normal(p, poly)` = `FUN_1400483ed`: `ER1(p, poly)` for poly ≥ 0, `NR1(p, node −poly)` for
poly < 0 **[fact]**. The slot's projected point is `Q[nacc]` (written by the kernel call of
this partner).

#### 4.6.4 After the loop

```
if nacc == 0: return
if nacc == 1 and mode == 0: Init NI4(n) := I; Pn := Q[1]; return      (NR4 not saved)
if nacc == 2:
   (u1, l1) := unit(Q[1] − P); (u2, l2) := unit(Q[2] − P)     (1404d68dd)
   if mode == 0 and l1 <= tolS and l2 <= tolS:
       WRITE(unit 7a7bc,*) 'Node ', n, ' is inside of two Objects:', -codes[dir1], -codes[dir2]
       return                                              (Pn = P: node not moved, both codes kept)
   x2 := (u2·S)·l2, x1 := (u1·T)·l1  with S = DAT_14066d1f0, T = DAT_14066d220 (statics
        written only by the mode ≠ 0 branch; 0 in a process that never ran it)
   if 0.5 < |S·T| and l1 < tolS and l2 < tolS: FUN_14004522c(..., &f, &tolS); if f == 1: return
   keep slot 2 if |x2| < |x1| else slot 1; drop the other: nacc := 1, codes[dir] := 0,
   M[dir] := 0, I.code[dir] := 0, I.poly[dir] := 0
Pn := Q[kept slot] (slot 1 when nacc == 3)
Init NR4(n) := M; Init NI4(n) := I
```

With S = T = 0 the drop rule keeps slot 1 (the first accepted partner in list order) **[fact]**.
In GENCTC the l ≤ tol test nearly always holds for two accepted partners, so the usual
result for a node touching two objects is: both codes, no move **[inferred]**.

`FUN_140046625` (461 bytes, read fully): for slots i = 1..nacc with
`codes[dirOfSlot[i]] == −evict`: clear that code, `I.code`, `I.poly` and the slot; then compact
slots and their Q (Q[nacc+1] is moved to Q[m+1] too), `nacc := m`, `evict := 0`.

### 4.7 `FUN_14005266d`: final constraint correction (2268 bytes, read fully, listing)

`X` = `coords[n]` (already set to P), `Pn` in/out, `codes` from 140040d77.

```
fix[] := directions with code 1 or > 600 (nfix);  neg[] := directions with code < 0 (nneg)
if nneg == 0 or nfix == 0: return (Pn unchanged)
nrm := NR1(n); M := NR4(n); I := NI4(n)
if nfix == 2 and nneg == 1:
    t := cross(M[fix1], M[fix2]) (140066ff1); s := t·nrm; M[neg1] := (s < 0 ? -1 : 1)·t; Init NR4(n) := M
if nfix == 1 and nneg == 1:
    t := cross(M[fix1], M[neg1]); w := cross(M[fix1], t); s := w·nrm; nrm := (s<0 ? -1 : 1)·w   (local only)
if nfix == 3: Pn := X
if nfix == 2: v := Pn − X; s := v·nrm; Pn := nrm·s + X                 (Pn_x = nrm_x·s + X_x)
if nfix == 1: k := I.poly[fix1]; v := Pn − X_k (DAT_140769750 + 24k); s := v·M[fix1]; Pn := Pn − M[fix1]·s
for r with codes[r] == 1: Pn[r] := X[r]
```

For a code-1 direction `M[fix1] = ±e_r` and `I.poly = 0`, so `X_0` (`DAT_140769750`, the word
before the 1-based plane table) is read; only component r changes and it is reset to `X[r]`
in the last line, so the stale read has no effect unless it is inf/NaN **[fact/inferred]**.

### 4.8 Geometry helpers (all read fully; order from the listings)

Common rule [fact, every listing checked]: a 3-term dot product is
`(a1·b1 + a2·b2) + a3·b3` (x term added to y term first); the decompiler prints it in
reverse order. Sums of squares likewise `(x² + y²) + z²`.

| Routine | Operation |
|---|---|
| `FUN_1404d68dd(a, b, u, len)` | `u = b − a`; `len = (ux²+uy²)+uz²`; if `len != 0`: `len = sqrt(len)`, `u = u / len` (three DIVSD); else u unnormalised, len 0 |
| `FUN_140066ff1(a, b, c, len)` | `c1 = a2·b3 − b2·a3`, `c2 = a3·b1 − b3·a1`, `c3 = a1·b2 − b1·a2`; `s = (c1²+c2²)+c3²`; if `|s| > 1.0000000168623835e-16` (float const `DAT_140badec8`): `len = sqrt(s)`, `c = c / len`; else c = 0, len = 0 |
| `FUN_1404cce5e(a, b, c, len)` | same cross product; `len = sqrt(n3² + (n2² + n1²))` (order per fp-audit.md, correction 4); if `len > 1e-16f` (`DAT_140bb2668`): `inv = 1.0 / len`, `c = c · inv`; else len = 0 and **c stays unnormalised** |
| `FUN_1404d1b1c(pts, box, r, n, list)` | box init (1e10, −1e10); per point min (strict <) / max (strict >); then `min − r`, `r + max` |
| `FUN_1404d1ed7(a, b, f)` | `f = 1` iff `b.min <= a.max` and `a.min <= b.max` on all three axes |
| `FUN_1404d3e13(ξ, η, k)` quad locator | eps = `DAT_140643a80` (0.001, .data); corners (−1,−1),(1,−1),(1,1),(−1,1) at `DAT_140643a90`. Strictly inside `(−1+eps, 1−eps)²` → 0. Else for edges k = 1..4 (k→k+1): band `[1−eps, 1+eps]` or `[−1−eps, −1+eps]` across the edge, `[−1−eps, 1+eps]` along it; exactly one edge matches → k; else the nearest corner by `sqrt((dη²)+(dξ²))`, start 2.0, strict < → −corner |
| `FUN_1404e37c4(ξ, η, k)` triangle locator | `L1 = ξ, L2 = η, L3 = (1 − ξ) − η`; first i with `|L_i − 1| < 0.001` (`DAT_140644870`) → −i; else first i with `|L_i| < 0.001` → i+1 (4 → 1); else 0 |

## 5. Outputs and messages

| Effect | Where |
|---|---|
| BCCDEF codes of node n: `−partner` in the first zero slot z, y, x; codes released (0) or moved to another object by §4.3/4.4 | `bcc` = `obj+0x990` |
| node moved to `Pn` (projection on the partner polygon, corrected for fixed and symmetry directions); not moved for "inside of two Objects"; moved to the re-projection for existing contacts (§4.3) | `coords` = `obj+0x8a8` |
| code 6 ↔ `600 + BCCDFN`: encoded before / decoded after the Fortran call by the driver (GENCTC.md §5.1 step 6); here codes > 600 act as symmetry codes and are never changed | driver |

Messages. Unit 6 = stdout (visible); unit `DAT_14077a7bc` = 1 = `NEWEQ.MSG` (deleted at exit).
`*` = list-directed (spacing from the PathScale runtime, not verified).

| Routine | Unit | Text / format | Arguments |
|---|---|---|---|
| 140040d77 | **6** | `*`: `'Invalid Contact search at Node '`, `' wrt Die No. '` | n, p |
| 140040d77 | 7a7bc | `*`: `' Info: Intialize Contact at Node : '`, `' With Object : '` | n, \|evict\| |
| 140040d77 | 7a7bc | `*`: `'Potential DIEGEO interference at Node'` | n |
| 140040d77 | 7a7bc | `1001(' Info: Node ',I7,' of Object ',I6,       ' is inside Object ',I6)` | n, O, p (every accepted contact) |
| 140040d77 | 7a7bc | `*`: `'Node '`, `' is inside of two Objects:'` (two sites) | n, obj1, obj2 |
| 14005190e | 7a7bc | `1001('Node ',I7,' Old coord. ',3F15.7,/,        12X,      ' New coord. ',3F15.7)` | n, X, Pn (every written node) |
| 140049b2d | 7a7bc | `*`: `'Release Contact Boundary Condition of'`, `'from die '`, `' because of large distance'` | n, p |
| 14004522c | 7a7bc | `*`: `'Contact Poly for Object'`, `'has been changed for Node:'` | obj, n |
| 14004ad46 | **6** | `1000(' Info : Node separation is detected. Node number is ',I10)` | n |
| 14004ad46 | **6** | `1001(' Info : Node #',I7, 'Contact condition is changed')` | n |
| 14004ad46 | **6** | `1002(' Info : Node contact is moved from Poly # ',I7,        'of Object (',I3,'to Poly #', I7, 'of Object ',I3)` | poly, p, poly2, q |
| 14004ad46 | **6** | `1003(' Info : Node #',I7, 'Contact condition ',I7,' is changed')` | n, −p |
| 1404d6b13 | **6** | `1001( ' ERROR IN PNTLIN ',/,3F20.7)` | a (3 values) |
| 14005ee88 | 7a7bc | `*`: `'SKIPPING CRV #:'`, `'IN CRVPLY'` | curve no. |
| 14005190e | 7a7bc | `Before IODIE2` / `After IODIE2` dumps | dead (dbg = 0) |

No error return: every failure skips the partner or the node and continues.

## 6. Helper table

| Address | Role | Size | Status |
|---|---|---|---|
| `FUN_14005190e` | per-node driver | 3423 | read fully, listing |
| `FUN_14004946f` | constraint rows, NI4/NR4 init | 1726 | read fully, listing |
| `FUN_14004e10c` | node normal for constrained nodes | 1230 | read fully, listing |
| `FUN_140049b2d` | existing contacts | 4633 | read fully, listing |
| `FUN_14004ce7a` | contact normal from polygons | 881 | read fully |
| `FUN_14004c429` | duplicate-code pairs | 315 | read fully |
| `FUN_14004c564` | duplicate contact, same object | 1057 | read fully |
| `FUN_14005ee88` | CRVPLY corner walk | 2118 | summarised |
| `FUN_14004ad46` | separation / object change | 3923 | read fully, listing |
| `FUN_1400446b4`, `FUN_14004bc99` | corner → edge topology walks | 2936, 1936 | summarised |
| `FUN_14004522c` | contact polygon switch | 4677 | read fully, listing |
| `FUN_1404d6b13` | point-segment distance | 1017 | read fully, listing |
| `FUN_140040d77` | decision | 14653 | read fully, listing for key tests |
| `FUN_14003abfd` | shared-code test | 763 | read fully |
| `FUN_14006790a` | polygon presence in box | 129 | read fully, listing |
| `FUN_140046471`, `FUN_140046625`, `FUN_140046a93` | common code, evict, normal sign | 436, 461, 340 | read fully |
| `FUN_14007e62b` | copy n×m doubles | 304 | read fully |
| `FUN_1400483ed` | contact normal (ER1 / node NR1) | 331 | read fully |
| `FUN_14006fb9b` | projection on neighbour polygon | 530 | read fully (callees in search spec) |
| `FUN_14005266d` | final constraint correction | 2268 | read fully, listing |
| `FUN_1404d68dd`, `FUN_140066ff1`, `FUN_1404cce5e` | unit vector, cross products | 151, 258, 250 | read fully, listing |
| `FUN_1404d1b1c`, `FUN_1404d1ed7`, `FUN_1404d3e13`, `FUN_1404e37c4` | box, overlap, locators | 489, 156, 997, 383 | read fully |
| `FUN_14006798b`, `FUN_14003a978`, `FUN_14004f363`, `FUN_1400702c8`, `FUN_1404d203b`, cfnCUBE_* | kernel and library | | see GENCTC_3d_search.md |
| `FUN_1404b5ef4` | Fortran WRITE | 201 | not followed (runtime) |

Constants (all read from the binary): `DAT_140bad138` 1.0, `DAT_140bad880` −1.0,
`DAT_140bad268` sign mask, `DAT_140bad200` abs mask, `DAT_140bad3c8` 1e5, `DAT_140bad320` 1e6,
`DAT_140bad5a0` 1e4, `DAT_140bad888` 1e-6 and `DAT_140bad4a0` 1e-10 (computed in 140040d77,
result unused), others as quoted above.

## 7. Open items

1. `FUN_1400446b4`, `FUN_14004bc99`, `FUN_14005ee88` (corner handling) are summarised, not
   transcribed; they matter only when a projection foot lies on a polygon corner.
2. ~~EI2 semantics are taken from GENCTC_3d_search.md; the `st2 == 0 / −2000` "distance 0"
   acceptance (§4.6.2) should be checked with the oracle (a case where SRCNT disables polygons).~~
   Checked (§4.6.2, oracle findings): EI2 = 0 is unreachable in GENCTC; the −2000/−k acceptance
   at distance 0 is seen in `d3d_two_masters`. Open: the x-code loss of nodes 5, 9, 13 there.
3. nacc == 2 fallback uses statics `DAT_14066d1f0`/`DAT_14066d220` that only the mode ≠ 0
   path writes (callers `FUN_1403722fc`, `FUN_140087c32`, `FUN_140086598`). If such a call ran
   earlier in the process, GENCTC's choice between two partners can differ.
4. List-directed output spacing (unit 6 `Invalid Contact search` line) not verified.
5. The symmetry plane tables (§3.3) are never filled in DEF_PRE as far as found; codes
   `600+k` then act as "fixed" with a zero constraint row (§4.2, §4.7).

## 8. Tests

`tests/commands/GENCTC/cases_d3d.json` with `d3d_*.KEY` (seed `UPSET_Brick.KEY`: workpiece
2, z 0..15; top die 1 from z = 15; bottom die 3 up to z = 0; partner list of object 2 is
[1, 3]):

| File | Exercises | Prediction |
|---|---|---|
| `d3d_two_masters.KEY` | INICTC 2, GENCTC 16 | **oracle: prediction wrong, test does not exercise the two-code rule**: tol 16 ≥ die gap 15 triggers partner–partner contact; see §4.6.2 oracle findings. The two-code rule is exercised by `v_two_masters_12` (`GENCTC 12`): nodes on the outer faces at z = 4 / 11 get −1 in z and −3 in y (−3 in x for the y-fixed nodes 8, 12), not moved; nodes 5, 9 (x and y fixed) get only −1 (die 1 first, no free slot for die 3) — exactly as predicted here |
| `d3d_move_rows.KEY` | INICTC 2, GENCTC 5 | side nodes at z = 4 / 11 accepted by die 3 / 1 and moved to z = 0 / 15. Oracle: OK — only the nodes on the outer faces x = 12 / y = 15 plus nodes 5, 9; nodes lying only on the symmetry faces x = 0 or y = 0 (6, 7, 10, 11, 21, 25, 37, 41) are skipped by the shared-code test §4.6.1 (all their neighbours share the fixed code); nodes 5 and 9 (x and y fixed) get the code but are not moved (§4.7 nfix = 2 with a normal orthogonal to z) |
| `d3d_repeat.KEY` | INICTC 2, GENCTC 0 twice | second call: same result (existing codes kept, partners already coded skipped). Oracle: OK (OUT.KEY identical to one call) |
| `d3d_repeat_grow.KEY` | INICTC 2, GENCTC 0.0001, GENCTC 5 | second call adds codes to side rows only. Oracle: OK (identical to `d3d_move_rows`) |
| `d3d_noinictc_tol5.KEY` | GENCTC 5 without INICTC | nodes already coded go through §4.3; new codes only in free slots. Oracle: OK (identical to `d3d_move_rows`) |
| `d3d_separation.KEY` | top die up 0.5, GENCTC 0 without INICTC | top nodes keep −1 and follow the die to z = 15.5 (foot inside polygons, no separation message). Oracle: OK |
| `d3d_release.KEY` | top die up 50, GENCTC 0 without INICTC | top −1 codes released (`|d| > 5L`), nodes not moved. Oracle: OK (message in NEWEQ.MSG, which is deleted) |

## 9. Note for the parent (GENCTC.md)

* §5.4 should be replaced by a link to this file (§4.1-4.7) with a one-line summary; the
  statement "code goes into the first zero component z, y, x" stays correct, "keeps the nearer
  one" should read "keeps both codes and does not move the node when both projections are within
  tol; otherwise keeps the first accepted partner" (§4.6.4).
* §5.5: add the unit-6 messages of §5 (`Invalid Contact search ...`, 14004ad46 formats
  1000-1003, `ERROR IN PNTLIN`); `Contact condition is changed` is only reached for nodes that
  already had a single contact code.
* §5.1 step 5: the "instance flags" `DAT_140778684[obj]` are the cube entity number (always
  1); their persistence has no effect.
* Helper table row `FUN_14005190e`, `FUN_140040d77`: "read fully, see GENCTC_3d_decide.md".
