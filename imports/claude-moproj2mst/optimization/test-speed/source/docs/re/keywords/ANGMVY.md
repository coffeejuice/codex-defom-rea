# ANGMVY (id 203, group 2, case 0x3b)

The 2D angular movement of the object about its axis (2D only), used for torsion and inertia
welding.
* `obj+0x2ac0` holds the mode:
  * 0: torque table at `obj+0x2ac8`;
  * 1: angular velocity table at `obj+0x2b40`;
  * 2: four plain doubles.
* The mode names come from the handler3 `FUN_1402b0ef0`. It maps the table name `"Torque"` to
  `+0x2ac8` and `"Angular Velocity"` to `+0x2b40`.
* An extra double `obj+0x2bd0` goes with every mode.

## KEY grammar (read)

    ANGMVY <objNo:%i> <mode:%i> <kind:%i> <d1:%Flf> <d2:%Flf> <d3:%Flf> <d4:%Flf>
    [mode 0/1 with kind 1 or 2: (int)d1 pairs "x y", free format]

* **2D only** (`FUN_1400015e0`). In 3D or with an unset dimension, the case reads nothing
  after the object number and returns 0. The interpreter then reads the rest of the line (and
  any table lines) as unknown keywords: warning, line skipped.
* Line 0 is read by `FUN_140416220` (`"%i %i %Flf %Flf %Flf %Flf"`). It replaces commas and
  zeroes all six outputs first. Missing values become 0.
* mode 0 or 1:
  * `obj+0x2bd0 = d2`;
  * T (`+0x2ac8` for mode 0, `+0x2b40` for mode 1) is reset with `FUN_14012fc70(T, 0)`;
  * then by kind:
    * kind 0: constant `T+8 = d1`, `T.ndim = 0`;
    * kind 1: `T.ndim = 1`, `T.var[0] = 1`, `T.cnt[0] = (int)d1`;
    * kind 2: `T.ndim = 1`, `T.var[0] = 10`, `T.cnt[0] = (int)d1`;
    * other kinds: T stays a constant 0;
  * the body is read by `FUN_14041a800` (as in ANGMOV.md: `cnt` pairs, then one `fgets`).
* mode 2: `obj+0x2ac0 = 2` and `+0x2bb8 = d1`, `+0x2bc0 = d2`, `+0x2bc8 = d3`, `+0x2bd0 = d4`.
  `kind` is read and ignored. No body, no table touched.
* Any other mode: nothing stored, nothing more read; the case returns 0.

## Model storage

| Field | Type | mode 0 | mode 1 | mode 2 |
|---|---|---|---|---|
| `obj+0x2ac0` | int | 0 | 1 | 2 |
| `obj+0x2bd0` | double | d2 | d2 | d4 |
| `obj+0x2bb8`, `+0x2bc0`, `+0x2bc8` | double | — | — | d1, d2, d3 |
| table T | 0x78-byte table object | `+0x2ac8` | `+0x2b40` | — |
| T.ndim | int | `+0x2acc` | `+0x2b44` | |
| T+8 constant | double | `+0x2ad0` | `+0x2b48` | |
| T.var[0] | int | `+0x2ad8` | `+0x2b50` | |
| T.cnt[0] | int | `+0x2ae4` | `+0x2b5c` | |

Fields of the other modes are left unchanged.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* 2D: line helper EOF or blank line → **2**, with nothing changed. Body EOF → **2**; at that
  point the mode and `+0x2bd0` are already set.
* Not 2D → 0. No range checks.

## KEY writer

`FUN_1402b0a40` (handler2). It is called from `FUN_1402a7c80` **only in 2D**, after ANGMOV and
before CNTRAX. It is written for all object types (type 5 included).

* Condition: `model+0x850 == 3`, **or** `model+0x850 == 1` and `FUN_140186da0(obj+0x1ea0) > 0`.
  * `model+0x850` is the 2D geometry type. The data check prints `"Invalid rotation defined
    for axisymmteric/torsion Object"` for values 1/3, and group2-statevar-writers.md calls 3
    torsion.
  * `FUN_140186da0` counts entries whose `+4 == 6` in the list at `obj+0x20a0`.
  * Otherwise nothing is written and the handler returns 0.
* mode 0 or 1 (T as above):
  * T.ndim 0: `"%-6s %7i %7i %7i %20.10E %20.10E\n"`, name, N, mode, `0`, `T+8`, `+0x2bd0`.
  * T.ndim 1: kind = 1 for var code 1, 2 for var code 10, otherwise **return 3**.
    `"%-6s %7i %7i %7i %7i %20.10E\n"`, name, N, mode, kind, `T.cnt[0]` (an int where the
    reader takes `%Flf`, read back as a double and truncated, so it round-trips), `+0x2bd0`.
  * ndim not 0/1 → **return 3**.
  * Then `FUN_1402f3130(f, T, 0)`: a constant writes nothing; ndim 1 writes one line per point,
    `" %20.10E %20.10E\n"`.
* mode 2: `"%-6s %7i %7i %7i %20.10E %20.10E %20.10E %20.10E\n"`, name, N, `2`, `0`, `+0x2bb8`,
  `+0x2bc0`, `+0x2bc8`, `+0x2bd0`.
* Other modes: nothing written.
* A return of 3 stops `FUN_1402a7c80`, so CNTRAX, OBJUPD and OBJSTP are skipped silently (the
  caller ignores the status).
* The values are not round-trip-checked against the condition: a 2D file whose geometry type
  fails the condition loses its ANGMVY on re-write.

## Unit conversion

None.

## Helpers

* `FUN_140416220`: `"%i %i %Flf %Flf %Flf %Flf"` reader.
* `FUN_14012fc70`: table reset.
* `FUN_14041a800`: table body reader.
* `FUN_1402b0a40`: writer.
* `FUN_1402f3130`: table writer.
* `FUN_140186da0`: counts list entries of type 6 for the writer condition.
* `FUN_1402b0ef0`: handler3. Table by name (`"Torque"`, `"Angular Velocity"`, else 3). GUI
  only, not on the KEY path.
* `FUN_140003f50`: object summary print (`"  12     ANGMVY  by: %s "`).

## Corpus check

Only `2D\SI\Inertia_Welding\Inertia_Welding_Example1_SI.KEY` and its EN copy use ANGMVY (16
lines). All of them parse.

    ANGMVY       1       1       1       4    0.0000000000E+000
        0.0000000000E+000    0.0000000000E+000
        2.0000000000E-001    1.5000000000E+001

This line is mode 1, kind 1, 4 points, `+0x2bd0` = 0. The other objects have
`ANGMVY N 1 0 0.0 0.0`.

## Open questions

* The meaning of `+0x2bd0` and the mode-2 values `+0x2bb8..+0x2bc8` (perhaps flywheel inertia
  or energy for inertia welding). The code that uses them, e.g. the setters `FUN_1403a8xxx`
  called from `FUN_14039e670`, would settle it.
* What the type-6 list entries at `obj+0x20a0` are (the condition for axisymmetric models).
* The meaning of abscissa variable codes 1 and 10 (see ANGMOV.md).
