# LOCATM (id 231, group 2, case 0x57)

A numbered local atom (diffusion) boundary condition: an entry of the object's list at
`obj+0x2f70` (`LLWINDEF_DATA`, kind 6) with three tables, named by handler3 `FUN_1402aeda0`:
`"Env. Atom"` (`+0x5e0`), `"React. Coef."` (`+0x658`), `"Atom Flux"` (`+0x6d0`). Same machinery
as [LOCTMP](LOCTMP.md), which describes the shared list, reset and mode-1 table writer in full.

## KEY grammar (read)

```
LOCATM <objNo:%i> <id:%i> <v838:%i> <tAtom:%i> <tReact:%i> <tFlux:%i>
<Atom value line>   [Atom table body]
<React value line>  [React table body]
<Flux value line>   [Flux table body]
```

* Line 0: `FUN_140414200(f, 5 × int*, skipBlanks = 0)`: `%i` × 5, commas allowed, missing values 0.
* Tables (value line → shape → `FUN_14041a800` body, `_common.md` section 3):

| Table | value line | type 0 | type 1 | type 2 | type 3 |
|---|---|---|---|---|---|
| Atom `+0x5e0` | `FUN_140414990` `v` | constant `v` | 1-D, var 1, `(int)v` points | – | – |
| React `+0x658` | `FUN_140414a40` `v w` | constant `v` | 1-D, var 1, `(int)v` pts | 1-D, var 0xc, `(int)v` pts | 2-D, vars (1, 0xc), sizes `(int)v × (int)w` |
| Flux `+0x6d0` | `FUN_140414990` `v` | constant `v` | 1-D, var 1, `(int)v` pts | – | – |

* Unlisted type codes: value line consumed, nothing stored, no body. Same in 2D and 3D.

## Model storage

* `bc = FUN_140269bc0(obj+0x2f70, id)` (grow positional list to `id` entries, see LOCTMP);
  `FUN_14025d1a0(bc, 6)` (full reset: all ten tables to constant 0.0, `+0x174 = 6`,
  `+0x838 = 0`; keeps `+0x170`, `+0x840`); then `bc+0x838 = v838`.
* Table fields as in LOCTMP: constant → `T+4 = 0, T+8 = v`; 1-D → `T+4 = 1, T+0x10 = var,
  T+0x1c = (int)v`; 2-D → `T+4 = 2, T+0x10 = 1, T+0x14 = 0xc, T+0x1c = (int)v, T+0x20 = (int)w`.

## Read errors and validation

As LOCTMP: every helper failure → **2** immediately (partial entry kept); line 0 / value line EOF
or blank → 2; body EOF → 2; non-numeric first tokens give 0; `id <= 0` overwrites the first entry
or crashes on an empty list (NULL dereference); no messages.

## KEY writer

`FUN_1402ac560` second loop (after all `LOCTMP` entries; only reached if every `LOCTMP` handler
returned 0): for `i = 1 .. count(obj+0x2f70)` handler2 `FUN_1402aeb40(f, model, ref(obj, i))`.

```
"%-6s %7i %7i %7i %7i %7i %7i\n"   name, N, bc+0x170, bc+0x838, cAtom, cReact, cFlux
```

* `cAtom = (Atom.ndim != 0)`, `cFlux = (Flux.ndim != 0)`;
* `cReact`: `ndim 0` → 0; `ndim 1` → 1 if `var[0] == 1` else 2; other `ndim` → 3.

Then `FUN_1402f3130(f, T, 1)` for Atom, React, Flux (mode 1 table writer: constant →
`" %20.10E\n"`; 1-D → `" %7i\n"` count, then `" x y"` lines; 2-D → `" %7i %7i\n"` sizes, axes,
values — see LOCTMP). Status 0.

Round trip: exact for contiguous ids and listed type codes; gaps in the id sequence come back as
all-zero constant entries, unknown codes come back as constant 0 / code 0.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x57 (p_main.c lines 2894–2969).
* `FUN_140414200` (5 × `%i`), `FUN_140414990`, `FUN_140414a40`: line helpers; `FUN_14041a800`: body.
* `FUN_140269bc0`, `FUN_14025d1a0`, `FUN_140269900`, `FUN_14019a590`: list / entry helpers (LOCTMP).
* `FUN_1402ac560`: LOC* writer loop; `FUN_1402aeb40`: handler2; `FUN_1402f3130` (mode 1).
* handler3 `FUN_1402aeda0`: GUI table accessor by name; not used by the KEY writer.

## Corpus check

No corpus file uses LOCATM (`corpus_kw.tsv`: 0 files). Grammar from the code only.

## Open questions

* Meaning of `bc+0x838` and of variable code 0xc not evidenced here.
