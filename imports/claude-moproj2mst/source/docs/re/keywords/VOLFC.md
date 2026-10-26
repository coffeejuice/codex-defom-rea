# VOLFC (id 205, group 2, case 0x3d)

Per-element phase volume fractions of the object (state variable code 0x31): stored as
`obj+0xa58[element*mG + phase]` over **all** `mG = model+0x1320` phases of the model, but written
and read in the KEY file only for the phases that occur in the object's mixture materials
(local columns).

## KEY grammar (read)

    VOLFC <objNo> <count:%i> <k:%i> [<default:%Flf> ...]    rest of the keyword line
    count × { <element:%i> <k × value:%Flf> }               (k == 0: mL values per record)

`FUN_1403981c0(model, f, obj, obj+0xa58, n = obj+0x884, mG = model+0x16bc)` reads the block
with `FUN_140416c90(f, tmp, n, &mL, mode 1)` (`group2-helpers.md`: header `"%i %i %Flf"` =
count, k, default; records via `FUN_140419b90`, index + k values; no final `fgets` when k > 0).

## Model storage

1. `map`/`mL` from `FUN_140398350(model, obj, map, &mL)` (see `group2-statevar-writers.md`,
   "phase-column gather"): for each mixture material (`mat+0x1cc8 == 1`) used by the object's
   elements (`int obj+0x9f8`, i.e. MTLGRP must already be read), its phase materials
   (`mat+0x1ccc` / `mat+0x1cd0`) get local columns `0..mL-1` in global phase order;
   `map[c] = -1` for phases not used. Nothing is computed when `obj+0x87c == 0` (map stays all 0,
   `mL` uninitialised).
2. `tmp` = `n·mL` doubles (zeroed); `FUN_140416c90` fills it (default to all entries if given;
   record values `min(mL, k)` at `tmp[(element-1)*mL + j]`; out-of-range elements dropped).
3. Only if the read succeeded: for every element `e < n` and global phase `c < mG` with
   `map[c] ≥ 0`: `obj+0xa58[e*mG + c] = tmp[e*mL + map[c]]`. Columns of unused phases keep
   their previous values; elements not listed get the default (or 0.0).
4. `mG = model+0x16bc` = `ncomp[0x31]` = `model+0x1320` when `FUN_1401d64f0(model, 0x31)`
   (mixture material present or `model+0x85c != 0`), else 0 (then nothing is stored).

## Read errors and validation

* Prologue → **2**.
* `FUN_140416c90` nonzero (header EOF / blank, EOF in a record) → **2**; the object array is
  then left unchanged (and the two scratch buffers are not freed). Else **0**.

## KEY writer

Handler `FUN_1402b5940` → `FUN_1402a4a80(model, f, obj, 0x31, N, 0)` → `FUN_1402a3690(model, f,
"VOLFC", obj, N, obj+0xa58, model+0x16bc, 0)`; writer map step 41, always attempted. It gathers
the local columns (`buf[e*mL + map[c]] = v[e*mG + c]`) and calls
`FUN_1402f5c10(f, "VOLFC", N, buf, obj+0x884, mL, 0.0, fmt 1)`:

    "%-6s %7i %7i %7i %20.10E\n"    name, N, count, mL, default
    " %7i" + mL × " %20.10E" + "\n" per written element, 5 per line

Uniform → count 0 + the value; else default 0.0 and only elements with a nonzero fraction.
Nothing when `mL == 0` (no mixture phases), `n == 0`. Round-trips when the element materials
and the material data are the same at read time as at write time.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x3d; `FUN_1403981c0` — phase scatter reader; `FUN_140398350` — phase map;
  `FUN_14029c870` (phase index of a material number), `FUN_1401d7060` (material record);
  `FUN_140416c90`, `FUN_140419b90` (`group2-helpers.md`).
* `FUN_1402b5940`, `FUN_1402a4a80` case 0x31, `FUN_1402a3690`, `FUN_1402f5c10`
  (`group2-statevar-writers.md`). VOLFS (id 216, code 0x32, `obj+0xa60`, `model+0x16c0`) uses
  the same reader and writer.

## Corpus check

* `2D/EN/HeatTreat/HT_CARB1_EN.KEY` parses (mL = k = 3):

      VOLFC        1     521       3    0.0000000000E+000
             1    9.9999800867E-001    1.9913300173E-006    0.0000000000E+000

## Open questions

* `mL` is uninitialised when `obj+0x87c == 0` (reader and writer); what `obj+0x87c` means
  (probably "mesh present") was not traced.
