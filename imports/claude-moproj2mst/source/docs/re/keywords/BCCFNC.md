# BCCFNC (id 161, group 2, case 0x11)

A numbered 1-D function (table) attached to the object: a list of tables at `obj+0x2e68`, each
identified by its function number (table `+0x54`), with abscissa variable code 1 (time, template
0xd). The name and its neighbours suggest the time functions referenced by the BCC codes; nothing in
the reader or writer links them.

## KEY grammar (read)

```
BCCFNC <objNo:%i> <fnNo:%i> <nPts:%i>
[nPts points:  <x:%Flf> <y:%Flf>]
```

* The rest of the keyword line is read by `FUN_140413ee0` (`fgets`, commas become spaces,
  `sscanf("%i %i")`, missing values are 0, extra text ignored).
* Points: `FUN_14041a800` with `ndim = 1`, `cnt[0] = nPts`, `ncomp = 1`: `nPts` pairs `x y`,
  `fscanf("%Flf")`, free format (pairs may be split or joined across lines; commas **not**
  accepted). If `nPts > 0`, one `fgets` then discards the rest of the last data line
  (`_common.md` section 3).
* `nPts <= 0`: no points are read (counts are clamped to 0 by the reallocation) and no extra line
  is consumed.
* Several `BCCFNC` lines per object; same grammar in 2D and 3D.

## Model storage

* `obj+0x2e68`: singly linked list of 0x78-byte tables (head `+0`, tail `+8`, count `+0x10`;
  link field `table+0x60`; function number `table+0x54`).
* Lookup `FUN_1401368c0(list, fnNo)` = first table with `+0x54 == fnNo`. Not found →
  `FUN_140136670(list)` appends a new table (initialised by `FUN_14012d050`, template 0) and
  `+0x54 = fnNo` is set. An existing table is reused (redefinition replaces its data, list
  position unchanged).
* `FUN_14012fc70(table, 0xd)`: re-initialise to template 0xd keeping `+0x54` and `+0x60`
  (template 0xd: `ndim = 1`, `var[0] = 1`, `ncomp = 1`, constant `+8 = 0`).
* `table+0x1c` (`cnt[0]`) = `nPts`; abscissae at `+0x28`, values at `+0x48` (filled by
  `FUN_14041a800`).

## Read errors and validation

* object number EOF / `< 1` → **2** (a missing object is created);
* `FUN_140413ee0` fails (EOF, or empty/blank rest of line) → **2**;
* `fnNo < 1` → **2** (nothing stored);
* `FUN_14041a800` returns 1 (EOF inside the points) → **2**; the table keeps template 0xd, the new
  `nPts` and the values read so far.
* A non-numeric token among the points is not detected (value 0, token left in the stream).
* No messages from the reader.

## KEY writer

`FUN_14029e250` (writer-map step 26, after `DRZ`/`DRMESH`, before `BCCDEF`): for
`i = 0 .. FUN_14013c1b0(obj+0x2e68)-1` (list count `+0x10`) it calls handler2 `FUN_1402b4050`
with ref = (obj, `i+1`); a nonzero status would abort the object (it is always 0).

`FUN_1402b4050`: `N` = `obj+0x3f24` if > 0 else the stored object number;
`t = FUN_140136860(list, i+1)` (1-based position in list order, **not** sorted by `fnNo`).
Only if `t+4 == 1` (`ndim`) and `t+0x10 == 1` (abscissa variable 1):

```
"%-6s %7i %7i %7i\n"   name, N, t+0x54 (fnNo), t+0x1c (nPts)
```

then `FUN_1402f3130(f, t, 0)` (mode 0, `_common.md` section 3): writes nothing when `nPts < 1`;
otherwise one line per point `" %20.10E"` (x) `" %20.10E"` (y) `"\n"`. Status ignored; returns 0.

Tables whose `ndim`/variable were changed elsewhere are silently skipped. Round trip: exact (to the
11 digits printed); the output order is list (creation) order.

## Unit conversion

None.

## Helpers

* `FUN_14038b8a0` case 0x11 (p_main.c lines 1339–1355).
* `FUN_140413ee0`: `"%i %i"` rest-of-line reader (group2-helpers.md).
* `FUN_1401368c0(list, no)`: find table by `+0x54`; `FUN_140136670(list)`: append a new 0x78-byte
  table; `FUN_140136860(list, k)`: k-th table (1-based); `FUN_14013c1b0(list)`: count.
* `FUN_14012fc70` / `FUN_14012d2a0`: table (re)initialisation; `FUN_14041a800`: table body reader.
* `FUN_1402b4050`: registry handler2 (writer). handler3 `FUN_1402b4180(model, name, ref, &out)`:
  returns the `ref[1]`-th table pointer (1-based) for the GUI; not used by the KEY writer.
* `FUN_1402f3130`, `FUN_1402f2e80`: table writer and value-line writer.

## Corpus check

A scripted pass over every corpus KEY file (record counts, values per record, next line is a keyword) accepted all 20 occurrences of BCCFNC.

`C:/Users/alext/Projects/DEFORM_corpus/key/2D/EN/plane stress/EP_PlaneStress_EN.KEY`: parses exactly
(fnNo 1, 5 points, one pair per line; older `%15.7E` float layout):

```
BCCFNC       1       1       5
  0.0000000E+000  1.0000000E+001
  3.0000000E-001  2.5000000E+001
```

## Open questions

* Which code consumes the function numbers (BCCDFN/BCCTFN values?) is outside the KEY path and
  not traced.
