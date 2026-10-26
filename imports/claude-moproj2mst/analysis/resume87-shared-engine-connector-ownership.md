# CP87 — shared-engine / connector ownership next action

Read-only source and retained-evidence audit. No source, build, test, GUI, original execution, or mutable REA change was made.

## Finding

There is no evidence-backed production ownership correction to make at this point. The shared graph already has a concrete owner: `SubProject::flow.items` is a `vector<unique_ptr<FlowItem>>`; connector endpoints are nonowning `FlowItem::next/prev` pointers; operation objects are project-owned and resolved by operation ID. The full-project connector test and active DoeOutput guard test both exist. The latter currently checks the requested edge before `openActiveOperation`, but does not assert endpoint identity/membership after the expected partial-load refusal.

The smallest accessible next increment is a test-only extension to:

- `source/apps/moproj2mst/tests/mdt/test_doeoutput_active_guard.cpp`

No production source, header, or CMake change is needed.

## Source basis

The current test `active_doe_output_stops_at_retained_ds_dblist_guard_after_study_setup` uses the retained full `120_B_DOE/COG_P1_B1.moproj` fixture, copies it to `M2M_TEST_SCRATCH`, activates Sub_Project 1, checks the real registry flags (`Cogging 0x1a`, `DoeOutput 0x20097`), and checks the operation 3 -> 6 reciprocal connector before dispatch. It then calls `engine::openActiveOperation` and expects the first candidate guard at DBLIST/ReportList import. It already checks the `.DS` read, three study text writes, empty `FileEffects` writes/deletes, and unchanged copied input manifest.

Candidate source explains the unchecked adapter boundary:

- `include/m2m/model.hpp`: `FlowItem` contains `next` and `prev`; `FlowChart::items` owns each item with `unique_ptr`; `SubProject` owns the `FlowChart`.
- `src/engine/loader.cpp::openActiveOperation`: obtains the current `FlowItem` through `activeSubProject(p)->flow.items[activeItem]`, resolves its operation by id, sets class current, resets its engine model, and invokes `loadOperation`. The `oprFlag` fallback, material sync, display reset, and returned operation occur only after `loadOperation` returns normally.
- `src/ops/noop.cpp::DoeOutputClass::loadOperation` and `loadClassData`: DoeOutput resets its class-local state and model, then loads the selected Type 2 DOE document. The unresolved DBLIST/ReportList condition throws during `readDoeDocument`, before normal completion of `loadOperation`.

This supports a narrow candidate regression that the *graph ownership projection* is stable across this exception. It does not establish original pointer lifetime or QObject behavior during native GUI callbacks.

## Proposed focused assertion contract

In the existing active guard test, immediately before `openActiveOperation` save:

1. The actual `SubProject*` for index 1 and its `flow.activeItem` value.
2. The `FlowItem*` for operation 3 and operation 6, plus their `next/prev` values.
3. The operation 6 pointer returned by `operationById(p, 6)` and the Project/FlowChart owner identities.

After catching the exact expected DBLIST/ReportList refusal, assert that:

- `p.subs[1]` is the same SubProject, it remains `activeSub == 1`, and its active item is still index 5.
- Both endpoint pointers are still present at their original indices in the same `flow.items` owner vector, and reciprocal `Cogging.next == DoeOutput` / `DoeOutput.prev == Cogging` still hold.
- Operation 6 still resolves to the same project-owned `Operation*`; no graph endpoint or owner list was silently replaced/cleared by the partial load.
- Existing assertions continue to validate the pre-refusal study writes, `.DS` read/warnings, no `FileEffects` writes/deletes, and unchanged source/disposable input manifests. Do not assert rollback of the DOE class's already reached internal resets or the three study text effects.

These are identity and membership assertions, not counts alone. They catch a regression where partial DoeOutput loading accidentally detaches, replaces, or invalidates the active connector graph while preserving a visually equal pair of IDs.

## Native evidence and qualification

`analysis/resume54-connector-regression.md` binds the retained project and synthetic P03 edge check; `analysis/resume59-connector-next.md` distinguishes that check from active operation dispatch and records the candidate's exact first DBLIST refusal. `analysis/resume83-report-nonnull-next.md` additionally resolves the Report constructor's `SfSessionManager` identity and destructor chain, but does not bind this accepted Close or any native connector graph lifetime during the DoeOutput attempt.

The retained original connector admission probe is `analysis/connector-admission-original.json`, source `0x140074f50`. It uses controlled type/owner callbacks; it is not a full-project parse, active-operation dispatch, or exception-lifetime capture. The proposed regression is therefore candidate-only adapter validation. It must not be described as original connector/ownership parity, original Open/Save/Close acceptance, or evidence that native objects survive the same failure path.

## Gates

Before adding the assertions, root should reserve the existing test file after the active source freeze. Keep this to the single test file, preserve all current partial-effect expectations, and use Testkit `REQUIRE`/`CHECK` macros. Validate vector membership by pointer identity without mutating `flow.items`. Root's source-stable build and test group remains required. Any original parity claim requires a separate fresh paired original/candidate lifecycle observation with the same active Sub_Project and captured graph identities; current static/source evidence cannot supply it.
