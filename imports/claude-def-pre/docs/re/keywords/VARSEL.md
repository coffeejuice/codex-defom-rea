# VARSEL (id 143, group 5 phase transformation)

One integer per phase pair `from` → `to`; by its name a variant-selection option (inferred).
Only the value 0 occurs in the corpus.

## Handlers
read: case 9 of `FUN_140383120`; write: inline in `FUN_1403cbf00` (handler2 is the stub
`0x1402f75d0`); accessor: none (stub `0x1402f7610`).

## KEY grammar (read)
```
line 1: VARSEL <from:%i> <to:%i>      fscanf "%i %i"
        <v:%i>                        rest of the line, FUN_140413de0(fp,&v,0)
```
v is stored even when the helper fails (blank remainder → v = 0 stored, return 2).

## Model mapping
| KEY field | object | offset | C type |
|---|---|---|---|
| v | pair | +0x768 | int |

## Defaults
Constructor: +0x768 = 0.

## Validation, errors, return codes
2 on EOF/blank remainder; any integer accepted.

## Unit conversion
none found

## KEY grammar (write)
Last line of each active pair's block in `FUN_1403cbf00`, after IPSNRG:
```
"%-6s %7i %7i %7i\n"   "VARSEL", from, to, pair+0x768
```
Always written for an active pair. Exact round trip.

## Helpers
- `0x140413de0` - one-int line helper
- `0x1403cbf00` - per-pair writer

## Corpus check
34 instances in 4 files (`key/2D/{EN,SI}/InductionHardening/2d_Induction_Hardening_Process_*.KEY`,
`key/2D/{EN,SI}/ResistanceSpotWelding/…Welding_cycle_*.KEY`), all `VARSEL a b 0`, one line
each (`g5sim.py`). Oracle golden `variants/2D__SI__InductionHardening__2d_Induction_Hardening_Process_SI.OUT.KEY`
line 829 `VARSEL       1       2       0`; JIS-S45C golden outputs show the default
`VARSEL       1       2       0` for every pair although the input has none.

## Open questions
- Meaning of non-zero values.
