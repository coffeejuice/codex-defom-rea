# MSHSEC (id 285, group 2, case 0x8d)

An integer count, `obj+0x2c28` (default 21), plus an on/off byte, `obj+0x2c20`. It belongs to the
same 2D→3D conversion settings block as GEO23 and GEOSEC. Inferred meaning: the number of mesh
sections, and whether that number is used. It mirrors GEOSEC.md exactly, with different offsets
and default.

## KEY grammar (read)
    MSHSEC <objNo:%i> [<n> [<flag>]]
The grammar is the same as GEOSEC: tokens from the rest of the keyword line via
`FUN_140413900(f,&cnt,2)`, converted with `strtol(...,10)`. One line only. Same in 2D and 3D.

## Model storage
    n = strtol(tok0)
    obj+0x2c28 (int)  = n >= 1 ? n : 21
    obj+0x2c20 (byte) = n > 0
    if cnt > 1: obj+0x2c20 = (strtol(tok1) == 1)

## Read errors and validation
Tokenizer EOF → **2**. Otherwise 0. An empty line gives 21 and flag 0.

## KEY writer
Not written by `FUN_14029e250`. The only writer is the dead-code exporter `FUN_1402a9c80` (last
line): `"%-6s %7i %7i %7i\n"` with name, objNo, `obj+0x2c28`, `obj+0x2c20 != 0`. See CRDSYS.md
for why it is unreachable.

## Unit conversion
None.

## Helpers
`FUN_140413900`, `FUN_140413a50`, `FUN_14047b6c0`, `FUN_1402a9c80`.

## Corpus check
No corpus file uses MSHSEC.

## Open questions
None beyond GEOSEC's.
