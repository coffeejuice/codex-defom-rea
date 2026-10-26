# TRGVOL (id 259, group 2, case 0x73)

Per-object target volume. It is an integer mode/flag plus a double volume, stored in the
`obj+0x1ea0` sub-structure. A negative mode means "off".

## KEY grammar (read)
    TRGVOL <objNo:%i> <mode:%i> <volume:%Flf>
The rest of the keyword line is read by `FUN_1404154c0` (`"%i %Flf"`, commas → blanks, missing
values → 0). It is one line, the same in 2D and 3D.

## Model storage
* `obj+0x2098` (int) = mode; `obj+0x2090` (double) = volume.
* If the volume read is **exactly 0.0** (`== DAT_14051bed8`, which is 0.0), then `obj+0x2098 = -1`,
  whatever mode was read. This also covers a missing volume token.
* Defaults (`FUN_140186200`): mode -1, volume 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Helper returns 1 (EOF or blank rest of line) → 2. Both fields are already 0 when this happens,
  and the 0.0 test above still runs first, so the mode becomes -1.
* No range check.

## KEY writer
Inline in `FUN_14029e250` (group2-writer-map.md step 15). It is written always, for objects of a
type other than 5 or 7, after `REFPOS` and before `ELPSOL`:

    m = *(int*)(obj+0x2098);
    if (m < 0) { m = 0; if (*(double*)(obj+0x2090) != 0.0) *(double*)(obj+0x2090) = 0.0; }
    fprintf(f, "%-6s %7i %7i %20.10E\n", "TRGVOL", N, m, *(double*)(obj+0x2090));

Note that **the writer modifies the model**: when the mode is negative, a nonzero volume is cleared
to 0.0.

Round trip: `TRGVOL n 0 0.0` → mode -1 → written as `0 0.0`, which is stable. `TRGVOL n k 0.0` with
k ≠ 0 is written as `0 0.0` (lossy). A nonzero volume keeps its mode exactly.

## Unit conversion
None in the reader or writer. The volume is in model units.

## Helpers
* `FUN_14038b8a0` case 0x73: reader.
* `FUN_1404154c0`: `%i %Flf` line reader.
* `FUN_14029e250`: writer.
* `FUN_140186200`: reset (defaults).

## Corpus check
`2D/EN/MultipleOperations/New_MO_Release_Examples_EN/MO2_Extrusion_and_Boolean_Cycles/Objects/Object00007.KEY`:
`TRGVOL       4       0    0.0000000000E+000` parses exactly. Across the corpus there are 1096 lines,
all with 4 fields:
* 1058 × `0 0.0`;
* 20 with mode 0 and a nonzero volume;
* 14 with mode 1 and a nonzero volume;
* 4 with mode 2 and a nonzero volume.

None has mode ≠ 0 with a volume of 0, which is consistent with the writer.

## Open questions
* What modes 1 and 2 mean is not visible here.
