# s1-04: MST writers of Boolean, Cycle, DoeOutput, Convert2Dto3D, CopyMirror, Group, Distortion, MultiBlow

DEF_GUI_MO_64.exe v11.0.2, static only (Ghidra headless on `gproj_s1d`, no auto-analysis).
Addresses are VAs. Decompiles: `s1-04/d1.c` .. `d13.c` (cleaned copies `d*c.c`, where `Sxxxx` is the
`__chkstk` frame slot `rsp+xxxx`), disassembly checks `ins1.txt`..`ins3.txt` (`scripts/ListIns.java`,
`run_ins.sh`). Sample copies in `s1-04/samples/`, check script `tools/validate.py`.

Shared emitters `140131c10`..`140136810` and the base writer `1405f5340` are s1-02's; they are only
named here, with the exact arguments each writer passes.

## 0. Conventions

### 0.1 Writer signature (vtable slot +0x280)

```
void W(SfOprX* self,            // per-class singleton (the "handler"), not the operation
       QTextStream& s,          // MST stream
       Project* prj,            // 140131df0 param_2
       const QString& db,       // primary DB name (1400a7670)
       int* sim,                // running SIMULATION counter
       OpItem* item,            // the operation (moproj <Operation>)
       int dbreadArg,           // passed to 1401343e0 as 4th arg
       int onlySim);            // 0 = write every block; n = write only block n (and stop)
```

Save always passes `dbreadArg = 0, onlySim = 0`: `140131df0` calls `140132100(..., 0, 0)` or
`1401321a0(..., 0, 0)` (DOE). So every `if (onlySim == 0 || *sim == onlySim)` below is true on
Save and every `if (onlySim) return/break` is not taken; they are kept for the port.

`140131df0` sets `self->curItem = item` (`1405ef280`, QPointer at self+0x20) before the call and
restores the old value after. Writers called by Cycle (section 2) do NOT get this; see O-3.

Most writers start by `props = dynamic_cast<SfOprXProperties*>(1405ef320(self, item))`:
`1405ef320(self, it)` = `(it ? it : self->curItem)->props` (item+0x10, `140051230`),
`14146f8fc` = MSVC `__RTDynamicCast(p, 0, &SfOprBaseProperties, &target, 0)` (no throw). A null
cast writes nothing and does not touch `*sim`.

### 0.2 Notation used in the pseudo-code

- `s << "lit"` is `QTextStream::operator<<(const char*)`; `<< n` (int) is `operator<<(int)`;
  `<< d` (double) is `operator<<(double)` (default real notation, precision 6; s1-06);
  `endl` writes `"\n"` (all line ends in MST and side files are LF) and flushes.
- `SEP` = `QDir::separator()` (`'\\'` on Windows). `cleanDirPath`/`cleanPath` (Qt 4, same
  function) turn `'\\'` into `'/'`, so every path printed with `cleanDirPath` has `/`.
- `taskRel` = `item->taskDir` QString at item+0x130 (`140051110`), e.g. `Operations/Task00004`.
- `taskAbs` = `140050df0(item)` = `convertSeparators(cleanPath(prjAbs + SEP + taskRel))`, with
  `prjAbs = 1400a4220(item->project)` = `cleanPath(QFileInfo(project+0x18).absolutePath())`; native
  `'\\'` separators. Null QString when `item->project` (item+0x100) is null.
- `P(name)` = `cleanDirPath(taskRel + SEP + name)`, e.g. `Operations/Task00004/Step.KEY`.
- `REL(path)` = `QDir(QFileInfo(db).absolutePath()).relativeFilePath(path)` (`140819590` +
  `QDir::relativeFilePath`); `db` is relative, so this is relative to the current directory
  (the project directory after Open).
- `kw(id)` = `140921f30(id)`: copies the name of entry `id` of the DEF_PRE keyword table at
  `0x1427c06e0` (0x130 records of 0x30 bytes, `+0 int id`, `+8 char* name`; `140922160`) into the
  static buffer `0x143dd9ba0` and returns it (`"ERROR: KEYWORD NOT EXISTS!"` if absent).
  Used ids: `kw(3)` = `"KFREAD"`, `kw(10)` = `"KFWRIT"`.
- Emitters are written `E_xxxxxx(args)` with the exact arguments; `5th`/`6th` are stack args.

### 0.3 OpItem accessors used here

| Accessor | Returns | Meaning |
|---|---|---|
| `140051230(it)` | `*(it+0x10)` | properties object (SfOpr*Properties) |
| `140050ac0(it)` | QString copy of it+0x118 | operation name (moproj `<Name>`) |
| `140050d00(it)` | QString copy of it+0x120 | type name (`<Type>`, e.g. `SfOprBoolean2D`) |
| `140051110(it)` | QString copy of it+0x130 | `taskRel` |
| `140050910(it)` | `*(int*)(it+0x108)` | operation id |
| `140055030(it)` | `*(it+0x100)` | project |
| `140055050(it)` | `1400a7390(project)` | active sub-project (flow chart) |
| `140050b20(it)` | `1400b5f40(subprj, id)` | operation ordinal in the flow chart (CURSIM 2nd value) |
| `140050b10(it)` | 0 | CURSIM 3rd value |
| `140050bc0(it)` | `140086720(diagram, id) == -1` | no predecessor in the flow chart |
| `140064740(it)` | byte it+0x150 | start-from-KEY flag (s1-01) |
| `14005f230(it)` | `140050bc0(it) || it+0x150` | "first operation" test |
| `140054e90(it)` | handler singleton for `<Type>` | via global registry `0x143d766b8` |
| `14005f150(it)` | `handler->dim() == 2` | 2D (vtable +0x190, base `1405f5bf0`: flags of +0x188()->+0x58(): `&3==3`→4, `&1`→2, `&2`→3) |
| `14005f1c0(it)` | `handler->dim() == 3` (true if no handler) | 3D |
| `140054ff0(it)` | `*(int*)(project+0x20)` (0 if no project) | project unit (moproj `Unit`; s1-01) |
| `140065790(it)` | `it+0x70` | contact block (`ICB`, see 0.4) |
| `14005f770(it)` | `14005f820(it)` | the operation's DEF_PRE model (see O-5) |
| `140061250(it, k)` | object k (0-based) of the operation | `14004de90(obj)` = obj+0x28 (equipment id) |
| `1400600a0(it, id)` | operation object with id | `14006ba60(obj)` = `cleanPath(obj+0x28 QString)` |

### 0.4 Contact block ICB = item+0x70 (read by s1-02's `140136040` → `1400ac010`)

`byte flag[100]` at +0x00 (object n ↔ `flag[n-1]`), `byte enable` at +0x64, `byte gen` at +0x65,
`double genVal` at +0x68. `1400ac010` prints `"INICTC    \t" << n` for every set flag when
`enable`, then `"GENCTC    \t" << genVal` when `gen`. Two writers here change it before calling
`140136040`: `140065270(icb, n, v)` sets `flag[n-1] = v`, `1400ab7e0(icb, v)` sets `gen = v`. The
changes stay in the model.

## 1. SfOprBoolean: `1411df600` → `1411df6d0`

Classes: `SfOprBoolean` (legacy type name `SfOprBoolean2D` maps here, s1-01). Corpus: 7 blocks.

```
1411df600(self, s, prj, db, sim, item, a7, a8):
    props = 1405ef320(self, item)
    bp = dynamic_cast<SfOprBooleanProperties*>(props)        // TD 0x143a76500
    if (bp) 1411df6d0(self, s, prj, db, sim, item, bp, a7, a8)

1411df6d0(self, s, prj, db, sim, item, bp, dbreadArg, onlySim):
    QString nm, tmp;
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        E_140131c10(s, *sim, item);                       // SIMULATION / *!SIMTYP
        E_1401343e0(s, db, *sim, dbreadArg);              // DBREAD 0 + db
        E_140134db0(s, prj, db, 1, item);                 // note: 4th arg is the constant 1
        E_1401360e0(s, prj, db, 1, item);
        E_140135220(s, prj, db, 1, item, 0);
        E_140135f10(s, prj, db, *sim, item);
        E_140134680(s, item);
        nm = 140050ac0(item);                             // name
        E_140134730(s, nm);                               // OPRNAM
        E_1401347e0(s, nm);                               // SIMNAM
        E_140134900(s, *sim, 140050b20(item), 140050b10(item)=0);   // CURSIM
        s << "NSTEP" << "\t" << "0" << endl;              // 142cc92f4, 142cc7c9c, 142cc7e0c
        BD = *(BooleanData**)(bp+0x38);                   // 140025010
        140065270(ICB(item), BD->workpiece /*+4, 14142ff20*/, 1);   // INICTC on the workpiece
        1400ab7e0(ICB(item), 0);                          // GENCTC off
        E_140136040(s, prj, db, *sim, item);              // INICTC lines (s1-02)
        s << " * Keyword file which contains objects " << endl     // 142ccb1d8
          << kw(10) << endl                               // "KFWRIT"
          << P("Step.KEY") << endl << endl;               // 1411e0aa0 -> 142cd5de0
        1411e1820(BD);                                    // WRITES Boolean.BWZ + DEF_MACHDIST_CTRL2.INI (1.2)
        s << "EXECMD" << "\t" << "4" << endl;             // 142cc9ac4, 142cc7e1c, 142cc7ed4
        s << "DEF_MACHDIST_CTRL23.EXE" << "   "          // 142ccb200, 142cc7f04
          << P("DEF_MACHDIST_CTRL2.INI")                  // 1411e0da0 -> 142cd5e08
          << "  >  "                                      // 142ccb218
          << cleanDirPath(taskRel + SEP + "DEF_MACHDIST_CTRL2.OUT")   // 140002110(fromAscii), 142ccb220
          << endl;
        s << "KFREAD    \t" << 1 << endl;                 // 142ccb238
        tmp = taskRel + SEP + "DEF_MACHDIST_OUT.KEY";      // 1411e0b10 -> 142cd5df0
        s << cleanDirPath(tmp) << endl;
        nm = self->vfunc_0x88();                          // 1411df2d0: "Boolean"; result unused
        E_140134a10(s, db, *sim, 2, (byte)1);             // GENDB ... + db
    }
```

Sample (MO2_Boolean_Oprations, SIMULATION 4) for the writer-owned lines:

```
NSTEP	0
INICTC    	1                    <- 140136040 (flag[0] set from BWZ workpiece = 1)
 * Keyword file which contains objects 
KFWRIT
Operations/Task00004/Step.KEY

EXECMD	4
DEF_MACHDIST_CTRL23.EXE   Operations/Task00004/DEF_MACHDIST_CTRL2.INI  >  Operations/Task00004/DEF_MACHDIST_CTRL2.OUT
KFREAD    	1
Operations/Task00004/DEF_MACHDIST_OUT.KEY
GENDB     	1	2	2
```

### 1.1 BooleanData (bp+0x38, 0x18 bytes, ctor `1411e23f0` from `1411e0850`)

| Off | Type | Default | Meaning | Set by | Read by |
|---|---|---|---|---|---|
| +0x00 | (base `140906740`) | | KEY-token reader base | | |
| +0x04 | int | 1 | workpiece object no | BWZ reader `1411e1dd0` | writer (INICTC flag), `1411e1970`, `1411e0e40` |
| +0x08 | int | 0 | cutter object no | BWZ reader | `1411e1970`, `1411e0e40` (as PASS_OBJNO) |
| +0x0c | byte | 0 | remesh after boolean | BWZ reader | `1411e1970` |
| +0x10 | SfOprBooleanProperties* | owner | | ctor | `1411e1820`, `1411e1cd0` (`1405f8d90(p)` = p+0x20 QPointer → item) |

The values come from `Operations/TaskNNNNN/Boolean.BWZ`, read by properties slot +0xc0
`1411e09d0` → `1411e1cd0` → `1411e1dd0(BD, taskAbs)` (open path, s1-01): file
`convertSeparators(taskAbs + SEP + "Boolean.BWZ")` opened `"rt"` (`14146fcd8`); a loop reads a
token (`1409068f0(rdr, f, 2, buf)`), compares (80 chars) with `WZD_MACHDIST_WORKPIECE_OBJECT_NO` →
int into +4, `WZD_BOOLEAN_CUTTER_OBJECT_NO` → int into +8, `WZD_BOOLEAN_REMESH` → type 1 into +0xc,
`$END` → stop, until the token is `DONE`. A missing file leaves the defaults (1, 0, 0); a read
error shows "ERROR LOADING FILE". The moproj `<Properties Number="0"/>` carries nothing for Boolean.

### 1.2 Side files written during MST generation: `1411e1820(BD)`

```
1411e1820(BD):
    if (!BD->owner || !(it = 1405f8d90(BD->owner))) return -1;
    dir = 140050df0(it);                          // taskAbs, native separators
    1411e1970(BD, dir);                           // Boolean.BWZ
    1411e0e40(BD, dir);                           // DEF_MACHDIST_CTRL2.INI
```

`1411e1970(BD, dir)`: `QFile f(convertSeparators(dir + SEP + "Boolean.BWZ"))`, open mode 2
(WriteOnly, truncate, binary: LF), `QTextStream t(&f)`:

```
t << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << endl << "  * OBJECT # FOR WORKPIECE " << endl << BD->+4 << endl << endl;
t << "WZD_BOOLEAN_CUTTER_OBJECT_NO" << endl << "  * OBJECT # FOR CUTTER " << endl << BD->+8 << endl << endl;
t << "WZD_BOOLEAN_REMESH" << endl << "  * PERFORM REMESH AFTER BOOLEAN" << endl << (int)BD->+0xc << endl << endl;
t << "DONE" << endl << endl;
```

`1411e0e40(BD, dir)`: path `1411e0b80(BD, dir)` = `convertSeparators(cleanDirPath(dir + SEP +
"DEF_MACHDIST_CTRL2.INI"))`; `QDir(subprjDir).cdUp()` is computed and unused (`1400a4ee0`).

```
stepPath = cleanDirPath(taskRel + SEP + "Step.KEY");   // relative, '/' separators
s1 = stepPath.isEmpty() ? "(null)" : stepPath.toAscii()  // QString::operator const char* (latin1)
t << " * Keyword file which contains the workpiece " << endl << kw(3) << endl << s1 << endl << endl;
t << " * Workpiece object #" << endl << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << "\t" << BD->+4 << endl << endl;
t << " * Pass keyword file name" << endl << "WZD_MACHDIST_PASS_OBJNO" << endl << BD->+8 << endl << endl;
t << " * Boolean operation" << endl << "WZD_MACHDIST_OPERATION_BOOLEAN" << endl << endl;
if (14005f1c0(it) /* 3D */) {                  // 2D samples skip this
    FD fd; 1411631e0(&fd); fd.+0x91 = 1 (1411e2450); 1411638a0(&fd, t);   // free-distortion BCC block
}
t << " * Output file name " << endl;
t << "WZD_BOOLEAN_OUTPUT_FILENAME" << endl;
t << cleanDirPath(taskRel + SEP + "DEF_MACHDIST_OUT.KEY") << endl << endl;
```

3D block `1411638a0(fd, t)` with the zeroed `fd` of `1411631e0` (literals 142c44898..142c449b8):
`BEGIN_FREE_DIST_BCC_INFO`, `BCC_INIT    \t1`, `FIXEDNODE    \t<1411634?>`, `FIXEDCOORD    ` +
3×(`"\t"` << 0.0), `FIXED_Z_COORD    ` + 3×(`"\t"` << 0.0), `FIXED_X_COORD    ` + 3×..., 
`FIXED_Y_COORD    ` + 3×..., `AUTO    \t`, `FIXEDZNODE    \t`, `FIXEDZANGLE    \t`,
`FIXEDXNODE    \t`, `FIXEDXANGLE    \t`, `FIXEDYNODE    \t`, `FIXEDYANGLE    \t` each followed by a
getter value (`141344d40`, `1411634e0`, `1411634a0`, `14042fb50`, `14086dde0`, `1411634b0`,
`140da76e0`, `1411634c0` on the zeroed struct; not decompiled, values unknown → O-7),
`END_FREE_DIST_BCC_INFO`. No 3D Boolean sample exists.

Both files are byte-identical to the copies on disk in all 7 corpus Boolean operations
(`tools/validate.py`), so a converter that does not rewrite them still produces the same MST.

## 2. SfOprCycle: `14134dcc0`

Corpus: MO2_Operation_Cycles2 (cycle count 9, one member).

```
14134dcc0(self, s, prj, db, sim, item, a7, a8):
    props = 140051230(item); if (!props) return;
    cp = dynamic_cast<SfOprCycleProperties*>(props);     // TD 0x143ae2588, not null-checked
    ops = 1400a58b0(prj);                                 // operation table
    if (!ops) return;
    sub = 140055050(item); if (!sub) return;
    QList<int> ids = 1400b7470(sub, 140050910(item));     // members of the cycle, flow order
    n = **(int**)(cp+0x38);                               // 14134eeb0: CycleData.count
    140136af0(140050b20(item), 140050ac0(item));          // cycle context ON
    for (k = 0; k < n; ++k) {
        140136b40(k + 1);                                 // cycle iteration
        foreach (int id, ids) {                           // Q_FOREACH (1401b27e0, 140dfc080)
            it = 140047dc0(ops, &id);                     // op item by id
            if (it && (h = 140054e90(it)))
                h->vfunc_0x280(h, s, prj, db, sim, it, a7, a8);   // member's own writer
        }
    }
    140136b30();                                          // cycle context OFF
```

Cycle context (globals, read by s1-02's `140134730`/`1401347e0`):
`140136af0(ord, name)`: `*(int*)0x143d80254 = ord; *(QString*)0x143d80280 = name;
*(byte*)0x143d8025a = 1`. `140136b40(k)`: `*(int*)0x143d80250 = k`. `140136b30()`:
`*(byte*)0x143d8025a = 0`. With it ON, the sample shows `OPRNAM` = cycle name and `SIMNAM` =
`CYCL<k>-<member name>` (e.g. `CYCL1-Forming (2)` .. `CYCL9-Forming (2)`, SIMULATION 2..10).

CycleData (cp+0x38, 0x10 bytes, ctor `14134ef10`): `+0 int count` (default 5; set from the
flow chart `DiagramItem/ItemData CycleCount="9"`, s1-01), `+8` list (`14015d170`, unused here).

Members `1400b7470(sub, cycleId)`: `diag = *sub` (`1400b5a70`); `g = 140084f00(diag, cycleId)` =
the diagram item whose virtual +0x28 (id) == cycleId; `set = 140085470(diag, g)` = items with
`(item->+0x8()->+0x58() & 0x1000) == 0` (not a group box) whose cell `(row,col)` (`+0xc8`) lies
inside g's rectangle `[(+0xd8),(+0xe8)]` (moproj `GroupInfo BeginRow/BeginCol/EndRow/EndCol`,
lexicographic compare), sorted with `140084a50`; `order = 140084ad0(diag)` = all non-group items in
flow order (take the first sorted item, walk to the root with diagram vfunc +0x138 = predecessor,
then forward with +0x130 = successor); result = ids (`+0x28`) of the items of `order` that are in
`set`, in that order. The main walk (`140131df0` → `1400b6630`, s1-02) must leave cycle members
out (sample: op 2 appears only inside the cycle).

Nested writers called from here do not get `curItem` set (O-3). The member counter `*sim` is the
shared one, so numbering continues.

## 3. SfOprDoeOutput: `140e32970`, SfOprGroup: `141352540`

Both are `mov [rsp+8..0x20], rcx..r9; ret` (bytes `4c894c2420 4c89442418 4889542410
48894c2408 c3`, no .pdata): nothing is written and `*sim` is unchanged. DoeOutput's sim-count
virtual +0x298 `140e32990` is empty too. Sample: BEND_2D has a `SfOprDoeOutput` operation
(`SelecedOpr`, `SelectedObj` properties) that is not in the flow chart and produces no block.
Group members are not written by `141352540`; whether the main walk visits them is s1-02's
(`1400b6630`), see O-4.

## 4. SfOprConvert2Dto3D: `1401c4440` → `1401c4520`

No corpus project. `1401c4520` has a 0x4e00-byte frame (`__chkstk` `1417f5860`).

```
1401c4440(self, s, prj, db, sim, item, a7, a8):
    props = 1405ef320(self, item);
    cp = dynamic_cast<SfOprConvert2Dto3DProperties*>(props);   // TD 0x1435d68a8
    if (cp) { cp->vfunc_0x60();  /* 1401ce590, O-5 */  1401c4520(self, s, prj, db, sim, item, cp, a7, a8); }

1401c4520(self, s, prj, db, sim, item, cp, dbreadArg, onlySim):
    is2D  = 14005f150(item);                       // unused
    first = 14005f230(item);
    DB_STEP_DATA tmp(3);                           // 140821bf0  (twin DEF_PRE 1401c8950)
    Cls_1401e7b10 sc;                              // 1408d3bc0  (sim-control block ctor)
    Cls_14018b220 mv;                              // 14085f3f0  (movement block ctor, unused)
    1408d3cd0(&sc, &tmp);                          // owner (twin 1401e7c10)
    1408e0200(&sc, 140054ff0(item), 0);            // sc+0xfc = unit (twin 1401f40b0)
    cp->vfunc_0x70(&sc);                           // base 1405fb6a0: +0x80,+0x90,+0xa0,+0xb0 fill sc
    QString unused1;
    QString base = convertSeparators(QFileInfo(db).baseName(1));   // unused
    QString p23;
    QString cnv = 1401c6e00(self);                 // = cleanPath("DEF_CNV23.DAT")  (see note)
    1407fbc10(cnv.toLocal8Bit());                  // DELETE ./DEF_CNV23.DAT   (twin 1401b85d0)
    1407fbc10("DEF_REM.INI");                      // DELETE ./DEF_REM.INI
    1407fbc10("REMESH.SCR");                       // DELETE ./REMESH.SCR
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        E_140131c10(s, *sim, item);
        if (!first) E_1401343e0(s, db, *sim, dbreadArg);          // DBREAD
        else        E_140134bf0(s, prj, db, *sim, item);          // MDSOBJ/TNOW/KFREAD SimCtrl
        E_140134db0(s, prj, db, *sim, item);
        E_140135220(s, prj, db, *sim, item, 0);
        QString m23 = 1401c68c0(self);             // cleanPath(taskAbs(curItem) + SEP + "M23_INPUT.KEY")
        m23 = REL(m23);
        s << "KFREAD" << endl;                     // 142203144 (no tab, no "1")
        s << cleanDirPath(m23) << endl;
        s << "KFREAD" << endl;                     // 14220314c
        p23 = 1401c6a70(self);                     // =1401c6f20: cleanPath(taskAbs(curItem) + SEP + "SfOprConvert2Dto3D_TEMP_STEP.KEY")
        p23 = REL(p23);
        s << cleanDirPath(p23) << endl;
        E_140134730(s, 140050ac0(item));           // OPRNAM
        E_1401347e0(s, 140050ac0(item));           // SIMNAM
        E_140134900(s, *sim, 140050b20(item), 0);  // CURSIM
        s << "NSTEP" << "\t" << "0" << endl;       // 142203154, 1421ffab4, 1421ffae4
        E_140134a10(s, db, *sim, 2, (byte)1);      // GENDB
    }
    QString st = 1401c6c50(self);                  // cleanPath(taskAbs(curItem) + SEP + "CONV_STATUS.DAT")
    st = QFileInfo(st).fileName();                 // 140818f10 -> "CONV_STATUS.DAT"
    1402cf3c0(0x143d76700, "2signalSetProcessStatusFilename()", Q_ARG(QString, st), 9 x QGenericArgument(0,0));
    // destructors: mv (14085f580), sc (1408d3d70), tmp (140824560, twin 1401cb2c0)
```

Notes:
- `1401c6e00`, `1401c68c0`, `1401c6c50`, `1401c6f20` use `self->curItem` (`1405ef2b0(self)`), not
  `item`; with curItem null the directory part is the empty string (`1421ff74d/e/f` = `""`).
- `1401c6e00` builds `taskAbs` but then assigns `"DEF_CNV23.DAT"` over the result, so the deleted
  file is `DEF_CNV23.DAT` in the current directory (project directory), not in the task folder.
- `1407fbc10(path)` = `fopen(path,"rt")` (`14146fcd8`, mode at 0x14385aee4); if it opened:
  `fclose`, `return remove(path) == 0`; else `return 1`. The three deletions happen even when the
  block itself is skipped by `onlySim`. They are file-system side effects only; nothing written to
  the MST depends on them. The converter must not reproduce them (plan §2 guarantee).
- `140134a10` here gets byte 1 (Boolean/CopyMirror too); `140134680` is not called.

## 5. SfOprCopyMirror: `140e24e80` → `140e24f60`

No corpus project.

```
140e24e80(self, s, prj, db, sim, item, a7, a8):
    cp = dynamic_cast<SfOprCopyMirrorProperties*>(1405ef320(self, item));   // TD 0x1439c1d88
    if (cp) { cp->vfunc_0x60(); /* 140e2d230, same body as 1401ce590 */ 140e24f60(self, s, prj, db, sim, item, cp, a7, a8); }

140e24f60(self, s, prj, db, sim, item, cp, dbreadArg, onlySim):
    QString ini("mirrormerge.ini");                // 142abf798
    (void)14005f150(item); (void)140050bc0(item);
    QString nm; QFileInfo fi(db); QString base = convertSeparators(fi.baseName(1));  // unused
    QString outKey;
    ++*sim;
    if (onlySim == 0 || *sim == onlySim) {
        E_140131c10(s, *sim, item);
        E_1401343e0(s, db, *sim, dbreadArg);       // DBREAD
        E_140134bf0(s, prj, db, 1, item);          // 4th arg 1
        E_140134db0(s, prj, db, 1, item);
        E_1401360e0(s, prj, db, 1, item);
        E_140135220(s, prj, db, 1, item, 0);
        E_140134680(s, item);
        nm = 140050ac0(item);
        E_140134730(s, nm); E_1401347e0(s, nm);
        E_140134900(s, *sim, 140050b20(item), 0);
        1400ab7e0(ICB(item), 0);                   // GENCTC off
        E_140136040(s, prj, db, *sim, item);       // INICTC lines
        s << " * Keyword file which contains objects " << endl     // 142abf7a8
          << kw(10) << endl
          << P("Step.KEY") << endl << endl;        // 140e24ac0 -> 142abf788
        s << "EXECMD" << "\t" << "3" << endl;      // 142abf7d0, 142abf4cc, 142abf4dc
        s << "DEF_MIRRORMERGE.EXE" << "   "        // 142abf7d8, 142abf594
          << cleanDirPath(taskRel + SEP + ini)
          << "  >  "                                // 142abf7ec
          << cleanDirPath(taskRel + SEP + "DEF_MIRRORMERGE.OUT") << endl;   // 142abf7f8 (fromAscii)
        s << "KFREAD" << endl;                     // 142abf80c
        outKey = taskRel + SEP + "DEF_MIRRORMERGE_OUT.KEY";               // 142abf818
        s << cleanDirPath(outKey) << endl;
        nm = self->vfunc_0x88();                   // 140e27350: "Copy/Mirroring"; unused
        E_140134a10(s, db, *sim, 2, (byte)1);      // GENDB
        s << "ARMCTL\t0\t0\t" << *sim + 1 << endl; // 142abf830
    }
```

`mirrormerge.ini` is only named; it is written elsewhere (operation save, out of scope).

## 6. SfOprDistortion: `141362e90` → `141362f60`

No corpus project. `141362f60` has a 0x4dc0-byte frame.

```
141362e90(self, s, prj, db, sim, item, a7, a8):
    dp = dynamic_cast<SfOprDistortionProperties*>(1405ef320(self, item));    // TD 0x1439ee1b0
    if (dp) 141362f60(self, s, prj, db, sim, item, dp, a7, a8);       // no vfunc_0x60 call here

141362f60(self, s, prj, db, sim, item, dp, dbreadArg, onlySim):
    (void)14005f150(item);
    noPred = 140050bc0(item);
    QString stepKey("Step.KEY");                      // 142e23238, unused
    DB_STEP_DATA tmp(3); Cls_1401e7b10 sc; Cls_14018b220 mv;           // as in 4
    1408d3cd0(&sc, &tmp); 1408e0200(&sc, 140054ff0(item), 0);
    dp->vfunc_0x70(&sc);                              // base 1405fb6a0
    QString simnam; QString base = convertSeparators(QFileInfo(db).baseName(1));  // unused
    QString outKey;
    D = 1401cc6b0(dp);                                // *(dp+0x40): DistortionPassData (0x5e0 bytes)
    n = *(int*)(D+8);                                 // 140cbc690: number of passes
    if (n > 0)
    for (i = 0; i <= n + 1; ++i) {
        if (*(int*)(D+0xc) /*140cbc680: first pass to write*/ > i) continue;
        ++*sim;
        if (!(onlySim == 0 || *sim == onlySim)) continue;
        E_140131c10(s, *sim, item);
        s << endl << "* Operation No " << *sim;      // empty line first; 142e23248
        if (i == 0)          s << " Original workpiece";          // 142e23258
        else if (i == n + 1) s << " Unload";                      // 142e23270
        else                 s << " Pass No " << i;               // 142e23278
        s << endl;
        if (i == 0) {
            if (!noPred) E_1401343e0(s, db, *sim, dbreadArg);
            E_140134bf0(s, prj, db, *sim, item);
            E_140134db0(s, prj, db, *sim, item);
            E_1401360e0(s, prj, db, *sim, item);
            E_140135220(s, prj, db, *sim, item, 0);
            E_140135da0(s, prj, db, *sim, item);
            E_140135f10(s, prj, db, *sim, item);
            E_140136040(s, prj, db, *sim, item);
            E_140136090(s, prj, db, *sim, item);
        } else {
            E_1401343e0(s, db, *sim, dbreadArg);
            E_140134bf0(s, prj, db, i, item);         // 4th arg = pass index (checked in ins2.txt)
        }
        E_140132a40(s, item);
        E_1401322d0(s, item);
        gflag = 0; g4 = 2;
        if (i == n + 1)      stage = 1;
        else if (i == 0)   { stage = 2; s << "NSTEP\t0" << endl; gflag = 1; }   // 142e23288
        else {
            stage = 0;
            obj = 1400600a0(item, *(int*)(D + 0x3d8 + 4*(i-1)));   // 1413651f0: object id of pass i
            if (obj) {
                QString k = 14006ba60(obj);          // cleanPath(obj+0x28)
                if (!k.isEmpty()) {
                    k = REL(k);
                    s << "KFREAD    \t" << 1 << endl;     // 142e23290
                    s << k << endl;                       // not cleanDirPath'ed
                }
            }
        }
        simnam = 141363fc0(self, stage, i, /*bare*/0);
        E_140134680(s, item);
        E_140134730(s, 140050ac0(item));          // OPRNAM = operation name
        E_1401347e0(s, simnam);                   // SIMNAM
        E_140134900(s, *sim, 140050b20(item), 0);
        s << " * Keyword file which contains objects " << endl << kw(10) << endl   // 142e232a0
          << cleanDirPath(taskRel + SEP + QString().sprintf("Step%d.KEY", i)) << endl << endl;   // 141366850
        s << "EXECMD" << "\t" << "4" << endl;     // 142e21ab4, 142e1fc8c, 142e1fdfc
        s << "DEF_MACHDIST_CTRL23.EXE" << "   "  // 142e232c8, 142e1fe0c
          << cleanDirPath(taskRel + SEP + ("DEF_MACHDIST_CTRL2.INI" + "_" + QString::number(i, 10)))  // 141366c30
          << "  >  "                              // 142e232e0
          << cleanDirPath(taskRel + SEP + "DEF_MACHDIST_CTRL2.OUT") << endl;   // 142e232e8
        s << "KFREAD" << endl;                    // 142e23300
        outKey = taskRel + SEP + QString().sprintf("DEF_MACHDIST_OUT%d.KEY", i);   // 1413668e0
        s << cleanDirPath(outKey) << endl;
        E_140134a10(s, db, *sim, g4 /*2*/, (byte)gflag);
        if (onlySim) break;
    }
```

`141363fc0(self, stage, i, bare)`: `name = 140050ac0(self->curItem)` (curItem dereferenced
without a null check); `suf` = `QString::sprintf("PS%02d", i)` (stage 0) / `"UNLOAD"` (1) /
`"LOAD"` (2); returns `bare ? suf : name + suf` (no separator), e.g. `Machining DistortionPS01`.

DistortionPassData (dp+0x40, ctor `1413690c0`, filled from the operation's properties, s1-01):
`+0x04` int (`14142ff20`), `+0x08` int pass count, `+0x0c` int first pass written,
`+0x3d8` int[] object id per pass. `dp+0x58` (`1413665c0`) copies +4 and +8 into `*(dp+0x38)+0x10/0x14`
(not used by the writer). The sim-count virtual `1413642d0` counts all `n+2` stages and ignores
+0x0c, unlike the writer (O-8).

## 7. SfOprMultiBlow(2D/Common): `140602810` → `1406028b0`

Classes `SfOprMultiBlow`, `SfOprMultiBlow2D`, `SfOprMultiBlowCommon` share every slot used here
(+0x280 `140602810`, +0x118 `140601f30` = 1, +0x120 `140601f50` = 2, +0x190 `1405f5bf0`,
+0x298 `140606c10`). No corpus project. Handler fields (ctor `140601da0` → `140601e40`):
`+0x60 int = 1` (workpiece), `+0x64 int = 2` (top die), `+0x68 int = 3` (bottom die),
`+0x70 double = 1.0`, `+0x78/+0x80/+0x88 double = 0`, `+0x90 byte`. The writer rewrites
+0x70..+0x90 (state kept on the singleton).

### 7.1 Driver `1406028b0`

```
140602810(self, s, prj, db, sim, item, a7, a8):
    mp = 140608ea0(self, item);       // dynamic_cast<SfOprMultiBlowProperties*>(1405ef320(...)), TD 0x1437751e8
    if (mp) 1406028b0(self, s, prj, db, sim, item, mp, a7, a8);

1406028b0(self, s, prj, db, sim, item, mp, dbreadArg, onlySim):
    model = 14005f770(item); if (!model) return;           // O-5
    m = 140c7acd0(model);                                  // model+0xe0 (DB_STEP_DATA*)
    DB_STEP_DATA tmp(3); Cls_1401e7b10 sc; Cls_14018b220 mv;
    int dim = 140100980(m);                                // m+0x1c
    140821e40(&tmp, &dim);                                 // set_dimension (twin 1401c8ba0)
    1408d3cd0(&sc, &tmp); 1408e0200(&sc, 140054ff0(item), 0);
    mp->vfunc_0x70(&sc);                                   // base 1405fb6a0
    14085f540(&mv, &tmp);                                  // mv owner (twin 14018b370) + 14085f6d0
    14060def0(mp, &mv);                                    // mv from MultiBlowData (7.3)
    double ang = 0.0; int nextSim = 0;
    if ((sub = 1400a7390(prj)))                            // active sub-project
        nextSim = 1400b8820(sub, 140050b20(item) + 1);     // first SIMULATION of the next op (7.4)
    self->+0x70 = (140054ff0(item) == 1) ? 100.0 : 5.0;    // 1421de200 / 1421d3c60
    is2D = 14005f150(item);
    self->+0x78 = -self->+0x70 * mv.+0x50;                 // XOR 1421d3d30 = negate
    self->+0x80 = -self->+0x70 * mv.+0x58;
    if (!is2D) self->+0x88 = -self->+0x70 * mv.+0x60;
    QString eqp;
    if ((o = 140061250(item, self->+0x64 - 1))) {          // the top-die object of the operation
        QString p; int id = *(int*)(o+0x28);               // 14004de90
        if (id > 0) p = 1400a5970(140055030(item))->vfunc_0x208(&id);   // equipment KEY path (O-9)
        if (!p.isEmpty()) eqp = REL(p);                    // not cleanDirPath'ed
    }
    if (is2D) ang = 1408609b0(&mv);                        // atan2(mv+0x58, mv+0x50)*57.29577951308232 (twin 14018c810)
    self->+0x90 = 0;
    bool first = 1;                                        // S34b9, cleared by the first BL block
    np = 14060c6f0(mp);                                    // MBD+0x140: passes
    for (i = 1; i <= np; ++i) {
        nb = 14060c710(mp, i);                             // MBD+0x148 QList<int>[i-1] (0 if out of range)
        for (j = 1; j <= nb; ++j) {
            if (!1406030c0(self, s, prj, db, sim, item, dbreadArg, onlySim, mp, i, j, &first, &sc, &mv, eqp, nextSim)) break;
            if (!140604910(self, s, prj, db, sim, item, dbreadArg, onlySim, mp, i, j, &first, &mv, ang)) break;
            if (!140605830(self, s, prj, db, sim, item, dbreadArg, onlySim, mp, i, j, &first)) break;
        }                                                  // break leaves only the j loop
    }
```

### 7.2 Block writers (one call each per (pass i, blow j))

Literals not shown are `"\t"` (all separator strings in 1424acbd4..1424b0ad8 are the single byte
09). `T` below means `<< "\t" <<`. Objects: `W = self->+0x60`, `TD = self->+0x64`,
`BD = self->+0x68`. `cont(kind)` = `1406067e0(self, s, prj, db, *sim, item, kind, &first)`:

```
1406067e0(self, s, prj, db, simv, item, kind, firstp):
    rd = 1; if (firstp && *firstp) { rd = 0; *firstp = 0; }
    if (kind in {0x100010, 0x100000, 0x100020})
        s << "PDIE " T self->vfunc_0x118() /*1*/ << endl;
    else if (kind == 0x200010) {
        E_140134bf0(s, prj, db, simv, item); E_140134db0(...); E_140135220(..., item, rd);
        E_1401360e0(...); E_1401365f0(...);
        s << "PDIE " T self->vfunc_0x120() /*2*/ << endl;
        E_140135f10(...); E_140135da0(...);
    }
```

Names (`QString("BL%1-xx").arg(j, 0, 10, ' ')`, the pass index i is NOT used; the 6th arg 1
returns the bare form, 0 would prefix `<op name> + "-"`): `1406078f0` `BL%1-BL`, `140607b00`
`BL%1-DW`, `1406072c0` `BL%1-TI`, `1406074d0` `BL%1-HT`, `1406076e0` `BL%1-TO`.

**BL (blow) `1406030c0`** returns 0 only when `onlySim` hit:

```
is2D=14005f150(item); fA=MBD+0xa0 (14060bef0); f8a=MBD+0x8a; f8b=MBD+0x8b; f89=MBD+0x89;
++*sim; if (!(onlySim==0 || *sim==onlySim)) return 1;
E_140131c10(s,*sim,item); E_1401343e0(s,db,*sim,dbreadArg); cont(0x200010);
rot = 0;
if (j > 1) {
    if (fA && 14060ce60(mp,i,j-1) /*MBD+0x188[i-1][j-2]*/ && f8a) {
        s << "NDTMP " T W T 0 T MBD+0xa8 << endl;
        if (f8b) s << "STRAIN " T W T 0 T 0.0 << endl;
    }
    (void)14060bdd0(mp,i,j-1);
    if (f89) rot = 14060cfc0(mp,i,j-1);                       // MBD+0x190[i-1][j-2]
}
if (!is2D) {                                                  // 3D
    if (rot) {
        s << "* Rotate" << endl;
        R r = 14060d150(mp,i,j-1);                            // 7.3 rotation record
        if (!140609b10(&r)) s << "* Error : " << 140609b80(&r) << endl;
        else {
            s << "*    Rotate the workpiece: " << 1406091d0(&r) << endl;
            if (!r.selfCenter) s << "OBJPOS " T W T 3 T r.cx T r.cy T r.cz T r.ax T r.ay T r.az T r.angle << endl;
            else               s << "OBJPOS " T W T 6 T r.ax T r.ay T r.az T r.angle << endl;
        }
    }
    if (j != 1) {
        s << "* Positioning" << endl;
        s << (rot ? "*    Put the flipped Workpiece on the Bottom die" : "*    Put the Workpiece on the Bottom die") << endl;
        s << "OBJPOS " T W T 2 T 0.0001 T mv.+0x50 T mv.+0x58 T mv.+0x60 T BD << endl;
        s << "*    Put Top die on Workpiece" << endl;
        s << "OBJPOS " T TD T 2 T 0.0001 T mv.+0x50 T mv.+0x58 T mv.+0x60 T W << endl;
    }
} else {                                                      // 2D
    if (rot) {
        s << "* Flip" << endl
          << "*    Flip the workpiece with respect to a line perpendicular to the movement direction" << endl;
        s << "OBJPOS " T W T 4 T 0 T 0 T mv.+0x58 T mv.+0x50 << endl;
    }
    if (j != 1) {
        s << "* Positioning" << endl;
        s << (rot ? "*    Put the flipped Workpiece on the Bottom die" : "*    Put the Workpiece on the Bottom die") << endl;
        s << "OBJPOS " T W T 2 T 0.0001 T BD T mv.+0x50 T mv.+0x58 << endl;
        s << "*    Put Top die on Workpiece" << endl;
        s << "OBJPOS " T TD T 2 T 0.0001 T W T mv.+0x50 T mv.+0x58 << endl;
    }
}
s << "* Generate the contacts" << endl;
s << "INICTC " T W << endl;
s << "GENCTC " T 0.0001 << endl;
if (!eqp.isEmpty()) {
    s << "IMPEQP " T TD << endl << eqp << endl;
    d1 = 14060c810(mp,i,j) /*MBD+0x170[i-1][j-1]*/; d2 = 14060cb20(mp,i,j) /*MBD+0x178*/;
    if (!MBD+0x88) { s << "DOESCL " T d1 << endl; s << "MOVCTL " T TD T "1" << endl; }
    else           { s << "DOESCL " T d1/100.0 << endl; s << "MOVCTL " T TD << endl; }
    if (!MBD+0x8c) { s << "DOESCL " T d2 << endl; s << "MOVCTL " T TD T "2" << endl; }
}
s << "* TRANS will not be redefined in Blow operation." << endl << "* Definition in SimCtrl.KEY will be used." << endl;
s << "* Define next simulation number - if minimum die distance criteria met, jump to next operation" << endl;
s << "NEXTSIMNO" T 3 << endl;
s << *sim + 1 T *sim + 1 T nextSim << endl;
E_140134680(s,item); E_140134730(s,140050ac0(item)); E_1401347e0(s,"BL<j>-BL");
E_140134900(s,*sim,140050b20(item),0); E_140134a10(s,db,*sim,2,(byte)0);
return onlySim ? 0 : 1;
```

In the 2D rotate/Flip line the order is `+0x58` then `+0x50`. The two strings `"1"`/`"2"` after
MOVCTL are literals (1424b0660, 1424b0688), not numbers.

**DW (dwell) `140604910`**:

```
is2D=14005f150(item); self->+0x90 = 0;
on = 14060c280(mp,i,j);             // MBD+0x110 && dwell(i,j) > 0
dwell = 14060ccc0(mp,i,j);          // MBD+0x180[i-1][j-1]
heatOk = 14060bdd0(mp,i,j); fE8 = MBD+0xe8;
if (!on) return 1;
++*sim; if (!(onlySim==0 || *sim==onlySim)) return 1;
E_140131c10(s,*sim,item); E_1401343e0(s,db,*sim,dbreadArg); self->+0x90 = 1;
s << "* Positioning" << endl << "*    Move Top die away" << endl;
if (!is2D) s << "OBJPOS " T TD T 1 T self->+0x78 T self->+0x80 T self->+0x88 << endl;
else       s << "OBJPOS " T TD T 1 T self->+0x78 T self->+0x80 << endl;
s << "* Generate the contacts" << endl << "INICTC " T W << endl << "GENCTC " T 0.0001 << endl;
h = 14060c4e0(mp);                  // unit value of QString MBD+0x128, kind 0x20 (1402ede00)
s << "IHTCOF " T W T TD T 0 T h << endl;
s << "IHTCOF " T W T BD T 0 T h << endl;
if (!is2D) { s << "MOVCTL " T TD T 1 T 0 T 0 T 0 T -1 T 0 << endl;
             s << "MOVCTL " T BD T 1 T 0 T 0 T 0 T 1 T 0 << endl; }
else       { s << "MOVCTL " T TD T 1 T 0 T ang T 0 << endl;
             s << "MOVCTL " T BD T 1 T 0 T -ang T 0 << endl; }
if (heatOk && fE8) {
    a = MBD+0xf0; b = MBD+0xf8;     // if (b == 0.0) MBD+0xa8 is read and dropped
    s << "OTPRNG" T W T 1 T a T b T 0 << endl;
    s << "NEXTSIMNO" T 3 << endl;
    s << *sim + 4 T *sim + 1 T 0 << endl;
}
s << "DSMAX " T 0 << endl;
dt = 14060c3a0(mp);                 // unit value of QString MBD+0x120, kind 0
s << "DTMAX " T dt << endl;
n = 1; if (dt > 0.0) { x = dwell/dt + 0.5; n = (int)(x >= 1.0 ? x : 1.0); }
s << "NSTEP " T n << endl;
s << "* Turn off Deformation in Dwell" << endl;
s << "TRANS " T 1 T 0 T -1 T -1 T -1 T -1 << endl;
E_140134680; OPRNAM(op name); SIMNAM("BL<j>-DW"); CURSIM; E_140134a10(s,db,*sim,2,(byte)0);
return onlySim ? 0 : 1;
```

**TI/HT/TO (transfer/heat) `140605830`**:

```
ok = 14060bdd0(mp,i,j); fE8 = MBD+0xe8;
tTI = MBD+0xc8; tHT = MBD+0xb8; tTO = MBD+0xd0;
if (!ok) return 1;
if (tTI > 0 && (++*sim, onlySim==0 || *sim==onlySim)) {
    140605ee0(..., *sim, ..., tTI, dt=MBD+0xd8, env=MBD+0xb0, "BL<j>-TI", 0x100010, &first, ..., mp, 0, 0, 0);
    if (onlySim) return 0; }
if (tHT > 0 && (++*sim, ...)) {
    140605ee0(..., tHT, dt=MBD+0xc0, env=MBD+0xa8, "BL<j>-HT", 0x100000, &first, ..., mp,
              fE8, MBD+0x100, 1.1*MBD+0x100 /*1424797e0*/);
    if (onlySim) return 0; }
if (tTO > 0 && (++*sim, ...)) {
    140605ee0(..., tTO, dt=MBD+0xd8, env=MBD+0xb0, "BL<j>-TO", 0x100020, &first, ..., mp, 0, 0, 0);
    if (onlySim) return 0; }
return 1;

140605ee0(self, s, prj, db, simv, item, dbreadArg, onlySim, total, dt, env, name, kind, firstp, _, mp, otp, o1, o2):
    E_140131c10(s,simv,item); E_1401343e0(s,db,simv,dbreadArg); cont(kind);   // "PDIE \t1"
    if (!otp) s << "OTPRNG" T W T 0 T 0.0 T 0.0 T 1 << endl;
    else    { s << "OTPRNG" T W T 2 T o1 T o2 T 1 << endl;
              s << "NEXTSIMNO" T 3 << endl;
              s << simv + 1 T simv + 1 T 0 << endl; }
    s << "OBJTYP" T TD T 0 << endl;
    s << "OBJTYP" T BD T 0 << endl;
    s << "DSMAX " T 0 << endl;
    s << "DTMAX " T dt << endl;
    n = 1; if (dt > 0.0) { x = total/dt + 0.5; n = (int)(x >= 1.0 ? x : 1.0); }
    s << "NSTEP " T n << endl;
    s << "ENVTMP" T 0 T env << endl;
    if (kind == 0x100000 && MBD+0x8b) s << "* Initialize strain" << endl << "STRAIN " T W T 0 T 0.0 << endl;
    s << "* Generate the contacts" << endl << "INICTC " T W << endl << "GENCTC " T 0.0001 << endl;
    s << "* Turn off Deformation in Heating" << endl;
    s << "TRANS " T 1 T 0 T -1 T -1 T -1 T -1 << endl;
    E_140134680; OPRNAM(op name); SIMNAM(name); E_140134900(s,simv,140050b20(item),140050b10(item));
    E_140134a10(s,db,simv,2,(byte)0);
```

### 7.3 MultiBlowData MBD = mp+0x38 (0x1d8 bytes, ctor `140618d20` from `14060b5d0`)

Filled from the operation's `<Properties>` through the Qt slots of SfOprMultiBlowProperties (qt_metacall `14061ab80`, set* slots; P14 decompiles, O-6 resolved). Property names below in brackets; ctor defaults `140618d20`.

| Off | Type | Accessor | Use |
|---|---|---|---|
| +0x09 | byte | | hammer = !(+0x0a) [Equipment, `140612de0`]; `14060def0`: 1 → hammer fields, 0 → screw fields |
| +0x10 | QString | | [EquipName] copied into mv+0 (`strcpy`) |
| +0x20,+0x28,+0x30,+0x38 | | | hammer [HammerEnergy, HammerMass, HammerEfficiency, int HammerCounterblow]: mv+0x1f0,+0x1e8,+0x1f8, int mv+0x200, mv+0x68 = 2 |
| +0x40..+0x58 | double×4 | | screw [ScrewEnergy, ScrewEfficiency, MomentOfInertia, LeadScrewPitch]: mv+0x280..+0x298, mv+0x68 = 3 |
| +0x68,+0x70,+0x78 | double | | movement direction → mv+0x50,+0x58,+0x60 (OBJPOS/ang/+0x78..) |
| +0x88 | byte | `14060bb10` | DOESCL in percent [UseRelativeEnergy, default 1] |
| +0x89 | byte | `14060bc90` | rotate/flip enabled [UseWorkpieceRotation] |
| +0x8a | byte | `14060bce0` | NDTMP enabled (also blocks `14060bdd0`) [InitTemp] |
| +0x8b | byte | `14060bd30` | STRAIN reset [InitStrain] |
| +0x8c | byte | `14060bd80` | skip 2nd DOESCL/MOVCTL [UseEfficiencyFunctionData] |
| +0xa0 | byte | `14060bef0` | heating on |
| +0xa8 | double | `14060bf40` | NDTMP temperature / HT ENVTMP |
| +0xb0 | double | `14060bf90` | TI/TO ENVTMP |
| +0xb8 | double | `14060bfb0` | HT total time |
| +0xc0 | double | `14060c000` | HT dt |
| +0xc8 | double | `14060c050` | TI total time |
| +0xd0 | double | `14060c0a0` | TO total time |
| +0xd8 | double | `14060c0f0` | TI/TO dt |
| +0xe8 | byte | `14060c140` | OTPRNG on |
| +0xf0,+0xf8 | double | `14060c190`,`14060c1e0` | OTPRNG values (DW) |
| +0x100 | double | `14060c230` | HT OTPRNG value (and ×1.1) |
| +0x110 | byte | `14060c300` | dwell on |
| +0x120 | QString | `14060c3a0` | dwell DTMAX (unit string, kind 0) |
| +0x128 | QString | `14060c4e0` | IHTCOF value (unit string, kind 0x20) |
| +0x140 | int | `14060c6f0` | passes |
| +0x148 | QList<int> | `14060c710` | blows per pass |
| +0x170 | QList<QList<double>> | `14060c810` | DOESCL 1 |
| +0x178 | QList<QList<double>> | `14060cb20` | DOESCL 2 |
| +0x180 | QList<QList<double>> | `14060ccc0` | dwell time |
| +0x188 | QList<QList<bool>> | `14060ce60` | [BlowReheat] per blow (NDTMP before the next blow, heating test) |
| +0x190 | QList<QList<bool>> | `14060cfc0` | [BlowFlip] per blow |
| +0x198..+0x1d0 | 8 lists | `14060d150` | rotation record R (angle, selfCenter, cx, cy, cz, ax, ay, az) |

Other fields [names]: +0xa0 UseHeating, +0xa8 HeatingTemperature, +0xb0 RoomTemperature (20, English 68),
+0xb8 HeatingTime (3600), +0xc0 HeatingTimeStep (20), +0xc8 TransferInTime (10), +0xd0 TransferOutTime (10),
+0xd8 TransferTimeStep (1), +0xe8 UseAdaptiveReheat, +0xf0 TriggerHeatTemp, +0xf8 TriggerHeatTempUpperLimit,
+0x100 HeatStopTemp, +0x110 UseDwell (1), +0x118 DefaultDwellTime (1), +0x120 DwellTimeStep ("0.1 sec"),
+0x128 DwellHeatCoeff ("1.0 N/sec/mm/C" / "0.0003 Btu/sec/in^2/F"), +0x140 NumHeats (`1406140c0`), +0x148 NumBlows
per heat (`140615190`), +0x150 Temperature per heat (100), per blow +0x170 BlowEnergy (100), +0x178
BlowEfficiency (1), +0x180 BlowDwellTime (1), +0x198 BlowRotAngle, +0x1a0 BlowRotSelfCenter (1), +0x1a8..+0x1b8
BlowRotCenterX/Y/Z, +0x1c0..+0x1d0 BlowRotAxisX/Y/Z. Tree `140612150`: root MultiBlowForging2D/3D with groups
EquipmentType, EquipmentSpec, MovementSpec, Init, Heating, AdaptiveReheat, Dwell, Interobject, MultiBlow, then
Heat nodes (Temperature, NumBlows, Blow nodes); all heats share the name "Heat" and all blows "Blow".
Port: P14 `src/ops/multiblow_props.cpp`.

Index rule for the list accessors: `(i < 1 || i > outer.count()) → 0/false`, then
`(j < 1 || j > inner.count()) → 0/false`, else `outer[i-1][j-1]`.

`14060bdd0(mp,i,j)` = `MBD+0xa0 && (14060ce60(i,j) || MBD+0xe8) && (tTI>0 || tHT>0 || tTO>0) && !MBD+0x8a`.
`14060c280(mp,i,j)` = `MBD+0x110 && 14060ccc0(i,j) > 0`.

Rotation record R (`140609140`: all 0, selfCenter = 1). `140609b10(R)` = `angle != 0 &&
!(ax==0 && ay==0 && az==0)`. `140609b80(R)` (error text) = `(angle==0 ? "Rotation angle is zero" : "")
+ (axis zero ? " " + "Rotation axis is not defined" : "")`. `1406091d0(R)` (text):
`angle==0 → tr("undefined")`, else `tr("Angle %1").arg(angle,0,'g',-1,' ') + "," + 140609690(R) +
"," + (selfCenter ? tr("Self Center") : tr("center (%1,%2,%3)").arg(cx).arg(cy).arg(cz))` (all `'g', -1`).
`140609690(R)`: axis all zero → `"undefined axis"`; only x nonzero → `"+X axis"`/`"-X axis"`
(`x <= 0` → minus); only y → `"+Y axis"`/`"-Y axis"`; only z → `"+Z axis"`/`"-Z axis"`; else
`"axis (%1,%2,%3)"` with `.arg(ax).arg(ay).arg(az)`.

### 7.4 nextSim `1400b8820(sub, ord)`

`recs = 1400b8360(sub)`: walk the flow list `1400b6630(sub)` (s1-02); for each op at position
p (1-based) call `handler->vfunc_0x298(handler, p, &cnt, item, &recs)`; each adds one record per
SIMULATION it will write (`1400683f0`: +0x8 op position, +0xc sim number; also `1408100e0`
reads step data of that sim from the open DB `cdab70(...)` for fields not used here).
Result: `+0xc` of the first record with `+0x8 == ord`; if none and `ord > 1`: last record of
`ord-1` (`1400b8540`) `+0xc + 1`; else 0. Per-class +0x298: base `1405f5880` (1 record),
Cycle `14134e280` (count × members), MultiBlow `140606c10`, Distortion `1413642d0` (n+2),
DoeOutput `140e32990` (none), and `14013fbb0`, `14029a4b0`, `1402b7380`, `140759d50`,
`1411b0170`, `1411b2790`, `1412b05c0`, `141423390` for the other families (O-10).
`140068210` replaces the op position by the global `0x143d7b7a4` when byte `0x143d7b7a8` is set
(DOE).

## 8. File and process side effects during MST generation

| Writer | Kind | Path | Effect on MST |
|---|---|---|---|
| Boolean `1411e1820` | WRITE | `taskAbs\Boolean.BWZ` | none (round trip of BooleanData) |
| Boolean `1411e0e40` | WRITE | `taskAbs\DEF_MACHDIST_CTRL2.INI` | none |
| Boolean (open, `1411e1dd0`) | READ | `taskAbs\Boolean.BWZ` | workpiece no → INICTC flag |
| Convert2Dto3D `1407fbc10` ×3 | DELETE | `./DEF_CNV23.DAT`, `./DEF_REM.INI`, `./REMESH.SCR` | none |
| Convert2Dto3D `1402cf3c0` | signal | `0x143d76700`, "2signalSetProcessStatusFilename()" + "CONV_STATUS.DAT" | none (see below) |
| MultiBlow `1400b8820` → `1408100e0` | READ | open project DB (via `cdab70`) | none for NEXTSIMNO |
| Convert2Dto3D/CopyMirror vfunc_0x60, MultiBlow `14005f770` | model | may create the operation model (`1405f6410`, `140c7a5b0`) and sets `handler->curItem` | none |

No `OutputGeoOp%1Obj%2.GEO` or KEY file is written by these eight writers (that GEO path belongs to
s1-02's `140132bd0`). The DB_STEP_DATA / Cls_1401e7b10 / Cls_14018b220 temporaries in
Convert2Dto3D, Distortion and MultiBlow are filled only from properties (`1405fb980`, `1405fbd10`,
`1405fc060`, `1405fc5d0` write sc+0x16c/+0x164/+0x1d0/+0x3d0/+0x4c8 and `1405fd4b0`, using
`1402ede00` unit evaluation) and destroyed; no output depends on them except, in MultiBlow,
mv+0x50..+0x60 which `14060def0` overwrites from MBD afterwards.

`1402cf3c0(obj, name, a0..a9)`: unless `obj+0x141` (blocked), looks up the receivers registered
under `QString(name)` in `obj+0x88` (`1402d03f0`), drops dead ones, and for each live receiver
calls `QMetaObject::invokeMethod(recv, receiverMethodName(ascii), Qt::DirectConnection, a0..a9)`.
It only updates UI state (process status file name); it writes nothing to the MST stream and
does not change the model the MST writers read (not proven for every possible receiver, O-2).

## 9. DEF_PRE twins (port on origin/main)

Masked byte match of the GUI function against `DEF_PRE_64.exe` (`scripts_p673/img`), then the
port's `@src` notes:

| GUI | DEF_PRE | Port |
|---|---|---|
| `1407fbc10` delete-if-exists | `1401b85d0` | noted in `defcmd/.../p67_5_gensub.hpp`, `geoprim/builder.hpp` |
| `140821bf0` DB_STEP_DATA(dim) | `1401c8950` | `DB_STEP_DATA` (`defmodel`, used in `defcmd/src/cnvt3d/driver.cpp`) |
| `140824560` ~DB_STEP_DATA | `1401cb2c0` | idem |
| `140821e40` set_dimension | `1401c8ba0` | `DB_STEP_DATA::set_dimension` |
| `1408d3cd0` sim_ctrl owner+init | `1401e7c10` | `defmodel/layout/cls_1401e7b10.hpp` |
| `1408d3bc0`/`1408d3d70` | ctor/dtor of Cls_1401e7b10 (`1401e7b10`/`1401e7cb0`, by layout; bytes differ by a global) | idem |
| `1408e0200` set unit (+0xfc) | `1401f40b0` | not ported |
| `14085f540` movement owner | `14018b370` | `defmodel/llobj_ctor/obj_defaults.cpp` |
| `14085f3f0`/`14085f580` | Cls_14018b220 ctor/dtor (`14018b220`, Phase 2 not ported) | `defmodel/llobj/cls_14018b220.hpp` |
| `1408609b0` movement angle | `14018c810` | `defkey/src/kw_g2obj/handlers.cpp` |

`1409068f0` (BWZ token reader) has no byte twin; `140906740`/`140906760` match DEF_PRE
`140411920`/`140411940`.

## 10. Validation (sample copies only)

`python -I tools/validate.py samples/MO2_Boolean_Oprations samples/MO2_Extrusion_and_Boolean_Cycles samples/MO2_Operation_Cycles2`:
7/7 Boolean writer tails (NSTEP .. GENDB + db) equal the spec, 14/14 side files (BWZ, INI) equal
the bytes 1411e1970/1411e0e40 produce from the BWZ values, 7/7 workpiece INICTC lines present,
9/9 cycle SIMNAM lines have the `CYCL<k>-<name>` form, 9 blocks for `CycleCount="9"`.
No samples for Convert2Dto3D, CopyMirror, Distortion, MultiBlow, Group; DoeOutput only as an
unwalked operation (BEND_2D).

## 11. Open questions

- O-1 Boolean/Distortion/CopyMirror/MultiBlow emit through s1-02 emitters with non-default
  arguments (`140134db0`/`1401360e0`/`140135220`/`140134bf0` with 4th arg `1` or the pass index;
  `140134a10` with byte 1 or 0). s1-02 should confirm the exact text for those arguments.
- O-2 Receivers of `"2signalSetProcessStatusFilename()"` on `0x143d76700` were not enumerated.
- O-3 Writers nested in a Cycle run with a stale `handler->curItem`: Convert2Dto3D's paths and
  Distortion's SIMNAM (`141363fc0`, null dereference if curItem is null) depend on GUI state.
  The converter needs a rule (proposal: treat curItem = item).
- O-4 How the main walk `1400b6630` treats Group and Cycle members (s1-02).
- O-5 `14005f770(item)` lazily creates the operation model (`14005f820` → handler vfunc +0xe0
  (base returns 0) → +0x1b0 `1405f6410`, `140c7a5b0`); whether that reads KEY files was not
  followed. MultiBlow writes nothing when it returns null.
- O-6 Mapping from moproj `<Properties>` names to MBD / DistortionPassData / CycleData fields
  (properties reader and QObject properties) is not in this packet; s1-01 or a follow-up must give
  it before MultiBlow/Distortion can be ported data-driven.
- O-7 3D Boolean INI block: getters `141344d40`, `1411634e0`, `1411634a0`, `14042fb50`,
  `14086dde0`, `1411634b0`, `140da76e0`, `1411634c0` on the zeroed struct not decompiled.
- O-8 Distortion: `1413642d0` counts n+2 blocks, the writer skips passes before MBD+0xc; affects
  only a later MultiBlow's NEXTSIMNO.
- O-9 Equipment path accessor `1400a5970(prj)->vfunc_0x208(&id)` (class built by `1400beca0`)
  not resolved; probably the same accessor as s1-02's IMPEQP emitter `140136810`.
- O-10 NEXTSIMNO needs every class's +0x298; only base, Cycle, MultiBlow, Distortion, DoeOutput
  were read.
- O-11 Unit evaluation `1402ede00` (`0x143d76368`, kinds 0, 0x20; `1402edb70`, `1402ed750`) and
  `QString::arg(double,0,'g',-1)` / `tr()` without a translator: s1-06.
- O-12 Should the DLL also (re)write Boolean.BWZ / DEF_MACHDIST_CTRL2.INI? Not needed for a
  byte-identical MST (they already exist and match), but the original Save does write them.
