# MGUSER (id 194, group 2, case 0x32)

One user-defined mesh window (local mesh-density region) of an object: a header line with the
window number, element size and shape, followed by the shape's geometry lines. Windows are
`LLWINDEF_DATA` records (RTTI name; 0x920 bytes) in a linked list at `obj+0x2d50`; the error
text `"ERROR: WindefShape invalid!\n"` names the shape field.

## KEY grammar (read)
### 3D (`model+0x1c != 2`)
Header = rest of the keyword line, `FUN_140415d70` (`%i` + 7 × `%Flf`, commas = spaces):

    MGUSER <objNo:%i> <win:%i> <size:%Flf> <p1:%Flf> <p2:%Flf> <p3:%Flf> <shape:%Flf> <opt:%Flf> [<flag:%Flf>]

`shape`, `opt`, `flag` are read as doubles and truncated to int. `shape` 0 or 1 → 1; 2, 3, 11,
12, 13 kept; anything else → return 2 (body not read). The body depends on the shape:

| shape | body (each line read with the named helper) |
|---|---|
| 1 (box) | 8 lines `<i:%i> <x> <y> <z>` (`FUN_140415880`); `i` must equal the line number 1..8 |
| 2 (cylinder) | 2 lines `<x> <y> <z>` **without index** (`FUN_140414c20`), then 1 line `<a> <b>` (`FUN_140414a40`) |
| 3 (surface) | line `<nn:%i>` (`FUN_140413de0`, no blank skip); `nn` lines `<i> <x> <y> <z>` (`FUN_140415880`, `i` ignored); line `<ne:%i>`; `ne` lines `<i> <n1> <n2> <n3> <n4>` (`FUN_140414200`, 5 × `%i`, no blank skip, `i` ignored) |
| 11, 12, 13 (point list) | line `<np:%i>` (`FUN_140413de0`); `np` lines `<i:%i> <x> <y> <z>` (`FUN_140415880`), `i` must equal 1..np |

Each body line is one `fgets` line (commas = spaces, extra text ignored, missing values 0).
The 3D corpus also shows the box/cylinder geometry as the fixed 8/2 points.

### 2D (`FUN_1400015e0`)
Header via `FUN_140416220` (`%i %i` + 4 × `%Flf`):

    MGUSER <objNo:%i> <win:%i> <np:%i> <size:%Flf> <p1:%Flf> <p2:%Flf> <opt:%Flf>

then `np` lines `<i:%i> <r:%Flf> <z:%Flf>` (`FUN_140415680`), `i` must equal 1..np. No shape.

## Model storage
Window lookup `FUN_140269bc0(obj+0x2d50, win)`: while the list count (`list+0x14`) < `win`,
append a new window (`FUN_1402694a0`: `new LLWINDEF_DATA(model = list+0x20)`, appended by
`FUN_1402693f0`, then `+0x170 = new count`); return the `win`-th window (`FUN_140269900`, walk from
`list+0` along `+0x840`). For `win <= 1` the head is returned (so `win <= 0` overwrites window 1;
with an empty list the result is NULL and the next call dereferences it, i.e. a crash).

The window is then reset completely by `FUN_14025d1a0(w, 2)` (keeps only `+0x170` number and
`+0x840` next): frees the points, clears the surface geometry, `+0x174 = 2`, `+0x178 = 0`,
`+0x180 = NULL`, `+0x210 = 1`, `+0x214 = 0`, `+0x890 = 0`, `+0x898 = model dimension`
(`model+0x1c`, when the window has a model pointer `+0x888`), `+0x1b8/+0x1c0 = 0`,
`+0x350..+0x360 = 0`, `+0x368 = +0x36c = 0`, `+0x370 = 0`, `+0x378 = 0`, ten 0x78-byte table
objects at `+0x380 + k*0x78` (k = 0..9) re-initialised with type 0 (`_common.md` §3).

Header fields:

| field | 3D | 2D |
|---|---|---|
| `size` | `+0x370` (double) | `+0x370` |
| `p1, p2` | `+0x350`, `+0x358` | `+0x350`, `+0x358` |
| `p3` | `+0x360` | — |
| `shape` | `+0x210` (int, mapping above) | stays 1 |
| `opt` | see below | see below |
| `flag` | `+0x378` = 0/1 only if shape ∈ {11,12,13} and `(int)flag` ∈ {0,1}; else stays 0 | — |

`opt` (`(int)opt`): 0 or 1 → `+0x368 = 0`; 2 → `+0x368 = 1`; negative → `+0x368 = 2`,
`+0x36c = -opt`; other positive values leave `+0x368 = 0`.

Geometry: `+0x178` = point count, `+0x180` = `malloc`ed `double[count × d]`, zero filled, with
`d = 2` if `+0x898 == 2` (2D window) else 3 (`FUN_140263cb0(w, count)`; count 0 → no buffer).
Shape 1 allocates 8 points, shape 2 allocates 2 points and stores `a → +0x1b8`, `b → +0x1c0`;
shapes 11–13 use `np`; 2D uses `np`. Shape 3 stores nodes in the `_OBJ_GEO3D` object at
`+0x220` (double vector at `+0x220+0x10`, 3 per node, resized to `nn×3` only when `nn ≥ 1`) and
quads in its int vector at `+0x220+0x18` (4 per element, resized to `ne×4` when `ne ≥ 1`).

3D only, after the body: `FUN_140267760(w)` derives auxiliary primitive data for shapes 1 and 2 (built from
the corner points, and for shape 2 also from `+0x1b8/+0x1c0`); this is internal and not written to the KEY file.

## Read errors and validation
All return 2 (Reading Error), see `_common.md` §1 for the effect:
* objNo EOF / < 1.
* 3D invalid shape (after truncation not in {0,1,2,3,11,12,13}): direct return 2. The header
  line is consumed, the window is already reset and holds size/p1..p3; the body lines remain in
  the stream and will be read as keywords.
* Body: any helper returning 1 (EOF or empty line), or an index mismatch on indexed lines
  (shape 1, shapes 11–13, 2D) → 1 → 2. Points read before the failure stay stored.
* `FUN_14041b7d0` itself returns 3 with `printf("ERROR: WindefShape invalid!\n")` for a shape
  outside {1,2,3,11,12,13}; unreachable from this reader (shape already validated).
* **Header failure is masked**: the status of the header read is overwritten by the body read.
  An empty rest of line gives all header values 0 (`win = 0` → window 1, or NULL with an empty
  list), and the body is then read from the following lines; the result is that of the body.

## KEY writer
Inline loop at the end of `FUN_1402a9180` (line 15 of the mesh-settings block; order and
conditions in [MGNELM.md](MGNELM.md)). Counter `n = 0`, `optOut = 0` (persists across windows).
For `k = 1 .. list count`: `w = k`-th window; skip if NULL or `!(w+0x370 > 0.0)` (size ≤ 0 or NaN).
`optOut` = 1 if `+0x368 == 0`, 2 if `== 1`, `-abs(+0x36c)` if `== 2`, else unchanged from the
previous window.

* **3D**: `shapeOut` = `+0x210` for 1, 2, 3; **11 for 11, 12 and 13**; any other shape → the
  writer returns 3 at once (remaining windows not written; the caller ignores the code).
  `n += 1` (windows are renumbered 1..n, skipped ones leave no gap). Header:
  * shape 1–3: `"%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i %7i\n"`
    objNo, n, `+0x370`, `+0x350`, `+0x358`, `+0x360`, shapeOut, optOut;
  * shape 11–13: same plus `" %7i"` of `+0x378`:
    `"%-6s %7i %7i %20.10E %20.10E %20.10E %20.10E %7i %7i %7i\n"`.
* **2D**: `"%-6s %7i %7i %7i %20.10E %20.10E %20.10E %7i\n"` objNo, `+0x170` (the window's
  own number, gaps kept), `+0x178` (np), `+0x370`, `+0x350`, `+0x358`, optOut.

Body: `FUN_1402f4890(f, w, 0)` (values via `FUN_1402f2e80`, `" %20.10E"` each, `_common.md` §3):
* 2D window (`+0x898 == 2`): per point `" %7i"` (1-based) + `" %20.10E %20.10E\n"`.
* shape 1: per point `" %7i"` + `" %20.10E %20.10E %20.10E\n"`.
* shape 2: per point `" %20.10E %20.10E %20.10E\n"` (no index), then
  `" %20.10E %20.10E\n"` of `+0x1b8`, `+0x1c0`.
* shape 3: `"%d\n"` node count (vector size / 3), per node `" %7i %20.10E %20.10E %20.10E\n"`;
  `"%d\n"` element count (size / 4), per element `" %7i %7i %7i %7i %7i\n"`.
* shapes 11–13: `"%d\n"` of `+0x178`, then per point `" %7i"` + 3 × `" %20.10E"` + `"\n"`.
* other shapes: nothing.

Round-trip: lossless except (a) 3D renumbering of windows, (b) shapes 12/13 become 11,
(c) windows with size ≤ 0 disappear (3D) or leave unwritten gaps that re-read as fresh reset
windows (2D), (d) `opt` values > 2 are written as the previous window's `optOut` (but the reader
never produces such a state), (e) MGUSER windows with shapes 14/15 (settable by other code,
`FUN_140263450`) abort the 3D loop.

## Unit conversion
None.

## Helpers
* `FUN_14038b8a0` case 0x32 — reader.
* `FUN_140415d70` (`%i`+7×`%Flf`), `FUN_140416220` (`%i %i`+4×`%Flf`) — header readers.
* `FUN_140269bc0` — get-or-create window `n`; `FUN_1402694a0` — new window; `FUN_1402693f0` —
  append; `FUN_140269900` — n-th window; `FUN_14019a590` — list count (`+0x14`).
* `FUN_14025c100` — `LLWINDEF_DATA` constructor; `FUN_14025d1a0` → `FUN_14025d0a0` +
  `FUN_14025c350` — reset; `FUN_14025c930` — `+0x898 == 2`; `FUN_140263cb0` — allocate points;
  `FUN_140263360` — free points; `FUN_140263450` — set shape from a UI code (not used by KEY).
* `FUN_14041b7d0` — 3D body reader; `FUN_14041b6a0` — 2D body reader (indexed `%i %Flf %Flf`).
* `FUN_140267760` — derived box/cylinder faces.
* `FUN_1402a9180` — writer loop; `FUN_1402f4890` — window body writer; `FUN_1402f2e80` — value line
  writer; `FUN_140484ab8` — `abs`.

## Corpus check
* 2D `key\2D\SI\Fracture\FRAC_SI.KEY`:
  `MGUSER       1       1       4  6.5000000E+001  0.0000000E+000  0.0000000E+000       1`
  then `       1  9.6822080E+000 -1.3495840E+000` … — parses (np = 4, indexed points).
* 3D shape 1: `key\3D\EN\Diestress\3D_Shrinkfit.KEY` (`... 0.0000000E+000       1       1` +
  8 indexed points); shape 2: `key\3D\EN\Extrusion\ALE\European_Benchmark_2005_ALE.KEY` (2
  unindexed points + `    0.0000000000E+000    1.5805500000E+001`); shape 11:
  `key\3D\SI\FlowForming\Implicit_ALE\Operations\Task00001\STEP.KEY`
  (`... 11       1       1`, then `4`, then indexed points). All parse and match the writer.
  Corpus totals: 3D shape 1 ×139, 2 ×33, 11 ×2; 2D np 4/5/6; opt is always 1.

## Open questions
* Semantics of `p1..p3` (always 0 in the corpus; possibly window velocity), of `opt` codes and of
  the 3D `flag` (`+0x378`); settle by reading the mesher's use of `+0x350..+0x378`.
* Whether shapes 12/13 differ from 11 anywhere else (writer collapses them).
