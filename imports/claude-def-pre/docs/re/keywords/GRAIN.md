# GRAIN (id 208, group 2, case 0x40)

Per-element grain state (state-variable id 0x35, DB array LLGSZ element rows); width = number of
grain variables of the material grain model (16 in every corpus file). Selects element storage of
grain data (ELMNOD option 5, `model+0x898 = 0`); GRAIN2 is the nodal alternative.

## KEY grammar (read)

    GRAIN <objNo:%i> <count:%i> <W:%i> [<d_1:%Flf> ... <d_20:%Flf>] [ignored]
    then <count> records:  <index:%i> <v_1:%Flf> ... <v_W:%Flf>

* Header = rest of the keyword line, `FUN_1404192d0` mode 2: `sscanf("%i %i" + 20×" %Flf")`, **no
  comma replacement**. Up to 20 defaults.
* Records: `FUN_140419b90` (k = W), `fscanf`, free line breaks; `index` 1-based, `< 1` or
  `> obj+0x884` (element count) read and dropped. **No `fgets` after the records.**
* Same in 2D and 3D.

## Model storage

* Before reading: if the **current** `W[0x35] = model+0x16cc` is > 20 → status 1 (nothing read).
* `W'` → `model+0x16cc`. `FUN_140183840(obj+0x9e0, 0x35, W')` re-sizes the element field, getter
  `FUN_140184520` returns `double *obj+0xa78` (and posts `FUN_1401d48d0(model, &0x35)`).
* Defaults: `nd = conversions − 2`. `nd > 0`: if `nd == W'` every element row = `(d_1..d_W')`; else
  each row gets `d_j` for `j ≤ nd` and **0.0** for the other components (unlike the other
  readers, which spread `d_1`). `nd == 0`: array left as re-sized.
* Records: `arr[(index-1)*W' + j]`.
* On success (status 0): `model+0x898` (ELMNOD option 5) := 0 if it was not 0.
* Initial `W[0x35]` (`FUN_1401d0320`): the count from `FUN_1401d64f0(model, 0x35, &n)` (material grain
  model), else 0.

## Read errors and validation

Helper status 1 → the parser returns **2** ("Reading Error", printed to stdout, KEY loop
continues; `_common.md` §1). There is no Data Error (3) path. Object number `< 1` → 2 before the
helper runs (`group2-helpers.md`).

* Old `W[0x35] > 20` → 1 **before** the `fgets`: the rest of the line stays in the stream and the
  main loop reads the count as a keyword. A file may set `W' > 20` (accepted), after which every
  further GRAIN in the same model fails.
* `fgets` EOF → 1; `W' < 1` (or missing) → 1 (nothing stored, `0x898` unchanged).
* `fscanf` EOF in records → 1; non-numeric tokens not detected.

## KEY writer

`FUN_14029e250` step 42: only when `model+0x898 == 0` (otherwise GRAIN2 or nothing). Handler
`FUN_1402b5b80` → `FUN_1402a4a80(model, f, obj, 0x35, N, 0)` → case 0x35:
`FUN_1402f5c10(f, "GRAIN", N, obj+0xa78, obj+0x884, model+0x16cc, 0.0, mode 1, 0)`.

`FUN_1402f5c10(f, name, N, data, n, W, dflt = 0.0, mode, extra)` ("KwdWriteUtl::writeNodeElemVarDouble"):

1. `n == 0`, `W == 0`, `data == NULL` or `name == NULL` → writes nothing, returns 0.
   (`f == NULL` → 2.)
2. Uniform test (`FUN_1402f5360`, mode ≠ 2): the array is *uniform* when **all** `n×W` values
   compare equal (`==`) to `data[0]`. Uniform → header default = `data[0]`, record count = 0.
   Otherwise default = `dflt` (0.0) and count = number of rows having at least one value `!= 0.0`.
3. Header line (by `mode`):
   * mode 1: `"%-6s %7i %7i %7i %20.10E\n"` = name, N, count, W, default
4. If not uniform: for each row `r` (0-based) having a value `!= 0.0`: `" %7i"` with `r+1`, then
   `FUN_1402f2e80(f, W, &data[r*W], 5, 0)` = `" %20.10E"` per value, `"\n"` + 8 spaces before the
   6th, 11th, … value, final `"\n"` (`_common.md` §3). Rows that are all 0.0 are skipped.
5. Returns 0 (2 only for `f == NULL`). The handler ignores the `FUN_1402a4a80` status and always
   returns 0.

`%20.10E` uses the MSVC CRT (VS2008), i.e. a **3-digit exponent** (`0.0000000000E+000`), as the
corpus confirms. Values are rounded to 11 significant digits, so a write/read cycle is not
bit-exact for arbitrary doubles; `-0.0` compares equal to 0.0 (a row of `-0.0` is skipped and
reads back as +0.0).

**Does not round-trip for uniform non-zero arrays with W > 1**: the writer emits a single default
`v` (count 0) and the reader (nd = 1 ≠ W) sets component 1 = `v` and components 2..W = 0.0.
Uniform 0.0 and non-uniform arrays round-trip (up to rounding).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x40 (p_main.c 2224–2231).
* `FUN_1404192d0(f, obj, 0x35, n, &W, 2)` — header (20 defaults), allocation, default fill,
  `FUN_140419b90` records; only code 0x35 with mode 2 is accepted (else 1).
* `FUN_140183840`, `FUN_140184520` — element field re-size / getter (`obj+0x9e0+0x98`).
* `FUN_1402b5b80`, `FUN_1402a4a80` case 0x35, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80`.

## Corpus check

4 files / 8 occurrences (2D and 3D), all parse with record width = header W = 16.
`key/2D/SI/HeatTreat/MRX_SRX_2D_SI.KEY` (ELMCON 1046, 1046 records of 4 lines):

    GRAIN        1    1046      16    0.0000000000E+000
           1    8.9766036000E-001    2.9001691000E+001    1.0023090000E+002    8.9766036000E-001    0.0000000000E+000
                0.0000000000E+000    2.0881056000E+001    0.0000000000E+000    0.0000000000E+000    3.0000000000E+000

## Open questions

* Whether the field re-size keeps old values (content when no default is given).
