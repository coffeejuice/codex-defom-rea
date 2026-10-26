# LOCTMP (id 230, group 2, case 0x56)

A numbered local thermal boundary condition of the object: an entry of the object's list at
`obj+0x2f40` (an `LLWINDEF_DATA` object, 0x920 bytes, kind 5) holding five tables. Their names
come from handler3 `FUN_1402ae950`: `"Env Tempreature"` [sic] (`+0x380`), `"Conv. Coef."`
(`+0x3f8`), `"Emissivity"` (`+0x470`), `"Heat Flux"` (`+0x4e8`), `"Heat Trans. Coef."`
(`+0x568`, optional).

## KEY grammar (read)

```
LOCTMP <objNo:%i> <id:%i> <v838:%i> <tEnv:%i> <tConv:%i> <tEmis:%i> <tFlux:%i> <tHtc:%i>
<Env value line>    [Env table body]
<Conv value line>   [Conv table body]
<Emis value line>   [Emis table body]
<Flux value line>   [Flux table body]
[if tHtc >= 1:  <Htc value line>  [Htc table body]]
```

* Line 0 (rest of the keyword line): `FUN_1404144b0`, 7 × `%i`, commas allowed, missing values 0.
* Each table = one **value line** read by a scalar line helper, then the type code decides the
  table shape, then `FUN_14041a800` reads the body (nothing for a constant; `_common.md`
  section 3 for the body layout):

| Table | value line | type 0 | type 1 | type 2 | type 3 |
|---|---|---|---|---|---|
| Env `+0x380` | `FUN_140414990` `v` | constant `v` | 1-D, var 1, `(int)v` points | – | – |
| Conv `+0x3f8` | `FUN_140414a40` `v w` | constant `v` | 1-D, var 1, `(int)v` pts | 1-D, var 3, `(int)v` pts | 2-D, vars (1, 3), sizes `(int)v × (int)w` |
| Emis `+0x470` | `FUN_140414a40` `v w` | as Conv | as Conv | as Conv | as Conv |
| Flux `+0x4e8` | `FUN_140414990` `v` | constant `v` | 1-D, var 1, `(int)v` pts | – | – |
| Htc `+0x568` | `FUN_140414990` `v` | – | constant `v` (code 1!) | 1-D, var 1 (code 2) | 1-D, var 4 (code 3) |

  For Htc the codes are shifted: `tHtc < 1` → no Htc table and no line read; 1 = constant,
  2 = 1-D over var 1, 3 = 1-D over var 4.
* Body for a 1-D table: `n` pairs `x y` (`%Flf`, free format), then `fgets` of the rest of the
  last line; 2-D: `cnt0` abscissae, `cnt1` abscissae, `cnt0·cnt1` values, then `fgets`.
* A type code outside the listed values: the value line is still consumed but nothing is stored
  (the table stays the reset constant 0.0) and no body is read.
* `(int)v` truncates toward zero (`"5.9"` → 5). Same grammar in 2D and 3D.

## Model storage

* `bc = FUN_140269bc0(obj+0x2f40, id)`: the list (head `+0`, count `+0x14`, link `bc+0x840`) is
  extended until it holds `id` entries — each new entry is constructed (`FUN_14025c100`) and gets
  `+0x170` = its 1-based position — then entry number `id` (positional) is returned.
* `FUN_14025d1a0(bc, 5)`: full reset keeping only `+0x170` and the link `+0x840`: frees and
  re-initialises all ten tables `+0x380 … +0x7c0` to template 0 (`ndim 0`, `ncomp 1`, constant
  0.0), sets kind `+0x174 = 5`, `+0x838 = 0`, `+0x560 = 0`, window-shape fields cleared.
  A repeated `LOCTMP` with the same id therefore replaces the entry completely.
* `bc+0x838 = v838` (int; meaning not evidenced).
* Per table `T` (byte offsets inside the table, `_common.md` section 3): type 0 → `T+4 = 0`,
  `T+8 = v`; 1-D → `T+4 = 1`, `T+0x10 = var`, `T+0x1c = (int)v`; 2-D → `T+4 = 2`,
  `T+0x10 = 1`, `T+0x14 = 3`, `T+0x1c = (int)v`, `T+0x20 = (int)w`.
* `bc+0x560` (byte) = 1 if `tHtc >= 1`, else 0.

## Read errors and validation

Every helper failure returns **2** at once (no message from the case; the KEY loop prints
"Reading Error" and continues). The entry keeps what was stored so far.

* object number EOF / `< 1` → 2; line 0 EOF or blank → 2;
* a value line EOF or blank (`sscanf == -1`) → 2; a non-numeric first token is **not** an error
  (value 0);
* table body EOF → 2; non-numeric tokens in a body are not detected.
* `id <= 0`: the list is not extended and `FUN_140269900` returns the first entry; with an empty
  list it returns NULL and `FUN_14025d1a0` dereferences it (crash). A port should reject
  `id < 1`.

## KEY writer

`FUN_14029e250` → `FUN_1402ac560(model, f, obj, N)` (writer-map step 53, after `WINRSE`, before
the EC block; its status is ignored by the caller). First loop: for `i = 1 .. count(obj+0x2f40)`
handler2 `FUN_1402ae590(f, model, ref(obj, i))`; a nonzero status stops `FUN_1402ac560` (skipping
the rest, including `LOCATM`/`LOCDEF`).

`FUN_1402ae590`: `bc` = entry `i` (positional); `N` = `obj+0x3f24` if > 0 else the object number.
Header:

```
"%-6s %7i %7i %7i %7i %7i %7i %7i %7i\n"   name, N, bc+0x170, bc+0x838, cEnv, cConv, cEmis, cFlux, cHtc
```

* `cEnv = (Env.ndim != 0)`, `cFlux = (Flux.ndim != 0)`;
* `cConv`/`cEmis`: `ndim 0` → 0; `ndim 1` → 1 if `var[0] == 1` else 2; any other `ndim` → 3;
* `cHtc`: `bc+0x560 == 0` → 0; else `ndim 0` → 1; `ndim 1 && var 1` → 2; `ndim 1 && var 4` → 3;
  anything else → 1.

Then `FUN_1402f3130(f, T, 1)` for Env, Conv, Emis, Flux, and Htc only if `bc+0x560 == 1`.
Mode 1 of the table writer (not covered by `_common.md`, which describes mode 0):

* no `cnt >= 1` precheck;
* `ndim 0`: `ncomp == 1` → `" %20.10E\n"` (the constant `T+8`) — this is the value line;
  `ncomp >= 2` → the values through `FUN_1402f2e80(.., 5, 0)`; `ncomp < 1` → nothing;
* `ndim 1`: `" %7i\n"` (`cnt0`, the value line), then per point `" %20.10E"` x and
  `FUN_1402f2e80(f, ncomp, y, 5, 0)` (`" %20.10E"`… `"\n"`);
* `ndim 2..3`: `FUN_1402f2fe0(f, ndim, cnt, ndim, 0)` = `" %7i %7i[ %7i]\n"` (the value line),
  then each axis through `FUN_1402f2e80(f, cnt[i], axis, maxCnt, 0)`, then all values through
  `FUN_1402f2e80(f, total, data, maxCnt, 0)` (line break + 8 spaces every `maxCnt` values);
* `ndim > 3`: nothing.

Status 0 (3 only if the ref has < 2 slots, never from this caller).

Round trip: exact for well-formed input (the value line written for a 1-D/2-D table is the integer
count line that the reader parses as `%Flf`). Not exact: (1) entries are written for **every**
position `1..count`, so a file defining only `id 3` comes back with entries 1 and 2 as all-zero
constants (`LOCTMP N 1 0 0 0 0 0 0` / `LOCTMP N 2 …`, header codes 0, four ` 0.0000000000E+000`
lines); (2) unknown type codes come back as constant 0 / code 0.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x56 (p_main.c lines 2760–2893).
* `FUN_1404144b0` (7 × `%i`), `FUN_140414990` (1 double), `FUN_140414a40` (2 doubles): line helpers.
* `FUN_14041a800`: table body reader; `FUN_1402f3130` (mode 1) + `FUN_1402f2e80` / `FUN_1402f2fe0`: writer.
* `FUN_140269bc0(list, id)`: grow list to `id` entries, return entry `id`; `FUN_1402694a0`: append
  (`new 0x920` + `FUN_14025c100`); `FUN_140269900(list, k)`: k-th entry (1-based; `k <= 1` → head);
  `FUN_14019a590(list)`: count (`list+0x14`).
* `FUN_14025d1a0(bc, kind)` = `FUN_14025d0a0` (free) + `FUN_14025c350` (init); shared by every
  `LLWINDEF_DATA` keyword (WIN*, LOC*).
* `FUN_1402ac560`: LOC* writer loop; `FUN_1402ae590`: handler2.
* handler3 `FUN_1402ae950(model, name, ref, &out)`: returns the table pointer by the GUI names
  listed above (`"Env Tempreature"`, …); GUI accessor only, not called by any KEY writer, so it
  does not affect KEY output.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 44 occurrences of LOCTMP.

`C:/Users/alext/Projects/DEFORM_corpus/key/2D/SI/viewfactor/Viewfactor3_SI.KEY` (12 files use
LOCTMP, all with type codes 0): parses exactly — 7 ints, four constant lines, no Htc line:

```
LOCTMP       1       1       0       0       0       0       0       0
  1.0000000E+002
  0.0000000E+000
```

Corpus files with several entries (`LOCTMP 4 1 …`, `LOCTMP 4 2 …`, `LOCTMP 4 3 …`) number them
1..n contiguously, consistent with the positional list.

## Open questions

* Meaning of `bc+0x838` (written back verbatim) and of the variable codes 1, 3, 4 (likely time and
  temperature-type abscissae) is not evidenced in the code read here.
* No corpus file exercises type codes ≠ 0; the 1-D/2-D table paths are from the code only.
