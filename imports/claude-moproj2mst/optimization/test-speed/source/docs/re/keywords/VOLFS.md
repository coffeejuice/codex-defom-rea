# VOLFS (id 216, group 2, case 0x48)

Per-element phase volume fractions (state-variable id 0x32, written with VOLFC/DVOLF). The model
array has one column per material (`W[0x32] = model+0x16c0` = material count); the KEY holds a
**compressed** column set: only the phases (constituents of mixture materials) used by the object.

## KEY grammar (read)

    VOLFS <objNo:%i> <count:%i> <k:%i> [<default:%Flf>] [ignored]
    then <count> records:  <index:%i> <v_1:%Flf> ... <v_k:%Flf>

* Header = rest of the keyword line, `FUN_140416c90` mode 1: `sscanf("%i %i %Flf")`, no comma
  replacement. `k` = values per record in the file.
* Records: `FUN_140419b90(f, tmp, count, n, cnt, k)`: `k ≥ 1` → `index` + `k` values, the first
  `min(cnt, k)` kept, **no** final `fgets`; `k == 0` → `index` + `cnt` values, then one `fgets`.
  `index` 1-based, `< 1` or `> obj+0x884` (element count) dropped. Same in 2D and 3D.

## Model storage

`FUN_1403981c0(model, f, obj, obj+0xa60, n = obj+0x884, m = model+0x16c0)`:

1. Phase map `FUN_140398350(model, obj, map[model+0x1320], &cnt)` (only if `obj+0x87c != 0`, which
   ELMCON sets): for each element in order, when its material group `obj+0x9f8[e]` differs from
   the previous one (start 0), the material's 0-based position `p` in the material list
   `model+0x1328` (`FUN_14029c870`) is visited once; if the material (`FUN_1401d7060`) has
   `+0x1cc8 == 1` (mixture), each constituent id in `mat+0x1cd0[0 .. mat+0x1ccc−1]` is marked. Then
   `map[c]` = running index (0, 1, …) over the marked material positions `c` in ascending order,
   −1 for the others; `cnt` = number marked.
2. `tmp = calloc(n × cnt)`; `FUN_140416c90(f, tmp, n, &cnt, 1)`: a converted default fills all of
   `tmp`; records as above.
3. For every element `e` and every material column `c < m` with `map[c] ≥ 0`:
   `obj+0xa60[e*m + c] = tmp[e*cnt + map[c]]`, so **all** elements' phase columns are overwritten
   (elements without a record get the default or 0.0); non-phase columns are untouched.

If `obj+0x87c == 0` the map is not built (all 0) and `cnt` is **uninitialised** (only harmless
because `n` is then normally 0). MTLGRP (`obj+0x9f8`) and the materials must be known before VOLFS.
Initial `W[0x32]` (`FUN_1401d0320`): `model+0x1320` (number of materials) if `FUN_1401d64f0(m, 0x31)`,
else 0. The array `obj+0xa60` is allocated by ELMCON (`FUN_1401653b0`), not by this reader.

## Read errors and validation

Helper status 1 → the parser returns **2** ("Reading Error", printed to stdout, KEY loop
continues; `_common.md` §1). There is no Data Error (3) path. Object number `< 1` → 2 before the
helper runs (`group2-helpers.md`).

* `fgets` EOF / `sscanf < 1` → 1; `fscanf` EOF in records → 1 (map/tmp leaked, model unchanged:
  step 3 runs only on success).

## KEY writer

`FUN_14029e250` step 41 (VOLFC, **VOLFS**, DVOLF), always. Handler `FUN_1402b5a00` →
`FUN_1402a4a80(…, 0x32, N, 0)` → case 0x32 (the decompile groups case 0x25 with it) →
`FUN_1402a3690(model, f, "VOLFS", obj, N, obj+0xa60, model+0x16c0, 0)`: same map and `cnt` as
above, `tmp[e*cnt + map[c]] = data[e*m + c]`, then
`FUN_1402f5c10(f, "VOLFS", N, tmp, obj+0x884, cnt, 0.0, mode 1, 0)` (with its last argument ≠ 0 it
would call the matrix writer `FUN_1402f7490`, `"%d  %d\n"` + `"%15.7E  "`; not used here).

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

`cnt == 0` (no mixture phases) → nothing is written. Round-trips (writer `k` = reader `cnt`; a
uniform array gives one default which the reader spreads to all phase columns).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x48 (p_main.c 2281–2285); `FUN_1403981c0` reader; `FUN_140398350` phase map;
  `FUN_140416c90` mode 1; `FUN_140419b90`.
* `FUN_1402b5a00`, `FUN_1402a4a80` case 0x32, `FUN_1402a3690` (compressing writer, shared with VOLFC:
  case 0x31, `obj+0xa58`, `model+0x16bc`), `FUN_1402f5c10`.

## Corpus check

30 occurrences (2D and 3D), all parse; k = 3 or 7. `key/2D/EN/HeatTreat/HT_CARB1_EN.KEY`:

    VOLFS        1       0       3    0.0000000000E+000

## Open questions

* Case 0x25 sharing the VOLFS branch of `FUN_1402a4a80` (it would print VOLFS for id 0x25): check the
  jump table. Irrelevant for VOLFS itself.
