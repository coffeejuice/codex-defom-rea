# HDNRUL (id 120, group 4 material)

Hardness rule / hardness model selector (inferred from the name; corpus values 0 and 1, 1 in the heat-treatment examples).

## Handlers
read: case 0x1c of `FUN_140384a50` -> `FUN_140413de0(fp, material+0x668, 0)`;
write: inline in `FUN_1403cdca0` (registry handler2 is the stub `0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: HDNRUL <mat:%i> <v:%i>    rest of the line: fgets(0x400), ',' -> ' ', sscanf "%i" (v zeroed
                                 first; "%i" is base-detecting: 0x.. hex, leading 0 octal; a
                                 non-numeric or real text such as "1.5" gives 1 / 0 silently)
rest of line blank or EOF -> 1 -> parser returns 2 (v = 0)
```

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| v | material | +0x668 | int |

## Defaults
`FUN_14028cdc0` sets +0x668 = 0.

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword HDNRUL Reading Error. \n`), value 0.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* No range check. Never returns 3.

## Unit conversion
none

## KEY grammar (write)
```
inline in FUN_1403cdca0, after UTSDAT:
  "%-6s %7i %7i\n"   "HDNRUL", mat, *(int*)(material+0x668)
always written
```
Round-trips exactly.

## Helpers
- `0x140413de0` - one-int line reader
- `0x1403cdca0` - per-material writer (inline)

## Corpus check
740 instances in 440 files: `HDNRUL m 0` (722), `HDNRUL m 1` (18, e.g. `key/2D/EN/HeatTreat/HT_CRACK_EN.KEY` line 168); consumed exactly (one line).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every HDNRUL block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of values 0/1.
