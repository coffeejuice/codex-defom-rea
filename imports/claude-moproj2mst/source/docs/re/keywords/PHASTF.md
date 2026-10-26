# PHASTF (id 134, group 5 phase transformation)

Switches the transformation from phase (material) `from` to phase `to` on or off and stores the
raw switch value. The pair record (0x780-byte phase-pair object, `_common_g457.md` §5) is created on
first reference by any group-5 keyword. A pair whose switch is 0 (the default) is never written
by the KEY writer, so every other group-5 keyword of that pair is lost on write. The value 2 is
special: only then is PHEQVF written (see PHEQVF.md), so 2 probably selects an
equilibrium-fraction based model, 1 the plain kinetics model (inferred; the corpus has only 1
and 2).

## Handlers
read: case 0 of `FUN_140383120`; write: inline in `FUN_1403cbf00` (registry handler2 is the stub
`0x1402f75d0`); accessor: none (stub `0x1402f7610`, returns -1).

## KEY grammar (read)
```
line 1: PHASTF <from:%i> <to:%i>           (fscanf "%i %i", may cross line breaks)
        <v:%i>                             rest of the same line, FUN_140413de0(fp,&v,0): fgets + sscanf "%i"
```
Nothing follows. Extra text after `v` on the line is discarded. A blank remainder (only
whitespace / just "\n") makes FUN_140413de0 return 1 → v = 0 is still stored (the store happens
before the return code is checked) and the parser returns 2. EOF in the `%i %i` fscanf → 2,
no pair is created.

## Model mapping
| KEY field | object | offset | C type | notes |
|---|---|---|---|---|
| from, to | pair | +0x0, +0x4 | int | lookup key (`FUN_14033a040`), append via `FUN_140339df0` |
| v | pair | +0x8 | int | raw value |
| v != 0 | pair | +0xC | int | 1 if v != 0, else 0 ("transformation on") |

Side effects when v != 0: `FUN_1401d6450(modelData)` recounts the pairs with `+0xC == 1` into
model-data `+0x13f4`; then `FUN_1401d0fc0(modelData, 0)` rebuilds the per-element state-variable
layout (table at model-data `+0x15f8`, 0x4f ints) and reshapes every object's element arrays
(`FUN_1401d0320`, `FUN_140199650`); with argument 0 no material remapping is done.
When v == 0 neither function is called, so `+0x13f4` keeps counting a pair that was switched off
(stale until the next PHASTF with v != 0).

## Defaults
Constructor `FUN_140333f60`: `+0x8 = 0`, `+0xC = 0` (off). Nothing is reset before reading.

## Validation, errors, return codes
No string literals. Returns 2 on EOF before the pair numbers or on an EOF/blank value line; any
integer is accepted.

## Unit conversion
none found

## KEY grammar (write)
Inside `FUN_1403cbf00(modelData, fp, pair, index)`, only if `pair+0xC != 0`, after the pair
banner and TTTN:
```
"%-6s %7i %7i %7i\n"   "PHASTF", from, to, pair+0x8
```
Round trip: v is written back raw. A pair with v = 0 writes nothing at all (no banner, TTTN,
TTTD, …).

## Helpers
- `0x140383120` - group-5 parser
- `0x14033a040` - find pair by (from, to) in list at model-data +0x1358
- `0x140339df0` - allocate + construct (`FUN_140333f60`) + append pair
- `0x140413de0` - one-int line helper
- `0x1401d6450` - count active pairs → model-data +0x13f4
- `0x1401d0fc0` - rebuild element state-variable layout
- `0x1403cbf00` - per-pair KEY writer

## Corpus check
Checked by simulating the reader (`g5sim.py`, all `^PHASTF` lines in `key/**` and
`materials/*.KEY`, plus `.MST`): 168 instances in 24 files, values 1 (152) and 2 (8 in the 2D
Resistance_Spot_Welding KEYs, pair 4→7), e.g. `materials/JIS-S45C(HeatTreatment)_s000006.KEY`
line 465 `PHASTF     1    2    1`. All consume exactly one line. Oracle golden output
(`out/kw457/oracle/golden/JIS-S45C(HeatTreatment)_s000006.OUT.KEY`,
`variants/…Resistance_Spot_Welding…Cooling_off_cycle_SI.OUT.KEY`) shows
`PHASTF       1       2       1` / `PHASTF       4       7       2`, matching the format.

## Open questions
- Meaning of values other than 0/1/2 (any non-zero value switches the pair on).
- Consumer of model-data `+0x13f4`.
