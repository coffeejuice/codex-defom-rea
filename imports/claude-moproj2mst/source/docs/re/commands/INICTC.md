# INICTC (registry id 5): clear the contact boundary codes of one object

Source: `DEF_PRE_64.exe` v11.0.2, `FUN_140378e00` `case 5` (`out/bm_cmd.c` lines 555-566) and
`FUN_1402049b0` (`out/p67/inictc.c`). Shared mechanics: [_common.md](_common.md).

## 1. Summary

`INICTC n` sets every **negative** BCCDEF code of object `n` to 0. Negative codes are the contact
codes (−object, and in 3D also −(200+object), see `objarrays.md` §6), so the command releases all
node-to-object contact conditions of the object before a new `GENCTC`. Positive codes (fixed /
velocity / function BCs) are not touched **[fact]**. Nothing else is cleared: no contact lists,
no pair data (CNTACT), no coordinates, no BCCDFN (`obj+0x998`) **[fact]**.

2D and 3D differ only in the number of codes per node: 2 in 2D, 3 in 3D **[fact]**.

## 2. Argument grammar

| Step | Code | Effect |
|---|---|---|
| 1 | `FUN_140413de0(f, &n, 0)` | `fgets` the rest of the keyword line, `sscanf("%i")` (hex/octal aware, commas → blanks). EOF or an empty rest of line (`sscanf == -1`) → returns 1 → the case returns **2** **[fact]** |
| 2 | `FUN_1401d6ea0(model, n)` | lookup only (`FUN_14017aaf0(model+0x1380, n)`); NULL → case returns **2** **[fact]** |
| 3 | `FUN_1402049b0(obj)` | the clear (below); status 0 → dispatcher returns 0 |

Exactly one line is consumed (the keyword line). `skipBlank` is 0, so `INICTC` followed by a
line break and the number on the next line fails (the next line is then read as a keyword).
A non-numeric token gives `sscanf == 0`, n = 0, and object 0 is normally not found → return 2
**[fact]** (`FUN_140413de0` behaviour from `group2-helpers.md`).

Return 2 makes the interpreter print `ERROR: Keyword INICTC Reading Error.` and continue
**[fact]** (see `_common.md` §1).

The object is not checked for type, mesh or emptiness (`obj+0xc == 7` slots are found too):
with no mesh `obj+0x880 = 0` and nothing happens **[fact]**.

## 3. Model fields

| Field | Use |
|---|---|
| `obj+15000` | owning model; `FUN_1400015e0(model)` → 2D (model `+0x1c == 2`) **[fact]** |
| `obj+0x880` | node count N **[fact]** |
| `obj+0x990` | BCCDEF array, `int[N*d]`, d = 2 (2D) or 3 (3D), node-major **[fact]** |

## 4. Algorithm (`FUN_1402049b0`, 209 bytes, read fully)

```
d = is2D(obj->model) ? 2 : 3
for i in 0..N-1:
  for k in 0..d-1:
    if bcc[i*d + k] < 0: bcc[i*d + k] = 0
```

d is taken from the dimension test, not from `model+0x1640` (the DOF width used by GENCTC);
both are 2/3 in practice **[inferred]**.

## 5. Outputs

No stdout output of its own. Returns 0 on success, 2 on read failure or missing object
(message printed by the interpreter). No files.

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---|---|
| `FUN_140413de0` | `%i` rest-of-line reader | | documented in `group2-helpers.md` |
| `FUN_1401d6ea0` | object lookup | 39 | read fully |
| `FUN_1402049b0` | clear negative BCCDEF codes | 209 | read fully |
| `FUN_1400015e0` | is-2D | 8 | read fully |

## 7. Open items

None for the command itself. Corpus usage: always `INICTC <obj>` on the deforming (slave) object
right before `GENCTC` (e.g. `key/2D/EN/MultipassRolling/ROLL_EN.MST`, 3D `.d3mo` files with
`INICTC 5` / `INICTC 7` then `GENCTC 0.0195139`).

## 8. Tests

`tests/commands/INICTC/` (cases.json): object 2 of UPSET_Brick and UPSET_Tet, all objects of
SPIKE_SI (object 1 carries `-3` contact codes in the seed, the others none), a missing object
and an empty argument line (both → `ERROR: Keyword INICTC Reading Error.`).

Oracle run: all five cases as specified (`ERROR: Keyword INICTC Reading Error. ` — note the
trailing blank — for the missing object and the empty line, model unchanged; otherwise every
negative code of the object is 0 and all positive codes are kept: UPSET_Brick obj 2 −1 ×16 and
−3 ×16 cleared, UPSET_Tet obj 2 −1 ×25 and −3 ×25, SPIKE_SI obj 1 −3 ×14 and −2 ×1). No stdout,
stderr or file output of its own. In OUT.KEY the BCCDEF count shrinks because KFWRIT writes only
nodes with a nonzero code (46 → 28 rows for UPSET_Brick obj 2).
