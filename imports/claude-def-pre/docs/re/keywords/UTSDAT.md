# UTSDAT (id 119, group 4 material)

Ultimate tensile strength (inferred from the name; corpus tables of strength vs temperature in
heat-treatment examples), as a constant or a table in temperature only.

## Handlers
read: case 0x1b of `FUN_140384a50` (inline code on the DYNARY at material+0x1b10);
write: `FUN_1403d0680` -> `FUN_1402f3aa0(fp, "UTSDAT", mat, material+0x1b10, 0, NULL)`;
accessor: `FUN_1403d0720` (returns 0; ignores the label; stores `material+0x1b10` in `*out` if the
material exists).

## KEY grammar (read)
```
line 1: UTSDAT <mat:%i> <t:%i> <a:%Flf>   rest of line: FUN_1404154c0 (fgets, ',' -> ' ',
                                           sscanf "%i %Flf"; t, a zeroed first)
        blank/EOF -> return 2 (DYNARY untouched)
FUN_14012fc70(dyn, 0x1b)                  (type 0x1b: ndim 0, ncomp 1, arrays freed)
t == 0 : ndim 0, const = a                 (no body)
t == 1 : ndim 1, var 3, n0 = (int)a         body FUN_14041a800: n0 rows "temperature value"
                                             (+ fgets of the rest of the last line if n0 > 0)
other t: DYNARY stays reset (constant 0), nothing more read, no error
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| t | DYNARY (inside sub-object +0x1ad0, `FUN_14028c040`, at its +0x40) | ndim +0x1b14, var0 +0x1b20 | int | type 0x1b, ncomp 1 |
| a | same | const +0x1b18 (t 0) / n0 +0x1b2c (t 1) | double / int | |
| body | same | axis0 +0x1b38, values +0x1b58 | double[] | |

## Defaults
`FUN_14028c040` (constructor of the sub-object at +0x1ad0) initialises this DYNARY with
`FUN_14012d2a0(dyn, 0x1b)`: constant 0.0, ndim 0.

## Validation, errors, return codes
* Rest of line blank or EOF -> 2 (`ERROR: Keyword UTSDAT Reading Error. \n`).
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Unknown t silently gives constant 0. Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1402f3aa0(fp, "UTSDAT", mat, dyn, 0, NULL), then body FUN_1402f3130(fp, dyn, 0):
  ndim 0              : "%-6s %7i %7i %20.10E\n"   UTSDAT, mat, 0, const        (no body)
  ndim 1              : "%-6s %7i %7i %7i\n"       UTSDAT, mat, t, n0
                        t = 1 if var0==3, 2 if var0==9, 6 if var0==8, else 3
  ndim 2, var (3,12)  : "%-6s %7i %7i %7i %7i\n"   UTSDAT, mat, 4, n0, n1
  ndim 2, var (3,4)   : same with t = 7
  ndim 2, var (3,20)  : same with t = 4
  ndim 2, other vars  : NO header line (body only)
  ndim 3              : NO header line
body, only when every n[k] >= 1 (FUN_14012f610):
  ndim 1: per row " %20.10E" x, " %20.10E" v, "\n"
  ndim 2: axis0 as " %20.10E" per value, with "\n" + 8 spaces before every value whose index
          is a non-zero multiple of max(n0,n1), then "\n"; axis1 the same; then the n0*n1
          values the same.
Written for every material (FUN_1403cdca0 skips only materials with byte +0x2210 set).
The handler always returns 0 (the result of FUN_1402f3aa0 is discarded).
```
Only ndim 0 and ndim 1 / var 3 can come from this reader: written as `UTSDAT m 0 <const>` and
`UTSDAT m 1 <n>` + rows, both re-readable. (If an accessor edit produced another shape, e.g.
ndim 1 var 12 -> t 3, or a 2-D table, the reader would reset it to constant 0 and, for 2-D, take
the body as keywords.)

## Helpers
- `0x1404154c0` - "%i %Flf" line reader; `0x14041a800` body; `0x14012fc70` re-init
- `0x1403d0680` write handler; `0x1402f3aa0` header writer; `0x1402f3130` body writer
- `0x1403d0720` accessor; `0x14028c040` sub-object constructor

## Corpus check
Checked with a script that simulates this grammar on every `*.KEY` under `key/` and `materials/` (every line whose first token is the keyword) and verifies that the line after the consumed text is a keyword, a `*` comment, or EOF.
740 instances in 440 files: `UTSDAT m 0 <c>` (730), `UTSDAT m 1 <n>` + rows (10, e.g.
`key/2D/EN/HeatTreat/HT_CRACK_EN.KEY` line 160, n=7); all consumed exactly.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every UTSDAT block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
none
