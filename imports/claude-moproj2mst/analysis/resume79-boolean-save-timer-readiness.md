# CP79: Save poll and Boolean first-Open timer readiness

## Result

No source-closed predicate or retained runtime event proves that an Open-registered `modelUpdated()` timer fires before the first Boolean side-file attempt on a fresh single-project lifecycle. The source proves timer **requests**, a later nonblocking dispatcher **opportunity**, and Boolean side-file **writer sites**. The missing event is actual delivery: a registered timer becomes OS-ready and its receiver callback is invoked before the side-file boundary.

## Bounded static sequence

1. On the selected successful Open route, diagram binding virtual `0x140170430` is reached twice with a nonnull model. Each binding calls `QTimer::singleShot(1000, receiver, modelUpdated())` at `0x14017085f`; separate timers are requested, without same-model coalescing/cancellation.
2. The parse/cursor `processEvents` sites precede these subproject-binding registrations in this selected path, so they cannot deliver these two registrations.
3. Native Save's `processEvents(flags=0)` call at `0x140098129` occurs before subproject validation and operation Save. Qt calls the current-thread dispatcher once using the zero-timeout poll. It can dispatch a timer already due; it does not wait for a not-yet-due 1000 ms timer.
4. If dispatched, Qt's single-shot handler consumes/deletes its timer and activates timeout. The callback can reach `modelUpdated()`, fit the view, then read the receiver's current model and current item; subsequent selection/editor updates are conditional on those live values and other gates.
5. Candidate native Save invokes Boolean side-file handling in the per-operation writer loop (`core/convert.cpp:364-385`, `booleanSideFiles` at `:377-378`) and again during Boolean MST generation (`ops/boolean.cpp:139-143`). This locates candidate writer boundaries but supplies no original timer readiness or delivery event.

## Exact missing dependency

At minimum, qualification needs a gap-free event occurrence ordered against the first Boolean side-file attempt:

`singleShot registration (receiver, dispatcher/thread, timer ID/deadline)` → `timer becomes ready / native timer event delivered` → `timerEvent` → unblocked/live receiver activation → actual `modelUpdated()` entry → callback's current attachment/item and resulting relevant editor state → Boolean side-file writer entry.

The evidence must distinguish callbacks delivered before Save entry, at the Save poll, or in a later event loop. The callback must also be correlated to one of the Open registrations and to the same still-live diagram view. Registration count, timer interval, successful Open, and Save's flags-zero poll do not establish readiness. A declared empty due-ID list is only a candidate adapter schedule, not an original no-due observation. An explicitly declared due ID likewise proves only the candidate schedule unless backed by original delivery evidence.

## Evidence audit

- `analysis/wave3-boolean-timer-contract.json`: native registrations, Qt timer/dispatcher mechanics, Save-entry pump and unknown actual elapsed time/readiness.
- `analysis/wave3-boolean-open-sequence-contract.json`: two nonnull bindings on the selected successful fresh-Open path and unresolved callback timing.
- `analysis/resume70-save-dispatcher-timers.md` and `analysis/resume75-save-dispatcher-boolean-sidefiles.md`: prior bounded ordering; both distinguish poll opportunity from delivered event.
- `analysis/resume72-boolean-first-open-timer.md`: callback/reselection chain and minimal owner-correlated first-Open observation. It reports no actual timer delivery capture and leaves the selected Open-to-Save phase unresolved.
- `analysis/wave3-save-dispatch-validation.json`: candidate validation qualifications do not constitute a fresh original timer trace.

The retained history includes no occurrence meeting the dependency above. Historical Open screenshots do not show timer IDs, timer readiness or callback phase, and the recorded original GUI attempt ended immediately after Open without Save-dispatch evidence. Static callgraph absence is also incomplete for indirect nested event loops; the Save poll cannot be asserted as the first possible delivery opportunity.

## Conclusion

Keep due IDs and callback timing external to the candidate's public Save path. The smallest resolving original capture is one owned fresh process recording both timer registrations and their dispatcher/deadline, the first timer callback and its current receiver/model/item state, Save poll entry/exit, and the first Boolean side-file writer attempt with a shared monotonic event order. Until that capture exists, the exact answer remains conditional: a timer callback **may** precede side-file emission if already due at an earlier pump or the Save poll, but no retained evidence establishes that it did.

Static retained-evidence audit only. No source, build, GUI, or mutable REA operation was performed.
