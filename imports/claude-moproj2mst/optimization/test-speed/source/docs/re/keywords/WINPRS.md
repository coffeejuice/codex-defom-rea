# WINPRS (id 226, group 2, case 0x52)

Window-defined pressure boundary condition: a list of `LLWINDEF_DATA` "window" records (a 2D
polygon or a 3D box/cylinder region plus an origin, an attachment mode and two scalar-or-time
functions) kept in list `obj+0x2eb0`; each record is reset to BC kind 8 when read. This file also
holds the shared description of the window record, its list, the shape reader/writer and the
attachment-mode encoding used by WINTMP, WINATM and WINRSE.

## KEY grammar (read)

All lines go through the rest-of-line helpers (commas = spaces, partial lines accepted, see
`group2-helpers.md`). The dimension test is the model's (`FUN_1400015e0` 2D / `FUN_1400015f0` 3D).

2D model:

    WINPRS <objNo:%i> <win:%i> <nPts:%i> <tA:%i> <tB:%i> <mode:%i>      FUN_140414200 (5 ints, no blank skip)
    <ox:%Flf> <oy:%Flf> [<oz:%Flf>]                                     origin, FUN_140414c20
    nPts lines: <x:%Flf> <y:%Flf>                                       FUN_14041b6a0(f,w,nPts,0), no index
    <vA:%Flf>                                                           FUN_140414990
    [table A body]                                                      FUN_14041a800, only if tA == 1
    <vB:%Flf>
    [table B body]                                                      only if tB == 1

3D model: identical except the second header integer is the **shape type** `s`:

    WINPRS <objNo> <win> <s:1|2> <tA> <tB> <mode>
    <ox> <oy> <oz>
    shape body (s = 1 box, s = 2 cylinder; see "Window shape body (3D)")
    <vA> [table A]  <vB> [table B]

Function lines (same rule for A and B; `t` is the header integer, `v` the double read):

| t | table at window+off | effect |
|---|---|---|
| 0 | constant | `ndim = 0`, `+8 (const) = v`; no body |
| 1 | f(variable 1) | `ndim = 1`, `var[0] = 1`, `cnt[0] = (int)v`; body = `cnt[0]` lines `x y` (`_common.md` §3 reader) |
| other | untouched | table stays in its reset state (`ndim 0`, const 0); `v` is discarded; no body |

Variable code 1 is the time axis in DEFORM tables (WINTMP's code 3 is temperature, see corpus
check there). Table A is at window `+0x748`, B at `+0x7c0`.

Dimension unset (neither 2D nor 3D): the case reads **nothing** (status `local_108c` = 0) and
returns 0; the header remains in the stream for the KEY loop.

### Window shape body (3D) — `FUN_14041b7d0(f, w, 1)` (shared)

Reached with `w+0x210` = 1 or 2 (the parsers only set these):

* **1 = box**: `FUN_140263cb0(w, 8)`; 8 lines `<i:%i> <x> <y> <z>` (`FUN_140415880`); `i` must
  equal the 1-based line number, else return 1.
* **2 = cylinder**: `FUN_140263cb0(w, 2)`; 2 lines `<x> <y> <z>` (no index, `FUN_140414c20`), the
  axis end points; then one line `<r1:%Flf> <r2:%Flf>` (`FUN_140414a40`) → `w+0x1b8`, `w+0x1c0`.
* (unreachable from WIN* keywords) **3 = surface mesh**: `<nNodes:%i>` line (`FUN_140413de0`),
  nNodes lines `<i> <x> <y> <z>` (index not checked) into the vector at `w+0x220` (+0x10 part),
  `<nElem:%i>`, nElem lines of 5 ints `<i> n1 n2 n3 n4` (index not checked) into `w+0x220` (+0x18).
  **0xb/0xc/0xd**: `<n:%i>` → `w+0x178`, then n points as for the box (indexed when the flag is 1).
  Any other type: `printf("ERROR: WindefShape invalid!\n")` (stdout), return 3.
* EOF / blank line in any helper → 1. Return 0 on success.

Then the parser always calls `FUN_140267760(w)` (derived points, not part of the KEY data):
for 3D windows of type 1/2 it allocates `w+0x214` points at `w+0x218` — box: 6 face centres
(midpoints of corner pairs (7,2) (8,1) (8,3) (5,2) (7,5) (3,1), 1-based); cylinder: point 1,
point 2, point 2 + a radial vector of length `r2`, and when `r1 >= 1e-20` (`DAT_140522fa0`) a
fourth point at radius `r1` (`w+0x214` = 4, else 3). So `r2 = w+0x1c0` is the (outer) radius and
`r1 = w+0x1b8` an optional inner radius. Radii `< 1e-10` are replaced by `1e-6`.

### Window shape body (2D) — `FUN_14041b6a0(f, w, n, idx)` (shared)

`FUN_140263cb0(w, n)` (frees old points, `w+0x178 = n`, allocates `n × 2` doubles zeroed when the
window is 2D, `n × 3` otherwise; `n == 0` → no buffer). Then `n` lines: `idx == 0` →
`<x> <y>` (`FUN_140414a40`); `idx == 1` → `<i:%i> <x> <y>` (`FUN_140415680`) with `i` required to
be the 1-based line number (else 1). Values go to `pts[2k]`, `pts[2k+1]`. EOF/blank line → 1.
No check on `n`: `n <= 0` reads no lines.

## Model storage

Window list: `obj+0x2eb0` (one of seven 0x30-byte lists in the sub-object at `obj+0x2e68`:
`+0x48` WINPRS 0x2eb0, `+0x78` WINTMP 0x2ee0, `+0xa8` WINATM 0x2f10, `+0xd8`/`+0x108`/`+0x138`
other window kinds, `+0x168` WINRSE 0x2fd0). List fields: `+0x00` head, `+0x08` tail, `+0x14`
count (`FUN_14019a590`), `+0x20` model pointer (set by `FUN_1401957b0` from `obj+0x3a98`), `+0x28`
extra pointer (non-NULL → new entries get `FUN_140263450(e, 0xb)`, undone by the reset below).

`FUN_140269bc0(list, win)` = get-or-create by **position**: while count < `win`, append a new
`LLWINDEF_DATA` (0x920 bytes, ctor `FUN_14025c100(e, list+0x20)` → reset with kind 0) and set its
`+0x170` = new count; then return entry number `win` (`FUN_140269900`: walks `+0x840` links;
`win <= 1` returns the head). `win <= 0` with an empty list returns NULL and the next call
dereferences it (crash); `win <= 0` with a non-empty list overwrites entry 1.

`FUN_14025d1a0(w, kind)` keeps `+0x170` and `+0x840`, frees everything (`FUN_14025d0a0`) and
re-initialises (`FUN_14025c350(w, kind)`). WINPRS kind = **8**.

`LLWINDEF_DATA` fields (from `FUN_14025c100`/`FUN_14025c350` and the readers/writers):

| Off | Type | Meaning |
|---|---|---|
| +0x000 | vftable | `LLWINDEF_DATA` (subclass `LLRANGEDEF_DATA` adds `+0x920`/`+0x928`) |
| +0x170 | int | window number (1-based list position) |
| +0x174 | int | BC kind: 8 WINPRS, 3 WINTMP, 4 WINATM, 10 WINRSE |
| +0x178 | int | number of shape points |
| +0x180 | double* | points, 2 (2D) or 3 doubles each |
| +0x188..+0x1b0, +0x1c8..+0x208 | double | zeroed by reset, unused here |
| +0x1b8, +0x1c0 | double | cylinder radii r1 (inner, optional), r2 (outer) |
| +0x210 | int | shape type: 1 (reset default; 2D polygon / 3D box), 2 cylinder, 3 mesh, 0xb–0xf others |
| +0x214, +0x218 | int, double* | derived points (count, xyz array) |
| +0x220 | object | surface-mesh container (nodes vector at +0x10, element ints at +0x18) |
| +0x350, +0x358, +0x360 | double | origin / reference point (x, y, z) |
| +0x368 | int | attachment mode (0, 1, 2; see below) |
| +0x36c | int | object number for mode 2 |
| +0x370 (q), +0x378 (i), +0x560 (byte), +0x838 (i) | | zeroed, not touched by these keywords |
| +0x380, +0x3f8, +0x470, +0x4e8, +0x568, +0x5e0, +0x658, +0x6d0, +0x748, +0x7c0 | table | ten 0x78-byte tables (`_common.md` §3), reset with template id 0 (`ndim 0`, `ncomp 1`, const 0) |
| +0x840 | ptr | next window in list |
| +0x888 | ptr | model (copied from list+0x20) |
| +0x890 | ptr | zeroed by reset |
| +0x898 | int | dimension copied from `model+0x1c` at every reset (`FUN_14025c930`: == 2, `FUN_14025c970`: == 3) |

WINPRS uses tables A = `+0x748`, B = `+0x7c0`.

Attachment mode (header integer `m`, shared by all four keywords except WINRSE):

| m | `+0x368` | `+0x36c` |
|---|---|---|
| 0 or 1 | 0 | unchanged (0 after reset) |
| 2 | 1 | unchanged |
| < 0 | 2 | `-m` |
| >= 3 | unchanged (0 after reset) | unchanged |

Corpus values are 1 and negative numbers equal to minus another object's number (e.g. `-2`,
`-3`), so mode 2 most likely means "window moves with object `+0x36c`", mode 0 fixed in space;
mode 1 (`m = 2`) not seen.

In WINPRS the mode is stored only after both tables were read successfully. In 3D the shape type
is stored in `+0x210` before the body is read; in 2D `+0x210` stays 1.

## Read errors and validation

All failures return **2** ("Reading Error"):

* header: EOF or blank line (`FUN_140414200` returns true);
* origin line EOF/blank;
* 2D: a point line EOF/blank (`FUN_14041b6a0` → 1);
* 3D: shape type not 1 or 2 → 2 (the window was already created and reset, origin stored);
  box index mismatch, EOF/blank in the body → 2 (the `FUN_14041b7d0` code is not passed through);
* either function value line EOF/blank, or table body EOF.

Missing header integers are 0 (so `nPts`/`s`, `tA`, `tB` default to 0, mode 0 → `+0x368 = 0`);
`s = 0` in 3D fails. Garbage tokens in tables are not detected (`_common.md` §3).

## KEY writer

Two writers with identical layout:

* `FUN_14029e250` step 48 (all object types except 5 and 7), `N` = `outNo` or `obj+8`;
* `FUN_1402ab880(model, f, obj, N)` for type-5 objects when `model+0x1744 & 0x1000000`
  (`N = param` if > 0 else `obj+8`); returns 0. Type-5 objects never write WINTMP/WINATM/WINRSE.

If 2D, for every entry `k = 1..count` of `obj+0x2eb0` (in list order):

    "%-6s %7i %7i %7i %7i %7i %7i\n"   "WINPRS", N, w+0x170, w+0x178, (A.ndim != 0), (B.ndim != 0), M
    " %20.10E %20.10E\n"               w+0x350, w+0x358
    shape: FUN_1402f4890(f, w, 1)      -> one " %20.10E %20.10E\n" line per point (no index)
    FUN_1402f3130(f, w+0x748, 1)
    FUN_1402f3130(f, w+0x7c0, 1)

then (separate loop) if 3D, every entry:

    "%-6s %7i %7i %7i %7i %7i %7i\n"   "WINPRS", N, w+0x170, w+0x210, (A.ndim != 0), (B.ndim != 0), M
    " %20.10E %20.10E %20.10E\n"       w+0x350, w+0x358, w+0x360
    FUN_1402f4890(f, w, 0)             (box: indexed lines; cylinder: 2 lines + radii line)
    FUN_1402f3130(f, w+0x748, 1)
    FUN_1402f3130(f, w+0x7c0, 1)

`M` (shared by WINPRS/WINTMP/WINATM): `+0x368 == 0` → 1, `== 1` → 2, `== 2` → `-abs(+0x36c)`,
other → 0. Nothing is written when the dimension is unset. Return values of the two helpers are
ignored; the step never fails.

### Shape writer `FUN_1402f4890(f, w, noIdx)` (shared)

`f == NULL` → 2. `idx = (noIdx == 0)`. Points are written with `FUN_1402f2e80(f, n, p, n, 0)`
(`_common.md` §3), i.e. one line `" %20.10E"`×n + `"\n"`.

* window 2D (`w+0x898 == 2`): per point `[" %7i" k]` + 2 values.
* otherwise by `w+0x210`:
  * 1: per point `[" %7i" k]` + 3 values;
  * 2: per point 3 values (never indexed), then `" %20.10E %20.10E\n"` (`+0x1b8`, `+0x1c0`);
  * 3: `"%d\n"` nodes, per node `" %7i %20.10E %20.10E %20.10E\n"`, `"%d\n"` elements, per
    element `" %7i %7i %7i %7i %7i\n"` (k, 4 node ids);
  * 0xb/0xc/0xd: `"%d\n"` `+0x178`, then per point `[" %7i" k]` + 3 values;
  * other: nothing.

Returns the status of the last point line (0), or 0.

### Table writer in mode 1 (`FUN_1402f3130(f, t, 1)`, differences to `_common.md` §3)

Mode 1 (also 2) skips the `cnt[i] < 1` early exit and writes the size line:

* `ndim 0`: `ncomp == 1` → `" %20.10E\n"` const; `ncomp >= 2` → values 5 per line; else nothing.
* `ndim 1`: `" %7i\n"` `cnt[0]`, then per point `" %20.10E"` x + values (5 per line).
* `ndim 2..3`: sizes via `FUN_1402f2fe0(f, ndim, cnt, ndim, 0)` (one line `" %7i"`×ndim), then
  each axis on one line, then the data wrapped every `max(cnt)` values (8-space continuation).
* `ndim > 3`: nothing. Mode 2 uses `" %20.10E,"` / `" %7i,"` (not used here).

Round-trip notes: the header flags are `ndim != 0`, so a table with `ndim 1` and a variable code
other than 1 is re-read as a time table. Placeholder windows (created to fill gaps up to `win`)
are written with 0 points: fine in 2D, but in 3D they are written as shape type 1 with no point
lines while the reader expects 8 indexed corner lines → the next record is misread. Mode
`+0x368 == 2` with `+0x36c == 0` is written as 0 and re-read as mode 0.

## Unit conversion

None.

## Helpers

| Address | Role |
|---|---|
| `FUN_140414200` | header, 5 ints |
| `FUN_140414c20` | origin line (3 doubles) |
| `FUN_140414990` | function value line |
| `FUN_14041a800` | table body reader (`_common.md` §3) |
| `FUN_14041b6a0` | 2D window points reader |
| `FUN_14041b7d0` | 3D window shape reader |
| `FUN_140263cb0` | allocate shape points (`+0x178`, `+0x180`) |
| `FUN_140267760` / `FUN_140263dc0` | derived points (face centres / cylinder rim points) |
| `FUN_140269bc0` / `FUN_140269900` / `FUN_1402694a0` | list get-or-create / nth / append |
| `FUN_14025c100` / `FUN_14025c350` / `FUN_14025d0a0` / `FUN_14025d1a0` | ctor / init(kind) / free / reset keeping number+link |
| `FUN_14025c930` / `FUN_14025c970` | window is 2D / 3D (`+0x898`) |
| `FUN_14029e250` (step 48), `FUN_1402ab880` | writers |
| `FUN_1402f4890` | shape writer |
| `FUN_1402f3130`, `FUN_1402f2e80`, `FUN_1402f2fe0` | table / value-line writers |
| `FUN_140484ab8` | `abs` |

## Corpus check

No corpus file uses WINPRS. The shared shape grammar and writer layout are confirmed by the
WINTMP corpus (see WINTMP.md).

## Open questions

* Physical meaning of tables A (`+0x748`) and B (`+0x7c0`) (pressure value and a second
  quantity, both f(time)); settle by finding the solver/GUI use of these offsets.
* Meaning of attachment mode 1 (`m = 2`).
