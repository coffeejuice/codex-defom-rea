# NDFLUX (id 172, group 2, case 0x1c)

Per-node heat flux value of the object: one double per node in `obj+0x8f8` (state variable code
0x0b).

## KEY grammar (read)

    NDFLUX <objNo> <count:%i> [<default:%Flf> ...]      rest of the keyword line
    count × { <node:%i> <m × value:%Flf> }              m = ncomp[0xb] = model+0x1624 (default 1)

`FUN_140416c90(f, obj+0x8f8, obj+0x880, &ncomp[0xb], mode 0)` (`group2-helpers.md`): header
`sscanf("%i %Flf")`, `count` records via `fscanf`, final `fgets`. The reader does **not** test
the dimension (accepted in 2D too).

## Model storage

`obj+0x8f8[(node-1)*m + j]` (double), `n = obj+0x880` (nodes); a given default fills all
entries first. Before the mesh is read `n = 0` and records are dropped.

## Read errors and validation

* Prologue: `group2-helpers.md` → **2**.
* `FUN_140416c90` nonzero → **2**; else **0**. Out-of-range node numbers dropped.

## KEY writer

Handler `FUN_1402b5340` → `FUN_1402a4a80(model, f, obj, 0xb, N, 0)`; writer map step 32,
**only for 3D models** (`FUN_1400015f0`): a 2D model never writes NDFLUX even if it read one.
`FUN_1402f5c10(f, "NDFLUX", N, obj+0x8f8, obj+0x880, ncomp[0xb], 0.0, fmt 0)`:

    "%-6s %7i %7i %20.10E\n"         name, N, count, default
    " %7i" + m × " %20.10E" + "\n"   per written node

Uniform → count 0 + the value; else default 0.0 and only nonzero nodes. Nothing when `n == 0`,
array NULL or `ncomp[0xb] == 0`. A handler status nonzero would abort the object, but the handler
always returns 0.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x1c; `FUN_140416c90`, `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_1402b5340`, `FUN_1402a4a80` case 0xb, `FUN_1402f5c10` (`group2-statevar-writers.md`).

## Corpus check

* `3D/EN/NAFEMS_Benchmarks/NAFEMS_T2.KEY` (smallest): `NDFLUX       1       0    0.0000000000E+000`
  parses.
* No corpus file has a nonzero NDFLUX count, and no 2D file contains NDFLUX (consistent with the
  3D-only writer).

## Open questions

* None for the format.
