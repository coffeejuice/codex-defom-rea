# CP75: Save dispatcher delivery before Boolean side-file writes

## Result

Retained static evidence does **not** establish a predicate that guarantees an Open-registered diagram timer is delivered before the Boolean side-file writer runs. It establishes a possible ordering: on successful fresh Open, two non-null diagram-model bindings request separate 1000 ms single-shots; native Save then polls the current-thread dispatcher once with flags zero before Save validation and before property writers. If a timer is already ready at that poll, its callback runs before the later Boolean side-file writes. Whether the timer is ready is not statically determined.

The missing fact is the original session's elapsed time/readiness and delivery trace between registration and the Save-entry poll. Registration multiplicity is not a due-set predicate, and the zero-timeout poll does not advance time to the timer deadline.

## Bounded ordering

- Normal successful Open reaches `02a0e0 -> 02a9b0`, activates subproject zero, and binds the diagram model through `0a7b90` and subsequent native-view refresh. The selected path calls view binding virtual `0x140170430` twice on the same lazy view with non-null model. Each call independently requests `QTimer::singleShot(1000, receiver, modelUpdated())`; there is no coalescing, cancellation, or early equality return.
- Open's parse/cursor event pumps occur before these later registrations in the selected normal route. They cannot deliver these particular registrations.
- Native Save calls `processEvents(flags=0)` once at `0x140098129`, before subproject validation and before operation-writer dispatch. The traced Qt implementation uses a zero-timeout dispatcher poll. A ready timer can dispatch here; an unready timer is not waited for.
- On delivery, Qt consumes the one-shot and invokes `modelUpdated()`. That slot fits the view, then reads the receiver's current model and current item and may follow the current diagram-selection/property-editor update path. These effects are conditional on delivery; they do not establish that the selection changed to Boolean.
- In the candidate's native-save pipeline, Boolean property side files are invoked in the operation loop at `convert.cpp:364-385` (notably `booleanSideFiles` at `:377-378`). Boolean MST generation invokes the same helper later at `ops/boolean.cpp:139-143`, after `KFWRIT/Step.KEY` and before `EXECMD`, when an effects sink is present. This candidate ordering is useful for defining a test boundary, but does not prove the original timer due set or exact native writer/callback ordering beyond the retained native Save-entry poll trace.

## Static predicate assessment

The only source-level condition known at Save entry is the dispatcher call with flags zero. This does not answer the relevant predicate `timer_deadline <= dispatcher_poll_time`. Neither the requested interval (1000 ms), the fact that two timers were registered, nor successful Open proves that inequality. The timer's wall-clock origin is each registration, and the selected path does not retain a duration bound from the later registration(s) to the Save poll. The evidence also does not exhaust every indirect event loop reachable in operation loading, so it cannot prove the Save poll is the first possible delivery point in every implementation branch.

Therefore:

1. **Possible native ordering:** delivered Open callback(s) at Save-entry poll, then Save validation/property writers/Boolean side files.
2. **Also possible:** no callback at the poll; a timer becomes ready later, potentially in a separate warning modal loop or after Save.
3. **Not established:** guaranteed callback before Boolean side-file timing, no-due original schedule, callback count/order, or that any callback selects Boolean.

## Candidate-only test boundary

A focused candidate regression can inject an explicitly declared due-ID list at the Save-entry event prefix and assert that its callback/selection effects precede `booleanSideFiles`; a second case can inject an explicit empty list and assert no callback before the writer. These would validate adapter ordering under the supplied schedules only. They must not be labeled original due-set observations or used to infer native timer readiness. The public Save binding remains schedule-guarded until an original fresh-session trace supplies delivery evidence.

## Evidence and next dependency

Primary retained sources: `analysis/wave3-boolean-timer-contract.json` (registration, Save poll, dispatcher semantics, callback effects and unknowns), `analysis/wave3-boolean-open-sequence-contract.json` (successful Open ordering and two bindings), `analysis/resume66-save-event-due-source.md` (no static due-set/no-due route), and `analysis/resume63-save-event-prefix.md` (candidate explicit schedule boundary). Candidate writer ordering is visible in `source/apps/moproj2mst/src/core/convert.cpp:364-385` and `source/apps/moproj2mst/src/ops/boolean.cpp:139-143`.

Smallest resolving evidence: a fresh original single-project Open/Save capture that records both timer registrations and their deadlines (or equivalent native timer IDs), the Save-entry `processEvents(0)` boundary, each actual `modelUpdated()` callback and current item/editor state, and the first Boolean side-file attempt. Record whether a timer fires before, at, or after that writer boundary. Without this runtime evidence, retain the static conditional and candidate-only schedule tests; do not assert a guaranteed pre-sidefile callback.

No source, build, GUI, or mutable REA action was performed.
