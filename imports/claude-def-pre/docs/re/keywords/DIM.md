# DIM (id 288, group 7 dimension)

Selects which of two linked models receives the keywords that follow. DEF_PRE keeps a primary
model and a companion model of the other dimension (2D ↔ 3D). `DIM 2` / `DIM 3` sets a flag in
the primary model; while the flag is set, the KEY interpreter sends every group 1–6 keyword to
the companion model instead of the primary one. DIM does **not** change the dimension of either
model. It occurs in the multiple-operation master files (`.MST`, "Integrated 2D / 3D"), one
`DIM` per operation. There is no KEY writer for DIM.

## Handlers
read: `FUN_14037e530` (group-7 parser; accepts only id 0x120); write: not written (registry
handler2 is the stub `0x1402f75d0`; no call `FUN_1402f7720(0x120)` exists: all 517 call sites
of `FUN_1402f7720` load a constant other than 0x120 except two in `FUN_1402f5090`, which writes
keyword 299 only; the string "DIM" is not referenced as a standalone literal); accessor: none
(stub `0x1402f7610`).

## KEY grammar (read)
```
line 1: DIM <d:%i>       rest of the keyword line, FUN_140413de0(fp, &d, 0) (d zeroed first)
if d == 2 or d == 3: FUN_1401d8140(model, &d)
```
Other values of d (including a blank remainder, d = 0) do nothing. Return 0, or 2 if the
helper hit EOF / a blank line.

## Model mapping
`model` is the object the interpreter `FUN_1401b6520` got (group 7 is passed the model itself,
not `FUN_1401d81b0(model)`).

| Field | Offset (model) | Type | Meaning |
|---|---|---|---|
| dimension | +0x1c | int | 2 or 3 (`FUN_1400015e0` tests ==2, `FUN_1400015f0` tests ==3) |
| companion | +0x33e8 | model* | other-dimension model; NULL if none |
| use-companion flag | +0x33f0 | byte | read by `FUN_1401d81b0` |

`FUN_1401d8140(model, &d)`: if `companion != NULL`:
1. `FUN_1401d8050(model)` syncs the companion's dimension to the opposite of the model's:
   model 3D and companion not 2D → `companion->vtable[0](companion, &2)`; model 2D and companion
   not 3D → `vtable[0](companion, &3)` (vtable slot 0 is the set-dimension method; the KEY
   header line "DEFORM 3D"/"DEFORM-2D"… calls the same slot on both models the same way).
2. flag `+0x33f0` = (companion dimension `FUN_140158e90` = `+0x1c`) == d ? 1 : 0.
If there is no companion nothing happens.

Effect: `FUN_1401d81b0(model)` returns the companion when it exists and the flag is set, else
the model. `FUN_1401b6520` calls it before every group 1–6 parser (groups 1, 2, 3, 4, 5, 6 =
`FUN_14039aa90`, `FUN_14038b8a0`, `FUN_140388fb0`, `FUN_140384a50`, `FUN_140383120`,
`FUN_140378e00`). So in a model whose primary is 3D, `DIM 2` routes the following data to the
2D companion and `DIM 3` routes it back to the primary; in a 2D primary, `DIM 3` routes to the
companion and `DIM 2` back.

The flag is never reset by the interpreter (it is cleared only by `FUN_1401d7fd0`, called from
the model constructor, which also clears the companion pointer); it persists after the KEY file
ends. The companion is installed by `FUN_1401d8000(model, companion)` from `FUN_14001c770`, which
builds two models at +0x190 and +0x4eb0 of its object (both `FUN_1401c8950(…, 3)`) and links the
second as companion of the first.

## Defaults
Flag 0 and companion NULL from the model constructor (`FUN_1401d7fd0`); in DEF_PRE's main
object the companion is set by `FUN_14001c770`. Without DIM all keywords go to the primary.

## Validation, errors, return codes
No messages. 2 on EOF/blank line; any other id → 2 (not reachable). Values other than 2/3
ignored silently.

## Unit conversion
none found

## KEY grammar (write)
Not written. Confirmed: registry handler2 is the stub, the KEY writers (`FUN_1401c5af0`,
`FUN_1401c5ea0`, …) have no inline DIM, and no code calls the keyword-name lookup with 0x120 (see
Handlers). The routing state is therefore lost on a KEY round trip; the companion's data are
written only if some other writer handles the companion.

## Helpers
- `0x14037e530` - group-7 parser
- `0x140413de0` - one-int line helper
- `0x1401d8140` - set routing flag
- `0x1401d8050` - sync companion dimension
- `0x140158e90` - get model dimension (+0x1c)
- `0x1401d81b0` - pick model for groups 1–6
- `0x1401d8120` - get companion (used by the interpreter for the header line)
- `0x1401d8000` - set companion; `0x1401d7fd0` - clear companion and flag

## Corpus check
No `.KEY` file contains DIM. Found in 6 `.MST` master files
(`key/2D/{EN,SI}/MultipleOperations/New_MO_Release_Examples_*/MO2_Boolean_Oprations/…MST`,
`…/MO2_Extrusion_and_Boolean_Cycles/…MST`, `…/MO2_Operation_Cycles2/…MST`), all
`DIM    \t2` (searched `^\s*DIM\s` with grep -a over `key/`, `materials/`, `db/`). The tab is
skipped by `sscanf "%i"`; one line consumed. Whether the .MST files go through
`FUN_1401b6520` was not traced.

## Open questions
- Who reads `.MST` files and whether DIM there uses this parser.
- Whether anything writes the companion model's data back to a KEY file.
