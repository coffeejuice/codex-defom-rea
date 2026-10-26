# URZ (id 165, group 2, case 0x15)

Nodal velocity vector: one vector per node, stored in `double* obj+0x8b8`, with component count
`m = int model+0x1600` (entry +0x08 of the component table at `model+0x15f8`). The format is
the same as [DRZ](DRZ.md); only the array, the table entry and the reader mode differ.

## KEY grammar (read)

`FUN_140416c90(f, obj+0x8b8, n = obj+0x880, &model+0x1600, mode 6)`:

    URZ <obj> <count:%i> [<d1:%Flf> ... <d10:%Flf>]        rest of the keyword line (sscanf, no comma replacement)
    count records (fscanf, free format):  <node:%i> <v1:%Flf> ... <vm:%Flf>
    then one fgets (only if count > 0)

Mode 6: the header may carry up to 10 defaults; if their number equals m each node gets the vector, otherwise every component gets d1. Records are read as in mode 0. `m > 10` → **2** before anything is read. A record whose node is outside 1..n (n = RZ node count) is read and dropped. DRZ.md has
the full rules.

## Model storage

`obj+0x8b8[(node-1)*m + j]`, allocated with the node block by RZ (`FUN_1401652b0`). The reader
does not change `model+0x1600`.

## Read errors and validation

As for DRZ: header EOF or `sscanf < 1` → **2**; record EOF → **2**; final `fgets` EOF → **2**.
Non-numeric record tokens are not detected.

## KEY writer

Registry handler `FUN_1402b5040` (group2-writer-map.md step 28) → `FUN_1402a4a80(model, f, obj, 2, N, 0)`
→ `FUN_1402f5c10(f, "URZ", N, obj+0x8b8, n, m, default 0.0, fmt 0)`:

* uniform (all n×m values equal value[0]): `"%-6s %7i %7i %20.10E
"` → URZ, N, 0, value[0];
* otherwise `"%-6s %7i %7i %20.10E
"` → URZ, N, c, 0.0, where c = number of nodes with any
  component ≠ 0.0. Then, for each such node, `" %7i"` node (1-based) and the m values as
  `" %20.10E"` (`FUN_1402f2e80`: 5 per line, continuation `"
"` + 8 spaces), then `"
"`;
* nothing when n or m is 0 or the array is NULL.

## Unit conversion

None.

## Helpers

* `FUN_140416c90`: reader.
* `FUN_1404199a0` / `FUN_140419b90`: record readers.
* `FUN_1402b5040`: write handler (variable code 2).
* `FUN_1402a4a80`, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80`: writer chain, see DRZ.md.

## Corpus check

`2D\EN\plane stress\Ela_PlaneStress_EN.KEY`: `URZ          1       3  0.0000000E+000` / `       1  0.0000000E+000  1.0000000E-001` parses (3 records, m = 2).

## Open questions

* The setter of `model+0x1600` (the component count) is not located; see DRZ.md.
