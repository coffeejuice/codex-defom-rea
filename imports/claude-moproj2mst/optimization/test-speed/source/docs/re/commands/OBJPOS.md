# OBJPOS (command id 7): object positioning

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only: `case 7` of the command dispatcher
`FUN_140378e00` (`out/bm_cmd.c` lines 582-1433) and its callees, decompiled with headless Ghidra
(`out/p67/objpos.c`, `out/p67/objpos-geo/kern.c`, `geo.c`) and checked in the listing where the
floating-point order matters. Shared mechanics (dispatch, return codes, readers, test layout):
[_common.md](_common.md). **[fact]** = read in the code, **[inferred]** = deduced.

This file covers the grammar, the full mode table, the post-processing common to all modes and
the geometric modes. The interference modes (2, 12, 102, 112) are in
[OBJPOS_interference.md](OBJPOS_interference.md) and the drop modes (7, 8, 9) in
[OBJPOS_drop.md](OBJPOS_drop.md) (worker objpos-ifd).

## 1. Summary

`OBJPOS obj mode v2 v3 ...` moves, rotates, mirrors or positions object `obj` and optionally a
list of "coupled" objects with it. 3D and 2D read different numbers of values and support
different modes (2D has no drop, reference or two-object modes; 3D mode 5 does nothing) **[fact]**.
Every geometric transform walks the whole object: FE nodes, element tensors, rigid die
geometry, boundary-condition windows, movement data (CNTRAX centres), process data (REFPOS,
ROTSYM, WPAXIS), remesh windows and inter-object windows (section 6) **[fact]**.

Two numeric surprises a port must reproduce:

* the point rotation kernel converts degrees as `(a / 45.0) * (pi/4)` and uses its own Cody-Waite
  `sin`/`cos` (section 6.3), while die-geometry rotation uses `(a * pi) / 180.0` with the CRT
  `sin`/`cos` and a 4x4 matrix product (section 6.4) **[fact]**;
* the 3D mirror runs in **single precision**: every coordinate is converted to `float`,
  reflected with a float 3x4 matrix and stored back as a double. 3D node coordinates lose their
  low bits after mode 4. The 2D mirror is in double **[fact]** (section 6.5, 6.6).

## 2. Argument grammar

### 2.1 First line

`FUN_140413900(f, &n, 10)` reads the rest of the keyword line (`fgets`, `strtok` on
`" ,\n\t\r"`, at most 10 tokens) **[fact]**. EOF returns 1 and the case returns **2** (Reading
Error). Missing tokens convert from `""` to 0 (see `_common.md` section 3) **[fact]**.

| Token | 3D local | 2D local | Conversion |
|---:|---|---|---|
| 0 | object number `local_5708` | same | `strtol(s,NULL,10)` (`FUN_14047b6c0`) |
| 1 | mode `local_5704` | same | `strtol` |
| 2 | v2 `local_5d18` | v2 `local_5d18` | `atof` (`FUN_14048754c`) |
| 3 | v3 `local_5d10` | v3 `local_5d10` | `atof` |
| 4 | v4 `local_5d08` | v4 `local_5d08` | `atof` |
| 5 | v5 `local_5d00` | v5 `local_5d00` | `atof` |
| 6 | v6 `local_5cf8` | **coupled count** `local_5cf8` | `atof` |
| 7 | v7 `local_5cf0` | - | `atof` |
| 8 | v8 `local_5ce8` | - | `atof` |
| 9 | **coupled count** `local_5ce0` | - | `atof` |

Coupled count `nc = (int)(double)token` (truncation, `cvttsd2si`) **[fact]**. In 3D tokens 10+
and in 2D tokens 7+ are ignored. Object and mode are parsed with `strtol`, so `2.0` gives 2 and
`1e1` gives 1 **[fact]**.

### 2.2 Second line (coupled objects)

Only when `nc > 0`: `FUN_140413900(f, &m, 100)` reads the next line (EOF: return 2) **[fact]**.
The list length becomes `min(m, nc)`; entry `i` = `(int)atof(token i)` stored in `local_5cb8[i]`
**[fact]**. Because an empty line still counts one token, `nc > 0` with an empty second line
gives one coupled object number 0 **[fact]**. When `nc <= 0` no second line is read.

When used, each coupled number is taken as `abs()` (`FUN_140484ab8`) and looked up with
`FUN_1401d6ea0`; the result is passed to `FUN_1401611f0` **without a NULL check**, which reads
`obj+0xc`: a coupled number that is not in the object list crashes DEF_PRE (access violation)
**[fact]**. A coupled object of type 7 or without mesh/die only prints the `FUN_1401611f0`
message and is skipped.

### 2.3 Per-mode meaning of the values

3D (`v2..v8` = tokens 2..8):

| Mode | v2 | v3 v4 v5 | v6 | v7 | v8 | Spec |
|---|---|---|---|---|---|---|
| 1, 11 | dx | dy dz, (v5 unused) | - | - | - | 7.1 |
| 2, 12, 102, 112 | interference parameter | direction | reference object (0 = automatic) | - | - | [OBJPOS_interference.md](OBJPOS_interference.md) |
| 3 | origin x | origin y z, direction x | direction y | direction z | angle (deg) | 7.2 |
| 4 | point x | point y z, normal x | normal y | normal z | - | 7.3 |
| 6 | axis x | axis y z, angle (deg) in v5 | - | - | - | 7.4 |
| 7, 8, 9 | see drop spec | | | | | [OBJPOS_drop.md](OBJPOS_drop.md) |
| 13 | unused | direction D | reference object | sub-mode | value | 7.5 |
| 14 | object B | v3 = sub-mode, v4 = interference parameter, v5 = reference object | - | - | - | 7.6 |

2D (`v2..v5`):

| Mode | v2 v3 | v4 v5 | Spec |
|---|---|---|---|
| 1, 11 | dx dy | - | 8.1 |
| 2, 12, 102, 112 | v2 interference parameter, v3 reference object | direction | [OBJPOS_interference.md](OBJPOS_interference.md) |
| 3 | centre x y | v4 angle (deg) | 8.2 |
| 4 | point A x y | point B x y | 8.3 |
| 5 | - | - | 8.4 (reuse the last displacement) |
| 6 | v2 angle (deg) | - | 8.5 |

Corpus usage agrees, e.g. `OBJPOS 4 13 0 -1 0 0 1 2 15 1` (D = (-1,0,0), ref 1, sub-mode 2,
value 15, 1 coupled), `OBJPOS 4 14 5 0 0.001 1` (B = 5, sub-mode 0, tolerance 0.001, ref 1),
`OBJPOS 1 3 0 0 0 1 0 0 22.5 4` + line `4 5 6 7` (Cogging_Multiple_Ops_Lab*.MST) **[fact]**.

## 3. Mode table

The 3D switch is `switch(mode - 1)` with cases 0, 10, 1, 0xb, 0x65, 0x6f, 2, 3, 5, 6, 7, 8,
0xc, 0xd; the 2D switch has cases 0, 10, 1, 0xb, 0x65, 0x6f, 2, 3, 4, 5 **[fact]**.

| Mode | 3D | 2D | Sets "moved" | Sets "rotated" | Sets "mirrored" | Sets "dropped" | STROKE (sec. 5.1) | Re-transform (5.2) |
|---:|---|---|---|---|---|---|---|---|
| 1 | translate | translate | yes | | | | no | no |
| 11 | translate | translate | yes | | | | yes | no |
| 2 | interference | interference | on success* | | | | no | yes |
| 12 | interference | interference | on success* | | | | yes | yes |
| 102 | interference | interference | on success* | | | | no | yes |
| 112 | interference | interference | on success* | | | | yes | yes |
| 3 | rotate about axis | rotate about point | | yes | | | | |
| 4 | mirror at plane | mirror at line | | | yes | | | |
| 5 | **no-op** | translate by last displacement | 2D yes | | | | | |
| 6 | rotate about own centre | rotate about own centre | | yes | | | | |
| 7, 8, 9 | drop | **no-op** | | | | 3D on success | | |
| 13 | reference positioning | **no-op** | on success | | | | | |
| 14 | two-object positioning | **no-op** | never | | | | | |
| other | no-op | no-op | | | | | | |

\* only when the interference did not change the current time (see the interference spec).
A no-op mode still runs the post-processing (section 5) with all flags clear, i.e. nothing
happens and the command returns 0 with no output **[fact]**.

## 4. Common start and error paths

1. Read the lines (section 2). Return 2 on EOF **[fact]**.
2. Locals cleared; `local_5558 = 1`; `local_5560 = 1e29` (`DAT_14052c598`); an empty
   pointer vector `local_5640` is created (`FUN_140382350`, freed by `FUN_1403823c0` on every
   exit path) **[fact]**.
3. `obj = FUN_1401d6ea0(model, objnum)` (no abs). NULL: **return 3 silently** (Data Error
   printed by the interpreter: `ERROR: Keyword OBJPOS Data Error.`) **[fact]**.
4. `FUN_1401611f0(obj)`: type 7 prints `ERROR  : Object %d Undefined\n`; no mesh and no die
   prints `ERROR  : No Mesh or Die Defined for Object %d\n`; either returns 3 **[fact]**.
5. Mode switch (sections 7, 8). Reference-object lookups inside modes 13 and 14 (and the
   interference modes) return 3: silently when the number is not in the list, after
   `ERROR  : Object %d Undefined\n` when its type is 7, after
   `ERROR  : No Mesh or Die Defined for Object %d\n` when `FUN_140160cc0(ref)` is false and
   `ref+0x87c == 0` (3D; 2D uses `FUN_140203980`) **[fact]**.
6. Post-processing (section 5), then return 0 **[fact]**.

`FUN_140160cc0(o)`: if `o+15000` (owning model) is non-NULL and 2D, `FUN_140160d20(o)` (2D
geometry edge list at `o+0x3998` non-empty); otherwise `o+0x3990` (3D rigid-geometry flag)
**[fact]**. `FUN_140203980(o)`: 3D `FUN_140203a20` = `o+0x3990` and geometry 0 has points and
faces; 2D `FUN_1402039d0` = edge list non-empty and `FUN_140304e40(o,0)` **[fact]**.

## 5. Post-processing common to all modes

Run after the switch with `obj` = the main object **[fact]**.

### 5.1 STROKE update

```
upd = (mode == 11 || mode == 12 || mode == 112)      // 0x0b, 0x0c, 0x70; NOT 102
3D: upd = upd && moved                               // local_5547
if (obj+0x21c8 == 4) upd = false                     // movement type 4 (mv+0x68)
if (upd) obj+0x22d0 += G.x; obj+0x22d8 += G.y; 3D only: obj+0x22e0 += G.z
```

`G` = the global last displacement `DAT_140706bb0/bb8/bc0` (section 5.4). obj+0x22d0 is
`mv+0x170` = STROKE **[fact]**. 2D does **not** test `moved`: after a failed 2D interference
(modes 12/112) STROKE is still incremented by whatever `G` holds **[fact]**. Movement type 4 is
excluded because the translate itself already updates STROKE for that type (section 6.1, movement)
**[inferred]**.

### 5.2 Movement-direction re-transform (modes 2, 12, 102, 112)

The tests use the raw mode values 2, 0xc, 0x66, 0x70 = modes **2, 12, 102, 112** **[fact]**.

```
retr = false
if (mode in {2,12,102,112}):
  3D: retr = FUN_14023ace0((v3,v4,v5), (0,0,0), 1e-12)   // |v3|,|v4|,|v5| all < 1e-12
  2D: retr = FUN_140308d60((v4,v5),    (0,0),   1e-12)
```

`FUN_14023ace0` / `FUN_140308d60` return 1 when every component difference has
`fabs(...) < eps` (`FUN_140484558` = fabs, eps `DAT_14051f260` = 1e-12) **[fact]**. So `retr` is
true when the interference direction was given as zero (the interference then follows each
object's own movement direction **[inferred]**). For each coupled object, when `retr`:

```
L = FUN_140239a30(&G)       3D: sqrt((G.x*G.x + G.y*G.y) + G.z*G.z)
                            2D: FUN_1403087e0 = sqrt(G.x*G.x + G.y*G.y)
FUN_1401952e0(c+0x2160, 0.0, L, &disp)
```

`FUN_1401952e0(mv, p2, p3, out)` (same 3D routine in 2D; in 2D `mv+0x60` is 0) **[fact]**:
`u = normalize(mv+0x50..0x60)` by `FUN_140239b40` (`n = sqrt((x*x+y*y)+z*z)`; if
`fabs(n) <= 1e-12` then `u = 0` else `u = (x/n, y/n, z/n)`); `l = |u|` (same sqrt). If
`fabs(l) > 1e-6` (`DAT_14051f1b8`) and `fabs(p3) > 1e-6`: `out = ((p3 - p2)*u.x, (p3 - p2)*u.y,
(p3 - p2)*u.z)` (`FUN_140239ea0`, scalar times component), else `out = 0` **[fact]**. The
coupled object is then moved by `out` instead of `G`: same distance, along its own MOVCTL
direction [listing-checked]. `out` overwrites the shared copy (`local_54c8` / `local_5540`), which is harmless
because every object recomputes it from `G` **[fact]**.

### 5.3 Coupled-object loop

`disp = G` copied once before the loop (2D: `disp.z = 0`) **[fact]**. For `i = 0..n-1`:

```
c = FUN_1401d6ea0(model, abs(list[i]));    // NULL -> crash in FUN_1401611f0
if (!FUN_1401611f0(c)) continue;           // prints its error message
if (moved):
   if (retr) FUN_1401952e0(c+0x2160, 0.0, L, &disp)       // section 5.2
   FUN_14021d0b0(c, &disp, 1, 1)                          // translate, section 6.1
   3D printf("Object %d has been moved by (%g, %g, %g).\n", c#, disp.x, disp.y, disp.z)
   2D printf("Object %d(coupled) has been moved by (%g, %g).\n", c#, disp.x, disp.y)
   if (upd) c STROKE += disp   (3D 3 comps, 2D 2 comps)   // upd from the MAIN object
if (rotated):   same call and printf as the main rotation (section 7.2 / 8.2), same origin,
                direction and angle (mode 6: the main object's centre)
if (mirrored):  same call and printf as the main mirror
3D if (dropped): FUN_1401fe240(c, local_5628); printf("Object %d has been moved by drop positioning.\n", c#)
```

**[fact]**. `upd` (5.1) is the main object's flag, the coupled object's own movement type is not
tested **[fact]**. Mode 14 never sets `moved`, so its coupled list is read but unused **[fact]**.

### 5.4 Global last displacement `G = DAT_140706bb0, bb8, bc0`

Process-lifetime globals (zero at start **[inferred]**, `.data`/`.bss`). Written by **[fact]**:

| Writer | Value |
|---|---|
| 3D mode 1/11 | `(v2, v3, v4)` (after the translate) |
| 2D mode 1/11 | `x, y = (v2, v3)`; `bc0` keeps its old value |
| 3D mode 13 | YZ-centering shift of the reference (7.5), then the final displacement |
| 3D mode 14, sub-mode != 0 | the displacement of object A (7.6) |
| interference modes | passed by address to `FUN_14019fa00` (see the interference spec) |

2D mode 5 translates by `(G.x, G.y)` without changing `G`, i.e. "repeat the last
displacement" (for example after an interference positioning of another object) **[fact]**.
Since `G` survives across commands and across KFREAD/DBREAD, a 2D mode 5 as the first OBJPOS of
a run translates by (0,0) (no-op inside `FUN_14021d0b0`) **[inferred]**.

## 6. Transform kernels

General: the C++ helpers in this module are compiled without optimisation (every
intermediate goes through the stack), so the decompiled expression order is the executed order;
it was spot-checked in the listing (`FUN_14023bd90`, `FUN_14032ab90`, the float mirror helpers)
**[fact]**. The Fortran kernels (`FUN_140325691`, `FUN_1404cf5c0`, `FUN_1404ded0c`, sin/cos)
were read in the listing **[fact]**. All arithmetic is IEEE double SSE2 unless marked float.

### 6.1 Translate `FUN_14021d0b0(o, d, &flagA, &flagB)`

Both flags are `char *` (by reference). OBJPOS always passes `flagA = flagB = 1` **[fact]**
(GENAXS `FUN_1401af680` also passes `(0,1)`, per the gencmds worker). What they control **[fact]**:

| Argument | Name here | When 0 | When 1 |
|---|---|---|---|
| 3rd (`param_3`, OBJPOS `local_1a5`) | flagA | windows of *other* objects / of inter-object entries not owned by `o` that follow `o` (`w+0x36c == o#`) are left in place | they are moved too (`FUN_14019ef20`, step 4) |
| 4th (`param_4`, OBJPOS `local_1a6`) | flagB | rigid die geometry (`o+0x3c00[k]`, 2D edge curves) and the mesh-edge display objects are not moved | moved (`FUN_1402000a0`, step 2) |

Everything else (nodes, FLDGEO/FLWNET, own windows, movement, process data, owned inter-object
windows `FUN_14019f100`) is moved regardless of the flags. Object fields used for the
branches: `o+0x39c0` = object dimension (copied from model+0x1c at construction, per the
coordinator; `FUN_140158ce0`), `o+0x3990` = has rigid die geometry (also selects the
bounding-box source in modes 13/14) **[fact]**. Order of work:

1. Skip everything when `d` is NULL or all components are tiny: 3D `FUN_14023ace0(d, 0, 1e-30)`,
   2D (`FUN_140158d00`: `o+0x39c0 == 2`) `FUN_140308d60(d, 0, 1e-30)` (`DAT_14051bee0`)
   **[fact]**.
2. `flagB`: `FUN_1402000a0(o, d)`, rigid geometry **[fact]**:
   * 3D (`o+0x3990` set): for `k < o+0x3994`: `geo = o+0x3c00[k]`, virtual `+0x38`
     = `_OBJ_GEO3D` `FUN_1402fb480`: every point of `geo+0x10` (count `FUN_1400034d0`, 3 doubles
     each) `p[j] += d[j]`. The list `geo+0x28` is **not** translated (it is rotated and mirrored)
     **[fact]**. Then, when the edge list `o+0x3998` is non-empty, for each item of the second
     list (`FUN_1401983e0` count, `FUN_140197f40`): unless `item+0x31`, `item+0x38+0x60..0x70 += d`
     (`FUN_1402816c0`/`FUN_140285550`, a 4x4 transform of a display object **[inferred]**).
   * 2D: when the edge list is non-empty, for each edge `FUN_1401982c0(o,i)` virtual `+0x38`
     = `_OBJ_GEO2D` `FUN_140277b00`: record width `w = FUN_1402760e0`, for each record
     (`+0x14` records at `+0x18`): kind `+0x10 == 1` -> `r[0] += dx, r[1] += dy`; otherwise
     `r[1] += dx, r[2] += dy, r[3] += dx, r[4] += dy` **[fact]** (vtable from the class table;
     that the edge object is an `_OBJ_GEO2D` is **[inferred]**).
3. `FUN_140200610(o, d)`, only when `o+0x87c` (mesh) **[fact]**: node container `o+0x8a0`
   (`FUN_140181cb0`): RZ (`o+0x8a8`, width `W[0]` = dim) `x[n][j] += d[j]` for every node
   (`c+300` = node count). Other node arrays (DRZ, URZ, ...) are unchanged. Then FLDGEO
   (`o+0x3328`) and FLWNET (`o+0x3458`) surface meshes when present (`FUN_1402f9360`: points and
   faces > 0): virtual `+0x38` (`_OBJ_GEO3D` translate **[inferred]**).
4. `FUN_14021d190(o, d, &flagA)` **[fact]**:
   * remesh windows `o+0x2d10+0x40` (MGUSER list, `FUN_140269eb0`): every window
     (`FUN_140268040`): points `w+0x180` (count `w+0x178`, 2 or 3 doubles per point: 2 when
     `w+0x898 == 2`) `+= d`; when `w+0x898 == 3` `FUN_140267760` recomputes the box faces;
   * BC windows `o+0x2e68` (`FUN_14021f7c0`): 3D list `+0x18` (SYMSUF, `o+0x2e80`): item `+8`
     point += d (`FUN_1402576e0`); window lists `+0x78` (WINTMP) and `+0xa8` (WINATM) as above.
     WINPRS (`+0x48`), WINRSE (`+0x168`) and the LOC* lists are **not** translated **[fact]**;
   * movement `o+0x2160` (`FUN_14021dba0`): CNTRAX centre `mv+0x630..` (2 or 3 comps) `+= d`;
     3D also CNTRA2 centre `mv+0x760..`. Both centres are moved **unconditionally**: no test of the
     axis `mv+0x648` / `mv+0x778` and no die-geometry gate (unlike the rotate, 6.2 step 4, and the
     drop's `FUN_14021dfa0`) (confirmed by oracle run: `OBJPOS/1-basic_3d`, `11-basic_3d` shift
     CNTRAX and CNTRA2 with a zero axis; the interference modes use the same translate); if movement type `mv+0x68 == 4`:
     `s = FUN_140194f90(mv, d, dir)` = projection of `d` on the normalised movement direction
     (3D `FUN_14023b720` normalise + `FUN_14023b3a0` dot; 2D `FUN_1403091c0`/`FUN_1403090e0`;
     0 when the direction length is not > 0), then STROKE `mv+0x170.. += dir_raw[j] * s` where
     `dir_raw` is the **unnormalised** copy of `mv+0x50` **[fact]** [listing-checked];
   * process `o+0x1ea0` (`FUN_14021f210`): REFPOS point `+0x30..` (= `o+0x1ed0`) when REFPOS type
     `+0x28 == 1`; 3D: ROTSYM point `+0x1c0` (`o+0x2060`) when the ROTSYM axis `+0x1d8` is not
     ~0 (test `FUN_14023ace0(axis, 0, 1e-6)` false); WPAXIS list `+0x200` (`o+0x20a0`,
     `FUN_14025a410` -> `FUN_1402595a0`): item `+8` += d; types (`+4`) 1 and 3 also `+0x20`;
     type 4 also `+0x98` **[fact]**;
   * sub-object `o+0x2be8` (`FUN_140220080`, not identified): if `+0x81`: `+0xa0..`, `+0xb8..`
     += d; if `+0xd0`: `+0x108..` += d (3 comps) **[fact]**;
   * `flagA` and owning model `o+15000`: `FUN_14019ef20(m, d, o#)` moves windows that belong to
     *other* objects but follow this one: for every other usable object its remesh windows
     (`+0x2d10+0x40`) and window lists `+0x48, +0x78, +0xa8, +0x168` of `+0x2e68` whose
     `w+0x36c == o#` (`FUN_14021d290`); and for every inter-object entry `E` (`m+0x13c8`) not
     owned by this object (see next item) its windows `E+0x188` with `w+0x36c == o#` **[fact]**;
   * `FUN_14019f100(m, d, o#)`: for every entry `E` with (`E+0x170 == 2` and master
     `E+0x168 == o#`) or (`E+0x170 == 1` and slave `E+0x16c == o#`): all windows `E+0x188` += d
     (inter-object field names: `interobj.md`) **[fact]**.

All translations are `x = x + d` per component, one rounding each **[fact]**.

### 6.2 Rotate `FUN_14021d2f0(o, origin, dir, angle)`

3D passes `dir`, 2D passes `dir = NULL` **[fact]**. Nothing happens when
`fabs(angle) < 1e-30` **[fact]**. `angle` is in degrees everywhere at this level. Order:

1. `FUN_1402002b0` rigid geometry **[fact]**:
   * 3D (`o+0x3990` set and `dir != NULL`): for each `geo[k]`, virtual `+0x40` = `FUN_1402fb550`:
     the list `geo+0x28` items (`FUN_140258400` -> `FUN_140257750`): point `+8` rotated with
     6.3, direction `+0x20` = `R(dir) - R(0)` then normalised (`FUN_14023b720`); then
     `rad = (angle * pi) / 180.0` (`DAT_14051f230`, `DAT_14051f228`) and every point of
     `geo+0x10` is multiplied by the matrix of 6.4. Then, when the edge list `o+0x3998` is
     non-empty, the same 6.4 matrix (built from `(angle * pi) / 180.0`) is multiplied into each
     display object of the second list (`FUN_140281660`, `M = M_item * M`) unless `item+0x31`.
   * 2D (edge list non-empty): each edge virtual `+0x40` = `FUN_140277c50`: kind 1 records rotate
     `(r[0], r[1])`; other records rotate `(r[1], r[2])` and `(r[3], r[4])`, all with 6.6a.
2. `FUN_1402006c0` (only with mesh) **[fact]**:
   * node container `FUN_140181db0(o+0x8a0, origin, dir, angle, true)`: 3D (container model
     `+0x138 == 3`, needs `dir`) or 2D (`== 2`). `z0 = R(0)`. RZ: every node `R(x)` in place.
     Then the vector fields `c+0x10..c+0x38` = DRZ `o+0x8b0`, URZ `+0x8b8`, FRZ `+0x8c0`,
     *FRZB `+0x8c8`, PRZ `+0x8d0`, *PRZB `+0x8d8`, each when allocated: `v = R(v) - z0`
     (3D `FUN_14023b1f0`, 2D `FUN_140308fb0`, componentwise subtraction). In 2D a vector field is
     rotated only when its width `W[k] == 2` (so 3-component URZ/FRZ of GEOTYP 3 stay unrotated)
     **[fact]**. Row stride is the field width.
   * element container `FUN_140184ac0(o+0x9e0, origin, dir, angle)`: STRESS `o+0xa00` (kind 1),
     STNCMP `o+0xa10` (kind 2, in blocks of 6 (3D) / 4 (2D) components, block count
     `width / 6` or `width / 4`), YLDS `o+0xa98` (kind 1) are rotated as symmetric tensors
     by `FUN_140325884` (3D) / `FUN_14032e585` (2D); 2D tensors only when the width is 4.
     MATAXI `o+0xa40`: 3D with width 9, the three axis vectors of each element as
     `v = R(v) - z0`; 2D: `axi += angle` (degrees, per element) **[fact]**. Kernel details:
     section 6.7.
   * FLDGEO / FLWNET surface meshes virtual `+0x40` (as geometry, 6.4) **[inferred]**.
3. Remesh windows `o+0x2d10+0x40` and BC window lists `o+0x2e68` `+0x78`, `+0xa8`
   (`FUN_140268160`): every point rotated (3D 6.3; 2D 6.6a; 3D windows need `dir`);
   box windows (`+0x898 == 3`) refresh their faces (`FUN_140267760`). 3D SYMSUF list `+0x18`:
   `FUN_140257750` as above **[fact]**.
4. Movement, **only if** `FUN_140203980(o)` (object has die geometry): 3D: CNTRAX centre
   `mv+0x630` and axis `mv+0x648` when the axis is not ~0 (1e-12 test): centre `R(c)`, axis
   `R(a) - R(0)` then normalised; same for CNTRA2 (`mv+0x760`, `mv+0x778`). 2D: CNTRAX centre
   with 6.6a. The movement **direction `mv+0x50` is never rotated** **[fact]**. The axis test gates
   the centre too: with a zero CNTRAX axis neither centre nor axis is touched (confirmed by oracle
   run: `OBJPOS/3-off_3d` rotates object 1 about (10,-5,3) and its CNTRAX centre stays (0,0,0)).
5. Process `o+0x1ea0` (`FUN_14021f390`): REFPOS point when type 1 (3D 6.3, 2D 6.6a); 3D ROTSYM
   point `R(p)` and axis `R(a) - R(0)` normalised when the axis is not ~0 (1e-6 test); 3D WPAXIS
   items (`FUN_1402596a0`): types 1, 3: both vectors `+8`, `+0x20` rotated as points; other
   types: `+8` as point, `+0x20` as direction (`R(v) - R(0)`, normalised) when not ~0 (1e-6)
   **[fact]**.
6. Sub-object `o+0x2be8`: if `+0x81`: `+0x88`, `+0xa0`, `+0xb8` rotated as points; if `+0xd0`:
   `+0xd8`, `+0xf0`, `+0x108` (3D kernel 6.3; in 2D `dir` is NULL, so a set flag would crash
   **[inferred]**) **[fact]**.
7. `FUN_14019f2c0(m, origin, dir, &angle, o#)`: windows of the inter-object entries owned by the
   object (same ownership rule as `FUN_14019f100`) rotated by `FUN_140268160` **[fact]**. There is
   no counterpart of `FUN_14019ef20` (windows of other objects that follow this object are not
   rotated) **[fact]**.

### 6.3 3D point rotation `FUN_140325691(P, O, D, &a)` (Fortran **[inferred]** position.f)

Oracle check: RZ, DRZ, URZ (and SYMSUF points / directions) of `OBJPOS/3-*`, `gk-tensor-*` and
`v-bits-rot_3d` agree with this section; in `v-bits-rot_3d` (40.6 degrees, an angle where the
6.3.1 kernel and the AMD libm of 6.4 give different bits) every rotated RZ/DRZ double written by
GENDB is bit-identical to 6.3 + 6.3.1 and about a third differ with the AMD sin/cos (confirmed by
oracle run). With `D = 0` and no earlier rotation the static frame is all zero and every node ends
at `O` (`OBJPOS/v-dir0_3d`: all RZ = (0,0,0)).

Per call (one point, frame rebuilt every time) **[fact]** (listing 0x140325691-0x140325883):

1. Frame `T` (9 doubles, column-major `T(j,k) = T[(k-1)*3 + (j-1)]`, static at `0x140701790`)
   by `FUN_1404cf5c0(D, T)` with `a = D.x, b = D.y, c = D.z`:
   * if `a == 0 && b == 0 && c == 0`: set flag `0x140730310 = 1` and **return without writing
     T** (the previous T is reused; at program start T is all zero, which maps every point onto
     `O`) **[fact]**;
   * `r1 = sqrt(a*a + c*c)`, `r2 = sqrt((b*b + a*a) + c*c)`, `s = r1 / r2`, `t = b / r2`;
   * if `r1 < 1e-9` (`DAT_140643930`; NaN also takes this branch): `T = [1, 0, 0, 0, s, t, 0,
     -t, s]`;
   * else `ca = a / r1`, `cc = c / r1`: `T = [cc, -(t*ca), s*ca, 0, s, t, -ca, -(cc*t), cc*s]`.
     (`-x` is a sign-bit flip.) Row 3 of T is the unit axis.
2. `rad = (a_deg / 45.0) * 0.7853981633974483` (`DAT_140baea58`, `DAT_140baea60`; divide then
   multiply). `sn = SIN(rad)`, `cs = COS(rad)` with the kernel of 6.3.1.
3. Forward (`FUN_1404ded0c`, mode 1): for `j = 1..3`:
   `L_j = ((0.0 + (P1-O1)*T(j,1)) + (P2-O2)*T(j,2)) + (P3-O3)*T(j,3)` (the `0.0 +` is executed:
   it only turns `-0.0` into `+0.0`).
4. In-plane rotation: `x' = (L1*cs) - (sn*L2)`, `y' = (sn*L1) + (L2*cs)`, `z' = L3`.
5. Inverse (`FUN_1404ded0c`, mode 2): for `j = 1..3`:
   `P_j = (((0.0 + x'*T(1,j)) + y'*T(2,j)) + z'*T(3,j)) + O_j`.

The routine has more arguments in some callers (a 5th dimension argument); it is ignored
**[fact]** (register use in the listing).

#### 6.3.1 SIN / COS of the Fortran runtime (`FUN_1404b3960`, flag 0 = sin, 1 = cos)

Not the CRT functions; a Cody-Waite reduction with a degree-8 odd polynomial **[fact]**
(listing 0x1404b3960-0x1404b3aaf; constants read from the image):

```
y   = fabs(x);  neg = (flag == 0 && x < 0)
if (flag == 1) y = y + 1.5707963267948966           // 0x14051f238 (pi/2), only for the range test and N
if (y > 210828714.0) return 0.0                      // 0x1405724a0 (+0.0 for both sin and cos)
N   = (int)(y * 0.3183098861837907 + 0.5)            // 0x140572498, 0x14051f240; cvttsd2si truncation
XN  = (double)N; if (N odd) neg = !neg
if (flag == 1) XN = XN + (0.0 - 0.5)                 // = XN - 0.5
f   = (fabs(x) - XN*3.1416015625) - XN*(-8.908910206761538e-06)   // C1 0x140572488, C2 0x140572490
if (fabs(f) >= 1.05367121277235e-08) {               // 0x140572480
   g = f*f
   r = ((((((( g*2.7204790957888847e-15 - 7.642917806891047e-13)*g + 1.605893649037159e-10)*g
             - 2.5052106798274583e-08)*g + 2.7557319210152756e-06)*g - 1.984126984120184e-04)*g
             + 8.333333333333165e-03)*g - 1.6666666666666666e-01)
   f = f + (r*f)*g
}
return neg ? (-0.0 - f) : f
```

Constants: 0x140572478..0x140572440 in the order used; `-0.0` at 0x14056ef60. `FUN_1404b3b30(x,
&s, &c)` calls it twice (sin then cos) **[fact]**.

### 6.4 3D die-geometry rotation matrix `FUN_1402214c0(M, O, D, &rad)`

4x4 doubles, row-vector convention (`p' = [p 1] * M`) **[fact]**:

* `A = T(-O)`: identity with row 3 = `(-O.x, -O.y, -O.z)` (sign flip) built as `I * T`
  (`FUN_140221c20`, a full product with the identity);
* `R = FUN_140221cd0(copy of D, rad)`: `n = sqrt((x*x + y*y) + z*z)` (`FUN_140484644` = CRT-checked
  `sqrtsd`); if `n < 1e-7` (`DAT_140530cf8`) R stays the identity; else `x/=n, y/=n, z/=n`,
  `c = FUN_140bb3490(rad)` (cos), `s = FUN_140bb3000(rad)` (sin) — CRT SSE2 routines, not the
  6.3.1 kernel **[fact]** (which is cos/sin is **[inferred]** from the formula). With `k = 1.0 - c`:
  `R[0][0] = c + ((k*x)*x)`, `R[1][0] = ((k*x)*y) - (z*s)`, `R[2][0] = ((k*x)*z) + (y*s)`,
  `R[0][1] = ((k*x)*y) + (z*s)`, `R[1][1] = c + ((k*y)*y)`, `R[2][1] = ((k*y)*z) - (x*s)`,
  `R[0][2] = ((k*x)*z) - (y*s)`, `R[1][2] = ((k*y)*z) + (x*s)`, `R[2][2] = c + ((k*z)*z)`
  (index = row*4 + col as in the decompile: `param_1[4]` is `R[1][0]`);
* `C = T(+O)`; `M = (A * R) * C` with `FUN_140221a90`: `out[i][j] = sum_{k=0..3}` starting
  from 0.0, k ascending, `a[i][k]*b[k][j]`.
* Apply (`FUN_14027f460`): `v = (x, y, z, 1.0)`; `p'_j = (((0.0 + v0*M[0][j]) + v1*M[1][j]) +
  v2*M[2][j]) + v3*M[3][j]`, j = 0..2 [listing-checked] (R matrix, `(angle * pi) / 180.0` in `FUN_1402fb550`, products).
* Bit-exact against the oracle (confirmed by oracle run, `OBJPOS/v-bits-rot_3d`, die object 2):
  all 234 DIEGEO doubles in the GENDB output equal this exact sequence (I*T, (A*R)*C, apply) with
  the AMD libm sin/cos of OBJPOS_geo_kernels.md 3.1; with the 6.3.1 kernel or the host UCRT
  sin/cos only 63 of 234 match, and a shortcut `(p - O)*R + O` is not bit-exact either. So the
  die geometry and the mesh of the same object rotate with different sin/cos and formulas.

### 6.5 3D mirror `FUN_1401fd850(o, P, N)` — single precision

Skipped entirely when `N` is ~0 (`FUN_14023ace0(N, 0, 1e-12)`) **[fact]**. Matrix
(`FUN_140240340`, all `float`, `P`, `N` first rounded to float) **[fact]** (listing checked):

```
k   = 2.0f / ((n0*n0 + n1*n1) + n2*n2)                    // FUN_140239960, DAT_140530be0 = 2.0f
H00 = 1.0f - (k*n0)*n0   H11 = 1.0f - (k*n1)*n1   H22 = 1.0f - (k*n2)*n2      // DAT_14053176c
H01 = H10 = ((-k)*n0)*n1  H02 = H20 = ((-k)*n0)*n2  H12 = H21 = ((-k)*n1)*n2
H row 3 = (0,0,0,1), H[i][3] = 0
Tm  = identity (DAT_1405e3070) with Tm[i][3] = 0.0f + (-p_i)
M[i][j] = (((0.0f + H[i][0]*Tm[0][j]) + H[i][1]*Tm[1][j]) + H[i][2]*Tm[2][j]) + H[i][3]*Tm[3][j]
M[i][3] = M[i][3] + p_i                                   // FUN_14023fdf0
apply (FUN_140240980): q_i = ((M[i][0]*x + M[i][1]*y) + M[i][2]*z) + M[i][3]   (float)
```

The 0x1406fe9f0 accumulator is a zero matrix in the image **[fact]**, assumed never written
**[inferred]**. Confirmed by oracle run (`OBJPOS/v-mirror-bits_3d`): every mirrored RZ (192) and
DIEGEO (168) double in the GENDB output equals this float computation; a double-precision mirror
differs from the KEY output by up to 4e-6 (`OBJPOS/4-oblique_3d`). Points are converted with `(float)double` (round to nearest) and back with
`(double)float` (exact) **[fact]**. Order of work:

1. Back up the boundary surfaces (`FUN_14017ec20(o+0xbe0)`) and the 9 boundary-face arrays
   `o+0xae8..o+0xb28` (`FUN_140185a10`, `o+0x88c` faces) when `o+0x88c > 0` **[fact]**.
2. Rigid geometry `FUN_1401fddc0`: each `geo[k]` (`FUN_1402fbbe0`): points mirrored (float);
   face node order reversed (`FUN_1402fbf60`: per face swap nodes 0<->3 and 1<->2; if the old 4th
   entry was negative (triangle marker): node0 := abs(node0), node3 := -abs(node3)); list
   `geo+0x28` mirrored (`FUN_1402578d0`) **[fact]**.
3. Mesh `FUN_1401fde90` (when `o+0x87c`): **only RZ** (`o+0x8a8`) is mirrored (float); velocities,
   forces and element tensors are **not** **[fact]**. Element orientation fixed by
   `FUN_1401fe410` on ELMCON `o+0x9e8` (`o+0x894` nodes per element): element kind `o+0x890 == 2`
   (tetra) swap nodes 1<->2; `== 4` (brick) swap 1<->3 and 5<->7 (0-based) **[fact]**. FLDGEO /
   FLWNET meshes mirrored like geometry.
4. `FUN_1401fc290`: rebuild the boundary surfaces from the elements (3D `FUN_140354ed0`, Fortran
   boundary extraction; then `FUN_140163300`, `FUN_140200a70`, face lists copied into
   `o+0xbf8`, `o+0xc00`), `FUN_140165d30` (face count `o+0x88c`), `FUN_1401631e0` **[fact]**
   (Fortran routine, face order and re-mapping: [OBJPOS_interference_kernels.md §1.7](OBJPOS_interference_kernels.md#17-use-by-the-3d-mirror-fun_1401fc290obj-objposmd-65-step-4)).
   Confirmed by oracle run: FNDBND + run sort + the set-inclusion re-mapping reproduce the ECCTMP
   face list (order and node order) of `OBJPOS/4-basic_3d`, `4-oblique_3d`, `ik-mirror-brick-3d`
   (36 faces) and `ik-mirror-tet-3d` (144); the DIEGEO face reversal of step 2 also matches.
5. Remesh windows (`FUN_14026a0a0` -> `FUN_140268930`: points, float), BC window lists `+0x78`,
   `+0xa8` and SYMSUF (`FUN_140258500`); movement `FUN_1401fea60`: CNTRAX centre `M(c)`, axis
   `M(a) - M(0)` (float subtraction, then widened), CNTRA2 centre and axis the same way (no
   normalisation, no "die geometry" gate); process `FUN_1401ff040`: REFPOS point when type 1;
   sub-object `o+0x2be8` (`FUN_14021f940`); mesh-edge display objects (`FUN_140281710`, float);
   inter-object windows of owned entries (`FUN_14019f660`) **[fact]**.
6. Restore face data: the backup surfaces are re-oriented (`FUN_140201fb0`), and for every old
   face `i` the new index `FUN_140200ff0(o, n0, n1, n2, n3)` receives the old values of
   `o+0xae8` (int), `+0xaf0` (int), `+0xaf8` (8 B), `+0xb00`, `+0xb08` (int), `+0xb10` (8 B),
   `+0xb18`, `+0xb20` (int), `+0xb28` (8 B) **[fact]** (face-field names: objarrays.md, ECCDEF
   family).

### 6.6 2D kernels

**6.6a rotate point `FUN_14032ab90(P, &a, O)`** (listing checked) **[fact]**:
`dx = P.x - O.x; dy = P.y - O.y; rad = (a / 45.0) * 0.7853981633974483`
(`DAT_140baeb08`, `DAT_140baeb10`); `sn, cs` by 6.3.1;
`P.x = ((dx*cs) - (dy*sn)) + O.x`; `P.y = ((dy*cs) + (dx*sn)) + O.y`.

**6.6b mirror point at the line A-B `FUN_140309640(P, A, B)`** (double) **[fact]**:
skipped when `A ~ B` (1e-12); `d = B - A`; `n = sqrt(d.x*d.x + d.y*d.y)`; if `fabs(n) <= 1e-12`
`u = 0` else `u = (d.x/n, d.y/n)`; `w = P - A`; `t = w.x*u.x + w.y*u.y`; `q = (t*u.x, t*u.y)`;
`r = q - w`; `s = (2.0*r.x, 2.0*r.y)`; `P = P + s` [listing-checked].

**2D mirror `FUN_1403066a0(o, A, B)`** (skipped when `A ~ B`) **[fact]**:
1. geometry edges (`FUN_140278740`): kind 1 records mirror `(r0, r1)`; other records mirror
   `(r1, r2)` and `(r3, r4)` and negate `r5` (sign flip); then `FUN_1402788a0` (record
   re-ordering, OBJPOS_geo_kernels.md 4.2: the record order is reversed for type-1 point lists
   too, confirmed by oracle run `OBJPOS/4-basic_2d`, `4-oblique_2d`). Observation (oracle, code
   not followed): for a type-2 edge the TPLOGY vertex coordinates written by KFWRIT follow the
   record end points after a mirror and after a translate (`gk-4-type2_2d`, `v-tplogy_2d`);
2. nodes (`FUN_140306860`): RZ only; then ELMCON swap nodes 1<->3 of every 4-node element
   (`FUN_1403050b0`), and every boundary loop node list re-oriented (`FUN_1401ffd00` ->
   `FUN_140201fb0`);
3. `FUN_140305180`: boundary-face arrays of each loop reversed in place (first `cnt-1`
   entries from the loop offset): `+0xae8`, `+0xaf0`, `+0xaf8` (8 B), `+0xb00`, `+0xb08`,
   `+0xb10` (8 B), `+0xb18`, `+0xb20`, then **`+0xb10` again** (so `+0xb10` ends unchanged and
   `+0xb28` is never reversed); loop offset: 0 for loop 0, else `prev_offset + 1 + cnt_k` with
   `cnt_k` the *current* loop's node count (not the previous one) **[fact]** (looks like a bug;
   reproduce as is);
4. remesh windows (`FUN_14026a020` -> `FUN_140268890`), window lists `+0x78`, `+0xa8`; CNTRAX
   centre `mv+0x630`; REFPOS point when type 1; inter-object windows (`FUN_14019f830`) **[fact]**.

### 6.7 Tensor rotation `FUN_140325884` (3D) / `FUN_14032e585` (2D)

`FUN_140325884(arr, &n, &kind, D, &a, &flag)`: frame as in 6.3 but with threshold 1e-12
(`DAT_140baea68`), angle `(a * 3.141592653589793) / 180.0` (multiply first, different from 6.3),
sin/cos by 6.3.1, `flag == 1` (always, `local_5c`) negates the sine; `Q = T^T R T` by two triple
loops; per element the 6 components are expanded to a symmetric 3x3 and `Q S Q^T` is formed;
`kind == 2` (strain) halves the shear terms before and doubles them after (`0.5`, `2.0`,
`DAT_140baea80/88`) **[fact]** from the decompile. The exact summation order of these loops was
not transcribed (open item 1).

## 7. 3D geometric modes

### 7.1 Modes 1 and 11: translate

`FUN_14021d0b0(obj, &(v2,v3,v4), 1, 1)`; `G = (v2, v3, v4)`;
`printf("Object %d has been moved by (%g, %g, %g).\n", obj#, v2, v3, v4)`; `moved = 1`. Mode 11
also updates STROKE (5.1) **[fact]**. The message is printed even when the translate skipped a
zero displacement.

### 7.2 Mode 3: rotate about an axis

`FUN_14021d2f0(obj, &(v2,v3,v4), &(v5,v6,v7), v8)`;
`printf("Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n",
obj#, v8, v2, v3, v4, v5, v6, v7)`; `rotated = 1` **[fact]**. Positive angle = counter-clockwise
looking against the axis direction (right-hand rule) **[inferred]**.

### 7.3 Mode 4: mirror at a plane

`FUN_1401fd850(obj, &(v2,v3,v4), &(v5,v6,v7))`;
`printf("Object %d has been flip w.r.t. to plane defined by point (%g, %g, %g) and normal (%g, %g, %g).\n",
obj#, v2..v7)`; `mirrored = 1` **[fact]**. Printed even when the normal is zero (no change).

### 7.4 Mode 6: rotate about the object's own centre

`FUN_1401ff360(obj, &C)` (C starts at 0): 3D `FUN_1401fe7d0`: if the object has a mesh and no
rigid geometry, a temporary surface geometry is built from the mesh (`FUN_1401fc730`, freed by
`FUN_1401ffed0`); geometry 0 (`FUN_140198620(o,0)`) points and faces (abs of the face node
numbers) go to the Fortran routine `FUN_1403e8510` whose first output (`CENTER` in its debug
prints, the second is `AXIS`) is copied to `C` **[fact]**; `C` is the volume centroid of the
closed surface **[inferred]**. The success flag is ignored by OBJPOS: on failure, or when the
object has neither mesh nor geometry, `C` stays `(0,0,0)` or holds the routine's output anyway
**[fact]**. Then the values are re-assigned: angle := v5, direction := (v2,v3,v4), origin := C,
and the mode continues exactly like mode 3 (same call, same message, `rotated = 1`, so coupled
objects rotate about the main object's centre) **[fact]**. `FUN_1403e8510` FP order not followed
(open item 2).

### 7.5 Mode 13: reference positioning

Values: `D = (v3, v4, v5)`, `ref = abs((int)v6)`, `sub = (int)v7`, `val = v8` (v2 unused) **[fact]**.

```
ref lookup (section 4 step 5)
ok = FUN_140181ae0(ref+0x8a0, rmin, rmax)       // node bbox of the reference; needs nodes
if (!ok) { printf("Object %d to Object %d reference positioning failed.\n", obj#, ref#); moved = 0; break }
if (obj+0x3990) FUN_1402faca0(geo0(obj), bmin, bmax)   // geometry-0 points; init 1e29 / -1e29
else            FUN_140181ae0(obj+0x8a0, bmin, bmax)   // nodes; init from node 0
C   = ((bmin+bmax)*0.5)                          // per component, multiply by 0.5
a   = rmin;  b = rmin + D                        // FUN_14023b110
Pc  = proj(C,    a, b)                           // FUN_14023bd90
Pm  = proj(rmax, a, b), t = its parameter
B   = (t < 0.0) ? Pm : rmin
if (obj# == 2 && !FUN_140203fa0(ref)) {          // ref has no node BCCDEF code 7 or 8 (o+0x990)
   my = (rmax.y + rmin.y) / 2.0;  mz = (rmax.z + rmin.z) / 2.0
   if (fabs(my) > 0.001 || fabs(mz) > 0.001) {   // DAT_14051f1e8
      G = (0.0, -my, -mz);  FUN_14021d0b0(ref, &G, 1, 1)
      printf("Object %d has been centered on YZ plane.\n", ref#)
      printf("Object %d has been moved by (%g, %g, %g).\n", ref#, G.x, G.y, G.z)
   }                                             // rmin/rmax/B are NOT recomputed
}
sub == 0: diag = sqrt(((rmax-rmin).x^2 + .y^2) + .z^2)   // FUN_14023b8f0
          B = B + D*(val*diag)                   // FUN_14023b2d0 then FUN_14023b110
sub == 1: ax = FUN_140259f90(ref+0x20a0, 1)      // WPAXIS item tag 1; CREATED if missing
          if (ax) { B = (val == 0.0) ? ax+8 (vector A) : ax+0x20 (vector B);  B = proj(B, a, b) }
other:    B = B + D*val
G = B - Pc                                       // DAT_140706bb0 = B.x - Pc.x, ...
FUN_14021d0b0(obj, &G, 1, 1)
printf("Object %d has been reference positioned w.r.t. Object %d.\n", obj#, ref#)
printf("Object %d has been moved by (%g, %g, %g).\n", obj#, G.x, G.y, G.z)
moved = 1
```

**[fact]** for all lines (all four mode-13 results of the oracle run match these formulas, e.g.
`13-sub0_3d`: reference 1 centred by (0, -18, -35), object 2 moved by (0, 0, 7.5)). `proj(p, a, b)` (`FUN_14023bd90`, listing checked): `d = b - a`;
`t = ((p.x-a.x)*d.x + (p.y-a.y)*d.y) + (p.z-a.z)*d.z`; `t = t / ((d.x*d.x + d.y*d.y) + d.z*d.z)`;
`q_i = a_i + d_i*t`; it also returns the squared distance (unused). With `D = 0` the division is
0/0 and NaN propagates into the move **[inferred]**. Bounding-box updates use strict `<` / `>`
(`p <= m && p != m`) **[fact]**. The created WPAXIS item (`FUN_140259950`, item `+4 = 3`,
`+0x60 = 1`) is a side effect on the reference object that KFWRIT writes as `WPAXIS <ref> 1 3 1`
with both vectors A and B = (0,0,0), so sub-mode 1 on a fresh item positions on `proj(0, a, b)`
(confirmed by oracle run, `OBJPOS/13-sub1_3d`: object 3 moved by (0,0,20)). The YZ-centering special case keys on
the literal object number 2 (the usual workpiece) and moves the **reference**, but the
positioning that follows still uses the pre-centering bounding box, so object 2 ends up relative
to the old reference position **[fact]**. `local_5318 = 100.0` (`DAT_140524cd8`) is assigned and
overwritten, never used **[fact]**. Arithmetic of this mode (centres, `my`/`mz`, `val*diag`, `G = B - Pc`) [listing-checked].

### 7.6 Mode 14: two-object relative positioning (`case 0xd`)

Values: `objB = (int)v2` (**no abs**), `sub = (int)v3`, `p = v4` (interference parameter),
`ref = abs((int)v5)` **[fact]**. Order **[fact]**:

1. Look up `objB` (section 4 step 5 checks; NULL -> return 3 silently).
2. `cA` = bbox centre of `obj`, `cB` = bbox centre of `objB` (same rule as `C` in 7.5: geometry 0
   when `+0x3990`, else nodes; `(min+max)*0.5`). `v = cB - cA` (`FUN_14023b1f0`).
3. Look up `ref` (checks of step 5).
4. `sub == 0`: `FUN_14019fa00(model, obj, ref, &v, &p, local_5cc8, &G, 1, &nc, list, &tchg, &1)`
   (interference of A toward the reference along `v`); `v = v * -1.0` (`DAT_140522fc8`); the same
   call for `objB` along `-v`. Return values ignored. See the interference spec for what these
   calls print and move.
5. `sub != 0`: `FUN_140181ae0(ref+0x8a0, rmin, rmax)` (result ignored); `h = rmax.z - rmin.z`;
   `FUN_14023b720(&v)` (normalise in place: `n = sqrt((x*x + y*y) + z*z)`, unchanged when
   `n == 0`, else divide); `v = v * (-h)` (sign flip of h); `G = v`;
   `FUN_14021d0b0(obj, &G, 1, 1)`; `v = v * -1.0`; `FUN_14021d0b0(objB, &v, 1, 1)`. A moves by
   `-h` along the unit A->B direction and B by `+h`: the two objects separate by twice the
   reference's Z height **[fact]** (meaning **[inferred]**) [listing-checked].
6. No message is printed by the command itself, `moved` stays 0 (no coupled motion, no STROKE)
   **[fact]**.

## 8. 2D geometric modes

### 8.1 Modes 1 and 11

`FUN_14021d0b0(obj, &(v2,v3,v4), 1, 1)` (the translate reads 2 components);
`G.x = v2, G.y = v3`; `printf("Object %d has been moved by (%g, %g).\n", obj#, v2, v3)`;
`moved = 1`; mode 11: STROKE `+0x22d0, +0x22d8` (5.1) **[fact]**.

### 8.2 Mode 3: rotate about a point

`FUN_14021d2f0(obj, &(v2,v3), NULL, v4)`;
`printf("Object %d has been rotated by %g w.r.t. (%g, %g).\n", obj#, v4, v2, v3)`; `rotated = 1`
**[fact]**.

### 8.3 Mode 4: mirror at the line through A and B

`FUN_1403066a0(obj, &(v2,v3), &(v4,v5))`;
`printf("Object %d has been flip w.r.t. to (%g, %g)-(%g, %g).\n", obj#, v2, v3, v4, v5)`;
`mirrored = 1` **[fact]**.

### 8.4 Mode 5: translate by the last displacement

`FUN_14021d0b0(obj, &G, 1, 1)`; `printf("Object %d has been moved by (%g, %g).\n", obj#, G.x,
G.y)`; `moved = 1`; coupled objects follow by `G` **[fact]**.

### 8.5 Mode 6: rotate about the object's own centre

`FUN_1401ff4f0(obj, &C)`: bbox of the 2D geometry edges (`FUN_1401ff600`, union of
`FUN_140279040` per-edge boxes, only when `FUN_140160cc0`), else of the nodes (`FUN_1401ff5b0` ->
`FUN_140181ae0`); `C_i = (min_i + max_i) / 2.0` (division) [listing-checked]; on failure C stays (0,0) **[fact]**.
Then angle := v2, centre := C, and the mode continues like mode 3 (same call and message,
`rotated = 1`) **[fact]**.

## 9. Outputs

stdout only; no files. Messages (exact format strings) **[fact]**:

| Where | Format |
|---|---|
| 3D move, mode 13 result, YZ centering, 3D coupled move | `Object %d has been moved by (%g, %g, %g).\n` |
| 2D move, mode 5 | `Object %d has been moved by (%g, %g).\n` |
| 2D coupled move | `Object %d(coupled) has been moved by (%g, %g).\n` |
| 3D rotate (3, 6, coupled) | `Object %d has been rotated by %g w.r.t. origin(%g, %g, %g) direction(%g, %g, %g).\n` |
| 2D rotate (3, 6, coupled) | `Object %d has been rotated by %g w.r.t. (%g, %g).\n` |
| 3D mirror | `Object %d has been flip w.r.t. to plane defined by point (%g, %g, %g) and normal (%g, %g, %g).\n` |
| 2D mirror | `Object %d has been flip w.r.t. to (%g, %g)-(%g, %g).\n` |
| mode 13 | `Object %d to Object %d reference positioning failed.\n`, `Object %d has been reference positioned w.r.t. Object %d.\n`, `Object %d has been centered on YZ plane.\n` |
| coupled drop | `Object %d has been moved by drop positioning.\n` |
| errors | `ERROR  : Object %d Undefined\n`, `ERROR  : No Mesh or Die Defined for Object %d\n` |
| interference / drop | see the two other files |

Return: 2 when a line cannot be read; 3 for a missing/undefined/empty main, reference or
second object; 0 otherwise (unknown modes included) **[fact]**.

## 10. Helper table

| Address | Role | Status |
|---|---|---|
| `FUN_140378e00` case 7 | OBJPOS | read fully |
| `FUN_140413900`, `FUN_140413a50`, `FUN_14047b6c0`, `FUN_14048754c` | line tokenizer, strtol, atof | group2-helpers.md |
| `FUN_1401d6ea0`, `FUN_14017aaf0` | object lookup | read |
| `FUN_1401611f0`, `FUN_140160f10`, `FUN_140160ba0`, `FUN_140160bf0`, `FUN_140160cc0`, `FUN_140160d90`, `FUN_140160d20`, `FUN_140203980`, `FUN_140203a20`, `FUN_1402039d0` | usability / geometry tests | read |
| `FUN_140484ab8` abs, `FUN_140484558` fabs, `FUN_140484644` sqrt | CRT | read |
| `FUN_14021d0b0`, `FUN_1402000a0`, `FUN_140200610`, `FUN_140181cb0`, `FUN_14021d190`, `FUN_14021dba0`, `FUN_140194f90`, `FUN_14021f210`, `FUN_1402595a0`, `FUN_140220080`, `FUN_14021f6b0`, `FUN_14021f7c0`, `FUN_140269eb0`, `FUN_140268040`, `FUN_1402576e0`, `FUN_14019ef20`, `FUN_14021d290`, `FUN_14018b190`, `FUN_140197850`, `FUN_14019f100`, `FUN_1402fb480`, `FUN_140277b00`, `FUN_1402816c0`, `FUN_140285550` | translate tree | read |
| `FUN_140267760` | box-window face refresh | summarised |
| `FUN_14021d2f0`, `FUN_1402002b0`, `FUN_1402006c0`, `FUN_140181db0`, `FUN_140184ac0`, `FUN_14021f6e0`, `FUN_14021f830`, `FUN_140269f20`, `FUN_140268160`, `FUN_140258400`, `FUN_140257750`, `FUN_14021dd70`, `FUN_14021f390`, `FUN_1402596a0`, `FUN_14025a480`, `FUN_140220190`, `FUN_14019f2c0`, `FUN_1402fb550`, `FUN_140277c50`, `FUN_140281660` | rotate tree | read |
| `FUN_140325691`, `FUN_1404cf5c0`, `FUN_1404ded0c`, `FUN_1404b3b30`, `FUN_1404b3960`, `FUN_1404b4898` | 3D point rotation, frame, sin/cos | read fully + listing |
| `FUN_14032ab90` | 2D point rotation | read + listing |
| `FUN_1402214c0`, `FUN_140221c20`, `FUN_140221cd0`, `FUN_140221a90`, `FUN_140220670`, `FUN_14027f460` | 4x4 geometry rotation | read |
| `FUN_140bb3490`, `FUN_140bb3000` | CRT cos / sin | not followed |
| `FUN_140325884`, `FUN_14032e585` | tensor rotation | summarised |
| `FUN_1401fd850`, `FUN_1401fddc0`, `FUN_1402fbbe0`, `FUN_1402fbf60`, `FUN_1401fde90`, `FUN_1401fe410`, `FUN_1401ff040`, `FUN_1401fea60`, `FUN_1401ff2b0`, `FUN_1401ff2f0`, `FUN_14026a0a0`, `FUN_140268930`, `FUN_140258500`, `FUN_140240340`, `FUN_140240220`, `FUN_140240980`, `FUN_14023fdf0`, `FUN_140239630`, `FUN_140239960` | 3D mirror | read (+ listing for the float helpers) |
| `FUN_1401fc290`, `FUN_140354ed0`, `FUN_14032d65e`, `FUN_140200ff0`, `FUN_140201fb0`, `FUN_1402578d0`, `FUN_14021f940`, `FUN_140281710`, `FUN_14019f660` | surface rebuild, face mapping, minor mirror parts | summarised |
| `FUN_1403066a0`, `FUN_1403067b0`, `FUN_140278740`, `FUN_140306860`, `FUN_140309640`, `FUN_1403050b0`, `FUN_1401ffd00`, `FUN_140305180`, `FUN_140305360`, `FUN_140305420`, `FUN_140306b70`, `FUN_140306bb0`, `FUN_140306ae0`, `FUN_140306b20`, `FUN_14026a020` | 2D mirror | read |
| `FUN_1402788a0`, `FUN_140268890`, `FUN_14019f830` | 2D mirror parts | not followed |
| `FUN_1403089b0`, `FUN_140308810`, `FUN_140308a90`, `FUN_140308940`, `FUN_140308a20`, `FUN_1403087a0`, `FUN_1403087e0`, `FUN_140308730`, `FUN_140308d60`, `FUN_140308fb0` | 2D vector helpers | read |
| `FUN_14023ace0`, `FUN_14023b050`, `FUN_14023b110`, `FUN_14023b1f0`, `FUN_14023b2d0`, `FUN_14023b3a0`, `FUN_14023b610`, `FUN_14023b720`, `FUN_14023b850`, `FUN_14023b8f0`, `FUN_14023bd90`, `FUN_140239800`, `FUN_140239a30`, `FUN_1402399b0`, `FUN_140239b40`, `FUN_140239ea0` | 3D vector helpers | read (`FUN_14023bd90` listing) |
| `FUN_1401952e0` | displacement along movement direction | read |
| `FUN_1401ff360`, `FUN_1401fe7d0`, `FUN_1401ff4f0`, `FUN_1401ff600`, `FUN_1401ff5b0`, `FUN_1401fc730`, `FUN_1401ffed0` | own-centre (mode 6) | read |
| `FUN_1403e8510` | Fortran surface mass properties (CENTER/AXIS) | not followed |
| `FUN_140279040` | 2D edge bbox | not followed |
| `FUN_140181ae0`, `FUN_1402faca0`, `FUN_140198620`, `FUN_140203fa0`, `FUN_140259f90`, `FUN_140258f10`, `FUN_140258f90`, `FUN_140259950`, `FUN_14025a000` | mode 13 / 14 helpers | read (last two summarised) |
| `FUN_14019fa00`, `FUN_140378c80`, `FUN_1401a0b00`, `FUN_14019ed10`, `FUN_1401f3ae0` | interference | OBJPOS_interference.md |
| `FUN_1401a7d60`, `FUN_140240fc0`, `FUN_1401fe240` | drop | OBJPOS_drop.md |

About 190 functions were decompiled for this file (`kern.c` 398 incl. CRT, `geo.c`); ~150 of them
were read.

## 11. Tests

`tests/commands/OBJPOS/cases.json` (37 cases, labels `OBJPOS/<mode>-<variant>_<dim>`; the
interference/drop cases are in `cases_ifd.json`). Seeds: 3D `UPSET_Brick.KEY` (objects 1-3,
all meshed with DIEGEO), 2D `SPIKE_SI.KEY`. Each `*.KEY` starts with a `*` line giving the
prediction. Oracle run (2026-10-21): all 37 cases match their predictions (messages and the
model changes, numerically recomputed); report `oracle/ov-objpos.md`. Extra checks in
`cases_v_ov-objpos.json` (`v-bits-rot_3d`, `v-mirror-bits_3d`: GENDB bit checks; `v-tensor_2d`,
`v-mirror-bits_2d`; `v-dir0_3d`; `v-tplogy_2d`; `v-ecflx*_2d`). Not tested on purpose: a coupled object number that does not exist (predicted crash,
section 2.2).

## 12. Open items

Items 1, 2, 3, 5 and 6 are covered by [OBJPOS_geo_kernels.md](OBJPOS_geo_kernels.md) (§1, §2, §3, §4, §5);
item 4 by [OBJPOS_interference_kernels.md §1.7](OBJPOS_interference_kernels.md#17-use-by-the-3d-mirror-fun_1401fc290obj-objposmd-65-step-4).


1. Tensor rotation `FUN_140325884` / `FUN_14032e585`: the exact accumulation order of the
   `T^T R T` loops and of the `Q S Q^T` expansion (STRESS, STNCMP, YLDS) is only in the
   decompile; transcribe from `out/p67/objpos-geo/kern.c` and confirm in the listing.
2. `FUN_1403e8510` (3D mode 6 centre): Fortran mass-property routine, 9 KB, not followed; its
   CENTER output defines the rotation origin bit-for-bit.
3. CRT `cos`/`sin` at `0x140bb3490` / `0x140bb3000` (used for die geometry 4x4 rotation): which
   CRT build; a port must reproduce them bit-exactly (or link the same CRT).
4. `FUN_1401fc290` boundary rebuild after the 3D mirror (`FUN_140354ed0`) and the face re-mapping
   (`FUN_140200ff0`, `FUN_140201fb0`): order of the regenerated faces not followed; it decides
   the face numbering written to KEY/DB.
5. 2D geometry re-ordering after mirror (`FUN_1402788a0`), 2D window mirror (`FUN_140268890`),
   `_OBJ_GEO2D` edge bbox (`FUN_140279040`): not followed.
6. Sub-object `obj+0x2be8` (moved/rotated/mirrored) is not identified in the reference specs.
7. ~~Content of a WPAXIS item created by mode 13 sub-mode 1~~ closed by the oracle run (7.5:
   both vectors zero).
8. That FLDGEO/FLWNET meshes use the `_OBJ_GEO3D` vtable is inferred (virtual call).
