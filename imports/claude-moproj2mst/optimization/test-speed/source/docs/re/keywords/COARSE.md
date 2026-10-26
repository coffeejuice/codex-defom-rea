# COARSE (id 126, group 4 material)

Grain coarsening (inferred from the name): an integer switch on the keyword line and, when it
is > 0, a generic property (constant or table) on the following line(s).

## Handlers
read: case 0x22 of `FUN_140384a50` -> `FUN_140413de0(fp, material+0x20d0, 0)`, then if
`*(int*)(material+0x20d0) > 0`: `FUN_14041b270(fp, material+0x20d8, 0, NULL)`;
write: `FUN_1403d1fe0`;
accessor: `FUN_1403d20e0` (returns 0; ignores the label; stores `material+0x20d8` (the DYNARY) in
`*out` if the material exists).

## KEY grammar (read)
```
line 1: COARSE <mat:%i> <flag:%i>    rest of line: fgets, ',' -> ' ', sscanf "%i" into
                                      material+0x20d0 (zeroed first)
        blank/EOF -> 2 (flag = 0)
if flag <= 0: done
if flag > 0:
line 2: <t:%i> <a:%Flf> <b:%Flf>      a whole new line read by FUN_14041b270 exactly as for YOUNG
                                      (t 0..7, re-init FUN_14012fc70(dyn, 0), body)
        blank/EOF -> 2
```

## Model mapping
| KEY field | Object | Byte offset | C type | DYNARY |
|---|---|---|---|---|
| mat | material | +0x0 | int | |
| flag | material | +0x20d0 | int | |
| t, a, b, body | DYNARY at +0x20d8 | ndim +0x20dc, const +0x20e0, var +0x20e8/+0x20ec, n +0x20f4/+0x20f8, axes +0x2100.., values +0x2120 | | type 0, ncomp 1, as the generic table |

## Defaults
`FUN_14028cdc0` sets +0x20d0 = 0; the DYNARY at +0x20d8 is built by `FUN_14012d050` (type 0,
constant 0.0). With flag 0 the DYNARY is neither read nor written.

## Validation, errors, return codes
* Line 1 blank or EOF -> 2 (`ERROR: Keyword COARSE Reading Error. \n`).
* flag > 0 and line 2 blank or EOF, or EOF in the body -> 2.
* `fscanf("%i")` for the material number at EOF -> 2 before any material is created (a
  failed conversion is not detected: the number is then an uninitialised stack value).
* Writer-side warning (stdout via printf, see below):
  `"\nWarning: data for keyword %s has error. Its data type will be clear(0).\n"`.
* Never returns 3.

## Unit conversion
none found

## KEY grammar (write)
```
FUN_1403d1fe0:
flag(+0x20d0) < 1:
  "%-6s %7i %7i\n"   "COARSE", mat, flag                         (DYNARY not written)
flag >= 1: extra = (double)flag; FUN_1402f3aa0(fp, "COARSE", mat, dyn, 1, &extra), which has
special cases for name == "COARSE" (strcmp):
  ndim 0 : "%-6s %7i %7i\n"   "COARSE", mat, (int)extra
           " %7i %20.10E\n"   0, const
  ndim 1 : NOT special-cased: "%-6s %7i %7i %7i %20.10E\n"  "COARSE", mat, t, n0, extra
           (t = 1 var 3, 2 var 9, 6 var 8, else 3), then the rows
  ndim 2 : "%-6s %7i %7i\n"   "COARSE", mat, (int)extra
           " %7i %7i %7i\n"   t, n0, n1    with t = 5 if var == (3,8), else 0
           then the 2-D body
           (the branch for a missing extra prints the Warning above and
            "%-6s %7i %7i\n" "COARSE", mat, 0 - unreachable from FUN_1403d1fe0, which always
            passes &extra)
  ndim 3 : no header line, body only
returns 0
```
Read/write asymmetries (bugs of the original that a port must reproduce):
* ndim 1 (t 1, 2, 3, 6 on line 2): written on ONE line `COARSE m t n0 flag` followed by rows; on
  re-read `t` is taken as the flag and the first table row as the "t a b" line - the output
  does not round-trip.
* ndim 2 with var (3,12) (t 4) or (3,4) (t 7): written with t = 0 on line 2, so re-read as a
  constant equal to n0 and the body becomes stray tokens.
* t 5 (3,8) and t 0 round-trip; ndim 3 cannot be produced by the reader.

## Helpers
- `0x140413de0` flag reader; `0x14041b270` / `0x14041a800` property reader
- `0x1403d1fe0` writer; `0x1402f3aa0` header writer (COARSE special cases); `0x1402f3130` body
- `0x1403d20e0` accessor; `0x1402f7720(0x7e)` keyword name

## Corpus check
158 instances in 92 files, all `COARSE m 0` (one line); consumed exactly. No corpus file has
flag > 0, so the second-line forms are verified from code only.

Write grammar: confirmed byte for byte against the oracle KFWRIT outputs (`deform-rea-work/out/kw457/oracle/golden/*.OUT.KEY` and `golden/variants/*.OUT.KEY`, 23 files: the original DEF_PRE read a corpus KEY and wrote it back). A script re-read every COARSE block of those files with the read grammar above and re-wrote it with the write formats above; all 63 blocks were reproduced exactly (formats, spacing, 3-digit exponents, wraps). Lines end in CRLF in those files (text-mode FILE).

## Open questions
Meaning of flag values > 1 (the value is only echoed back); meaning of the property.
