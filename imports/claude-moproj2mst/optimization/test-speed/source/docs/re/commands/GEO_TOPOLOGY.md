# 2D GEO topology section `FUN_140249700(geo, f, obj)`

Task T8-4 (packet p8-09). Read by the 2D GEO reader `FUN_1402478f0` (GFREAD_STL_GEO_NAS.md
§5.2, also the DEFAMG 2D GEO reader of BRDEXT.md) after it met a record whose type is `99`,
**after** the curves were stored (`FUN_14024bac0(geo, obj, 4)`), with the file positioned on
the line after the `99` token (the rest of the `99` line was consumed by the token reader
`FUN_140411ad0`, which reads whole lines). Specified from the listing
(`deform-rea-work/out/p809/all_l.txt`, 140249700..14024a38d, frame 0x258 with a stack cookie).
Port: `port/defcmd/src/geolib/amggeo.cpp`. Oracle: `gfread_geo2d_topo`.

## Arguments and set-up

* RCX `geo` (`_DEF_AMGGEO`, only passed on), RDX `f` (`FILE*`), R8 `obj` (`LLOBJ_DATA*`). No
  return value.
* `f == NULL` → return.
* `geom = FUN_14024ae10(geo, obj)`: NULL when `obj` is NULL, when `FUN_1401983e0(obj)` (the
  geometry count) is 0 or negative, else `FUN_140197f40(obj, 0)`. `geom == NULL` → return.
* `FUN_140284690(geom)` clears faces, loops, every edge's loop list and the vertices;
  `FUN_1402847d0(geom, &0)` sets `geom+0xb8 = 0` (and the per-edge flag), exactly as the
  TPLOGY reader (TPLOGY.md). The curves (edges) the store just created stay.
* All counters and ids are zeroed first (no effect on the result).

## Grammar

Every line is read with the KEY line readers (common.md: `fgets`, commas to blanks, `sscanf`;
nonzero return = EOF, blank line or too few values). **Every nonzero return ends the function at
once**, with whatever was linked so far; nothing is printed and no status is returned (the
reader's result, from the store, is unaffected).

```
<any line>                               FUN_140413c60(f, buf, 200)   (one extra line vs TPLOGY)
<nV> <nE> <nL> <nF>                       FUN_140414100 "%i %i %i %i"
    create nV vertices (FUN_140283a30), then nL loops (FUN_140283980), then nF faces
    (FUN_1402838d0); ids 1..n in creation order. No edge is created.
<comment line>                            FUN_140413c60
nV × { <id> <k> <x> <y>                   FUN_140415ff0 "%i %i %Flf %Flf"
       k × <edgeId> }                     FUN_140413de0(f, &v, 0) "%i", one per line
<comment line>
nE × { <id> <k> <v1> <v2>                 FUN_140414100
       k × <loopId> }
<comment line>
nL × { <id> <ke> <kf>                     FUN_140413fb0(f, a, b, c, NULL) "%i %i %i"
       ke × <edgeId>
       kf × <faceId> }
<comment line>
nF × { <id> <k> <attr>                    FUN_140413fb0
       k × <loopId> }
```

## Storage (per record, in listing order)

Entities are addressed **by index id-1** (`FUN_1402840e0` vertex, `FUN_1402800a0` edge (index by
value), `FUN_140284030` loop, `FUN_140283f80` face; each returns NULL outside 0..count-1). Links
name entities **by id**, matched by a linear scan of the first nV / nE / nL / nF entities in list
order; every match is appended (duplicates included).

| Record | Effect |
|---|---|
| vertex | `v = vertex(id-1)`; `FUN_14041cfc0(v, &x, &y)` (`v+0x08 = x`, `v+0x10 = y`); for each edgeId, for e in 0..nE-1: `ed = edge(e)`, if `edgeId == ed+0x30` (`FUN_14041c610`) → `FUN_14041d0b0(v, ed)` (append to `v+0x18`) |
| edge | `ed = edge(id-1)`; for i in 0..nV-1: `vi = vertex(i)`; if `v1 == vi+0x24` (`FUN_140256770`) → `ed+0x18 = vi` (`FUN_140256730`); if `v2 == vi+0x24` → `ed+0x20 = vi` (`FUN_140256750`); then for each loopId, for l in 0..nL-1: if `loopId == loop(l)+0x18` (`FUN_140256720`) → `FUN_14041c6c0(ed, loop)` (append to `ed+0x10`) |
| loop | `L = loop(id-1)`; for each edgeId, for e in 0..nE-1: if `edgeId == edge(e)+0x30` → `FUN_1404121e0(L, edge)` (`L+0x08`); for each faceId, for i in 0..nF-1: if `faceId == face(i)+0x44` (`FUN_140256710`) → `FUN_140412440(L, face)` (`L+0x10`) |
| face | `F = face(id-1)`; `FUN_1402f7f90(F, &attr)` (`F+0x40 = attr`); for each loopId, for l in 0..nL-1: if `loopId == loop(l)+0x18` → `FUN_1402f7e80(F, loop)` (`F+0x08`) |

## No checks: where the original faults

Nothing is range-checked. With an id outside the created entities the index lookup returns NULL
and the original faults (access violation, the process ends) **at the first use of that NULL**,
not at the lookup:

* vertex record: at once (`FUN_14041cfc0` stores through `v`);
* any scan over the edges (vertex and loop records) when nE in the header is larger than the
  number of curves: `FUN_14041c610` reads `NULL+0x30`;
* edge record with a bad id: only when v1 or v2 matches a vertex id, or a loopId matches a loop
  (the setters store through `ed`); otherwise the record and its k loop lines are read and
  nothing happens. (The TPLOGY reader instead skips the record without reading its loop lines.)
* loop record with a bad id: only when an edgeId or faceId matches;
* face record with a bad id: at once (`FUN_1402f7f90`). (TPLOGY returns error 2 here.)

The port ends the process with `STATUS_ACCESS_VIOLATION` at those points
(`def::rt::abnormal_exit`, defrt/stdlib/crt_emulation.hpp).

## Differences from the TPLOGY keyword reader `FUN_140399120`

1. One extra line before the counts line (the TPLOGY counts are on the keyword line).
2. No status: read errors return silently; no geometry → nothing (TPLOGY: 3 / 2).
3. Bad edge ids in edge records do not skip the loop lines; bad face ids are not an error
   (see above).
4. The geometry test is `FUN_1401983e0(obj) > 0` via `FUN_14024ae10`.

## KEY output

KFWRIT writes the topology read here with the TPLOGY writer (TPLOGY.md); with two or more curves
and no `99` section no TPLOGY is written (GFREAD_STL_GEO_NAS.md §5.3). Case `gfread_geo2d_topo`:
two curves, 2 vertices, 2 edges, 1 loop, 1 face written back as in the file.
