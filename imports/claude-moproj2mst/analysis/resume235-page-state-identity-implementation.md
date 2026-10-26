# CP235: page-state control identity query implementation

## Change

Added `GuiModel::matchesCurrentSimControl(expected)`, a value-only identity query. It calls `requireGeometrySyncAvailable()` once, resolves the current step control as `step ? &step->sim_ctrl : nullptr`, and returns raw pointer equality. It does not dereference or reject `expected`; null/null compares equal. `StagedEditor300::pageState()` now negates that query in its existing final condition. The eager `model_.get()` and preceding OPI, state/page completeness, model and source-model checks remain in their original order. `pageState()` remains `noexcept`.

Added direct checks for current match, mismatch, null expected with a live step, missing step with non-null expected, and null/null with a missing step. A retained completed-page fixture moves the old owned step allocation into a local `unique_ptr`, installs a fresh step, and confirms `pageState()` refuses while `partialPageState()` remains accessible; the old allocation stays alive through the check. Parked direct query checks require `M2M_E_UNSUPPORTED` for null expected with the live step and null expected with the step temporarily moved out. The parked fixture step is restored immediately after the query, before any later fixture operation or teardown.

## Validation

Focused candidate build: `python admin/build_candidate.py --label cp235-page-state-identity-repair --target m2m_mdt_simcontrol300_owner` passed (Release; source stable during build). The first target-name attempt was invalid (`m2m_mdt_tests` does not exist); the registered owner target is `m2m_mdt_simcontrol300_owner`.

Focused registered CTest passed 2/2: `m2m.mdt.simcontrol300_owner` and `m2m.mdt.viewer_box`. The viewer test includes parked direct-query refusal. No full build, freeze, or shared-state edits were performed by this implementation.

## Source SHA-256

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `953E9319F02452C11E77B3052394D548DD9A8D85C7A66E875DD2FBDB7F4DC781`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `DF037FF2D4BA12291057C99B8FB89D825496AA6F8E1B020F0662C9BE9504E9ED`
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: `EB324255B67B6E205886AD28709A56B15F16E12F35B8AAD5BFA187ECBDC5417A`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_owner.cpp`: `AF1F0DFBFE652DE6C7C4E3724B704400C3CE3A081CC090A1F8D6BC68BA2F070A`
- `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp`: `443D09152F97FF10DC777DF560E9915A89915982D6B7DDA77DA14A83FCF32903`

## Evidence limits

This is a source-preserving adapter extraction, not a recovered native editor handler or original-binary provenance claim. The parked query's direct throw is tested. A parked `pageState()` call was not executed: because it is `noexcept`, the existing geometry-barrier exception crossing that boundary would terminate the process. This retains the reviewed pre-existing behavior and does not establish original-binary termination semantics. The raw expected token, public step ownership, and other raw control consumers remain; no broader ownership-closure claim follows.

