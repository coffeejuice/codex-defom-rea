# FSTRES (id 94, group 4 material)

Flow-stress definition of a material. It is stored in the material sub-object at `+0x50`
(constructor `FUN_140288ae0`), which holds an internal *mode* int and one data block per mode:
coefficient sets of 4, 5, 6, 7+7 or 4 doubles, a 3-D table f(strain, strain rate, temperature)
(label `"Flow Stress-f(Stn,Str,Temp)"`), a pair of temperature/var-12 functions labelled
`"Coefficient (Y)"` and `"Coefficient (H)"`, a 3-D table f(strain, var 12, temperature)
(`"Strain-Atm-Temp"`), an `"Initial Flow Stress"` function, four constants plus a
`"Bird-Mukherjee-Dorn n"` function, an N-variable table (three DYNARYs), a 3-D table with several
values per point (`"Strain-StrainRate-Temp"`), or a user-routine number (negative KEY type; the
interactive prompt says `"Enter Flow Stress Definition Method:  N for Function N; -N for Routine N"`).
Which formula each coefficient set implements is **not** in this reader/writer; the model names
below are taken from accessor labels where they exist and are otherwise marked *inferred* from the
corpus data shape.

The two machining material files the user cares about (`AISI4340-JohnsonCook_*.KEY`,
`AISI-1020(Machining)_*.KEY`) both use **KEY type 2** (3-D table). They contain no coefficient
form of Johnson-Cook or Oxley; the models were tabulated before the KEY was written (see
"Corpus check").

## Handlers

* read: case 2 of `FUN_140384a50` (material number read first by the group parser).
* write: `FUN_1403d0c90`, called from `FUN_1403cdca0` (order … FRAE2H FPERV FRCMOD **FSTRES** YOUNG …).
* accessor: `FUN_1403d1c50(model, label, {mat}, out)`; for an existing material it sets `*out`
  and returns 0 for these labels (any mode, no mode check), else returns 3:

| label | `*out` |
|---|---|
| `"Bird-Mukherjee-Dorn n"` | mat+0x388 |
| `"Coefficient (Y)"` | mat+0x1b0 |
| `"Coefficient (H)"` | mat+0x138 |
| `"Initial Flow Stress"` | mat+0x400 |
| `"Flow Stress-f(Stn,Str,Temp)"` | mat+0x78 |
| `"Strain-StrainRate-Temp"` | mat+0x5e8 |
| `"Strain-Atm-Temp"` | mat+0x228 |

## KEY grammar (read)

Notation as in `_common_g457.md`: `LINE(fmt)` = one-line helper (fgets + `,`→space + sscanf, missing
numbers 0); `DYN(D)` = `FUN_14041a800` body (whitespace tokens, discards the rest of the last
line when the value count > 0); `fc70(D,t)` = `FUN_14012fc70(D, t)`. `M` = internal mode int at
mat+0x50.

```
L1: FSTRES <mat:%i> <t:%i> <s1:%i> <s2:%i>     LINE("%i %i %i") FUN_140413fb0  (s1,s2 optional, 0)
    EOF/blank -> return 2

t == 1  : M=0;  L2 LINE(4 x %Flf) FUN_140414e40 -> c[0..3] at +0x58,+0x60,+0x68,+0x70
t == 2,3: M=1;  fc70(T,0x4f)  [T = +0x78: ndim 3, var 7,8,3, ncomp 1]
          L2 LINE("%i %i %i") -> n0,n1,n2 (+0x94,+0x98,+0x9c)          EOF -> return 2
          DYN(T): n0 strain values, n1 strain-rate values, n2 temperatures, n0*n1*n2 values
          flag +0xc8 = (t==2 ? 1 : 0)        (set even when DYN failed; return code = DYN result)
t == 4  : M=2;  L2 LINE(5 x %Flf) FUN_140414f50 -> +0xf0,+0xf8,+0x100,+0x108,+0x110
t == 5  : M=3;  L2 LINE(4 x %Flf) -> +0x118,+0x120,+0x128,+0x130
t == 6  : M=4;
          L2 LINE("%Flf %Flf") FUN_140414a40 -> a,b                   EOF -> return 2
          fc70(Y,0x24)  [Y = +0x1b0]; by s1:
             0: ndim 0, const = a
             1: ndim 1, var 3,  n0=(int)a
             2: ndim 1, var 12, n0=(int)a
             3: ndim 2, var 3,12, n0=(int)a, n1=(int)b
             other: left as re-initialised (ndim 0, const 0); a,b ignored
          DYN(Y)                                                      fail -> return 2
          L3 LINE("%Flf %Flf") -> a,b                                 EOF -> return 2
          fc70(H,0x25)  [H = +0x138]; same 0..3 table by s2
          DYN(H)                                       (its result is the return code)
t == 7,8: M=5;  fc70(S,0x50)  [S = +0x228: ndim 3, var 7,12,3, ncomp 1]
          L2 LINE("%i %i %i") -> n0,n1,n2 (+0x244,+0x248,+0x24c)      EOF -> return 2
          DYN(S); flag +0x278 = (t==7 ? 1 : 0)
t == 9  : M=6;  L2 LINE(7 x %Flf) FUN_1404151c0 -> +0x2a0..+0x2d0 (7 doubles, step 8)
                L3 LINE(7 x %Flf)                -> +0x2d8..+0x308
                (only L3's status is returned; EOF on L2 alone is ignored)
t == 10 : M=7;  L2 LINE(7 x %Flf) -> +0x310..+0x338 (6 doubles); the 7th number is read into a
                local and DISCARDED
t == 11 : M=8;  L2 LINE(5 x %Flf) -> +0x340,+0x348,+0x350,+0x358,+0x360
t == 12 : M=9;
          L2 LINE("%Flf %Flf") -> a,b                                 EOF -> return 2
          fc70(F,0x26)  [F = +0x400]; by s1:
             0: ndim 0, const = a
             1: ndim 1, var 3, n0=(int)a
             2: ndim 1, var 8, n0=(int)a
             3: ndim 2, var 3,8, n0=(int)a, n1=(int)b
             other: ndim 0, const 0
          DYN(F)                                                      fail -> return 2; else 0
t == 13,15: M = (t==13 ? 10 : 12)
          L2 LINE("%i") FUN_140413de0(skipBlank=0) -> cnt (+0x478)    EOF -> return 2
          fc70(A,0xa6) [A = +0x480: ndim 1, var 0, ncomp 2]; n0 (+0x49c) = cnt
          DYN(A): cnt rows "<k_i> <p_i> <q_i>"                         fail -> return 2
          sum  = Σ (int)k_i ;  prod = Π (int)k_i   (int arithmetic, i < cnt)
          fc70(B,0xa7) [B = +0x4f8, ndim 0]; ncomp (+0x538) = sum
          DYN(B): sum values (read only if sum >= 2, see below)      fail -> return 2
          fc70(C,0xa8) [C = +0x570, ndim 0]; ncomp (+0x5b0) = prod
          DYN(C): prod values (only if prod >= 2)                     fail -> return 2; else 0
t == 14 : M=0xb; L2 LINE(4 x %Flf) -> +0x368,+0x370,+0x378,+0x380   (status ignored)
          L3 FUN_14041b270(fp, N, 0, NULL) [N = +0x388]: generic "t a b c" property line + body
             (_common_g457.md §4: t 0 const a; 1 var 3; 2 var 9; 3 var 12; 4 (3,12); 5 (3,8);
              6 var 8; 7 (3,4); N is re-initialised to DYNARY type 0 first)
t == 16 : M=0xd; fc70(P,0x51) [P = +0x5e8: ndim 3, var 7,8,3, ncomp 1, no allocation]
          L2 LINE("%i") -> ncomp of P (+0x628)                        EOF -> return 2
          L3 LINE("%i %i %i") -> n0,n1,n2 (+0x604,+0x608,+0x60c)      EOF -> return 2
          DYN(P): axes, then ncomp*n0*n1*n2 values; then +0x638 = 0
t <  0  : M=0xe; +0x660 = -t        (no further lines)
t == 0  : M=-1                       (no further lines)
t >= 17 : nothing stored, no further lines, return 0
```

Return: the group parser returns 2 when the last helper status (`local_b8`) is non-zero, else 0.

Short lines are accepted (missing numbers 0). In 2-D/3-D bodies the axes and values are plain
whitespace-separated tokens, so line breaks are free; after the last value the rest of that line
is discarded. For `DYN` on an ndim-0 DYNARY with `ncomp < 2` nothing is read (so for t 13/15 a
`sum` or `prod` of exactly 1 reads **no** value, and that value stays in the file to be read as the
next keyword token).

## Model mapping

Material offsets in hex bytes (sub-object offset = material offset − 0x50).

| KEY t | M (+0x50) | data | offsets | DYNARY type / ndim / var / ncomp | model |
|---|---|---|---|---|---|
| 0 | -1 | none | | | none ("no flow stress", inferred) |
| 1 | 0 | 4 doubles | +0x58 +0x60 +0x68 +0x70 | | inferred σ = c0·ε^c1·ε̇^c2 + c3 (corpus: `784 0.1 0.1 0`, `121 0 0.115 0`, `100 1 0 400`) |
| 2 | 1 | table T, flag +0xc8 = 1 | T +0x78 (ndim +0x7c, var +0x88..+0x90, n +0x94..+0x9c, axes +0xa0/+0xa8/+0xb0, ncomp +0xb8, values +0xc0) | 0x4f / 3 / 7,8,3 / 1 | tabular σ(strain, strain rate, temperature), label "Flow Stress-f(Stn,Str,Temp)" |
| 3 | 1 | same, flag +0xc8 = 0 | | | same table; the meaning of the flag (T+0x50, the DYNARY's "cleared on init" int) is not visible here (inferred: interpolation choice) |
| 4 | 2 | 5 doubles | +0xf0 … +0x110 | | inferred hyperbolic-sine (Sellars-Tegart) law: corpus rows `α Q A n R` = `0.045 1.53E5 2.385E8 2.976 8.314` |
| 5 | 3 | 4 doubles | +0x118 … +0x130 | | unknown |
| 6 | 4 | Y, H | Y +0x1b0 (ndim +0x1b4, const +0x1b8, var +0x1c0/+0x1c4, n +0x1cc/+0x1d0); H +0x138 (ndim +0x13c, const +0x140, var +0x148/+0x14c, n +0x154/+0x158) | Y 0x24, H 0x25; per s: 0 / 1 var 3 / 1 var 12 / 2 (3,12); ncomp 1 | labels "Coefficient (Y)", "Coefficient (H)"; inferred σ = Y + H·ε (corpus Y ≈ 25 ksi, H ≈ 600 ksi vs T) |
| 7 | 5 | table S, flag +0x278 = 1 | S +0x228 (n +0x244..+0x24c) | 0x50 / 3 / 7,12,3 / 1 | label "Strain-Atm-Temp" |
| 8 | 5 | same, flag +0x278 = 0 | | | |
| 9 | 6 | 14 doubles | +0x2a0 … +0x308 | | unknown |
| 10 | 7 | 6 doubles (7th read, dropped) | +0x310 … +0x338 | | unknown |
| 11 | 8 | 5 doubles | +0x340 … +0x360 | | unknown (corpus: `417.56 0 0.3789 0.0122 0` SI = `60.56 …` EN, so field 1 is a stress) |
| 12 | 9 | F | F +0x400 (ndim +0x404, const +0x408, var +0x410/+0x414, n +0x41c/+0x420) | 0x26; per s1: 0 / 1 var 3 / 1 var 8 / 2 (3,8) | label "Initial Flow Stress" |
| 13 | 10 | cnt, A, B, C | cnt +0x478; A +0x480 (n0 +0x49c, axis0 ptr +0x4a8, values ptr +0x4c8); B +0x4f8 (ncomp +0x538, values +0x540); C +0x570 (ncomp +0x5b0, values +0x5b8) | A 0xa6 / 1 / 0 / 2; B 0xa7 / 0; C 0xa8 / 0 | inferred N-variable table: k_i = points of variable i, B = all axes concatenated, C = Π k_i values; meaning of p_i, q_i unknown |
| 14 | 0xb | 4 doubles + N | +0x368 … +0x380; N +0x388 (ndim +0x38c, const +0x390, n0 +0x3a4) | N re-init to type 0 by the generic header | Bird-Mukherjee-Dorn (label "Bird-Mukherjee-Dorn n": N is the exponent n) |
| 15 | 0xc | as 13 | as 13 | | same layout as 13, different mode |
| 16 | 0xd | P | P +0x5e8 (n +0x604..+0x60c, ncomp +0x628, +0x638 = 0) | 0x51 / 3 / 7,8,3 / ncomp from file | label "Strain-StrainRate-Temp" (several values per point; meaning unknown) |
| <0 | 0xe | routine no. | +0x660 = −t | | user routine N |

Var codes: 7 strain, 8 strain rate, 3 temperature (`_common_g457.md`; also `FUN_140288f80` takes the
temperature range of the flow-stress data from T.axis2, Y/H.axis0 and S.axis2 when their var is
3); 12 inferred carbon content (the corpus 2-D Y/H tables have axis 2 = `0.2 1.17`).

The value order inside 3-D tables is file order (`memcpy`); the corpus data are consistent with
the first axis (strain) varying fastest, then strain rate, then temperature.

Side effects: only the selected mode's block is (re)initialised; the blocks of other modes keep
their data. A KEY with several FSTRES lines for one material keeps the last mode.

## Defaults

Sub-object constructor `FUN_140288ae0`: M = 1 (table, flag +0xc8 = 0 → written as KEY type 3);
T type 0x4f with n = 0,0,0; all coefficient doubles 0 (+0x58…+0x77, +0xf0…+0x137,
+0x2a0…+0x387); H 0x25, Y 0x24, S 0x50, N 0xae (ndim 0, const **1.0**), F 0x26, A 0xa6, B 0xa7,
C 0xa8, P 0x51; cnt +0x478 = 0; +0x660 = 0. The reader resets only the DYNARYs of the selected
branch (`FUN_14012fc70`, which keeps DYNARY +0x54/+0x60).

## Validation, errors, return codes

* Return 2: EOF or blank L1; EOF of L2/L3 in branches 2/3, 6, 7/8, 12, 13/15, 16; any `DYN`
  returning 1 (EOF inside a body) in branches 6 (Y), 12, 13/15; and, through the final
  `local_b8` check, any non-zero last status in every branch.
* t 9: only the second 7-number line's status counts; t 14: only the generic-header/body status
  counts.
* Unknown `t >= 17` and `t == 0` are accepted silently (return 0).
* Nothing returns 3 on read. No string is printed by the reader.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d0c90`, `name` = `FUN_1402f7720(0x5e)` = `"FSTRES"`; `E` = `" %20.10E"`; bodies via
`FUN_1402f3130` (`_common_g457.md` §4). Switch on M:

```
M 0 : "%-6s %7i %7i\n" name,mat,1 ; " %20.10E %20.10E %20.10E %20.10E\n" +0x58,+0x60,+0x68,+0x70
M 1 : flag +0xc8 ==1 -> "%-6s %7i %7i\n" name,mat,2 ; ==0 -> same with 3 ; other -> return 3
      FUN_1402f3130(T, mode 1): " %7i %7i %7i\n" n ; axis0 / axis1 / axis2 each wrapped every
      max(n) ; values wrapped every max(n) (continuation lines: "\n" + 8 spaces)
M 2 : header t=4 ; " %20.10E" x5 "\n" +0xf0..+0x110
M 3 : header t=5 ; " %20.10E" x4 "\n" +0x118..+0x130
M 4 : sY = Y.ndim0 -> 0 | ndim1&var3 -> 1 | ndim1&var12 -> 2 | ndim2&var(3,12) -> 3 | else return 3
      sH likewise from H
      "%-6s %7i %7i %7i %7i\n" name,mat,6,sY,sH
      FUN_1402f3130(Y, 1); FUN_1402f3130(H, 1)
        (mode 1: ndim 0 -> " %20.10E\n" const; ndim 1 -> " %7i\n" n0 then rows "E x E v";
         ndim 2 -> " %7i %7i\n" n0 n1, axis0, axis1, values)
M 5 : flag +0x278 ==1 -> header t=7 ; ==0 -> t=8 ; other -> return 3 ; FUN_1402f3130(S, 1)
M 6 : header t=9 ; " %20.10E" x7 "\n" +0x2a0..+0x2d0 ; " %20.10E" x7 "\n" +0x2d8..+0x308
M 7 : header t=10 ; " %20.10E" x6 "\n" +0x310..+0x338
M 8 : header t=11 ; " %20.10E" x5 "\n" +0x340..+0x360
M 9 : s = F.ndim0 -> 0 | ndim1&var3 -> 1 | ndim1&var8 -> 2 | ndim2&var(3,8) -> 3 | else return 3
      "%-6s %7i %7i %7i %7i\n" name,mat,12,s,0      (5th field is a literal 0, checked in asm)
      FUN_1402f3130(F, 1)
M 10, M 0xc : header t=13 (M 10) / 15 (M 0xc)
      +0x478 = A.n0 (writer modifies the model) ; " %7i\n" A.n0
      for i < A.n0: " %7i %7i %7i\n" (int)axis0[i], (int)A.values[2i], (int)A.values[2i+1]
      k = 0; for i < A.n0: for j < (int)axis0[i]: "%15.7E" B.values[k++] ; then "\n"
      FUN_1402f3130(C, 1): ncomp==1 -> " %20.10E\n" C.const ; ncomp>=2 -> values 5 per line
M 0xb: header t=14 ; " %20.10E" x4 "\n" +0x368..+0x380
      N.ndim == 0 -> " %7i %20.10E\n" 0, N.const
      else        -> " %7i %7i\n" 6, N.n0 ; FUN_1402f3130(N, 0)   (rows only if all n >= 1)
M 0xd: header t=16 ; " %7i\n" P.ncomp (+0x628) ; FUN_1402f3130(P, 1)
M 0xe: "%-6s %7i %7i\n" name, mat, -(+0x660)
other (M = -1 from KEY t 0, or any other value): return 3 -> "Data Error", KEY writer aborts
```

Header for M 0..3, 5..8, 0xb, 0xd: `"%-6s %7i %7i\n"`. The body-writer return value is ignored
everywhere (so the default material, M 1 with n = 0, writes `FSTRES m 3` and
`"       0       0       0\n"` and nothing else, because `FUN_1402f2e80` refuses perLine 0).

Read/write asymmetries:
* KEY t 0 reads fine but cannot be written (return 3 aborts the whole KEY write).
* t 10: the 7th number is lost.
* t 13/15: B values are written with `"%15.7E"` (7 decimals, no separating space beyond the
  field width) → precision loss; p_i, q_i are written as truncated ints. If `sum == 1` or
  `prod == 1` the writer emits a value (`prod == 1`: C.const, which the reader never set) that the
  reader will not consume.
* t 14: any non-constant N is written with generic type 6 (var 8, strain rate) whatever it was
  read as (t 1/2/3 change var; 2-D t 4/5/7 are written as "6 n0" followed by a 2-D body without
  n1, which does not read back).
* t 12 with s1 outside 0..3, t 6 with s outside 0..3: read as constant 0, written as s = 0.
* Otherwise (t 1–9, 11, 12, 16, <0) the round trip is exact apart from float formatting.

## Helpers

* `0x140413fb0` - `"%i %i %i"` line (L1, table sizes).
* `0x140413de0` - `"%i"` line (t 13/15 count, t 16 ncomp).
* `0x140414a40` / `0x140414e40` / `0x140414f50` / `0x1404151c0` - 2 / 4 / 5 / 7 doubles line.
* `0x14041b270` - generic property header + body (t 14).
* `0x14041a800` - DYNARY body reader; `0x14012fc70` - DYNARY re-init.
* `0x1402f3130`, `0x1402f2e80`, `0x1402f2fe0` - body writers.
* `0x140288ae0` - sub-object constructor; `0x140288f80` - temperature range of the flow-stress
  data (M 1 / 4 / 5); `0x1402894b0` - counts DYNARYs per mode (M 1, 4, 5, 9, 10/0xc, 0xb, 0xd).
* `0x1402f7720` - keyword name.

## Corpus check

`grep -rah "^FSTRES "` over `key/` and `materials/`: 740 instances; t 2 (398), 1 (178), 6 with
s 1 1 (78), 3 (60), 6 with s 3 3 (14), 4 (8), 11 (4). t 5, 7–10, 12–16 and negative t do not occur.
Checked by hand / by token count:
* `materials/AISI4340-JohnsonCook_s000008.KEY`, `_e000008.KEY` (t 2, n 8 7 9 → 24 axis + 504
  values; consumed exactly up to `YOUNG`) and `materials/AISI-1020(Machining)_s000008.KEY`,
  `_e000008.KEY` (t 2, n 6 5 7 → 18 + 210; exact). Both machining materials therefore go
  through the M 1 branch above (lines L1, L2, DYN(T)); verified line by line against the code.
* `key/2D/EN/Creep/CREEP_Example1_EN.KEY` (t 3, n 2 1 2), `key/2D/EN/HeatTreat/HT_CRACK_EN.KEY`
  (t 6 1 1: `20` + 20 rows, `21` + 21 rows), `key/2D/EN/InductionHeating/Induction_Heating_Example3_EN.KEY`
  (t 6 3 3: `19 2`, 19 + 2 axes, 38 values, twice), `key/3D/EN/Extrusion/ALE/European_Benchmark_2005_ALE.KEY`
  (t 4), `key/2D/EN/Rolling/2D_Bimetalic_Rolling_Uniform_Upper_and_Lower_Roll_RPM_EN.KEY` (t 11),
  `key/2D/EN/Forging/RING_EN.KEY` (t 1): all consumed exactly.
* The reference reader `tools/kwcheck457.py` reads all 740 instances without failure and agrees
  with this grammar.
* Oracle (original DEF_PRE KFWRIT): a Python emulation of this read + write grammar reproduces
  the FSTRES blocks of `oracle/golden/*.OUT.KEY` (8 material files: t 1, 2, 6 1 1) and
  `oracle/golden/variants/` (t 1, 2, 3, 4, 6, 11) **byte for byte** (three-digit exponents,
  wrap every max(n) with 8-space continuation, 12 spaces before continuation numbers).

Data shape of the two machining tables (not code evidence):
* AISI4340-JohnsonCook: exactly multiplicatively separable. Rate factor relative to 0.01/s is
  1.03445, 1.06891, … per decade = (1 + C ln ε̇)/(1 + C ln 0.01) with **C = 0.014**, ε̇0 = 1/s;
  temperature factor 1 − (T − 20)/1500 for T ≤ 700 °C (m = 1, T_melt − T_room = 1500 °C);
  strain part A + B ε^n with A ≈ 794.7 MPa, B ≈ 511.7 MPa, n ≈ 0.26 (reproduces 1470.88 at ε = 5).
  That is the Johnson-Cook law tabulated; the last temperature row (1370 °C) does not follow it
  (factor 0.19–0.38).
* AISI-1020(Machining): σ = σ1(ε̇,T)·ε^n(ε̇,T) holds exactly for ε ≥ 0.01 at every (ε̇,T), with the
  ε = 0 entry equal to the power law at ε ≈ 0.0076 and n from 0.044 to 0.18; rate and temperature
  are strongly coupled (not separable; some rate ratios < 1); rows T = 1200 and 1300 °C are
  identical. Consistent with an Oxley-type machining model (σ1, n functions of a velocity-modified
  temperature) — **inferred**; a simple Tmod = T(1 − ν log ε̇) collapse with one ν was not found.

## Open questions

* Meaning of flag +0xc8 (t 2 vs 3) and +0x278 (t 7 vs 8) — set here, used elsewhere.
* Formulas of t 1, 4, 5, 9, 10, 11, 14 and the 6/7th coefficient roles (no evaluator in this code
  path; t 1 and t 4 only inferred from corpus values).
* Meaning of p_i, q_i in t 13/15 rows and of the extra components in t 16.
* Difference between t 13 and t 15 (only M differs).
