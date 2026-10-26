# ELMCON (id 173, group 2, case 0x1d)

Element connectivity of the object's FE mesh: 4-node quadrilaterals in 2D; 4-node tetrahedra or
8-node bricks in 3D. Reading it marks the object as meshed (`obj+0x87c = 1`), which the RZ and
ELMCON writers require, and builds the boundary data.

## KEY grammar (read)

    3D:  ELMCON <obj> <nelem:%i> <npe:%i>      FUN_140413ee0 ("%i %i"), npe must be 4 or 8
    2D:  ELMCON <obj> <nelem:%i>               FUN_140413de0 (skipBlank 0); further tokens ignored
    nelem lines:
      2D, and 3D with npe 4:  <e:%i> <n1> <n2> <n3> <n4>                  FUN_140414200, %i×5
      3D with npe 8:          <e:%i> <n1> ... <n8>                        FUN_140414600, %i×9

* Element lines are line based (`fgets` + `sscanf`, commas allowed), and blank lines are **not**
  skipped (`skipBlanks = 0`, so a blank line → error). Missing integers are 0.
* `e` is ignored; elements are stored in file order. Node numbers are 1-based and stored as
  given.
* The reader does no range or consistency check on node numbers.

## Model storage

* 3D: `FUN_140165500(obj, k)` with k = 2 for npe 4 and k = 4 for npe 8:
  * `obj+0x890 = k`;
  * k = 2 (tet): `+0x894 = 4` (nodes/element), `+0x898 = 4` (faces/element), `+0x89c = 3`
    (nodes/face);
  * k = 4 (brick): `+0x894 = 8`, `+0x898 = 6`, `+0x89c = 4`.

  In 2D `+0x890..+0x89c` are not touched.
* `obj+0x884 = nelem`.
* `FUN_1401653b0(obj, nelem, model+0x15f8)`:
  * `FUN_140163090` allocates the element-data block `obj+0x9e0` (`FUN_1401836c0`) with
    `+0x894` nodes per element, or with 4 when the model is 2D and `obj+0x39c0 != 3`;
  * if `obj+0x3980 > 0`, the material-group array `obj+0x9f8` (nelem × `model+0x16ec`) is
    filled with `obj+0x3980`;
  * `obj+0xa30` (nelem × `model+0x16a8`) is filled with 1.0 and `obj+0xa38`
    (nelem × `model+0x16ac`) with 0.0.
* Connectivity goes to `int* obj+0x9e8`, at `[e*4 + 0..3]` (2D, tet) or `[e*8 + 0..7]` (brick).
* After the loop: `obj+0x87c = 1`. If the model byte `+0x1b5c` is 0 (`FUN_14039aa20`),
  `FUN_1401fc290(obj)` builds the boundary:
  * 2D: boundary node chains, up to 100, in `obj+0xbd8` (count) and the 0x30-byte records at
    `obj+0xbe0…`;
  * 3D: boundary faces into `obj+0xbf8` / `obj+0xc00` (`FUN_140354ed0`, `FUN_140200a70`);
  * then `FUN_140165d30` and `FUN_1401631e0`.

## Read errors and validation

* Line 0 EOF/blank → **2**.
* 3D `npe` not 4 or 8 → **2**, before anything is stored.
* An element line EOF/blank → **2**. `+0x884` stays nelem and `+0x87c` is not set.
* `nelem ≤ 0`: no lines are read; `+0x87c = 1` anyway and the boundary is built.
* No messages.

## KEY writer

`FUN_1402a30c0(model, f, obj, N)` (group2-writer-map.md step 36). It writes nothing unless
`obj+0x87c != 0` and `obj+0x884 != 0`. With `d = obj+0x39c0`:

* 3D model, or 2D with `d == 3` → `FUN_1402a32f0`:
  * `+0x890 == 2`: `"%-6s %7i %7i %7i\n"` → ELMCON, N, nelem, 4; then per element
    `" %7i %7i %7i %7i %7i\n"` with e (1-based), n1..n4;
  * `+0x890 == 4`: header with 8; per element `" %7i"` × 9 + `"\n"`;
  * any other `+0x890`: nothing.
* 2D otherwise: `"%-6s %7i %7i\n"` → ELMCON, N, nelem; per element
  `" %7i %7i %7i %7i %7i\n"`.

Returns 0, or 3 if `f` or `obj` is NULL; `FUN_14029e250` ignores the status. It is followed
immediately by MTLGRP (`FUN_1402a4a80(…, 0x3d)`).

Round-trip notes:
* A 2D model with `d == 3` gets the 3D header with npe; the 2D reader ignores npe and reads
  5 integers per line, which fails for bricks.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413ee0`, `FUN_140413de0` | header line |
| `FUN_140414200`, `FUN_140414600` | 5 / 9 integer element lines |
| `FUN_140165500` | element-type fields `+0x890..+0x89c` |
| `FUN_1401653b0`, `FUN_140163090`, `FUN_1401836c0` | element count and element-array allocation and defaults |
| `FUN_14039aa20` | model byte `+0x1b5c` (skip boundary build) |
| `FUN_1401fc290` | boundary extraction |
| `FUN_1402a30c0`, `FUN_1402a32f0` | writers |

## Corpus check

* 2D: `2D\EN\plane stress\Ela_PlaneStress_EN.KEY`: `ELMCON       1       4` /
  `       1       6       3       2       7`.
* 3D brick: `3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY` line 245: `ELMCON       1      10       8` /
  `       1       1       3       4       2       5       7       8       6`.
* Header shapes over the corpus: 3D has 180 headers with npe 4 and 125 with npe 8; 2D has 723
  headers with 2 integers. Two files in 3D folders (e.g. `Flowform2DMesh.KEY`) carry a 2D mesh.
  All parse.

## Open questions

* The 2D boundary chains' consumer, and the `+0xbe0` record layout, are not documented here;
  they are not needed for reading or writing.
