# Isolated Save-entry event prefix

## Implementation

Added an isolated `SaveEntryEventPrefix` state helper and focused unit cases:

- `source/apps/moproj2mst/include/m2m/engine/save_event_prefix.hpp`
- `source/apps/moproj2mst/src/engine/save_event_prefix.cpp`
- `source/apps/moproj2mst/tests/s/test_save_event_prefix.cpp`

The helper models only the statically recovered prefix at original `0x140098129`: the one `processEvents(flags=0)` call before per-subproject validation. It rejects nonzero flags, a mismatched source occurrence, a missing dispatcher observation, or a second attempt. With a dispatcher, it requires an explicit due-ID list and passes that list to the injected dispatch callback once, in order. An explicitly qualified empty due list calls the dispatcher once with no IDs; the helper never infers emptiness. If the current thread has no dispatcher, it records the prefix attempt and performs no callback. It records the attempt before callback entry, so callback failure cannot cause this one-shot boundary to be replayed.

This is an adapter/session-state prefix, not a Qt dispatcher or timer implementation. The callback must bind the IDs to the owning `ConversionSession` and use its existing timer-consumption / current-attachment semantics. The helper does not establish which IDs are due, inspect a clock, claim a timer is pending, or dispatch the later warning modal loop.

## Evidence and scope

`analysis/wave3-boolean-timer-contract.json` binds Save entry `0x140098129` to `processEvents(QFlags)` with flags zero, distinguishes it from the time-limit overload, and traces one current-thread dispatcher poll. The selected dispatcher code has a zero-timeout poll and blocking wait requires flag 4, so this prefix does not wait for a not-yet-due 1000 ms timer. The pump precedes per-subproject validation and the last-operation warning at `0x1400b8fb0`.

If validation later opens the last-operation warning, that `QDialog::exec` is a separate nested event interval. Its due IDs/result must be supplied separately; this helper does not combine the two intervals and does not alter the warning's unsupported candidate behavior. Public Save remains unbound.

## Focused test contract

The isolated test covers: zero-flag one-poll call and due-ID order; explicit empty profile; missing/mismatched observation fail-closed before consuming the prefix; nonzero flags; duplicate attempt; dispatcher absence; and callback failure without retry. These tests are authored but were not built or run. No public Save/Close behavior is enabled by this helper.

## Limits

The `qualification` and `Origin` fields are supplied by the adapter caller; this helper checks nonempty qualification and matching point but does not authenticate an immutable receipt. Timer readiness at this Save point and the partition between this poll and the later warning modal remain unobserved. Root owns build/integration and any receipt-validation layer.

No shared CMake, build, public Save guard, GUI, mutable REA, or source integration was changed.
