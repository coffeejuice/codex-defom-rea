# CP149: wired DoeOutput directory/model Save prefix

The actual `engine::saveCurrentOperation` DoeOutput branch now executes the existing immediate Control/Regions directory attempt and, only after a successful attempt, the existing actual operation-model getter. It remains guarded before Regions enumeration. A failed attempt is retained and stops before the unresolved warning-modal return. This is a wired partial Save effect, not successful active DOE Open/Save or a cleanup/geometry/task/document implementation.

Root assigned this cut from `resume149-doe-output-wired-next.md`. Only the two assigned source/test files and this receipt were written. Source was frozen and released to root before finishing this report. No compiler, executable/CTest, full build, GUI, original runtime or REA operation was performed by this lane.

## Frozen files and registration

Paths below are relative to `source/apps/moproj2mst`.

| File | SHA-256 |
| --- | --- |
| src/engine/save_current.cpp | bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129 |
| tests/doe/test_output_save_prefix.cpp | 68cf795c5ff369dfe2835bf458faa1d8205a862accc3418e1923016adf521c69 |

Existing `CMakeLists.txt:135` discovers `tests/doe/test_*.cpp` with CONFIGURE_DEPENDS. Lines137-140 derive the target name, the normal branch links m2m_core and Testkit, and line186 registers the corresponding CTest name. Expected target is `m2m_doe_output_save_prefix`; CTest is `m2m.doe.output_save_prefix`. No CMake or shared header edit was needed.

The CP142 `output_regions_mkpath_prefix.hpp/.cpp` helper remains unchanged. No model/Session/Study ownership declaration, loader, geometry, SPRING2, emitter, activation, XML/PAR/INI writer or public ABI file was changed. A focused old-test search found no exact assertion of the replaced DoeOutput Save error text; any broader regression remains root validation's responsibility.

## Actual caller and effect ordering

The branch remains under the original class/current-operation/project dispatch preconditions. It additionally requires the supplied Project to equal the operation's actual project and a present active SUB/owned Study before dereferencing that owner. The existing helper checks active Type2 identity, Study alias/backlink/name, absolute derived path and immediate native-directory profile before filesystem work.

At `save_current.cpp:764`, the branch passes the supplied step2 FileEffects to the existing immediate mkdir helper. That helper derives Control/Regions from the actual owned Study, calls `nativeMkpathAbsolute`, then records the returned physical Boolean. It does not schedule a deferred replay. If `WarningModalRequired` is returned, line766 refuses explicitly at `Control/Regions warning modal return remains unsupported`. No model getter, cleanup, property copy or task/document operation has then run.

The native shared helper's failed physical attempt shows its modal and eventually returns true. This candidate refusal is the unrepresented modal-return boundary, not a claim that original Save aborted or that its helper returned false. No synthetic modal result is supplied.

After a successful attempt, line769 calls existing `opModel(p, op)`, corresponding to native 0x140e2e76b/773 current-operation accessor and 0x14005f770. It uses the actual class/operation's existing model or existing lazy createModel producer. A faithfully reached null return returns the entire callback, matching native 0x140e2e790 ->0x140e2ec26. Tests do not fabricate a null-producing override or corrupt identity to force that branch.

A nonnull result stops at line770: `Control/Regions enumeration remains unsupported after operation-model lookup`. There is no call to the later display ROIs getter 0x140d0d670, no Regions enumeration/deletion, no Common_Mesh removal, no geometry writer, no propsToEngine, and no `.DS`/task/manager synchronization. The generic base Save sequence cannot run past this explicit guard.

The caller-owned SaveEffects retains reached step2 attempts when the guard unwinds. `engine::saveProject` reaches this dispatcher before setting projectWriteAttempted or entering its project-file/refresh writer; the tests verify that placement. Original retained evidence and corrected order/hash receipts are in `resume149-doe-output-wired-next.md`.

## Focused tests and static checks

Four new cases invoke real `engine::saveProject`, with a registered actual DoeOutput OpClass, its real current operation, active flow selection, owned Type2 Study and caller-owned SaveEffects. They do not invoke only the private mkdir helper.

1. Successful mkdir with an existing actual GuiModel retains that same model and a preexisting Regions sentinel, then refuses at enumeration. No project writer, views, XML journal, deferred writes/deletes, removals, rename, output document or MST effect is reached.
2. A regular-file blocker at the exact Regions target records a failed attempt and remains intact. The warning-modal guard is reached with the operation model still absent, proving the subsequent lazy getter was not run.
3. With the actual operation model initially absent and mkdir succeeding, the existing real class lazy producer publishes the model before the enumeration guard. Its ROIs marker remains0, and the same later effects remain unreached.
4. A broken Study backlink refuses before an attempt/model. After restoring it, the non-immediate profile also refuses before those effects. Neither case creates the Regions target.

The fixture uses test_paths scratchOutput/scratchRoot, unique process/tick/serial leaf names, and a scratch owner destroyed after the Project/model/Study owners. Cleanup removes only that checked unique leaf, preserving the shared scratch root. File blockers/sentinels are created under that same leaf. Minimal DiagramViews uses the exact `RectF rect(const SubProject&, int)` interface; all view calls should remain unreached.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_output_save_prefix.cpp` returned `ok: true` with no issues. Both frozen files passed direct UTF-8 decoding, final-newline and trailing-whitespace checks. Testkit uses TEST_CASE/CHECK/REQUIRE and `testkit::run_all()`; declared includes resolve under the actual test preflight. These static checks do not compile or execute the tests. Root's grouped Release build/CTest remains authoritative.

## Limits and next dependency

A retained active B120 Open still refuses at `.DS` DBLIST/ReportList; selected-session/global-task application is also incomplete. This explicit engine-dispatch fixture does not establish a returning retained Open or successful public conversion. Existing public output status remains guarded. No arbitrary document or database manager is substituted for the actual later output context.

The next nonnull branch needs the actual active-subproject re-fetch and ordered QDir Regions enumeration/recursive cleanup contract, preserving immediate partial effects and ignored removal results. Failed mkdir needs actual modal delivery/return before even that continuation. Cached common-mesh/ROI geometry and actual SfTaskDoe/parented document/manager synchronization remain later independent owners. Codec/key and Type2 refresh guards remain unchanged; no later helper is reordered ahead of these boundaries.

The CP142 helper records an attempt after the physical call, so host vector-allocation failure could leave an unjournaled directory effect. Likewise host/native model allocation and destructor parity is not claimed by this increment. Reached effects are not rolled back, and no native QString/codec/modal cleanup is synthesized during host unwinding.
