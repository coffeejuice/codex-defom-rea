# RMDPTH (id 180, group 2, case 0x24)

One per-object remesh-criterion value (the `RM*` family). It is a double in the object's
mesh/remesh sub-structure, which starts at `obj+0x2d10`. The code only stores and prints the value,
so it does not show what the value means.

## KEY grammar (read)
    RMDPTH <objNo:%i> <value:%Flf>
The value is the rest of the keyword line, read by `FUN_140414990` (see group2-helpers.md,
"Rest-of-line scalar readers"). Commas count as blanks, extra tokens are ignored, and a
non-numeric token gives 0. The record is this one line. 2D and 3D are identical.

## Model storage
* `obj+0x2dc8` (double) = value.
* Default: 3D or dimension unset: -0.7 (double constant `DAT_140522fb8`); 2D: 0.0. It is set by the mesh sub-structure reset `FUN_140189790(obj+0x2d10, 1)`, which the
  object reset `FUN_14015f0b0` calls when the object is created.
* No side effects.

## Read errors and validation
* EOF on the object number, or `objNo < 1` → 2 (common prologue).
* `FUN_140414990` returns 1 → 2 "Reading Error". This happens on EOF, or when the rest of the line
  is empty or blank (for example `RMDPTH       1` followed by a newline). The field is already 0 at that
  point: the helper zeroes its output first, and the output is the model field itself.
* No range check.

## KEY writer
Registry write handler `FUN_1402b49d0`. It takes the object number from `ref[0]`, looks the object up with `FUN_14017aaf0(model+0x1380, n)` (the pointer is not checked for NULL) and prints `obj+0x3f24` instead of the number when that is > 0. Then it prints:

    fprintf(f, "%-6s %7i %20.10E\n", "RMDPTH", N, *(double*)(obj+0x2dc8));

It always returns 0. `FUN_1402a9180` calls it with a ref built from `obj+8`.

Position: the mesh block writer `FUN_1402a9180`. Its order is `RMDPTH`, `RMSTRK`, `RMTIME`
(handlers), `RMSTEP` (inline), `MGSIZR`, `MGWTMP`, `MGWSTN`, `MGWSTR` (handlers), `MGNELM`,
`MGTELM` (inline), `MGWCUV` (handler), `MGGRID` (inline), `MGERR` (handler), `MGWUSR`, and then the
`MGUSER` entries (inline). The first nonzero handler status ends `FUN_1402a9180` and becomes its
return value (the RM handlers always return 0). `FUN_1402a9180` is called:
* from `FUN_14029e250` for object type `obj+0xc == 5`, only when the byte `obj+0x2e05 == 0`
  (group2-writer-map.md step 5b);
* from `FUN_14029e250` for the other types (not 7), only when `!(model+0x1744 & 0x10000)`
  (step 39); its status is ignored there;
* from `FUN_1401c6820`, which writes a stand-alone mesh-settings KEY: the file header
  (`FUN_1401c5a20`), then `FUN_1402a9180` for one object. The corpus `MeshSettings*.KEY` files
  come from this writer.

The value is always written: there is no default suppression. It round-trips exactly up to
`%20.10E` precision.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x24: reader dispatch.
* `FUN_140414990`: rest-of-line `%Flf` reader.
* `FUN_1402b49d0`: writer (registry handler2).
* `FUN_1402a9180`: mesh/remesh block writer.
* `FUN_140189790`: mesh sub-structure reset (defaults).
* `FUN_1401c6820`: stand-alone mesh-settings file writer.

## Corpus check
`2D/SI/MultipleOperations/New_MO_Release_Examples_SI/MO2_Operation_Cycles2/Operations/Task00001/MeshSettings00001.KEY`: the line `RMDPTH       1    0.0000000000E+000` parses exactly.
Across 3D corpus files the value is mostly `-7.0000000000E-001` (the 3D default); a few files have -0.6, -0.8 or 0.0.

## Open questions
* The physical meaning and units of the value are not visible in the pre-processor's reader or
  writer. Reading the solver code would settle them.
