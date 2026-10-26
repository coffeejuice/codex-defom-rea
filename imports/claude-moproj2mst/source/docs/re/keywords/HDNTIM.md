# HDNTIM (id 117, group 4 material)

A 1-D table in independent variable 1 (inferred: hardness / time or Jominy-distance relation
used by hardness prediction; HT_CARB1 has 18 rows from (0, 0) to larger x with increasing y).

## Handlers
read: case 0x19 of `FUN_140384a50` (inline code on the DYNARY at material+0xf88);
write: `FUN_1403d03b0`;
accessor: `FUN_1403d0480` (returns 0; ignores the label; stores `material+0xf88` in `*out` if the
material exists).

## KEY grammar (read)
```
line 1: HDNTIM <mat:%i> <n:%i>     rest of line: FUN_140413de0 (fgets, ',' -> ' ', sscanf "%i")
        blank/EOF -> return 2 (DYNARY untouched)
FUN_14012fc70(dyn, 0x53)            (type 0x53: ndim 1, var 1, ncomp 1, n0 0, arrays freed)
n0 = n
body FUN_14041a800: n rows "x v" (whitespace-separated %Flf), then fgets of the rest of the
     last line if n > 0. n <= 0: nothing read (negative n clamped to 0 by FUN_14012fad0).
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| n | DYNARY at +0xf88 | n0 +0xfa4 | int | type 0x53, ndim 1 (+0xf8c), var0 1 (+0xf98), ncomp 1 |
| rows | same | axis0 +0xfb0, values +0xfd0 | double[] | |

## Defaults
`FUN_14028cdc0` re-initialises the DYNARY with `FUN_14012d2a0(dyn, 0x53)`: ndim 1, var 1, n0 0
(empty table). Not written in that state.

## Validation, errors, return codes
* Count line blank or EOF -> 2 (`ERROR: Keyword HDNTIM Reading Error. \n`).
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1403d03b0: only if FUN_14012f610(dyn) (n0 >= 1):
  "%-6s %7i %7i\n"   "HDNTIM", mat, n0(+0xfa4)
  FUN_1402f3130(fp, dyn, 0): per row " %20.10E %20.10E\n" (x, v)
otherwise nothing at all (returns 0 either way)
```
Asymmetry: `HDNTIM m 0` (present in the corpus) is read but not written back.

## Helpers
- `0x140413de0` count reader; `0x14012fc70` re-init; `0x14041a800` body reader
- `0x1403d03b0` writer; `0x14012f610` completeness test; `0x1402f3130` body writer
- `0x1403d0480` accessor

## Corpus check
43 instances in 39 files: `HDNTIM m 18` + 18 rows (20, e.g. `key/2D/EN/HeatTreat/HT_CARB1_EN.KEY`
line 124), `HDNTIM m 1` + 1 row (20, e.g.
`key/2D/EN/GravityLoading/2d_verify_Centrifugal_plus_Gravity_EP_EN.KEY` line 129), `HDNTIM m 0`
(3, `key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY`); all consumed exactly.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every HDNTIM block of those files with the read grammar above and re-wrote it with the write formats above; all 9 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).
The oracle output of USER_HT.KEY has no HDNTIM line (input had `HDNTIM m 0`), confirming the
"not written when empty" rule.

## Open questions
Meaning of independent variable 1 and of the table.
