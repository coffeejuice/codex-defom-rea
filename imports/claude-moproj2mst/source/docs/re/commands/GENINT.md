# GENINT — rebuild all inter-object pairs from a template KEY file

Shared mechanics: [common.md](common.md). Decompile: `deform-rea-work/out/p89/genint.c`.
Inter-object entry layout: `claude-deform-rea-dbmap/docs/re/dbmap/interobj.md`,
`claude-deform-rea-kw13/docs/re/keywords/_common.md` (list at `M+0x13c8`).

## 1. Summary

| | |
|---|---|
| Registry id | 15 |
| Dispatcher | `FUN_140378e00` case 0xf |
| Handler | `FUN_1401ac020(model, filename)` |
| External program | none (in-process) |

Deletes every inter-object pair of the model, then creates, for every object i, the two pairs
(master i, slave 1, flag 2) and (master 1, slave i, flag 1), and copies the inter-object data
(friction, heat transfer, …) of the **first** pair found in the given KEY file into each of them.
3D skips i = 1; 2D does not (so 2D also gets the self pairs (1,1,2) and (1,1,1)).

## 2. Argument grammar

```
GENINT [anything]      rest of the keyword line is discarded
<KEY file name>        FUN_140413ab0(f, buf, 0x400): next line, '\n' cut, max 1023 chars,
                       trailing ' ', '\r', '\n' removed
```

The reader's status and the handler's return value are ignored: the case always returns 0, even at
EOF (then the file name is whatever was in the buffer — uninitialised stack; inferred harmless only
if the file then fails to open).

Example: `GENINT` / `InterObject.KEY`.

## 3. Behaviour (`FUN_1401ac020`)

1. `FUN_1401fade0(model+0x13c8)`: delete all pairs (`next` at pair+0x390; scalar deleting dtor),
   list head/tail/count = 0.
2. `n = model+0x1378` (number of object slots, `FUN_1401d6db0`).
3. Temporary model `T` = `_DB_STEP_DATA` ctor `FUN_1401c8950(T, 3)`. Read the file into it:
   `FUN_1401b5f00(T, file, &1, &0)` (see §4). If the read returns 0, `tmpl = FUN_1401fac90(T+0x13c8, 1)`
   = first pair of T's list (null if the file defines none); otherwise `tmpl = 0`.
4. For i = 1 .. n (skipping i == 1 when the model is 3D, `model+0x1c == 3`):
   * `A = FUN_1401faaa0(model+0x13c8)` (new 0x3a0-byte pair, appended); allocation failure → return 0.
     `A+0x170 = 2`, `A+0x168 = i`, `A+0x16c = 1`; 3D: `A+0x174 = 1`.
     If `tmpl`: `FUN_1401f9e80(A, tmpl)` = `FUN_1401f9900` copy with `+0x168/+0x16c/+0x170` restored
     (so in 3D `+0x174` becomes the template's value).
   * `B` likewise with `+0x170 = 1`, `+0x168 = 1`, `+0x16c = i`; 3D `+0x174 = 1`; template copy.
5. Destroy T (`FUN_1401cb2c0`), return 1.

Objects are not looked up: empty slots (`obj+0xc == 7`) also get pairs.

Fields copied by `FUN_1401f9900` (dst ← src): `+0x168,+0x16c,+0x170` (then restored), `+0x174`
(3D only), `+0x178`, `+0x180`, `FUN_1401f9d50`, `FUN_1401f9e10` (sub-objects), 2D only `+0x238` and
array `+0x240` (`FUN_14012eff0`), window list `+0x188` (`FUN_140269340`), doubles
`+0x108..+0x160`, array `+0x2b8`, `+0x330..+0x388`. Meanings: see interobj.md (CNTACT = `+0x170`
flag / `+0x174` 3D option; FRCWIN windows in `+0x188`; …).

KEY output consequence (inferred from the CNTACT writer rule "skip slave < master, and master ==
slave with flag 1"): pairs (i,1,2) for i > 1 are not written; (1,i,1) are written as `CNTACT 1 i …`.

## 4. File read (format)

`FUN_1401b5f00(T, name, &1, &0)`:
* `T+0x2f88 = T+0x2f89 = 0` (`FUN_1401d7f10`); `strcpy(T+0x2f8a, name)`.
* `fopen(name, "r")`; failure → `"ERROR: Keyword file %s open error!\n"` (stdout), status 1 (no
  template; pairs still created with ids/flags only).
* `T+0x1740 = 0`; `FUN_1401b6520(T, file)` — the ordinary KEY interpreter (any keyword, including
  commands, is executed against T); `T+0x1740 = 0`; fclose; `strcpy(T+0x2f8a, " ")`;
  `FUN_1401d7f40(T)` (post-read fix-ups).

So the file is a normal KEY file; in practice an `InterObject.KEY` of a Multiple-Operations
project: `CNTACT m s code`, `FRCFAC m s …`, `IHTCOF …`, `SEPRES …`, `INTRST …`, `CNTMTH …`
(3D), `FRCROT`, `SEPDEN` (2D). Only the first pair created while reading it is used (the pair
created by the first inter-object keyword; inferred `CNTACT` on line 1). Its master/slave numbers
are irrelevant. A `*  DEFORM-2D`/`DEFORM-3D` comment header sets T's dimension.

Example inputs (read-only):
* 2D: `C:\Users\alext\Projects\DEFORM_corpus\key\2D\SI\MultipleOperations\New_MO_Release_Examples_SI\MO2_Operation_Cycles2\Operations\Task00001\InterObject.KEY`
* 3D: `C:\Users\alext\Projects\DEFORM_corpus\key\3D\SI\Multiple_Operations\New_MO_Release_Examples_SI\Operations\Task00001\InterObject.KEY`

## 5–6. Launch / read-back

None.

## 7. Messages and returns

Only `"ERROR: Keyword file %s open error!\n"` and whatever the KEY interpreter prints for the
template file (`"WARNING: Keyword %s not found. Skip %s\n"`, `"ERROR: Keyword %s Reading Error. \n"`,
`"ERROR: Keyword %s Data Error. \n"`). Case return always 0.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401ac020` | handler |
| `FUN_1401fade0` | clear pair list |
| `FUN_1401faaa0` | append new pair (`FUN_1401f46e0` ctor, size 0x3a0) |
| `FUN_1401fac90(list, k)` | k-th pair (1-based) |
| `FUN_1401f9e80` / `FUN_1401f9900` | copy pair data keeping ids/flag / full copy |
| `FUN_1401c8950` / `FUN_1401cb2c0` | temporary `_DB_STEP_DATA` ctor / dtor |
| `FUN_1401b5f00` | open + run KEY interpreter on a file |
| `FUN_1401b6520` | KEY interpreter |
| `FUN_1401d6db0` | `model+0x1378` object count |

## 9. Open questions

* Whether the KEY interpreter creates the T pair before or after objects exist (T has no objects);
  the corpus InterObject files only contain inter-object keywords, which work on pairs alone (inferred).
* 2D self pairs (1,1,*) — effect on the solver input not checked.
