# BURGRS (id 110, group 4 material)

Burgers vector of the material (name-based, inferred; part of the dislocation-density group
BURGRS / ALPHA / NDISFM / RECVRY stored together in the sub-object at material `+0x18f0`,
constructor `FUN_14028bcc0`). One DYNARY at `+0x18f0`: a constant or a table in 1, 2 or 3 of the
variables 3 (temperature), 6 and 24.

## Handlers

* read: case `0x12` of `FUN_140384a50`.
* write: `FUN_1403d2c20`, called by `FUN_1403cdca0` after GRNDAT (… GRNDAT BURGRS ALPHA NDISFM RECVRY SIZEMD …).
* accessor: `FUN_1403d2f20`: returns `mat+0x18f0` for any label (returns 0).

## KEY grammar (read)

```
L: BURGRS <mat:%i> <t:%i> <a:%Flf> <b:%Flf>      (FUN_140415680)
   (the DYNARY is NOT re-initialised: type/ncomp stay from the constructor, unused var/n stale)
t 0: ndim 0, const = a
t 1: ndim 1, var0 3,  n0 = (int)a
t 2: ndim 1, var0 6,  n0 = (int)a
t 3: ndim 1, var0 24, n0 = (int)a
t 4: ndim 2, var 3, 6,  n = (int)a, (int)b
t 5: ndim 2, var 3, 24, n = (int)a, (int)b
t 6: ndim 2, var 6, 24, n = (int)a, (int)b
t 7: L: <n0:%i> <n1:%i> <n2:%i>                   (FUN_140413fb0, next line)
     ndim 3, var 3, 6, 24
other t: return 2
then DYN(D)  (ndim 0 with ncomp 1: nothing read)
```

## Model mapping

| KEY field | Offset | Type | Notes |
|---|---|---|---|
| D | material +0x18f0 | DYNARY (type 0 from ctor) | ncomp 1 |
| ndim | +0x18f4 | int | |
| const (t 0) | +0x18f8 | double | |
| var0..2 | +0x1900, +0x1904, +0x1908 | int | 3 / 6 / 24 |
| n0..2 | +0x190c, +0x1910, +0x1914 | int | |

No side effects beyond new-material bookkeeping.

## Defaults

`FUN_14028bcc0` → `FUN_14012d050`: type 0, ndim 0, const 0, ncomp 1. Nothing reset by the reader
except the fields listed above.

## Validation, errors, return codes

* Header EOF/whitespace → 2; `t` outside 0..7 → 2; t 7 n-line EOF → 2; body EOF → 2.
* No return 3.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d2c20`, `name` = `"BURGRS"`. Nothing at all is written when `FUN_14012f610(D)` is false
(some `n[k] < 1`, k < ndim).

```
ndim 0: "%-6s %7i %7i %20.10E\n"      name, mat, 0, const
ndim 1: var0 3 -> 1, 6 -> 2, 24 -> 3:
        "%-6s %7i %7i %7i\n"          name, mat, t, n0;  FUN_1402f3130(fp, D, 0)
ndim 2: (3,6) -> 4, (3,24) -> 5, (6,24) -> 6:
        "%-6s %7i %7i %7i %7i\n"      name, mat, t, n0, n1;  FUN_1402f3130(fp, D, 0)
ndim 3: "%-6s %7i %7i\n"              name, mat, 7;  FUN_1402f3130(fp, D, 1)
        (mode 1: first " %7i %7i %7i\n" n0 n1 n2, then the three axes and the values)
no match (other var combination or ndim): "%-6s %7i %7i %20.10E\n" name, mat, 0, 0.0
```

Always returns 0. The ndim-3 variables are not checked (always written as t 7 = (3, 6, 24)).
An incomplete table drops the keyword entirely (re-read keeps the constructor default).

## Helpers

* `0x140415680` - header; `0x140413fb0` - `"%i %i %i"` line (t 7).
* `0x14041a800` / `0x1402f3130` - body read / write; `0x14012f610` - "is complete".

## Corpus check

676 instances, all `BURGRS <mat> 0 <const>` (const 0): one line each, exact. Oracle material
goldens show `"BURGRS       1       0    0.0000000000E+000"`. Tables do not occur.

## Open questions

* Meaning of var 6 and var 24.
