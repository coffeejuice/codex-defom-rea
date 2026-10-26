# In-process DXF / IGES readers, DEF_IGS and FIXGEO

Decompiles (Ghidra copy `ghidra_p89d`): `deform-rea-work/out/p89/d/chain.c` (GFREAD → reader
chain), `rd.c` (reader classes), `ext.c` (DEF_IGS / FIXGEO launches), `ext2.c` (DXF parser,
extension map, GEO readers), `gp.c` (geometry-primitive files), `fx.c` (primitive builder);
call trees `ct1.txt`, `ct3.txt` (script `d/gs/CallTree.java`). Launcher: [common.md](common.md).

## 1. Summary and reachability

| Function | What | Reached in DEF_PRE_64 from |
|---|---|---|
| `FUN_140409af0` (`Uic2CObjectIgesFile`, mode 300) | IGES file → 2D object geometry, in-process (Fortran IGES parser + `DEF_IGS.OUT`) | `GFREAD` (2D object, extension `IGS`/`igs`) |
| `FUN_1404093a0` (`Uic2CObjectDxfFile`, mode 300) | DXF file → 2D object geometry, in-process | `GFREAD` (2D object, extension `DXF`/`dxf`) |
| `FUN_14027ece0` | FIXGEO cleanup of a 2D profile: `FIXGEO.EXE > DEF_GEO.TMP` | CNVT3D geometry (`FUN_140153c00`/`FUN_140154270` → `FUN_1402ba500` → `FUN_1402ce190` → `FUN_1402d7cd0` → `FUN_1402da250`), `FUN_140147f90`, and `FUN_14041e270` ← `FUN_1402778c0` (← `FUN_140153c00`, `FUN_14021bbc0`, `FUN_140278350`) |
| `FUN_1402e4340` | external IGES: `DEF_IGS.EXE < DEF_IGS.INP > DEF_IGS_TEMP.OUT` + FIXGEO | **dead**: only from `FUN_1402e11d0`/`FUN_1402e2330`/`FUN_1402e3200` ← GEOPRIM_EXTRUDE/REVOLVE parsers ← `FUN_1402b9160` |
| `FUN_1402e6c10` | DXF (in-process parse) + FIXGEO | **dead** (same callers) |
| `FUN_1402e7930` | FIXGEO on a `.GEO` file | **dead** (same callers) |
| `FUN_1402b9160` | geometry-primitive file reader (`GEOPRIM_CUBE`, `_CYLINDER`, `_ROLL`, `_EXTRUDE`, `_REVOLVE`, `$END`, `DONE`) | no caller: no call, and no absolute pointer to it in the image (`d/findptr.py`) |
| `FUN_1401980f0`, `FUN_140198170` | object-level import/export by type (IGES/DXF/GEO) | no caller, no pointer |

None of the main-menu options reaches these readers (menu 2 options 3–8 use the external
`DEF_CVT3`, see [FILE_INPUT_MENU.md](FILE_INPUT_MENU.md)). The only keyword path is
**`GFREAD`** (registry 298, `FUN_1401aa9a0`), and FIXGEO is also run by **`CNVT3D`**.

## 2. GFREAD dispatch to the readers

`FUN_1401aa9a0` → `FUN_140243830(?, file, obj, &type)`:

1. `FUN_140243370(file, ext)`: the extension is the text after a `.` found at position
   len-3, len-4, len-5, len-6 or len-7 (first match in that order); no dot → fail (0).
2. `FUN_140243500(ext)` (prefix compare, upper or lower case only):
   `KEY`/`key` 0, `DB`/`db` 1, `GEO`/`geo` 2, `STL` 3, `PDA` 4, `PAT` 4, `UNV` 5, `NAS` 10,
   `IGS` 6, `DXF` 7, anything else -1 (→ fail). `IGES` does **not** match `IGS` (→ -1).
3. `FUN_140158ca0(obj)` (3D flag, inferred) false → `FUN_140244010(?, file, obj, type)`;
   true → `FUN_140243a70` (3D path; does not reach the DXF/IGES classes).
4. `FUN_140244010`: type 0/1 → nothing (returns 1); 2 → GEO reader (`FUN_140246f70`,
   `FUN_14024bac0`); 6/7 → `FUN_140244200(?, obj, file, type, 300)` → `FUN_140244500`.
5. `FUN_140244500` constructs both `Uic2CObjectIgesFile` (`FUN_1404099d0`) and
   `Uic2CObjectDxfFile` (`FUN_1404092b0`) on the stack and calls
   * type 6: `FUN_140409af0(iges, file, "", obj, &x, 2, 0, 1, 300)`; on 0:
     `FUN_14019a650(obj, FUN_1402566b0(iges))`;
   * type 7: `FUN_1404093a0(dxf, file, "", obj, 2, 300)`; on 0:
     `FUN_14019a650(obj, FUN_1402566c0(dxf))`.
   Result -1 → GFREAD failure; anything else is success for `FUN_140244200`
   (`!= -1`). On success `FUN_140256670(obj, type)` records the type.

## 3. IGES reader `FUN_140409af0` (mode 300)

Arguments: file name, output prefix (`""`), object, … . Errors: object NULL -1; name NULL or
empty -2; `FUN_1401b8560(name)` (cannot open for read) -3; prefix NULL -12.

1. `FUN_140197bc0(obj)` (clear geometry, inferred).
2. `out = prefix + "DEF_IGS.OUT"` (= `DEF_IGS.OUT` in the working directory);
   `FUN_1401b8420(out)`.
3. Global tables `DAT_14070c050` (8·`DAT_140628568` bytes) and `DAT_14070c058`
   (4·`DAT_140628554`) are (re)allocated and zeroed.
4. Pass 1: `FUN_14040c220(name, &DAT_14070bc20, 1, 0)` — copies the name, calls the Fortran IGES
   routines (`IgesInterface.f`/`IgesCore.f`, `FUN_1404570f0`, `FUN_140457196`; the Fortran side
   names `DEF_IGS.OUT` at `FUN_140457312`). Errors: -4 (first routine), -5/-6/-7 (second routine
   status 1/2/other), -8 (no entities). Non-zero → tables freed, return that code.
5. On 0: `DEF_IGS.OUT` deleted; the object's boundary container is fetched/created
   (`FUN_1401982c0`, `FUN_140198270`); point table size `n = DAT_14070c024 · FUN_140276090(2)`
   (or `DAT_14070c020 · …` when `DAT_14070c034 + DAT_14070c030 > 0`).
   * `n >= 1`: pass 2 `FUN_14040c220(name, ctx, 2, 500)`; result 200 or 0 → the points are
     stored in the object (`FUN_140276030`, `FUN_14040cde0`), counts set with `FUN_140198230`,
     `FUN_14019a650`, `FUN_14040ce60` (number of boundaries = `DAT_14070c024`, at least 1 when
     the code was 200), return 0; other codes → tables freed, return that code.
   * `n < 1` (no table): fallback text route — `FUN_140455e60(tmp, "DEF_IGS.OUT")` reads
     `DEF_IGS.OUT` (§3.1); -1 → -14 (`-0xe`), 0 → `FUN_14040b100(iges, obj, 2)` converts it,
     other → -15. `DEF_IGS.OUT` is deleted again.

### 3.1 `DEF_IGS.OUT` format read by `FUN_140455e60` ("rt")

```
"%d\n"                       nLoops
per loop:  "%d %d\n"         (ignored, nSeg)
  per seg: "%d %s %s %s %s %s %s\n"  (idx, type, x1, y1, x2, y2, a)   values via atof
```
Points (x1,y1) and (x2,y2) are added to a point list (`FUN_1404564c0`, duplicates merged);
`type == 2.0` → arc: angle `a·π/180` (|·| < 1e-6 → 2π) and a mid point from
`FUN_14046b2d0`; segment stored by `FUN_1404565c0(tmp, (int)type, p1-1, …)`. Then
`FUN_140455150` (twice), `FUN_1404557a0`, `FUN_140455aa0(tmp,0,1)`, `FUN_140456720(tmp,1)`.

## 4. DXF reader `FUN_1404093a0` (mode 300)

Errors: object NULL -1; name NULL/empty -2; not readable -3.

1. `FUN_140197bc0(obj)`.
2. `FUN_140422ab0(name, &nEnt, &nPts, &data)`:
   * -100 name NULL, -200 empty, -300 `FUN_140428b10(name)` false, -1 no work buffer,
     -400 `fopen(name, "rb")` failed;
   * `FUN_140422f70(f, buf)`: reads **pairs of lines** (`fgets` 512): group code, value. The
     very first group-code line equal to `"AutoCAD Binary DXF"` → -10000 (`0xffffd8f0`, binary
     DXF rejected). A group code `"0"` makes the value line an entity/section name, looked up in
     the 10-entry table at `0x14062bdb0` (name, handler):

     | Name | Handler |
     |---|---|
     | `SECTION` | `FUN_1404245b0` |
     | `ENDSEC` | `FUN_1404246c0` |
     | `LINE` | `FUN_140424700` |
     | `POLYLINE` | `FUN_140424da0` |
     | `LWPOLYLINE` | `FUN_1404251e0` |
     | `VERTEX` | `FUN_1404259c0` |
     | `SEQEND` | `FUN_140425cf0` |
     | `ARC` | `FUN_140425d50` |
     | `CIRCLE` | `FUN_1404262e0` |
     | `SPLINE` | `FUN_140426810` |

     The loop ends when a handler returns 0 for a matched name (presumably `EOF`/end of
     ENTITIES, inferred) or at end of file. Other entity names are skipped.
   * then `FUN_1404231c0` builds the point/segment arrays; a tolerance
     `0.01 · size` (`FUN_14027ba50`) is set with `FUN_140428f70`; work buffer freed
     (`FUN_140424390`).
3. On 0: geometry container (`FUN_1404099c0` or `FUN_140409930(obj)`), `FUN_1404098d0`,
   `FUN_1404099a0(dxf, …)`, then conversion through an `Uic2CObjectIgesFile` helper:
   `FUN_14040ba80(iges, obj, geo, nEnt, …, 700)`; non-zero → the DXF data is freed
   (`FUN_140422d90`). The boundary count is stored with `FUN_140409970`.

No external program and no scratch file are used by the DXF path.

## 5. External programs

### 5.1 DEF_IGS (`FUN_1402e4340(geo, igesFile)`, dead in DEF_PRE_64)

1. delete `DEF_IGS.INP`, `DEF_IGS.OUT`;
2. `DEF_IGS.INP` ("wt"; open failure → return 0):
   ```
   "%s\n\n"   igesFile
   "%d\n\n"   50
   "%lf\n\n"  3e-05            (DAT_140541490 → "0.000030")
   ```
3. `sprintf("DEF_IGS.EXE < %s > %s", "DEF_IGS.INP", "DEF_IGS_TEMP.OUT")`,
   `FUN_1402240d0` (dirKind 0, waits);
4. `DEF_IGS.OUT` missing → delete `DEF_IGS.INP`, `DEF_IGS_TEMP.OUT`, return 0;
5. delete `DEF_IGES_TEMP_GEO.GEO`; `FUN_1402e7320("DEF_IGS.OUT", "DEF_IGES_TEMP_GEO.GEO")`:
   for each line of `DEF_IGS.OUT` (fgets 256) beginning with `DIEGEO`: `sscanf("%s%d%d%d")`
   (keyword, a, b, n) and write `"%5d\n%5d %5d\n"` (3, n, 1) followed by the next n lines copied
   verbatim (several DIEGEO blocks are concatenated);
6. FIXGEO as in §5.3 on `DEF_IGES_TEMP_GEO.GEO`; result = `FUN_1402e74d0(geo, "PNTGEO.GEO",
   &1)`;
7. delete `DEF_IGS.INP`, `DEF_IGS_TEMP.OUT`, `DEF_IGES_TEMP_GEO.GEO`, `DEF_IGS_TEMP.OUT`,
   `FIXGEO.INI`, `PNTGEO.GEO`, `NEWGEO.GEO`, `FIXGEO_TEMP.OUT`. `DEF_IGS.OUT` is left.

### 5.2 DXF with FIXGEO (`FUN_1402e6c10`, dead)

`FUN_140422ab0` as in §4; scale `geo+0x18 = size / 100000`; when the parsed geometry kind
(`local_318`) is 2: `DXF_TEMP.GEO` ("wt"): `"%5d\n%5d %5d\n"` (3, n, 1), then per segment
`"%d %8d %16lf %16lf %16lf %16lf %16lf\n"` (i+1, type, v1, v2, v3, v4, v5; for type 2 the last
value is `v5·180/3.1415926`); FIXGEO §5.3 on `DXF_TEMP.GEO`; `FUN_1402e74d0(geo,
"PNTGEO.GEO", &1)`; delete `FIXGEO.INI`, `PNTGEO.GEO`, `NEWGEO.GEO`, `DXF_TEMP.GEO`. Then the
point order is reversed when the signed area (`FUN_140309550`) < -1e-6.

### 5.3 FIXGEO, generic form (`FUN_1402e4340`, `FUN_1402e6c10`, `FUN_1402e7930`; dead)

1. delete `FIXGEO.INI`, `PNTGEO.GEO`, `NEWGEO.GEO`;
2. `FIXGEO.INI` ("wt"):
   ```
   "%s\n"            input .GEO file
   "%f  %f  %f\n"    0.001, 0.3, 5.233      (FUN_1402e7930: its own three arguments)
   "%d  %d\n"        0, 0
   ```
3. `sprintf("FIXGEO.EXE > %s", "FIXGEO_TEMP.OUT")`, `FUN_1402240d0` (dirKind 0);
4. read `PNTGEO.GEO` with `FUN_1402e74d0(geo, "PNTGEO.GEO", &1)` ("rt", token reader
   `FUN_140411ad0`): first token must be `1` (else fail), then `"%d"` count, then count lines
   `"%*d %lf %lf"`; then `FUN_1402e7cf0`; orientation fix (area < -1e-6 → reverse);
5. delete `FIXGEO.INI`, `PNTGEO.GEO`, `NEWGEO.GEO` (+ `FIXGEO_TEMP.OUT` in §5.1).

### 5.4 FIXGEO for profiles (`FUN_14027ece0(geo, par, &out, delPnt, recount)`, reachable)

1. delete `DEF_GEO.TMP`; `FUN_14027cc00(geo, "FIXGEO_BUF.GEO")` writes the profile (non-zero →
   return -1);
2. `FIXGEO.INI` ("wt"; failure → -2), one `fprintf`:
   `"%s\n%f  %f  %f\n%d  %d\n%d  %d\n"` with `"FIXGEO_BUF.GEO"`,
   `par[0]` (when int `par+0x20 == 0`) else `par[1]`, `par[2]·0.5·π/180`, `par[3]·π/180`,
   byte `par+0x28`, byte `par+0x29`, int `par+0x20`, int `par+0x24`
   (π/180 = `0.017453292519943295`, `DAT_140522fd0`);
3. `sprintf("%s > %s", "FIXGEO.EXE", "DEF_GEO.TMP")`, **`FUN_140224100` (dirKind 2 → `2D\`
   folder)**; the install has `2D\FIXGEO.exe` and root `FIXGEO.exe`;
4. delete `FIXGEO.INI`, `PNTGEO.GEO` (only when `delPnt`), `FIXGEO_BUF.GEO`, `DEF_GEO.TMP`;
5. `NEWGEO.GEO` missing → return 0 (profile unchanged); else `FUN_14027ce90(geo, "NEWGEO.GEO")`
   reads it: ok → 1 (and `out = FUN_14027efa0(geo)` when `recount`), error → -3.
   **`NEWGEO.GEO` is not deleted.**

In CNVT3D (`FUN_1402da250`) the call is `FUN_14027ece0(tmpGeo, par, &n, 1, 0)` and happens only
when a profile point had to be moved (`local_1c8`); `par` comes from `FUN_140220390`.

## 6. Helper functions

| Address | Role |
|---|---|
| `FUN_140243830`, `FUN_140243930` | GFREAD import by extension (2D / 3D-only variant) |
| `FUN_140243370`, `FUN_140243500` | extension extraction / type code |
| `FUN_140244010`, `FUN_140244200`, `FUN_140244500`, `FUN_1402442f0` | 2D import dispatch (mode 300 read, mode 400 variant unused here) |
| `FUN_140244c20`, `FUN_140244750`, `FUN_140244830` | export counterparts (`FUN_14040ab10` IGES write, `FUN_1404097a0` DXF write) |
| `FUN_1404099d0` / `FUN_1404092b0` | `Uic2CObjectIgesFile` / `Uic2CObjectDxfFile` ctors (vtables `0x140564680`, `0x140563800`) |
| `FUN_140409af0`, `FUN_14040c220`, `FUN_140455e60`, `FUN_14040b100` | IGES read |
| `FUN_1404093a0`, `FUN_140422ab0`, `FUN_140422f70`, `FUN_1404231c0`, `FUN_14040ba80` | DXF read |
| `FUN_1402e4340`, `FUN_1402e7320` | DEF_IGS route (dead) |
| `FUN_1402e6c10`, `FUN_1402e7930`, `FUN_1402e74d0` | DXF/GEO + FIXGEO (dead) |
| `FUN_14027ece0`, `FUN_14027cc00`, `FUN_14027ce90` | FIXGEO for profiles (reachable) |
| `FUN_1402b9160` | GEOPRIM file reader (dead) |

## 7. Open questions

Resolved by [IGES_READER.md](IGES_READER.md) and [DXF_READER.md](DXF_READER.md), which also
correct this file: `FUN_1401b8420` opens `DEF_IGS.OUT` with "at" (creates an empty file; deleted
only when pass 1 succeeds, never written on the GFREAD path); `FUN_140455e60` is reached only
when the segment table cannot be allocated; any pass-1 failure returns -4 (-5/-6/-7 cannot
occur); `FUN_140197bc0` is an undo snapshot, not a clear; the DXF parse ends as a success when a
handler returns 0. The items below are kept for the record.

* The exact interplay of `DEF_IGS.OUT` in `FUN_140409af0`: it is deleted after pass 1 and read
  in the `n < 1` fallback; whether the Fortran pass 2 rewrites it was not followed into the
  Fortran code (`FUN_1404570f0`, `FUN_140457196`, `FUN_140457312`). Externally: no
  `DEF_IGS.OUT` should remain after a successful GFREAD of an `.IGS` file.
* `FUN_1401b8420` (called on `DEF_IGS.OUT` before pass 1) not decompiled (delete or test).
* DXF entity handlers (`0x1404245b0`…`0x140426810`) and the unit/tolerance logic are not
  derived; which group codes each handler consumes is open.
* `FUN_140428b10(name)` (-300 check) not decompiled (existence or extension test).
