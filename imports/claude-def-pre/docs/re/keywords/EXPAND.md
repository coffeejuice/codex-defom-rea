# EXPAND (id 97, group 4 material)

Thermal expansion coefficient (inferred from the name and corpus values ~1E-005), constant or
table with the generic header types, plus one extra double on the header line that is
inferred to be the reference temperature of the expansion (corpus values 68 F, 20 C, 1202).

## Handlers
read: case 5 of `FUN_140384a50` -> `FUN_14041b270(fp, material+0x760, 1, material+0x7d8)`;
write: `FUN_1403cf2b0` -> `FUN_1402f3aa0(fp, "EXPAND", mat, material+0x760, 1, material+0x7d8)`;
accessor: `FUN_1403cf360` (returns 0; ignores the label; stores `material+0x760` (the DYNARY) in
`*out` if the material named by element 0 of the id vector exists; the extra double is not
exposed).

## KEY grammar (read)
```
line 1: EXPAND <mat:%i> <t:%i> <a:%Flf> <b:%Flf> <c:%Flf>
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
extra (reference temperature), stored straight into material+0x7d8:
  t in 0..3 and sscanf converted >= 3 fields (t, a, b) : extra = b
  t == 4    and sscanf converted >= 4 fields (t,a,b,c) : extra = c
  otherwise (t 5, 6, 7, unknown t, or field missing)   : +0x7d8 NOT modified (keeps old value)
```
So the header lines are
`EXPAND m 0 <const> <ref>`, `EXPAND m 1|2|3 <n> <ref>`, `EXPAND m 4 <n0> <n1> <ref>`.

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY settings by the reader |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| t | material DYNARY | ndim +0x764, var +0x770/+0x774 | int | type (+0x760) := 0, ncomp 1 |
| a | same | const +0x768 (t 0) or n0 +0x77c (`(int)a`) | double / int | |
| b | same | n1 +0x780 (t 4, 5, 7) | int | |
| b (t 0..3) / c (t 4) | material | +0x7d8 | double | extra |
| body | same | axes +0x788/+0x790, values +0x7a8 | double[] | |

## Defaults
DYNARY at +0x760 built by `FUN_14012d050` (type 0, const 0.0); `FUN_14028cdc0` sets +0x7d8 = 0.0.
The reader re-initialises the DYNARY (after a successful header sscanf) but never clears +0x7d8.

## Validation, errors, return codes
* Rest of the keyword line blank or EOF -> 2 (`ERROR: Keyword EXPAND Reading Error. \n`); DYNARY
  and extra untouched.
* EOF inside the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Unknown t / missing fields silently accepted. Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1402f3aa0(fp, "EXPAND", mat, dyn, 1, &extra), then FUN_1402f3130(fp, dyn, 0):
  ndim 0              : "%-6s %7i %7i %20.10E %20.10E\n"   EXPAND, mat, 0, const, extra
  ndim 1              : "%-6s %7i %7i %7i %20.10E\n"       EXPAND, mat, t, n0, extra
                        t = 1 (var 3), 2 (var 9), 6 (var 8), else 3
  ndim 2, var (3,12)  : "%-6s %7i %7i %7i %7i %20.10E\n"   EXPAND, mat, 4, n0, n1, extra
  ndim 2, var (3,4)   : same with t = 7
  ndim 2, var (3,20)  : same with t = 4
  ndim 2, other / ndim 3 : NO header line (body only)
body as for YOUNG (rows " %20.10E %20.10E\n"; 2-D: axis0, axis1, values wrapped every max(n)).
```
Always written; the handler returns 0.

Read/write asymmetries:
* t 6 (var 8): written with the extra, but on re-read t 6 does not store the extra -> the extra
  keeps its previous value (0.0 on a fresh material).
* t 7 (var 3,4): written as t 7 with the extra; re-read ignores it (same consequence).
* t 5 (var 3,8): no header line -> output not re-readable.
* Unknown t: constant 0 written as `EXPAND m 0 0.0 <extra>`.

## Helpers
- `0x14041b270` header + body reader (extra handling); `0x14041a800` body
- `0x1403cf2b0` write handler; `0x1402f3aa0` header writer; `0x1402f3130` body writer
- `0x1403cf360` accessor; `0x14029c2c0` find material

## Corpus check
Checked with a script that simulates this grammar on every `*.KEY` under `key/` and `materials/` (every line whose first token is the keyword) and verifies that the line after the consumed text is a keyword, a `*` comment, or EOF.
740 instances in 440 files, all consumed exactly. Variants: `EXPAND m 0 <c> <ref>` (477),
`EXPAND m 1 <n> <ref>` + rows (248, e.g. `key/2D/EN/DieStress/DieStress_Example1_EN.KEY` line
111, ref 68), `EXPAND m 3 4 <ref>` + 4 rows (8, `key/2D/EN/HeatTreat/HT_CARB1_EN.KEY` line 183),
`EXPAND m 4 20 1 <ref>` + 2-D block (4, `key/2D/EN/InductionHeating/Induction_Heating_Example3_EN.KEY`
line 280), and in `key/3D/EN/Cogging/Cogging_Example_Old/USER_HT.KEY` lines without the extra
(`EXPAND 2 1 6`, `EXPAND 1 0 .0000000E+00`: the extra keeps its default 0.0).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every EXPAND block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Exact meaning of the extra (reference temperature is inferred) and of var 12 in t 3/t 4.
