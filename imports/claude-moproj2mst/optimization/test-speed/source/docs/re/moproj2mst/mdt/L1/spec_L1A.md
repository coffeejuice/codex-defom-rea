# L1 part A: SfOprCogging load slots (vt+0xb8, vt+0x78, vt+0xc0) and cogging properties vt+0x58

DEF_GUI_MO_64.exe v11.0.2, static analysis only (headless Ghidra copy `gproj_mdtA`). Decompiles and
disassembly in `gui_mo/s1/mdt/L1/A/`: `a0.c` .. `a6.c` (annotated `a*n.c`), `766740.asm`,
`766740b.asm`, `7507b0.asm`, `1b2470.asm`, `crt*.asm`; helper scripts `rd.py` (read .rdata
constants by VA), `iatcall.py` (callers of an imported function). All addresses are GUI VAs unless
marked PRE (DEF_PRE_64.exe).

Notation as in L0 (`docs/re/moproj2mst/mdt/L0/spec_*.md`): `OB` = operation class object
(SfOprCogging), `OPI` = operation item (`0x1405ef2b0(OB)` = `*(OB+0x20)`), `W` = GUI model wrapper
(`0x14005f820(OPI)`, lazily created), `E` = engine `DB_STEP_DATA` (`W+0xe0`), `S` = sim control
`E+0x750` (DEF_PRE `Cls_1401e7b10`, field names from
`port/defmodel/include/defmodel/layout/cls_1401e7b10.hpp`), `PROPS` = `OPI+0x10`
(SfOprCoggingProperties), `M` = CogModel = `PROPS+0x40` (`0x1401cc6b0`), `cog` = `*(M+0x540)`
(CogData), `bil` = `cog+0xd8`, `die` = `cog+0x1170`, `man` = `cog+0x2e20`. Field names are those
of `apps/moproj2mst/include/m2m/data/cogging.hpp` (offsets relative to the struct start). Engine
objects use DEF_PRE `LLOBJ_DATA` offsets: `+0x8` number, `+0xc` type, `+0x828` name, `+0x8a0` node
set, `+0x2160` movement block (`+0x2160` PRSNAM char[0x50], `+0x21c8` movement type, `+0x2248`
speed DYNARY: `+0x224c` ndim, `+0x2250` constant value), `+0x2d94` mesh-generation kind,
`+0x3aa4` primitive type, `+0x3ab0 + 8k` primitive parameter k (k < 20).

Call order (L0 spec_C section 2): OB vt+0x58 `0x1405ef430` = reset, **vt+0xb8**, **vt+0x78**,
vt+0x1b8 (KEY/DB load, L0), **vt+0xc0**, `0x140054350` -> **PROPS vt+0x58**, UI refresh. The
operation loader `0x140051340` / `0x140051c30` calls `QDir::setCurrent(projectFolder)`
(`0x1400a4220`) just before OB vt+0x58, so the current directory during all slots below is the
project folder (`cleanPath(QFileInfo(prj+0x18).absolutePath())`). No other `QDir::setCurrent`
caller (`iatcall.py`: 26 callers) is on the load path; `SetCurrentDirectoryA` has one caller,
`0x1417f4a0c`, not on this path.

---

## 1. vt+0xb8 = `0x140750650` `void SfOprCogging::loadPrologue()`

```cpp
void SfOprCogging_loadPrologue(OB* self) {
    SfOprBase_loadPrologue(self);         // 0x1405f1bb0 (L0 spec_C 2.1, ported):
                                          //   S->tnow3(+0x650) = 1, S->tnow2(+0x648) = 0.0 (qword 0),
                                          //   S->trans[1](+0x108) = 1, S->trans[0](+0x104) = 1
    setTrans2(self, 1);                   // 0x1405ef6f0: S->+0x108 = (uint)(byte)1   (again)
    setTrans1(self, 1);                   // 0x1405ef750: S->+0x104 = (uint)(byte)1   (again)
    OPI* opi = OB_opi(self);              // 0x1405ef2b0
    if (!opi) return;
    W* w = OPI_model(opi);                // 0x14005f820 (may create W)
    if (w) {
        SimCtrl* s = W_simctrl(w);        // 0x140c7b2d0: E ? E+0x750 : 0
        if (s) {
            s->nstep  /*+0x16c*/ = 100;
            s->stpinc /*+0x164*/ = 10;
            s->nstart /*+0x160*/ = -1;
            int z = 0; setDtMode(s, &z);  // 0x1408da6f0: *(int*)(s+0x240) = *arg  (DTTYPE mode = 0: DSMAX)
            s->dsmax  /*+0x1e0*/ = 0.1;   // qword copy of .rdata 0x1421bfdd8 (0x3fb999999999999a)
            s->solmtd /*+0x2e4*/ = 1;
            s->solmtt /*+0x318*/ = 1;
            s->dplen  /*+0x2c8*/ = 0;     // 64-bit zero store (= +0.0)
        }
    }
    ContactBlock* icb = OPI_icb(opi);     // 0x140065790: OPI+0x70 (never null)
    if (icb) setLubSystem(icb, 1);        // 0x1400ab6f0: byte icb+0x70 (= OPI+0xe0, lubSystem) = 1
}
```

* `0x1405ef6f0` / `0x1405ef750`: `m = 0x1405ef2e0(self)` (= `OPI ? 0x14005f770(OPI) : 0`),
  `s = 0x140c7b2d0(m)`, store when both non-null. The duplicate TRANS stores are in the original.
* All stores are integer/qword moves; no FP arithmetic.
* Effect on files: these are the operation's sim-control defaults; vt+0x1b8 (L0) runs after this and
  overwrites them from the operation's `SimCtrl.KEY` when it reads it. They reach files only
  through whatever the Save writes from the engine model (operation KEY files, not this part).
  `lubSystem = 1` is the `InterObjectMethod="System"` attribute of the operation's `<Lubricant>` in
  the `.moproj` written on Save (`apps/moproj2mst/src/model/operation_rescue.cpp`); all corpus
  cogging operations checked already have "System". Nothing here touches the Cogging.MDT.

## 2. vt+0x78 = `0x140750790`

Empty (`ret`). Nothing to port.

## 3. vt+0xc0 = `0x1407507b0` `void SfOprCogging::postLoad()`

Does not call the base `0x1405f1c00`.

```cpp
void SfOprCogging_postLoad(OB* self) {
    OPI* opi = OB_opi(self);                          // 0x1405ef2b0
    if (!opi) return;
    W* w = OPI_model(opi);                            // 0x14005f820
    if (!w) return;
    SfOprCoggingProperties* pr = dynamic_cast<SfOprCoggingProperties*>(
        (SfOprBaseProperties*)opi->props /*OPI+0x10, 0x140051230*/);
                                                      // __RTDynamicCast 0x14146f8fc(p, 0,
                                                      //   0x143579b60 SfOprBaseProperties,
                                                      //   0x1437cf500 SfOprCoggingProperties, 0)
    if (!pr) return;
    CogModel* M = pr->model;                          // 0x1401cc6b0: *(PROPS+0x40)
    if (!M) return;

    if (W_objectCount(w) /*0x140c7c1a0: E ? E+0x1378 : 0*/ < 3) {   // fresh operation only
        initObject(M, 1); initObject(M, 2); initObject(M, 3);        // 0x140765dd0 (3.1)
        if (useManip(M) /*0x140765960: byte cog+0xc8*/) {
            initObject(M, 4); initObject(M, 5); initObject(M, 6); initObject(M, 7);
        }
        if (dieCount(M) /*0x140765b00: *(int*)(die+0x14a8) == 1 ? 4 : 2*/ == 4) {
            initObject(M, 8); initObject(M, 9);
        }
        if (useMandrel(M) /*0x14075dc00: byte M+0x8*/) initObject(M, 10);
        FILE* f = fopen("DEF_CNT.DAT", "wt");         // 0x14146fcd8; relative = current dir = project folder
        if (f) { fprintf(f, "1\n"); fclose(f); }      // 0x14146da0c, 0x14146fb28; text mode -> "1\r\n"
    }

    syncObjectGeometry(M, 1);                         // 0x140766740 (3.2), always
    syncObjectGeometry(M, 2);
    syncObjectGeometry(M, 4);
    syncObjectGeometry(M, 3);
    syncObjectGeometry(M, 8);
    if (useMandrel(M)) syncObjectGeometry(M, 10);     // 0x14075dc00

    // ---- equipment of the top die (object 2) -------------------------------------------------
    SfMoEqpManager* em = prj_eqpmgr(opi->project /*0x140055030: OPI+0x100*/);   // 0x1400a5970 (lazy)
    if (!em) return;
    LLOBJ_DATA* o2 = W_object(w, 2);                  // 0x140c7ae80; NOT null-checked (crash if missing)
    SlotRec* rec = OPI_slotByNumber(OB_opi(self), o2->number /*+8*/);  // 0x1400612d0 -> 0x140061250(opi, no-1)
    if (!rec || rec->equipmentId /*+0x28, 0x14004de90*/ >= 1) return;

    int n = em->count();                              // vt+0x1b8 (0x14006cdd0, rowCount)
    QString name = QString("Equipment (") + QString("%1").arg(n + 1, 0, 10, QChar(' ')) + ")";
                                                      // strings 0x1425e3ec8, 0x1425e0c34, 0x1425e0b7c
    if (o2->movType /*+0x21c8*/ == 1 && o2->speed.ndim /*+0x224c*/ == 0 &&
        fabs(o2->speed.value /*+0x2250, double*/) < 1e-6 /*0x142157420*/)   // asm: COMISD 1e-6, |v|; JBE skips
        defaultDieSpeed(w, o2);                       // 0x140353840 (3.4)
    QString tmp = QDir::tempPath() + QDir::separator() + name + QString(".KEY");   // 0x1425e3ed4
                                                      // order: (tempPath + sep) + name, then + ".KEY"
    WriteEquipmentKey(W_engine(w) /*0x140c7acd0*/, o2, QString(tmp) /*copy*/, /*force*/1);  // 0x140358190 (3.5)
    int id = importEquipment(em, name, tmp);          // 0x1400bf510 (3.6)
    QFile(tmp).remove();                              // QFile instance, remove()
    strncpy(o2->prsnam /*+0x2160*/, name.toLocal8Bit().constData(), 0x50);   // 0x141470020 (pads with NULs)
    tmp = em->filePath(&id);                          // vt+0x208 (0x14006d380); result unused afterwards
    rec->equipmentId = id;                            // 0x14004deb0 (+0x28); may be -1 (import failed)
}
```

Notes
* The object count `E+0x1378` is the engine's object-mapping count (DEF_PRE `DB_STEP_DATA`
  `u1378`, model-layout.md line 242). A cogging operation loaded from a project normally has 7
  objects, so the `< 3` branch (object creation, names, DEF_CNT.DAT) is the "new operation" path.
* The objects synchronised unconditionally are 1, 2, 4, 3, 8 (+10): object 9 (Left Die) and 5..7
  (other manipulators) are never synchronised here (`0x140766740` has no case for them either).
* The order "write the temp KEY, then rename PRSNAM" means the equipment KEY copied into the project
  carries the object's PRSNAM as it was before (`""` when `0x140353840` ran, else the old name);
  only the engine object gets `Equipment (N)`.
* UI-only calls that can be dropped: none in this function itself; inside callees see 3.1, 3.3, 3.6.

### 3.1 `0x140765dd0` `void initObject(CogModel* M, int no)`

```cpp
void initObject(CogModel* M, int no) {
    OPI* opi = ownerOpi(M->ownerItem /*M+0x548*/);   // 0x1405f8d90: *(item+0x20)
    addSlotObject(opi, no);                           // 0x140057770 (3.1.1), result unused
    W* w = OPI_model(opi);                            // 0x14005f770
    LLOBJ_DATA* o = W_object(w, no);                  // 0x140c7ae80
    if (!o) return;
    QString nm = objectName(M, no);                   // 0x140765c30 (3.1.2)
    sprintf(o->name /*+0x828*/, (const char*)nm);     // 0x14146de38 = CRT sprintf with the name as the
                                                      //   format (no '%' in any of the names);
                                                      //   QString::operator const char* (toAscii)
    renameSlot(opi, no - 1, QString(o->name));        // 0x1400585e0 (3.1.3), result unused
    o->meshKind /*+0x2d94*/ = 1;
    syncObjectGeometry(M, no);                        // 0x140766740
}
```

#### 3.1.1 `0x140057770` `LLOBJ_DATA* addSlotObject(OPI* opi, int no)`

```cpp
LLOBJ_DATA* addSlotObject(OPI* opi, int no) {
    W* w = OPI_model(opi);                            // 0x14005f770
    if (!w) return 0;
    LLOBJ_DATA* r = W_objectWrap(w, no, true, true);  // 0x140c7c950 (L0 spec_B, ported)
    SfMoObjManager* om = OPI_objmgr(opi);             // 0x1400550d0 (not null-checked)
    int id = om->newId();                             // vt+0x180 = 0x14006c520: id = vt+0x168()
                                                      //   (0x14006c230: first id >= 1 with no item,
                                                      //   by vt+0x1c8), vt+0x188(id) (append item, file
                                                      //   = vt+0x178(id) = Objects/Object%05d.KEY rule
                                                      //   0x140068e60)
    for (int i = opi->slotCodes.size() /*OPI+0x40, QListData::size*/; i < no - 1; ++i)
        emit signalObjDataAdded(tr("Undefined"), i, opi->id /*+0x108*/);   // 0x1402cf3c0 on
                                                      //   0x143d76700, "2signalObjDataAdded()": UI only
    setSlotCode(opi, no - 1, id, /*reload*/0, /*view*/1, /*signal*/1);     // 0x140057c60 (L0 spec_B 7):
                                                      //   reload 0: objectWrap(no,1,1); obj+0x2bf0 = 360.0,
                                                      //   +0x2bf8 = 0; slot items, objmgr refresh, signal
    objmgrRefresh(OPI_objmgr(opi));                   // 0x140069620
    return r;
}
```

The new object-manager items (one per `initObject`) and the slot codes are project state that the
`.moproj` written on Save contains (object list, operation slots).

#### 3.1.2 `0x140765c30` `QString objectName(CogModel*, int no)`

`no` 1..10: `"Billet"`, `"Top Die"`, `"Bottom Die"`, `"Left Manipulator 1"`, `"Left Manipulator
2"`, `"Right Manipulator 1"`, `"Right Manipulator 2"`, `"Right Die"`, `"Left Die"`, `"Mandrel"`
(0x142678698, 6a8, 6b0, 6e0, 6f8, 710, 728, 6c0, 6d0, 6a0); any other value: null QString.

#### 3.1.3 `0x1400585e0` `bool renameSlot(OPI* opi, int s, QString name /*by value*/)`

```cpp
bool renameSlot(OPI* opi, int s, QString name) {
    if (!OPI_model(opi)) return false;                // 0x14005f770
    if (!slotInRange(opi, s)) return false;           // 0x140060600: 0 <= s < OPI+0x40 size
    SfMoItem* it = objLibItem(opi, slotCode(opi, s)); // 0x140060030(opi, 0x14005fb30(opi, s)):
                                                      //   objmgr vt+0x1c8 + 0x1400690a0
    if (!it) return false;
    it->setName(name);                                // item vt+0x40 (library object name -> .moproj)
    if (LLOBJ_DATA* o = slotObject(opi, s)) {         // 0x140057690: code > 0 || -2 -> W object s+1
        o->vt[1](o, name.toLocal8Bit().data());       // LLOBJ vt+0x8 = 0x140833b10 = PRE 14015faa0:
                                                      //   strcpy(o+0x828, name)
        refreshSlotItems(opi, o->number);             // 0x140060100 (L0 spec_B 7)
    }
    classTreeUpdate(OPI_class(opi) /*0x140054e90*/, OPI_treeModel(opi, false) /*0x140054f40:
                    OPI+0x160, not created*/, s + 1, 0x3f2);   // 0x1405f0b60: QStandardItem tree of
                                                      //   the operation (UI only, drop)
    emit signalObjDataChanged(name, s, opi->id);      // 0x1402cf3c0 "2signalObjDataChanged()": UI only
    return true;
}
```

### 3.2 `0x140766740` `void syncObjectGeometry(CogModel* M, int no)`

Writes the cogging model's geometry into the engine object's primitive description; for an
inherited billet (type 5) it first reads the object's KEY file and updates the model's billet
dimensions from its node bounding box. Jump table at `0x140766e48` (cases `no-1` = 0..9):
`0 -> billet, 1/2/7 -> die, 3 -> manipulator, 9 -> mandrel, 4/5/6/8 -> nothing`.

```cpp
void syncObjectGeometry(CogModel* M, int no) {
    OPI* opi = ownerOpi(M->ownerItem);                // 0x1405f8d90(M+0x548)
    W* w = OPI_model(opi);                            // 0x14005f770
    LLOBJ_DATA* o = W_object(w, no);                  // 0x140c7ae80
    if (!o) return;
    switch (no - 1) {
    case 0: {                                         // Billet
        CogBillet* b = &M->cog->billet;               // 0x14075dcb0 (*(M+0x540)), 0x14044b5d0 (+0xd8)
        if (o->type /*+0xc*/ == 5) {                  // inherited ("Read From DB") slot
            int code = inheritedSlotCode(opi, 0);     // 0x14005fd20 (ported, loader.cpp): -99 unless
                                                      //   slot 0 is -2 and a previous operation owns it
            SfMoObjManager* om = prj_objmgr(opi->project);  // 0x140055030, 0x1400a58f0
            QString f = om->filePath(&code);          // vt+0x208 (0x14006d380): null QS for -99
            if (QFile::exists(f)) {                   // static QFile::exists
                W* t = new W(/*parent*/0, /*dim*/3);  // 0x14146eabc(0x278), 0x140c7a5b0 (null if alloc fails)
                char reset = 1; int mode = 0;
                ReadKeyFile(W_engine(t) /*0x140c7acd0*/, f.toLocal8Bit().data(), &reset, &mode);
                                                      // 0x1408969a0 = PRE 1401b5f00, rc ignored
                LLOBJ_DATA* s1 = W_object(t, 1);      // 0x140c7ae80
                double mn[3], mx[3];                  // rsp+0x58.., rsp+0x70..
                if (s1 && NodeBox(s1 + 0x8a0, mn, mx)) {   // 0x140855d00 = PRE 140181ae0
                    double dz = mx[2] - mn[2];        // SUBSD, stored to a local first
                    b->length /*+0x410*/ = mx[0] - mn[0];
                    b->width  /*+0x430*/ = dz;
                    b->height /*+0x428*/ = dz;
                    b->dia    /*+0x418*/ = 2.0 * dz;  // MULSD with .rdata 0x142101520 (2.0): exact
                }
                if (t) t->vt[3](t, 1);                // vt+0x18 scalar deleting dtor
            }
        }
        if (M->rotationalSymmetry /*byte M+0x15, 0x14075dca0*/) setPrimParam(o, 6, 1.0);   // 0x1421249e0
        setPrimType(o, b->geoTyp /*+0x408*/ + 6);     // 0x1401b2660: o+0x3aa4
        setPrimParam(o, 0, b->length);                // 0x1401b2470: 0 <= k < 20 -> o+0x3ab0+8k
        setPrimParam(o, 1, b->width);                 //   (+0x430)
        setPrimParam(o, 2, b->height);                //   (+0x428)
        setPrimParam(o, 3, b->dia);                   //   (+0x418)
        setPrimParam(o, 4, b->inDia);                 //   (+0x420)
        setPrimParam(o, 5, b->radius);                //   (+0x438)
        break;
    }
    case 1: case 2: case 7: {                         // Top Die, Bottom Die, Right Die
        CogDie* d = &M->cog->die;                     // 0x14075dcf0 (+0x1170)
        setPrimType(o, 9);                            // always 9 (GeoTyp 3), whatever d->geoTyp is
        setPrimParam(o, 0, d->length /*+0x410*/);
        setPrimParam(o, 1, d->width  /*+0x418*/);
        setPrimParam(o, 2, d->height /*+0x420*/);
        setPrimParam(o, 3, d->radius /*+0x428*/);
        break;
    }
    case 3: {                                         // Left Manipulator 1
        CogManip* m = &M->cog->manip;                 // 0x14075dcd0 (+0x2e20)
        setPrimType(o, 10);
        setPrimParam(o, 0, m->length  /*+0x410*/);
        setPrimParam(o, 1, m->width   /*+0x418*/);
        setPrimParam(o, 2, m->height  /*+0x420*/);
        setPrimParam(o, 3, m->radius1 /*+0x430*/);
        setPrimParam(o, 4, m->radius2 /*+0x438*/);
        setPrimParam(o, 5, m->radius3 /*+0x428*/);
        setPrimParam(o, 6, m->height1 /*+0x448*/);
        setPrimParam(o, 7, m->width1  /*+0x440*/);
        setPrimParam(o, 8, m->f490    /*+0x490*/);    // not in CogManip yet: ctor default 45.0,
                                                      //   not read from the MDT (mdt.cpp line 900)
        break;
    }
    case 9: {                                         // Mandrel
        double p0 = 0.0, p3 = 0.0, p4 = 0.0;
        if (projectUnit(opi) /*0x140054ff0: prj ? prj+0x20 : 0*/ == 2) { p0 = 40.0; p3 = 20.0; }
        else                                                          { p0 = 1000.0; p3 = 500.0; }
        setPrimType(o, 6);
        setPrimParam(o, 0, p0); setPrimParam(o, 3, p3); setPrimParam(o, 4, p4);   // 1, 2 untouched
        break;
    }
    default: break;                                   // 4, 5, 6, 8
    }
}
```

FP notes (assembly `766740.asm`): only the billet bounding-box branch computes: `dz = mx[2] -
mn[2]` (one SUBSD, double, kept in memory), `length = mx[0] - mn[0]` (SUBSD), `dia = 2.0 * dz`
(MULSD, exact). All other values are 64-bit moves into XMM2 (the setter takes a double in XMM2,
`1b2470.asm`). Constants (.rdata): 0x1421249e0 = 1.0, 0x142101520 = 2.0, 0x1421d3cb0 = 40.0,
0x1421ce520 = 20.0, 0x1421d94e0 = 1000.0, 0x142678c70 = 500.0. The billet's axis is taken as X and
both width and height as the Z extent; the Y extent is not used.

`setPrimType` (`0x1401b2660`, `o+0x3aa4 = v`) and `setPrimParam` (`0x1401b2470`, range-checked
store) are GUI accessors on LLOBJ_DATA fields that DEF_PRE also has (copied by PRE `140161ac0`,
reset by PRE `14019ccc0`), but their bodies have no unique twin; port as two inline setters.

### 3.3 Fields of the KEY read (3.2 case 0)

`0x1408969a0(E, path, &reset, &mode)` = PRE `1401b5f00` (ported, `defcmd/key_file.cpp`): with
`reset` set it resets the scratch model, then reads the KEY. On a failed `fopen(path, "r")` it
prints `"ERROR: Keyword file %s open error!\n"` to stdout and returns 1 (not reachable here: the
file was tested with `QFile::exists`). `0x140855d00` = PRE `140181ae0` (verified,
`defdbio/src/gendb/p4_02b/object.cpp`): min/max per coordinate over all nodes of the node set
(count `+0x12c`, coordinates `+8`, dimension `*(+0x130)+0x15f8`); false when there is no node
array.

### 3.4 `0x140353840` `bool defaultDieSpeed(W* w, LLOBJ_DATA* o)`

```cpp
bool defaultDieSpeed(W* w, LLOBJ_DATA* o) {
    if (!w) return false;
    if (!o) o = W_primaryDie(w, true);                // 0x140c7b140 -> 0x140c7b1a0 (not reached: o given)
    if (!o) return false;
    o->movType /*+0x21c8*/ = 1;                       // speed
    o->speed.ndim /*+0x224c*/ = 0;                    // constant
    strcpy(o->prsnam /*+0x2160*/, "");                // 0x14146fec0, string 0x142318c2f is empty
    if (W_simctrl(w)->unit /*S+0xfc*/ == 1) o->speed.value /*+0x2250*/ = 76.0;   // 0x14231da08 (SI)
    else                                    o->speed.value = 3.0;                // 0x1421ce530 (English)
    return true;
}
```

No null check of `W_simctrl(w)` (E exists here). GUI-only (no twin).

### 3.5 `0x140358190` `bool WriteEquipmentKey(DB_STEP_DATA* E, LLOBJ_DATA* o, QString path /*by value*/, bool force)`

(Also used by L0 `0x1400538f0`; summary in L0 spec_A section 6.)

```cpp
bool WriteEquipmentKey(DB_STEP_DATA* E, LLOBJ_DATA* o, QString p, bool force) {
    if (!p.toUpper().endsWith(QString(".KEY"), Qt::CaseSensitive)) p += ".KEY";   // 0x14231cc7c/84
    QFileInfo fi(p);
    p = fi.baseName(); p += "."; p += fi.suffix();    // baseName = text before the FIRST '.'
    QString sep; sep = QDir::separator();
    p = QDir::convertSeparators(fi.absolutePath() + sep + p);
    bool ask = QFile(p).exists() && !force;
    if (ask) {
        int r = QMessageBox::question(0, tr("Overwrite File"),
              tr("The file %1 already exists\n. Do you want to overwrite it?").arg(p),
              QMessageBox::Yes /*0x4000*/, QMessageBox::No /*0x10000*/, 0);
        if (r == QMessageBox::No) return false;
    }
    WriteEquipmentKeyFile(E, p.toLocal8Bit().constData(), o->number);   // 0x140896840 = PRE 1401c6d00
    return true;
}
```

Here `force = 1`: no dialog. Path: `QDir::tempPath() + '\' + "Equipment (N).KEY"` ->
`<temp>\Equipment (N).KEY` (native separators). `0x140896840`: `fopen(path, "wt")`, object by
number (`0x14084ed10` on `E+0x1380`), header `0x140895560`, object block `0x14097da30`, the
counter-blow partner object (`+0x2360 > 0`) `0x14097e170`, `0x1409ee2a0`; returns 3 when the open
fails. Ported as `port/defkey/src/item_writers/equipment_key.cpp` (status file
`docs/re/status/mdt-model.tsv`).

### 3.6 `0x1400bf510` `int importEquipment(SfMoEqpManager* em, QString name, QString file)`

```cpp
int importEquipment(SfMoEqpManager* em, const QString& name, const QString& file) {
    int id = em->vt[0x160](name, file);               // 0x14006c610 (below)
    emit signalMoEquipManagerChanged(id);             // 0x1402cf3c0 "2signalMoEquipManagerChanged()": UI only
    return id;
}
```

`0x14006c610` (SfMoItem manager vt+0x160, `int addFromFile(name, file)`):

```cpp
int addFromFile(Mgr* m, const QString& name, const QString& file) {
    QString src = m->vt[0x230](file);                 // Eqp 0x1400bf290: copy of `file`
    if (!QFile::exists(src)) return -1;
    int id = m->vt[0x168]();                          // 0x14006c230: first id >= 1 with no item (vt+0x1c8)
    QString dst = m->vt[0x178](id);                   // 0x1400bef30: cleanPath(prj + sep + "Equipments")
                                                      //   + sep + "Equipment%05d.KEY" (QDir::separator,
                                                      //   no final cleanPath)
    QString nm;
    bool ok = m->vt[0x238](src, dst, &id, &nm);       // Eqp 0x1400bf310 (below)
    if (!name.isEmpty()) nm = name;
    if (!ok) return -1;
    QStandardItem* root = m->invisibleRootItem();
    SfMoItem* it = m->vt[0x170](id);                  // 0x1400beec0: new 0x40-byte item (0x1400beba0)
    QList<QStandardItem*> row = m->vt[0x240](nm, file, id, QString(dst));   // 0x1400bfdf0: another
                                                      //   item vt+0x170(id): setName(nm) (vt+0x40),
                                                      //   +0x20 = file (0x14006b970), +0x28 = dst
                                                      //   (0x14006ba30), plus a checkable QStandardItem
    if (!row.isEmpty()) root->appendRow(row);         // always taken; `it` is leaked (original)
    else { it->setName(nm); it->+0x20 = file; it->+0x28 = dst; root->appendRow(it); }
    return id;
}
```

`0x1400bf310` (Eqp vt+0x238, `bool copyIn(src, dst, int* id, QString* name)`):

```cpp
*name = QFileInfo(src).completeBaseName();            // "Equipment (N)"
mkpathWithMessage(QFileInfo(dst).absolutePath(), true);   // 0x140819590, 0x14081afb0 (ported P07):
    // QDir::mkpath; on failure (only) a modal QMessageBox(Information, tr("Information"),
    // "  " + tr("Can not make the directory %1.").arg(dir) + "  " (two spaces each side,
    // 0x1427a50f0 / 0x1427a50c8, via 0x1400ac2b0 / 0x140002110), Ok 0x400); returns true either way
if (QFile::exists(dst)) QFile::remove(dst);
return QFile::copy(src, dst);
```

Effect: `<project>/Equipments/Equipment%05d.KEY` (first free id) is created (folder made when
missing) as a byte copy of the temp KEY, and the equipment manager gets a new row (name
`Equipment (N)`, file = that path), which the `.moproj` written on Save lists.

---

## 4. PROPS vt+0x58 = `0x14075ee20` `bool SfOprCoggingProperties::load()`

```cpp
bool SfOprCoggingProperties_load(Props* p) {
    readStepsAndDtmax(p, 0);     // 0x1405fb720
    readEnvTemp(p, 0);           // 0x1405fbae0
    readConvection(p, 0);        // 0x1405fbe30
    return true;
}
```

Each reader takes `SimCtrl* s` (here 0): `s = 0 -> E = 0x1405f8df0(p)` (`item = 0x1405f8d90(p)` =
`*(p+0x20)`; `E = 0x14005f7a0(item)` = `W ? W->E : 0`), `s = E ? E+0x750 : 0`; nothing when `s`
is null; return `s != 0`. `blk = *(p+0x30)` (CommonBlock, `m2m/data/common.hpp`), not
null-checked.

```cpp
// 0x1405fb720
blk->numSteps /*+0x00*/ = s->nstep  /*+0x16c*/;
blk->stepIncr /*+0x04*/ = s->stpinc /*+0x164*/;
blk->dtmax    /*+0x40*/ = QString("%1 %2").arg(s->dtmax1 /*+0x1d0, double*/, 0, 'g', -1, QChar(' '))
                                          .arg(QString(unitName(0, s->unit /*+0xfc*/)), 0, QChar(' '));
// 0x1405fbae0
blk->envTemp  /*+0x20*/ = QString("%1 %2").arg(s->envtmp.value /*+0x3d0*/, 0, 'g', -1, ' ')
                                          .arg(QString(unitName(9, s->unit)), 0, ' ');
// 0x1405fbe30
blk->convection /*+0x30*/ = QString("%1 %2").arg(s->cnvcof.value /*+0x4c8*/, 0, 'g', -1, ' ')
                                            .arg(QString(unitName(0x20, s->unit)), 0, ' ');
```

`unitName` = `0x140917de0` (ported P17, `src/model/units.cpp`: `0x1427bc000 + k*0x28` when unit
== 1, else `0x1427bc014 + k*0x28`). Format strings 0x14249d1c0/1c8/1d0 (`"%1 %2"`). The temporary
order in the original: the unit QString is built before the `"%1 %2"` QString, then `arg(double)`,
then `arg(QString)`. No arithmetic.

Effect: properties-panel strings only. The cogging `<Properties>` element is empty in every corpus
project (spec.md section 2), so these values do not reach the `.moproj`; the Cogging.MDT writer
reads only `PROPS+0x40` (CogModel). No file effect found.

---

## 5. What reaches files

| Change | Where | File effect |
|---|---|---|
| sim-control defaults (1) | engine model | none directly; overwritten by the operation's SimCtrl.KEY when vt+0x1b8 reads it; otherwise whatever the Save writes from the engine model (not this part) |
| `lubSystem = 1` (1) | OPI+0xe0 | `.moproj`: `InterObjectMethod="System"` on Save (corpus cogging ops already "System") |
| new object-manager items, slot codes, item names, engine object names, `+0x2d94 = 1`, `+0x2bf0 = 360.0` (3.1) | project / engine model | `.moproj` object list and operation slots on Save; only when the engine model has < 3 objects (new operation) |
| `DEF_CNT.DAT` = `"1\r\n"` (3) | current directory = project folder | **new/overwritten file in the project folder**; only when < 3 objects |
| billet Length/Width/Height/Dia from the object KEY bounding box (3.2 case 0) | `M->cog->billet` (+0x410/+0x430/+0x428/+0x418) | **Cogging.MDT bytes** (BILLET section) on Save. Taken when object 1 has type 5 and its resolved library file exists: all corpus cogging operations have slots 1..3 inherited (`Id="-2"`, ObjectType 5), so this explains oracle B `42_vdie` (Length 1378.36 -> 1112.56, Width/Height = Z extent, Dia = 2 x Z extent) |
| object primitive type and parameters for objects 1, 2, 3, 4, 8, (10) (3.2) | engine objects `+0x3aa4`, `+0x3ab0..` | Save's `0x140766200` reads objects 1, 2, 4 back with the same index mapping (billet 0..5, die 0..3, manip 0..8), so the round trip is the identity for the MDT except through the billet values above. Die objects always get type 9 (GeoTyp 3) but `0x140766200` does not set `die->geoTyp`, so no MDT change. Whatever the Save writes of objects into operation KEY files would carry them (not this part) |
| PRSNAM `"Equipment (N)"`, movement type 1 / constant speed 76.0 (SI) or 3.0 (English) on object 2 (3, 3.4) | engine object 2 | **Cogging.MDT**: Save's `0x140766200(M, 2)` copies `o2+0x21c8` to `die->movType` (+0xc78) and, for type 1, `o2+0x2250` to `die->spddie` (+0xc88). Only when object 2's slot has no equipment (`EquipmentID < 1`); every corpus cogging operation has an equipment on object 2, so not seen in the corpus |
| `<prj>/Equipments/Equipment%05d.KEY` + equipment manager row + slot `equipmentId` (3.5, 3.6) | project folder, `.moproj` | **new KEY file in the project folder** (copy of the temp file, PRSNAM as before the rename), `.moproj` equipment list and the object's EquipmentID; same condition as the row above. The temp file `<temp>\Equipment (N).KEY` is written and removed |
| CommonBlock strings (4) | PROPS+0x30 | none |

## 6. Dialogs on this path

| Dialog | Where | Condition | Choices | Converter policy |
|---|---|---|---|---|
| "Overwrite File" / "The file %1 already exists\n. Do you want to overwrite it?" | 0x140358190 | never here (`force = 1`) | Yes / No | not needed |
| "Information" / "Can not make the directory %1." | 0x14081afb0 (from 0x1400bf310) | `QDir::mkpath` of `<prj>/Equipments` fails | Ok | report as an error/warning; the copy then fails and the id is -1 |

`signalObjDataAdded`, `signalObjDataChanged`, `signalMoEquipManagerChanged`, the class tree update
`0x1405f0b60` and `0x140069620`'s view refresh are UI notifications and can be dropped (they do not
change model data; `0x140069620` = `0x1400a5c10` + `0x14006d490`, per L0 spec_A, check before
dropping that it has no model side effect).

## 7. Engine calls

| GUI | DEF_PRE twin | Status | Role |
|---|---|---|---|
| 0x1408969a0 | 1401b5f00 | ported (defcmd key_file) | read object KEY into scratch model |
| 0x140855d00 | 140181ae0 | verified (defdbio p4_02b) | node bounding box |
| 0x140896840 | 1401c6d00 | ported (defkey equipment_key.cpp, mdt-model.tsv) | equipment KEY writer |
| 0x140833b10 (LLOBJ vt+0x8) | 14015faa0 | ported (defmodel llobj_data) | set object name (strcpy +0x828) |
| 0x140830270 / 0x14084ed10 | (1401d6ea0/ed0) / 14017aaf0 | ported | object by number (inside 0x140c7ae80 / writer) |
| 0x140821bf0 (in 0x140c7a5b0) | 1401c8950 | ported | scratch DB_STEP_DATA ctor |
| 0x140830180 | (sig not unique) | trivial | E+0x1378 object count |
| 0x1408da6f0 | (sig not unique) | trivial | S+0x240 = v |
| 0x1408e01e0 | (trivial) | - | S+0xfc unit |
| 0x1401b2470 / 0x1401b2660 | (sig not unique) | trivial, not ported | primitive param / type setters |
| 0x140853240 | (sig not unique) | inside 140181ae0 | node-set dimension block |

GUI-side functions this part needs (no DEF_PRE twin; status from `docs/re/moproj2mst/ledger.tsv`,
`mdt/ledger.tsv`, `status/mdt-*.tsv` and a grep of `@src`):

| GUI | Role | Status |
|---|---|---|
| 0x140750650, 0x1405ef6f0, 0x1405ef750 | cogging prologue, TRANS setters | not ported (0x1405f1bb0 ported, MDT) |
| 0x1407507b0 | cogging post-load | not ported |
| 0x140765dd0, 0x140765c30 | object init, default names | not ported |
| 0x140057770, 0x1400585e0, 0x140057690 | add slot object, rename slot, slot object | not ported |
| 0x140057c60, 0x140060100 | set slot code, refresh slot items (L0 spec_B 7) | not ported (spec in L0) |
| 0x140766740 | geometry from the cogging model | not ported |
| 0x140353840 | default die speed | not ported |
| 0x140358190 | equipment KEY with path rules | not ported (spec in L0 spec_A 6) |
| 0x1400bf510, 0x14006c610, 0x1400bf310, 0x1400bfdf0, 0x1400bf290, 0x14006c230, 0x14006c520, 0x14006c570 | equipment import into the manager | not ported (0x1400beec0/0x1400beba0, 0x1400bef30, 0x14081afb0, 0x14006d380 ported) |
| 0x14075ee20, 0x1405fb720, 0x1405fbae0, 0x1405fbe30, 0x1405f8df0, 0x14005f7a0 | props load | not ported (0x140917de0 ported P17) |
| 0x140c7c950, 0x140c7ae80, 0x140c7c1a0, 0x140c7acd0, 0x140c7b2d0, 0x140c7a5b0, 0x14005fd20, 0x140765960, 0x140765b00, 0x14075dc00, 0x1400a5970, 0x1400612d0, 0x14004de90, 0x14004deb0, 0x1400ab6f0, 0x140060030, 0x140060600, 0x140054ff0 | wrappers / accessors | ported (MDT, P01..P17) |
| 0x1405f0b60, signals 0x1402cf3c0, 0x140054f40 | UI | drop |

## 8. Open questions

1. Object number in library object KEY files: 3.2 case 0 takes object **1** of the scratch model
   read from `Objects/ObjectN.KEY`. If such a file stores the object under another number the bbox
   step is skipped silently. Check the object KEY files of the corpus (e.g. `42_vdie`) and confirm
   with an oracle-B re-save that the new Length/Width/Height/Dia equal the X / Z extents of the
   resolved file's nodes (the corpus note in spec.md section 6 lists only two of the four re-saved
   values).
2. `0x14005fd20` returns -99 unless slot 0 itself is -2; an inherited billet in an operation whose
   slot 0 code is a library id never reads a file (by design: then the object is not type 5 after
   the KEY load). Confirm type 5 is only set for slot code -2 (L0 part B).
3. `M+0x540` is a pointer in the binary (`0x14075dcb0`), while `cogging.hpp` embeds `CogData` at
   +0x540; the port keeps value semantics (no effect on behaviour).
4. `CogManip` lacks the `+0x490` double (ctor default 45.0, never read from the MDT); 3.2 case 3
   and Save's `0x140766200` copy it as parameter 8. Add it to `cogging.hpp` (owner P06/P11).
5. `0x140069620` (object manager refresh) is called from 3.1.1 and `0x140057c60`; decide whether it
   has model side effects before dropping it (`0x1400a5c10`).
6. The `< 3 objects` branch, the equipment branch (no equipment on object 2) and the mkpath
   dialog do not occur in the 27 corpus Cogging.MDT projects (object 2 always has
   EquipmentID >= 1; operations have 7 or 9 objects). They need synthetic oracle-B cases if the
   converter is to claim them.
7. `S->dplen` (+0x2c8) is stored as a 64-bit zero; `S->dsmax` = 0.1 as the exact .rdata bits
   0x3fb999999999999a.
