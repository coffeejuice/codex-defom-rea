# IMPLUB — import lubricant (inter-object data) for one object pair

Shared mechanics: [common.md](common.md). Decompile: `deform-rea-work/out/p89/implub.c`.
Pair layout: see [GENINT.md](GENINT.md) and `claude-deform-rea-dbmap/docs/re/dbmap/interobj.md`.

## 1. Summary

| | |
|---|---|
| Registry id | 16 |
| Dispatcher | `FUN_140378e00` case 0x10 (assembly checked at 0x14037d823) |
| Handler | `FUN_1401ac310(model, filename, &a, &b, &lubFlag)` |
| External program | none (in-process) |

Reads a KEY file into a temporary model, takes its first inter-object pair as template, and copies
the template's data into the pairs (a, b, flag 2) and (b, a, flag 1) of the model (creating them if
missing). Then rewrites the contact codes in BCCDEF of object b for master a, depending on
`lubFlag`.

## 2. Argument grammar

```
IMPLUB <a> <b> <f>     rest of keyword line, FUN_140413fb0(f, &a, &b, &f, NULL): "%i %i %i" (',' -> ' ')
<KEY file name>        fgets(1024) + FUN_140323c50 (trailing ' ', '\r', '\n' removed)
```

* The `%i %i %i` reader status is **not** stored (the following test reads the case status, still
  0): missing values are 0, an empty line is accepted.
* File-name line at EOF → 2. Otherwise the case returns 0 (handler result ignored).
* `lubFlag = (f != 0)` (bool passed by pointer).

Example: `IMPLUB 1 2 1` / `Lubricant.KEY`.

## 3. Behaviour (`FUN_1401ac310`)

1. Temporary model T (`FUN_1401c8950(T, 3)`), `FUN_1401b5f00(T, file, &1, &0)` (same as GENINT §4:
   ordinary KEY interpreter, `"ERROR: Keyword file %s open error!\n"` if it cannot be opened).
   Read status 0 → `tmpl` = first pair of T (`FUN_1401fac90(T+0x13c8, 1)`), else 0.
2. `P = FUN_1401fad60(model+0x13c8, a, b, 2)` (pair with `+0x168 == a`, `+0x16c == b`,
   `+0x170 == 2`). Missing → append (`FUN_1401faaa0`; failure → return 0) with `+0x170 = 2`,
   `+0x168 = a`, `+0x16c = b`. 3D model: `P+0x174 = 1`. `tmpl` → `FUN_1401f9e80(P, tmpl)` (copy all
   data, keep ids/flag; 3D `+0x174` taken from the template).
3. Same for `Q = (b, a, 1)`.
4. `obj = FUN_1401d6ea0(model, b)`; if found: `FUN_140201bc0(obj, &a, &lubFlag, model+0x1640)`.
5. Destroy T, return 1.

Existing pairs are kept and overwritten; the rest of the list is untouched (unlike GENINT).

### `FUN_140201bc0(obj, &a, &lubFlag, &w)` — BCCDEF contact codes

`obj+0x990` = BCCDEF array (objarrays.md row 12), `obj+0x880` = node count, `w = model+0x1640`
= ints per node. `obj+0x3a98` (decompiled as `param_1 + 15000`) is the owner-model pointer; the
3D test `FUN_1400015f0` and 2D test `FUN_1400015e0` are applied to that model. Nothing happens when a == 0. Let `L = -200 - a` (3D only), `C = -a`.
For every node row r[0..w-1], with a per-row `seen = false`:

| | lubFlag = 0 | lubFlag = 1 |
|---|---|---|
| 3D model | for each slot: `L` → if seen: 0, else `C`, seen; `C` → seen | for each slot: `C` → if seen: 0, else `L`, seen; `L` → seen |
| 2D model | for each slot: `C` → if seen: 0, else seen (keep) | at the first slot equal to `C`: every slot of the row that is 0 becomes `C`; next row |

So in 3D the flag switches the contact code of master a between "plain" `-a` and "lubricated"
`-200-a` and keeps one code per node; in 2D flag 0 removes duplicates of `-a` and flag 1 fills the
row's empty slots with `-a` (inferred meaning; behaviour exact). The 2D branches never use `L`
(which is uninitialised there).

## 4. File format

An ordinary KEY file (any keywords). Intended: one inter-object block, e.g.

```
*
*  DEFORM-2D V11.0 KEYWORD FILE (Qt)
*
CNTACT       1       2       1
FRCFAC       1       2       1       0    3.0000000000E-001
IHTCOF       1       2       0    1.1000000000E+001
```

Only the first pair created is used; its numbers do not matter. No lubricant library ships with
the v11.0.2 install (the `Lubricants` folders of the MO examples are empty); the corpus
`InterObject.KEY` files are suitable (see GENINT §4).

## 5–6. Launch / read-back

None.

## 7. Messages

`"ERROR: Keyword file %s open error!\n"` and KEY-interpreter messages for the file. Case return 0
except EOF on the file-name line (2).

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401ac310` | handler |
| `FUN_1401fad60(list, m, s, flag)` | find pair by master, slave, flag |
| `FUN_1401faaa0`, `FUN_1401f9e80`, `FUN_1401fac90` | append pair, copy template, k-th pair |
| `FUN_140201bc0` | BCCDEF contact-code rewrite |
| `FUN_1401b5f00`, `FUN_1401c8950`, `FUN_1401cb2c0` | KEY read into temporary model |
| `FUN_1401d6ea0` | find object |

## 9. Open questions

* Meaning of the `-200-a` code (inferred: lubricated / "with lubricant" contact in 3D BCCDEF).
