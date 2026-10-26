# DIFBND (id 129, group 4 material)

A two-component property (DYNARY type 0xaf, ncomp 2): either a constant or a 2-D table in
(temperature, var 4) with two values per point. Meaning inferred from the name: boundary
diffusion data (e.g. coefficient and activation energy) - not established.

## Handlers
read: case 0x25 of `FUN_140384a50` (inline code on the DYNARY at material+0x2160);
write: `FUN_1403d2170` -> `FUN_1402f3aa0(fp, "DIFBND", mat, material+0x2160, 0, NULL)`;
accessor: `FUN_1403d2210` (returns 0; ignores the label; stores `material+0x2160` in `*out` if the
material exists).

## KEY grammar (read)
```
line 1: DIFBND <mat:%i> <t:%i> <a:%Flf> <b:%Flf>   rest of line: FUN_140415680 (fgets,
                                                    ',' -> ' ', sscanf "%i %Flf %Flf"; zeroed)
NO re-initialisation of the DYNARY (type stays 0xaf, ncomp stays 2).
if helper ok and t == 7:
    ndim (+0x2164) = 2, var[0] (+0x2170) = 3, var[1] (+0x2174) = 4,
    n0 (+0x217c) = (int)a, n1 (+0x2180) = (int)b
    body FUN_14041a800: n0 axis0, n1 axis1, then ncomp*n0*n1 = 2*n0*n1 values, then fgets of
    the rest of the last line if that count > 0
else if helper failed (blank/EOF) or t != 0: return 2
else (t == 0):
    n0 = n1 = 0, const (+0x2168) = a          (ndim is NOT changed)
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| t | DYNARY at +0x2160 | ndim +0x2164 (t 7 only) | int | type 0xaf (unchanged), ncomp 2 (unchanged) |
| a | same | const +0x2168 (t 0) / n0 +0x217c (t 7) | double / int | |
| b | same | n1 +0x2180 (t 7) | int | |
| body | same | axes +0x2188/+0x2190, values +0x21a8 | double[] (2 per point) | var (3, 4) |

## Defaults
`FUN_14028cdc0` re-initialises the DYNARY with `FUN_14012d2a0(dyn, 0xaf)`: ndim 0, ncomp 2, const
0.0 (two-value array allocated, both 0).

## Validation, errors, return codes
* Header line blank or EOF -> 2 (`ERROR: Keyword DIFBND Reading Error. \n`), nothing changed.
* t other than 0 or 7 -> 2 (Reading Error), nothing changed.
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1402f3aa0(fp, "DIFBND", mat, dyn, 0, NULL) + FUN_1402f3130(fp, dyn, 0):
  ndim 0           : "%-6s %7i %7i %20.10E\n"     DIFBND, mat, 0, const
  ndim 2, var (3,4): "%-6s %7i %7i %7i %7i\n"     DIFBND, mat, 7, n0, n1
                     then (if n0, n1 >= 1) axis0, axis1 and the 2*n0*n1 values, each group
                     " %20.10E" per value, wrapped ("\n" + 8 spaces) every max(n0, n1) values
  (other shapes cannot come from this reader)
always written; returns 0
```
Asymmetry: reading `t 0` after a `t 7` table leaves ndim 2 with n = 0; the constant is stored
but the writer then emits `DIFBND m 7 0 0` and the constant is lost. On a fresh material
(ndim 0) `t 0` round-trips.

## Helpers
- `0x140415680` "%i %Flf %Flf" reader; `0x14041a800` body reader
- `0x1403d2170` writer; `0x1402f3aa0`; `0x1402f3130`; `0x1403d2210` accessor

## Corpus check
Checked with a script that simulates this grammar on every `*.KEY` under `key/` and `materials/` (every line whose first token is the keyword) and verifies that the line after the consumed text is a keyword, a `*` comment, or EOF.
151 instances in 87 files, all `DIFBND m 0 0.0000000000E+000`; consumed exactly. No corpus
file has t = 7.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every DIFBND block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of var 4 and of the two components.
