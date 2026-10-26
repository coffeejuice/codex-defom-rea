# FPERV (id 104, group 4 material)

Two per-material quantities, each either a constant or a 1-D table of var 1, stored in two
DYNARYs at material `+0x1ef0` and `+0x1f68`. The accessor names the first `"Body Force"`; the second
has no label (the accessor returns it for any other label). Name "F PER V" suggests force per
volume (inferred); the meaning of the second quantity and of var 1 is unknown (var 1 is time in
CREEP type 6 data, so possibly time here too — inferred).

## Handlers

* read: case `0xc` of `FUN_140384a50`.
* write: `FUN_1403cf830`, called from `FUN_1403cdca0` (order FRAE2H **FPERV** FRCMOD FSTRES …).
* accessor: `FUN_1403cfbf0(model, label, {mat}, out)`: material found → `*out = mat+0x1ef0` if
  label is `"Body Force"`, else `mat+0x1f68`; returns 0 always (material missing: `*out`
  unchanged, still 0).

## KEY grammar (read)

```
L1: FPERV <mat:%i> <v1:%Flf> <t1:%Flf> <v2:%Flf> <t2:%Flf>   LINE(4 x %Flf) FUN_140414e40
    EOF/blank -> return 2
    fc70(D1 = +0x1ef0, 0); fc70(D2 = +0x1f68, 0)       (both: ndim 0, const 0, ncomp 1)
    (int)t1 == 0: D1.const = v1
    (int)t1 == 1: D1 ndim 1, var0 1, n0 = (int)v1 ; DYN(D1): n0 rows "<x> <y>"
    other t1    : D1 stays constant 0
    (int)t2 == 0: D2.const = v2
    (int)t2 == 1: D2 ndim 1, var0 1, n0 = (int)v2 ; DYN(D2)   (follows D1's body)
    other t2    : D2 stays constant 0
```

The type fields are doubles truncated to int. Short line: missing numbers 0, so the common corpus
form `FPERV 1 0.0` means both constants 0.

## Model mapping

| KEY field | Offset | Type | Notes |
|---|---|---|---|
| v1 (t1 0) | +0x1ef8 (D1 const) | double | D1 = DYNARY at +0x1ef0, type 0 |
| table 1 (t1 1) | D1: ndim +0x1ef4, var0 +0x1f00, n0 +0x1f0c | DYNARY type 0 | ndim 1, var 1, ncomp 1 |
| v2 (t2 0) | +0x1f70 (D2 const) | double | D2 = DYNARY at +0x1f68, type 0 |
| table 2 (t2 1) | D2: ndim +0x1f6c, var0 +0x1f78, n0 +0x1f84 | DYNARY type 0 | ndim 1, var 1, ncomp 1 |

No other side effects.

## Defaults

Material constructor `FUN_14028c6f0`: both DYNARYs constructed with type 0 (constant 0). The
reader fully re-initialises both on every FPERV line.

## Validation, errors, return codes

* Return 2: EOF/blank L1; EOF inside the D1 body when t2 is 0/other; EOF inside the D2 body
  (the D2 status overwrites D1's when t2 == 1).
* Type values other than 0/1 silently give constant 0. No 3, no message.
* Writer returns 3 when a table's var0 is not 1 (cannot arise from a read).

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403cf830`, name `"FPERV"`:

```
D1.ndim 0, D2.ndim 0: "%-6s %7i %20.10E %7i %20.10E %7i\n"  name, mat, D1.const, 0, D2.const, 0
D1.ndim 1, D2.ndim 0: (D1.var0 != 1 -> return 3)
                      "%-6s %7i %7i %7i %20.10E %7i\n"      name, mat, D1.n0, 1, D2.const, 0
                      FUN_1402f3130(D1, 0)
D1.ndim 0, D2.ndim 1: (D2.var0 != 1 -> return 3)
                      "%-6s %7i %20.10E %7i %7i %7i\n"      name, mat, D1.const, 0, D2.n0, 1
                      FUN_1402f3130(D2, 0)
D1.ndim 1, D2.ndim 1: (var checks) "%-6s %7i %7i %7i %7i %7i\n" name, mat, D1.n0, 1, D2.n0, 1
                      FUN_1402f3130(D1, 0); FUN_1402f3130(D2, 0)
any other ndim combination: nothing written, return 0
```

Bodies: rows `" %20.10E %20.10E\n"`, written only when n0 >= 1. The n0 is written with `%7i`
where the reader expects a double — fine for `%Flf`. Round trip exact.

## Helpers

* `0x140414e40` - 4-double line.
* `0x14012fc70` - DYNARY re-init; `0x14041a800` / `0x1402f3130` - body read / write.

## Corpus check

`grep -rah "^FPERV "` over `key/` and `materials/`: 740 instances; 665 full form with both type
fields 0 (values 0 or e.g. `7.693E-02`, `2.84E-04`), 75 short form `FPERV m 0.0` (3 fields). No
table form occurs. All consume exactly one line; `tools/kwcheck457.py` reads all cleanly and
agrees. Oracle: emulation reproduces every FPERV line of `oracle/golden/*.OUT.KEY` and
`oracle/golden/variants/` byte for byte (`"FPERV        1    0.0000000000E+000       0    0.0000000000E+000       0"`;
the short input form is written in full form).

## Open questions

* What the second quantity (+0x1f68) is, and the meaning of var 1 here.
