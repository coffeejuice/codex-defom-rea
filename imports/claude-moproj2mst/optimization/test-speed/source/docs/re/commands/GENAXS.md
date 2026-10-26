# GENAXS (id 19, group 6, `case 0x13` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1751-1759, dispatcher
listing `0x14037dc56..0x14037dcc0`, `FUN_1401af680` decompile `out/p67/genaxs.c` and listing
`0x1401af680..0x1401b05a0`). Shared mechanics: [_common.md](_common.md). Facts are marked
**[fact]**, deductions **[inferred]**.

## 1. Summary

Cogging set-up helper. For the fixed object layout of the cogging templates (object 1 = workpiece,
2 = upper die, 4/5 and 6/7 = two manipulator gripper pairs with spring movement, MOVCTL type 5)
it

1. undoes a spring pre-displacement of each gripper pair (and moves the workpiece with it),
2. decides for each gripper pair whether it is clear of the die in X,
3. opens (moves apart by the workpiece height) every pair that is **not** clear, and
4. replaces the workpiece's type-7 `WPAXIS` records by one record per clear pair (holding range
   in X).

Only `GENAXS 7 ...` does anything: the layout code 7 is the only one implemented **[fact]**.
It is C++ code (no Fortran call, no `findaxis.f` routine on this path) **[fact]**. The code does
not branch on the model dimension; it is written for 3D (2D see section 7) **[inferred]**.

Corpus: `GENAXS 7 15 15` (984) and `GENAXS 7 10 10` (375), always right after
`OBJPOS 4 14 5 0 1e-05 1` / `OBJPOS 6 14 7 0 1e-05 1` in
`key/3D/EN/Cogging/Cogging_Multiple_Ops_Lab{1,2}/*.MST`. The result is visible in the corpus
`Cogging_Multiple_Ops_Lab2/_TEMP_STEP.KEY` line 7324: `WPAXIS 1 1 7` / `1 0 0` / `0 0 0` /
`9.957... 6 7 -90 -80` (a record written by this command, section 4 step 9) **[fact]**.

```
GENAXS <layout> <tolA> <tolB>
```

## 2. Argument grammar

| Line | Reader | Content |
|---|---|---|
| rest of the GENAXS line | `FUN_140415680(f, &layout, &tolA, &tolB)` | outputs zeroed, `fgets(0x400)`, `,` → space, `sscanf("%i %Flf %Flf")` |

* The dispatcher sets `layout = 0` first (`0x14037dc56`) **[fact]**.
* Quirk **[fact]**: the reader result is discarded; the test after it reads the dispatcher status
  `[RSP+0x12d4]` (0 at entry). EOF / blank → all 0 → `layout 0` → no-op, status 0.
* Exactly one line consumed. `FUN_1401af680(model, layout, tolA, tolB)` gets the values by value
  (`XMM2`, `XMM3`).

## 3. Model / object fields

Objects are looked up with `FUN_1401d6ea0(model, n)` for n = 1, 2, 4, 5, 6, 7 (in that order).
**There is no NULL check**: if any of the six objects is missing the code dereferences NULL
(access violation) **[fact]**. Names below: `W` = obj 1, `D` = obj 2, `G4..G7`.

| Field | Access | Meaning |
|---|---|---|
| `obj+0x2588` | read, written 0 (G4..G7) | MOVCTL type-5 (spring) value `t2` (MOVCTL.md type 5: `+0x2588/+0x2590/+0x2598 = atof(t2/t3/t4)`) |
| `obj+0x21b0..+0x21c0` | read (G4, G6) | movement direction (MOVCTL header h0..h2) |
| `obj+0x2300..+0x2310` | written 0 (G4..G7, only in step 1) | FORCE vector |
| `obj+0x3990` | read | byte flag: object has die geometry (`FUN_140160d90`); selects the bounding-box source |
| `obj+0x3c00[0]` | read | first die surface (`FUN_140198620(obj, &0)`) |
| `obj+0x8a0` | read | mesh block (node coordinates; `+0x12c` node count) |
| `W+0x20a0` | rewritten | WPAXIS list (records 0xe8 bytes, `+0` id, `+4` type, `+0xe0` next; WPAXIS.md, branch j4c7t9) |
| geometry of W, G4..G7 | moved | by `FUN_14021d0b0` (see step 1 / 8) |

## 4. Algorithm (`FUN_1401af680`)

Constants: `1e-6` (`0x14051f1b8`), sign mask (`0x14051bec0`), `0.5` (`0x14051f240`), `-1.0`
(`0x140522fc8`), translation zero tolerance `1e-30` (`0x14051bee0`), `±1e29` (`0x14052c598`,
`0x14052c5a0`). `fabs` = CRT `FUN_140484558`, `sqrt` = CRT `FUN_140484644` (`sqrtsd`).

0. `if (layout != 7) return;` **[fact]**.
1. **Spring reset** `v = (0,0,0)` **[fact]**:
   * if `fabs(G4+0x2588) > 1e-6`: `v_i = (-(G4+0x2588)) * G4.dir_i` (sign flip by `XORPD`, then
     `MULSD`; i = x, y, z); translate G4 by `v` (`FUN_14021d0b0(G4, v, &0, &1)`), set
     `G4+0x2588 = 0` and FORCE `G4+0x2300..0x2310 = 0`; translate G5 by the same `v`, zero its
     `+0x2588` and FORCE.
   * the same for the pair G6/G7 with **G6**'s spring value and direction (overwrites `v`).
   * translate W by `v` (`FUN_14021d0b0(W, v, &0, &1)`): W follows the **last** pair that was
     reset (G6/G7 if both), or does not move (`v = 0`, ignored by the zero test of
     `FUN_14021d0b0`).
2. **Bounding boxes** `bbox(obj) -> (lo[3], hi[3])` **[fact]**:
   * if `obj+0x3990 != 0`: `FUN_1402faca0(surface0)`: start `lo = 1e29`, `hi = -1e29`, for every
     point (3 doubles; count = vector size / 3) `if (p < lo) lo = p; if (p > hi) hi = p` per axis;
   * else `FUN_140181ae0(obj+0x8a0)`: per axis over `ndim = *(int*)(model+0x15f8)` components:
     first node sets `lo = hi`, later nodes `if (v < lo) lo = v; else if (v > hi) hi = v`. **If the
     object has no mesh it returns without writing**: `lo`/`hi` keep the values of the previous
     bbox call (uninitialised stack for the first call) **[fact]**.
   Calls, in order: `D`, `G4`, `G6` (each by the flag), then `W` (mesh only), then `G5` (by flag).
   Kept: `Dlo = D.lo.x`, `Dhi = D.hi.x`; `c4 = (G4.lo + G4.hi) * 0.5` (all three axes, `ADDSD`
   then `MULSD`), `A4lo = G4.lo.x`, `A4hi = G4.hi.x`; `A6lo = G6.lo.x`, `A6hi = G6.hi.x`. [listing-checked]
3. **Overlap flags** (listing `0x1401afd90..0x1401afe30`; `COMISD`, NaN gives false) **[fact]**:
   ```
   blockedA = (Dhi >= A4hi) || (Dhi >= A4lo) || (Dlo >= A4hi) || (Dlo >= A4lo);  // false iff pair 4 lies wholly at +X of D
   blockedB = (A6hi >= Dhi) || (A6hi >= Dlo) || (A6lo >= Dhi) || (A6lo >= Dlo);  // false iff pair 6 lies wholly at -X of D
   ```
4. **Centre distance** (`0x1401afe37..0x1401afefc`) **[fact]**:
   `cD = 0.5*(Dhi + Dlo)`, `c4x = 0.5*(A4hi + A4lo)`, `c6x = 0.5*(A6hi + A6lo)` (`ADDSD` then
   `MULSD 0.5`); `if (tolA >= fabs(cD - c4x)) blockedA = true;`
   `if (tolB >= fabs(cD - c6x)) blockedB = true;`. [listing-checked]
5. **Workpiece height** `h = fabs(W.hi.y - W.lo.y)` (bbox of W's mesh; stale G6 values if W has no
   mesh) **[fact]**.
6. **Opening vectors** **[fact]**: `c5 = (G5.lo + G5.hi) * 0.5`; `u = c5 - c4` (`FUN_14023b1f0`,
   per component `a - b`); normalise (`FUN_14023b720`: `len = sqrt((x*x + y*y) + z*z)`, if
   `len != 0` divide each component by `len` with `DIVSD`; a zero vector stays zero);
   `u = u * (-h)` (`FUN_14023b2d0`, per component `MULSD`); `w = u * (-1.0)`. Both pairs use the
   direction of pair 4/5. [listing-checked]
7. Delete all type-7 records of `W+0x20a0` (`FUN_140259ec0`: count them, then unlink and destroy
   them one at a time with `FUN_140259a40` / `FUN_14025bf40`) **[fact]**.
8. **Branch** **[fact]**:

   | blockedA | blockedB | Moves (`FUN_14021d0b0(obj, vec, &1, &1)`) | WPAXIS records |
   |---|---|---|---|
   | true | true | G4 by `u`, G5 by `w`, G6 by `u`, G7 by `w` | none |
   | false | false | none | id 1 = (4, 5, A4lo, A4hi); id 2 = (6, 7, A6lo, A6hi) |
   | false | true | G6 by `u`, G7 by `w` | id 1 = (4, 5, A4lo, A4hi) |
   | true | false | G4 by `u`, G5 by `w` | id 1 = (6, 7, A6lo, A6hi) |

9. **Record** `e = FUN_140259df0(W+0x20a0, id)`: finds a record with type 7 and `+0 == id`; none
   exist after step 7, so it appends a new 0xe8-byte record (`FUN_140259950(list, 0)`:
   constructor `FUN_140258580`, appended at the tail, `+0 = new count`), then sets `+4 = 7`,
   `+0 = id`. Fields **[fact]**: `+0x08..+0x18 = (0, 0, 0)`, `+0x20..+0x30 = (1.0, 0, 0)`,
   `+0x40 = 0.5 * h`, `+0xc8`/`+0xcc` = pair object numbers, `+0xd0`/`+0xd8` = X range of the
   pair's first gripper. Record id 2 is written only if record 1 was obtained (non-NULL). The
   WPAXIS KEY writer prints `e+0` as the id, so after an existing type-1 record with id 1 the new
   record also prints as `WPAXIS 1 1 7` (as in the corpus) **[fact]**.

`FUN_14021d0b0(obj, v, flagA, flagB)` is the OBJPOS translation core (see OBJPOS.md): it does
nothing when every component satisfies `fabs(v_i) < 1e-30` (3 components, 2 when
`obj+0x39c0 == 2`); otherwise translates the die geometry if `*flagB` (`FUN_1402000a0`), the
mesh and attached sub-objects (`FUN_140200610`), and the remesh / BC-window / movement / process
/ `+0x2be8` sub-objects plus model-level items (`FUN_14021d190`; `*flagA` adds
`FUN_14019ef20(model, v, objNo)`) **[fact]**, not followed further here.

Status: always 0 (`iVar4` is the untouched dispatcher status) **[fact]**.

## 5. Outputs

No stdout, no files. Changes: positions of W, G4..G7 (and everything `FUN_14021d0b0` moves),
`+0x2588` and FORCE of reset grippers, type-7 WPAXIS records of object 1. Return code 0. A missing
object 1, 2, 4, 5, 6 or 7 crashes DEF_PRE (no message).

## 6. Helpers

| Address | Role | Size | Status |
|---|---|---:|---|
| `FUN_140415680` | `%i %Flf %Flf` reader | | group2-helpers.md |
| `FUN_1401af680` | GENAXS body | 3867 | read fully, FP from listing |
| `FUN_1401d6ea0` / `FUN_14017aaf0` | object lookup | 39 / 85 | read fully |
| `FUN_140484558` / `FUN_140484644` | CRT `fabs` / `sqrt` | | read |
| `FUN_14021d0b0` | translate object | 211 | summarised (OBJPOS.md) |
| `FUN_140181ae0` | mesh bounding box | 452 | read fully |
| `FUN_1402faca0`, `FUN_1400034d0`, `FUN_14026a470` | die-surface bounding box, point count, point access | 771 / 35 / 41 | read fully |
| `FUN_140160d90`, `FUN_140198620` | `obj+0x3990` flag, surface 0 | 17 / 32 | read fully |
| `FUN_14023b1f0`, `FUN_14023b720`, `FUN_14023b610`, `FUN_14023b2d0` | vector sub, normalise, length, scale | | read fully (listing) |
| `FUN_140259ec0`, `FUN_140259a40` | delete type-7 WPAXIS records | 197 / 279 | read fully |
| `FUN_140259df0`, `FUN_140259e60`, `FUN_140259950` | find-or-append WPAXIS record | 97 / 82 / 225 | read fully |
| `FUN_140258ed0`, `FUN_140258f50` | copy 3 doubles to `e+8` / `e+0x20` | 54 / 54 | read fully |
| `FUN_140258580`, `FUN_14025bf40` | WPAXIS record ctor / dtor | | not followed (defaults also open in WPAXIS.md) |

## 7. Open items

* 2D: the bbox helper copies only `ndim` (2) components, so the z parts of `c4`, `c5` (and of
  `u`) come from earlier calls / the stack; the result is undefined. Not seen in the corpus.
* Behaviour with a missing object is a crash; the tests avoid it.
* The defaults of the new WPAXIS record's other fields (`FUN_140258580`).

## 8. Tests

`tests/commands/GENAXS/`, seed `Cogging_Multiple_Ops_Lab2/_TEMP_STEP.KEY` (objects 1..9; 4..7
spring grippers, `+0x2588 = 0` in the seed): `corpus15.KEY` (`GENAXS 7 15 15`), `corpus10.KEY`
(`GENAXS 7 10 10`), `tol0.KEY` (only the overlap tests), `tolbig.KEY` (both pairs blocked → both
opened, type-7 records deleted), `objpos14.KEY` (the corpus sequence OBJPOS mode 14 + GENAXS; OBJPOS 14 leaves `+0x2588` at 0,
so the spring reset does **not** trigger — corrected after oracle run: OBJPOS 14 closes pair 4/5
by `-u` and GENAXS opens it again by `u`), `layout6.KEY` (layout ≠ 7 → no change), `empty.KEY` (no
arguments → no change).

Oracle results (2026-10-21, `docs/re/commands/oracle/ov-misc.md`): bounding boxes, flags, `h`,
`u` and every moved die point recomputed from the seed match to the printed digit
(`ov-misc/genaxs.py`). Seed values: D x = -5..5, pair 4 x = -20..-10 (blocked by the overlap
test, whatever the tolerance), pair 6 x = -90..-80 (|cD - c6x| = 85), h = 19.914211275,
u = (0, -18.3739365620, 7.6794704193). `cases_v_ov-misc.json` covers the other branches:
`v_clearA_tol10` (pair 4/5 moved +30 by OBJPOS: both clear, records (4 5 10 20) and
(6 7 -90 -80), nothing moved), `v_clearA_tol15` (|cD - c4x| = 15 <= 15 blocks: `>=` confirmed),
`v_clearA_Bblk` (pair 6 opened with pair 4/5's direction, record (4 5 10 20)), `v_tolB85` /
`v_tolB8499` (equality on tolB), `v_spring` (spring values 2.5 / 1.5: G4, G5 by (-2.5,0,0), G6, G7
and W by (+1.5,0,0), springs and FORCE 4 zeroed, then the normal decision on the reset
positions; record (6 7 -88.5 -78.5)), `v_objpos14_ctl` (state after the two OBJPOS 14 lines).
The new records print their direction line `1 0 0` before the origin line `0 0 0` (WPAXIS writer
order).

