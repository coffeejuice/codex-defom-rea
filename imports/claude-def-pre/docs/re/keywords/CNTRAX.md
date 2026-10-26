# CNTRAX (id 157, group 2, case 0x0d)

The centre (axis) of the object's rotation for ANGMOV.
* 2D: centre X, Y and the current angle. The 2D text menu asks
  `"  Enter Center of rotation (X,Y) and current angle"` / `"  CNTRAX =  %lf  %lf  %lf ? "`.
* 3D: centre point (3), direction vector (3) and current angle.

## KEY grammar (read)

    2D:      CNTRAX <objNo:%i> <cx:%Flf> <cy:%Flf> <angle:%Flf>
    not 2D:  CNTRAX <objNo:%i> <cx> <cy> <cz> <dx> <dy> <dz> <angle>     (7 × %Flf)

* One line (rest of the keyword line), commas allowed. Missing values become 0.
* 2D: `FUN_140414c20`. Not 2D (3D or unset dimension): `FUN_1404151c0`.

## Model storage

| Field | Type | 2D | not 2D |
|---|---|---|---|
| `obj+0x2790` | double | cx | cx |
| `obj+0x2798` | double | cy | cy |
| `obj+0x27a0` | double | unchanged | cz |
| `obj+0x27a8` | double | unchanged | dx |
| `obj+0x27b0` | double | unchanged | dy |
| `obj+0x27b8` | double | unchanged | dz |
| `obj+0x28b0` | double | angle | angle |

The angle is the 3rd value in 2D and the 7th value in 3D, and both go to the same field
`+0x28b0`. `+0x27a8..+0x27b8` is the direction of the axis (the CNTRA2 menu, at the analogous
fields, prints `"Direc. Vector"`). No side effects.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* Helper EOF or blank line → **2**.
* No checks of the vector.

## KEY writer

`FUN_1402b1000` (handler2), called from `FUN_1402a7c80` after ANGMOV and the 2D-only ANGMVY.
It always returns 0.

* Condition: `obj+0xc != 5` **or** byte `obj+0x2bd8 != 0`.
* Not 2D: `"%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n"` with
  `+0x2790, +0x2798, +0x27a0, +0x27a8, +0x27b0, +0x27b8, +0x28b0`.
* 2D: `"%-6s %7i %20.10E %20.10E %20.10E\n"` with `+0x2790, +0x2798, +0x28b0`.
* N follows the `obj+0x3f24` renumbering rule. Exponents have three digits. The line
  round-trips exactly.

## Unit conversion

None.

## Helpers

* `FUN_140414c20`, `FUN_1404151c0`: readers (3 / 7 doubles).
* `FUN_1402b1000`: writer. `0x1402f7610`: handler3 (no-op).
* `FUN_1402a7c80`: movement-block writer.
* `FUN_1400179f0`: interactive object menu (edits these fields).
* `FUN_14039e670`: copies the object (`FUN_1403a8420(…, 7 values)`).

## Corpus check

All lines match: 907 lines in 2D with 3 values, 430 lines in 3D with 7 values. Example
`2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Equipments\Equipment00006.KEY`:

    CNTRAX       3    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000

In that file the object has no STROKE or FORCE line but does have CNTRAX. This fits a type-5
object with `+0x2bd8` set.

## Open questions

* Where `obj+0x2bd8` is set. No instruction with displacement `0x2bd8` writes it, only the two
  writers read it. It is probably set through a sub-structure pointer or by a block
  copy/constructor. A cross-reference on the object constructor would settle it.
