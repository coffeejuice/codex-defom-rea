# NDHEAT (id 171, group 2, case 0x1b)

Per-node heat value of the object: one double per node in `obj+0x8e8` (state variable code 9).
The code reads, stores and writes it back; no further meaning is evidenced here.

## KEY grammar (read)

    NDHEAT <objNo> <count:%i> [<default:%Flf> ...]      rest of the keyword line
    count × { <node:%i> <m × value:%Flf> }              m = ncomp[9] = model+0x161c (default 1)

`FUN_140416c90(f, obj+0x8e8, obj+0x880, &ncomp[9], mode 0)` (`group2-helpers.md`): header
`sscanf("%i %Flf")` (no comma replacement), `count` records read with `fscanf` (free line
layout; the writer's 5-per-line wrapping is accepted), then one `fgets` ending the last record
line; `count ≤ 0` reads nothing more. Same in 2D and 3D.

## Model storage

`obj+0x8e8[(node-1)*m + j]` (double), `n = obj+0x880` (nodes). A given default is first stored
in every entry (helper rule). Before the mesh is read `n = 0` and records are dropped.

## Read errors and validation

* Prologue (object number / lookup): `group2-helpers.md` → **2**.
* `FUN_140416c90` nonzero (EOF, empty header line, EOF in records or final `fgets`) → **2**;
  else **0**.
* Node numbers outside `1..n` are read and dropped; non-numeric tokens are not detected.

## KEY writer

Handler `FUN_1402b5280` → `FUN_1402a4a80(model, f, obj, 9, N, 0)`; writer map step 31 (after
NDTMP), always attempted. `FUN_1402f5c10(f, "NDHEAT", N, obj+0x8e8, obj+0x880, ncomp[9], 0.0, fmt 0)`:

    "%-6s %7i %7i %20.10E\n"         name, N, count, default
    " %7i" + m × " %20.10E" + "\n"   per written node (5 values per line)

Uniform → count 0 + the value; else default 0.0 and only nodes with a nonzero component.
Nothing when `n == 0`, array NULL or `ncomp[9] == 0`. Round-trips (to `%20.10E` precision).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x1b; `FUN_140416c90`, `FUN_1404199a0` (`group2-helpers.md`).
* `FUN_1402b5280`, `FUN_1402a4a80` case 9, `FUN_1402f5c10`, `FUN_1402f5360`, `FUN_1402f2e80`
  (`group2-statevar-writers.md`).

## Corpus check

* `2D/EN/plane stress/Ela_PlaneStress_EN.KEY`: `NDHEAT       1       0  0.0000000E+000` parses.
* `3D/SI/Forging/Spike_ForgingBlow2.KEY`: `NDHEAT       1       0    0.0000000000E+000`.
* No corpus file has a nonzero NDHEAT count (`rg "^NDHEAT +\d+ +[1-9]"`), so records are only
  checked through the same helpers as DENSTY / NDTMP 3D.

## Open questions

* None for the format.
