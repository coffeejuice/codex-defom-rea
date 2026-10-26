# Plan: faithful C++ port of DEF_PRE_64 batch mode

Target: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe` (6 775 296 bytes, build 2014-09-29;
`2D\` and `3D\` hold copies of the same size). One binary serves 2D and 3D ("DEFORM-23").

Goal: a modern C++ program that behaves like DEF_PRE_64 in every non-interactive use, recovered
from static analysis of the binary, with byte-identical `.DB` and KEY output on every test input.

Rules carried from the project: layouts and algorithms come from the code; example DBs and KEY
files are used only to test what the code shows, always on copies; DEFORM binaries are never
modified; licence code (LManager, Sentinel, `FTN_license.f`, SDSPM) is noted and skipped.

Numbers below come from `docs/re/batch_scope.py` over the Ghidra function list
(`deform-rea-work/out/pre64_funcs.tsv`); the per-function ledger is
`docs/re/pre64_batch_ledger.tsv`.

## 1. What is in the binary

### Size

| Part | Functions | Code |
|---|---:|---:|
| Whole image (Ghidra) | 15 876 | 5.1 MB |
| Licence, Sentinel, sockets, XML config (0x140090000–0x14012ffff) | 2 771 | 0.6 MB |
| MSVC CRT/STL, Fortran runtime, process monitor (0x140470000–0x1404bffff) | 1 698 | 0.3 MB |
| Import thunks and SEH unwind funclets (0x1404e8000–) | 2 803 | 0.1 MB |
| **Application code** | **8 604** | **4.0 MB** |

The thunk/funclet range also holds 62 C++ static initialisers, which are application code; the
ledger counts them as such (8 666 application functions in all).

### Source modules

PathScale/Open64 Fortran objects carry an ident string ("`# : <path> compiled with : -O2 ...`").
50 of them are in the image (path separators were eaten by the compiler; reconstructed here):

| Group | Files |
|---|---|
| `pre2d3d` (DB core) | `datarm23.f`, `DBOperations23.f`, `func_sort.f` |
| `defdbe\for_2d3dmod` | `diepro.f`, `posdie2.f`, `fnc_temporary_2d.f`, `genbcc2.f`, `DBOperations3.f` |
| `defdbe\from_pre2d` | `diepro_cross_section.f`, `utility2.f`, `utility2_cross_section.f`, `keyio2_cross_section.f` |
| `defdbe\from_pre3d` | `func_gui.f`, `func_temporary_3d.f` |
| `defdbe\dds` | `dds_c_cftn_gate.f`, `dds_c_objdat.f`, `dds_c_utility.f` |
| `defpre` | `FOR3\position.f`, `FOR3\findaxis.f`, `FOR2\Volume.f`, `FOR2\dspmdf2_cross_section.f`, `FOR2\FTN_DieGeo_cross_section.f`, `FOR2\FTN_IgesInterface.f`, `FOR2\FTN_IgesCore.f`, `for_2d3dmod\dspmdf2_2d.f`, `for_2d3dmod\dspmdf2_3d.f`, `from_pre3d\posdie2.f` |
| `deffor_common` | `DefGraf.f`, `FOR3\FTN_DefGraf3D.f`, `LCPlot.f`, `fortran_call_c.f` |
| `defutl` | `FTN_forutl1.f`, `FTN_forutl2_Absoft.f`, `FTN_forutl3.f`, `FTN_forutl_nt.f`, `FTN_math.f`, `FTN_math3f1.f`, `FTN_math3f2.f`, `FOR2\FTN_math2f1.f`, `FOR2\st.f`, `defMath32.f`, `FTN_geoutl.f`, `FTN_lib_pc.f`, `FTN_license.f` (skip) |
| solver pieces | `fem3d\eq.f`, `fem3d\def_mpi_null_gui.f`, `newamg3d\ne.f`, `newamg3d\neweq.f`, `newamg3d\mpi_null.f` |
| unclear | one ident is garbled (`...map_c_i...version.f`) |

C and C++ objects have no ident strings. RTTI gives about 100 classes; the application ones fall in
these families:

* keyword data model: `LLOBJ_DATA`, `FRICTION_DATA`, `OPSTOP_DATA`/`OpStopLinkedList`,
  `DYNARY_DATA`/`DynAryLinkedList`, `LLRANGEDEF_DATA`, `LLWINDEF_DATA`, `StateVar*` (node and
  element state variables), `SdctlGList`, `SdctlPtrCollection`, `SfLinkedList`;
* DB step layer: `_DB_STEP_DATA`, `_ACTION_DB_STEP`, `_DB_WRITE_VAR`, `CGlobalDataProcessor`;
* geometry and meshing: `_OBJ_GEO2D`, `_OBJ_GEO3D`, `OBJ_MESH2D`, `GEO2D_AND_MESH2D`,
  `OBJ_GEO2D_DIGITIZED`, `_DEF_GEOPRIM`, `_GEOPRIM_2D_*_CS`, `C2d3dGeometry`, `C3dCrossSection`,
  `_DEF_AMGGEO`, `_DEF_VERTEX/EDGE/LOOP/FACE`, `IVertex/IEdge/ILoop/IFace`, `_MeshGenBase`;
* import: `Uic2CObjectIgesFile`, `Uic2CObjectDxfFile`, `_DEF_STL`, `_DEF_NASTRAN`;
* Fortran bridge: `DdsCObject`, `DdsCCommonOBJDAT`, `DdsCGatewayOBJDAT`, `DdsCCommonCFTN_GATE`;
* skip: `CLManagerClient*`, `CDEFORMLMObject`, `CDFMBaseSocket`, `CJobInformationFile`,
  `CsdspmSimulationProcessMonitorDataSource`, `SfLicenseLogData`.

### Address map of application code

| Range | Area | Functions | In batch closure |
|---|---|---:|---:|
| 0x140001000–0x14001ffff | text pre-processor UI: `main`, menus, `-m` CLI (C++) | 331 | 222 |
| 0x140020000–0x14008ffff | Fortran pre-processing kernels: positioning, interference, polygon search, mesh checks | 345 | 133 |
| 0x140130000–0x14030ffff | C++ keyword data model, DB step layer, geometry, import, KEY formatting | 4 726 | 3 233 |
| 0x140310000–0x14037ffff | Fortran contact/BCC generation, inter-material data, command handler | 506 | 297 |
| 0x140380000–0x1403affff | keyword group parsers, DDS gateway | 352 | 61 |
| 0x1403b0000–0x1403bffff | `datarm23.f` / `DBOperations23.f` | 64 | 45 |
| 0x1403c0000–0x14046ffff | units, point locator, DXF/IGES core, DDS objdat, low-level DB I/O | 2 010 | 1 156 |
| 0x1404c0000–0x1404e7fff | Fortran FEM/geometry (`eq.f`, `MDSP2S`, `PNTEDG`) | 270 | 46 |
| 0x140501000–0x140504fff | C++ static initialisers (option label tables, iostreams, globals), among unwind funclets | 62 of 265 | 62 |

## 2. Batch-mode entry and control flow

`entry` → `__tmainCRTStartup` → **`main` = `FUN_14001c9f0`**:

1. `FUN_140224d80("TEXT_PRE", ..., "DEFORM-23")` and `FUN_1401b8e10` — licence/session setup (skip;
   the port starts no licence check).
2. **`-m` utilities**: `FUN_14001b7b0` parses `-h`, `-m {header|stepList|lastStepNo|simStepList}`,
   `-d <db>`, `-o <file>` (strings `summary`, `reclist`, `diff` are parsed but have no handler);
   `FUN_14001c500` runs mode 1–4 (`FUN_14001bc80`, `FUN_14001be50`, `FUN_14001bf40`,
   `FUN_14001c100`) and exits. This is what the built-in help calls "Batch mode".
3. `FUN_14001c770` builds the keyword model (object at `this+400`, a C++ class with a vtable).
4. Banner, then **`FUN_140009f20`**:
   * if `DEF_MULTI.INI` exists: read it as a keyword file (`FUN_1401b5f00`), print
     "Multiple operation keyword processed", exit 0. DEF_SIMULATION, DEF_DOE and auto_test write
     this file and run DEF_PRE to build multi-operation databases: the main production batch path.
   * else load `DEF_PRE3.INI` (sets 3D) or `DEF_PRE.INI` (2D) the same way, report
     INITIAL/CONTINUING/REMESHING, then `FUN_140009e90`.
5. **Main menu loop `FUN_14001ad90`** reading stdin (`FUN_140009c50` line reader,
   `FUN_140007d30` selection prompt): 1 problem ID, 2 file input (`FUN_140014ba0`: KEY file,
   DB via `FUN_14000b170`, IGES/ANSYS/STL/PATRAN/IDEAS/UGII via external `DEF_CVT3`),
   3–6 interactive edit menus (`FUN_140017540`, `FUN_1400157c0`, `FUN_14001a050`,
   `FUN_140019c20`), 7 DB generation (`FUN_14000a370` → `FUN_1401b9940`), 8 KEY save
   (`FUN_14000ac60` → `FUN_1401c5810`), 9 2D→3D (`FUN_14000afd0`), E exit. Scripted stdin is a
   second batch path: the GUI runs `DEF_PRE.EXE < DB_CONVERT.INP > DB_CONVERT.OUT`, and our KEY
   export test feeds `\n2\n1\nCMD.KEY\n\nE\nE\nY\n`.

**KEY interpreter** `FUN_1401b5f00` (open file) → `FUN_1401b6520` (loop):

* `fscanf("%39s")`; `*` lines are comments, and the first comment naming `DEFORM 3D/2D/F3/F2`,
  `DEFORM-3D`, `DEFORM-MACH2` … switches the model's dimension;
* `FUN_1402f76c0` looks the name up in the 304-entry registry (`docs/re/keyword_registry.tsv`);
  unknown names print "WARNING: Keyword %s not found. Skip" (except `SRFNRG`);
* dispatch by group: 1 `FUN_14039aa90`, 2 `FUN_14038b8a0`, 3 `FUN_140388fb0`,
  4 `FUN_140384a50`, 5 `FUN_140383120`, **6 command `FUN_140378e00`**, 7 `FUN_14037e530`;
  return 2 = "Reading Error", 3 = "Data Error".

**Command keywords** (`FUN_140378e00`, one switch case per keyword). Functions reachable by direct
calls from each case, with the reader itself cut out:

| Command | Funcs | What it does / external program |
|---|---:|---|
| PROBID, RUNTYP, TLOC, DSTROKE, KFREAD, EXECMD, INICTC | 1–9 | set fields; KFREAD re-enters the reader; EXECMD runs a shell command |
| ARMCTL, MORP, TCYCLE | 61–113 | remesh control; `DEF_MORP2.EXE`; thermal cycle |
| SVSCAL, DOESCL, GENBCC | 196–233 | scaling, boundary-condition generation |
| GENAXS, CPYOBJ, IMPEQP | 454–654 | axis generation, object copy, equipment import |
| WRTOBJ, GENCTC, GENSUB, GENINT, IMPLUB | 830–955 | object output, contact generation, sub-object, interface, lubricant import |
| BRDEXT, DEFINT, DEFFRZ | 1 196–1 355 | border extraction; `DEF_INT`; `DEF_FRZ` |
| DBREAD, GFREAD | 1 452–1 561 | read step from DB; read geometry file |
| GENDB, KFWRIT, OBJPOS, DEFAMG | 1 804–1 989 | write DB; write KEY; positioning; `DEF_AMG`/`DEF_AMG3_GUI` mesher |
| REMESH, CNVT3D | 2 360–3 275 | remeshing (`DEF_REM3_GUI`, `DEF_AMG`); 2D→3D conversion |

External programs (`DEF_AMG`, `DEF_INT`, `DEF_FRZ`, `DEF_CVT3`, `DEF_IGS`, `FIXGEO`, `DEF_MORP2`,
`DEF_LOCREM3`, `COATINGMESH`) are separate executables. The port spawns the same programs with the
same input files and reads their output back; porting them is out of scope.

## 3. Scope of the batch port

**5 255 functions, 2.5 MB** (of 8 666 application functions, counting the 62 static
initialisers). By how they are reached from the batch roots (`via` column of the ledger):

| Reached by | Functions | What |
|---|---:|---|
| direct calls | 4 915 | call instructions from the phase entry points, the static initialisers and the registry handlers |
| function pointers | 10 | callbacks: `qsort` comparators, `eh vector constructor iterator` constructors, Fortran procedure arguments |
| pointer tables | 23 | DXF entity name/handler table at `0x14062bdb0` (SECTION, LINE, POLYLINE, ...) and its callees |
| vtable slots | 307 | virtual methods of 62 live classes (keyword model, `StateVar*`, geometry, `SdctlGList`, iostreams) |

The direct-call union is 4 915 rather than the earlier 4 785 because it now also has the reader
and dispatcher functions themselves (they were cut out as stop points), the 62 static
initialisers and the 60 registry handlers that nothing calls directly.

How indirect targets were found (`docs/re/ghidra/IndirectRefs.java`, `docs/re/batch_scope.py`):

* **vtables**: every run of function pointers in data that Ghidra's RTTI analysis names
  `vftable` (101 classes; 10 licence, socket, job-file and process-monitor classes skipped). A
  vtable is live from the phase whose functions reference it (constructors, destructors); a
  phase's `call [reg+8k]` sites then reach slot k of every live vtable. This matches slots by
  offset only, so it over-approximates.
* **keyword registry** (`0x140544400`, 304 records of 0x30 bytes): handler column 2 (155 KEY
  write handlers, `%-6s %20.10E`) are roots of phase 2; column 3 (50 handlers that return table
  axis labels such as "Coefficient (Y)" or "Speed") are roots of phase 10. Column 1 is the
  default handler everywhere.
* **C++ static initialisers**: the `_initterm` tables at `0x1405056c8`–`0x1405058e0` (called by
  `_cinit`/`doexit`) point to 62 initialisers placed after the CRT; they build global option
  label tables, iostreams and step-layer name strings and run in every mode, so they are roots
  of phase 1.
* **function pointers and tables**: code that takes a function's address (`lea`) and data
  references to pointer tables or record arrays holding function pointers.

Still unresolved or outside the method:

* Registry column 3 handlers and the registry accessors `FUN_1402f7790`, `FUN_1402f77e0`,
  `FUN_1402f7830`, `FUN_1402f7880`, `FUN_1402f79c0` (calls column 2), `FUN_1402f7a70` (calls
  column 3) have no caller and no reference. The column 3 handlers are counted in phase 10 to be
  safe; they are likely GUI leftovers and can become `not-needed` if no batch output uses the
  labels.
* Vtable slot matching cannot tell classes apart, so a few virtual methods may be counted that
  the batch paths never call; they show as `via=vtable` and are checked when their phase is
  ported.
* `call reg` sites (78 functions) are all outside the batch scope; none needs resolving.
* 1 510 application functions have no caller and no reference at all (dead code kept by the
  linker, or code Ghidra did not recognise); they are outside the scope.
* 3 411 application functions are outside the scope in total.

By size the scope is mostly small functions (constructors, destructors, accessors, STL
instantiations) that collapse into ordinary C++; the largest is the group-2 parser
`FUN_14038b8a0` (50 KB).

### Already done (branch `claude/project-thread-5xiymd`)

`deformdb` (C++20) covers the `datarm23.f` container: header, step directory, step header, region
tables, RLE codec, MAR/A arrays, dynamic arrays, typed access to about 45 node/element keywords,
a step `Writer`, `purge` and `merge`. Verified against DEF_PRE's own KEY export on the two test
databases, and record-layout identical when rewriting them (only ISTART and two header-only fields
differ). In the ledger this is most of area `datarm23_dbops` and the low-level I/O part of
`utils_io_units_iges`; it becomes the storage layer of phases 1, 2 and 4.

## 4. Port architecture

The binary is one in-memory keyword model with three codecs around it, and the port keeps that
shape so each function has an obvious home:

```
             +--------------- commands (OBJPOS, GENCTC, REMESH, ...) ----------+
             |                                                                 |
KEY text --> KEY reader --+                                    +--> KEY writer --> KEY text
                          +--> model (LLOBJ_DATA, FRICTION_... )+
.DB file --> DBREAD ------+                                    +--> GENDB -----> .DB file
             (deformdb)                                             (deformdb Writer)
```

Libraries: `deformdb` (container, existing) → `defmodel` (keyword classes, registry, units) →
`defkey` (KEY codec) and `defdbio` (model ↔ DB arrays) → `defcmd` (command keywords, Fortran
kernels ported to C++) → `def_pre` executable (same argv, stdin protocol, INI and DEF_MULTI.INI
handling, same stdout text).

Fidelity rules:

* Each ported function records its source address in a comment, and the ledger tracks its status
  (`todo` / `ported` / `verified` / `not-needed`).
* Floating point: keep the original operation order; build with `/fp:precise`, no FMA
  contraction, SSE2 only (the Fortran was built `-msse2 -mno-sse3`). `printf` formatting goes
  through the same MSVC CRT, so `%20.10E` output matches when the double matches.
* Dates: the DB header stores simulation dates. The port takes its clock from one function that
  tests can pin; comparisons against cached oracle output mask the documented date words.

## 5. Phases

Function counts are "reach" (closure of the phase's entry points) and "new" (not already counted
in an earlier phase).

| Phase | Content | Reach | New | of which indirect |
|---|---|---:|---:|---:|
| 0 | Oracle harness, corpus, ledger completion | – | – | – |
| 1 | `-m` CLI + model skeleton (constructors, registry), static initialisers | 1 439 | 1 439 | 219 |
| 2 | DBREAD + KEY writer (KFWRIT, menu 8), registry write handlers | 2 335 | 992 | 5 |
| 3 | KEY reader, all keyword groups | 2 065 | 329 | 72 |
| 4 | GENDB / DB generation (menu 7) | 2 328 | 475 | 0 |
| 5 | Menu shell, INI files, DEF_MULTI.INI, stdin protocol | 2 426 | 19 | 0 |
| 6 | Object commands: OBJPOS, CPYOBJ, SVSCAL, DOESCL, WRTOBJ, GENAXS, GENBCC, GENSUB, TCYCLE | 2 843 | 738 | 1 |
| 7 | Contact: INICTC, GENCTC | 1 725 | 45 | 0 |
| 8 | External-tool commands: DEFAMG, REMESH, BRDEXT, DEFINT, DEFFRZ, MORP, GFREAD, GENINT, IMPLUB, IMPEQP | 3 109 | 587 | 40 |
| 9 | CNVT3D / menu 9, file-input menu (DEF_CVT3 imports) | 3 634 | 506 | 3 |
| 10 | Interactive edit menus 3–6 (only for scripted stdin), registry label handlers | 2 701 | 125 | 0 |

"Indirect" counts the new functions that direct calls from the batch roots do not reach. Phase 1
grows most because the `-m` path already constructs and destroys the whole keyword model, so the
virtual methods of its classes are counted there.

### Phase 0: harness, corpus, ledger

* **Oracle runner**: runs the original DEF_PRE_64 in a scratch copy of the input (stdin script,
  DEF_MULTI.INI, or `-m`), collects every file it writes plus stdout. Outputs are cached by
  hash of (input files, script, DEF_PRE build), so the original runs once per test case, ever.
* **Comparator**: byte compare for KEY; for DB, record-by-record with a mask list of documented
  volatile words (dates, ISTART), reporting the first differing step/region/keyword through
  `deformdb`. Stdout transcripts compared after masking dates and paths.
* **Corpus** (copies only): the two test DBs; 33 example DBs and about 1 080 example KEY files
  (plus about 4 300 material KEY files) shipped under the DEFORM install, and Aleksandr's projects
  under `C:\Users\alext\Projects\DEFORM_examples`. Approved by Aleksandr on 2026-10-21: all of them
  are copied into the test corpus first and used only for testing.
* **Ledger** (done): vtables, registry handler columns, static initialisers, callbacks and
  pointer tables added to `pre64_batch_ledger.tsv`; see section 3.

Exit: one command runs oracle vs. stub port on the corpus and prints a pass/fail table.

### Phase 1: `-m` CLI and model skeleton

Recover `FUN_14001b7b0`, `FUN_14001c500` and the four mode functions on top of `deformdb`; recover
the model constructors/destructors (`FUN_1401c8950`, `FUN_1401cb2c0`, `FUN_1401dbfe0`) and the
keyword registry as data.
Verify: `def_pre -m header|stepList|lastStepNo|simStepList` output files identical on every corpus
DB, plus the help and error texts.

### Phase 2: DBREAD and KEY writer

Recover `DBREAD` (`FUN_1401cc2a0` and the `ReadFromDB` methods of each model class) and the KEY
writer (`FUN_1401c5810` and the per-keyword write handlers, handler column 2 of the registry).
This turns `deformdb`'s typed view into model objects.
Verify: `DBREAD n` + `KFWRIT` KEY identical to the original for every step of every corpus DB.
This is the strongest test set available today: it exercises every keyword the corpus DBs carry.

### Phase 3: KEY reader

Recover the seven group parsers and the per-keyword read handlers (registry column 1),
header/dimension detection, error and warning texts.
Verify: `KFREAD a.KEY` + `KFWRIT b.KEY` identical to the original on every corpus KEY file;
`DBREAD` → KEY → `KFREAD` → KEY round trip identical.

### Phase 4: GENDB

Recover model → DB arrays (`WriteToDBArrays` methods, `FUN_1401b9940`, old/new DB options) on
top of `deformdb::Writer`.
Verify: from each corpus KEY and each DB step, generate a new DB with both programs and compare
bytes (masked dates); also append to an existing DB.

### Phase 5: menu shell and batch drivers

Main, INI handling, DEF_MULTI.INI, menu loop and prompts, the 20-trial and 100-iteration limits,
exit codes.
Verify: stdout transcript and output files identical for scripted stdin sessions (the
DB_CONVERT and KEY-export scripts, menu 7 and 8 flows) and for DEF_MULTI.INI files written by
DEF_SIMULATION for the multi-operation examples.

### Phase 6: object commands

OBJPOS (move, rotate, mirror, interference, drop and reference positioning in 2D and 3D), CPYOBJ,
SVSCAL, DOESCL, WRTOBJ, GENAXS, GENBCC, GENSUB, TCYCLE. This is where the Fortran geometry
kernels (`position.f`, `posdie2.f`, `findaxis.f`, `genbcc2.f`) come in, and where floating-point
fidelity is tested hardest.
Verify: command scripts on corpus models, KEY and DB output identical; one script per command and
option, built from the switch cases in `FUN_140378e00`.

### Phase 7: contact generation

INICTC/GENCTC and the Fortran contact code (`NODE CONTACT`, BCC changes, `CNTINF`).
Verify: GENCTC on every corpus object pair, KEY output (contact node lists) identical.

### Phase 8: external-tool commands

Port the parts that write the external program's input, run it, and read its output back; the
external executables are called unchanged.
Verify: same input files handed to the external tool (compared directly), same model after the
read-back, same KEY and DB.

### Phase 9: 2D→3D and imports

CNVT3D and menu 9, DB-input menu, IGES/ANSYS/STL/PATRAN/IDEAS/UGII through `DEF_CVT3`, DXF/IGES
readers.
Verify: converted KEY files identical on the 2D corpus DBs; imports on example geometry files.

### Phase 10: interactive edit menus

Menus 3–6 as text forms, only for scripted stdin compatibility.
Verify: scripted edit sessions, transcript and KEY identical.

## 6. Testing strategy that does not slow down each step

Aleksandr asked whether the growing test suite will slow every next subtask. It will not if the
suite is layered and the expensive part is paid once:

1. **The original runs once per case.** Oracle output is cached by input hash; a subtask never
   re-runs DEF_PRE for cases already in the cache, and the cache is shared by all threads.
2. **Three tiers with fixed budgets.**
   * Tier 1, per function/module: unit tests on decoded values (golden vectors taken from oracle
     outputs), seconds. A subtask runs only the tier-1 tests labelled with the modules it touched
     (`ctest -L <module>`).
   * Tier 2, smoke: a fixed, small set (about 30 cases chosen to cover every keyword group and
     command at least once), under one minute, run before every commit. When a new case would
     not raise keyword or command coverage it goes to tier 3, never to tier 2, so tier 2 does not
     grow with the project.
   * Tier 3, full corpus: every DB step and KEY file, run in parallel (`ctest -j`, one process per
     case) at phase gates and nightly, not inside subtasks. Large DBs (the 5 GB file) only here.
3. **Coverage-driven selection.** Each case records which keywords, commands and ledger functions
   it exercises; the tier-2 set is the smallest set that covers them, recomputed at phase gates.
4. **Cheap comparisons.** Comparators hash outputs and only diff on mismatch; DB comparison stops
   at the first differing record and names the step, region and keyword.
5. **Regression on change, not on time.** Tier 3 failures are bisected to the commit and module
   that caused them, so a slow full run never blocks unrelated work.

## 7. Method per phase (REA + Ghidra)

1. Headless Ghidra: decompile the phase's closure in bulk (`Decomp.java @listfile`), dump tables,
   update the ledger.
2. REA (`mcp__rea__*`): interactive recovery of each non-trivial function and call chain, the
   default tool for per-function work.
3. Write the C++ with the source address in a comment; unit-test it against golden vectors.
4. Run tier 1 + tier 2, then tier 3 at the phase gate; mark ledger rows `verified`.
5. A mismatch is traced back to the code, never patched from the expected output.

## 8. Risks and open questions

* **Floating-point identity in geometry code** (phases 6–9): Fortran `-O2` may have reordered
  expressions and may call its own math library rather than the MSVC CRT. Each kernel is
  checked against the oracle in isolation; where the compiled code reassociates, the port
  reproduces the compiled order rather than the evident source order.
* **Indirect calls**: scope grows when Phase 0 adds vtable and table targets (estimate above).
* **Corpus breadth**: even with the shipped examples and Aleksandr's projects, some keywords or
  command options may have no test input; Phase 0's coverage report lists them.
* **External tools**: phase 8 depends on `DEF_AMG`, `DEF_INT`, `DEF_FRZ` running headless in a
  scratch directory; any of them needing a licence or GUI is tested only up to its input files.
* **Optional runtime tracing**: observing the original under a debugger (no modification) would
  speed up locating floating-point mismatches. Not used without Aleksandr's explicit approval.

## 9. Reproducing the numbers

```
GPROJ=<your project copy> scripts/run.sh DEF_PRE_64.exe IndirectRefs.java <work>/out/ledger
python -I docs/re/batch_scope.py            # writes docs/re/pre64_batch_ledger.tsv
python -I docs/re/batch_scope.py --direct   # direct calls only, for comparison
```

Needs, in `deform-rea-work/out`: `pre64_funcs.tsv` (FuncList.java), `bm_cmd.c` (Decomp.java on
`140378e00`) and `ledger/{ptrtables,addrtaken,dataitems}.tsv` (`docs/re/ghidra/IndirectRefs.java`,
copied into `deform-rea-work/scripts`; about 10 s headless). Other decompiles used for this plan
are `bm_main.c` and `bm_kwread.c` in the same folder.

Ledger columns: `address`, `size`, `area` (address map above), `first_phase`, `via` (`direct`,
`fnptr`, `table`, `vtable`: the cheapest edge set that reaches the function), `status`.
