# TXTURE (id 128, group 4 material)

Three integers per material describing a texture setting (inferred from the name; default
0, 1, 1). Written right after SIZEMD.

## Handlers
read: case 0x24 of `FUN_140384a50` -> `FUN_140413fb0(fp, material+0x2154, material+0x2158,
material+0x215c, NULL)`;
write: inline in `FUN_1403cdca0` (registry handler2 is the stub `0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: TXTURE <mat:%i> <a:%i> <b:%i> <c:%i>   rest of line: fgets(0x400), ',' -> ' ',
                                                sscanf "%i %i %i" (a, b, c zeroed first;
                                                missing ones stay 0)
rest of line blank or EOF -> helper 1 -> parser returns 2 (a = b = c = 0)
```

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| a | material (sub-object at +0x2154, `FUN_14028c500`) | +0x2154 | int |
| b | same | +0x2158 | int |
| c | same | +0x215c | int |

## Defaults
Constructor `FUN_14028c6f0` calls `FUN_14028c500(material+0x2154)`: a = 0, b = 1, c = 1.
`FUN_14028cdc0` does not reset them. The helper zeroes all three before reading, so a short line
`TXTURE m 0` gives 0, 0, 0 (not 0, 1, 1).

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword TXTURE Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* No range check. Never returns 3.

## Unit conversion
none

## KEY grammar (write)
```
inline in FUN_1403cdca0, after SIZEMD:
  "%-6s %7i %7i %7i %7i\n"   "TXTURE", mat, a(+0x2154), b(+0x2158), c(+0x215c)
always written
```
Round-trips exactly.

## Helpers
- `0x140413fb0` - three-int line reader
- `0x14028c500` - sub-object constructor (0, 1, 1)
- `0x1403cdca0` - per-material writer (inline)

## Corpus check
151 instances in 87 files: `TXTURE m 0 1 1` (145) and `TXTURE m 0 0 0` (6); all consumed exactly.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every TXTURE block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of the three integers.
