# GRNDAT (id 108, group 4 material)

Grain / recrystallization model data of one material. A model selector (`type`, material
`+0x17e8`) picks one of four mutually exclusive data sets inside the material sub-object at
`+0x1078` (constructor `FUN_14028b850`):

* **type 1**: nine 1-D tabulated coefficient sets (DYNARY types 0x55…0x5c), four on/off flags and
  two trailing doubles. Inferred from the coefficient counts and the IN718 / MRX_SRX corpus values,
  this is the JMAK-style recrystallization model: peak/critical strain, dynamic, metadynamic and
  static recrystallization kinetics and grain sizes, and grain growth (see the table in
  "Model mapping"; every per-set meaning is **inferred**).
* **type 2**: one "main" record plus five linked lists of records, each record holding up to 12
  doubles, an int, a double and one or three small sub-functions of temperature (var 3) and/or
  var 23. Meaning unknown (the clamp message calls the count "SubbdryVarSize").
* **type 4**: one generic DYNARY at `+0x1740`, normally f(temperature, strain rate).
* **type 5**: six plain doubles at `+0x17b8`.
* **type < 0**: stores `|type|` at `+0x17ec` and sets `type = -1` (a reference number, meaning
  unknown; the writer does not write it back).
* **type 0** (or 3, ≥ 6): no data; nothing is written.

After every completed GRNDAT read the parser calls `FUN_1401d0fc0(modelData, 0)`, which
recomputes the model's state-variable layout (grain state variables depend on the GRNDAT type of
the materials) and resizes every object's state arrays.

## Handlers

* read: case `0x10` of `FUN_140384a50` (group 4 parser, id 108 − 0x5c).
* write: `FUN_1403d3a90(FILE*, modelData, vector<int>{mat})`, called unconditionally by
  `FUN_1403cdca0` between ANISO and BURGRS.
* accessor: none (registry handler3 is the stub `0x1402f7610`, returns -1).

## KEY grammar (read)

Notation: `L:` = one line read with `fgets` by a line helper (`,` → space, short line = zeros, see
`_common_g457.md` §3); `S:` = whitespace-separated `fscanf` tokens that may span lines; `DYN(d)` =
body read by `FUN_14041a800` (`_common_g457.md` §4).

```
L: GRNDAT <mat:%i> <type:%i> [<c:%i>]          ("%i" by FUN_140384a50, rest of line FUN_140413ee0 "%i %i")
   material.+0x17e8 = type                       (set before branching, for every type)

type == 1:
  9 x  L: <code_k:%i> <n_k:%i>                   (FUN_140413ee0, k = 1..9, no blank-line skip)
         for each k, in this order, BEFORE any body is read:
           FUN_14012fc70(D_k, T_k)   -> ndim 1, var0 3, n 0, ncomp per type (table below)
           var0 / flag from code_k (table below), n0 = n_k
  9 x  DYN(D_k)  k = 1..9                        (ndim 1: n_k rows of  <x> <v1>..<v_ncomp_k>)
                                                 (rest of the last row's line discarded)
  L: <g1:%Flf> <g2:%Flf>                          (FUN_140414a40)  g1 -> +0x14c8, g2 -> +0x14c0

type == 2:  (c from the keyword line -> +0x14d0)
  REC(main record @ +0x14d8)
  for list in (L1 @+0x16f8, L2 @+0x16e0, L3 @+0x16c8, L4 @+0x1710, L5 @+0x1728):
     list is emptied first (FUN_14028b240: every record destroyed)
     L: <count:%i>                                (FUN_140413de0 with blank-line skip; result IGNORED:
                                                   EOF or empty line -> count 0)
     count x { append new record (FUN_14028b050); REC(record) }

  REC(r):                                         (FUN_14041bed0)
     S: <n:%i>                                    (n > 12 -> n = 12 and stdout message, see below)
     S: n x <v:%Flf>                              -> r+0x08 .. (double[12])
     S: <i:%i> <d:%Flf>                           -> r+0x70 (int), r+0x68 (double)
     if r is NOT the main record (byte r+0x1e0 == 0):
        SUB(r+0x78)
        SUB(r+0xf0)
     SUB(r+0x168)

  SUB(D):                                         (FUN_14041c070, DYNARY is NOT re-initialised)
     L: <t:%i> <a:%Flf> <b:%Flf> <c:%Flf>         (if the fgets returns exactly "\n" one more line is read;
                                                   this is what eats the end of the "<i> <d>" line)
     t == 0: ndim 0, const = a                    (no body)
     t == 1: ndim 1, var0 3,  n0 = (int)a         then DYN(D)
     t == 2: ndim 1, var0 23, n0 = (int)a         then DYN(D)
     t == 3: ndim 2, var 3,23, n = (int)a,(int)b  then DYN(D)
     t >= 4: no field changed, but DYN(D) is still called with the DYNARY's previous ndim/n
     t < 0 or a whitespace-only line (t stays -1): nothing, no error

type == 4:
  L: <t:%i> <a:%Flf> <b:%Flf> <c:%Flf>            FUN_14041b270(fp, +0x1740, 0, NULL) generic header
  DYN(+0x1740)                                    (writer always uses t = 5: ndim 2, var 3, 8)

type == 5:
  L: <p1> <p2> <p3> <p4> <p5> <p6>  (%Flf x6)     FUN_140415080 -> +0x17b8,+0x17c0,+0x17c8,+0x17d0,+0x17d8,+0x17e0

type < 0:   no further input; +0x17ec = -type, +0x17e8 = -1
type 0, 3, >= 6: no further input; +0x17e8 = type

then (all types, unless an early "return 2" happened): FUN_1401d0fc0(modelData, 0)
```

Type-1 per-set code mapping (`D_k`, `T_k` = DYNARY type, `F` = flag int):

| k | D_k (material) | T_k | ncomp | code 0 | code 1 | code 2 | code 3 | other |
|---|---|---|---|---|---|---|---|---|
| 1 | +0x1088 | 0x55 | 4 | var 3 | var 3 | var 8 | var 3 | var 3 |
| 2 | +0x1100 | 0x54 | 6 | var 3 | var 3 | var 8 | var 3 | var 3 |
| 3 | +0x1178 | 0x56 | 7 | F(+0x1078)=0 | F=1, var 3 | F=1, var 8 | F unchanged, var 3 | same as 3 |
| 4 | +0x11f0 | 0x57 | 6 | var 3 | var 3 | var 8 | var 33 | var 3 |
| 5 | +0x1268 | 0x58 | 7 | F(+0x107c)=0 | F=1, var 3 | F=1, var 8 | F unchanged | F unchanged |
| 6 | +0x12e0 | 0x59 | 6 | var 3 | var 3 | var 8 | var 33 | var 3 |
| 7 | +0x1358 | 0x5a | 9 | F(+0x1080)=0 | F=1, var 3 | F=1, var 8 | F unchanged | F unchanged |
| 8 | +0x13d0 | 0x5b | 6 | var 3 | var 3 | var 8 | var 33 | var 3 |
| 9 | +0x1448 | 0x5c | 3 | F(+0x1084)=0 | F=1, var 3 | F=1, var 8 | F=1, var 33 | F unchanged |

("var 3" in the code-0/other columns is the default left by the re-init; `n0 = n_k` is stored for
every code, including code 0, so a disabled set still reads `n_k` rows.)

Var codes: 3 = temperature, 8 = strain rate (`_common_g457.md`), 23 and 33 unknown.

What ends each part: the nine code lines are exactly nine `fgets`; each body consumes exactly
`n_k·(1+ncomp_k)` numbers regardless of line breaks, then the rest of its last line; the final
pair is one line. Type 2 is count-driven throughout. Nothing is terminated by a keyword.

Short lines / EOF: a short code line gives 0 for the missing fields. EOF or a whitespace-only line
for any type-1 code line, any type-1 body, the type-2 main record, or any type-2 list record
returns 2 **immediately** (no `FUN_1401d0fc0`). EOF on the final type-1 line, in type 4 or type 5,
returns 2 after `FUN_1401d0fc0` was called. A blank/EOF count line in type 2 silently gives count 0.

Worked example (`materials/IN718_6um[1650-2200F(900-1200C)]_s000006.KEY`, lines 123–153):

```
123  GRNDAT       1       1          mat 1, type 1, c = 0 (absent)
124  "1 1"  D1 0x55 var 3, n0 1      (ncomp 4)
125  "1 1"  D2 0x54 var 3, n0 1      (ncomp 6)
126  "1 1"  D3 0x56 F+0x1078=1, n0 1 (ncomp 7)
127  "1 1"  D4 0x57 var 3, n0 1      (ncomp 6)
128  "1 1"  D5 0x58 F+0x107c=1       (ncomp 7)
129  "1 1"  D6 0x59                  (ncomp 6)
130  "1 1"  D7 0x5a F+0x1080=1       (ncomp 9)
131  "1 1"  D8 0x5b                  (ncomp 6)
132  "1 1"  D9 0x5c F+0x1084=1       (ncomp 3)
133      D1: 1000 | 0 0 0 0                                  5 numbers = 1+4
134-135  D2: 0 | 4.659e-3 0 0.1238 4.952e4 0 0.83            7 = 1+6 (wraps after 6)
136-137  D3: 0 | 3.16 0 -0.75 0 7.479e4 0.693 0.3            8 = 1+7
138-139  D4: 0 | 678 0 0 0 -31694 0                          7 = 1+6
140-141  D5: 0 | 5.043e-9 0 -1.42 -0.408 1.96e5 0.693 1      8 = 1+7
142-143  D6: 0 | 4.85e10 0 -0.41 -0.028 -2.4e5 0             7 = 1+6
144-145  D7: 0 | 5.043e-9 0 -1.42 -0.408 1.96e5 0 0.693 1 0.8   10 = 1+9
146-147  D8: 0 | 4.85e10 0 -0.41 -0.028 -2.4e5 0             7 = 1+6
148      D9: 0 | 2 1.58e16 3.90753e5                         4 = 1+3
149      "0 0"  -> +0x14c8 = 0, +0x14c0 = 0
150  BURGRS ...                     next keyword, stream aligned
```

Every body ends exactly at the end of a line, so the discarding `fgets` only eats `"\n"`. The
whole block is consumed exactly.

## Model mapping

Sub-object at material `+0x1078` (`FUN_14028b850`, 0x778 bytes; offsets below are absolute
material offsets, sub-object offset in brackets).

| Material off | Type | Field / KEY source | Set by reader |
|---|---|---|---|
| +0x1078 [+0x000] | int | flag of D3 (inferred: DRX on) | type 1, code_3 |
| +0x107c [+0x004] | int | flag of D5 (inferred: MDRX on) | type 1, code_5 |
| +0x1080 [+0x008] | int | flag of D7 (inferred: SRX on) | type 1, code_7 |
| +0x1084 [+0x00c] | int | flag of D9 (inferred: grain growth on) | type 1, code_9 |
| +0x1088 [+0x010] | DYNARY 0x55 | D1, ncomp 4, ndim 1 (meaning unknown; all zeros in the corpus) | type 1 |
| +0x1100 [+0x088] | DYNARY 0x54 | D2, ncomp 6 (inferred: peak strain a1 h1 m1 Q1 c1 + critical-strain ratio a2) | type 1 |
| +0x1178 [+0x100] | DYNARY 0x56 | D3, ncomp 7 (inferred: DRX kinetics, a h n m Q β k) | type 1 |
| +0x11f0 [+0x178] | DYNARY 0x57 | D4, ncomp 6 (inferred: DRX grain size a h n m Q c) | type 1 |
| +0x1268 [+0x1f0] | DYNARY 0x58 | D5, ncomp 7 (inferred: MDRX kinetics) | type 1 |
| +0x12e0 [+0x268] | DYNARY 0x59 | D6, ncomp 6 (inferred: MDRX grain size) | type 1 |
| +0x1358 [+0x2e0] | DYNARY 0x5a | D7, ncomp 9 (inferred: SRX kinetics) | type 1 |
| +0x13d0 [+0x358] | DYNARY 0x5b | D8, ncomp 6 (inferred: SRX grain size) | type 1 |
| +0x1448 [+0x3d0] | DYNARY 0x5c | D9, ncomp 3 (inferred: grain growth exponent m, a, Q; IN718: 2, 1.58e16, 390753) | type 1 |
| +0x14c0 [+0x448] | double | g2, 2nd number of the last type-1 line (a temperature: 0 C in SI files, 32 F / 1580 F vs 860 C in EN/SI pairs; meaning inferred) | type 1 |
| +0x14c8 [+0x450] | double | g1, 1st number of the last type-1 line (0 in every corpus file) | type 1 |
| +0x14d0 [+0x458] | int | `c`, 3rd int of the keyword line | type 2 only |
| +0x14d8 [+0x460] | record (0x1f0) | main record, constructed with byte +0x1e0 = 1 | type 2 |
| +0x16c8 [+0x650] | list {head*, tail*, int count} | L3 | type 2 |
| +0x16e0 [+0x668] | list | L2 | type 2 |
| +0x16f8 [+0x680] | list | L1 (first in the file) | type 2 |
| +0x1710 [+0x698] | list | L4 | type 2 |
| +0x1728 [+0x6b0] | list | L5 | type 2 |
| +0x1740 [+0x6c8] | DYNARY (ctor type 0xac: ndim 2, var 3, 8; re-init to type 0 by the reader) | type-4 table | type 4 |
| +0x17b8 [+0x740] | double[6] | p1..p6 | type 5 |
| +0x17e8 [+0x770] | int | GRNDAT type (1, 2, 4, 5, -1 or the raw value) | always |
| +0x17ec [+0x774] | int | `-type` for type < 0 | type < 0 |

Type-1 DYNARYs after reading: `type` = T_k, `ndim` 1, `var0` per the code table, `n0 = n_k`,
`ncomp` per T_k (4, 6, 7, 6, 7, 6, 9, 6, 3), `values` = `n_k·ncomp` doubles; row `i` holds
`axis0[i]` = x (temperature or strain rate) followed by the `ncomp` coefficients.

Type-2 record (0x1f0 bytes, constructor `FUN_14028a4c0` → init `FUN_14028a580`, destructor
`FUN_14028a530`):

| Rec off | Type | Field |
|---|---|---|
| +0x00 | int | n (number of values, ≤ 12; init sets 12) |
| +0x08 | double[12] | values (init 0) |
| +0x68 | double | `d` |
| +0x70 | int | `i` |
| +0x78 | DYNARY 0x5e (ndim 0) | sub-function 1 (not read/written for the main record) |
| +0xf0 | DYNARY 0x5f (ndim 0) | sub-function 2 (not read/written for the main record) |
| +0x168 | DYNARY 0x60 (ndim 0) | sub-function 3 |
| +0x1e0 | byte | 1 = main record (set by `FUN_14028b640` via `FUN_14029cda0`), 0 for list records |
| +0x1e8 | record* | next |

Sub-function DYNARYs: `t` 1 → ndim 1 var 3; 2 → ndim 1 var 23; 3 → ndim 2 var (3, 23); ncomp
stays 1 (from the 0x5e/0x5f/0x60 init), so each 1-D row is `x v`.

Side effects:

* New material bookkeeping on first sight of `mat` (`_common_g457.md` §5).
* `FUN_1401d0fc0(modelData, 0)` after the switch case (skipped on the early `return 2`s): copies
  the 0x4f-int state-variable size table at modelData `+0x15f8`, recomputes it with
  `FUN_1401d0320`, then for every object in modelData `+0x1380` whose kind (`+0xc`) is not 7 calls
  `FUN_140199650(obj, …)` to resize the element (`obj+0x8a0`) and node (`obj+0x9e0`) state
  storage to the new layout (with argument 0 the per-material data copy branch is skipped).
  GRNDAT-dependent entries: `FUN_1401d64f0(md, 0x35)` is true when modelData `+0x864 != 0` and the
  first material with GRNDAT type 1, 4 or 5 is found; entry `+0x16cc` (index 0x35) becomes
  `FUN_1401d70c0(md, type)` = 16, and modelData `+0x898` is set to 0. `FUN_1401d64f0(md, 0x1d)` is
  true when `+0x864 != 0` and some material has GRNDAT type 2; entry `+0x166c` (index 0x1d)
  becomes `13·FUN_1401d71b0(...) + 2·k` (from the type-2 data), and `+0x898` is set to 1.

## Defaults

Constructor (`FUN_14028b850` and callees): type `+0x17e8 = 0`, `+0x17ec = 0`; flags 0; the nine
type-1 DYNARYs initialised with their types 0x55…0x5c (ndim 1, var 3, n0 0, ncomp per type);
`+0x14c0 = +0x14c8 = 0`; `+0x14d0 = 0`; main record: n = 12, values 0, `+0x68 = 0`, `+0x70 = 0`,
sub-functions types 0x5e/0x5f/0x60 ndim 0 const 0, byte `+0x1e0 = 1`; five empty lists;
`+0x1740` DYNARY type 0xac (ndim 2, var 3, 8, n 0); six doubles 0.

Reader resets: type 1 re-initialises all nine DYNARYs (`FUN_14012fc70`) but **not** the four flags
when the code is outside the handled values, and not the trailing doubles until the last line is
read. Type 2 empties the five lists but does **not** reset the main record (its sub-function
DYNARYs keep earlier ndim/n/var when the new `t` is ≥ 4 or < 0). Type 4 re-inits `+0x1740` to
DYNARY type 0. Type 5 zeroes the six doubles (line helper) before scanning. The data of the other
types is left untouched when the type changes.

## Validation, errors, return codes

* Any early failure listed in the grammar → `return 2` ("ERROR: Keyword %s Reading Error. \n").
* Final-helper failure (type-1 last line, type 4, type 5) → 2 after `FUN_1401d0fc0`.
* Type-2 count `n > 12`: `FUN_14029ccd0` clamps to 12 and prints to stdout
  `"Error - SubbdryVarSize is incorrect.\n"`; reading continues with 12 values (the extra values
  are then misread as `i d` and the following fields).
* No `return 3`. Unknown type values, unknown sub-codes and unknown sub-function `t` are accepted
  silently.

## Unit conversion

None found. The corpus EN files hold temperatures in °F (e.g. 1832 for 1000 °C) and the reader
stores them as read.

## KEY grammar (write)

`FUN_1403d3a90`; `name` = `"GRNDAT"`, `%-6s`. Branches on `+0x17e8`:

```
type 1:
  "%-6s %7i %7i\n"             name, mat, 1
  9 x " %7i %7i\n"             code_k, n0(D_k)
      code_1 (+0x1098), code_2 (+0x1110):            var0 == 3 ? 1 : 2
      code_3 (flag +0x1078, var +0x1188):            flag == 0 ? 0 : (var0 == 3 ? 1 : 2)
      code_4 (+0x1200), code_6 (+0x12f0), code_8 (+0x13e0): var0 3 -> 1, 8 -> 2, else 3
      code_5 (flag +0x107c, var +0x1278):            as code_3
      code_7 (flag +0x1080, var +0x1368):            as code_3
      code_9 (flag +0x1084, var +0x1458):            flag 0 -> 0, var 3 -> 1, 8 -> 2, else 3
  9 x FUN_1402f3130(fp, D_k, 0)  (each row " %20.10E" x then the ncomp values, 5 per line,
                                  continuation lines start with "\n" + 8 spaces; nothing when n0 < 1)
  " %20.10E %20.10E\n"         +0x14c8, +0x14c0
type 2:
  "%-6s %7i %7i %7i\n"         name, mat, 2, +0x14d0
  WREC(main @ +0x14d8)
  for list in (+0x16f8, +0x16e0, +0x16c8, +0x1710, +0x1728):
     " %7i\n"                  count (list +0x10)
     WREC(each record, list order)
  WREC(r) = FUN_1402f71f0:
     " %7i\n"                  n
     FUN_1402f2e80(fp, n, r+0x08, 5, 0)   (" %20.10E" each, wrap every 5, final "\n"; nothing if n < 1)
     " %7i %20.10E\n"          r+0x70, r+0x68
     if byte r+0x1e0 == 0: WSUB(r+0x78); WSUB(r+0xf0)
     WSUB(r+0x168)
  WSUB(D) = FUN_1402f7310:
     ndim 0:                  " %7i %20.10E\n" 0, const
     ndim 1, var0 3 / 23:     " %7i %7i\n" 1 / 2, n0       then FUN_1402f3130(fp, D, 0)
     ndim 2, var (3,23):      " %7i %7i %7i\n" 3, n0, n1   then FUN_1402f3130(fp, D, 0)
     anything else:           " %7i %20.10E\n" 0, 0.0
type 4:
  "%-6s %7i %7i\n"             name, mat, 4
  " %7i %7i %7i\n"             5, n0 (+0x175c), n1 (+0x1760)
  FUN_1402f3130(fp, +0x1740, 0)
type 5:
  "%-6s %7i %7i\n"             name, mat, 5
  " %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n"   +0x17b8 … +0x17e0
type 0, -1, anything else: nothing written.
```

Always returns 0.

Read/write asymmetries:

* type < 0 (stored as -1 and `+0x17ec`) and the raw types 3, ≥ 6 are not written: lost on a round
  trip.
* Type 1 codes: code 0 and unknown codes of D1/D2/D4/D6/D8 read as var 3 and are written back as
  1; code 3 of D3/D5/D7 is not a valid code (flag unchanged). Any var other than 3/8 would be
  written as 2 for D1/D2/D3/D5/D7, i.e. var 33 cannot be written for those (cannot arise from a
  read either).
* Type 1 with `n0 < 1`: header line written, body skipped; reread gives the same state.
* Type 4: the header always says `t = 5`. A KEY read with any other `t` (constant, 1-D, (3,12),
  (3,4)) is written with `5 n0 n1` and a body in its own ndim layout, which does not read back
  (e.g. a 1-D table gets `n1 = 0` and its rows are misparsed).
* Type 2 sub-function with `t ≥ 4` or `t < 0`: written as `" 0 0.0"` or by its stale shape.
* Type 2 trailing whitespace after `<i> <d>` (any text other than `"\n"`) makes the first
  sub-function read an empty/whitespace line (`t = -1`) and shifts everything after; this writer
  never produces it.

## Helpers

* `0x140384a50` - group 4 parser, case 0x10.
* `0x140413ee0` - `"%i %i"` line (keyword line, nine type-1 code lines).
* `0x14012fc70` - DYNARY re-init keeping +0x54/+0x60.
* `0x14041a800` - DYNARY body reader.
* `0x140414a40` - `"%Flf %Flf"` line (type-1 trailer).
* `0x14041bed0` - type-2 record reader (count, values, `%i %Flf`, sub-functions).
* `0x14041c070` - sub-function header line `"%i %Flf %Flf %Flf"` + body.
* `0x14029ccd0` - set record count with clamp to 12 and message.
* `0x14029cd30` / `0x14029cd20` / `0x1402f7580` - record: main flag (+0x1e0), count (+0x0), values (+0x8).
* `0x140413de0` - `"%i"` line with blank-line skip (list counts).
* `0x14028b240` - destroy all list records, zero head/tail/count.
* `0x14028b050` - allocate (0x1f0) and append a record, returns it.
* `0x14028aff0` - n-th (1-based) record of a list; `0x14029cd50` - list count (+0x10).
* `0x14028a4c0` / `0x14028a580` / `0x14028a530` / `0x14029cd60` - record ctor / init / dtor / delete.
* `0x14028b850` - sub-object ctor; `0x14028a140` (type-1 block), `0x14028b640` (type-2 block),
  `0x14028a3e0` (type-4 DYNARY, type 0xac), `0x14028a440` (six doubles).
* `0x14041b270` - generic "t n1 n2" header (type 4).
* `0x140415080` - six doubles line (type 5).
* `0x1401d0fc0` - state-variable layout refresh; `0x1401d0320` layout compute; `0x1401d64f0`
  group predicates; `0x1401d70c0` count per GRNDAT type; `0x140199650` per-object resize.
* `0x1403d3a90` - writer; `0x1402f3130` DYNARY body writer; `0x1402f71f0` record writer;
  `0x1402f7310` sub-function writer; `0x1402f2e80` double array writer.

## Corpus check

`grep -rl GRNDAT` over `DEFORM_corpus` finds 6 files, all type 1:

* `materials/IN718_6um[1650-2200F(900-1200C)]_s000006.KEY` — verified line by line above: exact.
* `materials/IN718_6um[1650-2200F(900-1200C)]_e000006.KEY` — same layout, EN units (D1 axis 1832,
  others 32, trailer `0 32`): exact.
* `key/2D/EN/HeatTreat/MRX_SRX_2D_EN.KEY`, `key/2D/SI/HeatTreat/MRX_SRX_2D_SI.KEY` — codes all 1,
  n = 1,2,1,1,2,1,4,2,1; bodies 5 / 2×7 / 8 / 7 / 2×8 / 7 / 4×10 / 2×7 / 4 numbers, trailer
  `0 1580` (EN) / `0 860` (SI): exact, next token BURGRS.
* `key/3D/EN/HeatTreatment/MRX_SRX_3D.KEY`, `key/3D/SI/HeatTreatment/MRX_SRX_3D.KEY` — n =
  1,2,1,1,1,1,4,2,1: exact.

Oracle round trips (original DEF_PRE, read + KFWRIT, `deform-rea-work/out/kw457/oracle/golden/`):
`IN718_6um[...]_s000006.OUT.KEY` GRNDAT block is byte-for-byte the write grammar above (header
`"GRNDAT       1       1"`, nine `"       1       1"` lines, rows `" %20.10E"` with 3-digit
exponents, wrap after 5 values with 8-space continuation, trailer `"    0.0000000000E+000    0.0000000000E+000"`);
`variants/2D__SI__HeatTreat__MRX_SRX_2D_SI.OUT.KEY` GRNDAT block is identical (`diff`) to the
corpus input. No ERROR/WARNING in the oracle stdout.

The MRX files use exactly this writer's `" %20.10E"` layout (6 numbers on the first row line,
continuation lines indented 8 spaces + field). Types 2, 4, 5 and negative types appear in no
corpus file (searched `^GRNDAT +[0-9]+ +<t>` for t = 2, 4, 5, -1, -2): those grammars are
unverified against data.

## Open questions

* Physical meaning of D1 (ncomp 4), of each coefficient position, of var codes 23 and 33, and of
  the two trailing doubles (the second behaves like a temperature).
* Meaning of type 2 (main record + five lists, `c` on the keyword line), type 4, type 5 and the
  number stored for negative types.
* `FUN_1401d71b0` / `FUN_1401d7250` (type-2 state-variable count) were not decompiled further.
