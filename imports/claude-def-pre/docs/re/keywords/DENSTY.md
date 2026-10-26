# DENSTY (id 177, group 2, case 0x21)

Per-element relative density of the object: one double per element in `obj+0xa30` (state
variable code 0x2c); sintering examples carry values such as 0.57.

## KEY grammar (read)

    DENSTY <objNo> <count:%i> [<default:%Flf> ...]      rest of the keyword line
    count × { <element:%i> <m × value:%Flf> }           m = ncomp[0x2c] = model+0x16a8 (default 1)

`FUN_140416c90(f, obj+0xa30, obj+0x884, &ncomp[0x2c], mode 0)` (`group2-helpers.md`): header
`sscanf("%i %Flf")` (no comma replacement), `count` records via `fscanf`, final `fgets`.
Same in 2D and 3D.

## Model storage

`obj+0xa30[(element-1)*m + j]` (double), `n = obj+0x884` (elements); a given default fills all
entries first. Before the mesh is read `n = 0` and records are dropped.

## Read errors and validation

* Prologue: `group2-helpers.md` → **2**.
* `FUN_140416c90` nonzero (EOF, empty header line, EOF in records / final `fgets`) → **2**;
  else **0**. Element numbers outside `1..n` are read and dropped.

## KEY writer

Handler `FUN_1402b5700` → `FUN_1402a4a80(model, f, obj, 0x2c, N, 0)`; writer map step 38
(STRAIN, STNCMP, DENSTY, DAMAGE, STRESS, YLDS, USRELM), always attempted.
`FUN_1402f5c10(f, "DENSTY", N, obj+0xa30, obj+0x884, ncomp[0x2c], 0.0, fmt 0)`:

    "%-6s %7i %7i %20.10E\n"         name, N, count, default
    " %7i" + m × " %20.10E" + "\n"   per written element

Uniform (e.g. all 1.0) → `count 0` + that value; otherwise default **0.0** and only elements with
a nonzero density are listed. Nothing when `n == 0`, array NULL or `ncomp[0x2c] == 0`.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x21; `FUN_140416c90`, `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_1402b5700`, `FUN_1402a4a80` case 0x2c, `FUN_1402f5c10` (`group2-statevar-writers.md`).

## Corpus check

* `3D/EN/Sintering_3D/Unified_Compaction_And_Sintering/Sintering_Skorohod.KEY` parses exactly:

      DENSTY       1   29305    0.0000000000E+000
             1    5.7000000000E-001

* `2D/EN/plane stress/Ela_PlaneStress_EN.KEY`: `DENSTY       1       0  1.0000000E+000` (uniform).

## Open questions

* None for the format.
