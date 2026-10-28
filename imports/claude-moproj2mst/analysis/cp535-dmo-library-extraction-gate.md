# CP535 upstream library extraction gate

Status: **no supported unchanged, database-reader-free source/link closure for a
Cogging plus Heat Transfer project session exists in the current converter**.
This is a bounded source-level finding, not a negative link experiment or a
conclusion that such a target can never be built. The reviewed CP534 freeze is
`build/cp534-verified/source.zip`, SHA-256
`87dad0c43b08f6cdb32e198cf691a23b6e5b000cc6af94a491ec76edeec79afa`.
The converter recovery queue and public guards remain unchanged. No DMO
source or build was touched.

The requested downstream boundary is a standalone C++ move-only, one-project
session with native Open, editable operation/object/material/contact structure,
Save/Save As, Cogging MDT and explicit MST generation for arbitrary ordered 3D
Cogging and Heat Transfer operations. It must use the existing recovered
implementation unchanged and must not import a second native DB reader or
protocol parser. A format-only MDT serializer is insufficient for this scope.

## Current source and call closure

`source/apps/moproj2mst/CMakeLists.txt:69-103` unconditionally configures
`deformdb` and `port/{defmodel,defkey,defdbio,defcmd,def_pre}` when their targets
are absent. It builds `m2m_core` from **every** `src/*.cpp` below the converter
except DLL/CLI sources and publicly links `defrt`, `defcmd` and `def_pre_core`.
`moproj2mst.dll` links that entire target. There is no selective extraction
target, link map or archive-member proof of a narrower closure in the retained
source snapshot.

The existing one-shot coordinator in `src/core/convert.cpp:157-228,257-390`
constructs a `ConversionSession`, initializes the Project/registry/DOE owners,
opens the project, activates SUB 0, and then calls
`engine::openActiveOperation` before Save. Native Save calls
`engine::saveCurrent`, the native MOPROJ writer, ordered operation-property
writers, Cogging MDT and `mst::writeMainMst`. This coordinator destroys its
Session on return; `include/m2m/convert.hpp` and `src/dll/abi.cpp` offer
path-based conversion, not an editable session interface.

The callable C++ pieces are already present: `ConversionSession` in
`include/m2m/engine/conversion_session.hpp` and
`src/engine/conversion_session.cpp`; `initProject`/`makeRegistry` in
`src/model/project.cpp` and `src/model/registry_rescue.cpp`; `openProject`
in `src/moproj/read.cpp`; `activateSubProject` in `src/model/subproject.cpp`;
`openActiveOperation` in `src/engine/loader.cpp`; `saveCurrent` in
`src/engine/save_project.cpp`; the native project writer in
`src/moproj/write.cpp`; Cogging MDT in `src/datafiles/mdt.cpp` and
`mdt_write.cpp`; MST root/dispatch in `src/mst/master.cpp` and the
`src/ops` operation classes. These are **component locations**, not a verified
standalone source manifest. An adapter would need to reproduce the real
owner initialization and FileEffects/order from `convert.cpp`; calling the
project writer alone bypasses Save-current and later effects.

The DB dependency is on the actual Open path. `src/engine/loader.cpp` directly
includes `defdbio/db_query.hpp`, `dbread/dbread.hpp`,
`step/db_info_read.hpp`, `gendb/checks_p4_04.hpp` and `objcheck/objcheck.hpp`.
`src/engine/cogging_load.cpp` calls DB/engine model helpers, including
`def::dbio::gendb::node_coord_range`; Heat Transfer load is in
`src/engine/loader_l2.cpp` and shares broader engine/model slots. Further
Save/step/viewer translation units also include `defdbio`. Merely linking
`readMdtFile`/`writeMdtBytes` or pruning unrelated operations cannot establish
unchanged native Open→Save behavior for the requested families.

## Additional behavioral gates

`src/datafiles/mdt_write.cpp:432-439` emits `NoReheats` and refuses counts
of two or more because the recovered native second loop advances the array
pointer incorrectly. The current implementation supports zero or one reheat;
it cannot be advertised as unrestricted Cogging MDT output or bypassed in a
downstream adapter. `saveActiveCogging` in that file invokes the active
operation Open/Save engine path before the MDT writer.

`ConversionSession` owns a noncopyable `Project`, but no reusable public
Open/edit/Save/Save As/Close façade exists. `saveProjectFileNative` writes via
`Project::path`; rebinding that path for Save As is only an inference and does
not establish native rebase, overwrite or side-file behavior. The existing
Close release requires cleared diagram binding and does not reconstruct the
full native Close callbacks. Current source guards for editor300, active DOE,
graph/backend, operation/property and failure paths remain in the converter
queue. The 389-test CP534 launcher and 93 historical SAME comparisons are
baseline evidence, not fresh native proof of this new session surface.

There is meaningful **retained native output evidence** for narrower writers.
`tests/mdt/test_mdt_write.cpp::corpus_sp2_mdts_reproduced` compares 27 complete
MDT side files against corpus bytes or GUI re-save oracles, and
`tests/mdt/test_cogging_save.cpp` compares a whole active-Cogging MDT against
an oracle-B re-save. `source/docs/re/moproj2mst/s/folder_sweep.txt` records
zero-difference GUI-save oracle-B folder comparisons for Cogging cases
`105_B_COG7`/`115_B_COG7P1` and Heat Transfer-containing cases including
`103_B_HT3F`/`104_B_HT2F`. The B sweep also has refusals and differences;
these selected historical matches do not establish arbitrary operation order,
new session editing, Save As, DB-free linkage or fresh acceptance for the
requested DLL. An A corpus match is distinct from a GUI re-save match.

## Admissible unblock condition

Build a **new upstream adapter/target beside the unchanged recovered source**
only after a source and link experiment establishes an exact, DB-reader-free
manifest for the actual Cogging/Heat Transfer Open→Save/MST/MDT call graph,
including any SDK-supplied DB data seam. Preserve the recovered engine and
writer bodies and their guards. Prove the DLL's dependency/import closure
excludes `defdbio`, `deformdb`, a second protocol parser, DMO, Qt Widgets,
VTK and `mst.dll`; verify its move-only session and one-project ownership.
Then compare native project, MDT and MST bytes, directory effects and failure
ordering on bounded fresh original Cogging and Heat Transfer cases, including
the multiple-reheat boundary. Until that evidence exists, the only plausible
narrow extraction is the Cogging MDT **format** reader/writer, which does not
satisfy the requested operation/session library.
