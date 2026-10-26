# MGSIZM (id 195, group 2, case 0x33)

A 3D-only mesh-size double at `obj+0x2da8` (name: "MG size ..."). It is read in 3D but never
written by the KEY writer.

## KEY grammar (read)
3D:

    MGSIZM <objNo:%i> <value:%Flf>        (FUN_140414990, rest of line)

2D: only the object number is read. The parser then returns 0 **without consuming the rest of the
line**: whatever follows the object number on the line stays in the stream and is read by the
KEY interpreter as the next keyword token (`fscanf("%39s")`), e.g. `MGSIZM 1 2.5` makes
`2.5` the next "keyword" (unknown-keyword handling of the interpreter applies).

## Model storage
3D: `value → obj+0x2da8` (double). 2D: nothing (but the object is still get-or-created by the
prologue).

## Read errors and validation
* objNo EOF / < 1 → 2 (both dims).
* 3D: helper returns 1 (EOF or empty/blank rest of line) → 2, field = 0.
* 2D: always 0 (the parser's status variable `local_108c` is initialised to 0 before the switch).

## KEY writer
None. No writer function references id 195 (`writer_index.txt` lists none; `FUN_1402a9180`
does not print it; registry handler2 is the no-op `0x1402f75d0`). A value read from a KEY file
is therefore lost on re-write.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x33 — reader; `FUN_1400015f0` — 3D test; `FUN_140414990` — one-double reader.

## Corpus check
No corpus file uses MGSIZM.

## Open questions
Whether `obj+0x2da8` is written to the DB or used anywhere (search xrefs to the offset).
