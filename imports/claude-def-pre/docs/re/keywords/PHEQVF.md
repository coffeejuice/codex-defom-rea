# PHEQVF (id 140, group 5 phase transformation)

Equilibrium volume fraction for the transformation `from` → `to` (inferred from the name), as a
constant or a table over temperature and/or var 12 (carbon content, inferred: the only corpus
table is over 0 / 0.77 / 0.78, i.e. around the eutectoid carbon content, values 1, 0, 0). It is
written only when the pair's PHASTF value is 2. The type selector t is stored raw; t 4–7 read
like t 0–3 (their extra meaning is unknown).

## Handlers
read: case 6 of `FUN_140383120`; write: `FUN_1403cca00` → `FUN_1402f4530(fp, "PHEQVF", from, to,
pair+0x5e0, pair+0x568)` only if `pair+0x8 == 2`; accessor: `FUN_1403ccad0` returns `pair+0x568`
(label ignored, returns 0).

## KEY grammar (read)
```
line 1: PHEQVF <from:%i> <to:%i> <t:%i> <a:%Flf> <b:%Flf>   (pair via fscanf, rest via FUN_140415680)
+0x5e0 = t (raw); DYNARY +0x568 re-initialised to type 0x6c (ndim 0, ncomp 1), then
  t=0 or 4: constant a
  t=1 or 5: ndim 1, var 3,  n0=(int)a            body: n0 rows "T value"
  t=2 or 6: ndim 1, var 12, n0=(int)a            body: n0 rows "x value"
  t=3 or 7: ndim 2, var 3,12, n0=(int)a, n1=(int)b
  other t:  constant 0, no body
body = FUN_14041a800
```

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| t | pair | +0x5e0 | int | raw |
| a, b, table | pair | +0x568 | DYNARY | type 0x6c; ncomp 1 |

## Defaults
Constructor: +0x5e0 = 0; DYNARY type 0, constant 0.

## Validation, errors, return codes
2 on EOF/blank header remainder or body EOF. Unknown t → constant 0 silently (t itself is kept
and written back).

## Unit conversion
none found

## KEY grammar (write)
Only when `pair+0x8` (PHASTF value) == 2; otherwise nothing. `FUN_1402f4530(fp, name, from, to,
t, dyn)`:
```
ndim 0: "%-6s %7i %7i %7i %20.10E\n"    name, from, to, t, value
ndim 1: "%-6s %7i %7i %7i %7i\n"        name, from, to, t, n0
ndim 2: "%-6s %7i %7i %7i %7i %7i\n"    name, from, to, t, n0, n1
ndim 3: no header
then FUN_1402f3130(dyn, 0)
```
The stored t is written, not a value derived from ndim/var (unlike `FUN_1402f43a0`).
Asymmetry: a PHEQVF read for a pair whose PHASTF value is not 2 is silently dropped on write.

## Helpers
- `0x140415680` - header helper; `0x14012fc70` - DYNARY re-init; `0x14041a800` - body
- `0x1403cca00` - write handler; `0x1402f4530` - "from to t n…" header writer with raw t
- `0x1403ccad0` - accessor

## Corpus check
8 instances (the 8 2D Resistance_Spot_Welding KEYs, pair 4→7, PHASTF 2): `PHEQVF 4 7 2 3` + 3
rows `0 1`, `0.77 0`, `0.78 0`; `g5sim.py` confirms exact consumption (next token IPSNRG). Oracle
golden `variants/2D__SI__ResistanceSpotWelding__…Cooling_off_cycle_SI.OUT.KEY` line 1233:
`PHEQVF       4       7       2       3` followed by ` %20.10E %20.10E` rows, matching.
No material file contains PHEQVF.

## Open questions
- Difference between t 0–3 and t 4–7.
