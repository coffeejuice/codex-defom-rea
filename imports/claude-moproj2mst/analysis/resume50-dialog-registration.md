# Candidate diagram timer queue before last-operation warning

## Static registration facts

On the selected successful fresh `.moproj` Open route, the retained Open sequence and timer contract give a deterministic registration subpath:

- Post-open activation binds the nonnull active subproject model through the lazy diagram view.
- The subsequent native-view refresh binds the operation model through the same view.
- The original `0x140170430` schedules a separate `QTimer::singleShot(1000, view, modelUpdated())` for each nonnull attachment. There is no equality early return, coalescing, saved timer ID, or cancellation. Thus this selected route requests two distinct timer registrations for one receiver. This count is path-qualified; other routes are not counted here.
- Earlier Open parse pumps are before these registrations on the selected normal path and cannot consume these newly registered timers.

At completion of that Open path, the source-derived logical candidate queue therefore contains two distinct pending registrations, absent an unqualified external dispatch assumption. The adapter can deterministically model those registrations with two IDs and 1000 ms requested intervals.

## Queue state at the warning is not closed

Nested Save calls Qt `processEvents(flags=0)` at `0x140098129` before it reaches per-subproject validation and the last-operation warning. Static QtCore evidence says this pump dispatches once and can deliver due timer messages, but it does not wait for a timer that is not yet due. The evidence does not tell us which of the two timers are due at that instant. The warning then enters a nested `QDialog::exec`, which repeatedly dispatches the current-thread event dispatcher until the dialog exits. The evidence does not tell us which registrations remain pending at modal entry, become due before the answer, or are delivered during the modal interval.

Consequently, the candidate event queue can be represented as two registrations before the Save pump, followed by a conditional transition parameterized by an explicit due-in-order observation at that pump. There is no justified single queue state at warning entry: it may contain both, one, or neither timer under different permitted schedules. A timer-delivery result at the warning cannot be synthesized from the 1000 ms interval or from the fact that the default answer is Yes.

## Source-increment decision

The retained evidence does not close the warning-entry queue state, so no behavior-changing source increment is justified. In particular, do not automatically dispatch both timers, force an empty due set, or merge the two registrations. The existing `DiagramLifecycleEvents` representation already captures the deterministic registration facts and the `dispatchAt(PumpPoint, DueObservation, ...)` API already supports an explicit event-conditional transition.

The smallest future source/API increment, if an evidence-backed dispatch contract is later obtained, would be to add a typed modal-warning pump kind/identity and a validated immutable dispatch-profile reference. That profile must state ordered due timer IDs at modal entry/answer interval, including an evidenced empty set when applicable. Current `DueObservation::qualification` is a free-form string and the static evidence supplies no such profile; adding the typed guard now would only harden provenance, not resolve the actual queue state.

## Qualification

The source contracts are offline static analyses, not original runtime captures. They establish registration order/count on one selected Open path, the Save pump location and capabilities, and modal event-loop dispatch mechanics. They do not establish elapsed time, OS timer readiness, exact event order, or a complete inventory of callbacks. This report makes no GUI or runtime claim.

No source edits, builds, GUI sessions, or mutable REA operations were performed.
