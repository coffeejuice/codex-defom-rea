# CSPMTX (id 270, group 2, case 0x7e)

A 4×4 matrix of doubles belonging to the object's first curve/surface geometry item
(`geom = FUN_140197f40(obj, 0)`, the first entry of the container at `obj+0x3998`), stored at
`geom+0x38`. It is written right after that item's curve DIEGEO and TPLOGY records. The last row
holds a translation and [3][3] is 1.0 in the corpus, so it looks like a coordinate-system or
placement transform.

## KEY grammar (read)
    CSPMTX <objNo:%i> <ignored:%i>
    <i:%i> <m[0][0]:%Flf> <m[0][1]> <m[0][2]> <m[0][3]>
    ... 4 lines in total (rows 0..3)
Line 0: `FUN_140413de0(f, &tmp, 0)`. The integer is read and discarded (the writer always prints 1).
Rows: `FUN_140415990` = `fgets` + comma→space + `sscanf "%i %Flf %Flf %Flf %Flf"`, with all outputs
zeroed first and return 1 on EOF or `sscanf == -1`. The leading row index is ignored; rows are
taken in file order. Partial rows leave 0. One line per row, and blank lines are not skipped. No
2D/3D check.

## Model storage
A local 4×4 matrix is initialised to the identity (`FUN_140220540` → `FUN_140220570`), then fully
overwritten row by row (`FUN_14026a4a0(mat,&i,&j)` = `mat + i*0x20 + j*8`, row-major). It is then
copied with `FUN_14039aa60(geom, mat)` → `FUN_140220670(geom+0x38, mat)` (16 doubles at
`geom+0x38 + (i*4+j)*8`).

## Read errors and validation
* `geom == NULL` (the object has no curve-geometry item yet) → **2**, and **nothing is consumed**.
  The KEY loop then reads the rest of the line and the 4 rows as keywords: for each numeric token
  it prints "WARNING: Keyword %s not found. Skip …" and skips the rest of that line.
* line 0 helper fails (EOF, or blank rest of line) → **2**.
* a row helper fails (EOF or blank line) → **2**, and the matrix is not stored.
* otherwise 0. Return-code handling is as in `_common.md` §1.

## KEY writer
`FUN_1402aaf40(model, f, obj, objNo)`, called from `FUN_1402ab160`. The call chain is
`FUN_14029e250` → `FUN_1402a26c0` → `FUN_1402a27a0` → `FUN_1402ab160`.
`FUN_1402a27a0` calls `FUN_1402ab160` only when the model is **not 2D** (`!FUN_1400015e0`),
`FUN_140197f40(obj,0) != NULL`, and `FUN_140159160(geom)` (the count at `geom+0x18` is > 0).
`FUN_1402ab160` writes the curve DIEGEO (`FUN_1402aaa70`), then TPLOGY (`FUN_1402a9f90`), then
CSPMTX. It stops at, and returns 3 on, the first nonzero result, but `FUN_1402a27a0` ignores that
return. This block comes before the mesh DIEGEO/POLYTP/DIEPNT/DIEEDG of the same object. For when
`FUN_14029e250` calls `FUN_1402a26c0`, see DIEGEO.md.
`FUN_1402aaf40`:
* `FUN_1401983e0(obj)` (item count of `obj+0x3998`) == 0, or `geom == NULL` → return 3, nothing
  written.
* `"%-6s %7i %7i\n"` name, objNo, 1
* for r = 0..3: `" %7i %20.10E %20.10E %20.10E %20.10E\n"` r+1, m[r][0], m[r][1], m[r][2], m[r][3]
  (the matrix is copied out of `geom+0x38` by `FUN_14026a3e0`).
* return 0.
Round-trips exactly.

## Unit conversion
None.

## Helpers
* `FUN_14039a5f0(model, f, obj)`: reader.
* `FUN_140413de0`: `%i` line reader.
* `FUN_140415990`: `"%i %Flf %Flf %Flf %Flf"` line reader (not yet in the helpers table).
* `FUN_140197f40(obj, i)`: item i of the container at `obj+0x3998`, or NULL. `FUN_1401983e0(obj)`:
  its count.
* `FUN_140220570`: identity 4×4. `FUN_140220670(dst, src)`: 4×4 copy. `FUN_14026a4a0`: element
  address. `FUN_14039aa60` / `FUN_14026a3e0`: set/get `geom+0x38`.
* `FUN_1402aaf40`: writer. `FUN_1402ab160`: curve DIEGEO + TPLOGY + CSPMTX sequence.

## Corpus check
`key\3D\EN\Cogging\Cogging_Multiple_Ops_Lab2\Objects\Object00005.KEY` (21 files use CSPMTX) parses
exactly (lines 76–80):
    CSPMTX       5       1
           1    6.1232339957E-017   -1.0000000000E+000    1.2246467991E-016    0.0000000000E+000
           4   -2.0000000000E+001    3.9553394516E+000   -3.9565037501E+000    1.0000000000E+000

The file order matches the writer: curve DIEGEO (point list), `TPLOGY 5 1 1 1 1` (lines 62–75),
CSPMTX, then the mesh `DIEGEO 5 1 58 112` (line 81).

## Open questions
* Which keyword creates item 0 of `obj+0x3998` (the curve DIEGEO or TPLOGY) is settled in
  worker B's DIEGEO.md/TPLOGY.md. If neither precedes CSPMTX, the `geom == NULL` path applies.
