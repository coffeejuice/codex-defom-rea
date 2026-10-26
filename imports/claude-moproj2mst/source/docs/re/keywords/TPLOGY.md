# TPLOGY (id 269, group 2, case 0x7d)

Boundary-representation topology of the object's curve geometry (geometry 0 of `obj+0x3998`):
vertices (`_DEF_VERTEX`), edges (`_DEF_EDGE`), loops (`_DEF_LOOP`) and faces (`_DEF_FACE`),
with their incidences. **The edges are the DIEGEO curves**: TPLOGY creates no edges and only
links the existing ones, so it must follow the object's DIEGEO (see DIEGEO.md for the
geometry and curve storage).

## KEY grammar (read)

`FUN_140399120(model, f, obj)`, the same in 2D and 3D. Every line is one `fgets` + `sscanf`
(commas allowed, missing values 0):

    TPLOGY <obj> <nV:%i> <nE:%i> <nL:%i> <nF:%i>         FUN_140414100
    <comment line>                                        FUN_140413c60 (any text, e.g. "* Vertex Data: ")
    nV × { <id:%i> <k:%i> <x:%Flf> <y:%Flf>               FUN_140415ff0
           k × <edgeId:%i> }                              FUN_140413de0, one per line
    <comment line>                                        ("* Edge Data:")
    nE × { <id:%i> <k:%i> <v1:%i> <v2:%i>                 FUN_140414100
           k × <loopId:%i> }                              (only if edge id-1 exists, see below)
    <comment line>                                        ("* Loop Data:")
    nL × { <id:%i> <ke:%i> <kf:%i>                        FUN_140413fb0
           ke × <edgeId:%i>
           kf × <faceId:%i> }
    <comment line>                                        ("* Face Data:")
    nF × { <id:%i> <k:%i> <attr:%i>                       FUN_140413fb0
           k × <loopId:%i> }

The comment lines are consumed whatever they contain; the reader relies on the line count.
Blank lines are not skipped anywhere: a blank sub-line gives `sscanf == -1` → error.

## Model storage

* `geom = FUN_140197f40(obj, 0)`. The geometry fields used:
  * `+0x08` face list, `+0x10` loop list, `+0x18` edge list, `+0x20` vertex list;
  * `+0xb8` byte flag.
* Before the header is read:
  * `FUN_140284690(geom)` clears the face and loop lists, resets every edge
    (`FUN_140284c00`) and clears the vertex list;
  * `FUN_1402847d0(geom, 0)` sets `+0xb8 = 0` and `FUN_14041cd40(edge, 0)` on every edge.
* After the header it creates nV vertices (`FUN_140283a30`, id `+0x24` = count+1), nL loops
  (`FUN_140283980`, id `+0x18`) and nF faces (`FUN_1402838d0`, id `+0x44`), so ids are 1..n.
* Records address their entity **by index** (id-1). Links name other entities by **id**,
  matched by a linear search over the first nV/nE/nL/nF entities:
  * vertex: `+0x08` x, `+0x10` y (`FUN_14041cfc0`); each edgeId whose edge id `+0x30` matches
    is appended to the vertex edge list `+0x18` (`FUN_14041d0b0`);
  * edge (`FUN_1402800a0(geom, id-1)`): v1 → start vertex `edge+0x18`, v2 → end vertex
    `edge+0x20` (matched by vertex id); loops → `edge+0x10` (`FUN_14041c6c0`);
  * loop (`FUN_140284030`): edges → `loop+0x08`, faces → `loop+0x10`;
  * face (`FUN_140283f80`): `attr` → `face+0x40` (`FUN_1402f7f90`); loops → `face+0x08`.

## Read errors and validation

* No geometry (`FUN_1401983e0 == 0`) or geometry 0 NULL → the function returns 3 and the case
  returns **2**.
* Header, a comment line, or any record or sub-line EOF/blank → **2**.
* A face id outside 1..nF → **2**.
* Unchecked, undefined behaviour:
  * a vertex id outside 1..nV dereferences NULL;
  * a loop id outside 1..nL is used as NULL when a link matches.
* An edge id outside the existing edges skips the record **and its k loop lines are not read**,
  which desyncs everything that follows.
* Links whose target id is not found are dropped silently.
* No messages.

## KEY writer

`FUN_1402a9f90(model, f, obj, N)`:
* 2D: group2-writer-map.md step 21.
* 3D: inside `FUN_1402ab160` (called from the surface DIEGEO writer `FUN_1402a27a0` when
  geometry 0 has edges), right after the curve DIEGEO and before CSPMTX.

Conditions:
* No geometry → 3, nothing written.
* If `FUN_140158ec0(geom)` is false (no face, or face 0 has no loop):
  * more than 1 edge → return 3, **no TPLOGY**;
  * otherwise `FUN_140281bf0(geom, 0)` builds the topology automatically (when
    `FUN_140284730`), then it is written.

Output:

    "%-6s %7i %7i %7i %7i %7i\n"         TPLOGY, N, nV, nE, nL, nF
    "%s\n" "* Vertex Data: "             (note the trailing space)
    per vertex i:  " %7i %7i %20.10E %20.10E\n"   id(+0x24), k (vtable+0x10), x, y
                   k × " %7i\n"                    edge id(+0x30)
    "%s\n" "* Edge Data:"
    per edge:      " %7i %7i %7i %7i\n"            id, k (FUN_14041c5c0), start-vertex id, end-vertex id
                   k × " %7i\n"                    loop id
    "%s\n" "* Loop Data:"
    per loop:      " %7i %7i %7i\n"                id, ke (vtable+0x18), kf (FUN_1404125e0)
                   ke × " %7i\n" edge id ; kf × " %7i\n" face id
    "%s\n" "* Face Data:"
    per face:      " %7i %7i %7i\n"                id(+0x44), k (FUN_140158f10), attr (FUN_1402f7fb0)
                   k × " %7i\n"                    loop id

Entities are iterated by index. A sub-entity whose dynamic cast fails is skipped in the
sub-lines. An edge whose start or end vertex is missing is skipped entirely, while the header
still counts it. The result then does not round-trip.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140399120` | reader |
| `FUN_140413c60` | read and trim one line into a buffer (comment skip, 200 bytes) |
| `FUN_140414100`, `FUN_140415ff0`, `FUN_140413fb0`, `FUN_140413de0` | record lines |
| `FUN_140284690`, `FUN_1402847d0` | clear topology / reset flag |
| `FUN_140283a30`, `FUN_140283980`, `FUN_1402838d0` | create vertex / loop / face |
| `FUN_1402840e0`, `FUN_1402800a0`, `FUN_140284030`, `FUN_140283f80` | vertex / edge / loop / face by index |
| `FUN_140256770`, `FUN_14041c610`, `FUN_140256720`, `FUN_140256710` | vertex / edge / loop / face id |
| `FUN_14041cfc0`, `FUN_14041d0b0`, `FUN_140256730`, `FUN_140256750`, `FUN_14041c6c0`, `FUN_1404121e0`, `FUN_140412440`, `FUN_1402f7f90`, `FUN_1402f7e80` | link setters |
| `FUN_1402a9f90` | writer |
| `FUN_140158ec0`, `FUN_140281bf0` | "has faces" test / automatic topology build |

## Corpus check

* 3D: `3D\EN\Cogging\Cogging_Multiple_Ops_Lab1\Objects\Object00001.KEY` line 48 parses:
  `TPLOGY 1 1 1 1 1`, `* Vertex Data: `, `1 1 0.0E+000 0.0E+000`, `1`, `* Edge Data:`,
  `1 1 1 1`, `1`, `* Loop Data:`, `1 1 1`, `1`, `1`, `* Face Data:`, `1 1 0`, `1`.
  It follows the curve DIEGEO, which supplies the single edge.
* 2D: `2D\EN\HeatTreat\HT_CARB1_EN.KEY` line 2794: `TPLOGY 2 2 1 1 1` after a one-curve
  DIEGEO.
* A line-count simulation of the grammar over all 379 corpus occurrences ends exactly at a
  keyword or comment line every time (script check, read-only).

## Open questions

* The meaning of the face `attr` (`+0x40`); the corpus has 0.
* What `FUN_140281bf0` builds for a single open or closed curve; not needed for I/O.
