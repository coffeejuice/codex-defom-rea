# MOVCTL (id 151, group 2, case 0x07)

Movement control of an object: movement type (force, speed, hammer, screw press, mechanical
press/stroke profile, spring, hydraulic press, ...), direction and the type's parameters, with an
optional 1-D function table. Evidence: the registry column-3 accessor `FUN_1402b3da0` maps the
table names "Force" → `+0x21d0`, "Speed" → `+0x2248`, "Hammer Blow efficiency" → `+0x2368`,
"Screw Press efficiency" → `+0x2400`, "Spring Stiffness" → `+0x2510`, "Hydrolic Strain rate"
→ `+0x2678`, "Stroke profile" → `+0x2498`. The data lives in the object's movement sub-structure
at `obj+0x2160` (initialised by `FUN_14018b500`, which also holds STROKE, FORCE, ANGMOV, ...).

Table objects below are the 0x78-byte table of `_common.md` §3 (`+4` ndim, `+8` constant,
`+0x10` variable code, `+0x1c` point count, `+0x40` ncomp). "T@X" = the table at object offset X;
"T@X.ndim" = `X+4` etc.

## KEY grammar (read)

The first line is always read by `FUN_140415d70` (group2-helpers.md): after the object number,
the rest of the line is `%i %Flf×7`, commas count as spaces, missing values are 0:

    MOVCTL <objNo:%i> <type:%i> <h0> <h1> <h2> <h3> <h4> <h5> <h6>

`h0..h6` are doubles; where they are used as integers the code truncates (`(int)h`).
Then, by dimension (`FUN_1400015e0(model)`: 2D; everything else takes the 3D branch) and by
`type`, more lines may follow:

* **L2(n)** = one more line read by a rest-of-line helper with n doubles (`FUN_140415080` n=6,
  `FUN_140414e40` n=4, `FUN_140414c20` n=3); extra values on that line are ignored.
* **TOK** = one more line split by the tokenizer `FUN_140413900(f, &cnt, 7)` (tokens separated
  by blanks, commas, tabs; `strtol`/`atof` conversion, missing tokens → 0).
* **TAB(T@X)** = table body read by `FUN_14041a800` (free-format numbers, may span lines; for
  `ndim=1, ncomp=c`: `n` points of `x v1..vc`, then the rest of the last line is skipped).

### 3D branch (no angle; direction vector dx dy dz)

| type | header usage | extra lines | stored |
|---|---|---|---|
| 0 force | h0 = sub, h1..h3 = dx dy dz, h4 = value or n | sub 1/2: TAB(T@21d0) | see "sub-mode" below, table T@21d0 |
| 1 speed | h0 = sub, h1..h3 = dir, h4 = value or n | sub 1/2/4: TAB(T@2248) | sub-mode with T@2248, plus 1001..1999 form |
| 2 hammer | h0..h2 = dir | L2(6): `e0 e1 e2 e3 e4 e5`; then maybe TAB(T@2368) | see type 2 |
| 3 screw press | h0..h2 = dir | L2(6): `e0..e3 sub n`; sub 4: TAB(T@2400) | `+0x23e0..+0x23f8` = e0..e3 |
| 4 mech. press | h0..h2 = dir | L2(4): `e0 e1 e2 n`; n ≥ 1: TAB(T@2498) | `+0x2478,+0x2480,+0x2488` = e0..e2 |
| 5 spring | h0..h2 = dir | TOK: `flag v d2 d3 d4 i5 i6`; flag≠0: TAB(T@2510) | see type 5 |
| 6 | h0 = kind, h1 = sub, h2 = i, h3 = n (no direction) | sub ≥ 0: [kind 2: L2(3) and L2(6)] then TAB(T@25a8) | see type 6 |
| 7 hydraulic | h0 = sub, h1..h3 = dir, h4 = value or n, h5, h6 | sub 1/2: TAB(T@2678) | sub-mode with T@2678; `+0x2768`=h5, `+0x2770`=h6 |
| 8 | h0 = sub, h1..h3 = dir, h4 = value or n, h5 | sub 1/2: TAB(T@26f0) | sub-mode with T@26f0; `+0x2768`=h5 |
| 9 | h0..h2 = dir, h3 | – | `+0x2768`=h3 |
| 10 | h0..h2 = dir, h3, h4 | – | `+0x2768`=h3, `+0x2778`=h4 |
| 11 | h0 h1 h2 (not a direction) | – | `+0x22e8,+0x22f0,+0x22f8`=h0..h2; direction untouched |
| other | – | – | nothing stored, returns 0 |

Direction for types 0–10 (3D): `+0x21b0 = dx`, `+0x21b8 = dy`, `+0x21c0 = dz` (stored as read,
not normalised). Type 6 and 11 leave the direction alone.

### 2D branch (angle in degrees instead of a vector)

Same types, but everything is on the header line except type 5 (TOK) and the tables;
`a` = angle in degrees:

| type | header | extra lines |
|---|---|---|
| 0 | h0 = sub, h1 = a, h2 = value or n | sub 1/2: TAB(T@21d0) |
| 1 | h0 = sub, h1 = a, h2 = value or n | sub 1/2/4: TAB(T@2248) |
| 2 | h0 = a, h1 → `+0x2350`, h2 → `+0x2358`, h3 → `+0x2348`, h4 → `+0x2360`, h5 = flagA, h6 = flagB | maybe TAB(T@2368) |
| 3 | h0 = a, h1..h4 → `+0x23e0..+0x23f8`, h5 = sub, h6 = n | sub 4: TAB(T@2400) |
| 4 | h0 = a, h1..h3 → `+0x2478..+0x2488`, h4 = n | n ≥ 1: TAB(T@2498) |
| 5 | h0 = a | TOK (as 3D), flag≠0: TAB(T@2510) |
| 6 | h0 = sub, h1 = i, h2 = n (no kind, no angle) | sub ≥ 0: TAB(T@25a8) |
| 7 | h0 = sub, h1 = a, h2 = value or n, h3 → `+0x2768`, h4 → `+0x2770` | sub 1/2: TAB(T@2678) |
| 8 | h0 = sub, h1 = a, h2 = value or n, h3 → `+0x2768` | sub 1/2: TAB(T@26f0) |
| 9 | h0 = a, h1 → `+0x2768` | – |
| 10 | h0 = a, h1 → `+0x2768`, h2 → `+0x2778` | – |
| 11 | h0 → `+0x22e8`, h1 → `+0x22f0` (no angle) | – |

2D direction for types 0–10 except 6: `+0x21b0 = cos(a·k)`, `+0x21b8 = sin(a·k)`,
`k = DAT_140522fd0 = 0.017453292519943295` (π/180; `FUN_140bb3490` = cos, `FUN_140bb3000` = sin).
`+0x21c0` is not written in 2D. After the switch, for **every** 2D MOVCTL (also types 6, 11 and
unknown types) each of `+0x21b0`, `+0x21b8` is set to 0.0 when `fabs(value) <= 1e-12`
(`DAT_14051f260`).

### Sub-mode (types 0, 1, 7, 8)

`sub = (int)h0`; `v` = the value/count slot (3D h4, 2D h2). T = the type's table. First
`FUN_14012fc70(T, 0)` (template 0: ndim 0, ncomp 1, constant 0).

| sub | T.ndim | T.var | T.cnt[0] | constant | KEY extra |
|---|---|---|---|---|---|
| 0 | 0 | – | – | `T+8 = v` | none |
| 1 | 1 | 2 | `(int)v` | – | n points `x y` |
| 2 | 1 | 1 | `(int)v` | – | n points `x y` |
| 4 (type 1 only) | 1 | 5 | `(int)v` | – | n points `x y` |
| 1001..1999 (type 1 only) | 0 | – | – | – | `+0x22c0 = sub-1000`, `+0x22c8 = v` |
| < 0 | 0 | – | – | – | `+0x2780 = 1`, `+0x2784 = abs(sub)` |
| anything else | 0 | – | – | 0 (from the reset) | value is lost |

`+0x2780/+0x2784` ("negative sub" flag and its magnitude, shared by types 0, 1, 7, 8) are set to
0/0 when sub ≥ 0 (types 0, 7, 8), and reset to 0/0 before the decision in type 1. Type 1 also
resets `+0x22c0 = 0`, `+0x22c8 = 0.0` first. `abs` is `FUN_140484ab8`.

### Type 2 (hammer)

Values `A B C N fA fB` (3D: the L2(6) line e0..e5; 2D: h1..h6). `iA = (int)fA`, `iB = (int)fB`:

* `iA == 0 && iB == 0`: `+0x2350 = A`, `+0x2358 = B`, `+0x2348 = C`, `+0x2360 = (int)N`,
  reset T@2368 (template 0), `T@2368.ndim = 0`. No table.
* `iB != 0`: same four stores, reset T@2368, `ndim = 1`, `cnt = iB`, `var = 5` only if
  `iA == 4` (else it stays 0); TAB(T@2368).
* `iA != 0 && iB == 0` (legacy form): `+0x2350 = A`, `+0x2348 = C`, `+0x2360 = (int)N`
  (`+0x2358` not stored), reset T@2368, `ndim = 1`, `var = 5` if `iA == 4`, `cnt = (int)B`;
  TAB(T@2368). So `B` is the point count in this form.

Table read failure returns **2** directly. The direction/angle is stored before the L2 line is
read.

### Type 3 (screw press)

`sub = (int)` (3D e4 / 2D h5): 0 → `T@2400.ndim = 0`; 4 → `ndim = 1`, `var = 5`,
`cnt = (int)` (3D e5 / 2D h6), TAB(T@2400) (failure → **2** directly); any other value → table
stays as reset (ndim 0). T@2400 is reset (template 0) before the L2 line.

### Type 4 (stroke profile)

`n = (int)` (3D e3 / 2D h4). Reset T@2498. `n < 1`: byte `+0x2490 = 0`, `T.ndim = 0`,
`T+8 = 0`. `n ≥ 1`: `+0x2490 = 1`, `ndim = 1`, `var = 10`, `cnt = n`, TAB(T@2498).

### Type 5 (spring)

TOK line, tokens t0..t6: `flag = strtol(t0)`, `+0x2588/+0x2590/+0x2598 = atof(t2/t3/t4)`,
`+0x25a0 = strtol(t5)`, `+0x25a4 = strtol(t6)`. Reset T@2510 (template 0).
`flag == 0`: `T.ndim = 0`, `T+8 = atof(t1)`. `flag != 0`: `ndim = 1`, `var = 0x13`,
`cnt = strtol(t1)`, TAB(T@2510). Tokenizer EOF → **2** directly.

### Type 6

3D: `+0x2620 = kind = (int)h0`; T@25a8 is re-initialised with `FUN_14012d2a0` (not `fc70`, so
`+0x54/+0x60` are reset too) as template 0x7a (kind 1: ndim 1, cnt 1, ncomp 4, var 1) or 0x7b
(other kind: ncomp 3). 2D: template 0x79 (ncomp 2), `+0x2620` untouched.
Then (3D h1..h3 / 2D h0..h2): `sub`, `+0x2624 = (int)i`, `T.cnt = (int)n`, `+0x2670 = 0`;
sub 1 → `T.var = 1`; sub 2 → `T.var = 0x1a`; sub < 0 → `+0x2670 = 1`, `+0x2674 = abs(sub)`;
other → `T.var = 1`. If `+0x2670 == 0`: (3D and kind == 2 only) read L2(3) into
`+0x2628,+0x2630,+0x2638` and L2(6) into `+0x2640..+0x2668`; then TAB(T@25a8)
(points of `x v1..v_ncomp`). The status of the two L2 reads is overwritten by the table read.

## Model storage

Object offsets (object pointer from `FUN_14019ccc0`); types: d = double, i = int32, b = byte.

* `+0x21c8` i movement type (default 1). `+0x21b0/+0x21b8/+0x21c0` d direction (3D default:
  component `FUN_140158e90(model)-1` = -1.0, others 0; 2D default `+0x21b0 = 1.0, +0x21b8 = 0`).
* Tables (0x78 bytes each): `+0x21d0` force, `+0x2248` speed, `+0x2368` hammer, `+0x2400` screw,
  `+0x2498` stroke profile, `+0x2510` spring (default template 0x6f), `+0x25a8` type 6
  (default 0x7a in 3D, 0x79 in 2D), `+0x2678` type 7, `+0x26f0` type 8.
* Scalars: `+0x22c0` i / `+0x22c8` d (type 1, 1001..1999), `+0x22e8..+0x22f8` d (type 11),
  `+0x2348/+0x2350/+0x2358` d (defaults 0, 0, 1.0), `+0x2360` i (type 2 N; `FUN_1401c6d00`
  treats it as an object number), `+0x23e0..+0x23f8` d (default `+0x23e8 = 1.0`),
  `+0x2478..+0x2488` d, `+0x2490` b, `+0x2588..+0x2598` d, `+0x25a0/+0x25a4` i,
  `+0x2620` i (default 1), `+0x2624` i, `+0x2628..+0x2668` d, `+0x2670/+0x2674` i,
  `+0x2768/+0x2770/+0x2778` d, `+0x2780/+0x2784` i.
* Nothing belonging to other types is cleared: switching type keeps the old type's data.
* Cross-keyword: SPDLMT read later with a table (n ≥ 1) changes `+0x21c8` from 1 to 8 (see
  SPDLMT spec). Its data stays in T@2248, but type 8 is written from T@26f0, so a speed-type
  object with a speed-limit table does not round-trip.

## Read errors and validation

* Header EOF / empty line (`FUN_140415d70` returns 1) → **2** directly.
* Type 5 tokenizer EOF → **2**; type 2 and 3 table failure → **2** directly.
* Other cases: the status of the last L2/table read becomes the case status (nonzero → **2**).
* No range checks: unknown types and sub values are accepted silently (return 0).

## KEY writer

`FUN_1402b1400` (registry handler2). It is called from the movement block writer
`FUN_1402a7c80`, which writes PRSNAM (id 257, string at `+0x2160`), then MOVCTL, SPDLMT, PRSTRE,
STROKE, FORCE, ANGMOV, [2D: ANGMVY], CNTRAX, [3D: ANGMO2, CNTRA2], OBJUPD, OBJSTP, stopping at
the first nonzero return. `FUN_14029e250` calls `FUN_1402a7c80` for type-5 objects and, for
normal objects, when `(model+0x1744 & 0x40000) == 0`; **its return value is ignored** there, so a
MOVCTL return of 3 drops the rest of the movement block but not the rest of the object.

Prefix of every header line: `"%-6s %7i %7i"` = name, object number (renumbered by `+0x3f24`),
type. `E` = `%20.10E`, `I` = `%7i`, all separated by one blank, line ends `\n`. Continuation
lines start with one blank. "TABW" = `FUN_1402f3130(f, T, 0)` (`_common.md` §3: nothing when
ndim 0; ndim 1: one line `" %20.10E %20.10E\n"` per point for ncomp 1; nothing if a count < 1).

Sub field written for types 0/1/7/8 (`k`): T.ndim 0 → `0` and the constant; T.ndim 1 → `1` if
var 2, `2` if var 1, (type 1:) `4` if var 5, then the count `I` and TABW; other var → **return 3**;
other ndim → **return 3**. If `+0x2780 != 0` the sub field is `-abs(+0x2784)` and no value/table.
Type 1 with `+0x22c0 ≥ 1` writes sub `+0x22c0 + 1000` and `E +0x22c8`.

3D formats (D = `E E E` = `+0x21b0 +0x21b8 +0x21c0`):

| type | header after prefix | following |
|---|---|---|
| 0/1 const | `I(0) D E(const)` | – |
| 0/1 table | `I(k) D I(cnt)` | TABW |
| 0/1 negative | `I(-m) D` | – |
| 1 1000-form | `I(+0x22c0+1000) D E(+0x22c8)` | – |
| 2, `+0x236c == 0` | `D` | `" E E E I\n"`: `+0x2350 +0x2358 +0x2348 +0x2360` |
| 2, `+0x236c == 1` | `D` | `" E E E I I I\n"`: same, `4`, `+0x2384`; TABW. (`+0x2378 != 5` → return 3) |
| 3, `+0x2404 == 0` | `D` | `" E E E E\n"`: `+0x23e0..+0x23f8` |
| 3, `+0x2404 == 1` | `D` | `" E E E E I I\n"`: same, `4`, `+0x241c`; TABW (`+0x2410 != 5` → 3) |
| 4 | `D` | `" E E E I\n"`: `+0x2478 +0x2480 +0x2488`, n (`+0x24b4` if `+0x2490` else 0); TABW if `+0x2490` |
| 5 | `D` | `+0x2514 == 0`: `" I E E E E I I\n"` = `0, +0x2518, +0x2588, +0x2590, +0x2598, +0x25a0, +0x25a4`; `== 1`: `" I I E E E I I\n"` = `1, +0x252c, ...`, TABW; other → 3 |
| 6 | `I I I I` = `+0x2620`, sub (`1` if var 1, `2` if var 0x1a, else `1`; or `-abs(+0x2674)`), `+0x2624`, cnt (`+0x25c4`, or 0 when negative) | if `+0x2670 == 0`: kind 2: `" E E E\n"` (`+0x2628..`) and `" E E E E E E\n"` (`+0x2640..+0x2668`); then TABW |
| 7 const | `I(0) D E(const) E E` (`+0x2768 +0x2770`) | – |
| 7 table | `I(k) D I(cnt) E E` | TABW |
| 7 negative | `I(-m) D E(0.0) E E` | – |
| 8 | as 7 with only `E(+0x2768)` | |
| 9 | `D E(+0x2768)` | – |
| 10 | `D E(+0x2768) E(+0x2778)` | – |
| 11 | `E E E` = `+0x22e8 +0x22f0 +0x22f8` | – |

(The "+0x236c"/"+0x2404"/"+0x2514" tests are T.ndim of T@2368/T@2400/T@2510; ndim other than
0/1 writes nothing for types 2/3.)

2D formats: `D` is replaced by one `E` = angle = `atan2(+0x21b8, +0x21b0) × 57.29577951308232`
(`FUN_14018c810`, `DAT_140522fd8`), and all continuation values move onto the header line:

| type | header after prefix |
|---|---|
| 0/1/7/8 | `I(k) E(angle)` then as 3D (`E(const)` / `I(cnt)` / nothing), plus type 7 `E E`, type 8 `E` |
| 2 | `E(angle) E E E I` (`+0x2350 +0x2358 +0x2348 +0x2360`), table form adds `I(4) I(+0x2384)` then TABW |
| 3 | `E(angle) E E E E`, table form adds `I(4) I(+0x241c)` then TABW |
| 4 | `E(angle) E E E I(n)`, TABW if `+0x2490` |
| 5 | `E(angle)`, then the same continuation line as 3D |
| 6 | `I(sub) I(+0x2624) I(cnt)` then TABW (no kind, no extra lines) |
| 9 | `E(angle) E(+0x2768)` |
| 10 | `E(angle) E(+0x2768) E(+0x2778)` |
| 11 | `E(+0x22e8) E(+0x22f0)` |

Unknown types write nothing (return 0).

Other writer: `FUN_1402a83c0(model, f, obj)` writes only type 2, in the legacy form
(3D continuation `" E I E I I\n"` = `+0x2350, +0x2384, +0x2348, +0x2360, 4`; 2D
`"... E(angle) E I E I I"`); it returns 3 for other types. It is called only from
`FUN_1401c6d00` (writes one object's movement block to a separate file, then the legacy MOVCTL of
the object numbered `+0x2360`). Not part of the KEY writer `FUN_14029e250`.

Round-trip notes: 2D angle goes through cos/sin/atan2 (last-digit drift possible; tiny components
are snapped to 0); 2D never writes `+0x21c0`; sub values outside the table above, var codes the
writer does not know, and type 2/3 tables with flag ≠ 4 (var 0) are read but make the writer
return 3; the `-m` form loses the value slot (written as nothing / 0.0).

## Unit conversion

Degrees ↔ radians for the 2D angle only. No unit-system scaling.

## Helpers

* `FUN_14038b8a0` case 7 — reader. `FUN_140415d70` header, `FUN_140415080`/`FUN_140414e40`/
  `FUN_140414c20` continuation lines, `FUN_140413900`/`FUN_140413a50`/`FUN_14047b6c0`/
  `FUN_14048754c` tokenizer and conversions, `FUN_14041a800` table body, `FUN_14012fc70`/
  `FUN_14012d2a0` table init, `FUN_140484ab8` abs(int), `FUN_140484558` fabs,
  `FUN_140bb3490` cos, `FUN_140bb3000` sin.
* `FUN_1402b1400` — writer (handler2); `FUN_14018c810` 2D angle (atan2 via `FUN_140485d6c`);
  `FUN_1402f3130`/`FUN_1402f2e80` table writer.
* `FUN_1402a7c80` — movement block writer. `FUN_1402a83c0` — legacy type-2 writer
  (`FUN_1401c6d00` only).
* `FUN_1402b3da0` — handler3: returns the address of the named table (strings above), 3 if the
  object is missing or the name unknown. Only for table lookup by name; the KEY writer and
  `FUN_1402a7c80` never call it, so it does not affect KEY output.
* `FUN_14018b500` — movement sub-structure defaults (`obj+0x2160`).

## Corpus check

All 1422 MOVCTL records in the corpus parse with this grammar (script check: after consuming the
header, continuation line and table, the next line starts with a keyword or `*`). Types seen:
2D 0/1/2/3/4/8, 3D 0/1/4/5; subs 0, 1, 2, 1002. Examples:

    MOVCTL       1       1       2 -9.0000000E+001       5        (2D, PLATE_BEND-EP_EN.KEY:135, 5 table lines follow)
    MOVCTL       2       4  0.0000000E+000  0.0000000E+000 -1.0000000E+000
      2.7000000E+002  1.4000000E+000  1.5000000E+003            (3D IDS_GC_Dwell.KEY:16787, n missing → 0)
    MOVCTL       6       5   -1.0000000000E+000 ... / "       0    1.0000000000E+002 ..."  (3D spring, TOK line)

## Open questions

* Physical meaning of types 6, 8, 9, 10, 11 and of the variable codes 1, 2, 5, 10, 0x13, 0x1a
  (a variable-code name table would settle it).
* `FUN_140158e90` (axis used for the 3D default direction) not decompiled.
