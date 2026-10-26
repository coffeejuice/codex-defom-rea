# FLDGEO (id 289, group 2, case 0x91)

A surface mesh (nodes and 4-node facets) stored in the surface object at `obj+0x3328` (built by
`FUN_1402f9020`). It uses the same class and node/facet layout as the 3D DIEGEO surface, and
FLWNET has the same format in its own surface at `obj+0x3458`. The name suggests a "fold/field
geometry"; the code gives no meaning. This file also documents the shared reader
`FUN_14039a800` and writer `FUN_1402a2b70`.

## KEY grammar (read)

Both dimensions (there is no dimension test in the case).

    FLDGEO <obj> <a:%i> <nnode:%i> <nelem:%i>    FUN_140413fb0: a is read and ignored (the writer prints 1)
    nnode lines  <i:%i> <x:%Flf> <y:%Flf> <z:%Flf>       FUN_140415880
    nelem lines  <i:%i> <n1:%i> <n2:%i> <n3:%i> <n4:%i>  FUN_140414200, skipBlanks 0

* One record per line (commas allowed, missing values 0). Indices are ignored and records are
  stored in order.
* Blank lines between records are errors.
* The 4th node of a facet: **if it is negative it is replaced by its absolute value** (the
  `fixNeg` flag is 1 for both FLDGEO and FLWNET).

## Model storage

`FUN_14039a800(model, f, surf = obj+0x3328, &nnode, &nelem, &fixNeg = 1)`:
* `FUN_1402fcd90(surf, &nnode, &nelem)`:
  * resizes the node vector `surf+0x10` (3 doubles per node) and the facet vector `surf+0x18`
    (4 ints per facet) with zero fill, each only when its count is ≥ 1 (otherwise the old
    contents stay);
  * callocs `surf+0x20` (one int per facet);
  * resizes `surf+0x68` (one int per facet).
* Node i → `surf+0x10` data `[i*3 + 0..2]`; facet i → `surf+0x18` data `[i*4 + 0..3]`.
* The loops run over the file's nnode/nelem.

## Read errors and validation

* Header EOF/blank → **2**.
* Node line EOF/blank → **2**; facet line EOF/blank → **2**.
* `nnode ≤ 0` / `nelem ≤ 0`: that section is not read and the old data stays.

## KEY writer

In `FUN_14029e250` (group2-writer-map.md step 22), **3D only**: if `FUN_140160e50(obj)` (i.e.
`FUN_1402f9360(obj+0x3328)`: node count > 0 and facet count > 0), it calls
`FUN_1402a2b70(model, f, "FLDGEO", obj+0x3328, N)`:

    "%-6s %7i %7i %7i %7i\n"                   FLDGEO, N, 1, nnode, nelem
    " %7i %20.10E %20.10E %20.10E\n"           i (1-based), x, y, z         × nnode
    " %7i %7i %7i %7i %7i\n"                   i, n1, n2, n3, n4            × nelem

It returns 0, or 3 if `f`/`surf` is NULL or the surface is empty; the status is ignored.
Round-trip: the absolute-value fix makes a negative 4th node positive on re-read. In 2D the data
is read but never written.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413fb0` | header `%i %i %i` |
| `FUN_14039a800` | surface reader (nodes, facets, negative-node fix) |
| `FUN_1402fcd90` | size the surface arrays (see DIEGEO.md) |
| `FUN_140415880`, `FUN_140414200` | node / facet lines |
| `FUN_1402f9360` | "surface non-empty" test |
| `FUN_140160e50` | `FUN_1402f9360(obj+0x3328)` |
| `FUN_1402a2b70` | surface writer (name passed in) |

## Corpus check

No corpus file uses FLDGEO.

## Open questions

* Physical meaning, and which program part consumes `obj+0x3328`.
