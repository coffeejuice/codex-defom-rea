# RSEDEF (id 302, group 2, case 0x9e)

A vector of n doubles held in the table object (`_common.md` §3) at `obj+0x20d8`. The writer
handles only n = 8, whose first value is an integer flag or type. The name suggests a
re-mesh/"RSE" definition; the code shows no string for it.

## KEY grammar (read)

    RSEDEF <obj> <n:%i>                    FUN_140413de0 (skipBlank 0), rest of the keyword line
    if n > 0:  FUN_14041a800(f, obj+0x20d8)  -> n values, fscanf("%Flf"), free format
               then one fgets (rest of the last value line)

* `n ≤ 0`: nothing more is read and the table is unchanged.
* The value count follows the table reader rules (`_common.md` §3). This assumes the table's
  `ndim` is 0, which nothing here sets: with `ndim == 0` and `ncomp = n ≥ 2` it reads n values;
  with `n == 1` it reads nothing (that value would be the constant `+8`, and is not read
  either). The corpus (8 values on the line after the header) matches `ndim == 0`.
* The first value is written as an integer (`%7i`) and read back as a double.

## Model storage

* `obj+0x2118` (= table `+0x40`, ncomp) = n.
* `FUN_14041a800` reallocates and fills `double* obj+0x2120` (table `+0x48`).
* The rest of the table (`+0x20d8` template id, `+0x20dc` ndim) is not touched by this case.

## Read errors and validation

* Line 0 EOF/blank → **2**.
* **The status of `FUN_14041a800` is ignored**: missing values at EOF give 0 (status 0) and are
  not reported.
* No messages.

## KEY writer

`FUN_1402b7140(f, model, ref)` is a write-handler-style function, but it is called directly by
`FUN_14029e250` in both dimensions (group2-writer-map.md step 60). The object comes from
`ref[0]`; it prints `obj+0x3f24` when > 0. It writes only if `obj+0x2118 == 8`:

    "%-6s %7i %7i\n"                                                     RSEDEF, N, 8
    " %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n"     (int)v[0], v[1..7]

It always returns 0. Any n other than 8 is read but never written.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140413de0` | count line |
| `FUN_14041a800` | table body reader (`_common.md` §3) |
| `FUN_1402b7140` | writer |

## Corpus check

`3D\SI\Multiple_Operations\New_MO_Release_Examples_SI\Objects\Object00003.KEY` line 509:
`RSEDEF       3       8` /
`       1    0.0000000000E+000 … 0.0000000000E+000` (int + 7 doubles) parses.
All 24 occurrences have n = 8.

## Open questions

* The table's template and ndim at `obj+0x20d8` are set at object construction (not traced).
  If ndim were not 0, the read would differ. The table lies inside the sub-object at
  `obj+0x1ea0`, which also holds the WPAXIS list at `+0x20a0`. Its constructor
  `FUN_1401860a0` and initialiser `FUN_1401861c0` (called from the object constructor
  `FUN_14015ec80` / `FUN_14015ef60`) would settle this.
* The meaning of the 8 values.
