# CNTRA2 (id 261, group 2, case 0x75)

The second rotation axis (3D only), used with ANGMO2: centre point, direction vector and current
angle. The 3D text menu prints `"  Define Center of Axis by: center point and a direction
vector:"`, `"  Center Coord. = …"` (`+0x28c0..+0x28d0`) and `"  Direc. Vector = …"`
(`+0x28d8..+0x28e8`). It is the layout of 3D CNTRAX, moved by `+0x130`.

## KEY grammar (read)

    CNTRA2 <objNo:%i> <cx> <cy> <cz> <dx> <dy> <dz> <angle>     (7 × %Flf, one line, commas allowed)

* Read only when the model is 3D (`FUN_1400015f0`, model `+0x1c == 3`), with `FUN_1404151c0`.
* **Not 3D** (2D or unset): the case reads nothing after the object number and returns 0.
  The rest of the line stays in the stream. The KEY interpreter then reads its next token as a
  keyword name, which is unknown. It prints `WARNING: Keyword %s not found. Skip %s` and skips
  the rest of that line. See `_common.md` §1.

## Model storage

| Field | Type | Value |
|---|---|---|
| `obj+0x28c0`, `+0x28c8`, `+0x28d0` | double | cx, cy, cz |
| `obj+0x28d8`, `+0x28e0`, `+0x28e8` | double | dx, dy, dz |
| `obj+0x29e0` | double | angle (7th value) |

No side effects.

## Read errors and validation

* Object number EOF or objNo < 1 → 2.
* 3D: helper EOF or blank line → **2**.
* Not 3D: always 0.

## KEY writer

`FUN_1402b01e0` (handler2). It is called from `FUN_1402a7c80` **only in 3D**, right after
ANGMO2. It always returns 0.

* Condition: `obj+0xc != 5` **or** byte `obj+0x2bd8 != 0` (the same test as CNTRAX).
* Format: `"%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n"` with
  `+0x28c0, +0x28c8, +0x28d0, +0x28d8, +0x28e0, +0x28e8, +0x29e0`.
* Three-digit exponents. N follows the `obj+0x3f24` renumbering rule. The line round-trips
  exactly.

## Unit conversion

None.

## Helpers

* `FUN_1404151c0`: reader (7 doubles).
* `FUN_1402b01e0`: writer. `0x1402f7610`: handler3 (no-op).
* `FUN_1402a7c80`: movement-block writer.
* `FUN_1400179f0`: menu.
* `FUN_14039e670`: object copy.

## Corpus check

All 430 3D lines have 7 values, and no 2D file contains CNTRA2. Example
`3D\SI\Multiple_Operations\New_MO_Release_Examples_SI\Equipments\Equipment00004.KEY`:

    CNTRA2       1    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000

## Open questions

* Setter of `obj+0x2bd8`: see CNTRAX.md.
