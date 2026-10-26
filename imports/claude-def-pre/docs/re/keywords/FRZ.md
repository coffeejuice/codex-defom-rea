# FRZ (id 166, group 2, case 0x16)

Nodal force vector: one vector per node, stored in `double* obj+0x8c0`, with component count
`m = int model+0x1604` (entry +0x0c of the component table at `model+0x15f8`). The format is
the same as [DRZ](DRZ.md); only the array, the table entry and the reader mode differ.

## KEY grammar (read)

`FUN_140416c90(f, obj+0x8c0, n = obj+0x880, &model+0x1604, mode 0)`:

    FRZ <obj> <count:%i> [<d:%Flf>]        rest of the keyword line (sscanf, no comma replacement)
    count records (fscanf, free format):  <node:%i> <v1:%Flf> ... <vm:%Flf>
    then one fgets (only if count > 0)

Mode 0: header `"%i %Flf"` (count, one optional default). If the default is present, every component of every node is set to it. A record whose node is outside 1..n (n = RZ node count) is read and dropped. DRZ.md has
the full rules.

## Model storage

`obj+0x8c0[(node-1)*m + j]`, allocated with the node block by RZ (`FUN_1401652b0`). The reader
does not change `model+0x1604`.

## Read errors and validation

As for DRZ: header EOF or `sscanf < 1` → **2**; record EOF → **2**; final `fgets` EOF → **2**.
Non-numeric record tokens are not detected.

## KEY writer

Registry handler `FUN_1402b5100` (group2-writer-map.md step 28) → `FUN_1402a4a80(model, f, obj, 3, N, 0)`
→ `FUN_1402f5c10(f, "FRZ", N, obj+0x8c0, n, m, default 0.0, fmt 0)`:

* uniform (all n×m values equal value[0]): `"%-6s %7i %7i %20.10E
"` → FRZ, N, 0, value[0];
* otherwise `"%-6s %7i %7i %20.10E
"` → FRZ, N, c, 0.0, where c = number of nodes with any
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
* `FUN_1402b5100`: write handler (variable code 3).
* `FUN_1402a4a80`, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80`: writer chain, see DRZ.md.

## Corpus check

`2D\EN\plane stress\Ela_PlaneStress_EN.KEY`: `FRZ          1       0  0.0000000E+000` parses (uniform form).

## Open questions

* The setter of `model+0x1604` (the component count) is not located; see DRZ.md.
