# STROKE (id 153, group 2, case 0x09)

The object's stroke vector: its accumulated displacement, one double per coordinate. It is
stored at `obj+0x22d0` (X), `obj+0x22d8` (Y) and `obj+0x22e0` (Z, 3D only). The code supports
this reading in four places:
* The positioning code `FUN_140378e00` adds each translation it applies to the object
  (`"Object %d has been moved by (%g, %g, %g)."`) to `+0x22d0/+0x22d8/+0x22e0`, except for
  movement type 4.
* For a mechanical press (movement type 4), the data check in `FUN_140167e90` recomputes the
  three components from the press kinematics (`FUN_1401952e0` on the movement block at
  `obj+0x2160`). When they differ it prints `WARNING  : Inconsistent Mechanical Stroke data
  corrected for Object %6d`.
* For movement type 6, `FUN_1402007d0` copies a 2- or 3-vector from the movement block into
  these fields.
* The text menus print the fields as `" STROKE =   %lf  %lf  %lf? "` (3D) and
  `" STROKE =      %lf  %lf ? "` (2D).

**Answer to the DSMAX_NSTEP question:** `obj+0x22e0` is the **Z component** of this
displacement vector. Only the 3D reader sets it. In 2D the reader leaves it unchanged, the 2D
writer does not write it, and the 2D menu does not show it (it keeps whatever an earlier 3D
read or the positioning code put there).

## KEY grammar (read)

    STROKE <objNo:%i> <sx:%Flf> <sy:%Flf> [<sz:%Flf>]     (rest of the line, commas allowed)

* The reader takes a 2D branch (`FUN_1400015e0`, model `+0x1c == 2`) and a non-2D branch (3D,
  or a dimension that is not set yet).
* 2D: `FUN_140414a40` (`"%Flf %Flf"`). The reader takes 2 values. Extra tokens are ignored.
* Not 2D: `FUN_140414c20` (`"%Flf %Flf %Flf"`). The reader takes 3 values.
* One line only. Missing values become 0 (see group2-helpers.md, "Rest-of-line scalar readers").

## Model storage

| Field | Type | 2D | not 2D |
|---|---|---|---|
| `obj+0x22d0` | double | sx | sx |
| `obj+0x22d8` | double | sy | sy |
| `obj+0x22e0` | double | unchanged | sz |

Reading has no side effects. The helper zeroes its outputs first, so the fields it writes get 0
when a value is missing.

## Read errors and validation

* Object number: EOF → 2; objNo < 1 → 2 (group2-helpers.md §1–2).
* The helper returns 1 on EOF or on a blank line (`sscanf == -1`) → parser returns **2**
  ("Reading Error"). The fields are already zeroed by then.
* A non-numeric first token gives `sscanf == 0`. That counts as success: all values are 0 and
  the parser returns 0.
* No range checks.

## KEY writer

`FUN_1402b0350` (registry handler2). It is called from the movement-block writer
`FUN_1402a7c80`, after PRSTRE and before FORCE. Its result is checked, but it always returns 0.

* Condition: `obj+0xc != 5`. Objects of type 5 never get a STROKE line, even though
  `FUN_14029e250` does call the movement block for them.
* Not 2D: `"%-6s %7i %20.10E %20.10E %20.10E\n"`, name, N, `+0x22d0`, `+0x22d8`, `+0x22e0`.
* 2D: `"%-6s %7i %20.10E %20.10E\n"`, name, N, `+0x22d0`, `+0x22d8`.
* N is the object number, or `obj+0x3f24` when that is > 0 (renumbering).
* The output uses the MSVC CRT's three-digit exponent, e.g. `0.0000000000E+000`.
* All values are written even when they are 0. The line round-trips exactly through the reader.

The movement block is emitted by `FUN_14029e250` when `!(flags & 0x40000)` (and always for type 5
objects). It is also emitted by the single-object "equipment" file writer `FUN_1401c6d00`
(file header + `FUN_1402a7c80` only).

## Unit conversion

None.

## Helpers

* `FUN_140414a40`, `FUN_140414c20`: rest-of-line readers that take 2 or 3 doubles.
* `FUN_1402b0350`: writer (handler2). `0x1402f7610`: handler3 (no-op).
* `FUN_1402a7c80`: movement-block writer.
* `FUN_140378e00`: positioning; accumulates translations into the stroke vector.
* `FUN_140167e90`: data check (mechanical-press stroke correction).
* `FUN_1402007d0`: copies the stroke vector for movement type 6.
* `FUN_14039e670`: copies the object to another structure (`FUN_1403a83d0(…, sx, sy, sz)`).

## Corpus check

The script went through all corpus KEY files. Each STROKE line has 2 values in 2D files (617
lines) and 3 values in 3D files (416 lines). Files without a dimension header follow their
directory: 2D → 2 values, 3D → 3 values. Example
`2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Equipments\Equipment00001.KEY`:

    STROKE       1    0.0000000000E+000    0.0000000000E+000

Older files use `%15.7E`-style numbers (`0.0000000E+000`). These parse the same.

## Open questions

* Is the 2D-menu use of `+0x22e0` in DSMAX_NSTEP (group 1) intended? Since 2D never writes the
  field, it can be stale or 0. Only a check of how that code is used would settle it; nothing
  here needs it.
