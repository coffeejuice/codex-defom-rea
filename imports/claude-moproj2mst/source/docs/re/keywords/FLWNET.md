# FLWNET (id 290, group 2, case 0x92)

A second surface mesh (nodes and 4-node facets) in the surface object at `obj+0x3458`, with the
same format as [FLDGEO](FLDGEO.md). The name suggests a "flow net"; the code gives no meaning.

## KEY grammar (read)

Both dimensions:

    FLWNET <obj> <a:%i> <nnode:%i> <nelem:%i>    FUN_140413fb0: a is ignored
    nnode lines  <i:%i> <x:%Flf> <y:%Flf> <z:%Flf>
    nelem lines  <i:%i> <n1> <n2> <n3> <n4>        (a negative n4 is replaced by -n4)

The rules are those of FLDGEO.md (`FUN_14039a800` with `fixNeg = 1`).

## Model storage

* `surf = obj+0x3458`: node vector `+0x10` (3 doubles per node), facet vector `+0x18`
  (4 ints per facet), plus `+0x20` and `+0x68` sized by `FUN_1402fcd90`.
* After a successful read: if `FUN_1402f9360(surf)` (non-empty), it calls `FUN_1402fbb30(surf)`,
  which is an empty function in this build.

## Read errors and validation

Header, node or facet line EOF/blank → **2**; otherwise **0**.

## KEY writer

3D only, right after FLDGEO (group2-writer-map.md step 22): if `FUN_140160eb0(obj)` (surface
non-empty), it calls `FUN_1402a2b70(model, f, "FLWNET", obj+0x3458, N)`:

    "%-6s %7i %7i %7i %7i\n"                   FLWNET, N, 1, nnode, nelem
    " %7i %20.10E %20.10E %20.10E\n"           × nnode
    " %7i %7i %7i %7i %7i\n"                   × nelem

## Unit conversion

None.

## Helpers

`FUN_140413fb0`, `FUN_14039a800`, `FUN_1402fcd90`, `FUN_1402f9360`, `FUN_1402fbb30` (no-op),
`FUN_140160eb0`, `FUN_1402a2b70` (see FLDGEO.md).

## Corpus check

No corpus file uses FLWNET.

## Open questions

* Physical meaning. `FUN_1402fbb30` is empty here, so it was perhaps a post-processing hook
  in other builds.
