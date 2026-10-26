# CP172 R300 Stopping pending-call barrier plan

## Decision

Use a distinct, model-owned **parked-call barrier** for the zero-length Stopping branch. Its only first implementation is: after CP163's query and A publication, read the actual `S.u2b28[11]`; if zero, install the barrier, publish `S.u26c4[11]=1`, mark the barrier committed, then return the existing guarded `CoordinateSync` refusal. The barrier never resumes the reader. It blocks later candidate entry until the exact `GuiModel` is destroyed. Reset/reload and public Save remain refused while it is installed. This is the smallest cut that can safely retain cache1 without exposing it to a later cached getter.

The frame is adapter state, not an asserted native structure. Use an in-place, fixed-size payload on `GuiModel`, not an owning wrapper around the native step or A allocation. The payload records identity tokens and stage for audit; on a parked retry, code reads only the model's pending bit and throws before dereferencing any captured token. Destruction discards the observational payload and lets the real `DB_STEP_DATA` owner free A.

This plan is read-only. No source, tests, fixtures, builds, executables, GUI/app-control state, live REA session, or mutable analysis DB was touched. The original remains DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected. No new original-runtime or public Open/Save behavior is claimed.

## Native boundary and evidence limit

CP171 (`analysis/resume171-r300-next.md`, SHA-256 `e001a4919182f292c1efa5f7c2c53e155ceb48d7da3d857d179dadad49b7d3f9`) bounds the native next step to the selected length load at `0x140828248`, then the zero-only cache1 write at `0x140828265`. Loader `[0x140828060,0x1408283d5)` has native-byte hash `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796`; branch `[0x140828235,0x140828275)` hashes to `f00c9d46a8a960e005bc09ce4ed3abb11314dc29b9e02bbe7bb27c32d9f628a5`. The write precedes loader return. The reader's arrays checkpoint at `0x14082d8d6`, main-object assignment loop and node-timer Stop at `0x14082dc7c` occur afterward. They are in retained complete-reader interval `[0x14082d7d0,0x14082dc8b)`, native hash `a3bc02528ce5ce18e576c5f938d82fc8b081616504437a8ca980001ce74dd6e4`.

Static code is sufficient to justify a conservative candidate barrier: the zero branch commits cache1, and later candidate calls must not treat that bit as proof of a completed reader. It does **not** prove that the original runtime is single-threaded for every consumer, that no callback/concurrent reader can enter, or that public Open-to-InitialFill reaches this exact occurrence. Directory receipt code documents its own serialized DB/model-thread use; do not generalize that comment to every application access path. Original public fidelity still needs a same-occurrence witness tying Project/DB reader, actual S and node owner, directory receipt/index, query/version change, A producer, zero length, cache1, loader normal return, arrays timer, recipient assignments and reader/outer return. Current GUI-control release limitations continue to prohibit acquiring a new GUI session.

## Minimal data and state contract

Add a `StoppingSyncBarrier` separate from `GeometrySyncBarrier`. Its first version needs one stage (`ZeroCacheCommittedParked`; an internal `Prepared` stage may be useful during construction) and these nonowning observations:

- `GuiModel` identity/lifetime token, actual `DB_STEP_DATA*`, selected main-object and canonical node-set identities;
- Project identity token, weak `DbHandle` identity, actual reader pointer, and the copied physical directory receipt identity/generations, signed index and query result/version;
- selected region 11, pre/post query version, actual selected length zero, published A pointer and the bounded producer count;
- stage and owner-thread identity if the owning-thread contract can be established.

Do not retain a strong `DbHandle`/reader lease beyond the native-like call, own/free A, store a span over `S.u1b78`, or dereference the project/DB/step/node/receipt from a blocked call. An atomic pending flag may gate concurrent reads of the model's barrier state, but an atomic bit alone does not synchronize the rest of `GuiModel` or make raw `step` access safe. The implementation must either establish the existing serialized DB/model-thread invariant for all relevant consumers or refuse to claim concurrent safety; adding only a mutex around the flag is insufficient.

Prefer an in-place fixed-size payload (for example, an optional value stored by `GuiModel`) whose install operation cannot allocate or throw after the query/A effects. Its field types must have a reviewed no-throw installation path. If that cannot be provided, prepare the frame before any mutation and prove that allocation failure leaves the existing CP163 behavior/effects intact; do not introduce a throwing allocation between cache write and guard publication. Publish the pending state before cache1 becomes visible. Since the first cut is parked forever, no vector of recipients or continuation-owned buffers is needed yet. Any later reader-tail expansion requires new preallocation and a reviewed stage machine before its first effect.

The guard returns a refusal, it does not resume. No retry repeats the query/A allocation/cache write/timer. `reset`, DB reload/replacement, model rebind, Save/Apply, display exposure or other converted model entry rejects before touching model or file state. Only destruction of the owning model may retire the barrier; the barrier itself has no ownership effects. Never clear it merely because project/reader/receipt identity changed.

## Synchronization and reentry

The recovered path is synchronous: query, A publication, selected-length read, cache1 store, and return occur in the same getter call. The first safe contract is serialized access on the DB/model thread already required by the directory receipt registry. The current cache1-to-return span has no recovered callback. Under that exact serialization assumption, the store and stage transition are adjacent, and later entry is blocked before any owner access.

Do not claim cross-thread safety or same-thread nested reentry from static order alone. The candidate implementation must bind the barrier to its model and ensure every supported entry checks the barrier before it reads or mutates the model. If caller/thread serialization cannot be proven from the existing runner and entrypoint architecture, add a real operation gate that protects the *whole access*, including outstanding borrows, and handles same-thread recursion without deadlock; a separate flag/mutex around only barrier metadata is not enough. Tests should not invent a second thread until the architectural contract is evidenced.

The first cut never removes a barrier and has no continuation/reentry API. To complete the native reader later, either implement a synchronous same-call tail through arrays checkpoint, every actual main recipient, Stop and outer return, or design a reviewed resumable adapter stage that proves each effect is committed once. A later call must never silently resume by re-running a native prefix.

## Source files and required closure

Candidate ownership should remain scoped to the imported subtree. The following are the minimum known seams; this is a plan, not an authorization to edit them:

| File | Purpose |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | Declare the fixed-size `StoppingSyncBarrier` and private `GuiModel` member/access friend. Keep it distinct from viewer geometry state. |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | Implement the Stopping availability check; apply it before `simCtl`, `object`, `objectAt`, `objectCount`, `pdie`, reset, object mutation, KEY/material imports and other model methods. Destructor remains the terminal retirement boundary. |
| `source/apps/moproj2mst/src/engine/gui_model_access.hpp`, `source/apps/moproj2mst/src/engine/engine_internal.hpp` | Gate borrowed-step access and expose a narrow internal `requireModelUseAvailable` for cross-file entrypoints. Preserve the geometry barrier behavior. |
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | Check pending state at Stopping public entry; prepare/install before cache1; set exact committed stage; refuse immediately. Preserve all CP163 gates and query/A ordering. |
| `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp` | Refuse editor construction/repeat/apply entry before page, SC or temporary state is consumed. |
| `source/apps/moproj2mst/src/engine/loader.cpp`, `source/apps/moproj2mst/src/engine/db_load.cpp` | Gate `opStep` raw-step return, model readers, KEY/load and step-replacement/reset paths before access. |
| `source/apps/moproj2mst/src/engine/viewer.cpp`, `source/apps/moproj2mst/src/engine/viewer_camera.cpp` | Ensure viewer mutation/refresh and display selection cannot expose or mutate a parked model; keep Stopping and geometry barriers distinct while sharing only the broad access gate. |
| `source/apps/moproj2mst/src/engine/save_project.cpp`, `source/apps/moproj2mst/src/engine/save_current.cpp` | Resolve the active operation's actual model and refuse before any Save step, property write, native IO, project write or file effect. Cover direct `saveCurrentOperation` entry too. |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` | Add real owner/receipt tests for the new partial commit and every protected entry. |

Before implementation, search all direct reads of `GuiModel::step`, `Operation::engineModel`, and `opStep` users. `GuiModel::step` is currently public and direct aliases may outlive the guarded call; a member barrier cannot intercept those aliases. Either show those paths are inaccessible during the serialized operation and every supported top-level path is gated, or close/encapsulate the remaining raw borrow surface. This is a hard source-closure requirement, not a warning that a model-level flag alone solves.

## Focused tests

Extend `m2m_mdt_simcontrol300_stopping_cached` / CTest `m2m.mdt.simcontrol300_stopping_cached` using its real `Project` / `ProjectScope`, actual model/step/object/node, current physical directory receipt, query and A producer. Cover:

1. selected length zero: query and A producer each occur once, cache1 is committed exactly once, frame stage is parked, node and arrays timer deltas match the recovered prefix, and no arrays-timer assignment, main-object write or Stop is reached;
2. fresh Stopping call/page retry: refusal occurs before cache-hit success or any query/A/cache/timer replay; A pointer/content and cache1 stay unchanged;
3. each covered model/API entry: `simCtl`, object access/count, `GuiModelAccess::withStep`, `opStep`, reader/load/reset, viewer/display, StagedEditor construction/apply and Save reject before effects; check Save outputs and file effects remain untouched;
4. project scope/DB reader/receipt replacement: refusal occurs without dereferencing stale owner tokens; the old model remains parked. Destroy the actual model and verify barrier retirement plus normal step-owned A cleanup once. Reset/reload while parked must refuse and must not free A early;
5. preserve CP161/163 low-version, wrong-owner/index, missing/zero/negative-header, invalid count, cached marker and existing-A cases. Positive/negative selected lengths remain cache0 and refused in this cut.

No test should call the captured raw `step` pointer as if the barrier protects it; if such an alias is part of a supported consumer, the test should expose that as a closure failure. A sequential same-thread retry is enough for this first contract. Add race/reentry tests only after synchronization evidence supports the scenario. Then run the focused Stopping target and existing Stopping snapshot/lifecycle regressions under source-stable hashes; none ran in this investigation.

## Evidence-bound conclusion

The zero-length native cache store is understood, and existing geometry barrier code supplies a candidate pattern: `GuiModel`-owned pending state, guards before model borrows, and no release until a narrowly proven completion. It does not supply Stopping ownership, Save closure or raw-step encapsulation. A cache1-only cut is source-ready only after that closure is reviewed. Static native evidence is enough for the conservative refusal/barrier implementation; original same-occurrence evidence remains mandatory before claiming the reconstructed public path matches native Open-to-InitialFill and fully returns.

## Inspected source hashes

Hashes bind the read-only current source reviewed for this design (not a patch):

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `26768d26df546b8335a48f2374b5743c4a4b60242ff61631326652e8fdba0d45` |
| `source/apps/moproj2mst/src/engine/gui_model_access.hpp` | `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26` |
| `source/apps/moproj2mst/src/engine/engine_internal.hpp` | `d2ed7167152588552c4023a10d0f01e32fbbc677a42058e2e9cf834971ee67d8` |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `3cd7ba2978cf3823c8500b289ac27c04b1548f545a3e0b00bd3ed3a4bf8bbea4` |
| `source/apps/moproj2mst/src/engine/viewer_camera.cpp` | `9deecc6d53f2dcd2c3d305ce46845bda9714f5e724d820e762206717992666a0` |
| `source/apps/moproj2mst/src/engine/db_load.cpp` | `82b2262da2c793d75ecf61af0cca9c59b77038752cfe836330a0ca7586d63920` |
| `source/apps/moproj2mst/src/engine/save_project.cpp` | `90a0ddbf6b4c92b4f4fe8bc106aa2e9248f31c84d2a0981d0ee10eda0baf2337` |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `bc77d02830ac0c75ea5fa669f303b818178610291fdbda837f1a1abb08185129` |
| `source/apps/moproj2mst/src/engine/loader.cpp` | `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c` |
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae` |
| `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp` | `fcb187ffc0ad12f82ae565de419c3ca91e5ba5952fff07f42d65ea7aa02dfea1` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` | `31dc5b593077a7bace4a22255d58cf722481b9f90542d38f2f07e82b27324373` |
