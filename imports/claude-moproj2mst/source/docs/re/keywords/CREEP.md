# CREEP (id 106, group 4 material)

Creep law of a material, stored in the material sub-object at `+0xac8` (constructor
`FUN_140289b80`): a mode int and one DYNARY per law. The accessor labels name the laws:
`"Creep - Prezyna"` (KEY 1; Perzyna, spelled so in the binary), `"Creep - Power Law"` (2),
`"Creep - GE"` (3), `"Creep - WG"` (4), each a temperature table with 2, 2, 5, 3 coefficients per
point; `"Creep - TableDataStr"` (5, 3-D table over var 7, 6, 3) and `"Creep - TableData"` (6, 3-D
table over var 1, 6, 3); types 7/8 an N-variable table (same layout as FSTRES 13/15); negative a
user-routine number. Corpus type-6 data show var 1 = time, var 6 = stress, values = creep strain
(inferred from data shape). Strings `"Creep can be active only as part of the …"` exist elsewhere
(licensing of the creep module; not analysed).

## Handlers

* read: case `0xe` of `FUN_140384a50`.
* write: `FUN_1403d22a0`, called from `FUN_1403cdca0` (order … JOMINY MSTMTR **CREEP** DIFCOE …).
* accessor: `FUN_1403d2a00(model, label, {mat}, out)`: returns 0 with `*out` =
  `"Creep - Prezyna"` mat+0xbc0, `"Creep - Power Law"` +0xc38, `"Creep - GE"` +0xcb0,
  `"Creep - WG"` +0xd28, `"Creep - TableDataStr"` +0xb48, `"Creep - TableData"` +0xad0;
  otherwise (or material missing) returns 3.

## KEY grammar (read)

```
L1: CREEP <mat:%i> <t:%i> <n:%i> <x:%i>     LINE("%i %i %i") FUN_140413fb0   EOF/blank -> return 2
t 0   : M = 0
t 1..4: M = t ; fc70(D_t, 0x44+t) ; D_t.n0 = n ; DYN(D_t): n rows "<T> <c1> … <c_ncomp>"
        D_1 +0xbc0 (0x45: var 3, ncomp 2)   D_2 +0xc38 (0x46: var 3, ncomp 2)
        D_3 +0xcb0 (0x47: var 3, ncomp 5)   D_4 +0xd28 (0x48: var 3, ncomp 3)
        x is ignored
t 5   : M = 5 ; L2 LINE("%i %i %i") -> n0,n1,n2       EOF -> return 2
        fc70(D5 = +0xb48, 0x44: ndim 3, var 7,6,3) ; n -> +0xb64,+0xb68,+0xb6c ; DYN(D5)
t 6   : M = 6 ; L2 LINE("%i %i %i") -> n0,n1,n2       EOF -> return 2
        fc70(D6 = +0xad0, 0x43: ndim 3, var 1,6,3) ; n -> +0xaec,+0xaf0,+0xaf4 ; DYN(D6)
t 7,8 : M = t ; L2 LINE("%i") FUN_140413de0 -> cnt (+0xda4)            EOF -> return 2
        fc70(A = +0xda8, 0xa9: ndim 1, var 0, ncomp 2) ; A.n0 (+0xdc4) = cnt
        DYN(A): cnt rows "<k_i> <p_i> <q_i>"                           fail -> return 2
        sum = Σ(int)k_i ; prod = Π(int)k_i
        fc70(B = +0xe20, 0xaa: ndim 0) ; B.ncomp (+0xe60) = sum ; DYN(B)  fail -> return 2
        fc70(C = +0xe98, 0xab: ndim 0) ; C.ncomp (+0xed8) = prod ; DYN(C) fail -> return 2; else 0
t < 0 : M = 9 ; +0xda0 = -t
t > 8 : nothing changed, return 0
```

As in FSTRES 13/15, ndim-0 DYNARYs with `ncomp < 2` read no value (sum or prod of 1).

## Model mapping

| KEY t | M (+0xac8) | DYNARY | offsets | type / ndim / var / ncomp | label |
|---|---|---|---|---|---|
| 0 | 0 | none | | | (no creep) |
| 1 | 1 | D_1 | +0xbc0 (n0 +0xbdc, ncomp +0xc00) | 0x45 / 1 / 3 / 2 | Creep - Prezyna |
| 2 | 2 | D_2 | +0xc38 (n0 +0xc54, ncomp +0xc78) | 0x46 / 1 / 3 / 2 | Creep - Power Law |
| 3 | 3 | D_3 | +0xcb0 (n0 +0xccc, ncomp +0xcf0) | 0x47 / 1 / 3 / 5 | Creep - GE |
| 4 | 4 | D_4 | +0xd28 (n0 +0xd44, ncomp +0xd68) | 0x48 / 1 / 3 / 3 | Creep - WG |
| 5 | 5 | D5 | +0xb48 (n +0xb64..+0xb6c) | 0x44 / 3 / 7,6,3 / 1 | Creep - TableDataStr |
| 6 | 6 | D6 | +0xad0 (n +0xaec..+0xaf4) | 0x43 / 3 / 1,6,3 / 1 | Creep - TableData |
| 7, 8 | 7, 8 | cnt, A, B, C | cnt +0xda4; A +0xda8 (n0 +0xdc4, axis0 ptr +0xdd0, values ptr +0xdf0); B +0xe20 (ncomp +0xe60, values +0xe68); C +0xe98 (ncomp +0xed8) | 0xa9 / 1 / 0 / 2; 0xaa / 0; 0xab / 0 | none (inferred N-variable table) |
| <0 | 9 | — | +0xda0 = −t | | user routine |

Var 3 temperature, 7 strain; 6 stress and 1 time inferred from corpus data.

## Defaults

`FUN_140289b80`: M = 0; D6 0x43, D5 0x44, D_1..D_4 0x45..0x48 (n 0), +0xda0 = 0, **+0xda4 = -1**,
A 0xa9, B 0xaa, C 0xab. Absent keyword → written as `CREEP m 0`. The reader re-inits only the
selected DYNARY(s).

## Validation, errors, return codes

* Return 2: EOF/blank L1; EOF on L2 (t 5–8); EOF inside any body.
* t > 8 silently accepted. No 3 on read, no message.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d22a0`, name `"CREEP"`:

```
M 0  : "%-6s %7i %7i\n" name, mat, 0
M 1-4: "%-6s %7i %7i %7i %7i\n" name, mat, M, D_M.n0, D_M.ncomp + 1 ; FUN_1402f3130(D_M, 0)
       (rows " %20.10E" T then ncomp values, 5 per line; rows only if n0 >= 1)
M 5  : "%-6s %7i %7i\n" name, mat, 5 ; FUN_1402f3130(D5, 1)  (n line, 3 axes, values, wrap max(n))
M 6  : same with 6 and D6
M 7,8: "%-6s %7i %7i\n" name, mat, M ; +0xda4 = A.n0 (writer modifies the model) ;
       " %7i\n" A.n0 ; per row " %7i %7i %7i\n" (int)k_i, (int)p_i, (int)q_i ;
       per row i: (int)k_i values "%15.7E" from B, then "\n" ; FUN_1402f3130(C, 1)
M 9  : "%-6s %7i %7i\n" name, mat, -(+0xda0)
other: return 3 (KEY writer aborts)
```

Asymmetries: the 4th L1 field `x` is ignored on read and written as ncomp+1 (column count incl.
temperature) for t 1–4; t 7/8 have the same losses as FSTRES 13/15 (`%15.7E` precision, ints for
p/q, stray value when sum or prod is 1).

## Helpers

* `0x140413fb0` - `"%i %i %i"` line; `0x140413de0` - `"%i"` line.
* `0x14012fc70` - DYNARY re-init; `0x14041a800` / `0x1402f3130` - body read / write.
* `0x140289b80` - sub-object constructor; `0x140289d80` - counts DYNARYs per mode.

## Corpus check

`grep -rah "^CREEP "` over `key/` and `materials/`: 733 instances, 729 `t 0` (one line) and 4
`t 6`: `key/2D/{EN,SI}/Creep/CREEP_Example{1,2}_*.KEY`, e.g. `CREEP 1 6` / `2 2 3` / axes
`0 10`, `0.145 36.26` (ksi) / `68 932 1832` (°F) / 12 values (0 at time 0, 0.02 at time 10):
consumed exactly up to DIFCOE. `tools/kwcheck457.py` reads all cleanly and agrees. Oracle:
emulation reproduces the CREEP blocks of `oracle/golden/*.OUT.KEY` (t 0, including materials with
no CREEP line in the input → `CREEP m 0`) and `oracle/golden/variants/2D__SI__Creep__CREEP_Example1_SI.OUT.KEY`
(t 6) byte for byte.

## Open questions

* Formulas of the Prezyna / Power Law / GE / WG laws and the role of each coefficient column.
* Meaning of t 7 vs 8 and of p_i, q_i.
