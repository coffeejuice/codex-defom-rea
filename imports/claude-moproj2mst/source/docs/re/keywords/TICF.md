# TICF (id 207, group 2, case 0x3f)

Per-element phase-related state of the object: `m` doubles per element in `obj+0xa70` (state
variable code 0x34), `m = ncomp[0x34] = model+0x13f4` when the model has phase data (same
sizing as DVOLF).

## KEY grammar (read)

    TICF <objNo> <count:%i> <k:%i> [<default:%Flf> ...]      rest of the keyword line
    count × { <element:%i> <k × value:%Flf> }                (k == 0: m values per record)

`FUN_140416c90(f, obj+0xa70, obj+0x884, &ncomp[0x34], mode 1)` (`group2-helpers.md`): header
`sscanf("%i %i %Flf")` = count, k, default; records via `FUN_140419b90`; no final `fgets` when
`k > 0`. Same in 2D and 3D.

## Model storage

* `n = obj+0x884` (elements); `m = model+0x16c8` = `ncomp[0x34]`, set by `FUN_1401d0320` to
  `model+0x13f4` if `FUN_1401d64f0(model, 0x31)` is true, else 0. Not changed by the reader.
* Default present → all `n·m` entries = default; records store `min(m, k)` values at
  `obj+0xa70[(element-1)*m + j]`.

## Read errors and validation

* Prologue → **2**; header EOF / blank → **2**; EOF in a record → **2**; else **0**.
* Out-of-range element numbers: values read and dropped.

## KEY writer

Handler `FUN_1402b5f40` → `FUN_1402a4a80(model, f, obj, 0x34, N, 0)`; writer map step 43
(SIZESH, TXTODF, ERECVL, **TICF**, DATOM, …), always attempted.
`FUN_1402f5c10(f, "TICF", N, obj+0xa70, obj+0x884, m, 0.0, fmt 1)`:

    "%-6s %7i %7i %7i %20.10E\n"    name, N, count, m, default
    " %7i" + m × " %20.10E" + "\n"  per written element (5 per line)

Uniform → count 0 + value; else default 0.0 and nonzero elements only. Nothing when `m == 0`,
`n == 0` or array NULL.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x3f; `FUN_140416c90`, `FUN_140419b90` (`group2-helpers.md`).
* `FUN_1402b5f40`, `FUN_1402a4a80` case 0x34, `FUN_1402f5c10`, `FUN_1401d0320`
  (`group2-statevar-writers.md`).

## Corpus check

* `2D/EN/HeatTreat/HT_CARB1_EN.KEY`: `TICF         1       0       4    0.0000000000E+000` parses
  (count 0, k 4). 22 corpus files use TICF; none was checked with records.

## Open questions

* Physical meaning (name suggests a transformation incubation fraction) not traced.
