# TRNSFP (id 139, group 5 phase transformation)

Transformation-plasticity switch and coefficient for the transformation `from` → `to` (inferred
from the name; corpus coefficients like 4.18E-05, 5.08E-05 fit a transformation-plasticity
constant K). The coefficient is a constant or a table over temperature and/or var 12 (carbon,
inferred).

## Handlers
read: case 5 of `FUN_140383120`; write: `FUN_1403cc720`; accessor: `FUN_1403cc970` returns
`pair+0x5e8` (label ignored, returns 0).

## KEY grammar (read)
```
line 1: TRNSFP <from:%i> <to:%i>            fscanf "%i %i"
        <on:%i> <t:%i> <a:%Flf> <b:%Flf>    rest of line, FUN_140415ff0 ("%i %i %Flf %Flf")
on == 0: +0x5e4 = 0; nothing else (t, a, b ignored); no body
on != 0: +0x5e4 = 1; DYNARY +0x5e8 is NOT re-initialised (keeps type and other fields):
  t=0: ndim 0, constant a
  t=1: ndim 1, var0 3,  n0=(int)a                 body: n0 rows "T value"
  t=2: ndim 1, var0 12, n0=(int)a                 body: n0 rows
  t=3: ndim 2, var 3,12, n0=(int)a, n1=(int)b     body: axes then n0·n1 values
  other t: DYNARY unchanged; the body is read with whatever ndim/n it already has
  body = FUN_14041a800(fp, pair+0x5e8, 0)
```
EOF/blank remainder → 2 (nothing changed).

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| on | pair | +0x5e4 | int | 0/1 |
| t, a, b, table | pair | +0x5e8 | DYNARY | type stays 0 (constructor); ndim at +0x5ec, value +0x5f0, var +0x5f8/+0x5fc, n +0x604/+0x608; ncomp 1 |

Because the DYNARY is not re-initialised, stale fields survive: e.g. t=1 after an earlier t=3
keeps var1/n1 (harmless for ndim 1).

## Defaults
Constructor: +0x5e4 = 0 (off); DYNARY `FUN_14012d050` → type 0, constant 0.

## Validation, errors, return codes
No messages. 2 on EOF/blank line or body EOF; unknown t accepted silently.

## Unit conversion
none found

## KEY grammar (write)
`FUN_1403cc720`, always for an active pair:
```
+0x5e4 != 1:  "%-6s %7i %7i %7i\n"                  "TRNSFP", from, to, 0
+0x5e4 == 1:
  ndim 0: "%-6s %7i %7i %7i %7i %20.10E\n"         …, 1, 0, value
  ndim 1: "%-6s %7i %7i %7i %7i %7i\n"             …, 1, (var0==3 ? 1 : 2), n0
  ndim 2: "%-6s %7i %7i %7i %7i %7i %7i\n"         …, 1, 3, n0, n1
  ndim 3: no header line
  then FUN_1402f3130(fp, pair+0x5e8, 0)            (body only if all n >= 1)
```
Round trip exact for t 0–3. `on` values other than 0 are normalised to 1.

## Helpers
- `0x140415ff0` - `%i %i %Flf %Flf` line helper
- `0x14041a800` - body reader
- `0x1403cc720` - write handler; `0x1403cc970` - accessor
- `0x1402f3130` - DYNARY body writer

## Corpus check
`g5sim.py` over `key/**` and `materials/`: 168 instances, consumed exactly: `TRNSFP a b 0` (140,
incl. all of `materials/JIS-S45C…`) and `TRNSFP a b 1 0 <K>` (28, Resistance_Spot_Welding and
3D/2D HeatTreat files, K = 4.18E-05, 5.08E-05, …). Oracle golden: `TRNSFP       1       2       0`
(JIS-S45C) and `TRNSFP       4       5       1       0    4.1800000000E-005`
(`variants/…Resistance_Spot_Welding…Cooling_off_cycle_SI.OUT.KEY`), matching the formats.

## Open questions
none
