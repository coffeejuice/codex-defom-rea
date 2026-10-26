# PHASLH (id 137, group 5 phase transformation)

Latent heat of the transformation `from` → `to` (inferred from the name and the corpus: e.g.
JIS-S45C 1→2 `595.36`, 2→1 `-595.36`, i.e. opposite signs for opposite directions), as a
constant or a table over temperature and/or var 12 (carbon content, inferred).

## Handlers
read: case 3 of `FUN_140383120`; write: `FUN_1403cc4a0` → `FUN_1402f43a0(fp, "PHASLH", from, to,
pair+0x478)`; accessor: `FUN_1403cc550` returns `pair+0x478` (label ignored; returns 0 even if the
pair index does not exist, leaving `*out` untouched).

## KEY grammar (read)
```
line 1: PHASLH <from:%i> <to:%i>        fscanf "%i %i"
        <t:%i> <a:%Flf> <b:%Flf>        rest of line, FUN_140415680
DYNARY +0x478 re-initialised to type 0x1f (ndim 0, ncomp 1), then
  t=0: constant = a                       (no body)
  t=1: ndim 1, var 3,  n0=(int)a          body: n0 rows "T value"
  t=2: ndim 1, var 12, n0=(int)a          body: n0 rows "x value"
  t=3: ndim 2, var 3,12, n0=(int)a, n1=(int)b   body: axis0[n0], axis1[n1], n0·n1 values
  other t: constant 0, no body
body = FUN_14041a800 (rest of the last value line discarded)
```
EOF/blank header remainder → 2 (DYNARY not touched). Body EOF → 2.

## Model mapping
| KEY field | object | offset | C type | DYNARY |
|---|---|---|---|---|
| t, a, b, table | pair | +0x478 | DYNARY (0x78 bytes) | type 0x1f; ndim/var/n per t; ncomp 1 |

No side effects.

## Defaults
Constructor: `FUN_14012d050(pair+0x478)` → type 0, ndim 0, value 0.0 (written as `0 0.0`).

## Validation, errors, return codes
No messages. 2 on EOF/blank line or body EOF; unknown t silently becomes the constant 0.

## Unit conversion
none found. (The English-unit corpus files hold other numbers for the same material, e.g.
9.24594 vs 595.36 in SI; whoever wrote those files converted them, not this reader.)

## KEY grammar (write)
`FUN_1402f43a0(fp, name, from, to, dyn)`:
```
ndim 0: "%-6s %7i %7i %7i %20.10E\n"    name, from, to, 0, value
ndim 1: "%-6s %7i %7i %7i %7i\n"        name, from, to, (var0==3 ? 1 : 2), n0
ndim 2: "%-6s %7i %7i %7i %7i %7i\n"    name, from, to, 3, n0, n1
ndim 3: no header line
then FUN_1402f3130(fp, dyn, 0): body only if every n >= 1 (ndim 1: rows " %20.10E" x + values;
ndim 2: axis lines, values wrapped every max(n0,n1))
```
Always written for an active pair (no condition). Round trip is exact for t 0–3; unknown t
comes back as `0 0.0`. The ndim-2 header writes 3 whatever the vars are (the reader only creates
(3,12)).

## Helpers
- `0x140415680` - `%i %Flf %Flf` line helper
- `0x14012fc70` - DYNARY re-init
- `0x14041a800` - body reader
- `0x1403cc4a0` - write handler; `0x1402f43a0` - shared "from to t n…" header writer
- `0x1403cc550` - accessor

## Corpus check
`g5sim.py` over `key/**` and `materials/`: 168 instances, all `t=0` constants (e.g. `PHASLH 1 2 0
0.5953600E+03` in `materials/JIS-S45C(HeatTreatment)_s000006.KEY` line 493; ±9.24594, ±10.2733 in
the `_e000006`/EN files; 0, 500, 7.765, …), each consuming exactly one line. Oracle golden
`JIS-S45C(HeatTreatment)_s000006.OUT.KEY`: `PHASLH       1       2       0    5.9536000000E+002`.
Table forms (t 1–3) are not in the corpus.

## Open questions
- Unit of the value (energy per volume vs per mass) is not visible in the reader.
