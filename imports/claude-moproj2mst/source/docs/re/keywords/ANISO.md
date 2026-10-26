# ANISO (id 123, group 4 material)

Anisotropy data of a material (name-based; the models behind types 1–4 are **inferred**, e.g. a
6-coefficient yield function for type 1/4 and 3 coefficients such as r-values for type 2).
Sub-object at material `+0x1e40` (constructor `FUN_140289fe0`): a model type, one multi-component
DYNARY (`ncomp` coefficients per table point, optionally tabulated against temperature (var 3),
strain (var 7) or var 30), and a reference number for negative types.

## Handlers

* read: case `0x1f` of `FUN_140384a50`.
* write: `FUN_1403d0a10`, called by `FUN_1403cdca0` after MATDEN (… MATDEN ANISO GRNDAT …).
* accessor: `FUN_1403d0c00`: returns `mat+0x1e48` for any label (returns 0).

## KEY grammar (read)

```
L: ANISO <mat:%i> <t:%i> <nc:%i> <tv:%i>          (FUN_140413fb0 "%i %i %i")
t 0..4: +0x1e40 = t
t <  0: +0x1e40 = 5, +0x1ec0 = -t, return 0       (nothing else read)
t >  4: +0x1e40 unchanged, continue
re-init (only t 1..4), then D.ncomp = nc:
   t 1: FUN_14012fc70(D, 0x68)  ndim 1, var0 0, ncomp 6
   t 2: FUN_14012fc70(D, 0x69)  ndim 1, var0 0, ncomp 3
   t 3: FUN_14012fc70(D, 0x67)  ndim 0, ncomp 1, not allocated
   t 4: FUN_14012fc70(D, 0x65)  ndim 0, ncomp 6, not allocated
table variable (applies for every t >= 0):
   tv 1: ndim 1, var0 3;  L: <n0:%i>              (FUN_140413de0, no blank-line skip)
   tv 2: ndim 1, var0 7;  L: <n0:%i>
   tv 3: ndim 2, var 7, 3;  L: <n0:%i> <n1:%i>    (FUN_140413ee0)
   tv 4: ndim 2, var 3, 30; L: <n0:%i> <n1:%i>
   other tv: no line, shape left from the re-init (or from before, for t 0 / t > 4)
DYN(D):  ndim 0: nc values if nc >= 2 (none if nc < 2)
         ndim 1: n0 rows "<x> <v1> .. <v_nc>"
         ndim 2: axis0 (n0), axis1 (n1), nc*n0*n1 values
```

## Model mapping

| KEY field | Offset | Type | Notes |
|---|---|---|---|
| t | material +0x1e40 | int | 0..4, 5 = negative reference |
| D | +0x1e48 | DYNARY 0x68/0x69/0x67/0x65 | |
| ndim | +0x1e4c | int | from tv (or type default) |
| var0 / var1 | +0x1e58 / +0x1e5c | int | 3, 7 / 3, 30 |
| n0 / n1 | +0x1e64 / +0x1e68 | int | |
| nc | +0x1e88 (D+0x40) | int | overrides the type default ncomp |
| -t | +0x1ec0 | int | t < 0 |

Side effect (not at read time): `FUN_1401d64f0(md, 0x2e)` (used by the state-variable layout
`FUN_1401d0320`) is true when the model dimension code is 2 or any material has `+0x1e40` in
{1, 2, 4, 5}. ANISO's reader does not call `FUN_1401d0fc0` itself.

## Defaults

`FUN_140289fe0`: `+0x1e40 = 0`, D type 0 (ndim 0, ncomp 1), `+0x1ec0 = 0` → nothing written.

## Validation, errors, return codes

* Header EOF/whitespace → 2; n-line EOF/whitespace → 2; body EOF → 2. No return 3 on read.
* Unknown t / tv silently accepted.

## Unit conversion

None found.

## KEY grammar (write)

`FUN_1403d0a10`, `name` = `"ANISO"`:

```
+0x1e40 == 5:  "%-6s %7i %7i\n"            name, mat, -abs(+0x1ec0)
+0x1e40 >  0:  (ndim, var0) (1,3) -> tv 1, (1,7) -> tv 2, (2,7) -> tv 3, (2,30) -> tv 4,
               anything else -> return 3 (nothing written, KEY writer aborted)
               "%-6s %7i %7i %7i %7i\n"    name, mat, t, nc (+0x1e88), tv
               FUN_1402f3130(fp, D, 1)     mode 1: ndim 1 -> " %7i\n" n0 then rows;
                                           ndim 2 -> " %7i %7i\n" n0 n1 (FUN_1402f2fe0), axes, values
+0x1e40 <= 0:  nothing
```

Read/write asymmetries:

* **tv 4 cannot be written**: the reader sets var0 = 3, var1 = 30, but the writer tests
  `var0 == 30` (instruction at `0x1403d0b46`: `CMP dword ptr [RAX+0x1e58],0x1e`). Any material read
  with tv 4 makes the KEY writer return 3. A port must reproduce or document this.
* t 1..4 without a valid tv (ndim 0 or var0 0) also return 3 on write.
* t 0 with a tv is read (table stored) but never written.
* Mode 1 skips the completeness check, so an n0 = 0 table is written as header + `" 0"` line.

## Helpers

* `0x140413fb0` - `"%i %i %i"` header; `0x140413de0` / `0x140413ee0` - n lines.
* `0x14012fc70` - DYNARY re-init; `0x14041a800` / `0x1402f3130` - body read / write.
* `0x140484ab8` - `abs`; `0x140289fe0` - sub-object ctor.

## Corpus check

No corpus file contains ANISO (`grep -rIl ANISO` over `DEFORM_corpus`: no match). Grammar
unverified against data; no oracle variant.

## Open questions

* Meaning of types 1–4 and of var 30.
* Whether the tv-4 write failure is reachable from the GUI (it would always abort KFWRIT).
