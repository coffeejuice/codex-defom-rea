# GEOSEC (id 284, group 2, case 0x8c)

An integer count, `obj+0x2c24` (default 20), plus an on/off byte, `obj+0x2c21`. It belongs to the
same 2D→3D conversion settings block as GEO23 and MSHSEC (written together by `FUN_1402a9c80`).
Inferred meaning: the number of geometry sections, and whether that number is used.

## KEY grammar (read)
    GEOSEC <objNo:%i> [<n> [<flag>]]
The rest of the keyword line is split by `FUN_140413900(f, &cnt, 2)` (delimiters `" ,\n\t\r"`, at
most 2 tokens). Each token is converted with `strtol(...,10)` (`FUN_14047b6c0`): decimal only, and
a missing token counts as 0. Only that one line is consumed. Same grammar in 2D and 3D.

## Model storage
    n = strtol(tok0)
    obj+0x2c24 (int)  = n >= 1 ? n : 20
    obj+0x2c21 (byte) = n > 0
    if cnt > 1: obj+0x2c21 = (strtol(tok1) == 1)
So a second token overrides the flag: any value other than 1 clears it, even when n > 0.

## Read errors and validation
* tokenizer EOF → **2**.
* otherwise always 0. An empty line gives n = 0, which stores 20 and flag 0.

## KEY writer
Not written by `FUN_14029e250`. The only writer is the dead-code exporter `FUN_1402a9c80` (see
CRDSYS.md for why it is unreachable): `"%-6s %7i %7i %7i\n"` with name, objNo,
`obj+0x2c24`, `obj+0x2c21 != 0`. Its output round-trips through the reader, apart from a stored
count < 1, which reads back as 20.

## Unit conversion
None.

## Helpers
`FUN_140413900` / `FUN_140413a50` (tokenizer), `FUN_14047b6c0` (strtol), `FUN_1402a9c80` (writer).

## Corpus check
No corpus file uses GEOSEC.

## Open questions
* The exact meaning of the 20 default and of the flag. GUI code that uses `obj+0x2c24` would settle it.
