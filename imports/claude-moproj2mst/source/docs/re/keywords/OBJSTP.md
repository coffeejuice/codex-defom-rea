# OBJSTP (id 268, group 2, case 0x7c)

Per-object "object stop" definition, in the movement sub-structure `obj+0x2160`. It consists of a
point and a vector. The writer emits it only when the vector is not (near) zero, so the vector acts
as the on/off switch, presumably a stop-plane normal or direction.

## KEY grammar (read)
3D (not `FUN_1400015e0`):

    OBJSTP <objNo:%i> <p1> <p2> <p3> <v1> <v2> <v3>        (6 × %Flf, FUN_140415080)

2D:

    OBJSTP <objNo:%i> <p1> <p2> <v1> <v2>                  (4 × %Flf, FUN_140414e40)

The values are on the rest of the keyword line: one line, commas → blanks, missing values → 0.

## Model storage
* 3D: `obj+0x29e8`, `+0x29f0`, `+0x29f8` = p1..p3; `obj+0x2a00`, `+0x2a08`, `+0x2a10` = v1..v3.
* 2D: `obj+0x29e8`, `+0x29f0` = p1, p2; `obj+0x2a00`, `+0x2a08` = v1, v2. `+0x29f8` and `+0x2a10`
  are untouched.
* Defaults (`FUN_14018b500`): all 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 (EOF or blank rest of line) → 2. The fields have already been zeroed.

## KEY writer
In the movement-block writer `FUN_1402a7c80`, as its last line, after `OBJUPD`.
`FUN_1402a7c80` runs for type-5 objects, and for other types (not 7) when
`!(model+0x1744 & 0x40000)`. The line is written only if `FUN_140198c80(obj)` is true:
* `obj+0xc == 0` (object type 0) → false;
* the model dimension is taken from `obj+0x3a98` (the object's model pointer):
  * 2D: `|(v1, v2)| > 1e-12` (`FUN_140309230`; `DAT_14051f260` ≈ 1e-12);
  * 3D: `|(v1, v2, v3)| > 1e-12` (`FUN_14023b610`);
  * neither (dimension unset): true.

Formats:
* 3D: `fprintf("%-6s %7i %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", "OBJSTP", N, +0x29e8, +0x29f0, +0x29f8, +0x2a00, +0x2a08, +0x2a10)`
* 2D: `fprintf("%-6s %7i %20.10E %20.10E %20.10E %20.10E\n", "OBJSTP", N, +0x29e8, +0x29f0, +0x2a00, +0x2a08)`

A record with a zero vector, or on a type-0 object, is dropped on output.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x7c: reader.
* `FUN_140415080` / `FUN_140414e40`: 6 / 4 double line readers.
* `FUN_1402a7c80`: writer.
* `FUN_140198c80`: write condition.
* `FUN_140309230` / `FUN_14023b610`: 2D / 3D vector length (`sqrt`, 0 when the sum of squares is 0).
* `FUN_14018b500`: reset.

## Corpus check
`2D/EN/MultipassRolling/ROLL_EN.KEY` line 247 parses exactly (2D, 4 values):

    OBJSTP       1    1.8964000000E+000    0.0000000000E+000    1.0000000000E+000    0.0000000000E+000

Only 8 KEY files use OBJSTP (14 lines when `.MST` files are included), all 2D, all with 6 fields.
No 3D example exists.

## Open questions
* The 3D layout is derived from the code only; no corpus file confirms it.
