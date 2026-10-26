# PRZ (id 167, group 2, case 0x17)

Per-node vector `PRZ` (array `obj+0x8d0`; no string in the code names it): one vector per node, stored in `double* obj+0x8d0`, with component count
`m = int model+0x160c` (entry +0x14 of the component table at `model+0x15f8`). The format is
the same as [DRZ](DRZ.md); only the array, the table entry and the reader mode differ.

## KEY grammar (read)

`FUN_140416c90(f, obj+0x8d0, n = obj+0x880, &model+0x160c, mode 0)`:

    PRZ <obj> <count:%i> [<d:%Flf>]        rest of the keyword line (sscanf, no comma replacement)
    count records (fscanf, free format):  <node:%i> <v1:%Flf> ... <vm:%Flf>
    then one fgets (only if count > 0)

Mode 0: header `"%i %Flf"` (count, one optional default). If the default is present, every component of every node is set to it. A record whose node is outside 1..n (n = RZ node count) is read and dropped. DRZ.md has
the full rules.

## Model storage

`obj+0x8d0[(node-1)*m + j]`, allocated with the node block by RZ (`FUN_1401652b0`). The reader
does not change `model+0x160c`.

## Read errors and validation

As for DRZ: header EOF or `sscanf < 1` → **2**; record EOF → **2**; final `fgets` EOF → **2**.
Non-numeric record tokens are not detected.

## KEY writer

Registry handler `FUN_1402b51c0` (group2-writer-map.md step 29) → `FUN_1402a4a80(model, f, obj, 5, N, 0)`
→ `FUN_1402f5c10(f, "PRZ", N, obj+0x8d0, n, m, default 0.0, fmt 0)`:

* uniform (all n×m values equal value[0]): `"%-6s %7i %7i %20.10E
"` → PRZ, N, 0, value[0];
* otherwise `"%-6s %7i %7i %20.10E
"` → PRZ, N, c, 0.0, where c = number of nodes with any
  component ≠ 0.0. Then, for each such node, `" %7i"` node (1-based) and the m values as
  `" %20.10E"` (`FUN_1402f2e80`: 5 per line, continuation `"
"` + 8 spaces), then `"
"`;
* nothing when n or m is 0 or the array is NULL.
* **The writer emits PRZ only in 3D**: `FUN_14029e250` calls the handler only after an `FUN_1400015f0` test. The reader accepts PRZ in 2D too.

## Unit conversion

None.

## Helpers

* `FUN_140416c90`: reader.
* `FUN_1404199a0` / `FUN_140419b90`: record readers.
* `FUN_1402b51c0`: write handler (variable code 5).
* `FUN_1402a4a80`, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80`: writer chain, see DRZ.md.

## Corpus check

`3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY`: `PRZ          1       0    0.0000000000E+000` parses.

## Open questions

* The setter of `model+0x160c` (the component count) is not located; see DRZ.md.
