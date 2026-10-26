# DOESCL (id 14, group 6, `case 0xe` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1576-1665, listing
`0x14037c855..0x14037cc94`, closure `out/p67/doescl.c`). Shared mechanics: [_common.md](_common.md).
**[fact]** = read in code, **[inferred]** = deduced.

## 1. Summary

"DOE scaling": multiplies (or, for hammer / screw-press energy and efficiency, assigns) one
material or object quantity named by a keyword. Used by DOE studies to perturb inputs. Same code
in 2D and 3D **[fact]**.

```
DOESCL <factor>
<KEYWORD> i1 [i2 ... i7]
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the DOESCL line | `FUN_140414990(f, &factor)` | `fgets(1024)`, `,`→space, `sscanf("%Flf")`; factor zeroed first; returns 1 (→ **return 2**) only for EOF or a blank line; a non-numeric value gives factor 0.0 |
| next line | `FUN_140413900(f, &cnt, 8)` | tokenizer: `fgets` into the global buffer `0x14070d060`, `strtok(" ,\n\t\r")`, up to 8 tokens; `cnt` counts token 0 even when the line is empty; EOF → **return 2** |

* Token 0: keyword name → `id = FUN_1402f76c0(tok0, &group)` (registry lookup, exact name; not
  found or empty → **0x131 = 305**, group 0) **[fact]**.
* Tokens 1..7 → `ints[0..6] = strtol(tok, NULL, 10)` (base 10 only: `0x10` → 0, `1.9` → 1),
  only for `k < cnt`; the rest stay 0 **[fact]**.
* Always exactly two lines.

Quirk **[fact]**: between the two reads the case executes `FUN_140484f90(local_6128, local_6528)`
= `strcpy` of the stack buffer used by `case 0xb` (SYSTEM command line) into another `case 0xb`
buffer. Both are uninitialised in this call (each dispatcher call has a fresh frame), the result
is never used. A port must treat it as a no-op (the original could in theory overrun if the
garbage has no NUL in 1024 bytes).

## 3. Messages printed before the action (switch on `id − 0x5e`)

`%lf` = MSVC `printf("%lf")` (6 decimals). Printed to stdout **before** any validation:

| id | keyword | message |
|---:|---|---|
| 0x5e (94) | FSTRES | `Scale %lf FSTRES : Mat# %d\n` (factor, i1) |
| 0x5f (95) | YOUNG | `Scale %lf YOUNG  : Mat# %d\n` (two spaces before `:`) |
| 0x68 (104) | FPERV | `Scale %lf FPERV  : Mat# %d [%d %d]\n` (factor, i1, i2, i3) |
| 0x97 (151) | MOVCTL | i2 == 0: `Scale energy %lf MOVCTL : Obj# %d type %d\n` (factor, i1, 0); i2 == 1: `Assign energy ...type %d` (…, 1); i2 == 2: `Assign efficiency ...` (…, 2); other i2: no message |
| 0xaa (170) | NDTMP | `Scale %lf NDTMP  : Obj# %d\n` |
| 0xe6 (230) | LOCTMP | `Scale %lf LOCTMP : Obj# %d Win# %d [%d %d %d %d %d]\n` (factor, i1 … i7) |
| other | – | none |

The MOVCTL message wording is printed whatever the object's movement type is (see §4.2).

## 4. Action `FUN_1401ac710(model, id, &factor, 7, ints)` **[fact]**

Its return value is ignored; the case always returns **0** after the second line was read.

| id | lookup | then |
|---|---|---|
| 0x5e, 0x5f, 0x68 | `mat = FUN_14029c2c0(model+0x1328, i1)` (material list: number at `+0`, next at `+0x2200`) | NULL → `Error on reading DOESCL : material # %d is not defined.\n`; else print `VarFieldWidth[NODE_VAR_DAMAG]... %d\n` with `model+0x1668` (W[0x1c], a leftover debug line) and call `FUN_14029b3f0(mat, id, &factor, 7, ints)` |
| 0x97, 0xaa, 0xe6 | `obj = FUN_1401d6ea0(model, i1)` | NULL → `Error on reading DOESCL : object # %d is not defined.\n`; else `FUN_14021d9c0(obj, id, &factor, 7, ints)` |
| any other (incl. 305 = unknown name, and real keywords such as STRAIN) | – | `Error on reading DOESCL : undefined keyword definition\n`; nothing changed |

`S(T)` below = `FUN_140136220(T, &factor)` on a table object (`_common.md` §3): if `T.ndim == 0`
the constant `T+8 *= factor`; otherwise every value `T.values[k] *= factor` for
`k < FUN_14012ff20(T)` (= `ncomp × Π cnt`); abscissae are not touched. (For `ndim == 0` with
`ncomp >= 2` only the constant is scaled, not the value vector.)

### 4.1 Materials `FUN_14029b3f0`

| keyword | effect | spec of the data |
|---|---|---|
| FSTRES | only if the flow-stress mode `mat+0x50 == 1` (tabular σ(strain, rate, T), KEY FSTRES types 2 and 3): `S(mat+0x78)` — all flow-stress values × factor. Other modes: nothing | `docs/re/keywords/FSTRES.md` (branch `claude/project-thread-ujpfry`) |
| YOUNG | `S(mat+0x670)` (constant or table values) | YOUNG.md (same branch) |
| FPERV | `i2 != 0` → `S(mat+0x1ef0)` ("Body Force"); `i3 != 0` → `S(mat+0x1f68)` (second quantity) | FPERV.md (same branch) |

### 4.2 Objects `FUN_14021d9c0`

| keyword | effect |
|---|---|
| NDTMP | `t = FUN_140180c40(obj+0x8a0)` = node field `obj+0x8e0` (id 8, created on demand); if non-NULL: `t[k] = t[k] * factor` for `k < obj+0x880` (one value per node; the width W[8] = 1 is not used) |
| LOCTMP | `w = FUN_140269900(obj+0x2f40, i2)`: the `i2`-th entry (positional, 1-based; `i2 <= 1` gives the first; past the end → NULL → nothing, no message). Then `i3 != 0` → `S(w+0x380)` Env temperature; `i4` → `S(w+0x3f8)` Conv. coef.; `i5` → `S(w+0x470)` Emissivity; `i6` → `S(w+0x4e8)` Heat flux; `i7 != 0` and byte `w+0x560 != 0` → `S(w+0x568)` Heat-trans. coef. (names: LOCTMP.md, branch `claude/project-thread-j4c7t9`) |
| MOVCTL | `FUN_1401954d0(obj+0x2160, &factor, 7, ints)` on the movement type `mv+0x68` = `obj+0x21c8`, below |

MOVCTL by movement type (`mv = obj+0x2160`; names from MOVCTL.md, branch `claude/project-thread-j4c7t9`):

| type | i2 == 0 | i2 == 1 | i2 == 2 | other i2 |
|---|---|---|---|---|
| 0 (force) | `S(mv+0x70)` = T@`obj+0x21d0` | same | same | same |
| 1 (speed) | `S(mv+0xe8)` = T@`obj+0x2248` | same | same | same |
| 2 (hammer) | energy `obj+0x2350 *= factor` | energy `obj+0x2350 = factor` | if the hammer table T@`obj+0x2368` has `ndim (obj+0x236c) == 0`: efficiency `obj+0x2358 = factor`; else nothing | nothing |
| 3 (screw press) | energy `obj+0x23e0 *= factor` | `obj+0x23e0 = factor` | if T@`obj+0x2400` `ndim (obj+0x2404) == 0`: efficiency `obj+0x23e8 = factor` | nothing |
| 8 | `S(mv+0x590)` = T@`obj+0x26f0` | same | same | same |
| 4-7, 9-11 | nothing (returns 0, ignored) | | | |

For types 0, 1 and 8 the "Scale energy / Assign …" message is misleading: the speed / force
table is scaled for every i2 **[fact]**.

## 5. Floating point

Every change is either one `MULSD` (`v * factor`, one rounding, order irrelevant) or a plain
store of `factor`. The factor itself comes from MSVC `sscanf("%Flf")`. No other arithmetic
**[fact]**.

## 6. Outputs and errors

| Situation | stdout | return |
|---|---|---|
| blank/EOF factor line, EOF on the keyword line | – | 2 (`ERROR: Keyword DOESCL Reading Error. `) |
| supported keyword, target exists | message of §3 (+ `VarFieldWidth…` for materials) | 0 |
| material / object missing | message of §3, then `Error on reading DOESCL : material # %d is not defined.` / `object # %d …` | 0 |
| unsupported or unknown keyword | `Error on reading DOESCL : undefined keyword definition` | 0 |

## 7. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140414990` | `%Flf` rest-of-line reader | 164 | read fully |
| `FUN_140413900` / `FUN_140413a50` | tokenizer / token i (`""` if missing) | 320/76 | read fully |
| `FUN_14047b6c0` | `strtol(s, NULL, 10)` | 11 | read |
| `FUN_1402f76c0` | registry lookup, not found → 0x131 | 82 | read (`FUN_1402f78d0` search not followed) |
| `FUN_140484f90` | `strcpy` | 183 | read |
| `FUN_1401ac710` | dispatcher of the action | 365 | read fully |
| `FUN_14029c2c0` | material find (`+0`, next `+0x2200`) | 72 | read fully |
| `FUN_14029b3f0` | material scaling | 198 | read fully |
| `FUN_14021d9c0` | object scaling | 472 | read fully |
| `FUN_1401954d0` | movement scaling | 493 | read fully |
| `FUN_140136220` | scale a table | 149 | read fully |
| `FUN_14012ff20` | table value count | 200 | from `_common.md` §3 |
| `FUN_140269900` | i-th list entry (next `+0x840`) | 88 | read fully |
| `FUN_140180c40` | NDTMP field accessor (`c+0x40`) | 108 | read |

## 8. Open items

* `FUN_1402f78d0` (name search) assumed to be the exact, case-sensitive match used by the KEY
  loop **[inferred]**.

## 9. Oracle check (2026-10-21)

All 22 cases agree (`docs/re/commands/oracle/ov-misc.md`): every message text, the
`VarFieldWidth[NODE_VAR_DAMAG]... 0` line after each material message when the material exists
(W[0x1c] = 0 in both seeds; not printed for a missing material), the `Error on reading DOESCL`
lines, and every changed value (FSTRES table: all 18 values × 1.1 to the printed digit, abscissae
untouched; hammer / screw energy scale and assign, efficiency assign, speed table scaled for
i2 = 0 and 2, no message and no change for i2 = 3).
