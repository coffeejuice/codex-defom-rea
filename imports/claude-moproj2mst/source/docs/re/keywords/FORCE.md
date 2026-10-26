# FORCE (id 303, group 2, case 0x9f)

The object's force (load) vector: `obj+0x2300` (X), `obj+0x2308` (Y) and `obj+0x2310`
(Z, 3D only).
* `FUN_1401cd020` fills these fields per object from the step result array at `model+0x1b80`,
  using the component indices `DAT_1406e85d0/d4/d8`. In the same loop it fills a second
  3-vector at `+0x22e8..+0x22f8`, using indices `DAT_1406e85dc/e0/e4`.
* `FUN_1401af680` clears the fields.

## KEY grammar (read)

    FORCE <objNo:%i> <fx:%Flf> <fy:%Flf> [<fz:%Flf>]     (rest of the line, commas allowed)

* 2D (`FUN_1400015e0`): `FUN_140414a40` (`"%Flf %Flf"`).
* Not 2D (3D or dimension unset): `FUN_140414c20` (`"%Flf %Flf %Flf"`).
* One line. Missing values become 0.

## Model storage

| Field | Type | 2D | not 2D |
|---|---|---|---|
| `obj+0x2300` | double | fx | fx |
| `obj+0x2308` | double | fy | fy |
| `obj+0x2310` | double | unchanged | fz |

No side effects.

## Read errors and validation

The rules are the same as for STROKE:
* object number EOF or objNo < 1 → 2;
* helper EOF or blank line → **2**;
* a non-numeric first token → all values 0, returns 0;
* no range checks.

## KEY writer

`FUN_1402b04b0` (handler2), called from `FUN_1402a7c80` right after STROKE. It always returns 0.

* Condition: `obj+0xc != 5`.
* Not 2D: `"%-6s %7i %20.10E %20.10E %20.10E\n"`, name, N, `+0x2300`, `+0x2308`, `+0x2310`.
* 2D: `"%-6s %7i %20.10E %20.10E\n"`, name, N, `+0x2300`, `+0x2308`.
* N follows the `obj+0x3f24` renumbering rule. Exponents have three digits (MSVC).
* The line is always written for non-type-5 objects, zeros included. It round-trips exactly.

## Unit conversion

None.

## Helpers

* `FUN_140414a40`, `FUN_140414c20`: readers.
* `FUN_1402b04b0`: writer. `0x1402f7610`: handler3 (no-op).
* `FUN_1402a7c80`: movement-block writer.
* `FUN_1401cd020`: loads the force from step results.
* `FUN_1401af680`: clears the force.

## Corpus check

Every line matches: 2 values in 2D (149 lines) and 3 values in 3D (128 lines). Example
`2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Equipments\Equipment00001.KEY`:

    FORCE        1    0.0000000000E+000    0.0000000000E+000

Many older files have STROKE but no FORCE line, e.g. `3D\EN\Rolling\3D_Ring_Rolling_Example_1.KEY`.
These files predate the keyword.

## Open questions

* What the second vector at `+0x22e8..+0x22f8` is (probably velocity). It is not a FORCE field.
