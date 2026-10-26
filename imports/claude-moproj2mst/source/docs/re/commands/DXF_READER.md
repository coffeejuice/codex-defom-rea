# In-process DXF reader (GFREAD, 2D objects)

DEF_PRE_64.exe v11.0.2. Reached only from `GFREAD` on a 2D object whose file extension is
`DXF`/`dxf` (see [GFREAD.md](GFREAD.md) §3 and [DXF_IGES_READERS.md](DXF_IGES_READERS.md) §2).
Decompiles: `deform-rea-work/out/p89f/dxf/all.c` (reader, depth 1), `f1.c`, `f2.c`, `f3.c`
(Fortran chaining stage, OCS helpers), `kw.c`; listings `l1.txt` (Fortran sin/cos/atan2),
`l2.txt` (Fortran atan). Ghidra copy `ghidra_p89f3`. Shared mechanics: [common.md](common.md).

Every rule below was checked against the original with 25 oracle cases (§15): a reference
model written from this spec reproduces the object's `DIEGEO` block of `OUT.KEY` byte for byte
in all of them.

## 1. Summary

```
FUN_140244500 (GFREAD 2D dispatch, type 7)
 └ FUN_1404093a0(dxf, file, "", obj, 2, 300)           entry, §2
    ├ FUN_1401b8560(file)                               fopen "r" test
    ├ FUN_140197bc0(obj)                                undo snapshot of the object's curves (§12)
    ├ FUN_140422ab0(file, &nPts, &nSeg, &data)          parse + segments + tolerance, §3
    │   ├ FUN_140428b10  (fopen "rt" test, -300)
    │   ├ FUN_140422f10  (work struct, calloc 0x60)
    │   ├ FUN_140422f70  group-code/value pair loop, §5 → 10 handlers, §6
    │   ├ FUN_1404231c0  entity records → segment array (6 doubles each), §7
    │   ├ FUN_14027ba50  "size" = smallest relevant distance, §8
    │   ├ FUN_140428f70  global G = 0.01·size
    │   └ FUN_140424390  free work struct
    ├ tol = FUN_1404099c0() (= G) or obj+0x3a58, stored obj+0x3a48, dxf+0x20
    └ FUN_14040ba80(iges, obj, tol, nPts, nSeg, data, 700)   §9–§11
        └ FUN_14040c220(.., 2, 700) → FUN_14040c740 (duplicate removal)
                                     → FUN_140457312 (Fortran: merge ends, chain, orient, write back)
```

Result: curve 0 of the object's geometry becomes one **type-2 curve** (6 values per point:
`type, x1, y1, x2, y2, angle`), one record per line or arc segment, chained into one
contiguous path when the drawing forms a single chain. KFWRIT writes it as
`DIEGEO <obj> 2 <n>` (§13). No file is written, no external program is run and nothing is
printed by the reader.

## 2. Entry `FUN_1404093a0(dxf, file, prefix, obj, kind, mode)`

A local `Uic2CObjectIgesFile` (`FUN_1404099d0`) is built on the stack (it only supplies the
conversion helper).

| Check | Return |
|---|---|
| `obj == NULL` | -1 |
| `mode != 300` | not reached from GFREAD: delegates to `FUN_140409af0` (IGES reader) with the DXF object's geometry |
| `file == NULL` or `strlen(file) < 1` | -2 |
| `FUN_1401b8560(file)` false (cannot `fopen(file,"r")`) | -3 |

Then `FUN_140197bc0(obj)`, `r = FUN_140422ab0(file, &nPts, &nSeg, &data)`. If `r == 0`:

1. `tol = obj+0x3a50 (int, FUN_1404098f0/FUN_140409910) == 0 ? FUN_1404099c0() : obj+0x3a58 (double, FUN_140409930)`.
   `FUN_1404099c0` returns the global double `DAT_14062bc68` (= G, just set by the parse).
   `obj+0x3a50`/`+0x3a58` are zeroed by the object constructor; no setter was found, so
   `tol = G`.
2. `obj+0x3a48 = tol` (`FUN_1404098d0`), `dxf+0x20 = tol` (`FUN_1404099a0`).
3. `r = FUN_14040ba80(iges, obj, tol, nPts, nSeg, data, 700)`; non-zero → the segment array is
   freed (`FUN_140422d90` → `free`).
4. `dxf+0x1c = iges+0x1c` (`FUN_1402566b0` then `FUN_140409970`). The caller copies this into
   `obj+0xbc8` (`FUN_14019a650`) when the result is 0.

Return `r`. GFREAD treats only **-1** as failure (`"INFO   : Loading geometry - Failed\n"`);
every other value prints `- Done`, even when nothing was stored (§12).

## 3. Driver `FUN_140422ab0(name, int* nPts, int* nSeg, double** data)`

Outputs zeroed first. Returns:

| Code | Cause |
|---|---|
| -100 | name NULL |
| -200 | `strlen(name) < 1` |
| -300 | `FUN_140428b10(name)` false: name NULL/empty or `fopen(name, "rt")` (`_fsopen`, share 0x40) fails; the file is closed again |
| -1 | work struct `calloc(1, 0x60)` failed |
| -400 | `fopen(name, "rb")` failed |
| -10000 | binary DXF header (§5) |
| -500 | no segment (§7) |
| -1 | segment array allocation failed |
| 0 | ok |

Order after the open: `rc = pair loop`; `fclose`; if `rc == 0`, `rc = FUN_1404231c0(nSeg, nPts, data, work)`.
Then **always** (also after -10000/-500): a temporary `_OBJ_GEO2D` (`FUN_140275cd0`) gets the
segments (`FUN_140277420(geo, *nSeg, *data)`: mode 2, copies `nSeg·6` doubles; nothing when
`nSeg < 1`), `FUN_14027ba50(geo, &size)` (size = 0 when there are no segments),
**`G = DAT_14062bc68 = size · 0.01`** (`DAT_14051f258`), work struct freed. G persists for the
whole DEF_PRE process (initial value `1e-9`, `0x3e112e0be826d695`) and is also the OCS threshold
of the *next* DXF read (§7.1, case `gfread_dxf_seq_tolzero`).

Note the argument naming: the third output of `FUN_1404231c0` is the **segment count**, the
second the **point count** (2 per line, 3 per arc); `FUN_1404093a0` passes them to
`FUN_14040ba80` as `(nPts, nSeg)`.

## 4. Line and token rules

* Every read is `fgets(buf, 0x200, f)` on the `"rb"` stream: at most 511 bytes, up to and
  including `\n`. A longer line is split into several reads (pairs then shift). CR is kept and
  trimmed below. `\x1a` is ordinary data.
* `trim` (`FUN_140427df0(s, 1)`): skip leading **spaces only** (not tabs); remove trailing `\n`,
  `\r`, `' '` (not tabs).
* `eq(a, b)` (`FUN_140428650`): both strings upper-cased with `toupper` into 512-byte buffers,
  then `strcmp`; 0 = equal. So names and codes are case-insensitive; `"010"` ≠ `"10"`.
* Numbers: `atof` (`_atof_l(s, NULL)`, VS2008 CRT) on the **raw** value line: leading white
  space, sign, digits, `.`, exponent `e/E/d/D`; no `inf`/`nan`/hex; no number → 0.0. Code 70 of
  LWPOLYLINE uses `atol` (`strtol(s, NULL, 10)`). SPLINE integers are `(int)atof(...)`
  (truncation).
* Group codes are compared as trimmed strings, never converted to numbers.

## 5. Pair loop `FUN_140422f70(f, work)`

```
work->inEntities (+0x08) = 0; first = 1
loop:
  if !fgets(code) return 0;  code = trim(copy)
  if first: if eq(code, "AutoCAD Binary DXF") return -10000;  first = 0
  if !fgets(value) return 0; value = trim(copy)
  if eq(code, "0"):
     for i in 0..9 (table 0x14062bdb0, 16-byte entries {char* name, handler}):
        if eq(value, name[i]) and handler[i](f, work) == 0: return 0
```

* The return value is always 0 (or -10000): a handler that returns 0 **ends the whole parse
  successfully**, keeping everything stored so far. This is how incomplete entities stop the
  read (case `gfread_dxf_badline`).
* Unknown entity names (POINT, TEXT, INSERT, `EOF`, …) and all non-`0` pairs are skipped pair by
  pair. End of file ends the loop. There is no special `EOF` handling.
* Handlers are entered with the file positioned after the `0/NAME` pair. They read pairs
  themselves and, when they meet the next entity, `fseek` back so the main loop reads that
  `0/NAME` pair again.

Table (`DumpTable`, names at the listed pointers):

| i | Name | Handler |
|---:|---|---|
| 0 | `SECTION` | `FUN_1404245b0` |
| 1 | `ENDSEC` | `FUN_1404246c0` |
| 2 | `LINE` | `FUN_140424700` |
| 3 | `POLYLINE` | `FUN_140424da0` |
| 4 | `LWPOLYLINE` | `FUN_1404251e0` |
| 5 | `VERTEX` | `FUN_1404259c0` |
| 6 | `SEQEND` | `FUN_140425cf0` |
| 7 | `ARC` | `FUN_140425d50` |
| 8 | `CIRCLE` | `FUN_1404262e0` |
| 9 | `SPLINE` | `FUN_140426810` |

## 6. Entity handlers

### 6.1 Common pair-reading pattern

All entity handlers (except SECTION/ENDSEC/SEQEND) start with `if (!work->inEntities) return 1;`
and then run:

```
save = ftell(f)
do {
   if !fgets(code) break                       // EOF on a code line: leave the loop normally
   (found, idx, term) = lookup(trim(code), table, n)     // FUN_140424c80
   if !fgets(value) return 0                   // EOF on a value line: handler returns 0
   if found: v = atof(value); store by idx (see each handler); a duplicate sets term
   if term: fseek(f, save, SEEK_SET); break    // back to the start of this pair
   save = ftell(f)
} while (!done-condition)
```

`lookup` (`FUN_140424c80(code, table, n, &idx, &term)`): `term = 0`; if `eq(code,"0")` →
term = 1, not found; else if `code` equals one of the 10 entity names → term = 1, not found;
else the first `n` entries of the handler's code table (16-byte entries `{char* code, int idx}`)
are searched; match → found, `idx` written (otherwise `idx` keeps its previous value).

Code tables (`n` = entries used):

| Handler | Table | n | Entries (code → idx) |
|---|---|---:|---|
| LINE | `0x14062be50` | 7 | 210→4, 220→5, 230→6, 10→0, 20→1, 11→2, 21→3 |
| POLYLINE | `0x14062be50` (LINE's) | 4 | 210→4, 220→5, 230→6, 10→0 |
| LWPOLYLINE | `0x14062bf00` | 4 | 10→0, 20→1, 42→2, 70→4 |
| VERTEX | `0x14062bf40` | 3 | 10→0, 20→1, 42→2 |
| ARC | `0x14062bf70` | 8 | 210→5, 220→6, 230→7, 10→0, 20→1, 40→2, 50→3, 51→4 |
| CIRCLE | `0x14062bff0` | 6 | 210→3, 220→4, 230→5, 10→0, 20→1, 40→2 |
| SPLINE | `0x14062c050` | 12 | 210→0, 220→1, 230→2, 70→3, 71→4, 72→5, 73→6, 40→7, 10→8, 20→9, 30→10, 41→11 |

Codes not in the table (8 layer, 30/31 z, 39, 62, 100, …) are ignored.

### 6.2 SECTION `FUN_1404245b0` / ENDSEC `FUN_1404246c0`

SECTION reads one more pair (two `fgets`, no seek back); if `eq(trim(code),"2")` and
`eq(trim(value),"ENTITIES")` → `inEntities = 1` (otherwise unchanged). Returns 1.
ENDSEC: `inEntities = 0`, returns 1, reads nothing. Entities in HEADER/TABLES/BLOCKS are ignored
(case `gfread_dxf_lines_gap` has a LINE inside a BLOCK).

### 6.3 LINE `FUN_140424700`

Slots `x1 (10), y1 (20), x2 (11), y2 (21)` and extrusion `(210, 220, 230)` default `(0,0,1)`
(`FUN_140427ec0`). Acceptance:
* 10 always; 20 only after 10; 11 only after 10 and 20; 21 only after 10, 20, 11. A code that
  arrives too early is silently ignored.
* A second occurrence of an accepted slot (also 210/220/230) is a duplicate → term.
* Done when all 7 slots are set (normally the loop ends at the next `0`).

After the loop: any of 10/20/11/21 missing → **return 0** (parse ends). Else append a 0x38-byte
record to `work+0x18` (count `+0x10`, `FUN_140424b90`, realloc): `{x1, y1, x2, y2, ext[3]}`.
Return 1.

### 6.4 POLYLINE `FUN_140424da0`

1. Append a 0x30-byte polyline record to `work+0x28` (count `+0x20`, `FUN_1404250f0`):
   `{int nVert; vert* v (0x18 each: x, y, bulge); int flag (+0x10); double ext[3] (+0x18)}`,
   ext = `(0,0,1)`. `work+0x50` = this record (current polyline), `work+0x58 = 1` (in polyline).
2. Pair loop with LINE's table limited to 4 entries. Only **code 210** has an effect:
   first 210 → `closed = (atof == 1.0)`, a second 210 → term. 220/230/10 are ignored and
   **code 70 is not read**. The loop ends only at a terminator or EOF.
3. If 210 was seen: `flag = closed`, ext = `(0,0,1)`.
4. Value-line EOF → return 0, else 1.

So the POLYLINE closed bit (70 & 1) is ignored; a POLYLINE is "closed" only when its 210 value
is exactly 1.0 (cases `gfread_dxf_polyline_open`, `gfread_dxf_polyline_210`).

### 6.5 VERTEX `FUN_1404259c0`

Returns 1 doing nothing unless `inEntities` and `work+0x58`. Slots 10, 20, 42 (bulge, default 0);
each accepted once, a second one is term. The loop also stops (without seek back) after the pair
that completes "10 and 20 and 42". Missing 10 or 20 → **return 0**. Otherwise append
`(x, y, bulge)` to the current polyline (`work+0x50`, `FUN_1404258d0`). Vertex flags (70) are
ignored, so spline-frame and fit vertices are all used.

### 6.6 SEQEND `FUN_140425cf0`

If `inEntities` and `work+0x58`: `work+0x58 = 0`. Returns 1. `work+0x50` is not cleared.

### 6.7 LWPOLYLINE `FUN_1404251e0`

Builds a temporary polyline (`FUN_140428e70`, calloc 0x30, ext `(0,0,1)`):

```
term=0; haveX=haveY=0; firstX=firstY=undefined; flags=0
save = ftell
while fgets(code):
   (found, idx, t) = lookup(code, LW, 4); if t: term = 1
   if !fgets(value) return 0
   if found:
      v = atof(value)
      idx 0: haveX=1, x=v, first x kept as firstX
      idx 1: haveY=1, y=v, first y kept as firstY
      idx 4: flags = atol(value); tmp.flag = flags
      idx 2: nothing (a 42 not directly after a vertex is ignored)
      if haveX && haveY:
         haveX=haveY=0; bulge=0; save = ftell
         fgets(code2)            (its NULL return is not tested)
         (f2, i2, t2) = lookup(code2, LW, 4); if t2: term = 1
         if !fgets(value2) return 0
         if f2 || term: bulge = (idx-after-lookup == 2) ? atof(value2) : 0; fseek(save)
         // else: the peeked pair is consumed (not re-read)
         append (x, y, bulge) to tmp
   if term: fseek(save); break
   save = ftell
```

* The bulge is taken only from a `42` **immediately after** the pair that completed the vertex.
  With per-vertex widths `10, 20, 40, 41, 42` the peek consumes the `40` pair, `41` is ignored
  and the `42` is ignored: **the bulge is lost** (case `gfread_dxf_lwpoly_width`).
* No duplicate detection; codes 90, 38, 39, 43, 91 are ignored.
* After the loop: if `flags != 0` (any bit, e.g. 128) → append `(firstX, firstY, 0.0)` (closing
  vertex; the closing segment uses the last real vertex's bulge).
* `FUN_140428cf0`: if the temporary has **exactly 2 vertices** and neither
  `v1.x - v0.x > 1e-8` nor `v1.y - v0.y > 1e-8` (signed, `DAT_1405691a8`), it is dropped
  (freed, nothing stored). A 2-vertex polyline running left or down is therefore lost (case
  `gfread_dxf_lw2pt`).
* Otherwise a new polyline record is appended to `work+0x28` and filled by `FUN_140428b80`
  (flag, ext, vertices copied), **`work+0x50` = it and `work+0x58 = 1`**. The in-polyline flag
  stays set after an LWPOLYLINE, so a later stray VERTEX is appended to it (case
  `gfread_dxf_stray_vertex`).
* Returns 1 (0 only on value-line EOF).

### 6.8 ARC `FUN_140425d50`

Slots: 10 cx, 20 cy, 40 r (accepted only after 10; before that ignored), 50 start°, 51 end°,
210/220/230. Each slot once, duplicates term. Done as soon as 10, 20, 40, 50, 51 are all set
(extrusion not required). Any of the five missing → **return 0**.
Append a 0x40-byte arc record to `work+0x38` (count `+0x30`, `FUN_1404261f0`):
`{cx, cy, r, a50, a51, ext[3]}` — **the extrusion is not copied** (stays `(0,0,0)`), so ARC
entities are never OCS-transformed (case `gfread_dxf_arc_wrap`). Return 1.

### 6.9 CIRCLE `FUN_1404262e0`

Slots 10, 20, 40 (any order), 210/220/230 (default `(0,0,1)`); duplicates term; done when all 6
are set. Missing 10/20/40 → return 0. Appends **three** arc records with the extrusion:
`(cx, cy, r, 0, 120)`, `(cx, cy, r, 120, 240)`, `(cx, cy, r, 240, 360)` (`DAT_140541428` 120.0,
`DAT_140569508` 240.0, `DAT_14051f210` 360.0). Return 1.

### 6.10 SPLINE `FUN_140426810`

1. New polyline record (`work+0x50`, `work+0x58 = 1`), flag 0, ext `(0,0,1)`.
2. Pair loop, **no duplicate detection**; it ends only at a terminator or EOF:
   * 71 → degree `p = (int)v`;
   * 72 → `nk = (int)v`, `knots = malloc(8·nk)`, knot index `ki = 0`;
   * 73 → `nc = (int)v`, `ctrl = malloc(24·nc)`;
   * 40 → `knots[ki++] = v`;
   * 10 → `ctrl[ci].x = v`, seen10 = 1; 20 → `ctrl[ci].y = v`, seen20 = 1;
   * 30 → if seen10 and seen20 (ever): `ctrl[ci].z = v; ci++`. **The control-point index only
     advances on code 30**;
   * 41 (weights), 70, 74, 210–230, 11/21/31 fit points, 12/13 tangents: ignored.
   A 40 before 72 or a 10 before 73 writes through a NULL pointer (crash); missing 30 codes leave
   control points 1.. uninitialised. Not tested.
3. Evaluation `FUN_140463374(ctrl, knots, w, &nc, &p, out, &30, &idx=1, &knots[0], &knots[nk-1])`
   with all weights 1.0 and **30 output points** (`DAT_140628550`), §6.11.
4. The 30 points `(x, y, bulge 0)` are appended to `work+0x50`; `work+0x58 = 0`; buffers freed.
   Return 1. The spline polyline is open (flag 0).

### 6.11 B-spline evaluation (Fortran, `FUN_140463374`, `FUN_1404635be`, `FUN_1404639eb`)

1-based knots `U(1..)`, `n = p+1`, `NK = p + 1 + nc` (from degree and control count, not from 72).

```
for j = 0 .. 29:
   t = (double)j * ((tN - t0) / (double)(30-1)) + t0
   for i = p+1 .. NK:                       first span with U(i) <= t <= U(i+1) (both inclusive)
      B(r,c) = 0 for r,c in 1..n ;  B(n,1) = 1.0
      for k = 1..p:  for m = 1..k+1:  jj = m-1
          d1 = U(i+k-jj) - U(i-jj)
          A  = d1 != 0 ? ((t - U(i-jj)) / d1) * B(p-jj+1, k) : 0
          d2 = U(i+k+1-jj) - U(i+1-jj)
          Bt = d2 != 0 ? ( (p-jj+2 > n) ? 0 : ((U(k+1+i-jj) - t) / d2) * B(p-jj+2, k) ) : 0
          B(n-jj, k+1) = A + Bt
      out = 0, W = 0 (3 each)
      for cp = i-p .. i, row = 1..:  b = B(row, n)
          for d in x,y,z: out[d] = (1.0 * P(cp)[d]) * b + out[d];  W[d] = 1.0 * b + W[d]
      point = out[d] / W[d]                  (z dropped later)
      break
```

The output counter is decremented once at the end; the handler copies all 30 slots regardless
(if a `t` finds no span, later slots are uninitialised).

## 7. Segments `FUN_1404231c0(int* nSeg, int* nPts, double** data, work)`

**Count** `N = nLines + nArcs + Σ_poly (nVert - 1 + (flag != 0 && nVert > 2 ? 1 : 0))`.
`N < 1` → **-500**. A work buffer of `6·N` doubles is calloc'd and filled in this order:

1. **Lines** (file order). `FUN_140427f00(rec)` transforms p1, p2 in place by the extrusion
   (§7.1). Segment `[1.0, x1, y1, x2, y2, 0]`; nPts += 2.
2. **Polylines** (file order; POLYLINE, LWPOLYLINE and SPLINE share the list). For
   `b = 1 .. nVert-1`, `v0 = V[b-1]`, `v1 = V[b]`:
   * `v0.bulge == 0.0` → points transformed (§7.1) → `[1.0, v0', v1', 0]`; nPts += 2.
   * else (bulge arc, all CRT double math, this order):
     ```
     th = 4.0 * atan(v0.bulge)                         (DAT_14051f248, CRT atan FUN_1404865ac)
     mx = (v1.x + v0.x) / 2.0 ;  my = (v1.y + v0.y) / 2.0
     a  = mx - v1.x ;  e = my - v1.y
     h  = sqrt(a*a + e*e)
     if h != 0 && tan(th/2.0) != 0:                    (CRT tan FUN_140487b70)
        d  = h / tan(th/2.0)
        cx = mx + (e*d)/h ;  cy = my - (a*d)/h
        (v0', c') = transform(v0, (cx,cy))             // §7.1, only c' is used
        th >= 0: [2.0, v0.x, v0.y, c'.x, c'.y, th]
        th <  0: [2.0, v1.x, v1.y, c'.x, c'.y, -th]    (sign bit flipped)
        nPts += 3
     else: nothing written (N not reduced → one zero segment at the end)
     ```
     The start point is the **untransformed** vertex; the arc always runs counter-clockwise by
     `|th|` from the start.
   * Closure, when `flag != 0 && nVert > 2`: if the 24 bytes `(x, y, bulge)` of the last and
     first vertex are equal (`memcmp`) → N -= 1. Else transform `(last, first)`;
     if `first'.x - last'.x > 1e-8 || first'.y - last'.y > 1e-8` (signed) → line
     `[1.0, last', first', 0]` (nPts += 2), else N -= 1. A closing edge that runs left/down is
     dropped and the curve stays open (seen with the first `gfread_dxf_polyline_210` draft); an
     LWPOLYLINE with `flags != 0` already ends with the first-vertex copy, so its closure test is
     the `memcmp` case.
3. **Arcs** (ARC and CIRCLE in file order):
   * `r <= 0.0` → N -= 1, skipped.
   * `FUN_140428030(arc)` (only CIRCLE arcs can have a non-default extrusion, §7.1).
   * ```
     s  = a50 * 0.017453292519943295            (DAT_140522fd0)
     sw = a51 - a50 ; if sw < 0: sw = sw + 360.0 ; sw = sw * 0.017453292519943295
     [2.0, cx + r*cos(s), cy + r*sin(s), cx, cy, sw]        (CRT cos 0x140bb3490 / sin 0x140bb3000)
     nPts += 3
     ```

`*nSeg = N` (after the decrements), `*data = FUN_140422d40(N+2)` (`(N+2)·6` doubles, zeroed),
first `N·6` doubles copied. Returns 0 (or -1 on allocation failure). N can reach 0 here
(e.g. only arcs with radius 0); §10 then fails with -1 (case `gfread_dxf_arc_r0`).

### 7.1 OCS transform (`FUN_140427f00`, `FUN_140428030`, `FUN_140428140`)

Skipped when the extrusion is exactly `(0,0,1)` or `(0,0,0)` (`FUN_140427f90`). Otherwise:

```
N  = normalize(ext)                  len = sqrt(x*x + y*y + z*z); len == 0 → unchanged
if G <= |N.x| || G <= |N.y|:  Ax = cross((0,0,1), N)   else  Ax = cross((0,1,0), N)
Ax = normalize(Ax) ;  Ay = normalize(cross(N, Ax))
cross(a,b) = (a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x)
point (x,y) → (x*Ax.x + y*Ax.y,  x*Ay.x + y*Ay.y)      (FUN_140428510; z ignored)
```

The threshold is **G** (`FUN_1404099c0`, the 0.01·size of the previous DXF read, initially 1e-9),
not AutoCAD's 1/64. With `N = (0,0,-1)` and G = 1e-9: `Ax = (-1,0,0)`, `Ay = (0,1,0)`, i.e.
`x → -x` (cases `gfread_dxf_mirror`, `gfread_dxf_circle_mirror`). With G = 0 (after a read that
produced no segments) `Ax = 0` and every point becomes `(0,0)` (case `gfread_dxf_seq_tolzero`).

Arc records (`FUN_140428030`): centre transformed as a point; each angle
(`FUN_1404285b0`): `r = a·π/180; (c', s') = transform(cos r, sin r); a = atan2(s', c') ·
57.29577951308232` (CRT atan2 `FUN_140485d6c`); then if `a51 < a50` (strictly) swap them.

## 8. Size and tolerance `FUN_14027ba50`

For the segment list (mode 2), with `upd(d)`: first value `> 1e-6` (`DAT_14051f1b8`) starts the
minimum, later values replace it when `1e-6 < d < min`.

```
last = seg[n-1]
prev = (int)last.type == 1 ? (last.x2, last.y2) : arcEndF(last)
for each seg s:
   if (int)s.type == 1:
       a = prev.x - s.x1; b = prev.y - s.y1;  upd(a*a + b*b)
       c = s.x1 - s.x2;  d = s.y1 - s.y2;    upd(c*c + (d*d))
       prev = (last.x2, last.y2)                     // always the LAST segment, fields 3,4
   else:
       E = arcEndF(last)                              // the LAST segment again
       a = prev.x - s.x1; b = prev.y - s.y1;  upd(a*a + b*b)
       c = s.x1 - E.x;   d = s.y1 - E.y;      upd(c*c + (d*d))
       prev = E
size = min > 0 ? sqrt(min) : 0 ;   G = tol = size * 0.01
```

Squared distances are compared with 1e-6, so pieces shorter than 0.001 do not count. Because
`prev` is always taken from the last segment (for a last arc, its centre when the current segment
is a line) the size is not the shortest edge: in case `gfread_dxf_tol_quirk` it is 5.0 (arc centre
to a line start) instead of 10, tol = 0.05, and a 0.07 gap stays open.

`arcEndF(s)` (`FUN_1403078f0`, in **float**): `sx,sy,cx,cy,ang` = `(float)` of fields 1–5,
widened to double; `dx = sx-cx; dy = sy-cy; S = fsin(ang); C = fcos(ang)` (Fortran routines,
§11); `y = (dy*C + dx*S) + cy; x = (C*dx + cx) - S*dy` (`FUN_14043658f`); result
`((float)x, (float)y)`.

## 9. Conversion `FUN_14040ba80(iges, obj, tol, nPts, nSeg, data, 700)`

1. Context `DAT_14070bc20` (0x430 bytes) zeroed; real work array `R = DAT_14070c050`
   (`DAT_140628568` = 1 500 000 doubles) and int array `I = DAT_14070c058` (`DAT_140628554` =
   1 500 000 ints) freed, re-allocated and zeroed (failure → -1).
2. `c020 = nPts`, `c024 = nSeg`, `c040 = tol`.
3. Curve 0 of geometry 0 (`FUN_1401982c0(obj, &0)`); created with `FUN_140198270` while missing.
4. `nSeg·6 < 1` → data pointer `c048 = 0` → arrays freed, **return -1** (GFREAD "Failed").
   Else `c048 = data`.
5. `rc = FUN_14040c220(buf, ctx, 2, 700)`: mode 700 → Fortran kind 3, flag 1; first
   `FUN_14040c740(ctx)` (§9.1), then `FUN_140457312` (§10). Status → rc: 1 → -9, 2 → -10,
   3 → **200**, 0 → **0**, other → -11.
6. `rc == 0` or `200`: curve `+0x14 = c024` (segment count after write-back), `+0x10 = 2`,
   old points freed (`FUN_140276030`), `+0x18 = data` (`FUN_14040cde0`); `FUN_140198230(obj, 1)`
   (geometry 0 flag); 200 also sets `iges+0x18 = 1` (`FUN_14040ce40`).
   `obj+0xbc8 = iges+0x1c = (rc == 200 && nSeg >= 2) ? nSeg : 1` (nSeg = count passed in).
   Arrays freed, return 0.
7. Other rc: arrays freed, return rc (data freed by the caller; GFREAD "Done", curve unchanged).

### 9.1 Duplicate removal `FUN_14040c740`

Copy of the `c024` segments; for each kept `i` and each later kept `j`
(`near(a,b,t)` = `b - a <= t && a - b <= t`, `FUN_14040cd30`; type tests with `t = 1e-4`
`DAT_14051f250`):
* both lines (`near(type, 1.0)`): drop `j` if `i.p1≈j.p2 && i.p2≈j.p1` (reversed copy), else if
  `i.p1≈j.p1 && i.p2≈j.p2`; coordinate tolerance = `tol` (`ctx+0x420`).
* else both arcs (`near(type, 2.0)`) and all five fields within `tol` → drop `j`.
Kept segments are compacted in order; `c024` = kept count (case `gfread_dxf_dedupe`).

## 10. Fortran stage `FUN_140457312` (kind 3, flag 1)

Arrays are 1-based. `if tol <= 0: tol = 9.999999717180685e-10` (`(float)1e-9`, `DAT_140bb2110`).
Layout: `P = R[3·nPts0+1 ..]` (3 doubles per point, z = 0), `E = I[5·nSeg+6 ..]` (5 ints per
entity), `D = R[after P]` (one double per point), `M = I[after E]` (2 ints per point), then 5
blocks of `2·nSeg` ints. `nPts0` = the count from §7. If `1 500 000 < I-end` or
`1 500 000 < start of D` → status 1 (-9).

### 10.1 Points and entities `FUN_140458e6f`

For segment k = 1..nSeg, `t = (int)type`:
* `t == 2` (arc): points `start = (x1,y1)`, `centre = (x2,y2)`, `end`:
  `dx = x1-cx; dy = y1-cy; S = fsin(a); C = fcos(a); end.x = cx + (C*dx - S*dy);
  end.y = cy + (S*dx + C*dy)` (`FUN_1404672b2`, `FUN_1404b3b30` = Fortran sin, cos).
  Entity `[1, k, first, first+2]` (Fortran type 1 = arc).
* otherwise (line, also a zero record): points `(x1,y1)`, `(x2,y2)`, entity
  `[2, k, first, first+1]`.
Points are numbered consecutively; `nPts` = 2 per line + 3 per arc.

### 10.2 End-point matching `FUN_140464abc`

```
D[p] = tol, M[p] = (0,0) for all points ;  D[0] reads P[nPts].z = 0.0
for e = 1..nSeg:  for p in (first(e), last(e)):            (arc centres are never endpoints)
   dmin = D[p];  M[p].ent = e
   if dmin != 0:
     for e2 = 1..nSeg, e2 != e:
        for q in (first(e2), last(e2)):
           if p == q: D[p]=D[q]=0; M[p]=(q,e); M[q]=(p,e2); next p
           if D[q] != 0:
              d = sqrt((dz*dz + dy*dy) + dx*dx)   with d* = P[q] - P[p]
              if d < dmin: c = e2; dmin = d; M[p] = (q, e); if d == 0: next p (no update below)
        if dmin < D[p]: q = M[p].pt; D[p] = D[q] = dmin; M[q] = (p, c)
for p = 1..nPts: q = M[p].pt; if q > 0:
   M[q].pt = -M[q].pt
   x = (P[p].x + P[q].x) * 0.5 ; y = (P[p].y + P[q].y) * 0.5
   P[p].xy = P[q].xy = (x, y) ;  D[p] = D[q] = 0
M[p].pt = |M[p].pt| for all p
```

So every end point is paired with the nearest other end point closer than `tol` (strict), and
both are moved to their midpoint (case `gfread_dxf_lines_gap`: the 0.05 gap becomes
`(10.015, 10.02)`). Unmatched ends keep `D = tol`.

### 10.3 Chaining `FUN_140467473`

`mark(i)`: `D[i] = -|D[i]|`, and `-1.0000000133514320e-10` (`(float)-1e-10`) if it was 0.
Loop over points p = 1..nPts with `D[p] >= 0`:
* the **second** such point (second chain) → status **3**: every entity becomes its own chain
  `(first(e), e)` in entity order, `nChains = nSeg`, stop. So a drawing with more than one chain
  (two loops, or loops plus loose pieces) is stored in segment order with original directions,
  only the end averaging applied (case `gfread_dxf_two_loops`).
* the first such point starts the chain: `A = M[p].ent`, `fwd = [(p, A)]`, `bwd = []`,
  `other = (p == first(A)) ? last(A) : first(A)`, `cur = p`, `v = D[p]`:

```
loop:
   loop:                                   (repeat while D[cur]-before-mark != D[q])
      loop:
         d0 = v
         if v < 0 and other == 0: goto done
         if v >= 0: break
         cur = other; other = 0; v = D[cur]          // switch to the other side of A
      v = mark(cur)          (repeated while tol < d0; never true)
      q = M[cur].pt          (0 for an unmatched end; D[0] = 0)
      if d0 == D[q]: break
   mark(q); e = M[q].ent
   if e == A: break                                   // loop closed
   (other == 0 ? bwd : fwd).append((q, e))
   cur = (last(e) == q) ? first(e) : last(e); v = D[cur]
done:
seq = reverse(bwd) + fwd
mark every point first(e)..last(e) of every entity in seq (this also marks arc centres)
if len(seq) > 1: orient(seq)            // FUN_140467e54
```

The walk starts at the lowest-numbered point (entity 1's first point), goes first away from it
through its partner, then (open chains) continues from A's other end. Matching relies on equal
`D` values: matched pairs both hold 0, an open end holds `tol` and its `q = 0` gives 0.

`orient` (`FUN_140467e54`), for consecutive entries k, k+1 (entities a, b; `fa/la`, `fb/lb` their
end points), distances `d1=|fa-fb|, d2=|la-fb|, d3=|fa-lb|, d4=|la-lb|`, best = first strict
minimum in that order; `entry(k) = (best ∈ {1,3}) ? la : fa`. Last entry: from the last pair's
best, `entry(n) = (best ∈ {1,2}) ? fb : lb`. (Its tracking bug — `min = d3` when d4 wins — does
not change the choice.)

Net effect: one chain, traversed from one end (open) or from entity 1 (closed), and entity 1 is
run **against** its file direction: case `gfread_dxf_lines_open` gives
`(3,1)-(2,0), (2,0)-(1,0), (1,0)-(0,0)`; the Rivet/Anvil files start with their first LINE
reversed; a 3-vertex POLYLINE comes out reversed (`gfread_dxf_polyline_open`).

### 10.4 Write-back `FUN_140459149`, `FUN_1404594a3`, `FUN_140459607`

Over chains, then their entries `(entry, e)`, counter `n` = 1, 2, …; `rev = (first(e) != entry)`;
records overwrite `data` from the start:
* arc: `ang = FUN_14046736c(P[f], P[f+2], P[f+1])`, if `ang < 0: ang += 6.2831853071796`
  (`DAT_140639680`, **not** 2π exactly); `rev` → `[2.0, P[f+2], centre, -ang]`, else
  `[2.0, P[f], centre, ang]`. So a reversed arc has a negative sweep (clockwise).
  `FUN_14046736c(a, b, c)`: `ax=a.x-c.x, ay=a.y-c.y, la=sqrt(ay*ay+ax*ax)`, same for b;
  `ang = fatan2(((ax*by - bx*ay)/la)/lb, ((ay*by + ax*bx)/la)/lb)`.
  The sweep is recomputed from the (averaged) points; a full turn gives 0.
* line: `rev` → `[1.0, P[f+1], P[f], 0]`, else `[1.0, P[f], P[f+1], 0]`.
`c024 = n`.

## 11. Fortran runtime math (must be reproduced, not taken from the C library)

`fsin(x)` / `fcos(x)` = `FUN_1404b3960(x, flag 0/1)` (Cody–Waite):

```
ax = |x|; neg = 0
cos: y = ax + 1.5707963267948966          sin: y = ax; if x < 0: neg = 1
if y > 210828714.0: return 0.0
n = (int)(y * 0.3183098861837907 + 0.5)   (truncation); xn = n; if n odd: neg = !neg
cos: xn = xn + (0.0 - 0.5)
f = (ax - xn*3.1416015625) - xn*(-8.908910206761538e-06)
if |f| >= 1.05367121277235e-08:
   g = f*f
   r = ((((((( g*c8 - c7)*g + c6)*g - c5)*g + c4)*g - c3)*g + c2)*g - c1) * f * g    (left to right)
   f = f + r
return neg ? (-0.0 - f) : f
c1..c8 = 0.16666666666666666, 0.008333333333333165, 1.984126984120184e-04, 2.7557319210152756e-06,
         2.5052106798274583e-08, 1.605893649037159e-10, 7.642917806891047e-13, 2.7204790957888847e-15
```

`fatan(x)` = `FUN_1404b3f48`:

```
neg = 0; k = 0; f = x ; if x < 0: f = -0.0 - x, neg = 1
if f > 1.0: f = 1.0/f, k = 2
if f > 0.2679491924311227: f = (((f*0.7320508075688773 - 0.5) - 0.5) + f) / (f + 1.7320508075688772); k += 1
if |f| > 1.05367121277235e-08:
   g = f*f
   p = ((((-8.494624035132068 - g*0.8375829936815006)*g - 20.505855195861653)*g - 13.688768894191927)*g)
   q = (((g + 15.024001160028575)*g + 59.57843614259735)*g + 86.15734959713025)*g + 41.06630668257578
   f = f + (p / q) * f
if k > 1: f = -0.0 - f
k == 1: f += 0.5235987755982989 ; k == 2: f += 1.5707963267948966 ; k == 3: f += 1.0471975511965979
return neg ? -0.0 - f : f
```

`fatan2(y, x)` = `FUN_1404b3878`: `x == 0` → `y == 0 ? 0 : (y >= 0 ? π/2 : -π/2)`; else
`a = fatan(|y/x|)`; `x < 0` → `a = π - a`; sign bit of y set → `a = -0.0 - a`.

The C++ stages (§6–§8 except `arcEndF`) use the VS2008 x64 CRT `atan`, `tan`, `sqrt`, `sin`,
`cos`, `atan2`; a port on the same CRT reproduces them. (The reference model used Python's libm
there and still matched all printed digits.)

## 12. What the object gets; return codes

| Reader result | GFREAD | Object |
|---|---|---|
| 0 | Done | curve 0 = type 2, n = written segments; `obj+0x3a48 = tol`; `obj+0xbc8` per §9; `obj+0x3a40 = 7` (GFREAD) |
| -1 (§9 step 4, allocation failures) | **Failed** | unchanged (curve 0 created if it did not exist) |
| -2, -3, -100…-400, -500, -10000, -9, -10, -11 | Done | unchanged |

* Only curve 0 is replaced. Other curves of geometry 0 and the topology (**TPLOGY** from IN.KEY,
  e.g. RING's two die vertices) stay as they were and are written again by KFWRIT.
* `FUN_140197bc0` does **not** clear the geometry (DXF_IGES_READERS.md §4 says so): it clears the
  undo list `obj+0x3a60` (vtable +8), copies every current curve into it (`FUN_140275cd0`,
  `FUN_14019a620`, `FUN_14019ab80`) and calls vtable +0.
* The DXF path prints nothing of its own; G is always updated (§3).

## 13. KEY lines after KFWRIT

Curve writer `FUN_1402aaa70` (spec `DIEGEO.md` on the group-2 branch):

```
"%-6s %7i %7i %7i\n"   DIEGEO, obj, 2, n
per segment i=1..n:
" %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"   i, (int)type, x1, y1, x2, y2, v5*57.29577951308232
```

Type 1 = line (v5 = 0 → `0.0000000000E+000`), type 2 = arc (`x1,y1` start, `x2,y2` centre,
sweep in **degrees**, negative = clockwise). Exponents have three digits (MSVC). Example
(`gfread_dxf_circle`, CIRCLE centre (3,4) r 2.5):

```
DIEGEO       1       2       3
       1       2    1.7500000000E+000    6.1650635095E+000    3.0000000000E+000    4.0000000000E+000   -1.2000000000E+002
       2       2    5.5000000000E+000    4.0000000000E+000    3.0000000000E+000    4.0000000000E+000   -1.2000000000E+002
       3       2    1.7500000000E+000    1.8349364905E+000    3.0000000000E+000    4.0000000000E+000   -1.2000000000E+002
```

KFWRIT drops a degenerate curve (type 2 with one record whose `(x1,y1) == (x2,y2)` within
1e-12); with no curve left neither DIEGEO nor TPLOGY is written for the object
(`gfread_dxf_seq_tolzero`).

## 14. Work-struct and record layout

| Offset | Work struct (0x60, `FUN_140422f10`) |
|---|---|
| +0x00 | 0 (argument) ; +0x0c = 1 |
| +0x08 | in ENTITIES flag |
| +0x10 / +0x18 | line count / lines (0x38: x1,y1,x2,y2,ext[3]) |
| +0x20 / +0x28 | polyline count / polylines (0x30: nVert, vert*, flag@+0x10, ext[3]@+0x18) |
| +0x30 / +0x38 | arc count / arcs (0x40: cx,cy,r,a50,a51,ext[3]) |
| +0x50 | current polyline (pointer into the polyline array) |
| +0x58 | in-polyline flag |

## 15. Oracle cases

`tests/commands/GFREAD/gfread_dxf_*.json` (2D RING_SI.KEY, `GFREAD 1` of the DXF, `KFWRIT OUT.KEY`);
synthetic files in `tests/commands/GFREAD/data/dxf_*.dxf` (CRLF). Run 2026-10-21; all completed,
every prediction equal to the original's object-1 DIEGEO block (or its absence).

| Case | Confirms |
|---|---|
| `gfread_2d_dxf` (existing), `dxf_inst_anvil`, `_rivetpin`, `_rivettopsheet`, `_rivetbottomsheet` | install LINE files: one closed chain, first LINE reversed, exact coordinates |
| `dxf_lines_open` | open chain order and orientation (§10.3) |
| `dxf_lines_gap` | lower-case names/section, POINT/TEXT skipped, BLOCKS ignored, end averaging |
| `dxf_two_loops` | two chains → status 3, file order, original directions; no other KEY change |
| `dxf_lwpoly_bulge` | bulge arc centre/sign, negative bulge start = end vertex, closed LWPOLYLINE |
| `dxf_lwpoly_width` | 40/41 before 42 lose the bulge; flag 128 closes |
| `dxf_polyline_open` | POLYLINE 70 ignored, VERTEX/SEQEND, reversed output |
| `dxf_polyline_210` | 210 == 1.0 closes a POLYLINE (closing edge running up) |
| `dxf_arc_loop` | ARC start/sweep, mixed arc/line chain, negative sweep on reversed arc |
| `dxf_arc_wrap` | ARC 300→30 sweep 90, ARC extrusion ignored |
| `dxf_circle`, `dxf_circle_mirror` | three 120° arcs; CIRCLE OCS mirror and angle swap |
| `dxf_mirror` | LINE OCS mirror with G = 1e-9 |
| `dxf_spline` | 30-point B-spline sampling + closing LINE |
| `dxf_badline` | an incomplete LINE ends the parse |
| `dxf_lw2pt` | 2-vertex LWPOLYLINE running down-left dropped |
| `dxf_dedupe` | duplicate code 10 ends a LINE; reversed/shifted duplicates removed |
| `dxf_stray_vertex` | VERTEX after LWPOLYLINE appended to it |
| `dxf_tol_quirk` | size from the last-segment quirk (tol 0.05, gap 0.07 not closed) |
| `dxf_empty`, `dxf_binary` | -500 / -10000: Done, curve unchanged |
| `dxf_arc_r0` | N drops to 0 → -1 → **Failed**, curve unchanged |
| `dxf_seq_tolzero` | G persists: after an empty read the OCS threshold 0 collapses mirrored points; KFWRIT drops the degenerate curve |

## 16. Helpers

| Address | Role |
|---|---|
| `FUN_1404093a0` | entry (mode 300) |
| `FUN_140422ab0` | open, parse, segments, size, G |
| `FUN_140428b10` | `fopen(name,"rt")` test (-300) |
| `FUN_140422f10` / `FUN_140424390` (`FUN_140424400/450/510/560`) | work struct alloc / free |
| `FUN_140422f70` | pair loop |
| `FUN_140424c80` | code lookup + terminator |
| `FUN_140427df0` / `FUN_140428650` | trim / case-insensitive compare |
| `FUN_14048754c` / `FUN_14047b6c0` | `atof` / `atol` |
| `FUN_140427ec0` | vector (0,0,1) |
| `FUN_140424b90`, `FUN_1404250f0`, `FUN_1404258d0`, `FUN_1404261f0`, `FUN_140428e70` | append line / polyline / vertex / arc; temp polyline |
| `FUN_140428cf0`, `FUN_140428b80`, `FUN_140428eb0`/`FUN_140428ef0` | 2-vertex check, polyline copy, free |
| handlers `FUN_1404245b0` … `FUN_140426810` | §6 |
| `FUN_140463374`, `FUN_1404635be`, `FUN_1404639eb` | B-spline sampling, basis, point |
| `FUN_1404231c0` | segment array |
| `FUN_140427f00`, `FUN_140428030`, `FUN_140427f90`, `FUN_140428140`, `FUN_140428510`, `FUN_1404285b0`, `FUN_140428850`, `FUN_1404288c0`, `FUN_1404289a0`, `FUN_140428a10` | OCS |
| `FUN_14027ba50`, `FUN_1403078f0`, `FUN_14043658f`, `FUN_1403fb460` | size, float arc end |
| `FUN_140428f70` / `FUN_1404099c0` | set / get G (`DAT_14062bc68`) |
| `FUN_1404098f0`(`FUN_140409910`), `FUN_140409930` | `obj+0x3a50`, `obj+0x3a58` |
| `FUN_1404098d0`, `FUN_1404099a0`, `FUN_140409970` | set `obj+0x3a48`, `dxf+0x20`, `dxf+0x1c` |
| `FUN_140422d40` / `FUN_140422d90` | alloc `(n)·6` doubles / free segment array |
| `FUN_14040ba80`, `FUN_14040c220`, `FUN_14040c740`, `FUN_14040cd30` | conversion, mode 700, duplicates, near |
| `FUN_140457312`, `FUN_140458e6f`, `FUN_140464abc`, `FUN_1404681dc`, `FUN_140467473`, `FUN_140467e54`, `FUN_140459149`, `FUN_1404594a3`, `FUN_140459607`, `FUN_14046736c`, `FUN_1404672b2` | Fortran stage |
| `FUN_1404b3960`/`ab0`/`ab8`, `FUN_1404b3b30`, `FUN_1404b3f48`, `FUN_1404b3878`, `FUN_1404b4898` | Fortran sin, cos, sincos, atan, atan2, fabs |
| `FUN_1401982c0`, `FUN_140198270`, `FUN_140198230`, `FUN_140276030`, `FUN_14040cde0`, `FUN_14040ce40`/`ce60`, `FUN_14019a650` | curve 0 storage |
| `FUN_140197bc0` | undo snapshot |

## 17. Open questions

* `FUN_140198230(obj, 1)` (`FUN_14027fe20` on geometry 0) and `iges+0x18` (status 200): meaning
  not followed; no effect on OUT.KEY in the cases.
* `obj+0xbc8` (1 or nSeg) is not visible in OUT.KEY; its later use (mesh curve count check in
  `FUN_1401bdaf0`) was not followed.
* No setter of `obj+0x3a50/0x3a58` (user tolerance) was found; if one exists the tolerance
  differs from G.
* Kind-3 Fortran write-back for entity types 3/5/6 (IGES only) not needed here.
* Untested by design: SPLINE without 72/73/30 (NULL writes, uninitialised points), the 1.5 M
  work-array limit (-9), lines longer than 511 bytes, a bulge on coincident vertices (trailing
  zero segment), POLYLINE with 0 vertices (count −1).
* Corrections for DXF_IGES_READERS.md §4 (not edited here): `FUN_140197bc0` is an undo snapshot,
  not a clear; the loop ends when a handler returns 0 (incomplete entity), not at ENDSEC/EOF.
