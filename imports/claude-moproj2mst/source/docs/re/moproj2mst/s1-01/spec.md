# s1-01: .moproj open path and the model the MST writers read

Packet s1-01 of stage 1 (moproj2mst). Static evidence only: Ghidra decompiles of a copy of
`DEF_GUI_MO_64.exe` (DEFORM v11.0.2, image base `0x140000000`) in `gproj_s1a`, plus read-only
copies of the 58 corpus projects in `s1-01/samples/` (`NN_<name>.moproj` and the saved `.MST`).
Raw decompiles are in `s1-01/dc/*.c` (string literals annotated by `tools/ann.py`), helper
scripts in `s1-01/tools/`. Nothing was run; no licence code was followed.

Notation: `P` = project object (`SfMoProject`-like, 0x1500 bytes, no vtable), `M` = the data root
that owns it (`P+0x08`), `OPI` = `SfOperationInfo` (per-operation data, 0x168 bytes), `OBJ` =
per-object data (0x1c8 bytes), `SUB` = sub-project record (0x50 bytes), `OB` = the registered
operation-class singleton (`SfOpr*`, vtable slot +0x280 is the MST writer), `PROPS` = the
operation's `SfOpr*Properties` object (`OPI+0x10`). `QString` and `QList` are 8-byte d-pointers.

## 0. Summary for the porters

* Open = `14008a9b0` → `14008afa0` (read file, `QDomDocument::setContent`) → `14008b410` (walk
  root children). Operations are built in document order by `14008ccf0`; the flow chart of
  sub-project 0 is parsed later, when the GUI activates it (`1400a7250` → `1400a7b90` →
  `14008f530`). The converter must do both.
* The MST operation order is the connector chain of sub-project 0 (section 9). A prototype
  (`tools/order.py`) reproduces the `OPRNAM` sequence of all 51 corpus MSTs that were saved by
  v11 (the one other MST is from 2013 and differs only by duplicate names, see 6.2).
* `ActionKeywords` are parsed into structured fields (TLOC, OBJPOS, INICTC, GENCTC, per-object
  state variables); the raw text is not kept. The writers must regenerate from the fields.
* `<Path>` of `MoItem` is ignored: every item path is recomputed from `Id`.
* Open has file-system side effects the DLL must not repeat: `mkpath` of `Operations/TaskNNNNN`,
  `Objects`, `Materials`, `Equipments`, `Lubricants` (and `DoeStudies`), writing
  `ObjPropertiesNNNNN.KEY` (and legacy `SimCtrl.KEY`, `InterObject.KEY`, inline item `Data`) from
  the hex blobs, and the optional FOR003 rename dialog. In the DLL these become an in-memory
  overlay (path → bytes) that later KEY reads consult first (open question Q1).
* KEY-type files read during open: per class post-load hook (`PROPS` vtable +0xc0):
  Cogging → `Operations/TaskN/Cogging.MDT`, Boolean → `Boolean.BWZ`, Express (HeatTransferExpress,
  FormingExpress) → `Forming.FWZ`, Cycle → `Cycle.CWZ`. The per-operation lazy load
  ("Open operation - %1", `140051340`/`140051c30`) is not reached from open or from the MST walk;
  it runs only when the GUI selects an operation (Q4).

## 1. Entry: `14008a9b0` OpenProject(P, const QString& file) → bool

```
P+0x55 = P+0x56 = P+0x57 = 0
QCoreApplication::processEvents(); 1402ca7a0(busy=1); processEvents()      // UI, skip
if (endsWithSuffix(file, "moproj"))      P+0x18 = QDir::cleanPath(file)
else if (endsWithSuffix(file, ".DB"))    P+0x18 = QDir::cleanPath(changeSuffix(file, "moproj"))
else if (endsWithSuffix(file, ".KEY"))   P+0x18 = QDir::cleanPath(changeSuffix(file, "moproj"))
else return false
P+0x39 = 1   (1400a4150)    P+0x3a = 1   (1400a41f0)
1400a5290(P)        // FOR003 rename offer, see 1.2
if (M) { M->1400cdcdc0(cleanPath(P+0x30), 0); M->140cdc400(); }   // DB name into data root, UI
if (endsWithSuffix(file,"moproj")) P+0x55 = 14008afa0(P)          // the path we spec
else if (".DB")  P+0x56 = 14008b2d0(P,file)   (not followed)
else if (".KEY") P+0x57 = 14008b380(P,file)   (not followed)
QDir::setCurrent(projectDir(P)); 1400aa960(P); busy=0 ...         // UI refresh
return true          // NOTE: true even when the parse failed; success is P+0x55
```

Literals: `"moproj"` (0x14213f7e8/7f0/800/808), `".DB"` (0x14213c414, 0x14213c4d4),
`".KEY"` (0x14213f7f8/810).

* `140818b50 endsWithSuffix(s, ext)`: strip one leading `'.'` from `ext` (`140818e20`), then
  `s.endsWith("." + ext, Qt::CaseInsensitive)`.
* `140818d50 changeSuffix(s, ext)` = `1408192e0(QFileInfo(s).path(), completeBaseName(s), ext)` =
  `cleanPath(path + QDir::separator() + base + "." + ext')` (ext' without leading dot).
* `1400a4220 projectDir(P)` = `cleanPath(QFileInfo(P+0x18).absolutePath())`.
* Caller `14001ca30` (File > Open / `-m open -f`): suffixes `dprj d2prj d3prj d2mo d3mo d2md dmd
  mdt` go to `14001c2f0` instead (out of scope). For other files it creates the data root `M` for
  DB name `changeSuffix(file, ".DB")` (`140c79390`), then `P = M->140044a90()` (creates P on first
  use, ctor `140089880`), then `14008a9b0(P, file)`. Afterwards it shows licence-type messages
  for DOE/optimisation sub-projects (`140010330`, `140010360`; fenced, not followed).

### 1.1 Project ctor `140089880(P, M, dir, base)` (defaults the open path relies on)

| Field | Value set by ctor |
|---|---|
| P+0x00 int | 0 (`140906740`, KEY tokenizer state, see 7.1) |
| P+0x08 | M |
| P+0x10 int | 5 |
| P+0x18 QString path | `cleanPath(dir + sep + base)` (no suffix) via `14008a470`, which also sets P+0x30 |
| P+0x30 QString | `cleanPath(changeSuffix(P+0x18, ".DB"))` (`1400a4cc0`) |
| P+0x28 QString title | `base` |
| P+0x24 int GEOTYP | 0 |
| P+0x20 int unit | global `0x143d79724` via `1400a2150` (Q2) |
| P+0x38 byte | 1;  P+0x39 byte 1 (or 0 for "NewProject"); P+0x3a byte 0 |
| P+0x3c int | 5;  P+0x40 byte 0 |
| P+0x48 | `new QDomDocument("ProjectFile")` with skeleton elements (replaced by setContent) |
| P+0x50 int | 0 (active sub-project index) |
| P+0x54..0x57 bytes | 0 |
| P+0x58 QList<SUB*> | one entry: `1400b5470(new 0x50, P, type 0, id 0)` = main sub-project |
| P+0x60 | 0x14a0-byte struct (`1412f10d0`), filled by `1400aa960` after open (Q6) |

`dir`/`base` come from M: `140cdd4f0` = absolutePath(M+0x28) if M+0x20 else null,
`140cdd620` = completeBaseName(M+0x28). M+0x28 is the DB path, so P+0x18 starts as
`<dir>/<base>` and is then overwritten by 14008a9b0.

### 1.2 `1400a5290` FOR003 offer (UI, must not run in the DLL)

If `!QFile::exists(P+0x30)` and `QFile::exists(cleanPath(projectDir + sep + "FOR003"))` and
`1400a56a0(P, P+0x30)` is false: QMessageBox "The primary database of this project now exists as an
outdated FOR003 file ..." and on Yes `QDir(projectDir).rename("FOR003", P+0x30)`. The DLL skips
it (it renames a project file).

## 2. `14008afa0` ReadProjectFile(P) → bool

```
QFile f(QString(P+0x18));
if (!f.open(QIODevice::ReadOnly | QIODevice::Text))          // flags 1|0x10
    { qDebug() << "Error: Cannot read file " << path << ": " << f.errorString(); return false; }
QString err; int line, col;
if (!P->dom(+0x48)->setContent(&f, false, &err, &line, &col)) { f.close(); return false; }
f.close();
return 14008b410(P, *(QDomDocument*)(P+0x48));
```
The parsed DOM stays in P+0x48 and is read again later (sub-project elements by index,
`1400a78e0`; sub-project names for `*!PRJNAM` of DOE runs, `1400a75a0`). Parse rules needed by
the DLL: namespace processing off; elements are matched by exact tag name; `QDomElement::text()`
is the concatenation of descendant text; attribute order is irrelevant. Text mode means CRLF in
the file reads as LF.

## 3. `14008b410` WalkProject(P, doc) → bool

```
root = doc.documentElement();  QString tmp;  int nSub = 0;
QList<int> subType, subId;
for (node = root.firstChild(); !node.isNull(); node = node.nextSibling()) {
  e = node.toElement(); if (e.isNull()) continue;
  tag = e.tagName();
  if      (tag == "Title")           P+0x28 = e.text();                    // NOT the Name attribute
  else if (tag == "GeoTyp")          P+0x24 = e.text().toInt();
  else if (tag == "Unit")            { tmp = e.attribute("Name", QString());
                                       1400a2150(P, tmp == "SI" ? 1 : 2); }   // P+0x20, global 0x143dd9b98
  else if (tag == "Primary_DB_Name") { tmp = e.attribute("Name", QString()); 1400a4da0(P, tmp); }
  else if (tag == "Sub_Project")     { subType.append(e.attribute("Type", QString()).toInt());
                                       int id = e.attribute("Id", QString::number(-1)).toInt();
                                       if (id == -1) id = (nSub == 0) ? 0
                                          : e.attribute("Name", QString()).split(" ").last().toInt();  // only if list non-empty
                                       subId.append(id); nSub++; }
  else if (tag == "Object_List")     14008e290(P, e, ObjManager(P));      // 1400a58f0 → M+0x19d0
  else if (tag == "Material_List")   14008e290(P, e, MatManager(P));      // 1400a5930 → M+0x19d8
  else if (tag == "Lubricant_List")  14008e290(P, e, LubManager(P));      // 1400a59b0 → M+0x19e8
  else if (tag == "Equipment_List")  14008e290(P, e, EqpManager(P));      // 1400a5970 → M+0x19e0
  else if (tag == "Operation_List")  { if (!14008cb50(P, e)) return false; }
}
for (i = 0; i < nSub - 1; i++)                  // sub-projects 1..nSub-1 (0 exists from the ctor)
  P+0x58.append(1400b5470(new 0x50, P, subType[i+1], subId[i+1]));
for (i = 1; i < nSub; i++)
  if (sub[i] && !sub[i]->loaded(+0x08)) { e = 1400a78e0(P, i); 14008f200(P, e, i); }
return true;
```
Literals (0x14213f834..0x14213f8d0): `Title GeoTyp Unit Name Primary_DB_Name Name Sub_Project Type
Name Object_List Material_List Lubricant_List Equipment_List Operation_List`, `"Id"`
(0x14213cb7c), `"SI"` (0x14213c61c), `" "` (0x14213ccec).

Notes
* The main `Sub_Project` (index 0) keeps type 0 / id 0 from the ctor; its attributes are ignored
  here. Its children are parsed only on activation (section 8).
* Title: `<Title Name=""/>` gives an empty title (`text()` of an empty element). All 51 v11 MSTs
  have `*!TITLE  ` with nothing after it. `*!GEOTYP` = P+0x24 (0 in all files, no `GeoTyp`
  element in the corpus), `*!UNIT` = P+0x20 (SI → 1, anything else → 2; matches all MSTs).
* `1400a4da0 SetPrimaryDb(P, name)`: if `name` non-empty: `p = cleanPath(projectDir + sep +
  name)`; if `QFile::exists(p)`: P+0x30 = p (and `exists(P+0x30)` again, result unused). If the
  file does not exist P+0x30 keeps the ctor value `<dir>/<base>.DB`. So P+0x30 depends on the
  file system (Q3).
* `1400a2150` also sets global `0x143dd9b98` = (unit==2) and calls `1409dd680(unit)` (global unit
  state of the DEF_PRE engine; the DLL keeps it per call).

## 4. Item lists: `14008e290` / `14008e3d0`

`14008e290(P, listElem, mgr)`: for each child element with tag `"MoItem"` call
`14008e3d0(P, item, mgr)`. The `Number` attribute is ignored.

`14008e3d0 ParseMoItem(P, e, mgr)`:
```
QString name, path, data, matId, matType, phaseList; int id = 0, objType = 0;
bool hasData = false, hasInfo = false, hasMat = false, hasGeo = false, hasMesh = false;
for child c:  tag =
  "Name"      → name = c.text()
  "Id"        → id = c.text().toInt()
  "Path"      → path = c.text()                         // never used afterwards
  "Data"      → data = c.text(); hasData = true
  "ObjectInfo"→ hasInfo = true; objType = attr("ObjectType").toInt();
                hasGeo = attr("HasGeo").toInt() != 0; hasMesh = attr("HasMesh").toInt() != 0;
                matId = attr("MatId")
  "MatType"   → hasMat = true; matType = c.text()
  "PhaseList" → hasMat = true; phaseList = c.text()
if (name.isEmpty()) return true;                        // item skipped
QString file = mgr->vt[0x178](id);                      // KEY path from Id, see below
SfMoItem* it;
if (!hasData) it = mgr->vt[0x198](name, file, &id);     // 14006c9c0 for all four managers
else { bytes = qUncompress(QByteArray::fromHex(data.toAscii()));
       mkpath(QFileInfo(file).absolutePath());         // 14081afb0, message box on failure
       QFile f(file); if (f.open(WriteOnly)) { f.write(bytes); f.close();
                                               it = mgr->vt[0x198](name, file, &id); } }
if (it && hasInfo && (o = dynamic_cast<SfMoObjItem*>(it))) {
   OBJ* d = o->+0x40;  d+0x04 = objType; d+0x18 = hasGeo; d+0x19 = hasMesh;
   d+0x20.clear();  if (!matId.isEmpty())
      for tok in matId.split(" ") (KeepEmptyParts): d->addMatId(tok.toInt()) }  // 14004dc20 adds if absent
if (it && hasMat && (m = dynamic_cast<SfMoMatItem*>(it))) {
   m+0x40 = (matType=="Single") ? 0 : (matType=="Mixture") ? 1 : 2;     // 140041eb0
   if (m+0x40 == 1) { m+0x48.clear(); for tok in phaseList.split(" "): m+0x48.append(tok.toInt()); } }
```
Note the decompile of the ObjectInfo loop shows `14004dc20(d)` with the argument dropped; by
analogy with `140093c30` it adds `tok.toInt()`. An empty `MatId=""` adds nothing (the split is
skipped). A non-empty value with double spaces gives empty tokens → id 0 (KeepEmptyParts).

`mgr->vt[0x198]` = `14006c9c0(mgr, name, file, &id)`: `it = mgr->vt[0x170](id)` (new item with
`it+0x18 = id`), `it->vt[0x40](name)` (setText), `it+0x28 = file` (`14006ba30`),
`invisibleRootItem()->appendRow(it)`. Item order = document order.

KEY path per manager (`vt[0x178]`): `"%1%2.KEY".arg(kind, 0, ' ').arg(id, 5, 10, '0')` appended
to `listDir + QDir::separator()`, **not cleaned afterwards**:

| Manager (class, M offset) | vt+0x178 | kind | listDir (cleanPath(projectDir + sep + X)) | item class (vt+0x170) |
|---|---|---|---|---|
| `SfMoObjManager` M+0x19d0 | `140068e60` | `Object` | `1400a4510` `Objects` | `SfMoObjItem` 0x48 (`140068df0`) |
| `SfMoMatManager` M+0x19d8 | `140042850` | `Material` | `1400a4640` `Materials` | `SfMoMatItem` 0x50 (`1400427e0`) |
| `SfMoEqpManager` M+0x19e0 | `1400bef30` | `Equipment` | `1400a4770` `Equipments` | `SfMoEqpItem` 0x40 (`1400beec0`) |
| `SfMoLubManager` M+0x19e8 | `1400bd910` | `Lubricant` | `1400a48a0` `Lubricants` | `SfMoLubItem` 0x40 (`1400bd8a0`) |

On Windows `QDir::separator()` is `'\\'`, so the stored path is e.g.
`C:/PROBLEM/X/Objects\Object00001.KEY` (forward slashes from cleanPath, one backslash). Any MST
writer that prints it through `QDir::relativeFilePath` (Qt 4 cleans with `fromNativeSeparators`)
gets `Objects/Object00001.KEY`; printing it raw would keep the backslash (s1-06 must reproduce
Qt 4 exactly).

## 5. Operations: `14008cb50` / `14008ccf0`

`14008cb50(P, listElem)`: `n = 0`; for each child with tag `"Operation"`: `n++`;
`if (!14008ccf0(P, op, true, n)) return false`. `Number` ignored.

### 5.1 `14008ccf0 ParseOperation(P, e, bool withProps, int ordinal)` → bool

```
QString lubMethod, name, type, subType; int id = 0; int oprFlag = 1; SfMoOprItem* item = 0;
oprMgr = 1400a58b0(P)  (= M+0x19c8, SfMoOprManager)
for child c (document order): tag =
 "Name"    → name = c.text()
 "Id"      → id = c.text().toInt()
 "Type"    → type = c.text();  diagName = type + "DiagItem";
             if (id == 0) id = ordinal;
             item = 140046cc0(oprMgr, name, type, diagName, id, 0)      // factory, 5.2
 "SubType" → subType = c.text(); OPI(item)->140050d50(subType)          // OPI+0x128 if different
 "OprFlag" → oprFlag = c.text().toInt()
 "Objects" → 140093c30(P, c, item)                                      // 5.4
 "SimCtrlKey"     → 1400937b0(P, c, 140059290(OPI))   // legacy: write <Data> to TaskN/SimCtrl.KEY
 "InterObjectKey" → 1400937b0(P, c, 14005a690(OPI))   // legacy: write <Data> to TaskN/InterObject.KEY
 "Equipment" → OPI+0x4c = c.attribute("Id").toInt()                     // 14004ca30
 "Lubricant" → OPI+0x48 = c.attribute("Id").toInt();                    // 1407b5170
               lubMethod = c.attribute("InterObjectMethod");
               OPI+0xe0 = (lubMethod == "System")                       // 140065790 + 1400ab6f0
 "ActionKeywords" → 140095cc0(P, c, item)                               // 7
 "NSTART"         → 140095e40(P, c, item)                               // 5.5
 "UserDefinedKey" → 140096010(P, c, item)
 "RestartKey"     → 1400961d0(P, c, item)
 "Properties"     → if (withProps) 140094f70(P, c, item)               // 6.3
OPI = item ? item+0x48 : 0
if (!OPI || !OPI+0x10 /*PROPS*/) {
   qCritical() << "Error on reading project file!";
   qCritical() << "   " << item->vt[0x48]() /*OPI+0x118*/ << item+0x50 /*type+"DiagItem"*/;
   return false;                                   // aborts the whole open (P+0x55 = 0)
}
OPI+0x10c = oprFlag                                 // 140050930
PROPS->vt[0xc0]()                                   // class post-load hook, 6.4
return true
```
Literals 0x14213f904..0x14213f9e8: `Name Id Type SubType OprFlag Objects SimCtrlKey InterObjectKey
Equipment Lubricant InterObjectMethod System ActionKeywords NSTART UserDefinedKey RestartKey
Properties`, `"DiagItem"` (0x14213f918), `"Error on reading project file!"`.

Order dependence: `Name` and `Id` must precede `Type` (they do in every corpus file; a child
before `Type` that needs `item` would dereference null in the original — the DLL should fail with
`M2M_E_PARSE`). An unknown `Type` (no registry entry) makes PROPS null → open fails.

### 5.2 Factory `140046cc0 CreateOperation(oprMgr, name, type, diagName, id, src=0)`

```
row0 = oprMgr->invisibleRootItem();
item = oprMgr->vt[0x170](id)       // 1400468e0 → new SfMoOprItem(0x58) 140045c30:
                                   //   item+0x18 = id, item+0x40 = P (14006c210(mgr) = mgr+0x20),
                                   //   item+0x48 = new SfOperationInfo(P, id)   (14004fcd0, 5.3)
blank = new QStandardItem (vtable 0x14210eb18)
cls = type; legacy = "";
if (type.contains("SfOprHeatExpress") || type.contains("SfOprAirConvect") ||
    type.contains("SfOprHeatResting") || type.contains("SfOprHeatDwelling"))   // case-sensitive
   { cls = type.contains("2D") ? "SfOprHeatTransferExpress2D" : "SfOprHeatTransferExpress";
     legacy = type; }
if (type == "SfOprMultiBlow") cls = "SfOprMultiBlow2D";
OB = 140047c00(oprMgr, cls)        // registry lookup, 5.6; qCritical "Error in operation manager",
                                   // "   " "Cannot get OprBase for " cls when missing
if (OB) { displayName = OB->vt[0x88](); OPI->1400511f0(OB->vt[0x98]()) }   // OPI+0x10 = new PROPS,
                                                                          // PROPS+0x20 = QPointer(OPI)
uname = 140047630(oprMgr, name.isEmpty() ? displayName : name)            // unique name, 6.2
blank->setText("");  item->vt[0x40](uname) /*setText*/;  row0->appendRow([blank, item]);
OPI+0x118 = uname   (140050a90)
OPI+0x120 = cls     (140050c80; if changed: clear OPI+0x160 model if any)
OPI+0x128 = legacy  (140050d50; only when different)
item+0x50 = diagName (14004c9a0) — built from the original Type text
return item
```
Literals: `SfOprHeatExpress` 0x14211d580, `SfOprAirConvect` 0x14211d598, `SfOprHeatResting`
0x14211d5a8, `SfOprHeatDwelling` 0x14211d5c0, `2D` 0x14211a8dc, `SfOprHeatTransferExpress2D`
0x14211d5d8, `SfOprHeatTransferExpress` 0x14211d5f8, `SfOprMultiBlow` 0x14211d618,
`SfOprMultiBlow2D` 0x14211d628.

### 5.3 `SfOperationInfo` ctor `14004fcd0(OPI, P, id)` and `140050550` defaults

```
QObject base; vtable 0x1421247d8; OPI+0x38 QPointer; OPI+0x40 QList<int>; OPI+0x50/0x60 lists
(140066860 sets list+8 = 1); OPI+0x70 InterObject block (1400651f0); QStrings at +0x118 +0x120
+0x128 +0x130 +0x148 +0x158; OPI+0x160 QPointer
140050550: OPI+0x110 = 0; OPI+0x10c = 0; 140051250: OPI+0x10 = 0, +0x48 = -1, +0x4c = -1,
           +0xe8 = 0.0, +0xf0 = 0, +0xf8 = 0;  14005fa60: OPI+0x30 = 0;
           14005a370: OPI+0x18 = 0, OPI+0x1c = 0;  OPI+0x140 = 0; OPI+0x150 = 0;
           OPI+0x158 = "RestartStep.KEY"; OPI+0x20 = new QList; OPI+0x28 = new QList; OPI+0x160 = 0
OPI+0x100 = P
140050820(OPI, id): OPI+0x108 = id; if (id >= 1) { OPI+0x130 = "Operations/Task" + sprintf("%05d", id);
                     140051190: mkpath(taskDirAbs(OPI)); 14008c630(P) }   // side effects, skip in DLL
```
`14008c630(P)` mkpaths `Operations`, `Objects`, `Materials`, `Equipments`, `Lubricants` under
the project dir and `DoeStudies` subfolders when the active sub-project needs it.

`140050df0 taskDirAbs(OPI)`: `P = OPI+0x100`; if null → null string; else
`QDir::convertSeparators(cleanPath(projectDir(P) + sep + OPI+0x130))` — native separators, i.e.
backslashes on Windows. All task file paths below are built from it
(`taskDirAbs + sep + name`, then `cleanPath`, which turns them back into forward slashes).

| Helper | File |
|---|---|
| `140059290` | `cleanPath(taskDirAbs + sep + "SimCtrl.KEY")` |
| `14005a690` | `cleanPath(taskDirAbs + sep + "InterObject.KEY")` |
| `14005eb10(OPI, slot)` | `cleanPath(taskDirAbs + sep + "%1%2.KEY".arg("ObjProperties").arg(slot+1, 5, 10, '0'))` |

### 5.4 Objects of an operation: `140093c30(P, objectsElem, item)`

```
OPI = item+0x48; slot = 0;
for child c with tag "Object":
  id = c.attribute("Id").toInt();
  14005fc80(OPI, slot, id)        // OPI+0x40[slot] = id; pads with -1 up to slot
  d = 140061250(OPI, slot)        // OPI+0x50[slot], created on demand: new OBJ (14004d160),
                                  // d+0 = slot+1, d+0x30+0x104 = slot+1 (14004d520/14004e810)
  if (d) {
    d+0x10 = attr("Name"); d+0x04 = attr("ObjectType").toInt();
    d+0x18 = attr("HasGeo").toInt() != 0; d+0x19 = attr("HasMesh").toInt() != 0;
    d+0x28 = attr("EquipmentID").toInt();
    d+0x20.clear(); for tok in attr("MatId").split(" "): d->addMatId(tok.toInt())  // only if MatId non-empty
    d+0x1b9 = attr("Remesh").toInt() != 0          // 14004df60
    d+0x1b8 = attr("SkipDefine").toInt() != 0      // 14004df20
    d+0x1ba = attr("MeshSetting").toInt() != 0     // 14004dfa0
    d+0x1bb = attr present("Mesh2dAbsolute") ? toInt != 0 : 0            // 14004dfe0
    d+0x1bc = attr present("Mesh3dFinerInternal") ? toInt != 0 : 0       // 14004e020
    d+0x1be = attr present("DefineBCC") ? toInt != 0 : 0                 // 14004e0a0
    d+0x1bf = attr("AssignMovement").toInt() != 0  // 14004e0e0
    d+0x1c0 = attr("DefineRotationMovement").toInt() != 0   // 14004e120
    d+0x1c1 = attr("ThermalStop").toInt() != 0     // 14004e160
    140093ab0(P, c, OPI, slot)    // ObjPropertiesKey
  }
  slot++
```
(The order of the setter calls is: Name, ObjectType, HasGeo, HasMesh, EquipmentID, MatId, then the
nine flags in the order Remesh, SkipDefine, MeshSetting, Mesh2dAbsolute, Mesh3dFinerInternal,
DefineBCC, AssignMovement, DefineRotationMovement, ThermalStop.) "present" = attribute value
non-empty. Attribute literals 0x14213fe34..0x14213ff40.

`140093ab0`: for each child `ObjPropertiesKey`: `1400937b0(P, child, 14005eb10(OPI, slot))`.

`1400937b0 WriteBlob(P, e, path)`: for each child `"Data"`: `bytes = qUncompress(
QByteArray::fromHex(text.toAscii()))`; `mkpath(QFileInfo(path).absolutePath())`; `QFile(path)
.open(WriteOnly)` (truncate) and write the bytes. The last `Data` child wins. `qUncompress`
format: 4-byte big-endian length then zlib. In COG_P1_B1 the first blob decodes to the 3-line
KEY header plus `OTPRNG`/`OPSTOP` with LF line ends. DLL: store `{path → bytes}` in the overlay
(Q1).

Slot semantics (used by writers): `OPI+0x40[slot]` = project object id (`>0`), `-2` = "Read From
DB" (`1400606c0`), `-1` padding; `14005fb30(OPI, slot)` returns it or `-99` (0xffffff9d) when the
slot is invalid (`140060600`); `140060640` = id > 0 or id == -2.

### 5.5 NSTART, UserDefinedKey, RestartKey

* `140095e40`: `found = false`; for child `"StartStepNo"`: `OPI+0x1c = text.toInt()`, `found = true`;
  then `OPI+0x18 = found` (`14005a2c0`). `14005a300(OPI)` = `!OPI+0x18 ? 0 : (v = OPI+0x1c,
  v < 0 ? -1 - v : v)`.
* `140096010`: for child `"KeyFileName"`: `OPI+0x148 = text`, found; `OPI+0x140 = found`.
* `1400961d0`: for child `"KeyFileName"`: `OPI+0x158 = text`, found; then
  `140064760(OPI, found, false)`: `OPI+0x150 = found` (signal suppressed). `140064740(OPI)` returns
  OPI+0x150. Without the element the defaults stay (`RestartStep.KEY`, 0).

## 6. Factory registry, names, properties

### 6.1 Registry (`14006dba0`, insert `14006f940`)

At start-up `14006dba0` builds one singleton per class and calls `14006f940(reg, OB)`:
`OB->vt[0x180]()` (init, sets the class name in `OB+0x28` via `1405fd780`), `info =
OB->vt[0x188]()`, `info->vt[0x70](reg)`, `map1[info->vt[0x38]()] = info` (reg+8 → +0x8),
`map2[name] = OB` (reg+8 → +0x150). `140047c00`/`140054e90`/`140070650` look up `map2`
(`QMap<QString,SfOprBase*>`) by `OPI+0x120` (`140050d00`); the registry object is
`mainWindow+0x2b8` (`1400135a0`).

Registered names (= valid `OPI+0x120` values) and the class that writes them:

| Key | Class (vtable) | init | MST writer (+0x280) |
|---|---|---|---|
| `SfOprForming` | SfOprForming | `140c05bc0` | `1405f5340` |
| `SfOprForming2D` | SfOprForming2D | `140c043e0` | `1405f5340` |
| `SfOprHeatTransfer` | SfOprHeatTransfer | `141353ec0` | `1405f5340` |
| `SfOprHeatTransfer2D` | SfOprHeatTransfer2D | `1413530d0` | `1405f5340` |
| `SfOprHeatTransferExpress` | SfOprHeatTransferExpress | `1413e6060` | `1405f5340` |
| `SfOprHeatTransferExpress2D` | SfOprHeatTransferExpress2D | `1413e57a0` | `1405f5340` |
| `SfOprHTFurnace` | SfOprHTFurnace | `140c65420` | `1405f5340` |
| `SfOprCogging` | SfOprCogging | `140750140` | `1407520c0` |
| `SfOprBoolean2D` / `SfOprBoolean3D` | SfOprBoolean (`OB+0x60` = 1 / 0, ctor `1411de8d0`) | `1411de9b0` | `1411df600` |
| `SfOprDistortion2D` / `SfOprDistortion3D` | SfOprDistortion (`OB+0x60` = 1 / 0, ctor `141361f70`) | `141362050` | `141362e90` |
| `SfOprMultiBlow2D` | SfOprMultiBlow2D | `140600a50` | `140602810` |
| `SfOprMultiBlow3D` | SfOprMultiBlow | `140601480` | `140602810` |
| `SfOprPre2D` | SfOprPre2D | `1413474d0` | `1405f5340` |
| `SfOprPre3D` | SfOprPre | `1413489d0` | `1405f5340` |
| `SfOprFormingExpress` | SfOprFormingExpress | `141413000` | `1414136b0` |
| `SfOprFormingExpress2D` | SfOprFormingExpress2D | `1414126f0` | `141412c90` |
| `SfOprHeatTreat` / `SfOprHeatTreat2D` | SfOprHeatTreat / 2D | `1411b0ca0` / `1411ae690` | `1411b1460` / `1411aee40` |
| `SfOprCycle` | SfOprCycle | `14134d2e0` | `14134dcc0` |
| `SfOprGroup` | SfOprGroup | `141352020` | `141352540` |
| `SfOprDoeOutput` | SfOprDoeOutput | `140e2df30` | `140e32970` |
| `SfOprReport` | SfOprReport | `1412ad930` | `1412b05a0` |
| `SfOprShapeOptim` | SfOprShapeOptim | `14013ab80` | `14013fb90` |
| `SfOprConvert2Dto3D` | SfOprConvert2Dto3D | `1401c24d0` | `1401c4440` |
| `SfOprCopyMirror` | SfOprCopyMirror | `140e22470` | `140e24e80` |
| `SfOprDiestress` | SfOprDiestress | `1402b4fb0` | `1402b5410` |
| `SfOprDiestressBM2D` / `SfOprDiestressBM3D` | SfOprDiestressBM2D / 3D (ctors `1400707b0` / `140070810`) | `14029a880` / `14029c6c0` | `14029ad10` / `14029cb50` |

(Insertion order and slots `reg+8 → +0x88..0x148` are in `14006dba0`; the key strings come from
each init's `1405fd780` call. Not registered: `SfOprBase`, `SfOprMultiBlowCommon`,
`SfOprHeatTransferCommon`, `SfOprExpressBase`, `SfOprFormingExpressCommon`, `SfOprDiestressBM`.)

Corpus `Type` values and their resolution:

| `<Type>` | count | OPI+0x120 (class key) | OPI+0x128 after open | `*!SIMTYP` in saved MST |
|---|---|---|---|---|
| SfOprHeatTransfer | 529 | SfOprHeatTransfer | `<SubType>` text | SfOprHeatTransfer |
| SfOprCogging | 337 | SfOprCogging | "" | SfOprCogging |
| SfOprForming / SfOprForming2D | 20 / 19 | same | "" | same |
| SfOprBoolean2D | 14 | SfOprBoolean2D (class SfOprBoolean, 2D) | "" | SfOprBoolean2D |
| SfOprCycle | 2 | SfOprCycle | "" | (cycle writer) |
| SfOprDoeOutput | 1 (BEND_2D, not in the chart) | SfOprDoeOutput | "" | not written |
| SfOprAirConvect | 2 | SfOprHeatTransferExpress | "SfOprAirConvect" (no SubType element) | SfOprAirConvect |
| SfOprAirConvect2D | 3 | SfOprHeatTransferExpress2D | "SfOprAirConvect2D" | SfOprAirConvect2D |
| SfOprHeatResting2D | 2 | SfOprHeatTransferExpress2D | "SfOprHeatResting2D" | SfOprHeatResting2D |
| SfOprHeatExpress | 1 | SfOprHeatTransferExpress | "SfOprHeatExpress" | SfOprHeatExpress |

So for the express family the MST type line evidently comes from OPI+0x128 (or the diagram
`ClassName`), not from OPI+0x120; s1-02/s1-03 to confirm in `140131c10` / the writer (Q8).
`1400b3190` (registry helper used by the express wizard, not by open) shows the reverse mapping
(`SfOprHeatDwelling(2D)`, `SfOprHeatResting(2D)`, `SfOprFormingExpress(2D)`, `SfOprDistortion2D/3D`).

### 6.2 Unique names `140047630(oprMgr, name)`

```
cand = name; i = 1;
while (oprMgr->vt[0x1e0](cand)) {              // 14006d040: any row's OPI+0x118 == cand
  cand = name + (name == "Cycle" ? "_%1" : " (%1)").arg(i + 1, 0, 10, ' ');
  i++;
}
return cand;
```
Duplicate `<Name>`s are therefore renamed on open (second "X" → "X (2)"). The only corpus project
with duplicates (56, `Multiple_Operations_Example1`, saved 2013 by an older GUI) has an MST with
both names unchanged, so this cannot be checked against oracle A (Q9).

### 6.3 Properties: `140094f70` / `1400952c0`

```
PROPS = OPI+0x10; tree = PROPS+0x10; tree->vt[0x8]();          // reset
roots = 140353770(tree)                                         // QList of top-level QtProperty*
for child c with tag "Property": 1400952c0(P, c, roots)
```
`1400952c0 ApplyProperty(P, e, candidates)`: reads children `Name`, `TypeName`, nested
`Property`, `Value` in document order:
* `Name`: find the candidate whose name equals it. Legacy rule: if the XML name contains
  `HeatingExpress`, `AirConvection`, `HeatResting` or `HeatDwelling`, accept the first candidate
  whose name contains `TrasnferExpress` or `HeatingExpress`. If nothing matches, the rest of the
  element is ignored.
* `TypeName`: remembered.
* nested `Property`: recurse with the matched node's children (`140370f60`).
* `Value`: `double` → `QVariant(text.toDouble())`, `int` → `QVariant(text.toInt())`, anything else
  (`doubleUnits`, `enum`, `bool`, `group`, ...) → `QVariant(text)` as a string, then
  `140370600(node, variant)`.
Only names that already exist in the class's property tree are applied; the tree shape and
default values belong to each `SfOpr*Properties` class (built by `OB->vt[0x98]`), which the
writer packets must take from their class (Q10). Literals: `Property Name TypeName Value double
int HeatingExpress AirConvection HeatResting HeatDwelling TrasnferExpress` (0x14213ff70..0x14214000c).

### 6.4 Post-load hook `PROPS->vt[0xc0]()` (KEY-type reads during open)

| PROPS class | +0xc0 | Effect |
|---|---|---|
| SfOprBaseProperties and most others | `140cac130` | none |
| SfOprCoggingProperties | `140760430` → `140761c90` → `140761d90` | reads `taskDir\Cogging.MDT` (`fopen "rt"`, `fscanf "%s %d"`, `"%s %lg"`, ...; "FILE MISSING!"/"FILE CORRUPT!" dialogs) into the cogging data (`PROPS+0x40`, fields +0x0..+0x530) |
| SfOprBooleanProperties | `1411e09d0` → `1411e1cd0` → `1411e1dd0` | reads `taskDir\Boolean.BWZ` (`WZD_MACHDIST_WORKPIECE_OBJECT_NO`, `WZD_BOOLEAN_CUTTER_OBJECT_NO`, `WZD_BOOLEAN_REMESH`, `$END`) |
| SfOprExpressProperties (HeatTransferExpress, FormingExpress) | `141402060` → `1413f8280` → `1413f83e0` | reads `taskDir\Forming.FWZ` |
| SfOprCycleProperties | `14134ec40` | reads `taskDir\Cycle.CWZ` with the KEY tokenizer (`1409068f0`): `WZD_CYCLE_COUNT n` → `*(PROPS+0x38) = n` (`14134ee90`), until `DONE` |
| SfOprHeatTreat / Diestress / DiestressBM / Distortion | `1411c0940` / `1402b95a0` / `14029eea0` / `141365c60` | not followed (no corpus) |

Path = `convertSeparators(taskDirAbs + sep + name)` (backslashes). These readers belong to the
class specs (s1-03 Cogging/HeatTransfer, s1-04 Boolean/Cycle, s1-05 Express). A missing file
leaves defaults (and shows a dialog for Cogging).

## 7. ActionKeywords: `140095cc0` → `140096390`

`140095cc0`: for each child `"Keyword"`: `140096390(P, c.text(), item)`. Each `Keyword` text is
parsed separately; the raw text is not stored.

`140096390 ParseKeywordText(P, text, item)`:
```
QTextStream ts(&copy_of_text, ReadWrite); OPI = item+0x48; if (!OPI) return;
do {
  n = tok(P, ts, 2 /*upper word*/, kw, -5000)        // always starts a new line; n = tokens on it
  if      (kw == "TLOC")   { double v = 0; tok(10, &v); OPI+0xe8 = v; }          // 1400abc90
  else if (kw == "OBJPOS") { o = 140061e30(OPI, 0);  // new 0xd8 record appended to OPI+0x60
                             int a = 0; tok(5,&a); o+0x04 = a;  int m; tok(5,&m); o+0x00 = m;
                             switch (m) { ... see 7.2 }
                             lim = is2D ? 7 : 10;  at = is2D ? 8 : 11;   // is2D = 14005f150(OPI)
                             if (n > lim) { int k = 0; tok(5, &k, at);   // token number `at` of the line
                                 if (k > 0) { o+0xd0.clear(); int v; tok(5,&v,-5000) /*next line*/;
                                              o+0xd0.append(v); for (j=1;j<k;j++){tok(5,&v); append} } } }
  else if (kw == "INICTC") { int i = 0; tok(5,&i); OPI+0x70+(i-1) = 1; OPI+0xd4 = 1; }   // 140065270, 1400ab7c0
  else if (kw == "GENCTC") { OPI+0xd5 = 1; double g = 0; tok(10,&g); OPI+0xd8 = g; }      // 1400ab7e0, 1404ce5b0
  else if (kw is per-object) { ... 7.3 }
} while (kw != "DONE");                                // tok returns "DONE" at end of text
```
INICTC with `i <= 0` writes before the array (undefined in the original; the DLL should reject).

### 7.1 Tokenizer `140906e40 tok(P, ts, type, out, idx)`

DEF_PRE-style KEY line reader with global state (buffer `0x143dd9250` 2000 bytes, token table
`0x143dd8e48`, counters `0x14386f86c` (next index), `0x143dd8e30` (token count)); make it per call.
* `idx == -5000` or no tokens left: read lines (`readLine(2000)`) until a line whose first token
  is not null and does not start with `'*'`; an empty read (end of text or a blank line) returns
  -2 and, for type 2, sets `out = "DONE"`. The line is copied as ASCII (max 1999), cut at `"//"`,
  and split with `strtok` on `" ,:=\n\t\r"`. If `P+0` == 1 the line is cut at column 63 (it is 0
  here). Returns the token count for `-5000`.
* `idx > 0`: returns token number `idx` of the current line (1-based), -1 if out of range.
* Otherwise: next token of the current line.
* Types: 1 bool (`atoi != 0`), 2 string upper-cased (`toupper`), 3/4/12 string, 5/6/7 `int` (atoi),
  8 `int` (strtol base 10), 9 float (`D`/`d`→`E`, atof), 10 double (`D`/`d`→`E`, strtod),
  11 skip. (Checked in the bytes: types 5..8 store a 32-bit int.)

A blank line inside a `Keyword` text therefore ends that text's parse (`DONE`).

### 7.2 OBJPOS record (0xd8, ctor `1400658e0`; `o+0` = 1, everything else 0, list at +0xd0)

`is2D` = `OB->vt[0x190]() == 2` (`14005f150`); `is3D` = `== 3` or no OB (`14005f1c0`). All values
are doubles unless marked int; fields are read in the order listed.

| mode (`o+0`) | fields |
|---|---|
| 1, 11 | +0x30, +0x38; if is3D: +0x40 |
| 2, 12, 102, 112 | if !is2D: +0x10, +0x18, +0x20, +0x28, int +0x08; else: +0x10, int +0x08, +0x18, +0x20 |
| 3 | +0x48, +0x50; if is3D: +0x58, +0x60, +0x68, +0x70; then +0x78 |
| 4 | if is2D: +0x80, +0x88, +0x98, +0xa0 |
| 6 | if is3D: +0x60, +0x68, +0x70; then +0x78 |
| 7, 8, 9 | if is3D: +0x10, +0x18, +0x20, +0x28 and, for 9, +0x60, +0x68, +0x70 |
| other | none |

`o+0x04` = object number (first int), `o+0xd0` QList<int> optional list. Accessors: count
`140062070(OPI)` (OPI+0x60), item `1400620a0(OPI, i)`, list count `1400666e0(o)`.
`OB->vt[0x190]`: base `1405f5bf0` = from `OB->vt[0x188]()->vt[0x58]()` flags (`&3==3` → 4,
`&1` → 2, `&2` → 3, else 0); Cycle `14134dc50` (engine model dimension, else base); DoeOutput
`140e329d0`; ShapeOptim `14013fbf0`; Diestress family `1402b76b0`, `14029c4e0`, `14029e2e0`;
Report `1412b0600`.

### 7.3 Per-object keywords (target `s = OBJ(slot n-1)+0x30`, via `1400612d0(OPI, n)`)

Each reads `n` first; values are applied only when `n > 0` and the slot exists.

| Keyword | Tokens read | Sets |
|---|---|---|
| NDTMP | int n, int skip, double v | s+0x00 = 1, s+0x08 = v |
| STRAIN | int n, int skip, double v | s+0x10 = 1, s+0x18 = v |
| STRESS | int n, int skip, double v | s+0x20 = 1, s+0x28 = v |
| DAMAGE | int n, int skip, double v | s+0x30 = 1, s+0x38 = v |
| URZ | int n, int skip, double a, double b; if is3D: double c | s+0x40 = 1, s+0x48 = a, s+0x50 = b, s+0x58 = c |
| DRZ | int n, int skip, double a, double b; if is3D: double c | s+0x60 = 1, s+0x68 = a, s+0x70 = b, s+0x78 = c |
| GRAIN | int n, 4 ints skipped, double v | s+0x80 = 1, s+0x88 = v |
| DENSTY | int n, int skip, double v | s+0xb0 = 1, s+0xb8 = v |
| SVSCAL | int n, int a, int b | s+0x180 = 1; `1400aba20(s, a, b, ts)`: only `a == 1`: s+0x118 = 7, s+0x124 = b, then `1400abab0` reads the array from the following lines of the same text with a fresh tokenizer (type 10 values; layout from `s+0x108`) |
| AVGSTR | int n, double v | s+0xc0 = 1, s+0xc8 = v |
| LMTSTR | int n, double v | s+0xd0 = 1, s+0xd8 = v |

Keyword literals 0x14214004c..0x1421400b4 (`TLOC OBJPOS INICTC GENCTC NDTMP STRAIN STRESS DAMAGE
GRAIN DENSTY SVSCAL AVGSTR LMTSTR DONE`), `URZ` 0x14213f57c, `DRZ` 0x14213f58c. Comparisons
are `strncmp(kw, lit, 80)` on the upper-cased token. SVSCAL is the twin of the DEF_PRE
state-variable reader (kw-g2sv port); s1-07 should reuse it.

## 8. Sub-projects and the flow chart

### 8.1 SUB record (0x50, ctor `1400b5470(SUB, P, type, id)`)

| Off | Type | Meaning |
|---|---|---|
| +0x00 | `SfMoDiagEditorModel*` | flow-chart model (`140070f10`, 0x58); the interface used below is at model+0x10 (vtable `0x14213ba88`) |
| +0x08 | byte | loaded (diagram parsed) |
| +0x10 | P | project |
| +0x18 | int | Type (0 main, 2 DOE, 3 optimisation, 4 die stress, other "Subproject") |
| +0x1c | int | Id |
| +0x20 | ptr | DOE study (0xb8, `1400d1180`) when type 2 or 3, else 0 (`1400b5a90`) |
| +0x28 | ptr | optimisation data (0x50, `140917ff0`) when type 3 |
| +0x30 | QPointer | 0 |
| +0x38 | QString | `Subproject_DbName DbName=` (`1400b9f10`) |
| +0x40, +0x48 | | lists (`1413df3f0`, `140015d170`) |

Sub-projects created by the GUI (`1400a6a00`) use `1400b5680` instead, sharing the main model;
open always uses `1400b5470` (own model per sub-project).

### 8.2 Activation `1400a7250(P, idx)` → `1400a7b90(P, idx)` → `14008f530(P, elem)`

`1400a7250` is called from the window code `14002a9b0` (several UI callers) and from
`1400a70b0`; the open path itself does not call it. That sub-project 0 is active and parsed at
save is inferred from the corpus (every v11 MST follows the chain of sub-project 0, 8.3). `1400a7b90`: `e = 1400a78e0(P, idx)` (idx-th `Sub_Project` element of the
DOM, 0-based, document order), `P+0x50 = idx`; if `!sub->loaded`: `P+0x54 = 1`, `14008f530(P,
e)`, `loaded = 1`, `P+0x54 = 0`; then UI signals. The converter must parse sub-project 0 this
way.

`14008f530 ParseSubProject(P, e)` (sub = P+0x58[P+0x50], model = sub+0x00):
```
active = e.attribute("Active").toInt();  QList<Item*> items;  QMap<int,int> next;  k = 0;
for child c:
  "DiagramItem" → next[k] = -1; it = 140091f00(P, c, k, &next); if (k == active) model->vt[0xf8](it->vt[0x28]());
                  items.append(it); k++
  "SfDoeStudy" (sub type 2) → sub+0x20->1400d2aa0(c) ...  Samples_LHS.par / Samples_FF.par /
                  Samples_USR.par paths (1400d7a80 idx 0/1/2) under DoeStudies dir; 1400d7820(DoeStudies)
  "SfDoeStudy" (sub type 3) → optimisation: 1400ba350 ... "<base>.OPT", 140908630 (not followed)
  "Subproject_DbName" → sub+0x38 = c.attribute("DbName")
  (ViewTransform, SfDispView: UI, ignored here)
for (j = 0; j < k; j++) { t = next[j];
  if (t >= 0 && items[j] && items[t])
     model->vt[0x90](items[j]->vt[0x48]()[0] /*out connector*/, items[t]->vt[0x40]()[0] /*in connector*/);  // connect, 140075050
}
```
`140091f00 ParseDiagramItem(P, e, k, &next)`: `Expanded` (compared with "yes", result unused),
`opId = attribute("Operation_Id").toInt()`; children:
* `ClassName`: same legacy mapping as 5.2 (HeatExpress/AirConvect/HeatResting/HeatDwelling →
  HeatTransferExpress[2D]; SfOprMultiBlow → SfOprMultiBlow2D); `it = model->vt[0x58](cls, opId)`
  (create item, `140073900`), `model->vt[0x68](it)` (append to model+0x20 list, `14014cf20`).
* `ItemName` → `it->vt[0x10](text)`; `RectF` → `it->vt[0x90](rect)` (`140097d00`).
* `CellPos Row Col` → `it->vt[0xc0](Row, Col)`.
* `GroupInfo BeginRow BeginCol EndRow EndCol` → `it->vt[0xd0](BeginRow, BeginCol)`,
  `it->vt[0xe0](EndRow, EndCol)`.
* `ItemData` → `140091db0`: for each attribute `it->vt[0xf8](name, value.toInt())` (Cycle:
  `CycleCount`).
* `Connectors` → `140092ff0` → `Connections` → `140093140`: each `ItemIndex` sets `next[k] =
  text.toInt()` (last one wins).
Children that need `it` before `ClassName` would use a null item (does not occur).

### 8.3 MST order: `1400b6630(SUB, QList<int>& ids)` (used by `140131df0`)

Model item interface (vtable of the diagram items): `+0x28` id (Operation_Id), `+0x58` flags
(`0x1000` = group item: Cycle/Group), `+0xc8(&row,&col)` cell, `+0xd8(&r,&c)` group begin,
`+0xe8(&r,&c)` group end. Model (+0x10 interface): `+0x130(it)` successor (first out connector's
first connection's target, `14014d540`), `+0x138(it)` predecessor (`14014d620`).

```
groupOf = {}                                                       // QMap<int,int>
groups = items with flag 0x1000, sorted by (beginRow, beginCol)    // 140085190, comparator 140085110
for g in groups:
   for it in non-group items whose (row,col) lies in [begin, end] (row-major, inclusive),
            sorted by (row, col)                                     // 140085470, comparator 140084a50
       groupOf[it.id] = g.id                                         // later groups overwrite
chain = []                                                           // 140084ad0
plain = non-group items sorted by (row, col); if empty → []
h = plain[0]; while (prev(h)) h = prev(h);  chain = [h]; while (next(h)) { h = next(h); chain.append(h) }
ids = []; last = -1
for it in chain:
   if (!groupOf.contains(it.id)) { ids.append(it.id); last = -1; }
   else if (groupOf[it.id] != last) { ids.append(groupOf[it.id]); last = groupOf[it.id]; }
```
The sort is Qt `qSort` (not stable); cell positions are unique in the corpus, and files without
`CellPos` (55, 56) still work because the head is found by walking predecessors. Items not on
the chain (e.g. BEND_2D's DoeOutput, which has no `DiagramItem`) are not written. There is no
cycle guard in the original (`tools/order.py` adds one).

`140085800(model, opId)` (used through `140050b20(OPI)`) numbers the same chain 1, 2, ... with all
members of a group sharing one number; it caches the map in model+0x40. This is the operation
number written after the simulation number (`CURSIM sim op 0`).

Check: `tools/order.py` implements 8.3 from the XML and compares the operation names with the
`OPRNAM` lines of the saved MST (consecutive duplicates merged): 51 of 51 v11 MSTs match
(366-operation projects 29/30, cycles and groups 47/48/50/51 included); the 2013 MST of project 56
also matches once its duplicate names are taken into account.

## 9. Model table (fields the MST writers need)

### 9.1 Project P (0x1500, created by `140044a90` on M)

| Off | Type | Meaning | Set by | Read by (known) |
|---|---|---|---|---|
| +0x00 | int | KEY tokenizer mode (0) | ctor `140906740` | `140906e40` |
| +0x08 | M* | data root | ctor | `1400a5890` |
| +0x18 | QString | project file path (cleaned) | `14008a9b0` | `14008a4b0` (raw), `14008a500` = completeBaseName → `*!PRJNAM`; `1400a4220` projectDir; `1400a50a0` MST path |
| +0x20 | int | unit 1 = SI, 2 = English | `1400a2150` from `<Unit Name>` | `1400a2180` → `*!UNIT`; `140054ff0(OPI)` |
| +0x24 | int | GEOTYP | `<GeoTyp>` text | `140c0df50` → `*!GEOTYP` |
| +0x28 | QString | title | `<Title>` text | `140ceed20` → `*!TITLE` |
| +0x30 | QString | primary DB path | ctor / `1400a4da0` | `1400a4ee0` (cleanPath copy), `1400a7670` |
| +0x48 | QDomDocument* | parsed file | `14008afa0` | `1400a78e0`, `1400a75a0` |
| +0x50 | int | active sub-project | `1400a7b90` | `1400a73c0`, `1400a7390` (active SUB) |
| +0x55 | byte | opened from .moproj OK | `14008a9b0` | Save `1400980d0` |
| +0x58 | QList<SUB*> | sub-projects | ctor, `14008b410` | `1400a7360` count, `1400a74f0(P,i)` |

M (data root, owner of P): +0x19c0 P (`140044a90`), +0x19c8 `SfMoOprManager` (`140044c80`,
`QStandardItemModel`, 2 columns, column 1 holds `SfMoOprItem`), +0x19d0 objects, +0x19d8
materials, +0x19e0 equipment, +0x19e8 lubricants (all created on first use).

### 9.2 Items

`SfMoItem` (QStandardItem, vtable `0x14212d2d8`): +0x10 QLatin1String kind (`SfMoObjItem`,
`SfMoMatItem`, `SfMoLubItem`, `SfMoEqpItem`, `SfMoOprItem`), +0x18 int id, +0x20/+0x28 QString
(+0x28 = KEY path, `14006ba30`), +0x30 byte; name = item text (`vt+0x40` set, `vt+0x48` get).
`SfMoObjItem` +0x40 → OBJ-like data (ObjectType +0x04, HasGeo +0x18, HasMesh +0x19, MatId list
+0x20). `SfMoMatItem` +0x40 int MatType (0 Single, 1 Mixture, 2 other), +0x48 QList<int> phases.
`SfMoOprItem` (0x58, vtable `0x142119a58`): +0x18 id, +0x40 P, +0x48 OPI, +0x50 QString
`<Type>DiagItem`; `vt+0x48` returns OPI+0x118.

### 9.3 SfOperationInfo OPI (0x168, vtable `0x1421247d8`)

| Off | Type | Meaning | Set by (open) | Accessor |
|---|---|---|---|---|
| +0x10 | PROPS* | `SfOpr*Properties` | factory | `140051230` |
| +0x18 | byte | NSTART given | `140095e40` | `14005a2a0` |
| +0x1c | int | StartStepNo | `140095e40` | `14005a2e0`, `14005a300` |
| +0x30 | ptr | 0 | ctor | |
| +0x38 | QPointer | per-op engine model cache | `14005f820` | `14005f770` |
| +0x40 | QList<int> | object id per slot | `14005fc80` | `14005fa80` count, `14005fb30`, `14005fc50` |
| +0x48 | int | Lubricant Id (-1 default) | `1407b5170` | |
| +0x4c | int | Equipment Id (-1 default) | `14004ca30` | |
| +0x50 | QList<OBJ*> | object data per slot | `140061250` | `140061220` count, `140061250`, `1400612d0(n)`, `140061310` |
| +0x60 | QList<OBJPOS*> | OBJPOS records | `140061e30` | `140062070`, `1400620a0` |
| +0x70..+0xd3 | byte[100] | INICTC flag per object (index n-1); [0] = 1 by default | `140065270` | |
| +0xd4 | byte | INICTC seen (default 1) | `1400ab7c0` | |
| +0xd5 | byte | GENCTC seen (default 1) | `1400ab7e0` | |
| +0xd8 | double | GENCTC value | `1404ce5b0` | |
| +0xe0 | byte | lubricant method "System" | `1400ab6f0` | |
| +0xe8 | double | TLOC | `1400abc90` | |
| +0xf0, +0xf8 | | 0 | ctor | |
| +0x100 | P* | project | ctor | `140055030`; `140055050` active SUB; `140055150` its model; `1400550d0` object manager; `140054ff0` unit |
| +0x108 | int | operation id | `140050820` | `140050910` |
| +0x10c | int | OprFlag (default 1 from the reader) | `140050930` | `1400509d0` (== 0) |
| +0x110 | int | 0 | ctor | |
| +0x118 | QString | name (unique) | factory | `140050ac0`, item `vt+0x48` |
| +0x120 | QString | class key | factory | `140050d00` (registry key, `*!SIMTYP` for most) |
| +0x128 | QString | SubType / legacy Type | factory, `<SubType>` | (s1-02) |
| +0x130 | QString | `Operations/TaskNNNNN` | `140050820` | `140051110`; `140050df0` absolute, native separators |
| +0x140 | byte | UserDefinedKey given | `140096010` | |
| +0x148 | QString | UserDefinedKey file | `140096010` | |
| +0x150 | byte | RestartKey given | `1400961d0` | `140064740` |
| +0x158 | QString | restart KEY (default `RestartStep.KEY`) | `1400961d0` | |
| +0x160 | QPointer | QStandardItemModel (cleared on class change) | | `140054f40` |

### 9.4 OBJ per-object data (0x1c8, ctor `14004d160`, defaults `14004d450`)

| Off | Type | Meaning (open) | Default |
|---|---|---|---|
| +0x00 | int | slot number (1-based) | |
| +0x04 | int | ObjectType | 7 |
| +0x08 | int | ? | 7 |
| +0x10 | QString | Name | null |
| +0x18 / +0x19 | byte | HasGeo / HasMesh | 0 |
| +0x20 | QList<int> | MatId list | empty |
| +0x28 | int | EquipmentID | set by `14004de70` (not decompiled) |
| +0x30 | struct (0x188?) | state variables of 7.3; +0x104 = slot number, +0x108 SVSCAL data | 0 |
| +0x1b8 | byte | SkipDefine | 0 |
| +0x1b9 | byte | Remesh | 0 |
| +0x1ba | byte | MeshSetting | 0 |
| +0x1bb | byte | Mesh2dAbsolute | 0 |
| +0x1bc | byte | Mesh3dFinerInternal | 0 |
| +0x1bd | byte | (not set by the reader) | 0 |
| +0x1be | byte | DefineBCC | 0 |
| +0x1bf | byte | AssignMovement | 0 |
| +0x1c0 | byte | DefineRotationMovement | (not in `14004d450`) |
| +0x1c1 | byte | ThermalStop | 0 |

## 10. Accessors the MST writers use

| Address | Signature | Returns |
|---|---|---|
| `140050ac0` | `QString name(OPI)` | OPI+0x118 (operation name, after uniquifying) |
| `140050b10` | `int f()` | constant 0 (stub) |
| `140050b20` | `int flowNumber(OPI)` | `140085800(model of active SUB, OPI+0x108)`: 1-based chain number (groups share one), 0 if no SUB/model |
| `140050bc0` | `bool isChainHead(OPI)` | true if the op has no predecessor in the active SUB's chain (`140086720` == -1, also when the id is not in the model); false if no model |
| `140050d00` | `QString classKey(OPI)` | OPI+0x120 |
| `140050910` | `int id(OPI)` | OPI+0x108 |
| `140051110` | `QString taskDirRel(OPI)` | OPI+0x130 |
| `140050df0` | `QString taskDirAbs(OPI)` | `convertSeparators(cleanPath(projectDir + sep + OPI+0x130))`, null if no P |
| `140051230` | `PROPS* props(OPI)` | OPI+0x10 |
| `140054e90` | `SfOprBase* opBase(OPI)` | registry[OPI+0x120] (null if absent) |
| `140054ff0` | `int unit(OPI)` | P+0x20 via OPI+0x100 (0 if no P) |
| `140055030` | `P* project(OPI)` | OPI+0x100 |
| `140055050` | `SUB* activeSub(OPI)` | P+0x58[P+0x50] |
| `140055150` | `Model* activeModel(OPI)` | activeSub->+0x00 |
| `14005a300` | `int startStep(OPI)` | see 5.5 |
| `14005f0f0` | `int dim(OPI)` | `opBase->vt[0x190]()`, 3 if no opBase |
| `14005f150` | `bool is2D(OPI)` | `opBase && opBase->vt[0x190]() == 2` |
| `14005f1c0` | `bool is3D(OPI)` | `!opBase || opBase->vt[0x190]() == 3` |
| `14005f230` | `bool startsFromKey(OPI)` | `isChainHead(OPI) ? 1 : OPI+0x150` (restart flag) |
| `140064740` | `bool hasRestartKey(OPI)` | OPI+0x150 |
| `14005f770` / `14005f820` | `EngineModel* model(OPI)` | `opBase->vt[0xe0](OPI)` or OPI+0x38 created by `opBase->vt[0x1b0]()` (filled only by the lazy load, Q4) |
| `14005fa80` | `int objSlots(OPI)` | count of OPI+0x40 |
| `14005fb30` / `14005fc50` | `int objId(OPI, slot / n)` | OPI+0x40[slot], -99 if invalid |
| `140061250` / `1400612d0` / `140061310` | `OBJ* obj(OPI, slot / n / ?)` | OPI+0x50 entry (created on demand) |
| `140060640` / `140060690` / `1400606c0` | `bool(int id)` | id>0 or -2 / id>0 / id == -2 |
| `140062070` / `1400620a0` | OBJPOS count / item | OPI+0x60 |
| `1405ef320` | `PROPS* props(OB, OPI)` | `(OPI ? OPI : OB->current(+0x20))->+0x10` |
| `1405ef2b0` / `1405ef280` | `OPI* current(OB)` / `setCurrent(OB, OPI)` | QPointer at OB+0x20 (the MST walk sets it per operation) |
| `1405f8d90` | `OPI* owner(PROPS)` | QPointer at PROPS+0x20 |
| `1405f8dc0` | `int unit(PROPS)` | `140054ff0(owner)` |
| `1400a2180` / `140c0df50` / `140ceed20` | P unit / GEOTYP / title | P+0x20 / +0x24 / +0x28 |
| `14008a500` / `14008a4b0` | P base name / path | completeBaseName(P+0x18) / P+0x18 |
| `1400a4ee0` | P primary DB | cleanPath(P+0x30) |
| `1400a7360` / `1400a74f0` / `1400a7390` / `1400a73c0` | SUB count / SUB i / active SUB / active index | P+0x58, P+0x50 |
| `1400a75a0(P, i)` | sub-project name | `Name` attribute of the i-th `Sub_Project` element in P+0x48 |
| `1400a7670(P)` | primary DB (cleaned) | cleanPath(1400a4ee0) |
| `1400b5a70` / `1400b5a90` / `1400b5c60` | SUB model / DOE study / Id | SUB+0x00 / +0x20 / +0x1c |
| `1400b6630(SUB)` | flow-chart order | 8.3 |
| `140047dc0(oprMgr, &id)` | `OPI*` for an operation id | `oprMgr->vt[0x1c8](id)` → item → +0x48 |
| `1400a58b0` / `1400a58f0` / `1400a5930` / `1400a5970` / `1400a59b0` | P → OprManager / Obj / Mat / Eqp / Lub manager | M+0x19c8 / d0 / d8 / e0 / e8 |
| `140060030(OPI, id)` | `SfMoObjItem*` | object manager `vt[0x1c8](id)` cast to SfMoObjItem |

## 11. Things the DLL must not reproduce (file-system side effects of open)

1. `mkpath` of `Operations/TaskNNNNN` for every operation id ≥ 1 and of `Operations`, `Objects`,
   `Materials`, `Equipments`, `Lubricants` (`14008c630`); `DoeStudies` for DOE projects.
2. Writing `ObjPropertiesNNNNN.KEY` (and `SimCtrl.KEY`, `InterObject.KEY`, inline `MoItem/Data`)
   from the hex blobs. The bytes are what a later KEY read sees; keep them in memory.
3. FOR003 rename (`1400a5290`).
4. Global state: unit globals (`0x143dd9b98`, `1409dd680`), tokenizer buffers, current operation
   pointers in the OB singletons.

## 12. Open questions

* Q1 Do MST writers read `ObjPropertiesNNNNN.KEY` (or other task files) from disk after open? If
  so they see the bytes written from `<ObjPropertiesKey>` at open, which can differ from what was
  on disk; the DLL overlay must serve them. Same for legacy `SimCtrlKey` / `InterObjectKey`.
* Q2 Default unit when `<Unit>` is missing: global `0x143d79724` (start-up setting, not traced).
* Q3 P+0x30 depends on whether `<dir>/<Primary_DB_Name>` exists at open; the converter must test
  the real file system (read-only) the same way.
* Q4 After open the GUI marks the item with index `Active` as current (`model->vt[0xf8]`) and may
  open that operation lazily (`140047f80` → `140051340`: `OB->vt[0x58]` loads KEY files into the
  engine model, then `140064cb0` copies material ids from the model into OBJ+0x20). If any writer
  uses `14005f770` (engine model) or the "current operation" state, the MST depends on which
  operation was selected before Save. Not reached from the MST walk in the direct-call closure.
* Q5 Connecting items (`140075050`) also updates "Read From DB" labels; not checked whether it
  changes OPI data (it seems UI-only).
* Q6 `1400aa960` (after open) registers each operation class's `OB+0x28` string into P+0x60
  (`1412ec7b0`/`1412f08d0`); purpose unknown, not followed.
* Q7 `MatId` values with repeated spaces produce id 0 entries (KeepEmptyParts); not seen in the
  corpus.
* Q8 `*!SIMTYP` for the express family prints the legacy name; confirm the source field
  (OPI+0x128, diagram ClassName, or `item+0x50`) in s1-02.
* Q9 Duplicate operation names are renamed on open; no v11 oracle in the corpus.
* Q10 Property trees (names, defaults, enum meaning) are per `SfOpr*Properties` class and are
  needed by the writers; only values present in the class tree are taken from the file.
* Q11 DOE sub-projects (index ≥ 1) are only partly parsed at open (`14008f200`: DbName); their
  diagram and `SfDoeStudy` are parsed when activated. The `.MST.TEMP` path (s1-05) must check
  what `140131530` expects for sub-projects that were never activated. No corpus project has one.
* Q12 GroupInfo membership uses the member's own cell; nested groups: the later group (by begin
  cell) wins. Only Cycle groups are in the corpus.
