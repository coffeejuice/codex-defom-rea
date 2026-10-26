# SIZESH (id 210, group 2, case 0x42)

Per-element state variable with a material-dependent width (`W[0x36]`, from the material grain/phase data); grain size/shape data by name.

## KEY grammar (read)

    SIZESH <objNo:%i> <count:%i> <flag:%i> <W:%i> [<default:%Flf>] [ignored]
    then <count> records:  <index:%i> <v_1:%Flf> ... <v_W:%Flf>

* Header = rest of the keyword line, `sscanf("%i %i %i %Flf")` in `FUN_140417c80` (mode 4); **no
  comma replacement**. Exactly one default value is read.
* `flag == 0`: normal form.
* `flag == 1` → **error** (status 1) for this keyword (only NRECVL and ERECVL accept it); any other
  flag except 0 → error.
* Records (`FUN_140419b90` with k = W): `fscanf`, whitespace separated, free line breaks. `index`
  is 1-based, `index < 1` or `> n` (element count `obj+0x884`) read and dropped. **No `fgets` after the
  records**: the newline after the last value stays in the stream (harmless: the next keyword
  token is read with `%39s`).
* Same grammar in 2D and 3D.

## Model storage

* `W'` is stored in `model+0x16d0` (width table entry `W[0x36]`), replacing the previous value
  (initial value from `FUN_1401d0320`: count returned by `FUN_1401d64f0(model, 0x36, &n)` (material data), else 0).
* `flag == 0`: `FUN_140183840(obj+0x9e0, 0x36, W')` re-sizes the field to `n × W[0x36]` (virtual resize on the container's
  field list, `FUN_140329e60`; whether old contents survive was not checked) and `FUN_1401845b0` returns the
  array `double *obj+0xa80` (it also posts a change notification `FUN_1401d48d0(model, &0x36)`).
  If a default was converted (4 conversions): every one of the `n × W'` entries = default.
  Without default: the re-sized array is left as is.
* Records: `arr[(index-1)*W' + j]`, `j < W'`.
* `FUN_140183840` for id 0x36 also stores the width in the element container: `obj+0x9e0+0xfc`
  (`FUN_14019a410`) = `model+0x16d0`.

## Read errors and validation

Helper status 1 → the parser returns **2** ("Reading Error", printed to stdout, KEY loop
continues; `_common.md` §1). There is no Data Error (3) path. Object number `< 1` → 2 before the
helper runs (`group2-helpers.md`).

* `fgets` EOF → 1. `W' < 1` (also when fewer than 3 integers converted) → 1, nothing stored.
* `flag` not 0 → 1 (after `W'` was already stored).
* `fscanf` EOF in the mask/values or records → 1. Non-numeric tokens are not detected (`fscanf`
  returns 0, the token stays, the buffer keeps the previous values).

## KEY writer

Emitted by `FUN_14029e250` step 43 (1st), always called. Handler `FUN_1402b5d00` → `FUN_1402a4a80(model, f, obj, 0x36, N, 0)`; case 0x36 (only when `model+0x16d0 != 0` and `obj+0xa80 != NULL`, else nothing) calls
`FUN_1402f5c10(f, "SIZESH", N, obj+0xa80, obj+0x884, model+0x16d0, 0.0, mode 4, extra 0)`.

`FUN_1402f5c10(f, name, N, data, n, W, dflt = 0.0, mode, extra)` ("KwdWriteUtl::writeNodeElemVarDouble"):

1. `n == 0`, `W == 0`, `data == NULL` or `name == NULL` → writes nothing, returns 0.
   (`f == NULL` → 2.)
2. Uniform test (`FUN_1402f5360`, mode ≠ 2): the array is *uniform* when **all** `n×W` values
   compare equal (`==`) to `data[0]`. Uniform → header default = `data[0]`, record count = 0.
   Otherwise default = `dflt` (0.0) and count = number of rows having at least one value `!= 0.0`.
3. Header line (by `mode`):
   * mode 4: `"%-6s %7i %7i %7i %7i %20.10E\n"` = name, N, count, extra, W, default (extra = 0)
4. If not uniform: for each row `r` (0-based) having a value `!= 0.0`: `" %7i"` with `r+1`, then
   `FUN_1402f2e80(f, W, &data[r*W], 5, 0)` = `" %20.10E"` per value, `"\n"` + 8 spaces before the
   6th, 11th, … value, final `"\n"` (`_common.md` §3). Rows that are all 0.0 are skipped.
5. Returns 0 (2 only for `f == NULL`). The handler ignores the `FUN_1402a4a80` status and always
   returns 0.

`%20.10E` uses the MSVC CRT (VS2008), i.e. a **3-digit exponent** (`0.0000000000E+000`), as the
corpus confirms. Values are rounded to 11 significant digits, so a write/read cycle is not
bit-exact for arbitrary doubles; `-0.0` compares equal to 0.0 (a row of `-0.0` is skipped and
reads back as +0.0).

So the writer always produces `flag = 0`; the output round-trips (uniform: one default, the reader
fills every component with it).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x42 (p_main.c 2243–2247).
* `FUN_140417c80(f, obj, code, n, &W, 4)` — header, flag 0/1 handling, default fill, then
  `FUN_140419b90` records. Shared by SIZESH, TXTODF, TIMERC, ERECVL, NRECVL (switch on `code − 0x1d`:
  0x22 TIMERC, 0x23 NRECVL, 0x36 SIZESH, 0x37 TXTODF, 0x3f ERECVL; the 0x1d (GRAIN2) branch is not
  used — GRAIN2 calls the twin `FUN_1404176a0`; any other code → 1; mode ≠ 4 → 1).
* `FUN_140183840(obj+0x9e0, 0x36, W')`, `FUN_1401845b0`; node container `obj+0x8a0` (count at +0x12c, model at +0x130), element container
  `obj+0x9e0` (count at +0xe8, model at +0xf0); width table `model+0x15f8 + 4·id`.
* `FUN_1402b5d00`, `FUN_1402a4a80`, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80` — writer.

## Corpus check

No corpus file uses SIZESH.

## Open questions

* Whether the container re-size keeps old values (decides the content after `flag 0` without a
  default): read the virtual method at vtable+0x10 of the field objects (`FUN_140329e60`).
