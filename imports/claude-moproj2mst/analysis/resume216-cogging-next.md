# CP216 Cogging event-pump and relative lookup continuation

## Disposition

No public Save-path change is source-closed. The exact missing occurrence is still one original same-run trace of Save's `processEvents(flags=0)` through the subsequent Cogging relative lookup, with queue delivery and post-pump Project/operation identity correlated. Current source can support a narrow test seam for the *declared adapter schedule* and lookup resolver, but that seam cannot establish native Qt delivery or justify enabling the guarded path.

## Reconciliation

- The original-source chain retained in CP156/171 establishes a possible route: Save pump at `0x140098129`; queued File > Open can reach `OpenProject` at `0x14008a9b0`, which sets CWD; the continuing Save then resolves active operation and Cogging probes `QFile::exists("MAN_COUPLE.DAT")` at `0x140762f30`. This is capability, not a witnessed event occurrence.
- `SaveEntryEventPrefix` (header and implementation) is deliberately an explicit schedule adapter. It requires a qualified observation when a dispatcher exists, dispatches supplied IDs once, and has no Qt queue, CWD, Project lifetime, or operation-selection semantics. Its tests establish only adapter behavior.
- `cogging_save.cpp` currently constructs `projectDir(p)/MAN_COUPLE.DAT` and passes that explicit path to `qt::fileExists`. The native check is process-relative after the pump. Substituting project directory for CWD, or assuming which operation survives a reentrant Open, remains unsupported.
- CP203 says the only resolving witness is a single controlled original Save trace binding pump entry/exit, event dispatch, CWD setter caller/target, selected Project/operation after the pump, and existence result at the Cogging site. Candidate-injected schedules and existence booleans are not substitutes.

## Next source-closed increment

There is no independent behavioral increment in this seam until that witness is available. The smallest useful candidate-only seam (if implementation is subsequently selected) is a pure resolver taking an explicit lookup base and filename, with tests for path construction and existence-result routing. Keep it disconnected from public Save and label inputs as adapter/test data; it must not claim to model the original CWD or event queue. Existing explicit-directory projection tests may remain candidate-only. Do not add a test asserting a queued Open changes the continuing operation.

Once the original witness exists, extend the adapter seam only with its observed ordered event and separately asserted CWD/Project/operation facts; then bind the Cogging lookup to the observed process CWD semantics and test against the trace. If no such capture can be made, record the missing occurrence as the blocker and leave public Save guarded.

## Evidence qualifications

Sources: `analysis/resume203-cogging-event-static.md`, `analysis/resume171-rcog-next.md`, `analysis/resume156-cogging-cwd-event-next.md`, `analysis/resume164-cogging-next.md`; `source/apps/moproj2mst/src/engine/save_event_prefix.cpp`, `source/apps/moproj2mst/include/m2m/engine/save_event_prefix.hpp`, `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`, and `source/apps/moproj2mst/src/engine/cogging_save.cpp`.

This is a converter-only source/evidence audit. No shared code/state, original process, GUI, live instrumentation, mutable REA state, build, or tests were used. The recovery status reports the overall GUI acceptance queue externally blocked by unavailable release lifecycle; this analysis lane is independent of that queue, but does not resolve its blocker.
