# Plan: faithful C++ reconstruction of DEF_GUI_MO_64 (Multiple Operations GUI)

Target: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe` (67 MB, PE32+, link time
2014-12-04, MSVC x64, Qt 4.8). DEF_GUI23 starts it as `DEF_GUI_MO_64.EXE -m open -f <file>` or
`-m new -prob_dir <dir>`.

Goal (Aleksandr, 2026-10-21): a modern C++ application that is fully compatible with the original,
visually and internally, with every application function recreated as it is in the binary.

Status: plan only, nothing implemented. Aleksandr will revise it; the defaults this plan picks
where the request leaves a choice open are collected in section 9.

Rules carried from the project:

* This is a separate project from the DEF_PRE batch port. It reuses that port's libraries where the
  two binaries share code (section 1.4), but it has its own tree, ledger, branches and gates.
* Recovered code and findings stay private (private origin only).
* Structure, layouts and algorithms come from the code. Example projects are used only to test
  what the code shows, always on copies. DEFORM binaries and files are never modified.
* Licence, Sentinel and process-monitor code (`CLManagerClient*`, `CDEFORMLMObject`,
  `Csdspm*`, `CDFMProcessMonitor`, `SfLicenseLogData`, `FTN_license.f`) is fenced at its call
  boundary and replaced by a stub. It is never analysed, reconstructed or bypassed.
* Running the original GUI on copies for pixel-level visual matching (oracle C) was approved by
  Aleksandr on 2026-10-22 (section 9, D1). Driving it to produce output files (oracle B) and
  attaching a debugger are not covered by that approval. Apart from oracle C, every oracle in this plan is static or uses files the original
  already wrote.

Starting point: `docs/re/gui-mo-moproj-mst.md` (branch `claude/gui-mo-research`) traces
open → Save → MST. The numbers below come from `docs/re/gui_mo_survey.py` on a copy of the exe.

## 1. What is in the binary

### 1.1 Size

| Part | Functions | Code |
|---|---:|---:|
| Whole image (`.pdata`) | 189 695 | 27.3 MB |
| SEH unwind funclets, 0x141800000–0x141cfffff (25–28 byte thunks) | 148 242 | 4.3 MB |
| Static initialisers and atexit destructors, 0x141d00000– | 2 324 | – |
| **Application range, 0x140001000–0x1417fffff** | **39 129** | **21.8 MB** |

That is about 4.5 times the application code of DEF_PRE_64 (8 666 functions, 4.0 MB). Of the
39 129, 28 505 are longer than 64 bytes.

The application range still contains linked-in libraries (section 1.3). Subsystems by address
locality of the 9 777 vtable slots (MSVC keeps one object file's functions together, so a
function is attributed to the class whose vtable slot last preceded it; rough, refined by the
Phase 0 ledger):

| Subsystem | Functions | Code | Main classes |
|---|---:|---:|---|
| Pre-processor panels and engines | 6 518 | 7.0 MB | `SfPre*` (82), `SfWindef`, materials, meshing, Fortran pre kernels |
| Post-processing tasks, graphs, reports | 5 197 | 2.8 MB | `SfTask*` (126), `SfSv*`, `SfGraph*`, `SfChart*`, `SfReport*`, `SfFN*`, `SfPtrk*` |
| Shared engine (same families as DEF_PRE) | 3 714 | 1.6 MB | `LLOBJ_DATA`, `OPSTOP_DATA`, `DYNARY_DATA`, `StateVar*`, `_OBJ_GEO2D/3D`, `_DEF_*`, `Dds*`, `Uic2CObject*` |
| Qt library code linked in | 3 641 | 1.1 MB | template instantiations, Qt Solutions property browser, `QFormBuilder` |
| MO shell: main window, flow chart, explorer | 3 560 | 1.6 MB | `SfMo*` (43), `SfDiag*` (14), `SfMainWindowBase`, undo/redo |
| Display (OpenGL and graphics scene) | 3 468 | 1.6 MB | `SfDisp*` (29), `OGLDataNew`, `SfScene`, `LayerView`, libQGLViewer |
| Operations | 2 702 | 1.1 MB | `SfOpr*` (88): 21 operation types |
| DOE studies | 1 049 | 0.6 MB | `SfMoDoe*`, `SfDoe*`, `DefDoe*`, `SfTaskDoe*` |
| STL, exceptions, iostreams | 1 535 | 0.7 MB | Dinkumware STL instantiations |
| SQL tables | 440 | 0.2 MB | `DefSql*` over `QSQLITE` |
| Licence and process monitor (fence only) | 835 | 0.2 MB | see rules |
| Not yet attributed | 6 470 | 2.7 MB | utilities, Fortran objects without vtables, CRT |

### 1.2 Classes, UI and resources

* RTTI names 1 390 types. Application families: `SfTask` 126, `SfOpr` 88, `SfPre` 82, `SfSv`
  81, `SfItem` 46, `SfMo` 43, `Sf2D` 30, `SfObject` 30, `SfDisp` 29, `SfGraph` 22, `SfState` 22,
  `SfDiag` 14, `SfStep` 11, `DefSql` 10, `SfUnits` 10, `SfFN` 10, plus about 150 smaller ones.
* 21 operation factories: Forming, FormingExpress, HeatTransfer, HeatTransferExpress,
  HeatTreat, HTFurnace, Cogging, MultiBlow, Cycle, Group, Boolean, CopyMirror, Convert2Dto3D,
  Diestress, DiestressBM, Distortion, ShapeOptim, Pre, Report, DoeOutput, and the base.
* 2 541 distinct `slot…(…)` signatures in moc string data; 101 classes carry `qt_metacast`.
  Widgets are mostly built in code (uic output compiled in), with 13 `.ui` files loaded at run
  time through `QFormBuilder` (the furnace and fixture dialogs: `sfPrebfurnace_*.ui`,
  `SfOprHTFurn*.ui`, `SfOprFurnLoadPattern.ui`, `SfOprFxtrPattn.ui`, `ySfOprWKinFxtr.ui`).
* Embedded Qt resources: 706 (582 PNG, 101 XPM, 13 UI, 9 ICO, 1 GIF): icons, function images
  (`FlowStressFuncImage*.xpm`), chapter banners, logos.
* Imports: QtCore 977, QtGui 3 268, QtXml 86, QtSql 73, QtCommercialChart 58, QtOpenGL 32,
  QtNetwork 13, QtSvg 8, OpenGL 96, GLU 19, plus Win32 (KERNEL32, USER32, ADVAPI32, WS2_32,
  COMDLG32, OLE). No DEFORM DLL is imported.

### 1.3 Linked-in third-party code

These are identified, not reconstructed function by function. Each gets a replacement that
behaves the same where the application can observe it (section 3.2).

| Library | Evidence | Replacement |
|---|---|---|
| Qt 4.8 inline and template code | `QList`, `QMap`, `QHash` instantiations | Qt 6 plus `qt4compat` |
| Qt Solutions property browser | `QtVariantPropertyManager`, `QtProperty*`, `QtAbstract*` | Same component, ported to Qt 6 |
| `QFormBuilder` (Designer static lib) | `FormBuilderPrivate` | `QUiLoader` / `QFormBuilder` from Qt 6 UiTools |
| libQGLViewer | `Camera`, `Frame`, `ManipulatedFrame`, `KeyFrameInterpolator`, `MouseGrabber`, about-box string | libQGLViewer of the matching version, Qt 6 build |
| QtCommercialChart 1.x | import table | Qt Charts (its successor) behind an adapter |
| Dinkumware STL, MSVC CRT (static), PathScale Fortran runtime, `f90_intrinsic` `sgemmx`/`sgemvx` | strings, idents | C++ standard library plus `defrt` fidelity helpers (CRT `printf`, binary `libm`) |

### 1.4 Fortran modules and overlap with DEF_PRE

64 PathScale idents (`-O2 -march=opteron -msse2 -mno-sse3`, as in DEF_PRE):

| Group | Modules |
|---|---|
| Shared with DEF_PRE (already ported or in the port plan) | `datarm23.f`, `DBOperations23.f`, `dds_c_*.f`, `position.f`, `Volume.f`, `dspmdf2_cross_section.f`, `FTN_DieGeo_cross_section.f`, `FTN_IgesInterface.f`, `FTN_IgesCore.f`, `DefGraf.f`, `FTN_DefGraf3D.f`, `LCPlot.f`, `defutl` maths and utilities, `st.f` |
| Post-processing (new) | `defpost` `FTN_PostFortran2D/3/23.f`, `FTN_CylCoord.f`, `FTN_usrvar*.f`, `FTN_usrvio23.f` |
| Point tracking (new) | `defptrk` `FTN_PTExtract/Interface/Functions 2D/3D.f`, `velgrd3.f`, `defgrd3.f` |
| Flow net (new) | `deffn` `FTN_FNPattern*.f`, `FTN_FNExtract*.f`, `FTN_FNTrack2D.f`, `FTN_FNFunctions3.f`, `FTN_FNInterface3.f` |
| Slicing (new) | `defslc` `FTN_Slicing.f`, `FTN_SliceArea.f` |
| Geometry (new) | `def_morp2_lib.f90`, `crndetect.f90`, `surf_fit.f`, `laplace-bcc.f`, `setupa.f` |
| Licence (skip) | `FTN_license.f` |

The C++ engine families (`LLOBJ_DATA`, `OPSTOP_DATA`, `DYNARY_DATA`, `StateVar*`,
`SdctlGList`, `_DB_STEP_DATA`, `_OBJ_GEO2D/3D`, `OBJ_MESH2D`, `_DEF_GEOPRIM`, `Dds*`,
`Uic2CObject*`) are the same sources as in DEF_PRE_64. Phase 0 matches them to the DEF_PRE
ledger function by function, so already-ported code is linked, not ported twice.

## 2. What "fully compatible" means

Acceptance is checked per packet and again at the end (Phase 9):

1. **Files.** For the same project and the same user actions the reconstruction writes the same
   bytes: `.moproj`, every KEY file under `Objects/`, `Materials/`, `Lubricants/`,
   `Equipments/`, `Operations/TaskNNNNN/`, the `.MST`, `.MST.TEMP`, `DEF_MULTI.INI`,
   DOE study folders and sample files, the SQLite tables, and the `QSettings` entries.
2. **Processes.** Children (DEF_PRE, DEF_SIMULATION chain helpers, any other tool) get the same
   executable, argv, working directory and redirections.
3. **Visual.** Every window, dock, dialog, menu, toolbar, context menu, tooltip and message box
   has the same widget tree: class role, object name, text, icon, shortcut, enabled state,
   geometry at the reference DPI. Rendered pixels match the original screenshot within a set
   tolerance per widget (fonts and anti-aliasing differ slightly between Qt 4 and Qt 6; the
   tolerance is fixed in Phase 0 from a calibration set). The OpenGL views match on a fixed
   camera and scene.
4. **Internal.** Every application function in the ledger has a C++ counterpart that names its
   source address, with the same class hierarchy and virtual slot order, the same algorithm and
   the same floating-point operation order. Status per function: `todo`, `ported`, `verified`,
   `library`, `twin` (reused DEF_PRE port function), `fenced` (licence), `not-needed`
   (dead code proven unreachable).
5. **Drop-in.** DEF_GUI23 can start the reconstruction with the same arguments, and a project
   saved by either program opens in the other.

## 3. Architecture

### 3.1 Tree

```
mo/                          new top-level tree, own CMake project
  cmake/                     MoPort.cmake: mo_module(), FP fidelity flags shared with port/
  qt4compat/                 Qt 4.8 behaviours that reach files or pixels (3.2)
  thirdparty/                property browser, libQGLViewer, chart adapter (Qt 6 builds)
  mocore/                    project model: SfMoModel, item managers, ids, units, settings
  moio/                      .moproj reader/writer, KEY file layout, MST framework, Save/Open
  moops/                     21 operation types: data, KEY save, MST writer, preview data
  mokern/                    Fortran ports new to this binary (post, ptrk, flow net, slicing, morph)
  modisp/                    OpenGL rendering, picking, display controls, scene graphics
  moui/                      widget kit, main window, flow-chart editor, explorer, dialogs
  mopre/                     SfPre* panels and operation property pages
  mopost/                    SfTask*, SfSv*, graphs, charts, reports
  modoe/                     DOE studies, shape optimisation, DefSql tables
  def_gui_mo/                the executable: startup, argv, resources (.qrc from Phase 0)
  tests/                     tier 1/2/3, visual harness
```

Dependencies point down: `port/{defrt,defmodel,defkey,defdbio,defcmd}` and `deformdb` →
`qt4compat` → `mocore` → `moio`, `mokern` → `moops` → `modisp` → `moui` → `mopre`, `mopost`,
`modoe` → `def_gui_mo`. The DEF_PRE port is consumed as a library and is not edited from this
project; a change it needs goes to its owners as a request.

### 3.2 Toolchain and Qt 4 fidelity

* C++20, MSVC x64, CMake, `/fp:precise`, no FMA contraction, SSE2 only (as the DEF_PRE port).
* Qt 6 LTS. Qt 4 behaviours that change bytes or pixels are reproduced in `qt4compat`, each
  derived from the code in `QtCore4_64.dll` / `QtGui4_64.dll` / `QtXml4_64.dll` of the install
  (static analysis of copies) and covered by tests. Known ones to check first:
  * `QDomElement` attribute order on `toString(1)`: Qt 4 stores attributes in a `QHash`, so the
    written order is the Qt 4 hash order. Qt 6 keeps insertion order and seeds hashes. The writer
    must emit the Qt 4 order, or `.moproj` files will not match.
  * `QString::number` / `arg` with `double`, `QTextStream` default codec, line ends and real
    number notation (MST and KEY text).
  * `qCompress` zlib level and stream bytes (`ObjPropertiesData` blobs).
  * `QSettings` value encoding, `QVariant` to string, `QDir::relativeFilePath`, `QFileInfo`
    suffix rules.
  * Style metrics, default fonts, palette and the "windows" style drawing used for visuals.
* Legacy OpenGL (compatibility profile, fixed function plus GLU) is kept as the binary uses it,
  so render output can match.

### 3.3 Fidelity rules

As in `docs/port-conventions.md`: one C++ function per binary function where practical, an
`@src 0x14…` marker on each, original names where RTTI or strings give them, class layouts
from constructors, the ledger as the single source of status. A mismatch is traced to the
code, never patched from the expected output.

## 4. Oracles and tests

| Oracle | What it checks | Needs |
|---|---|---|
| A. Round trip on saved projects | Open a project the original last saved, save it unchanged, compare every written file byte for byte. The original's own output is the reference (the method `p-dbread` used for DBREAD). | Nothing new. Copies of `C:\PROBLEM\TEST_REA\*`, `DEFORM_examples`, shipped `.moproj` examples. |
| B. Scripted original | Drive the original GUI on a copy through UI Automation (open, edit, save, generate DB), cache outputs by hash of inputs and script, run each case once ever (as the DEF_PRE oracle runner). | Approval D1b-files (not yet given). |
| C. Visual capture | UI Automation tree dump plus screenshots of every window, dialog and menu in fixed states, captured once and cached. | Approved 2026-10-22 (D1). |
| C′. Static layout | Layout spec recovered from the compiled `setupUi` code and the 13 `.ui` files: widget tree, object names, texts, layouts, size policies. Used to explain C diffs and for states the capture cannot reach. | Nothing new. |
| D. Leaf routines | Pure maths leaf routines called in a mapped copy of the exe, the method Aleksandr approved for DEF_PRE (`p1-01`). | Same limits as that approval; extending it to this exe is D1b. |
| E. Downstream | The reconstructed MST and KEY files run through the original DEF_ARM_CTL3 / DEF_RDMS3 / DEF_PRE chain and give the same `.DB` (DEF_PRE oracle cache). | DEF_PRE runs headless, already allowed. |

Test tiers follow the DEF_PRE plan (section 6 there): tier 1 per module in seconds, tier 2 a
fixed smoke set under a minute before each commit (a dozen projects covering all 21 operation
types, plus one screen per main window area), tier 3 full corpus and full visual set at gates.
Comparators hash first and diff only on mismatch; the `.moproj` comparator reports the first
differing element path; the visual comparator reports the first differing widget.

## 5. Phases and packets

Packets are sized to about 150–400 ledger functions (or one coherent class family) so a thread
finishes one in a few sessions. Packet ids are `mo<phase>-<nn>`, branches `claude/mo<phase>-<nn>`.
Exact function lists come from packet carving (`mo0-17`); the lists below fix the scope and
the owner of each class family.

### Phase 0: ledger, tooling, oracles (17 packets, all parallel except mo0-17)

| Packet | Content |
|---|---|
| mo0-01 | Headless Ghidra analysis of the copy, split by region into four projects; function list, decompile cache |
| mo0-02 | Function ledger: all 39 129 application functions plus 2 324 initialisers, area, subsystem, status column |
| mo0-03 | RTTI, vtables, class hierarchy, constructor-derived layouts for 1 390 types |
| mo0-04 | Qt moc recovery: per class signals, slots, properties, enums; every `connect()` site as a wiring table |
| mo0-05 | Library identification (section 1.3): mark `library` rows, pin versions (Qt 4.8.x, property browser, libQGLViewer) |
| mo0-06 | Twin matching against DEF_PRE_64 and its port ledger (byte and call-shape fingerprints); mark `twin` rows |
| mo0-07 | Resource extraction from the copy: 706 files, regenerated `.qrc`, hash manifest |
| mo0-08 | User-visible string catalogue: menu texts, tooltips, messages, `tr()` literals, with code references |
| mo0-09 | Runtime I/O inventory: files read and written, `QSettings` keys, env vars, argv, every child process with exact command line |
| mo0-10 | Static initialisers: globals, tables, factory registrations, option lists |
| mo0-11 | Corpus and oracle A: collect projects (copies), comparators for `.moproj`, KEY, MST, SQLite |
| mo0-12 | Oracles B and C: UI Automation driver and capture harness, oracle C enabled on copies of the original (D1 approved); oracle B scripts built but run on the original only after D1b-files |
| mo0-13 | Oracle C′: static layout extractor from `setupUi` code and `.ui` files |
| mo0-14 | `qt4compat` specification: list each Qt 4 behaviour the app reaches, with the DLL address it comes from |
| mo0-15 | Fortran module map: 64 idents to address ranges; post, ptrk, flow net, slicing, morph boundaries |
| mo0-16 | Build skeleton: `mo/` tree, CMake, Qt 6 and third-party builds, testkit, conventions addendum, licence fence stubs |
| mo0-17 | Packet carving for Phases 1–8 from the ledger closures (runs after mo0-02/03/05/06) |

Gate 0: ledger complete with no unknown area, every `library`/`twin`/`fenced` row justified,
oracle A green on the comparator self-test, packet lists published.

### Phase 1: core model and project I/O, no UI (17 packets)

| Packet | Content | Anchors |
|---|---|---|
| mo1-01 | `qt4compat` implementation and tests | from mo0-14 |
| mo1-02 | Runtime additions on top of `defrt` (Fortran runtime pieces this exe uses) | |
| mo1-03 | Units: `SfUnits*`, unit conversion, `doubleUnits` property values | |
| mo1-04 | Project model: `SfMoModel`, `SfMoManager`, `SfMoModelManager`, ids | |
| mo1-05 | Item managers and items: Obj, Mat, Lub, Eqp, Opr (`SfMo*Manager`, `SfMo*Item`) | |
| mo1-06 | `.moproj` reader, including legacy `SimCtrlKey`/`InterObjectKey` and `.d2mo` | `14008a9b0`, `14008afa0`, `14008b410` |
| mo1-07 | `.moproj` writer and list writers | `1400983c0`, `14009cfd0`, `1400a1110`–`1400a16b0` |
| mo1-08 | Sub-project and flow-chart persistence (`Sub_Project`, `DiagramItem`, camera) | |
| mo1-09 | Object, material, lubricant, equipment KEY files on top of `defkey` | `14005eb10`, `140059290` |
| mo1-10 | Operation base: `SfOprBase`, typed property tree, `ActionKeywords`, factory registry | vtable +0x68, +0x280 |
| mo1-11 | MST framework: header, main/sub project, trailer, the shared keyword emitters | `1401313c0`, `140131680`–`140136810` |
| mo1-12 | Save orchestration, per-operation checks, `LOCAL_REMESH.DAT` | `1400980d0`, `1400b8fb0`, `1400982e0` |
| mo1-13 | Open from `.DB` and from `.KEY`, `FOR003` rename | `14008b2d0`, `14008b380` |
| mo1-14 | Engine wiring: link the DEF_PRE port libraries for `twin` rows, adapters where call conventions differ | from mo0-06 |
| mo1-15 | Process runner (`QProcess` wrapper), single-operation DEF_PRE run via `DEF_MULTI.INI` | `1400a2200` |
| mo1-16 | Settings, preferences, recent files (`QSettings`, `SfMoMainWindowSetting`) | |
| mo1-17 | Startup and argv (`-m open -f`, `-m new -prob_dir`), `SfApplication`, licence fence | `140001410` (argv part only) |

Gate 1: oracle A round trip byte-identical on every corpus project for `.moproj`, KEY and MST,
with operations represented by their generic base (operation-specific MST lines come in Phase 2).

### Phase 2: operation types, model and file side (17 packets)

Each packet covers one operation family: data class, factory, KEY save (virtual +0x68), MST
writer (virtual +0x280), `.moproj` properties, preview data. Property pages come in Phase 6.

| Packet | Operation classes |
|---|---|
| mo2-01 | Forming, Forming2D |
| mo2-02 | FormingExpress, FormingExpress2D, FormingExpressCommon, ExpressBase |
| mo2-03 | HeatTransfer |
| mo2-04 | HeatTransferExpress |
| mo2-05 | HeatTreat |
| mo2-06 | HTFurnace and furnace load patterns |
| mo2-07 | Cogging and its preview |
| mo2-08 | MultiBlow |
| mo2-09 | Cycle |
| mo2-10 | Group and block items |
| mo2-11 | Boolean and its preview |
| mo2-12 | CopyMirror |
| mo2-13 | Convert2Dto3D |
| mo2-14 | Diestress, DiestressBM, DiestressBM2D/3D |
| mo2-15 | Distortion and its preview |
| mo2-16 | ShapeOptim |
| mo2-17 | Pre, Report, DoeOutput |

Gate 2: oracle A and oracle E green for every corpus project; every operation type has at least
one project in tier 2 (gaps listed for oracle B).

### Phase 3: computational kernels (10 packets)

| Packet | Content |
|---|---|
| mo3-01 | Post Fortran: `FTN_PostFortran2D/3/23`, `FTN_CylCoord` |
| mo3-02 | User variables: `FTN_usrvar*`, `FTN_usrvio23` |
| mo3-03 | Point tracking: `defptrk` 2D and 3D, `velgrd3`, `defgrd3` |
| mo3-04 | Flow net: `deffn` pattern, extract, track, interface |
| mo3-05 | Slicing: `FTN_Slicing`, `FTN_SliceArea` |
| mo3-06 | Morphing and corner detection: `def_morp2_lib.f90`, `crndetect.f90` |
| mo3-07 | `surf_fit.f`, `laplace-bcc.f`, `setupa.f` and the non-twin rest of `position.f` |
| mo3-08 | C++ geometry: `C2d3dGeometry`, `C3dCrossSection`, Boolean helpers, `SfCalcScaleGeo`, `SfCheckGeo2D` |
| mo3-09 | Post-side DB reading (`Dds*`, step data, state variables) beyond the twin set |
| mo3-10 | GUI-side mesh tools: mesh checks, coating mesh, user-defined 2D mesh data |

Gate 3: each kernel matches golden vectors from oracle D or E; no UI involved.

### Phase 4: UI kit and shell (9 packets)

| Packet | Content |
|---|---|
| mo4-01 | Widget kit: `SfCheckBox`, `SfComboBox`, `SfDoubleSpinBox`, `SfFilePathWidget`, `SfBlueTitleBar`, `SfDockWidget`, `SfAutoHide*`, `SfCentralArea` |
| mo4-02 | Property browser integration, unit-aware delegates, `ComboBoxDelegate` family |
| mo4-03 | Main window: menus, toolbars, actions, shortcuts, status bar, dock layout (`SfMoMainWindow`, `SfMainWindowBase`) |
| mo4-04 | Explorer and tree docks: `SfMoExplorer`, `SfMoTreeViewDock`, item models, drag and drop |
| mo4-05 | Flow-chart editor: `SfDiagEditor`, items, connectors, groups, expansion, magnifier, cell editor |
| mo4-06 | Undo/redo stack and commands (`SfDiagEditorUndoRedo*`, `SfMovingItemCommand`, delete commands) |
| mo4-07 | Manager views: `SfMoObjManagerView`, `SfMoOprManagerView`, `SfMoSaveManagerView` |
| mo4-08 | File dialogs, about box, preferences, every message box (texts from mo0-08) |
| mo4-09 | Look: palette, fonts, icon mapping, style metrics; shell visual parity cases |

Gate 4: the shell opens every corpus project and shows the same flow chart and trees as oracle
C (or C′); File, Edit and View menus match.

### Phase 5: display (8 packets)

| Packet | Content |
|---|---|
| mo5-01 | GL widget, `OGLDataNew`, camera and frames (libQGLViewer integration) |
| mo5-02 | 2D geometry and mesh display |
| mo5-03 | 3D geometry and mesh display |
| mo5-04 | Picking (`IPickingPrivate`, all `*Picking` classes) |
| mo5-05 | Display controls: BCC, contact, window and range controls, flags, properties |
| mo5-06 | Axes, dimensions, reference points, annotations, callouts, measurement |
| mo5-07 | `SfScene` / graphics-view scenes, `LayerView` |
| mo5-08 | Animation, cross-section, fold, slice display, transparency, highlight |

Gate 5: fixed-camera renders of every corpus object match within tolerance.

### Phase 6: pre-processor panels and operation pages (17 packets)

| Packet | Content |
|---|---|
| mo6-01 | Simulation control: `SfPreSimControl`, `100`, `300`, `Adv`, `SimConSelObj`, express |
| mo6-02 | Object panel, general page, properties (`SfPreObject`, `SfPreGeneral*`, `SfPreProperty`) |
| mo6-03 | Geometry panels and tree (`SfPreGeometry`, `300`, `GeoTree`, poly type) |
| mo6-04 | Mesh panels 2D, 3D, tet/brick, coating, user-defined |
| mo6-05 | Material panel, flow-stress options, function editors (`SfFunction*`), material library |
| mo6-06 | Boundary conditions (`SfPreBcc*`) |
| mo6-07 | Inter-object, relations, friction equations, express |
| mo6-08 | Position (express, scheduled) and movement (300, Cog, express, sub, preview, conveyor spring) |
| mo6-09 | Stop and step control, initial state variables, unit conversion |
| mo6-10 | Furnace, heat exchange, pressure, RSE, diffusion and environment windows (the 13 `.ui` files) |
| mo6-11 | Database generation panels, load pattern, free-distortion BCC |
| mo6-12 | Window definitions (`SfWindef*`), die-stress data items |
| mo6-13 | Operation pages: Forming, FormingExpress, MultiBlow, Cycle |
| mo6-14 | Operation pages: HeatTransfer, HeatTransferExpress, HeatTreat, HTFurnace |
| mo6-15 | Operation pages: Cogging, Boolean, CopyMirror (incl. "all" widget), Convert2Dto3D |
| mo6-16 | Operation pages: Diestress, DiestressBM, Distortion, ShapeOptim |
| mo6-17 | Operation pages: Group, Pre, Report, DoeOutput; template and mirroring helpers |

Gate 6: every pre panel and operation page matches oracle C/C′; scripted edits (oracle B, when
approved) give byte-identical files.

### Phase 7: post-processing tasks, graphs, reports (11 packets)

| Packet | Content |
|---|---|
| mo7-01 | Task framework: `SfTaskBase`, factories, items, step/object/region/point selection, output delegates |
| mo7-02 | Summary and load-stroke graphs, min/max |
| mo7-03 | State-variable tasks and widgets (`SfSv*`, `SfStateVar*`, histogram, tool wear/life) |
| mo7-04 | Point tracking task and graphs |
| mo7-05 | Flow net and metal flow tasks |
| mo7-06 | Graph framework (`SfGraph*`, items, tags, axes, step tracer) |
| mo7-07 | Charts adapter over Qt Charts (`SfChart*`, `SfHistoChart`) |
| mo7-08 | Microstructure tasks: JMAK, DRX/MRX/SRX, grain growth, Monte Carlo, TTT |
| mo7-09 | Flow-stress fitting (`SfTaskItemFS*`, `SfFsCurvefit`), UES, Siesta, user routines |
| mo7-10 | Coupon, region, user-variable tracking, custom tasks, Ansys and CA output |
| mo7-11 | Reports: `SfReport*`, `SfTaskReport*`, report generation |

### Phase 8: DOE, optimisation, SQL (5 packets)

| Packet | Content |
|---|---|
| mo8-01 | DOE manager, tables, delegates, variable definitions (`SfMoDoe*`) |
| mo8-02 | DOE sample files (LHS, FF, USR), `MST.TEMP`, study folder copy, `DEF_DOE_PARSER.INI` (`140131530`, `1401321a0`, `1400ba650`–`1400bb760`) |
| mo8-03 | DOE output tasks (`SfTaskDoe*`, min/max, all-node values) |
| mo8-04 | `DefSql*` tables over SQLite (sim info, DB info, global variables) |
| mo8-05 | Shape optimisation driver and its outputs |

### Phase 9: integration and sign-off (4 packets)

| Packet | Content |
|---|---|
| mo9-01 | Full tier 3: every corpus project through oracles A, B, E |
| mo9-02 | Full visual set through oracle C, tolerance report per widget |
| mo9-03 | Drop-in test from DEF_GUI23 and cross-open (each program opens the other's saves) |
| mo9-04 | Ledger close-out: no `todo` rows; every `not-needed` row with its proof |

## 6. Running 17 threads at a time

Total: 115 packets (17 + 17 + 17 + 10 + 9 + 8 + 17 + 11 + 5 + 4) before carving splits or
merges. Each wave below keeps at most 17 threads busy; a lane picks the next ready packet when
its packet merges.

| Wave | Packets | Ready when |
|---|---|---|
| A | mo0-01 … mo0-16 (16), then mo0-17 | start |
| B | mo1-01 … mo1-17 | Gate 0 |
| C | mo2-01 … mo2-17 | mo1-04/05/09/10/11 merged (the rest of Phase 1 can still be in review) |
| D | mo3-01 … mo3-10, mo4-01 … mo4-07 | Gate 1 for Phase 3; mo1-01 and mo1-16 for Phase 4 |
| E | mo4-08, mo4-09, mo5-01 … mo5-08, mo8-01, mo8-02, mo8-04, mo7-01, mo7-06, mo7-07, mo1 leftovers | Gate 2 for DOE files; mo4-01/02 for UI packets |
| F | mo6-01 … mo6-17 | Gate 4 and mo5-01 |
| G | mo7-02 … mo7-05, mo7-08 … mo7-11, mo8-03, mo8-05 | Gate 5 and mo3 kernels |
| H | mo9-01 … mo9-04, plus fixes from gates | everything merged |

Rules that keep parallel threads from colliding (as in the DEF_PRE port):

* One packet owns each class family and each source file; others ask through the coordinator.
* Calls into another packet's functions go through a declared seam (header with the agreed
  signature and a stub) until that packet merges.
* Shared files (`CMakeLists.txt`, the ledger, the `.qrc`) are changed only by the gate thread;
  packets append rows to a per-packet ledger fragment that the gate folds in.
* Each phase ends in a gate thread that merges the packet branches, runs tier 3 and pushes.
* CPU budget on the shared machine: Ghidra and tier 3 runs are queued, at most four heavy jobs at
  once, so 17 threads mostly spend time reading decompiles and writing code.

## 7. Method per packet

1. Take the packet's function list and seams from the carving output.
2. Bulk decompile the closure headless (Ghidra copy per lane, as in `p67`/`p89`), read layouts
   from mo0-03 and wiring from mo0-04.
3. REA for per-function work and call paths; never on licence code.
4. Write the C++ with `@src` markers; for UI, build the widget tree from the compiled `setupUi`
   code, then wire signals exactly as the `connect()` table says.
5. Tier 1, tier 2, then the packet's oracle cases (A, C′ and C always; B once approved); mark rows
   `verified`.

## 8. Risks and open questions

* **Scale.** 39 129 functions is 4.5 times DEF_PRE. The twin and library marking in Phase 0
  decides the real size; an estimate before that would be a guess.
* **Visual capture coverage.** Oracle C (approved) only sees states the capture script can reach;
  rare dialogs fall back to oracle C′ for structure.
* **Qt 4 to Qt 6.** Fonts, high-DPI scaling and style drawing differ. Running at 96 DPI with the
  Windows style narrows it; anything left is measured per widget and listed, not hidden.
* **Hash order.** Qt 4 `QHash`/`QSet` iteration order may reach files (`.moproj` attributes,
  maybe lists). `qt4compat` must reproduce the Qt 4 hash function and bucket order wherever the
  ledger shows a hash container being iterated into output.
* **OpenGL.** Fixed-function output differs between drivers; visual tolerance for 3D views is
  per driver, and the reference machine is fixed.
* **Commercial chart library.** Qt Charts is close to QtCommercialChart 1.x but not identical;
  the adapter reproduces the calls the binary makes, and chart pixels get their own tolerance.
* **Licence fence.** Startup `140001410` shares a function with the licence check; only its
  argv and window-creation parts are reconstructed, and the fence must not change behaviour
  that a user sees, other than the licence check itself.

## 9. Defaults picked here (for Aleksandr's revision)

| Id | Choice | Default in this plan |
|---|---|---|
| D1 | Run the original GUI on copies for pixel-level visual matching (oracle C) | **Approved by Aleksandr, 2026-10-22.** |
| D1b-files | Drive the original GUI on copies to produce output files (oracle B) | Not assumed; static oracle A until then. |
| D1b | Extend the approved leaf-routine harness (p1-01) to `DEF_GUI_MO_64.exe` | Not assumed; asked at Gate 2. |
| D2 | Qt version | Qt 6 LTS with `qt4compat`. Alternative: Qt 5.15 (closer visuals, older). |
| D3 | Third-party code (property browser, libQGLViewer, chart library) | Use upstream components at the identified versions, ported to Qt 6, verified against the binary's use; not reconstructed function by function. |
| D4 | Where the code lives | `mo/` in this private repo, consuming `port/` as a library. |
| D5 | Exe name | `DEF_GUI_MO_64.exe` built under `mo/`, tested side by side; never copied into the DEFORM install. |
| D6 | Reading Qt 4.8 sources | Not used; Qt 4 behaviour is derived from the install's Qt DLLs by static analysis (the earlier "no web search" rule). |

## 10. Reproducing the numbers

```
python -I docs/re/gui_mo_survey.py <copy of DEF_GUI_MO_64.exe>
```

Prints the function totals, RTTI and vtable counts, the subsystem table of section 1.1 and the
resource counts. The Fortran idents in 1.4 are the `# : … compiled with :` strings in the
image.
