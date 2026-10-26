# L0 part A: KEY loader 0x1400569d0 (all but the object slot loop), equipment 0x1403568a0, DB-step load 0x1400572e0

DEF_GUI_MO_64.exe v11.0.2. Static analysis only (headless Ghidra copy `gproj_mdtA`). Decompiles:
`gui_mo/s1/mdt/L0/A/{a0a,b0a,c0a,d0a,e0a,f0a}.c`, disassembly `L0/A/569d0.asm`. Every address is a
GUI VA unless marked "PRE" (DEF_PRE_64.exe). Engine object offsets are DEF_PRE's (`docs/re/model-layout.md`).

Notation: `OPI` = operation item (the `param_1` of the loader). `W` = GUI model wrapper (0x278 bytes,
ctor 0x140c7a5b0). `E` = engine `DB_STEP_DATA` held by `W` (0x3408 bytes, PRE ctor 1401c8950).
`S` = `E+0x750` = sim control (`Cls_1401e7b10`). `QS` = QString. Decompiler argument drops were
checked against the assembly; where they differed the assembly wins and is noted.

## 0. Fields used

### OPI (operation item)
| off | type | meaning (accessor) |
|---|---|---|
| +0x18 | u8 | start step set (0x14005a2a0) |
| +0x1c | i32 | start step number (0x14005a2e0) |
| +0x20 | ptr | point-tracking list, local (0x1401cc700) |
| +0x28 | ptr | point-tracking "shared" flags list (0x140970130) |
| +0x38 | holder | W (lazy, 0x14005f820) |
| +0x40 | QList<int> | per-slot source code (0x14005fa80 size, 0x14005fb30 get; -99 if index out of range) |
| +0x50 | QList<SlotRec*> | per-slot records (0x140061250 grows it lazily with 0x140061020) |
| +0x100 | ptr | project (0x140055030) |
| +0x108 | i32 | operation id (0x140050910) |
| +0x10c | i32 | tested by 0x1400509d0 (`== 0`); loader-spec reads `0` as "not the first operation" |
| +0x120 | QS | operation name (0x140050d00) |
| +0x130 | QS | operation sub-folder (0x140051110) |
| +0x158 | QS | operation KEY file name (0x140064a50) |

Slot source codes (OPI+0x40): `-1` empty slot, `-2` "Read From DB" (0x1400606c0), `-99` invalid,
otherwise an object-manager id (0x140060690, not decompiled here, belongs to part B).

### SlotRec (0x1c8 bytes, ctor 0x14004d160, number set by 0x14004d520)
+0x0 i32 slot number (= index+1), +0x10 QS object name (0x14004d700), +0x20 QList<int> material ids
(0x14004db50 size, 0x14004db80 get or -1, 0x14004db20 clear, 0x14004dc20 add-unique), +0x28 i32
equipment id (0x14004de90 get, 0x14004deb0 set), +0x30 sub-record (0x14004df00; +0xf0 of it =
point-tracking count, set by 0x140065540), +0x1bf u8 (0x14004e0e0), +0x1c0 u8 (0x14004e140).

### Project chain
* `prj = OPI+0x100`; `prjData = prj+8` (0x1400a5890).
* managers in prjData (lazy, created on first use): +0x19c0 document object (0x140044a90, 0x1500 bytes,
  0x140089880), +0x19c8 `SfMoOprManager` (0x140044c80 / 0x1400a58b0), +0x19d0 `SfMoObjManager`
  (0x140044d20 / 0x1400a58f0), +0x19d8 `SfMoMatManager` (0x140044dc0 / 0x1400a5930), +0x19e0
  `SfMoEqpManager` (0x140044e60 / 0x1400a5970). Manager ctors: 0x140045fe0, 0x140068ba0, 0x1400425a0,
  0x1400beca0, base 0x14006c120 (QStandardItemModel, +0x20 = document object).
* `subprj = 0x140055050(OPI)` = `0x1400a7390(prj)` = `prj->list(+0x58)[prj+0x50]` (current sub-project).
* `db = 0x1400b9cd0(subprj)`: sub-project database object (`subprj+0x48`, created lazily as a 0x19c0
  object `0x140cda460` when `subprj+0x18 == 4`). `db+0x19a8` = point-tracking global list (0x14005a400).
* Project folder `0x1400a4220(prj)` = `cleanPath(QFileInfo(prj+0x18).absolutePath())`.
* Operation folder `0x140050df0(OPI)` = `convertSeparators(cleanPath(prjFolder + sep + OPI+0x130))`
  (null QString if no project).

### Manager base slots (shared by `SfMoMatManager`, `SfMoEqpManager`, `SfMoObjManager`)
| vt | function | role |
|---|---|---|
| +0x1b8 | 0x14006cdd0 | count = rowCount(QModelIndex()) (vt+0x68) |
| +0x1c0 | 0x14006ce30 | item(row) = dynamic_cast<SfMoItem*>(QStandardItemModel::item(row,0)) |
| +0x1c8 | 0x14006cea0 | item by id: linear search rows, `item+0x18 == id` |
| +0x208 | 0x14006d380 | file path of item id: `cleanPath(item+0x28)`, null QS if no item |
| +0x1b0 | 0x14006cd50 | re-path item id: `item(id)->path(+0x28) = vt178(id)` |
| +0x180 | 0x14006c520 | new id: `id = vt168(); vt188(id); return id` |
| +0x188 | 0x14006c570 | append item: `it = vt170(id); it->path = vt178(id); root->appendRow(it)` |
| +0x178 | Mat 0x140042850, Eqp 0x1400bef30 | default file of id (below) |
| +0x220 | Mat 0x14006d700 | emit dataChanged over the whole table |

Default file of an id (vt+0x178): `prjFolder + "/Materials" (resp. "/Equipments") + sep +
QString("%1%2.KEY").arg("Material" (resp. "Equipment"), 0, ' ').arg(id, 5, 10, '0')`, i.e.
`<prj>/Materials/Material00007.KEY`, `<prj>/Equipments/Equipment00003.KEY` (the folder is joined
with QDir::separator() then cleanPath; strings at 0x14210e600/0x14210e610/0x142140a38,
0x142153188/0x142153198/0x142140a48).

`SfMoItem`: +0x18 id (0x14006b8a0 get, vt+0x50 = 0x14006b950 set), +0x28 QS file (0x14006ba30 set,
0x14006ba60 get with cleanPath). `SfMoMatItem` (dynamic_cast 0x140042a90 from `SfMoItem`, type
descriptors 0x14356ebb0 -> 0x14356eb88): +0x40 material kind (0 single, 1 mixture with id list at
+0x48, 2 other; setters 0x140041fa0 / 0x140042010 / 0x140041ff0; tests 0x140041e30 `==0`,
0x140041e70 `==1`).

### W (GUI model wrapper, ctor 0x140c7a5b0(mem, parent, dim))
+0x10 ptr (0), +0x18 viewer object list, +0x30/+0x38 (cleared by 0x140c8ba80), +0x59/+0x5a u8,
+0xd8 DB file object (0x140c7aa80 sets it; 0x140c7ac50 returns its step table `0x140cdab70`),
+0xe0 shared holder of E (0x140c7acd0 returns E, created in 0x140c7a870 as `new DB_STEP_DATA(dim)`
0x140821bf0 = PRE 1401c8950), +0xf8 int block (0x140c7d3a0), +0x100 i32 state (0x140c833c0),
+0x230 owned object (0x140c902c0), +0x25c i32. `0x140c7b2d0(W) = E ? E+0x750 : 0` (S).
The full wrapper is L0 (other part); this part uses only the methods listed in section 7.

## 1. FUN_1400569d0 — KEY (batch) load of the active operation

`DB_STEP_DATA-wrapper* LoadFromKey(OPI* op, bool withObjects)`; called with `withObjects = 1` from
OB vt+0x1b8 (0x1405f6520, mode 2) and with the caller's flag from 0x140055360. Returns W or 0.

```cpp
W* LoadFromKey(OPI* op, bool withObjects) {
  QString matFile;                                   // auStack_160, lives for the whole call
  int i = 0;
  W* w = op_model(op);                               // 0x14005f770 -> 0x14005f820
  if (!w) return 0;
  SubPrj* sp = op_subproject(op);                    // 0x140055050
  if (!sp) return 0;
  if (!subprj_db(sp)) return 0;                      // 0x1400b9cd0 (creates the DB object lazily)
  (void)op_step_number(op);                          // 0x140050b20: result unused; builds the
                                                     //   operation-id -> step map (0x140085800)
  SimCtrl* S = w_simctrl(w);                         // 0x140c7b2d0
  if (op->startStepSet /*+0x18*/ && S) S->nstart /*+0x160*/ = op->startStep /*+0x1c*/;
  LoadSimCtrlKey(op);                                // 0x1400594d0 (section 2), result unused
  int savedPdie = w_get_pdie(w);                     // 0x140c7b5a0: S ? S->pdie(+0x168) : 0
  LoadInterObjectKey(op);                            // 0x14005a7b0 (section 3), result unused

  // ---- materials -------------------------------------------------------------------------
  int matId = -1;                                    // aiStack_150[0]
  SfMoMatManager* mm = op_matmgr(op);                // 0x140055110 (no null check in the original)
  int n = mm->count();                               // vt+0x1b8
  for (i = 0; i < n; ++i) {
    SfMoItem* it = mm->itemAt(&i);                   // vt+0x1c0
    if (!it) continue;
    SfMoMatItem* mi = dynamic_cast<SfMoMatItem*>(it);// 0x140042a90; NOT null-checked below
    if (!(mi->kind == 0 || mi->kind == 1)) continue; // 0x140041e30 || 0x140041e70 (+0x40)
    matId = it->id;                                  // 0x14006b8a0 (+0x18)
    matFile = mm->filePath(&matId);                  // vt+0x208 into a temp, assigned, temp freed
    int reqId = matId;                               // iStack_118
    if (w_material(w, matId)) reqId = -1;            // 0x140c7cb20 -> E material list lookup
    int* mat = w_read_material(w, matFile, &reqId);  // 0x140c7cf80 (asm: r8 = &reqId)
    if (mat && *mat /*material number*/ != matId) {
      int newId = *mat;                              // iStack_f8
      it->setId(newId);                              // item vt+0x50 (0x14006b950)
      mm->repath(newId);                             // vt+0x1b0 (0x14006cd50)
      QString f = mm->defaultFile(newId);            // vt+0x178 (0x140042850)
      QByteArray b = f.toLocal8Bit();
      WriteMaterialKey(op_model(op)->E() /*0x14005f770, 0x140c7acd0*/, b.data(), newId,
                       0, 0, 0);                     // 0x1408959e0 = PRE 1401c5ea0, rc ignored
    }
  }

  // ---- objects (withObjects) -------------------------------------------------------------
  if (withObjects) {
    TouchSubprojectOperations(op->project);          // 0x1400a3540 (section 4)
    SlotRec* rec = 0;                                // [rsp+0xb8]; two more locals zeroed (unused)
    (void)prj_objmgr(op->project);                   // 0x1400a58f0, result stored, unused
    int ns = op_slot_count(op);                      // 0x14005fa80
    for (i = 0; i < ns; ++i) {                       // ---- part B owns the body; skeleton:
      int code = op_slot_code(op, i);                // 0x14005fb30
      int no = i + 1;
      if (code == -1) { w_object(w, no, 0, 1)->type /*+0xc*/ = 7; continue; }   // 0x140c7c950
      if (code == -2)  w_object(w, no, 0, 1)->type = 5;
      else if (code == -99) continue;
      if (!LoadSlotObject(op, w, i, 0)) continue;    // 0x14005aec0 (part B)
      rec = op_slot(op, i);                          // 0x140061250
      if (rec && slot_mat_count(rec) == 0) {         // 0x14004db50 (QList size) == 0  (sic)
        int k = 0; matId = slot_mat(rec, &k);        // 0x14004db80 -> -1 for an empty list
        SetObjectMaterial(op, i, matId);             // 0x140060be0 (asm: 3 args, r8 = matId)
      }
      w_object(w, no, 0, 1)->u2bd8 = rec->u1c0;      // 0x140c7c950, 0x14004e140 (rec not null-checked)
    }
  }
  w_set_pdie(w, savedPdie);                          // 0x140c7b5e0

  // ---- equipment ---------------------------------------------------------------------------
  int ns2 = op_slot_count(op);
  for (int j = 0; j < ns2; ++j) {                    // iStack_158
    int no = j + 1;
    Obj* obj = op_engine_object(op, no);             // 0x140057710 -> 0x140c7ae80 (E object list)
    SlotRec* r = op_slot(op, j);                     // 0x140061250
    int eqId;
    if (r && obj && obj->number /*+8*/ == no && (eqId = r->equipmentId /*+0x28*/) > 0) {
      SfMoEqpManager* em = prj_eqpmgr(op->project);  // 0x1400a5970 (lazy)
      QString f = em->filePath(&eqId);               // vt+0x208
      bool noDialog = 1; int objNo = obj->number;
      LoadEquipment(w, f, obj, /*parent*/0, &noDialog, &objNo);   // 0x1403568a0, rc ignored
    }
  }
  return w;
}
```

Notes
* The material KEY read uses the file stored in the material item; when the engine assigns a new
  number (because `matId` was already present in E, or the file asks for another number), the item
  is renumbered and the material is written back to `Material%05d.KEY` of the new number (with the
  default header: flag 0, no comment, `param_6 = 0`).
* Return value of every helper above except W is ignored.

## 2. FUN_1400594d0 — SimCtrl.KEY and SimCtrl.PPV of the operation

`bool LoadSimCtrlKey(OPI* op)`.

```cpp
bool LoadSimCtrlKey(OPI* op) {
  DB_STEP_DATA* E = op_engine(op);                   // 0x14005f7a0 = W ? W->E : 0
  if (!E) return false;
  QString key = SimCtrlKeyPath(op);                  // 0x140059290: cleanPath(opFolder + sep + "SimCtrl.KEY")
  if (!QFile::exists(key)) return InheritSimCtrl(op, false);   // 0x1400599c0
  int rc = 0;
  if (!op_test_10c(op) /*0x1400509d0: OPI+0x10c == 0*/) {
    QByteArray b = key.toLocal8Bit();
    rc = ReadSimCtrlKey(E, b.constData(), 1);        // 0x140897720 = PRE 1401b6c80
  }
  QString ppv = SimCtrlPpvPath(op);                  // 0x1400593b0: ... "SimCtrl.PPV"
  if (QFile::exists(ppv)) {
    PtList* glob  = db_point_list(op);               // 0x14005a400: db ? *(db+0x19a8) : 0
    FlagList* flg = op->ppvFlags;                    // OPI+0x28
    PtList* loc   = op->ppvLocal;                    // OPI+0x20
    int cnt = 0;
    if (!op_has_no_predecessor(op)) {                // 0x140050bc0: true when the diagram has no
                                                     //   previous operation (0x140086720 == -1)
      ReadPpv(loc, ppv, glob, flg);                  // 0x140d4b400
      cnt = list_size(loc) + list_size(glob);        // 0x140004ae0 (QListData::size)
    } else {
      ReadPpv(loc, ppv, 0, 0);
      cnt = list_size(loc);
    }
    int ns = op_slot_count(op);
    for (int i = 0; i < ns; ++i) {
      SlotRec* r = op_slot(op, i);
      if (r && slot_sub(r) /*r+0x30, never 0*/) slot_sub(r)->ppvCount /*+0xf0*/ = cnt;
    }
  }
  return rc == 0;
}
```

Note: the `if` reads "has no predecessor -> local only". The decompile shows exactly this
(`cVar1 == 0` -> global branch). Kept as is.

### FUN_140d4b400 — ReadPpv(list, path, glob, flags) -> bool
```cpp
QFile f(path);                       // path QString passed by value (destroyed at the end)
bool ok = false;
if (f.open(QIODevice::ReadOnly /*1*/)) {
  list_clear(list);                  // 0x1400670b0 (swap with an empty list)
  QTextStream ts(&f);
  QString line = ts.readLine(0);
  (void)line.trimmed();              // result discarded (sic)
  int n = line.toInt(0, 10);
  if (n > 0) {
    ok = true;
    for (int k = 0; k < n; ++k) {
      line = ts.readLine(0);
      QStringList t = line.split(*(QRegExp*)0x1446b4b20, QString::SkipEmptyParts /*1*/);
      if (t.size() < 6) { ok = false; break; }
      int    a0 = t[0].toInt(0,10);           // stored, unused
      QString s1 = t[1];
      int    a2 = t[2].toInt(0,10), a3 = t[3].toInt(0,10);
      double d4 = t[4].toDouble(0), d5 = t[5].toDouble(0);
      bool   fl = false;
      if (t.size() == 7) fl = t[5].toInt(0,10) != 0;   // index 5, not 6 (sic)
      bool shared = (s1.compare("true", Qt::CaseInsensitive /*0*/) == 0) && glob;
      if (!shared) a0 = FindOrAddPoint(list, a2, a3, d4, d5);          // 0x140d4af00
      else { a0 = FindOrAddPoint(glob, a2, a3, d4, d5);
             if (flags) list_insert(flags, a0, fl); }                   // 0x140d4bb70
    }
  }
  f.close();
}
return ok;
```
`FindOrAddPoint` (0x140d4af00): build a point {a2, a3, d4, d5} (0x140d49e30), look it up
(0x140d4bab0); if absent append (0x140067140) and return size-1, else return its index (0x140d4b8c0).
The regexp object at 0x1446b4b20 is a static initialised elsewhere (open question 4).

### FUN_1400599c0 — InheritSimCtrl(op, bool direct)
Large stack frame (0x35xx bytes, `__chkstk`): holds a temporary DB_STEP_DATA.
```cpp
DB_STEP_DATA* E = op_engine(op); if (!E) return false;
QString path; OPI* p = op_previous(op);              // 0x140062960 (diagram predecessor via 0x140062df0
                                                     //   and SfMoOprManager item id -> OPI 0x140047dc0)
QString pname = p ? p->name /*0x140050d00*/ : QString("");   // pname kept, unused
while (p) {
  path = "";  path = SimCtrlKeyPath(p);              // 0x140059290 on the predecessor
  if (QFile::exists(path)) {
    int rc;
    if (!direct) {
      DB_STEP_DATA tmp(3);                           // 0x140821bf0 = PRE 1401c8950
      bool one = 1; int zero = 0;
      rc = ReadKeyFile(&tmp, path.toLocal8Bit().constData(), &one, &zero);  // 0x1408969a0 = PRE 1401b5f00
      if (rc != 0) { tmp.~DB_STEP_DATA(); return rc != 0 /*see Q5*/; }      // 0x140824560 = PRE 1401cb2c0
      if (E->dim /*+0x1c*/ == 2) E->sim.geotyp /*E+0x850 = S+0x100*/ = tmp.sim.geotyp;
      E->sim.pdie /*E+0x8b8 = S+0x168*/ = tmp.sim.pdie;
      tmp.~DB_STEP_DATA();
    } else {
      rc = ReadSimCtrlKey(E, path.toLocal8Bit().constData(), 1);           // 0x140897720
      QString ppv = SimCtrlPpvPath(p);
      if (QFile::exists(ppv))                        // ppv = SimCtrlPpvPath(p) of the predecessor
        ReadPpv(op->ppvLocal /*OPI+0x20*/, ppv, db_point_list(op), op->ppvFlags /*OPI+0x28*/);
    }
    return rc == 0;
  }
  p = op_previous(p);
  pname = p ? p->name : QString("");
}
return false;
```
Only `direct == false` is reached on this path (called as `0x1400599c0(op, 0)`): the operation
inherits the primary die (PDIE) and, in 2D, GEOTYP from the nearest predecessor that has a
SimCtrl.KEY; nothing else of that file is used.

## 3. FUN_14005a7b0 — InterObject.KEY
```cpp
bool LoadInterObjectKey(OPI* op) {
  DB_STEP_DATA* E = op_engine(op); if (!E) return false;
  QString f = cleanPath(opFolder(op) + sep + "InterObject.KEY");   // 0x14005a690
  if (!QFile::exists(f)) return false;
  bool r = true;
  if (!op_test_10c(op)) r = ReadInterObjectKey(E, f.toLocal8Bit().constData()); // 0x140897fd0 = PRE 1401b7530
  return r;
}
```

## 4. FUN_1400a3540 — TouchSubprojectOperations(prj)
```cpp
SfMoOprManager* om = prj_oprmgr(prj);                // 0x1400a58b0
SubPrj* sp = prj_current_subproject(prj);            // 0x1400a7390
if (om && sp) {
  QList<int> ids = subprj_operation_ids(sp);         // 0x1400b6350 (below)
  int n = ids.size(); OPI* last = 0;
  for (int k = 0; k < n; ++k) { int id = ids[k];     // 0x1400250c0
    OPI* o = om->operation(&id);                     // 0x140047dc0: item(vt+0x1c8) -> SfMoItem cast
    if (o) last = o; }                               //   (0x140047910) -> item+0x48
}
```
No state change of its own: it only makes sure every operation item of the current sub-project
exists (the lookups create nothing; `last` is unused). `0x1400b6350`: if `sp+8 == 0` and
`sp+0x10 != 0`, the ids are the `Operation_Id` attributes of the `DiagramItem` children of the
sub-project XML element (0x1400a8650 over 0x1400a78e0); otherwise the ids of the diagram
items in the sub-project's diagram (0x140084ad0 order: items without flag 0x1000, sorted by
0x140084a50, then the chain from the first through slots +0x138 / +0x130).

## 5. FUN_1403568a0 — LoadEquipment(W* target, QString file, Obj* tgtObj, QWidget* parent, bool* noDialog, int* objNo)

Callers: 0x1400569d0 (`noDialog = 1`, `objNo = object number`, parent 0), 0x140355ba0,
0x14141ca90, 0x1414364f0 (interactive; not in this part). Returns 1 on success, 0 on failure; the
dialog-rejected path also returns 1 (initial value).

```cpp
int LoadEquipment(W* target, QString file, Obj* tgt, QWidget* parent, bool* noDialog, int* objNo) {
  g_cbPartnerMove /*0x143d93120, u64*/ = 0;
  SelDialog* dlg = 0;                                // plStack_198
  int ret = 1;
  W* scr = new (0x278) W(/*parent*/0, /*dim*/3);     // 0x140c7a5b0 (W+0xd8 = 0)
  W* keep = scr;                                     // plStack_178
  if (!scr) return 0;
  wait_cursor(mainWnd /*0x143d766b8*/, 1);           // 0x1402ca7a0: setOverrideCursor(Qt::WaitCursor 3) + processEvents
  bool ok = w_read_key(scr, file);                   // 0x140c7d020 (section 7)
  wait_cursor(mainWnd, 0);                           //   restore all override cursors + processEvents
  if (!ok) { scr->deleteThis(1) /*vt+0x18*/; return 0; }
  int n = w_object_count(scr);                       // 0x140c7c1a0 = E+0x1378
  for (int k = 0; k < n; ++k) {
    Obj* o = w_object_at(scr, k + 1);                // 0x140c7aed0 (0x1408302a0 = PRE 14017aa80)
    if (o && o->type /*+0xc*/ == 7 && o->move.cbPartner /*+0x2360*/ > 0) o->type = 0;
  }
  QWidget* par = parent ? parent : *(QWidget**)mainWnd;   // 0x1402c8090
  int res;
  if (!*noDialog) {
    dlg = new (0x38) SelDialog(par, Qt::WindowFlags(*(int*)0x143d766c8));   // 0x140452d00
    dlg->setModel(scr);                              // 0x140452ec0
    res = dlg->exec();
  } else res = 1;
  if (res == 1) {
    Obj* src;                                        // lStack_170
    if (!*noDialog)           src = dlg->selected(); // 0x140452fb0 -> 0x140d652b0
    else if (*objNo == 0)     src = w_first_defined_object(scr);   // 0x140c7c3b0 -> 0x1408302d0
    else                      src = w_object(scr, *objNo);         // 0x140c7ae80 (0x140830270 = PRE 14017aaf0)
    if (!src) {
      scr->deleteThis(1); keep = 0;
      if (!*noDialog && dlg) dlg->deleteThis(1);     // and dlg is deleted again below (sic, Q3)
      ret = 0;
    } else {
      if (src->move.cbPartner > 0 && !*noDialog) CounterBlowDialog(...);   // 5.1, never in batch
      MovementCopy(&tgt->move /*+0x2160*/, &src->move);                    // 0x14085fed0 = PRE 14018bd30
      int mt = tgt->move.type /*+0x21c8*/;
      if (mt == 3 || mt == 4)
        memcpy(&simctrl(target)->lmax /*+0x1b0*/, &simctrl(scr)->lmax, 0x18);   // 0x140c7b2d0 both
      if (mt == 7 || mt == 8 || mt == 9 || mt == 10) {
        memcpy(&simctrl(target)->vmin /*+0x198*/, &simctrl(scr)->vmin, 0x18);
        memcpy(&simctrl(target)->lmax /*+0x1b0*/, &simctrl(scr)->lmax, 0x18);
      }
    }
  }
  if (keep) keep->deleteThis(1);
  if (dlg)  dlg->deleteThis(1);
  return ret;
}
```
(`memcpy` = 0x14146f100.) `w_first_defined_object` returns the first object of E's list for
which PRE 140160f10 holds and PRE 140160cc0 or 140160bf0 holds (the decompile also repeats the
140160f10 test).

### 5.1 Counter-blow dialog (only when `*noDialog == 0`)
```cpp
QStringList items;                                   // 0x1400653e0
QString s; s.sprintf("%-3d - ", 0); s += "None"; items << s;      // 0x14231cb98 / 0x14231cba0
for (int k = 0; k < w_object_count(scr); ++k) {     // count re-read every pass
  Obj* o = w_object_at(scr, k + 1);
  if (o && obj_defined(o) /*0x140834fa0 = PRE 140160f10*/ && o->number != tgt->number) {
    s.sprintf("%-3d - ", o->number);                 // 0x14231cba8
    s += o->name /*+0x828, char*/;
    if (QString(o->name).isEmpty()) s += "Object";   // 0x14231cbb0
    items << s;
  }
}
bool ok;
QString pick = QInputDialog::getItem(par, "Counter-blow definition is detected",   // title 0x14231cbe8
                                     "Select object as the counter blow hammer:",   // label 0x14231cbb8
                                     items, 0, false, &ok, Qt::WindowFlags(0));
if (!ok) tgt->move.cbPartner = 0;
else {
  tgt->move.cbPartner = pick.isEmpty() ? 0 : pick.toAscii()[0] - '0';   // first character only (sic)
  Obj* a = w_object(scr, src->move.cbPartner);       // a may be 0: dereferenced below (sic)
  src->move.cbPartner = tgt->move.cbPartner;
  Obj* b = w_object(scr, tgt->move.cbPartner);
  if (b && obj_defined(b)) {
    g_cbPartnerMove = a->move.u1e8 /*+0x2348*/;
    MovementInit(&b->move);                          // 0x14085f6d0 (no PRE twin; ported in moproj2mst s1-04)
    b->move.cbPartner = tgt->number;
    b->move.u1e8 = g_cbPartnerMove;
  }
}
```
Buttons: QInputDialog standard OK / Cancel; QMessageBox none. The selection dialog class at
0x140452d00 (vtables 0x142387fa8 / 0x142387f68, UI built by 0x1403a2530, accepted()/rejected()
wired to accept()/reject(); OK disabled when the scratch model has no object) lists the scratch
model objects (0x140d64e70, initial row from PRE 1401caa00 via 0x140c7b1a0).

## 6. DB-step load (mode 1)

### FUN_1400572e0 — LoadFromDbStep(OPI* op, int step, bool p3) -> W*
```cpp
W* w = op_model(op); if (!w) return 0;
if (step != 0) {
  w_reset(w);                                        // 0x140c7b720
  w_load_db_step(w, &step, 1);                       // 0x140c7b870
  SyncFromModel(op, p3, 0);                          // 0x1400538f0
}
return w;
```
`0x140c7b720` (reset): if E: clear viewer list (0x140c82c70), drop +0x30 (0x140c8ba80), reset E
(0x1408246a0 = PRE 1401cb400), drop +0x230 (0x140c902c0), `W+0x100 = 0`.

### FUN_140c7b870 — w_load_db_step(W* w, int* step, bool p3) -> bool
```cpp
if (!w->db /*+0xd8*/) return false;
StepTable* t = w_step_table(w);                      // 0x140c7ac50
if (!t) return false;
int idx = step_index(t, *step);                      // 0x14080e560 = PRE 1401ddf60
if (idx < 0) return false;
if (!w_engine(w)) return false;
return w_read_db_step(w, &idx, p3);                  // 0x140c7b900
```
`0x140c7b900`: if p3 clear viewer list; drop +0x30; set an E flag through 0x140896940 to 0 when
the app mode `*(mainWnd)+0xc` (0x1402cb7a0) is 3, else 1; `rc = 0x1408256c0(E, stepTable, idx, 0)`
(DB step read, no PRE twin); rc != 0 -> false. App mode 1 and `S->nstart >= 0` ->
`S->nstart = -(S->nstart + 1)`. Then 0x140cdbcd0(db, &false), 0x140c7d3a0 (copies 26 values of the
S+0x440 entry 0xe through 0x14089c490 into the W+0xf8 block), 0x140c7ebe0 (primitive table),
`stepNo = 0x14080e500(table, idx)`, `0x140c7ff80(w, p3, 0)`, viewer calls 0x140d0cc00 / 0x140d15b40 /
0x1402ca890, 0x140c90150, `W+0x100 = 1`, 0x140d77610, signals `signalActiveStepNoChanged()` (int
stepNo) and `signalAssignFlownetData()` on 0x143d76700 (0x1402cf3c0), then 0x140c8c930 / 0x140cf97f0 /
0x140cff370 / 0x140c80a20 / 0x140c80b00 (viewer). Returns 1.

### FUN_1400538f0 — SyncFromModel(OPI* op, bool updateDiagram, bool writeEqp)
Brings the operation items in line with what the engine model now holds (after a DB step or a
restart load). Call level:
```cpp
W* w = op_model(op); if (!w) return;
SfMoMatManager* mm = op_matmgr(op);
DB_STEP_DATA* E = op_engine(op);
for (int k = 0; k < E->n_materials /*+0x1320*/; ++k) {
  Material* m = material_at(op_engine(op), k + 1);   // 0x140830460 -> PRE 14029c260 (material by index)
  if (!m) continue;
  int id = m->number /*+0*/;
  if (!mm->item(&id) /*vt+0x1c8*/) mm->append(id);   // vt+0x188
  SyncMatItem(op, id);                               // 0x140060c40: item name = material +0xc,
                                                     //   kind from +0x1cc8 (0/1 with +0x1ccc/+0x1cd0 / 2)
  QString f = mm->filePath(&id);                     // vt+0x208
  if (m->kind /*+0x1cc8*/ != 2)
    WriteMaterialKey(op_engine(op), f.toLocal8Bit().data(), id, 0, 0, 0);   // 0x1408959e0
}
MatTableChanged(op);                                 // 0x140055280 -> mm vt+0x220
int cap = material_count(E+0x1328) /*0x1408bc4a0 = PRE 14029c410*/; if (cap < 1) cap = 100;
void* buf = operator new[](cap * 4 /*overflow -> SIZE_MAX*/);   // 0x14146c25c; freed at the end (0x1414dfc68)
int no = w_object_count(w);
op_resize_slots(op, no);                             // 0x140060ee0: OPI+0x40 to `no` entries, new = -1
for (int k = 0; k < no; ++k) {
  Obj* o = w_object_at(w, k + 1); if (!o) continue;
  int code = op_slot_code(op, k);
  if (code == -1 && o->type != 7) {
    if (o->type == 5) code = -2;
    else { code = objmgr->newId();                   // 0x1400550d0 vt+0x180
           if (auto* it = objmgr->item(&code)) it->setName(QString(o->name)); }   // item vt+0x40
    AddSlotObject(op, k, code, 0, 0, updateDiagram); // 0x140057c60 (part B / later)
  }
  SlotRec* r = op_slot_by_number(op, o->number);     // 0x1400612d0
  r->name = QString(o->name);                        // 0x14004d700
  SyncSlotMaterials(op, o->number);                  // 0x140064d40 (0x140064e60 / 0x140065020)
  if (!obj_has_equipment(o) /*0x14084d4a0 = PRE 140179280*/) { r->u1bf = 0; r->equipmentId = 0; }
  else {
    r->u1bf = 1;
    int eq = r->equipmentId;
    if (eq < 1) { eq = eqpmgr->newId(); r->equipmentId = eq; }        // 0x1400a5970 vt+0x180
    if (writeEqp) { QString f = eqpmgr->filePath(&eq);               // vt+0x208
                    WriteEquipmentKey(w->E, o, f, 1); }               // 0x140358190
  }
}
objmgr_refresh(objmgr);                              // 0x140069620 (0x1400a5c10 + 0x14006d490)
if (auto* cls = op_class(op) /*0x140054e90*/) cls->vt_c8();
if (updateDiagram) {
  UpdateDiagramItem(op_diagram(op) /*0x140055150*/, op->id, w);       // 0x140087270
  emit signalUpdateUIInWidgetArea();                 // 0x1402cf3c0(0x143d76700, 0x142122ea0)
}
```
`0x140358190` (WriteEquipmentKey): appends ".KEY" when the upper-cased name does not end with it,
rebuilds `absolutePath + sep + baseName + "." + suffix`; if the file exists and `force == 0` asks
QMessageBox::question("Overwrite File", "The file %1 already exists\n. Do you want to overwrite
it?", Yes 0x4000 | No 0x10000) and returns 0 on No; then `0x140896840(E, path, o->number)` (engine
equipment KEY writer). With `force = 1` from 0x1400538f0 no dialog.

## 7. W methods used by this part

| GUI | role | engine call |
|---|---|---|
| 0x14005f770 / 0x14005f820 | OPI -> W (class vt+0xe0, else OPI+0x38, created by 0x1405ef280 + class vt+0x1b0) | - |
| 0x140c7acd0 | W -> E | - |
| 0x140c7b2d0 | W -> S (E+0x750) | - |
| 0x140c7b5a0 / 0x140c7b5e0 | get / set S+0x168 (PDIE) | - |
| 0x140c7cb20 | material by number | 0x140830430 -> 0x1408bc350 (PRE 14029c2c0) |
| 0x140c7cf80 | read material KEY: `m = 0x140897940(E, f.toLocal8Bit().constData(), *reqId, 0)`; then `0x140c7ff80(w,1,1)`; return m | PRE 1401b6ea0 |
| 0x140c7d020 | read a KEY file: `rc = 0x1408969a0(E, f, &true, &0)`; rc == 0 -> 0x140c7d3a0, 0x140c7ebe0; return rc == 0 | PRE 1401b5f00 |
| 0x140c7c950 | object by number, create if missing (part B) | 0x140830270, 0x1408bde10 |
| 0x140c7ae80 / 0x140c7aed0 | object by number / by index | 0x140830270 (PRE 14017aaf0) / 0x1408302a0 (PRE 14017aa80) |
| 0x140c7c1a0 | object count E+0x1378 | - |
| 0x140c7c3b0 | first defined object | 0x1408302d0 |
| 0x140c7ff80 | rebuild the viewer objects (0x140c82c70, 0x140c82930, 0x1411f17f0, 0x1411f1a10, 0x1411ee800, 0x140c8e640, 0x140c8c930) | viewer only |
| 0x140c7a5b0 | ctor (0x140c7a870 dim, 0x140c7aa80 db) | 0x140821bf0 (PRE 1401c8950) |

## 8. Engine calls

| GUI | DEF_PRE twin | ledger status | used for |
|---|---|---|---|
| 0x140897720 | 1401b6c80 | not in ledger (mentioned in `port/defcmd/src/p8_08/model_copy.cpp` comment only) | read SimCtrl.KEY |
| 0x140897fd0 | 1401b7530 | not in ledger | read InterObject.KEY |
| 0x140897940 | 1401b6ea0 | not in ledger | read material KEY with requested number |
| 0x1408959e0 | 1401c5ea0 | not in ledger | write material KEY ("DEFORM MATERIAL KEYWORD FILE") |
| 0x1408969a0 | 1401b5f00 | ported | read KEY file into a model (equipment, inherit) |
| 0x140821bf0 | 1401c8950 | ported | DB_STEP_DATA ctor |
| 0x140824560 | 1401cb2c0 | ported | DB_STEP_DATA dtor |
| 0x1408246a0 | 1401cb400 | not in ledger | reset model |
| 0x14085fed0 | 14018bd30 | ported | movement copy |
| 0x14085f6d0 | none | ported in moproj2mst (s1-04 ledger, `apps/moproj2mst/src/ops/express.hpp`) | movement init (counter-blow only) |
| 0x140834fa0 | 140160f10 | ported | object defined |
| 0x140834d50 / 0x140834c80 | 140160cc0 / 140160bf0 | verified / verified | object tests |
| 0x140830430 / 0x1408bc350 | (1401d7060/1401d7090) / 14029c2c0 | ported | material by number |
| 0x140830460 / 0x1408bc2f0 | (1401d7060/1401d7090) / 14029c260 | ported | material by index |
| 0x1408bc4a0 | 14029c410 | ported | material count |
| 0x140830270 / 0x14084ed10 | (1401d6ea0/1401d6ed0) / 14017aaf0 | ported | object by number |
| 0x1408302a0 / 0x14084eca0 | - / 14017aa80 | ported | object by index |
| 0x14084d4a0 | 140179280 | verified | object has equipment |
| 0x14080e560 | 1401ddf60 | not in ledger | DB step index |
| 0x1408256c0 | none | - | DB step read |
| 0x140896840 | 1401c6d00 | not in ledger | equipment KEY writer (1400538f0 only) |
| 0x140876640 | 1402027e0 | verified | set object material (part B path) |
| 0x140823ca0 | 1401caa00 | verified | dialog initial object |
| 0x140921f30 | 1402f7720 | ported | inside 0x1408959e0 |

## 9. Open questions

1. `OPI+0x10c`: 0x1400594d0 / 0x14005a7b0 read SimCtrl.KEY and InterObject.KEY only when
   `OPI+0x10c != 0`. Loader-spec reads `+0x10c == 0` as "not the first operation", which would mean
   only the first operation reads its own SimCtrl/InterObject KEY in the batch path. Needs the writer
   of +0x10c.
2. Counter-blow dialog: on the batch path 0x1400569d0 passes `noDialog = 1`, so the dialog never
   shows there (loader-spec section 3/6 says otherwise; correct it). It can show only from the
   interactive callers 0x140355ba0 / 0x14141ca90 / 0x1414364f0.
3. 0x1403568a0 deletes the selection dialog twice when the dialog is accepted but returns no object
   (original bug, unreachable in batch).
4. Static QRegExp at 0x1446b4b20 (PPV field separator): find its initialiser to get the pattern.
5. 0x1400599c0 return value: Ghidra lost it (`void`); stack bytes suggest `rc != 0` on the
   `direct == 0` path and `rc == 0` on the other. Unused by 0x1400594d0's caller; check the epilogue
   if needed.
6. Part B: the slot loop calls 0x140060be0 only when the slot's material list is empty, which passes
   material -1 (0x14004db80 on an empty list). Literal from the assembly; confirm intent with part B.
7. `SfMoMatItem` dynamic_cast result is not null-checked; a non-material item in the material manager
   would crash the original. Reproduce as is or guard (evolve).
8. 0x14005a400 reads `db+0x19a8` from the sub-project DB object (0x140cda460 class); its layout is not
   recovered here.
