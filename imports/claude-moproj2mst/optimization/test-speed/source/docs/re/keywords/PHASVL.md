# PHASVL (id 138, group 5 phase transformation)

Volume change of the transformation `from` → `to` (inferred from the name and the corpus: a small
dilatation such as 0.00588 at 0 °C falling to 0.00138 at 900 °C for S45C 1→2, with the opposite
sign for 2→1), as a constant or a table over temperature and/or var 12 (carbon content,
inferred: single axis point 0.44 for S45C).

## Handlers
read: case 4 of `FUN_140383120`; write: `FUN_1403cc5e0` → `FUN_1402f43a0(fp, "PHASVL", from, to,
pair+0x4f0)`; accessor: `FUN_1403cc690` returns `pair+0x4f0` (label ignored, returns 0).

## KEY grammar (read)
Identical to PHASLH with DYNARY `+0x4f0` re-initialised to type 0x20:
```
line 1: PHASVL <from:%i> <to:%i> <t:%i> <a:%Flf> <b:%Flf>    (pair via fscanf, rest via FUN_140415680)
  t=0: constant a
  t=1: ndim 1, var 3,  n0=(int)a    body: n0 rows "T value"
  t=2: ndim 1, var 12, n0=(int)a    body: n0 rows "x value"
  t=3: ndim 2, var 3,12, n0=(int)a, n1=(int)b   body: axis0 (T), axis1 (var 12), n0·n1 values
  other t: constant 0, no body
```

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| t, a, b, table | pair | +0x4f0 | DYNARY | type 0x20; ndim/var/n per t; ncomp 1 |

## Defaults
Constructor: `FUN_14012d050(pair+0x4f0)` → type 0, constant 0.0.

## Validation, errors, return codes
As PHASLH: 2 on EOF/blank header remainder or body EOF; unknown t → constant 0, silently.

## Unit conversion
none found

## KEY grammar (write)
`FUN_1402f43a0` (see PHASLH.md): ndim 0 `"%-6s %7i %7i %7i %20.10E\n"` (t 0, value); ndim 1
`"%-6s %7i %7i %7i %7i\n"` (1 if var0 == 3 else 2, n0); ndim 2 `"%-6s %7i %7i %7i %7i %7i\n"`
(3, n0, n1); then `FUN_1402f3130(dyn, 0)`. Always written for an active pair. Exact round trip
for t 0–3.

## Helpers
- `0x140415680` - header line helper; `0x14012fc70` - DYNARY re-init; `0x14041a800` - body
- `0x1403cc5e0` - write handler; `0x1402f43a0` - header writer; `0x1403cc690` - accessor

## Corpus check
`g5sim.py` over `key/**` and `materials/`: 168 instances, all consumed exactly: t=0 (56, e.g.
`-0.000586`, `±0.01`, `0`), t=1 with n0 = 20 (24; rows "T value"), t=3 with (n0,n1) = (20,1),
(1,20), (13,3), (20,2), (20,4) (80). `materials/JIS-S45C(HeatTreatment)_s000006.KEY` line 494:
`PHASVL 1 2 3 20 1` + 20 temperatures, `0.44`, 20 values. Oracle golden outputs
(`JIS-S45C…_s000006.OUT.KEY`: `PHASVL       1       2       3      20       1`, axis0 on one line
of 20, `4.4000000000E-001`, values on one line of 20; `variants/…HT_CRACK_SI.OUT.KEY` t=1:
`PHASVL       1       2       1      20` + rows `    0.0000000000E+000    5.8800000000E-003`;
`variants/…Resistance_Spot_Welding…OUT.KEY` t=0: `PHASVL       4       7       0
1.0000000000E-002`) match the write format.

## Open questions
none
