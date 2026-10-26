# IPSNRG (id 142, group 5 phase transformation)

Interphase energy (inferred from the name) of the pair `from` → `to`: constant or a table over
temperature and/or var 23 (unknown variable). Always written for an active pair (default
`0 0.0`).

## Handlers
read: case 8 of `FUN_140383120`; write: `FUN_1403ccc50` → `FUN_1402f43a0(fp, "IPSNRG", from, to,
pair+0x6f0)`; accessor: none (stub `0x1402f7610`).

## KEY grammar (read)
```
line 1: IPSNRG <from:%i> <to:%i> <t:%i> <a:%Flf> <b:%Flf>   (pair via fscanf, rest via FUN_140415680)
DYNARY +0x6f0 re-initialised to type 0x6e (ndim 0, ncomp 1), then
  t=0: constant a
  t=1: ndim 1, var 3,  n0=(int)a            body: n0 rows "T value"
  t=2: ndim 1, var 23, n0=(int)a            body: n0 rows
  t=3: ndim 2, var 3,23, n0=(int)a, n1=(int)b   body: axes then n0·n1 values
  other t: constant 0, no body
body = FUN_14041a800
```

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| t, a, b, table | pair | +0x6f0 | DYNARY | type 0x6e; ndim/var/n per t; ncomp 1 |

## Defaults
Constructor: DYNARY type 0x6e, ndim 0, constant 0.

## Validation, errors, return codes
2 on EOF/blank header remainder or body EOF; unknown t silently → constant 0.

## Unit conversion
none found

## KEY grammar (write)
`FUN_1402f43a0` (see PHASLH.md): ndim 0 `"%-6s %7i %7i %7i %20.10E\n"` (0, value); ndim 1
`"%-6s %7i %7i %7i %7i\n"` (1 if var0 == 3 else 2 → var 23 gives 2, n0); ndim 2
`"%-6s %7i %7i %7i %7i %7i\n"` (3, n0, n1); body `FUN_1402f3130(dyn, 0)`. Round trip exact for
t 0–3.

## Helpers
- `0x140415680` - header helper; `0x14012fc70` - DYNARY re-init; `0x14041a800` - body
- `0x1403ccc50` - write handler; `0x1402f43a0` - header writer

## Corpus check
160 instances in 22 KEY files, all `IPSNRG a b 0 0` (constant 0, one line; `g5sim.py`: next token
`*` or VARSEL). `materials/JIS-S45C(HeatTreatment)_e000006.KEY` has 4 (pairs 12↔13/14), the
`_s000006` file none; the oracle golden output of `_s000006` nevertheless has `IPSNRG       1       2       0    0.0000000000E+000` for every active pair (default
written), matching the ndim-0 format.

## Open questions
- Meaning of var 23 (also used by GRNDAT sub-functions).
