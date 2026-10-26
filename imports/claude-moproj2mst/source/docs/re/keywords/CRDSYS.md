# CRDSYS (id 282, group 2, case 0x8a)

One integer per object, stored at `obj+0x2c38`. The only code that writes it out is the
standalone "2D-to-3D settings" exporter `FUN_1402a9c80`, next to GEO23/GEOSEC/MSHSEC. That points to
a coordinate-system selector for the 2D→3D conversion block, but the code shows no enum values.

## KEY grammar (read)
    CRDSYS <objNo:%i> <v:%i>
`v` is read from the rest of the keyword line with `FUN_140413de0(f, &v, 0)` (`%i`, so hex and
octal are accepted, commas count as spaces, text after the value is ignored; see
`group2-helpers.md`). Blank lines are not skipped: if the line has no value, the helper reads
`"\n"` and fails. Same grammar in 2D and 3D.

## Model storage
`*(int*)(obj+0x2c38) = v`. No other side effects.

## Read errors and validation
* helper returns 1 (EOF, or the rest of the line is empty/blank) → **2**, and nothing is stored.
* a non-numeric token gives v = 0, which is stored. Return 0.
No range check. Return codes are as in `_common.md` §1 (2 = print "Reading Error" and continue).

## KEY writer
**`FUN_14029e250` never writes this keyword in DEF_PRE_64.** The only code that prints id 282 is
`FUN_1402a9c80` (first line): `"%-6s %7i %7i\n"` with name, objNo, `obj+0x2c38`.
`FUN_1402a9c80`'s only caller is `FUN_1401c6920(model, path, objNo)`. That function opens `path`,
writes the KEY header with `FUN_1401c5a20`, finds the object with `FUN_14017aaf0` and calls
`FUN_1402a9c80`. `FUN_1401c6920` itself is never called: it has no call xref, no absolute pointer
to it (no 8-byte `0x1401c6920` in memory), and its RVA appears only in `.pdata`. Its sibling
exporters `FUN_1401c5de0`, `FUN_1401c64f0` … `FUN_1401c6d00` are uncalled in the same way. So in
this binary the write path is dead code, and batch KEY output never contains CRDSYS.
Search method: an operand sweep of the whole program for immediates 0x11a–0x11d and for the object
offsets 0x2be8–0x2c38 (`out/g2/J_imm.txt`). The only `0x11a` immediate that feeds
`FUN_1402f7720` is in `FUN_1402a9c80`. `FUN_1402f5090` handles only id 299.

## Unit conversion
None.

## Helpers
* `FUN_140413de0`: `%i` rest-of-line reader.
* `FUN_1402a9c80`: dead-code writer of CRDSYS/GEO23/GEOSEC/MSHSEC (see GEO23.md).
* `FUN_1401c6920`: unreferenced exporter that calls it.

## Corpus check
No corpus file uses CRDSYS (corpus_kw.tsv: 0 files).

## Open questions
* What the values mean. The users of `obj+0x2c38` in other DEFORM binaries (the GUI) would settle it.
