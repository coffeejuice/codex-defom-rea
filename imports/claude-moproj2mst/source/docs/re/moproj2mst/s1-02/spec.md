# s1-02 — MST framework, shared emitters, base operation writer

Binary: `DEF_GUI_MO_64.exe` (copy in `gui_mo/`), image base 0x140000000. All addresses are VAs.
Evidence files in this folder: `a1s.c` … `a12s.c` (Ghidra decompiles with .rdata literals annotated inline),
`asm1.txt` … `asm3.txt` (linear disassembly used to fix stack-passed arguments), `COG_P1_B1.MST.copy` and
`COG_P1_B1.moproj.copy` (copies of the reference project, used for the check in §9).

Notation: `ts` = the `QTextStream`. `<<"…"` = `QTextStream::operator<<(const char*)`, `<<i` = `operator<<(int)`,
`<<d` = `operator<<(double)`, `<<S` = `operator<<(const QString&)`, `ENDL` = `endl` (writes one `\n`, then flushes).
Every literal is quoted with its .rdata VA and its exact length in bytes. `\t` is byte 0x09.
The 75-byte rule line `*-------------------------------------------------------------------------*`
('*', 73×'-', '*') is written `RULE75`, and the 75-byte blank box line
`*                                                                         *` ('*', 73×' ', '*') is written `BOX75`.

## 1. Stream, file and number formatting

### 1.1 File
* Opened with `QFile(path).open(QIODevice::WriteOnly)`: flags value 2 (`140004a60(&f,2)`). Text mode (0x10) is
  not set, so line ends are bare LF (0x0A), as the reference MST confirms. In Qt 4, WriteOnly without Append
  truncates the file.
* If `open` fails, the writer returns false and writes nothing (no message box at this level).
* `QFile::close()` is called **before** `~QTextStream`. Text still in the stream buffer at close is lost. Every
  writer ends its last line with `ENDL` (which flushes), so in practice nothing is lost.
* No BOM is written (Qt 4 QTextStream default).

### 1.2 Codec (byte encoding)
* The writers never call `QTextStream::setCodec`, so Qt 4 uses `QTextCodec::codecForLocale()`.
* At start-up, `1400024f0` (not licence code; it sets plugin paths and codecs) reads the language global
  `0x143d79718` (int, set by `1403b5520`) and, when the codec exists, calls
  `setCodecForLocale` + `setCodecForTr` + `setCodecForCStrings` with: 1→"Shift-JIS", 2→"Big5", 3→"KOI8-R",
  4→"EUC-KR", 7→"GBK". For any other value nothing is set, and the locale codec is the Windows "System"
  codec, i.e. the ANSI code page (`GetACP()`, `WideCharToMultiByte(CP_ACP)`, unmappable characters become '?').
* Literal `const char*` writes: Qt 4 `operator<<(const char*)` writes `QLatin1String(s)`. Every literal in this
  closure is ASCII, so the codec only matters for QString data (title, names, paths).
* **Port rule**: encode every QString through one function `encode_locale(QString)`, CP_ACP by default.
  Add an option for the five language codecs. ASCII-only projects are unaffected.

### 1.3 Numbers (Qt 4.x QTextStream)
QTextStream state is never changed in this closure: no `setRealNumberPrecision`, `setRealNumberNotation`,
`setFieldWidth`, `setNumberFlags` or `setLocale` call. `QLocale::setDefault` is not imported. Defaults apply:
SmartNotation, precision 6, field width 0, number flags 0, locale `QLocale::c()`.
* `<<i` (int): plain decimal, `-` sign for negatives, no padding, no grouping (in C locale the ThousandsGroup flag
  is not set).
* `<<d` (double): `QLocalePrivate::doubleToString(..., precision 6, DFSignificantDigits, flags 0)`. This equals
  C `printf("%.6g")` with correct rounding:
  - 6 significant digits, trailing zeros and a trailing '.' removed (PMChopTrailingZeros);
  - exponent form when decimal exponent < -4 or >= 6; mantissa as above, then `e`, then a sign that is always
    present (`+`/`-`), then at least 2 exponent digits (`1e-05`, `1e+09`, `1.23457e+15`, `1e+100`);
  - decimal point '.', no grouping; `-0.0` → `0` (no sign: `doubleToString` drops the minus when the value is
    zero, `QtCore4_64.dll` 4.8.4 `0x6704b550`; corrected by P04, the earlier text said `-0`); `0.0` → `0`;
  - inf → `inf`, -inf → `-inf`, NaN → `nan` (Qt spellings, not the MSVC ones; confirmed in the same function).
  - Not quite `%.6g`: an exact tie on an integer ≥ 1e6 keeps its trailing zeros (`1000005` → `1.00000e+06`), from
    qdtoa's small-integer path. The port is `apps/moproj2mst/src/qt4/{dtoa,number}.cpp`.
  Corpus evidence: `0.0001`, `1e-05`, `1e+09`, `315.8`, `55.55`, `0.001`, `2.46`. The decimal point is '.' on the
  author's machine, which confirms a C locale and not the system locale.
* `QString::number(int, 10)` (used for "CYCL<n>" and the material id strings) has the same format as `<<i`.
* `QString::arg(int, fieldWidth, base, fillChar)` is used for file names: `%2` with width 5 and fill '0' (see §6).

### 1.4 Path helpers used by the emitters (to port in s1-06)
* `REL(p)` = `QDir(QFileInfo(dbPath).absolutePath()).relativeFilePath(p)`, where `dbPath` is the writer argument
  (§3.2). Qt 4 algorithm: both sides go through `cleanPath`. If either side is relative, return the cleaned `p`
  unchanged. Otherwise compare drive and components case-insensitively (Windows) and build a `../` path with '/'
  separators. A null `p` stays null.
* `CLEAN(p)` = `QDir::cleanPath`. Qt 4 on Windows: first replaces '\\' with '/', then collapses `//`, `./` and
  `../`, and drops a trailing '/'. A null or empty input is returned unchanged.
* `NATIVE(p)` = `QDir::convertSeparators` ('/' → '\\').
* `SEP` = `QDir::separator()` = '\\'.
* `FILENAME(p)` = `QFileInfo(p).fileName()`, `CBASE(p)` = `QFileInfo(p).completeBaseName()`,
  `ABSDIR(p)` = `QFileInfo(p).absolutePath()`, `PATH(p)` = `QFileInfo(p).path()`.

## 2. Entry points

### 2.1 `1400a20d0` — `bool Project::saveMst(Project* prj)` (File > Save calls it from `1400980d0`)
```
r = 1401313c0(prj);                          // main .MST
sub = 1400a7390(prj);                        // current sub-project = list[prj+0x50]
if (sub && (sub->state(+0x18)==2 || ==3)) {  // 140010330 / 140010360
    1400ba650(sub);  // DOE: writes <DoeStudies>/<n>/Control/<name>.MST.TEMP and more (s1-05)
    1400ba800(sub);  // DOE dir structure, out of scope (file-system side effects)
}
return r;                                    // only the main-MST result is returned
```
In `1400980d0` the order is: `1400983c0(prj,0)` (KEY save, out of scope) → `1400982e0` (object hooks, out of
scope) → `1400a20d0` → `LOCAL_REMESH.DAT` handling (out of scope).

### 2.2 `1400a50a0` — `QString mstPath(Project* prj, QString* out, bool full)`
```
base = full ? QString(prj+0x18)                 // 14008a4b0: copy of the .moproj full path
            : CBASE(prj+0x18);                  // 14008a500
*out = 140818d50(base, ".MST" /*142140abc, 4*/);
// 140818d50(p, ext) = CLEAN( PATH(p) + SEP + CBASE(p) + '.' + 140818e20(ext) )
// 140818e20(ext): null if empty, else ext with one leading '.' removed
```
For `C:/x/COG_P1_B1.moproj` the result is `C:/x/COG_P1_B1.MST`. The DLL's default output path follows the same
rule.

### 2.3 `1401313c0` — `bool writeMainMst(Project* prj)`
```
if (!prj) return false;
if (1400a7360(prj) < 1) return false;           // size of QList<SubProject*> at prj+0x58
path = 1400a50a0(prj, true);
QFile f(path); if (!f.open(WriteOnly)) return false;
140131390();                                     // clear g_objMaskCur (0x143d80288) and g_objMaskPrev (0x143d80290)
QTextStream ts(&f);
140131680(ts, prj);  // header
1401318b0(ts, prj);  // main-project block
140131df0(ts, prj, 0);  // operation walk over sub-project 0
140131d10(ts, prj);  // trailer
f.close(); return true;
```
The plan's `M2M_E_NO_OPERATIONS` maps to "no sub-project". With a sub-project that holds no operations the
original still writes header + main block + trailer and returns true.

### 2.4 `140131530` — `bool writeSubMst(Project* prj, int subIdx, const QString& path)` (.MST.TEMP, DOE)
Same as 2.3 except: the path is given by the caller; there is no `1400a50a0` call; the block is `140131a50(ts,prj,subIdx)`;
the walk is `140131df0(ts,prj,subIdx)`. Caller `1400ba650`: `path = CLEAN(1400ba350(sub) + SEP + 1400a51e0(prj))`,
where `1400ba350(sub)` = `CLEAN(1400ba000(sub) + SEP + "Control")` and `1400ba000(sub)` =
`CLEAN(projectDir + SEP + "DoeStudies" + SEP + (doe ? doe->name(1400ab770) : "DoeStudy"+QString::number(sub->id)))`.
`1400a51e0(prj)` = `140818bf0(CBASE(prj+0x18), ".MST.TEMP")` = `name + ".MST.TEMP"` (unless it already ends with it,
`140818b50`). `subIdx` = `1400b5c80(sub)` = the index in `prj+0x58` of the sub-project whose id (`+0x1c`) matches.
The rest of `1400ba650` (`140915eb0`) and all of `1400ba800` are DOE file-system work (s1-05, out of scope here).

## 3. Blocks

### 3.1 `140131680` — header
```
ts<<RULE75 /*1421ac120*/ ENDL
ts<<"*  MASTER FILE FOR DEFORM MUlTIPLE OPERATION SIMULATION                   *" /*1421ac170, 75, lower-case 'l' sic*/ ENDL
ts<<BOX75 /*1421ac1c0*/ ENDL
ts<<"*  Integrated 2D / 3D                                                     *" /*1421ac210, 75*/ ENDL
ts<<BOX75 /*1421ac260*/ ENDL
ts<<"*!MASTERFORMAT 4                                                          *" /*1421ac2b0, 75*/ ENDL
ts<<RULE75 /*1421ac300*/ ENDL
ts<<"*!TITLE  " /*1421ac350, 9*/ << QString(prj+0x28)  /*140ceed20*/ ENDL
ts<<"*!GEOTYP " /*1421ac360, 9*/ << *(int*)(prj+0x24)  /*140c0df50*/ ENDL
ts<<"*!UNIT   " /*1421ac370, 9*/ << *(int*)(prj+0x20)  /*1400a2180*/ ENDL
ts<<"*" /*1421a8ccc*/ ENDL
ts<<RULE75 /*1421ac380*/ ENDL
ENDL                                                    // empty line
```
GEOTYP and UNIT are plain ints stored on the project object. The writer does not derive or convert them, so
they are whatever the open path stored. In the reference: Title `Name=""` → `*!TITLE  ` (9 bytes, then LF);
`<Unit Name="SI"/>` → `*!UNIT   1`; GEOTYP `0`. The reader mapping (string → int, and the source of GEOTYP)
belongs to s1-01: **open item O1**.

### 3.2 `1401318b0` — main-project block
```
ENDL
ts<<RULE75/*1421ac3d0*/ ENDL; ts<<BOX75/*1421ac420*/ ENDL
ts<<"* Main Project                                                            *"/*1421ac470, 75*/ ENDL
ts<<BOX75/*1421ac4c0*/ ENDL; ts<<RULE75/*1421ac510*/ ENDL
ts<<"*!MAINPRJ "/*1421ac560, 10: trailing space*/ ENDL
ts<<"*!PRJNAM "/*1421ac570, 9*/ << CBASE(prj+0x18) /*14008a500*/ ENDL
ts<<"*"/*1421a8dc4*/ ENDL
ts<<RULE75/*1421ac580*/ ENDL
```

### 3.3 `140131a50` — sub-project block (.MST.TEMP only)
```
ENDL
ts<<RULE75/*1421ac5d0*/ ENDL; ts<<BOX75/*1421ac620*/ ENDL
ts<<"* Sub-Project                                                             *"/*1421ac670, 75*/ ENDL
ts<<BOX75/*1421ac6c0*/ ENDL; ts<<RULE75/*1421ac710*/ ENDL
ts<<"*!SUBPRJ "/*1421ac760, 9*/ << (subIdx+1) ENDL
ts<<"*!PRJNAM "/*1421ac770, 9*/ << 1400a75a0(prj, subIdx) ENDL
ts<<"*"/*1421a8e8c*/ ENDL
ts<<RULE75/*1421ac780*/ ENDL
```
`1400a75a0(prj, n)`: attribute `Name` (default "") of the n-th (0-based) child element `<Sub_Project>` of the
document element of the **stored QDomDocument** `*(prj+0x48)` (`1400a78e0`). The name comes from the DOM as
loaded, not from a model field.

### 3.4 `140131d10` — trailer
```
ENDL
ts<<RULE75/*1421ac890*/ ENDL; ts<<BOX75/*1421ac8e0*/ ENDL
ts<<"* END OF MASTER FILE                                                      *"/*1421ac930, 75*/ ENDL
ts<<BOX75/*1421ac980*/ ENDL; ts<<RULE75/*1421ac9d0*/ ENDL
ENDL
```
The file therefore ends with `RULE75 LF LF`.

## 4. Operation walk

### 4.1 `140131df0` — `void walk(QTextStream& ts, Project* prj, int subIdx)` (arguments checked in `asm1.txt`)
```
sub = 1400a74f0(prj, subIdx);  if (!sub) return;   // prj+0x58 [subIdx]
doe = *(sub+0x20);                                 // 1400b5a90; DefDoeModel/SfMoDoeManager, null for a plain project
QString dbPath = CLEAN(CLEAN(prj+0x30));           // 1400a7670 -> 1400a4ee0; subIdx is not used
QList<int> order = 1400b6630(sub);                 // operation ids in flow-chart order, §4.2
oprMgr = 1400a58b0(prj);                           // (prj+8)->[+0x19c8] SfMoOprManager (created on demand)
int counter = 0;                                   // SIMULATION number, passed by pointer, incremented by each writer
QMap<int,int> doeMap; if (doe) doeMap = doe->v1c8();   // DefDoeModel slot +0x1c8 = 14091bdb0
for (i = 0; i < order.size(); ++i) {
    int id = order[i];
    item = 140047dc0(oprMgr, &id);   // mgr->v1c8(&id) (14006cea0: row whose item+0x18 == id),
                                     // dynamic_cast SfMoItem->SfMoOprItem, then +0x48 = the "SimItem"
    if (!item) continue;
    op = 140054e90(item);            // per-type operation singleton: main window (0x143d766b8) cast to
                                     // SfMoMainWindow, registry +0x2b8, QMap<QString,SfOpr*> at
                                     // (reg+8)+0x150 keyed by item+0x120 (type name, e.g. "SfOprHeatTransfer")
    if (!op) continue;
    saved = *(op+0x20); *(op+0x20) = item;          // 1405ef2b0 / 1405ef280: "current item" of the singleton
    if (!doe) 140132100(ts, prj, &dbPath, &counter, item, 0, 0);
    else      1401321a0(ts, prj, &dbPath, doe, id, &counter, item, &doeMap, 0, 0);
    *(op+0x20) = saved;
}
```
Notes for the port:
* The operation object is a **shared singleton per type**. Writers read the item only through their arguments
  and `op+0x20`. Per-type state kept on the singleton survives across operations: **open item O2**, for
  s1-03..05 to check in their hooks.
* `counter` starts at 0 for each file and is shared by all operations, so `SIMULATION` numbers run across
  writers that emit several simulations (Cogging, Cycle).

### 4.2 `1400b6630` — flow-chart order (ids)
`flow = *(sub+0)` (`1400b5a70`). If `flow` is null, the list is empty.
1. `groups = 140085190(flow)`: items of the container at `flow+0x20` whose `item->v8()->v58() & 0x1000`
   (group flag), sorted with `140085110` (by `v_d8` (row,col): row first, then column, ascending).
2. For each group G: `members = 140085470(flow, G)`: non-group items whose cell `v_c8` (row,col) lies between
   G's `v_d8` start (row,col) and `v_e8` end (row,col), inclusive in (row,col) lexicographic order, sorted by
   `140084a50` ((row,col) from `v_c8`). Record `memberToGroup[member->v28()] = G->v28()`.
3. `chain = 140084ad0(flow)`: the non-group items sorted by `140084a50`. Take the first, walk back with
   `conn->v138(x)` (previous) until null, which gives the head. Then append the head and every `conn->v130(x)`
   (next) until null, where `conn = flow+0x10`. If the container is empty, the result is empty.
4. Output: for each chain item `c`, `id = c->v28()`. If `id` is a group member with group `g`, append `g` only
   when `g` differs from the last appended group (consecutive members collapse into one group entry, and `last`
   is reset to -1 after a non-member). Otherwise append `id`.

In moproj terms (from `COG_P1_B1.moproj.copy`): `<Sub_Project>` holds `<DiagramItem Operation_Id=… >` with
`<CellPos Row Col>` and `<Connectors><Connections><ItemIndex>` links. The reader (s1-01) must rebuild
prev/next and the group rectangles: **open item O3**. For a project without groups the order is the
connection chain from the head.

### 4.3 `140132100` — non-DOE dispatch
```
op = 140054e90(item); if (!op) return;
g_doeMode(byte 0x143d80258) = 0;
op->v280(ts, prj, dbPath, pCounter, item, p6 /*=0*/, p7 /*=0*/);
```

### 4.4 `1401321a0` — DOE dispatch (`asm1.txt` for the argument order)
```
op = 140054e90(item); if (!op) return;
g_doeMode = 1;
g_doeVarList (QList, 0x143d80270) = doe->v1b8(opId);   // DefDoeModel +0x1b8 = 14091bbb0: DOE variables whose
                                                         // 14091dbb0(var, opId) is true; also sets var+0x18 = index
g_doeOpId (int, 0x143d80268)      = opId;
g_doeModel (ptr, 0x143d80260)     = doe;
g_doeObjMap (QMap<int,int>, 0x143d80298) = *pDoeMap;
op->v280(ts, prj, dbPath, pCounter, item, p9 /*=0*/, p10 /*=0*/);
```
Globals are never reset to their start values at the end of a file, only overwritten. A port keeps them as
per-call state initialised as in §8.

## 5. Base operation writer `1405f5340` (slot +0x280 for Base, DiestressBM, ExpressBase, Forming, Forming2D,
FormingExpressCommon, HTFurnace, HeatTransfer, HeatTransfer2D, HeatTransferCommon, HeatTransferExpress(2D),
Pre, Pre2D)

Signature (from `asm1.txt`): `void write(SfOprBase* this, QTextStream& ts, Project* prj, const QString& dbPath,
int* pCounter, SimItem* sim, int dbreadArg, int /*unused*/)`.
```
n = ++*pCounter;
140131c10(ts, n, sim);                                   // SIMULATION header
int gendbMode = 2;
if (!sim->keyMode /*+0x150, 140064740*/) {
    if (this->v200())  140134250(ts, dbPath, n, sim, dbreadArg);   // DIM/NSTART or DBREAD
    else               1401343e0(ts, dbPath, n, dbreadArg);        // DBREAD only
    this->v288(ts, prj, dbPath, n, sim);                 // default 1405f56c0 for all 34 classes, §5.1
    140132a40(ts, sim);   1401322d0(ts, sim);           // DOE blocks, empty unless g_doeMode
    140135f10(ts, prj, dbPath, n, sim);                  // OBJPOS
    140136040(ts, prj, dbPath, n, sim);                  // INICTC / GENCTC
    140136090(ts, prj, dbPath, n, sim);                  // per-object state variables, NRECVL/ERECVL
    140134680(ts, sim);                                  // extra KFREAD
    gendbMode = 2;
} else {
    140134510(ts, dbPath, sim);                          // KFREAD of the operation's own KEY
    140132a40(ts, sim);   1401322d0(ts, sim);
    140134680(ts, sim);
    gendbMode = 0;
}
140134730(ts, QString(sim+0x118));                       // OPRNAM
1401347e0(ts, QString(sim+0x118));                       // SIMNAM (same string)
140134900(ts, n, 140050b20(sim), 140050b10(sim) /*always 0*/);   // CURSIM
140134a10(ts, dbPath, n, gendbMode, /*bool*/ false);     // GENDB
```
* `v200` (bool "writes DIM/NSTART"): `1405f79b0` returns false (Base, Boolean, Cogging, CopyMirror, Cycle,
  Diestress*, Distortion, DoeOutput, Group, HTFurnace, MultiBlow*, Pre, Pre2D, Report, ShapeOptim).
  `1413e5ef0` returns true (Convert2Dto3D, ExpressBase, Forming, Forming2D, FormingExpress*, HeatTransfer,
  HeatTransfer2D, HeatTransferCommon, HeatTransferExpress(2D), HeatTreat(2D)). Slot +0x1f8 has the same split.
* `v290` (called inside 1405f56c0): `1405f5860` is empty for all 34 classes.

### 5.1 `1405f56c0` — default slot +0x288 (`writeBody`)
Signature `(this, ts, prj, dbPath, int n, SimItem* sim)`. Order (from `asm1.txt`):
```
140134bf0(ts, prj, dbPath, n, sim);   // MDSOBJ, TNOW, KFREAD SimCtrl
140134db0(ts, prj, dbPath, n, sim);   // KFREAD material files
140135220(ts, prj, dbPath, n, sim, false);   // OBJTYP/KFREAD object/MTLGRP + OBJTYP-0 lines
140135da0(ts, prj, dbPath, n, sim);   // CNTACT + KFREAD InterObject
1401360e0(ts, prj, dbPath, n, sim);   // KFREAD MeshSettings, REMESH
1401363b0(ts, prj, dbPath, n, sim);   // KFREAD BccDef
140136810(ts, prj, dbPath, n, sim);   // IMPEQP
1401365f0(ts, prj, dbPath, n, sim);   // KFREAD ObjProperties
this->v290(ts, prj, dbPath, n, sim);  // 1405f5860: nothing
140135fc0(ts, prj, dbPath, n, sim);   // TLOC
```

## 6. Shared emitters (all take `ts` first; `n` = SIMULATION number; `sim` = SimItem)

Path helpers on SimItem, all `CLEAN(...)` of the parts shown:
* `opDir(sim)` = `140050df0`: null if `sim+0x100` (project) is null, else
  `NATIVE(CLEAN(ABSDIR_clean(prj) + SEP + QString(sim+0x130)))`, where `ABSDIR_clean(prj)` = `1400a4220` =
  `CLEAN(QFileInfo(prj+0x18).absolutePath())`. The result has '\\' separators; it is cleaned again below.
* `simCtrl(sim)` = `140059290` = `CLEAN(opDir + SEP + "SimCtrl.KEY"/*142123360*/)`.
* `interObj(sim)` = `14005a690` = `CLEAN(opDir + SEP + "InterObject.KEY"/*142123410*/)`.
* `numbered(sim, prefix, i)` = `CLEAN(opDir + SEP + QString("%1%2.KEY").arg(prefix, 0, ' ').arg(i+1, 5, 10, '0'))`:
  `14005cf70` prefix "MeshSettings"(1421234d8), `14005da90` "BccDef"(14212350c), `14005eb10` "ObjProperties"(1421235d8).
* `keyFile(sim, raw)` = `140064a50`: `CLEAN(opDir + SEP + QString(sim+0x158))`. With `raw == false` it is first made
  relative to `opDir` (only `raw == true` is used here).

| Addr | Emits (exact) | Reads |
|---|---|---|
| `140131c10` header | `ENDL; RULE75/*1421ac7d0*/ ENDL; "SIMULATION\t"/*1421ac820,11*/ <<n ENDL; "*!SIMTYP  \t"/*1421ac830,11*/ <<S(sim+0x120) ENDL; RULE75/*1421ac840*/ ENDL` | sim+0x120 type name (`140050d00`) |
| `140134250` | if `n==1`: if `sim && sim[+0x18](byte) && sim[+0x1c](int)!=0`: `"DIM    \t"/*1421acb38,8*/ <<dim ENDL` then `140134360(ts,db,1,sim[+0x1c])`. Else (n≠1): `if (sim && sim[+0x18]) arg = 14005a300(sim)`, then `1401343e0(ts,db,n,arg)` | dim = `14005f0f0(sim)`: 3 if no op, else `op->v190()` (`1405f5bf0`, §7) |
| `140134360` | if `n==1`: `"NSTART    \t"/*1421acb48,11*/ <<v ENDL` | — |
| `14005a300` | returns `sim[+0x18] ? (v=sim[+0x1c], v<0 ? -1-v : v) : 0` | |
| `1401343e0` | if `n!=1`: `"DBREAD    \t"/*1421acb58,11*/ <<arg ENDL; <<S FILENAME(dbPath) ENDL` | |
| `140134510` | if `sim[+0x150]`: `p = keyFile(sim,true)`; if `!p.isEmpty()`: `p = REL(p)`; `"KFREAD    \t"/*1421acb78,11*/ <<1 ENDL; <<S p ENDL` | sim+0x150 byte, +0x158 QString |
| `140134680` | if `sim[+0x140]`: `s = QString(sim+0x148)`; if `!s.isEmpty()`: `"KFREAD    \t"/*1421acb88*/ <<1 ENDL; <<S s ENDL` (**not** relativised) | sim+0x140 byte, +0x148 QString |
| `140134730` OPRNAM | if `!g_cycleMode(0x143d8025a)`: `"OPRNAM"/*1421acb94,6*/ ENDL; <<S name ENDL`; else `"OPRNAM"/*1421acb9c*/ ENDL; <<S g_cycleOprName(0x143d80280) ENDL` | |
| `1401347e0` SIMNAM | if `!g_cycleMode`: `"SIMNAM"/*1421acba4*/ ENDL; <<S name ENDL`; else `"SIMNAM"/*1421acbac*/ ENDL; <<S ("CYCL"/*1421acdb4*/ + QString::number(g_cycleIdx 0x143d80250)) <<S "-"/*1421ac34c*/ <<S name ENDL` (`140136b60`) | |
| `140134900` CURSIM | if `!g_cycleMode`: `"CURSIM    \t"/*1421acbb8,11*/ <<n "\t" <<a "\t" <<b ENDL`; else the same but the middle value is `g_cycleCursim (int 0x143d80254)` | a = `140050b20(sim)`, b = 0 |
| `140134a10` GENDB | if `!g_kfwrit(0x143d80259)`: `c = flag5 ? 2 : 1`; if `n==1`: `"GENDB     \t"/*1421acbd8,11*/ <<2 "\t" <<c "\t" <<0 ENDL`; else `"GENDB     \t"/*1421acbe8*/ <<1 "\t" <<c "\t" <<mode ENDL`; then `<<S FILENAME(dbPath) ENDL`. Else: `"KFWRIT"/*1421acbf4*/ ENDL; <<S g_kfwritPath(0x143d80278) ENDL` | the base passes flag5 = false |
| `140134bf0` | `p = simCtrl(sim)`; if `!p.isEmpty()` (always true): `"MDSOBJ    \t"/*1421acc00,11*/ "  0       0       0    0.0000000000E+000"/*1421acc10,40*/ ENDL; "TNOW\t   \t"/*1421acc40,9*/ "  -1E21       0       1"/*1421acc50,23*/ ENDL; "KFREAD    \t"/*1421acc68*/ <<1 ENDL; <<S REL(p) ENDL` | sim+0x100, +0x130 |
| `140134db0` | `mm = SfMoMatManager (prj+8)->[+0x19d8]` (`1400a5930`); `cnt = mm->v1b8()` (`14006cdd0` = rowCount); if `sim==0 || cnt==0` return. `ids = 140058e30(sim)` (§6.1); for each string `s`: `id = s.toInt()`; `p = mm->v208(&id)` (`14006d380`: item with id → `CLEAN(item+0x28)`, or null); if `!p.isEmpty()`: `"KFREAD    \t"/*1421acc78*/ <<1 ENDL; <<S REL(p) ENDL` | material items +0x18 id, +0x28 path |
| `140135220` | §6.2 | |
| `140135680` | single-object variant of 140135220 (only index 0; skips the `+4 != 7` test); called by MultiBlow `140606a40`, not by the base | |
| `140135b80` | §6.2 | |
| `140135da0` | `p = REL(interObj(sim))`; `"CNTACT    \t"/*1421acd38,11*/ <<0 ENDL; "KFREAD    \t"/*1421acd48*/ <<1 ENDL; <<S p ENDL` (always written) | |
| `1401360e0` | `N = objCount(sim)`; if `N<1` return. Loop 1, `i=0..N-1`: `o = obj(sim,i)`; if `o && o[+0x1ba]` (MeshSetting byte, `14004dfc0`): `"KFREAD    \t"/*1421acd68*/ <<1 ENDL; <<S REL(numbered(sim,"MeshSettings",i)) ENDL`. Loop 2, `i=0..N-1`: if `o && o[+0x1b9]` (Remesh byte, `14004df80`): `"REMESH    \t"/*1421acd78,11*/ <<o[+0](int) ENDL` | |
| `1401363b0` | for `i=0..N-1`: if `o && o[+4]==5` (`14004d600`) `&& o[+0x1be]` (DefineBCC byte, `14004e0c0`): `"KFREAD    \t"/*1421acd88*/ <<1 ENDL; <<S REL(numbered(sim,"BccDef",i)) ENDL` | |
| `140136810` | `em = SfMoEqpManager (sim+0x100 →+8)->[+0x19e0]` (`140055030`, `1400a5970`); if null return. For `i=0..N-1`: if `o && (e = o[+0x28](int)) > 0`: `p = REL(em->v208(&e))`; if `!p.isNull()`: `"IMPEQP    \t"/*1421acda8,11*/ <<(i+1) ENDL; <<S p ENDL` | object +0x28 equipment id |
| `1401365f0` | for `i=0..N-1`: if `o && o[+4]==5`: `"KFREAD    \t"/*1421acd98*/ <<1 ENDL; <<S REL(numbered(sim,"ObjProperties",i)) ENDL` | |
| `140135fc0` | `t = *(double*)(sim+0xe8)` (`1400ac0e0`); if `t > 0.0` (COMISD with the 0.0 at 1421ad1d0, so NaN is false): `"TLOC    \t"/*1421acd58,9*/ <<d t ENDL` | |
| `140135f10` OBJPOS | `L = QList at sim+0x60` (`140062070`/`1400620a0`); for `i` in L: `is2D = 14005f150(sim)` (op exists and `op->v190()==2`); `is2D ? 140065af0(L[i],ts) : 140065f80(L[i],ts)` (§6.3) | |
| `140136040` | `c = sim+0x70` (`140065790`); `1400ac010(c, ts)`: if `c[100]`: for `k=0..99`: if `c[k]`: `"INICTC    \t"/*1421425f0,11*/ <<(k+1) ENDL`. Then if `c[0x65]`: `"GENCTC    \t"/*1421425e0,11*/ <<d *(double*)(c+0x68) ENDL` | sim+0x70..+0xd8 |
| `140136090` | if `objCount(sim) > 0`: `1400613b0(sim, ts)` (§6.4) | |

`objCount(sim)` = `14005fa80` = size of `QList<int>` at `sim+0x40` (object id per slot).
`obj(sim,i)` = `140061250`: null when `i` is out of `[0,objCount)`. Otherwise it **grows** the `QList<ObjEntry*>`
at `sim+0x50` up to `i` by appending default entries (`140061020` → new 0x1c8-byte `ObjEntry`, `14004d160`/`14004d450`:
`+0=k+1, +4=7, +8=7`, all flags 0), then returns `list[i]`. Default entries have type 7, so the emitters skip them.

### 6.1 `140058e30` / `140058f30` — material id strings
For `i=0..objCount-1`: `id = 14005fb30(sim,i)` (= `sim+0x40[i]`, or -99 when out of range). If `id != -1 && id != -99
&& (o=obj(sim,i))`: for each `m` in `o+0x20` (QList<int>, via `14004db80` which gives -1 when out of range),
append `m` if it is not already in the list. Result: `QString::number(m)` for each, in first-seen order.

### 6.2 `140135220` (OBJTYP) and `140135b80` (closing OBJTYP-0 lines)
```
N = objCount(sim); g_objMaskCur.resize(N, 0);           // 140136c60: keeps the existing prefix
for (i=0;i<N;++i) {
  g_objMaskCur[i] = 0;
  o = obj(sim,i);
  if (!o || o[+4]==7 || o[+0x1b8]/*14004df40*/) continue;
  id  = 14005fb30(sim,i);  num = 14005fc10(sim,i) /* = i+1 */;
  if (!single /*false from the base*/ || num != 1) {
     if (id != -2) {                                   // 1400606c0: -2 = "from DB"
        od = 140060030(sim,id);  // object manager (prj+8)->[+0x19d0] ->v1c8(&id), dynamic_cast SfMoItem->SfMoObjItem
        if (!od) continue;                             // mask stays 0
        p = CLEAN(QString(od+0x28));                   // 14006ba60
        if (!p.isEmpty()) {
           ts<<"OBJTYP\t\t"/*1421acc98,8*/ <<num <<"\t" <<0 ENDL;
           ts<<"KFREAD    \t"/*1421acca8*/ <<1 ENDL; ts<<S REL(p) ENDL;
        }
     }
     if (o+0x20 list size == 1)
        ts<<"MTLGRP    \t"/*1421accb8,11*/ <<num <<"\t" <<0 <<"\t" <<matId0 ENDL;   // matId0 = list[0]
     g_objMaskCur[i] = 1;
  } else {
     if (o+0x20 list size == 1) ts<<"MTLGRP    \t"/*1421acc88*/ <<1 <<"\t" <<0 <<"\t" <<matId0 ENDL;
     g_objMaskCur[i] = 1;
  }
}
140135b80(ts, sim):
  nc = g_objMaskCur.size(); np = g_objMaskPrev.size();
  for (i=0;i<np;++i) {
    o = obj(sim,i);   // may grow the list
    if (!o || o[+4]==7 || i>=nc || !g_objMaskCur[i])
       ts<<"OBJTYP    \t"/*1421acd08/18/28, 11*/ <<(i+1) <<"\t" <<0 ENDL;
  }
  g_objMaskPrev.resize(nc, 0); copy g_objMaskCur[0..nc) into it.
```
Every `"\t"` in these lines is a separate 1-byte literal (1421ac70c, …), all 0x09. The two OBJTYP spellings
differ: `"OBJTYP\t\t"` (8 bytes) before a KFREAD, `"OBJTYP    \t"` (11 bytes) for "object removed".
`g_objMaskPrev` is cleared at the start of each file (`140131390`). The only other writer is `140135ae0`
(from `1400a2200`, the Run/DOE preparation path, not Save).

### 6.3 OBJPOS lines (`140065f80` 3D, `140065af0` 2D). `int* p` is an ObjPos record.
Common start: `"OBJPOS"/*1421249e8*/ "\t" <<p[1] "\t" <<p[0] "\t"` (object, type). Then, by `t = p[0]`
(doubles are read at byte offsets; `D(o)` = `<<d *(double*)((char*)p+o)`, `Z` = `<<d 0.0`, `I` = `<<i p[2]`;
values are separated by "\t"):

| t | 3D (`140065f80`) | 2D (`140065af0`) |
|---|---|---|
| 1, 11 | D30 D38 D40 Z Z Z Z | D30 D38 Z Z |
| 2, 12, 102, 112 | D10 D18 D20 D28 I Z Z | D10 I D18 D20 |
| 3 | D48 D50 D58 D60 D68 D70 D78 | D48 D50 D78 Z |
| 4 | (nothing) | D80 D88 D98 DA0 |
| 6 | D60 D68 D70 D78 Z Z Z | D78 Z Z Z |
| 7, 8 | D10 D18 D20 D28 Z Z Z | (nothing) |
| 9 | D10 D18 D20 D28 D60 D68 D70 | (nothing) |
| other | (nothing) | (nothing) |

Then `k = size of QList<int> at p+0xd0` (`1400666e0`). If `k>0`: `"\t" <<k ENDL`, then for `j=1..k`: `<<list[j-1]`
(`140066700`), followed by `"\t"` except after the last one, then `ENDL`. If `k==0`: `ENDL`.
For a type with "(nothing)", the line ends `"…\t<type>\t"` + (`"\t"<<k` …) + LF.

### 6.4 `1400613b0` — per-object state variables and recovery lists
```
N = objCount(sim);
for (i=0;i<N;++i) {
  o = obj(sim,i); if (!o) continue;
  sv = o+0x30;                                          // 14004df00
  if (14005f1c0(sim) /* no op, or op->v190()==3 */) { 14004f1c0(sv,ts); 14004f050(sv,ts); }   // 3D
  else                                               { 14004e830(sv,ts); 14004f050(sv,ts); }  // 2D
  A = *(QList<BC*>**)(model+0x19a8)                     // 14005a400: model = 1400b9cd0(current sub-project) =
                                                        // holder (prj+8), or for sub kind 4 a per-sub model at sub+0x48;
                                                        // no null check on A in the original (see O4)
  F = *(QList<bool>**)(sim+0x28) (may be null);  B = *(QList<BC*>**)(sim+0x20) (not null-checked);
  nA0 = #A with BC->type(+0)==0, nA1 = #A with type 1; same for B (counted together as n0, n1)
  if (n0 > 0) {
    ts<<"NRECVL    \t"/*142123680*/ <<o[+0] "\t" <<0 "\t" <<1 "\t" <<n0 ENDL;
    flags: for A with type 0 (index j in A): f = (F non-empty ? F[j] : false); for B with type 0: f = true;
           print <<(int)f, then ",    "(142123x8c, 5 bytes) between items, ENDL after the n0-th
    values: same order, <<d BC[+0x18], then ",    " between items, ENDL after the last
  }
  if (n1 > 0) { "ERECVL    \t"/*1421236b0*/ … same scheme for type 1, flag default true for A when F is empty }
}
```
The NRECVL/ERECVL block depends on lists that are **not** per object, so it repeats for every object
(as written). State-variable lines (`sv` offsets relative to `o+0x30`; `#` = `<<i *(int*)(sv+0x104)` = object
number; "\t" between fields):

| flag | value(s) | 2D `14004e830` | 3D `14004f1c0` |
|---|---|---|---|
| +0x00 | d+0x08 | `NDTMP    \t#\t0\td` | same |
| +0x10 | d+0x18 | `STRAIN    \t#\t0\td` | same |
| +0x20 | d+0x28 | `STRESS    \t#\t0\td` | same |
| +0x30 | d+0x38 | `DAMAGE    \t#\t0\td` | same |
| +0x40 | d+0x48,+0x50(,+0x58) | `URZ    \t#\t0\t[48]\t[50]\t` (trailing tab) | `URZ    \t#\t0\t[48]\t[50]\t[58]` |
| +0x60 | d+0x68,+0x70(,+0x78) | `DRZ    \t#\t0\t[68]\t[70]\t` | `DRZ    \t#\t0\t[68]\t[70]\t[78]` |
| +0x80 | d+0x88 | `GRAIN    \t#\t0\t16\t0\t[88]\t[88]` | `GRAIN    \t#\t0\t16\t0\t0\t[88]` |
| +0xa0 | d+0xa8 | `SIZESH    \t#\t[a8]\t0\t0\t0` ("0" literals) | same |
| +0xb0 | d+0xb8 | `DENSTY    \t#\t0\td` | same |
| +0xc0 | d+0xc8 | `AVGSTR    \t#\td` | same |
| +0xd0 | d+0xd8 | `LMTSTR    \t#\td` | same |
| 3D only: +0xe0 byte && int+0xf0>0 | | | `NRECVL    \t#` `"\t0\t0\t"` `<<int(+0xf0) "\t" <<(int)byte(+0xe0)` |
| 3D only: +0xf4 byte && int+0x100>0 | | | `ERECVL    \t#` `"\t0\t0\t"` `<<int(+0x100) "\t" <<d(+0xf8)` |

Then `14004f050`: if `sv[+0x180]`: `"SVSCAL    \t" # "\t" 1 "\t" <<k(int +0x124) ENDL`, then for `j<k`:
`<<d ((double*)sv[+0x130])[j] "\t" <<d 14089c490(sv+0x108, j,0,0,0) ENDL` (table lookup:
`*(double*)(*(sv+0x108+0x48) + idx*8)` with `idx = 14089c2a0(...)`, or 0.0). Each keyword literal is `<name>`
padded as in the binary: `"NDTMP    \t"`, `"STRAIN    \t"`, `"STRESS    \t"`, `"DAMAGE    \t"`,
`"URZ    \t"`, `"DRZ    \t"`, `"GRAIN    \t"`, `"SIZESH    \t"`, `"DENSTY    \t"`, `"AVGSTR    \t"`,
`"LMTSTR    \t"`, `"SVSCAL    \t"` (14211f0e8…14211f198). The exact padding is copied from .rdata in `a4s.c`.

### 6.5 DOE-only emitters (output only when `g_doeMode`; details to s1-05)
* `140132a40(ts, sim)`: `v = g_doeOpId; doe = g_doeModel; L = QList`. For `i<objCount`: `id = sim+0x40[i]`; if
  `id >= 0`: `L = doe->v1b0(v, id)` (`14091ba60`); if `!L.isEmpty()`: `"* DOE Geometry Variables "/*1421aca58,25*/ ENDL`,
  then `140132bd0(ts, sim, v, id, L, &g_doeObjMap)`.
* `1401322d0(ts, sim)`: if `!g_doeVarList.isEmpty()`: `"* DOE Variables "/*1421aca20,16*/ ENDL`; then for each var:
  `ENDL "* DOE Study "/*1421aca38,12*/ ENDL`; if `var->link(+0x40)`: `"* Link: "/*1421aca48,8*/ <<var->+0x1c "->" <<link->+0x1c`
  [+ `":" <<g_doeObjMap[link->+0x18]` when the key is present] ENDL; `"* (" <<idx ") " <<S QString(var) ENDL`; then for
  each entry `e` of `var->+0x30`: `"DOEVAR" "\t" <<(idx+1) "\t" <<lines(e+0x18) "\t" <<lines(e+0x20) ENDL; <<S e+0x18 ENDL; <<S e+0x20 ENDL`
  (`lines` = `14093a200`: number of non-empty '\n'-separated parts). The block closes with ENDL. In the
  `"* ("` line, `idx` = (link ? link+0x1c : var+0x1c). Before the DOEVAR lines, `idx` is replaced by
  `g_doeObjMap[key]` when the map is non-empty and contains `key` = (link ? link+0x18 : var+0x18). `QString(var)`
  is the QString at var+0 (`1403047c0`).
* `140132bd0`: geometry DOE (`"* Geometry change : "`, `"* DOE Study "`, `"* ("`, `"* Link: "`, `DOEVAR\t0\t<5n+3>\t<n>\t1`,
  `MORP\t<n>\t<2|102>`, object lines, `../../Control/` paths, `.GEO` → `.COR`, `OutputGeoOp%1Obj%2.GEO`,
  `GFREAD`, `DEFAMG`, `GENBCC`). It calls `140909a30`, which **writes files** (.COR) as a side effect. This is
  out of scope for a pure MST converter: **open item O5**.

## 7. Dimension (`DIM` and the 2D/3D choice)
`op->v190()` is `1405f5bf0` for every corpus class:
```
b = this->v188();                    // 141354350 (HeatTransfer), 140750600 (Cogging), 140c060a0 (Forming)…: all
                                     // return the virtual base of the member at op+0x28: op+0x30+vbtable[1] = op+0x50
f = b->v58();                        // vftable 14249dbe8 slot +0x58 = 1405fece0
     // 1405fece0: *(int*)(*(void**)(op+0x38) + 0x80)  → info object (0xa0 bytes, 1405fe2e0) field +0x80, initially 0
return (f&3)==3 ? 4 : (f&1) ? 2 : (f&2) ? 3 : 0;
```
`14005f0f0` returns 3 when the item has no operation object. `DIM` in the reference is 3, so `f == 2`.
Who sets `info+0x80` (op dimension flags) and from which moproj or KEY data belongs to s1-01: **open item O6**.
The classes with their own +0x190 (Cycle, DiestressBM2D/3D, Diestress, DoeOutput, Report, ShapeOptim) are listed
in `vtables.tsv`.

## 8. Globals (per-call state in the port)

| VA | Type | Meaning | Set by | Initial |
|---|---|---|---|---|
| 0x143d80250 | int | cycle index (CYCL<n>) | `140136b40` (Cycle) | 0 (.bss) |
| 0x143d80254 | int | CURSIM middle value in cycle mode | `140136af0` | 0 |
| 0x143d80258 | byte | g_doeMode | `140132100`=0, `1401321a0`=1 | 0 |
| 0x143d80259 | byte | g_kfwrit (GENDB → KFWRIT) | `140136ab0`=1 / `140136ae0`=0 (callers `1401cae90`, `140e29270`, `140eaf030`; not on Save) | 0 |
| 0x143d8025a | byte | g_cycleMode | `140136af0`=1 / `140136b30`=0 (Cycle writer `14134dcc0`, set before and reset after its loop) | 0 |
| 0x143d80260 | ptr | g_doeModel | `1401321a0` | 0 |
| 0x143d80268 | int | g_doeOpId | `1401321a0` | 0 |
| 0x143d80270 | QList | g_doeVarList | `1401321a0` | empty (`141cb3800`) |
| 0x143d80278 | QString | g_kfwritPath | `140136ab0` | null |
| 0x143d80280 | QString | g_cycleOprName | `140136af0` | null |
| 0x143d80288 | vector<char> | g_objMaskCur | `140135220/680`, cleared by `140131390` | empty |
| 0x143d80290 | vector<char> | g_objMaskPrev | `140135b80`, `140135ae0`, cleared by `140131390` | empty |
| 0x143d80298 | QMap<int,int> | g_doeObjMap | `1401321a0` | empty |

On the Save path g_kfwrit is 0 and g_cycleMode is 0 except inside the Cycle writer. They are process globals in
the original, so a stale value from an earlier Run/DOE action in the same GUI session could leak into a later
Save. A fresh process (oracle B) cannot show this, and the port starts from the initial values.

## 9. Field table (for matching with s1-01)

| Object | Off | Type | Meaning | Accessor | Read by |
|---|---|---|---|---|---|
| Project | +0x08 | ptr | main-window model holder (managers at +0x19c0..+0x19e0, created on demand) | 1400a5890 | 140131df0, 140134db0, 140136810, 140060030 |
| Project | +0x18 | QString | .moproj full path | 14008a4b0 / 14008a500 (CBASE) / 1400a4220 (abs dir) | 1400a50a0, 1401318b0, opDir |
| Project | +0x20 | int | UNIT | 1400a2180 | header |
| Project | +0x24 | int | GEOTYP | 140c0df50 | header |
| Project | +0x28 | QString | Title | 140ceed20 | header |
| Project | +0x30 | QString | primary DB path (cleaned twice) | 1400a4ee0 / 1400a7670 | walk → dbPath |
| Project | +0x48 | QDomDocument* | loaded moproj DOM | 1400a78e0 | 140131a50 |
| Project | +0x50 | int | current (active) sub-project index | 1400a73c0 / 1400a7390 | 1400a20d0, CURSIM a, NRECVL list A |
| Project | +0x58 | QList<SubProject*> | sub-projects, [0] = main | 1400a7360 / 1400a74f0 | 1401313c0, walk |
| Holder | +0x19c8 | SfMoOprManager* | op items (id at item+0x18; SfMoOprItem+0x48 = SimItem) | 140044c80, 140047dc0 | walk |
| Holder | +0x19d0 | object manager (vft 14212cf68) | objects (SfMoObjItem +0x18 id, +0x28 KEY path) | 140044d20, 140060030 | 140135220 |
| Holder | +0x19d8 | SfMoMatManager* | materials (+0x18 id, +0x28 path) | 140044dc0, slots 1b8=14006cdd0, 208=14006d380 | 140134db0 |
| Holder | +0x19e0 | SfMoEqpManager* | equipment (same layout) | 140044e60 | 140136810 |
| SubProject | +0x00 | flow* | flow chart (+0x10 conn mgr, +0x20 items, +0x40 QMap<int,int> pos cache) | 1400b5a70 | 1400b6630, 140085800 |
| SubProject | +0x10 | Project* | owner | 1400b5c40 | DOE |
| SubProject | +0x18 | int | kind (2/3 = DOE variants) | 140010330/360 | 1400a20d0 |
| SubProject | +0x1c | int | id | 1400b5c80 | DOE |
| SubProject | +0x20 | DefDoeModel* | DOE model, null for main | 1400b5a90 | walk |
| SimItem | +0x18 | byte | NSTART defined | 14005a2a0 | 140134250 |
| SimItem | +0x1c | int | NSTART step (negative = from end) | 14005a2e0 | 140134250/360, 14005a300 |
| SimItem | +0x20 | QList<BC*>* | recovery list B (BC: +0 int 0=node/1=element, +0x18 double) | 1401cc700 | 1400613b0 |
| SimItem | +0x28 | QList<bool>* | recovery flags F for list A (may be null) | 140970130 | 1400613b0 |
| SimItem | +0x40 | QList<int> | object id per slot (-1 none, -2 from DB, -99 invalid) | 14005fa80/14005fb30 | many |
| SimItem | +0x50 | QList<ObjEntry*> | object entries (grown on demand) | 140061250 | many |
| SimItem | +0x60 | QList<ObjPos*> | OBJPOS records | 140062070/1400620a0 | 140135f10 |
| SimItem | +0x70 | byte[100] | INICTC flags per object | 140065790 | 1400ac010 |
| SimItem | +0xd4 | byte | any INICTC (c+0x64) | | 1400ac010 |
| SimItem | +0xd5 | byte | GENCTC present (c+0x65) | | 1400ac010 |
| SimItem | +0xd8 | double | GENCTC value (c+0x68) | | 1400ac010 |
| SimItem | +0xe8 | double | TLOC | 1400ac0e0 | 140135fc0 |
| SimItem | +0x100 | Project* | owner project | 140055030 | opDir, managers |
| SimItem | +0x108 | int | operation id | 140050910 | CURSIM a |
| SimItem | +0x118 | QString | operation name (OPRNAM, SIMNAM) | 140050ac0 | 1405f5340 |
| SimItem | +0x120 | QString | type name (SIMTYP; registry key) | 140050d00 | 140131c10, 140054e90 |
| SimItem | +0x130 | QString | operation folder relative to the project dir | 140051110 | opDir |
| SimItem | +0x140 | byte | extra KFREAD | 140062e40 | 140134680 |
| SimItem | +0x148 | QString | extra KFREAD text | 140062e80 | 140134680 |
| SimItem | +0x150 | byte | KEY mode (own KEY file instead of body) | 140064740 | 1405f5340 |
| SimItem | +0x158 | QString | KEY file name | 140064a50 | 140134510 |
| ObjEntry | +0x00 | int | object number (slot+1) | 14004d560 | REMESH, NRECVL |
| ObjEntry | +0x04 | int | ObjectType (7 = undefined; 5 = "from DB") | 14004d5a0 / 14004d600 | 140135220/b80, 1363b0, 1365f0 |
| ObjEntry | +0x20 | QList<int> | material ids | 14004db50/14004db80 | MTLGRP, materials |
| ObjEntry | +0x28 | int | EquipmentID | 14004de90 | IMPEQP |
| ObjEntry | +0x30 | block | state variables (§6.4) | 14004df00 | 1400613b0 |
| ObjEntry | +0x1b8 | byte | skip object (SkipDefine?) | 14004df40 | 140135220/680 |
| ObjEntry | +0x1b9 | byte | Remesh | 14004df80 | REMESH |
| ObjEntry | +0x1ba | byte | MeshSetting | 14004dfc0 | KFREAD MeshSettings |
| ObjEntry | +0x1be | byte | DefineBCC | 14004e0c0 | KFREAD BccDef |
| op singleton | +0x20 | SimItem* | current item | 1405ef2b0/280 | hooks |
| op singleton | +0x38 | info* | info (+0x80 dimension flags) | via 1405fece0 | DIM, 2D/3D choice |

`CURSIM a` = `140050b20(sim)`: `sub = 140055050(sim)` (the **current** sub-project of the item's project, not the
walked one). If `sub` is null, 0; else `140085800(sub->flow, sim+0x108)`: the 1-based position of the op in the
flow chart, where a group counts once and all its members share the group's number. It is computed once and
cached in `flow+0x40`, and is 0 if the id is unknown.

## 10. Check against `COG_P1_B1.MST` (copy)
* Lines 1–13 match §3.1: GEOTYP 0, UNIT 1, empty title. Lines 14–23 match §3.2 (PRJNAM `COG_P1_B1` =
  CBASE of the moproj). Lines 294–300 match §3.4 (ending `RULE75 LF LF`). LF only, ASCII.
* SIMULATION 1 (HeatTransfer, `v200`=true, n=1): `DIM\t3` (written as `"DIM    \t3"`), `NSTART    \t-1` (sim+0x18 set,
  +0x1c=-1). The body: MDSOBJ/TNOW literals; KFREAD of `Operations/Task00001/SimCtrl.KEY` (REL to the DB directory);
  material `Materials/Material00001.KEY`; `OBJTYP\t\t1\t0` + KFREAD object + `MTLGRP    \t1\t0\t1`; CNTACT +
  InterObject; `IMPEQP    \t1` (equipment id 1 on slot 0); `INICTC    \t1`; `GENCTC    \t0` (double 0.0 → `0`).
  Then OPRNAM/SIMNAM `HeatTransfer`, `CURSIM    \t1\t1\t0`, `GENDB     \t2\t1\t0` + `COG_P1_B1.DB`. Matches §5.
* SIMULATION 2: n≠1, so `DBREAD    \t0` (`14005a300`: -1-(-1) = 0) + DB name. Objects 2 and 3 newly defined
  (OBJTYP/KFREAD/MTLGRP 2), slot 0 is `Id=-2` / ObjectType 5, so it has no OBJTYP line and gets
  `KFREAD ObjProperties00001.KEY` after IMPEQP 2/3. The order KFREAD-material → OBJTYP → CNTACT → IMPEQP →
  ObjProperties → INICTC matches §5.1 + §5.
* SIMULATION 3/4 (ENTER/EXIT, HeatTransfer): all objects are `-2`, so there is no OBJTYP or MTLGRP and no OBJTYP-0
  line (mask = 1 for every slot). Three ObjProperties KFREADs; OBJPOS lines `OBJPOS\t2\t2\t0.0001\t0\t0\t-1\t1\t0\t0`
  (3D type 2: D10 D18 D20 D28 I Z Z) and `OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t0` (type 3, seven doubles). `CURSIM a`
  = 3 and 4 follows the flow order. All three match §6.
* SIMULATION 5–7 come from the Cogging writer (`1407520c0`, s1-03). It uses the same OPRNAM/SIMNAM/CURSIM/GENDB
  emitters in a different order (SIMNAM before OPRNAM), its own OBJPOS formats and TRANS/NSTEP lines.
* The flow order 1, 4, 5, 2, 3 (Task ids) cannot be checked against the Sub_Project connections without a
  reader. Left to s1-01/s1-00.

## 11. Open questions
* **O1** GEOTYP/UNIT/Title source: which moproj element sets prj+0x24 / +0x20 (`Unit Name="SI"` → 1; the GEOTYP
  source is unknown).
* **O2** Operation objects are per-type singletons (`140054e90` registry). Hooks may keep state on `this` across
  operations of the same type within one file; s1-03..05 must check every hook for members written during
  `v280`.
* **O3** The flow-chart model (prev/next connections, group rectangles via `v_c8`/`v_d8`/`v_e8`, group flag
  0x1000) must be rebuilt from `<DiagramItem>`/`<CellPos>`/`<Connections>` by s1-01. The classes behind
  `flow+0x10` (+0x130/+0x138) and the item vtables were not resolved here.
* **O4** `CURSIM a` and the NRECVL list A use the **current** sub-project (prj+0x50), while the walk uses
  sub-project 0. For a main-project save these normally agree. The meaning of the moproj attribute `Active` and
  the persistence of prj+0x50 need confirming (s1-01).
* **O5** DOE path: `140132bd0` → `140909a30` writes .COR files while the MST is written; `1400ba650/ba800` create
  DoeStudies directories. A converter must not do this (plan guarantee). Decide whether DOE MSTs are in scope or
  `M2M_E_UNSUPPORTED`.
* **O6** Dimension flags `info+0x80` at `*(op+0x38)`: who writes them and from what (probably the SimCtrl KEY
  or the moproj object Mesh2d/3d data). `DIM` and the 2D/3D OBJPOS and state-variable choice depend on them.
* **O7** inf/NaN doubles print as Qt spellings (`inf`, `nan`); confirmed in `QtCore4_64.dll` 4.8.4 by P04, no corpus case.
* **O8** The codec is the system ANSI code page unless the DEFORM language global (0x143d79718, set in `1403b5520`)
  is 1/2/3/4/7. The DLL needs a code-page option, and the corpus may contain non-ASCII names to test.
