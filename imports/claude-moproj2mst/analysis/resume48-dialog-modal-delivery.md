# Yes-only warning: modal delivery and receiver boundary

## Static event-loop facts

The retained artifacts bind this analysis to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtGui/QtCore artifacts recorded in their contracts. No original runtime or GUI execution is represented here.

- The invalid last-operation warning calls `QMessageBox::warning`, whose implementation enters `QDialog::exec`. The latter calls `QEventLoop::exec(0x40)`. The retained directory-failure-dialog trace follows QEventLoop into current-thread dispatcher `+0x58` repeatedly until loop exit. This is a nested event-dispatch opportunity, not just a synchronous button-value lookup.
- A separate retained timer contract traces diagram attachment to `QTimer::singleShot(1000, receiver, modelUpdated())`. Each nonnull attachment registers a distinct timer; null rebind leaves existing timers pending; receiver destruction disconnects the callback but does not itself destroy dispatcher-owned timers. At delivery, the callback resolves the then-current model and item, not a registration-time snapshot.
- For this callback family, `modelUpdated()` first performs fit-view, then reads the currently attached model/item. If present, it emits the diagram-item click/update route, which can update the main-window selection/property editor and replace/refill an existing editor. A null current attachment skips selection after fit. These effects are statically traced for the selected route, not runtime-observed at the warning dialog.
- The warning contract does not prove that any timer is due when its modal loop runs. Nor does it enumerate every possible event source, posted callback, external event filter, or nested loop reachable on every operation branch. Therefore the defensible statement is: eligible queued diagram timers can in principle be delivered by the modal loop; whether one is delivered before the Yes answer, and any other callback families, remain schedule/callgraph unknowns.

The normal Close guard contract independently warns that pending eligible timers may run before a default answer and that nested Save has additional `processEvents(flags=0)` delivery points. The specific Save pump at `0x140098129` is not the warning's modal loop; do not use its nonblocking behavior to claim that QDialog::exec does not dispatch events.

## Receiver/model mutation boundary

The original diagram callback's live receiver identity and the model attachment are separate. During a modal warning, the callback—if delivered—uses the current receiver connection and then-current attachment. The current attachment may be rebound or cleared while the event system remains live. The session integration audit therefore requires receiver/dispatcher lifetime outside project storage, attachment as separate mutable state, null binding before project models are freed, and due-event delivery against the current binding.

The warning's Yes result itself is statically known to return from `0x1400b8fb0`; nested Save `0x1400980d0` then short-circuits before current-operation and file writes. Close maps that Yes to Cancel, so the accepted-Close post-answer callback (including `d0e540(0,0,true)`) does not run on this branch. This does not rule out `modelUpdated()` or other callbacks inside the warning's nested modal loop before the result returns.

## Exact guard required at the adapter API

The existing adapter API has `PumpPoint{sourceSite, occurrence}` and `DueObservation{origin, point, qualification, dueInOrder}`. `ConversionSession::dispatchAt` checks for a nonempty qualification and matching point, consumes each supplied timer before callback, and resolves the current attachment on callback. These are useful safeguards, but `qualification` is an arbitrary string and an empty `dueInOrder` can be supplied without machine-verifiable proof that the modal loop was observed and that no eligible event was due.

Before representing this warning as a delivered Yes branch, require all of the following at the API boundary:

1. A distinct pump-point kind identifying the warning's actual modal `QDialog::exec` boundary, with caller/source site and occurrence; it must not be confused with Save's `processEvents(flags=0)` pump.
2. A structured, immutable dispatch-profile receipt tied to the original binary/Qt hashes and this exact source occurrence. It must state the eligible event set and order for this modal interval, including an evidence-backed empty set when none is due. Free-form `qualification` text alone is insufficient.
3. Event entries that identify callback kind and session receiver, while timer records reference the existing logical timer IDs. Dispatch in observed order, consume each single-shot before invoking it, and resolve the receiver and then-current model/item at delivery. Retain null-bound live-receiver behavior (fit first, then skip selection) and receiver-disconnected behavior separately.
4. No inference from wall-clock elapsed time, requested 1000 ms interval, or an unspecified profile. If the required receipt/profile is absent or does not cover this modal interval, reject the conversion before returning Yes, preserving the existing unsupported boundary.

The current `DueObservation::Origin` enum distinguishes original trace from declared adapter schedule, but does not itself make either source immutable/validated or establish modal coverage. A declared schedule can support deterministic adapter tests only when its provenance, applicable modal event window, eligible event set (including explicit none), ordering, and callback bindings are part of the typed contract. It cannot be presented as original-observed behavior absent a trace.

## Qualification

The directory-failure-dialog contract supplies the static Qt nested-loop mechanics and explicitly says readiness/time-to-answer remain unknown. The Boolean timer contract supplies the diagram timer callback route and explicitly says historical timer readiness is unknown. Together they identify a possible state-changing callback family and the precise fail-closed adapter requirement; they do not establish that a callback actually ran during the last-operation warning or that all other event callbacks are absent.

No source edits, builds, app-control sessions, mutable REA operations, or runtime captures were performed.
