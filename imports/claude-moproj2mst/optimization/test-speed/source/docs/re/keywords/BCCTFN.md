# BCCTFN (id 169, group 2, case 0x19)

Per-node integer companion to `BCCTMP` (`obj+0x9a8`, one int per node; by its name the thermal BC function number). Only stored and written.

## KEY grammar (read)

```
BCCTFN <objNo:%i> <count:%i> [<default:%i>]
[count records:  <node:%i> <v1:%i> ... <vm:%i>]
```

* After the object number, the rest of the keyword line is read by `fgets` and parsed with `sscanf("%i %i")` (**no** comma replacement, unlike the scalar line helpers). `count` is required; `default` is optional; extra text on the line is ignored.
* `m` values per record: 1 (see Model storage).
* Records are read with `fscanf` (`%i` index, then `m` x `%i`), free format: they may be split over or joined on lines in any way; `%i` accepts hex/octal (`0x...`, leading `0`).
* After the last record one `fgets` discards the rest of that line. With `count < 1` nothing after the keyword line is read (no extra `fgets`).
* Same in 2D and 3D except for `m`.

## Model storage

* Array `int[n x m]` at pointer `obj+0x9a8`; `n` = `obj+0x880` (node count); node `i` (1-based) component `j` at `[(i-1)*m + j]`. The reader does not allocate; it writes into the existing buffer.
* `m` = `model+0x164c` = 1. Set by `FUN_1401d09c0(model, dim)` (dimension change) and `FUN_1401d0320(model)`; both first set the 0x4f ints from `model+0x15f8` to 1, then the dimension-dependent ones.
* If `default` is present (2 conversions) all `n x m` entries are set to it first; otherwise existing values are kept.
* Each record overwrites node `index` if `1 <= index <= n`; other indices are read and dropped.

## Read errors and validation

Helper `FUN_140419d40` -> `FUN_140419ea0`; any nonzero helper status -> parser returns **2** ("Reading Error" printed, the KEY loop continues; `_common.md` section 1).

* object number EOF or `< 1` -> 2 (group2-helpers.md); a missing object is created;
* `fgets` EOF on the keyword line -> 2; line empty/blank (`sscanf == -1`) -> 2;
* first token not a number (`sscanf == 0`): `count` is an **uninitialised** stack int (undefined behaviour; a port should treat it as 0) and no default is applied;
* `fscanf` EOF inside the records -> 2 (values already stored stay); EOF on the final `fgets` -> 2;
* a non-numeric token inside the records is not detected (`fscanf` returns 0, the slot keeps the temp buffer's previous/0 value, the token stays in the stream and breaks the following reads);
* no range check on the values; the reader prints nothing.

## KEY writer

direct call `FUN_1402a4a80(model, f, obj, 0x15, N)` in `FUN_14029e250` (writer-map step 30, right after `BCCTMP`). Case 0x15 of `FUN_1402a4a80` calls the int line writer `FUN_1402f4df0(f, FUN_1402f7720(169), N, obj+0x9a8, n = obj+0x880, m = model+0x164c, default = 0)`. `N` is the output object number (`obj+0x3f24` if > 0, else `obj+8`).

* Nothing is written if `n == 0`, `m == 0` or the array pointer is NULL (status 0).
* If all `n x m` entries equal the first one `v`: one line
  `"%-6s %7i %7i %7i\n"` = name, `N`, `0`, `v`.
* Otherwise: header `"%-6s %7i %7i %7i\n"` = name, `N`, `k`, `0`, where `k` = number of nodes with any component `!= 0`; then for each such node in index order `" %7i"` (1-based node) followed by `FUN_1402f2fe0(f, m, values, 10, 0)`: `" %7i"` per value (a line break plus 8 spaces before value 11, 21, ...; never reached since `m <= 3`), then `"\n"`.
* Status 0 (2 only for a NULL `FILE*`).

`FUN_14029e250` ignores the status of this direct call. The object part is written only for objects whose type `obj+0xc` is neither 7 (empty) nor 5 (group2-writer-map.md steps 5a/5b), and only if all earlier status-checked handlers returned 0.

Round trip: exact (the default-0 encoding and the uniform shortcut both read back to the same array).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x19 (p_main.c lines 1407-1410): reader dispatch.
* `FUN_140419d40(f, int *dst, n, m)`: header `"%i %i"`, default fill, calls `FUN_140419ea0`.
* `FUN_140419ea0(f, dst, count, n, m)`: `count` records `%i` + `m` x `%i`, final `fgets`.
* `FUN_1402a4a80` case 0x15: state-variable writer dispatch.
* `FUN_1402f4df0`: int node-array writer (formats above); `FUN_1402f2fe0`: int value-line writer.
* `FUN_1402f7720(169)`: keyword name.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 780 occurrences of BCCTFN.

* `C:/Users/alext/Projects/DEFORM_corpus/key/2D/EN/plane stress/Ela_PlaneStress_EN.KEY` (uniform 0): parses exactly with the grammar above.

```
BCCTFN       1       0       0
```

* `C:/Users/alext/Projects/DEFORM_corpus/key/2D/EN/NAFEMS_benchmarks/NAFEMS_T3_EN.KEY` (5 records): parses exactly with the grammar above.

```
BCCTFN       1       5       0
     101       1
     102       1
```

## Open questions

* The reader writes into `obj+0x9a8` without allocating; which function allocates/resizes it when the node count changes (`RZ`), and whether a `BCCTFN` placed before `RZ` survives, is not settled (decompile the object's node-count setter).
