# Plan: DEF_GUI_MO stage 1, MOPROJ to MST converter (`moproj2mst`)

Requested by Aleksandr on 2026-10-22. DEF_GUI_MO work is split into two stages:

* **Stage 1 (this plan):** a DLL for Aleksandr's DMO app (`coffeejuice/DMO`). It opens a
  `.moproj` and the project files it uses, and writes the `.MST` that DEF_GUI_MO_64's File > Save
  writes, **byte for byte**, without changing any project file.
* **Stage 2:** the full reconstruction in `docs/plan-gui-mo-reconstruction.md`.

The coordinator runs the packets below as separate threads, at most 17 at a time. Everything a
fresh thread needs is in this file, the specs under `docs/re/moproj2mst/`, and the work folder
`C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\` on Sim2.

## 1. Rules (apply to every packet)

From `docs/re/moproj2mst/RULES.md`, plus the repo's port conventions:

* Never modify any DEFORM binary, file or original project. Analysis runs on copies:
  exe copy `deform-rea-work\gui_mo\DEF_GUI_MO_64.exe`, corpus copies `...\s1\corpus\`.
* No debugger. Do not run DEF_GUI_MO, DEF_GUI23 or a simulation. (Running the original GUI to
  produce reference MSTs, "oracle B", was asked of Aleksandr on 2026-10-22 and is **not
  approved**; only screenshots for stage 2 are.)
* Never touch, analyse or bypass licence, Sentinel or process-monitor code (`CLManager*`,
  `CDEFORMLM*`, `Csdspm*`, `CDFM*`, `SfLicense*`, `FTN_license`, start-up `140001410`).
* No web search. Everything stays private (private origin only).
* Port style: one C++ function per binary function where practical, an `// @src 0x14…` marker
  on each, the original operation order, every emitted literal byte-exact. A mismatch is traced
  to the code, never patched from the expected output.
* Each packet owns only the files listed for it. Calls into another packet go through the seam
  headers G0 creates; until the owner merges, the seam is a stub. Ask the gate thread (G1)
  before touching a file you don't own.
* Branch per packet: `claude/m2m-<id>` from the stage 1 base branch (G0 names it). Commit and
  push your branch; G1 merges. Do not push to main.
* CPU on Sim2 is shared with the DEF_PRE port: at most one Ghidra headless run per thread.

## 2. What the original does (summary of the specs)

Paths in `DEF_GUI_MO_64.exe` (image base `0x140000000`, Qt 4.8, MSVC x64):

* **Open** `14008a9b0` → `14008afa0` (`QDomDocument::setContent`) → `14008b410` (DOM walk):
  item lists `14008e290`/`14008e3d0`; operations `14008cb50`/`14008ccf0`; factory
  `140046cc0` with registry `14006dba0` (30 keys; legacy names AirConvect, HeatResting,
  HeatExpress, HeatDwelling → `SfOprHeatTransferExpress(2D)`, MultiBlow → MultiBlow2D);
  ActionKeywords tokenised into fields (`140095cc0`, `140096390`, `140906e40`); per-operation
  data files read at open through `PROPS->vt[0xc0]` (`Cogging.MDT`, `Boolean.BWZ`,
  `Forming.FWZ`, `Cycle.CWZ`, `DieStressBmWizard.Wzd`); the flow chart of sub-project 0
  parsed on activation (`1400a7b90` → `14008f530`); MST order `1400b6630` (connector chain,
  groups collapse to the group id). Spec: `docs/re/moproj2mst/s1-01/spec.md`.
* **MST** `1400a20d0` → `1401313c0`: header `140131680` (`*!MASTERFORMAT 4`, `*!TITLE`,
  `*!GEOTYP`, `*!UNIT`), project block `1401318b0`, operation walk `140131df0` →
  `140132100`, trailer `140131d10`; DOE variant `1401321a0`, `140131a50` (`*!SUBPRJ`),
  `140131530` (`.MST.TEMP`). Shared emitters `140131c10` … `140136810`. Spec:
  `docs/re/moproj2mst/s1-02/spec.md` (matches simulations 1–4 of `COG_P1_B1.MST`; 5–7 are Cogging's).
* **Operation writers** = vtable slot +0x280, 19 distinct functions over 34 classes:

| Writer | Classes | Spec | Corpus (oracle A) |
|---|---|---|---|
| `1405f5340` base, hooks +0x288 `1405f56c0`, +0x200 `1413e5ef0` (1) / `1405f79b0` (0), +0x290 empty | HeatTransfer(2D/Common), HeatTransferExpress(2D), ExpressBase, FormingExpressCommon, Forming(2D) [+0x200 = 1]; HTFurnace, Pre(2D), DiestressBM [+0x200 = 0] | s1-02 (base), s1-03 §2 | HeatTransfer 86 ops / 27 cases, Forming 38 / 21 |
| `1407520c0` → `140752190` | Cogging (PHT, PDWL, BDEF, BDCT, BDW, BHT, SHT blocks, CHKSTOP) | s1-03 §3–5 | 22 ops / 15 cases (prototype: 335/337 ops of all samples) |
| `1411df600` → `1411df6d0` | Boolean | s1-04 §1 | 14 ops / 4 cases |
| `14134dcc0` | Cycle (`CYCL<k>-` names via globals) | s1-04 §2 | 2 / 2 |
| `140e32970`, `141352540` | DoeOutput, Group (no output) | s1-04 §3 | 1 / 1 (DoeOutput) |
| `1401c4440` → `1401c4520` | Convert2Dto3D | s1-04 §4 | none |
| `140e24e80` → `140e24f60` | CopyMirror | s1-04 §5 | none |
| `141362e90` → `141362f60` | Distortion (n+2 blocks) | s1-04 §6 | none |
| `140602810` → `1406028b0` | MultiBlow(2D/Common) (BL, DW, TI, HT, TO) | s1-04 §7 | none |
| `1414136b0`, `141412c90` → `14141f2f0` | FormingExpress(2D) (BL, DW, TO) | s1-05 §5 | none |
| `1402b5410` | Diestress | s1-05 §2 | none |
| `14029ad10`, `14029cb50` | DiestressBM2D/3D | s1-05 §3 | none |
| `1411b1460`, `1411aee40` | HeatTreat(2D) | s1-05 §4 | none |
| `1412b05a0`, `14013fb90` | Report, ShapeOptim (no output) | s1-05 §1 | none |

Ledgers (address, size, role, status) of the closure: `docs/re/moproj2mst/s1-0x/ledger.tsv`,
922 distinct functions in s1-01/03/04/05 (783 `spec`, 112 `library` = Qt/STL, 135
`out-of-scope`, 6 `pending`) plus 249 rows in s1-02. G0 folds them into one stage 1 ledger.

### 2.1 Decisions already taken

* The converter writes only the output MST. It does not reproduce what the original writes or
  deletes as a side effect: folders and `ObjPropertiesNNNNN.KEY` created at open (blob bytes are
  kept in memory), Boolean's `Boolean.BWZ` and `DEF_MACHDIST_CTRL2.INI`, Convert2Dto3D's
  deletions (`DEF_CNV23.DAT`, `DEF_REM.INI`, `REMESH.SCR`), HeatTreat's `HT_Proc<k>.MOKEY`,
  DOE `DEF_DOE_PARSER.INI` and `.COR`. The MST bytes do not depend on them. When the MST refers
  to such a file and it is missing, the converter warns. A later flag
  `M2M_FLAG_WRITE_SIDE_FILES` may port those writers if Aleksandr wants runnable projects.
* Values the original reads back from such files at open are read the same way.
* The GUI "current item" a writer reads when it runs inside a Cycle is taken as the operation
  being written (s1-04 Q1), until a sample shows otherwise.
* Old-format inputs (MDT version below 11000, 2013 MSTs) are out of the oracle; code follows
  the binary.

## 3. The DLL

`apps/moproj2mst/` in this repo, built by its own CMake project that can also be added from
`port/CMakeLists.txt`. Products: static library `m2m_core`, `moproj2mst.dll` (x64, MSVC 2022,
`/MT` static CRT, no Qt or other runtime dependency), `moproj2mst.exe` CLI over the same calls.
DEF_PRE port libraries (`port/defrt` etc.) are linked only where a writer needs DEF_PRE engine
code (s1-04 §9 lists the twins, e.g. `DB_STEP_DATA` ctor `140821bf0` = DEF_PRE `1401c8950`).

```c
/* apps/moproj2mst/include/moproj2mst.h */
#define M2M_API_VERSION 1
enum {
  M2M_OK = 0,
  M2M_E_ARG = 1,            /* bad argument or buffer */
  M2M_E_OPEN = 2,           /* project file cannot be read */
  M2M_E_PARSE = 3,          /* not a valid .moproj ("Error on reading project file!") */
  M2M_E_INPUT = 4,          /* a referenced data file is missing or malformed */
  M2M_E_NO_OPERATIONS = 5,  /* the original writes no MST without operations */
  M2M_E_WRITE = 6,          /* output cannot be written */
  M2M_E_UNSUPPORTED = 7     /* operation type whose writer is not verified yet */
};
#define M2M_FLAG_DOE_TEMP   0x1u  /* write the DOE .MST.TEMP form (later)            */
#define M2M_FLAG_STRICT     0x2u  /* turn warnings (missing side files) into errors  */
/* Text is written in the ANSI code page, as the original (s1-02 O8); the default is 1252.
   m2m_set_codepage() selects another (932, 950, 20866, 949, 936 for DEFORM language codes). */
int m2m_set_codepage(unsigned codepage);

int m2m_api_version(void);
int m2m_convert_w(const wchar_t *moproj_path, const wchar_t *out_mst_path /* NULL: next to moproj */,
                  unsigned flags, wchar_t *msg, size_t msg_chars);
int m2m_convert_utf8(const char *moproj_path, const char *out_mst_path,
                     unsigned flags, char *msg, size_t msg_bytes);
```

* Reads only. Creates or replaces only `out_mst_path` (temp file in the same folder, then
  rename). No globals: the original's global state (Cycle names, DOE sample) is per call.
  Thread-safe for different projects.
* `msg` receives the error text, or warnings separated by `\n` when the result is `M2M_OK`.
* CLI: `moproj2mst <project.moproj> [out.MST] [--strict]`, exit code = result code.
* Release check: load and call from C++, C# (P/Invoke) and Python (`ctypes`).

## 4. Oracles and tests

* **Oracle A (the gate criterion):** the MST that the original saved together with each
  `.moproj`. Corpus copies in `deform-rea-work\gui_mo\s1\corpus\` (56 cases, list
  `docs/re/moproj2mst/runner/cases.tsv`): 50 oracle-A cases, 46 unique and copied (two others
  are 838 MB projects left in place, `C:\Users\alext\Projects\DEFORM_examples\big_projects`;
  G1 runs them read-only at the end).
* Runner: `docs/re/moproj2mst/runner/run_corpus.py --cli <moproj2mst.exe> --oracle-a`
  (python -I, stdlib only). Per case it converts a scratch copy, compares bytes, and fails if
  any project file changed. `--self-test` proves the runner. Result codes PASS, DIFF (first
  line and column), CHANGED, EXIT, NOOUT, TIMEOUT, NOORACLE, SKIP.
* MST facts from the corpus: ASCII, LF, no BOM, ends with `\n\n`, tabs and trailing spaces
  present, all paths relative with `/`, `*!UNIT 1` = SI, `2` = English.
* **Spec-only types** (no corpus case): unit tests from the spec's literals and branches. They
  return `M2M_E_UNSUPPORTED` in release builds until a sample project or oracle B covers them,
  unless Aleksandr decides otherwise.
* Tiers: tier 1 per packet (`ctest -L m2m-<id>`), tier 2 = 12 fixed corpus cases (one per
  operation type present), tier 3 = all oracle-A cases. Packets run tier 1 and 2; G1 runs tier 3.

## 5. Source tree and seams (created by G0)

```
apps/moproj2mst/
  CMakeLists.txt                 m2m_core, moproj2mst.dll, moproj2mst.exe, tests
  include/moproj2mst.h           C ABI (G0)
  include/m2m/model.hpp          project model seam (P01/P02/P03 fill)
  include/m2m/qt4.hpp            Qt 4 text/path seam (P04/P05 fill)
  include/m2m/mst/stream.hpp     MST output stream (QTextStream behaviour) (P04)
  include/m2m/mst/emit.hpp       one declaration per shared emitter (P07/P08 fill)
  include/m2m/mst/writer.hpp     operation writer interface: +0x280, +0x288, +0x200, +0x290, +0x298 (P09)
  src/xml/ src/moproj/ src/model/ src/datafiles/ src/qt4/ src/mst/ src/ops/ src/dll/ src/cli/
  tests/<packet>/
```

Each seam declares the function with its `@src` address and a stub body that throws
`m2m::NotPorted`. A packet replaces the stubs it owns.

## 6. Packets and waves (at most 17 threads at once)

| Wave | Packets | Starts when |
|---|---|---|
| 0 | G0 | now (alone; skeleton, seams, ledger, s1-02 spec in) |
| 1 | P01–P17 (17 threads) | G0 pushed the base branch |
| 2 | G1 gate and integration, then P18 release; fix-ups go back to the owning packet | packets push; G1 merges continuously |

Packet table (briefs in §7):

| Id | Content | Owns | Depends on | Test |
|---|---|---|---|---|
| G0 | Base branch, tree, CMake, seam headers with stubs, C ABI + DLL + CLI shell, runner wiring, folded stage 1 ledger, add s1-02 spec | `apps/moproj2mst/{CMakeLists.txt,include/moproj2mst.h,include/m2m/*.hpp,src/dll,src/cli}`, `docs/re/moproj2mst/ledger.tsv` | s1-02 spec | builds; CLI returns `M2M_E_UNSUPPORTED`; runner self-test |
| P01 | XML reader matching `QDomDocument::setContent` as the files need; `.moproj` file read `14008afa0`; DOM walk `14008b410`; item lists `14008e290`/`14008e3d0`; Title/Unit/Primary_DB_Name | `src/xml`, `src/moproj/read.cpp` | G0 | load all corpus files; field dump vs s1-01 §9 |
| P02 | Operations: `14008cb50`/`14008ccf0`, factory `140046cc0`, registry `14006dba0`/`14006f940`, unique names `140047630`, OPI ctor/defaults `14004fcd0`/`140050550`, objects `140093c30`, NSTART/UserDefinedKey/RestartKey, properties `140094f70`/`1400952c0`, ActionKeywords `140095cc0`/`140096390`/`140906e40`, OBJPOS records `1400658e0`, accessors (s1-01 §10) | `src/model/operation*.cpp`, `src/model/accessors.cpp` | P01 seam | unit tests per field; legacy-type map |
| P03 | Sub-projects and order: SUB `1400b5470`, activation `1400a7250`/`1400a7b90`/`14008f530`, order `1400b6630`, project ctor defaults `140089880`, project record P | `src/model/project.cpp`, `src/model/subproject.cpp` | P01 seam | operation order = OPRNAM order of every corpus MST (s1-01 `tools/order.py`) |
| P04 | Qt 4 numbers and MST stream: `QTextStream` `<<` int/double (Qt 4.8 defaults, `-0.0`→`0`), `QString::number`/`arg` as used, `endl` = LF, encoding | `src/qt4/number.cpp`, `src/mst/stream.cpp`, `include/m2m/mst/stream.hpp` | G0 | golden vectors: every number in corpus MSTs with its source value (from KEY/MDT); behaviour from `QtCore4_64.dll` copy |
| P05 | Qt 4 paths: `QDir::relativeFilePath`, `cleanDirPath`, `convertSeparators`, `QFileInfo` baseName/suffix/absolutePath, case rules on Windows | `src/qt4/path.cpp` | G0 | path vectors from corpus MSTs and specs |
| P06 | Data files read at open: `Cogging.MDT` (`140761c90`, `14076a460`, `140761d90`, defaults `1407613b0`, `1407656d0`, `1407614e0`, SfOprCogData ctor `140767a20`), `Boolean.BWZ`, `Forming.FWZ`, `Cycle.CWZ`, `DieStressBmWizard.Wzd`; the `PROPS->vt[0xc0]` dispatch | `src/datafiles/*` | G0 | every corpus MDT/BWZ/CWZ/FWZ parses; values vs s1-03 `tools/mdt.py` |
| P07 | MST top level: `1400a20d0`, path `1400a50a0`, `1401313c0`, header `140131680` (GEOTYP `140c0df50`, UNIT `1400a2180`), `1401318b0`, walk `140131df0`, `140132100`, trailer `140131d10` | `src/mst/master.cpp` | P04 seam | header, project block, trailer of every corpus MST |
| P08 | Shared emitters A: `140131c10`, `140134250`, `140134360`, `1401343e0`, `140134510`, `140134680`, `140134730`, `1401347e0`, `140134900`, `140134a10`, `140134bf0`, `140134db0` | `src/mst/emit_a.cpp` | P04 seam | unit vectors from s1-02; lines in corpus MSTs |
| P09 | Shared emitters B: `140135220`, `140135680`, `140135b80`, `140135da0`, `140135f10`, `140135fc0`, `140136040` (→ `1400ac010`), `140136090`, `1401360e0`, `1401365f0`, `140136810`, `140132a40`, `1401322d0`, `140132bd0` | `src/mst/emit_b.cpp` | P04 seam | as P08 |
| P10 | Base writer `1405f5340`, hooks `1405f56c0`, `1413e5ef0`, `1405f79b0`, +0x290; writer interface and the class table of the HeatTransfer/Forming/Pre/HTFurnace/Express-base family | `src/ops/base.cpp`, `src/ops/family.cpp`, `include/m2m/mst/writer.hpp` | P07–P09 seams | oracle A: HeatTransfer and Forming cases (e.g. `DMO_LIB_TEMPLATES/HEAT1`) |
| P11 | Cogging writer `1407520c0`, `140752190`, PHT `1407532b0`, PDWL `140754250`, BDEF `140755b90`, BDCT/BDW/BHT/SHT, CHKSTOP, `only` mode | `src/ops/cogging*.cpp` | P06, P07–P09 seams | oracle A: 15 Cogging cases; s1-03 `tools/cogw.py`, `cmp_cog.py` |
| P12 | Boolean `1411df600`/`1411df6d0` (BooleanData `1411e23f0`; side-file writer `1411e1820` NOT run), Convert2Dto3D `1401c4440`/`1401c4520` (no deletions), CopyMirror `140e24e80`/`140e24f60` | `src/ops/boolean.cpp`, `src/ops/convert23.cpp`, `src/ops/copymirror.cpp` | P06, P07–P09 seams | oracle A: 4 Boolean2D cases; spec tests for the others |
| P13 | Cycle `14134dcc0` (per-call state for OPRNAM/SIMNAM `CYCL<k>-`), Group `141352540` and DoeOutput `140e32970` (no output), Distortion `141362e90`/`141362f60` | `src/ops/cycle.cpp`, `src/ops/noop.cpp`, `src/ops/distortion.cpp` | P10 seam | oracle A: 2 Cycle cases, 1 DoeOutput |
| P14 | MultiBlow `140602810`/`1406028b0` with its block writers and MultiBlowData `140618d20`/`14060b5d0`; next simulation number `1400b8820` and every class's +0x298 simulation list | `src/ops/multiblow*.cpp`, `src/model/simcount.cpp` | P10 seam | spec tests (no corpus) |
| P15 | FormingExpress(2D) `1414136b0`, `141412c90`, driver `14141f2f0`, BL `14141fc50`, DW `1414216e0`, TO `141422320`, objects `141423130`, SIMNAM builders `141423f20`/`141424130`/`141423d10`, SimCtrl props `1405fb6a0`, model load `14005f770` | `src/ops/express*.cpp` | P10, P14 seams | spec tests (no corpus) |
| P16 | Diestress `1402b5410`, DiestressBM2D/3D `14029ad10`/`14029cb50`, HeatTreat(2D) `1411b1460`/`1411aee40` (MOKEY files referenced, not written), Report `1412b05a0` and ShapeOptim `14013fb90` (no output) | `src/ops/diestress.cpp`, `src/ops/heattreat.cpp`, `src/ops/report.cpp` | P06, P10 seams | spec tests (no corpus) |
| P17 | Cross-class helpers: unit-string evaluator `1402ede00`, DB path `1400b9e80`/`1400a4f60`, equipment path accessor, `*!SIMTYP` source, DOE path `1401321a0`/`140131a50`/`140131530` behind `M2M_FLAG_DOE_TEMP` | `src/model/units.cpp`, `src/model/paths.cpp`, `src/mst/doe.cpp` | P04, P07 seams | unit tests; DOE spec tests |
| G1 | Gate: merge packet branches in dependency order, fold the ledger, run tier 3 on all oracle-A cases, route each DIFF to its owning packet, keep `main` untouched until Aleksandr agrees | merge commits, `docs/re/moproj2mst/status.md` | all | 46/46 PASS, no CHANGED |
| P18 | Release: DLL load tests from C++, C# and Python, README for DMO, version resource, the two big projects read-only | `apps/moproj2mst/tests/abi`, `apps/moproj2mst/README.md` | G1 green | all three callers convert a corpus case |

## 7. Briefs (paste one into a fresh thread)

Common header for every brief:

> You work on stage 1 of DEF_GUI_MO in the private repo `claude-deform-rea` on Sim2: a DLL that
> converts a DEFORM `.moproj` project into the `.MST` that DEF_GUI_MO_64.exe's File > Save
> writes, byte for byte, without changing any project file. Read
> `docs/plan-gui-mo-stage1-moproj2mst.md` sections 1–6 and `docs/re/moproj2mst/RULES.md`
> first and follow them strictly. Base branch: the one G0 created (see the plan's status
> section, or ask the coordinator). Your branch: `claude/m2m-<id>`. Specs are in
> `docs/re/moproj2mst/`; raw decompiles and sample copies are in
> `C:\Users\alext\Projects\deform-rea-work\gui_mo\s1\s1-0x\`; the exe copy is
> `...\gui_mo\DEF_GUI_MO_64.exe`; for new decompiles copy `...\gui_mo\gproj` to your own
> `gproj_<id>` and use `...\gui_mo\scripts\DecompAt.java` (see `...\gui_mo\run.sh`). Own only the
> files listed for your packet. Report to the coordinator when your branch passes its tests.

Packet-specific part:

* **G0:** Create the base branch `claude/m2m-base` from `main`. Build the tree of §5 with
  CMake (MSVC 2022 via `vcvars64.bat`, Ninja, C++20, `/W4`, `/MT`, fidelity flags from
  `port/cmake/DefPort.cmake`). Write every seam header from the specs: one declaration per
  function in §6 with its `@src` address and a stub. Implement the C ABI of §3 (DLL exports,
  temp-file-and-rename write, message buffers) and the CLI; with stubs the call returns
  `M2M_E_UNSUPPORTED`. Fold all `s1-0x/ledger.tsv` into `docs/re/moproj2mst/ledger.tsv` with a
  `packet` column from §6. Wire `runner/run_corpus.py` as a CMake target (`m2m_tier3`). Write
  the status section at the end of this plan (base branch name, how to build). Push, then tell
  the coordinator that wave 1 can start.
* **P01:** Implement `src/xml` (a small XML reader with the `QDomDocument::setContent`
  behaviour these files rely on: entities, whitespace, attribute values, DOCTYPE) and the
  `.moproj` read and walk of s1-01 §2–4 into the model seam. Test: every corpus `.moproj` loads;
  a dump of the fields of s1-01 §9 matches what the spec says for `COG_P1_B1` and `HEAT1`.
* **P02:** Implement operations per s1-01 §5–7 and §10: parse, factory with all 30 registry
  keys and legacy mapping, unique names, OPI and OBJ defaults, NSTART/UserDefinedKey/
  RestartKey, properties, ActionKeywords tokenising into fields, OBJPOS records, every
  accessor the writers call. Test: unit tests per field and type mapping; all corpus projects
  build their operation list.
* **P03:** Implement the project record, sub-project records, flow-chart activation and the MST
  order of s1-01 §1.1, §8, §9.1. Test: for each corpus case, the operation order equals the
  `OPRNAM` order in the saved MST (port s1-01 `tools/order.py` as a ctest).
* **P04:** Implement `QTextStream` output as DEF_GUI_MO uses it (Qt 4.8: int, double with
  default `%g`-like precision 6 unless the spec says otherwise, `-0.0` printing, `endl` = LF,
  Latin-1/locale codec) and `QString::number`/`arg` forms used by the specs. Derive behaviour
  from the specs and, where needed, static reading of `C:\Program Files\SFTC\DEFORM\v11.0.2\QtCore4_64.dll`
  through a copy. Test: golden vectors from numbers in corpus MSTs with their source values.
* **P05:** Implement the `QDir`/`QFileInfo` functions listed in §6 with Qt 4.8 Windows behaviour.
  Test: every path in the corpus MSTs regenerated from the project folder and KEY paths.
* **P06:** Implement the open-time data-file readers of s1-03 §5 and s1-04/s1-05 (`Boolean.BWZ`,
  `Forming.FWZ`, `Cycle.CWZ`, `DieStressBmWizard.Wzd`), including the positional fscanf
  behaviour (float widening, `%d` writes). Test: every corpus data file parses; values equal
  s1-03 `tools/mdt.py`.
* **P07:** Implement the MST top level of s1-02 (path, header, project block, walk, trailer,
  output through the P04 stream). Test: header, project block and trailer bytes for every
  corpus case, with operation bodies stubbed.
* **P08 / P09:** Implement the shared emitters in your half of §6 exactly as s1-02 specifies,
  including non-default arguments other writers pass (s1-04 Q5, s1-05 Q7). Test: unit vectors
  per emitter; matching lines in corpus MSTs.
* **P10:** Implement the base writer and its hooks (s1-02 for `1405f5340`, s1-03 §2), the writer
  interface header, and the class table. Test: oracle A on HeatTransfer and Forming cases once
  P07–P09 are merged (until then, with their branches merged locally).
* **P11:** Implement Cogging per s1-03 §3–4. Test: oracle A on the 15 Cogging cases; s1-03's
  Python prototype as a cross-check.
* **P12 / P13:** Implement the writers in your row per s1-04 §1–6 without side files. Test:
  oracle A on Boolean2D (P12) and Cycle/DoeOutput (P13) cases; spec tests for the rest.
* **P14:** Implement MultiBlow per s1-04 §7 and the simulation-count machinery (`1400b8820`,
  +0x298 for every class; s1-04 Q2, s1-05 §5.6). Test: spec tests.
* **P15:** Implement FormingExpress per s1-05 §5. Test: spec tests; list what a sample project
  needs.
* **P16:** Implement the writers in your row per s1-05 §1–4. Test: spec tests.
* **P17:** Implement the helpers of your row (s1-04 Q4, s1-05 Q1/Q2, s1-01 open questions on
  `*!SIMTYP`) and the DOE path of s1-05 §7. Test: unit tests; DOE spec tests.
* **G1:** Merge packet branches into `claude/m2m-base` in dependency order, keep it building,
  run tier 3, write `docs/re/moproj2mst/status.md` (per case and per ledger row), and send each
  DIFF to its owner with the first differing line. Do not push to main unless Aleksandr agrees.
* **P18:** After G1 is green: ABI tests from C++, C# (P/Invoke) and Python (ctypes), a README
  for DMO with the header, error codes and an example per language, and a read-only run on the
  two big projects in `DEFORM_examples\big_projects`.

## 8. Open questions carried into the packets

From the specs (owner in brackets): `*!SIMTYP` for legacy types prints the mapped name per the
code, the only samples are 2013 files [P02/P17]; Qt 4 double formatting [P04]; MDT defaults for
versions below 11000 [P06]; MSVC `%f` overflow in MDT parsing [P06]; whether writers read
`ObjProperties` files from disk [P07–P09]; default unit when `<Unit>` is missing [P01];
property-tree defaults per class [P02]; renaming of duplicate names (no oracle) [P02];
MultiBlow/Express `NEXTSIMNO` uses the current sub-project [P14]; SimCtrl+0x1ec default [P15];
whether to port the side-file writers for runnable projects [Aleksandr, later].
From s1-02 §11: source of GEOTYP (project+0x24) and title/unit fields in the moproj [P01/P03];
operation objects are one shared instance per class, so hooks must not keep state between
operations unless the original does [P10–P16]; flow-chart previous/next links and group
rectangles rebuilt from `<DiagramItem>`/`<CellPos>`/`<Connections>` [P03]; the second CURSIM
number uses the active sub-project (project+0x50) while the walk uses sub-project 0 [P03/P07];
DOE path writes `.COR` files and DoeStudies folders, so DOE stays `M2M_E_UNSUPPORTED` unless
Aleksandr wants it [P17]; dimension flags at `*(op+0x38)+0x80` drive DIM and 2D/3D [P02/P10];
`inf`/`nan` spelling [P04].
Engine model (P15, 2026-10-22): `14005f770` creates the model on demand (`1405f6410`: a default
DB_STEP_DATA(dim), no file read), so writers see ctor values (`engineFacts`). Open: the GUI may have
loaded an operation's KEY files into that model before Save (the Active operation after open, s1-01
Q4: `140047f80` -> `140051340` -> OB vt+0x58); the port does not preload. Revisit if tier 2/3 shows
diffs on active operations [P01/P02, P15].

Registry (P01 for P02, 2026-10-22, accepted by the coordinator): `14006dba0` inserts 28 classes,
not 30. `SfOprGroup` and `SfOprCopyMirror` are never registered (`14006f940` has no other caller;
the Group ctor `141351f50` is only called from the Cycle ctor), so a `<Type>` naming either fails
the open as in the original. No corpus project uses them.
`*!SIMTYP` of the legacy express types (P01, 2026-10-22): the only samples (46, 52, 53) are MSTs
saved in 2013 by an older GUI; they print the legacy name (`SfOprAirConvect2D`). In v11 the header
`140131c10` prints OPI+0x120 (`140050d00`), which the factory sets to the mapped class
(`SfOprHeatTransferExpress[2D]`); nothing after the factory rewrites it (callers of `140050c80`:
the factory, UI `14001d650`, FormingExpress UI `141424340`; the Express post-load `141402060` ->
`1413f83e0` only reads Forming.FWZ). The port keeps the v11 behaviour; 46/52/53 are excluded from
the v11 oracle for this line [G1].
Case 56 (2013 MST) has `*!TITLE  Multiple_Operations_Example1` while v11 reads the title from
`<Title>` text() (empty there); exclude it from the v11 oracle with its duplicate names [G1].

InterObject block on load (P09, resolved 2026-10-22, coordinator): the port follows the binary. The
OPI ctors `14004fcd0`/`14004fe20`/`14004ff80` call `1400651f0` (INICTC 1, enable 1, GENCTC on) and
`140095cc0`/`140096390` only set flags, so an operation with INICTC and no GENCTC keyword writes
`GENCTC 0`. GENCTC is cleared (`1400ab7e0` with 0) only on GUI or writer paths (InterObject page apply
`140f3c500`, Express slot +0xe8 `1413f91f0`, writers `140e24f60`/`1411df6d0`), so files saved without
reopening can lack it. Oracle B (P12): v11 open+save of 11, 19, 46, 49 and 52 adds `GENCTC 0` after the
INICTC lines, and a second open+save gives identical bytes (gui_mo/oracle_b/R11, R19, R46, R49,
82_R52_BEND_2D). The oracle-A MSTs of those cases are pre-reopen saves: runner waiver `unopened-genctc`
in cases.tsv rewrites their expected MST the same way (checked equal to the R11/R19/R46/R49 MSTs).
An earlier P09 port rule (clear the block before the keywords) was dropped [P09].

MultiBlow root property name (P14, 2026-10-22, open for a decision): the root template `0x143d9ef10`
is process-wide and `0x140375640` fills it only while it is null, inside the props ctor path
`14060b500 -> 14060b5d0 -> 14060e2c0`. The only callers of that ctor are the classes' vt+0x98
`140601320` (2D, dim 2) and `140601cb0` (3D, dim 3), i.e. an operation being created or opened.
Neither the registry `14006dba0` (it builds SfOprMultiBlow 3D at its line 127 before 2D at 136,
handlers only) nor the class inits `140600a50`/`140601480` (their vt+0x98 call is a QColor setter on
a sub-object) nor the factory `14061a5b0` makes props. So a fresh v11 GUI names the root after the
first MultiBlow operation it creates or opens: a fresh open of 75_B_MB3 (3D only) would name it
"MultiBlowForging3D", miss the saved "MultiBlowForging2D" root and keep the defaults. The oracle-B
file says "MultiBlowForging2D", so the session that saved it had made a 2D MultiBlow first (inferred:
probably 74_B_MB2 in the same session; not checked). Port now: root fixed to "MultiBlowForging2D" (passes 74
and 75). Faithful alternatives: (a) per-call first-wins from a null start (fresh GUI; fails 75 against
its oracle B), (b) per-call first-wins seeded with the root name found in the .moproj (reproduces the
session that saved the file). Decision (Aleksandr 2026-10-22: reproduce the original one to one, nothing silent): one
conversion = one fresh GUI process, so the template is first-wins from null per conversion
(Project::multiBlowRoot via the thread-local ProjectScope; no process global). When a MultiBlow
operation's saved MultiBlowForging* root differs from that name, the original silently ignores the
saved settings; the port writes the same MST and returns a warning (M2M_E_INPUT with
M2M_FLAG_STRICT). Oracle B: the batch ran in ONE GUI process (oracle_b/_tools/run.py opens each case
in the running GUI; 74_B_MB2 was saved 16 s before 75_B_MB3), so 75_B_MB3 and 79_B_MB3X carry
session state and now DIFF (DOESCL). Open: (1) a fresh-process Oracle B run of the 75/79 inputs is
needed to confirm the fresh-GUI prediction; (2) every GUI-created MultiBlow operation saves
UseRelativeEnergy = false and writes absolute DOESCL, while the data ctor 0x140618d20 sets MBD+0x88 = 1
and the template default (0x140374300 for 0x143d9f0f0) is true; the writer of the false value is not
found yet [P14].

## 9. Status

* 2026-10-22: all five specs done (s1-01 to s1-05). Corpus built and runner self-tested. 
* 2026-10-22 G0 done. **Base branch `claude/m2m-base`** (from `claude/m2m-plan` 511cc196). Build:
  `apps\moproj2mst\build.cmd [Release|Debug]` (vcvars64, Ninja, C++20, /W4, /MT, DefPort fidelity
  flags, defrt linked) then `ctest --test-dir apps\moproj2mst\build -L m2m-<id>`; corpus:
  `cmake --build apps\moproj2mst\build -t m2m_tier2` (12 cases, `runner/tier2.txt`) or
  `-t m2m_tier3` (oracle A). Corpus path is the CMake cache variable `M2M_CORPUS`.
  * Seams: `include/m2m/{model,props,datafiles,qt4,qt4path,xml,convert,common}.hpp` and
    `include/m2m/mst/{stream,state,emit,writer}.hpp`; each declaration carries `@src` and its owner.
    Stubs: `src/stubs/p<NN>.cpp`, one per packet, throwing `m2m::NotPorted` (the ABI returns
    `M2M_E_UNSUPPORTED` with "not ported yet: 0x..."). A packet deletes its stubs as it ports.
  * Tests: `tests/<packet>/test_<topic>.cpp` are globbed into `m2m_<packet>_<topic>` with labels
    `m2m`, `m2m-<packet>` and `// ctest-labels:` extras; `// ctest-standalone: <files>` builds a test
    from the listed sources without `m2m_core` (test doubles). Env: `M2M_CORPUS`, `DEF_PORT_REPO`,
    `M2M_CLI`.
  * ABI additions to section 3: `m2m_set_gui_unit(0|1|2)` (s1-01 Q2, 0 = derive as the original);
    `out_mst_path` equal to the project file is `M2M_E_ARG`; `M2M_FLAG_DOE_TEMP` is
    `M2M_E_UNSUPPORTED`; unknown flags are `M2M_E_ARG`.
  * Ownership changes against section 6: P09 also owns `1401363b0`, `1400613b0`, `14004e830`,
    `14004f1c0`, `14004f050`, `140065af0`, `140065f80`, `1400ac010`; P08 also `140058e30`, the DEF_PRE
    keyword table `140921f30`/`140922160`, `cyclePrefix`, `itemKeyPathClean`; P02 also `OpClass::
    makeProps(guiUnit)`/`displayName`, `defaultPropTree`, the ActionKeywords tokenizer `140906e40`
    (P06 owns the BWZ/CWZ reader `1409068f0`); P03 also `1400b7470` (group members); P14 owns every
    +0x298 sim-list body (base `1405f5880` included) and MultiBlow property slots; P15 owns
    `hasEngineModel` (`14005f770`) and `data/common.hpp` (defaults by P02); each writer packet owns
    its `make<Class>()` factory, its class's `Props::applyValue` override and OB fields such as
    Boolean's 2D flag; the cycle/KFWRIT setters `140136af0`..`140136b40` are inline in
    `mst/state.hpp` (G0). P04 owns `cmake/release.cmake` and `src/dll/version.rc` (release work).
  * Model structs are G0's; a packet adds fields in an "added by Pxx" block, G1 merges.
  * Ledger: `docs/re/moproj2mst/ledger.tsv` (992 rows) from `tools/fold_ledger.py`; `packet` comes
    from the seams (`assign=seam`) or from address/role rules (`assign=rule`, check when porting).
    `--ported P08` marks a merged packet's rows; the integrator re-runs it as packets land.

