# L0 part B: object load path of the KEY loader `0x1400569d0` (mode 2, batch)

DEF_GUI_MO_64.exe v11.0.2. Static analysis only. Decompiles and asm in `L0\B\` (`b1a.c` .. `b6.c`
annotated with .rdata strings; `b2f0.asm`, `69d0b.asm`, `d460.asm` for the points checked in asm).
Helpers written for this part: `L0\B\lk.py` (GUI va -> DEF_PRE twin, ledger status, port file),
`L0\B\pair.py` (pairs the call targets of two twin functions, which gives the twin of a callee whose
signature is not unique), `L0\B\rtti.py` (RTTI type descriptor name).

Conventions below: `op` = the operation item (OPI, `SfMoOprItem`-like QObject); `model` = the GUI
model wrapper `SfStepData` (vtable `0x1428fef98`, 0x278 bytes) that `0x14005f770(op)` returns; `eng`
= its DEFORM engine model `DB_STEP_DATA` (`SfStepData+0xe0`, `0x140c7acd0`); `obj` = engine object
record (`LLOBJ_DATA` layout of DEF_PRE: number +8, type/kind +0xc, name +0x828, owner model
+0x3a98). Slot `s` is 0-based, object number is `s+1`. `QS(x)` = QString. `tr(x)` =
`QMetaObject::tr` on the meta object `0x143d7e3a0` (`0x1400654a0`). All file names below are
built with `QDir::separator()` and `QDir::cleanPath` exactly as written.

## 0. Data the path reads

| Where | Field | Meaning (as used here) |
|---|---|---|
| op+0x10 | `SfOprBaseProperties*` | dynamic_cast to `SfOprExpressProperties` in 14005b2f0 |
| op+0x38 | QPointer model | lazily created by the class (14005f820: class vt+0xe0(op), else op+0x38, else class vt+0x1b0 after setting the class's current op) |
| op+0x40 | QList<int> slot codes | `code(s)` (14005fb30): `-99` when s out of range. `>0` own object file id (SfMoObjManager id), `-2` inherit from previous operation / DB, `-1` empty slot |
| op+0x50 | QList<SlotItem*> | slot records, 0x1c8 bytes, created lazily up to the code count (140061250 / 140061020 / ctor 14004d160) |
| op+0x100 | project | `0x140055030` |
| op+0x108 | int | operation id (tree key, also the signal argument) |
| op+0x120 | QString | class name (`SfOprForming`, `SfOprConvert2Dto3D`, ...) (140050d00) |
| op+0x130 | QString | operation folder name (140051110) |
| project+0x18 | QString | project file path; its `QFileInfo::absolutePath()` is the project folder (1400a4220) |

SlotItem (0x1c8, ctor 14004d160 + 14004d450): +0 int object number, +4 int shown type (init 7),
+8 int original type (init 7), +0x10 QString name, +0x18 byte has geometry, +0x19 byte has mesh,
+0x20 QList<int> (first value is the material group, see 140060be0), +0x28 int equipment id
(14004de90), +0x1ba byte "mesh settings defined", +0x1be byte "BCC defined", +0x1c0 byte (copied to
obj+0x2bd8). Flags +0x1b8..+0x1c1 cleared by the ctor.

Operation folder `opDir(op)` (140050df0): if project null -> null QString; else
`convertSeparators(cleanPath(cleanPath(absolutePath(project+0x18)) + sep + op+0x130))`.

Per-slot file name rule (14005cce0 / 14005cf70 / 14005da90 / 14005eb10, identical code with a
different stem): `cleanPath(opDir(op) + sep + QS("%1%2.KEY").arg(QS(stem), 0, ' ').arg(s+1, 5, 10, '0'))`
-> e.g. `MeshSettings00001.KEY`. Stems: `RzElmCon` (14005cce0), `MeshSettings` (14005cf70),
`BccDef` (14005da90), `ObjProperties` (14005eb10).

Predecessor operation `prev(op)` (140062960): `id = 0x140086720(tree(project), op+0x108)` (tree node
of the operation, its parent's id via model vt+0x138 then item vt+0x28; -1 = none); `-1` -> null;
else `0x140047dc0(opList, &id)`. `hasNoPred(op)` (140050bc0) = that id == -1.
`prevInOrder(project, op)` (1400b7fb0): the entry before `op` in the project's flat operation list
(`0x1400b94f0`), null at the start.

## 1. Slot loop of `0x1400569d0` (context; the object part only)

```cpp
// inside 1400569d0, when param_2 (load objects) != 0, after 0x1400a3540(project)
int n = slotCount(op);                         // 14005fa80: op+0x40 size
for (int s = 0; s < n; ++s) {
    int code = slotCode(op, s);                // 14005fb30
    int no = s + 1;
    if (code == -1) { model.objectWrap(no, false, true)->type = 7; continue; }   // 140c7c950(m,no,0,1); +0xc = 7
    if (code == -2) model.objectWrap(no, false, true)->type = 5;                 // isInherited 1400606c0
    else if (code == -99) continue;
    LLOBJ_DATA* o = loadSlot(op, model, s, false);                            // 14005aec0
    if (o) {
        SlotItem* li = slotItem(op, s);                                         // 140061250
        if (li && li->list20.size() == 0) {                                     // 14004db50 == 0
            int g = li->list20.value(0);                                         // 14004db80(li,&0) -> -1 (list empty)
            setSlotMaterialGroup(op, s, g);                                      // 140060be0
        }
        LLOBJ_DATA* o2 = model.objectWrap(no, false, true);
        o2->byte_2bd8 = li->byte_1c0;           // 14004e140(li); li is NOT null-checked here
    }
}
```
(The equipment loop that follows uses 140057710 = `model.object(no)` (140c7ae80), the slot item's
+0x28 equipment id (14004de90), the equipment manager `0x1400a5970(project)` vt+0x208 path and
`0x1403568a0`; not part B.)

`0x140060be0(op, s, g)`: `o = 0x140057690(op, s)` (model object s+1 when `code(s) > 0 || == -2`,
via 140060640); if `o`: `0x140876640(o, g, 0, 1)` (set material group, DEF_PRE 1402027e0
verified). Returns `o != 0`. The third argument was checked in asm (r8 = result of 14004db80).

## 2. `0x14005aec0` loadSlot(op, model, s, bool refresh)

```cpp
LLOBJ_DATA* loadSlot(Op* op, SfStepData* m, int s, bool refresh) {
    LLOBJ_DATA* r = nullptr;
    int code = slotCode(op, s);
    if (code > 0) r = loadOwn(op, m, s, false);              // 14005b030
    else if (code == -2) r = loadInherited(op, m, s, false); // 14005b2f0
    if (refresh) 0x1400547b0(op, 0);                          // never on this path (refresh = 0)
    return r;
}
```
`0x14005af80` is the same without the refresh argument and with `bare = true` for both loaders; it
is called by the CopyMirror class slot vt+0x2a0 (section 4.3).

## 3. `0x14005b030` loadOwn(op, model, s, bool bare): object from its own object file

```cpp
LLOBJ_DATA* loadOwn(Op* op, SfStepData* m, int s, bool bare) {
    LLOBJ_DATA* obj = nullptr;
    int code = slotCode(op, s);
    int no = slotObjNo(op, s);                    // 14005fc10: s+1 when 0 <= s < count, else 0
    if (code > 0) {
        QString path = objManager(project)->filePath(code);  // SfMoObjManager vt+0x208 = 14006d380
        if (!QFile::exists(path)) return nullptr;
        char reset = 1; int mode = 0;
        if (keyRead(eng(m), path.toLocal8Bit().data(), &reset, &mode) != 0)     // 1408969a0 = DEF_PRE 1401b5f00
            return nullptr;
        obj = m->objectWrap(no, false, true);    // 140c7c950(m, no, 0, 1)
        if (bare) return obj;                     // no viewer sync, no slot bookkeeping
        if (SlotItem* li = slotItem(op, s)) {
            if (li->byte_1ba) loadMeshSettingsOwn(op, s);   // 14005d200
            loadObjProperties(op, s);                       // 14005eda0
        }
        if (SlotItem* lib = objLibItem(op, code))           // 140061360
            syncSlotItem(op, lib, obj);                     // 1400601b0
    }
    viewerSync(m, obj, true);                     // 140c80280 (no-op when obj null)
    return obj;
}
```
* `SfMoObjManager` = `0x1400a58f0(project+0x100)`, a QStandardItemModel (vtable `0x14212cf68`,
  8 columns, ctor 140068ba0; cached at owner+0x19d0). `filePath(code)` (14006d380):
  `item = vt+0x1c8(code)` (14006cea0: linear search of rows `vt+0x1c0(row)` for `item+0x18 ==
  code`, row count vt+0x1b8); null item -> null QString; else `cleanPath(item+0x28)`. Where
  item+0x28 comes from (the `Objects/ObjectN.KEY` rule) is set when the project is read, not here.
* The KEY file is read into the whole engine model (`reset = 1, mode = 0`); the object number is
  whatever the file says. `objectWrap(no)` then fetches (or creates) object `no`.
* `objLibItem(op, code)` (140061360): `0x140060030`: `SfMoObjManager vt+0x1c8(&code)`,
  dynamic_cast `SfMoItem` -> `SfMoObjItem` (1400690a0), then its +0x40 (140068b40) = a record with
  the SlotItem layout.

## 4. `0x14005b2f0` loadInherited(op, model, s, bool bare): object from a previous operation

```cpp
LLOBJ_DATA* loadInherited(Op* op, SfStepData* m, int s, bool bare) {
    LLOBJ_DATA* dst = nullptr;
    int no = s + 1;
    int code = resolvedCode(op, s);               // 14005fd20 (4.1)
    if (code > 0) {
        dst = getOrCreate(eng(m), no);            // 1408bde10 = DEF_PRE 14019ccc0
        bool fromDb = false;
        QString cls = op->className();            // 140050d00
        bool always = true;                       // local constant [rsp+0x60] = 1 (asm 14005b39c)
        if (!hasNoPred(op) && (always || cls == "SfOprConvert2Dto3D"))   // 2nd test is dead
            fromDb = objectFromDb(op, no, simNo(op) - 1, /*last*/1, dst); // 14005cc00 (4.2); simNo = 140050b70
        if (!fromDb) {
            QString path = objManager(project)->filePath(code);   // path fixed here, before the walk
            if (!QFile::exists(path)) return nullptr;
            bool handled = false;
            for (Op* p = prev(op); p; p = prev(p)) {
                OprClass* c = classOf(p);              // 140054e90: main window registry by class name
                Op* saved = c->currentOp();            // 1405ef2b0 (c+0x20)
                c->setCurrentOp(p);                    // 1405ef280
                handled = c->vt_2a0(no, dst);          // SfOprBase 1405f5b70: false; SfOprCopyMirror 140e25980 (4.3)
                c->setCurrentOp(saved);
                if (handled) break;
                int pc = slotCode(p, s);
                if (pc > 0) { code = pc; break; }      // value not used afterwards (path already built)
                if (pc != -2) break;
            }
            if (!handled) {
                SfStepData* sc = (cls == "SfOprConvert2Dto3D")
                    ? new SfStepData(/*dim*/2)               // 140c7a6b0(mem, 2)
                    : new SfStepData(/*parent*/0, /*dim*/3); // 140c7a5b0(mem, 0, 3)
                if (cls == "SfOprConvert2Dto3D") {
                    int n1 = s + 1; sc->loadObjectKey(path, &n1);              // 140c7cd90
                } else if (cls.compare("SfOprHeatTreat", Qt::CaseInsensitive) == 0 ||
                           cls.compare("SfOprHeatTreat2D", Qt::CaseInsensitive) == 0) {
                    char reset = 1; int mode = 0;
                    keyRead(eng(sc), path.toLocal8Bit().data(), &reset, &mode); // 1408969a0, result ignored
                } else {
                    bool mark5 = false; int n1 = s + 1;
                    sc->loadObjectKeyGeometry(path, &n1, &mark5);             // 140c7ce40
                }
                LLOBJ_DATA* src = sc->object(s + 1);                          // 140c7ae80
                if (!src) return nullptr;                                     // sc is leaked
                dst = getOrCreate(eng(m), s + 1);
                if (src->dim_39c0 == 2 && opIs3D(op))                         // 140065840 == 2, 14005f1c0
                    dst = convert2Dto3D(op, m, sc, s);                        // 14005bd30 (5)
                else
                    copyKeepNumber(dst, src);                                 // 140835800 = DEF_PRE 140161770
                strcpy(dst->name_828, src->name_828);
                delete sc;                                                    // vt+0x18(1)
            }
        }
        if (!dst) return nullptr;
        int oldType = dst->type;  dst->type = 5;
        if (bare) return dst;
        if (SlotItem* li = slotItem(op, s)) {
            li->type4 = dst->type;            // 14004d580 (= 5)
            li->type8 = oldType;              // 14004d640
            li->name  = QS(dst->name_828);    // 140833ab0, 14004d700
            if (li->byte_1ba) loadMeshSettingsOwn(op, s);     // 14005d200
            else              loadMeshSettingsInherited(op, s); // 14005d460
            if (li->byte_1be && !loadBcc(op, s, nullptr))     // 14005dd20
                li->byte_1be = 0;
            loadObjProperties(op, s);                         // 14005eda0
        }
    }
    // label 14005bc61 (also reached directly when code <= 0, dst == nullptr)
    if (!dst) return nullptr;
    dst->type = 5;
    auto* ex = dynamic_cast<SfOprExpressProperties*>(op->props /*op+0x10*/);
    if (objHasMovement(dst) && ex && ex->vt_100())    // 14084d4a0 = DEF_PRE 140179280
        resetMovement(&dst->movement_2160);            // 14085f6d0
    viewerSync(m, dst, true);                          // 140c80280
    return dst;
}
```
* String literals: `0x142123420`, `0x142123438`, `0x142123450` = `"SfOprConvert2Dto3D"` (QString ==
  const char*, case sensitive); `0x142123468` = `"SfOprHeatTreat"`, `0x142123478` =
  `"SfOprHeatTreat2D"` (QString::compare with CaseSensitivity 0 = Qt::CaseInsensitive).
* `SfOprExpressProperties::vt+0x100`: base = pure virtual (0x141470184), FormingExpress
  (1400707e0) = false, HeatTransferExpress (1413e5ef0) = true. So inherited moving objects get their
  movement reset only in HeatTransferExpress operations.
* `opIs3D(op)` (14005f1c0): no class -> true; else `class vt+0x190() == 3`. Every SfOpr class
  except a few previews uses 1405f5bf0: `f = (vt+0x188())->vt+0x58()`; `(f&3)==3` -> 4, `f&1` -> 2,
  `f&2` -> 3, else 0.
* The object type 5 is the GUI marker "from previous operation"; the real type is kept in
  SlotItem+8.

### 4.1 `0x14005fd20` resolvedCode(op, s)

```cpp
int code = slotCode(op, s);
if (code == -2)
    for (Op* p = prev(op); p; p = prev(p)) {
        int c = slotCode(p, s);
        if (c > 0) return c;
        if (c != -2) return -99;
    }
return -99;      // note: a positive own code also returns -99 (only called for code -2)
```

### 4.2 `0x14005cc00` objectFromDb(op, no, sim, bool last, dst) and `0x14005c890`

```cpp
bool objectFromDb(Op* op, int no, int sim, int last, LLOBJ_DATA* dst) {
    SfStepData* m = model(op); if (!m) return false;
    DbHandle* db = m->db();                 // 140c7ac50: SfStepData+0xd8 -> 0x140cdab70
    if (!db) return false;
    db->refreshIfChanged(0);                // 140d397a0 (QFileInfo lastModified vs db+0x1910)
    if (sim < 1) return false;
    int step = last ? lastSavedStep(db, sim)     // 14080eb30 = DEF_PRE 1401de530 (via 1401df8a0)
                    : firstSavedStep(db, sim);   // 14080ea20 = DEF_PRE 1401de420
    if (step == 0) return false;
    return copyObjectFromDbStep(op, no, step, dst);   // 14005c890
}
```
`simNo(op)` (140050b70) = `0x1400b8820(project, 0x140050b20(op))`: the simulation number of the
operation's position (falls back to previous number + 1).

`0x14005c890(op, no, step, dst)` (0x4e20-byte frame):
```cpp
if (!model(op) || !model(op)->db() || !project) return false;
QString dbPath = projectDbPath(project, true);        // 1400b9e80(project, &s, 1): type 4 -> 1400bbee0
                                                      // (project+0x38 name, computed by 1400bc290 if
                                                      // empty; with flag: 1400bc160 dir + sep + name)
QFile f(dbPath); if (!f.exists()) return false;
DB_STEP_DATA sd(3);                                   // 140821bf0
DbInfo dbi; dbInit(&dbi);                             // 14080c660 -> DEF_PRE 1401dc030
bool ok = false;
if (dbOpen(&dbi, dbPath.toLocal8Bit().data(), 0) == 0) {    // 14080ca10 (no twin)
    if (dbHasPrevFile(&dbi)) dbChainPrev(&dbi);             // 140814e80 / 1408150f0
    if (dbReadStep(&sd, &dbi, step, 1) == 0) {              // 140825540 = DEF_PRE 1401cc2a0
        if (LLOBJ_DATA* o = sd.object(no)) {                // 140830270
            char f1=1,f2=1,f3=1,f4=1,f5=0,f6=1;             // same flag set as 140835800
            llobjCopy(dst, o, &sd.aux_15f8, &f1, &f2, &f3, &f4, &f5, &f6);  // 140835430 = 1401613a0
            ok = true;
        }
        if (ok) bccGroupsFromModel(dst, &sd.aux_15f8);      // 14087e1a0 = DEF_PRE 14020a340
    }
}
dbFree(&dbi); /*14080c820*/ sd.~DB_STEP_DATA(); return ok;
```

### 4.3 Class slot vt+0x2a0 (object provided by a previous operation's class)

All classes return false (1405f5b70) except `SfOprCopyMirror` (140e25980):
```cpp
bool CopyMirror::provideObject(int no, LLOBJ_DATA* dst) {
    Op* cur = currentOp(); if (!cur) return false;           // 1405ef2b0
    LLOBJ_DATA* o = loadSlotBare(cur, model(cur), no - 1);   // 14005af80 (loaders with bare = true)
    bool ok = false;
    if (o) {
        if (hasGeometry(o)) { if (hasMesh(o)) deleteMesh(o); ok = true; }      // 140834d50/140834c80/140873be0
        else if (hasMesh(o)) { char c = 1; geometryFromMesh(o, &c); deleteMesh(o); ok = true; } // 140870590
    }
    if (!ok) return false;
    mirrorObject(no, o);                 // 140e25b20 -> 140e25c00 (CopyMirror transform, needs the
                                         // viewer item 140c82d50(model(cur), &no)+0x348 count > 0)
    char f1=1,f2=1,f3=1,f4=1,f5=0,f6=1;
    llobjCopy(dst, o, &o->owner->aux_15f8, ...);             // 140835430, owner = o+0x3a98 (1400c35b0)
    return true;
}
```
140e25c00 (CopyMirror geometry construction; only a qDebug "EditGeo Construction error!") is left
to the CopyMirror packet.

### 4.4 Model wrapper loaders used for the scratch model

`0x140c7ce40(m, path, &no, &mark5)` loadObjectKeyGeometry:
```cpp
LLOBJ_DATA* o = importObjectKey(eng(m), path.toLocal8Bit().constData(), *no);  // 140897cf0 = DEF_PRE 1401b7250
bool r = false;
if (o) {
    if (hasGeometry(o)) { if (hasMesh(o)) deleteMesh(o); r = true; }
    else if (hasMesh(o)) { char c = 1; geometryFromMesh(o, &c); deleteMesh(o); r = true; }
    if (*mark5) o->type = 5;
    viewerSync(m, o, true);
}
return r;
```
So inherited objects of ordinary operations keep only their geometry (the mesh is dropped, or
turned into geometry first).

`0x140c7cd90(m, path, &no)`: `o = importObjectKey(eng(m), path, *no)`; if `o` viewerSync(m, o,
true); returns `o != 0` (mesh kept: Convert2Dto3D).

`importObjectKey` (140897cf0, DEF_PRE 1401b7250, not ported): scratch `DB_STEP_DATA(3)`, copy the
model's sim controls into it (1408df7b0 = 1401f3660), KEY read with reset 1 / mode 0; first object
with kind != 7 (140823e60 = 1401cabc0); none -> null. Else `model vt[0](&dimOf(scratch))`
(140100980); `no < 1` -> first free number (14084ee40 = 14017ac20); `dst = getOrCreate(model, no)`;
`copyKeepNumber(dst, src)`; empty name -> file base name (1409e3c30 / 1409e3bc0 = 140323d50 /
140323ce0) cut to 0x50 chars; `scratch+0x16fc > 0` -> `model+0x16cc = it, model+0x864 = 1`.

`0x140c7c950(m, no, bool reset, bool viewer)` objectWrap:
```cpp
LLOBJ_DATA* o = eng(m)->object(no);                 // 140830270 (list +0x1380, 14084ed10 = 14017aaf0)
bool created = (o == nullptr);
if (created) o = getOrCreate(eng(m), no);           // 1408bde10
if (reset && kindIs7(o)) { resetKind7(o, 0); defaultObjectName(o); }   // 140834fe0 = 140160f50, 140833b40
if (created) {
    if (!m->hasDeformable()) o->type = 1;           // 140c7c260: first object with type not 7/0 (1401cac80)
    defaultObjectName(o);                           // 140833b40
    m->pickPrimary();                               // 140c7b630
    viewerSync(m, o, viewer);                       // 140c80280
}
return o;
```
`defaultObjectName` (140833b40, no twin): reads the QSettings bool `"systemOption/autoObjName"`
(default true, settings object `0x143d76370`, 140808270); false -> `sprintf(o+0x828, "Object %d",
no)`; true -> no 1 and type != 0 -> `"Workpiece"`, no 2 and type 0 -> `"Top Die"`, no 3 and type 0
-> `"Bottom Die"`, else `"Object %d"`.

`pickPrimary` (140c7b630): `n = count(type != 7)` (1401cab30); `n < 2` -> A; `n == 2` -> B; else
B if `type(sd+0x168)` is 7 or missing. B: `id = first rigid` (1401cad40); `id < 1` -> A, else
`sd+0x168 = id`. A: `sd+0x168 = first object with kind != 7` (1401cabc0). `sd` = engine+0x750
(140c7b2d0).

`0x140c7d020(m, path)` (used by the equipment reader, listed for completeness): KEY read (reset 1,
mode 0) into `eng(m)`; on success `0x140c7d3a0(m)` and `0x140c7ebe0(m)`; returns success.
`140c7d3a0`: when `sd+0x3c0`: table `t = curve(sd+0x440, 0xe)` (1409f0660 = 140375980); with
`v(i,c) = dynary_get(t, i, 0, 0, c)` (14089c490 = 140130720): `(int)` truncation of row 0 c=0,1,6,
7,8,9,10,11 to `m+0xf8 -> +0x00,+0x08,+0x10,+0x14,+0x18,+0x28,+0x2c,+0x30`, raw doubles c=12,13,14
to +0x40,+0x48,+0x50; row 1 to +0x04,+0x0c,+0x1c,+0x20,+0x24,+0x34,+0x38,+0x3c and +0x58,+0x60,
+0x68. `140c7ebe0`: table 0xd, for each row i < t+0x1c: `o = m.object((int)v(i,0))`; `o+0x3aa4 =
(int)v(i,5)`; `o+0x3ab0+8k = v(i,6+k)` for k = 0..5.

`viewerSync` (140c80280): `n = firstObjectNo(eng)` (140c7c220 / 1408301c0 = 1401d6df0);
`0x140c82930(m, &n)` (resize the viewer item list to n, items of 0x87c0 bytes `0x1411f07d0`);
`v = 0x140c82d50(m, &obj->no)`; `0x1411f17f0(v, obj, 1)`; `w = 0x1411f1a10(v)`; if `w && flag`
`0x1411ee800(w, obj, &true)`; `0x140c8e640(m)` (viewer refresh). Viewer code; the converter needs
none of it.

## 5. `0x14005bd30` convert2Dto3D(op, model, scratch, s)

```cpp
LLOBJ_DATA* convert2Dto3D(Op* op, SfStepData* m, SfStepData* sc, int s) {
    if (!m) return nullptr;
    LLOBJ_DATA* dst = nullptr;
    if (resolvedCode(op, s) > 0) {
        dst = getOrCreate(eng(m), s + 1);
        LLOBJ_DATA* src;
        if (sc && (src = sc->object(s + 1))) {
            dst->type = src->type;
            dst->matGroup_3980 = src->matGroup_3980;           // 140876860 / 140876840
            bool conv = src->dim_39c0 == 2 && opIs3D(op);
            Op* cop = findPrevOpOfClass(op, QS("SfOprConvert2Dto3D"));   // 140062a80, 0x142123490
            if (conv && cop) {
                QString path;
                auto* cvt = dynamic_cast<SfOprConvert2Dto3D*>((SfOprBase*)classOf(cop));
                if (cvt) {
                    path = cleanPath(cvt->geoConvtPath(src->no, 2));      // 1401c65f0
                    if (!QFile::exists(path))
                        path = cleanPath(opDir(cop) + sep + path);
                    if (!QFile::exists(path)) {
                        if (src->geo23_type_2be8 == 1 && src->u2d94 == 0)
                            cvt->axisCheck(eng(sc), src);                 // 1401c71f0
                    } else {
                        readGeoConvt(eng(sc), path.toLocal8Bit().constData(), src->no);  // 140898880 = 1401b7de0
                    }
                    if (dst && geomHasEdge(src, 0)) {                     // 14086c1f0 = 140198050
                        copy_u3aa0(dst, src);                             // 140835b50 = 140161ac0
                        copy_u3998(dst, src);                             // 140835a50 = 1401619c0
                        copyGeo23(&dst->geo23_2be8, &src->geo23_2be8);    // 14085ceb0 = 140188c90
                        char winlist = 0, typed = 1;
                        copyMeshSettings(&dst->ms_2d10, &src->ms_2d10, &winlist, &typed); // 14085dd40 = 140189b70
                        viewerSync(m, dst, true);
                        dst->u2d94 = src->u2d94;
                        dst->b2c34 = 1;
                        char saved = dst->b2c20; dst->b2c20 = 0;          // saved value never restored
                        if (dst->geo23_type_2be8 == 2)
                            extrude2D(dst, QS(""), 0, dst->type != 0);    // 14036a350 (5.1)
                        else
                            revolve2D(dst, QS(""), 0, dst->type != 0);    // 14036b180 (5.1)
                    }
                    bool axis2 = src->i2c38 == 2;
                    if ((axis2 && src->geo23_type_2be8 == 1) || (!axis2 && src->geo23_type_2be8 == 2)) {
                        double* org = new double[3]{0, 0, 0};             // operator new(0x18)
                        double* dir = new double[3]{1.0, 0, 0};
                        objRotate(dst, org, dir, src->geo23_type_2be8 == 2 ? 90.0 : -90.0); // 1408911f0
                        delete org; delete dir;
                        char cr = 1;
                        auto* w = wpaxisKind4(&dst->list_20a0, &cr);      // 1408ea050 = 14025a270
                        w->set68({1, 0, 0}); w->set80({0, 1, 0}); w->set98({0, 0, 0});  // 1408e8db0/e40/ed0
                    } else {
                        char cr = 1;
                        auto* w = wpaxisKind4(&dst->list_20a0, &cr);
                        w->set68({1, 0, 0}); w->set80({0, 0, 1}); w->set98({0, 0, 0});
                    }
                }
            }
        }
    }
    return dst;
}
```
* `findPrevOpOfClass(op, name)` (140062a80): empty name -> null; `p = prevInOrder(project, op)`;
  while `p`: `p.className().compare(name, Qt::CaseInsensitive) == 0` -> return p; `p =
  prevInOrder(project, p)`. Note: the order list, not the operation tree used by `prev()`.
* `SfOprConvert2Dto3D::geoConvtPath(n, 2)` (1401c65f0; the third argument is not used):
  `dir = currentOp() ? opDir(currentOp()) : ""` (class current op, c+0x20, not necessarily `cop`);
  `cleanPath(dir + sep + QS("%1%2.KEY").arg("GeoConvt23Data", 0, ' ').arg(n, 5, 10, '0'))` with `n`
  = object number (no +1).
* `axisCheck` (1401c71f0 -> 1401c70d0): `src+0x2c08 = 1`, set to 0 when some node j of `src`
  (`< src+0x880`) has a BC flag `src+0x990[j*nf + k] == 1` (k from `eng+0x1640`-1 down to 0) whose
  coordinate `node_var_02(src+0x8a0)[j*ndim + k]` has `fabs <= 1e-6` (`nf = eng+0x1640`, `ndim =
  eng+0x1600`).
* `objRotate` (1408911f0, no twin): `fabs(angle) >= 1e-30` -> 140874110 (1402002b0), 140874520
  (1402006c0), 1408935e0(+0x2d10) (14021f6e0), 140893730(+0x2e68) (14021f830), movement +0x2160
  when 1408777e0 (14021dd70), 140893290(+0x1ea0) (14021f390), 140894090(+0x2be8) (140220190), owner
  `+0x3a98` -> 1408c0410 (14019f2c0). Same sequence as DEF_PRE `14021d2f0`
  (`def::cmd::objpos::rotate`, ported) except the die-geometry test 1408777e0 (checks
  `owner+0x1c == 2`, then 140203a20 / 1402039d0).
* Doubles: `0x1421249e0` = 1.0, `0x1421249d8` = 90.0, `0x1421249d0` = -90.0.

### 5.1 `0x14036a350` extrude2D(obj, QString name, ?, bool fixgeo) and `0x14036b180` revolve2D

Both: `g = obj+0x2be8`; only when `geomHasEdge(obj, 0)`. `DEF_AMGGEO amg` (1408f6d10 = 140246d00);
`c = geometry(obj, 0)` (14086c0e0: obj+0x3998 -> 140a21a10 = 140301b40); if `!0x1400667b0(c)`
(140158ec0) `0x1408eef90(c, &false)` (140281bf0); `name = "GeoCS2D.GEO"`; `writeGeo(&amg,
"GeoCS2D.GEO", obj, &0, 2)` (1408f8c00 = 140248bf0); if `!fileExists(name)` (1407fbba0) ->
`fprintf(stderr, "File %s does not exist. Extrusion can not be performed.\n" | "... Revolve can not
be performed.\n")` and return. Then write `DEF_GEOPRIM.INI` ("wt"):

Extrude: `GEOPRIM_EXTRUDE`, `GEOPRIM_CROSS_SECTION_GEO`, `<name>`, type line by the extension code
(140243370 / 140243500: 0 `GEOPRIM_2D_GEO_TYPE_KEY`, 2 `GEOPRIM_2D_GEO_TYPE_AMGGEO`, 6
`GEOPRIM_2D_GEO_TYPE_IGES`, 7 `GEOPRIM_2D_GEO_TYPE_DXF`; type uninitialised when the extension is
not found), `GEOPRIM_3D_GEO_TRIANGLE`, `GEOPRIM_EXTRUDE_DIR`, `"%lf %lf %lf\n"` of
`((g+0xb8)-(g+0xa0))/(g+0x28)`, `((g+0xc0)-(g+0xa8))/(g+0x28)`, `((g+0xc8)-(g+0xb0))/(g+0x28)`,
`GEOPRIM_EXTRUDE_LENGTH`, `"%lf\n"` g+0x28, `GEOPRIM_EXTRUDE_NUM_SECTIONS`, `"%d\n"` g+0x3c,
`GEOPRIM_CROSS_DIG_PARMS`, `0.01 0.3 5.235`. Each keyword with `"%s\n"`.

Revolve: `GEOPRIM_REVOLVE`, `GEOPRIM_CROSS_SECTION_GEO`, `<name>`, the same type line,
`GEOPRIM_3D_GEO_TRIANGLE`, `GEOPRIM_REVOLVE_AXIS` `"%lf %lf %lf\n"` g+0xf0,+0xf8,+0x100,
`GEOPRIM_REVOLVE_CENTER` g+0x108,+0x110,+0x118, `GEOPRIM_REVOLVE_START_ANGLE` `"%lf\n"` g+0x10,
`GEOPRIM_REVOLVE_ANGLE` g+0x8, `GEOPRIM_REVOLVE_NUM_SECTIONS` `"%d\n"` obj+0x2c24,
`GEOPRIM_CROSS_DIG_PARMS`, `0.01 0.3 5.235`.

Then: if `fixgeo` -> `0x14036ada0(name)` (FIXGEO: writes `FIXGEO.INI` ("w"): `"%s\n"` name,
`"0.0  %f  %f\n"` (0, pi), `"%d  %d\n"` 0 0 twice; runs `FIXGEO.EXE` (14081f530 = 140224100, mode
1); if `NEWGEO.GEO` exists and its second integer > 4: remove name, rename `NEWGEO.GEO` to name;
otherwise if `PNTGEO.GEO` exists: remove name, rename it; removes `FIXGEO.INI`, `PNTGEO.GEO`,
`FIXGEO_TEMP.OUT`). If name missing -> return. `0x140369cf0(obj, 0)` (primitive build, 5.2). Then
the placement transform with the ported `matrix4` helpers (14036c7c0 identity, 14036cbb0 =
1402214c0 rotation about axis, 14036cb00 = 140221c20 translation, 14036c970 = 140221a90 product,
14036c8e0 = 140220670 assign, 14036c790 apply to geometry 0):
* extrude (dir = the three quotients written above, `pi/2` = `0x142265010`):
  ```cpp
  if (1.0 - fabs(dirx) < 1e-5) {                 // along x
      Matrix4 A, B; auto* c = geometry(obj, 0);
      A.assign(rotation_about_axis({0,0,0}, {1,0,0}, pi/2));
      B.assign(rotation_about_axis({0,0,0}, {0,0,1}, pi/2));
      A.assign(A.multiply_by(B)); applyMatrix(c, A);          // 14036c790
  } else if (1.0 - fabs(diry) < 1e-5) {          // along y
      Matrix4 A, B; auto* c = geometry(obj, 0);
      A.assign(rotation_about_axis({0,0,0}, {1,0,0}, pi/2));
      double z0 = 0, x0 = 0; B.set_translation(&x0, &g->len_28, &z0);   // 14036cb00(&B, &0, g+0x28, &0)
      A.assign(A.multiply_by(B)); applyMatrix(c, A);
  }                                              // along z: nothing
  ```
* revolve: `w = wpaxisList(obj+0x20a0, 1)` (1408e99a0); `w+0xb8 = g+0x8`; `w+0x8..0x18 = 0`;
  `w+0x20..0x30 = g+0xf0..0x100`; if `fabs(1.0 - (g+0xf8)) >= 1e-6` (`0x142157420`): `A =
  rotation_about_axis({0,0,0}, {1,0,0}, pi/2); applyMatrix(geometry(obj,0), A)` (geometry taken
  in the extrude prologue), else `applyMatrix(c, identity)`.
Finally remove `GeoCS2D.GEO` and `FIXGEO_TEMP.OUT` if present. All files are relative to the
current directory.

### 5.2 `0x140369cf0` primitive build (DEF_GEOPRIM3)

```cpp
QString title = tr("ERROR IN CREATING PRIMITIVE");                  // via 14036c700
QString msg = tr("The input file %1 does not exists.\nPrimitive can not be generated.\n").arg("DEF_GEOPRIM.INI");
if (!fileExists("DEF_GEOPRIM.INI")) {
    QMessageBox::critical(nullptr, title, msg, QMessageBox::Ok /*0x400*/);   // DIALOG
    return;
}
sprintf(cmd, "%s > %s", "DEF_GEOPRIM3.EXE", "DEF_GEOPRIM3.OUT");
removeIfExists("DEF_GEOPRIM3.OUT");                 // 1407fbc10
launch(1, cmd);                                     // 14081f560 = 140224130
removeIfExists("DEF_GEOPRIM.INI");
if (!fileExists("DEF_GEOPRIM.KEY")) {
    msg = tr("Error occurred during geometry primitive construction.\nPlease refer to file %1 for detail information.\n").arg("DEF_GEOPRIM3.OUT");
    QMessageBox::critical(nullptr, title, msg, QMessageBox::Ok);              // DIALOG
    return;
}
removeIfExists("DEF_GEOPRIM3.OUT");
auto* t = new DB_STEP_DATA(3);                      // 0x3408 bytes
char reset = 1; int mode = 0; keyRead(t, "DEF_GEOPRIM.KEY", &reset, &mode);
removeIfExists("DEF_GEOPRIM.KEY");
LLOBJ_DATA* o = objectAt(t, 1);                     // 1408302a0
if (o && hasGeometry(o)) {
    copyGeometry(obj, o);                           // 1408358d0 = 140161840
    if (param_2) 0x14086c390(obj, o);               // 1401981f0 (not on this path: param_2 = 0)
    if (count(o+0x20a0) > 0) {
        copyWpaxis(obj+0x20a0, o+0x20a0);           // 1408e96a0 = 1402598c0
        auto* w = wpaxisList(obj+0x20a0, 1);
        if (w->i4 == 0) w->d_c0 = obj->d_2bf8;
    }
}
delete t;                                           // 140065860(t, 1)
```

## 6. Per-slot side files

`0x14005d200` loadMeshSettingsOwn(op, s): `eng = 14005f7a0(op)`; null -> false; `path =
meshSettingsPath(op, s)`; missing -> false; `return readMeshSettings(eng, path, s+1)` (1408986a0 =
DEF_PRE 1401b7c00: KEY into a scratch `DB_STEP_DATA(3)`, first kind != 7 object `src`;
`model vt[0](&dim)`; `dst = getOrCreate(model, no)`; `copyMeshSettings(dst+0x2d10, src+0x2d10,
&1, &1)` (14085dd40 = 140189b70)).

`0x14005d460` loadMeshSettingsInherited(op, s):
```cpp
DB_STEP_DATA* e = eng(op); if (!e) return false;
Op* p = prev(op);
QString cls = p ? p->className() : QS("");
SlotItem* li = p ? slotItem(p, s) : nullptr;
while (p) {
    QString path = QS("");
    if (cls.compare("SfOprConvert2Dto3D", Qt::CaseInsensitive) == 0) return false;   // 0x1421234e8
    if (li && li->byte_1ba) path = meshSettingsPath(p, s);   // asm: r8 = s
    if (QFile::exists(path)) return readMeshSettings(e, path, s + 1);
    p = prev(p);
    cls = p ? p->className() : QS("");
    li = p ? slotItem(p, s) : nullptr;
}
return false;
```

`0x14005eda0` loadObjProperties(op, s): as 14005d200 with `ObjProperties%05d.KEY` and
`readObjProperties` (140898490 = DEF_PRE 1401b79f0: scratch read; `dst = getOrCreate(model, no)`
(no dimension call); copy `+0x1ea0` block fields +0x4c, +0x50, +0x58, +0x60, +0x68 and
`1408ea510(dst+0x1ea0+0x218, src+0x1ea0+0x218)` (14025a730)).

`0x14005dd20` loadBcc(op, s, eng) (0x3540-byte frame; params from the home slots):
```cpp
if (!e) { e = eng(op); if (!e) return false; }     // jumps straight to the exit; return byte not set there (see open question 1)
QString rz = rzElmConPath(op, s);
bool bad = false, ok = false, bccOk = false;
LLOBJ_DATA* o = e->object(s + 1);                  // 140830270
if (!o) return false;
if (!hasMesh(o)) {
    if (QFile::exists(rz)) { char r=1; int md=0; if (keyRead(e, rz, &r, &md) == 0) ok = true; }
} else {
    int ne = -1, nn = -1;
    DB_STEP_DATA t(3); char r=1; int md=0;
    if (keyRead(&t, rz, &r, &md) == 0)
        if (LLOBJ_DATA* q = t.object(s + 1)) { ne = q->i884; nn = q->i880; }
    if (o->i884 == ne && o->i880 == nn) ok = true; else { ok = false; bad = true; }
}
if (ok) {
    QString bcc = bccDefPath(op, s);
    if (QFile::exists(bcc)) { char r=1; int md=0; if (keyRead(e, bcc, &r, &md) == 0) bccOk = true; }
}
if (bad)  msgLog(tr("Mesh has been changed since BCC has been defined."), 1, false);    // 1402d0c80
if (!ok || !bccOk) msgLog(tr("An error occurred during loading BCC definition."), 3, false);
return ok && bccOk;
```
`msgLog` (1402d0c80 on the message window `0x143d76350`): a coloured line in the message view, not a
modal dialog.

## 7. Slot bookkeeping reached from loadOwn: `0x1400601b0` syncSlotItem(op, li, obj)

```cpp
if (!li || !obj) return 0;
li->type4 = obj->type;
if (slotCode(op, obj->no - 1) > 0) li->type8 = obj->type;          // 14005fc50
li->name = QS(obj->name_828);
li->hasGeo = hasGeometry(obj);  li->hasMesh = hasMesh(obj);        // 14004d780 / 14004d7d0
li->byte_1c0 = obj->byte_2bd8;                                     // 14004e120
int s = obj->no - 1;
if (slotCode(op, s) != -2) {
    if (obj->type == 5) setSlotCode(op, s, -2, true, true, true);  // 140057c60
} else if (obj->type != 5) {
    int id = objManager(project)->newId();                         // vt+0x180 = 14006c520: id = vt+0x168(); vt+0x188(id)
    setSlotCode(op, s, id, true, true, true);
}
return 1;
```

`0x140057c60` setSlotCode(op, s, code, bool reload, bool view, bool signal):
```cpp
SfStepData* m = model(op); if (!m) return nullptr;
slotCode(op, s);                                   // result unused
int no = s + 1;
storeSlotCode(op, s, code);                        // 14005fc80: grows op+0x40 with -1
LLOBJ_DATA* r = nullptr;
if (!reload) {
    r = m->objectWrap(no, true, true);
    if (code == -2) r->type = 5;
    if (r) { r->d_2bf0 = 360.0; r->d_2bf8 = 0; }   // 0x1421249c8
} else if (code > 0 || code == -2) {               // 140060640
    r = loadSlot(op, m, s, false);                 // 14005aec0 (recursion; stops once codes agree)
    emitViewer(0x143d76700, "viewFit", QString()); // 1402ce8a0
    if (view) viewSetModel(op);                    // 140054820: 1402ce790(model) + "viewFit"
}
if (refreshSlotItems(op, no)) objManagerRowChanged(op, no);   // 140060100, 140055220 (vt+0x228)
if (view) classRefresh(op);                        // 140054c20: class vt+0x270(op, model)
objManagerRefresh(objManager(project));            // 140069620
if (signal) {
    QString src = slotSourceName(op, code);        // 14005ff30: code == -2 -> tr("Read From DB"),
                                                   // else slot item of code (14005fb80) name or ""
    QMetaObject::activate-like call 1402cf3c0(0x143d76700, "2signalObjDataAdded()" , Q_ARG(QString, src),
                                              Q_ARG(int, s), Q_ARG(int, op+0x108));
}
return r;
```
`refreshSlotItems(op, no)` (140060100, decompiled as void, used as bool): `li = slotItem(op,
no-1)`; `o = model.object(no)`; `c = slotCode(op, no-1)`; `c > 0` and `lib = objLibItem(op, c)` ->
`syncSlotItem(op, lib, o)`; then `syncSlotItem(op, li, o)` (its result is the return value).

## 8. Engine calls on this path

Status from `docs/re/pre64_batch_ledger.tsv` (column 6) and a grep of `@src` in `port/`.
"not in ledger" = the twin exists in DEF_PRE but is outside the batch scope and has no port code.

| GUI | DEF_PRE twin | Status | Role here |
|---|---|---|---|
| 1408969a0 | 1401b5f00 | ported (defcmd key_file) | KEY reader |
| 1408bde10 | 14019ccc0 | ported (defmodel llobj_ctor) | get-or-create object |
| 140830270 | 1401d6ea0 / 1401d6ed0 (sig not unique; body = 14084ed10) | ported | object by number |
| 14084ed10 | 14017aaf0 | ported | object list lookup |
| 1408302a0 | 1401d6ea0 / 1401d6ed0 (14084eca0 = 14017aa80) | ported | object at position |
| 140834fa0 | 140160f10 | ported | kind != 7 |
| 140834fe0 | 140160f50 | **not in ledger** | kind-7 reset + 140833a10 (14015f9a0 verified) |
| 140833b40 | none | GUI engine | default object name (QSettings) |
| 140835800 | 140161770 | **not in ledger** | copy object keeping its number |
| 140835430 | 1401613a0 | ported | object copy |
| 1400c35b0 | 140158f50 | ported | owner model (+0x3a98) |
| 14084d4a0 | 140179280 | verified | object has movement |
| 14085f6d0 | none | GUI engine | movement block reset (obj+0x2160) |
| 140065840 | none (trivial) | - | obj+0x39c0 (dimension) |
| 140876860 / 140876840 | none (trivial) | - | get/set obj+0x3980 |
| 140876640 | 1402027e0 | verified | set material group |
| 14080ea20 | 1401de420 | **not in ledger** | first saved step of a sim (1401df8a0 verified) |
| 14080eb30 | 1401de530 | **not in ledger** | last saved step of a sim |
| 140d397a0 | none | GUI | DB handle refresh |
| 14080c660 / 14080c820 | wrappers of 1401dc030 / 1401dc220 | ported | DB info init / free |
| 14080ca10 | none (analogue 1401dc3f0 verified, bodies differ) | GUI engine | DB open |
| 140814e80 | 1401e4c10 | **not in ledger** | previous DB file `%s%d.DB` chain |
| 1408150f0 | 1401e4e80 | **not in ledger** | DB chain setup |
| 140825540 | 1401cc2a0 | ledger todo, code in defdbio/dbread/lead.cpp | read DB step |
| 14087e1a0 | 14020a340 | **not in ledger** | BCC groups after DB copy |
| 140821bf0 / 140824560 | 1401c8950 / 1401cb2c0 | ported | DB_STEP_DATA ctor / dtor |
| 140897cf0 | 1401b7250 | **not in ledger** | import object KEY into model |
| 14084ee40 | 14017ac20 | **not in ledger** | first free object number |
| 140823e60 | 1401cabc0 | **not in ledger** | first object kind != 7 |
| 140823dd0 / 140823f20 / 140823fe0 | 1401cab30 / 1401cac80 / 1401cad40 | verified | object counts / first deformable / first rigid |
| 1408301c0 | 1401d6df0 | **not in ledger** | first object number (viewer) |
| 1408df7b0 | 1401f3660 | ported | sim control copy |
| 1408986a0 | 1401b7c00 | **not in ledger** | MeshSettings KEY -> obj+0x2d10 |
| 14085dd40 | 140189b70 | ported | mesh settings copy |
| 140898490 | 1401b79f0 | **not in ledger** | ObjProperties KEY -> obj+0x1ea0 |
| 1408ea510 | 14025a730 | ported | list copy |
| 140898880 | 1401b7de0 | **not in ledger** | GeoConvt23Data KEY -> obj+0x2be8 |
| 14085ceb0 | 140188c90 | **not in ledger** | copy obj+0x2be8 block |
| 140834d50 / 140834c80 | 140160cc0 / 140160bf0 | verified | has geometry / has mesh |
| 140870590 / 140873be0 | 1401fc730 / 1401ffd80 | verified | geometry from mesh / delete mesh |
| 14086c1f0 | 140198050 | verified | geometry i has an edge |
| 140835b50 / 140835a50 | 140161ac0 / 1401619c0 | ported | copy +0x3aa0 / +0x3998 |
| 1408911f0 | none (analogue 14021d2f0 ported, objpos::rotate) | GUI engine | object rotate |
| 140874110 140874520 1408935e0 140893730 140891c70 140893290 140894090 1408c0410 | 1402002b0 1402006c0 14021f6e0 14021f830 14021dd70 14021f390 140220190 14019f2c0 | verified/ported | rotate parts |
| 1408777e0 | none | GUI engine | die-geometry test (140203a20 ported / 1402039d0 verified) |
| 1408ea050 | 14025a270 | ported | WPAXIS kind 4 |
| 1408e8db0 / 1408e8e40 / 1408e8ed0 | 140258fd0 / 140259060 / 1402590f0 | ported | set +0x68 / +0x80 / +0x98 |
| 1408e99a0 | 140258330 / 140259bc0 (sig not unique) | check | WPAXIS list get-or-create |
| 1408f6d10 | 140246d00 | verified | DEF_AMGGEO ctor |
| 1408f8c00 | 140248bf0 | ported | GEO writer |
| 14086c0e0 | none (140a21a10 = 140301b40 verified) | GUI engine | geometry i |
| 1400667b0 / 1408eef90 | 140158ec0 / 140281bf0 | ported / verified | curve topology |
| 1407fbba0 / 1407fbc10 | 1401b8560 / 1401b85d0 | verified | file exists / remove |
| 1408f3380 / 1408f3510 | 140243370 / 140243500 | verified | extension / type code |
| 14081f530 / 14081f560 | 140224100 / 140224130 | ported | launch program |
| 1408358d0 | 140161840 | ported | copy geometry |
| 14086c390 | 1401981f0 | **not in ledger** | (only with param_2 != 0, not reached) |
| 1408e96a0 | 1402598c0 | ported | WPAXIS list copy |
| 14036c8e0 14036c970 14036cbb0 14036cb00 | 140220670 140221a90 1402214c0 140221c20 | ported | matrix4 |
| 1409f0660 / 14089c490 | 140375980 / 140130720 | ported | curve / dynary_get (140c7d3a0) |
| 140100980 | 140158e90 | (twin by pair.py) | model dimension |

GUI code reached but not specified line by line: viewer (140c80280 chain: 140c82930, 140c82d50,
1411f07d0, 1411f17f0, 1411f1a10, 1411ee800, 140c8e640), signals (1402ce790, 1402ce8a0,
1402cf3c0), the message log 1402d0c80, CopyMirror 140e25c00, SfStepData ctors 140c7a5b0 /
140c7a6b0, the DB handle 140cdab70 / 140d397a0.

## 9. Dialogs on this path

| Where | Condition | Text | Buttons |
|---|---|---|---|
| 140369cf0 | 2D object in a 3D op through Convert2Dto3D, `DEF_GEOPRIM.INI` missing after it was written | title tr("ERROR IN CREATING PRIMITIVE"), text tr("The input file %1 does not exists.\nPrimitive can not be generated.\n") with %1 = DEF_GEOPRIM.INI | Ok |
| 140369cf0 | `DEF_GEOPRIM3.EXE` produced no `DEF_GEOPRIM.KEY` | same title, tr("Error occurred during geometry primitive construction.\nPlease refer to file %1 for detail information.\n") with %1 = DEF_GEOPRIM3.OUT | Ok |

Non-modal: message-log lines in 14005dd20 (types 1 and 3), stderr lines in 14036a350 / 14036b180.

## 10. Open questions

1. `14005dd20` and `14005c890` returns: the decompiler shows them as `void` (stack-probe frames);
   the callers use AL. The values written above come from the locals the epilogue loads; confirm in
   asm before porting (`14005dd20` writes `[rsp+0x34e0]`, `14005c890` `[rsp+0x4de6]`).
2. `1408e99a0` has two possible DEF_PRE twins (140258330 / 140259bc0, same masked body); pick by
   the list type when porting.
3. `1400569d0`: the material-group call fires only when the slot item's list +0x20 is empty, and
   then passes -1 (`14004db80` on an empty list). Faithful, but it looks inverted; confirm with an
   oracle case that has object materials.
4. `1400569d0` dereferences the slot item (`14004e140`) without a null check.
5. `14005fd20` returns -99 for a positive own code; harmless because it is only used for -2 slots.
6. `geoConvtPath` (1401c65f0) uses the class's current op (c+0x20), not the Convert2Dto3D
   operation found by name; the fallback joins `opDir(cop)` with the already absolute path.
   Which folder holds `GeoConvt23DataNNNNN.KEY` in practice?
7. `objRotate` (1408911f0) has no DEF_PRE twin only because of 1408777e0 (owner dimension test);
   decide whether to reuse `def::cmd::objpos::rotate` with a GUI die-geometry test.
8. 14085f6d0 (movement reset), 140833b40 (default names) and 14080ca10 (DB open) are GUI-only
   engine variants with no twin: port as GUI code. 140833b40 needs a policy for the QSettings key
   `systemOption/autoObjName` (default true).
9. `0x1400b8820` (simulation number of the op) and `0x140050b20` were only skimmed; they decide the
   DB sim used by `objectFromDb`. Part A or the project reader may own them.
10. The object-file path (`SfMoObjItem+0x28`) is set by the project reader; this path only
    consumes it.
11. 14036a350: the geometry-type local is uninitialised when the cross-section file has no
    recognised extension (`GeoCS2D.GEO` always has one, so type 2).
12. 140e25c00 (CopyMirror transform) and the equipment loader 1403568a0 are outside part B.
