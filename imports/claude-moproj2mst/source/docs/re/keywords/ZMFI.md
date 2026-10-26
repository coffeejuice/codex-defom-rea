# ZMFI (id 238, group 2, case 0x5e)

Per-element state variable with 2 (2D) / 3 (3D) components, written after ZEFI (magnetic field intensity by name).

## KEY grammar (read)

    ZMFI <objNo:%i> <count:%i> [<default:%Flf>] [ignored]
    then <count> records:  <index:%i> <v_1:%Flf> ... <v_W:%Flf>

* The header is the rest of the keyword line, parsed by `FUN_140416c90` mode 0 with
  `sscanf("%i %Flf")`, **no comma replacement**: `5,1.0` gives count 5 and no default.
* `W` (values per record) is **not** in the file: it is `model+0x16e4` (state-variable width table
  entry `W[0x3b]`, see Model storage).
* Records are read with `fscanf` (`FUN_1404199a0`): whitespace separated, may be split over lines
  freely, commas not accepted. After the last record one `fgets` consumes the rest of that line.
  `count ≤ 0` → no records and no extra `fgets` (the header line is already consumed).
* `index` is 1-based; records with `index < 1` or `index > n` (element count, `obj+0x884`) are read and
  dropped.
* Same in 2D and 3D (only the default width `W` may differ, see below).

## Model storage

* Array `double *obj+0xaa8`, row-major `[(index-1)*W + j]`, size `n × W`, `n = obj+0x884`
  (element count). The reader does **not** allocate it: it is allocated (with width `W[0x3b]`)
  when the element count is set (ELMCON: `FUN_1401653b0` → `FUN_140163090`). If ZMFI comes before that, `n = 0` and every record is
  dropped.
* Header default present (2 conversions): **all** `n × W` entries are set to the default (every
  component gets the same value). Default missing: the array keeps its previous contents and only
  the records change it.
* `W = model+0x16e4` is read, never changed by this keyword. Initial value (`FUN_1401d0320`, run by
  `FUN_1401d0fc0` after STNOUT/ELMNOD/GEOTYP): 2 when the model is not 3D, 3 in 3D.

## Read errors and validation

Helper status 1 → the parser returns **2** ("Reading Error", printed to stdout, KEY loop
continues; `_common.md` §1). There is no Data Error (3) path. Object number `< 1` → 2 before the
helper runs (`group2-helpers.md`).

* `fgets` EOF on the header → 1.
* `sscanf < 1` (blank rest of line, or non-numeric first token) → 1.
* `fscanf` EOF inside the records → 1 (temporary buffer leaked).
* Final `fgets` EOF → 1 (a file that ends right after the last value, without newline, fails).
* A non-numeric token in the records is not detected: `fscanf` returns 0, the index/value buffer
  keeps the previous record's values (which are stored again) and the token stays in the stream.

## KEY writer

Emitted by `FUN_14029e250` step 47, always. Handler `FUN_1402b6600` (registry handler2): object `FUN_14017aaf0(model+0x1380, objNo)`,
`N = obj+0x3f24` if > 0 else objNo, then `FUN_1402a4a80(model, f, obj, 0x3b, N, 0)`, whose case
0x3b calls `FUN_1402f5c10(f, "ZMFI", N, obj+0xaa8, obj+0x884, model+0x16e4, 0.0, mode 0, 0)`.

`FUN_1402f5c10(f, name, N, data, n, W, dflt = 0.0, mode, extra)` ("KwdWriteUtl::writeNodeElemVarDouble"):

1. `n == 0`, `W == 0`, `data == NULL` or `name == NULL` → writes nothing, returns 0.
   (`f == NULL` → 2.)
2. Uniform test (`FUN_1402f5360`, mode ≠ 2): the array is *uniform* when **all** `n×W` values
   compare equal (`==`) to `data[0]`. Uniform → header default = `data[0]`, record count = 0.
   Otherwise default = `dflt` (0.0) and count = number of rows having at least one value `!= 0.0`.
3. Header line (by `mode`):
   * mode 0: `"%-6s %7i %7i %20.10E\n"` = name, N, count, default
4. If not uniform: for each row `r` (0-based) having a value `!= 0.0`: `" %7i"` with `r+1`, then
   `FUN_1402f2e80(f, W, &data[r*W], 5, 0)` = `" %20.10E"` per value, `"\n"` + 8 spaces before the
   6th, 11th, … value, final `"\n"` (`_common.md` §3). Rows that are all 0.0 are skipped.
5. Returns 0 (2 only for `f == NULL`). The handler ignores the `FUN_1402a4a80` status and always
   returns 0.

`%20.10E` uses the MSVC CRT (VS2008), i.e. a **3-digit exponent** (`0.0000000000E+000`), as the
corpus confirms. Values are rounded to 11 significant digits, so a write/read cycle is not
bit-exact for arbitrary doubles; `-0.0` compares equal to 0.0 (a row of `-0.0` is skipped and
reads back as +0.0).

Round trip: uniform arrays come back exactly (the default fills every component); non-uniform
arrays come back with the skipped rows = 0.0, i.e. exactly (up to the 11-digit rounding).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x5e (p_main.c 3121–3125) — parser.
* `FUN_140416c90` (mode 0) — header + default fill; `FUN_1404199a0` — records (`group2-helpers.md`).
* `FUN_1402b6600` — write handler; `FUN_1402a4a80` case 0x3b — state-variable writer.
* `FUN_1402f5c10` — node/element variable line writer; `FUN_1402f5360` — uniform/count test;
  `FUN_1402f2e80` — value line writer.
* `FUN_1401d0320` — width table `model+0x15f8 + 4·id` initialisation.

## Corpus check

195 occurrences, all count 0, all parse; e.g. `key/2D/EN/MultipleOperations/New_MO_Release_Examples_EN/MO2_Operation_Cycles2/Objects/Object00003.KEY`: `ZMFI         3       0    0.0000000000E+000`.

## Open questions

* The width table before any STNOUT/ELMNOD/GEOTYP keyword (model constructor) was not read; a KEY
  without those keywords may use another `W`.
