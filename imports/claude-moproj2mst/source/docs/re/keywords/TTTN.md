# TTTN (id 136, group 5 phase transformation)

Name (free text, one line) of the transformation `from` → `to`, stored in the phase pair. The
writer never writes the stored name; it always writes the generated text
`Transformation from <from> -> <to>`.

## Handlers
read: case 2 of `FUN_140383120`; write: inline in `FUN_1403cbf00` (handler2 is the stub
`0x1402f75d0`); accessor: none (stub `0x1402f7610`).

## KEY grammar (read)
```
line 1: TTTN <from:%i> <to:%i>      (fscanf "%i %i"); the rest of this line is discarded
line 2: <name>                      whole line, FUN_140413ab0(fp, pair+0x18, 0x51)
```
`FUN_140413ab0`: `fgets` (discard rest of line 1), `fgets` line 2 (max 1023 chars), cut at the
first `'\n'`, `strncpy(dst, line, 0x50)`, then scan back from `dst[0x4f]` over `'\0'`, `' '`,
`'\n'`, `'\r'` and write `'\0'` after the last other character (so the result is always
terminated, at most 80 chars, trailing blanks/CR removed; an all-blank line gives `""`). Leading
blanks and tabs are kept. EOF on either `fgets` → parser returns 2 (name unchanged).

## Model mapping
| KEY field | object | offset | C type |
|---|---|---|---|
| name | pair | +0x18 | char[0x51] |

No side effects.

## Defaults
Constructor `FUN_140333f60`: `sprintf(pair+0x18, " ")` (a single space).

## Validation, errors, return codes
2 on EOF; nothing validated.

## Unit conversion
none found

## KEY grammar (write)
In `FUN_1403cbf00`, for pairs with `+0xC != 0`, first thing after the banner
```
"*\n*  %s\n*\n"   with %s = sprintf("Inter-Material Data for     %i and %i", from, to)
"%-6s %7i %7i\n"  "TTTN", from, to
"%s\n"            sprintf("Transformation from %d -> %d", from, to)
```
Asymmetry: the name read at `+0x18` is never written; a custom name is replaced by the generated
text on every round trip. (Note the banner pads only the first number.)

## Helpers
- `0x140413ab0` - "skip rest of line, read next line as string" helper
- `0x1403cbf00` - per-pair KEY writer (prints the banner, TTTN and PHASTF inline)

## Corpus check
168 TTTN instances (24 files, `key/**` and `materials/`), all followed by a line
`Transformation from a -> b` matching the pair (simulated with `g5sim.py`: next token is always
`PHASTF`). Oracle golden `JIS-S45C(HeatTreatment)_s000006.OUT.KEY` shows
`TTTN         1       2` / `Transformation from 1 -> 2`, i.e. the writer format above.

## Open questions
none
