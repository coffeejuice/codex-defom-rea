# CP130: next DOE Save notifier branch

## Result

There is one distinct branch beyond CP125's scalar reset: **the native suppressed-return arm**. It does not need a `DoeTableRefresh` receiver or timer. Native notifier `0x1402cf3c0` checks suppression (`+0x141`) before resetting `+0x90`, looking up the signal key, or traversing receivers; if suppressed, the callee returns. Native Type2 Save then continues to the cached-manager current-PAR call at `0x1400997b2 -> 0x1400d75a0` (and the fixed modes 0, 1, 2 afterward), provided the caller itself returns normally.

The candidate deliberately does **not** yet implement that public continuation. `src/doe/table_owner_state.cpp::requireRefreshDelivery` has a `[[noreturn]]` signature and, even when `notifier->suppressed` is true, falls through to the `main-window +458 is unavailable` guard. Thus it models the no-reset scalar effect, but not the native suppressed callee's normal return. This is an explicit coverage gap rather than a contradiction in the reset ordering.

## Is this a source-closed public increment?

Not yet. The suppressed arm itself is a source-closed prefix: once a source-owned actual Session notifier has `suppressed=true`, it can return from the dispatch boundary without fabricating a table receiver, timer stop, or slot result. But the reviewed source does not establish a production transition that makes the actual Save Session's flag true. The field defaults false, and the focused test sets it directly as a modeled branch. No public Save integration should infer reachability from that test setup.

Even with a justified true value, the continuation must revalidate the already cached active-Study/manager/I/O identities and run the native ordered current-PAR then modes 0/1/2 sequence before the common Subproject_DbName/DOM/project-file tail. The isolated `saveType2SampleSequenceImmediate` helper and its sequence contract (`analysis/resume71-doe-sample-sequence-contract.md`) are not wired after this guard, and do not supply the whole remaining Save tail. In the unsuppressed arm, the exact blocker remains actual `+0x458` receiver generation/registration, delivery-versus-no-delivery outcome, timer-stop effect, and normal return of `slotUpdateUI()`/its refresh; static constructor registration does not bind those facts to this Save occurrence.

## Frozen failure order remains consistent

The CP125 cases remain correctly ordered. Open failure and post-close error are ordinary writer returns, and the native wrapper dispatches refresh after either; unsuppressed dispatch reaches the `+0x90=false` store, while suppressed dispatch returns without it. An exception before the wrapper reaches its dispatch call does not reach either arm. These cases do not add another branch after the notifier.

## Next action

Keep the public guard. Recover the actual producer/lifetime of `+0x141` on the same Save Session. If it is proven true at this occurrence, that arm can bypass receiver/timer work and proceed to the cached-manager sequence after exact owner revalidation; otherwise the ordinary unsuppressed receiver/timer normal-return witness remains mandatory. Do not use a synthetic `suppressed=true` unit fixture to enable public Save.

No source, test, build, GUI, original application or mutable REA state was changed.
