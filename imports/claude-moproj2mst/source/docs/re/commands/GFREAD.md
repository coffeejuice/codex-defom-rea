# GFREAD — load a geometry / mesh file into an object

Shared mechanics: [common.md](common.md). Decompiles: `deform-rea-work/out/p89/gfread.c`
(depth 2) and `out/p89/c/gf2d.c gf3d.c gfpda.c gfunv.c gfnas.c gfigs.c gfdxf.c gfmisc.c`
(readers, depth 1).

## 1. Summary

| | |
|---|---|
| Registry id | 298 |
| Dispatcher | `FUN_140378e00`, `switch (param_3 - 0x11f)` case 0xb |
| Handler | `FUN_1401aa9a0(model, &id, filename)` → `FUN_140243830(loader, filename, obj, &type)` |
| External programs | only for 3D `.PDA`/`.PAT` (`PDA2KEY3`) and 3D `.UNV` (`STB2KEY3`); all other formats are read in-process |

Picks a reader from the file-name extension and stores the result as the object's geometry
(surface) or mesh. 2D/3D is decided by the **object** dimension `obj+0x39c0 == 3`
(`FUN_140158ca0` / `FUN_140158ce0`), not the model's.

## 2. Argument grammar

```
GFREAD <id>          rest of keyword line, FUN_140413de0(f, &id, 0): "%i"
<file name>          next line: fgets(1024) + FUN_140323c50 (trailing ' ', '\r', '\n' removed)
```

* The reader status of the `%i` line **is** tested: EOF / empty line → 2.
* `FUN_14019ccc0(model, id)`: id < 1 → returns 0 → case returns 2. Otherwise finds object `id`
  in `model+0x1380`; if absent, appends empty objects (`obj+8` = running number, `obj+0xc` = 7)
  until the list holds id-1 entries, then appends object `id` and sets `model+0x1378` = list count.
* File-name line at EOF → 2.
* Handler return is ignored; the case returns 0 after a successful parse of both lines.

Example: `GFREAD 3` / `TOPDIE.STL`.

## 3. Handler `FUN_1401aa9a0`

1. `obj = FUN_1401d6ea0(model, id)`; 0 → return 0. Filename pointer null → return 0.
2. `FUN_1401b8560(filename)` (fopen "r" test) fails →
   `"ERROR  : Reading GFREAD\n"` and `"         %s does not exist.\n"` (stdout), return 0.
3. Loader object (`FUN_1402437c0(loader, 0)`): `loader[0]` = 1.0E-5 (`DAT_140522f98`, STL merge
   tolerance), `loader[1]` = 1.0 (`DAT_140509c90`, scale), `loader[2]` = callback = 0.
4. `ok = FUN_140243830(loader, filename, obj, &type)`; prints
   `"INFO   : Loading geometry - Done\n"` or `"INFO   : Loading geometry - Failed\n"`.

### `FUN_140243830`

1. Extension (`FUN_140243370`): the text after a `.` that is 3rd..7th from the end, tried in the
   order: `.` at len-3 (2-char extension), len-4, len-5, len-6, len-7. Only if `len > k`. No dot
   found → fail (returns 0, type stays 0).
2. Type (`FUN_140243500`, `strncmp` prefix, upper or all-lower case only):

| Prefix | Type |
|---|---|
| `KEY`/`key` | 0 |
| `DB`/`db` | 1 |
| `GEO`/`geo` | 2 |
| `STL`/`stl` | 3 |
| `PDA`/`pda`, `PAT`/`pat` | 4 |
| `UNV`/`unv` | 5 |
| `NAS`/`nas` | 10 |
| `IGS`/`igs` | 6 |
| `DXF`/`dxf` | 7 |
| other | -1 → fail |

   Prefix match: `.GEOM` → 2, `.STLX` → 3 etc. Mixed case (`.Stl`) → -1.
3. Object 3D → `FUN_140243a70` (3D table), else `FUN_140244010` (2D table).
4. Success → `obj+0x3a40 = type` (`FUN_140256670`).

### 3D table `FUN_140243a70(loader, file, obj, type)` (result starts at 1)

| Type | Action |
|---|---|
| 0 KEY, 1 DB, 6 IGS, 7 DXF, other | nothing, **success** |
| 2 GEO | `FUN_140246f70(geo, file, &0, 3, 0)` → `FUN_1402484c0` (format below); if ok `FUN_14024bac0(geo, obj, 3)` → `FUN_14024be40` stores it as object geometry; then `FUN_1402059a0(obj, 0)` |
| 3 STL | `FUN_140244fa0(stl, file, &(float)loader[0], &1)` then `FUN_140246ad0(stl, obj)`; then if `loader[1] > 0` and `|loader[1] - 1.0| > 1E-6` scale (`FUN_1401fffa0`) — never with the defaults |
| 4 PDA/PAT | external `PDA2KEY3` (§4); result type 2 → volume mesh (`FUN_14024d650`), else surface (`FUN_14024d4f0`) |
| 5 UNV | external `STB2KEY3` (§4); same storage choice (`FUN_14024ef70` / `FUN_14024ee10`) |
| 10 NAS | in-process (§6.4) |

Volume-mesh storage (type-2 results of PDA/UNV/NAS): `FUN_140202700(obj, nNodes, nElem, k, dim, 0)`
with k = 2 when 4 nodes/element else 4; node coordinates copied to the array at `obj+0x8a0`,
connectivity to `obj+0x9e0`; `FUN_1401fc290(obj)`; `FUN_1401fc730(obj, &1)`.
Surface storage: `FUN_140163340(obj,1)`, `FUN_1402009f0(obj, nNodes, nElem, 0)`, polygon 0 of the
object (`FUN_140198620`) gets nodes (3 doubles) and elements (4 ints), `FUN_1401985e0(obj,1)`.
The volume path first calls the loader callback `FUN_140244e60(loader,1)`; the callback is 0, so
it returns 1.

### 2D table `FUN_140244010(loader, file, obj, type)` (verified in assembly)

| Type | Action | Result |
|---|---|---|
| 0, 1, 3, 4, 5, 10, other | nothing | success |
| 2 GEO | reads twice: `FUN_140246f70(g1, file, &0, 3, 0)` (3D-style reader `FUN_1402484c0`) and if ok `FUN_14024bac0(g1, obj, 2)` (`FUN_14024bb30`, needs per-boundary data the 3D-style reader does not fill, inferred to fail); then `FUN_140246f70(g2, file, &0, 4, obj)` → `FUN_1402478f0(…, 4, obj)` (2D GEO reader, writes into obj) | **always success** (obj ≠ 0); the reader results are discarded |
| 6 IGS, 7 DXF | `FUN_140244200(loader, obj, file, type, 300)` → `FUN_140244500` → IGES `FUN_140409af0(r, file, "", obj, …, 300)` / DXF `FUN_1404093a0(r, file, "", obj, 2, 300)` | success unless the reader returns -1 (other negative codes count as success) |

IGES/DXF readers are in-process; they write a temporary `DEF_IGS.OUT` (prefix `""`, so in the
working directory) and delete it (`FUN_1401b85d0`). On success the object's `+0xbc8` gets the
reader's count (`FUN_14019a650`).

## 4. Files written for external programs (3D PDA/PAT and UNV only)

| | PDA/PAT (`FUN_14024c290`) | UNV (`FUN_14024d8b0`) |
|---|---|---|
| INI | `PDA3.INI` ("wt") | `STB3.INI` ("wt") |
| line 1 `"%s\n"` | the GFREAD file name | same |
| line 2 `"%d\n"` | 1 | 1 |
| line 3 `"%d\n"` | 1 | 1 |
| line 4 `"%15.7f\n"` | `loader[1]` = 1.0 → `      1.0000000` | same |
| command | `"PDA2KEY3 > DEF_PDA3.TMP"` (`sprintf("PDA2KEY3 > %s","DEF_PDA3.TMP")`) | `"STB2KEY3 > DEF_STB3.TMP"` |
| launcher | `FUN_140224130(1, cmd)` (dirKind 3) | same |

INI not opened → fail (nothing launched).

## 5. Launch

`system("\"\"<dir>PDA2KEY3\" > DEF_PDA3.TMP\"")`: `<dir>` = `%DEFORM_PC_PATH%` + `3D\` (or `..\3D\`).
The install has `3D\PDA2KEY3.com` and `3D\STB2KEY3.com`, so the `.COM` probe finds them and the
un-suffixed name is used (cmd resolves the extension). Synchronous, exit code ignored. Success =
`SCRATCH.KEY` readable afterwards.

Order after the launch: delete `PDA3.INI`, delete `DEF_PDA3.TMP`; `SCRATCH.KEY` not readable →
fail; read mode from `PDA3_MESH.TMP` (`fopen "rt"`, `fscanf "%d"`; file missing/unopenable →
mode 1), delete `PDA3_MESH.TMP`; parse `SCRATCH.KEY`; delete `SCRATCH.KEY`.
(UNV: `STB3.INI`, `DEF_STB3.TMP`, `STB3_MESH.TMP`, `SCRATCH.KEY`.)

## 6. Read-back / in-process formats

### 6.1 SCRATCH.KEY (`FUN_14024c4b0` PDA, `FUN_14024dad0` UNV — identical code)

Lines are read with `FUN_1401b83d0(buf, 500, f)` (line read, returns length; 0 at EOF).

* mode 1 (surface): skip to the first line that **starts with** `DIEGEO`;
  `sscanf("%*s %*d %*d %d %d\n")` → nNodes, nElem.
* mode 2 (volume): skip to a line starting with `RZ`; `sscanf("%*s %*d %d\n")` → nNodes.
* keyword not found → fail.
* nNodes must be 1 ≤ n < 1.0E20 (`DAT_14051f1b0`) else fail (flag). Nodes: n × `fscanf("%*d %f %f %f\n")`
  (floats widened to double); EOF → fail.
* mode 2: skip to a line starting with `ELMCON`; `sscanf("%*s %*d %d %d\n")` → nElem,
  nodes-per-element (not found → return 0). mode 1: nodes-per-element = 4.
* Elements: npe == 4: `fscanf("%*d %d %d %d %d\n")`, else `fscanf("%*d %d %d %d %d %d %d %d %d\n")`.

### 6.2 3D GEO (`FUN_1402484c0`, after the check `FUN_1402488a0(file)`)

`fopen "rt"`; `fscanf("%d\n")` nNodes (1 ≤ n < `DAT_14051f1a8`); nNodes × `fscanf("%*d %lf %lf %lf\n")`
(fewer than 3 fields → fail); `fscanf("%d\n")` nElem; nElem × `fscanf("%*d %d %d %d %d\n")`
(fewer than 4 → fail). Stored as surface (4 ints per facet).

### 6.3 STL (`FUN_140244fa0`)

In-process ASCII/binary reader (`FUN_140245b90` detects, `FUN_140245cc0` / `FUN_140245ee0` read),
vertex merge with tolerance `(float)1E-5`, triangles with repeated nodes dropped. If any bounding
box extent > 1.0E10: prints `"\tRead file with %d points and %d triangles\n"` and the "Limits
greater than max tolerance of 1E10…" text, fails. With the flag set (always here) it writes
`DEF_STL.OUT` ("wt") and deletes it at once. Stored by `FUN_140246ad0` (3 node ids + first id
repeated as 4th). In 2D the store prints `"*** _DEF_STL::setDataToObjectGeo() ***  not in 2D\n"`
(unreachable from GFREAD: 2D never calls it).

### 6.4 NASTRAN (`FUN_14024f1d0`)

`FUN_140250280` scans tokens until `DONE`: `CTETRA` → (mode 2, 4 nodes), `CTRIA3` → (1, 3),
`CHEXA` → (4, 8); none → `"*** Supported mesh type not found           ***\n"` + 3 more lines and
**the reader still returns 1** with no data. Readers: mode 1 `FUN_14024f270` (fixed 8-column
fields; `GRID` fields 4–6 → x,y,z by `atof`; `CTRIA3` fields 4–6 → node ids, the first id pushed
again as 4th; `$` lines skipped), mode 2 `FUN_14024f620` (`FUN_14024fa50`, fallback `FUN_14024f6a0`),
mode 4 `FUN_14024fe90`. Storage in `FUN_140243a70`: mode 2 → volume (`FUN_140251d60`); otherwise
`FUN_140251be0` (surface, mode 1 only, result discarded) **then** `FUN_140251d60` (volume, modes
2/4 only) decides the result — so a CTRIA3 file stores the surface but GFREAD reports "Failed".

## 7. Messages

| Text (stdout) | When |
|---|---|
| `ERROR  : Reading GFREAD\n` + `         %s does not exist.\n` | file not readable |
| `INFO   : Loading geometry - Done\n` / `- Failed\n` | after every load attempt |
| `*** failed to open file %s  ***\n` | NAS type scan cannot open the file |
| STL bounds text, NAS "Supported mesh type" text | see §6 |

Case return: 2 on bad `id` line / id < 1 / missing file-name line; else 0.

## 8. Helper functions

| Address | Role |
|---|---|
| `FUN_1401aa9a0` | handler |
| `FUN_14019ccc0` | find-or-create object |
| `FUN_1402437c0` / `FUN_140243810` | loader ctor/dtor (`loader[0..2]`) |
| `FUN_140243830` | extension → type → 2D/3D table; sets `obj+0x3a40` |
| `FUN_140243370` / `FUN_140243500` | extension extraction / type code |
| `FUN_140244010` / `FUN_140243a70` | 2D / 3D dispatch |
| `FUN_140246f70`, `FUN_1402484c0`, `FUN_1402478f0` | GEO readers (3D style / 2D) |
| `FUN_14024bac0`, `FUN_14024be40`, `FUN_14024bb30` | GEO store 3D / 2D |
| `FUN_140244fa0`, `FUN_140246ad0` | STL read / store |
| `FUN_14024c290`, `FUN_14024c4b0`, `FUN_14024d650`, `FUN_14024d4f0` | PDA: run PDA2KEY3, parse SCRATCH.KEY, store volume / surface |
| `FUN_14024d8b0`, `FUN_14024dad0`, `FUN_14024ef70`, `FUN_14024ee10` | UNV equivalents (STB2KEY3) |
| `FUN_14024f1d0`, `FUN_140250280`, `FUN_140251be0`, `FUN_140251d60` | NASTRAN |
| `FUN_140244200`, `FUN_140244500`, `FUN_140409af0`, `FUN_1404093a0` | 2D IGES / DXF |
| `FUN_1401fffa0` | scale geometry (not reached with defaults) |
| `FUN_140256670` / `FUN_140256690` | set / get `obj+0x3a40` (geometry file type) |

## 9. Open questions

Resolved by the follow-up specs: STL, 3D/2D GEO (incl. `FUN_1402488a0`, `FUN_1402059a0`, and
the 2D first pass, which never changes the object) and NASTRAN in
[GFREAD_STL_GEO_NAS.md](GFREAD_STL_GEO_NAS.md); IGES in [IGES_READER.md](IGES_READER.md); DXF in
[DXF_READER.md](DXF_READER.md). Their own "open questions" sections list what remains
(boundary-face extraction `FL3_FNDBND` for CTETRA, untested edge cases).
