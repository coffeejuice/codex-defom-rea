# FRCMOD (id 105, group 4 material)

A per-material model selector with up to five numbers, stored in the material sub-object at
`+0x1ad0` (constructor `FUN_14028c040`): a mode int, mode-specific doubles, an always-stored first
value `a` and fifth value `e`, a 9-double set (KEY type 11), two 1-D tables (KEY types 12, 17) or a
user-routine number (negative type). Inferred meaning: fracture / damage criterion model (the name
matches FRCMTH/FRCNEL/FRCSTP "fracture" keywords; the corpus uses it in heat-treatment cracking with
type 10 and `a = 0.2`). Formulas are not in this code path.

## Handlers

* read: case `0xd` of `FUN_140384a50`.
* write: `FUN_1403d3820`, which uses `FUN_14028e140(mat, &t, double v[5])` to map the stored mode
  back to the KEY type and the five header numbers. Called from `FUN_1403cdca0` (order … FRAE2H
  FPERV **FRCMOD** FSTRES …).
* accessor: registry stub `0x1402f7610` (returns -1).

## KEY grammar (read)

```
L1: FRCMOD <mat:%i> <t:%i> <a:%Flf> <b:%Flf> <c:%Flf> <d:%Flf> <e:%Flf>
    LINE("%i %Flf %Flf %Flf %Flf %Flf") FUN_140415ac0          EOF/blank -> return 2
    always: +0x1b00 = a ; +0x1b88 = e
t 0..10 except 7: M = t ; t 2: +0x1ad8 = b ; t 4: +0x1ae0 = b ; t 5: +0x1ae8 = b
t 7  : M = 7 ; +0x1af0 = b ; +0x1af8 = c
t 11 : M = 0xc ; L2 LINE(9 x %Flf) FUN_140415320 -> +0x1c80, +0x1c88, +0x1ca0, +0x1ca8, +0x1cb0,
                                                     +0x1cb8, +0x1cc0, +0x1c90, +0x1c98
       EOF -> return 2
t 12 : M = 0xd ; +0x1b88 = 100.0 (DAT_140524cd8, overrides e) ;
       table G (+0x1b90, NOT re-initialised): ndim = 1, var0 = 31, n0 = (int)b
       DYN(G): n0 rows "<x> <v>"   (ncomp from the DYNARY, 1 by construction)
t 15 : M = 0xf ; +0x1ae8 = b
t 16 : M = 0x10
t 17 : M = 0x11 ; table K (+0x1c08, not re-initialised, ctor type 0xb4: ndim 1, var 32):
       n0 = (int)b ; DYN(K)
t < 0: M = 0xb ; +0x1b08 = -t
t 13, 14, >= 18: M unchanged (only a and e stored), return 0
```

`d` is read and always discarded. Short line: missing numbers 0 (the 7-field corpus form without
`e` reads `e = 0`).

## Model mapping

| KEY field | Offset (material) | Type | Notes |
|---|---|---|---|
| t → M | +0x1ad0 | int | KEY 0–10 → same; 11 → 0xc; 12 → 0xd; 15, 16, 17 → same; <0 → 0xb |
| a | +0x1b00 | double | every type |
| e | +0x1b88 | double | every type; forced to 100.0 for t 12 |
| b (t 2) | +0x1ad8 | double | |
| b (t 4) | +0x1ae0 | double | |
| b (t 5, 15) | +0x1ae8 | double | shared slot |
| b, c (t 7) | +0x1af0, +0x1af8 | double | |
| 9 doubles (t 11) | +0x1c80 +0x1c88 +0x1ca0 +0x1ca8 +0x1cb0 +0x1cb8 +0x1cc0 +0x1c90 +0x1c98 | double | file order as listed |
| table (t 12) | G +0x1b90 (ndim +0x1b94, var0 +0x1ba0, n0 +0x1bac) | DYNARY 0x1c | ndim 1, var 31, ncomp 1 |
| table (t 17) | K +0x1c08 (n0 +0x1c24) | DYNARY 0xb4 | ndim 1, var 32, ncomp 1 |
| -t (t<0) | +0x1b08 | int | routine number |

Var 31 and 32 meanings unknown. Also in the sub-object: DYNARY type 0x1b at +0x1b10 (not used by
FRCMOD).

## Defaults

`FUN_14028c040`: M = 0; doubles +0x1ad8…+0x1b00, +0x1b88, +0x1c80…+0x1cc7 = 0; +0x1b08 = 0;
G type 0x1c (ndim 1, var 31, n 0), K type 0xb4 (ndim 1, var 32, n 0). The reader resets nothing
except the fields of the selected type (G/K keep their type, ncomp and old data until the body
reallocates).

## Validation, errors, return codes

* Return 2: EOF/blank L1; EOF on the t 11 line; EOF inside the t 12 / t 17 body.
* Unknown types silently accepted (return 0). No `return 3` on read, no message.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_14028e140` returns false for M 0xe or M > 0x11 (and any other value not listed) → the writer
returns **3** (KEY writer aborts). Otherwise `v = {+0x1b00, b', c', 0, +0x1b88}` with
`b' = +0x1ad8 (M2) | +0x1ae0 (M4) | +0x1ae8 (M5, M0xf) | +0x1af0 (M7) | (double)G.n0 (M0xd) |
(double)K.n0 (M0x11) | 0`, `c' = +0x1af8 (M7) | 0`, and the KEY type `t = M` for 0–10, 0xf, 0x10,
0x11; `0xb` for M 0xc; `0xc` for M 0xd; for M 0xb: `t = +0x1b08` if `<= 0`, else `-(+0x1b08)`.

```
"%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"  "FRCMOD", mat, t, v0..v4
t 11: " %20.10E" x9 "\n"  +0x1c80 +0x1c88 +0x1ca0 +0x1ca8 +0x1cb0 +0x1cb8 +0x1cc0 +0x1c90 +0x1c98
t 12: FUN_1402f3130(G, 0)   (rows " %20.10E %20.10E\n", only if n0 >= 1)
t 17: FUN_1402f3130(K, 0)
```

Asymmetries: `d` is always written as 0; `b` of types other than 2, 4, 5, 7, 15 is written as 0;
`c` only kept for t 7; t 12 rewrites `e` as 100.0; t 13/14/≥18 cannot be represented (the stored
mode stays whatever it was). Everything else round-trips.

## Helpers

* `0x140415ac0` - `"%i %Flf x5"` line; `0x140415320` - 9-double line.
* `0x14028e140` - mode → (KEY type, 5 values) for writing.
* `0x14041a800` / `0x1402f3130` - DYNARY body read / write.
* `0x14028c040` - sub-object constructor.

## Corpus check

`grep -rah "^FRCMOD "` over `key/` and `materials/`: 739 instances, t 0 (727, 7- or 8-field
form), t 10 (10, `a` = 0 or 0.2, e.g. `key/2D/SI/HeatTreat/HT_CRACK_SI.KEY`), t 1 (2,
`key/2D/SI/ElastoPlastic/Extrusion-EP_SI.KEY`). One-line headers, nothing else consumed; next
token is FSTRES. `tools/kwcheck457.py` reads all of them cleanly and agrees with this grammar.
Oracle: an emulation of this read/write reproduces every FRCMOD line of `oracle/golden/*.OUT.KEY`
and `oracle/golden/variants/` (t 0, 1, 10) byte for byte, e.g.
`"FRCMOD       1       0    0.0000000000E+000 …"` (five values, d = 0).

## Open questions

* Model names for each type (fracture criteria inferred); meaning of a, b, c, e, the 9-double set
  and the var 31 / var 32 tables.
* Why t 12 forces e = 100.0.
