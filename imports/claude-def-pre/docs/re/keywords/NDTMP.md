# NDTMP (id 170, group 2, case 0x1a)

Nodal temperature of the object: one double per node in `obj+0x8e0`; in 2D the KEY line can also
carry a second column from which the per-node temperature increment `obj+0x900` (internal state
variable code 0x0c `"*DTMP"`) is derived.

## KEY grammar (read)

3D (`FUN_1400015e0` false):

    NDTMP <objNo> <count:%i> [<default:%Flf> ...]            (FUN_140416c90 mode 0)
    count × { <node:%i> <m × value:%Flf> }                   m = model+0x1618 (ncomp[8], default 1)

Exactly `FUN_140416c90(f, obj+0x8e0, obj+0x880, &model->ncomp[8], mode 0)` (see
`group2-helpers.md`): header `"%i %Flf"` on the rest of the keyword line; records read with
`fscanf`, free line layout, then one `fgets` ending the last record line.

2D:

    NDTMP <objNo> <count:%i> <default:%Flf> <k:%i>           (FUN_140415590, commas allowed)
    k == 2:  count × { <node:%i> <a:%Flf> <T:%Flf> }
    k != 2:  count × { <node:%i> <m × T:%Flf> }              m = model+0x1618

Missing `default`/`k` read as 0 (partial line accepted). With `k == 2` the pair is
(T − ΔT, T). Records and the final `fgets` as in `FUN_1404199a0`.

## Model storage

`n = obj+0x880` (number of nodes). Arrays are the object's node arrays (allocated by the mesh
readers; if the mesh is not read yet `n = 0` and every record is dropped).

* 3D: `obj+0x8e0[(node-1)*m + j]`; defaults as in `FUN_140416c90` mode 0 (only when a default is
  given). `obj+0x900` is not touched.
* 2D: first, for every node, `obj+0x8e0[i] = default` and `obj+0x900[i] = 0.0` (both whatever
  `k` is, and only `n` entries, independent of m).
  * `k == 2`: a scratch buffer of `2n` doubles is filled with `default`, records go to
    `buf[(node-1)*2 + 0/1]`, then for every node `T = buf[2i+1]`, `ΔT = buf[2i+1] − buf[2i]`
    (`obj+0x8e0[i] = T`, `obj+0x900[i] = ΔT`). Unlisted nodes: T = default, ΔT = 0.
  * `k != 2`: `FUN_1404199a0(f, obj+0x8e0, count, n, m)`; ΔT stays 0.

No other fields are changed.

## Read errors and validation

* Common prologue (object number, object lookup): see `group2-helpers.md` (→ 2).
* 3D: `FUN_140416c90` nonzero (EOF / empty header line / record EOF) → **2**.
* 2D: `FUN_140415590` nonzero (EOF or blank rest-of-line) → **2** immediately (nothing stored).
  Record read status of `FUN_1404199a0` (EOF in a record or in the final `fgets`) → **2**,
  but with `k == 2` the node arrays are still updated from the scratch buffer first.
* Node numbers outside `1..n` are read and dropped. No other validation.

## KEY writer

Handler `FUN_1402b4280` → `FUN_1402a4a80(model, f, obj, 8, N, 0)`; emitted in
`FUN_14029e250` after `DRZ … BCCTFN` (writer map step 31), always attempted.

* 3D: `FUN_1402f5c10(f, "NDTMP", N, obj+0x8e0, n, ncomp[8], 0.0, fmt 0)`:
  `"%-6s %7i %7i %20.10E\n"` (count, default), records `" %7i"` + m × `" %20.10E"` (5 per line).
* 2D: builds `buf[2i] = T − ΔT` (`obj+0x8e0[i] − obj+0x900[i]`), `buf[2i+1] = T`, and calls
  `FUN_1402f5c10(f, "NDTMP", N, buf, n, 2, 0.0, fmt 3)`:
  `"%-6s %7i %7i %20.10E %7i\n"` = count, default, **2**; records `" %7i %20.10E %20.10E\n"`.
  `ncomp[8]` is not used in 2D.
* Compression (`group2-statevar-writers.md`): all values equal → count 0 and that value
  (2D: only possible when ΔT = 0 everywhere); otherwise default 0.0 and only nodes with a
  nonzero value are written.
* Nothing is written when `n == 0`, the array is NULL, or (3D) `ncomp[8] == 0`.

Round trip: 3D and 2D layouts read back to the same arrays (to `%20.10E` precision; 2D ΔT is
recomputed as T − (T − ΔT) and can differ in the last bits).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x1a — reader.
* `FUN_140416c90`, `FUN_1404199a0`, `FUN_140415590` — see `group2-helpers.md`.
* `FUN_1402b4280` — write handler; `FUN_1402a4a80` case 8; `FUN_1402f5c10`, `FUN_1402f5360`,
  `FUN_1402f2e80` — see `group2-statevar-writers.md`.

## Corpus check

* `2D/SI/HeatTreat/MRX_SRX_2D_SI.KEY` (V11 2D): parses with the 2D `k == 2` grammar:

      NDTMP        1    1109    0.0000000000E+000       2
             1    1.0960447000E+003    1.0960561000E+003

* `3D/SI/Forging/Spike_ForgingBlow2.KEY`: 3D mode-0 grammar, `NDTMP        1    1269    0.0000000000E+000`
  then `       1    7.2709844444E+002`.
* `2D/EN/plane stress/Ela_PlaneStress_EN.KEY` (V9.1, `%15.7E`): `NDTMP        1       0  6.8000000E+001       2`.

## Open questions

* Whether `obj+0x900` (ΔT) is used anywhere except this round trip was not traced.
