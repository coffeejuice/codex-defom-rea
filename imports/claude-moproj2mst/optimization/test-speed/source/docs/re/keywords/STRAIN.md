# STRAIN (id 175, group 2, case 0x1f)

Effective strain of the object: per element `obj+0xa28` (state variable code 0x2b, `m =
ncomp[0x2b]`, default 1), optionally with a nodal companion `obj+0x938` (`ncomp[0x1b]`). When the
model stores strain per integration point (`model+0x88c == 3`) a fixed line-per-value layout is
used instead.

## KEY grammar (read)

The reader `fgets` the rest of the keyword line (status not checked; `fgetpos` is taken but not
used) and tests it with `sscanf(line, "%i %Flf %i %Flf %i")` (no comma replacement).

**A. 5 conversions → integration-point layout** (fields: count, default, nnode, default2, k;
only `count` and `k` are used):

    STRAIN <objNo> <count> <def> <nn> <def2> <k>
    count × { <index:%i>              one line (FUN_140413de0, index ignored)
              p × <value:%Flf> }      one value per line (FUN_140414990)

* 3D: `p = 8` if `k == 8`, else `p = 1`;
* 2D: `p = 4` whatever `k` is.
Records are stored **sequentially** (record r → element r), the index on the record line is read
and ignored.

**B. otherwise → `FUN_140417280` (`readNodeElemVarDouble2`, mode 0)** on the same line:

    STRAIN <objNo> <count1:%i> [<def1:%Flf> [<count2:%i> [<def2:%Flf>]]]
    count1 × { <element:%i> <m1 × value> }     then one fgets
    count2 × { <node:%i>    <m2 × value> }     then one fgets

(`group2-statevar-writers.md`, "Readers that pair with these writers"), with `v1 = obj+0xa28`,
`n1 = obj+0x884`, `m1 = model+0x16a4`, `v2 = obj+0x938`, `n2 = obj+0x880`,
`m2 = model+0x1664`. Same in 2D and 3D.

## Model storage

* A: `obj+0xa28[r*p + j]` for record `r = 0..count-1`; **no bound check** against the number
  of elements (count > elements overflows the array). When `count == 0` the first
  `k · obj+0x884` doubles are zeroed (`memset`; uses the `k` from the file). Defaults and
  `nn`/`def2` are ignored. Elements beyond `count` keep their old values.
* B: all `n1·m1` entries of `v1` = def1 (0.0 when missing, applied whenever `sscanf ≥ 1`); if
  `v2 != NULL && m2 != 0` all of `v2` = def2; records at `[(index-1)*m + j]` with out-of-range
  indices dropped. Nodal records with no nodal array are read into a scratch buffer and lost.

## Read errors and validation

* Prologue → **2**.
* A: an index line at EOF or blank (`FUN_140413de0` nonzero) → **2** immediately. Value lines:
  only the status of the **last** `FUN_140414990` call of the block is returned (→ **2** if it
  hit EOF or a blank line); earlier failures are ignored. `count == 0` → **0**.
* B: `FUN_140417280` returns 1 when `sscanf < 1` (blank line or EOF) or when a record read fails
  → **2**; else **0**.

## KEY writer

Handler `FUN_1402b4340` → `FUN_1402a4a80(model, f, obj, 0x2b, N, 0)`; writer map step 38
(first of STRAIN … USRELM), always attempted.

* `model+0x88c == 3` (integration-point layout, every element, no compression):
  * 3D, `obj+0x890 == 4`: `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` = N, elements, 0.0,
    nodes, 0.0, **8**; per element `" %7i\n"` (1-based) then 8 × `" %20.10E\n"`
    (`obj+0xa28[i*8 + j]`);
  * 3D, `obj+0x890 == 2`: same header with **1**, then `" %7i\n"` + `" %20.10E\n"` (`[i]`);
  * 3D, other `obj+0x890`: **nothing**;
  * 2D: same header with **4**, then `" %7i\n"` + 4 × `" %20.10E\n"` (`[i*4 + j]`).
* otherwise, `ncomp[0x1b]` of the object's model (`obj+0x3a98` → `+0x1664`) is 0 or
  `obj+0x938 == NULL`: `FUN_1402f5c10(f, "STRAIN", N, obj+0xa28, elements, ncomp[0x2b], 0.0,
  fmt 0)` → `"%-6s %7i %7i %20.10E\n"` + compressed records;
* otherwise `FUN_1402f60c0(..., fmt 0)`: `"%-6s %7i %7i %20.10E %7i %20.10E\n"` = count1,
  def1, count2, def2, then element records (`ncomp[0x2b]` values), then node records
  (`ncomp[0x1b]` values).

Nothing (non-integration-point paths) when `ncomp[0x2b] == 0`, no elements or array NULL.
`model+0x88c` is a model field set elsewhere (strain storage mode; `0` or `3` also force
`ncomp[0x1b] = 0`, see the component table).

Round trip: the integration-point header has exactly 5 numbers, the compressed headers 2 or 4,
so the reader picks the right branch. A 3D element type other than 2/4 in integration-point
mode loses the keyword.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x1f; `FUN_140413de0`, `FUN_140414990`, `FUN_1404199a0`
  (`group2-helpers.md`); `FUN_140417280`; `FUN_140478d30` = `memset`.
* `FUN_1402b4340`, `FUN_1402a4a80` case 0x2b, `FUN_1402f5c10`, `FUN_1402f60c0`
  (`group2-statevar-writers.md`).

## Corpus check

* `3D/SI/Forging/Spike_ForgingBlow2.KEY` (branch B, single array) parses:

      STRAIN       1    4978    0.0000000000E+000
             1    2.7861638000E-001

* `3D/SI/Rolling/3D_Ring_Rolling_Example_3.KEY` (two arrays): `STRAIN       1       0    0.0000000000E+000       0    0.0000000000E+000`.
* No corpus file uses the integration-point layout (`rg` for a 5-number header found none).

## Open questions

* What sets `model+0x88c` (and `obj+0x890`, element type: 2 = tetrahedron, 4 = hexahedron by
  the integration-point counts 1/8) was not traced.
