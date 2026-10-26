# HEATCP (id 99, group 4 material)

Heat capacity (inferred from the name; volumetric capacity in the corpus, e.g. 4.631 N/mm^2/C),
constant or table with the generic header types, plus an extra integer on the header line
(meaning unknown; always 0 in the corpus).

## Handlers
read: case 7 of `FUN_140384a50` -> `FUN_14041b270(fp, material+0x858, 1, &tmp)` with a local
double `tmp = 0.0`, then `*(int*)(material+0x8d0) = (int)tmp`;
write: `FUN_1403cf5c0` -> `FUN_1402f40f0(fp, "HEATCP", mat, material+0x858, 1, material+0x8d0)`;
accessor: `FUN_1403cf670` (returns 0; ignores the label; stores `material+0x858` in `*out` if the
material exists).

## KEY grammar (read)
```
line 1: HEATCP <mat:%i> <t:%i> <a:%Flf> <b:%Flf> <c:%Flf>
        mat: fscanf("%i") in FUN_140384a50 (stream stays on the keyword line)
        rest of the line: one fgets(0x400) in FUN_14041b270, ',' -> ' ',
        sscanf "%i %Flf %Flf %Flf" (t, a, b, c zeroed first; missing fields stay 0;
        c is used only for t 4)
        rest of line empty (whitespace only) or EOF -> helper returns 1 -> parser returns 2,
        DYNARY untouched
        otherwise FUN_14012fc70(dyn, 0): DYNARY type := 0, ndim 0, const 0, var/n 0, ncomp 1,
        arrays freed
  t = 0 : constant = a                                   (no body)
  t = 1 : ndim 1, var 3,  n0 = (int)a                    body: n0 rows "x v"
  t = 2 : ndim 1, var 9,  n0 = (int)a                    body: n0 rows "x v"
  t = 3 : ndim 1, var 12, n0 = (int)a                    body: n0 rows "x v"
  t = 4 : ndim 2, var (3,12), n0 = (int)a, n1 = (int)b   body: n0 axis0, n1 axis1, n0*n1 values
  t = 5 : ndim 2, var (3,8),  n0, n1 as for 4            body as for 4
  t = 6 : ndim 1, var 8,  n0 = (int)a                    body: n0 rows "x v"
  t = 7 : ndim 2, var (3,4),  n0, n1 as for 4            body as for 4
  other t (negative or >= 8): constant 0, nothing more read, no error
body (FUN_14041a800): whitespace-separated "%Flf" tokens, any line breaks;
  1-D: x[i] then v[i] for each row (ncomp 1);
  2-D: all n0 axis0 values, all n1 axis1 values, then n0*n1 values in file order.
  After the last value, if the value count is > 0, one fgets discards the rest of that line.
  EOF (fscanf -1) inside the body -> return 1 -> parser returns 2 (table stays partly filled).
  A non-numeric token is not detected: the value stays 0 and the stream does not advance, so
  every later read fails the same way; the interpreter then reads that token as a keyword.
  n0 / n1 <= 0: FUN_14012fad0 clamps negatives to 0; nothing read; no fgets.
extra: tmp = b if t in 0..3 and >= 3 fields converted; tmp = c if t == 4 and >= 4 fields;
       otherwise tmp stays 0.0.  Then material+0x8d0 = (int)tmp (C truncation) - ALWAYS, also
       when the header line was blank/EOF (then 0).
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY settings by the reader |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| t | material DYNARY | ndim +0x85c, var +0x868/+0x86c | int | type (+0x858) := 0, ncomp 1 |
| a | same | const +0x860 (t 0) or n0 +0x874 | double / int | |
| b | same | n1 +0x878 (t 4, 5, 7) | int | |
| b (t 0..3) / c (t 4) | material | +0x8d0 | int (`(int)` of a `%Flf`) | extra |
| body | same | axes +0x880/+0x888, values +0x8a0 | double[] | |

## Defaults
DYNARY at +0x858 built by `FUN_14012d050` (type 0, const 0.0); `FUN_14028cdc0` sets +0x8d0 = 0.
Each read overwrites +0x8d0 (0 when the extra is absent).

## Validation, errors, return codes
* Rest of the keyword line blank or EOF -> 2 (`ERROR: Keyword HEATCP Reading Error. \n`); DYNARY
  untouched, +0x8d0 set to 0.
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1402f40f0(fp, "HEATCP", mat, dyn, 1, &extraInt), then FUN_1402f3130(fp, dyn, 0):
  ndim 0 : "%-6s %7i %7i %20.10E %7i\n"         HEATCP, mat, 0, const, extra
  ndim 1 : "%-6s %7i %7i %7i %7i\n"             HEATCP, mat, t, n0, extra
           t = 1 (var 3), 2 (var 9), else 3  (NB: var 8 -> 3, unlike FUN_1402f3aa0)
  ndim 2 : "%-6s %7i %7i %7i %7i %7i\n"         HEATCP, mat, 4, n0, n1, extra  (any var pair)
  ndim 3 : no header line
body as for YOUNG.
```
Always written; the handler returns 0.

Read/write asymmetries:
* t 6 (var 8) is written as t 3 -> re-read as var 12.
* t 5 (3,8) and t 7 (3,4) are written as t 4 -> re-read as var (3,12).
* The extra is printed with `%7i`; re-read with `%Flf` and truncated, so it round-trips.

## Helpers
- `0x14041b270`, `0x14041a800` reader; `0x1403cf5c0` write handler; `0x1402f40f0` header writer
  (int extra); `0x1402f3130` body writer; `0x1403cf670` accessor.

## Corpus check
Checked with a script that simulates this grammar on every `*.KEY` under `key/` and `materials/` (every line whose first token is the keyword) and verifies that the line after the consumed text is a keyword, a `*` comment, or EOF.
740 instances in 440 files, all consumed exactly. Variants: `HEATCP m 1 <n>` + rows (345, no
extra), `HEATCP m 0 <c>` (237), `HEATCP m 1 12 0` + rows (96, e.g.
`key/2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY` line 136), `HEATCP m 0 <c> 0` (62, e.g.
`key/2D/EN/HyperElastic/RubberBoot_2D.KEY` line 104).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every HEATCP block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of the integer at +0x8d0 (always 0 in the corpus).
