# USRELM (id 197, group 2, case 0x35)

User-defined element state variables of the object: `m` doubles per element (state variable
code 0x3c), stored in the object's element-variable container `obj+0x9e0` (data pointer
`obj+0xac0`). The count `m` is model-wide (`model+0x33a8`) and is set by this keyword.

## KEY grammar (read)

    USRELM <objNo> <count:%i> <default:%Flf> <nvar:%i>       rest of the keyword line, commas allowed
    count × { <element:%i> <m × value:%Flf> }

Header by `FUN_140415590` (`"%i %Flf %i"`, commas → spaces, missing fields 0); records by
`FUN_1404199a0` (free layout, final `fgets`).

## Model storage

1. `nvar == 0` → `nvar = 2` (so a missing/zero count means 2 variables).
2. `FUN_1401d2090(model, nvar)`: if `model+0x33a8 != nvar`, `FUN_1401cb7d0` resizes the user
   element variable arrays of the model, `model+0x33a8 = nvar`, `FUN_1401d0fc0(model, 0)`
   re-derives the component table (`ncomp[0x3c]` = `model+0x16e8` = nvar). Model-wide, affects
   all objects.
3. `data = FUN_140184a30(obj+0x9e0)` (resizes slot 0x3c, returns `obj+0xac0`); for
   `i < obj+0x884 · model+0x16e8`: `data[i] = default` (always, 0.0 when absent).
4. Records: `data[(element-1)*m + j]`, `n = obj+0x884`, `m = model+0x16e8`; out-of-range
   element numbers are read and dropped.

## Read errors and validation

* Prologue → **2**; `FUN_140415590` nonzero → **2** immediately; `FUN_1404199a0` nonzero →
  **2** (changes kept); else **0**. Negative `nvar` is not checked.

## KEY writer

Handler `FUN_1402b5880` → `FUN_1402a4a80(model, f, obj, 0x3c, N, 0)`; writer map step 38 (last
of STRAIN … USRELM), always attempted.
`FUN_1402f5c10(f, "USRELM", N, obj+0xac0, obj+0x884, model+0x16e8, 0.0, fmt 3)`:

    "%-6s %7i %7i %20.10E %7i\n"    name, N, count, default, m
    " %7i" + m × " %20.10E" + "\n"  per written element, 5 per line

Uniform → count 0 + value; else default 0.0 and elements with a nonzero variable. Nothing when
`m == 0`, `n == 0` or the pointer is NULL. Round-trips.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x35; `FUN_140415590`, `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_1401d2090`, `FUN_1401cb7d0`, `FUN_1401d0fc0` → `FUN_1401d0320`, `FUN_140184a30`
  (element container slot 0x3c → `obj+0xac0`).
* `FUN_1402b5880`, `FUN_1402a4a80` case 0x3c, `FUN_1402f5c10` (`group2-statevar-writers.md`).

## Corpus check

* `2D/SI/Sintering_2D/2D_Insert_Hollow_Ring_Sintering_SI.KEY` parses (8 variables, 5 + 3 per record):

      USRELM       1     749  0.0000000E+000       8
             1  0.0000000E+000  0.0000000E+000  7.5000000E-007  0.0000000E+000  1.0000000E-007
                0.0000000E+000  0.0000000E+000  0.0000000E+000

* `2D/SI/Sintering_2D/2D_Cylinder_SI.KEY`: `USRELM       1       0    0.0000000000E+000       8`.

## Open questions

* As for USRNOD, whether the resize keeps old values was not traced.
