# CPYOBJ (id 13, group 6, `case 0xd` of `FUN_140378e00`)

Source: `DEF_PRE_64.exe` v11.0.2, static analysis only (`out/bm_cmd.c` lines 1529-1575, listing
`0x14037cf57..0x14037d14d` for the stack arguments, closure `out/p67/cpyobj.c`). Shared
mechanics: [_common.md](_common.md). **[fact]** = read in code, **[inferred]** = deduced.

## 1. Summary

Copies data from one existing object into another existing object of the same model: the whole
object (mode 0) or one sub-block (1 remesh settings, 2 movement, 3 process data). Same code in
2D and 3D, a few fields are dimension-dependent **[fact]**.

```
CPYOBJ <dstObj> <mode> <srcObj>
```

**The first number is the destination, the third the source** **[fact]**: every copier is
`copy(param_1 = object of int 1, param_2 = object of int 3)` and writes `param_1` from `param_2`
(e.g. `FUN_14018bd30`: `*(p1+0x50+8k) = *(p2+0x50+8k)`).

## 2. Argument grammar

`FUN_140413fb0(f, &dst, &mode, &src, NULL)`: rest of the keyword line, `fgets(1024)`, `,`→space,
`sscanf("%i %i %i")`, missing values 0, returns 1 only for EOF/blank (→ **return 2**). One line.

Then `d = FUN_1401d6ea0(model, dst)`, `s = FUN_1401d6ea0(model, src)` (list find, WRTOBJ.md §3);
either NULL → **return 2** (`ERROR: Keyword CPYOBJ Reading Error. `). Mode other than 0..3 →
nothing, return 0. No message on any path **[fact]**.

`dst == src` is not rejected. Mode 0 then first frees the object's arrays and copies from the
freed object (use after free); mode 1 clears the MGUSER list / remesh arrays before copying them
from itself (they end empty / zero-filled) **[fact for the call order; result inferred]**. Not tested.

## 3. Mode 0: whole object `FUN_1401613a0(d, s, model+0x15f8, &f4, &f5, &f6, &f7, &f8, &f9)`

Flag bytes (listing `0x14037d000..0x14037d098`: R9 and stack slots +0x20..+0x40) **[fact]**:

| param | value | meaning (where used) |
|---|---:|---|
| f4 `[R9]` | 1 | element copy: include USRELM (id 0x3c) |
| f5 `[rsp+0x20]` | 1 | node copy: include USRNOD (id 0x11) |
| f6 `[rsp+0x28]` | 1 | element copy: include VOLFC, VOLFS, DVOLF, TICF (ids 0x31..0x34) |
| f7 `[rsp+0x30]` | 1 | copy the object name `+0x828` |
| f8 `[rsp+0x38]` | **0** | copy the object number `+8` → **not copied**, the destination keeps its number |
| f9 `[rsp+0x40]` | 1 | element copy: include STNCMP (id 0x28) |

`model+0x15f8` = W[], the per-variable width table (objarrays.md §0). So CPYOBJ mode 0 copies
everything except the number.

Order **[fact]**:

1. `FUN_140158f70(d)` → `FUN_14015fbf0(d, keepGeom=0)`: **reset the destination**: free node
   container (`FUN_14017f130`), element container (`FUN_140182a50`), face BC arrays
   (`FUN_1401856e0`); clear the 100 geometry slots `+0x3c00[k]` (`FUN_1402f9340`, objects kept),
   boundary records `+0xbe0+0x30k` (`FUN_14017e800`/`FUN_14017e770`), FLDGEO/FLWNET
   (`+0x3328`, `+0x3458`); reset process block `+0x1ea0` (`FUN_140186510`), movement `+0x2160`
   (`FUN_14018bc30`), remesh `+0x2d10` (`FUN_140189ad0`), BC/window lists `+0x2e68`
   (`FUN_140195a40`), `+0x3920` (`FUN_140197b40`), 2D geometry `+0x3998` (`FUN_140301d10`,
   `FUN_140301ad0`), `FUN_140199b10`, `FUN_140199b60`, vtable reset of `+0x3a60`,
   `FUN_140162070`, delete `+0x3bf8`; zero `+0x10`, `+0x14`, `+0x818..+0x820`; strings `+0x18`,
   `+0x418` := `DAT_14051f7d6` / `DAT_14051f7d7` (empty **[inferred]**); byte `+0x3990 = 0`;
   `+0x87c..+0x88c = 0`. Fields reset here and not copied below stay reset (e.g. `+0x10`,
   `+0x14`, `+0x18`, `+0x418`, `+0x818..+0x820`, `+0x3920`, `+0x3a60`, `+0x3bf8`).
2. Scalars: `+0xc` type; `+0x828` name (`strncpy` 0x40, f7); `+0x87c` has-mesh, `+0x880` nodes,
   `+0x884` elements, `+0x888` boundary record count, `+0x88c` face/loop-node count, `+0x3984`
   first node, `+0x3988` first element, `+0x3588`, `+0x358c`, `+0x398c` first boundary face,
   `+0x3980`.
3. `FUN_140165500(d, s+0x890)`: element kind `+0x890`; 2 → `+0x894/0x898/0x89c = 4, 4, 3`;
   4 → `8, 6, 4` (nodes / faces / face nodes per element **[inferred]**); other values leave
   `+0x894..` unchanged (not reset in step 1) **[fact]**.
4. Geometry `FUN_140161840`: 3D → `FUN_1401618b0` (byte `+0x3990 = FUN_140160d90(s)`,
   `FUN_140198600(d, FUN_1401985c0(s))` sets the slot count, then each non-NULL slot
   `d+0x3c00[k]` gets vtable `+0x20` copy of `FUN_140198650(s, k)`); 2D → `FUN_1401619c0`
   (copy `s+0x3998` into `d+0x3998` if `d` has one, `FUN_1403019d0`). Not followed deeper.
5. FLDGEO `+0x3328` and FLWNET `+0x3458`: `FUN_140199c20` → `FUN_1402f9b60` (points ×3 doubles,
   faces ×4 ints, `+0x20` ints, `+0x68` ints, list `+0x28` via `FUN_140257c80`).
6. Boundary loops: `+0xbd8 = min(s+0xbd8, 100)` (`FUN_140163300`); for each `k`
   `FUN_14017ec20(d+0xbe0+0x30k, s+…)`: node count `+0`, face count `+0x10`, node list `+8`
   (n ints), face nodes `+0x18` (4·faces ints), face elements `+0x20` (faces ints).
7. Node fields `FUN_140180390(d+0x8a0, s+0x8a0, nodes, W, f5)`: realloc to `nodes`
   (`FUN_140180150`); generic copy of every registered double field `FUN_140329ef0` (pairs by
   field id; copies via the field's vtable `+0x18`, skipped ids from the exclusion set; returns
   without copying anything when the two containers have different field counts) — RZ, DRZ, URZ,
   FRZ, PRZ, NDTMP, NDHEAT, NDFLUX, … (objarrays.md §2.1); then memcpy of `+8` (coordinates
   `obj+0x8a8`, nodes·W[0] doubles **[inferred: also covered by the generic copy]**) and the int
   arrays `+0xf0..+0x118` = BCCDEF, BCCDFN, BCCTMP, BCCTFN, BCCCRB, BCCRHT (`obj+0x990..0x9b8`,
   nodes·W[0x12..0x17] ints). `nodes == 0` → container freed instead.
8. Element fields `FUN_140183900(d+0x9e0, s+0x9e0, elems, d+0x894, W, f4, f6, f9)`: if
   `elems == 0` free first; realloc (`FUN_140183770`); generic copy (STRESS, STRAIN, DENSTY,
   DAMAGE, …, objarrays.md §2.3); memcpy ELMCON `obj+0x9e8` (elems·nodes-per-element ints) and
   MTLGRP `obj+0x9f8` (elems·W[0x3d] ints).
9. Face BCs `FUN_140185a10(d+0xae8, s+0xae8, NBF(d))` (`FUN_140165170`): 12 arrays
   `obj+0xae8..0xb40` = ECCDEF, ECDEFN (int), ECPRES (double), ECCTMP, ECTMFN, ECHFLX(d),
   ECCATM, ECATFN, ECAFLX(d), ECCRHT, ECRHFN, ECRFLX(d), NBF entries each, only when the source
   array exists.
10. Process block = mode 3 (`FUN_140186590`), movement = mode 2 (`FUN_14018bd30`), remesh = mode 1
    (`FUN_140189b70` with both flags 1).
11. BC/window lists `FUN_140195c80(d+0x2e68, s+0x2e68)`: clear (`FUN_140195a40`), then deep copy
    (`FUN_140269340`: `+0x18` int, `+0x28` 8 bytes, each entry `FUN_14025c9b0`) of WINPRS
    `+0x2eb0`, WINTMP `+0x2ee0`, WINATM `+0x2f10`, WINRSE `+0x2fd0`, LOCTMP `+0x2f40`, LOCATM
    `+0x2f70`, LOCDEF `+0x2fa0`; BCCFNC function list (`FUN_140136560`, each table copied with
    `FUN_14012eff0`, link `+0x60` cleared); 3D only: list `+0x2e80` (`FUN_140257c80`).
12. `FUN_1401619c0(d, s)` again (2D geometry `+0x3998`, any dimension, only if `d` has one).
13. `FUN_140161ac0`: `+0x3aa0`, `+0x3aa4` (int), byte `+0x3aac`, 20 doubles `+0x3ab0..+0x3b48`
    (meaning not identified).

## 4. Mode 1: remesh settings `FUN_140189b70(d+0x2d10, s+0x2d10, &1, &1)` **[fact]**

`R = obj+0x2d10`; its `+0` is the owning model pointer (dimension test). Keyword names from the
group 2 specs (branch `claude/project-thread-j4c7t9`).

1. `FUN_140189ad0(Rd)`: clear MGUSER list `R+0x40`, free `R+0xe0`, `R+0xe8`, `R+0xd8 = 0`.
2. Copy 8-byte fields `R+0x10` MGSIZR (`obj+0x2d20`), `+0x18` MGWTMP, `+0x20` MGWSTN, `+0x28`
   MGWSTR, `+0x30` MGWUSR (1st real), `+0x38` MGWCUV.
3. flag 1: MGUSER window list `R+0x40` (`obj+0x2d50`) deep copy `FUN_140269340`.
4. flag 2, by the dimension of `*Rd`: 2D: `R+0x70` int (MGNELM n, `obj+0x2d80`); 3D: ints
   `R+0x78, +0x7c, +0x80, +0x84` (MGNELM a, b, c, flag `obj+0x2d88..0x2d94`), `R+0x98` MGSIZM
   (`obj+0x2da8`), `R+0x88` MGWUSR 2nd real (`obj+0x2d98`), `R+0x90` int MGWUSR (`obj+0x2da0`).
   (The dimension is unset → neither.)
5. Always: `R+0xa0`, `+0xa8` MGERR (`obj+0x2db0/0x2db8`), ints `+0xb0`, `+0xb4` MGGRID
   (`0x2dc0/0x2dc4`), int `+0x74` MGTELM (`0x2d84`), `+0xb8` RMDPTH (`0x2dc8`), `+0xc0` RMSTRK
   (`0x2dd0`), `+0xc8` RMSTEP (`0x2dd8`), `+0xd0` RMTIME (`0x2de0`).
6. `FUN_14018af80(Rd, Rs+0xd8)`: count `R+0xd8` (`obj+0x2de8`), calloc `R+0xe0` (doubles) and
   `R+0xe8` (ints), then memcpy both (count entries); int `R+0xf0` (`obj+0x2e00`). Meaning of these
   three not identified.

Not copied: `R+8`, `R+0x60..0x6f`, byte `obj+0x2e05` (mesh-settings write flag, MGNELM.md) and
anything else in the block.

## 5. Mode 2: movement `FUN_14018bd30(d+0x2160, s+0x2160)` **[fact]**

`mv = obj+0x2160`; names from MOVCTL.md and object.md §4. Copies:

* 3 doubles each: `mv+0x50` direction, `+0x170` STROKE, `+0x188` type-11 reals, `+0x1a0` FORCE,
  `+0x630` CNTRAX centre, `+0x648` CNTRAX axis (3D); then `FUN_14018beb0`:
* `strcpy(mv+0, s mv+0)` PRSNAM; `+0x68` movement type;
* type 2: `+0x1e8, +0x1f0, +0x1f8` doubles, `+0x200` int, table `+0x208`; type 3: `+0x280..+0x298`,
  table `+0x2a0`; type 4: `+0x318, +0x320, +0x328`, byte `+0x330`, table `+0x338`; type 5: table
  `+0x3b0`, `+0x428, +0x430, +0x438`, ints `+0x440, +0x444`; type 6: table `+0x448`, ints
  `+0x4c4, +0x4c0`, 9 doubles `+0x4c8..+0x508`, ints `+0x510, +0x514`;
* follow flag/obj `+0x620, +0x624`, ANGMOV mode `+0x628`, CNTRAX last `+0x750`;
* tables `+0x70` (type 0), `+0xe8` (type 1), `+0x590` (type 8), `+0x518` (type 7); doubles
  `+0x608, +0x610, +0x618`; type-1 `+0x160` int, `+0x168` double;
* ANGMOV tables `+0x660`, `+0x6d8`; SPDLMT byte `+0x8b8`, table `+0x8c0`, `+0x938`, `+0x940`;
  PRSTRE `+0x948, +0x950, +0x958`;
* 2D ctx copies `+0x1b8..+0x1e0` (6 doubles);
* ANGMVY `+0x960` int, tables `+0x968`, `+0x9e0`, doubles `+0xa58..+0xa70`;
* ANGMO2 mode `+0x758`, CNTRA2 `+0x880`, tables `+0x790`, `+0x808`, CNTRA2 vectors `+0x760..`,
  `+0x778..` (3 each); OBJSTP `+0x888..`, `+0x8a0..` (3 each); byte `+0xa78`.

Tables are deep-copied with `FUN_14012eff0` (table copy, `_common.md` §3 layout). Nothing is
cleared first; fields not listed keep the destination's values.

## 6. Mode 3: process data `FUN_140186590(d+0x1ea0, s+0x1ea0)` **[fact]**

`P = obj+0x1ea0`; `P+0x2b8` (`obj+0x2158`) is the owning model (dimension test). Copies
(keyword names from object.md §2):

| P offset | obj offset | keyword / field |
|---|---|---|
| `+0x00, +0x08, +0x10, +0x18` | `0x1ea0..0x1eb8` | LMTSTR, AVGSTR, PENVOL real, REFTMP |
| `+0x28` int, `+0x30..+0x40`, `+0x48` int | `0x1ec8`, `0x1ed0..0x1ee0`, `0x1ee8` | REFPOS type, point, node |
| `+0x4c`, `+0x50` int, `+0x58`, `+0x60`, `+0x68` int | `0x1eec..0x1f08` | OTPRNG |
| `+0x70` | `0x1f10` | TMPLMT |
| `+0x78` int | `0x1f18` | ELPSOL |
| 2D only: `+0x80` int, `+0x88`, `+0x90` int | `0x1f20`, `0x1f28`, `0x1f30` | PENVOL int, ZSTR real, ZSTR int |
| `+0x2b0` int | `0x2150` | USRSUB |
| `+0x7c` int | `0x1f1c` | OBJUPD (3D word) |
| `+0x94, +0x98, +0x9c, +0xa0` int | `0x1f34..0x1f40` | FRCSTP, FRCNEL, FRCMTH, CRPTIM |
| `+0xa4, +0xa8` int, `+0xb0, +0xb8` | `0x1f44..0x1f58` | HDNEST |
| table `+0xc0` (`FUN_14012eff0`) | `0x1f60` | CSFREQ |
| `+0x138` int | `0x1fd8` | VOLCRG kind |
| table `+0x140` | `0x1fe0` | VOLCRG value / function |
| 3D only: `+0x1b8`, `+0x1c0..+0x1d0`, `+0x1d8..+0x1e8` | `0x2058`, `0x2060..`, `0x2078..` | ROTSYM |
| list `+0x200` (`FUN_1402598c0`) | `0x20a0` | WPAXIS |
| list `+0x218` (`FUN_14025a730`, entry vtable `+8`) | `0x20b8` | OPSTOP |
| `+0x1f0`, `+0x1f8` int | `0x2090`, `0x2098` | TRGVOL real, int |
| `+0x2b4` int | `0x2154` | OBJTYP 2nd value |
| table `+0x238` | `0x20d8` (incl. `0x2118`, `0x2120`) | RSEDEF |

Not copied: `P+0x20` (`obj+0x1ec0`), `P+0x84`, `P+0x8c` (3D), anything else.

## 7. Floating point

None: pure copies (memcpy / 8-byte moves) **[fact]**.

## 8. Outputs

No stdout, no files. Return 2 for a blank first line or a missing object, else 0.

## 9. Helpers (followed: 47)

| Address | Role | Status |
|---|---|---|
| `FUN_140413fb0`, `FUN_1401d6ea0` | reader, object find | read fully |
| `FUN_1401613a0` | mode 0 object copy | read fully |
| `FUN_140158f70` → `FUN_14015fbf0` | object reset | read; sub-resets named, not followed |
| `FUN_140165500`, `FUN_140163300`, `FUN_14017ec20`, `FUN_140161840`, `FUN_1401618b0`, `FUN_1401619c0`, `FUN_140161ac0` | scalar / boundary / geometry copies | read; geometry vtable copies not followed |
| `FUN_140199c20` → `FUN_1402f9b60` | FLDGEO/FLWNET copy | read |
| `FUN_140180390`, `FUN_140180150`, `FUN_14017f130` | node container copy / realloc / free | read |
| `FUN_140183900`, `FUN_140183770`, `FUN_140182a50` | element container copy / realloc / free | read |
| `FUN_140329ef0` | generic field copy with exclusion set | read; per-field vtable `+0x18` not followed |
| `FUN_14019ac40`, `FUN_14019ad10`, `FUN_14019acf0` | exclusion set ctor / insert / dtor | summarised |
| `FUN_140165170`, `FUN_140185a10` | NBF, face BC copy | read |
| `FUN_140189b70`, `FUN_140189ad0`, `FUN_14018af80`, `FUN_140269340` | mode 1 | read fully |
| `FUN_14018bd30`, `FUN_14018beb0` | mode 2 | read fully |
| `FUN_140186590`, `FUN_1402598c0`, `FUN_14025a730`, `FUN_14019a500` | mode 3 | read fully (list entry copies summarised) |
| `FUN_140195c80`, `FUN_140195a40`, `FUN_140136560`, `FUN_140257c80` | window/BC list copies | read |
| `FUN_14012eff0` | table copy | not re-read (table code, `_common.md` §3) |

## 10. Open items

* Exact per-field behaviour of the generic copy `FUN_140329ef0` (field vtable `+0x18`): assumed
  "resize to source and memcpy" **[inferred]**; and the case where the destination container has a
  different registered-field count (copy silently skipped).
* 3D geometry slot copy (vtable `+0x20` of `+0x3c00[k]`) and 2D `FUN_1403019d0` not followed.
* Meaning of `obj+0x3aa0..0x3b48`, `obj+0x2de8..0x2e00`, `obj+0x358c`, `obj+0x3980`.

## 11. Oracle check (2026-10-21)

All 10 cases agree with this spec (`docs/re/commands/oracle/ov-misc.md`). Mode 0: the
destination's object section of OUT.KEY equals the source's section line for line with only the
object number changed (3D 1→3, 2→1; 2D 2→3), other objects and the inter-object data untouched.
Modes 1-3 change exactly the predicted keywords. `mode3_2d` did not exercise anything (objects 1
and 2 of SPIKE_SI have identical process data); `v_mode3_2d` (`cases_v_ov-misc.json`) sets
AVGSTR, ZSTR (2D int + real), PENVOL (real + 2D int) and REFTMP on object 1 first: all four are
copied to object 2.
