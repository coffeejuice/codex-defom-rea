# COARSN (id 141, group 5 phase transformation)

Coarsening data of the pair `from` → `to` (inferred from the name): two scalar coefficients and
one constant-or-f(temperature) property. Presence of the keyword sets a flag; only flagged pairs
write it.

## Handlers
read: case 7 of `FUN_140383120`; write: `FUN_1403ccb60` → `FUN_1402f4690`; accessor: none (stub
`0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: COARSN <from:%i> <to:%i>                 fscanf "%i %i"
        <c1> <c2> <t> <v> <unused>               rest of line, FUN_140414f50 ("%Flf" ×5)
+0x660 = 1 ; +0x668 = c1 ; +0x670 = c2 ; t = (int)<t> (C truncation)
DYNARY +0x678 re-initialised to type 0x6d (ndim 0, ncomp 1), then
  t=0: constant v
  t=1: ndim 1, var 3, n0=(int)v      body: n0 rows "T value"
  other t: constant 0, no body
body = FUN_14041a800
```
The fifth number is read into a scratch buffer and discarded. EOF/blank remainder → 2 (nothing
stored).

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| (present) | pair | +0x660 | int | 1 |
| c1, c2 | pair | +0x668, +0x670 | double | |
| t, v, table | pair | +0x678 | DYNARY | type 0x6d; ndim 0 or (1, var 3) |

## Defaults
Constructor: +0x660 = 0, +0x668 = +0x670 = 0.0, DYNARY type 0x6d (ndim 0, constant 0).

## Validation, errors, return codes
2 on EOF/blank header remainder or body EOF. Unknown t → constant 0 silently.

## Unit conversion
none found

## KEY grammar (write)
Only if `pair+0x660 == 1`. `FUN_1402f4690(fp, "COARSN", from, to, c1, c2, dyn)`:
```
ndim 0: "%-6s %7i %7i %20.10E %20.10E %7i %20.10E\n"     name, from, to, c1, c2, 0, value
ndim 1: "%-6s %7i %7i %20.10E %20.10E %7i %7i\n"         name, from, to, c1, c2, (var0==3 ? 1 : 2), n0
ndim 2: "%-6s %7i %7i %20.10E %20.10E %7i %7i %7i\n"     name, from, to, c1, c2, 3, n0, n1
ndim 3: no header
then FUN_1402f3130(dyn, 0)
```
Round trip exact for t 0/1 (the reader only creates var 3). The writer prints the ints with `%7i`
while the reader reads them as doubles, which is fine. No fifth value is written.

## Helpers
- `0x140414f50` - five-double line helper
- `0x14012fc70` - DYNARY re-init; `0x14041a800` - body
- `0x1403ccb60` - write handler; `0x1402f4690` - header writer with two doubles

## Corpus check
no corpus file contains COARSN (searched `^\s*COARSN\s` over all files under `key/`,
`materials/` and `db/` with grep -a; also via `g5sim.py`). Not verifiable against the oracle.

## Open questions
- Meaning of c1, c2 and the property.
