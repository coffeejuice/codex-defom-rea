# CRBFLX (id 221, group 2, case 0x4d)

Per-node double value (`obj+0x910`, one per node); by name the carbon flux paired with `BCCCRB`.

## KEY grammar (read)

```
CRBFLX <objNo:%i> <count:%i> [<default:%Flf>]
[count records:  <node:%i> <v1:%Flf>]
```

* The rest of the keyword line is read by `fgets` and parsed with `sscanf("%i %Flf")` (mode 0 of `FUN_140416c90`, **no** comma replacement). `count` is required, `default` optional.
* Records: `fscanf` `%i` index then 1 x `%Flf`, free format across lines. After the last record one `fgets` discards the rest of that line; `count < 1` -> nothing more is read.
* Same grammar in 2D and 3D.

## Model storage

* `double[n x m]` at pointer `obj+0x910`; `n` = `obj+0x880` (node count); `m` = `*(int*)(model+0x1630)` = 1 (set by `FUN_1401d09c0` / `FUN_1401d0320`).
* `default` present -> every one of the `n x m` entries is set to it (exactly one default is possible in mode 0, so with `m > 1` it goes to every component). No default -> existing values kept.
* A record for index `i` with `1 <= i <= n` stores its value(s) at `[(i-1)*m + j]`; other indices are read and dropped. The reader does not allocate.

## Read errors and validation

Helper `FUN_140416c90(f, obj+0x910, n, model+0x1630, 0)` -> `FUN_1404199a0`; nonzero -> **2**.

* object number EOF / `< 1` -> 2 (a missing object is created);
* keyword-line `fgets` EOF -> 2; `sscanf < 1` (blank line or non-numeric first token) -> 2;
* EOF inside the records or on the final `fgets` -> 2 (already stored values stay);
* a non-numeric token inside the records is not detected (value not stored, stream desynchronised);
* the reader prints nothing.

## KEY writer

registry handler2 `FUN_1402b6240` -> `FUN_1402a4a80(model, f, obj, 0x0e, N, 0)`; `FUN_14029e250` calls the handler **only in 3D** (writer-map step 44, after `BCCCRB`). Case 0x0e of `FUN_1402a4a80`: `FUN_1402f5c10(f, FUN_1402f7720(221), N, obj+0x910, n = obj+0x880, m = model+0x1630, default = 0.0, format 0)`. `N` = `obj+0x3f24` if > 0, else `obj+8`.

* Nothing written if `n == 0`, `m == 0` or the pointer is NULL.
* Uniform (all `n x m` values `==` the first value `v`, test in `FUN_1402f5360`): one line `"%-6s %7i %7i %20.10E\n"` = name, `N`, `0`, `v`.
* Otherwise: the same header with count `k` = number of nodes having any component `!= 0.0`, and value `0.0`; then for each such node `" %7i"` (1-based index) and `FUN_1402f2e80(f, m, values, 5, 0)` = `" %20.10E"` per value (line break + 8 spaces before value 6, 11, ...; not reached here), then `"\n"`.
* The handler ignores the status and returns 0.
* 3D only: a 2D object never gets a `CRBFLX` line, although the reader accepts one in 2D.

`%20.10E` prints the MSVC 3-digit exponent (`1.0000000000E+001`). The round trip is exact up to the 11 significant digits printed.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x4d (p_main.c lines 2309-2313).
* `FUN_140416c90` (mode 0) / `FUN_1404199a0`: header + records reader (group2-helpers.md).
* handler2 `FUN_1402b6240` -> `FUN_1402a4a80` case 0x0e; handler3 = no-op `0x1402f7610`.
* `FUN_1402f5c10` + `FUN_1402f5360` (uniform test / count) + `FUN_1402f2e80` (value line): double array writer.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 285 occurrences of CRBFLX.

* `C:/Users/alext/Projects/DEFORM_corpus/key/3D/EN/NAFEMS_Benchmarks/NAFEMS_T2.KEY` (3D, uniform 0): parses exactly with the grammar above.

```
CRBFLX       1       0    0.0000000000E+000
```

## Open questions

* Allocation/resizing of `obj+0x910` when the node count changes is not traced.
* No corpus file has a non-uniform `CRBFLX`; the record layout is from the code only.
