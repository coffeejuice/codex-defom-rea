# LOCDEF (id 232, group 2, case 0x58)

A numbered local deformation boundary condition: an entry of the object's list at `obj+0x2fa0`
(`LLWINDEF_DATA`, kind 7) with a pressure table (`+0x748`), a direction int (`+8`) and a friction
table (`+0x20`); table names from handler3 `FUN_1402af1f0`: `"Pressure"` (`+0x748`),
`"Friction"` (`+0x20`). Shared list/reset/table-writer machinery: see [LOCTMP](LOCTMP.md).

## KEY grammar (read)

```
LOCDEF <objNo:%i> <id:%i> <v838:%i> <tPres:%i> <dir:%i> <tFric:%i>
<Pressure value line>  [Pressure table body]
<Friction value line>  [Friction table body]
```

* Line 0: `FUN_140414200(f, 5 × int*, skipBlanks = 0)`: `%i` × 5, commas allowed, missing 0.
* Pressure: value line `FUN_140414990` (`v`); `tPres` 0 → constant `v`; 1 → 1-D over var 1 with
  `(int)v` points; other codes → nothing stored. Then `FUN_14041a800` body.
* `dir`: 1, 2 or 3 → stored; any other value → ignored (keeps the previous value).
* Friction: value line `FUN_140414c20` (**three** doubles `v w u`, the extra ones ignored for
  types 0–4):

| `tFric` | shape of the table at `bc+0x20` |
|---|---|
| 0 | constant `v` |
| 1 | 1-D, var 1, `(int)v` points |
| 2 | 1-D, var 4, `(int)v` points |
| 3 | 1-D, var 3, `(int)v` points |
| 4 | 1-D, var 0x15, `(int)v` points |
| 5 | 3-D, vars (4, 3, 0x15), sizes `(int)v × (int)w × (int)u` |
| other | nothing stored |

  then `FUN_14041a800` body (3-D: the three axes, then `Πcnt` values).
* Same in 2D and 3D.

## Model storage

* `bc = FUN_140269bc0(obj+0x2fa0, id)`; `FUN_14025d1a0(bc, 7)`; `bc+0x838 = v838` (see LOCTMP).
* The reset re-initialises the pressure table `+0x748` (and the other nine window tables) but
  **not** `bc+8` and **not** the friction table at `bc+0x20`: those belong to the base part
  constructed by `FUN_1401fb020` (`bc+8 = 1`, friction table template 0x15 = constant 0.0, a second
  template-0x15 table at `bc+0x98` unused here) and keep their state across re-reads of the same id.
  Consequences: an invalid `dir` keeps the previous (or constructor, 1) direction; an invalid
  `tFric` keeps the previous friction table; when switching e.g. from type 5 to type 2 the stale
  `var[1..2]`/`cnt[1..2]` remain but are unused (`ndim` = 1).
* Friction fields: constant → `+0x24 = 0, +0x28 = v`; 1-D → `+0x24 = 1, +0x30 = var,
  +0x3c = (int)v`; 3-D → `+0x24 = 3, +0x30/+0x34/+0x38 = 4/3/0x15, +0x3c/+0x40/+0x44 = (int)v/w/u`.
  Its `ncomp` stays 1 (template 0x15 falls to the default).
* Pressure fields: constant → `+0x74c = 0, +0x750 = v`; 1-D → `+0x74c = 1, +0x758 = 1,
  +0x764 = (int)v`.

## Read errors and validation

As LOCTMP: each helper failure → **2** at once (partial entry kept); EOF/blank line 0 or value
line → 2; body EOF → 2; `id <= 0` overwrites entry 1 or crashes on an empty list; `dir` and type
codes are not reported when invalid (silently ignored); no messages.

## KEY writer

`FUN_1402ac560` third loop (after LOCTMP and LOCATM, only if all their handlers returned 0): for
`i = 1 .. count(obj+0x2fa0)` handler2 `FUN_1402aef30(f, model, ref(obj, i))`.

* `cPres = (Pressure.ndim != 0)`.
* `bc+8` must be 1, 2 or 3; otherwise the handler returns **3 without writing anything**, and
  `FUN_1402ac560` stops (later LOCDEF entries of this object are not written); `FUN_14029e250`
  ignores that status and continues with the EC block. (Not reachable from KEY input, since the
  reader only stores 1–3 and the constructor sets 1.)
* `cFric`: `ndim 0` → 0; `ndim 1` with `var[0]` 1/4/3/0x15 → 1/2/3/4; `ndim 3` → 5;
  `ndim 1` with another variable, or `ndim 2` → an **uninitialised** local is printed (undefined;
  not reachable from KEY input).

```
"%-6s %7i %7i %7i %7i %7i %7i\n"   name, N, bc+0x170, bc+0x838, cPres, bc+8, cFric
```

then `FUN_1402f3130(f, bc+0x748, 1)` and `FUN_1402f3130(f, bc+0x20, 1)` (mode 1 table writer, see
LOCTMP: constant → `" %20.10E\n"`; 1-D → `" %7i\n"` + `" x y"` lines; 3-D →
`" %7i %7i %7i\n"` sizes, three axis lines, values with `maxCnt` per line).

Round trip: exact for contiguous ids and valid codes. For the friction value line the writer
prints one value (constant or count) or three sizes; the reader's `%Flf %Flf %Flf` accepts both.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x58 (p_main.c lines 2970–3049).
* `FUN_140414200`, `FUN_140414990`, `FUN_140414c20` (3 doubles): line helpers; `FUN_14041a800`: body.
* `FUN_140269bc0`, `FUN_14025d1a0`, `FUN_140269900`, `FUN_14019a590`: list/entry helpers.
* `FUN_14025c100`: `LLWINDEF_DATA` constructor → `FUN_1401fb020` base (sets `+8 = 1`, tables at
  `+0x20`, `+0x98` with template 0x15).
* `FUN_1402ac560`: LOC* writer loop; `FUN_1402aef30`: handler2; `FUN_1402f3130` (mode 1).
* handler3 `FUN_1402af1f0`: GUI table accessor (`"Pressure"`, `"Friction"`); not used for KEY output.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 10 occurrences of LOCDEF.

`C:/Users/alext/Projects/DEFORM_corpus/key/2D/EN/MultipleOperations/New_MO_Release_Examples_EN/MO2_Boolean_Oprations/Objects/Object00003.KEY`
(8 files use LOCDEF, all `tPres 0, dir 1, tFric 0`): parses exactly — header, two constant lines:

```
LOCDEF       3       1       0       0       1       0
    0.0000000000E+000
    0.0000000000E+000
```

## Open questions

* Meaning of `dir` 1/2/3, `bc+0x838` and variable codes 3, 4, 0x15 not evidenced in the code read.
* No corpus file exercises non-constant pressure/friction; the table paths are from the code only.
