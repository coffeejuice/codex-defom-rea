# TTTD (id 135, group 5 phase transformation)

Transformation-kinetics data of the phase pair `from` → `to`. The second number on the keyword
line selects one of 13 data layouts (plus "negative"), recorded in the pair as an internal kind
`pair+0x14` (0…16); each kind owns its own storage in the pair, so several kinds can coexist in
memory but only the one selected by `+0x14` is written. Layouts: a TTT diagram (log time of a
start and a finish fraction tabulated over carbon / stress / temperature, KEY type 1), a
two-component table (accessor name "Start and 50% Temperatur table", KEY type 2), fixed sets of
11 / 6 / 7 / 4 coefficients (types 3, 5, 7, 6), 4 coefficients plus a temperature function
(type 4, accessor "Coefficient f(T)"), a temperature function (type 8), a "Phase Compositions"
table (type 9), an n-component vector (types 10–13) and a bare negative model number. The
physical models behind the numbered types are not named anywhere in DEF_PRE; meanings below
marked "inferred" come from the corpus values.

## Handlers
read: case 1 of `FUN_140383120`; write: `FUN_1403ccd00` (called from `FUN_1403cbf00` right after
PHASTF); accessor: `FUN_1403cdb20(modelData, label, {index}, out)`: finds the pair by 1-based
list index (`FUN_140339fe0`) and returns, for label
`"Log(Time)"` → `pair+0x78`, `"Start and 50% Temperatur table"` → `pair+0x1f8`,
`"Coefficient f(T)"` → `pair+0x2c8`, `"Phase Compositions"` → `pair+0xf8` (all DYNARYs), return 0;
any other label or missing pair → 3.

## KEY grammar (read)
Notation: `L(fmt)` = one line via the `_common_g457.md` §3 helpers (fgets, `,`→space, sscanf, short
line → missing values 0, EOF or blank line → helper returns 1 → parser returns 2);
`S(n)` = n doubles via one `fscanf` (`%Flf` × n, crosses line breaks, rest of the last line is
NOT consumed; only fscanf result -1 (EOF before the first value) gives 2); `BODY(d)` =
`FUN_14041a800(fp, d, 0)`.
```
line 1: TTTD <from:%i> <to:%i>                 fscanf "%i %i"
        <type:%i> <kind:%i>                    L("%i %i") on the rest of the line
        kind ∈ {0,1,2} → pair+0x10 = kind; other kind values leave +0x10 unchanged

type 1  (pair+0x14 = sub):
        L("%i %i %i")  <sub> <nc> <x>
        L("%Flf %Flf") <f1> <f2>
        sub 0..3 → +0x14 = sub, re-init DYNARY +0x78 to type 0x2c+sub (FUN_14012fc70)
        (other sub: +0x14 and the DYNARY's type/ndim/var are left as they were)
        +0x6c = nc ; DYNARY+0x78 ncomp (+0xb8) = nc ; +0xf0 = x
        if nc == 1:  +0x70 = f1, +0x170 = f2   else: +0x170 = f1, +0x178 = f2
        L("%i %i %i")  n0 n1 n2  → DYNARY+0x78 n[] (+0x94,+0x98,+0x9c)
        BODY(+0x78): axis0[n0], axis1[n1], axis2[n2], then nc·n0·n1·n2 values; rest of last line discarded
type 2  (+0x14 = 4):
        L("%i %i %i %i") <q> n0 n1 n2
        re-init DYNARY +0x1f8 to type 0x30 (ndim 3, var 12,15,16, ncomp 2); n[] = n0,n1,n2
        q != 2 → return 3 (body not read; its lines are then read as keywords)
        ncomp (+0x238) = 2 ; BODY(+0x1f8)
type 3  (+0x14 = 5):  S(11) → +0x270,+0x278,…,+0x2c0
type 4  (+0x14 = 6):
        L("%Flf"×4) → +0x340,+0x348,+0x350,+0x358
        L("%i") n → re-init DYNARY +0x2c8 to type 0x32 (ndim 1, var 3 = temperature), n0 = n
        BODY(+0x2c8): n rows "T value"
type 5  (+0x14 = 7):  S(6)  → +0x360 … +0x388
type 6  (+0x14 = 8):  L("%Flf"×4) → +0x390,+0x398,+0x3a0,+0x3a8   (a missing 4th value is 0)
type 7  (+0x14 = 9):  S(7)  → +0x3b0 … +0x3e0
type 8  (+0x14 = 10):
        L("%i") n → re-init DYNARY +0x3f8 to type 0x36 (ndim 1, var 3), n0 = n ; BODY(+0x3f8)
type 9  (+0x14 = 11):
        L("%i %i") <ignored> <nc>
        L("%Flf %Flf") <f1> <f2>
        re-init DYNARY +0xf8 to type 0x2c (ndim 3, var 12,6,3) ; +0x6c = nc ; ncomp (+0x138) = nc
        if nc == 1: +0x70 = f1, +0x170 = f2   else: +0x3e8 = f1, +0x3f0 = f2
        L("%i %i %i") → n[] (+0x114,+0x118,+0x11c) ; BODY(+0xf8)
type 10,11,12,13 (+0x14 = 12,13,14,15):
        L("%i") nc → ncomp of DYNARY +0x180 (+0x1c0); the DYNARY is NOT re-initialised
        (constructor type 0xb3: ndim 0) ; BODY(+0x180): if nc >= 2, nc values, rest of line discarded;
        nc <= 1 reads nothing
type < 0  (+0x14 = 16):  +0x470 = |type| ; nothing else read
type 0 or > 13: nothing stored except +0x10; returns 0 (silently ignored)
```
Return value: 0 if the last helper/body call returned 0, else 2 (plus the explicit 2/3 above).

## Model mapping
Phase pair (byte offsets; `pair` = 0x780-byte object).

| KEY field | offset | C type | DYNARY type / ndim / var / ncomp |
|---|---|---|---|
| kind (line 1) | +0x10 | int | 0,1,2 only (meaning unknown) |
| internal kind | +0x14 | int | 0–3 (type 1), 4 … 15, 16 (see grammar) |
| type 1/9 nc | +0x6c | int | selects where f1,f2 go; default 2 |
| f1 when nc==1 | +0x70 | double | |
| type 1 table | +0x78 | DYNARY | 0x2c/0x2e: ndim 3, var 12,6,3; 0x2d/0x2f: var 7,14,3; ncomp = nc |
| type 1 x | +0xf0 | int | unknown |
| type 9 table | +0xf8 | DYNARY | 0x2c, ncomp = nc (accessor "Phase Compositions") |
| type 10–13 vector | +0x180 | DYNARY | ctor type 0xb3: ndim 0, ncomp = nc |
| f1/f2 (nc≠1), f2 (nc==1) | +0x170, +0x178 | double | defaults 0.01, 0.99 |
| type 2 table | +0x1f8 | DYNARY | 0x30: ndim 3, var 12,15,16, ncomp 2 |
| type 3 coeffs | +0x270 … +0x2c0 | double[11] | |
| type 4 f(T) | +0x2c8 | DYNARY | 0x32: ndim 1, var 3 |
| type 4 coeffs | +0x340 … +0x358 | double[4] | |
| type 5 coeffs | +0x360 … +0x388 | double[6] | |
| type 6 coeffs | +0x390 … +0x3a8 | double[4] | |
| type 7 coeffs | +0x3b0 … +0x3e0 | double[7] | |
| type 9 f1,f2 (nc≠1) | +0x3e8, +0x3f0 | double | |
| type 8 f(T) | +0x3f8 | DYNARY | 0x36: ndim 1, var 3 |
| negative type | +0x470 | int | stored as abs value (`FUN_140484ab8`) |

KEY type → internal kind: 1→sub (0–3), 2→4, 3→5, 4→6, 5→7, 6→8, 7→9, 8→10, 9→11, 10→12,
11→13, 12→14, 13→15, <0→16.

Interpretation (inferred from corpus values; var 3 = temperature is confirmed in `_common_g457.md`):
- var 12 = carbon content (JIS-S45C: single point 0.44; PHEQVF axis 0 / 0.77 / 0.78), var 6 =
  stress (axis −300, 0, 300). Type 1 values are log10(time) (accessor "Log(Time)"); `nc = 2`
  components with `f1, f2` = the two transformed fractions (0.01/0.99 or 0.001/0.999), i.e. the
  start and finish curves of a TTT diagram tabulated over (carbon, stress, temperature).
- Type 2 (accessor "Start and 50% Temperatur table"): e.g. `Induction_Heating_Example3` 1→3
  holds over carbon 0.2…1.0 the pairs 400/370, 375/315, 250/187.5, 219/169: martensite start and
  50% temperatures vs carbon.
- Type 5 (6 coefficients, e.g. `0.016 0 -0.0012237 -0.0010303 -5.18 0.45`) is used for
  austenite→martensite pairs; type 6 (4 values, e.g. `-4 2 722 816`, in EN files `1331.6 1500.8`
  = 722/816 °C in °F) for →austenite pairs: the last two look like Ae1/Ae3 temperatures.
- Type 3 (11 coefficients, e.g. `3.05e-5 150 200 6 550 200 6 0.0030591 -33.44 0.14 4`) is a
  closed-form kinetics model; no names in the binary.

## Defaults
Constructor `FUN_140333f60`: +0x10 = 0, +0x14 = 0 (→ written as type 1 / sub 0), +0x6c = 2,
+0x70 = 0, +0xf0 = 0, +0x170 = 0.01 (`DAT_14051f258`), +0x178 = 0.99 (`DAT_1405501c8`),
+0x3e8 = +0x3f0 = 0, all coefficient arrays 0, +0x470 = 0; DYNARYs: +0x78 type 0x2c,
+0xf8 type 0x37 (ndim 3, var 12,6,3, ncomp 2), +0x180 type 0xb3 (ndim 0, ncomp 1), +0x1f8 type
0x30, +0x2c8 type 0x32, +0x3f8 type 0x36 (all with n = 0). The reader resets only the DYNARY it
re-inits (see grammar); other kinds' data are kept.

## Validation, errors, return codes
No string literals. 2: EOF before the pair numbers; EOF/blank line in any `L(...)` line; `S(n)`
fscanf returning -1; body EOF. 3: type 2 with first value != 2. Silently accepted: type 0 and
type > 13 (no-op), unknown `kind`, unknown `sub` (type 1 data go into the existing DYNARY
without changing +0x14), conversion failures inside `S(n)` and bodies (value stays 0, stream
does not advance).

## Unit conversion
none found

## KEY grammar (write)
`FUN_1403ccd00(fp, modelData, {index})`, pair = `FUN_140339fe0(list, index)`. Header line for all
kinds: `"%-6s %7i %7i %7i %7i\n"` "TTTD", from, to, T, pair+0x10, with T and the body by `+0x14`:

| +0x14 | T | following lines |
|---|---|---|
| 0, 2, 3 | 1 | `" %7i %7i %7i\n"` +0x14, +0x6c, +0xf0 ; `" %20.10E %20.10E\n"` (+0x70,+0x170) if +0x6c==1 else (+0x170,+0x178) ; `FUN_1402f3130(+0x78, mode 1)` |
| 1 | 1 | same, first int written as literal 1 |
| 4 | 2 | `" %7i %7i %7i %7i\n"` ncomp(+0x238), n0, n1, n2 ; `FUN_1402f3130(+0x1f8, mode 0)` |
| 5 | 3 | 11 doubles via `FUN_1402f2e80(fp, 11, …, 5, 0)` (5 per line) |
| 6 | 4 | `" %20.10E %20.10E %20.10E %20.10E\n"` +0x340…+0x358 ; `FUN_1402f3130(+0x2c8, mode 1)` (writes `" %7i\n"` n0, then rows) |
| 7 | 5 | 6 doubles via `FUN_1402f2e80(…, 6, …, 5, 0)` |
| 8 | 6 | `" %20.10E %20.10E %20.10E %20.10E\n"` +0x390…+0x3a8 |
| 9 | 7 | 7 doubles via `FUN_1402f2e80(…, 7, …, 5, 0)` |
| 10 | 8 | `" %7i\n"` n0 of +0x3f8 ; `FUN_1402f3130(+0x3f8, mode 0)` |
| 11 | 9 | `" %7i %7i\n"` 2, +0x6c ; doubles (+0x70,+0x170) if +0x6c==1, (+0x3e8,+0x3f0) if ==2, else (+0x170,+0x178) ; `FUN_1402f3130(+0xf8, mode 1)` |
| 12–15 | 10–13 | `" %7i\n"` ncomp of +0x180 ; `FUN_1402f3130(+0x180, mode 6)` |
| 16 | −\|+0x470\| | nothing |
| other | — | return 3 (aborts the KEY writer; unreachable from the reader) |

Mode 1 for the 3-D tables writes the n[] line (`FUN_1402f2fe0`, `" %7i"` ×3), then each axis on
its own line(s) and the values wrapped every max(n) values (continuation lines start with 8
spaces), as in `_common_g457.md` §4. Mode 0 for kind 4 writes no n[] line (the counts are on the
previous line) and only when all n ≥ 1. The return codes of `FUN_1402f3130` / `FUN_1402f2e80`
are ignored (always returns 0 after a known kind); e.g. a never-read pair (kind 0, n = 0) writes
`0 2 0` / `1.0E-002 9.9E-001` / `0 0 0` and then nothing (perLine = max n = 0 → `FUN_1402f2e80`
returns 2 without output), which reads back identically.

Read/write asymmetries:
- types 10–13 with nc == 1: mode 6 writes the constant `" %20.10E\n"` but the reader reads no
  value (ndim 0, ncomp < 2) → an extra line that the interpreter then reports as an unknown
  keyword ("WARNING: Keyword %s not found. Skip %s").
- type 9 with nc ∉ {1, 2}: read into +0x3e8/+0x3f0, written from +0x170/+0x178.
- type 9 first int is written as 2 (ignored on read). Type 1 third int `x` round-trips.
- `kind` values other than 0–2 are not stored; the old +0x10 is written.
- type 1 with an unknown `sub`: data go into the old kind's DYNARY; output depends on the old
  +0x14.
- type 0 / > 13 in a file: the previous kind is written (default: type 1 sub 0, empty table).

## Helpers
- `0x140383120` - group-5 parser (case 1)
- `0x1403ccd00` - TTTD KEY writer
- `0x1403cdb20` - TTTD accessor
- `0x140413ee0`, `0x140413fb0`, `0x140414100`, `0x140413de0`, `0x140414a40`, `0x140414e40` - line helpers
- `0x14012fc70` - DYNARY re-init keeping +0x54/+0x60
- `0x14041a800` - DYNARY body reader
- `0x1402f3130`, `0x1402f2e80`, `0x1402f2fe0` - DYNARY / array writers
- `0x140484ab8` - abs()
- `0x140339fe0` - pair by 1-based index

## Corpus check
Simulated reader (`g5sim.py`, scratch tool) over every file in `key/**` and `materials/`
(`.KEY` and `.MST`): 168 TTTD instances in 24 files, all consumed exactly (next token is always
PHASLH): type 1 (sub 0, nc 2, `f1 f2` = 0.01/0.99 or 0.001/0.999, n = (1,1,3…15), (1,3,15),
(4,1,27); kind 0/1/2), type 2 (`2 4 1 1`, Induction_Heating_Example3), type 3 (HT_CARB1 etc.,
11 values on 3 lines), type 5 (6 values on 2 lines; kind 0/2), type 6 (4 values, kind 0/1).
Types 4, 7–13 and negative types do not occur. `materials/JIS-S45C(HeatTreatment)_s000006.KEY`:
1→2 type 1 (n 1,3,15: 90 values), 1→3 type 5, 2→1 and 3→1 type 6; `_e000006` the same in °F
(pairs 12→13 …). Oracle golden outputs (`oracle/golden/JIS-S45C(HeatTreatment)_s000006.OUT.KEY`,
`variants/2D__SI__InductionHeating__Induction_Heating_Example3_SI.OUT.KEY`,
`variants/2D__SI__HeatTreat__HT_CARB1_SI.OUT.KEY`) match the write table byte for byte for types
1 (incl. the `0 2 0` line, n line, 15-per-line wrapping with 8-space continuation), 2
(`2 4 1 1` line, axes 4/1/1, values wrapped at 4), 3 (5+5+1 values), 5 (5+1) and 6. Lines end
in CRLF (text-mode file).

## Open questions
- Meaning of `kind` (+0x10), of `x` (+0xf0), of var 14, 15, 16 and of all coefficient sets.
- Value order of the 3-D tables (which axis varies fastest) is up to the consumer; the reader
  stores file order.
- Whether types 10–13 with nc == 1 are ever produced by the GUI (writer/reader mismatch).
