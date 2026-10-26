# PRSTRE (id 256, group 2, case 0x70)

Per-object triple of doubles in the movement/equipment sub-structure `obj+0x2160`. The name
suggests a press stroke/rate-energy setting. It is written only when the first value is positive.
The registry flag is 5.

## KEY grammar (read)
    PRSTRE <objNo:%i> <a:%Flf> <b:%Flf> <c:%Flf>
The rest of the keyword line is read by `FUN_140414c20` (three `%Flf`, commas → blanks, missing
values → 0). There is one line; 2D and 3D are identical.

## Model storage
* `obj+0x2aa8`, `obj+0x2ab0`, `obj+0x2ab8` (doubles) = a, b, c.
* Defaults (`FUN_14018b500`, the reset of `obj+0x2160`): 0.0, 0.0, 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 (EOF or blank rest of line) → 2. The values have already been zeroed.
* No range check.

## KEY writer
Registry handler2 `FUN_1402afd70`. It is called by the movement-block writer `FUN_1402a7c80` as its
third handler, after `MOVCTL` and `SPDLMT` and before `STROKE`. It runs only if those two returned 0.
`FUN_1402a7c80` itself runs for type-5 objects, and for other types (not 7) when
`!(model+0x1744 & 0x40000)`.

    if (*(double*)(obj+0x2aa8) > 0.0)
        fprintf(f, "%-6s %7i %20.10E %20.10E %20.10E\n", "PRSTRE", N, a, b, c);

It returns 0. When `a <= 0` nothing is written, so a record with `a <= 0` but nonzero `b` or `c`
does not round-trip.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x70: reader.
* `FUN_140414c20`: three-double line reader.
* `FUN_1402afd70`: writer (handler2).
* `FUN_1402a7c80`: movement block.
* `FUN_14018b500`: reset.

## Corpus check
No corpus file uses PRSTRE.

## Open questions
* What the three values mean is not visible here.
