# CP209 current-Project resolver source review — PASS

This review accepts the three-file internal extraction for root integration. It preserves the existing composite resolver's behavior and makes its Project-return boundary directly testable. It does not establish original postOpen/B98 correspondence or whole-candidate acceptance.

Scope: the current uncommitted diff against HEAD in `source/apps/moproj2mst/include/m2m/engine/model_manager.hpp`, `source/apps/moproj2mst/src/engine/model_manager.cpp`, and `source/apps/moproj2mst/tests/mdt/test_conversion_session.cpp`. Read subtree AGENTS.md and `analysis/resume209-cpp-action.md`, with takeover provenance. No source files edited, build/test executed, original process launched, or GUI control acquired by this reviewer.

## Semantics and interface

`currentProjectForManager` extracts exactly the previous prefix: obtain `manager.currentModel()`; return null for no live registered current or non-Mo kind; guard absent Project; guard `project->dataDb.get() != model`; return that existing Project identity. The function takes the actual manager reference. It allocates no replacement Project, selects no child, and introduces no B98 condition, callback receiver or public lifecycle call.

`currentSubProjectDbOwner` now calls that prefix and retains the previous remaining sequence unchanged: null Project returns an empty shared pointer; absent active subproject or absent subproject backlink returns empty; a foreign Project backlink throws the existing unsupported error; non-type4 returns the Project DB; type4 without its own DB throws the existing unsupported error; otherwise return the exact ownDb shared pointer. Thus guard order, exceptions, active-subproject selection and type4 ownership remain preserved. The new resolver intentionally succeeds for an existing valid Project even when the active ordinal has no subproject.

The addition is a C++ free-function declaration and definition, with a forward declaration of the existing Project type. No object layout, virtual table, public C ABI declaration, error code, options structure or ABI version changes occur in these three files. CMake links this into `m2m_core`; the DLL's six public functions remain declared with `M2M_API` in `include/moproj2mst.h` (API version 1), while this new helper has no DLL export decoration. This is source-level ABI review, not an independent binary export comparison.

## Test evidence

The two new Testkit cases directly invoke the new helper, rather than inferring coverage solely through its caller. They distinguish empty and registered-but-unselected state from selected Mo state; check exact Project identity with `activeSub = 99` while the composite resolver returns empty; select a Base child to check null projection; restore the original selected Mo child; and check unsupported guards through both the helper and composite for foreign dataDb and missing moProject. The latter explicitly confirms no lazy replacement allocation. Existing adjacent coverage still exercises type4 missing-owner rejection, actual type4 ownDb identity, restoration to the normal Project DB, and storage-release missing-Project guard.

The test file has a `testkit::run_all()` entry point at line 1138, uses supported CHECK assertions that remain active in Release, and already resolves the model_manager include through its target's m2m_core public include directory. No new quoted include or filesystem probe was introduced. CMake discovers this file as `m2m.mdt.conversion_session` and links the real core rather than standalone doubles.

Read focused configure/build/test logs at `build/cp209-current-project/focused-{0,1,2}.log`. Configuration completed; the focused build linked `m2m_mdt_conversion_session.exe`; focused CTest passed both `m2m.mdt.conversion_session` and `m2m.mdt.model_manager` (2/2, 0.10 seconds). `Testing/Temporary/LastTest.log` explicitly names both new resolver cases as passing, reports 0/38 conversion-session cases failed and 0/7 model-manager cases failed. These logs substantiate execution of the added tests. They are focused results, not the root-owned complete launcher or a source-bound candidate freeze.

## Reviewed working-file hashes (SHA-256)

- model_manager.hpp: `d411553257b6023382049ef0c192b5ccdfdd2c3ce12a46935da48662f121b7f1`
- model_manager.cpp: `f09aadddac11e5243f56fc9d3a1cd71fcbcb395958092a4e3308c07acc596d4`
- test_conversion_session.cpp: `a06de6ae89b0c12c703c2b808acea0a16430a8f9e7e3b9be649f2667aa966e0b`

No actionable defect found within this bounded diff. Root still owns full integration checks and source/executable binding and freeze. Native RTTI, callback wrapper +0x50 ownership, lazy constructor recovery, and the original zero-return cause remain outside this extraction's established behavior.
