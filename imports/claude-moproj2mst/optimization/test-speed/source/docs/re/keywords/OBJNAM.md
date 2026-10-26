# OBJNAM (id 144, group 2, case 0x00)

Object name: a text line stored in the object at `obj+0x828` (81-byte buffer; cleared by the
object init `FUN_14015f0b0` and by `FUN_140161260` when the object becomes empty).

## KEY grammar (read)

    OBJNAM <objNo:%i> [rest of line ignored]
    <name line>

`FUN_140413ab0(f, obj+0x828, 0x51)` (group2-helpers.md, "String reader"): the rest of the keyword
line is discarded, the next line is the name, cut at `'\n'`, at most 80 characters copied,
trailing NUL/space/`\n`/`\r` removed (leading blanks and tabs kept). An empty line gives "".
Same in 2D and 3D.

## Model storage

`obj+0x828` char[0x51], NUL-terminated (the trim step always writes the terminator at
`last+1 ≤ 80`).

## Read errors and validation

EOF on either `fgets` → helper returns 1 → **2** ("Reading Error"). No other checks; any
characters are accepted.

## KEY writer

`FUN_1402a2530(model, f, obj, objNo)` called from `FUN_14029e250` as the first keyword after
OBJTYP, only for objects whose internal type is not 7 (empty) or 5 (see OBJTYP.md):

    fprintf("%-6s %7i\n%s\n", "OBJNAM", objNo, obj+0x828)

Returns 0 (does nothing when `obj` is NULL). Round-trips except: names longer than 80 characters
are truncated on read, trailing blanks are lost, and a name containing `'\n'` cannot exist.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0 — reader; `FUN_140413ab0` — string reader.
* `FUN_1402a2530` — writer.

## Corpus check

468 files / 1110 records, all `OBJNAM n` followed by one name line; no name longer than 80
characters. Example (CREEP_Example1_EN.KEY:132):

    OBJNAM       1
    ceipkroW       e

(the stored name in that file really is that string; trailing blanks would be removed).

## Open questions

None.
