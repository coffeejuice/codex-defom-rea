# DVOLF (id 206, group 2, case 0x3e)

Per-element phase-related state of the object: `m` doubles per element in `obj+0xa68` (state
variable code 0x33), with `m = ncomp[0x33] = model+0x13f4` when the model has phase
transformation data (heat treatment examples: 4 values per element).

## KEY grammar (read)

    DVOLF <objNo> <count:%i> <k:%i> [<default:%Flf> ...]     rest of the keyword line
    count × { <element:%i> <k × value:%Flf> }                (k == 0: m values per record)

`FUN_140416c90(f, obj+0xa68, obj+0x884, &ncomp[0x33], mode 1)` (`group2-helpers.md`): header
`sscanf("%i %i %Flf")` = count, k, default; records via `FUN_140419b90` (index + k values, free
layout); no final `fgets` when `k > 0`. Same in 2D and 3D.

## Model storage

* `n = obj+0x884` (elements); `m = model+0x16c4` = `ncomp[0x33]`, set by `FUN_1401d0320` to
  `model+0x13f4` if `FUN_1401d64f0(model, 0x31)` (a mixture material exists, `mat+0x1cc8 == 1`,
  or `model+0x85c != 0`), else 0. Not changed by the reader.
* Default present → all `n·m` entries = default. Records store `min(m, k)` values at
  `obj+0xa68[(element-1)*m + j]`. With `m == 0` nothing is stored (values are read and dropped).

## Read errors and validation

* Prologue → **2**; header EOF / blank (`sscanf < 1`) → **2**; EOF in a record → **2**; else **0**.
* Out-of-range element numbers: values read and dropped.

## KEY writer

Handler `FUN_1402b5ac0` → `FUN_1402a4a80(model, f, obj, 0x33, N, 0)`; writer map step 41
(VOLFC, VOLFS, DVOLF), always attempted.
`FUN_1402f5c10(f, "DVOLF", N, obj+0xa68, obj+0x884, m, 0.0, fmt 1)`:

    "%-6s %7i %7i %7i %20.10E\n"    name, N, count, m, default
    " %7i" + m × " %20.10E" + "\n"  per written element (5 per line)

Uniform → count 0 + the value; else default 0.0 and only elements with a nonzero value.
**Nothing is written when `m == 0`** (no phase data), `n == 0` or the array is NULL.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x3e; `FUN_140416c90`, `FUN_140419b90` (`group2-helpers.md`).
* `FUN_1402b5ac0`, `FUN_1402a4a80` case 0x33, `FUN_1402f5c10`, `FUN_1401d0320`, `FUN_1401d64f0`
  (`group2-statevar-writers.md`).

## Corpus check

* `2D/EN/HeatTreat/HT_CARB1_EN.KEY` parses exactly (k = 4):

      DVOLF        1     521       4    0.0000000000E+000
             1    0.0000000000E+000    0.0000000000E+000    4.9801500000E-010    0.0000000000E+000

## Open questions

* Physical meaning (name suggests "delta volume fraction") and the meaning of `model+0x13f4`
  were not traced.
