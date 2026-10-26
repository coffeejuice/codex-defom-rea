# SINTER (id 130, group 4 material)

Sintering model selection and up to four model constants of a material. Model names are
**inferred** from the corpus file names: 2 Skorohod, 3 Cocks, 4 Shinagawa, 5 Ashby,
6 Raj–Ashby; 1 has no constants (unknown model); 0 = off.

## Handlers

* read: case `0x26` of `FUN_140384a50`.
* write: `FUN_1403d45f0`, called by `FUN_1403cdca0` after TXTURE (… TXTURE SINTER GBENGY GBMOBI NUCSIZ).
* accessor: none (registry handler3 is the stub `0x1402f7610`).

## KEY grammar (read)

```
L: SINTER <mat:%i> <model:%i> <cnt:%i>          (FUN_140413ee0 "%i %i" -> model straight into +0x21d8)
if model != 0 && cnt > 0:
   L: <p1> <p2> <p3> <p4>   (%Flf x4)           (FUN_140414e40; all four zeroed first,
                                                 missing numbers stay 0; extra numbers ignored)
```

`cnt` only decides whether the data line exists; its value is not stored.

## Model mapping

| KEY field | Offset | Type |
|---|---|---|
| model | material +0x21d8 | int |
| p1..p4 | +0x21e0, +0x21e8, +0x21f0, +0x21f8 | double |

No side effects beyond new-material bookkeeping.

## Defaults

`FUN_14028cdc0`: model 0, p1..p4 = 0. When the data line is not read, p1..p4 keep their previous
values.

## Validation, errors, return codes

* Header EOF/whitespace → 2 (model and cnt zeroed, so no data line is attempted).
* Data line EOF/whitespace → 2. No return 3; any model value accepted.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d45f0`, `name` = `"SINTER"`, header `"%-6s %7i %7i %7i\n"` name, mat, model, cnt:

| model | cnt | data line |
|---|---|---|
| 1 | 0 | none |
| 2 | 2 | `" %20.10E %20.10E\n"` p1 p2 |
| 3 | 4 | `" %20.10E %20.10E %20.10E %20.10E\n"` p1..p4 |
| 4 | 3 | `" %20.10E %20.10E %20.10E\n"` p1..p3 |
| 5 | 2 | p1 p2 |
| 6 | 3 | p1..p3 |
| 0, < 0, > 6 | – | nothing at all |

Always returns 0. Asymmetries: constants beyond the model's count are dropped (re-read as 0); a
model ≥ 7 or < 0 read from KEY is not written back.

## Helpers

* `0x140413ee0` - `"%i %i"` header; `0x140414e40` - four-double line.
* `0x1403d45f0` - writer.

## Corpus check

10 files contain SINTER: `key/3D/{EN,SI}/Sintering_3D/Unified_Compaction_And_Sintering/`
`Sintering_Skorohod.KEY` (`2 2`, 2 values), `Sintering_CocksModel.KEY` (`3 4`, 4),
`Sintering_Shinagawa.KEY` (`4 3`, 3), `Sintering_Ashby.KEY` (`5 2`, 2),
`Sintering_RajAshby.KEY` (`6 3`, 3): header + one data line each, next token GBENGY: exact, and
the counts equal the writer's table. Oracle `golden/variants/3D__SI__Sintering_3D__…__*.OUT.KEY`
reproduce these blocks byte for byte (`"SINTER       1       5       2"` /
`"    1.0000000000E+000    4.0000000000E-001"`). Model 1 does not occur.

## Open questions

* Meaning of model 1 and of each constant.
