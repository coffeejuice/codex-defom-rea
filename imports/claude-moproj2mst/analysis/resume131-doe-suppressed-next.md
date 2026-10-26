# CP131: isolated suppressed DOE notifier prefix

## Finding

Yes. A small isolated helper can truthfully model the native suppressed return without fabricating a table, receiver, timer, or UI callback, while leaving public Save guarded. It should return an explicit **callee outcome**, not a Save-success value.

The owner and order are already available in candidate source. `DoeTableOwnerState::State::matches(session)` binds the state to the same live `ConversionSession`, ModelManager, widget runtime, and shared notifier owner, and rejects a released admission scope (`src/doe/table_owner_state.cpp:40-55`). The native ordering from the retained notifier evidence is then:

1. Read the actual notifier's `suppressed` (`+0x141`).
2. If true, return immediately. Do not read/change `flag90`, inspect the `+0x458` table field, construct a signal key, look up registrations, or enter timer/UI work.
3. Otherwise write `flag90=false` (`+0x90`) before the unresolved key/map/receiver work. Return an `UnsuppressedDispatchRequired` outcome to the isolated caller; do not imply dispatch happened.

The existing `requireRefreshDelivery` performs steps 1-2 partially but then always refuses (`src/doe/table_owner_state.cpp:233-245`). Its `[[noreturn]]` interface therefore cannot represent the native suppressed callee return. This does not require weakening or replacing the public guard: keep `write.cpp`'s call site guarded until the caller continuation and remaining Save tail are implemented.

## Narrow helper contract

Add a private/testable state-prefix function on the existing actual-Session owner, with an enum such as:

```cpp
enum class DoeTableRefreshPrefixOutcome {
    SuppressedCalleeReturned,
    UnsuppressedNeedsReceiverDispatch
};
```

Its operation should be exactly `matches(actualSession)` → lock/verify the bound notifier → test `suppressed` → early return outcome if true → otherwise set `flag90=false` and return the unsuppressed outcome. No `TableOwnerReport::tableField` query belongs in the suppressed case because native callee returns before resolving main-window `+0x458`. The helper must not mutate `emptyKeys`/attempt history, touch receiver metadata, or mark timer/slot receipts. If the admission scope is foreign, stale, or released, preserve the existing adapter failure before either scalar store/read is reported.

The isolated test can set `suppressed=true` diagnostically, seed `flag90` both true and false, keep table field unavailable, and assert the returned suppressed outcome plus unchanged `flag90`, key set, attempts, and absent receiver/timer state. Separate refusal cases should prove wrong/released Session scope stops before scalar effects. For unsuppressed input, assert the helper returns only the “dispatch still required” outcome and clears `flag90`; do not test public Save success from that result.

## Integration limit

This helper would close only the callee's early-return prefix. The focused Save test currently sets `suppressed=true` directly and labels it as a modeled branch (`tests/doe/test_refresh_notifier_prefix.cpp`); no production same-Session producer of `+0x141=true` has been established. Therefore the helper test must not route actual public Save through the suppressed branch.

Even if a real producer is later found, Save may continue only after binding the exact cached Study/ModelManager/I/O identities through the source-ordered current-PAR and fixed mode 0/1/2 writers, then the common Subproject_DbName, pending-node replacement, and main project-stream tail. The existing immediate sample sequence is isolated (`analysis/resume71-doe-sample-sequence-contract.md`). For the ordinary unsuppressed path the current blocker remains the occurrence-specific `+0x458` table receiver/registration, suppression-vs-delivery result, actual timer stop, and normal `slotUpdateUI` refresh return; static registration cannot substitute for those runtime facts.

No source, test, build, GUI, original application, or mutable REA state was changed.
