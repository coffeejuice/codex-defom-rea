# MTNAME (id 92, group 4 material)

Material name: a free-text line (at most 64 characters kept) on the line after the keyword.

## Handlers
read: case 0 of `FUN_140384a50` -> `FUN_140413ab0(fp, material+0xc, 0x41)`;
write: inline in `FUN_1403cdca0` (registry handler2 is the stub `0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: MTNAME <mat:%i> [anything]       rest of the line is read by fgets(0x400) and discarded
line 2: <name text>                      read by fgets(0x400); cut at the first '\n';
                                         strncpy(dst, line, 0x40) (64 chars max, no ',' mapping);
                                         then from dst[0x3f] down to dst[0], skip chars that are
                                         '\0', ' ', '\n' or '\r'; dst[last+1] = 0
                                         (all blank -> dst[0] = 0, i.e. empty name)
EOF on line 1's fgets or on line 2's fgets -> 1 -> parser returns 2 (name unchanged).
```
* Leading spaces and inner spaces/tabs are kept; trailing tabs are kept (only ' ', '\r', '\n' and
  NULs are trimmed).
* A line longer than 1023 chars is split by fgets; only the first 1023 chars are seen, and the
  remainder is left in the stream as the next "keyword" text.
* A name of 64 chars or more keeps its first 64 chars (dst[0x40] is always 0 after the trim).
* An empty second line gives the empty name; the following line is then read as a keyword.

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material (found by `FUN_14029c2c0`, created if absent) | +0x0 | int |
| name | material | +0xc .. +0x4c | char[0x41] |

No other side effects.

## Defaults
`FUN_14028cdc0` (called by the constructor `FUN_14028c6f0`) copies the empty string
`DAT_140539206` into +0xc (`FUN_140484f90`). The reader does not clear the field before reading;
on EOF the old name stays.

## Validation, errors, return codes
* EOF on either fgets -> 2 (`ERROR: Keyword MTNAME Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Nothing else is validated (no length warning, any characters accepted). Never returns 3.

## Unit conversion
none

## KEY grammar (write)
```
inline in FUN_1403cdca0, first keyword after the material banner:
  "%-6s %7i\n%s\n"   "MTNAME", mat, name
  name = nameOverride (param_4) if non-NULL and non-empty, else material+0xc
```
The KEY writer `FUN_1401c5af0` calls `FUN_1403cdca0` with `R9 = 0` (seen in the listing at
`0x1401c5baa: XOR R9D,R9D`), so in KEY output the name is always material+0xc.
Round trip: exact for names of up to 64 characters without trailing blanks; trailing
blanks/CR are lost; an empty name is written as an empty line and re-read as empty.

## Helpers
- `0x140413ab0` - skip rest of line, read string line
- `0x1403cdca0` - per-material KEY writer (inline write)
- `0x1401c5af0` - caller of `FUN_1403cdca0` (passes no override)
- `0x14028cdc0` - material reset (empty name)

## Corpus check
740 instances in 440 files (`grep -E "^MTNAME( |$)"` over `key/` and `materials/`); in every one
the name is the next line and the line after it is a keyword (checked by script). Longest name in
the corpus: 39 characters. Example: `key/2D/EN/Creep/CREEP_Example1_EN.KEY` line 86-87
(`GR-0001R  tset-T`, with a double inner space kept).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every MTNAME block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Who calls `FUN_1403cdca0` with a non-NULL name override (not the KEY writer); not needed for KEY
I/O.
