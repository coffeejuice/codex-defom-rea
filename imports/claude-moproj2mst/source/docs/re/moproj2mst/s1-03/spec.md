# s1-03 spec: SfOprCogging and HeatTransfer-family MST writers

Packet s1-03 of stage 1 (moproj2mst). Static evidence only: Ghidra decompiles of the
`DEF_GUI_MO_64.exe` copy (project `gproj_s1c`), `.rdata` reads, and copies of
`C:\PROBLEM\TEST_REA\COG_P1_B1` and `C:\PROBLEM\DMO_LIB_TEMPLATES\HEAT1` in `samples/`.
Nothing was run. Addresses are VAs (image base `0x140000000`).

Shared emitters `140131c10..140136810` and the base writer `1405f5340` are specified by s1-02;
this file refers to them by address and only states which ones each writer calls, in which order
and with which arguments.

Working files in this folder: `cog_d*.c`, `mdt_d*.c`, `hooks_d0.c`, `acc_d0.c`, `ctor_d*.c`,
`legacy_d0.c` (raw decompiles), `*c.c` (same, literals resolved and `QTextStream` chains
compacted by `tools/annot.py` + `tools/compact.py`), `literals.txt` (every literal used by the
cogging writer, with its `.rdata` address), `tools/mdt.py` + `tools/cogw.py` (Python prototype of
the MDT reader and the cogging writer) and `tools/cmp_cog.py` (corpus comparison).

---------------------------------------------------------------------------------------------------

## 0. Results in one screen

* **HeatTransfer family** (SfOprHeatTransfer, HeatTransfer2D, HeatTransferCommon,
  HeatTransferExpress, HeatTransferExpress2D, HTFurnace, Forming, Forming2D, Pre, Pre2D,
  ExpressBase, FormingExpressCommon, DiestressBM): all use the base writer `1405f5340` unchanged.
  The only class-specific behaviour the base writer reaches is vtable slot **+0x200**
  (`return 1` for 8 classes, `return 0` for 5, table in section 2). Slot +0x288 is the same function
  `1405f56c0` for every class, and the slot +0x290 it calls is the empty `1405f5860` for every class.
  None of these classes has any other MST-specific code: the operation `Properties` (HeatingTime,
  EnvTemp, ...) never reach the MST; they live in `SimCtrl.KEY`.
* **Legacy type names** `SfOprHeatExpress`, `SfOprAirConvect`, `SfOprHeatResting`,
  `SfOprHeatDwelling` (substring match, case sensitive) are created as class
  `SfOprHeatTransferExpress`, or `SfOprHeatTransferExpress2D` when the type also contains `2D`
  (`140046cc0`). They then write through the base writer like any HeatTransfer-family class.
  The class name stored at opItem+0x120 is the mapped name, and that is what `*!SIMTYP` prints
  (section 2.3; the shipped 2013 examples show the legacy name, saved by an older build: open question Q1).
* **Cogging** (`1407520c0` → `140752190`): one operation becomes a sequence of SIMULATION blocks
  driven by `Operations/TaskNNNNN/Cogging.MDT`, which is read when the operation's properties
  load (`140761c90`). Block kinds: PHT (pass heat transfer), PDWL (pass dwell), BDEF (bite
  deformation), BDCT (decompression), BDW (dwell), BHT (bite heat transfer), SHT (adaptive reheat),
  plus inline `CHKSTOP` sections between blocks for auto-bite and adaptive reheat.
* **Side files**: neither writer creates, writes or deletes any file. The only file I/O on this
  path is the read of `Cogging.MDT` (and, during the same load, optional geometry KEY files named
  inside the MDT, opened relative to the current directory; they do not affect the MST). Section 7.
* **Validation**: the Python prototype (`tools/mdt.py` + `tools/cogw.py`), checked against the
  cogging-specific lines of every corpus MST: **335 of 337 cogging operations, 3059 SIMULATION
  blocks, identical**. The 2 that differ are expected: `Cogging_Multiple_Ops_Lab1` (2013 build:
  `TRANS 1 0 0 0 0 0`) and `vdie` (its MDT is newer than its MST). The paths covered include
  auto-bite with CHKSTOP, four dies, heat transfer on and off, no manipulators, and a cogging
  operation as the first operation.

---------------------------------------------------------------------------------------------------

## 1. Call convention of vtable slot +0x280

Caller `140132100` (plain run) / `1401321a0` (DOE sample), see s1-02:

```
writer(SfOpr* self,            // the operation class object (140054e90(opItem))
       QTextStream* out,       // param_2
       Project* prj,           // param_3   (passed on to emitters; used for QDir::relativeFilePath)
       QString* dbName,        // param_4   (primary DB name, printed after DBREAD/GENDB)
       int* sim,               // param_5   running SIMULATION counter, pre-increment per block
       OpItem* op,             // param_6   project operation item (name +0x118, class +0x120 ...)
       int arg7,               // param_7   0 from the walk; used as DBREAD argument
       int arg8)               // param_8   0 from the walk; cogging: "write only block n" (see 4.10)
```

The emitters take `(out, prj, dbName, n, op)`. The fourth argument `n` is the SIMULATION number
when the base writer calls them, and the **pass index** when the cogging writer calls
`140134bf0/140134db0/1401360e0/140135220/140135da0`. In the five decompiles in s1-02 `a1.c`,
none of those emitters reads `n` (s1-02 to confirm).

---------------------------------------------------------------------------------------------------

## 2. HeatTransfer family (base writer 1405f5340)

### 2.1 Hooks the base writer reaches

`1405f5340` (spec: s1-02) calls exactly two virtuals on `self`: **+0x200** (choose
`140134250` DIM/NSTART-or-DBREAD versus a plain `1401343e0` DBREAD) and **+0x288** (the setup
lines). +0x288 calls **+0x290**. No other slot is called on `self` on this path; the emitters
make virtual calls only on other objects (DOE object +0x1b0/+0x1b8/+0x1c8, object items +0x208),
which s1-02 covers.

Resolved with `s1/vtables.tsv`:

| Class | +0x200 | +0x280 | +0x288 | +0x290 | +0x298 (not on MST path) |
|---|---|---|---|---|---|
| SfOprBase | 1405f79b0 → 0 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHeatTransfer | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHeatTransfer2D | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHeatTransferCommon | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHeatTransferExpress | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHeatTransferExpress2D | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprExpressBase | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprFormingExpressCommon | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 141423390 |
| SfOprForming | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprForming2D | 1413e5ef0 → 1 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprHTFurnace | 1405f79b0 → 0 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprPre | 1405f79b0 → 0 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprPre2D | 1405f79b0 → 0 | 1405f5340 | 1405f56c0 | 1405f5860 | 1405f5880 |
| SfOprDiestressBM | 1405f79b0 → 0 | 1405f5340 | 1405f56c0 | 1405f5860 | 14029a4b0 |
| (SfOprCogging, for reference) | 1405f79b0 → 0 | 1407520c0 | 1405f56c0 (unused) | 1405f5860 | 140759d50 |

`1413e5ef0`: `bool f(){ return true; }`. `1405f79b0`: `bool f(){ return false; }`.
`1405f5860`: `void f(...){}`.

Effect of +0x200 in `1405f5340` (from s1-02's decompile):

```cpp
if (!op->restart /*140064740: op+0x150*/) {
    if (self->vf200())                                   // HT family "1" classes
        emit_140134250(out, dbName, *sim, op, arg7);      // sim==1: "DIM    \t"<dim> + NSTART (when the
                                                          // op has an NSTART value); sim>1: DBREAD with
                                                          // the op's NSTART step if present, else arg7
    else                                                  // "0" classes
        emit_1401343e0(out, dbName, *sim, arg7);          // sim==1: nothing; sim>1: "DBREAD    \t"<arg7>
    self->vf288(out, prj, dbName, *sim, op);
    ...
}
```

So for HTFurnace, Pre, Pre2D and DiestressBM: no DIM/NSTART line in the first block, and the
operation's `NSTART/StartStepNo` is not used for DBREAD.

### 2.2 +0x288 = 1405f56c0 (all classes)

```cpp
void SfOprBase::writeSetup(QTextStream* out, Project* prj, QString* db, int n, OpItem* op)  // 1405f56c0
{
    emit_140134bf0(out, prj, db, n, op);   // MDSOBJ / TNOW / KFREAD SimCtrl.KEY
    emit_140134db0(out, prj, db, n, op);   // KFREAD material
    emit_140135220(out, prj, db, n, op, false);  // OBJTYP / KFREAD object / MTLGRP
    emit_140135da0(out, prj, db, n, op);   // CNTACT + KFREAD InterObject.KEY
    emit_1401360e0(out, prj, db, n, op);   // REMESH / KFREAD
    emit_1401363b0(out, prj, db, n, op);   // KFREAD <per-object key> (relative path)
    emit_140136810(out, prj, db, n, op);   // IMPEQP <obj> + equipment KEY
    emit_1401365f0(out, prj, db, n, op);   // KFREAD ObjPropertiesNNNNN.KEY
    this->vf290(out, prj, db, n, op);      // 1405f5860: empty for every class here
    emit_140135fc0(out, prj, db, n, op);   // "TLOC    \t"<t> when t > 0
}
```

`1401363b0` is in the shared-emitter range but was not in s1-02's first decompile. It is in
`e_d0c.c`: for each object i of the operation whose item says so (`14004d600`, `14004e0c0`) it
writes `"KFREAD    \t" << 1 << endl << relativeFilePath(14005da90(op,i)) << endl`.

### 2.3 Legacy type names (`140046cc0`, called by the moproj operation reader `14008ccf0`)

```cpp
QString cls = type, legacy;                       // auStack_1a0, auStack_1b0
if (type.contains("SfOprHeatExpress") || type.contains("SfOprAirConvect") ||
    type.contains("SfOprHeatResting") || type.contains("SfOprHeatDwelling")) {   // Qt::CaseSensitive
    cls = type.contains("2D") ? "SfOprHeatTransferExpress2D" : "SfOprHeatTransferExpress";
    legacy = type;
}
if (type == "SfOprMultiBlow") cls = "SfOprMultiBlow2D";
SfOpr* c = factory_140047c00(model, cls);         // class object
...
opItem->setClass(cls);        // 140050c80 -> opItem+0x120   (what *!SIMTYP prints, 140050d00)
opItem->setSubType(legacy);   // 140050d50 -> opItem+0x128   (overwritten later by <SubType>)
```

Corpus types reaching this mapping: `SfOprAirConvect` (2), `SfOprAirConvect2D` (3),
`SfOprHeatResting2D` (2), `SfOprHeatExpress` (1). All of them are in the shipped vendor examples
(2013 saves), whose MSTs print the legacy name in `*!SIMTYP`. Per the code above, this build
prints `SfOprHeatTransferExpress` or `SfOprHeatTransferExpress2D` instead (Q1). Everything else in
these blocks is base-writer output.

### 2.4 Validation: HEAT1 (copy in `samples/HEAT1`, MST and moproj saved together)

Two SfOprHeatTransfer operations, flow order Task1, Task2.

| Lines | Writer |
|---|---|
| blank, `*---*`, `SIMULATION\t1`, `*!SIMTYP  \tSfOprHeatTransfer`, `*---*` | 140131c10 |
| `DIM    \t3`, `NSTART    \t-1` | 140134250 (+0x200 = 1, sim 1, `<NSTART><StartStepNo>-1`) |
| `MDSOBJ    \t  0       0       0    0.0000000000E+000`, `TNOW\t   \t  -1E21       0       1`, `KFREAD    \t1`, `Operations/Task00001/SimCtrl.KEY` | 140134bf0 |
| `KFREAD    \t1`, `Materials/Material00001.KEY` | 140134db0 |
| `OBJTYP\t\t1\t0`, `KFREAD    \t1`, `Objects/Object00001.KEY`, `MTLGRP    \t1\t0\t1` | 140135220 |
| `CNTACT    \t0`, `KFREAD    \t1`, `Operations/Task00001/InterObject.KEY` | 140135da0 |
| `IMPEQP    \t1`, `Equipments/Equipment00001.KEY` | 140136810 |
| (none: object type 1, so no ObjProperties line; TLOC 0 not written) | 1401365f0, 140135fc0 |
| `INICTC    \t1`, `GENCTC    \t0` | 140136040 (ActionKeywords, TLOC and blank lines dropped) |
| `OPRNAM`, `HeatTransfer`, `SIMNAM`, `HeatTransfer` | 140134730, 1401347e0 (op name +0x118 twice) |
| `CURSIM    \t1\t1\t0` | 140134900 (sim, flow index, 0) |
| `GENDB     \t2\t1\t0`, `HEAT1.DB` | 140134a10 (sim 1) |
| Block 2: `DBREAD    \t0` + `HEAT1.DB`, then MDSOBJ/TNOW/SimCtrl, CNTACT/InterObject, `KFREAD ... Task00002/ObjProperties00001.KEY` (object Id -2 = from DB), INICTC/GENCTC, OPRNAM/SIMNAM `HeatTransfer (2)`, `CURSIM 2 2 0`, `GENDB 1 1 2` | as above (no material/object/IMPEQP lines: object from DB, equipment -1) |
| trailer | 140131d10 |

The same reading explains the four HeatTransfer blocks of COG_P1_B1. Their `OBJPOS` lines
(`ENTER`, `EXIT`) are ActionKeywords text written verbatim by 140136040. Corpus check of every
base-writer block in the 48+ project MSTs: the first block starts with `DIM` for the "1" classes
and later blocks with `DBREAD`. `OPRNAM` always precedes `SIMNAM` (the reverse of cogging). There
are two exceptions, both in old vendor examples: an `SfOprForming` and an `SfOprAirConvect` first
block that start with `MDSOBJ` (no NSTART value, per 140134250).

---------------------------------------------------------------------------------------------------

## 3. Cogging object chain and data model

```
opItem (+0x10) ──> SfOprCoggingProperties  (dynamic_cast from SfOprBaseProperties, RTTI 0x143579b60 -> 0x1437cf500;
                    │                       if the cast fails the writer emits nothing)
                    +0x40 ──> CogModel (0x550 bytes, ctor 140761310/1407613b0)    "model" below
                               +0x540 ──> SfOprCogData (ctor 140767a20)          "cog" below
                               +0x548 ──> SfOprCoggingProperties (back pointer; dir via 1405f8d90/140050df0)
```

`1405ef320(self, op)`: `op ? op : self->+0x20` (current op set by the walk through `1405ef280`),
returns `op->+0x10` (`140051230`).

### 3.1 CogModel fields (process data; read by 140761d90, defaults 1407613b0)

| Off | Type | MDT key (Process Data) | Default | Read by writer as |
|---|---|---|---|---|
| +0x000 | int | Process_Type | (from 1407656d0/1407614e0, not traced) | `1413ee240`: 0 → FORCE/STROKE lines, 1 → DSTROKE feed |
| +0x004 | int | Temperaure_Type | " | `14142ff20`: 0 disables PHT/PDWL/heat sub-blocks/SHT |
| +0x008 | int (byte read) | Use_Mandrel | " | `14075dc00` (read in BDEF, unused) |
| +0x014 | int | Use_DifferentDies | " | not used |
| +0x015 | byte | RotationalSymmetry (`%d` into +0x15 overwrites +0x15..+0x18) | " | `14075dca0`, CHKSTOP line 4 |
| +0x4cc | byte (`%d` → 4 bytes) | Use_Adaptive_Reheat | 0 | `140765a10` return → SHT + CHKSTOP |
| +0x4d0 | int | Checking_Method | 0 | CHKSTOP |
| +0x4d8 | double | Temperature_Range 1st (`%lg`) | 0 | CHKSTOP |
| +0x4e0 | double | Temperature_Range 2nd | 0 | SHT `ENVTMP` |
| +0x4e8 | double | Adaptive_Heating_Time | 0 | SHT `DTMAX` |
| +0x4f0 | int | Solver_Type | 0 | not used |
| +0x4f8 | double | Swaging_Stroke | 0 | `14075dd30` (DSMAX_NSTEP, dead) |
| +0x500 | int | Num_Steps_DSMAX | 4 | `14075dd50` (DSMAX_NSTEP, dead) |
| +0x504 | byte | Heat_Transfer (`%d`) | 0 | `14075dc20` |
| +0x505 | byte | Remaining_Cycle_Time_Dwell (`%d` → writes +0x505..+0x508) | 0 | `14075dc40` |
| +0x508 | double | Cycle_Time (`%lg`, then **low byte overwritten** by the next field's high byte) | 0 | `14075dd10` (TCYCLE) |
| +0x510 | double | Dwell_Time | 0 | `14075dc60` |
| +0x518 | double | Decompression_Time | 0 | `14075dc80` |
| +0x520 | int | Bottom_Fixed | 0 | `140765b70` (BHT die filter) |
| +0x528 | double | Clamp_Force | 0 | not used |
| +0x530 | double | Clamp_Speed | 0 | not used |
| +0x538 | byte | (never set except ctor = 0) | 0 | `14075dd70`: DSMAX_NSTEP gate, always false |
| +0x540 | SfOprCogData* | | | `14075dcb0` |

### 3.2 SfOprCogData (`cog`, ctor 140767a20)

| Off | Content |
|---|---|
| +0x000 | char[100] PRBID (`%s %s`), default "COGGING" |
| +0x064 | char[] TITLE (`%s %[^\n]s`), default "COGGING" |
| +0x0c8 | byte UseManip (Simulation Parameters, ver > 0xc1c, `!= 0`), default 1 → `14076b060`, `140765960` |
| +0x0cc | int number of reheats (default 1) → `140768d20`, `140765740` |
| +0x0d0 | REHEAT[nR] (400 bytes each) |
| +0x0d8 | BILLET block (`14044b5d0`) |
| +0x1170 | DIE block (`14075dcf0`) |
| +0x2e20 | MANIP block (`14075dcd0`) |

REHEAT (400 bytes): +0 int nPass, +8 PASS* (0x108 each), +0x10/+0x90/+0x110 three HEAT
(0x80 each, not used by the writer).

PASS (0x108, reader 140792810, ctor 140791580):

| Off | Type | MDT key | Used for |
|---|---|---|---|
| +0x00 | int | AutoBite (ver > 0xc1b) | `!=0`: automatic bites |
| +0x04 | int | NoBite (0 read → 1) | bite count (`140768db0`) |
| +0x08 | double←float | RotAng | BDEF bite 1 `OBJPOS 1 3` |
| +0x10 | double←float | BiteAngle | BDEF bite > 1 `OBJPOS 1 3` |
| +0x18 | double←float | MinDieDis | `MDSOBJ 2 3 3`, DSTROKE pass > 1, CHKSTOP |
| +0x20 | BITE* | | bites |
| +0x28 | HEAT | pass heat transfer (PHT, PDWL, SHT steps) | |
| +0xa8 | int | DiePosMethod | |
| +0xb0 | double←float | DieStrtPos | |
| +0xb8 | double←float | DieStopPos | |
| +0xc0 | int | DieStrtNod | |
| +0xc4 | int | DieStopNod | CHKSTOP when DiePosMethod == 1 |
| +0xc8 | int | DieMovDir (ctor -1) | |
| +0xd0/+0xd8 | double←float | LeftManDis / RightManDis | not used |
| +0xe8 | double←float | HorizDieShift (ver > 0x277d) | |
| +0xf0 | int | BitesToSkip (ver > 10999) | TCYCLE 2nd field |
| +0xf8 | double←float | RadialFeedPerBite | DSTROKE (Process_Type 1) |
| +0x100 | double←float | DwellTime | PDWL |

HEAT (0x80, reader 140797ef0, ctor 140797a10, then copy of the global default slot
`0x143db1a20 + k*0x80` by 140797d50; the five global slots are made by 140797a10 in 1407979c0):
+0 int Active, +4 int NSTEP, +8 int STPINC, **+0xc int SOLMTT = 1 (never read from the MDT)**,
+0x10 double←float TMAX, +0x18 double←float DTPMAX, +0x20 double IHTCOF (`%lf`),
+0x28 double IHTCOF2 (ver > 0xce4), +0x30 char[0x4f] UserKey ("---" → " ").

BITE (0x140, deformation reader 140790d80, ctor 140790700; the global default bite is copied
in by 1407908e0): +0 NSTEP, +4 STPINC, +8 PDIE, +0x10 SMAX, +0x18 LMAX, +0x20 DTSUB,
+0x28 StepMod, +0x30 DSMAX, +0x38 DTMAX, +0x40 ClampForce, +0x48 FricTyp, +0x50 FRCFAC,
+0x58/+0x60 IHTCOF, +0x68 UserKey (text after the last '/'), **+0xb8 NomBite**,
**+0xc0 HEAT** (bite heat transfer: +0xc4 NSTEP, +0xc8 STPINC, +0xcc SOLMTT (=1), +0xd0 TMAX).

BILLET (`cog+0xd8`, reader 140796f90): +0x408 GeoTyp, +0x000 GeoMod, +0x004 GeoFil,
+0x410 Length, +0x418 Dia, +0x420 InDia, +0x428 Height, +0x430 Width, +0x438 Radius,
+0x440 Layers, +0x858 MGNELM, +0x860 MGSIZR, +0x868 MGTELM, +0xc70 TMPBIL, +0xc78 BccMod,
+0xc7c MergeL, +0xc90 KeyBil, +0xc88 AxialSub, +0xc80 AxialType, +0x1090 DoBilStraight,
+0x454 MartMod, +0x450 MeshMod.

DIE (`cog+0x1170`, reader 140795760): +0x408 GeoTyp, +0 GeoMod, +4 GeoFile, +0x410 Length,
+0x418 Width, +0x420 Height, +0x428 Radius, +0x430 Layers, +0x858/+0x860/+0x868 mesh,
+0xc6c DieMov, +0xc80 TMPDIE, +0xc88 SPDDIE, +0xca0 CYCLES, +0xc90 TOTSTROKE, +0xc98 RODLENGTH,
NoDies → global 0x1437e3440 and **+0x14a8 = (NoDies == 4)**, +0xca8/+0x10a8/+0x14ac/+0x18ac
KeyDie1..4, +0xc70 RefPos, +0x438 Overlap, +0x440 StartPos, +0xc74 MovDir, **+0xc78 MovType**,
+0x448 StopPos, +0x454, +0x450.

MANIP (`cog+0x2e20`, reader 140793cc0, ver > 0xc1c): +0x408 GeoTyp, +0 GeoMod, +4 GeoFile,
**+0x410 Length**, +0x418 Width, +0x420 Height, +0x428 Radius3, +0x450 Layers, +0x8a0/+0x8a8/+0x8b0
mesh, +0xcb8 TMPDIE, +0xcc0/+0x10c0/+0x14c0/+0x18c0 KeyMan1..4, **+0x458 StartPos**,
**+0x460 LeftDis**, **+0x468 RightDis**, +0x470 UseMstMan (read `%f`, stored `(int)`),
**+0x8b4 UseSpring**, +0x478 Stiffness, +0x480 Preload, +0x488 MaxDisp, +0x440 Width1,
+0x448 Height1, +0x430 Radius1, +0x438 Radius2, +0x49c MartMod, +0x498 MeshMod
(ver < 0x2774 has a different order, see 140793cc0).

### 3.3 Accessors used by the writer (all trivial unless shown)

| Address | Meaning |
|---|---|
| 1401cc6b0 | props → model (+0x40) |
| 14075dcb0 | model → cog (+0x540) |
| 140765740 / 140768d20 | number of reheats: cog+0xcc |
| 140765770 / 140768d40 (r) | nPass of reheat r: `*(int*)(cog->reheats + (r-1)*400)` |
| 1407657b0 / 140768db0 (r,p) | NoBite of pass: `pass+4` |
| 1407692b0 (r,p) | PASS*: `reheat(r)->passes + (p-1)*0x108` |
| 140767bc0 (r,p,b) | BITE*: `pass->bites + (b'-1)*0x140`, b' = (AutoBite ? (b>1 ? 1 : b) : b) |
| 140765b00 | die count: `die+0x14a8 == 1 ? 4 : 2` |
| 140765f30 (i) | die object id: 0→2, 1→3, 2→8, 3→9, else 0 |
| 140765fa0 (i) | manipulator object id: 0→4, 1→5, 2→6, 3→7, else 0 |
| 140766010 (i, v[3]) | die direction: 0→(0,0,-1), 1→(0,0,1), 2→(0,-1,0), 3→(0,1,0), else (0,0,0) |
| 140766140 (i, v[3]) | manipulator direction: (1,0,0),(1,0,0),(-1,0,0),(-1,0,0) (result unused) |
| 140765960 / 14076b060 | UseManip: byte cog+0xc8 |
| 140765a10 | out Checking_Method(+0x4d0), TR0(+0x4d8), TR1(+0x4e0), AdaptiveTime(+0x4e8); returns byte +0x4cc |
| 140768650 (cog, a, b) | billet/die/manip size, below |
| 140768770 (r,p) | automatic bite count, below |
| 1407682a0 (r,p,b) | die X translation ("getTranslationX"), below |

```cpp
double cogSize_140768650(Cog* c, int a, int b) {
    if (a == 1) {                                    // billet
        if (b == 0) return billet.Length;            // cog+0x4e8
        switch (billet.GeoTyp) {                     // cog+0x4e0
            case 0: return billet.Dia;               // +0x4f0
            case 1: return billet.Width;             // +0x508
            case 2: return billet.Height;            // +0x500
            default: return billet.Width; }          // +0x508
    }
    if (a == 2 && b == 0) return die.Length;         // cog+0x1580
    if (a == 4 && b == 0) return manip.Length;       // cog+0x3230
    return 0.0;
}

int autoBites_140768770(Cog* c, int r, int p) {
    double L = billet.Length;                        // initial value cog+0x4e8
    PASS* P = pass(r,p);                             // error text 142680348 if invalid
    if (P->DiePosMethod == 1)      L = fabs(P->DieStrtPos - P->DieStopPos);
    else if (P->DiePosMethod == 0) L = ((1.0 - P->DieStrtPos) - P->DieStopPos) * L;
    else                           L = (L - P->DieStrtPos) - P->DieStopPos;
    double nom = P->bites[0].NomBite;                // +0xb8
    if (nom <= 0.0) { fputs(stderr_142680380); return 1; }   // stderr only
    double x = L / nom; int n = (int)x;
    if (n == 0) n = 1;
    return (x - (double)n < 0.5) ? n + 1 : n + 2;
}

double translationX_1407682a0(Cog* c, int r, int p, int b) {
    PASS* P = pass(r,p); int d = P->DieMovDir; double L = cogSize(c,1,0);
    double x;
    if (P->DiePosMethod == 1) x = ((d == pass(r,1)->DieMovDir) ? P->DieStrtPos : P->DieStopPos) + 0.0;
    else if (P->DiePosMethod == 0) x = (double)d * P->DieStrtPos * L + 0.0;
    else x = (double)d * P->DieStrtPos + 0.0;
    if (P->AutoBite == 0) {
        for (int i = 0; i < b - 1; i++) x = x + (double)d * P->bites[i].NomBite;
        if (c->UseManip && d == 1 && P->DiePosMethod != 1) x = x + -1.0 * L;
    } else if (d == -1)            x = x + (double)(-(b - 1)) * P->bites[0].NomBite;
    else if (P->DiePosMethod == 1) x = x + (double)(b - 1) * P->bites[0].NomBite;
    else                           x = x + -1.0 * L + (double)(b - 1) * P->bites[0].NomBite;
    return x;
}
```

`140050bc0(op)` ("first operation", called `firstOp` below): looks up the operation's
flow-chart item (`140055150` → `140055050`/`1400b5a70`) and returns
`140086720(item, op+0x108) == -1`. Corpus evidence: true when the cogging operation is the first
operation of the project (NewProject_1, brick_mesh_01). Exact meaning: s1-01/s1-02 (Q4).

---------------------------------------------------------------------------------------------------

## 4. Cogging writer

Notation: `OUT << a << b << endl` is one `QTextStream` chain on `out`; `endl` is `"\n"` + flush
(`140041bc0`). `g(x)` is `QTextStream << double` with the stream's default settings
(observed `%g`-like, 6 significant digits, exponent at least 2 digits as in `1e-05`/`1e+09`,
**`-0.0` printed as `0`**; s1-06 owns the exact algorithm). Integers print as decimal.
Each `"\t"` is a separate `.rdata` literal `09 00`. All other literals are in `literals.txt`.
`SEP` = the 75-byte literal `*` + 73×`-` + `*` (e.g. 0x1425e3f90).

### 4.1 1407520c0 (slot +0x280)

```cpp
void SfOprCogging::writeMst(QTextStream* out, Project* prj, QString* db, int* sim, OpItem* op, int arg7, int arg8)
{
    SfOprBaseProperties* bp = props_1405ef320(this, op);
    SfOprCoggingProperties* cp = dynamic_cast<SfOprCoggingProperties*>(bp);   // __RTDynamicCast 14146f8fc
    if (cp) writeBlocks_140752190(this, out, prj, db, sim, op, cp, arg7, arg8);
}
```

### 4.2 140752190: block loop

```cpp
void writeBlocks(self, out, prj, db, int* sim, OpItem* op, CogProps* cp, int dbStep /*arg7*/, int only /*arg8*/)
{
    CogModel* m = cp->model;                                  // 1401cc6b0
    bool first = true;                                        // auStack_ae: "next block is the op's first"
    bool firstOp = isFirstOp_140050bc0(op);                   // uStack_b0
    int  chkMethod; double tr0, tr1, aht;
    bool adaptive = m->adaptive(&chkMethod, &tr0, &tr1, &aht); // 140765a10
    bool bDecomp = false, bDwell = false, bBHT = false;
    bool heat = m->HeatTransfer;                              // byte +0x504
    double dwell = m->Dwell_Time;                             // +0x510
    double decomp = m->Decompression_Time;                    // +0x518
    int  ttype = m->Temperaure_Type;                          // +4
    if (heat && ttype != 0) {
        if (decomp > 1e-6) bDecomp = true;                    // 1e-6 = .rdata 142157420
        if (dwell  > 1e-6) bDwell  = true;
        if (m->Remaining == 0) bBHT = true;                   // byte +0x505
        else { bDwell = true; dwell = 0.0; }
    }
    int state = 0;                                            // uStack_5c: 0 none, 1 BDEF, 2 dwell, 3 heat
    int nR = numReheats(m);
    for (int r = 1; r <= nR; r++) {                           // (loop header iStack_44)
      int nP = numPasses(m, r);
      for (int p = 1; p <= nP; p++) {
        bool manipFirst = true;                               // auStack_32
        int  nb = 0, nAuto = 0, chkSim = 0; bool autoB = false;
        if (p > 1) first = false;
        PASS* P = pass(m, r, p);
        if (P->HT.TMAX > 1e-6 && ttype != 0 && (++*sim, only == 0 || *sim == only)) {
            writePHT_1407532b0(..., r, p, &first, &firstOp, state, &manipFirst);
            if (only) break;                                  // leaves the pass loop
            state = 3;
        }
        if (P->DwellTime > 1e-6 && ttype != 0) {
            ++*sim;
            if (only == 0 || *sim == only) { writePDWL_140754250(..., r, p, &first, &firstOp, state, &manipFirst);
                                             if (only) break; }
            state = 2;
        }
        if (P->AutoBite == 0) nb = P->NoBite;                  // 1407657b0
        else { nAuto = autoBites(m, r, p); nb = 2*nAuto - 1;
               chkSim = *sim + 1 + nb * (bBHT + 1 + bDecomp + bDwell); autoB = true; }
        for (int b = 1; b <= nb; b++) {
            if (b > 1) first = false;
            BITE* B = bite(m, r, p, b);
            if (!B) continue;
            ++*sim;
            if ((only == 0 || *sim == only)) { writeBDEF_140755b90(..., r, p, b, &first, &firstOp, state, &manipFirst);
                                               if (only) break; }
            state = 1;
            if (heat) {
                if (bDecomp && (++*sim, only == 0 || *sim == only)) {
                    writeBDCT_140758fd0(..., r, p, b, decomp); if (only) break; }
                if (bDwell) { ++*sim;
                    if (only == 0 || *sim == only) { writeBDW_140758620(..., r, p, b, dwell); if (only) break; }
                    state = 2; }
                if (bBHT) { ++*sim;
                    if (only == 0 || *sim == only) { writeBHT_140755170(..., r, p, b, state); if (only) break; }
                    state = 3; }
            }
            if (only == 0 && autoB && b > 1) writeAutoChkstop(out, ...);   // 4.9
        }
        if (adaptive && (++*sim, only == 0 || *sim == only)) {
            if (!autoB) writePassChkstop(out, *sim, tr0, chkMethod);        // 4.9
            writeSHT_1407595c0(..., r, p, tr1, aht);
            if (only) break;
        }
      }
    }
}
```

Notes, from the decompile:
* `*sim` is incremented for every block that would exist, even when `only` filters it out.
* `break` exits the **pass** loop only (not the reheat loop). With `only == 0` (the MST walk)
  there is never a break.
* BDCT does not change `state`. PHT sets `state = 3` only when that PHT block was written.
* The bite count with `AutoBite != 0` is `2*nAuto - 1` blocks of BDEF, and every BDEF uses bite 1's
  data (140767bc0).

Block count per operation (with `only == 0`):
`Σ_passes [ PHT? + PDWL? + nb·(1 + heat·(bDecomp + bDwell + bBHT)) + adaptive ]`.
COG_P1_B1: 1 pass, PHT (TMAX 1.11), no PDWL, 1 bite, heat=1, bBHT only → 3 blocks (sims 5, 6, 7).

### 4.3 Common block head and tail used by the sub-writers

```cpp
// head used by PHT/PDWL/BDEF (param first/firstOp):
emit_140131c10(out, *sim, op);                         // blank, SEP, "SIMULATION\t"<n>, "*!SIMTYP  \t"<class>, SEP
if (!*first) { emit_1401343e0(out, db, *sim, dbStep); emit_140134bf0(out, prj, db, p, op); }
else {
    *first = false;
    if (!*firstOp) emit_1401343e0(out, db, *sim, dbStep);       // DBREAD (sim != 1)
    emit_140134bf0(out, prj, db, p, op);                          // MDSOBJ/TNOW/SimCtrl
    emit_140134db0(out, prj, db, p, op);                          // materials
    emit_1401360e0(out, prj, db, p, op);                          // REMESH/KFREAD
    emit_140135220(out, prj, db, p, op, false);                   // OBJTYP/object/MTLGRP
    emit_140135da0(out, prj, db, p, op);                          // CNTACT/InterObject
}
// head used by BDCT/BDW/BHT/SHT: 140131c10; 1401343e0 (always); 140134bf0.

// tail used by every block:
//   [140136040 action keywords]  (not in BDCT)
//   ... block specific ...
emit_140134680(out, op);                               // KFREAD <user key> when set
OUT << SIMNAM(name)                                    // 1401347e0: "SIMNAM" endl name endl
emit_140134730(out, op->name /*140050ac0: op+0x118*/); // "OPRNAM" endl name endl
emit_140134900(out, *sim, flowIndex_140050b20(op), 0 /*140050b10*/);   // CURSIM
emit_140134a10(out, db, *sim, 2, false);               // "GENDB     \t1\t1\t2" + db  (sim 1: "2\t1\t0")
```

The first-block head writes no `IMPEQP`, no ObjProperties KEY lines and no `DIM`/`NSTART`
(cogging never calls 140134250/1401365f0/140136810/1401363b0). `SIMNAM` comes before `OPRNAM`
in every cogging block. Name format: `QString(fmt).arg(r,0,10,' ').arg(p,0,10,' ')[.arg(b,0,10,' ')]`,
so plain decimal.

### 4.4 Shared helper lines

```cpp
// manipulator start position (when UseManip and manipFirst):
v = manip.StartPos + manip.Length / 2.0;                       // cog+0x2e20: +0x458, +0x410; 2.0 = 142101520
OUT << "OBJPOS" << "\t" << 4 << "\t" << 13 << "\t" << 0 << "\t" << -1 << "\t" << g(0.0) << "\t" << g(0.0)
    << "\t" << 1 << "\t" << 2 << "\t" << g(v) << "\t" << 1 << endl;
OUT << 5 << endl;
OUT << "OBJPOS" << "\t" << 6 << "\t" << 13 << "\t" << 0 << "\t" << 1 << "\t" << g(0.0) << "\t" << g(0.0)
    << "\t" << 1 << "\t" << 2 << "\t" << g(v) << "\t" << 1 << endl;
OUT << 7 << endl;
//   → "OBJPOS\t4\t13\t0\t-1\t0\t0\t1\t2\t315.8\t1\n5\nOBJPOS\t6\t13\t0\t1\t0\t0\t1\t2\t315.8\t1\n7\n"

// die retraction (filter f differs per block):
half = -cogSize(c, 1, 2) / 2.0;            // XOR sign bit 1421d3d30 then divide by 2.0
for (i = 0; i < nDies; i++) if (f(dieId(i))) {
    dir = dieDir(i);
    OUT << "OBJPOS " << "\t" << dieId(i) << "\t" << 11 << "\t" << g(half*dir.x) << "\t" << g(half*dir.y) << "\t" << g(half*dir.z) << endl;
}
//   → "OBJPOS \t2\t11\t0\t0\t250" (note the space after OBJPOS; x and y are -0.0 and print "0")

TRANS line (all blocks except BDEF): OUT << "TRANS " << "\t" << 1 << "\t" << 0 << "\t" << -1 << "\t" << -1 << "\t" << -1 << "\t" << -1 << endl;
```

### 4.5 PHT: 1407532b0 (pass heat transfer)

```cpp
head(first/firstOp);
if (*manipFirst) { *manipFirst = false;
    if (UseManip) { manipStart(v);
        OUT << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 0 << "\t" << g(0.001) << "\t" << 1 << endl;
        OUT << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 0 << "\t" << g(0.001) << "\t" << 1 << endl;
        OUT << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 1 << "\t" << g(0.001) << "\t" << 1 << endl;
        OUT << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 1 << "\t" << g(0.001) << "\t" << 1 << endl; } }
if (state != 3) retract(id == 3 || state != 2);
H = P->HT;      // pass+0x28
OUT << "NSTEP"  << "\t" << H.NSTEP  << endl;          // pass+0x2c
OUT << "STPINC" << "\t" << H.STPINC << endl;          // pass+0x30
OUT << "SOLMTT" << "\t" << H.SOLMTT << endl;          // pass+0x34 (always 1)
OUT << "DTMAX " << "\t" << g(H.TMAX / (double)H.NSTEP) << endl;   // pass+0x38
OUT << "DSMAX " << "\t" << 0 << endl;
emit_140136040(...);                                   // action keywords
TRANS;
tail("RH%1-PS%2-PHT");
```

### 4.6 PDWL: 140754250 (pass dwell)

As PHT up to and including the manipulator lines. Then `if (state != 3)` retract **all** dies
(no filter), then

```cpp
OUT << "OBJPOS" << "\t" << 3 << "\t" << 12 << "\t" << g(1e-05) << "\t" << g(0.0) << "\t" << g(0.0)
    << "\t" << g(1.0) << "\t" << 1 << "\t" << g(0.0) << "\t" << g(0.0) << endl;  // "OBJPOS\t3\t12\t1e-05\t0\t0\t1\t1\t0\t0"
NSTEP/STPINC/SOLMTT from P->HT; "DTMAX \t" g(P->DwellTime / H.NSTEP); "DSMAX \t0";
140136040; TRANS; tail("RH%1-PS%2-PDWL");
```

### 4.7 BDEF: 140755b90 (bite deformation)

```cpp
head(first/firstOp);
nD = dieCount; B = bite(r,p,b);
int dbs = billet.DoBilStraight;                        // cog+0xd8+0x1090
if ((dbs == 1 && !(b == 1 && p == 1)) || (dbs == 2 && b == 1 && p > 1)) {
    OUT << "EXECMD" << "\t" << "3" << endl;
    OUT << "COG_GEO_MANIP.EXE" << "\t" << QFileInfo(*db).fileName() << endl;
    OUT << "KFREAD" << "\t" << 1 << endl;
    OUT << "COG_STRAIGHT_BILLET.KEY" << endl; }
if (billet.AxialSub != 0.0 && b > 1) {                  // +0xc88
    double v = (billet.AxialType == 0) ? billet.AxialSub : -billet.AxialSub;   // +0xc80
    OUT << "GENSUB" << "\t" << 1 << "\t" << g(v) << endl;
    OUT << "EXECMD" << "\t" << "3" << endl; OUT << "DEF_BRK.EXE" << endl;
    OUT << "KFREAD" << "\t" << 1 << endl;  OUT << "NEWBRICK.KEY" << endl; }
double tx = translationX(r,p,b);
bool c13 = false;
auto rotLine = [&](double a) {
    OUT << "OBJPOS" << "\t" << 1 << "\t" << 3 << "\t" << g(0) << "\t" << g(0) << "\t" << g(0) << "\t" << g(1.0)
        << "\t" << g(0) << "\t" << g(0) << "\t" << g(a);
    if (UseManip) { OUT << "\t" << 4 << endl; for (i = 0..3) OUT << manipId(i) << "\t"; }
    OUT << endl; };                                     // → "OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t55.55\t4\n4\t5\t6\t7\t\n"
if (b == 1) {
    if (P->DiePosMethod != 3 && (p > 1 || !*firstOp)) c13 = true;
    if (P->RotAng != 0.0) rotLine(P->RotAng);
} else tx = tx - translationX(r,p,b-1);
if (!c13)
    OUT << "OBJPOS" << "\t" << 2 << "\t" << 1 << "\t" << g(tx) << "\t" << g(0) << "\t" << g(0) << "\t" << g(0)
        << "\t" << g(0) << "\t" << g(0) << "\t" << g(0);
else {
    OUT << "OBJPOS" << "\t" << 2 << "\t" << 13 << "\t" << 0 << "\t" << P->DieMovDir << "\t" << g(0) << "\t" << g(0)
        << "\t" << 1 << "\t" << P->DiePosMethod << "\t";
    if (P->DiePosMethod == 1) OUT << (int)(P->DieMovDir != pass(r,1)->DieMovDir);
    else OUT << g(P->DieStrtPos); }
OUT << "\t" << nD - 1 << endl;
for (i = 1; i < nD; i++) OUT << dieId(i) << "\t";
OUT << endl;                                            // → "3\t" (or "3\t8\t9\t")
if (P->BiteAngle != 0.0 && b > 1) rotLine(P->BiteAngle);
double feed = P->RadialFeedPerBite;                     // +0xf8
if (b == 1) {
    if (p > 1) { feed = (pass(r,p-1)->MinDieDis - P->MinDieDis) / 2.0;
                 for (i < nD) OUT << "DSTROKE" << "\t" << dieId(i) << "\t" << g(feed) << endl; }
    double h = P->HorizDieShift;                        // +0xe8
    if (h != 0.0) {
        if (h > 1e-6) {          // dies 0,1 move, partners 8,9 stroke
            OUT << "OBJPOS " << "\t" << 2 << "\t" << 11 << "\t" << g(h*dir0.x) << "\t" << g(h*dir0.y) << "\t" << g(h*dir0.z) << endl;
            OUT << "OBJPOS " << "\t" << 3 << "\t" << 11 << "\t" << g(h*dir1.x) << "\t" << g(h*dir1.y) << "\t" << g(h*dir1.z) << endl;
            OUT << "DSTROKE" << "\t" << 8 << "\t" << g(-h) << endl; OUT << "DSTROKE" << "\t" << 9 << "\t" << g(-h) << endl;
        } else if (h != 1e-6) {  // h < 1e-6 (as compiled: h <= 1e-6 && h != 1e-6)
            OUT << "OBJPOS " << "\t" << 8 << "\t" << 11 << ... dir2 ...; OUT << "OBJPOS " << "\t" << 9 << "\t" << 11 << ... dir3 ...;
            OUT << "DSTROKE" << "\t" << 2 << "\t" << g(-h) << endl; OUT << "DSTROKE" << "\t" << 3 << "\t" << g(-h) << endl;
        } }
} else if (m->Process_Type == 1 && feed > 0.0)
    for (i < nD) OUT << "DSTROKE" << "\t" << dieId(i) << "\t" << g(feed) << endl;
retract(id == 3 || state != 2);                       // (dead flag b168 = 0 in the filter)
OUT << "OBJPOS" << "\t" << 2 << "\t" << 12 << "\t" << g(1e-05) << "\t" << g(0) << "\t" << g(0) << "\t" << g(0)
    << "\t" << 1 << "\t" << g(0) << "\t" << g(0) << "\t" << nD - 1 << endl;
for (i = 1; i < nD; i++) OUT << dieId(i) << "\t";  OUT << endl;
if (m->Process_Type == 0) for (i < nD) {
    OUT << "FORCE" << "\t" << dieId(i) << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl;
    if (die.MovType == 1) OUT << "STROKE " << "\t" << dieId(i) << "\t" << 0 << "\t" << 0 << "\t" << 0 << endl; }
if (UseManip) {
    if (*manipFirst) { *manipFirst = false; manipStart(v); }     // only the two "13" lines + 5/7
    OUT << "OBJPOS" << "\t" << 4 << "\t" << 14 << "\t" << 5 << "\t" << 0 << "\t" << g(1e-05) << "\t" << 1 << endl;
    OUT << "OBJPOS" << "\t" << 6 << "\t" << 14 << "\t" << 7 << "\t" << 0 << "\t" << g(1e-05) << "\t" << 1 << endl;
    OUT << "GENAXS" << "\t" << 7 << "\t" << g(manip.LeftDis) << "\t" << g(manip.RightDis) << endl; }
OUT << "URZ" << "\t" << 1 << "\t1 0.0 0.0 0.0" << endl;     // literal 1425e47b8 = 09 31 20 30 2e 30 20 30 2e 30 20 30 2e 30
OUT << "1\t 0.0 0.0 0.0" << endl;                          // literal 1425e47c8 = 31 09 20 30 2e 30 ...
if (UseManip && manip.UseSpring != 0)                       // +0x8b4
    for (i = 0..3) OUT << "STROKE " << "\t" << manipId(i) << "\t0.0 0.0 0.0" << endl;   // (140766140 called, result unused)
emit_140136040(...);
OUT << "MDSOBJ" << "\t" << 2 << "\t" << 3 << "\t" << 3 << "\t" << g(P->MinDieDis) << endl;
if (die.MovType == 4) OUT << "SMAX" << "\t" << 0 << "\t" << 0 << "\t" << g(die.TOTSTROKE) << endl;
if (m->byte538 /*never set*/ && m->Swaging_Stroke != 0.0 && m->Num_Steps_DSMAX != 0)
    OUT << "DSMAX_NSTEP" << "\t" << g(m->Swaging_Stroke) << "\t" << m->Num_Steps_DSMAX << endl;   // dead
tail("RH%1-PS%2-BDEF%3" + (P->AutoBite ? "(Auto)" : ""));
```

### 4.8 BDCT / BDW / BHT / SHT

```cpp
// BDCT 140758fd0 (decompression time dc)
head2; H = B->HT (bite+0xc0);
OUT << "NSTEP" << "\t" << H.NSTEP << endl;  OUT << "STPINC" << "\t" << H.STPINC << endl;  OUT << "SOLMTT" << "\t" << H.SOLMTT << endl;
OUT << "DTMAX " << "\t" << g(dc / H.NSTEP) << endl;  OUT << "DSMAX " << "\t" << 0 << endl;
TRANS;  tail("RH%1-PS%2-BDCT%3");                              // no action keywords, no die lines

// BDW 140758620 (dwell dw; dw == 0 in "remaining cycle time" mode)
head2; retract(id != 3);
NSTEP/STPINC/SOLMTT from bite HT; OUT << "DSMAX " << "\t" << 0 << endl; OUT << "DTMAX " << "\t" << g(dw / H.NSTEP) << endl;  // DSMAX first here
if (dw == 0.0) { double cyc = (die.MovType == 4) ? 1.0 / die.CYCLES : m->Cycle_Time;  double dec = m->Decompression_Time;
    if (dec + dw < cyc) OUT << "TCYCLE" << "\t" << H.NSTEP << "\t" << P->BitesToSkip << "\t" << g(cyc) << "\t" << g(dec) << "\t" << g(dw) << endl; }
140136040; TRANS; tail("RH%1-PS%2-BDW%3");

// BHT 140755170 (state s at entry)
head2; retract((id != 3 || m->Bottom_Fixed != 2) && (id == 3 || s != 2));
NSTEP/STPINC/SOLMTT from bite HT; OUT << "DTMAX " << "\t" << g(H.TMAX / H.NSTEP) << endl; OUT << "DSMAX " << "\t" << 0 << endl;
double cyc = (die.MovType == 4) ? 1.0 / die.CYCLES : m->Cycle_Time;
if (m->Decompression_Time + m->Dwell_Time < cyc)
    OUT << "TCYCLE" << "\t" << H.NSTEP << "\t" << P->BitesToSkip << "\t" << g(cyc) << "\t" << g(m->Decompression_Time) << "\t" << g(m->Dwell_Time) << endl;
140136040; TRANS; tail("RH%1-PS%2-BHT%3" + (P->AutoBite ? "(Auto)" : ""));

// SHT 1407595c0 (adaptive reheat; tr1 = TR1, aht = Adaptive_Heating_Time)
head2; retract(all);
H = P->HT; NSTEP/STPINC/SOLMTT; OUT << "DSMAX " << "\t" << 0 << endl;
OUT << "ENVTMP" << "\t" << 0 << "\t" << g(tr1) << endl;  OUT << "DTMAX " << "\t" << g(aht / H.NSTEP) << endl;
140136040; TRANS; tail("RH%1-PS%2-SHT");
```

### 4.9 CHKSTOP sections (not SIMULATION blocks)

They are written straight after the previous block's `GENDB` line and its DB name, before the next
block header (which begins with a blank line).

```cpp
// pass level, adaptive reheat without auto bites (sim already incremented for the SHT block)
OUT << SEP << endl; OUT << "CHKSTOP" << endl; OUT << 1 << endl; OUT << 2 << endl;
OUT << 0 << "\t" << *sim + 1 << endl; OUT << 1 << endl; OUT << g(tr0) << endl; OUT << chkMethod << endl; OUT << SEP << endl;

// bite level, auto bites, b > 1, only == 0 (chkSim from 4.2)
OUT << SEP << endl; OUT << "CHKSTOP" << endl;
OUT << (int)adaptive + 1 << endl; OUT << 1 << endl;
OUT << 0 << "\t" << chkSim << endl;
OUT << 2 << "\t" << 3 << "\t" << g(P->MinDieDis) << "\t" << (int)m->RotationalSymmetry << endl;
OUT << P->DieMovDir << "\t" << g(0.0) << "\t" << g(0.0) << endl;
OUT << g(0.5) << endl;                                   // .rdata 142101640
OUT << g(B->NomBite) << endl;                            // bite 1 (+0xb8)
OUT << P->DiePosMethod << endl;
if (P->DiePosMethod == 1) OUT << P->DieStopNod << endl; else OUT << g(P->DieStopPos) << endl;
if (adaptive) { OUT << 2 << endl; OUT << chkSim + 1 << "\t" << chkSim << endl; OUT << 0 << endl;
                OUT << g(tr0) << endl; OUT << chkMethod << endl; }
OUT << SEP << endl;
```

Corpus example (Cogging_Multiple_Ops_Lab2.MST lines 157-168):
`SEP, CHKSTOP, 1, 1, 0\t42, 2\t3\t19\t0, -1\t0\t0, 0.5, 5, 0, 0, SEP`.

### 4.10 `only` (arg8) mode

When `only != 0`, every block counts but only block number `only` is written, and the pass loop
stops after it. CHKSTOP sections are never written in this mode except the pass-level one
(adaptive, no auto) that precedes the selected SHT block. The MST walk always passes 0. This mode
serves the single-operation DEF_MULTI.INI path (`1400a2200`), which is out of scope.

---------------------------------------------------------------------------------------------------

## 5. Cogging.MDT reader (load path, `140761c90`)

Entry: properties slot +0xc0 `140760430` → `140761c90(model)`: dir = `140050df0(1405f8d90(model+0x548))`
(the operation's Task folder) → `140761d90(model, dir)`. Path = `QDir::convertSeparators(dir + QDir::separator() + "Cogging.MDT")`
(`140761580`), opened with `fopen(path.latin1(), "rt")` (text mode, CRLF → LF).
Missing file: `QMessageBox::warning("FILE MISSING!", "The MDT file %1 for the current cogging problem does not exist.")`.
The model keeps its defaults. (The converter should treat this as `M2M_E_KEY`.)

The reader is **positional**: every value is read with `fscanf(f, "%s <conv>\n", keybuf, &v)`, so
the keyword text is skipped without being checked, and the `\n` in the format skips all
whitespace. `skip(k)` (`14076b5c0`) = `fgets(buf, 0x1000)` repeated 3 times for k ∈ {0,1}, 5 times
for k ∈ {2,3}, once for k = 4. Fields shown as `f` are `%f` into a float, then widened to
double (so `55.55` is stored as `55.549999237060547`). `lf` = `%lf`, `d` = `%d`, `s` = `%s`
into a char array.

```
skip(2)                                 banner (5 lines)
header 14076b710: fgets×3, sscanf(line3, "* MASTER DATA FILE %d", &ver), fgets×2
"%s %s\n" PRBID → cog+0;  "%s %[^\n]s" TITLE → cog+0x64;  fscanf("\n")
skip(0) skip(2)                         Defaults / Reheats headers
HEAT ×3  (140797ef0 → globals 0x143db1a20 + k*0x80 via 140797b90)
skip(2) PASS-defaults (140792810, without skip) → global 0x143da7e00 (140791c50)
skip(2) BITE-defaults (140790ca0)  → global 0x143da7cc0 (140790a50)
BILLET (140796f90)  DIE (140795760)  MANIP (140793cc0)  SIMPARAMS (14076aca0)
skip(2) "%s %d\n" SIMLST n; n × fgets(100); "%s %d\n" NoReheats → realloc reheats (14076c920/14076c100)
for each reheat (14076c9b0): skip(0); "%s %d\n" NoPass; alloc passes (14076bfe0); HEAT×3;
    for each pass (140792630): skip(0); PASS (140792810: fields + HEAT);
        bites: AutoBite == 0 ? NoBite × BITE : 1 × BITE      (BITE = 140790ca0: skip(0); HEAT; 140790d80)
if ver > 10999: skip(0); PROCESS DATA (140761d90)
```

HEAT (`140797ef0`): `skip(0)`; `d` Active; `d` NSTEP; `d` STPINC; `f` TMAX; `f` DTPMAX;
ver < 0xce7: `f` (ENVTMP, discarded) else `"%s  %d  %f\n"` flag,count and, when flag == 1,
`count` × `"%lf %lf\n"` pairs (discarded); `lf` IHTCOF; ver > 0xce4: `lf` IHTCOF2; `"%s \n"`, then
`fgets(+0x30, 0x4f)` UserKey (trailing `\n` removed; `"---"` → `" "`).

PASS (`140792810`): `d` NoBite (0 → 1; the pass is reset by 140791580(pass, NoBite), which also
allocates NoBite bites from the global default bite); ver < 0xc21: `f` (discarded); `f` RotAng;
ver > 2999: `f` BiteAngle, `f` MinDieDis; ver > 5999: `d` DiePosMethod; ver > 3999: `f`
DieStrtPos, `f` DieStopPos; ver > 5999: `d` DieStrtNod, `d` DieStopNod; ver > 0xfa1: `d` DieMovDir;
ver > 0xc1b: `d` AutoBite; ver < 0x1771: defaults from `140794bb0/140794be0/140794c40`, else `f`
LeftManDis, `f` RightManDis; ver > 0x277d: `f` HorizDieShift; ver > 10999: `d` BitesToSkip,
`f` RadialFeedPerBite, `f` DwellTime; HEAT at +0x28.

BITE deformation (`140790d80`): `skip(0)`; `d` NSTEP; `d` STPINC; `d` PDIE; `f` SMAX; `f` LMAX;
ver > 4000: `f` ClampForce (> 0 also sets a global through 14076bf10); `f` DTSUB; `d` StepMod;
`f` DSMAX; `f` DTMAX; `d` FricTyp; `f` FRCFAC; `lf` IHTCOF; ver > 0xce4: `lf`; `"%s \n"`,
`fgets(199)` UserKey (kept after the last `/`); ver > 0xc20: `f` NomBite.

BILLET / DIE / MANIP / SIMPARAMS: each starts with `skip(2)`. Field order and version gates are
as in the tables of 3.2 (decompiles `mdt_d3c.c`). In SIMPARAMS: `d` UNIT (sets the global unit
UNIT+1), `f` POSERR, `f` INTERR, `f` CVGERR1, `f` CVGERR2, ver > 0x13ec: `f` AVGSTR, `f` LMTSTR;
`f` CTLPCT; ver > 0xc1c: `d` UseManip → cog+0xc8 = (v != 0).

PROCESS DATA (`140761d90`, after `skip(0)`): `"%s %d\n"` Process_Type → +0. If the keyword
token is not `Process_Type`, the reader shows "FILE CORRUPT!" / "The MDT file %1 for the current
cogging problem is corrupt." and stops. Then, in this order: `d` +4, `d` +8, `d` +0x4cc,
`d` +0x4d0, `"%s %lg %lg\n"` +0x4d8 +0x4e0, `d` +0x4f0, `d` +0x14, `lg` +0x4e8, `lg` +0x4f8,
`lg` +0x510, `lg` +0x508, `lg` +0x518, `d` +0x520, `d` +0x504, `d` +0x505, `lg` +0x528,
`lg` +0x530, `d` +0x15, `d` +0x500.
**Byte effects to reproduce**: `%d` writes 4 bytes. +0x505 therefore overwrites the lowest byte
of Cycle_Time (+0x508) with bits 24..31 of Remaining_Cycle_Time_Dwell (0 in practice): COG_P1_B1
Cycle_Time 2.46 becomes 2.4599999999999227. +0x15 overwrites +0x16..+0x18.

Geometry KEY side reads: when ver > 0xc1b, the BILLET, DIE and MANIP readers call
`14079a900(global, name)` for KeyBil, KeyDie1..4 and KeyMan1..4 (`fopen(name, "rt")` relative to
the **current directory**, which the GUI sets to the project folder). They parse RZ/MGNELM/TRGVOL/
NDTMP/ELMCON into globals, and `140795260`/`1407937f0` set die/manipulator names and directions
in globals. The MST writer reads none of these globals (it uses the fixed tables of 140765f30,
140765fa0, 140766010), so the port can skip these reads. They only matter if such a file exists
and is malformed (none in the corpus).

`1407617e0` is the matching MDT writer (`%s %6d`, `%s %15.7E`), called from properties slot
+0xb8 (operation save, outside Save→MST).

---------------------------------------------------------------------------------------------------

## 6. COG_P1_B1 walkthrough (copy in `samples/COG_P1_B1`, MST mtime = moproj mtime)

Flow order: HeatTransfer (Task1), HeatTransfer (3) (Task4), ENTER (Task5), EXIT (Task2),
CogABC (Task3, SfOprCogging). Sims 1-4 are base-writer blocks (section 2.4). Cogging data:
1 reheat, 1 pass, NoBite 1, AutoBite 0, pass HT TMAX 1.11, RotAng 55.55, MinDieDis 444.44,
DiePosMethod 0, DieStrtPos 0.123, DieMovDir -1, UseManip 1, UseSpring 1, LeftDis 1e9, RightDis 0,
manip StartPos 131.3 / Length 369, billet GeoTyp -1 / Width 500, Temperaure_Type 2,
Heat_Transfer 1, Remaining 0, Cycle_Time 2.46, Dwell/Decompression 0, MovType 1, NoDies 2.

| Sim | Lines | Source |
|---|---|---|
| 5 PHT | header; `DBREAD 0`; MDSOBJ/TNOW/SimCtrl; material, OBJTYP 4..7 + objects, MTLGRP; CNTACT + InterObject | first-block head (firstOp false) |
| | `OBJPOS 4 13 ... 315.8 1`, `5`, `OBJPOS 6 13 ... 315.8 1`, `7`, four `OBJPOS 4/6 14 5/7 0/1 0.001 1` | manipFirst, v = 131.3 + 369/2 |
| | `OBJPOS \t2\t11\t0\t0\t250`, `OBJPOS \t3\t11\t0\t0\t-250` | retract (state 0), half = -500/2 |
| | `NSTEP 1`, `STPINC 1`, `SOLMTT 1`, `DTMAX \t1.11`, `DSMAX \t0` | pass HT |
| | `INICTC 1/5/7`, `GENCTC 0`; `TRANS \t1\t0\t-1\t-1\t-1\t-1`; `SIMNAM RH1-PS1-PHT`; `OPRNAM CogABC`; `CURSIM 5 5 0`; `GENDB 1 1 2` | tail |
| 6 BDEF | header; `DBREAD 0`; MDSOBJ/TNOW/SimCtrl | head (first now false) |
| | `OBJPOS\t1\t3\t0\t0\t0\t1\t0\t0\t55.55\t4`, `4\t5\t6\t7\t` | RotAng, bite 1 |
| | `OBJPOS\t2\t13\t0\t-1\t0\t0\t1\t0\t0.123\t1`, `3\t` | c13 (pass 1, not firstOp), DiePosMethod 0 → DieStrtPos |
| | two `OBJPOS \t2/3\t11` lines | retract (state 3) |
| | `OBJPOS\t2\t12\t1e-05\t0\t0\t0\t1\t0\t0\t1`, `3\t` | |
| | `FORCE\t2\t0\t0\t0`, `STROKE \t2\t0\t0\t0`, same for 3 | Process_Type 0, MovType 1 |
| | `OBJPOS\t4\t14\t5\t0\t1e-05\t1`, `OBJPOS\t6\t14\t7\t0\t1e-05\t1`, `GENAXS\t7\t1e+09\t0` | UseManip |
| | `URZ\t1\t1 0.0 0.0 0.0`, `1\t 0.0 0.0 0.0`, `STROKE \t4..7\t0.0 0.0 0.0` | UseSpring |
| | INICTC/GENCTC; `MDSOBJ\t2\t3\t3\t444.44`; `SIMNAM RH1-PS1-BDEF1`; OPRNAM; `CURSIM 6 5 0`; GENDB | |
| 7 BHT | header; DBREAD; MDSOBJ/TNOW/SimCtrl; two retract lines (state 1); `NSTEP 1`, `STPINC 1`, `SOLMTT 1`, `DTMAX \t0`, `DSMAX \t0`; `TCYCLE\t1\t0\t2.46\t0\t0`; INICTC/GENCTC; TRANS; `SIMNAM RH1-PS1-BHT1`; OPRNAM; `CURSIM 7 5 0`; GENDB | bite HT TMAX 0; 0 + 0 < 2.46 |

The prototype reproduces these lines exactly (`python tools/cmp_cog.py <moproj>`).

---------------------------------------------------------------------------------------------------

## 7. Files read or written during MST generation

| Path | When | R/W | Note |
|---|---|---|---|
| `<prj>.MST` | 1400a20d0 (s1-02) | W | not created by these writers |
| `Operations/TaskNNNNN/Cogging.MDT` | properties load (+0xc0 → 140761c90) | R | must be read by the converter; the writers read only memory |
| `<KeyBil>`, `<KeyDie1..4>`, `<KeyMan1..4>` (names from the MDT, e.g. `BILLET.KEY`, `DIE1.KEY`, `MAN1.KEY`) | same load | R (relative to cwd) | no MST effect; skip in the port |
| `USER_HT.KEY`, `CogBilMaterial.KEY` ... | only in the MDT import UI path 14075b220 | | not on the Save path |

No `fopen`/`QFile` write and no delete is reachable from 1407520c0, 140752190 and its seven block
writers, 1405f5340/1405f56c0, or the +0x200/+0x290 hooks. `COG_GEO_MANIP.EXE`, `DEF_BRK.EXE`,
`COG_STRAIGHT_BILLET.KEY` and `NEWBRICK.KEY` are only text written into the MST (`EXECMD` lines run
later by DEF_PRE). `Cogging.MDT` has the same mtime as the moproj in COG_P1_B1 because the
operation save (+0x68 → properties +0xb8 → 1407617e0) rewrites it just before the moproj; that
is step 3 of Save, not the MST writer.

---------------------------------------------------------------------------------------------------

## 8. Open questions

* **Q1** Legacy SIMTYP: the code maps `SfOprAirConvect*`/`HeatResting*`/`HeatExpress`/`HeatDwelling`
  to class `SfOprHeatTransferExpress(2D)`, and `*!SIMTYP` prints the mapped name. The only corpus
  MSTs with legacy types are 2013 vendor examples, and they print the legacy name. No current-build
  sample exists, so oracle B is needed to confirm.
* **Q2** `QTextStream << double`: evidence says `-0.0` prints `0`, and `1e-05`/`1e+09` are formatted
  as shown. s1-06 must match Qt 4.x `QLocale::doubleToString` exactly (rounding of values such as
  `(double)55.55f`).
* **Q3** Process_Type/Temperaure_Type/Use_Mandrel/+0x14 defaults come from `1407656d0`/`1407614e0`,
  which were not decompiled. They only matter when the MDT has no Process Data (ver ≤ 10999; the
  corpus is all 11000).
* **Q4** Exact semantics of `140050bc0` (first-operation test through the flow-chart model). The
  corpus agrees with "op is first in the flow" (NewProject_1, brick_mesh_01). s1-01/s1-02 own it.
* **Q5** `%f` overflow into a float: the prototype uses ±inf; MSVC's result was not checked.
  The only corpus case is Lab2's HEAT `ENVTMP` count (`5887979...000.000000` with flag 0), which
  is discarded, so it does not affect the MST. The port only needs to parse it without failing.
* **Q6** The model used at Save for the *current* operation is the in-memory one. The MDT rewritten
  at the same Save is written with `%15.7E`, so a value typed in the GUI that is not exactly
  representable in 8 digits could print differently through the re-read MDT. This is visible only
  in computed lines (DTMAX = TMAX/NSTEP, translations). No corpus case shows it.
* **Q7** The writer's `only` mode (4.10) and its use from `1400a2200` are not in the Save path;
  they are described only as far as the decompile shows.
