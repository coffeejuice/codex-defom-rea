# FRAE2H (id 93, group 4 material)

One double per material, default 0.9: the fraction of deformation energy converted to heat
(inferred from the name "fraction E2H" and the classic 0.9 Taylor-Quinney value).

## Handlers
read: case 1 of `FUN_140384a50` -> `FUN_140414990(fp, material+0x1ec8)`;
write: `FUN_1403cf3f0` (registry handler2);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: FRAE2H <mat:%i> <f:%Flf>     rest of the line: fgets(0x400), ',' -> ' ', sscanf "%Flf"
                                      (f zeroed first; non-numeric text -> f = 0, no error)
rest of line blank or EOF -> 1 -> parser returns 2 (f is already 0 at that point)
```

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| f | material | +0x1ec8 | double |

## Defaults
`FUN_14028cdc0` sets +0x1ec8 = `DAT_14051f220` = 0.9. The helper zeroes the target before
`fgets`, so a failed read (EOF / blank line) leaves 0.0, not the old value.

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword FRAE2H Reading Error. \n`), value 0.0.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* No range check. Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
"%-6s %7i %20.10E\n"   "FRAE2H", mat, *(double*)(material+0x1ec8)
```
Always written (second keyword of each material). Value round-trips to 11 significant digits.

## Helpers
- `0x140414990` - one-double line reader
- `0x1403cf3f0` - write handler
- `0x14028cdc0` - material reset (0.9)

## Corpus check
740 instances in 440 files, all `FRAE2H m <double>` on one line; all consumed exactly (script
check: next line is a keyword).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every FRAE2H block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
none
