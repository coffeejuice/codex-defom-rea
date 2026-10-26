# CP256 Cogging lookup and state-dispatch audit

## Disposition

No safe independent production leaf can implement native Cogging lookup/state dispatch from retained evidence. The exact missing witness remains a single original same-run Save trace correlating event delivery at the Save pump, process CWD changes, continuing Project/active-operation identity, and the relative `MAN_COUPLE.DAT` result. Keep public Save guarded at this edge.

## Reconciled completed work

- CP145 source-closed the existing owner/callback chain: active operation -> Cogging properties -> CogModel -> manipulator placement. `MAN_COUPLE.DAT` is sampled in the spring branch of native `0x140762f30`; it is not model/project/MDT state. CP143's coupled-reference helper locations and object ordering are established, but their public wiring remains contingent on a qualified lookup result.
- CP164/CP203/CP216 agree that static evidence proves capability, not occurrence. Save calls `processEvents(flags=0)` at `0x140098129`; a queued File > Open can reach `OpenProject` (`0x14008a9b0`) and `QDir::setCurrent`, but no retained original trace shows that handler running in this Save interval or identifies the Project/operation used after the pump.
- Candidate `SaveEntryEventPrefix` and its tests model an explicit, qualified adapter schedule. They do not model Qt queue delivery, CWD, Project lifetime, or operation selection. `cogging_save.cpp` currently constructs `projectDir(p)/MAN_COUPLE.DAT`; that candidate path must not be represented as native parity while the original relative lookup is process-CWD based.
- Existing Cogging geometry/list-box, MDT write/open-failure, coupled-reference helper, and adapter-schedule fixes/tests cover their own qualified boundaries; none supplies the missing lookup/state witness. The retained 42_vdie comparison has one reheat and no `MAN_COUPLE.DAT` case, so it does not cover this branch.

## Smallest independent implementable leaf

There is no behaviorally meaningful independent production leaf at the unresolved dispatch edge. A pure candidate-only path resolver with an explicit base and existence result is technically implementable, but it duplicates path projection and cannot validate native CWD or event/state dispatch; CP216 already identifies that seam and it should not be promoted as resolution of this gap. Avoid adding another proxy test.

**Source owner:** `source/apps/moproj2mst/src/engine/cogging_save.cpp` (`coggingAfterSave` / `coggingGeometryStep` / `cogPlaceManipulators`) and `source/apps/moproj2mst/src/engine/save_event_prefix.cpp` plus its header.

**Test owner if witness arrives:** `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp` for only the observed ordered event schedule, with a focused public Save/Cogging integration test alongside `source/apps/moproj2mst/tests/s/test_session_cogging_save_geometry.cpp` asserting the observed CWD lookup result and post-pump selected operation separately. Do not invent a test asserting queued Open changes Save's continuing operation before the trace exists.

**Required behavioral test after capture:** replay the captured route with (1) `MAN_COUPLE.DAT` only in the launch CWD and (2) only in the second project's directory; assert event delivery, each CWD transition, the selected Project/operation, and the exact relative existence result at the Cogging site. Compare the resulting coupled object references and preserve existing failure/order assertions. The original run is the authority for these expectations; candidate-injected schedules alone are insufficient.

## Exact missing evidence

One fresh controlled original Save invocation with stable process/run identity and project hashes, recording immediately before/after the pump: event queue delivery/handler, CWD, Project and active-operation identities; every `QDir::setCurrent` caller/target in the interval; the continuing Save's first post-pump operation resolution; and at `0x140762f30`, actual CWD plus `QFile::exists("MAN_COUPLE.DAT")`. A second disposable project with the file only in its directory distinguishes route outcomes. No such correlated occurrence appears in CP156/164/171/203/216 or the retained oracle cited there.

## Evidence and qualifications

- `analysis/resume145-cogging-lookup-edge.md`, `analysis/resume164-cogging-next.md`, `analysis/resume203-cogging-event-static.md`, and `analysis/resume216-cogging-next.md`.
- Immutable native decompile/spec: `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` (especially §5 and Q7); `evidence/gui_mo/s1/mdt/L1/B/b21.c`, `evidence/gui_mo/s1/mdt/L1/B/f30.asm`, and `evidence/gui_mo/s1/mdt/save_d0.c`.
- Candidate owners/tests: `source/apps/moproj2mst/src/engine/cogging_save.cpp`, `source/apps/moproj2mst/src/engine/save_event_prefix.cpp`, `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`, and `source/apps/moproj2mst/tests/s/test_session_cogging_save_geometry.cpp`.

Static original code establishes the relative lookup and possible CWD-changing route, not runtime event occurrence or state selection. This read-only audit changed no implementation, tests, shared state, original process, GUI, or REA database; no build or tests were run.
