# MDT DB: loading an operation from a database step (GUI path)

DEF_GUI_MO_64.exe (DEFORM v11.0.2), static analysis only. This part covers what the GUI does when
the mode-0 decision `0x140055360` (spec_C section 3) takes its DB branch: the step choice, the
database step read into the engine model, the GUI slot / item sync from that model, and the
database handle the whole path reads. It also fills in `0x14005f520` (spec_C 3.4) with every callee
and answers spec_C Q3, Q4 and Q7 as far as static reading allows.

Scratch output (decompiles, disassembly, helper scripts): `deform-rea-work/gui_mo/s1/mdt/DB/`
(`d1.c` .. `d9.c`, `init.c`, `*.asm`; `tw.sh` twin lookup, `rtti.py`, `vtinfo.py`, `flagcalls.py`,
`pfunc.py`, `str.py`, `xref.py`). Ghidra copy `gui_mo/gproj_mdtDB`. Twins were found with
`gui_mo/s1/mdt/fmatch.py` (masked byte signature); a twin given here without "inferred" is a
byte-identical masked match. Port status is from `docs/re/pre64_batch_ledger.tsv`,
`docs/re/status/*.tsv` and `git grep` on `port/`.

Names (as in spec_C): `OPI` operation item, `OB` per-class handler, `P` project, `SP` sub-project,
`DBH` data root / DB handle (`P+8`, see section 1), `DBR` DB reader `DBH+0x40` (`0x140cdab70`),
`M` GUI model wrapper SfStepData (`0x14005f770(opi)`), `S` DB_STEP_DATA `M+0xe0`, `SC` sim control
`S+0x750`. `tr(x)` = `QObject::tr`.

Dialog policy (Aleksandr, 2026-10-22, loader-spec section 6): the converter answers a dialog with
its default button and logs a warning.

---

## 0. Call tree (DB branch, mode 0)

```
0x140051c30 openOperation                         (spec_C 1.3)
  0x140d397a0 DBR::refreshIfChanged                (spec_C 1.9; reopen = 0x140d395c0 -> 0x14080ca10)
  OB vt+0x58 0x1405ef430 -> vt+0x1b8 0x1405f6520
    0x140055360 decideAndLoad(opi, 1)              (spec_C 3; DB branch section 3 here)
      0x14005f520 dbHasStepsFor                    (section 2)
        0x1400b9cd0 dbHandle, 0x140050b20 index, 0x140050b70 -> 0x1400b8820 nextSim
        0x1400b8b80 prefixMatchesDb
          0x14080f690 DB op list, 0x1400b94f0 chain operations
        0x14080e840 op step count (-> 0x14080fc60 = 1401df660)
        0x140062960 previous op, OB vt+0x1f0 0x1405f68e0 class flag 0x01000000
      [Setup Type dialog, default Yes]
      0x14080e710 last step of op idx-1 | 0x14080e5e0 first step of op idx
      0x1400572e0 loadDbStep(opi, step, 1)          (section 4)
        0x140c7b720 model reset
        0x140c7b870 -> 0x140c7b900 readDbStepIntoModel  (section 5)
          0x14080e560 step -> directory index (1401ddf60)
          0x140896940 S+0x1b58 = 1
          0x1408256c0 _DB_STEP_DATA::ReadRecordFromDB   (= DEF_PRE 1401cc420, section 5.1)
          0x140cdbcd0 header cache (0x140d396d0 -> 0x14080d870 = 1401dd250)
          0x140c7d3a0 furnace table, 0x140c7ebe0 object environment table
          viewer / signals (UI)
        0x1400538f0 syncFromModel(opi, 1, 0)        (section 6)
          materials -> material items + Material KEY files (0x1408959e0 = 1401c5ea0)
          slots <- objects (0x140060ee0, 0x140057c60, 0x140064d40), equipment ids
          0x140069620 object manager refresh (may delete files)
          OB vt+0xc8 (section 7: Convert2Dto3D, MultiBlow, Express family override it)
          0x140087270 diagram item rows (when sync)
      NSTART / step / TNOW patches, HT-express reset, restart mode ON  (section 3)
```

At File > Open the same handle is prepared once (section 1): `0x14008a9b0` sets the DB path
(`0x140cdcdc0`) and opens it (`0x140cdc400` -> `0x140cdc480`), with the "2G Split DBs" dialog and
a licence check.

---

## 1. The database handle

### 1.1 Which object, which file

`P+8` (`0x1400a5890(P)`) is the data root object (the port's "data root M"; 0x19c0-byte QObject
part plus managers, ctor `0x140cda460`, created at `0x140044001` in `0x140043fe0`). It is the DB
handle `DBH` of spec_C: path `DBH+0x28` (QString), path-set flag `DBH+0x20`, loaded flag
`DBH+0x38`, reader `DBR = DBH+0x40`.

`0x1400b9cd0(SP)` (spec_C 1.8) returns `P+8` for every sub-project type except 4 (die stress),
which gets its own handle `SP+0x48` on first use, path from `0x1400b9e80(SP, &name, true)` ->
`0x1400bbee0` (die-stress DbName, ported in `p17.hpp`). **Note:** the model always reads through
`M+0xd8`, set by `createModel` `0x1405f6410` from `P+8` (`0x140c7a5b0(p->+8, dim)`), while
`0x14005f520` and the step choice in `0x140055360` query `0x1400b9cd0(SP)`. For a type-4
sub-project the step numbers come from the sub-project DB and are then looked up in the project
DB (inferred from the two handles; open question Q1). Only the main sub-project matters for the
corpus.

DBR is the GUI's copy of the DEF_PRE DB info class `Cls_1401dbfe0`: the engine code reads
`+0x8` / `+0xc` / `+0x330` part tables, `+0x650` name, `+0xabc` version, `+0xac8` directory count,
`+0xad8` directory (0x28-byte records), `+0xae8` header cache (0xe0-byte records), `+0x1900` cache
built. The GUI wrapper uses `+0x1908` as a byte ("cache build started", `0x140d396d0`), `+0x1910`
as a QDateTime (`0x140d395c0`, `0x140d39950`) and `+0x1918` as the owner DBH (`0x140cdd190`).
DEF_PRE's class has a `void*` at `+0x1908` and an int at `+0x1910` (layout
`port/defmodel/include/defmodel/layout/cls_1401dbfe0.hpp`, size 0x1918), so the GUI build of this
class is not byte-identical past `+0x1900` (open question Q2). The port should keep the GUI extras
outside `Cls_1401dbfe0`.

### 1.2 `0x140cdcdc0` `void DBH::setPath(QString name, bool keepItems)`

```cpp
void setPath(DBH* h, const QString& name, bool keepItems) {
    h->+0x20 = 1;
    if (h->path /*+0x28*/ != name) {
        h->path = name;
        if (!keepItems) 0x140cdabc0(h);      // deletes the objects of list DBH+0x1960 (vt+0x18), clears it
        h->+0x38 = 0;                         // not loaded
        0x140d39950(DBR);                     // DBR+0x1910 = QDateTime() (stamp invalid)
    }
    if (!h->path.isEmpty() && !0x140818b50(h->path, ".DB") /*suffix test*/
        && !h->path.endsWith("FOR003", Qt::CaseSensitive))
        h->path = 0x140818d50(h->path, ".DB");   // changeSuffix (same helper as 0x1400a4cc0)
    h->path = QDir::cleanPath(h->path);
    QString colors = QFileInfo(h->path).path() + QDir::separator() + ".ObjectColor.DAT";
    0x140cdeab0(0x1447c4860, &a, &b, colors.toLocal8Bit().data());   // object colour table (viewer)
}
```

Strings: `0x1429e6684` / `0x1429e66b4` ".DB", `0x1429e9bb8` "FOR003", `0x1429e9bc0`
".ObjectColor.DAT". The colour file is only read (viewer colours; not traced further, UI).

### 1.3 At File > Open: `0x14008a9b0` (excerpt)

```cpp
// after P+0x18 = the .moproj path, 0x1400a4150, 0x1400a41f0, 0x1400a5290 (FOR003 rename offer):
if (DBH* h = 0x1400a5890(P)) {
    0x140cdcdc0(h, 0x1400a4ee0(P) /* cleanPath(P+0x30) */, false);
    0x140cdc400(h);                            // open, section 1.4
}
// then the walk 0x14008afa0 (P+0x30 may change in 0x1400a4da0 when <Database Name> exists)
```

The handle path is set **before** the walk. `P+0x30` at that point is the default from project
construction, `cleanPath(changeSuffix(<dir>/<base>, ".DB"))` (port: `initProject` ->
`setDefaultDbPath`, `0x1400a4cc0`). The walk's `0x1400a4da0` (`setPrimaryDb`) changes only `P+0x30`
and does not call `0x140cdcdc0` (its callers are `0x140014475`, `0x14008ad98`, `0x1400a4d69`,
`0x1400b9db5`, `0x14011d76a`, `0x14011d84b`, `0x140c793ce`). So the DB the loader reads is
`<project dir>/<moproj base name>.DB`, not necessarily `primaryDbPath(p)` (inferred from the call
order; the converter's refusal in `loader.cpp` tests `primaryDbPath(p)`; open question Q3). In the
corpus the two are the same file.

### 1.4 `0x140cdc480` `bool DBH::open(bool* buildCache, bool* async)` (via `0x140cdc400`: `true`, `false`)

```cpp
bool open(DBH* h, const bool* buildCache, const bool* async) {
    if (h->+0x38) return true;                          // already loaded
    if (h->path.isEmpty()) return false;
    if (!0x140d395c0(DBR, h->path, false)) return false;  // load_db_info (section 1.5)
    int answer = QMessageBox::Yes;
    if (0x140814e80(DBR) == 1) {                        // = 1401e4c10 has_previous_part (ported)
        answer = QMessageBox::question(nullptr, tr("2G Split DBs"),
            tr("The DB may have been split into multiple files due to the 2G file size limit.\n"
               "Do you want to load all of them as an integrated DB?"),
            QMessageBox::Yes | QMessageBox::Default /*0x4000|0x100*/,
            QMessageBox::No | QMessageBox::Escape /*0x10000|0x200*/, 0);
    }
    if (answer == QMessageBox::Yes) 0x1408150f0(DBR);  // = 1401e4e80 setup_part_chain (ported)
    if (!h->vt[0x78](h)) {                              // @src 0x140cdc480 vt+0x78  omitted: licence
        QMessageBox::critical(nullptr, tr("License error"),
            tr("You don't have a license to load this database.\n"), QMessageBox::Ok, 0);
        return false;
    }
    if (*buildCache) 0x140d396d0(DBR, async);           // *async false: 0x14080d870 = 1401dd250 now
    h->+0x38 = 1;
    return true;
}
```

Note that `setup_part_chain` runs also when `has_previous_part` is false (answer stays Yes): it
then sets up a one-part chain. The converter takes Yes (default button) and must not stop on the
licence branch (treat as licensed).

### 1.5 `0x140d395c0` `bool DBR::reopen(QString path, bool headerOnly)`

`rc = 0x14080ca10(DBR, path.toLocal8Bit().data(), headerOnly)`; on `rc == 0`:
`DBR+0x1910 = QFileInfo(path).lastModified()`, `DBR+0x1908 = 0`, return true; else false.

`0x14080ca10` is the GUI build of DEF_PRE `1401dc3f0` `load_db_info(d, name, header_only)`
(**ported, verified**: `port/defdbio/src/step/db_info_read.cpp`). Not a masked-signature match;
the structure is the same statement for statement: file test -> reset (`0x14080c840` / 4),
signature read `0x14081fcf0`, `+0xacc = 0`, `+0x8 = 1`, 100 part slots and `+0x330` names freed,
name to `+0x650`, header read `0x140b348b0`, title `+0xa50`, version `+0xabc`, dates `+0xaa1`,
`+0xaa9`, `+0xab1`, `+0xac0`, `+0xac4`, `+0xac8` count, `+0xad0`, then (unless header-only) the
directory with 10 / 5 words per entry by family 2 / 3 (<3000: 5) / 0x17 into `+0xad8`
(0x28-byte records, `+0x24` stored when `+0xc >= 1`, counting `+0xacc`). The port's function is
the twin to call.

### 1.6 Header cache on demand

`0x140cdbcd0(DBH, bool* async)`: `if (DBH+0x38) 0x140d396d0(DBR, async); return DBH+0x38;`
`0x140d396d0(DBR, bool* async)`: once per load (`DBR+0x1908`): async false ->
`0x14080d870(DBR, 0)` (= `1401dd250` load_header_cache, verified); async true starts a QThread
(`0x140d399d0`), never on this path.

### 1.7 What the converter has today

`GuiModel::db` is a `Project*` stand-in and there is no DB info object; `primaryDbPath(p)` is used
as "a DB exists" test in four refusals: `loader.cpp` (OprFlag 0, inherited object `0x14005cc00`),
`l6_slots.cpp` (Diestress model dimension, DiestressBM step). Packet DB-1 (packets.md) adds the
handle; it unblocks all of them.

---

## 2. `0x14005f520` `bool dbHasStepsFor(OPI*, int* lastMatch, int* unused)` (complete)

Body as in spec_C 3.4, confirmed from `d4.c`. Callees:

| Addr | What | Port |
|---|---|---|
| `0x140055050` | active SP of the op's project | ported (`activeSubProject`) |
| `0x1400b9cd0` | DB handle of SP (section 1.1) | no handle yet (DB-1) |
| `0x140050b20` | 1-based chain number: `0x1400b5f40(SP, OPI+0x108)` -> `0x140085800(diagram, id)` (cached map diagram`+0x40`, groups share a number) | ported (`flowNumber`, `flowChainNumber`) |
| `0x140050b70` | `0x1400b8820(SP, index)` | ported (`nextSim`, writer.hpp); its value is passed to `0x1400b8b80` and not used there |
| `0x1400b8b80` | prefix match of chain ops against DB ops (spec_C 3.4) | not ported |
| `0x140cdab70` | `DBH+0x40` | trivial |
| `0x14080e840` | number of DB steps of op `idx-1` = `1401de240` | **ported** `def::dbio::query::op_step_count` (status `mdt-dbquery.tsv`) |
| `0x140062960` | previous operation | ported (`prevOp`) |
| `0x140054e90` | class of the previous op (no null check) | ported (`classOf`) |
| OB vt+0x1f0 | `0x1405f68e0`, all classes: class-info flags `& 0x01000000` (section 2.2) | new, constant per class |

`0x1400b8b80` callees:

* `0x14080f690(DBR, &list)` (GUI only): for `i < DBR+0xac8`, entries with directory `+0x24 != 0`
  (stored) whose cache record `+0x38` (operation number) is greater than the last one taken:
  append `{name = QString(cache+0x4c, char*), opNo = cache+0x38}` (`0x1400bd440`, `0x140140460`,
  `0x1401404c0`, `0x1401405b0`); result copied out by `0x140917800`. So the list holds one entry
  per increasing operation number, in directory order.
* `0x1400b94f0(SP, &ops)`: ported as `flowOperations` (`model.hpp`).
* Indexing `dbOps[k-1]` goes through `0x1400bc740`: detach (`0x1400bc780`, refcount at `[d]`),
  then `0x1400ad700(d, i)`: `node = d->+8->next; repeat i times node = node->next;` and returns
  `node + 0x10`. This is a linked list with a sentinel head (layout like MSVC `std::list`), not a
  QList. With `k-1 >= size` the walk reaches the sentinel and compares its payload (spec_C Q7;
  updated: no bounds check and no crash guaranteed; the payload is whatever the head node holds).
  This also happens when the DB file does not exist (empty list, first comparison), i.e. for every
  OprFlag-0 restart-capable non-first operation. Open question Q4; the port should stop the
  prefix loop at the end of the list (documented deviation) unless an oracle shows otherwise.

### 2.1 `0x14080e710` / `0x14080e5e0` / `0x14080e840`

Confirmed (`d4.c`): each builds the cache when `0x140814150(DBR)` (`+0x1900`) is false
(`0x14080d870(DBR, 0)`), then `0x14080fc60` (= `1401df660` `op_first_last`, ported). Twins
`1401de110` `op_last_step`, `1401ddfe0` `op_first_step`, `1401de240` `op_step_count`: **ported**
in `port/defdbio/src/db_query/queries.cpp` (spec_C section 7 listed them as unported; that is out
of date).

### 2.2 Class flag `0x01000000` (spec_C Q3, answered)

OB vt+0x1f0 reads `vbase(OB+0x30)->vt[0x58]()`. `OB+0x28` is a `SfOprDiagItemInfo` member
(ctor `0x1405fd4d0`, RTTI `.?AVSfOprDiagItemInfo@@`; its virtual-base vtable `0x14249dbe8` is
installed at `OB+0x28+[vbtable+4]`). vt+0x58 = `0x1405fece0` -> `0x1405fdcf0`: returns the int at
`d+0x80` of the info's private data. Each class init (vt+0x180) sets it with `0x1405fd8a0(OB+0x28,
flags)`. All calls (`flagcalls.py`, value = preceding `mov edx, imm32`):

| Call site | Class (init, s1-01 6.1) | Flags |
|---|---|---|
| `0x1401c26fd` | Convert2Dto3D (`0x1401c24d0`) | `0x1100009d` |
| `0x140e22692` | CopyMirror (`0x140e22470`) | `0x2100009a` |
| `0x1411debd2` / `0x1411decdd` | Boolean2D / Boolean3D (`0x1411de9b0`) | `0x01000099` / `0x0100009a` |
| others (Forming `0x19/0x1a`, HeatTransfer `0x19/0x1a`, HT Express `0x15/0x16`, FE `0x15/0x16`, Cogging `0x1a`, HTFurnace `0x1a`, MultiBlow `0x29/0x2a`, HeatTreat `0x29/0x2a`, Pre `0x59/0x5a`, Cycle `0x301b`, Group `0x3019`, Diestress `0xdf`, DiestressBM `0x4000009d/e`, Report `0x9b`, DoeOutput `0x20097`, ShapeOptim `0x40097`) | | bit not set |

So vt+0x1f0 is true exactly for Boolean, CopyMirror and Convert2Dto3D: the geometry operations
that write one DB step without a simulation (meaning inferred). In `0x14005f520` a previous
operation of those classes needs one stored step; any other needs at least two. (The same init
also calls `0x1405fd6c0` when licence flags `0x1446ee5c0` / `0x1446ee58f` are 0: `omitted:
licence`.)

---

## 3. `0x140055360` after the dialog (DB branch)

spec_C section 3 has the full body; checked again against `m.asm` (`0x1400563f4` ..
`0x1400565xx`). Summary of what must be ported, in binary order:

```cpp
// interactive == true (default button; converter policy), useDb = true
DBR* r = 0x140cdab70(dbh);                         // dbh = 0x1400b9cd0(SP)
if (opi->className() != "SfOprConvert2Dto3D") {
    if (lastMatch < idx) { step = 0x14080e710(r, idx - 1); lastOfPrev = true; }   // op_last_step
    else                 { step = 0x14080e5e0(r, idx);     lastOfPrev = false; }  // op_first_step
} else { step = 0x14080e710(r, idx - 1); lastOfPrev = true; }
if (step == 0) { KEY load (0x1400569d0) ; restart off ; return; }

SimCtrl keep; 0x1408d3bc0(&keep);                  // stack SC; restore only when guiMode() == 1:
bool kept = QPointer(opi+0x38) && guiMode() == 1;  //   never in the converter (mode 0)
m = 0x1400572e0(opi, step, true);                  // section 4
int ns = SC(m)->nstart /*+0x160*/;                 // as read from the DB step (read_sim_ctrl)
if (ns > 0) ns = -(ns + 1); else if (lastOfPrev) ns = ns - 1;   // 0x140056424..0x140056456
if (kept) 0x1408df7b0(SC(m), &keep);
SC(m)->nstart = ns;  S(m)->+0x744 = step;  SC(m)->+0x650 = 1;  SC(m)->+0x648 = 0.0;
OPI+0x18 = 1; OPI+0x1c = ns;                       // 0x14005a2c0 / 0x14005a350
if (class name contains "SfOprHeatTransferExpress" | "SfOprHeatExpress" | "SfOprAirConvect"
                       | "SfOprHeatResting" | "SfOprHeatDwelling") {
    double tmax = SC(m)->+0x178;
    0x1408d5100(SC(m));                            // = 1401e8fb0 (ported, sim_ctrl.cpp)
    SC(m)->+0x178 = tmax;  SC(m)->+0x108 = 0;  SC(m)->+0x104 = 1;
}
// UI: status bar / log "Start step number has been set to %1" (ns)
0x140064760(opi, true, true);                      // restart mode ON (OPI+0x150 = 1)
0x1408d3d70(&keep);
```

Notes:
* `0x1400572e0` returns `M` even when the read fails (section 4/5): the patches above then apply
  to whatever the reset model holds (`0x140c7b720` cleared it). There is no error message on a
  failed read (inferred from the code: `0x140c7b870`'s result is dropped by `0x1400572e0`).
* `S+0x744` is also written by the read itself (directory step of the entry, section 5.1); the
  patch writes the same step number again.
* SC twins: `0x1408d3bc0` / `0x1408d3d70` (ctor / dtor) have no unique masked match (likely
  `1401e7b10` / `1401e7cb0`, inferred); they matter only for the guiMode-1 keep/restore.

---

## 4. `0x1400572e0` `Model* loadDbStep(OPI*, int step, bool sync)`

```cpp
Model* loadDbStep(OPI* opi, int step, bool sync) {
    Model* m = 0x14005f770(opi);
    if (!m) return nullptr;
    if (step != 0) {
        0x140c7b720(m);                 // reset (ported: GuiModel::reset)
        0x140c7b870(m, &step, true);    // result ignored
        0x1400538f0(opi, sync, false);  // writeEqp = false (the restart path passes true)
    }
    return m;
}
```

---

## 5. `0x140c7b870` / `0x140c7b900`: the step read into the model

```cpp
bool readDbStep(Model* m, int* step, bool resetViewer) {          // 0x140c7b870
    if (!m->+0xd8 /*DBH*/) return false;
    DBR* r = 0x140c7ac50(m);                    // DBH ? 0x140cdab70(DBH) : 0
    if (!r) return false;
    int idx = 0x14080e560(r, *step);            // = 1401ddf60 step_index (ported); -1 none
    if (idx < 0) return false;
    if (!0x140c7acd0(m)) return false;          // S
    return 0x140c7b900(m, &idx, resetViewer);
}

bool readDbEntry(Model* m, int* idx, bool resetViewer) {          // 0x140c7b900
    if (!m->+0xd8 || !S(m)) return false;
    if (resetViewer) 0x140c82c70(m);            // UI: viewer items
    0x140c8ba80(m);                             // delete M+0x30, M+0x38 = 0 (as in reset)
    bool f = (APP->procType /*0x1402cb7a0: APP+0xc*/ != 3);   // 5 (MO) -> true
    0x140896940(S(m), &f);                      // S+0x1b58 = f (layout u1b58; ctor value 1)
    int rc = 0x1408256c0(S(m), r = 0x140c7ac50(m), *idx, false);   // section 5.1
    if (rc != 0) return false;
    if (APP->procType == 1 && SC(m) && SC(m)->nstart >= 0)          // never for MO (5)
        SC(m)->nstart = -(SC(m)->nstart + 1);
    bool async = false; 0x140cdbcd0(m->+0xd8, &async);              // header cache (1.6)
    0x140c7d3a0(m);                             // furnace table 0xe of SC+0x440 -> M+0xf8 (ported, gui_model.cpp)
    0x140c7ebe0(m);                             // object env table 0xd -> objects (ported, gui_model.cpp)
    int stepNo = 0x14080e500(r, *idx);          // = 1401ddf00 entry_step (ported); UI use only
    0x140c7ff80(m, resetViewer, false);         // viewer rebuild (packet V: viewerRebuild)
    // UI: step browser 0x140d0cc00(M+0x10, stepNo) when M+0x10; 0x140c911c0 (M+0x258) false ->
    //     0x140c905b0 / 0x140d15b40 / 0x1402ca890 (viewer windows)
    0x140c90150(m);                             // M+0x230 (0x60-byte object 0x140d78100) create or
                                                //   vt+0x140 refresh + MSG signal (UI; Q5)
    int one = 1; 0x140c833c0(m, &one);          // M+0x100 = 1 (GuiModel::state100)
    0x140d77610(m, m);                          // QMetaObject::activate signal of M (UI)
    // UI: MSG "signalActiveStepNoChanged()"(stepNo), "signalAssignFlownetData()"
    // UI: DBH list +0x18 not empty -> 0x140c8c930; M+0x10 viewer: 0x140cf97f0, 0x140cff370,
    //     0x140c80a20 / 0x140c80b00 by M+0x10+0x204
    return true;
}
```

UI-only callees were read to their Qt/viewer leaves (`d3.c`): `0x140c7ff80`, `0x140c8c930`,
`0x140d0cc00`, `0x140c905b0`, `0x140d15b40`, `0x1402ca890`, `0x140d77610`, `0x140cf97f0`,
`0x140c80a20`, `0x140c80b00`. None writes S, SC, OPI or a file.

### 5.1 `0x1408256c0` = DEF_PRE `1401cc420` `_DB_STEP_DATA::ReadRecordFromDB(db, index)`

Not a masked match (the GUI build has a 4th argument `progress` and extra calls), but each callee
is a masked twin and the order is the same as DEF_PRE's (compare
`port/defdbio/src/dbread/lead.cpp` `fun_1401cc420`):

| GUI | DEF_PRE twin | Role | Port |
|---|---|---|---|
| `0x1446ee5bc` / `0x1446ee5c0` test, messages "Error occurred on reading DEFORM database" / "License Error", rc 7 | `1401cc46b..4ff` | licence | `omitted: licence` (port reads every type) |
| `0x1408312c0` | `1401d7ef0` (not unique) | step_clear_u2f89 | ported |
| `0x140832340` / `0x140832370` / `0x140832430` | `140199dc0` / `140199df0` / `140199eb0` | step timer | ported (`step_timer.cpp`) |
| `0x1408246e0` | `1401cb440` | free step contents | ported |
| vt+0 | | set dimension 2 / 3 | ported |
| `0x1408253c0`, `0x140825400` | `1401cc120`, set_step_version | | ported |
| `0x1402f0ae0(&pct)` | none | progress bar, only when `progress` (false here) | UI |
| `0x140825e20` | `1401cca80` | read_step_header | ported (p-dbread w3) |
| `0x140826590` | `1401cd1f0` | read_arrays(m, d) | ported |
| `0x140831750` | `1401d83b0` (not unique) | header-only flag | ported |
| `0x140917d40` / `0x140917d60` | `140235c10` / `140235c30` | unit system get / set | ported |
| `0x140828840` | `1401cf4a0` | read_sim_ctrl | ported |
| `0x1408292c0` | `1401cff20` | read_materials | ported |
| `0x140829520` | `1401d0180` | read_inter_matl_info | ported |
| `0x14082b5d0` | `1401d2230` | read_obj_info | ported |
| `0x14082efe0` | `1401d5c10` | read_inter_obj_info | ported |
| `0x1408296c0` | `1401d0320` | step_material_state_sizes | ported |
| `0x14082bf90` | `1401d2be0` | read_arrays(m) | ported |
| `0x140065560` -> `0x14082f320` | model_is_2d -> `1401d5f50` | 2D object +0x8b8 doubles | in lead.cpp |
| `0x14035b400` -> `0x14082b970` | model_is_3d -> `1401d25d0` | read_obj_fold_info | ported |
| `0x1408263c0` | `1401cd020` | process step header (STROKE) | in lead.cpp |
| frees of `+0x1b78/0x1b80/0x1b88/0x2240` | same | | in lead.cpp |
| `0x140825420`, `0x14082bb00` | stored_version, `1401d2760` | dynamic arrays (version > 10999) | ported |
| `0x14035b400` -> `0x14082f790(m, 0)` | `1401d63c0` | surface normals (3D) | ported |
| `0x14082bf10` / `0x14082bf60` / `0x140832320` | names_up_to_date / `1401d2bb0` / `1401d8f80` | flag `+0x1b5a` | ported |

`1401cc420`, `1401cc2a0`, `1401cd020`, `1401d5f50` are implemented in
`port/defdbio/src/dbread/lead.cpp` (p-dbread, merged at the P5 gate `fe87a906`) but their ledger
rows still say `todo` and there is no status row: fold them before relying on the ledger. The
round-trip test `port/defcmd/tests/test_dbread_roundtrip.cpp` exercises `fun_1401cc420`
(680/682 against DEF_PRE's own round trip).

**Port**: call `def::dbio::dbread::fun_1401cc420(S, dbr, idx)` (3 arguments; the GUI's
`progress = false` changes nothing but the progress bar). Return codes: 0 ok, 2 no directory /
index out of range / step not stored, others from the header and array readers.

What the read does to the model (from the ported subtree): the step is cleared and freed, then the
whole model comes from the DB entry: dimension, step header, sim control (all of SC, including
NSTART, TMAX, TNOW), materials, inter-material data, objects (geometry, mesh, state arrays,
movement), inter-object data, fold info, dynamic arrays. Nothing of the operation's own KEY data
(SimCtrl.KEY, InterObject.KEY, object files) is read on this path.

---

## 6. `0x1400538f0` `void syncFromModel(OPI*, bool updateDiagram, bool writeEqp)`

spec_A section 6 has the body; checked against `d1.c` / `d3.c`, with the callee status:

```cpp
void syncFromModel(OPI* opi, bool updateDiagram, bool writeEqp) {
    Model* m = 0x14005f770(opi); if (!m) return;
    MatMgr* mm = 0x140055110(opi);                 // P+8 -> 0x140044dc0: SfMoMatManager
    int nMat = S(opi)->+0x1320;                    // 0x14005f7a0 = S
    for (int i = 0; i < nMat; ++i) {
        Material* mat = 0x140830460(S, i + 1);     // 0x1408bc2f0(S+0x1328, n): n-th of the list,
                                                   //   next at +0x2200 (twins 1401d7060/1401d7090, not unique)
        if (!mat) continue;
        int id = mat->+0;                          // material number
        if (!mm->vt[0x1c8](&id)) mm->vt[0x188](id);   // 0x14006cea0 find / 0x14006c570 append item
                                                   //   (item vt+0x170, file name vt+0x178, appendRow)
        0x140060c40(opi, id);                      // item text = QString(mat+0xc); item+0x40 type:
                                                   //   mat+0x1cc8 == 0 -> 0, list +0x48 cleared (0x140041fa0)
                                                   //   == 1 -> 1, list = mat+0x1cd0[0 .. mat+0x1ccc) (0x140042010)
                                                   //   else -> 2 (0x140041ff0)
        QString f = mm->vt[0x208](&id);            // 0x14006d380 item file path
        if (mat->+0x1cc8 != 2)
            0x1408959e0(S, f.toLocal8Bit().data(), id, 0, 0, 0);   // FILE WRITE: material KEY
    }
    0x140055280(opi);                              // mm vt+0x220 = 0x14006d700: dataChanged (UI)
    int cap = max(0x1408bc4a0(S+0x1328), else 100); void* buf = new int[cap];   // unused, freed at end
    int n = 0x140c7c1a0(m);                        // object count
    0x140060ee0(opi, n);                           // OPI+0x40 slot codes resized to n (new = -1)
    for (int k = 0; k < n; ++k) {
        LLOBJ_DATA* o = 0x140c7aed0(m, k + 1); if (!o) continue;
        int code = 0x14005fb30(opi, k);
        if (code == -1 && o->kind /*+0xc*/ != 7) {
            if (o->kind == 5) code = -2;
            else {
                code = objMgr->vt[0x180]();        // 0x14006c520: new object item, smallest free id
                if (Item* it = objMgr->vt[0x1c8](&code)) it->vt[0x40](QString(o->name /*+0x828*/));
            }
            0x140057c60(opi, k, code, false, false, updateDiagram);   // set slot code; default
                                                   //   engine object (0x140c7c950(m,k+1,1,1)), kind 5
                                                   //   for -2, +0x2bf0 = 360.0, +0x2bf8 = 0; manager refresh
        }
        SlotRec* r = 0x1400612d0(opi, o->number /*+8*/);
        r->name /*+0x10*/ = QString(o->name);      // 0x14004d700
        0x140064d40(opi, o->number);               // slot material ids from the object (spec_C 1.10)
        if (!0x14084d4a0(o)) {                     // = 140179280 "translation or rotation" (verified)
            r->+0x1bf = 0; r->equipmentId /*+0x28*/ = 0;
        } else {
            r->+0x1bf = 1;
            int eq = r->equipmentId;
            if (eq < 1) { eq = eqpMgr->vt[0x180](); r->equipmentId = eq; }   // 0x1400a5970 -> 0x140044e60
            if (writeEqp) 0x140358190(S, o, eqpMgr->vt[0x208](&eq), 1);      // FILE WRITE (restart path only)
        }
    }
    delete[] buf;
    0x140069620(objMgr);                           // refresh: unused object items and their files deleted
    if (OB* ob = 0x140054e90(opi)) ob->vt[0xc8](ob);   // section 7
    if (updateDiagram) {
        0x140087270(diagram /*0x140055150*/, opi->id, m);   // diagram item rows (6.1)
        // UI: MSG "2signalUpdateUIInWidgetArea()"
    }
}
```

Callee port status (`git grep` in `apps/moproj2mst`): `0x140060ee0`, `0x140057c60` (`addObjectAt`),
`0x140064d40`, `0x14006c520` (`newObjectItem`), `0x140069620` (`refreshObjManager`), `0x1408959e0`
(material KEY writer, `defkey` + `saveMaterials`), `0x140358190` (`saveEquipment`), `0x14084d4a0`
(`obj_has_movement`) are ported. New: the material item sync `0x140060c40`, the loop itself, the
equipment-id assignment, `0x140087270` rows.

Notes:
* The slot codes of objects that already had a positive code keep it; their files are rewritten
  at Save (`0x140057370`, `saveObject`) from the DB object. Slot codes past the DB's object count
  are dropped by `0x140060ee0`.
* The new object item gets only a name here; its file is written at Save.
* `0x140057c60` with `fromFile = false` creates a default engine object only when the object does
  not exist (`0x140c7c950` finds the DB object by number first), so the DB object is kept.

### 6.1 `0x140087270(diagram, opId, M)` (only when `updateDiagram`)

For the diagram item whose id (vt+0x28) is `opId` (`d3.c`): item text (vt+0x10) = the operation's
name (`0x140050ac0`) when the operation item is found, else `QString::fromLocal8Bit(S+0x7a1)`;
expansion rows beyond the template count `DAT_14358133c` are cleared (`0x14007bf20(diagram, id,
row, "")`); then one row per object `i` (`0x14007b630(diagram, id, i, text)`): "undefined"
(`0x14213b040`, through `0x140088920`) for an undefined object (`0x140834fa0` false), else the
object name, or "Read From DB" (`0x14213b030`) when the operation's slot code is -2; trailing
undefined objects get empty rows (`0x14007bf20`). Diagram flag `+0x54` is set around the update.
The expansion rows feed the `.moproj` layout (`write.cpp`, `views.expansion`), so this is not UI
only. `0x14007b630` / `0x14007bf20` were not read to the leaves (open question Q6).

---

## 7. OB vt+0xc8 (called at the end of `0x1400538f0`)

Base `0x1405f1e10`: empty (`ret`). Overrides (`vtables.tsv`):

| Class | Function | What it does on this path |
|---|---|---|
| SfOprConvert2Dto3D | `0x1401c3570` | base; if `!OB+0x130`: `0x1401c7560(OB, -1, true)` (MDT L5, ported in `l5_slots.cpp`: work-model objects lose surfaces/mesh, **deletes** `<op folder>/SfOprConvert2Dto3D_TEMP_STEP.KEY`); then, when the main window's viewer model `0x1402c8170(APP)` exists: work model `0x1401c7370` (OB+0x138, new 3D model on the same DB, SC copied, TRANS 1/2 = 1), every defined object of M gets `+0x2c68 = 0`, `+0x2bf8 = 0`, `+0x2bf0 = 360.0`, `+0x2be8 = 0x1408da610(SC)` (= `1401ee4c0` GEOTYP kind, ported), possibly `0x1401c71f0`, copied into the work model (`0x140835b50/bf0/a50` = `140161ac0/140161b60/1401619c0`, ported; `0x14085ceb0`, `0x14085dd40`, `0x1401cc360(o, 3)`), `+0x2c20` cleared for kind-0 objects without mesh; `0x140c7b1f0(work, 1, 0)`; then with `0x1402c81b0(APP)` (viewer model): **writes** the TEMP_STEP.KEY from the work model (`0x140c7d100` -> `0x140895350` = `1401c5810` write_key_file, verified) and reads it into the viewer model (`0x140c7d020`). |
| SfOprMultiBlow* | `0x140602230` | base, then `0x140602260`: MultiBlow props (`0x140608ea0`); object `vt+0x120()` (primary die number) of M: missing -> message "Currently Multi-Blow requires primary die defined as object number 2."; movement type `+0x21c8` 2 or 3 -> `0x14060b6b0(props, o+0x2160)` (copies the die movement into the MultiBlow data); else message "Currently Multi-Blow can support only hammer and screw type." Messages go through MSG `2signalShowMessage()` (UI log, type 3 for the errors). |
| SfOprExpressBase, SfOprFormingExpress*, SfOprHeatTransferExpress* | `0x1413fbd10` | (1) OprFlag 0 (`0x1400509d0`) and props `+0xc0 == 0` (`0x1413f6b20`): `0x1413f6ae0(props, 0)`, MSG message "Solution step mode has been switched to User defined." (2) when `0x1413fd0f0(OB) != 0` and the class name does not contain "SfOprHeatExpress" and `vt+0x2c0(OB, 1)` is true: **dialog** "Keep Mesh of Dies?" (section 8) Yes -> `0x14142ff20(props) == 0 ? 0x1413fd9d0(props, 2)`, `0x140cbc690(props) != 0 ? 0x141371aa0(props, 0)`; No -> `vt+0x2c0(OB, 0)`. (3) `vt+0x2b8(OB)`. |

`vt+0x2c0`, `vt+0x2b8`, `0x1413fd0a0`, `0x1413fa180`, `0x140010320`, `0x14142ff20`, `0x1413fd9d0`,
`0x141371aa0` were not read (open question Q7). The viewer-model part of Convert2Dto3D depends
on whether the converter models `0x1402c8170(APP)` as non-null (the L5 packet decided this for
its editor path; reuse that decision).

---

## 8. Dialogs and messages on the path

| Where | Kind | Condition | Buttons / default | Converter |
|---|---|---|---|---|
| `0x140cdc480` (File > Open) | question "2G Split DBs" | `has_previous_part` (first step `|s| >= 2` and `<base><n>.DB` exists) | Yes (Default) / No (Escape) | Yes: `setup_part_chain` |
| `0x140cdc480` | critical "License error" | DBH vt+0x78 false | OK | omitted: licence (never shown) |
| `0x140055360` | critical "Missing Restart File" | restart on, no restart file | OK | ported (warning) |
| `0x140055360` | "Setup Type (Interactive/Batch)" | OprFlag 0, restart-capable non-first op, `0x14005f520` | Yes (`setDefaultButton`) / No, "Keep my choice" | Yes (policy) |
| `0x1408256c0` | stdout text "Error occurred on reading DEFORM database" + "License Error" | licence flags | none | omitted: licence |
| `0x140055360` | status bar / log "Start step number has been set to %1" | DB branch | none | log only |
| `0x1413fbd10` | MSG message "Solution step mode has been switched to User defined." | Express, OprFlag 0, props +0xc0 == 0 | none | warning; state change kept |
| `0x1413fbd10` | `QMessageBox::question` "Keep Mesh of Dies?" ("Currently meshed dies are imported." / "But with current setting, mesh is not required in dies." / "Do you want to change the current setting to keep mesh of dies?" / "Yes - Keep mesh of dies, change setting" / "No - Delete mesh of dies, don't change setting") | Express (not HeatExpress) with meshed dies (inferred from the text) | Yes / No, old-style overload without a Default flag: Qt 4 makes the first auto-default button (Yes) the default (inferred) | Yes (to confirm with an oracle) |
| `0x140602260` | MSG message (MultiBlow primary die / movement type) | MultiBlow object 2 missing or not hammer/screw | none | warning |
| `0x140358190` | question "Overwrite File" | only with `force == 0`; this path passes 1 | - | not shown |

---

## 9. File effects of the DB branch

| Effect | Where | Notes |
|---|---|---|
| reads `<project dir>/<base>.DB` (and split parts `<base><n>.DB`) | open (1.3-1.5), refresh at operation open (spec_C 1.9), step read | read only |
| reads `<db dir>/.ObjectColor.DAT` | `0x140cdcdc0` | viewer colours; not needed |
| **writes** `Materials/Material%05d.KEY` for each DB material whose `+0x1cc8 != 2` | `0x1400538f0` -> `0x1408959e0` | at load time, before Save; Save writes them again (`0x140058cc0`) |
| new material items / object items in the project lists | `0x1400538f0` | persisted by Save in the `.moproj` |
| **deletes** files of object items no operation uses | `0x140069620` | ported (`refreshObjManager`) |
| **deletes** stale `<op folder>/RestartStep.KEY` | `0x140055360` restart branch | before the DB branch; ported |
| **deletes / writes** `<op folder>/SfOprConvert2Dto3D_TEMP_STEP.KEY` | Convert2Dto3D vt+0xc8 | section 7 |
| at Save: `RestartStep.KEY` written (restart mode is now ON), `SimCtrl.KEY` (with the patched NSTART), object / mesh / BCC / properties / equipment KEYs, MST with `KFREAD 1 RestartStep.KEY` | `0x140057370` (`syncOperationFiles`), `0x140134510` | already ported; this is the "Restart file will be created" of the dialog text |

The restart path (`0x140057230`, spec_C 3.3) shares `0x1400538f0` with `writeEqp = 1`: it also
writes `Equipments/Equipment%05d.KEY` of every moving object at load time (force overwrite).

---

## 10. Fields written

| Object+off | Written by | Value |
|---|---|---|
| S (all) | `1401cc420` subtree | the DB entry |
| S+0x1b58 | `0x140896940` | 1 (MO) |
| S+0x744 | read and `0x140055360` | DB step number |
| SC+0x160 | `0x140055360` | `ns` (section 3) |
| SC+0x648 / +0x650 | `0x140055360` | 0.0 / 1 |
| SC+0x104 / +0x108 / limits | HT-express reset | 1 / 0 / `1401e8fb0` with TMAX kept |
| M+0x100 | `0x140c833c0` | 1 |
| M+0xf8 table, object env | `0x140c7d3a0`, `0x140c7ebe0` | from SC+0x440 tables |
| OPI+0x18 / +0x1c | `0x140055360` | 1 / `ns` |
| OPI+0x150 | `0x140064760` | 1 (restart mode) |
| OPI+0x40 slot codes, slot records (+0x10 name, +0x28 equipment id, +0x1bf, material ids) | `0x1400538f0` | |
| OPI+0x10c | `0x140051c30` after the load | 1 |
| DBR (directory, cache, part tables), DBH+0x38 | open / refresh / cache | |
| material / object / equipment manager items | `0x1400538f0` | |

---

## 11. Open questions

1. **Q1** Type-4 (die-stress) sub-projects: the step is chosen on `SP+0x48`'s DB but read
   through `M+0xd8` = `P+8` (section 1.1). Check whether `createModel` of the Diestress classes
   (own vt+0x1b0?) passes the sub-project handle. Not in the corpus.
2. **Q2** GUI DBR layout past `+0x1900` (`+0x1908` byte, `+0x1910` QDateTime, `+0x1918` owner)
   against DEF_PRE `Cls_1401dbfe0` (`+0x1908` pointer, `+0x1910` int): is the GUI reader a
   different class around the same engine fields? Matters only for where the port keeps the
   extras.
3. **Q3** The DB path the GUI reads is the default `<dir>/<base>.DB` set before the walk, not the
   `<Database Name>` element (`P+0x30` after the walk). Confirm with an oracle on a project whose
   `.DB` name differs from the `.moproj` base name; the converter's checks should use the handle's
   path once DB-1 exists.
4. **Q4** `0x1400b8b80` reads past the end of the DB operation list (sentinel node) when the chain
   has more matching operations than the DB, and on the first comparison when there is no DB at
   all. Static reading cannot say what the head node's payload compares as; the port stops at the
   end of the list. An oracle with an OprFlag-0 operation and no `.DB` would show whether the GUI
   crashes or falls back to the KEY load.
5. **Q5** `0x140c90150` (M+0x230) and `0x140d77610`: assumed UI (flownet / animation data and a
   model signal); their slots were not traced.
6. **Q6** `0x14007b630` / `0x14007bf20` (diagram row text / clear) and their effect on the saved
   `.moproj` expansion rows.
7. **Q7** Express vt+0xc8 internals (section 7) and the default button of "Keep Mesh of Dies?".
8. **Q8** `0x1400572e0` ignores a failed read; the NSTART patch then runs on a reset model. Should
   the converter refuse instead (deviation) when `fun_1401cc420` returns non-zero?

---

## 12. Engine twins on this path (summary)

| GUI | DEF_PRE | Port |
|---|---|---|
| `0x14080ca10` | `1401dc3f0` load_db_info (structural, not masked) | verified (`defdbio/step/db_info_read.cpp`) |
| `0x140814e80` | `1401e4c10` has_previous_part | ported (`db_query`) |
| `0x1408150f0` | `1401e4e80` setup_part_chain | ported (`db_query`) |
| `0x14080d870` | `1401dd250` load_header_cache | verified |
| `0x14080d6f0` | `1401dd0d0` reread header | ported |
| `0x14080fc60` | `1401df660` op_first_last | ported |
| `0x14080e840` / `0x14080e710` / `0x14080e5e0` | `1401de240` / `1401de110` / `1401ddfe0` | ported |
| `0x14080e560` / `0x14080e500` | `1401ddf60` / `1401ddf00` | ported |
| `0x1408256c0` | `1401cc420` ReadRecordFromDB (structural; every callee masked) | implemented in `dbread/lead.cpp`, ledger row stale (`todo`) |
| `0x1408246a0` | `1401cb400` step clear (reset) | ported |
| `0x1408959e0` | `1401c5ea0` material KEY writer | ported (`defkey`) |
| `0x14084d4a0` | `140179280` movement test | verified |
| `0x1408bc4a0` | `14029c410` material count | ported |
| `0x1408d5100` / `0x1408df7b0` / `0x1408d3cd0` | `1401e8fb0` / `1401f3660` / `1401e7c10` | ported |
| `0x140895350` | `1401c5810` write_key_file (Convert2Dto3D temp step) | verified |
| `0x1408da610` | `1401ee4c0` | ported |
| `0x140835b50` / `0x140835bf0` / `0x140835a50` | `140161ac0` / `140161b60` / `1401619c0` | ported |
| `0x140873d30` / `0x140873be0` | `1401ffed0` / `1401ffd80` | verified |
| `0x140830460` | material by index (`1401d7060` / `1401d7090`, not unique) | trivial list walk |

No engine twin on the DB branch is unported. What is missing is GUI code: the DB handle (section
1), `0x1400b8b80`, `0x14080f690`, the DB branch of `0x140055360`, `0x1400572e0`, `0x140c7b870` /
`0x140c7b900`, `0x1400538f0` (material item sync, loop, equipment ids, diagram rows) and the three
vt+0xc8 overrides.
