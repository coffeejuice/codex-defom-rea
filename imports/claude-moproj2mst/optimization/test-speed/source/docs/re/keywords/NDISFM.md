# NDISFM (id 112, group 4 material)

Dislocation-density model coefficient (name-based, inferred: dislocation formation / multiplication coefficient). One DYNARY at material `+0x19e0` inside the sub-object at `+0x18f0` (constructor
`FUN_14028bcc0`, shared with BURGRS/ALPHA/NDISFM/RECVRY): a constant, a 1-D table of temperature
(var 3) or strain rate (var 8), or a 2-D table f(temperature, strain rate).

## Handlers

* read: case `0x14` of `FUN_140384a50`.
* write: `FUN_1403d3280`, called by `FUN_1403cdca0` in the order … GRNDAT BURGRS ALPHA NDISFM RECVRY SIZEMD ….
* accessor: `FUN_1403d34c0`: returns `mat+0x19e0` for any label (returns 0).

## KEY grammar (read)

```
L: NDISFM <mat:%i> <t:%i> <a:%Flf> <b:%Flf>      (FUN_140415680)
   (DYNARY NOT re-initialised: type/ncomp stay from the constructor, unused var/n stale)
t 0: ndim 0, const = a
t 1: ndim 1, var0 3, n0 = (int)a
t 2: ndim 1, var0 8, n0 = (int)a
t 3: ndim 2, var 3, 8, n0 = (int)a, n1 = (int)b
other t: return 2
then DYN(D)   (ndim 0 / ncomp 1: nothing read; 1-D rows "<x> <v>"; 2-D axis0, axis1, values)
```

## Model mapping

| KEY field | Offset | Type | Notes |
|---|---|---|---|
| D | material +0x19e0 | DYNARY (type 0 from ctor) | ncomp 1 |
| ndim | +0x19e4 | int | |
| const (t 0) | +0x19e8 | double | |
| var0 / var1 | +0x19f0 / +0x19f4 | int | 3 or 8 / 8 |
| n0 / n1 | +0x19fc / +0x1a00 | int | |

No side effects beyond new-material bookkeeping.

## Defaults

`FUN_14012d050`: type 0, ndim 0, const 0, ncomp 1. Not reset by the reader.

## Validation, errors, return codes

* Header EOF/whitespace → 2; `t` outside 0..3 → 2; body EOF → 2. No return 3.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d3280`, `name` = `"NDISFM"`. Nothing written when `FUN_14012f610(D)` is false (some n < 1).

```
ndim 0: "%-6s %7i %7i %20.10E\n"     name, mat, 0, const
ndim 1: var0 3 -> 1, 8 -> 2:
        "%-6s %7i %7i %7i\n"         name, mat, t, n0;       FUN_1402f3130(fp, D, 0)
ndim 2: var (3, 8) -> 3:
        "%-6s %7i %7i %7i %7i\n"     name, mat, 3, n0, n1;   FUN_1402f3130(fp, D, 0)
no match: "%-6s %7i %7i %20.10E\n"   name, mat, 0, 0.0
```

Always returns 0. Round trip exact for t 0..3; an incomplete table drops the keyword.

## Helpers

* `0x140415680` - header line; `0x14041a800` / `0x1402f3130` - body read / write;
  `0x14012f610` - "is complete".

## Corpus check

676 instances (`grep -rIh "^NDISFM "`), all `t = 0` with const 0: one line, exact. Oracle
material goldens show `"NDISFM       1       0    0.0000000000E+000"`. Tables do not occur.

## Open questions

* Physical meaning (inferred only from the name).
