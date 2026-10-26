# ROTSYM (id 258, group 2, case 0x72)

3D-only rotational-symmetry definition of an object. It holds one scalar (presumably the sector
angle) and two 3-vectors, stored in the `obj+0x1ea0` sub-structure. The string
`ROTSYM[1,2,3,4,5,6,7] = %f, …` elsewhere in the binary prints 7 values, which matches 1 + 3 + 3.

## KEY grammar (read)
3D (`FUN_1400015f0`, i.e. `model+0x1c == 3`):

    ROTSYM <objNo:%i> <s:%Flf>
    <a1:%Flf> <a2:%Flf> <a3:%Flf>
    <b1:%Flf> <b2:%Flf> <b3:%Flf>

There are three `fgets` lines: line 0 is the rest of the keyword line (`FUN_140414990`), and
lines 1 and 2 are read with `FUN_140414c20`. Commas count as blanks, and missing values → 0.

Not 3D (2D, or dimension unset): the case does **nothing** and returns 0. The rest of the keyword
line is **not consumed**, so the interpreter then reads its tokens (and the next two lines) as
keywords. A value such as `0.0000000000E+000` becomes an unknown keyword ("WARNING: Keyword … not
found", rest of that line skipped). The writer never emits ROTSYM in 2D.

## Model storage (3D)
* `obj+0x2058` (double) = s.
* Line 1 → `obj+0x2078`, `+0x2080`, `+0x2088` (doubles).
* Line 2 → `obj+0x2060`, `+0x2068`, `+0x2070` (doubles).
* Defaults (`FUN_140186200`, only when the model is 3D at reset time): all 0.0.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2.
* Any of the three helpers returns 1 (EOF, or a **blank** line) → 2. The parser returns
  immediately, so the fields read so far are kept and the field of the failed helper is zeroed.
* No range check.

## KEY writer
Inline in `FUN_14029e250` (group2-writer-map.md step 13). It runs only in 3D, for objects of a
type other than 5 or 7, after `TMPLMT` and before `REFPOS`. It is always written, even when all
values are 0:

    fprintf(f, "%-6s %7i %20.10E\n", "ROTSYM", N, s);           // obj+0x2058
    fprintf(f, " %20.10E %20.10E %20.10E\n", +0x2078, +0x2080, +0x2088);
    fprintf(f, " %20.10E %20.10E %20.10E\n", +0x2060, +0x2068, +0x2070);

The line order matches the reader, so the record round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x72: reader.
* `FUN_140414990`: one `%Flf` line reader.
* `FUN_140414c20`: three `%Flf` line reader.
* `FUN_14029e250`: writer.
* `FUN_140186200`: reset.
* `FUN_14043e0d0`: debug print (not KEY).

## Corpus check
`3D/EN/Cogging/Cogging_Multiple_Ops_Lab1/Objects/Object00001.KEY` (lines 18–20) parses exactly:

    ROTSYM       1    0.0000000000E+000
        0.0000000000E+000    0.0000000000E+000    0.0000000000E+000
        0.0000000000E+000    0.0000000000E+000    0.0000000000E+000

There are 460 ROTSYM records, all in 3D files, each followed by two 3-value lines. No 2D file uses it.

## Open questions
* Which of the two vectors is the axis point and which is the direction is not visible here
  (the KEY order is +0x2078 first, then +0x2060).
