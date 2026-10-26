# DRZ (id 160, group 2, case 0x10)

Nodal displacement vector, one per node, in `obj+0x8b0`. Its component count is the model's
table entry `model+0x15fc` (2 in 2D, 3 in 3D according to the corpus). This file also documents
the per-node double reader and writer line formats that URZ, FRZ, PRZ, DRMESH and MATAXI share.

## KEY grammar (read)

`FUN_140416c90(f, obj+0x8b0, n = obj+0x880, &model+0x15fc, mode 6)`:

    DRZ <obj> <count:%i> [<d1:%Flf> … <d10:%Flf>]      rest of the keyword line (sscanf, no comma replacement)
    count records, read with fscanf (free format):  <node:%i> <v1:%Flf> … <vm:%Flf>      m = model+0x15fc
    then one fgets (the rest of the last record's line)

* Header: `nd` = number of defaults read. If `nd > 0`: when `nd == m`, every node gets the
  default vector; otherwise every component of every node gets `d1`. With `nd == 0` the array is
  not touched (only the records change it).
* Mode 6 reads the records like mode 0 (`FUN_140419b90` with k = 0 → `FUN_1404199a0`). The only
  difference from mode 0 is that the header may carry up to 10 defaults.
* A record is stored at `dst[(node-1)*m + j]` only when `1 ≤ node ≤ n`. Records outside that
  range are read and dropped.
* `count ≤ 0`: no records and no trailing `fgets`.
* The header must be on the keyword line: `DRZ 1\n` followed by `0 0.0` on the next line gives
  `sscanf == -1` → error.

## Model storage

* `double* obj+0x8b0`, `n × m` values, allocated by RZ (`FUN_1401652b0` → node block
  `obj+0x8a0`).
* `m` = `int model+0x15fc`, which is entry +4 of the component table at `model+0x15f8`. The
  reader does not change it.
* Without a prior RZ, n = 0: defaults touch nothing and every record is dropped.

## Read errors and validation

* `m > 10` → **2** before anything is read (mode 6 check).
* Header line EOF, or `sscanf < 1` (blank line or non-numeric count) → **2**.
* Record `fscanf` EOF → **2**. A non-numeric token is not detected: the value stays as it was
  and the token stays in the stream.
* Final `fgets` EOF → **2** (only when count > 0).

## KEY writer

Registry handler `FUN_1402b4f80` (group2-writer-map.md step 24):
`FUN_1402a4a80(model, f, obj, 1, N, 0)` → `FUN_1402f5c10(f, "DRZ", N, obj+0x8b0, n, m,
default = 0.0, fmt 0, 0)`. That function is documented by worker D in group2-statevar-writers.md;
the lines it produces here are:

* Nothing at all if n == 0, m == 0 or the array is NULL.
* "Uniform" = all n×m values equal to value[0]:
  * uniform: `"%-6s %7i %7i %20.10E\n"` → DRZ, N, 0, value[0]
    (e.g. `DRZ          1       0    0.0000000000E+000`);
  * otherwise: `"%-6s %7i %7i %20.10E\n"` → DRZ, N, c, 0.0, where c = number of nodes with any
    component ≠ 0.0. Then for each such node: `" %7i"` node (1-based), then the m values through
    `FUN_1402f2e80(f, m, vals, 5, 0)`: `" %20.10E"` each, with `"\n"` + 8 spaces before every
    5th value after the first, and a final `"\n"`.
* Returns 0 (2 if `f == NULL`).

This round-trips: the reader's single default fills every component, and the records then
overwrite the non-zero nodes.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140416c90` | per-node header + records (modes 0/1/2/3/6, group2-helpers.md) |
| `FUN_1404199a0`, `FUN_140419b90` | record readers |
| `FUN_1402b4f80` | registry write handler (variable code 1) |
| `FUN_1402a4a80` | state-variable writer (switch on code) |
| `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80` | node/element double writer, uniformity/default analysis, value-line writer |

## Corpus check

`2D\EN\plane stress\Ela_PlaneStress_EN.KEY`: `DRZ          1       0  0.0000000E+000` parses
(uniform form). Non-uniform 3D, `3D\EN\Diestress\3D_DieStress_Ex1.KEY`:
`DRZ          4     163   0.0000000E+00` / `      15   3.9999996E-03  -1.8199106E-06   0.0000000E+00`
parses with m = 3.

## Open questions

* Where `model+0x15fc` (and the rest of the `model+0x15f8` table) is set. Presumably at the
  dimension switch; the setter would settle it.
