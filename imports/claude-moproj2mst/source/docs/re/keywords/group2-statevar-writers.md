# Group 2 state-variable writer `FUN_1402a4a80` and its line writers

Source: `DEF_PRE_64.exe` v11.0.2, Ghidra decompile (`deform-rea-work/out/g2/writers2.c`,
`out/g2/D_wsv.c`, `out/g2/D_lw.c`) and disassembly of `FUN_1402a4a80`
(`out/g2/D_wsv.lst`; every stack argument quoted below was checked there, the decompile drops or
garbles several of them). Every per-node / per-element keyword spec refers to this file for the
exact output layout. Shared mechanics (return codes 2/3 print an error and the KEY loop
continues, stdout = `FUN_14047903c()+0x30`, value-line writers `FUN_1402f2e80`/`FUN_1402f2fe0`,
table object and its writer `FUN_1402f3130`) are in `_common.md` (sections 1 and 3) and are not
repeated here.

Number formats used everywhere below: `%7i` (int, right-justified in 7), `%20.10E` (double,
1 digit + 10 decimals, MSVC prints a **3-digit exponent**: `-1.4567515000E-001`, width 20 right
justified). Names are printed with `%-6s` (registry name from `FUN_1402f7720(id)`, or an internal
`"*XXXX"` string for codes that have no keyword).

## Entry point

`int FUN_1402a4a80(MODEL *model, FILE *f, OBJ *obj, uint code, int objNo, int extra)`
(error text: `_DB_STEP_DATA::KWD_writeObjStateVariables`, `code` = "eObjVarTyp").

* `objNo < 1` → `objNo = obj+8` (the stored object number).
* `lVar2 = obj+0x3a98` (`FUN_140158f50`: the object's owning model; used only in cases
  0x26/0x2b/0x2d for the second-array component count).
* `switch(code)`; unknown code (7, 0x1a–0x1c, 0x1e–0x21, 0x24, 0x2f, 0x40, 0x4d and ≥ 0x4f) →
  `printf("ERROR: _DB_STEP_DATA::KWD_writeObjStateVariables doesn't handle eObjVarTyp %d", code)`
  (stdout, **no newline**) and return **3**. Every other path returns **0**: the status of the
  line writer is discarded.
* Callers: the registry write handlers (`FUN_1402b4280` …, signature `h(FILE*, MODEL*, ref)`;
  they look up the object, apply the `obj+0x3f24` renumbering, call
  `FUN_1402a4a80(model, f, obj, code, objNo, 0)` and **always return 0**), and
  `FUN_14029e250` directly for codes 0x12–0x15, 0x19, 0x2e, 0x3d. Code 0x4e is called by
  `FUN_1402b7310` with `extra = ref slot 1` (custom variable id). See `group2-writer-map.md`
  for the emission order.

Common arguments of the double writers below: `n` = entity count (`obj+0x880` = number of
nodes, `obj+0x884` = number of elements), `m` = components per entity = the model's
**component table** entry `model+0x15f8 + 4*code` (written `ncomp[code]` below), default value
`d = 0.0` (always 0.0 from this function), `fmt` (header layout) and `k` (extra int, fmt 4 only).

## Code table (all cases)

"Writer" column: `5c10/fmt` = `FUN_1402f5c10` with that header format; `4df0` = integer
writer; `60c0/fmt` = two-array writer; `58d0` = integration-point writer; `6600`/`6a10` =
edge/face writers; `6e40` = custom-variable writer. Name `id` is the registry id passed to
`FUN_1402f7720`.

| code | keyword (id) | data | n | m (model) | writer | condition / remark |
|---|---|---|---|---|---|---|
| 0x00 | RZ (159) | `obj+0x8a8` | 0x880 | – | `FUN_1402a2db0` | coordinates, see below |
| 0x01 | DRZ (160) | `obj+0x8b0` | 0x880 | 0x15fc | 5c10/0 | |
| 0x02 | URZ (165) | `obj+0x8b8` | 0x880 | 0x1600 | 5c10/0 | |
| 0x03 | FRZ (166) | `obj+0x8c0` | 0x880 | 0x1604 | 5c10/0 | |
| 0x04 | `"*FRZB"` | `obj+0x8c8` | 0x880 | 0x1608 | 5c10/0 | no caller found |
| 0x05 | PRZ (167) | `obj+0x8d0` | 0x880 | 0x160c | 5c10/0 | |
| 0x06 | `"*PRZB"` | `obj+0x8d8` | 0x880 | 0x1610 | 5c10/0 | no caller found |
| 0x08 | NDTMP (170) | `obj+0x8e0` (+`0x900`) | 0x880 | 0x1618 | 3D: 5c10/0; 2D: 5c10/3 with m = 2 | 2D writes the pair (T−DTMP, T), see NDTMP.md |
| 0x09 | NDHEAT (171) | `obj+0x8e8` | 0x880 | 0x161c | 5c10/0 | |
| 0x0a | `"*HITN"` | `obj+0x8f0` | 0x880 | 0x1620 | 5c10/0 | no caller found |
| 0x0b | NDFLUX (172) | `obj+0x8f8` | 0x880 | 0x1624 | 5c10/0 | (caller: 3D only) |
| 0x0c | `"*DTMP"` | `obj+0x900` | 0x880 | 0x1628 | 5c10/0 | no caller found |
| 0x0d | DATOM (214) | `obj+0x908` | 0x880 | 0x162c | 5c10/0 | |
| 0x0e | CRBFLX (221) | `obj+0x910` | 0x880 | 0x1630 | 5c10/0 | (caller: 3D only) |
| 0x0f | VOTAGE (213) | `obj+0x918` | 0x880 | 0x1634 | 5c10/0 | |
| 0x10 | RHTFLX (222) | `obj+0x920` | 0x880 | 0x1638 | 5c10/0 | (caller: any dimension) |
| 0x11 | USRNOD (196) | `obj+0x968` | 0x880 | 0x163c | 5c10/3 | |
| 0x12 | BCCDEF (162) | `int obj+0x990` | 0x880 | 0x1640 | 4df0, d = 0 | |
| 0x13 | BCCDFN (163) | `int obj+0x998` | 0x880 | 0x1644 | 4df0, d = 0 | |
| 0x14 | BCCTMP (168) | `int obj+0x9a0` | 0x880 | 0x1648 | 4df0, d = 0 | |
| 0x15 | BCCTFN (169) | `int obj+0x9a8` | 0x880 | 0x164c | 4df0, d = 0 | |
| 0x16 | BCCCRB (219) | `int obj+0x9b0` | 0x880 | 0x1650 | 4df0, d = 0 | |
| 0x17 | BCCRHT (220) | `int obj+0x9b8` | 0x880 | 0x1654 | 4df0, d = 0 | |
| 0x18 | NWEAR (255) | `obj+0x928` | 0x880 | 0x1658 | 5c10/0 | |
| 0x19 | DRMESH (263) | `obj+0x930` | 0x880 | 0x165c | 5c10/0 | only if 3D (`FUN_1400015f0`) |
| 0x1d | GRAIN2 (209) | `obj+0x950` | 0x880 | 0x166c | 5c10/4, k = 2 | only if `FUN_1401d7160(model,&b) == 2` |
| 0x22 | TIMERC (223) | `obj+0x958` | 0x880 | 0x1680 | 5c10/4, k = 0 | |
| 0x23 | NRECVL (225) | `obj+0x960` | 0x880 | 0x1684 | 5c10/4, k = 0 | |
| 0x25 | ELMCON (173) | `int obj+0x9e8` | 0x884 | – | `FUN_1402a30c0` | connectivity, see below |
| 0x26 | STRESS (179) | `obj+0xa00` (+`0x948`) | 0x884 | 0x1690 (+0x1660) | see STRESS.md | `model+0x890 == 3`: inline integration-point layout; else 5c10/(2D ? 3 : 0) or 60c0/(2D ? 3 : 0) |
| 0x27 | `"*EPS"` | `obj+0xa08` | 0x884 | 0x1694 | 5c10/0 | no caller found |
| 0x28 | STNCMP (176) | `obj+0xa10` | 0x884 | 0x1698 | 5c10/1 | |
| 0x29 | `"*EFSTS"` | `obj+0xa18` | 0x884 | 0x169c | 5c10/0 | no caller found |
| 0x2a | `"*EFEPS"` | `obj+0xa20` | 0x884 | 0x16a0 | 5c10/0 | no caller found |
| 0x2b | STRAIN (175) | `obj+0xa28` (+`0x938`) | 0x884 | 0x16a4 (+0x1664) | see STRAIN.md | `model+0x88c == 3`: inline layout; else 5c10/0 or 60c0/0 |
| 0x2c | DENSTY (177) | `obj+0xa30` | 0x884 | 0x16a8 | 5c10/0 | |
| 0x2d | DAMAGE (178) | `obj+0xa38` (+`0x940`) | 0x884 | 0x16ac (+0x1668) | see DAMAGE.md | `model+0x888 == 3`: 58d0; else 5c10/(m==1 ? 0 : 5) or 60c0/5 |
| 0x2e | MATAXI (254) | `obj+0xa40` | 0x884 | 0x16b0 | 5c10/2 | |
| 0x30 | `"*HTGN"` | `obj+0xa50` | 0x884 | 0x16b8 | 5c10/0 | no caller found |
| 0x31 | VOLFC (205) | `obj+0xa58` | 0x884 | 0x16bc | `FUN_1402a3690` → 5c10/1 | phase gather, see below |
| 0x32 | VOLFS (216) | `obj+0xa60` | 0x884 | 0x16c0 | `FUN_1402a3690` → 5c10/1 | phase gather |
| 0x33 | DVOLF (206) | `obj+0xa68` | 0x884 | 0x16c4 | 5c10/1 | |
| 0x34 | TICF (207) | `obj+0xa70` | 0x884 | 0x16c8 | 5c10/1 | |
| 0x35 | GRAIN (208) | `obj+0xa78` | 0x884 | 0x16cc | 5c10/1 | |
| 0x36 | SIZESH (210) | `obj+0xa80` | 0x884 | 0x16d0 | 5c10/4, k = 0 | only if `m != 0` and data ≠ NULL |
| 0x37 | TXTODF (211) | `obj+0xa88` | 0x884 | 0x16d4 | 5c10/4, k = 0 | only if `m != 0` and data ≠ NULL |
| 0x38 | HDNOBJ (217) | `obj+0xa90` | 0x884 | 0x16d8 | 5c10/0 | |
| 0x39 | YLDS (233) | `obj+0xa98` | 0x884 | 0x16dc | 5c10/0 | |
| 0x3a | ZEFI (237) | `obj+0xaa0` | 0x884 | 0x16e0 | 5c10/0 | |
| 0x3b | ZMFI (238) | `obj+0xaa8` | 0x884 | 0x16e4 | 5c10/0 | |
| 0x3c | USRELM (197) | `obj+0xac0` | 0x884 | 0x16e8 | 5c10/3 | |
| 0x3d | MTLGRP (174) | `int obj+0x9f8` | 0x884 | 0x16ec | 4df0, **d = 1** | |
| 0x3e | CURRNT (239) | `obj+0xab0` | 0x884 | 0x16f0 | 5c10/0 | |
| 0x3f | ERECVL (224) | `obj+0xab8` | 0x884 | 0x16f4 | 5c10/4, k = 0 | only if `m != 0` and data ≠ NULL |
| 0x41 | ECCDEF (241) | `int obj+0xae8` | edges/faces | 0x16fc | 6600, d = 0 | |
| 0x42 | ECDEFN (244) | `int obj+0xaf0` | | 0x1700 | 6600 | |
| 0x43 | ECPRES (247) | `obj+0xaf8` | | 0x1704 | 6a10, d = 0.0 | |
| 0x44 | ECCTMP (242) | `int obj+0xb00` | | 0x1708 | 6600 | |
| 0x45 | ECTMFN (245) | `int obj+0xb08` | | 0x170c | 6600 | |
| 0x46 | ECHFLX (248) | `obj+0xb10` | | 0x1710 | 6a10 | |
| 0x47 | ECCATM (243) | `int obj+0xb18` | | 0x1714 | 6600 | |
| 0x48 | ECATFN (246) | `int obj+0xb20` | | 0x1718 | 6600 | |
| 0x49 | ECAFLX (249) | `obj+0xb28` | | 0x171c | 6a10 | |
| 0x4a | ECCRHT (250) | `int obj+0xb30` | | 0x1720 | 6600 | |
| 0x4b | ECRHFN (251) | `int obj+0xb38` | | 0x1724 | 6600 | |
| 0x4c | ECRFLX (252) | `obj+0xb40` | | 0x1728 | 6a10 | |
| 0x4e | CUSVAR (198) | custom var `extra` in `obj+0xb58` | var | var | 6e40, d = 0.0 | see CUSVAR.md |

Handler → code (registry handler2): `FUN_1402b4280` 8, `b4340` 0x2b, `b4d40` 0x18, `b4e00` 0x26,
`b4ec0` 0x2d, `b4f80` 1, `b5040` 2, `b5100` 3, `b51c0` 5, `b5280` 9, `b5340` 0xb, `b5400` 0x22,
`b54c0` 0x23, `b5580` 0x11, `b5640` 0x28, `b5700` 0x2c, `b57c0` 0x39, `b5880` 0x3c, `b5940` 0x31,
`b5a00` 0x32, `b5ac0` 0x33, `b5b80` 0x35, `b5c40` 0x1d, `b5d00` 0x36, `b5dc0` 0x37, `b5e80` 0x3f,
`b5f40` 0x34, `b6000` 0xd, `b60c0` 0x38, `b6180` 0x16, `b6240` 0xe, `b6300` 0xf, `b63c0` 0x17,
`b6480` 0x10, `b6540` 0x3a, `b6600` 0x3b, `b66c0` 0x3e, `b6780` 0x2e, `b6840` 0x41, `b6900` 0x42,
`b69c0` 0x43, `b6a80` 0x44, `b6b40` 0x45, `b6c00` 0x46, `b6cc0` 0x47, `b6d80` 0x48, `b6e40` 0x49,
`b6f00` 0x4a, `b6fc0` 0x4b, `b7080` 0x4c, `b7310` 0x4e (with ref slot 1).

## The component table `model+0x15f8 + 4*code`

`ncomp[code]` decides the number of values per entity **and, when 0, suppresses the keyword**
(every line writer writes nothing when `m == 0`). It is (re)computed by `FUN_1401d0320(model)`
(called from `FUN_1401d0fc0`, which USRNOD/USRELM trigger through `FUN_1401d21c0`/`FUN_1401d2090`).
`D` = model dimension (`model+0x1c`, `FUN_140158e90`); `active(c)` = `FUN_1401d64f0(model, c, &x)`
(default rule: `ncomp[c] > 0`; special rules for 0x18 (always true), 0x1d, 0x2e, 0x31–0x34
(a mixture material exists, `mat+0x1cc8 == 1`, or `model+0x85c != 0`), 0x35, 0x36, 0x37).

1. all 0x4f entries = 1;
2. 3D: `ncomp[0x2e] = 0`;
3. codes 0, 1, 2, 3, 4, 5, 6, 0x12, 0x13, 0x19 = `D`;
4. `ncomp[0x18]` (NWEAR) = 5 (`active(0x18)` is always true); 3D models later get
   `model+0x205c` from `FUN_1401d2230`, and reading NWEAR sets it to 5 again;
5. `ncomp[0x26]` (STRESS) = `ncomp[0x1a]` = `ncomp[0x27]` = 2·D;
6. `ncomp[0x28]` (STNCMP) = `model+0x880 + model+0x884 + max(1, Σ model+0x86c..0x87c)·2·D`;
7. `ncomp[0x39]` (YLDS) = 2·D; `ncomp[0x2f]` = 2;
8. `active(0x31)`: `ncomp[0x31]` = `ncomp[0x32]` = `model+0x1320` (number of phases),
   `ncomp[0x33]` = `ncomp[0x34]` = `model+0x13f4`; else all four = 0;
9. `ncomp[0x35]`, `ncomp[0x1d]`, `ncomp[0x36]`, `ncomp[0x37]` = the count returned by
   `active()` (0 when inactive);
10. `ncomp[0x38]` = 2, `ncomp[0x3a]` = 2, `ncomp[0x3b]` = 3 in 3D else 2;
11. `ncomp[0x11]` (USRNOD) = `model+0x3398`; `ncomp[0x3c]` (USRELM) = `model+0x33a8`;
12. 3D: `ncomp[0x24]` = 0, `ncomp[0x2e]` = 9 if `active(0x2e)` else 0, `ncomp[0x3e]` = D;
    2D: `ncomp[0x24]` = 4, `ncomp[0x3e]` = D, and if `model+0x850 == 3` (torsion):
    codes 2, 3, 4, 0x12, 0x13 = 3; 0x26, 0x1a, 0x27 = 6; 0x28 = `max(..)·6 + model+0x880 + model+0x884`;
    0x39 = 6;
13. `ncomp[0x1c]` (nodal DAMAGE) = 0 if `model+0x888 ∈ {0, 3}` else `model+0x33c8`;
    `ncomp[0x1b]` (nodal STRAIN) = 0 if `model+0x88c ∈ {0, 3}` (else stays 1);
    `ncomp[0x1a]` (nodal STRESS) = 0 if `model+0x890 ∈ {0, 3}`;
14. `ncomp[0x2d]` (DAMAGE) = `model+0x33d8`; `ncomp[0x22]` (TIMERC) = `model+0x11c4`;
    `ncomp[0x3f]` (ERECVL) = `model+0x123c`; `ncomp[0x23]` (NRECVL) = `model+0x12b4`.

Some readers change entries directly (DAMAGE: `ncomp[0x2d]` and `ncomp[0x1c]`; NWEAR:
`ncomp[0x18]`; FUN_140416c90 mode 2: the entry it is given).

## `FUN_1402f5c10` — `writeNodeElemVarDouble(f, name, objNo, double *v, n, m, d, fmt, k)`

1. `n == 0 || m == 0 || v == NULL || name == NULL` → return 0, **nothing written**.
   `f == NULL` → return 2.
2. **Compression** (`FUN_1402f5360(v, &n, &m, &d, &fmt, &uniform, &count, defs)`):
   * `fmt == 2`: `uniform` = every entity equals entity 1 component by component
     (`v[i*m+j] == v[j]` for all i, j);
     other formats: `uniform` = every one of the `n*m` values equals `v[0]`.
   * uniform → `defs[j] = v[j]` (j < m), `count = 0`, **no records**;
   * not uniform → `defs[j] = d` (= 0.0) for all j; `count` = number of entities with at least
     one component `!= d`; those entities are written as records.
   Comparisons are `!=` on doubles: `-0.0` counts as equal to 0.0 (written back as +0.0);
   a NaN anywhere makes the data non-uniform and its entity always written.
3. **Header line** by `fmt` (`defs[0]` = first default):

   | fmt | format | fields after name, objNo |
   |---|---|---|
   | 0 | `"%-6s %7i %7i %20.10E\n"` | count, defs[0] |
   | 1 | `"%-6s %7i %7i %7i %20.10E\n"` | count, m, defs[0] |
   | 2 | `"%-6s %7i %7i %7i"` + m × `" %20.10E"` + `"\n"` | count, m, defs[0..m-1] |
   | 3 | `"%-6s %7i %7i %20.10E %7i\n"` | count, defs[0], m |
   | 4 | `"%-6s %7i %7i %7i %7i %20.10E\n"` | count, k, m, defs[0] |
   | 5 | `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` | count, defs[0], 0, 0.0, m |
   | other | `"KwdWriteUtl::writeNodeElemVarDouble() - Not supported format\n"` to stdout (`FUN_14047903c()+0x30`), no header | |

   Only fmt 2 writes more than one default; with fmts 0, 1, 3, 4, 5 a uniform multi-component
   entity whose components differ is impossible (uniform means all values equal `v[0]`).
4. **Records** (only when not uniform), entities in ascending order, entity `i` written iff
   some component `!= d`: `fprintf(" %7i", i+1)` then `FUN_1402f2e80(f, m, &v[i*m], 5, 0)`.
   A nonzero status from it stops the loop and is returned (only if `f` is NULL, impossible here).
5. Return 0.

**`FUN_1402f2e80(f, cnt, double *v, perLine, alt)`** — the value-line writer of `_common.md`
section 3 (copy: `deform-rea-work/out/g2/_common_g13.md`); additions: `f == NULL || perLine < 1`
→ 2; `cnt < 1 || v == NULL` → 0 and writes nothing (not even `"\n"`); `alt` is always 0 here.
All state-variable writers call it with `perLine = 5`, so a record with `m` values takes
`ceil(m/5)` lines; continuation lines start with 8 spaces, e.g. (m = 6)

```
       1    3.3004758000E+001    1.0243310000E+001   -4.1616936000E+001   -1.1885655000E-001   -6.9580994000E+000
           -1.4567515000E-001
```

**`FUN_1402f2fe0`** is the integer twin: `" %7i"` per value, `perLine` = 10 from its callers.

Reading back: the readers (`FUN_140416c90` → `FUN_1404199a0`/`FUN_140419b90`, see
`group2-helpers.md`) read records with `fscanf`, so the line split does not matter. The header
fields line up with the reader modes: fmt 0 ↔ mode 0 (`count default`), fmt 1 ↔ mode 1
(`count k default`), fmt 2 ↔ mode 2, fmt 3 ↔ `FUN_140415590` (`count default k`) or mode 3,
fmt 4 ↔ `FUN_140417c80` readers, fmt 5 ↔ the DAMAGE reader. `%20.10E` keeps 11 significant
digits, so values do not round-trip bit-exactly.

## `FUN_1402f4df0` — integer per-entity writer `(f, name, objNo, int *v, n, m, d)`

`n == 0 || m == 0 || v == NULL || name == NULL` → 0, nothing; `f == NULL` → 2.
* all `n*m` values equal `v[0]` → `"%-6s %7i %7i %7i\n"` (name, objNo, **0**, v[0]); no records;
* else `count` = entities with a component `!= d`; header `"%-6s %7i %7i %7i\n"` (count, d);
  records `" %7i"` index + `FUN_1402f2fe0(f, m, &v[i*m], 10, 0)` (10 values per line,
  continuation lines start with 8 spaces). `d` = 0 for BCC* codes, 1 for MTLGRP.

## `FUN_1402f60c0` — two-array writer `writeNodeElemVarDouble2`

`(f, name, objNo, double *v1, n1, m1, d1, double *v2, n2, m2, d2, fmt)`; used for STRESS, STRAIN
and DAMAGE when the element array has a nodal companion (`v2` = `obj+0x948`/`0x938`/`0x940`,
`n2` = nodes `obj+0x880`, `m2` = `lVar2+0x1660`/`0x1664`/`0x1668`; d1 = d2 = 0.0).

* `n1 == 0 || m1 == 0 || v1 == NULL || name == NULL` → 0, nothing;
  `n2 == 0 || m2 == 0 || v2 == NULL` → 0, **nothing at all** (callers only pick this writer when
  `m2 != 0` and `v2 != NULL`); `f == NULL` → 2.
* Compression of each array separately with `FUN_1402f5360` and the same `fmt` (fmt 2 is never
  used, so: all values equal the first → count 0 and that value).
* Header:

  | fmt | format | fields after name, objNo |
  |---|---|---|
  | 0 | `"%-6s %7i %7i %20.10E %7i %20.10E\n"` | count1, def1, count2, def2 |
  | 3 | `"%-6s %7i %7i %20.10E %7i %7i %20.10E\n"` | count1, def1, m1, count2, def2 |
  | 5 | `"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` | count1, def1, count2, def2, m1 |
  | other | `"KwdWriteUtl::writeNodeElemVarDouble2() - Not supported format\n"` to stdout | |

* Then the records of array 1 (as in `FUN_1402f5c10`, `m1` values), then the records of array 2
  (`m2` values), each only if that array was not uniform.

## `FUN_1402f58d0` — integration-point writer (DAMAGE with `model+0x888 == 3`)

`(f, name, objNo, double *v, n, m, d, ip, fmt)`: data layout `v[(i*ip + p)*m + j]`.
Same NULL/0 guards as `FUN_1402f5c10`. Compression `FUN_1402f55b0` (fmt is always 5 → "all
`n*ip*m` values equal `v[0]`"; else defaults = d and count = entities with any value `!= d`).
Header `"%-6s %7i %7i %20.10E %7i %20.10E %7i %7i\n"` = count, def, **0**, **0.0**, m, ip.
Records: `" %7i\n"` (index alone on its line), then for each of the `ip` points one
`FUN_1402f2e80(f, m, …, 5, 0)` list (own line(s)).

## Integration-point layouts written inline (STRESS, STRAIN)

Codes 0x26 / 0x2b with `model+0x890 == 3` / `model+0x88c == 3` print fixed layouts with
`fprintf`, no compression, every element written; details in STRESS.md and STRAIN.md. Header
`"%-6s %7i %7i %20.10E %7i %20.10E %7i\n"` (STRESS 2D: `"%-6s %7i %7i %20.10E %7i %7i %20.10E %7i\n"`),
then per element `" %7i\n"` and `ip` value lines.

## `FUN_1402a3690` — phase-column gather (VOLFC, VOLFS)

`(model, f, name, obj, objNo, double *v, int mG, char alt)`, `mG = ncomp[0x31]` or `[0x32]`.
`FUN_140398350(model, obj, map, &mL)` builds `map[0..model+0x1320-1]` (−1 = not used): for every
distinct material number in `obj+0x9f8` (per element, scanned in element order, a material is
processed when it differs from the previous element's) whose material record (`FUN_1401d7060`)
has `+0x1cc8 == 1` (mixture), each phase material listed at `+0x1cd0` (`+0x1ccc` entries) is
marked; marked phases get consecutive local columns `0..mL-1` in **global phase order**
(`FUN_14029c870(model+0x1328, matNo)` = phase index). Nothing is done when `obj+0x87c == 0`
(then `map` stays all 0 and `mL` is uninitialised). The writer copies `v[e*mG + c]` to column
`map[c]` of an `n × mL` buffer and calls `FUN_1402f5c10(f, name, objNo, buf, obj+0x884, mL,
0.0, fmt 1, 0)` (`alt` is 0 here; `alt != 0` would use `FUN_1402f7490`, a `"%d  %d\n"` +
`"%15.7E  "` dump). The reader `FUN_1403981c0` (see VOLFC.md) scatters back through the same map.

## Edge/face writers `FUN_1402f6600` (int) and `FUN_1402f6a10` (double)

`(f, name, objNo, obj, v, m, d)`, `d` = 0 / 0.0; entity count `FUN_140165170(obj)` (edges in 2D,
faces in 3D); `m >= 2` → return 1, nothing written. "Valid" entity: 3D always; 2D only if
`FUN_140200e20(obj, e, &n1, &n2)` gives `1 ≤ n1, n2 ≤ obj+0x880`.
* uniform over valid entities (`v[e] == v[0]`) → `"%-6s %7i %7i %7i\n"` / `"%-6s %7i %7i %20.10E\n"`
  with count 0 and `v[0]`;
* else header with count = valid entities whose value `!= d`, and `d`; then for every entity
  with `v[e] != d` a running number `s` (incremented for every such entity, valid or not) and
  * 2D: `" %7i %7i %7i %7i\n"` / `" %7i %7i %7i %20.10E\n"` = s, n1, n2, value;
  * 3D: `FUN_140201210(obj, e, &n1, &n2, &n3, &n4)`, `" %7i %7i %7i %7i %7i %7i\n"` /
    `" %7i %7i %7i %7i %7i %20.10E\n"` = s, n1..n4, value;
  a record whose nodes are outside `1..obj+0x880` is skipped (in 3D the header count still
  includes it, so the count can exceed the number of records).

## `FUN_1402f6e40` — custom variable (CUSVAR)

`(f, name, objNo, varId, obj, d = 0.0)`: `var = FUN_140185e00(obj+0xb58, varId)` (first
`StateVarCustom` with that id); NULL → return 1, nothing. `type = var+0xc`, `nr = var+0x10`,
`m = var+0x14`, data = `var+0x28` vector. Header `"%-6s %7i %7i %7i %7i %20.10E %7i\n"` =
varId, type, count, default, m:
* `type == 0`: type printed 0, count = `nr`, default = d; **all** `nr` rows written as records;
* else: compression as fmt 0 (`count` = rows with a value `!= d`), but the header always prints
  `d` (0.0) as default, **not** the uniform value: a uniform non-zero variable is written as
  `count 0, default 0.0` and reads back as all zeros.
Records `" %7i"` + `FUN_1402f2e80(m values, 5 per line)`.

## Other cases

* 0x00 RZ, `FUN_1402a2db0(model, f, obj, objNo)`: f or obj NULL → 3. Writes only if
  `obj+0x87c != 0` and `obj+0x880 > 0`: `"%-6s %7i %7i\n"` (name, objNo, nodes), then per node
  `" %7i %20.10E %20.10E %20.10E\n"` (3D model, or 2D model with object dimension
  `FUN_140158ce0(obj) == 3`) or `" %7i %20.10E %20.10E\n"` (2D), from `obj+0x8a8` (3 or 2 per node).
* 0x25 ELMCON, `FUN_1402a30c0`: only if `obj+0x87c != 0` and `obj+0x884 != 0`; 3D (or object
  dimension 3): `obj+0x890 == 2` → `"%-6s %7i %7i %7i\n"` (elements, 4) + `" %7i"`×5 lines,
  `== 4` → (elements, 8) + `" %7i"`×9 lines, other element types write nothing; 2D →
  `"%-6s %7i %7i\n"` + `" %7i %7i %7i %7i %7i\n"` (index, 4 nodes) from `int obj+0x9e8`.

## Readers that pair with these writers (shared)

* `FUN_140417280(f, v1, n1, m1, v2, n2, m2, mode, line)` = `KwdReadUtl::readNodeElemVarDouble2`
  (STRESS, STRAIN): uses `line` (already read by the caller) or `fgets`; mode 0
  `sscanf("%i %Flf %i %Flf")` = count1, def1, count2, def2; mode 3 `"%i %Flf %i %i %Flf"` =
  count1, def1, (ignored), count2, def2; other modes print
  `"KwdReadUtl::readNodeElemVarDouble2() - not supported format.\n"` to stdout and convert
  nothing. `sscanf < 1` → 1. Else: **all** `n1*m1` of v1 = def1 (0 when absent); if `v2 != NULL
  && m2 != 0` all `n2*m2` of v2 = def2; `count1 != 0` → `FUN_1404199a0(f, v1, count1, n1, m1)`;
  `count2 != 0` → `FUN_1404199a0(f, v2, count2, n2, m2)` (when v2 is missing, into a scratch
  buffer of `n2*m2` doubles that is discarded). Returns the last status.
* `FUN_140418910` = `readNodeElemVarDouble4` and `FUN_140418e40` (DAMAGE): see DAMAGE.md.
* `FUN_1403981c0` (VOLFC, VOLFS): see VOLFC.md.
