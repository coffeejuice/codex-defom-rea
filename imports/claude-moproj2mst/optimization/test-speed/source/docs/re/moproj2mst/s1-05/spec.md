# s1-05 spec: FormingExpress(2D), Diestress family, HeatTreat(2D), Report, ShapeOptim, DOE path

Static evidence only (Ghidra decompiles in `s1-05/d*.c`, annotated copies `d*s.c` with the .rdata
literals inlined by `annot.py`; disassembly listings `l*.txt` via `ListAt.java`). Addresses are VAs in
`DEF_GUI_MO_64.exe`. Shared emitters `140131c10`..`140136810`, the base writer `1405f5340`, the MST top
level and the Qt-path helpers `140819590`, `140818d50`, `1400a4f60` belong to s1-02 and are referenced
by address only.

## 0. Conventions

* `S` is the `QTextStream` on the MST file (`QFile` opened `WriteOnly`, no `Text` flag, so `nl` = `"\n"`
  only, written by `FUN_140041bc0(S, endl)`).
* `S << "lit"` = `QTextStream::operator<<(const char*)`, bytes exactly as quoted (C escapes: `\t` = 0x09).
* `S << i(x)` = `operator<<(int)`, plain decimal, stream defaults (no width, no padding).
* `S << d(x)` = `operator<<(double)` with QTextStream defaults (SmartNotation, precision 6). The
  formatting rules are s1-06's; nothing below changes stream settings.
* `S << q(str)` = `operator<<(const QString&)`.
* Emitters by address mean "call s1-02's spec of that function with these arguments".
* `isFirstOp(op)` = `140050bc0(op)`: `140086720(140055150(op), *(int*)(op+0x108)) == -1`.
* `is2D(op)` = `14005f150(op)`: `140054e90(op)->vfunc[0x190]() == 2`.
* `opProps(this, node)` = `1405ef320(this, node)`: `node ? node : *(this+0x20)`, then `*(node+0x10)`
  (`140051230`). The result is an `SfOprBaseProperties*`; every writer below `dynamic_cast`s it
  (`14146f8fc` = `__RTDynamicCast`, src type `SfOprBaseProperties` at `143579b60`).
* Operation numbers in `CURSIM` are `140050b20(op)` (flow-chart position); the 4th CURSIM argument
  is `140050b10(op)`, which always returns 0.

### Writer signature (vtable slot +0x280)

```cpp
void SfOprXxx::writeMst(QTextStream& S, Project* proj, const QString& db, int* sim,
                        OpNode* op, int dbreadArg, int target);
```
Called from `140132100` (plain MST) and `1401321a0` (DOE sample); both pass `dbreadArg = 0`,
`target = 0` (from `140131df0`). `target != 0` is used only by the single-operation path `1400a2200`
(`DEF_MULTI.INI`), but the logic is specced in full. `*sim` is the running simulation number of the
sub-project (starts at 0 in `140131df0`, every block pre-increments it).

### Resolved virtuals (from `vtables.tsv`)

| Class | +0x280 writer | +0x200 | +0x288 | +0x298 (sim list) |
|---|---|---|---|---|
| SfOprFormingExpress | `1414136b0` | `1413e5ef0` (true) | `1405f56c0` | `141423390` |
| SfOprFormingExpress2D | `141412c90` | `1413e5ef0` (true) | `1405f56c0` | `141423390` |
| SfOprDiestress | `1402b5410` | `1405f79b0` (false) | `1405f56c0` | `1402b7380` |
| SfOprDiestressBM2D | `14029ad10` | `1405f79b0` | `1405f56c0` | `14029a4b0` -> `1405f5880` |
| SfOprDiestressBM3D | `14029cb50` | `1405f79b0` | `1405f56c0` | `14029a4b0` -> `1405f5880` |
| SfOprHeatTreat | `1411b1460` | `1413e5ef0` | `1405f56c0` | `1411b2790` |
| SfOprHeatTreat2D | `1411aee40` | `1413e5ef0` | `1405f56c0` | `1411b0170` |
| SfOprReport | `1412b05a0` | `1405f79b0` | `1405f56c0` | `1412b05c0` (empty) |
| SfOprShapeOptim | `14013fb90` | `1405f79b0` | `1405f56c0` | `14013fbb0` (empty) |

Properties classes reached by the casts: `SfOprFormingExpressProperties` (type desc `1439e12d0`),
`SfOprDiestressProperties` (`1436155e0`), `SfOprDiestressBMProperties` (`1435fb1a8`),
`SfOprHeatTreatProperties` (`143a5b500`; used by both HeatTreat classes). Slot +0x70 of the props
object: FormingExpress/Diestress/DiestressBM/Report/ShapeOptim props `1405fb6a0`, HeatTreat props
`1411c05a0`.

---

## 1. SfOprReport `1412b05a0`, SfOprShapeOptim `14013fb90`

Both bodies are `mov [rsp+20h],r9; mov [rsp+18h],r8; mov [rsp+10h],rdx; mov [rsp+8],rcx; ret`
(bytes checked in the exe copy). They write nothing and do not touch `*sim`. Their sim-list hooks
(+0x298 `1412b05c0`, `14013fbb0`) are also empty, so they consume no simulation numbers anywhere.

```cpp
void SfOprReport::writeMst(...)     {}   // 1412b05a0
void SfOprShapeOptim::writeMst(...) {}   // 14013fb90
```
Sample: none in the corpus. A test project needs one Report (or ShapeOptim) operation between two
ordinary operations; expected effect: the operation contributes no lines and the following
operation's `SIMULATION`/`CURSIM` numbers are not shifted.

---

## 2. SfOprDiestress `1402b5410`

```cpp
void SfOprDiestress::writeMst(S, proj, db, sim, op, dbreadArg, target) {
  auto* props = dynamic_cast<SfOprDiestressProperties*>(opProps(this, nullptr)); // this's own node
  SubProject* sp = 140055050(op);           // 1400a7390(project of op) = current sub-project
  QString dbPath; 1400b9e80(sp, &dbPath, true);  // type==4 ? 1400bbee0(sp,&,1) : 1400b9df0 -> 1400a4f60(project,&,1)
  QString key = dbPath;
  key.replace(QString(".DB"), QString(".KEY"), Qt::CaseInsensitive);  // all occurrences
  if (!props) return;
  ++*sim;
  if (target == 0 || *sim == target) {
    140131c10(S, *sim, op);           // SIMULATION header
    1401344a0(S, key);                // "KFREAD    \t" << 1 << nl; << q(key) << nl
    140134a10(S, dbPath, *sim, 2, false);   // GENDB / KFWRIT
  }
}
```
Literals: `".KEY"` @`1422ad0bc`, `".DB"` @`1422a9a54`; `1401344a0` uses `"KFREAD    \t"` @`1421acb68`.
No DBREAD, OPRNAM, SIMNAM or CURSIM. Note `dbPath` is the sub-project DB path built by
`1400b9e80(...,true)` (with directory), not the `db` argument; the KEY name is not relativised.
Model fields: only the props pointer is null-checked; sub-project +0x18 (type), DB name fields read by
`1400b9df0`/`1400bbee0` (+0x38 cached name, `1400bc290`, `1400bc160`).

Sample: none. Test project: one Diestress operation after a forming operation; check both a project
whose DB name has upper-case `.DB` and one with `.db` (case-insensitive replace).

---

## 3. SfOprDiestressBM2D `14029ad10`, SfOprDiestressBM3D `14029cb50`

The two functions are byte-for-byte the same decompile (literal addresses differ, contents equal:
`DEFFRZ` @`14227b6a4`/`142282904`, `\t`, `OBJTYP` @`14227b6ac`/`14228290c`,
`"Die Stress Simulation"` @`14227b6b8`/`142282918`, `"/"` @`142278a8c`/`14227fcec`).
`SfOprDiestressBM` itself uses the base writer `1405f5340`.

```cpp
void SfOprDiestressBMxD::writeMst(S, proj, db, sim, op, dbreadArg, target) {
  auto* p = dynamic_cast<SfOprDiestressBMProperties*>(opProps(this, nullptr)); // this's node
  if (!p) return;
  int nObj = 14005fa80(op);                 // count(op+0x40)
  if (nObj <= 0) return;
  int* ids = new int[nObj];                 // operator new(4*nObj)
  int nDie = 14029a550(this, ids);          // see below
  ++*sim;
  if (target == 0 || *sim == target) {
    140131c10(S, *sim, op);
    1401343e0(S, db, *sim, dbreadArg);      // DBREAD
    140134bf0(S, proj, db, *sim, op);
    140134db0(S, proj, db, *sim, op);
    1401360e0(S, proj, db, *sim, op);
    140135220(S, proj, db, *sim, op, false);
    140135da0(S, proj, db, *sim, op);
    140132a40(S, op);                        // DOE geometry vars (section 7.4)
    1401322d0(S, op);                        // DOE variables   (section 7.3)
    140135f10(S, proj, db, *sim, op);
    140136040(S, proj, db, *sim, op);
    for (int k = 0; k < p->nObjTotalNum /*+0x38*/; ++k)
      if (p->bInterp[k] /*byte +0x3c+k*/ == 1) {
        int a = p->nObjNo[k] /*int +0x4c+4k*/, b = p->nRefNo[k] /*int +0x88+4k*/;
        double tol = p->dTol[k] /*double +0xc8+8k*/;
        if (a != b && b > 0)
          S << "DEFFRZ" << "\t" << i(a) << "\t" << i(b) << "\t" << d(tol) << nl;
      }
    for (int k = 0; k < nDie; ++k)
      S << "OBJTYP" << "\t" << i(ids[k]) << "\t" << i(0) << nl;
    140134680(S, op);
    140134730(S, op->name /*140050ac0: QString op+0x118*/);          // OPRNAM
    1401347e0(S, QString("Die Stress Simulation"));                  // SIMNAM
    140134900(S, *sim, 140050b20(op), 0);                             // CURSIM
    SubProject* sp = 140055050(op);
    QString dir;  1400a4220(1400b5c40(sp) /*project*/, &dir);  // cleanPath(QFileInfo(project+0x18).absolutePath())
    QString dbOut; 1400b9e80(sp, &dbOut, true);
    if (p->bUseNewDb /*byte +0x148*/) {
      dbOut = dir + "/" + p->qsNewDbName /*QString +0x140*/;        // 140002110 then 1400048f0
      *sim = 1;                                                       // resets the running counter
    }
    140134a10(S, dbOut, *sim, 2, false);                              // GENDB
  }
  delete[] ids;
}

int 14029a550(SfOprBase* self, int* out) {     // die objects of the op
  int n = 0;
  OpNode* node = 1405ef2b0(self);              // *(self+0x20)
  if (!node) return 0;
  for (int k = 0; k < 14005fa80(node); ++k) {
    OpObject* o = 140061250(node, k);          // op object k (list node+0x50)
    if (!o) continue;
    int id = *(int*)o;                         // 14004d560
    int type = *(int*)(o + 4);                 // 14004d5a0
    if (type == 5 || type == 1) { out[n++] = id; qDebug() << "set WP No = " << id << "   Type = " << type; }
  }
  return n;
}
```
The emitters after `SIMULATION` get `*sim` as 4th argument. `*sim = 1` before GENDB is a real side
effect on the counter (later operations continue from 1).

Props field origin: the BM props block at props+0x38 is read/written as XML file
`<taskDir>/DieStressBmWizard.Wzd` (`14029eef0`: `140050df0(node)` + sep + `"DieStressBmWizard.Wzd"`,
`convertSeparators`). Writer `1402a16c0` (slot +0xb8) shows the mapping: root `DieStressBmWizard`
attrs `nObjTotalNum` (+0x38), `qsNewDbName` (+0x140), `bUseNewDb` (+0x148); per object child element
named `"ObjId" + char('0'+k/10) + char('1'+k%10)` with `nObjNo` (+0x4c+4k), `nRefNo` (+0x88+4k),
`bIntepolate` (+0x3c+k), `dTolerance` (+0xc8+8k). Reader is slot +0xc0 (`14029eea0` -> `1402a1df0`,
not decompiled). So the open path must read that `.Wzd` file (a file READ, s1-01/s1-07).

Sample: none. Test project: a DiestressBM2D and a DiestressBM3D operation with (a) two die objects of
types 1/5 and one workpiece, (b) one object with interpolation on and `nRefNo != nObjNo`, (c) a second
project with "use new DB" on (checks `*sim = 1` and the GENDB name).

---

## 4. SfOprHeatTreat `1411b1460`, SfOprHeatTreat2D `1411aee40`

Both entries: `p = dynamic_cast<SfOprHeatTreatProperties*>(opProps(this, op)); if (p)
body(this,S,proj,db,sim,op,p,dbreadArg,target)` with body `1411b1530` (3D) / `1411aef10` (2D). The two
bodies decompile identically (only label addresses differ; checked by diff), same literals.

Frame check: body prologue `push rbx; sub rsp,4c10h`, so `[rsp+4c28]`=S, `4c30`=proj, `4c38`=db,
`4c40`=sim, `4c48`=op, `4c50`=p, `4c58`=dbreadArg, `4c60`=target (Ghidra's offsets are 8 lower).

```cpp
void HeatTreat_body(this, S, proj, db, sim, op, SfOprHeatTreatProperties* p, int dbreadArg, int target) {
  bool is2d = is2D(op);                              // stored, not used for output
  bool firstOp = isFirstOp(op) && target == 0;
  // engine scratch (no output): LLOBJ-like model tmp(3) (140821bf0), SimCtrl sc (1408d3bc0),
  // Movement mv (14085f3f0); sc.link(&tmp) (1408d3cd0); 1408e0200(&sc, unit(op)=140054ff0(op), 0);
  // p->vfunc[0x70](&sc) = 1411c05a0 (copies p+0x58 block into sc; may call 1411b3540)
  QFileInfo fi(db); QString unused = QDir::convertSeparators(fi.baseName());   // not used
  QString keyPath;
  HTSchedule* sch = *(p + 0x68);                     // 141681de0
  int n = sch ? count(sch + 0x28) : 0;               // 1411b0a70
  bool needEmitters = true;
  if (n > 0) {
    1411b71c0(sch, /*interactive=*/false);           // writes HT_Proc<k>.MOKEY side files, see 4.1
    for (int i = 0; i < n; ++i) {
      ++*sim;
      if (i >= *(int*)(sch + 0x20) - 1 && (target == 0 || *sim == target)) {   // 140514c40 = first step
        140131c10(S, *sim, op);
        if (!firstOp || i != 0) 1401343e0(S, db, *sim, dbreadArg);           // DBREAD
        S << nl << "* Operation No " << i(*sim) << nl;                       // @142c8eba0
        if (i == 0 || needEmitters) {
          needEmitters = false;
          // NOTE: 4th argument is the loop index i, not *sim (disassembly 1411b189e: R9D=[rsp+4b00])
          140134bf0(S, proj, db, i, op);  140134db0(S, proj, db, i, op);
          1401360e0(S, proj, db, i, op);  140135220(S, proj, db, i, op, false);
          140135da0(S, proj, db, i, op);  140135f10(S, proj, db, i, op);
          140136040(S, proj, db, i, op);  140136090(S, proj, db, i, op);
        }
        S << " * Keyword file which ECC data " << nl;                       // @142c8ebb0 (leading space)
        S << "KFREAD" << nl;                                                  // @142c8cd04, no count
        keyPath = 1411b38e0(sch, i);                  // <taskDir>\HT_Proc<i+1>.MOKEY (4.1)
        keyPath = QDir(140819590(db)).relativeFilePath(keyPath);
        S << q(QDir::cleanDirPath(keyPath)) << nl;
        HTStep* st = 1411b0a90(sch, i);               // list sch+0x28 [i] (creates one if out of range)
        if (st) {
          QString uk = *(QString*)(st + 0x118);       // 1412c4c80: user keyword file
          if (!uk.isEmpty() && uk != QString("None")) {                       // @142c8d4d4
            S << "KFREAD" << nl;                                              // @142c8ebd0
            S << q(QDir::cleanDirPath(uk)) << nl;                             // absolute/as stored
          }
        }
        140134900(S, *sim, 140050b20(op), 0);        // CURSIM
        140134a10(S, db, *sim, 2, false);            // GENDB
        if (target != 0) break;
      }
    }
  }
}
```
No OPRNAM/SIMNAM and no DOE emitters in HeatTreat blocks. Steps before `first-1` still consume
simulation numbers (counter increments, nothing written), matching the +0x298 hooks `1411b2790`/
`1411b0170`, which add one sim entry per step with text `"* Operation No. %d "`.

### 4.1 Side files written during MST generation (HeatTreat)

`1411b71c0(sch, false)` (no message boxes when the 2nd arg is false):
1. `model = 1411b3760(sch)` = `14005f770(*( *(sch+0x30) +0x20))` (the op's DEF_PRE-engine model,
   created lazily through node vfunc +0xe0/+0x1b0, cached at node+0x38). `data = model+0xe0`. Either
   null -> return false (then the MST block is still written; return value ignored).
2. `140830180(data)` (count at data+0x1378); `1411bae90(sch,1)` -> `1411bb040(sch)` (adjusts step
   items' object/region mappings; no names changed).
3. `1411b73c0(sch, false, -1)`: for every step k: `fopen(<taskDir>\HT_Proc<k+1>.MOKEY, "wt")` and
   writes a KEY file with CRT `fprintf` (text mode, so CRLF). Order and formats as decompiled
   (`d4s.c` 82..679), keyword names via `140921f30`:
   `sprintf(buf, "Proc. %d: %s", k+1, stepName)`; `"%-6s\n%s\n"` SIMNAM(0x19), buf;
   one time line `"%-6s %20.10E\n"` with TLOC(0x08) or TMAX(0x2a) depending on `1411b37c0(sch)` and
   the step's media flags; `"%-6s %20.10E\n"` DTMAX(0x32) = engine sc+0x1d0; `"*\n*  %s\n*\n"`
   stepName; `"%-6s %20.10E %20.10E %7i\n"` TNOW(0x1f) with -1E21 (three variants
   `142ca5778/5798/57b8`); `"%-6s %20.10E\n"` BLZMAN(0x4d); ENVTMP(0x4b) as `"%-6s %7i %7i\n"` or
   `"%-6s %7i %20.10E\n"`; CNVCOF(0x4e) `"%-6s %7i %20.10E\n"` / `"%-6s %7i %7i\n"`; optional
   ENVATM(0x4f) and ACVCOF(0x50) lines; PHASTF(0x86) `"%-6s %7i %7i %7i\n"`; per object
   LOCTMP(0xe6) `"%-6s %7i %7i %7i %7i %7i %7i %7i\n"` plus `" %20.10E\n"` lines; ECCTMP(0xf2),
   ECTMFN(0xf5), ECCATM(0xf3) via the engine table writer `1409267f0`; TICF(0xcf)
   `"%-6s %7i %7i %7i %20.10E\n"`; other engine writers `140923320`, `140b523c0`.
   The argument values come from the DEF_PRE-engine model; the full content spec is engine work
   (KEY writer twins) and is not needed for MST bytes. Keyword table: `140922160`, base `1427c06e0`,
   0x130 entries of 0x30 bytes, id at +0, name pointer at +8.
4. `1411b9080(sch, false)`: reads (fopen `"rt"`) the previous operation's `HT_Proc<first-1>.MOKEY` and
   the step's user keyword file (unless "None") into engine objects; with `false` no dialogs and no
   model changes are made. `1411b9e70` only validates volume fractions.

`<taskDir>` = `140050df0(node)` = `convertSeparators(cleanPath(absPath(project) + sep + QString(node+0x130)))`
(the operation's `Operations/TaskNNNNN` folder); file name = `"HT_Proc" + QString::number(k+1) + ".MOKEY"`
(`1411b38e0`: `1400ac2b0` `"HT_Proc"` @`142ca5000`, `140002110` `".MOKEY"` @`142ca4ff8`).

For the converter: the MST only needs these names; the files themselves are written by the original
during Save. moproj2mst must not write them (only the MST); s1-10 should report that a HeatTreat MST
references `.MOKEY` files that may be missing/outdated (open question Q3).

Model fields: props+0x68 schedule; schedule +0x20 first step (int), +0x28 QList<HTStep*>, +0x30 owner
props/op link; HTStep +0x118 user keyword file name; node+0x130 task sub-folder.

Sample: none. Test project: HeatTreat (3D) and HeatTreat2D operations each with 3 steps, first step
index 1 and 2 (to check skipped numbering), one step with a user keyword file and one with "None";
place one HeatTreat as the first operation (checks the missing DBREAD on step 1).

---

## 5. SfOprFormingExpress `1414136b0`, SfOprFormingExpress2D `141412c90`

Both are identical:
```cpp
void SfOprFormingExpress::writeMst(S, proj, db, sim, op, dbreadArg, target) {
  bool express = false;
  auto* p = 14141ce30(this, op);   // dynamic_cast<SfOprFormingExpressProperties*>(opProps(this, op))
  if (p && 14142bb30(p))           // D = *(p+0x40); D->type(+0x14) == 3 || == 4
    express = *(bool*)(D + 0x110);  // 14142bf50
  if (!express) 1405f5340(this, S, proj, db, sim, op, dbreadArg, target);  // base writer (s1-02)
  else          14141f2f0(this, S, proj, db, sim, op, dbreadArg, target);
}
```
With the base writer the class hooks are: +0x200 `1413e5ef0` returns true (DIM/NSTART on the first
op), +0x288 `1405f56c0` (base). Below, `D = *(p+0x40)` is the FormingExpress data block.

Object fields set in the ctor `14141bbe0` (called from `14141bb40`, which `141412f90` and the 2D/Common
ctors chain to): `+0x60 = 1` (workpiece no.), `+0x64 = 2` (top die), `+0x68 = 3` (bottom die),
`+0x70 = 1.0`, `+0x78 = +0x80 = +0x88 = +0x98 = 0.0`; `+0x90` (bool) is set by the writers.

### 5.1 Express driver `14141f2f0`

Frame: no push, `sub rsp,4c68h`; `[rsp+4c70..4ca8]` = args 1..8 (matches Ghidra).
```cpp
void FormingExpress_express(this, S, proj, db, sim, op, dbreadArg, target) {
  auto* p = 14141ce30(this, op);
  if (!p || !op) return;
  bool is2d = is2D(op);
  int pos[6] = {1, 1, 0, 0, 0, 0};          // never modified: TRANS arguments
  double angle = 0.0;
  int wp = 1, top = 2, bot = 3;             // locals, constants
  int nextSim = 0;
  SubProject* cur = 1400a7390(proj);        // CURRENT sub-project (project+0x58[index 1400a73c0])
  if (cur) nextSim = 1400b8820(cur, 140050b20(op) + 1);   // 5.6
  LLOBJ tmp(3);  SimCtrl sc;  Movement mv;  // 140821bf0 / 1408d3bc0 / 14085f3f0 (engine scratch)
  Model* m = 14005f770(op);                 // op's engine model (lazy, see 4.1 step 1)
  if (!m) { destroy; return; }              // nothing written, *sim unchanged
  if (auto* dd = *(m + 0xe0)) { int dim = *(int*)(dd + 0x1c); 140821e40(&tmp, &dim); }
  1408d3cd0(&sc, &tmp);
  1408e0200(&sc, unit(op) /*140054ff0*/, false);           // sc+0xfc = unit
  p->vfunc[0x70](&sc);    // 1405fb6a0 -> 1405fb980, 1405fbd10, 1405fc060, 1405fc5d0 (5.5)
  14085f540(&mv, &tmp);
  14142cb20(p, &mv);      // mv+0x68 = mode; mv+0x50/58/60 = D+0x88/0x90/0x98 (direction) etc.
  *(double*)(this+0x70) = (unit(op) == 1) ? 100.0 /*1421de200*/ : 5.0 /*1421d3c60*/;
  *(double*)(this+0x78) = -*(double*)(this+0x70) * mv.dir[0] /*mv+0x50*/;
  *(double*)(this+0x80) = -*(double*)(this+0x70) * mv.dir[1] /*mv+0x58*/;
  if (!is2d) *(double*)(this+0x88) = -*(double*)(this+0x70) * mv.dir[2] /*mv+0x60*/;
  *(bool*)(this+0x90) = false;
  QString equip;
  if (OpObject* o = 140061250(op, top - 1)) {             // 2nd object of the op
    QString e;
    int eid = *(int*)(o + 0x28);                           // 14004de90 (EquipmentID)
    if (eid > 0) e = 1400a5970(140055030(op))->vfunc[0x208](&eid);   // equipment table path by id
    if (!e.isEmpty()) equip = QDir(140819590(db)).relativeFilePath(e);
  }
  if (is2d) angle = 1408609b0(&mv);                        // atan2(mv+0x58, mv+0x50) * 57.29577951308232
  bool first = true;
  int nBlow = *(int*)(D + 0x120);                          // 14142c080
  for (int b = 1; b <= nBlow; ++b) {
    int nStep = 14142c110(p, b);                           // D+0x128 QList<int>[b-1], 0 if out of range
    for (int s = 1; s <= nStep; ++s) {
      if (!blockBL(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,&first,&sc,&mv,equip,wp,top,bot,nextSim)) break;
      if (!blockDW(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,&first,&mv,angle,wp,top,bot)) break;
      if (!blockTO(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,&first)) break;
    }   // break leaves only the step loop; the blow loop continues
  }
}
```
Per-(blow,step) list accessors on D (all 1-based, 0/false/0.0 when out of range):
`gap(b,s)` = `14142c490` D+0x160 `QList<QList<double>>`; `stroke(b,s)` = `14142c210` D+0x150;
`14142c3e0` D+0x158 (read, result unused); `drop(b,s)` = `14142c5f0` D+0x168 `QList<QList<bool>>`;
`flip(b,s)` = `14142c750` D+0x170. Scalars: `14142bd00` D+0xd0, `14142bd50` D+0xd8, `14142bce0` D+0xc8
(unused), `14142bc90` D+0xc0, `14142bc40` D+0xa8 (bool).
Unit-converted scalars (`1402ede00(table 143d76368, category, unitSystem=unit(node), QString, &out, &err)`,
returns the value in project units; owned by whoever ports the `doubleUnits` parser):
`ihtcDW(p)` = `14142be90` (D+0x100, cat 0x20); `dtDW(p)` = `14142be70`->`14142bdb0` (D+0xf8, cat 0);
`fric(p)` = `1405fc890` = `*(double*)(*(p+0x30)+0x10)` (no conversion); `ihtc(p)` = `1405fc8e0`
(`*(p+0x30)+0x28`, cat 0x20).

### 5.2 Block BL `14141fc50` (one per blow/step)

```cpp
bool blockBL(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,bool* first,SimCtrl* sc,Movement* mv,
             const QString& equip,int wp,int top,int bot,int nextSim) {
  bool is2d = is2D(op);
  ++*sim;
  if (!(target == 0 || *sim == target)) return true;
  if (!*(bool*)(op + 0x150) || !*first) {                 // 140064740
    140131c10(S, *sim, op);
    if (!this->vfunc[0x200]() /*true*/ || !*first) 1401343e0(S, db, *sim, dbreadArg);
    else 140134250(S, db, *sim, op, dbreadArg);           // DIM/NSTART
    141423130(this, S, proj, db, *sim, op, 0x200010, first);   // 5.4
    bool drop = false, flp = false; double g = 0.0;
    if (s > 1) { drop = 14142c5f0(p,b,s-1); flp = 14142c750(p,b,s-1); g = 14142c490(p,b,s-1); }
    double X = this->d78, Y = this->d80, Z = this->d88;      // +0x78/+0x80/+0x88
    if (!is2d) {
      if (s != 1) {
        S << "* Positioning" << nl;
        if (!flp) {
          if (!drop) {
            if (g <= 0.0) {
              S << "*    Put Top die on Workpiece" << nl;
              S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(2) << "\t" << d(0.0001)
                << "\t" << d(mv->dir[0]) << "\t" << d(mv->dir[1]) << "\t" << d(mv->dir[2])
                << "\t" << i(this->i60) << nl;
            } else {
              S << "*    Move Top die back" << nl;
              S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(1) << "\t" << d(-X) << "\t" << d(-Y) << "\t" << d(-Z) << nl;
            }
          } else {
            S << "*    Move Top die back" << nl;
            S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(1) << "\t" << d(-X) << "\t" << d(-Y) << "\t" << d(-Z) << nl;
            S << "*    Move Bottom die back" << nl;
            S << "OBJPOS " << "\t" << i(this->i68) << "\t" << i(1) << "\t" << d(X) << "\t" << d(Y) << "\t" << d(Z) << nl;
          }
        }                                                  // 3D + flip: only "* Positioning"
      }
    } else {
      if (flp) {
        S << "* Flip" << nl;
        S << "*    Flip the workpiece with respect to a line perpendicular to the movement direction" << nl;
        S << "OBJPOS " << "\t" << i(wp) << "\t" << i(4) << "\t" << i(0) << "\t" << i(0)
          << "\t" << d(mv->dir[1]) << "\t" << d(mv->dir[0]) << nl;
      }
      if (s != 1) {
        S << "* Positioning" << nl;
        if (!flp) {
          if (!drop) {
            if (g <= 0.0) {
              S << "*    Put Top die on Workpiece" << nl;
              S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(2) << "\t" << d(0.0001) << "\t" << i(this->i60)
                << "\t" << d(mv->dir[0]) << "\t" << d(mv->dir[1]) << nl;
            } else {
              S << "*    Move Top die back" << nl;
              S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(1) << "\t" << d(-X) << "\t" << d(-Y) << nl;
            }
          } else {
            S << "*    Move Top die back" << nl;
            S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(1) << "\t" << d(-X) << "\t" << d(-Y) << nl;
            S << "*    Move Bottom die back" << nl;
            S << "OBJPOS " << "\t" << i(this->i68) << "\t" << i(1) << "\t" << d(X) << "\t" << d(Y) << nl;
          }
        } else {
          S << "*    Put the flipped Workpiece on the Bottom die" << nl;
          S << "OBJPOS " << "\t" << i(this->i60) << "\t" << i(2) << "\t" << d(0.0001) << "\t" << i(this->i68)
            << "\t" << d(mv->dir[0]) << "\t" << d(mv->dir[1]) << nl;
          S << "*    Put Top die on Workpiece" << nl;
          S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(2) << "\t" << d(0.0001) << "\t" << i(this->i60)
            << "\t" << d(mv->dir[0]) << "\t" << d(mv->dir[1]) << nl;
        }
      }
    }
    S << "* Generate the contacts" << nl;
    S << "INICTC " << "\t" << i(wp) << nl;
    S << "GENCTC " << "\t" << d(0.0001) << nl;
    S << "FRCFAC " << "\t" << i(wp) << "\t" << i(top) << "\t" << i(1) << "\t" << i(0) << "\t" << d(fric(p)) << nl;
    double h = ihtc(p);
    S << "IHTCOF " << "\t" << i(wp) << "\t" << i(top) << "\t" << i(0) << "\t" << d(h) << nl;
    S << "IHTCOF " << "\t" << i(wp) << "\t" << i(bot) << "\t" << i(0) << "\t" << d(h) << nl;
    double st = 14142c210(p, b, s); 14142c3e0(p, b, s);
    if (!equip.isEmpty()) {
      S << "IMPEQP " << "\t" << i(top) << nl;
      S << q(equip) << nl;
      st = st / 100.0;
      S << "DOESCL " << "\t" << d(st) << nl;
      S << "MOVCTL " << "\t" << i(top) << nl;
    }
    if (*(int*)(sc + 0x1ec) == 0) {
      if (*(int*)(sc + 0x240) == 0) S << kw(0x30) /*"DSMAX"*/ << "\t" << d(*(double*)(sc+0x1e0)) << nl;
      if (*(int*)(sc + 0x240) == 1)
        S << kw(0x32) /*"DTMAX"*/ << "\t" << d(*(double*)(sc+0x1d0)) << "\t" << d(*(double*)(sc+0x1d8)) << nl;
      else if (*(double*)(sc + 0x1d8) > 0.0)
        S << kw(0x32) << "\t" << d(0.0) << "\t" << d(*(double*)(sc+0x1d8)) << nl;
      S << kw(0x33) /*"DTTYPE"*/ << "\t" << i(0) << "\t" << i(0) << nl;
    }
    S << "NSTEP " << "\t" << i(10000) << nl;
    S << "TRANS " << "\t" << i(pos[0]) << "\t" << i(pos[1]) << "\t" << i(pos[2]) << "\t" << i(pos[3])
      << "\t" << i(pos[4]) << "\t" << i(pos[5]) << nl;                     // "TRANS \t1\t1\t0\t0\t0\t0"
    S << "* Define next simulation number - if minimum die distance criteria met, jump to next operation" << nl;
    S << "NEXTSIMNO" << "\t" << i(3) << nl;
    S << i(*sim + 1) << "\t" << i(*sim + 1) << "\t" << i(nextSim) << nl;
    140134680(S, op);
    140134730(S, op->name);                                 // OPRNAM
    1401347e0(S, 141423f20(this, op, b, s, false));         // SIMNAM <opname>-BL<s> (P15: step, not blow)
    140134900(S, *sim, 140050b20(op), 0);                    // CURSIM
    140134a10(S, db, *sim, 2, false);                        // GENDB
    return target == 0;
  } else {                                                   // op+0x150 set and first block
    140131c10(S, *sim, op);
    140134510(S, db, op);
    140134680(S, op);
    140134730(S, op->name);
    1401347e0(S, 141423f20(this, op, b, s, false));
    140134900(S, *sim, 140050b20(op), 0);
    140134a10(S, db, *sim, 0, false);                        // GENDB mode 0
    *first = false;
    return target == 0;
  }
}
```
`kw(n)` = `140921f30(n)`: copies the name from table `1427c06e0` into the static buffer `143dd9ba0`
and returns it; 0x30=`DSMAX`, 0x32=`DTMAX`, 0x33=`DTTYPE` (no trailing space, unlike the literal
keywords). The `"-0.0"` constant `1421d3d30` is a sign-flip mask (`-x`). All literals above are
quoted from the annotated decompile (`d4s.c` 1186..1526); every `"\t"` is a separate 2-byte literal.

### 5.3 Blocks DW `1414216e0` and TO `141422320`

```cpp
bool blockDW(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,bool* first,Movement* mv,double angle,
             int wp,int top,int bot) {
  bool is2d = is2D(op);
  this->b90 = false;
  double g = 14142c490(p, b, s);
  if (!(g > 0.0)) return true;
  ++*sim;
  if (!(target == 0 || *sim == target)) return true;
  140131c10(S, *sim, op);
  1401343e0(S, db, *sim, dbreadArg);
  this->b90 = true;
  S << "* Positioning" << nl;
  S << "*    Move Top die away" << nl;
  if (!is2d) S << "OBJPOS " << "\t" << i(top) << "\t" << i(1) << "\t" << d(this->d78) << "\t" << d(this->d80) << "\t" << d(this->d88) << nl;
  else       S << "OBJPOS " << "\t" << i(top) << "\t" << i(1) << "\t" << d(this->d78) << "\t" << d(this->d80) << nl;
  S << "* Generate the contacts" << nl;
  S << "INICTC " << "\t" << i(wp) << nl;
  S << "GENCTC " << "\t" << d(0.0001) << nl;
  double h = ihtcDW(p);
  S << "IHTCOF " << "\t" << i(wp) << "\t" << i(top) << "\t" << i(0) << "\t" << d(h) << nl;
  S << "IHTCOF " << "\t" << i(wp) << "\t" << i(bot) << "\t" << i(0) << "\t" << d(h) << nl;
  if (!is2d) {
    S << "MOVCTL " << "\t" << i(top) << "\t" << i(1) << "\t" << i(0) << "\t" << i(0) << "\t" << i(0) << "\t" << i(-1) << "\t" << i(0) << nl;
    S << "MOVCTL " << "\t" << i(bot) << "\t" << i(1) << "\t" << i(0) << "\t" << i(0) << "\t" << i(0) << "\t" << i(1) << "\t" << i(0) << nl;
  } else {
    S << "MOVCTL " << "\t" << i(top) << "\t" << i(1) << "\t" << i(0) << "\t" << d(angle)  << "\t" << i(0) << nl;
    S << "MOVCTL " << "\t" << i(bot) << "\t" << i(1) << "\t" << i(0) << "\t" << d(-angle) << "\t" << i(0) << nl;
  }
  S << "DSMAX " << "\t" << i(0) << nl;
  double dt = dtDW(p);
  S << "DTMAX " << "\t" << d(dt) << nl;
  int n = 1;
  if (dt > 0.0) { double r = g / dt + 0.5; n = (int)(r >= 1.0 ? r : 1.0); }    // truncation
  S << "NSTEP " << "\t" << i(n) << nl;
  S << "TRANS " << "\t" << i(pos[0]) << "\t" << i(0) << "\t" << i(pos[2]) << "\t" << i(pos[3])
    << "\t" << i(pos[4]) << "\t" << i(pos[5]) << nl;                          // "TRANS \t1\t0\t0\t0\t0\t0"
  140134680(S, op);
  140134730(S, op->name);
  1401347e0(S, 141424130(this, op, b, s, false));          // <opname>-DW<s>
  140134900(S, *sim, 140050b20(op), 0);
  140134a10(S, db, *sim, 2, false);
  return target == 0;
}
```
```cpp
bool blockTO(this,S,proj,db,sim,op,dbreadArg,target,p,b,s,pos,bool* first) {
  bool is2d = is2D(op);
  if (!14142c5f0(p, b, s)) return true;                    // drop flag
  double dist = *(double*)(D + 0xd0);
  if (!(dist > 0.0)) return true;
  ++*sim;
  if (!(target == 0 || *sim == target)) return true;
  double dt = *(double*)(D + 0xd8);
  (void)*(double*)(D + 0xc8);
  140131c10(S, *sim, op);
  1401343e0(S, db, *sim, dbreadArg);
  141423130(this, S, proj, db, *sim, op, 0x100020, first);   // writes "PDIE \t1", clears *first
  double T = *(double*)(D + 0xc0);
  S << "* Initialize node temperature" << nl;
  S << "NDTMP " << "\t" << i(this->i60) << "\t" << i(0) << "\t" << d(T) << nl;
  if (*(bool*)(D + 0xa8)) {
    S << "* Initialize strain" << nl;
    S << "STRAIN " << "\t" << i(this->i60) << "\t" << i(0) << "\t" << d(0.0) << nl;
  }
  S << "* Positioning" << nl;
  double X = this->d78, Y = this->d80, Z = this->d88;
  if (!this->b90) {
    S << "*    Move Top die away" << nl;
    S << "OBJPOS " << "\t" << i(this->i64) << "\t" << i(1) << "\t" << d(X) << "\t" << d(Y) [<< "\t" << d(Z) if 3D] << nl;
  }
  S << "*    Move Bottom die away" << nl;
  S << "OBJPOS " << "\t" << i(this->i68) << "\t" << i(1) << "\t" << d(-X) << "\t" << d(-Y) [<< "\t" << d(-Z) if 3D] << nl;
  S << "* Generate the contacts" << nl;
  S << "INICTC " << "\t" << i(this->i60) << nl;
  S << "GENCTC " << "\t" << d(0.0001) << nl;
  S << "DSMAX " << "\t" << i(0) << nl;
  S << "DTMAX " << "\t" << d(dt) << nl;
  int n = 1;
  if (dt > 0.0) { double r = dist / dt + 0.5; n = (int)(r >= 1.0 ? r : 1.0); }
  S << "NSTEP " << "\t" << i(n) << nl;
  S << "TRANS " << "\t" << i(pos[0]) << "\t" << i(0) << "\t" << i(pos[2]) << "\t" << i(pos[3])
    << "\t" << i(pos[4]) << "\t" << i(pos[5]) << nl;
  140134680(S, op);
  140134730(S, op->name);
  1401347e0(S, 141423d10(this, op, b, s, false));          // <opname>-TO<s>
  140134900(S, *sim, 140050b20(op), 0);
  140134a10(S, db, *sim, 2, false);
  return target == 0;
}
```
`-0.0` in the bottom-die OBJPOS when X is 0: the original computes `x ^ 0x8000000000000000` and
prints the double, so a zero direction prints as `-0` per QTextStream rules (s1-06 must keep the
sign of negative zero). Same for `-X` in BL.

### 5.4 Object section `141423130(this,S,proj,db,sim,op,mode,bool* first)`

```cpp
bool doObj = false, doPos = false, notFirst = true;
if (first && *first) { doObj = doPos = true; notFirst = false; *first = false; }
if (mode == 0x100010 || mode == 0x100000 || mode == 0x100020) {
  S << "PDIE " << "\t" << i(1) << nl;
} else if (mode == 0x200010) {
  if (doObj) {
    140134bf0(S, proj, db, sim, op); 140134db0(S, proj, db, sim, op);
    140135220(S, proj, db, sim, op, notFirst /*=false here*/);
    1401360e0(S, proj, db, sim, op); 1401365f0(S, proj, db, sim, op);
  }
  S << "PDIE " << "\t" << i(2) << nl;
  if (doPos) { S << "* Positioning" << nl; 140135f10(S, proj, db, sim, op); }
  140135da0(S, proj, db, sim, op);
}
```

### 5.5 SimCtrl values used by BL (props slot +0x70 = `1405fb6a0`)

`1405fb6a0(p, sc)` calls p's +0x80/+0x90/+0xa0/+0xb0 = `1405fb980`, `1405fbd10`, `1405fc060`,
`1405fc5d0` (SfOprBaseProperties). With `B = *(p+0x30)` (common props block):
* `1405fb980`: sc+0x16c = B[0] (int), sc+0x164 = B[1], sc+0x1d0 = conv(B+0x40, cat 0) if ok.
* `1405fbd10`: sc+0x3d0 = conv(B+0x20, cat 9). `1405fc060`: sc+0x4c8 = conv(B+0x30, cat 0x20).
* `1405fc5d0`: sc+0x240 = B+8 (int mode, via `1408da6f0`); mode 0: sc+0x1e0 = conv(B+0x38, cat 1)
  (`1405fd4b0`) and sc+0x1d0 = 0; mode != 0: sc+0x1d0 = conv(B+0x40, cat 0), sc+0x1e0 = 0.
* Defaults from `1408d3bc0`->`1408d3cd0`->`1408d3d10`->`1408d47a0`: sc+0x1c8=1, +0x1d0=+0x1d8=+0x1e0=0;
  `1408da660` sets +0x240 from +0x1e0 only when +0x1ec==0. sc+0x1ec is word 1 of the keyword
  descriptor at sc+0x1e8 (`140899010(sc+0x1e8, 0x80)`, default 0; no case for 0x80 seen).
So in practice: mode 0 -> `DSMAX\t<dsmax>` then `DTTYPE\t0\t0`; mode 1 -> `DTMAX\t<dt>\t0` then
`DTTYPE\t0\t0` (sc+0x1d8 stays 0 unless the engine model changes it).

### 5.6 `nextSim` = `1400b8820(subProject, n)` and the +0x298 sim lists

`1400b8360(sp)` builds the list of simulation entries of the sub-project: for each flow item k
(`1400b6630` op ids, 1-based position k+1) it calls `op->vfunc[0x298](op, k+1, &counter, node, &list)`.
Entry fields: +8 operation position, +0xc simulation number (`140cbc690`/`140cbc680`).
`1400b8820(sp, n)` returns the sim number of the first entry with position n; if none and n > 1, the
last entry of position n-1 plus 1; else 0. Hooks for this packet's classes:
* FormingExpress `141423390`: if not express -> base `1405f5880` (one entry); else for b, s as in 5.1:
  one entry `BL` (++counter), one `DW` if gap(b,s) > 0, one `TO` if drop(b,s) && D+0xd0 > 0.
  (Same conditions as the writer, so the numbering matches.)
* HeatTreat(2D) `1411b2790`/`1411b0170`: one entry per schedule step.
* Diestress `1402b7380`, DiestressBM via `1405f5880`: one entry. Report/ShapeOptim: none.
Other classes' +0x298 hooks matter for `nextSim` too (s1-02/03/04).

### 5.7 SIMNAM builders `141423f20` ("BL%1"), `141424130` ("DW%1"), `141423d10` ("TO%1")

`r = QString("BL%1").arg(s, 0, 10, QChar(' '))` with the STEP number s (5th argument; corrected by P15
from the decompile and the +0x298 list `141423390`, which also passes the step); if `!short` ->
`op->name + "-" + r` else `r`.
All callers pass `short = false`.

Sample: none (no FormingExpress in the corpus). Test project needs: a 3D and a 2D FormingExpress
operation with express mode on (data type 3 or 4 and the express flag), 2 blows with 2-3 steps each,
at least one step with gap > 0, one with drop on and D+0xd0 > 0, one with flip on (2D), the top die
with an equipment (to get IMPEQP/DOESCL), DT mode 0 and mode 1 variants, SI and English units
(+0x70 = 5 or 100), and an operation after it (for `nextSim`). Also one FormingExpress with express
off (base writer path).

---

## 6. Records read by these writers (model fields)

| Object | Offset | Type | Meaning | Set by | Read by |
|---|---|---|---|---|---|
| OpNode | +0x10 | SfOprBaseProperties* | operation properties | open (s1-01) | `140051230` |
| OpNode | +0x40/+0x50 | QList<int>/QList<OpObject*> | op objects | open | `14005fa80`, `140061250`, `14005fb30` |
| OpNode | +0x108 | int | flow item id | open | `140050bc0` |
| OpNode | +0x118 | QString | operation name | open (`Name`) | `140050ac0` (OPRNAM/SIMNAM) |
| OpNode | +0x130 | QString | task sub-folder | open | `140050df0` |
| OpNode | +0x150 | bool | "read from DB" style flag | open | `140064740` |
| OpObject | +0 / +4 | int | object id / object type | open (`Object Id`, `ObjectType`) | `14029a550` |
| OpObject | +0x19 | bool | has mesh (DEFAMG/GENBCC) | open | `14004d800` |
| OpObject | +0x28 | int | EquipmentID | open | `14004de90` |
| FE props | +0x30 | block* | common props: +0 int,+4 int,+8 dt mode,+0x10 friction,+0x20/+0x28/+0x30/+0x38/+0x40 unit strings | Properties | 5.5, `1405fc890/8e0` |
| FE props | +0x40 | FE data* | +0x14 type, +0x18 name, +0x28..+0x98 motion, +0xa8,+0xc0..+0xd8, +0xf8/+0x100 unit strings, +0x110 express, +0x120 nBlow, +0x128, +0x150..+0x170 per step lists | Properties | 5.1 |
| BM props | +0x38.. | see 3 | wizard data | `DieStressBmWizard.Wzd` | 3 |
| HT props | +0x58 | block* | sim-ctrl data | Properties | `1411c05a0` |
| HT props | +0x68 | HTSchedule* | steps | open/KEY | 4 |
| SfOprFormingExpress | +0x60..+0x98 | see 5 | ctor constants / writer state | ctor `14141bbe0`, 5.1 | 5.2-5.3 |
| SubProject | +0x10 | Project* | owner | open | `1400b5c40` |
| SubProject | +0x18 | int | Type (0 main, 2/3 DOE, 4 ?) | `Sub_Project Type` | `140010330/360`, `1400ba650` |
| SubProject | +0x1c | int | Id | open | `1400b5c80` |
| SubProject | +0x20 | SfMoDoeManager* | DOE study | `SfDoeStudy` + `<Control>/<base>.OPT` | `1400b5a90` |
| Project | +0x18 | QString | project file path | open | `14008a4b0/500`, `1400a4220` |
| Project | +0x48 | QDomDocument* | project DOM | open/save | `1400a78e0` |
| Project | +0x58 | QList<SubProject*> | sub-projects | open | `1400a74f0`, `1400a7360` |

---

## 7. DOE path

### 7.1 When it runs

`1400a20d0` (MST save): `1401313c0(proj)` writes `<project>.MST` for sub-project 0 only. Then
`sp = 1400a7390(proj)` (current sub-project); if `sp->type == 2 || sp->type == 3` (`140010330`,
`140010360`): `1400ba650(sp)` then `1400ba800(sp)`.

`1400ba650(sp)`: returns false if `sp->project` is null or type is 0 or 4. Else:
```cpp
QString ctrl = 1400ba350(sp);   // cleanPath(1400ba000(sp) + sep + "Control")
// 1400ba000: cleanPath(absPath(project) + sep + "DoeStudies" + sep +
//            (manager ? manager->name (+0x98) : "DoeStudy" + QString::number(1400b5c60(sp))))
QString tmpName = 1400a51e0(proj);  // 140818bf0(completeBaseName(project+0x18), ".MST.TEMP")
                                    //   = base + "." + "MST.TEMP" unless base already ends with it
QString tmpPath = cleanPath(ctrl + sep + tmpName);
140131530(proj, 1400b5c80(sp) /*sub-project index*/, tmpPath);    // the .MST.TEMP
QString mst = 1400a50a0(proj, false);   // 140818d50(completeBaseName(path), ".MST") (s1-02)
140915eb0(tmpName, mst, ctrl);          // DEF_DOE_PARSER.INI, see 7.5
return true;
```

### 7.2 `140131530(proj, idx, path)` (.MST.TEMP) and `140131a50` (*!SUBPRJ)

Same as `1401313c0` but with the given path and `140131a50` instead of `1401318b0`:
```cpp
if (!proj || count(proj+0x58) < 1) return false;
QFile f(path); if (!f.open(QIODevice::WriteOnly)) return false;     // flags 2, LF line ends
140131390();                       // reset globals 143d80288/143d80290 (s1-02)
QTextStream S(&f);
140131680(S, proj);                // header (s1-02)
140131a50(S, proj, idx);
140131df0(S, proj, idx);           // operations of sub-project idx
140131d10(S, proj);                // trailer
f.close(); return true;

void 140131a50(S, proj, idx) {
  S << nl;
  S << "*-------------------------------------------------------------------------*" << nl;
  S << "*                                                                         *" << nl;
  S << "* Sub-Project                                                             *" << nl;
  S << "*                                                                         *" << nl;
  S << "*-------------------------------------------------------------------------*" << nl;
  S << "*!SUBPRJ " << i(idx + 1) << nl;
  S << "*!PRJNAM " << q(1400a75a0(proj, idx)) << nl;   // attribute "Name" (default "") of the
                                                         // idx-th <Sub_Project> child of the DOM root (proj+0x48)
  S << "*" << nl;
  S << "*-------------------------------------------------------------------------*" << nl;
}
```
(Literals `1421ac5d0..1421ac780`, `"*"` @`1421a8e8c`; each banner line is 75 bytes.)

### 7.3 Operation walk with a DOE study (`140131df0` branch) and `1401321a0`

In `140131df0(S, proj, idx)`: `mgr = 1400b5a90(sp)` (sp+0x20). If `mgr`, `linkMap = mgr->vfunc[0x1c8]()`
(`14091bdb0`, QMap<int,int>) once, then for each op id in flow order:
`1401321a0(S, proj, db, mgr, opId, &sim, node, &linkMap, 0, 0)`:
```cpp
SfOprBase* op = 140054e90(node);
if (!op) return;
g_doe = true;                                  // byte 143d80258 (140132100 sets it false)
g_vars = mgr->vfunc[0x1b8](opId);              // 14091bbb0: vars v (mgr+0x38 list, each v+0x18 := its index)
                                               //   with v->ops(+0x38) containing opId (14091dbb0)
g_opId = opId;                                 // 143d80268
g_mgr = mgr;                                   // 143d80260
g_linkMap = linkMap;                           // 143d80298 (implicitly shared copy)
op->vfunc[0x280](S, proj, db, &sim, node, 0, 0);
```
`linkMap` (`14091bdb0`): for every var v (mgr->vfunc[0x1a8] = all vars, re-indexed): key = v index
(+0x18), value = (v+0x40 link ? link index : own index); then each value is replaced by the position
of var(value) (`mgr->vfunc[0x1a0]`) in the list of unlinked vars (`mgr->vfunc[0x1c0]` = vars with
v+0x40 == null), via `QList::indexOf`. Resolved through `vtables.tsv`: `SfMoDoeManager` and
`DefDoeModel` share 0x1a0 `14091b950`, 0x1a8 `14091b9b0`, 0x1b0 `14091ba60`, 0x1b8 `14091bbb0`,
0x1c0 `14091bcb0`, 0x1c8 `14091bdb0`.

Only writers that call `1401322d0`/`140132a40` get DOE lines: the base writer `1405f5340` (so all
base-template classes, including FormingExpress with express off), DiestressBM2D/3D and Distortion
(`141362f60`). FormingExpress express blocks, HeatTreat, Diestress, Report, ShapeOptim (and any other
class not calling them) write the same text in the .MST.TEMP as in a plain MST.

`1401322d0(S, node)` (DOE variables):
```cpp
if (!g_doe || g_vars.isEmpty()) return;
S << "* DOE Variables " << nl;
for (Var* v : g_vars) {
  QList<Item*> items = v->items;                 // 1400d0e60: QList at v+0x30
  int id = v->index;                             // +0x18
  S << nl;
  S << "* DOE Study " << nl;
  Var* L = v->link;                              // +0x40
  int n;
  if (!L) n = v->number;                         // +0x1c
  else {
    id = L->index; n = L->number;
    S << "* Link: " << i(v->number) << "->" << i(n);
    if (g_linkMap.contains(id)) S << ":" << i(g_linkMap[id]);
    S << nl;
  }
  S << "* (" << i(n) << ") " << q(v->name /*QString at v+0*/) << nl;
  if (!g_linkMap.isEmpty() && g_linkMap.contains(id)) n = g_linkMap[id];
  for (Item* it : items) {
    QString a = it->text18, b = it->text20;      // QStrings at +0x18, +0x20
    S << "DOEVAR" << "\t" << i(n + 1) << "\t" << i(lines(a)) << "\t" << i(lines(b)) << nl;
    S << q(a) << nl;
    S << q(b) << nl;
  }
}
S << nl;
```
`lines(x)` = `14093a200`: `x.split(QChar('\n'), SkipEmptyParts /*int at 143875d68 = 1*/, Qt::CaseSensitive).count()`.
`QMap::value` with a missing key returns 0 (`140136f40` also returns 0 for an empty map).

`140132a40(S, node)` (DOE geometry variables):
```cpp
if (!g_doe) return;
for (int k = 0; k < 14005fa80(node); ++k) {
  int objId = 14005fb30(node, k);                // node+0x40 [k], -99 if invalid
  if (objId < 0) continue;
  QList<Var*> gv = g_mgr->vfunc[0x1b0](g_opId, objId);   // 14091ba60: geometry vars (140817800)
                                                         // of this op whose object (140817bc0(v,0)) == objId
  if (gv.isEmpty()) continue;
  S << "* DOE Geometry Variables " << nl;
  140132bd0(S, node, g_opId, objId, gv, &g_linkMap);
}
```

`140132bd0(S, node, opId, objId, gv, map)` (literals `1421aca78`..`1421acb30`):
```cpp
S << "* Geometry change : " << q(14005ff30(node, objId)) << nl;  // object name, or "Read From DB" if 1400606c0(objId)
S << "* DOE Study " << nl;
int pos = 14005fb80(node, objId); int objNo = 14005fc10(node, pos);   // pos+1, or 0
for (v : gv) S << "* (" << i(v->number) << ") " << q(v->name) << nl;
int mode = 2;
for (v : gv) {
  if (v->geoType /*+0x20*/ == 100) mode = 102;
  if (Var* L = v->link) {
    int ln = L->number;
    if (!map.isEmpty() && map.contains(L->index)) ln = map[L->index];
    S << "* Link: " << i(v->number) << "->" << i(ln) << nl;
  }
}
int nv = gv.count();
S << "DOEVAR" << "\t" << "0" << "\t" << i(nv * 5 + 3) << "\t" << i(nv) << "\t" << "1" << nl;
S << "MORP" << "\t" << i(nv) << "\t" << i(mode) << nl;
for (v : gv) {
  int id = v->link ? v->link->index : v->index;
  if (!map.isEmpty() && map.contains(id)) id = map[id];
  S << i(id + 1) << nl;
}
QString f0; 140817a80(gv[0], 0, &f0);            // gv[0]->files(+0x28)[0] (QString)
S << q(QString("../../Control/") + QFileInfo(f0).fileName()) << nl;
for (v : gv) {
  QString a, b; 140817940(v, 0, &a, &b);         // files[0]->+8, ->+0x10
  QString A = "../../Control/" + fileName(a), B = "../../Control/" + fileName(b);
  QString aCor, bCor;
  if (!140817e20(v, 0)) { S << "NA" << nl << "NA" << nl; }      // flag files[0]+0x18 && a non-empty
  else {
    aCor = a; aCor.truncate(a.lastIndexOf(".GEO")); aCor += ".COR";
    QString c = "../../Control/" + fileName(a); c.truncate(c.lastIndexOf(".GEO")); c += ".COR";
    S << q(A) << nl << q(c) << nl;
  }
  if (!140817eb0(v, 0)) { S << "NA" << nl << "NA" << nl; }      // flag files[0]+0x19 && b non-empty
  else {
    bCor = b; bCor.truncate(b.lastIndexOf(".GEO")); bCor += ".COR";
    QString c = "../../Control/" + fileName(b); c.truncate(c.lastIndexOf(".GEO")); c += ".COR";
    S << q(B) << nl << q(c) << nl;
  }
  140909a30(aCor, bCor, v);                      // SIDE FILES, 7.5
}
QString g = QString("OutputGeoOp%1Obj%2.GEO").arg(opId, 0, 10, QChar(' ')).arg(objId, 0, 10, QChar(' '));
S << q(g) << nl;
S << nl;
S << "GFREAD" << "\t" << i(objNo) << nl;
S << q(g) << nl;
OpObject* o = 140061310(node, objId);
if (o && o->hasMesh /*+0x19*/) {
  S << nl;
  S << "DEFAMG" << "\t" << i(objNo) << nl;
  S << nl;
  S << "GENBCC" << "\t" << i(objNo) << nl;
  S << nl;
}
```
`lastIndexOf` returns -1 when `.GEO` is missing; `truncate(-1)` empties the string in Qt 4 (then the
`.COR` name is just `".COR"`); keep that behaviour.

### 7.4 What the sample files influence

`Samples_LHS.par`, `Samples_FF.par`, `Samples_USR.par` are only path strings set on the manager at
open (`14008f530`: `1400d7a80(mgr, 0/1/2, <dir>/Samples_*.par)`; the manager is also loaded from
`<Control>/<base>.OPT` via `140908630`). None of the MST writers read them, and the `.MST.TEMP` does
not contain sample values: it carries `DOEVAR`/`MORP` placeholders (variable number, line counts,
the before/after keyword text of each item, geometry file names) for the external DOE parser
(`DEF_DOE_PARSER.INI` names the template and target). The samples influence the output only through
manager state built at open (variables, links, per-op filters, geometry files), which s1-01 owns.

### 7.5 Side files written by the DOE path during Save

| Writer | File | Content |
|---|---|---|
| `140131530` | `<proj>/DoeStudies/<study>/Control/<base>.MST.TEMP` | MST with `*!SUBPRJ`, LF |
| `140915eb0` | `<ctrl>/DEF_DOE_PARSER.INI` | `QFile` WriteOnly\|Text (CRLF): `"1"`, tmpName, mst (each + `endl`) |
| `140909c50` (from `140132bd0`) | `aCor`, `bCor` (full paths next to the geometry files) | WriteOnly\|Text: `i(nPairs)`, then per point pair `d(p1.x) \t d(p1.y) \t d(p2.x) \t d(p2.y)`, then `"XYR_MODE"` if v->geoType == 100 |
| `1400ba800` | creates `DoeStudies/<study>`, `Control`, copies `Operations`, `Objects`, ... (`1400bb760`, `14081b810`, `14081a650`), writes `.OPT` (`140907a10`) | not MST-related, out of scope |

`140909a30` only writes when the path is non-empty and the point lists (`140818420` v->files[0]+0x28,
`140818490` +0x30) are non-empty. moproj2mst must not write any of these (plan section 2); the
`M2M_FLAG_DOE_TEMP` form needs only 7.2/7.3 text.

Sample: none (every corpus `Sub_Project` has `Type="0"`; no `SfDoeStudy`). Test project: a forming
project with one DOE study (type 2) with (a) a keyword variable applied to two operations with
2-line before/after text, (b) a linked variable, (c) a geometry variable on a die with `.GEO` files,
then Save; compare `<base>.MST.TEMP`, `DEF_DOE_PARSER.INI`. A type 3 (optimization) study as a second
case.

---

## 8. Open questions

Q1. `1400b9e80`/`1400a4f60` (Diestress, DiestressBM DB path with directory) are not decompiled here;
    s1-02 should own them (also used for "Read From DB"). Exact path form (absolute, separators) is
    unknown until then.
Q2. FormingExpress output depends on `1402ede00` (unit-string conversion) for IHTC, DSMAX/DTMAX
    and DW/TO time steps; owner to be decided (s1-01 Properties parser or s1-06).
Q3. HeatTreat MSTs reference `Operations/TaskNNNNN/HT_Proc<k>.MOKEY` that the original writes during
    Save (4.1). moproj2mst writes only the MST; should s1-10 warn when those files are missing, or
    should a later packet port `1411b73c0` (DEF_PRE-engine KEY writer twins)?
Q4. FormingExpress needs the operation's engine model (`14005f770`): if it cannot be built, nothing is
    written for the op. [P15: resolved. vt+0xe0 = `140100e20` returns 0, then vt+0x1b0 = `1405f6410`
    builds the model whenever the op has a project; no KEY file is read. See m2m hasEngineModel.] Which KEY files that load needs is s1-01's lazy-load question.
Q5. `nextSim` uses the CURRENT sub-project (`1400a7390`), not the one being written; for a DOE
    .MST.TEMP and the main MST these can differ. Keep as is.
Q6. [P15: resolved, case 0x80 sets word 1 = 0, so sc+0x1ec = 0.] sc+0x1ec default: `140899010(sc+0x1e8, 0x80)`;
    confirm with the DEF_PRE port's SIM_CTRL defaults.
Q7. The 4th argument of the HeatTreat emitters is the loop index, not `*sim`; harmless only if those
    emitters ignore it (s1-02 to confirm).
Q8. No sample exists for any type in this packet or for DOE; all of it needs oracle B (or new samples)
    before the converter can claim byte identity.
