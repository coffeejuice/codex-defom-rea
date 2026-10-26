# DIFCOE (id 107, group 4 material)

Diffusion coefficient of a material (name-based, inferred; the corpus non-zero tables come from
induction-heating/carburizing-type examples). Stored in the material sub-object at `+0x950`
(constructor `FUN_140289930`) as a mode int plus three alternative DYNARYs: a constant or a 2-D
table f(temperature, var 12), a 1-D table f(temperature) with 2 values per point, a 1-D table
f(var 12) with 2 values per point, or (negative type) a reference number (inferred: a user routine
/ external definition number). Var 12 values in the corpus are 0.11…1.47 (inferred: carbon
content).

## Handlers

* read: case `0xf` of `FUN_140384a50`.
* write: `FUN_1403cfca0`, called by `FUN_1403cdca0` after CREEP (order: … MSTMTR CREEP DIFCOE ELRST …).
* accessor: `FUN_1403cff00(model, label, {mat}, out)`: label ignored; mode 1 → `*out = mat+0x958`,
  mode 2 → `mat+0x9d0`, mode 3 → `mat+0xa48`, other modes leave `*out` unchanged; returns 0
  (also when the material does not exist).

## KEY grammar (read)

```
L: DIFCOE <mat:%i> <t:%i> <a:%Flf> <b:%Flf> <c:%Flf> <d:%Flf>   (rest of line FUN_140415990 "%i %Flf x4")
t == 0:  mode 0; FUN_14012d2a0(D0, 0) (full re-init, type 0, ndim 0); D0.const = a   (no body)
t == 1:  mode 1; FUN_14012fc70(D0, 0x29) -> ndim 2, var 3, 12; n0 = (int)a, n1 = (int)b
         DYN(D0): axis0 (n0 values), axis1 (n1 values), n0*n1 values
t == 2:  mode 2; FUN_14012fc70(D2, 0x27) -> ndim 1, var 3, ncomp 2; n0 = (int)a
         DYN(D2): n0 rows "<x> <v1> <v2>"
t == 3:  mode 3; FUN_14012fc70(D3, 0x28) -> ndim 1, var 12, ncomp 2; n0 = (int)a
         DYN(D3): n0 rows "<x> <v1> <v2>"
t <  0:  mode 4; +0xac0 = -t                       (no body)
t >= 4:  nothing changed, no body, returns 0
```

`c`, `d` are read and ignored. Short header line: missing numbers are 0. Bodies are
whitespace-token based (`_common_g457.md` §4), the rest of the last body line is discarded.

## Model mapping

| KEY field | Object / offset | C type | Notes |
|---|---|---|---|
| t → mode | material +0x950 | int | 0, 1, 2, 3, 4 (4 = negative t) |
| a (t 0) | +0x960 (= D0 +0x08) | double | D0 re-initialised to DYNARY type 0, ndim 0 |
| table (t 1) | D0 at +0x958 | DYNARY 0x29 | ndim 2, var (3, 12), n = (a, b), ncomp 1 |
| table (t 2) | D2 at +0x9d0 | DYNARY 0x27 | ndim 1, var 3, ncomp 2, n0 = a |
| table (t 3) | D3 at +0xa48 | DYNARY 0x28 | ndim 1, var 12, ncomp 2, n0 = a |
| -t (t < 0) | +0xac0 | int | |

n0 at +0x974 / +0x9ec / +0xa64, n1 at +0x978. Side effects: none beyond the new-material
bookkeeping (`_common_g457.md` §5).

## Defaults

Constructor `FUN_140289930`: mode 0; D0 type 0x29 but then `ndim = 0` (`param_1[3] = 0`), i.e. a
constant 0; D2 type 0x27, D3 type 0x28 (ndim 1, n 0); `+0xac0 = 0`. The reader re-inits only the
DYNARY of the selected mode; the others keep their data.

## Validation, errors, return codes

* EOF / whitespace-only header line → 2.
* Body EOF → 2.
* Unknown `t ≥ 4` silently accepted (mode unchanged).
* No `return 3` on read. The writer returns 3 for a mode outside 0..4 (cannot arise from a read).

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403cfca0`, `name` = `"DIFCOE"`:

```
mode 0: "%-6s %7i %7i %20.10E\n"     name, mat, 0, +0x960
mode 1: "%-6s %7i %7i %7i %7i\n"     name, mat, 1, n0 (+0x974), n1 (+0x978); FUN_1402f3130(fp, D0, 0)
mode 2: "%-6s %7i %7i %7i\n"         name, mat, 2, n0 (+0x9ec);  FUN_1402f3130(fp, D2, 0)
mode 3: "%-6s %7i %7i %7i\n"         name, mat, 3, n0 (+0xa64);  FUN_1402f3130(fp, D3, 0)
mode 4: "%-6s %7i %7i\n"             name, mat, -abs(+0xac0)
other : nothing, return 3 (aborts the KEY writer)
```

The 2-D body: axis0 on one line, axis1 on one line, then the n0·n1 values wrapped every
`max(n0, n1)` values (continuation lines start with 8 spaces). Body skipped when any n < 1 (header
still written). Round trip is exact for every readable mode; `c`, `d` of the header are lost.

## Helpers

* `0x140415990` - `"%i %Flf %Flf %Flf %Flf"` line.
* `0x14012d2a0` / `0x14012fc70` - DYNARY init / re-init.
* `0x14041a800` - body reader; `0x1402f3130` - body writer.
* `0x140484ab8` - `abs`.
* `0x140289930` - sub-object constructor.

## Corpus check

`grep -rIh "^DIFCOE "` over `DEFORM_corpus`: 740 instances, 738 with `t = 0` (one constant,
e.g. `0.0`, `1.147e-07`, `7.4e-05`), 2 with `t = 1`:
`key/2D/SI/InductionHeating/Induction_Heating_Example3_SI.KEY` (`DIFCOE 1 1 26 13`) and
`key/2D/EN/InductionHeating/Induction_Heating_Example3_EN.KEY` (`DIFCOE 4 1 26 13`): 26 + 13 +
338 = 377 numbers on 15 lines (2 axis lines + 13 value lines of 26), next token ELRST: exact.
Oracle: `golden/variants/2D__SI__InductionHeating__Induction_Heating_Example3_SI.OUT.KEY` DIFCOE
block is identical (`diff`) to the input; material goldens show `"DIFCOE       1       0    0.0000000000E+000"`
etc., matching the mode-0 format. Modes 2, 3, 4 do not occur in the corpus.

## Open questions

* Meaning of var 12 (carbon content inferred) and of the two components of the mode-2/3 tables.
* Meaning of the number stored for negative `t`.
