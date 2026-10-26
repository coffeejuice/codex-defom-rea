# DAMAGE (id 178, group 2, case 0x22)

Damage value(s) of the object: per element `obj+0xa38` (element container `obj+0x9e0`, slot
0x2d, `m = ncomp[0x2d] = model+0x16ac`, initialised from `model+0x33d8`), optionally a nodal
companion `obj+0x940` (node container `obj+0x8a0`, slot 0x1c, `ncomp[0x1c] = model+0x1668`).
With `model+0x888 == 3` the values are stored per integration point.

## KEY grammar (read)

The reader `fgets` the rest of the keyword line (status unchecked) and counts the conversions
of `sscanf(line, "%i %Flf %i %Flf %i %i")` (same in 2D and 3D):

**6 conversions → `FUN_140418e40`** (integration-point layout):

    DAMAGE <objNo> <count> <def> <i3> <d4> <m> <ip>
    count × { <element:%i>  ip × m × <value:%Flf> }      all via fscanf, free layout

`i3`, `d4` ignored; `ip < 1` → 1. No final `fgets`.

**5 conversions → `FUN_140418910` mode 5**, **otherwise mode 0**:

    DAMAGE <objNo> <count1:%i> <def1:%Flf> <count2:%i> <def2:%Flf> [<k:%i>]     (mode 5 has k)
    count1 × { <element:%i> <values> }
    count2 × { <node:%i>    <values> }

Records via `FUN_140419b90`: with `k > 0` each record has `k` values and there is no final
`fgets`; with `k == 0` (mode 0) each element record has `m` and each node record `ncomp[0x1c]`
values, and each block ends with one `fgets`.

## Model storage

**`FUN_140418e40`** (`sscanf < 1` → 1):
* `m > 0` → `ncomp[0x2d] = m`; `FUN_140183840(obj+0x9e0, 0x2d, m)` resizes the element slot
  (only when the container is set up), `v = FUN_1401840d0(obj+0x9e0)` = `obj+0xa38`;
* all `elements · ip · ncomp[0x2d]` values = def;
* records: `fscanf("%i")` index (EOF → 1); if `1 ≤ index ≤ elements` read `ip·m` values
  (`fscanf("%Flf")`, EOF → 1) into `v[(index-1)*ip*m + p*m + j]`; **if the index is out of
  range its values are not consumed** and are parsed as the next indices (stream desync).

**`FUN_140418910`** (`sscanf < 1` → 1; other mode values print
`"KwdReadUtl::readNodeElemVarDouble4() - not supported format.\n"`, never happens here):
* `k > 0` → `ncomp[0x2d] = k`; resize element slot, `v1 = obj+0xa38`;
* if `ncomp[0x1c] != 0`: `k > 0` → `ncomp[0x1c] = k` too; resize node slot 0x1c,
  `v2 = FUN_1401812a0(obj+0x8a0)` = `obj+0x940`;
* all `elements · ncomp[0x2d]` of v1 = def1; if v2: all `nodes · ncomp[0x1c]` = def2;
* `count1 != 0` → `FUN_140419b90(f, v1, count1, elements, ncomp[0x2d], k)`;
* `count2 != 0` → `FUN_140419b90(f, v2, count2, nodes, ncomp[0x1c], k)`; without a nodal
  array the records are read with a NULL target and `m = 0` (values dropped; with `k == 0`
  only the index is consumed per record).

Out-of-range indices in `FUN_140419b90` records are read and dropped.

## Read errors and validation

* Prologue → **2**.
* Either reader returning nonzero (blank/EOF header, EOF in records) → **2**; else **0**.

## KEY writer

Handler `FUN_1402b4ec0` → `FUN_1402a4a80(model, f, obj, 0x2d, N, 0)`; writer map step 38 (after
DENSTY), always attempted. `m = model+0x16ac`.

* `model+0x888 == 3`: `FUN_1402f58d0(f, "DAMAGE", N, obj+0xa38, elements, m, 0.0, ip, fmt 5)`,
  `ip` = 8 (3D, `obj+0x890 == 4`), 1 (3D otherwise), 4 (2D):
  `"%-6s %7i %7i %20.10E %7i %20.10E %7i %7i\n"` = count, default, 0, 0.0, m, ip; per
  written element `" %7i\n"` then `ip` lists of `m` values (`" %20.10E"`, 5 per line, each list
  ends with `"\n"`). Compressed: all values equal → count 0 + value; else default 0.0 and
  elements with any nonzero value.
* otherwise, the object's model `ncomp[0x1c]` is 0 or `obj+0x940 == NULL`:
  * `m == 1`: `FUN_1402f5c10(..., fmt 0)` → `"%-6s %7i %7i %20.10E\n"` + records;
  * else `FUN_1402f5c10(..., fmt 5)` → `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` =
    count, default, 0, 0.0, m (5 numbers → reader mode 5 with `k = m`);
* otherwise `FUN_1402f60c0(..., fmt 5)`: `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` = count1,
  def1, count2, def2, m; element records (m values) then node records (`ncomp[0x1c]` values).

Round trip: header lengths 6 / 5 / 2 numbers select the matching reader. With two arrays the
reader uses `k = m` for the node records too and forces `ncomp[0x1c] = m`; if
`ncomp[0x1c] != ncomp[0x2d]` before writing, the nodal block does not read back correctly.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x22; `FUN_140418e40` (integration-point reader), `FUN_140418910`
  (`readNodeElemVarDouble4`), `FUN_140419b90` (`group2-helpers.md`), `FUN_140183840` /
  `FUN_1401840d0` (element slot resize / pointer), `FUN_140180310` / `FUN_1401812a0` (node slot).
* `FUN_1402b4ec0`, `FUN_1402a4a80` case 0x2d, `FUN_1402f58d0`, `FUN_1402f55b0`, `FUN_1402f5c10`,
  `FUN_1402f60c0` (`group2-statevar-writers.md`).

## Corpus check

* `3D/SI/Forging/Spike_ForgingBlow2.KEY` (mode 0) parses:

      DAMAGE       1    3619    0.0000000000E+000
             1    9.7642266000E-002

* `3D/SI/Extrusion/Lagrangian/Extrude_U_LAG_Cold.KEY` (mode 5, k = 1):
  `DAMAGE       1       0    0.0000000000E+000       0    0.0000000000E+000       1`.
* No corpus file uses the 6-number integration-point header.

## Open questions

* `FUN_140418e40` fills `elements · ip · m` values but the slot is resized by the component
  table (`elements · m`?); whether the container allocates `ip` times more was not traced
  (possible overflow for ip > 1).
* What sets `model+0x888` was not traced.
