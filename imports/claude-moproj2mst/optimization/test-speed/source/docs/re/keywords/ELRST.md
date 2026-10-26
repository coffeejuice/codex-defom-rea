# ELRST (id 115, group 4 material)

Electrical resistivity (inferred from the name; tables appear in the induction-hardening examples), constant or table in temperature, with the generic header types.

## Handlers
read: case 0x17 of `FUN_140384a50` -> `FUN_14041b270(fp, material+0x1cd8, 0, NULL)`;
write: `FUN_1403d0280`;
accessor: `FUN_1403d0320` (returns 0; ignores the label; looks up the material whose number is element 0 of the id vector and, if found, stores `material+0x1cd8` (the DYNARY) in `*out`; if not found `*out` is unchanged).

## KEY grammar (read)
```
line 1: ELRST <mat:%i> <t:%i> <a:%Flf> <b:%Flf> <c:%Flf>
        mat: fscanf("%i") in FUN_140384a50 (stream stays on the keyword line)
        rest of the line: one fgets(0x400) in FUN_14041b270, ',' -> ' ',
        sscanf "%i %Flf %Flf %Flf" (t, a, b, c zeroed first; missing fields stay 0;
        c is never used by this keyword)
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
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY settings by the reader |
|---|---|---|---|---|
| mat | material (found by number with `FUN_14029c2c0`, created if absent) | +0x0 | int | |
| t | material DYNARY | ndim +0x1cdc, var[0..1] +0x1ce8/+0x1cec | int | type (+0x1cd8) := 0, ncomp 1; ndim/var from t (table above) |
| a | same | const +0x1ce0 (t 0) or n0 +0x1cf4 (t 1..7, `(int)a`) | double / int | |
| b | same | n1 +0x1cf8 (t 4, 5, 7, `(int)b`) | int | |
| body | same | axes at +0x1d00/+0x1d08 (double*), values at +0x1d20 (double*) | double[] | |

Side effects: a material is created if `mat` is unknown (see `_common_g457.md` section 5). Each
successful header read frees the old table and resets the DYNARY type code to 0.

## Defaults
Material constructor `FUN_14028c6f0` builds the DYNARY at +0x1cd8 with `FUN_14012d050` (type 0, ndim 0, const 0.0, ncomp 1); `FUN_14028cdc0` does not touch it. Default: constant 0.0 (written as `ELRST mat 0 0.0000000000E+000`).
Before reading, the reader re-initialises the DYNARY with `FUN_14012fc70(dyn, 0)`, but only after
the header `sscanf` succeeded; on an empty line or EOF the old content is kept.

## Validation, errors, return codes
* Rest of the keyword line empty (whitespace only) or EOF -> 2 (interpreter prints
  `ERROR: Keyword ELRST Reading Error. \n` and continues).
* EOF inside the body -> 2.
* `fscanf("%i")` for mat at EOF -> 2 before any material is created.
* Unknown t, short lines, non-numeric text: silently accepted (zeros / unchanged values).
No message specific to ELRST; 3 is never returned.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1402f3aa0(fp, "ELRST", mat, dyn, 0, NULL), then body FUN_1402f3130(fp, dyn, 0):
  ndim 0              : "%-6s %7i %7i %20.10E\n"   ELRST, mat, 0, const        (no body)
  ndim 1              : "%-6s %7i %7i %7i\n"       ELRST, mat, t, n0
                        t = 1 if var0==3, 2 if var0==9, 6 if var0==8, else 3
  ndim 2, var (3,12)  : "%-6s %7i %7i %7i %7i\n"   ELRST, mat, 4, n0, n1
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

Read/write asymmetries:
* t = 5 (var 3,8) is read, but written with no header line: the KEY produced cannot be re-read
  (the body numbers would be taken as keywords). The same holds for an ndim-3 table (this reader
  cannot produce one).
* t = 2, 3, 6 round-trip (var 9 -> 2, var 12 -> 3, var 8 -> 6); t = 1, 4, 7 round-trip.
* An unknown t is stored as the constant 0 and written back as `ELRST mat 0 0.0000000000E+000`.
* Constants and table values are printed with `%20.10E` (11 significant digits, 3-digit
  exponent), so values are not bit-exact after a round trip.
* A 1-D/2-D table with some n = 0 gets its header and no body; re-reading gives the same state.

## Helpers
- `0x140384a50` - group 4 parser, case 0x17
- `0x14041b270` - "t a b" header + body reader
- `0x14041a800` - DYNARY body reader
- `0x14012fc70` - DYNARY re-init (keeps +0x54/+0x60)
- `0x14012fad0` - DYNARY allocation
- `FUN_1403d0280` - KEY write handler
- `0x1402f3aa0` - generic header writer
- `0x1402f3130` - DYNARY body writer (mode 0); `0x1402f2e80` value-line writer
- `0x14012f610` - "table complete" test
- `0x14029c2c0` - find material by number
- `0x1402f7720` - keyword name by id

## Corpus check
Checked with a script that simulates this grammar on every `*.KEY` under `key/` and `materials/` (every line whose first token is the keyword) and verifies that the line after the consumed text is a keyword, a `*` comment, or EOF.
740 instances in 440 files, all consumed exactly. Variants: `ELRST m 0 <const>` (648); `ELRST m 1 <n>` + n rows (92, e.g. `key/2D/EN/InductionHardening/2d_Induction_Hardening_Process_EN.KEY` line 148, n=32).

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every ELRST block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
none
