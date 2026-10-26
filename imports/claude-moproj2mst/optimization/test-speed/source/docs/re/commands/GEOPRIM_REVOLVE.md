# GEOPRIM revolve: surface of revolution, mesh-boundary sweep, cap triangulation

Packet p9-05, task T9-2. Companion to [CNVT3D_SWEEP.md](CNVT3D_SWEEP.md) §10.4, which stops at
`FUN_1402ce190`. Sources: `deform-rea-work/out/p905/all.c` (decompiles of the 69 functions),
`all_l.txt` (listings), `ce310.c`, `addseg.c`, `tri2.c`, `small.c` (callees). Port:
`port/defmodel/src/geoprim/{revolve,revolve_mesh,triangulate}.cpp`, tests
`port/defmodel/tests/test_geoprim_{revolve,triangulate}.cpp`.

Status labels as in CNVT3D_SWEEP.md: **[code]** read from the decompile/listing, **[oracle]**
confirmed by a DEF_PRE run, **[inferred]**.

All the C++ here is unoptimised in the binary (every value is spilled to the stack), so the
decompiled expression order is the executed order; comparisons were checked against the listing.

## 1. Reachability [code]

* `FUN_1402ce190` (revolve) is called from the builder `FUN_1402ba500` when `prim+0x2330 == 2`.
  Batch callers set `mode_geo = +0x23a0` and `mode_mesh = +0x23a4` with constants
  (docs/phase5_8_9_10-packets.md, "Not needed"): `FUN_140153c00`/`FUN_140147f90` (1, 0),
  `FUN_140154270` (0, 2).
* `FUN_1402ce290` runs `FUN_1402d4420` / `FUN_1402d2920` (chosen by
  `strstr(rev+0x420, "ExtrCrossSection2D.KEY")`) only for `mode_mesh == 1`. That never happens in
  batch, so they and everything only they reach are **not needed** (16 functions: `1402d2920
  1402d4420 1402d2720 1402d20c0 1402d0e50 1402d0d10 1402d0a30 1402d0470 1402d7220 1402e9dc0
  1402eaba0 1402eb330 140205510 140205620 140258140 14023dce0`; callers checked in the function
  table).
* `mode_geo == 2` (`FUN_1402dbb20`, quad strips) and the non-uniform sections (`FUN_1402cac30`,
  `FUN_1402caf40`; `FUN_1402caac0` is true for every revolve set up by `FUN_1402b8980`, because
  `prim+0x2388` stays 1.0) are not reached in batch either; they are ported anyway (small).

## 2. `FUN_1402ce190(prim)` [code]

```
if (rev && rev->axis_kind == 0) {
    FUN_1402d9bb0(prim, dir, &t, pt);
    rev->axis_kind = (t == 0 || t == 1) ? 1 : 2;   if (t >= 2) prim+0x24db = 1;
}
ok = false;
if (mode_geo)  ok = FUN_1402d7cd0(prim);
if (mode_mesh) ok = FUN_1402ce290(prim);          // 2 -> FUN_1402ce310
return ok;
```

## 3. Axis class `FUN_1402d9bb0(prim, dir, &t, pt)` [code]

Nothing is written without a revolve record. `dir = rev->axis_dir`, normalised (`14023b720`).
`t = 0` when `|1 - |dir.x|| < 1e-6`, else `1` for y, else `2` for z, else `3`. `t 0` / `t 1`
snap `dir` to `(+-1, 0, 0)` / `(0, +-1, 0)` by the sign. The third test of the source repeats
`t == 0`, so a z axis keeps its normalised direction. The axis point drops one component:

| t | `rev->axis_kind` | pt |
|---|---|---|
| 2 | 3 | (P.x, 0, P.z) |
| 2 | other | (0, P.y, P.z) |
| 1 | 2 | (0, P.y, P.z) |
| 1 | other | (P.x, P.y, 0) |
| 0 | 3 | (P.x, 0, P.z) |
| 0 | other | (P.x, P.y, 0) |
| 3 | any | P |

with `P = rev->axis_pt`. The record's axis direction is copied raw by the setup `1402b8980`
(`140239800` is a plain copy, p9-02), not normalised as CNVT3D_SWEEP.md §10.2 says.

## 4. Surface generator `FUN_1402d7cd0(prim)` [code, oracle]

Defaults: `t = 2`, `dir = (0,0,1)`, `pt = 0`.

1. Fail unless `angle >= 1e-12` and the profile (`prim+0x1700`, 1-based (r, z) points) is not
   empty. `angle = min(angle, 360)`; `full = |angle - 360| < 1e-8`; `nsec = prim+0x2380`, fail
   when < 1.
2. `start = rev ? rev->start : (u2340 ? *(double*)(u2340+0x858) : 0)`; with a record also
   `FUN_1402d9bb0`. Then `FUN_1402da250` (§5).
3. Steps: uniform (`FUN_1402caac0`, p9-04) gives layer k at `(angle / nsec) * k`. Otherwise
   `FUN_1402cac30` / `FUN_1402caf40` give four zones (counts n15, n17, n19, n21 and step sizes);
   the layer angle accumulates the zone's step (the zone order depends on whether
   `a0 - start <= 0.01`). When `|v2398[0]| <= 0.01` after `FUN_1402cac30` the code treats the
   sections as uniform but the step is never set (stack garbage in the binary; 0 in the port).
4. `FUN_1402e7f40(profile)` (p9-04) sets the profile's closed flag `+0x1710`. Caps are made only
   when `u24fb == 0`, not `full`, `mode_geo == 1` and the profile is closed: `FUN_14041f830` (§7),
   or p9-04's `FUN_1402e24f0` when `+0x24b9` is set (never in batch). Cap triangles become faces
   `a b c a`.
5. Layer 0: each profile point (x, y) becomes 3D by `t` and the record kind: t 2 `(0, x, y)` (kind
   3: `(y, 0, x)`), t 1 `(x, y, 0)` (kind 2: `(0, x, y)`), t 0 `(x, y, 0)` (kind 3: `(y, 0, x)`),
   t 3 by the kind alone (1 `(x, y, 0)`, 2 `(0, x, y)`, 3 `(y, 0, x)`, else nothing written).
6. When `|start| > 1e-6` each point is rotated by `start` about the axis (`FUN_140325691`, p1-10),
   then by -90 when `+0x24db` (z axis). [oracle] the -90 turn gives `y = r * 6.1232011757e-17`
   (KEY `5.8323491199E-014` for r = 952.5), which the p1-10 rotation reproduces.
7. With a record, `FUN_1402da9f0` (§6) may reverse the layer-0 points and the cap ids.
8. `FUN_1402db0d0` flags the points on the axis: `|dir x (p - pt)| < 1e-6`; returns the count.
9. Layers `1 .. (full ? nsec - 1 : nsec)`: each layer-0 point rotated by the layer angle,
   appended in order.
10. Side faces: `FUN_1402db310` (mode_geo 1): for each layer s (and, when full, a last pass that
    joins the last layer to layer 0) and each profile segment i → i+1 (the closing segment only
    when the profile is closed), two triangles; a triangle whose first point is on the axis is
    skipped. With `b = s*n`, `c1 = on_axis[i]`, `c2 = on_axis[i+1]`:
    `c1 == 0: (b+i+1, b+i+1+n, c2 ? i+2 : b+i+2+n)`, `c2 == 0: (c1 ? i+1 : b+i+1, b+i+2+n, b+i+2)`
    (the closing segment uses `1` / `b+1` / `b+1+n`; the join uses `nl+…` for the last layer and
    `1..n` for layer 0; `c1 ? i+1` has no layer offset in the binary). `FUN_1402dbb20` (mode_geo
    2) makes one quad per segment instead.
11. Far cap (partial turn only): each cap face `(a b c d)` again as `(d' c' b' a')` with
    `x' = x + n*nsec` unless point x is on the axis.
12. Store: `FUN_1402dbe70` (p9-04) when no point is on the axis, else `FUN_1402dc160`: points no
    face uses (only the first 3 ids of each face count) are dropped and the ids renumbered in point
    order, then `FUN_1402dbe70`.
13. WPAXIS record 1 of the object (`obj+0x20a0`, `140259bc0`): `+4 = 0`, `+8` axis point, `+0x20`
    direction (the record's raw values, or the local pt/dir without a record).

[oracle] `cnvt3d_geometry_die_rev90_obj1` (RING_SI die, 90°, GEOSEC 20): DIEGEO `85 140`; the
port's surface matches all 85 points to the printed 11 digits and all 140 faces
(`test_geoprim_revolve.cpp`, golden `tests/data/p905_die_rev90_diegeo.txt`).

## 5. Profile projection `FUN_1402da250(prim, dir, pt)` [code, oracle]

As CNVT3D_SWEEP.md §10.4, with these details from the listing: the cross product is
`FUN_140308ad0(v, a) = v.x*a.y - a.x*v.y`; a point counts as positive when `s > 1e-6`, negative
when `s < -1e-6`; the projected point is `o + (v . a) a` (`140308a90`). FIXGEO input: an
`OBJ_GEO2D` with curve type 1 and the profile as rows `(x, y, 0)` (`dof` 3); after FIXGEO
(`14027ece0(geo, par, &k, 1, 0)`, p9-01) the profile takes the new count (grown with
`1402e3fa0` when larger) and the rows' x, y. The die case (all r >= 0) moves nothing.

## 6. Orientation `FUN_1402da9f0` / `FUN_1402d6bf0` [code]

Both build `c = dir x n` (n = unit normal of the polygon, `140240d80`), `ref = c . pt` and
`A = 14023e670(polygon)` = `0.5 |sum p_i x p_i+1|` (never negative). They scan the points for
`(ref < c.p && A > 0) || (c.p < ref && A < 0)`.

* `FUN_1402da9f0` (profile, layer 0): reverse when no point is beyond and `A > 0`, or one is
  beyond and `A < 0` (never). Reversal reverses the points and maps every cap id `k -> n - k + 1`
  with entries 1 and 2 of each face swapped.
* `FUN_1402d6bf0` (first 2D element): answer = (none beyond and `A < 0`) or (beyond and `A > 0`),
  negated when `*invert`; when set and `*keep == 0` the 2D connectivity has entries 0 and 2 of
  every element swapped. `FUN_1402ce310` passes `invert = keep = 1` and gives the answer to
  `FUN_1402bc980` as its flip flag.

## 7. Cap triangulation `FUN_14041f830(ncontours, cntr, vertices, tri)` [code, oracle]

A trapezoidation triangulator (randomised incremental segment insertion with a point-location
query tree, then monotone polygons, then greedy triangulation of each monotone piece), all state
in globals (`14077abcc..14077abe8`, `140722740..140722780`). Every "equal" test is
`|a - b| <= 1e-12` (`DAT_14051f260`); the bounding trapezoids use `+-2^30`.

* Input: `cntr[c]` points per contour, `vertices` 1-based (16-byte stride, `prim+0x1708`), the
  contours one after another; segment i runs from point i to the next point of its contour.
* Tables: segments `n+1` (0x38 each), query nodes `8n+1` (0x28), trapezoids `4n+1` (0x48);
  overflow prints `newnode: Query-table overflow` / `newtrap: Trapezoid-table overflow` to stderr
  and returns -1 (the caller then indexes with -1). `add_segment: error` and `Haggu !!!!!` are
  the two other stderr messages.
* Order of insertion (`14045c530`): `srand((unsigned)time(NULL))` (`1404801c4`), then a
  Fisher-Yates style permutation with `rand() % (n + 1 - i) + 1` (static CRT `rand`:
  `holdrand = holdrand * 214013 + 2531011`, `(holdrand >> 16) & 0x7fff`).
* Phases (`14045cfa0`): the first segment builds the initial query tree (`14045d0f0`); then
  `math_logstar_n(n)` rounds, each inserting segments `math_N(n, h-1)+1 .. math_N(n, h)` and
  relocating the roots of the not yet inserted ones; then the rest. `math_N(n, h) =
  ceil(n / log2^(h)(n))` with the CRT log (140bb3c40). Checked with the p1-01 binary-routine
  harness (`scripts_p101/logcheck_p905.cpp`): the CRT log equals `std::log` bit for bit on all
  99 978 arguments these loops can reach (n <= 20000).
* Monotone pieces (`14045a340`): the first trapezoid in table order that is a triangle inside the
  polygon starts the traversal (`14045a690`, 8 cases, each adding at most one diagonal through
  `14045b630`); then each piece is triangulated from its topmost vertex (`14045bc60`,
  `14045c030`, cross product `<= 0` keeps the reflex chain).
* Output: 3 ids per triangle, the count returned.

**What the random seed changes [code, oracle].** The permutation only fixes the insertion order,
hence the numbering of trapezoids and query nodes. The traversal starts at the lowest-numbered
inside trapezoid, so the numbering of the monotone pieces, the order in which they are
triangulated and the first vertex of each triangle follow the seed. The pieces themselves (and
so the triangle set) are fixed by the geometry; in every run and every seed tried the triangle
set was the same, only order and rotation changed.

[oracle] `cnvt3d_geometry_wp_rev90_obj2` (RING_SI workpiece outline, 16 points, 90°, 20
sections): three runs two seconds apart gave two different CONV3D.KEY files, differing only in
the 14 near-cap and 14 far-cap faces of DIEGEO 2. With the clock pinned to the run's second, the
port reproduces each run's cap order exactly (seeds 1792596538/539 give run 1, 540/541 run 2,
first seen with a seed search). The oracle tool now records each run's `start_time`; with the
clock pinned to it the port's caps match the refreshed entry byte for byte at offset 0
(`oracle_caps_at_recorded_start_second`).

**Consequence for the oracle comparison.** Any output with triangulated caps (partial revolve of
a closed geometry profile; every extrude cap of p9-04 too, since `+0x24b9` is always 0) is not
reproducible from a cached run: the cache does not record the second DEF_PRE read. Either
compare those faces as a set (rotate each triangle to start at its smallest id keeping the
orientation, sort the triangles; the far cap and every id outside the caps stay byte-exact), or
pin the port to the entry's recorded `start_time` (`DEF_PORT_FIXED_TIME`, retries -1/+1/-2/+2 s): the
scheme adopted in p67-gate's `cmd_oracle_runner.hpp`, with the face-set comparison as flagged fallback.

## 8. Mesh-boundary sweep `FUN_1402ce310(prim)` [code]

For `FUN_140154270` (CNVT3D kind 2 from the 2D mesh). `prim+0x23d8` (the builder's 5th argument,
an int buffer from `FUN_140154270`) maps every 3D node to its 2D node (0-based).

1. `u23e0 = u23e4 = u23e8 = u23ec = 0`. Fail as in §4.1 (2D node count `1402e9b50`, coordinates
   `1402e9b90`). When the int lists are empty, `FUN_1402cc080` (p9-04) builds the boundary chain;
   still empty → `cross section mesh boundary can not be constructed.` on stdout, fail.
2. `step = angle / nsec`, `start` and the axis as §4.2. Layer 0: every 2D node mapped by `t`
   (t 3 writes nothing), `ctx[i] = i`. `u23e0 = u23e4 = u23e8 = nn`; `u23f0..u2400 = pt`,
   `u2408..u2418 = dir`, `u2420 = 0` (= start when it rotates), `u2428 = angle`.
3. With a record: `flip = FUN_1402d6bf0(prim, dir, pt, xyz, &1, &1)`.
4. Chain = int list 1; when closed (`FUN_1402e91d0`) its count is decremented in place.
   Layers `1 .. nsec-1`: the chain nodes rotated by `step * s`; when not full a last layer of all
   2D nodes at `angle`. `ctx` follows. `u23e4 += (nsec/2) * nb`, `u23e8 += nsec * nb`, `u23ec = nb`.
5. Quads: `FUN_1402bc980(prim, &nsec, xyz, buf, &flip, &full)` (p9-04), buffer of
   `nb*nsec + 2*ne` quads.
6. A scratch model `new DB_STEP_DATA(dim(obj))` (`140220520`), whose `+0x15f8` table is copied
   over the target model's (`1401d7320(scratch, model+0x15f8)` writes the scratch's values into
   `model+0x15f8`), and a scratch object `new LLOBJ_DATA(scratch)` get the nodes (`140202700`,
   `1401808d0`) and one boundary with the quads (`140163300`, `140200a70`).
7. Every face id whose node has `x == 0 && y == 0` becomes `ctx[id-1] + 1` (the 2D node: nodes on
   the axis are shared). Faces with all four nodes on the axis are dropped. The kept faces are
   rewritten: `a == b` → `(a c d b)`, `c == d` → `(c a b d)`, `b == c` → `(b a d c)` (each sets
   `+0x24fc` and pushes 0 to the list `+0x2518`), else unchanged (`+0x24fd`, pushes 1). The list
   `+0x2518` holds 8-byte values; `1401599e0` copies 8 bytes from the int's address, so the flag
   is the low half.
8. `1402fc1b0` on the scratch surface 0, then the target object gets the scratch's nodes and faces
   (one boundary), the scratch object and model are deleted, WPAXIS 1 as §4.13.

## 9. Small helpers [code]

`1403086e0(x, y)`: Vector2 `(x, y)`. `140308ad0(a, b) = a.x*b.y - b.x*a.y`.
`FUN_1402dc160`: §4.12. `FUN_1402db0d0`: §4.8. `FUN_1402cac30(a2, a3, start, angle, full, …)`:
`p8 = p10 = p14 = 0; p11 = start; p12 = angle; a2 = max(a2, start)`; nothing more unless
`a2 < start + angle`; when not full and `start + angle < a2 + a3`, `a3 = start + angle - a2`;
nothing more unless `|((a2 + a3) - start) + angle| > 0.01`; then either
`p11 = start, p12 = a2 - start, p13 = a2 + a3, p14 = (angle - p12) - a3, p7 = a2, p8 = a3, p10 = 0`
(not full, or `a2 + a3 - 360 <= start`), or the wrapped form
`p7 = start, p8 = (a2 + a3 - 360) - start, p9 = a2, p10 = (360 + start) - p9, p11 = p7 + p8,
p12 = angle - a3, p14 = 0`. `FUN_1402caf40`: `m = (int)(nsec / (1 + (v2398/angle) * u2388))`,
zone counts `(int)(m * p12 / (angle - v2398))`, `m - that`, `(int)((nsec - m) * p8 / v2398[0])`,
`nsec - m - that`, steps = zone length / count (0 for an empty zone).

## 10. Open items

* End-to-end oracle runs need p9-01 (CNVT3D driver, FIXGEO) and p9-03 (`FUN_140153c00`,
  `FUN_140154270`); the tier-1 goldens above feed the revolve the profile those functions pass.
* `FUN_1402ce310` with `prim+0x23d8 == 0` and a node on the axis would read through a null
  pointer; batch always passes a buffer (`FUN_140154270`).
