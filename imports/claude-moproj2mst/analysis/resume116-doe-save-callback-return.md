# CP116 active DOE Save: callback return versus dispatcher return

## Decisive edge

The global dispatcher has no branch on the Boolean returned by `QMetaObject::invokeMethod`. It stores that `AL` value at `[rsp+0x8c]` (`0x1402cf6cc`), destroys the local invocation arguments, and continues its registration loop (`0x1402cf6d3..0x1402cf71f`). The dispatcher later returns at `0x1402cf737`. Therefore **normal return from the signal dispatcher alone does not establish that `SfMoDoeTable::slotUpdateUI()` ran or returned**: the global suppression branch, an empty/all-dead receiver snapshot, or a failed invocation can all reach a normal dispatcher return without the table's QTimer/refresh effects.

This narrows CP114's missing edge to an actual per-registration invocation/return witness, not merely a normal return from `0x1400d3200` or `0x1402cf3c0`.

## Ordered native path

Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

1. Type2 Save caches the selected SUB's `+0x20` manager before InputTable work. `0x1400d3200` saves the XML writer result, emits no-argument `2signalDoeTableRefresh()` through `0x1402cf3c0` at `0x1400d369d`, then resumes after the dispatcher returns. The caller does not gate the later flow on the XML Boolean or dispatcher result.
2. Dispatcher `0x1402cf3c0` first tests suppression byte `+0x141` (`0x1402cf3f1..3fa`); on the unsuppressed path it resets `+0x90`, looks up the key and snapshots ordered registrations. For each live guard it builds QGenericArguments and calls the imported `QMetaObject::invokeMethod` at `0x1402cf6c6` with ConnectionType `1` (DirectConnection; `r8d=1`). The call result is stored but never tested. A no-target or failed-method route can therefore continue to ordinary dispatcher cleanup/return without a slot.
3. The constructor-time registration at `0x1400ebc5c` is a **possible** target: `SfMoDoeTable::this`, key `2signalDoeTableRefresh()`, method `1slotUpdateUI()`. The main-window getter publishes the table at `+0x458` only after construction. This static setup does not establish that the actual unsuppressed snapshot contains a live entry matching that published table for a selected Save occurrence.
4. If the selected invocation targets this table, slot `0x1400f4e10` calls `QTimer::stop()` on `table+0xb0` at `0x1400f4e25`, then calls the same table's virtual `+0x1b8` at `0x1400f4e38` (`0x1400f47e0` in the identified primary vtable). DirectConnection is synchronous: normal return from this slot follows normal return from refresh. Refresh uses then-current global Project/active SUB. It is not passed the manager cached in the Save frame.
5. Save continues with its original cached manager. A runtime witness may record a different refresh-time manager; that difference is not a native equality failure or permission to substitute the refreshed manager.

The potentially source-closed stage ends before signal emission: candidate `moproj/write.cpp:147-159` performs/journals the immediate XML normal return and stops in `requireRefreshDelivery`. `DoeTableOwnerState` does not own the native table/timer or complete typed receiver list. There is no candidate value proving either “actual table slot returned normally” or a qualified “no table invocation” profile.

## Required same-occurrence witness

For the actual Type2 Save occurrence, capture a correlated sequence containing:

- the pre-XML selected Project/SUB/Study/I/O/manager identity and XML normal-return Boolean;
- dispatcher entry, suppression value and exact ordered receiver snapshot with guard/liveness;
- the actual `invokeMethod` receiver/method and return for the registration that matches the published main-window `+0x458` table, or explicit evidence of the native no-invocation route;
- for direct table invocation, same-object receiver entry, timer pointer `+0xb0`, return from `QTimer::stop()`, refresh virtual `+0x1b8` entry and normal return/slot return;
- normal return to the original Save continuation and subsequent use of its pre-XML cached manager.

This must distinguish invocation success from dispatcher completion. `invokeMethod`'s Boolean alone is insufficient without receiver identity; dispatcher return alone is insufficient because that Boolean is ignored. Capturing both the actual receiver entry/return and the saved manager continuation closes the exact same-owner edge. The refresh-time Project/SUB/manager should be recorded separately, without an equality guard.

## Candidate disposition

No complete branch is source-closed in the headless candidate, and no new helper/test can prove this runtime delivery. Preserve the existing public Save guard and XML partial effects. The next useful evidence is one correlated original-runtime trace (if the app-control constraints later permit it), not another bounded static function name or synthetic callback flag. No source/test/build/GUI/original launch/REA access was used in this report.

## Retained evidence

- `analysis/resume51-doe-variable-notify-dispatch.asm`, complete selected dispatcher body, SHA-256 `72E47AEEF793CF4E9347BCEAE891A6EE540CECDD0254EE19C7D9FF8360395957`.
- `analysis/resume64-boolean-event-route-1402cf3c0.asm`, independent bounded dispatcher capture, SHA-256 `9AB15E4A88C891082CBFC05D07D01893D082AA204330970C893E783773DC3B64`.
- `analysis/resume103-doe-save-reconcile-1400d3200.asm`, Save wrapper/emitter capture, SHA-256 `73F96C7D1B7861059FEF9CD1A71DD7FC0283221C272A145B4B7EE027DB318E08`.
- `analysis/resume65-doe-refresh-slot.asm`, exact slot body, SHA-256 `A12B2571B28BA5CF842532FC23ACF407EBF2E4F9A64694EC4844B89B4BDA6762`.
