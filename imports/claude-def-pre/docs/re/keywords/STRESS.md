# STRESS (id 179, group 2, case 0x23)

Stress tensor components of the object: per element `obj+0xa00` with `m = ncomp[0x26] =
model+0x1690` (2·D, i.e. 4 in 2D and 6 in 3D; 6 for 2D torsion, `model+0x850 == 3`),
optionally with a nodal companion `obj+0x948` (`ncomp[0x1a] = model+0x1660`). When the model
stores stress per integration point (`model+0x890 == 3`) a fixed layout is used.

## KEY grammar (read)

The reader `fgets` the rest of the keyword line (status unchecked) and classifies it with
`sscanf` (no comma replacement):

**3D**: `sscanf(line, "%i %Flf %i %Flf %i")` = count, default, nnode, default2, k.

* 5 conversions → integration-point layout:

      count × { <index:%i>                           one line (FUN_140413de0; index ignored)
                p × <6 values:%Flf>              }   one line of 6 per point (FUN_140415080)

  `p = 8` if `k == 8`, else `p = 1`.
* otherwise → `FUN_140417280` mode 0 (`count1 def1 count2 def2`, element records with
  `ncomp[0x26]` values, node records with `ncomp[0x1a]` values; see STRAIN.md branch B and
  `group2-statevar-writers.md`).

**2D**: `sscanf(line, "%i %Flf %i %i %Flf %i")` = count, default, mc, nnode, default2, k.

* 6 conversions → integration-point layout, always `p = 4` points per element:
  `mc == 6` → 4 lines of 6 values (`FUN_140415080`), and if `model+0x850 != 3` it is set to 3
  with `"\n *** Info : Geometry type has been changed to torsional type due to STRESS Keyword.\n"`
  on stdout; otherwise 4 lines of 4 values (`FUN_140414e40`).
* otherwise → `FUN_140417280` **mode 3**: `"%i %Flf %i %i %Flf"` = count1, def1, (ignored),
  count2, def2.

## Model storage

* Integration-point layout: record `r` (0-based, **index ignored**, no bound check) →
  3D `obj+0xa00[r*6p + q*6 + c]`, 2D `obj+0xa00[r*4·w + q*w + c]` (w = 6 or 4). `count == 0`
  → `memset` of `k · model+0x1690 · elements` doubles (3D) or `k · mc · elements` (2D) to 0.
  Defaults are ignored. The torsion switch changes only `model+0x850`; the component table is
  not recomputed here.
* `FUN_140417280` path: `v1 = obj+0xa00`, `n1 = obj+0x884`, `m1 = model+0x1690`;
  `v2 = obj+0x948`, `n2 = obj+0x880`, `m2 = model+0x1660`; defaults fill all entries, records at
  `[(index-1)*m + j]`, out-of-range indices dropped.

## Read errors and validation

* Prologue → **2**.
* Integration-point layout: index line EOF/blank → **2** immediately; otherwise the status of
  the **last** value-line read is returned (→ **2** on failure), earlier failures ignored.
* `FUN_140417280` path: `sscanf < 1` or a record read failure → **2**.

## KEY writer

Handler `FUN_1402b4e00` → `FUN_1402a4a80(model, f, obj, 0x26, N, 0)`; writer map step 38 (after
DAMAGE), always attempted. `fmt = 3` in 2D, `0` in 3D.

* `model+0x890 == 3` (every element, no compression):
  * 3D, `obj+0x890 == 4`: `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` = N, elements, 0.0, nodes,
    0.0, **8**; per element `" %7i\n"` then 8 × `" %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n"`;
  * 3D, `obj+0x890 == 2`: same with **1** and one 6-value line; other element types: nothing;
  * 2D, `model+0x850 == 3`: `"%-6s %7i %7i %20.10E %7i %7i %20.10E %7i\n"` = N, elements, 0.0,
    `model+0x1690`, nodes, 0.0, **4**; per element `" %7i\n"` + 4 lines of 6 values;
  * 2D otherwise: same header, per element `" %7i\n"` + 4 × `" %20.10E %20.10E %20.10E %20.10E\n"`.
* otherwise, the object's model `ncomp[0x1a]` is 0 or `obj+0x948 == NULL`:
  `FUN_1402f5c10(f, "STRESS", N, obj+0xa00, elements, model+0x1690, 0.0, fmt)` →
  3D `"%-6s %7i %7i %20.10E\n"` (count, default), 2D `"%-6s %7i %7i %20.10E %7i\n"`
  (count, default, m); compressed records `" %7i"` + m values (5 per line);
* otherwise `FUN_1402f60c0(..., fmt)`: 3D `"%-6s %7i %7i %20.10E %7i %20.10E\n"` (count1, def1,
  count2, def2), 2D `"%-6s %7i %7i %20.10E %7i %7i %20.10E\n"` (count1, def1, m1, count2, def2);
  element records then node records.

Round trip: the writer's headers have 5 (3D) / 6 (2D) numbers only in integration-point mode;
the compressed headers have 2/4 (3D) or 3/5 (2D) numbers, so the reader branches correctly.
2D integration-point output written with `model+0x1690 == 6` but `model+0x850 != 3` cannot
occur (6 implies torsion in `FUN_1401d0320`).

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x23; `FUN_140413de0`, `FUN_140414e40`, `FUN_140415080`,
  `FUN_1404199a0` (`group2-helpers.md`); `FUN_140417280`; `FUN_140478d30` = `memset`;
  `FUN_14047903c` (stdout).
* `FUN_1402b4e00`, `FUN_1402a4a80` case 0x26, `FUN_1402f5c10`, `FUN_1402f60c0`
  (`group2-statevar-writers.md`).

## Corpus check

* `2D/SI/Sintering_2D/2D_Cylinder_SI.KEY` (2D, single array, fmt 3) parses:

      STRESS       1     217    0.0000000000E+000       4
             1   -3.9726700000E+001   -2.7693000000E+002   -3.9727800000E+001    3.7598800000E-001

* `2D/SI/Creep/CREEP_Example1_SI.KEY` (2D, two arrays, mode 3):
  `STRESS       1       0  0.0000000E+000       4       0  0.0000000E+000`.
* `3D/SI/Forging/Spike_ForgingBlow2.KEY` (3D, 6 values over 2 lines):

      STRESS       1    4978    0.0000000000E+000
             1    1.7659934000E+001    1.1054376000E+000   -8.7159086000E+000   -8.8597302000E-004   -1.7962966000E+000
                 -2.2915995000E-001

* No corpus file uses the integration-point layout.

## Open questions

* What sets `model+0x890` (stress storage mode) was not traced. Component order of the 4/6
  values is not evidenced in the code read.
