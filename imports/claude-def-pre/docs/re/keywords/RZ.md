# RZ (id 159, group 2, case 0x0f)

Node coordinates of the object's FE mesh: (r, z) in 2D, (x, y, z) in 3D. The node count it
sets (`obj+0x880`) sizes every per-node array (DRZ, URZ, FRZ, PRZ, DRMESH, BCC…, NDTMP, …), so
RZ must come before those keywords.

## KEY grammar (read)

    RZ <obj> <nnode:%i>                 FUN_140413de0(skipBlank = 0): rest of the keyword line
    nnode lines:
      3D:  <i:%i> <x:%Flf> <y:%Flf> <z:%Flf>    FUN_140415880
      2D:  <i:%i> <r:%Flf> <z:%Flf>             FUN_140415680

* One record per line (`fgets`), commas allowed. Extra tokens are ignored, missing values are 0.
* `i` is read and ignored: nodes are stored in file order.
* A blank line inside the records gives `sscanf == -1`, which is an error.

## Model storage

* `obj+0x880` = nnode.
* `FUN_1401652b0(obj, nnode, model+0x15f8)`:
  * `FUN_140162ff0` (re)allocates the node-data block at `obj+0x8a0` (`FUN_14017ffe0`) using
    the per-variable component table at `model+0x15f8`;
  * the node temperature array `obj+0x8e0` (nnode × `model+0x1618` entries) is filled with 20.0
    when `FUN_140235c10() == 1` (global `DAT_1406fe778 == 0`, SI) and with 68.0 otherwise.
* Coordinates go to `double* obj+0x8a8`: `[i*3 + 0..2]` in 3D, `[i*2 + 0..1]` in 2D.
* RZ does not set the mesh flag `obj+0x87c`; ELMCON sets it.

## Read errors and validation

* Line 0 EOF/blank → **2**.
* Any node line EOF/blank → **2**. Nodes already read stay stored, and the count stays nnode.
* `nnode ≤ 0`: no lines are read. Status 0.
* No message of its own.

## KEY writer

`FUN_1402a2db0(model, f, obj, N)` (group2-writer-map.md step 23; also state-variable code 0 of
`FUN_1402a4a80`). With `d = obj+0x39c0`:
* 3D model, or 2D with `d == 3` → `FUN_1402a2e90`;
* 2D otherwise → `FUN_1402a2fc0`.

Both write only if `obj+0x87c != 0` (a mesh exists) and `obj+0x880 > 0`:

    "%-6s %7i %7i\n"                         RZ, N, nnode
    3D (FUN_1402a2e90): " %7i %20.10E %20.10E %20.10E\n"   i (1-based), x, y, z
    2D (FUN_1402a2fc0): " %7i %20.10E %20.10E\n"           i, r, z

Both return 0. Round-trip notes:
* A 2D object with `obj+0x39c0 == 3` is written with 3 coordinates, but the 2D reader takes
  only 2 (the third is ignored and the coordinate stride differs).
* An object with nodes but no ELMCON (`+0x87c == 0`) loses its RZ on output.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413de0` | `%i` count line |
| `FUN_140415880`, `FUN_140415680` | `%i` + 3 / 2 doubles |
| `FUN_1401652b0` | set node count, allocate node arrays, initial temperature |
| `FUN_140162ff0`, `FUN_14017ffe0` | node-data block allocation |
| `FUN_140235c10` | unit system (1 = SI) |
| `FUN_1402a2db0`, `FUN_1402a2e90`, `FUN_1402a2fc0` | writer dispatch, 3-D / 2-D writer |
| `FUN_140158ce0` | `obj+0x39c0` |

## Corpus check

* 3D: `3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY` parses: `RZ           1      44` /
  `       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000`.
* 2D: `2D\EN\plane stress\Ela_PlaneStress_EN.KEY` parses:
  `RZ           1       9` / `       1  0.0000000E+000  6.0000000E+000`.
* `3D\SI\FlowForming\Implicit_ALE\Operations\Task00001\Flowform2DMesh.KEY` (in a 3D folder) has
  2-coordinate records, so it is a 2D mesh and parses only when the model is 2D.

## Open questions

* What `obj+0x39c0` (`FUN_140158ce0`) means; it looks like an object-level dimension. Its
  setter would settle it.
