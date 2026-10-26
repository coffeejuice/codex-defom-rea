# MTLGRP (id 174, group 2, case 0x1e)

Material group number of each element: an integer array `obj+0x9f8` with
`m = model+0x16ec` integers per element (entry +0xf4 of the component table at `model+0x15f8`).
After reading, the object's material group `obj+0x3980` is recomputed.

## KEY grammar (read)

`FUN_140419d40(f, obj+0x9f8, n = obj+0x884, m = model+0x16ec)`:

    MTLGRP <obj> <count:%i> [<default:%i>]       rest of the keyword line (sscanf "%i %i", no comma replacement)
    count records (fscanf):  <elem:%i> <g1:%i> ... <gm:%i>
    then one fgets

* If the default is present, all n×m entries are set to it.
* A record is stored at `[(elem-1)*m + j]` only if `1 ≤ elem ≤ n`; other records are read and
  dropped. n is the ELMCON element count, so ELMCON must come first.

## Model storage

* `int* obj+0x9f8`, allocated by ELMCON (`FUN_1401653b0`). ELMCON pre-fills it with
  `obj+0x3980` when that is > 0.
* `FUN_140202960(obj)` then runs: it saves `g = obj+0x3980`, sets `obj+0x3980 = 0` and calls
  `FUN_1401fb270(obj, &nGroups, &grp)`, which goes to `FUN_1401fb440` because `+0x3980 < 1`.
  `FUN_1401fb440`:
  * if `obj+0x87c != 0` and the array exists, it counts how often each value occurs in
    `obj+0x9f8[0..nelem-1]` (only the first component of each element when m = 1; it indexes
    by element, not by element × m);
  * grp = the most frequent value; on ties the value seen first wins;
  * otherwise grp = 0.

  Result: `obj+0x3980` = the most frequent group, or the old value `g` when that group is 0 or
  there is no mesh.
* The status of `FUN_140419d40` is the case result.

## Read errors and validation

* Header EOF or blank (`sscanf == -1`) → **2**.
* A non-numeric count (`sscanf == 0`) is **not** rejected: the count is then an uninitialised
  stack value (`local_18` in `FUN_140419d40`). Treat it as undefined; a port should reject it.
* Record EOF → **2**. Final `fgets` EOF → **2**.
* `FUN_140202960` runs even when reading fails.

## KEY writer

Group2-writer-map.md step 37: right after ELMCON, `FUN_1402a4a80(model, f, obj, 0x3d, N, 0)`
(code 0x30 is an alias) → `FUN_1402f4df0(f, "MTLGRP", N, obj+0x9f8, n = obj+0x884, m, def = 1)`:

* Nothing if n == 0, m == 0 or the array is NULL; returns 2 if `f` is NULL.
* All n×m values equal value[0] → `"%-6s %7i %7i %7i\n"` → MTLGRP, N, 0, value[0]
  (e.g. `MTLGRP       1       0       1`).
* Otherwise `"%-6s %7i %7i %7i\n"` → MTLGRP, N, c, 1, where c = number of elements with any
  value ≠ 1. Then for each such element: `" %7i"` elem (1-based) and the m values through
  `FUN_1402f2fe0(f, m, vals, 10, 0)`: `" %7i"` each, with `"\n"` + 8 spaces before every 10th
  after the first, and a final `"\n"`.

This round-trips: the default 1 fills the array and the records override it.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140419d40` / `FUN_140419ea0` | integer per-element reader (header / records) |
| `FUN_140202960`, `FUN_1401fb270`, `FUN_1401fb440` | recompute `obj+0x3980` as the most frequent group |
| `FUN_1402a4a80` (code 0x3d) | state-variable writer |
| `FUN_1402f4df0` | element integer writer |
| `FUN_1402f2fe0` | integer value-line writer |

## Corpus check

* `2D\EN\plane stress\Ela_PlaneStress_EN.KEY` and `3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY`:
  `MTLGRP       1       0       1` parses (uniform form).
* `3D\SI\FlowForming\Implicit_ALE\Operations\Task00001\Flowform2DMesh.KEY`:
  `MTLGRP       1      50` with no default, then 50 records `       1       2`. This parses:
  no fill, then 50 records with m = 1.

## Open questions

* None for reading or writing. Where `model+0x16ec` (m) is set is not located; the corpus
  always implies m = 1.
