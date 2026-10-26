# USRNOD (id 196, group 2, case 0x34)

User-defined nodal state variables of the object: `m` doubles per node (state variable code
0x11), stored in the object's node-variable container `obj+0x8a0` (data pointer `obj+0x968`).
The number of variables `m` is **model-wide** (`model+0x3398`) and is set by this keyword.

## KEY grammar (read)

    USRNOD <objNo> <count:%i> <default:%Flf> <nvar:%i>       rest of the keyword line, commas allowed
    count × { <node:%i> <m × value:%Flf> }                   m = nvar (after the model update)

Header read by `FUN_140415590` (`"%i %Flf %i"`, commas replaced by spaces, missing fields 0).
Records by `FUN_1404199a0(f, data, count, n, m)` (`fscanf`, free layout; final `fgets`).

## Model storage

1. `FUN_1401d21c0(model, nvar)`: if `model+0x3398 != nvar`, `FUN_1401cba80` resizes the user node
   variable arrays of the model (old count → new count), `model+0x3398 = nvar`, and
   `FUN_1401d0fc0(model, 0)` re-derives the component table, so `ncomp[0x11]` (`model+0x163c`) =
   `nvar`. This affects **every object** of the model; with several objects the last USRNOD read
   decides the count for all.
2. `data = FUN_1401815b0(obj+0x8a0)` (resizes slot 0x11 of the node container to
   `nodes × ncomp[0x11]` and returns `obj+0x968`); for `i < obj+0x880 · model+0x163c`:
   `data[i] = default` (the default is always applied, 0.0 when absent). The pointer is fetched
   again for every element of this loop (each call may resize; harmless).
3. Records: `data[(node-1)*m + j]`, `n = obj+0x880`, `m = model+0x163c`; nodes outside `1..n`
   are read and dropped. `nvar = 0` → `m = 0`, records then contain only the node number.

## Read errors and validation

* Prologue → **2**.
* `FUN_140415590` nonzero (EOF, blank rest of line) → **2** immediately, before any change.
* `FUN_1404199a0` nonzero (EOF in a record or at the final `fgets`) → **2** (the model/object
  changes are kept). Else **0**. No check of `nvar` (negative values are passed on to
  `FUN_1401cba80`).

## KEY writer

Handler `FUN_1402b5580` → `FUN_1402a4a80(model, f, obj, 0x11, N, 0)`; writer map step 35,
always attempted. `FUN_1402f5c10(f, "USRNOD", N, obj+0x968, obj+0x880, model+0x163c, 0.0, fmt 3)`:

    "%-6s %7i %7i %20.10E %7i\n"    name, N, count, default, m
    " %7i" + m × " %20.10E" + "\n"  per written node, 5 per line

Uniform → count 0 and the value; else default 0.0 and only nodes with a nonzero variable.
Nothing when `m == 0` (so a model without user node variables writes no USRNOD at all),
`n == 0` or the pointer is NULL. Reading the output restores `m` and the values.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x34; `FUN_140415590`, `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_1401d21c0` (set model user-node-variable count), `FUN_1401cba80` (resize), `FUN_1401d0fc0`
  → `FUN_1401d0320` (component table), `FUN_1401815b0` (node container slot 0x11 → `obj+0x968`).
* `FUN_1402b5580`, `FUN_1402a4a80` case 0x11, `FUN_1402f5c10` (`group2-statevar-writers.md`).

## Corpus check

* `3D/SI/Forging/3D_Germanbenchmark.KEY` parses (V9-style `%15.7E`):

      USRNOD       3       6  0.0000000E+000       2
          2945  1.5810101E-322  0.0000000E+000

* `2D/SI/viewfactor/Viewfactor2_SI.KEY`: `USRNOD       1       0    0.0000000000E+000      16`.
* `2D/EN/plane stress/Ela_PlaneStress_EN.KEY`: `USRNOD       1       0  0.0000000E+000       2`.

## Open questions

* Whether `FUN_1401cba80` keeps old values when the count changes was not traced (it matters
  only for multi-object files with different `nvar`).
