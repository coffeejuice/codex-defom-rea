# MDT L0 part C: skeleton around the KEY loader (SfOprBase defaults)

DEF_GUI_MO_64.exe (DEFORM v11.0.2), static analysis only. Decompiles and disassembly used here:
`gui_mo/s1/mdt/L0/C/` (`c0..c13*.c`, `m0.c` = cleaned `0x140055360`, `m.asm`, `b8b80*.asm`,
`f68e0.asm`). Pseudo-C++ keeps the binary's call order; `tr(x)` is `QObject::tr` (`0x1400654a0`,
context object `0x143d7e3a0`), which returns the literal when no translator is installed.

Names used below:

| Name | What | How it is reached |
|---|---|---|
| `OPI` | operation item (data of one operation in a sub-project) | diagram item `+0x48` (`0x140045f90`) |
| `OB` | per-class operation handler (SfOprBase, SfOprCogging, ...), one per class | `0x140054e90(opi)`: main-window registry, key = class name `OPI+0x120` |
| `P` | SfProject | `OPI+0x100` (`0x140055030`) |
| `SP` | active SfSubProject of `P` | `0x140055050(opi)` = `0x1400a7390(P)` |
| `DBH` | DB handle object (QObject, 0x19c0 bytes) | `0x1400b9cd0(SP)` |
| `DBR` | DB step-header reader inside DBH | `DBH+0x40` (`0x140cdab70`) |
| `M` | GUI engine model wrapper (0x278 bytes) | `0x14005f770(opi)` |
| `S` | DEF_PRE `DB_STEP_DATA` | `M+0xe0` (QPointer, `0x140c7acd0`) |
| `SC` | DEF_PRE sim-control block `Cls_1401e7b10` | `S+0x750` (`0x140c7b2d0`) |
| `MSG` | `SfMessenger` global object `0x143d76700` | signals to the UI |
| `APP` | global `0x143d766b8`: `+0` main window, `+0xc` proc type (5 = MO) | |

UI-only calls (dropped in the converter, listed per function): `MSG` signals through
`0x1402cf3c0` (`QMetaObject::invokeMethod` style "2signalXxx()"), `0x1402ce790/8a0/9c0/ae0`
(MSG signals 1, 2, 3, 4; skipped when `MSG+0x141` is set), status bar `0x1402d0bd0`, tree-model
code (`OB+0x18`, `OPI+0x160`, QStandardItem), step browser `0x140cdd800`, diagram editor
`0x140088090`, viewer `0x1411fxxxx`. Dropping them changes no operation, model or file state
(each was read to its leaves, or ends in Qt widgets/signals, except where an open question says so).

---

## 1. Activation after File > Open

### 1.1 Who calls `0x140047f80`

Callers (`callers.py`): `0x14003a3c0` and `0x140048280`.

* `0x14003a3c0` = **`SfMoMainWindow::slotProjectChanged(SfProject* p)`** (moc slot index 0x29 of
  SfMoMainWindow, dispatched by `qt_static_metacall` `0x1400db1a0`; signature from the moc string
  table at file offset 0x216dad0). Also called directly by `0x14003a500`
  (`slotDiagEditorVisibilityChanged(bool)`, slot 0xb) with the main window's project.
* `0x140048280` = diagram model "set active item id" (selection); returns early when the id is
  already active (`model+0x2c == id`).

```cpp
// 0x14003a3c0  void SfMoMainWindow::slotProjectChanged(SfProject* p)
void slotProjectChanged(MainWin* self, SfProject* p) {
    DBH* dbh = nullptr; DiagModel* model = nullptr; int activeId = -1;
    if (p) {
        SubProject* sp = 0x1400a7390(p);                 // active sub-project
        if (sp) { dbh = 0x1400b9cd0(sp); activeId = 0x1400b5e30(sp); } // active item id: d = *(sp+0); d ? (d+0x10)->vt+0xe8() : 0
        auto* x = 0x140045590(0x140c9b7c0(self), p);
        if (x) model = 0x140044c80(x);                   // diagram model of p
    }
    if (dbh && DBH_reader(dbh)) 0x140d397a0(DBH_reader(dbh), 0);   // refresh DB step cache, §1.9
    if (model) {
        0x140047af0(model, activeId);   // model+0x2c = activeId (also pushed to sp if >= 1; "nId < 1" qDebug otherwise)
        if (!0x140047f80(model, -1))
            0x14002aae0(self);          // UI: MSG signals 4,3,1 with 0 (clear views)
        else {
            int id = 0x140047a40(model);              // model+0x2c
            if (id > -1) 0x140038950(self, id);       // -> 0x140048280(model,id): id already active -> returns true
        }                                              //    then UI: signalDoeTableRefresh, vt+0x2a0, 0x140d3b300
    }
}
```

The signal that triggers slot 0x29 at File > Open is not connected by a string found in the exe
(open question Q1). On the open path the loader therefore runs once, from here, with id `-1`.

### 1.2 `0x140047f80` `bool activateItem(DiagModel* model, int id)`

```cpp
bool activateItem(DiagModel* model, int id) {
    Item* it = 0x140047950(model, &id);       // model->vt+0x1c8(&id), dynamic_cast to op-item (0x140047910)
    if (!it) it = 0x140047a00(model);         // item with id model+0x2c (0x140047a40 also touches UI only)
    if (it && (opi = it->+0x48) && 0x140054e90(opi)) {
        OB* ob = 0x140054e90(opi);
        if (0x1405ef9d0(ob)) {                // class gate, §1.4
            if (abs(id) == 1) 0x140051c30(opi);   // 0x1417f4b4c = abs()
            else              0x140051340(opi);
            return true;
        }
    }
    return false;
}
```

**Correction to `loader-spec.md` §1:** with `id = -1` (slotProjectChanged) `abs(id) == 1`, so the
open path runs **`0x140051c30`**, not `0x140051340`. `0x140051340` runs when the user selects
another item (id > 1) or from `0x141424340` (FormingExpress). The two differ only in the status
text and in one model write (§1.3).

### 1.3 `0x140051c30` / `0x140051340` `void openOperation(OPI* opi)`

Same body; differences marked [C30] (`0x140051c30`) and [340] (`0x140051340`).

```cpp
void openOperation(OPI* opi) {
    Model* m = 0x14005f770(opi);              // get/create engine model, §1.6
    if (!m) return;
    0x140c7b720(m);                           // reset model, §1.7
    OB* ob = 0x140054e90(opi);
    if (!ob) return;
    // [C30] UI: MSG signalShowMessage(false, MSG_TYPE 1, tr("New project has been created."))
    // UI: MSG signalShowMessage(false, 1, tr("Open operation - %1").arg(opi->name /*+0x118*/, 0, ' '))
    0x1405ef280(ob, opi);                     // ob->current (QPointer OB+0x20) = opi
    0x1400c3950(0x1405ef3a0(ob));             // UI: clear OB object-tree container, §1.5
    QString dir; 0x1400a4220(P(opi), &dir);   // cleanPath(QFileInfo(P+0x18).absolutePath())
    QDir::setCurrent(dir);                    // KEEP: cwd = project folder (relative paths later)
    SubProject* sp = 0x140055050(opi);
    if (sp && (dbh = 0x1400b9cd0(sp)) && DBH_reader(dbh))
        0x140d397a0(DBH_reader(dbh), 0);      // refresh DB step cache if the .DB changed, §1.9
    0x1405ef280(ob, opi);                     // again
    ob->vt[0x58](ob);                         // OB load: SfOprBase 0x1405ef430, §2
    SC* sc = 0x140c7b2d0(m);
    // [340] only:
    //   if (sc) { QString t = P(opi)->+0x28 /*0x140ceed20*/; strcpy(sc->title /*SC+0*/, t.toLatin1().data()); }
    //   (0x14146fec0 = CRT strcpy, no bound; SC title is char[0x51])
    if (0x1400509d0(opi))                     // OprFlag (OPI+0x10c) == 0
        0x140050930(opi, 1);                  // OprFlag = 1
    0x140064cb0(opi);                         // object material lists GUI <- engine, §1.10
    // UI: MSG signalShowMessage(false, 1, tr("Done - open operation"))
}
```

Fields: reads `OPI+0x118` (name), `+0x100` (P), `P+0x18` (.moproj path), [340] `P+0x28` (project
name); writes `OB+0x20`, `OPI+0x10c`, cwd, [340] `SC+0x000`.

### 1.4 Class gate `0x1405ef9d0` `bool OB::isAvailable()`

```cpp
bool isAvailable(OB* ob) {
    OPI* opi = 0x1405ef2b0(ob);               // QPointer OB+0x20
    if (!opi) return false;
    if (!0x140055030(opi)) return false;      // OPI+0x100 (project)
    QString f; 0x1405ef960(ob, &f);           // string at (OB+0x28)+0x18 (0x1400d7fc0)
    if (f.isEmpty()) return true;
    return 0x1400aab60(P, f, true);           // @src 1400aab60  omitted: licence
}
```

`0x1400aab60` (with `0x1400aa960`, `0x1412ec7b0`, `0x1412f08d0`) is a licence feature check and is
not analysed. Port: `return opi && opi->project;` (the recreated programs have no protection).
`0x1405ef960` is a plain accessor and may stay unused.

### 1.5 Small accessors on this path

| Addr | Signature | Body |
|---|---|---|
| `0x1405ef280` | `void OB::setCurrent(OPI*)` | `QPointer(OB+0x20) = opi` (`0x140cbc920`) |
| `0x1405ef2b0` | `OPI* OB::current()` | `*(OB+0x20)` |
| `0x1405ef2e0` | `Model* OB::currentModel()` | `cur ? 0x14005f770(cur) : 0` |
| `0x1405ef3a0` | `Obj* OB::objectTree()` | UI. `OB+0x18` (0x50-byte list object); sets flag bit0 at `(OB+0x18)+0x10`, clears it when the main window's sub-project is DOE (type 2, `0x140014170`) or Optimization (type 3, `0x1400141c0`); returns `OB+0x18` |
| `0x1400c3950` | `void clear(list)` | UI. `while (count(list+8) > 0) 0x1400c3b90(list)` (deletes last element via vt+0x18) |
| `0x1400a4220` | `QString projectDir(P)` | `QDir::cleanPath(QFileInfo(P+0x18).absolutePath())` |
| `0x140ceed20` | `QString P::name()` | copy of `P+0x28` |
| `0x140cdab70` | `DBR* reader(DBH)` | `DBH+0x40` (never null when DBH is not) |
| `0x1400509d0` | `bool OPI::isNew()` | `OPI+0x10c == 0` |
| `0x140050930` | `void OPI::setOprFlag(int)` | `OPI+0x10c = v` |
| `0x140050ac0` | `QString OPI::name()` | `OPI+0x118` |
| `0x140050d00` | `QString OPI::className()` | `OPI+0x120` |
| `0x140050910` | `int OPI::id()` | `OPI+0x108` |
| `0x140050b20` | `int OPI::index()` | position of `OPI+0x108` in the sub-project chain (`0x1400b5f40` -> `0x140085800`); 0 without SP |
| `0x140050bc0` | `bool OPI::isFirst()` | `0x140086720(diagram, OPI+0x108) == -1` (no predecessor item) |
| `0x140062960` | `OPI* OPI::previous()` | predecessor id `0x140062df0` (-1 -> null), then item lookup `0x140047dc0` in `0x140055090(opi)` |
| `0x140064740` | `bool OPI::restartMode()` | `OPI+0x150` |
| `0x14005a2c0` / `0x14005a350` | NSTART flag / value | `OPI+0x18` (u8) / `OPI+0x1c` (int) |
| `0x14005a2a0` / `0x14005a2e0` | getters of the same | |
| `0x140065530` | `int guiMode()` | global `0x143d9da20` |
| `0x1405f7ad0` | `int interactiveStep()` | global `0x143d9da24` |

`OprFlag` (`OPI+0x10c`) is read from the `.moproj` `<OprFlag>` element by the operation walk
`0x14008ccf0` (`toInt`, default 1 when the element is absent); the OPI constructor
`0x140050550` sets 0 and also `OPI+0x110 = 0`, `+0x140 = 0`, `+0x150 = 0` (restart off),
`+0x158 = "RestartStep.KEY"`. So 0 means "operation never set up"; the loader sets it to 1.

### 1.6 Model access `0x14005f770` (= `0x14005f820`) and model creation OB vt+0x1b0

```cpp
Model* opModel(OPI* opi) {
    OB* ob = 0x140054e90(opi);
    if (!ob) return nullptr;
    Model* m = ob->vt[0xe0](ob, opi);         // SfOprBase: 0x140100e20 returns 0
    if (m) return m;
    if (!QPointer(opi+0x38)) {
        0x1405ef280(ob, opi);
        QPointer(opi+0x38) = ob->vt[0x1b0](ob);   // SfOprBase: 0x1405f6410
    }
    return QPointer(opi+0x38);
}
// 0x1405f6410  Model* SfOprBase::createModel()
Model* createModel(OB* ob) {
    OPI* opi = 0x1405ef2b0(ob); if (!opi) return 0;
    P* p = opi->+0x100;           if (!p)   return 0;
    Model* m = new(0x278) 0x140c7a5b0(p->+8 /*DBH, 0x1400a5890*/, ob->vt[0x190](ob) /*dim*/);
    0x140822210(m->S, 0x1400a2180(p), 1);      // twin 1401c8f70 (not in ledger)
    0x140c7b2d0(m)->+0x650 = 1;                // SC TNOW 3 = 1
    return m;
}
```

OB vt+0x190 = `0x1405f5bf0`: `f = ob->vt[0x188]()->vt[0x58]()` (class info flags, a virtual base at
`OB+0x30`): `(f&3)==3 -> 4`, `f&1 -> 2`, `f&2 -> 3`, else 0.

### 1.7 Model reset `0x140c7b720` (GUI wrapper, no twin)

```cpp
void resetModel(Model* m) {
    if (!m->S) return;
    0x140c82c70(m);              // viewer: remove every item of list M+0x18 (0x140c82b70)
    0x140c8ba80(m);              // delete M+0x30 (0x140c91940), M+0x38 = 0
    0x1408246a0(m->S);           // engine: DB_STEP_DATA clear. twin 1401cb400 (no ledger row;
                                 //   its callees 1401cb440, 1401c8c00 are ported)
    0x140c902c0(m);              // delete M+0x230 (vt+0x18)
    int z = 0; 0x140c833c0(m, &z);   // M+0x100 = 0
}
```

### 1.8 Sub-project DB handle `0x1400b9cd0` `DBH* dbHandle(SubProject* sp)`

```cpp
DBH* dbHandle(SubProject* sp) {
    P* parent = sp->+0x10;            // 0x1400b5c40
    if (!parent) return nullptr;
    DBH* h = parent->+8;              // 0x1400a5890: project DB handle
    if (sp->type /*+0x18*/ == 4) {    // sub-project with its own DB
        if (!QPointer(sp+0x48)) {
            QString name; 0x1400b9e80(sp, &name, true);     // type 4 -> 0x1400bbee0
            QPointer(sp+0x48) = new(0x19c0) 0x140cda460(nullptr);
            0x140cdcdc0(QPointer(sp+0x48), name, false);    // set .DB path (appends ".DB" unless
        }                                                   //  it ends FOR003), reads
        h = QPointer(sp+0x48);                              //  <dir>/.ObjectColor.DAT
    }
    return h;
}
```

### 1.9 DB refresh `0x140d397a0` `bool DBR::refreshIfChanged(int)`

```cpp
bool refreshIfChanged(DBR* r, int arg) {
    bool ok = false;
    QString path; 0x140cdd190(r->+0x1918, &path, true);  // owner DBH: +0x28 path if +0x20 set, else null
    QFileInfo fi(path);
    if (fi.exists()) {
        QDateTime t = fi.lastModified();
        if (!r->stamp(+0x1910).isValid() || (r->stamp.isValid() && t != r->stamp)) {
            if (0x140d395c0(r, path, false)) {     // reopen: 0x14080ca10(r, path) == 0 ->
                                                   //   +0x1910 = lastModified, +0x1908 = 0
                bool f = false; r->+0x1908 = 0;
                0x140d396d0(r, &f);                // +0x1908 = 1; f == false -> 0x14080d870(r, 0)
                0x140cdd800(r->+0x1918, arg);      // UI: step browser refresh
                ok = true;
            }
        } else {
            0x14080d6f0(r);                        // engine, twin 1401dd0d0 (ported)
            ok = true;
        }
    }
    return ok;   // (toString() of both stamps is computed and discarded)
}
```

`0x14080d870` (twin `1401dd250`, verified) builds the step-header cache `DBR+0xae8` once
(`DBR+0x1900` = built). `0x14080ca10` (header open) has no twin.

### 1.10 `0x140064cb0` `void syncObjectMaterials(OPI* opi)`

```cpp
void syncObjectMaterials(OPI* opi) {
    Model* m = 0x14005f770(opi); if (!m) return;
    int n = 0x140c7c1a0(m);                       // object count of S (0x140830180)
    for (int i = 0; i < n; ++i) {
        LLOBJ_DATA* o = 0x140c7aed0(m, i + 1);    // object at position i+1 (0x1408302a0)
        if (o) 0x140064d40(opi, o->+8 /*object number*/);
    }
}
// 0x140064d40  bool syncSlot(OPI*, int objNo): GUI object slot (0x1400612d0(opi,objNo) = slot objNo-1)
// gets the object's material ids, only for objects whose kind (+0xc) is not 7 (undefined) and not 5:
//   0x140064e60: object has a mesh (0x140834c80, twin 140160bf0) -> clear list,
//                append each positive distinct element material (0x14086f6a0 = 1401fb840, buffer of
//                max(0x1408bc4a0(S+0x1328), else 100) ints)
//   else 0x140065020: object has geometry (0x140834d50 = 140160cc0) -> per face of
//                0x14086c0e0(o,0): positive face material (0x1408ec3a0) -> list
//   else: clear list; if 0x140876860(o) > 0 append it
```

Writes the GUI slot's material-id list (`0x14004db20` clear, `0x14004dc20` append). Not UI.

---

## 2. OB vt+0x58 `0x1405ef430` `void SfOprBase::load()`

Used by SfOprBase and 22 subclasses (Boolean, Cogging, Diestress*, Distortion, Express*, Forming*,
HTFurnace, HeatTransfer*, MultiBlow*). Own vt+0x58 (some call this one, callers.py): Convert2Dto3D
`0x1401c3a60`, CopyMirror `0x140e24b80`, HeatTreat2D `0x1411aec20`, HeatTreat `0x1411b1240`, Group
`0x1413524f0` (these five call `0x1405ef430`); Cycle `0x14134d7b0`, Pre `0x141348e80`, Pre2D
`0x141347980`, DoeOutput `0x140e2e320`, ShapeOptim `0x14013beb0`, Report `0x1412add20`.

```cpp
void SfOprBase_load(OB* self) {
    OPI* opi = 0x1405ef2b0(self);
    if (!opi) return;
    Model* m = 0x1405ef2e0(self);
    if (!m) return;
    0x140c7b720(m);              // reset (second time on the open path)
    self->vt[0xb8](self);        // load prologue  (base 0x1405f1bb0)
    self->vt[0x78](self);        // SfOprBase: pure virtual (0x141470184 = _purecall); every class overrides
    self->vt[0x1b8](self);       // KEY/DB load    (base 0x1405f6520)
    self->vt[0xc0](self);        // post-load      (base 0x1405f1c00)
    0x140054350(opi);            // props->vt+0x58
    0x1400547b0(opi, false);     // UI refresh (all UI, §2.6)
    0x140054250(opi);            // UI viewport
}
```

Slot users (vtables.tsv, operation classes only):

| Slot | Base | Classes on the base | Overrides |
|---|---|---|---|
| vt+0xb8 | `0x1405f1bb0` | SfOprBase, Pre, Pre2D | all other classes (e.g. Cogging `0x140750650`, Forming `0x140c060f0`, MultiBlow `0x140601f70`, HeatTransfer `0x1413543a0`) |
| vt+0x78 | pure | none | every concrete class (Cogging `0x140750790` empty, Forming `0x140c06760`, ...) |
| vt+0x1b8 | `0x1405f6520` | all except below | ShapeOptim `0x14013cbe0`, DiestressBM2D `0x14029b570`, DiestressBM3D `0x14029d3b0`, Diestress `0x1402b59e0` |
| vt+0xc0 | `0x1405f1c00` | SfOprBase, HeatTreat, HeatTreat2D, Pre, Pre2D, ShapeOptim | all others (Cogging `0x1407507b0`, Forming `0x140c06130`, ...) |
| vt+0x1f0 | `0x1405f68e0` | all | none |
| vt+0x1f8 | `0x1405f79a0` (false) | Base, Boolean, Cogging, CopyMirror, Cycle, Diestress*, Distortion, DoeOutput, Group, HTFurnace, MultiBlow*, Pre*, Report, ShapeOptim | `0x1413e5ef0` (true): Convert2Dto3D, ExpressBase, Forming, Forming2D, FormingExpress*, HeatTransfer*, HeatTransferExpress*, HeatTreat, HeatTreat2D |

### 2.1 vt+0xb8 base `0x1405f1bb0` `void SfOprBase::loadPrologue()`

```cpp
void loadPrologue(OB* self) {
    0x1405ef620(self, true);   // SC+0x650 (TNOW 3, int) = 1
    0x1405ef690(self);         // SC+0x648 (TNOW 2, double) = 0.0 (64-bit zero store)
    0x1405ef6f0(self, 1);      // SC+0x108 (TRANS 2) = 1
    0x1405ef750(self, 1);      // SC+0x104 (TRANS 1) = 1
}
```

Each setter: `m = 0x1405ef2e0(self); if (m && (sc = 0x140c7b2d0(m))) sc->field = value;`
(`0x1405ef620` stores `(uint)(arg != 0)`, the two TRANS setters store the byte zero-extended).

### 2.2 vt+0x1b8 base `0x1405f6520` `void SfOprBase::loadData()`

```cpp
void loadData(OB* self) {
    OPI* opi = 0x1405ef2b0(self);
    if (!opi) return;
    if (guiMode() == 0)                                   0x140055360(opi, true);   // §3
    else if (guiMode() == 1 && interactiveStep() != 0)    0x1400572e0(opi, interactiveStep(), true);
    else if (guiMode() == 2)                              0x1400569d0(opi, true);
    else                                                  0x140055360(opi, true);
}
```

`guiMode()` reads `0x143d9da20` three times (each test calls `0x140065530` again);
`interactiveStep()` reads `0x143d9da24` twice. After a fresh start the mode is 0 (set only by
`0x140062f00` / `0x1400636d0`), so the converter always takes `0x140055360`.

### 2.3 vt+0xc0 base `0x1405f1c00` `void SfOprBase::postLoad()`

```cpp
void postLoad(OB* self) {
    if (!0x1405ef3a0(self)) return;           // OB+0x18 (UI object, side effect on its flag bit only)
    OPI* opi = 0x1405ef2b0(self); if (!opi) return;
    Model* m = 0x14005f770(opi);  if (!m)   return;
    int nObj = 0x140c7c1a0(m);
    int n = 1;
    OPI* prev = 0x140062960(opi);
    if (prev) n = 0x14005fab0(prev);          // count of prev's slots whose type passes 0x140060640
                                              //   (0x140060690(t) || 0x1400606c0(t))
    if (nObj == 0 && n > 0) {
        for (int i = 0; i < n; ++i) {
            0x140057ba0(opi);                 // add one object (GUI list + engine, 0x140057c60(opi,id,type,0,1,1))
            if (0x140062960(opi)) {           // prev looked up again
                LLOBJ_DATA* o = 0x140057710(opi, i + 1);   // object by number (0x140c7ae80)
                o->kind /*+0xc*/ = 5;         // no null check in the binary
            }
            0x140060100(opi, i + 1);          // apply slot properties to object i+1
        }
    }
    if (0x1400509d0(opi) && prev) {           // OprFlag == 0 (first set-up) and a previous op
        SC* d = 0x14005f7e0(opi), *s = 0x14005f7e0(prev);   // SC of each model
        if (d && s) {
            for (int i = 0; i < 7;  ++i) d->stnout[i] /*+0x11c*/ = s->stnout[i];
            for (int i = 0; i < 10; ++i) d->elmnod[i] /*+0x138*/ = s->elmnod[i];
        }
    }
}
```

### 2.4 `0x140054350` `void propsLoad(OPI* opi)`

```cpp
void propsLoad(OPI* opi) {
    (void)APP->mainWindow;                     // 0x1402c8090(0x143d766b8), result unused
    Props* pr = opi->+0x10;                    // 0x140051230
    if (pr) pr->vt[0x58](pr);
}
```

Properties vt+0x58: base `0x1405fb600` (`return false`, nothing done) for SfOprBaseProperties,
Boolean, Cycle, DiestressBM, Diestress, Express. Overrides: Cogging `0x14075ee20`, Forming
`0x140c09da0`, FormingExpress `0x14142a6a0`, MultiBlow `0x140612ba0`, HeatTransfer `0x141357520`,
HeatTransferExpress `0x1413ed890`, HeatTreat `0x1411bfd60`, HTFurnace `0x140c69480`,
Convert2Dto3D `0x1401ce3e0`, CopyMirror `0x140e2d080`, Distortion `0x1413665c0`, DoeOutput
`0x140e36290`, Group `0x141352e70`, Pre `0x14134c380`, Report `0x1412b1f10`, ShapeOptim
`0x140142ca0` (bodies belong to the per-class packets).

### 2.5 `0x1400547b0` `void refreshViews(OPI* opi, bool keep)` (all UI)

```cpp
void refreshViews(OPI* opi, bool keep) {
    0x140054820(opi);   // MSG signal 1 (model 0x14005f770(opi)); MSG signal 2 ("viewFit", null)
    0x1400548c0(opi, keep);  // if APP+0xc == 5: 0x140054950 (OB vt+0xa0/0xa8/0x278 fill the object
                             //   tree QStandardItemModel at OPI+0x160, MSG signal 4) else 0x140054c80
                             //   (OB vt+0xb0, tree model, MSG signal 4). SfOprBase: vt+0xa0 = true,
                             //   vt+0xa8 / vt+0xb0 empty, vt+0x278 = 0x1405f15e0 (tree rows,
                             //   "No objects" / " object" / " objects" label)
    0x140054df0(opi);   // MSG signal 3 with props widget (OPI+0x10)+0x10
    if (guiMode() != 0) { sp = 0x140055050(opi); if (sp) 0x1400bc5b0(sp); }  // diagram editor update
}
```

`0x140054950` creates `OPI+0x160` (`new(0x50) 0x1400c4890`) when null; that QPointer is only a
tree model (droppable). `guiMode()` is read here a fourth time on the path.

### 2.6 `0x140054250` `void setViewport(OPI* opi)` (UI)

`if (0x14005f150(opi) /* OB vt+0x190 == 2, i.e. 2D */) MSG signal 2("viewportZPlus") else MSG
signal 2("viewportIso")`.

### 2.7 vt+0x1f0 `0x1405f68e0` and vt+0x1f8

`0x1405f68e0`: `return (vbase(OB+0x30)->vt[0x58]() & 0x01000000) != 0;` (same flags word as the
2D/3D bits of vt+0x190; `vbase = OB+0x30 + *(int*)(*(OB+0x30)+4)`). vt+0x1f8 is a constant per
class (table above): "class supports restart".

---

## 3. Mode-0 decision `0x140055360` `Model* decideAndLoad(OPI* opi, bool withObjects)`

The return value is never used by its callers (stored in a local only). Order and literals as in
the binary (`m0.c`, `m.asm`).

```cpp
void decideAndLoad(OPI* opi, bool withObjects) {
    Model* m = nullptr; int step = 0;
    int idx = 0x140050b20(opi);                       // operation index in the chain
    SubProject* sp = 0x140055050(opi);   if (!sp)  return;
    DBH* dbh = 0x1400b9cd0(sp);          if (!dbh) return;
    OB* ob = 0x140054e90(opi);           if (!ob)  return;

    if (0x140050bc0(opi) /*first op*/ || !ob->vt[0x1f8](ob) /*no restart support*/) {
        0x140064760(opi, false, true);                // restart mode off (+signal if it changed)
        m = 0x1400569d0(opi, withObjects);            // KEY load (part A/B)
        return;
    }

    // ---- restart branch ----
    QString rf; 0x140064a50(opi, &rf, true);          // absolute restart file path, §3.2
    if (0x140064740(opi) /*restart on*/ && QFile::exists(rf)) {
        0x140064760(opi, true, true);                 // unchanged -> no signal
        m = 0x140057230(opi);                         // load restart file, §3.3
        return;
    }
    if (QFile::exists(rf)) QFile::remove(rf);         // KEEP: stale restart file is deleted
    if (0x140064740(opi)) {
        QString title = tr("Missing Restart File");
        QString text  = tr("Restart mode is ON, but there is no restart file.") + "\n"
                      + tr("Restart mode will be deactivated.");
        QMessageBox::critical(APP->mainWindow, title, text, QMessageBox::Ok /*0x400*/, QMessageBox::NoButton);
        0x140064760(opi, false, true);                // restart off
    }

    // ---- setup-type branch ----
    bool interactive = false, useDb = false, lastOfPrev = false;
    if (0x1400509d0(opi) /*OprFlag == 0*/) {
        int lastMatch = 0, unused = 0;
        if (0x14005f520(opi, &lastMatch, &unused)) {  // DB holds steps usable for this op, §3.4
            if (!g_keepChoice /*0x143d77584*/) {
                QString title = tr("Setup Type (Interactive/Batch)");
                QString text = tr("[Op. %1] %2").arg(0x140050b20(opi), 0, 10, ' ').arg(opi->name, 0, ' ')
                    + "\n" + tr("You have a stored step in database that can be used to setup interactively.")
                    + "\n" + "\n" + tr("Do you want to start interactive mode?")
                    + "\n" + "\n" + tr("   Yes - Interactive Setup: All objects will be current. Restart file will be created.")
                    + "\n" + tr("   No  - Batch Setup: Future operation can be defined using Read-from-DB object type.")
                    + "\n" + "\n" + tr("Note")
                    + "\n" + tr("There can be a discontinuity in simulation by choosing interactive setup.")
                    + "\n" + tr("If you want to run DOE or Optimization problem, batch setup is recommended.")
                    + "\n" + tr("To keep your choice and don't want to see this dialog again, click the check box below.");
                QMessageBox box(QMessageBox::Question /*4*/, title, text, QMessageBox::NoButton,
                                APP->mainWindow, Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint /*3|0x100*/);
                QAbstractButton* yes = box.addButton(QMessageBox::Yes /*0x4000*/);
                QAbstractButton* no  = box.addButton(QMessageBox::No  /*0x10000*/);
                yes->setText(tr("YES - Interactive Setup"));
                no->setText(tr("NO - Batch Setup"));
                box.setDefaultButton(QMessageBox::Yes);
                QCheckBox keep(tr("Keep my choice and don't prompt"), &box);
                keep.blockSignals(true);
                box.addButton(&keep, QMessageBox::ActionRole /*3*/);
                interactive = (box.exec() == QMessageBox::Yes);
                if (keep.checkState() == Qt::Checked /*2*/) {
                    g_keepChoice = 1;                 // 0x143d77584, process lifetime only
                    g_keptAnswer = interactive;       // 0x143d77585
                }
            } else {
                interactive = g_keptAnswer;
            }
            if (interactive) {
                useDb = true;
                DBR* r = 0x140cdab70(dbh);
                if (opi->className() != "SfOprConvert2Dto3D") {
                    if (lastMatch < idx) { step = 0x14080e710(r, idx - 1); lastOfPrev = true; }   // last step of op idx-1
                    else                 { step = 0x14080e5e0(r, idx);     lastOfPrev = false; }  // first step of op idx
                } else {
                    step = 0x14080e710(r, idx - 1); lastOfPrev = true;
                }
            }
        }
    }

    if (!useDb || step == 0) {
        m = 0x1400569d0(opi, withObjects);            // KEY load
        0x140064760(opi, false, true);                // note: after the load here
        return;
    }

    // ---- DB branch ----
    SimCtrl keepSc; 0x1408d3bc0(&keepSc);             // stack SC (ctor; +0xfc = global 0x143d79724)
    bool kept = false;
    if (QPointer(opi+0x38) && guiMode() == 1) {       // never on the converter path (mode 0)
        kept = true;
        0x1408d3cd0(&keepSc, QPointer(opi+0x38)->S);  // owner = S, init (twin 1401e7c10)
        0x1408df7b0(&keepSc, 0x140c7b2d0(QPointer(opi+0x38)));   // copy SC (twin 1401f3660)
    }
    m = 0x1400572e0(opi, step, true);                 // load DB step, §3.5
    int ns = 0x140c7b2d0(m)->nstart /*+0x160*/;
    if (ns < 1) { if (lastOfPrev) ns = ns - 1; }
    else        ns = -(ns + 1);
    if (kept) 0x1408df7b0(0x140c7b2d0(m), &keepSc);   // restore the SC
    0x140c7b2d0(m)->nstart = ns;                      // SC+0x160
    0x140c7acd0(m)->+0x744 = step;                    // S+0x744 (DB step to read)
    0x140c7b2d0(m)->+0x650 = 1;                       // TNOW 3
    0x140c7b2d0(m)->+0x648 = 0;                       // TNOW 2 (64-bit zero)
    0x14005a2c0(opi, true);                           // OPI+0x18 = 1 (NSTART set)
    0x14005a350(opi, ns);                             // OPI+0x1c = ns
    QString cn = opi->className();
    bool ht = cn.contains("SfOprHeatTransferExpress", Qt::CaseSensitive)
           || cn.contains("SfOprHeatExpress", Qt::CaseSensitive)
           || cn.contains("SfOprAirConvect", Qt::CaseSensitive)
           || cn.contains("SfOprHeatResting", Qt::CaseSensitive)
           || cn.contains("SfOprHeatDwelling", Qt::CaseSensitive);   // short-circuit, in this order
    if (ht) {
        double tmax = SC(m)->tmax;                    // +0x178
        0x1408d5100(SC(m));                           // limits reset (twin 1401e8fb0)
        SC(m)->tmax = tmax;
        0x1408e0000(SC(m), 0);                        // SC+0x108 (TRANS 2) = 0
        0x1408dffe0(SC(m), 1);                        // SC+0x104 (TRANS 1) = 1
    }
    // UI: status bar + message log 0x1402d0bd0(0x143d76350, tr("Start step number has been set to %1").arg(ns, 0, 10, ' '), 0, 0)
    0x140064760(opi, true, true);                     // restart mode ON
    0x1408d3d70(&keepSc);                             // SC dtor
}
```

`0x1408d5100` zeroes `SC+0x170`, `+0x178`, `+0x180..+0x1c8` (3 x 3 doubles), `+0x288/0x28c/0x290`
(int), `+0x298`, `+0xbc8`; the caller puts TMAX back.

### 3.1 `0x140064760` `void OPI::setRestartMode(bool on, bool notify)`

```cpp
if (opi->+0x150 != on) { opi->+0x150 = on;
    if (notify) /* UI: MSG signalSetOprInteractiveMode(bool on, int opi->id) */ ; }
```

The state write is kept; the signal is UI (open question Q5).

### 3.2 `0x140064a50` `QString restartFilePath(OPI*, QString* out, bool absolute)`

```cpp
QString s = 0x140050df0(opi) + QDir::separator() + opi->+0x158;   // op folder + "RestartStep.KEY"
if (!absolute) s = QDir(0x140050df0(opi)).relativeFilePath(s);
*out = QDir::cleanPath(s);
// 0x140050df0: P ? convertSeparators(cleanPath(projectDir(P) + sep + OPI+0x130)) : QString()
```

### 3.3 `0x140057230` `Model* loadRestart(OPI*)`

```cpp
Model* m = 0x14005f770(opi); if (!m) return 0;
0x140c7b720(m);
QString f; 0x140064a50(opi, &f, true);
0x140c7d020(m, f);        // 0x1408969a0(S, f.toLocal8Bit(), &reset=1, &flags=0) = DEF_PRE KEY
                          //   loader 1401b5f00 (ported); on 0: 0x140c7d3a0, 0x140c7ebe0 (furnace
                          //   table 0xe / object env table 0xd of SC+0x440 into GUI tables M+0xf8
                          //   and objects 0x1401b2660 / 0x1401b2470)
0x140c7ff80(m, 1, 1);     // viewer sync (0x1411f17f0, 0x1411f1a10, 0x1411ee800: viewer code)
0x1400538f0(opi, 1, 1);   // GUI slots <- engine model (same as the DB-step path)
return m;
```

### 3.4 `0x14005f520` `bool dbHasStepsFor(OPI*, int* lastMatch, int* unused)`

```cpp
bool dbHasStepsFor(OPI* opi, int* lastMatch, int* unused) {
    SubProject* sp = 0x140055050(opi); if (!sp) return false;
    DBH* dbh = 0x1400b9cd0(sp);        if (!dbh) return false;
    bool r = false;
    int idx = 0x140050b20(opi);
    int x = 0x140050b70(opi);                         // 0x1400b8820(sp, idx); value unused by the callee
    if (0x1400b8b80(sp, idx, x, lastMatch, unused) && 0x140cdab70(dbh)) {
        int n = 0x14080e840(0x140cdab70(dbh), idx - 1);   // number of DB steps of op idx-1
        OPI* prev = 0x140062960(opi);
        if (!prev) r = true;
        else {
            OB* pob = 0x140054e90(0x140062960(opi));  // no null check in the binary
            if (pob->vt[0x1f0](pob) && n >= 1) r = true;
            else if (n > 1)                    r = true;
        }
    }
    return r;
}
```

`0x1400b8b80` `bool prefixMatchesDb(SubProject* sp, int idx, int, int* lastMatch, int*)`
(decompile drops the return value; from `b8b80*.asm`):

```cpp
DBH* dbh = 0x1400b9cd0(sp); if (!dbh) return false;
QList<DbOp> dbOps; 0x14080f690(0x140cdab70(dbh), &dbOps);   // GUI-only, no twin: for each DBR
        // header record i (count DBR+0xac8) with rec28[i]+0x24 != 0 and rece0[i]+0x38 > last:
        // append {name = rece0[i]+0x4c, opNo = rece0[i]+0x38}; last = that opNo
QList<OPI*> ops; 0x1400b94f0(sp, &ops);   // chain items of sp, index = OPI index - 1, skipping
                                          // SfOprReport, SfOprDoeOutput, SfOprShapeOptim
int k = 1;
foreach (OPI* o, ops) {
    QString nm = o->name();  (void)0x140050b10(o);   // returns 0, unused
    DbOp d = dbOps[k - 1];                           // no bounds check (Qt asserts only in debug)
    if (!(QString::fromLocal8Bit(d.name) == nm && d.opNo /*+0x28*/ == k)) break;
    *lastMatch = k; ++k;
}
return *lastMatch + 1 >= idx;    // `*lastMatch` is not initialised here; the caller sets 0
```

`0x14080e840(r, opNo)`, `0x14080e710(r, opNo)`, `0x14080e5e0(r, opNo)` each build the cache if
needed (`if (!r->+0x1900) 0x14080d870(r, 0)`) and call `0x14080fc60(r, opNo, a..k)` (scan of
header records with `rec28+0x24 != 0 && rece0+0x38 == opNo`; first hit sets
`a=rec28+0x1c, b=rece0+0, c=rece0+4, first=rec28+4, last=rec28+4, rece0+8, rece0+0x28`, later hits
set `last=rec28+4, rece0+0x10, rece0+0x28`; returns found).
* `0x14080e710` returns `last` (0 when not found), `0x14080e5e0` returns `first` (0 when not found).
* `0x14080e840` returns 0 when not found, 1 when `first == last`, else
  `(last < 1 ? -last : last) - (first < 1 ? -1 - first : first) + 1` (asymmetric as in the binary).

### 3.5 `0x1400572e0` `Model* loadDbStep(OPI*, int step, bool sync)` (named; DB packet)

`m = 0x14005f770(opi); if (m && step != 0) { 0x140c7b720(m); 0x140c7b870(m, &step, 1);
0x1400538f0(opi, sync, 0); } return m;`. `0x140c7b870`: needs `M+0xd8`; index =
`0x14080e560(reader, step)` (twin `1401ddf60`, not in ledger), `< 0` fails; then `0x140c7b900`.
`0x1400538f0` (GUI slots from the model: materials `0x1408959e0` = DEF_PRE `1401c5ea0`, objects,
names `+0x828`, `0x140358190`, `0x140064d40`) belongs to the DB-load packet.

### 3.6 Where the GUI mode global `0x143d9da20` is read on this path

Only through `0x140065530`, whose only callers are: `0x1405f6520` (three reads, §2.2),
`0x140055360` (one read, DB branch, SC keep/restore only when mode == 1), `0x1400547b0` (one read,
diagram update when mode != 0). `0x1400569d0` does not read it. `0x143d9da24` is read only by
`0x1405f7ad0` (in `0x1405f6520`).

---

## 4. Operation / project / model fields touched

| Object+off | Type | Meaning | R/W here |
|---|---|---|---|
| OPI+0x10 | Props* | properties object | R (vt+0x58) |
| OPI+0x18 / +0x1c | u8 / int | NSTART set / value | W (DB branch); R by `0x1400569d0` |
| OPI+0x38 | QPointer<Model> | engine model | R/W (created lazily) |
| OPI+0x100 | SfProject* | project | R |
| OPI+0x108 | int | operation id | R |
| OPI+0x10c | int | OprFlag (0 = never set up) | R/W |
| OPI+0x118 / +0x120 | QString | name / class name | R |
| OPI+0x130 | QString | operation folder (relative) | R |
| OPI+0x150 | u8 | restart mode | R/W |
| OPI+0x158 | QString | restart file name ("RestartStep.KEY") | R |
| OPI+0x160 | QPointer | object tree model (UI) | W (UI only) |
| OB+0x18 / +0x20 / +0x28 / +0x30 | | object tree (UI) / current OPI / licence feature (omitted) / class-info vbase | R/W |
| P+0x8 / +0x18 / +0x28 | | DB handle / .moproj path / project name | R |
| SP+0x10 / +0x18 / +0x48 | | parent project / type (2 DOE, 3 Opt, 4 own DB) / own DB handle | R/W |
| S+0x744 | int | DB step to read | W (DB branch) |
| SC+0x000 | char[0x51] | TITLE | W ([340] only) |
| SC+0x104 / +0x108 | int | TRANS 1 / 2 | W (prologue; HT-express reset) |
| SC+0x11c[7] / +0x138[10] | int | STNOUT / ELMNOD | W (postLoad, copied from prev op) |
| SC+0x160 | int | NSTART | R/W (DB branch) |
| SC+0x178 | double | TMAX | R/W (kept over the reset) |
| SC+0x648 / +0x650 | double / int | TNOW 2 / TNOW 3 | W |
| globals 0x143d77584 / 85 | u8 | keep choice / kept answer | R/W |

## 5. Engine calls (GUI -> DEF_PRE twin, port status)

| GUI addr | Role | DEF_PRE twin | Port status |
|---|---|---|---|
| `0x1408246a0` | DB_STEP_DATA clear | `1401cb400` | **no ledger row** (callees `1401cb440`, `1401c8c00` ported) |
| `0x140822210` | model dimension/unit init (createModel) | `1401c8f70` | **no ledger row** |
| `0x14080d870` | build DB step-header cache | `1401dd250` | verified (defdbio `db_header_cache`) |
| `0x14080d6f0` | header cache refresh | `1401dd0d0` | ported |
| `0x14080fc60` | find op's steps in header cache | `1401df660` | **no ledger row** |
| `0x14080e840` | number of steps of an op | `1401de240` | **no ledger row** |
| `0x14080e710` | last step of an op | `1401de110` | **no ledger row** |
| `0x14080e5e0` | first step of an op | `1401ddfe0` | **no ledger row** |
| `0x14080e560` | step number -> record index (DB load) | `1401ddf60` | **no ledger row** |
| `0x14080f690` | list of (op name, op no) in the DB | none | GUI code |
| `0x14080ca10` | open DB header | none | GUI code |
| `0x140814150` | cache-built flag `+0x1900` | not unique | trivial |
| `0x1408d3bc0` / `0x1408d3d70` | SC ctor / dtor (stack copy) | not unique (likely `1401e7b10` / `1401e7cb0`) | ported (`sim_ctrl.cpp`) if the match holds |
| `0x1408d3cd0` | SC set owner + init | `1401e7c10` | ported |
| `0x1408d3d10` | SC init all parts | `1401e7c50` | ported |
| `0x1408df7b0` | SC copy | `1401f3660` | ported |
| `0x1408d5100` | SC limits reset | `1401e8fb0` | ported |
| `0x1408e0000` / `0x1408dffe0` | SC +0x108 / +0x104 setters | not unique | trivial |
| `0x1408969a0` | KEY file loader (restart file) | `1401b5f00` | ported (defcmd `key_file`) |
| `0x1408959e0` | material KEY read (in `0x1400538f0`) | `1401c5ea0` | **no ledger row** |
| `0x140830180` | object count | not unique | trivial |
| `0x1408302a0` / `0x140830270` | object by position / by number | `1401d6ea0` / `1401d6ed0` (ambiguous) | ported |
| `0x140834fa0` / `0x1408350b0` | kind != 7 / kind == 5 | `140160f10` / `140161020` | ported / not-needed |
| `0x140834c80` | object has mesh | `140160bf0` | verified |
| `0x14086f6a0` | distinct element materials | `1401fb840` | ported |
| `0x140834d50` | object has geometry | `140160cc0` | verified |
| `0x14086c0e0` | object geometry | `140197f40/f80/fc0` (ambiguous) | check |
| `0x1408f1320` | face i | `140283f80` | ported |
| `0x1408ec3a0` / `0x140876860` | face material / object material | not unique | check |
| `0x1408bc4a0` | material count | `14029c410` | ported |
| `0x1400667b0` | first item has items | `140158ec0` | ported |
| `0x1409f0660` / `0x14089c490` | SC+0x440 table lookup / value (restart path) | `140375980` / `140130720` | ported |
| `0x140c7xxxx` (`c7b720`, `c7b2d0`, `c7acd0`, `c7a5b0`, `c7d020`, `c7b870`, `c7ff80`) | GUI model wrapper | none | GUI code |
| `0x1411f17f0`, `0x1411f1a10`, `0x1411ee800`, `0x140c82b70` | viewer | none | name only |

## 6. Open questions

1. **Q1** Which signal invokes `slotProjectChanged(SfProject*)` (slot 0x29) at File > Open? No
   "2...(SfProject*)" connect string exists in the exe; possibly a connect in a DLL or by index.
   The converter can call the activation directly after the walk.
2. **Q2** `loader-spec.md` §1 and the inventory say `0x140051340`; on open it is `0x140051c30`
   (§1.2). Difference that matters: `0x140051340` copies the project name into SC TITLE
   (`strcpy`, no bound), `0x140051c30` does not. Update both documents.
3. **Q3** The meaning of class-info flag bit `0x01000000` (vt+0x1f0): the per-class flags word is
   returned by the vbase at `OB+0x30` (vt+0x58); its source (ctor or factory) was not found.
   Needed for `0x14005f520` when the previous op's class has the bit set and has exactly one step.
4. **Q4** `0x140055360` returns without loading anything when the sub-project has no DB handle
   (`P+8` null for non-type-4 sub-projects). Check when `P+8` is created (project construction?)
   so the converter can assume it.
5. **Q5** `signalSetOprInteractiveMode(bool,int)`, `signalShowMessage`, MSG signals 1..4: assumed
   UI-only; their slots were not traced to the leaves.
6. **Q6** The restart branch deletes an existing `<op folder>/RestartStep.KEY` when restart mode
   is off, for restart-capable classes that are not first in the chain (Forming, HeatTransfer,
   Express, HeatTreat, Convert2Dto3D). A converter run must reproduce this file deletion (or
   document it as a deviation).
7. **Q7** `0x1400b8b80` indexes the DB op list with `k-1` without a bounds check when the chain has
   more matching operations than the DB; behaviour on overflow is undefined (release Qt).
8. **Q8** `0x1405f1c00` writes `o->kind = 5` without a null check; harmless only if
   `0x140057ba0` always creates object `i+1`.
9. **Q9** Dialog policy for the converter (no UI): Missing Restart File (OK only, restart turned
   off), Setup Type (default button Yes = Interactive/DB; "keep" globals are process-local). The
   24 MDT oracle cases have no DB beside the copies, so `0x14005f520` returns false there.

## 7. Unported DEF_PRE twins needed by this part

None left (updated by MDT DB-0, 2026-10-22). The list below was written before these were ported:
`1401de240`, `1401de110`, `1401ddfe0`, `1401df660`, `1401ddf60` (DB header cache queries,
`port/defdbio/src/db_query/queries.cpp`, status `mdt-dbquery.tsv`), `1401cb400` (DB_STEP_DATA
clear) and `1401c8f70` (model init in createModel), `1401c5ea0` (material KEY writer; all three
status `mdt-model.tsv`). The step read `1401cc420` and its lead helpers `1401cc2a0`, `1401cd020`,
`1401d5f50` (`port/defdbio/src/dbread/lead.cpp`) have status rows in `mdt-db.tsv`. The section 5
table above keeps its original "no ledger row" notes.
