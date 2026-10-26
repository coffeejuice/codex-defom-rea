# HDNPHA (id 114, group 4 material)

Hardness of the material/phase (name-based, inferred; corpus constants 20, 30, 55 look like HRC
values for heat-treatment phases). One DYNARY at material `+0xf10` (DYNARY type 0x23): a
constant, a 1-D table of var 12, temperature (var 3) or var 9, or a 2-D table f(temperature,
var 12).

## Handlers

* read: case `0x16` of `FUN_140384a50`.
* write: `FUN_1403cffe0`, called by `FUN_1403cdca0` after EMSVTY (… EMSVTY HDNPHA HDNTIM …).
* accessor: `FUN_1403d01f0`: returns `mat+0xf10` for any label (returns 0; `*out` untouched if
  the material does not exist).

## KEY grammar (read)

```
L: HDNPHA <mat:%i> <t:%i> <a:%Flf> <b:%Flf>      (FUN_140415680 "%i %Flf %Flf")
   FUN_14012fc70(D, 0x23)                        (re-init BEFORE the type check)
t == 0: ndim 0, const = a                        (no body)
t == 1: ndim 1, var 12, n0 = (int)a              DYN(D)
t == 2: ndim 1, var 3,  n0 = (int)a              DYN(D)
t == 3: ndim 1, var 9,  n0 = (int)a              DYN(D)
t == 4: ndim 2, var 3, 12, n0 = (int)a, n1 = (int)b   DYN(D)
other : return 3                                 (D already re-initialised: old data lost)
```

1-D body: n0 rows `<x> <v>` (ncomp 1); 2-D: axis0, axis1, n0·n1 values.

## Model mapping

| KEY field | Offset | Type | Notes |
|---|---|---|---|
| table / constant | material +0xf10 | DYNARY 0x23 | ncomp 1 |
| t → ndim | +0xf14 | int | |
| a (t 0) | +0xf18 | double | |
| var0 / var1 | +0xf20 / +0xf24 | int | 12, 3, 9 / 12 |
| n0 / n1 | +0xf2c / +0xf30 | int | |

No side effects beyond new-material bookkeeping.

## Defaults

Constructor `FUN_14012d050(+0xf10)`: DYNARY type 0, ndim 0, const 0 → written as
`HDNPHA mat 0 0.0`. The reader always re-inits with type 0x23.

## Validation, errors, return codes

* Header EOF / whitespace-only → 2; body EOF → 2.
* `t` not in 0..4 → **3** ("ERROR: Keyword %s Data Error. \n"), after the DYNARY was reset.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403cffe0`, `name` = `"HDNPHA"`, always called:

```
ndim 0:  "%-6s %7i %7i %20.10E\n"   name, mat, 0, const (+0xf18)
ndim 1:  var0 12 -> 1, 3 -> 2, 9 -> 3, other -> return 3 (nothing written)
         "%-6s %7i %7i %7i\n"       name, mat, t, n0
ndim 2:  "%-6s %7i %7i %7i %7i\n"   name, mat, 4, n0, n1      (vars not checked)
other ndim: return 3
then FUN_1402f3130(fp, D, 0)        (skipped when any n < 1; ndim 0 writes nothing)
```

Return 3 aborts the KEY writer. Round trip exact for t 0..4.

## Helpers

* `0x140415680` - `"%i %Flf %Flf"` header line.
* `0x14012fc70` - DYNARY re-init; `0x14041a800` - body reader; `0x1402f3130` - body writer.

## Corpus check

740 instances (`grep -rIh "^HDNPHA "`), all `t = 0` with one constant (0, 20, 30, 55): one line,
exact. Oracle material goldens (`golden/*.OUT.KEY`) show e.g.
`"HDNPHA       1       0    0.0000000000E+000"` and `5.5000000000E+001` — the mode-0 format.
Tables do not occur in the corpus.

## Open questions

* Meaning of var 9 and var 12 here (var 12 possibly carbon content, as in DIFCOE).
