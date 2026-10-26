# STNCMP (id 176, group 2, case 0x20)

Per-element strain-component state of the object: `m` doubles per element in `obj+0xa10`
(state variable code 0x28), `m = ncomp[0x28]` (e.g. 4 in the 2D and 12 in the 3D corpus files).

## KEY grammar (read)

    STNCMP <objNo> <count:%i> <k:%i> [<default:%Flf> ...]     rest of the keyword line
    count × { <element:%i> <k × value:%Flf> }                 (k == 0: m values per record)

`FUN_140416c90(f, obj+0xa10, obj+0x884, &ncomp[0x28], mode 1)` (`group2-helpers.md`): header
`sscanf("%i %i %Flf")` = count, k, default (no comma replacement); records read with
`FUN_140419b90(f, dst, count, n, m, k)`: index then `k` values via `fscanf` (free layout, the
writer's 5-per-line wrapping is accepted). With `k > 0` there is **no** final `fgets` (the rest
of the last record line stays unread; harmless because the next token read skips white space);
with `k == 0` the records hold `m` values and a final `fgets` follows. Same in 2D and 3D.

## Model storage

* `n = obj+0x884` (elements), `m = model+0x1698` = `ncomp[0x28]`, computed by
  `FUN_1401d0320` as `model+0x880 + model+0x884 + max(1, Σ model+0x86c..0x87c)·2·D`
  (2D torsion: `·6` instead of `·2·D`), see `group2-statevar-writers.md`. The reader does not
  change it; `k` from the file only says how many values each record has.
* If a default is present (3 conversions) every one of the `n·m` entries is set to it first.
* Record values: the first `min(m, k)` go to `obj+0xa10[(element-1)*m + j]`; extra values
  (k > m) are read and dropped; components beyond k keep their previous/default value.

## Read errors and validation

* Prologue: `group2-helpers.md` → **2**.
* Header EOF / blank line (`sscanf < 1`) → **2**; EOF inside a record → **2**; else **0**.
* Element numbers outside `1..n`: the record's values are still read and dropped.

## KEY writer

Handler `FUN_1402b5640` → `FUN_1402a4a80(model, f, obj, 0x28, N, 0)`; writer map step 38 (after
STRAIN), always attempted. `FUN_1402f5c10(f, "STNCMP", N, obj+0xa10, obj+0x884, m, 0.0, fmt 1)`:

    "%-6s %7i %7i %7i %20.10E\n"    name, N, count, m, default
    " %7i" + m × " %20.10E" + "\n"  per written element, 5 values per line,
                                    continuation lines start with 8 spaces

All `n·m` values equal → count 0 + that value; else default 0.0 and only elements with a
nonzero component. Nothing when `n == 0`, the array is NULL or `m == 0`. The written `k` equals
`m`, so the output reads back unchanged.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x20; `FUN_140416c90`, `FUN_140419b90`, `FUN_1404199a0`
  (`group2-helpers.md`).
* `FUN_1402b5640`, `FUN_1402a4a80` case 0x28, `FUN_1402f5c10`, `FUN_1401d0320` (component count)
  (`group2-statevar-writers.md`).

## Corpus check

* `3D/SI/Forging/Spike_ForgingBlow2.KEY` parses exactly (k = 12, 3 lines per record):

      STNCMP       1    4978      12    0.0000000000E+000
             1    3.3004758000E+001    1.0243310000E+001   -4.1616936000E+001   -1.1885655000E-001   -6.9580994000E+000
                 -1.4567515000E-001    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000

* `2D/SI/Sintering_2D/2D_Cylinder_SI.KEY`: `STNCMP       1       0       4    0.0000000000E+000`;
  `2D/EN/plane stress/Ela_PlaneStress_EN.KEY` (V9.1): `STNCMP       1       0      22  0.0000000E+000`.

## Open questions

* Meaning of the model fields `+0x86c..+0x87c`, `+0x880`, `+0x884` that size `m` (they are
  model fields, not the object's node/element counts) was not traced. A file whose `k` differs
  from the current `m` (e.g. the V9.1 file with 22) is accepted; only `min(m, k)` components are
  kept.
