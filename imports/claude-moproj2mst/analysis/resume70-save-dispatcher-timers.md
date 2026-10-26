# CP70: Save dispatcher timers after successful Open

## Finding

For the retained normal fresh-Open path, the earliest specifically evidenced Save-side dispatch opportunity for the timers registered by Open is the `processEvents(flags=0)` call at native `0x140098129`, before Save validates the active subproject and before it changes the current operation to Save. The call polls the thread event dispatcher once. It can deliver an already-due timer message; it does not wait for a 1000 ms timer to become due. The retained evidence does not show whether either Open timer was due or delivered at this boundary.

This is the earliest in-scope observable boundary established by the selected retained call chain, not a claim that no untraced descendant or OS message dispatch can occur between Open and Save.

## Retained call order

1. Normal `.moproj` Open follows `0271b0 -> 01ca30 -> 08a9b0 -> 08afa0 -> 08b410 -> 02a0e0`. The selected successful post-Open branch activates subproject zero through `02a9b0 -> 0a7250`; it binds the diagram/model at `0a7b90` and then refreshes the native view through `038380`.
2. The lazy native diagram view is created by `012fa0 -> 012fe0 -> 14ef60`. Its model-binding virtual is `0x140170430` (view vtable `+0x1b8`). Each non-null binding schedules a distinct `QTimer::singleShot(1000, this, modelUpdated())`. The selected active-subproject path reaches this binding twice on the same view; the implementation has no equality early return, saved timer ID, cancellation, or coalescing.
3. The Open parse pumps at `08a9b0` (flags zero) and cursor helper `2ca7a0` (flags one) precede these later subproject-binding registrations in the normal path. They therefore cannot deliver these registrations. This ordering is bounded to the selected normal path; it does not prove all indirect modal branches are absent.
4. Public native Save reaches `processEvents(QFlags)` at `0x140098129` with flags zero, before active-subproject validation (`0x1400b8fb0`) and current-operation Save (`0x1400981e1`). The import maps this overload to `6711d130`, which invokes the thread dispatcher vtable `+0x58` once. The traced dispatcher uses a zero-timeout poll. Its blocking wait path requires `flags & 4`, which is false here.

## If a timer is delivered

The dispatcher owns each single-shot timer. On delivery it kills the timer, marks its ID `-1`, activates the timeout, and deletes the timer. `modelUpdated()` first calls the view's fit operation, then reads the *current* model attachment and its *current* item. A null current model or item skips the diagram-click signal. Otherwise it sends the current item through `DiagItemClicked` and `UpdateMainView`; downstream view update uses the then-current item ID. For a non-Process item with an existing property editor, that path can apply the existing editor, leave it, create/fill a replacement, and show it. The callback does not preserve a registration-time model/item snapshot.

These are static effects conditional on delivery. They do not establish that the callback ran during the historical Open-to-Save interval, nor which item/editor state it saw.

## Earliest boundary and limits

The first boundary at which the retained selected path both has the Open timers registered and explicitly names a later dispatcher invocation is Save entry at `0x140098129`. The Open pumps are too early for those timers. The Save poll is nonblocking and only offers delivery if the OS timer is already ready. Actual elapsed time, timer readiness, message ordering, and delivery count are unknown. The historical GUI attempt ended after Open and contains no Save-dispatch observation. No retained capture establishes a timer callback between successful Open and this Save poll.

The nested warning `QDialog::exec` path, if Save reaches it, is a separate modal dispatch interval that can process events until the dialog exits. It must not be conflated with the one zero-timeout Save-entry poll, and it does not resolve whether an Open timer was delivered earlier.

## Candidate adapter boundary

The current synchronous candidate session owns a clockless logical event queue and delivers only explicitly qualified timer IDs through `dispatchAt`; it is not the native Qt dispatcher and has no measured due set. A candidate profile may explicitly declare zero timer deliveries at the Save-entry boundary, but that is an adapter schedule choice, not an observation of native readiness. Do not bind a timer count, partition, or callback state to the public Save path from this evidence alone.

## Evidence

- `analysis/wave3-boolean-open-sequence-contract.json`: selected successful Open order, two non-null view bindings, and unresolved Open-time callback timing.
- `analysis/wave3-boolean-timer-contract.json`: timer registration, dispatcher ownership/delivery, callback read timing, Save-entry poll, flags/overload, and explicit unknowns.
- `analysis/wave3-boolean-timer-contract.json` also records that exact earlier Open-time model state needs a deterministic fresh reference action/dispatch schedule with callback evidence; the retained historical Open capture does not supply it.
- `analysis/resume65-save-event-schedule.md` and `analysis/resume66-save-event-due-source.md`: prior bounded Save-schedule audits; no qualified native due set or candidate-to-native dispatcher ownership was established.

## Next evidence needed

A controlled fresh-session reference capture must record Open completion, the two registrations, and whether `modelUpdated()` is delivered before the Save-entry poll, at that poll, or later. It must also record callback receiver/current attachment/current item and the dispatcher boundary. Until a GUI release/status receipt and the recovery workflow's gate permit such capture, keep public Save's timer schedule guarded; the static evidence supports the opportunity and callback semantics, not delivery.
