# CP274 Cogging lookup: next-leaf audit

## Disposition

There is no evidence-backed independent production leaf at the native Cogging lookup/state-dispatch boundary. The exact remaining item is one original same-occurrence Save witness tying the event pump to process CWD, the continuing Project/active operation, and the Cogging lookup result. Keep this edge guarded; do not treat the candidate's project-directory probe as native parity.

## Source-qualified facts

- `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` identifies its basis as static analysis of `DEF_GUI_MO_64.exe v11.0.2` in the headless Ghidra copy `gproj_mdtB`, with the retained GUI-VA listings under `gui_mo/s1/mdt/L1/B/`. At native `0x140762f30`, the spring branch calls `QFile::exists("MAN_COUPLE.DAT")` using the GUI process's current directory (spec lines 349 and Q7). The spec's Q7 explicitly leaves the converter policy unresolved.
- The retained Save decompile `evidence/gui_mo/s1/mdt/save_d0.c` places `processEvents(flags=0)` at `0x140098129` before active-operation resolution and the call to `0x140052ec0`. Static OpenProject evidence at `0x14008a9b0` shows a route that can set the process CWD. This proves a possible route, not that queued Open is delivered during a particular Save or what Project/operation the continuing Save uses afterward. `analysis/resume203-cogging-event-static.md` and CP256 record this qualification.
- `source/apps/moproj2mst/src/engine/cogging_save.cpp` currently constructs `projectDir(p)/MAN_COUPLE.DAT` and checks it in the spring branch, then refuses the detected project-folder case because coupled manipulators are not ported. The native lookup is process-relative. The candidate's path therefore does not establish the native lookup outcome.
- The already-traced owner path is active operation → Cogging properties → `CogModel` → `coggingGeometryStep` → `cogPlaceManipulators`; CP145/CP256 establish that no additional model-owner lookup is missing. `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp` tests a supplied ordered adapter schedule only. It does not establish Qt queue delivery, CWD transitions, project lifetime, or post-pump operation selection. Existing Cogging geometry tests likewise do not witness this original lookup.

## Exact missing same-occurrence witness

One fresh original File > Save run, bound to the same process/run identity and disposable project hashes, must capture:

1. At Save's `processEvents(flags=0)` (`0x140098129`): entry/exit, actual queued-event delivery, process CWD, Project identity, and active-operation identity.
2. Every `QDir::setCurrent` during that interval, including caller and target, and whether File > Open actually ran and its result.
3. The Project and active-operation selected by the continuing Save at its first post-pump resolution and at `0x140052ec0`.
4. At Cogging `0x140762f30`: actual process CWD and the result of the relative `QFile::exists("MAN_COUPLE.DAT")` call.

For discrimination, queue Open of a second disposable project whose directory alone contains `MAN_COUPLE.DAT`; label its Open result separately from the Save still in progress. A candidate-injected event list, a post-run CWD sample, or a supplied Boolean is not a substitute for this correlated original observation.

## Next-leaf decision

No narrower public caller leaf is ready: changing the lookup base, enabling coupled references, or modeling reentrant Open would each choose unobserved behavior. A pure path resolver or another proxy test would not close the uncertainty; CP216/CP256 already identify that seam and its limitation. Once the witness exists, encode only the observed schedule/CWD/owner facts at the existing Save-entry and Cogging seam, with separate assertions for selected operation and lookup result. Until then, retain the guarded behavior and exact witness request above.

## Audited records

- Queue/continuation: `python admin/check_continuation.py` returned `CONTINUE`, incomplete, with 15 remaining items; the listed R300/RDOE/RBOOL/RIO items are unrelated to this Cogging edge.
- Prior Cogging audits: `analysis/resume145-cogging-lookup-edge.md`, `analysis/resume164-cogging-next.md`, `analysis/resume203-cogging-event-static.md`, `analysis/resume216-cogging-next.md`, and `analysis/resume256-cogging-next-leaf.md`.
- Original static basis: `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md`; retained supporting listings `evidence/gui_mo/s1/mdt/L1/B/b21.c`, `f30.asm`, and `evidence/gui_mo/s1/mdt/save_d0.c`.
- Candidate implementation/tests: `source/apps/moproj2mst/src/engine/cogging_save.cpp`, `save_event_prefix.cpp`, `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`, and `test_session_cogging_save_geometry.cpp`.

Read-only audit. No source, tests, original process, GUI, mutable REA state, or build was used or changed.
