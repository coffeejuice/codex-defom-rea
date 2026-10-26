# Group 2 (object) KEY writer: emission order of `FUN_14029e250`

Source: `DEF_PRE_64.exe` v11.0.2. The KEY writer `FUN_1401c5810(model, path, flags)` stores
`flags` in `model+0x1744` and calls `FUN_1401c5af0` (text KEY; bit 31 of `flags` selects the
other writer `FUN_1401c5d20`). `FUN_1401c5af0` writes the file header and simulation-control
block (`FUN_1401c5a20`, `FUN_1403d4990`), the material blocks, inter-material data, then **for
each object in list order** `FUN_14029e250(model, f, obj, 0, 0)`, then the
`"Inter-Object Data"` block (`FUN_1403c9ad0`). This file lists what one object call emits, in
order; line formats are in the keyword specs.

`FUN_14029e250(model, FILE *f, OBJ *obj, uint outNo, char withHeader)`:

* `obj == NULL` → returns 0, writes nothing.
* `obj+0x3f24 = outNo`; the printed object number `N` is `outNo`, or `obj+8` when `outNo == 0`.
  Every write handler reads `obj+0x3f24` and prints it instead of the stored number when it is
  > 0, so `outNo` renumbers the whole block. `obj+0x3f24` is reset to 0 on every exit path.
* `withHeader` → `FUN_1401c5a20` (file header) first (used when one object is written alone).
* Each `handler(...)` below returns a status; the first nonzero one is returned immediately
  (the rest of the object is not written). Sub-writers called as `FUN_…(model, f, obj, N)`
  have their status **ignored** unless noted.

Unless a condition says otherwise, a step always runs. "2D"/"3D" are the model dimension tests
`FUN_1400015e0`/`FUN_1400015f0`. `flags` is `model+0x1744`.

| # | Emits | Function | Condition / notes |
|---:|---|---|---|
| 1 | `STNOUT` (group 1, id 34) | `FUN_1403d6580` | `flags & 0x1000` |
| 2 | `ELMNOD` (group 1, id 35) | `FUN_1403d6620` | `flags & 0x2000` |
| 3 | comment `*` / `*  Data for Object #     N` / `*` | inline `sprintf` + `fprintf("*\n*  %s\n*\n")` | always (5 spaces after `#`) |
| 4 | `OBJTYP` | `FUN_1402a25a0` | status nonzero → **return 3** |
| 5a | (nothing more) | | `obj+0xc == 7` (empty object) → return 0 |
| 5b | movement block, mesh block, `WINPRS` | `FUN_1402a7c80`, `FUN_1402a9180`, `FUN_1402ab880` | `obj+0xc == 5`: `FUN_1402a7c80`; `FUN_1402a9180` if byte `obj+0x2e05 == 0`; `FUN_1402ab880` if `flags & 0x1000000`; then return 0 |
| 6 | `OBJNAM` | `FUN_1402a2530` | other types |
| 7 | `AVGSTR` | handler `FUN_1402af420` | |
| 8 | `LMTSTR` | handler `FUN_1402af4f0` | |
| 9 | `PENVOL` | inline `"%-6s %7i %20.10E\n"`, value `obj+0x1eb0` | 3D (not 2D) |
| 9′ | `ZSTR`, `PENVOL` | handlers `FUN_1402af5c0`, `FUN_1402b4400` | 2D, instead of 9 |
| 10 | `USRSUB` | inline `"%-6s %7i %7i\n"`, `obj+0x2150` | `obj+0x2150 != 0` |
| 11 | `REFTMP` | handler `FUN_1402af350` | |
| 12 | `TMPLMT` | handler `FUN_1402af6a0` | |
| 13 | `ROTSYM` | inline, 3 lines | 3D |
| 14 | `REFPOS` | inline, mode 1 or 2 | `obj+0x1ec8 != 0`; any other mode than 1/2 → **return 3** |
| 15 | `TRGVOL` | inline `"%-6s %7i %7i %20.10E\n"` | always (negative count is written as 0, see spec) |
| 16 | `ELPSOL` | inline `"%-6s %7i %7i\n"`, `obj+0x1f18` | always |
| 17 | `OTPRNG` | handler `FUN_1402af770` | |
| 18 | movement block (status ignored; also called by the equipment-file writer `FUN_1401c6d00`) (`PRSNAM`, `MOVCTL`, `SPDLMT`, `PRSTRE`, `STROKE`, `FORCE`, `ANGMOV`, `ANGMVY`, `CNTRAX`, `ANGMO2`, `CNTRA2`, `OBJUPD`, `OBJSTP`) | `FUN_1402a7c80` | `!(flags & 0x40000)` |
| 19 | `DIEGEO` | `FUN_1402aaa70` | not 2D and `FUN_140198050(obj, 0)` |
| 20 | geometry: in 3D `FUN_1402a27a0` → `FUN_1402ab160` (curve `DIEGEO` again, `TPLOGY`, `CSPMTX`), then surface `DIEGEO`, `POLYTP`, `DIEPNT`, `DIEEDG` (see DIEGEO.md, CSPMTX.md) | `FUN_1402a26c0` | not 2D: `FUN_140160d90(obj)`; 2D: `FUN_140160d20(obj)` and the geometry from `FUN_1401982c0` has `+0x10 != 0` |
| 21 | `TPLOGY` | `FUN_1402a9f90` | 2D with the same geometry test as 20 (in 3D TPLOGY comes from step 20) |
| 22 | `FLDGEO`, `FLWNET` | `FUN_1402a2b70` with name and `obj+0x3328` / `obj+0x3458` | 3D, each if `FUN_140160e50(obj)` / `FUN_140160eb0(obj)` |
| 23 | node coordinates (`RZ`) | `FUN_1402a2db0` | |
| 24 | `DRZ` | handler `FUN_1402b4f80` | |
| 25 | `DRMESH` (state var code 0x19) | `FUN_1402a4a80(…, 0x19)` | 3D (the code itself writes only in 3D) |
| 26 | `BCCFNC` × list | handler `FUN_1402b4050` per entry of `obj+0x2e68` | ref = (obj, entry index 1..n) |
| 27 | `BCCDEF`, `BCCDFN` | `FUN_1402a4a80(…, 0x12)`, `(…, 0x13)` | |
| 28 | `URZ`, `FRZ` | handlers `FUN_1402b5040`, `FUN_1402b5100` | |
| 29 | `PRZ` | handler `FUN_1402b51c0` | 3D |
| 30 | `BCCTMP`, `BCCTFN` | `FUN_1402a4a80(…, 0x14)`, `(…, 0x15)` | |
| 31 | `NDTMP`, `NDHEAT` | handlers `FUN_1402b4280`, `FUN_1402b5280` | |
| 32 | `NDFLUX` | handler `FUN_1402b5340` | 3D |
| 33 | `TIMERC` | handler `FUN_1402b5400` | `model+0x1680 != 0` and `obj+0x958 != NULL` |
| 34 | `NRECVL` | handler `FUN_1402b54c0` | `model+0x1684 != 0` and `obj+0x960 != NULL` |
| 35 | `USRNOD` | handler `FUN_1402b5580` | |
| 36 | `ELMCON` | `FUN_1402a30c0` | |
| 37 | `MTLGRP` (state var code 0x3d) | `FUN_1402a4a80(…, 0x3d)` | |
| 38 | `STRAIN`, `STNCMP`, `DENSTY`, `DAMAGE`, `STRESS`, `YLDS`, `USRELM` | handlers `FUN_1402b4340`, `FUN_1402b5640`, `FUN_1402b5700`, `FUN_1402b4ec0`, `FUN_1402b4e00`, `FUN_1402b57c0`, `FUN_1402b5880` | in this order |
| 39 | remesh / mesh-generation block (`RMDPTH` … `MGUSER`, order in MGNELM.md) | `FUN_1402a9180` | `!(flags & 0x10000)`; also called by the mesh-settings file writer `FUN_1401c6820` |
| 40 | `FRCSTP`, `FRCNEL`, `FRCMTH`, `CRPTIM` | inline `"%-6s %7i %7i\n"`, `obj+0x1f34`, `+0x1f38`, `+0x1f3c`, `+0x1f40` | always |
| 41 | `VOLFC`, `VOLFS`, `DVOLF` | handlers `FUN_1402b5940`, `FUN_1402b5a00`, `FUN_1402b5ac0` | |
| 42 | `GRAIN` or `GRAIN2` | handler `FUN_1402b5b80` / `FUN_1402b5c40` | `model+0x898 == 0` → GRAIN; `== 1` → GRAIN2 (and inside the writer only if `FUN_1401d7160` returns 2); else neither |
| 43 | `SIZESH`, `TXTODF`, `ERECVL`, `TICF`, `DATOM`, `HDNEST`, `HDNOBJ`, `BCCCRB` | handlers `FUN_1402b5d00`, `FUN_1402b5dc0`, `FUN_1402b5e80`, `FUN_1402b5f40`, `FUN_1402b6000`, `FUN_1402b4c40`, `FUN_1402b60c0`, `FUN_1402b6180` | in this order |
| 44 | `CRBFLX` | handler `FUN_1402b6240` | 3D |
| 45 | `VOTAGE`, `BCCRHT` | handlers `FUN_1402b6300`, `FUN_1402b63c0` | |
| 46 | `RHTFLX` | handler `FUN_1402b6480` | always (no dimension test) |
| 47 | `CSFREQ`, `VOLCRG`, `ZEFI`, `ZMFI`, `CURRNT` | handlers `FUN_1402af830`, `FUN_1402afb10`, `FUN_1402b6540`, `FUN_1402b6600`, `FUN_1402b66c0` | |
| 48 | `WINPRS` × list `obj+0x2eb0` | inline | 2D loop, then a separate 3D loop (neither if the dimension is unset) |
| 49 | (cleanup) | `FUN_1402696f0` | removes entries of list `obj+0x2ee0` without a shape (`FUN_1402632d0` false); after each removal the scan restarts at entry 2 (**the writer modifies the model**) |
| 50 | `WINTMP` × list `obj+0x2ee0` | inline | 2D layout, else (not 2D, including unset) 3D layout |
| 51 | `WINATM` × list `obj+0x2f10` | inline | 2D layout, else (not 2D, including unset) 3D layout |
| 52 | `WINRSE` × list `obj+0x2fd0` | inline | 3D (`FUN_1400015f0`) and 2D layouts; neither if dimension is unset |
| 53 | `LOCTMP`, `LOCATM`, `LOCDEF` | `FUN_1402ac560` | |
| 54 | `ECCDEF`, `ECDEFN`, `ECPRES`, `ECCTMP`, `ECTMFN`, `ECHFLX`, `ECCATM`, `ECATFN`, `ECAFLX`, `ECCRHT`, `ECRHFN`, `ECRFLX` | `FUN_1402abf20` | in this order |
| 55 | `MATAXI` (state var code 0x2e) | `FUN_1402a4a80(…, 0x2e)` | the registry handler `FUN_1402b6780` is never called |
| 56 | `NWEAR` | handler `FUN_1402b4d40` | |
| 57 | `WPAXIS` | `FUN_1402a39e0` | not 3D |
| 57′ | `SYMSUF`, `WPAXIS` | `FUN_1402a3880`, `FUN_1402a39e0` | 3D, instead of 57 |
| 58 | `OPSTOP` | `FUN_1402a4020` | |
| 59 | `DYNARY` | `FUN_1402a41c0` | |
| 60 | `RSEDEF` | `FUN_1402b7140` | status not checked by the caller (and FUN_1402b7140 always returns 0) |
| 61 | per entry of list `obj+0xb58` | `FUN_1402b7310` (→ `FUN_1402a4a80`) | ref = (obj, `FUN_14032ec90(entry)`); NULL entries skipped |

Keywords of group 2 that the KEY writer never emits (each spec gives the evidence): `MOVANG`,
`BCCANG`, `MGSIZM`, `OBJPHS` (also not parsed), and `GEO23`, `GEOSEC`, `MSHSEC`, `CRDSYS`, whose
only writer `FUN_1402a9c80` is reached only from the uncalled exporter `FUN_1401c6920`.
`CSPMTX`, `POLYTP`, `DIEPNT`, `DIEEDG` are written from step 20 and `CUSVAR` from step 61.

The keyword-name lookup `FUN_1402f7720(id)` returns the registry name, printed with `%-6s`
(left-justified, padded to 6). Most lines start `"%-6s %7i"` = name, space, object number
right-justified in 7. Reals are `%20.10E` through the MSVC CRT, which prints a **three-digit
exponent** (`1.0000000000E+000`); a port must reproduce that, not the C99 two-digit form.
