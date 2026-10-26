# DRMESH (id 263, group 2, case 0x77)

Per-node vector, 3D only, stored in `double* obj+0x930` with component count
`m = int model+0x165c` (entry +0x64 of the component table at `model+0x15f8`). The name and the
position right after DRZ in the writer suggest a mesh displacement. The record format is that of
[DRZ](DRZ.md) in reader mode 0.

## KEY grammar (read)

3D model only (`FUN_1400015f0`). In 2D the case does nothing and returns 0: the rest of the
keyword line and any records are **not** consumed, so record lines would then be parsed as
keywords.

`FUN_140416c90(f, obj+0x930, n = obj+0x880, &model+0x165c, mode 0)`:

    DRMESH <obj> <count:%i> [<d:%Flf>]       rest of the keyword line
    count records (fscanf):  <node:%i> <v1:%Flf> ... <vm:%Flf>
    then one fgets (only if count > 0)

If a default is present it is written to every component of every node. A record whose node is
outside 1..n is dropped.

## Model storage

`obj+0x930[(node-1)*m + j]` (node block allocated by RZ).

## Read errors and validation

As for DRZ (mode 0): header EOF or `sscanf < 1` → **2**; record or final `fgets` EOF → **2**.
In 2D the status is always 0.

## KEY writer

No registry handler (handler2 is the no-op). Group2-writer-map.md step 25: in 3D,
`FUN_1402a4a80(model, f, obj, 0x19, N, 0)`. Case `6`/`0x19` re-tests 3D and calls
`FUN_1402f5c10(f, "DRMESH", N, obj+0x930, n, m, 0.0, fmt 0)`:

* uniform: `"%-6s %7i %7i %20.10E\n"` → DRMESH, N, 0, value[0];
* otherwise the same header with c (nodes with any component ≠ 0.0) and 0.0, then `" %7i"` node
  plus m × `" %20.10E"` per such node (`FUN_1402f2e80`, 5 per line);
* nothing when n, m or the array is 0.

## Unit conversion

None.

## Helpers

`FUN_140416c90`, `FUN_1404199a0`, `FUN_1402a4a80` (code 0x19, and code 6 as an alias),
`FUN_1402f5c10`, `FUN_1402f2e80` (see DRZ.md).

## Corpus check

`3D\EN\NAFEMS_Benchmarks\NAFEMS_T2.KEY` line 185: `DRMESH       1       0    0.0000000000E+000`
parses. All 270 occurrences (164 files) are in 3D files.

## Open questions

* Physical meaning (mesh displacement vs. reference) is not confirmed by any string.
* Setter of `model+0x165c`.
