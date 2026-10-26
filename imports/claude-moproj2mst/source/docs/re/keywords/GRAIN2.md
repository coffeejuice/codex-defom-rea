# GRAIN2 (id 209, group 2, case 0x41)

Per-**node** grain state (state-variable id 0x1d, DB array LLGSZ node rows): the nodal alternative
of GRAIN, selected by ELMNOD option 5 `model+0x898 = 1`, used with the material grain model 2.

## KEY grammar (read)

    GRAIN2 <objNo:%i> <count:%i> <ignored:%i> <W:%i> [<default:%Flf>] [ignored]
    then <count> records:  <index:%i> <v_1:%Flf> ... <v_W:%Flf>

* Header = rest of the keyword line, `FUN_1404176a0` (mode 4) `sscanf("%i %i %i %Flf")`, no comma
  replacement. The 2nd integer is read and ignored (the writer puts 2 there); one default.
* Records: `FUN_140419b90` (k = W), `fscanf`; `index` 1-based, `< 1` or `> obj+0x880` (node count)
  dropped. **No `fgets` after the records.** Same in 2D and 3D.

## Model storage

* `W'` → `model+0x166c` (`W[0x1d]`). Array from `FUN_140181420(obj+0x8a0, &0)` (re-sizes the node
  field to `nodeCount × W[0x1d]` via `FUN_14017ffe0`, posts `FUN_1401d48d0(model, &0x1d)`, returns
  `double *obj+0x950`). Unlike the `FUN_140417c80` keywords there is no `FUN_140180310` call.
* Default converted (4 conversions): all `n × W'` entries = default.
* Records: `arr[(index-1)*W' + j]`.
* After the helper the parser calls `FUN_1401d70c0(model, &2)` (grain-variable count for grain
  model 2: `13·FUN_1401d71b0(…) + 2·k`) and **discards** the result.
* On status 0: `model+0x898` := 1.
* Initial `W[0x1d]`: from `FUN_1401d64f0(model, 0x1d, &n)` (needs `model+0x864` ≠ 0 and a material
  with grain model `mat+0x17e8` in {1, 2, 4, 5}), else 0.

## Read errors and validation

Helper status 1 → the parser returns **2** ("Reading Error", printed to stdout, KEY loop
continues; `_common.md` §1). There is no Data Error (3) path. Object number `< 1` → 2 before the
helper runs (`group2-helpers.md`).

* `fgets` EOF → 1; `W' < 1` → 1; `fscanf` EOF in records → 1. On error `0x898` is unchanged.

## KEY writer

`FUN_14029e250` step 42: only when `model+0x898 == 1`. Handler `FUN_1402b5c40` →
`FUN_1402a4a80(…, 0x1d, N, 0)` → case 0x1d writes only if `FUN_1401d7160(model, &0) == 2`
(`model+0x864 != 0` and `FUN_14029bb50(model+0x1328)` = 2: some material has `+0x17e8 == 2` and none
has 1); then `FUN_1402f5c10(f, "GRAIN2", N, obj+0x950, obj+0x880, model+0x166c, 0.0, mode 4, extra 2)`.

`FUN_1402f5c10(f, name, N, data, n, W, dflt = 0.0, mode, extra)` ("KwdWriteUtl::writeNodeElemVarDouble"):

1. `n == 0`, `W == 0`, `data == NULL` or `name == NULL` → writes nothing, returns 0.
   (`f == NULL` → 2.)
2. Uniform test (`FUN_1402f5360`, mode ≠ 2): the array is *uniform* when **all** `n×W` values
   compare equal (`==`) to `data[0]`. Uniform → header default = `data[0]`, record count = 0.
   Otherwise default = `dflt` (0.0) and count = number of rows having at least one value `!= 0.0`.
3. Header line (by `mode`):
   * mode 4: `"%-6s %7i %7i %7i %7i %20.10E\n"` = name, N, count, extra, W, default (extra = 2)
4. If not uniform: for each row `r` (0-based) having a value `!= 0.0`: `" %7i"` with `r+1`, then
   `FUN_1402f2e80(f, W, &data[r*W], 5, 0)` = `" %20.10E"` per value, `"\n"` + 8 spaces before the
   6th, 11th, … value, final `"\n"` (`_common.md` §3). Rows that are all 0.0 are skipped.
5. Returns 0 (2 only for `f == NULL`). The handler ignores the `FUN_1402a4a80` status and always
   returns 0.

`%20.10E` uses the MSVC CRT (VS2008), i.e. a **3-digit exponent** (`0.0000000000E+000`), as the
corpus confirms. Values are rounded to 11 significant digits, so a write/read cycle is not
bit-exact for arbitrary doubles; `-0.0` compares equal to 0.0 (a row of `-0.0` is skipped and
reads back as +0.0).

Round-trips (the reader spreads the single default over all components).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x41 (p_main.c 2232–2242).
* `FUN_1404176a0(f, obj, code, n, &W, 4)` — twin of `FUN_140417c80` without the flag handling (2nd
  integer ignored); codes 0x1d/0x22/0x23/0x36/0x37/0x3f, else 1.
* `FUN_140181420` — node getter id 0x1d (`obj+0x8a0+0xb0`). `FUN_1401d70c0`, `FUN_1401d7160`,
  `FUN_14029bb50` — grain-model queries.
* `FUN_1402b5c40`, `FUN_1402a4a80` case 0x1d, `FUN_1402f5c10`.

## Corpus check

No corpus file uses GRAIN2.

## Open questions

* Whether `FUN_1401d70c0` → `FUN_1401d71b0` → `FUN_1401d7250` → `FUN_14029bcd0` has side effects
  (the parser ignores its value; it looks like a pure query).
