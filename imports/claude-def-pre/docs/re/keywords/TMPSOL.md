# TMPSOL (id 102, group 4 material)

Solidus temperature (inferred from the name). Stored but never written to a KEY.

## Handlers
read: case 10 of `FUN_140384a50` -> `FUN_140414990(fp, material+0x1ed8)`;
write: not written (registry handler2 is the stub `0x1402f75d0`; `FUN_1403cdca0` has no inline
code for it, and no `FUN_1402f7720(0x66)` call exists in the decompiled writers);
accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: TMPSOL <mat:%i> <T:%Flf>   rest of line: fgets(0x400), "," -> " ", sscanf "%Flf"
                                    (target zeroed first)
```

## Model mapping
| KEY field | Object | Byte offset | C type |
|---|---|---|---|
| mat | material | +0x0 | int |
| T | material | +0x1ed8 | double |

## Defaults
`FUN_14028cdc0` sets +0x1ed8 = 0.0. The helper zeroes it before reading (0.0 after a failed read).

## Validation, errors, return codes
* Rest of the keyword line blank (whitespace only) or EOF -> 2
  (`ERROR: Keyword TMPSOL Reading Error. \n`).
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Non-numeric text is accepted silently. Never returns 3.

## Unit conversion
none

## KEY grammar (write)
Not written. A KEY read and written back by DEF_PRE loses this keyword (confirmed: the oracle
outputs of `key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY` and
`materials/JIS-S45C(HeatTreatment)_s000006.KEY`, which contain it, have no TMPSOL line).

## Helpers
- `0x140384a50` - group 4 parser
- `0x140414990` - one-double line reader

## Corpus check
7 instances in 2 files (USER_HT.KEY, `materials/JIS-S45C(HeatTreatment)_s000006.KEY`), all `TMPSOL m <double>`; consumed exactly.

## Open questions
Whether anything else uses +0x1ed8 (not searched beyond the KEY reader/writer).
