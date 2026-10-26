# In-process IGES reader for 2D objects (GFREAD `.IGS`)

DEF_PRE_64.exe v11.0.2. Reached only from GFREAD on a 2D object with extension `IGS`/`igs`
([GFREAD.md](GFREAD.md) §3, [DXF_IGES_READERS.md](DXF_IGES_READERS.md) §2):
`FUN_140244500` → `FUN_140409af0(iges, file, "", obj, &x, 2, 0, 1, 300)`.
Decompiles: `deform-rea-work/out/p89/c/gfigs.c`, `out/p89f/igs/fortran.c` (all Fortran
routines), `cside.c`, `fb.c`, `atan.c`, `misc.c`, listings `L_*.txt` (Ghidra copy `ghidra_p89f2`).
This spec supersedes §3/§3.1 and the IGES items of §7 in DXF_IGES_READERS.md.

## 1. Summary

* A C++ driver (`FUN_140409af0`) calls two passes of an embedded Fortran IGES library
  (`IgesInterface.f`/`IgesCore.f`, unit 7) through `FUN_14040c220`.
* **Pass 1** reads the D and P sections, keeps entities **100** (arc), **110** (line), **112**
  (parametric spline), **124** (transformation matrix) and **126** (rational B-spline); every
  other entity type (102, 116, 128, 212, 402 …) is ignored. The S, G and T sections are never
  parsed: **units and the G-section scale are ignored**, delimiters are always `,` and `;`.
  Splines become polylines (112: 8 steps per segment; 126: 30 samples), 124 matrices are applied,
  and the tolerance = 0.01f × shortest non-zero point distance.
* **Pass 2** matches end points within the tolerance (matched pairs are moved to their x/y
  midpoint), traces one chain, fixes the travel direction of every entity and writes a segment
  table `[type, x1, y1, x2, y2, angle]` (6 doubles per segment) that becomes curve 0 of the
  object's geometry. KFWRIT then writes it as `DIEGEO <obj> 2 <nseg>`.
* If a second, unconnected chain exists, tracing is abandoned and every entity is written in
  parameter-pointer order, forward (code 200).
* `DEF_IGS.OUT` is created **empty** before pass 1 (`fopen "at"`) and deleted only when pass 1
  succeeds; nothing writes into it on the GFREAD path. Its text reader (`FUN_140455e60`) is only
  reached on an allocation failure (§8).
* Every reader result except -1 counts as GFREAD success ("Loading geometry - Done"), also the
  failures -4 and -8, which leave the geometry unchanged.

## 2. Driver `FUN_140409af0(iges, name, prefix, obj, p5, p6=2, p7=0, p8=1, mode=300)`

1. `obj == 0` → -1. mode ≠ 300/400 → -1 (mode 400 = re-conversion of existing geometry, not
   reached from GFREAD). `iges+0x20 = 0`.
2. `name == NULL` or `strlen < 1` → -2; `FUN_1401b8560(name)` (fopen "r") fails → -3;
   `prefix == NULL` → -12.
3. `FUN_140197bc0(obj)`: clears the undo list `obj+0x3a60` (vtable +8) and pushes a **copy** of
   every curve of geometry 0 into it. The geometry itself is not cleared here.
4. `out = prefix + "DEF_IGS.OUT"`; `FUN_1401b8420(out)` = `fopen(out, "at")` + `fclose`:
   creates an empty `DEF_IGS.OUT` if missing (result ignored).
5. Free and re-allocate the work tables: `DAT_14070c050` = `double[1500000]`
   (`DAT_140628568`), `DAT_14070c058` = `int[1500000]` (`DAT_140628554`), both zeroed.
6. Pass 1: `r = FUN_14040c220(name, &DAT_14070bc20, 1, 0)`. `r ≠ 0` → free both tables, return
   r (DEF_IGS.OUT stays).
7. `FUN_1401b85d0(out)` (delete DEF_IGS.OUT). If `obj+0x3a50 ≠ 0` (user tolerance flag, 0 from
   the object ctor `0x14015f574`; set only by menu code) `DAT_14070c040 = obj+0x3a58`.
   `obj+0x3a48 = DAT_14070c040` (`FUN_1404098d0`), `iges+0x20 = DAT_14070c040`. (`p7 = 0`: the
   `p5` exchange is skipped.)
8. Table size: `c034 < 1 && c030 < 1` (no 126, no 112) → `n = c024·6`, else `n = c020·6`
   (`FUN_140276090(2) = 6`). `n < 1` → `DAT_14070c048 = 0`, else
   `DAT_14070c048 = FUN_140275fa0(n)` (malloc n doubles, zeroed).
9. `DAT_14070c048 == 0` → fallback §8. Otherwise:
   * `cont = FUN_1401982c0(obj, &0)` (curve 0 of geometry 0); missing → `FUN_140198270(obj)`
     (create geometry list if needed, append a curve) and fetch again.
   * Pass 2: `r = FUN_14040c220(name, ctx, 2, 500)`.
   * `r == 0` or `r == 200`: `cont+0x14 = c024` (segment count), `cont+0x10 = 2`,
     `FUN_140276030(cont)` (free old points), `cont+0x18 = DAT_14070c048`.
     For 200 also `FUN_14040ce40(iges)` (`iges+0x18 = 1`).
     `FUN_140198230(obj, 1)` (geometry 0 keeps exactly one curve: extra curves are deleted,
     dirty flag set). `nb = 1` for r = 0; for 200 `nb = max(1, k)` with `k = c024` saved after
     pass 1 (number of entity records). `obj+0xbc8 = nb` (`FUN_14019a650`), `iges+0x1c = nb`.
     Free `c050`, `c058`; `c048 = 0` (ownership moved); return 0.
   * other r: free `c050`, `c058`, `c048`; return r.
10. Caller `FUN_140244500`: on 0 sets `obj+0xbc8 = iges+0x1c` again (same value).
    `obj+0xbc8`, `obj+0x3a48` and `iges+0x18` do not appear in the KEY output.

## 3. Context `DAT_14070bc20` and the counters

| Address (ctx+) | Set by | Meaning |
|---|---|---|
| `bc20` (+0) | `FUN_14040c220` pass 1 (`strcpy`) | file name |
| `c020` (+0x400) | pass 1: `FUN_14046519b` | nA = number of output points (polyline points of all entities) |
| `c024` (+0x404) | pass 1: D reader (raw records), then `FUN_14046519b` (entity records without 124); pass 2: `FUN_140459149` | entity count; after pass 2: **segment count** |
| `c028` (+0x408) | D reader | number of 100 |
| `c02c` (+0x40c) | D reader | number of 110 |
| `c030` (+0x410) | D reader | number of 112 |
| `c034` (+0x414) | D reader | number of 126 |
| `c038` (+0x418) | D reader | number of 124 |
| `c040` (+0x420) | `FUN_140457196` (double) | point tolerance |
| `c048` (+0x428) | driver | segment table (6 doubles per segment) |
| `c050` / `c058` | driver | Fortran real / integer work arrays (`RW`, `IW` below) |
| `DAT_140721f74` | D reader | count of entity type -102 (never matches, see §4.3) |

Before pass 1 the entity counters are zeroed by the D reader only when a D section is reached.

## 4. Pass 1

`FUN_14040c220(name, ctx, 1, 0)`: `FUN_1401b9020` (no-op); `strcpy(ctx, name)`;
`len = FUN_140323bf0(nm, name, 256)` (`strncpy` 256 then blank-pad from `strlen` to 256).
`FUN_1404570f0(nm, …)` → `FUN_1404579fc(nm, &DAT_140721dc0, RW, &nA, &nB, IW, &n100, &n110,
&n112, &n126, &n124, &st, 200)` (nm is `CHARACTER*200`). `st ≠ 0` → **-4** (status 1 and 2
alike; the codes -5/-6/-7 of the second routine are unreachable because `FUN_140457196` never
sets its status). Then `FUN_140457196` (§4.5); `nA < 1` → **-8**; else 0.

### 4.1 `FUN_1404579fc` — main loop (unit 7)

```
try = 0
loop:
  OPEN(7, FILE = (try==0 ? nm(1:200) : TMP(1:256)), STATUS='OLD', ACTION='READ')
      error -> st = 1, return
  rewind (FUN_1404b5a10)
  repeat:
     READ(7,'(A72,A1,I7)') BUF, SEC, SEQ      error -> st = 1, return ; EOF -> leave
     if SEC == 'D' or 'd':
         D reader (4.2) with BUF/SEC holding the first D record
         if SEC == 'P' (after the D reader):  goto PSEC
     else if SEC == 'P': goto PSEC
  CLOSE(7)
  if try == 1: st = 2, return
  L = trimmed length of nm (FUN_14044a4b1: blanks/NULs stripped, string left-justified)
  FUN_1401b9100(nm(1:L), "_TMP_SFTC") ; error -> st = 2, return
  TMP = nm(1:L) // "_TMP_SFTC" ; try = 1 ; goto loop
PSEC:
  sort records (4.3), P reader (4.4) with BUF = first P record, CLOSE(7), return (st = 0)
```

* Records are 80 columns: data cols 1–72, section letter col 73, sequence cols 74–80 (`I7`;
  blanks read as 0; letters → read error → st 1 → **-4**). Shorter lines are blank-padded,
  longer ones truncated. CRLF and LF-only line ends both work (case `lf`).
* Only uppercase `P` starts the P section; `D` and `d` are both D records.
* `FUN_1401b9100(src, suffix)` (C): `stat` size `s` (`FUN_1401b8530`); `fopen(src,"rt")`,
  `fread(s)`; **bytes read ≠ s → error 9** (any CRLF file, because text mode drops the CRs);
  else `fopen(src+suffix,"wt")` and write the data in 80-byte pieces, each followed by
  `"\r"` and `"\n"` (text mode → `\r\r\n`); while more than 80 bytes remain a full piece is
  written, the last ≤ 80 bytes are written with the same terminator. Other errors 1–16 (null
  or empty names, source not readable, malloc, short writes). The `_TMP_SFTC` file is **never
  deleted**. So a file without line ends is split and read again (case `nonl`); a CRLF file that
  has no P section fails at once with -4 and no temp file (case `nop`).

### 4.2 D reader `FUN_140460f50`

Zeroes nB and the five counters. Per entity (first one taken from BUF, the following ones read
with `(A72,A1,I7)`; EOF → return; `SEC` not `D`/`d` → return with that record left in BUF/SEC):

1. Internal `READ(BUF,'(8I8,4I2)')` → f1..f8, s1..s4 (cols 1–64 in 8-column fields, status
   cols 65–72). **No error branch**: a non-numeric field aborts DEF_PRE (case `baddfield`):
   stderr `? FORTRAN Runtime Error:\r\n? Illegal character in numeric input\r\n? READ(UNIT=INTERNAL,...\n`,
   exit code -1, no further output. Blank fields read as 0.
2. `READ(7,'(5I8)') type, lw, color, nlines, form` — the next record, section not checked. The
   **type from this second line** is used; parameter pointer `f2` and transformation pointer
   `f7` come from line 1. Form numbers, status, level, view, subordinate flags are ignored.
3. `cnt` = number of D records read by this call so far (2 after the first entity, +2 each).
4. Record `IW[n] = {kind, f2, f7, 0, 0}` (5 ints, `n` = ++nB):

| type | kind | counter | field 3 |
|---|---|---|---|
| 100 | 1 | n100 (`c028`) | f7 |
| 110 | 2 | n110 (`c02c`) | f7 |
| -102 | 3 | `DAT_140721f74` | f7 (dead: 102 is positive in any file) |
| 124 | 4 | n124 (`c038`) | `cnt-1` = its own DE number (counted from the first D record) |
| 112 | 5 | n112 (`c030`) | f7 |
| 126 | 6 | n126 (`c034`) | f7 |
| other | — | — | not stored |

No bound check against the 1.5 M-int work array.

### 4.3 Sort `FUN_140468677(IW, &2, &nB, &1, &5)`

Bubble sort of the nB 5-int rows by column 2 (parameter pointer), ascending: for i = 1..nB-1,
for j = nB down to i+1: swap rows j-1, j when `key(j) < key(j-1)`. Stable; equal to a stable sort.

### 4.4 P reader `FUN_140461b95`

State: `cur` = number of the P record last read (1 = the record in BUF), value buffer
`V[1..]` (doubles), flag `rd` (0 = BUF already parsed into V).

* If record 1's pointer is 1: `V = parse(BUF)`, `cnt0 = #values`, `rd = 0`; if > 1: `rd = 1`;
  (0 records: nothing.)
* For each sorted record (index i, pointer p, kind k):
  1. read and discard records `cur+1 .. p-1` with `(A1)` (`cur` follows). Records are only
     read forward; pointers must therefore point at or after the current record.
  2. `IW[i].f4 = nA + 1` (first slot of this entity in `RW`, 3 doubles per slot).
  3. Values (`INPSEC` = 4.4.2): `need(m)`: if `rd == 0` → read `m - cnt0` more values (only if
     > 0) appended after `cnt0`; else read `m` values into `V[1..]`; then `rd = 1`.
     * kind 2 (110), m = 7: slots nA+1 = (V2,V3,V4), nA+2 = (V5,V6,V7); nA += 2.
     * kind 1 (100), m = 8 (`100,ZT,X1,Y1,X2,Y2,X3,Y3`): slots (V5,V6,ZT) start,
       (V3,V4,ZT) centre, (V7,V8,ZT) end; nA += 3.
     * kind 4 (124), m = 13: slot nA+i (i=1..4) = column i of the 3×4 matrix:
       (V[1+i], V[5+i], V[9+i]); nA += 4.
     * kind 5 (112): `rd == 0` → use V/cnt0, else INPSEC with m = 0 (reads exactly one record).
       `N = NINT(V5)`, then INPSEC `13N - cnt + 6` more values after `cnt` (always at least one
       record, even when ≤ 0). Block written from slot nA+1 (doubles b[0..]): b0 = N,
       b1 = 4N + (N+1)/3 + 2, b[3..3+N] = T(1..N+1) = V6..V(6+N); coefficients for segment s
       at b[q·3+6 + 12(s-1)] with q = (N+1)/3 (integer), 4 rows A,B,C,D × (X,Y,Z): row r comp
       c = V[N+7 + 12(s-1) + (r-1) + 4(c-1)] (IGES order AX BX CX DX AY … DZ). The 12 terminal
       values are not read. nA += 2 + 4N + q.
     * kind 6 (126): first values as for 112. `K = NINT(V2)`, `M = NINT(V3)`; INPSEC
       `5K + M + 15 - cnt` more. Block: b0 = K, b1 = M, b2 untouched (0), b3 = K+M+9,
       b4 = 2K+M+10, b5 = 5K+M+13 (1-based positions in b of weights, control points, V0);
       b[6 + j] = V[8 + j] for j = 0 .. 5K+M+8 (knots T(-M).., weights, control points, V0, V1,
       one extra value). nA += 1 + (5K+M+15)/3.
     * kind 3: dead.
  4. After the last record: `IW[nB+1].f4 = nA + 1` (sentinel row).

`NINT(x)` here is `|x| ≥ 2^52 ? (int)x : (int)(x + (x ≥ 0 ? 0.5 : -0.5))`.

#### 4.4.1 Line parser `FUN_140464245(rec, V+off, &n)`

`READ(rec,'(A72)') L`; `n = 0`; `start = 1`; for pos = 1..72: if `L(pos)` is `,` or `;`:
`prev = (pos == 1) ? L(72) : L(pos-1)`; if `prev == ','` → value 0.0; else value =
`FUN_140468960(L(start:pos-1))`; on its error flag **return immediately** (rest of the record
ignored, values so far kept). `n++`, store, `start = pos + 1`. Text after the last delimiter
(columns 65–72 hold the DE back-pointer, and anything after `;`) is not a value. A `;` does not
end the scan: delimiters after it still produce values. Hollerith constants are not supported
(they give an error, §4.4.3).

#### 4.4.2 `INPSEC` `FUN_140463e15(dest, &got, &cur, &want, 7)`

`k = 1`; do { `READ(7,'(A72,A1,I7)')`; EOF → `WRITE(*,*) 'INPSEC : STOPPING'` (stdout
`" INPSEC : STOPPING\r\n"`, written before the buffered C stdout text) and `STOP` (DEF_PRE ends
with exit code 0, nothing else written; case `trunc`); `cur++`; parse into `dest[k..]`;
`k += n`; `got = k-1` } while `got < want`. Section letters and back-pointers are not checked.

#### 4.4.3 Field value `FUN_140468960(s, len)`

`s` copied to a blank-padded `CHARACTER*80`. `len < 1` → value **unchanged** (the previous
value of the static result is stored again), no error. `len == 1`: `E`/`e` → 0.0; a non-digit
→ error; a digit → as below. Otherwise `WRITE(FMT(3:4),'(I2)') len` into `"(E  .0)"` and
`READ(s, FMT) value`; read error → error flag 1. Fortran `Ew.0` input: blanks ignored, all
blanks → 0, exponent letters E/D (either case) or a signed exponent without letter, no implied
decimal digits.

**Value (T8-2).** The runtime's REAL*8 text conversion is `FUN_1404c971c` (reached through the
item handler `FUN_1404bb320` → `FUN_1404c5c3c`; `FUN_1404c7090` removes the blanks first,
`FUN_1404c7130` mode 5 checks the characters, `FUN_1404c736c` gives the implied-decimal shift, 0
for `Ew.0`). It is **not** correctly rounded:
* after the sign, digits go into a 64-bit integer `u = 10u + d` while its high word is
  ≤ `0x19999999` (about 19 digits); further integer-part digits only add 1 to the decimal
  exponent, further fraction digits are dropped. Fraction zeros are counted and multiplied in only
  when a non-zero digit follows (trailing zeros cost nothing), each fraction digit taken subtracts
  1 from the exponent;
* exponent: letter `E`/`D` (either case), then an optional sign, digits (32-bit wrap). A `Q`
  passes the check but its exponent is **not** read (`1.5Q2` = 1.5);
* `u` → double by `FUN_1404ca0d4`, which shifts the integer into 53 bits and **drops** the low bits
  (`9007199254740993` → 2^53, `1180591620717411303424` → `0x444fffffffffffff`);
* exponent `e < -307`: `x = x / 10^(-(e+307))`, `e = -307`; then `x = 10^e · x` (e > 0) or
  `x / 10^-e` (e < 0), with `10^n` from `FUN_1404b47fc` (binary powering, `ps_powi`);
* the sign is or-ed into the result (`-0` → -0.0); `0E400` = 0·∞ = NaN; `I…`/`N…` give ∞ / NaN
  but are rejected by the check.

Checked bit for bit against the routine itself in a mapped copy of the binary
(`deform-rea-work/scripts_p803/ew0diff`, 1.16 M vectors: the probe spellings, 1–30 digit
mantissas with every point position, ties and near-ties between adjacent doubles, E/D/e/d/Q and
sign-only exponents, the high-word limit, the −307 split, subnormal and overflowing values): no
difference; 47 % of the vectors differ from `strtod`. The 28 GFREAD Ew.0 probes pass.

### 4.5 `FUN_140457196` — geometry build and tolerance

`ac0c = 3nA + 1` (RW2 = RW + ac0c - 1), `ac18 = 5nB + 6` (IW2), then
`FUN_14046519b(RW, IW, RW2, IW2, &nA, &nB, &30)`, `FUN_1404648ff(RW2, &nA, IW2, &nB, &dmin)`,
`tol = dmin · 0.009999999776482582` (`(double)0.01f`, `DAT_140bb2100`); `tol == 0` →
`9.999999717180685e-10` (`(double)1e-9f`). These globals (`ac0c`, `ac18`) are reused by pass 2.

#### `FUN_14046519b` (records → output points)

`cur = 1`, `k = 0`. For each sorted record i with kind ∉ {3, 4}: `k++`;
`IW2[k] = {kind, k, cur, last, i}`, `IW[i].f5 = k`; points:
* 110: copy 2 slots; 100: copy 3 slots (start, centre, end);
* 112: `FUN_140462e15` (below); 126: `FUN_140463267` → `FUN_140463374` (below);
then transform (`FUN_1404645b0`) when `f7 ≠ 0`: the first record j ≠ i with kind 4 and
`IW[j].f3 == f7`; for every output point p: `p'(r) = ((0 + R(r,1)p1) + R(r,2)p2) + R(r,3)p3 + T(r)`
(`FUN_140464720`); a missing matrix → no transform; the matrix's own f7 is ignored.
Arcs then `FUN_140467142`: `a1` = sweep of the source arc, `a2` = sweep of the output arc
(`FUN_14046736c`, each `+6.2831853071796` when < 0); `|a1 - a2| > 9.999999747378752e-06` →
swap the output start and end points (mirror). `cur += npts`. At the end `nA = cur - 1`,
`nB = k`. (Kind-3 composite handling after the loop is dead.)

**112 sampling `FUN_140462e15`** (8 steps, `DAT_14072c928`): for s = 1..N:
`dt = (T(s+1) - T(s)) / 8.0`; for j = 0..7: `u = (double)j·dt`, `u2 = u·u`, `u3 = u·u2`,
`P = ((u·B + A) + u2·C) + u3·D` per component. Final point: `u = T(N+1) - T(N)`, same formula
with the segment-N rows (`(B·u + A) + u2·C + u3·D` in that order). 8N + 1 points.

**126 sampling `FUN_140463374`** (30 = `DAT_140628550`): for i = 0..29:
`u = (double)i · ((V1 - V0) / 29.0) + V0`; knots `t(1..) = T(-M)..`; find the first
j = M+1 .. K+M+1 with `t(j) ≤ u ≤ t(j+1)`; none → no point. Basis `FUN_1404635be`
(table Nt, (M+1)², zeroed, `Nt(M+1,1) = 1`): for p = 1..M, s = 0..p:
`d1 = t(j+p-s) - t(j-s)`, `A = d1 ≠ 0 ? ((u - t(j-s)) / d1) · Nt(M-s+1, p) : 0`;
`d2 = t(j+p+1-s) - t(j+1-s)`, `B = d2 ≠ 0 && M-s+2 ≤ M+1 ? ((t(p+1+j-s) - u) / d2) · Nt(M-s+2, p) : 0`;
`Nt(M+1-s, p+1) = A + B`. Point `FUN_1404639eb`: for c = 1..M+1, ctrl index i = j-M-1+c
(weights/points 1-based from W(0)/P(0)): `o_k = (w_i·P_i,k)·Nt(c,M+1) + o_k`,
`d_k = w_i·Nt(c,M+1) + d_k`; point = `o_k / d_k`.

#### `FUN_1404648ff` (shortest distance)

`dmin = 1.0e6`; for every IW2 record and every consecutive pair (p, p+1) of its points (arcs:
start–centre, centre–end): `d = FUN_1404681dc` = `sqrt((dx·dx + dy·dy) + dz·dz)` with
`d = q - p` per component; `d ≠ 0 && d < dmin` → `dmin = d`.

## 5. Pass 2 (`FUN_14040c220(…, 2, 500)`)

Mode 500 → `mode = 1`, `flag = 1`, table = `c048`. `FUN_14040c740(ctx)` deduplicates the
`c024` rows of `c048` (type 1 rows equal or reversed within `ctx+0x420`, type 2 rows equal) —
a no-op here because the table is still all zero. Then
`FUN_140457312(&nA, &nB, &tol, RW, IW, c048, &1.5M, &1.5M, &mode, &flag, &st)`:
`tol ≤ 0` → 1e-9f; `mode ≠ 1` would rebuild (`FUN_140458e6f`, not used);
`ac10 = ac0c + 3nA` (D array), `ac1c = ac18 + 5nB` (L pairs), further int arrays of 2nB
(`dc4` loop starts, `dc8` S list, `dcc` A, `dd0` B, `dd4` merged list). If
`1.5M < dd4` or `1.5M < ac10` → st 1 (**-9**). Then match, trace, and (flag 1) output;
st 3 → **200**, st 0 → 0. (flag 0 would write DEF_IGS.OUT, §7.)

### 5.1 Match `FUN_140464abc`

Per point p: `L1(p) = 0` (partner), `L2(p) = 0` (own record), `D(p) = tol`.
For each record r (kind ≥ 0) and its two ends `e = first + m·(last-first)`, m = 0, 1 (a
one-point record divides by zero): `d = D(e)`; `L2(e) = r`; if `d ≠ 0`, for each record q ≠ r and
its two ends g (in that order): if `e == g` (never) …; if `D(g) == 0` skip g;
`dd = dist(P(e), P(g))`; `dd < d` → `d = dd`, `best = q`, `L1(e) = g`, `L2(e) = r`; `dd == 0`
→ stop the search for this end (no mutual update). After each q: if `d < D(e)`:
`g = L1(e)`, `D(e) = D(g) = d`, `L1(g) = e`, `L2(g) = best`.
Then for p = 1..nP: `g = L1(p)`; if `g > 0`: `L1(g) = -L1(g)`;
`mx = (P(p).x + P(g).x)·0.5`, `my = (P(p).y + P(g).y)·0.5` (z untouched) written to both
points; `D(p) = D(g) = 0`. Finally `L1(p) = |L1(p)|`.
Unmatched ends keep `D = tol` and `L1 = 0`. Interior points (arc centres, spline interior)
keep `D = tol`, `L2 = 0`.

### 5.2 Trace `FUN_140467473` (flag 1)

`neg(x) = (x == 0) ? -1.000000013351432e-10 : -|x|`.
For p = 1..nP with `D(p) ≥ 0`: loops++; **second time → st = 3**, every record r becomes its
own loop `S(r) = (first(r), r)`, loops = nB, return (coordinates keep the midpoint moves).
First time: `r0 = L2(p)`, `A = [(p, r0)]`, `B = []`, `c = first(r0)`, if `p == c` → `c = last(r0)`;
`d = D(p)`; loop:
```
if d < 0 and c == 0: stop
if d < 0: p = c; c = 0; d = D(p); continue
dold = d; D(p) = neg(d); d = D(p)
if tol < dold: continue                  (never true)
g = L1(p)                                (0 for an open end: D(0) is RW2's last z value)
if D(g) != dold: continue                (dead end -> turn round or stop)
D(g) = neg(D(g)); r = L2(g)
if r == r0: stop                         (closed)
append (g, r) to (c == 0 ? B : A)
p = other end of r (first if last == g, else last); d = D(p)
```
Merged list `S = reverse(B) + A`; all points of those records get `D = neg(D)`. If |S| > 1,
`FUN_140467e54` sets each entry's **start point**: for consecutive records (ri, rj) with ends
(fi, li), (fj, lj): `d1 = |fi-fj|`, `d2 = |li-fj|`, `d3 = |fi-lj|`, `d4 = |li-lj|`, the first
strict minimum gives the case (1..4); cases 1, 3 → `S(i).pt = li`, else `fi`. The last entry:
case of the last pair 1, 2 → `fj`, else `lj`. With |S| = 1 the start is the first visited point.

### 5.3 Output `FUN_140459149`

For each S entry (record r, start pt, `rev = first(r) ≠ pt`), segment index k counts from 1:
* arc (kind 1): `a = FUN_14046736c(start, end, centre)`, `a < 0` → `a += 6.2831853071796`;
  row `[2.0, S.x, S.y, C.x, C.y, a]` with S = first point, or for rev S = third point and
  `a = -a`. A full circle (start = end) gives `a = 0` (no 360 correction).
* line and splines (kinds 2, 5, 6): n points, rows `[1.0, Pj.x, Pj.y, Pj+1.x, Pj+1.y, 0.0]` for
  j forward 1..n-1, or backward from n when rev.
`c024 = k_last` (segment count).

`FUN_14046736c(a, b, c)`: `ax = a.x-c.x`, `ay = a.y-c.y`, `r1 = sqrt(ay·ay + ax·ax)`; same for b
→ `r2`; `y = ((ax·by - bx·ay) / r1) / r2`, `x = ((ay·by + ax·bx) / r1) / r2`; result
`FUN_1404b3878(y, x)`:
* `x == 0`: `y == 0` → 0, `y < 0` → -π/2, else π/2;
* else `t = atan(|y/x|)` (`FUN_1404b3f48`); `x < 0` → `t = π - t`; sign bit of y → `t = -0.0 - t`.

`FUN_1404b3f48(x)` (Cody–Waite atan, must be reproduced bit for bit): `neg = x < 0` → `x = -0.0 - x`;
`n = 0`; `x > 1` → `x = 1/x`, n = 2; `x > 0.2679491924311227` → n++,
`x = (((x·0.7320508075688773 - 0.5) - 0.5) + x) / (x + 1.7320508075688772)`;
if `|x| > 1.05367121277235e-08`: `g = x·x`,
`num = ((((-8.494624035132068 - g·0.8375829936815006)·g - 20.505855195861653)·g - 13.688768894191927)·g)`,
`den = ((((g + 15.024001160028575)·g + 59.57843614259735)·g + 86.15734959713025)·g + 41.06630668257578)`,
`x = x + (num/den)·x`; `n > 1` → `x = -0.0 - x`; add `0.5235987755982989` (n 1),
`1.5707963267948966` (n 2), `1.0471975511965979` (n 3); `neg` → `x = -0.0 - x`.
Constants: π = 3.141592653589793, 2π literal 6.2831853071796 (`0x401921FB54442D28`).

## 6. KEY output after KFWRIT

Only object N's `DIEGEO` block changes (TPLOGY etc. are untouched; whole OUT.KEY diffs
confirmed). Writer `FUN_1402aaa70` (see `DIEGEO.md` on branch kw-g2geo):
```
"%-6s %7i %7i %7i\n"                       DIEGEO, N, 2, nseg
" %7i %7i %20.10E %20.10E %20.10E %20.10E %20.10E\n"   k, (int)type, x1, y1, x2, y2, angle·57.29577951308232
```
type 1 = line (x2,y2 = end, angle 0), type 2 = arc (x2,y2 = centre, signed sweep in degrees).
Exponents have three digits (`E+000`). When the reader fails (-4, -8, …) the old geometry is
written unchanged.

## 7. DEF_IGS.OUT

Created empty by step 2.4 and deleted after a successful pass 1. On -4/-8 (and when the process
stops, §4.4.2, §4.2) the empty file stays. Writer `FUN_1404587d1` (only with flag 0, mode 600
from the mode-400 path; unreachable from GFREAD): `OPEN(7,'DEF_IGS.OUT',STATUS='UNKNOWN')`
(error → st 2), list-directed loop count, then per loop `(I5,I5)` loop number and segment count,
rows by `FUN_1404668e4` (arc) / `FUN_1404665f3` (lines, format `"(I5,' 2 ',5(F16.9,1X))"` family);
not specified byte for byte because no batch path reaches it.

## 8. Fallback text route (allocation failure only)

When `c048 == 0` (malloc of n doubles failed; `n < 1` cannot happen after a successful pass 1):
free c050/c058, `t = calloc(0x88)`, `r = FUN_140455e60(t, "DEF_IGS.OUT")`, delete DEF_IGS.OUT;
r -1 → -14, r 0 → `FUN_14040b100(iges, obj, 2)`, else -15; `FUN_140454e40(t)` frees.
* `FUN_140455e60`: `fopen "rt"` (fail → -1; `t == 0` → -2); `"%d\n"` nLoops; per loop
  `"%d %d\n"` (ignored, nSeg); per segment `"%d %s %s %s %s %s %s\n"` → `atof` type, x1, y1, x2,
  y2, a. Points added with `FUN_1404564c0` (merge when `|dx| ≤ 1e-6`, `|dy| ≤ 1e-6` and
  `sqrt(dx²+dy²) ≤ 1e-6`); `a·π/180`, `|.| < 1e-6` → 6.283185307179586; type 2.0 → mid point
  `FUN_14046b2d0` (rotation of p1 about p2 by a/…; `FUN_140476bb1`); segment
  `FUN_1404565c0`; bounding boxes in float (`FUN_14046a810`).
* `FUN_14040b100`: errors -1 (no obj / no data), -16 nLoops < 1, -17 nSeg < 1, -18 nPts < 1,
  -19/-20/-21 null arrays, -22 first segment type ∉ {1,2}, -24 nLoops ≥ 101 (geometry cleared).
  Otherwise one curve per loop (type 2, rows `[type, x1, y1, x2, y2, a·π/180]`, a < 1e-6 for
  type 2 → 360°).

## 9. Messages

| Output | When |
|---|---|
| stdout `" INPSEC : STOPPING\r\n"` then STOP (exit 0) | EOF inside an entity's parameter data |
| stderr `? FORTRAN Runtime Error:` / `? Illegal character in numeric input` / `? READ(UNIT=INTERNAL,...` (exit -1) | non-numeric D-entry field |
| GFREAD `INFO   : Loading geometry - Done` | every result except -1 |

The C reader itself prints nothing.

## 10. Oracle cases (`tests/commands/GFREAD/gfread_igs_*.json`, run 2026-10-21)

All use RING_SI.KEY, `GFREAD 1`, KFWRIT. Prediction = a Python model of this spec; **all 26
cases matched byte for byte** (DIEGEO block) and in their left-over files.

| Case | Input | Confirms |
|---|---|---|
| (existing) `gfread_2d_igs` | BILLET.IGS | trace direction from point 1's partner |
| `slot` | data/slot.igs | sort by P pointer, ignored 116, arcs traversed backwards (-180°) |
| `spline112` | data/spline112.igs | 112: 8 steps per segment, polynomial order |
| `nurbs126` | data/nurbs126.igs | 126: 30 samples, rational basis, reversed polyline |
| `xform124` | data/xform124.igs | 124 matrix on arc + lines, mirror swap of arc ends |
| `circle` | data/circle.igs | full circle → angle 0 |
| `gap` | data/gap.igs | tolerance 0.01f·dmin, midpoint averaging |
| `openchain` | data/openchain.igs | B list reversed + A, start-point cases |
| `lf` | inline LF-only BILLET | LF records accepted |
| `nonl` | inline BILLET without line ends | `_TMP_SFTC` split (`\r\r\n`), file left |
| `nop` | data/nop.igs | no P section, CRLF → copy fails, -4, empty DEF_IGS.OUT left |
| `badseq` | data/badseq.igs | letters in I7 → -4, geometry unchanged |
| `baddfield` | data/baddfield.igs | Fortran runtime abort, exit -1 |
| `trunc` | data/trunc.igs | INPSEC : STOPPING, exit 0, no OUT.KEY |
| `coldext` | Cold_ext.igs | two loops → code 200, P-pointer order, forward |
| `plane` | PLANE.IGS | single open line |
| `topblk` | TOPBLK.IGS | only 128/402 → -8, geometry unchanged |
| `install` (10 cases) | Rib_web, QUENCH_PART, TOOTH, bolt-punch3, Disk_wkp, Hot_axle, Cold_cup, Shp_roll, Spike_TopDie, StubShaft_HeadTools | larger files, arcs, status 0 and 3 |

Data files are written by a generator (fixed columns); `lf`/`nonl` use inline `text` so git
line-end conversion cannot change them.

## 11. Helpers

| Address | Role |
|---|---|
| `FUN_140409af0` | driver (§2) |
| `FUN_14040c220` | pass dispatcher (§4, §5) |
| `FUN_14040c740` | segment-table dedupe (no-op for GFREAD) |
| `FUN_1401b8420` / `FUN_1401b85d0` / `FUN_1401b8560` | `fopen "at"` / delete / readable test |
| `FUN_140323bf0`, `FUN_140323b90` | strncpy + blank pad |
| `FUN_1404570f0` → `FUN_1404579fc` | pass-1 reader (§4.1) |
| `FUN_140460f50` | D section (§4.2) |
| `FUN_140468677` | bubble sort |
| `FUN_140461b95`, `FUN_140464245`, `FUN_140463e15`, `FUN_140468960` | P section, line parser, INPSEC, field value |
| `FUN_14046414e` | 124 matrix → 4 slots |
| `FUN_1404627bf`, `FUN_140462b00` | 112 / 126 parameter blocks |
| `FUN_140457196`, `FUN_14046519b`, `FUN_1404648ff` | build points, min distance, tolerance |
| `FUN_140462e15`; `FUN_140463267`, `FUN_140463374`, `FUN_1404635be`, `FUN_1404639eb` | 112 / 126 sampling |
| `FUN_1404645b0`, `FUN_140464720`, `FUN_140467142` | transform, arc mirror check |
| `FUN_1404681dc`, `FUN_14046736c`, `FUN_1404b3878`, `FUN_1404b3f48` | distance, arc sweep, atan2, atan |
| `FUN_140457312`, `FUN_140464abc`, `FUN_140467473`, `FUN_140467e54`, `FUN_140459149`, `FUN_1404594a3`, `FUN_140459607` | pass 2 |
| `FUN_1404587d1`, `FUN_1404665f3`, `FUN_1404668e4`, `FUN_140458e6f`, `FUN_140465bb1`, `FUN_14045979e` | DEF_IGS.OUT writer, rebuild, composites (not reached) |
| `FUN_1401b9100`, `FUN_14044a4b1` | `_TMP_SFTC` copy, trim |
| `FUN_1404b5054`, `FUN_1404b5cd8`, `FUN_1404b5ef4`, `FUN_1404b53a4`, `FUN_1404b4108`, `FUN_1404b33e4`, `FUN_1404b5acc` | Fortran runtime OPEN/READ/WRITE/CLOSE/compare/assign/STOP |
| `FUN_1404c971c`, `FUN_1404ca0d4`, `FUN_1404b47fc` (+ `FUN_1404c7090`, `FUN_1404c7130`, `FUN_1404c736c`) | runtime REAL*8 text conversion of `Ew.0` fields (§4.4.3) |
| `FUN_140455e60`, `FUN_14040b100` (+ `FUN_140455150`, `FUN_1404564c0`, `FUN_1404565c0`, `FUN_1404557a0`, `FUN_140455aa0`, `FUN_140456720`) | fallback text route (§8) |
| `FUN_140276030`, `FUN_14040cde0`, `FUN_14040ce60`, `FUN_14040ce40`, `FUN_140198230`, `FUN_14019a650`, `FUN_1404098d0` | storage setters |

## 12. Open questions

* The character check of `Ew.0` fields is the probes' shape ([sign] digits [. digits] [exponent],
  exponent digits required); `FUN_1404c7130` mode 5 also accepts e.g. `1E`, `E5`, `.` and a
  second exponent sign (`1E5+3`), untested (no IGES writer produces them).
* Behaviour of other runtime errors without an error branch (D line 2 `(5I8)`, EOF on D line 2)
  is assumed to be the same abort as `baddfield`; only the internal read was tested.
* Edge cases left as read from code, untested: one-point records (integer divide by zero in
  §5.1), open-end lookup of `D(0)` (= last point's z), empty fields giving a stale value,
  pointers that go backwards, more than ~300 k entities (no bound check).
* DEF_IGS.OUT writer format (§7) and the fallback route (§8) are summarised only; both are
  unreachable from batch GFREAD.
