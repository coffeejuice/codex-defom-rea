# Resume46 Close architecture reconciliation

The missing root integration is an explicit accepted-Close owner transition. Current code has a session lifetime, a Project storage-release helper and many receiver effects, but no production normal-Close coordinator. Calling the storage helper or allowing the session destructor to run does not execute original Close.

This is a read-only source/evidence audit. No production changes, tests, builds, original execution, app control or REA database mutations were performed. Source paths below are relative to this converter subtree. The original reference is retained DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Reconciliation against current source

| Historical claim or requested work | Current source finding | Consequence |
| --- | --- | --- |
| `wave3-close-session-integration-audit.json` identifies Display dying before older operation models. | `ConversionSession` owns both scenes and their Displays. `releaseProjectStorage()` also retains `project_->display` in `displayLease_` before `project_.reset()`. | This candidate lifetime defect is already addressed structurally. Do not repeat the old proposed fix or claim an observed original fault. |
| Diagram receiver must survive Project destruction. | `releaseProjectAfterCloseEffects()` preserves receiver/dispatcher; their destruction occurs only in `~ConversionSession()`. | Ownership separation is present. Actual `170430(null)` view mutations and subsequent notification delivery are still separate obligations. |
| Normal Close clears owned models and current selection. | `ModelManager::clear()` releases manager children in encounter order, then clears collection/current/connections. `releaseProjectAfterCloseEffects()` only releases Project storage. | These are deliberately different boundaries. `test_conversion_session.cpp` explicitly expects the manager's current child and size 1 to survive Project storage release. Never rewrite that fixture to call storage release normal Close. |
| Shared ViewManager/splitter integration is next RCLOSE work. | `ConversionSession` creates the same MainView child manager and stages the same default splitter. `currentViewManagerOwner()` still refuses because aliases after View_Default/Primary are uncommitted; `ViewManagerState::nativeLayoutResolved()` returns false. | The owner bridge has advanced, but a returned pending splitter is not an installed native layout or current-manager alias. Do not bypass the guard using `managerChild()` as an invented current lookup result. |
| Graph removal and HideSecondary are available helpers. | `removeGraphItems()` performs suppression/map/scene prefixes, then unconditionally guards at live receiver HideSecondary delivery. | An empty graph manager or all-text scenes do not authorize skipping HideSecondary. |
| Converter now does Open/Save/Close. | `core/convert.cpp::run()` constructs `ConversionSession` and runs Open/Save, then returns through its destructor; it has no normal-Close invocation. | Current public completion still means adapter cleanup, not original normal Close. Source comments correctly distinguish them. |

The checkpoint35 reconciliation already calls normal Close open and excludes historical A,B,A cross-project tests from this stage. Preserve that correction: repeated independent single-project calls are the current comparison boundary.

## Ordered original contract

1. Wrapper `140031f10` enters `1400299a0`. Busy test `2ca920` may return false immediately. Manager can-close (`c9b8c0` / `c7a100`) iterates actual owned models and asks their virtual+0x68 guard. Neither failure permits cleanup.
2. Mo guard `044110` allows immediately for null Project+19c0 or process type outside 1/5. Successful native Open selects process type 5; this differs from the MoInputMode setting.
3. Native-project current-operation question has Yes/No/Cancel and default NoButton. The retained Windows/Common-style static chain selects Yes under unchanged initial focus. Startup style selection and original runtime default observation remain qualified. The question runs a modal event loop; a default answer does not establish zero timer delivery.
4. Yes invokes `0980d0` Save while the project and diagram remain attached. Its integer answer is distinct from write success. A normal checked nonnull subproject returns No (`0x10000`) even if later project/MST writers fail. No nonnull subproject leaves Yes (`0x4000`); the guard maps that exact value to Cancel (`0x400000`). The last-operation warning's sole Yes button also yields cancellation before writes. Do not infer Close acceptance from a bool helper or from an output-write error.
5. On non-Cancel, obtain actual main-window display via virtual+0x288/+0x778, execute `d0e540(display,&0,&0,&true)` if nonnull, then `signalRemoveGraphItems`. Its receiver executes both current-scene lookups, then selective cleanup, then unconditional HideSecondary. Unknown recipients and Qt hide events must remain guarded.
6. Only once all guards/callbacks have returned: `0299a0` sets busy true (`029a54`), calls `02aae0`, and clears actual manager `c78300` (`029a76`). `02aae0` requests null Tree, Property, Scene refresh in that order, subject to notifier+141 suppression. The inspected `2ceae0`, `2ce9c0`, `2ce790` bodies each test this suppression byte before forwarding; they do not establish a blanket reset of all process globals.
7. Manager deletes owned model objects through virtual+18. Mo cleanup runs Project cleanup `0982c0 -> 0aab30`, deletes Project, clears owner+19c0, then later releases Mo model slots+19c8..19e8. `0aab30` calls `12f1190(project+60)` and returns true; its payload must not be replaced by a speculative global reset.
8. Project destructor `08a3c0` resolves actual diagram receiver and invokes `170430(null)` before clearing subprojects at `0a80e0`. Null binding writes old view position/detaches old model and executes view refresh mutations. It skips nonnull-only timer registration; it does not destroy the diagram receiver or cancel pending singleShots.
9. After manager children/list/current are cleared, `029a83..029a93` resolves current Project and invokes `02a0e0`. Its null branch emits ProjectChanged(null), RefreshMainView, DiagEditorFitAll. Later `0299a0` tail effects must be retained/recovered separately; this audit does not claim the whole body is covered by these selected sites.
10. Session/main-window teardown is a later boundary. Actual Qt descendant destruction is not established by app destructor wrappers alone. MainView private storage, guarded pointers and QObject child ownership are distinct. Project Close must preserve session remesh storage, receiver identity and pending timers until their real terminal boundaries.

Sources: `wave3-normal-close-guard-contract.json`, `wave3-close-save-return-contract.json`, `wave3-normal-close-diagram-lifetime-contract.json`, `wave3-close-hide-secondary-splitview-contract.json`, `wave3-close-shared-view-owner-teardown-contract.json`. The ordering at `029a54..029a93` and the suppression tests were also checked directly in retained bounded ASM, rather than inferred from historical prose. These are static claims, not runtime observations.

## One smallest root integration

Add a distinct, guarded accepted-Close continuation in `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` and `src/engine/conversion_session.cpp`, conceptually `ConversionSession::continueAcceptedCloseOwnerTeardown(...)`. Its responsibility is the existing Project-storage boundary followed by the actual manager clear/current reset, while session owners stay alive. Keep `releaseProjectAfterCloseEffects()` as its explicitly narrower primitive.

The continuation must require explicit evidence-backed completion of the preceding can-close, nested Save, display/RemoveGraphItems, refresh and real null-rebind phases. A bool named `accepted` alone is inadequate: it would permit skipping partial receiver work. A phase record owned by the Close coordinator can retain the last reached boundary when an effect refuses. Use existing `ModelManager::clear()` for its supported list/current/connection transition; do not manufacture a second model list or reset the entire session notifier.

This is the smallest closed ownership slice, not permission to enable full public Close. Original Mo destruction begins inside manager clear, whereas the current adapter owns Project separately. The root must preserve Project-before-model-owned-slots ordering with an explicit child destruction callback/stage; a bare `project_.reset(); modelManager_->clear();` cannot be called complete destructor parity. Non-owning diagnostic leases must not turn a native-dead model into a live current model. The existing storage-only test must continue passing; add a separate accepted-tail test rather than changing its meaning.

Why this slice: historical Display lifetime and receiver ownership work is already integrated, while current-manager alias construction/HideSecondary still has live guards. This transition removes a concrete architecture gap without guessing those GUI dependencies. A subsequent root coordinator can enter it only after those dependencies are closed.

## Focused validation and remaining evidence

- Construct the real session/Mo manager child using `prepareProjectModel`, init, construction commit and selection commit. Verify current pointer/ordinal before Close; retain a diagnostic lease. Storage-only release must retain manager state as today. Accepted-tail continuation must clear actual manager list/current and Project backlink while retaining session scenes, Display, remesh and receiver. Inspect logical native lifetime independently of shared_ptr refcount.
- Verify a pending diagram timer remains pending across the accepted Project boundary. At an explicitly declared due observation after null binding, fit occurs first and the project-dependent click branch does not run. Do not count destructor-driven dispatcher destruction as this test.
- Verify phase order using hooks that inspect real state, including model selection surviving until native current-reset position. Test refusing display/HideSecondary/null-rebind effects: no later destruction/current reset may execute. Do not repeatedly replay completed native side effects after a refusal.
- For the eventual public coordinator, test nested Save answer values independently of project/MST booleans, including no nonnull subprojects and sole-Yes last-operation warning. Current SaveCallState already retains answer; reuse it.
- Missing before end-to-end acceptance: resolved View_Default/Primary constructor and actual manager aliases; complete HideSecondary recipient/Qt effects; actual normal-Close refresh/ProjectChanged consumers; full Mo/Project/model destruction payloads including `12f1190`; exact later `0299a0` tail; due-event profiles or qualified original traces; original fresh Open/Save/Close comparison. No new original capture is necessary for this static ownership slice, but static helper tests alone cannot close RCLOSE.

## Source snapshot

SHA-256 at inspection (later root edits may supersede):

- `src/core/convert.cpp`: `ea57d098969ab37d498314662dd8cd5829f200e73c97b3ceab191beaa05c21d5`
- `src/engine/conversion_session.cpp`: `0224745a5136a0e5b2870c5d4859638268b741959887508f7bcf16fce4fc1d14`
- `src/engine/model_manager.cpp`: `69f426a43ffead038991178fe7106c3fd3f4b46b93fc4968b48d0dbd2f94972d`
- `src/engine/close_effects.cpp`: `e69c1ad5058e3f4b988258fa0b6e6206c119624b5fe4f5f769123ff6bce4302f`
- `src/engine/view_manager.cpp`: `69e4b6f1e88644502abb047824db88bab7d511d7dc76c3f3943986f1d0ea66b7`
- `tests/mdt/test_conversion_session.cpp`: `20b57b6be635894cef20d1b694ec8ea3a98ec552ddb57160695dfacd3020c0b2`

The snapshot paths above are under `source/apps/moproj2mst/`. This report makes no build or deployment claim.
