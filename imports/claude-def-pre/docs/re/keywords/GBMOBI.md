# GBMOBI (id 132, group 4 material)

Grain-boundary mobility: two doubles per material (inferred from the name; e.g. a
pre-exponential factor and an activation energy - not established).

## Handlers
read: case 0x28 of `FUN_140384a50` -> `FUN_140414a40(fp, material+0x1868, material+0x1870)`;
write: `FUN_1403d44a0`, called directly by `FUN_1403cdca0` (the registry handler2 is the stub
`0x1402f75d0`);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: GBMOBI <mat:%i> <p:%Flf> <q:%Flf>   rest of line: fgets(0x400), ',' -> ' ',
                                             sscanf "%Flf %Flf" (both zeroed first)
rest of line blank or EOF -> 1 -> parser returns 2 (p = q = 0)
```
Note: no type field (unlike GBENGY/NUCSIZ).

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| p | material | +0x1868 | double |
| q | material | +0x1870 | double |

## Defaults
`FUN_14028cdc0` sets +0x1868 = +0x1870 = 0.0.

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword GBMOBI Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1403d44a0: "%-6s %7i %20.10E %20.10E\n"   "GBMOBI", mat, p, q     (always; returns 0)
```

## Helpers
- `0x140414a40` - two-double line reader
- `0x1403d44a0` - writer

## Corpus check
151 instances in 87 files, all `GBMOBI m 0.0 0.0`; consumed exactly.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every GBMOBI block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of the two values.
