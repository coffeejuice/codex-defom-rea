# JOMINY (id 118, group 4 material)

Jominy hardness data (inferred from the name): a 2-D table in independent variables (13, 12).

## Handlers
read: case 0x1a of `FUN_140384a50` (inline code on the DYNARY at material+0x1000);
write: `FUN_1403d0510`;
accessor: `FUN_1403d05f0` (returns 0; ignores the label; stores `material+0x1000` in `*out` if the
material exists).

## KEY grammar (read)
```
line 1: JOMINY <mat:%i> <t:%i> <n0:%i> <n1:%i>   rest of line: FUN_140413fb0 (fgets, ',' -> ' ',
                                                  sscanf "%i %i %i"; zeroed first)
        blank/EOF -> return 2 (DYNARY untouched)
FUN_14012fc70(dyn, 0x52)          (type 0x52: ndim 2, var (13, 12), ncomp 1, n = 0, freed)
if t == 1:
    n[0] = n0, n[1] = n1
    body FUN_14041a800: n0 axis0 values, n1 axis1 values, n0*n1 values (whitespace-separated
    %Flf), then fgets of the rest of the last line if n0*n1 > 0
other t: table stays empty, nothing more read, no error
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| t | - | (not stored) | | selects "table present" |
| n0, n1 | DYNARY at +0x1000 | +0x101c, +0x1020 | int | type 0x52, ndim 2 (+0x1004), var 13 (+0x1010), 12 (+0x1014) |
| body | same | axes +0x1028/+0x1030, values +0x1048 | double[] | |

## Defaults
`FUN_14028cdc0` re-initialises the DYNARY with `FUN_14012d2a0(dyn, 0x52)`: ndim 2, var (13, 12),
n = 0 (empty; not written).

## Validation, errors, return codes
* Header line blank or EOF -> 2 (`ERROR: Keyword JOMINY Reading Error. \n`).
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1403d0510: only if FUN_14012f610(dyn) (n0 >= 1 and n1 >= 1):
  "%-6s %7i %7i %7i %7i\n"   "JOMINY", mat, 1, n0, n1
  FUN_1402f3130(fp, dyn, 0): axis0 (n0 values), axis1 (n1 values), n0*n1 values, each group
  " %20.10E" per value, "\n" + 8 spaces before every value whose index is a non-zero multiple
  of max(n0, n1), ending "\n"
otherwise nothing (returns 0 either way)
```
Asymmetry: `JOMINY m t ...` with t != 1, or with n0/n1 = 0, is read but not written back.

## Helpers
- `0x140413fb0` three-int reader; `0x14012fc70` re-init; `0x14041a800` body reader
- `0x1403d0510` writer; `0x14012f610`; `0x1402f3130`; `0x1403d05f0` accessor

## Corpus check
285 instances in 163 files, all `JOMINY m 1 1 1` + three single-value lines (axis0, axis1,
value), e.g. `key/2D/EN/ElastoPlastic/Extrusion-EP_EN.KEY` line 126; all consumed exactly. The
oracle output for `materials/JIS-S45C(HeatTreatment)_s000006.KEY` also has `JOMINY 4 1 24 1`.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every JOMINY block of those files with the read grammar above and re-wrote it with the write formats above; all 38 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of independent variables 13 and 12.
