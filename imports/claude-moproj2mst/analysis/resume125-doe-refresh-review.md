# CP125 independent review: DOE refresh notifier prefix

## Finding

The frozen implementation matches the retained `0x1402cf3c0` suppression/read/store order for the bounded scalar prefix, and the reset is applied to the same live `ConversionSession` notifier bound to the DOE table owner. I found no actionable defect in this slice. This is a static source review, not fresh original parity or a test/build result.

## Evidence and comparison

The retained assembly `analysis/resume64-boolean-event-route-1402cf3c0.asm` is qualified against original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the assembly artifact SHA-256 is `9AB15E4A88C891082CBFC05D07D01893D082AA204330970C893E783773DC3B64`. It establishes this order:

1. At `0x1402cf3f1`, read byte `notifier+0x141` (suppression).
2. At `0x1402cf3fa`, nonzero branches directly to the callee epilogue `0x1402cf72e`.
3. Only on the zero branch, `0x1402cf408` stores zero to `notifier+0x90`.
4. QString key construction starts at `0x1402cf41f`; map lookup/default insertion starts at `0x1402cf454`.

`analysis/resume103-doe-save-reconcile-1400d3200.asm` binds the Save caller: after the XML writer's normal return, it passes global notifier `0x143d76700` and the `2signalDoeTableRefresh()` key to `0x1402cf3c0` at `0x1400d369d`. The wrapper continues only after that dispatcher call returns. Thus an unsuppressed scalar reset precedes any key/map/receiver work, while suppression skips the reset and the native callee returns. The candidate deliberately does not model the later caller continuation or claim that Save completes.

Frozen implementation `source/apps/moproj2mst/src/doe/table_owner_state.cpp`, `DoeTableOwnerState::requireRefreshDelivery` (around lines 233–246), first validates the pre-existing admission binding, locks its weak Session notifier, reads that notifier's suppression bit, and writes its `flag90=false` only when unsuppressed. The subsequent unsupported boundary remains in both branches. The guard checks are candidate adapter qualification and do not mutate the notifier; under the qualified live-Session branch, the ordering of native scalar effects is preserved.

The Session mapping is sound for this prefix: `ConversionSession` owns one canonical `SessionNotificationState`; `DoeTableOwnerState::State` stores weak references to that Session, its ModelManager, WidgetRuntime, and notifier. `State::matches` checks Session address, manager backlink, and pointer plus shared-control-block identity for these owners before the method locks and mutates the stored notifier. It does not use a StudyIo getter or create a substitute notifier. This models the global notifier object used by the Save callsite as the Session's existing notifier, rather than treating a receiver or diagnostic copy as notifier state.

## Scope limits

The implementation stops before native QString construction at `0x1402cf41f`. It does not create a key, map entry, ordered snapshot, receiver invocation, timer effect, or normal dispatcher return. In particular, the candidate's refusal on the suppressed branch is an explicit guard on unimplemented caller continuation; native suppression itself returns from the callee. It is not a claim that the original wrapper refuses there.

The focused test `source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp` checks the actual Session notifier on unsuppressed and explicitly modeled suppressed profiles, foreign/released scope refusal, and that XML interruption does not reach the store. Those explicit `flag90=true` test initial states are ordering probes, not evidence that a real original Save starts with that value. No build, tests, GUI, original runtime, or mutable REA operation was performed for this review.

## Frozen source identity

- `table_owner_state.cpp`: `AAD5EB5382EDF7DF98ADD74576FFAB8B89738BBFF0CD0E6CCC2427C113EE466E`
- `test_refresh_notifier_prefix.cpp`: `8C1789FF7E61E3C47E52DF121C686C5AE739858986CC650BBEEBCA941CDB1B04`
- implementation receipt: `FCABFE82B1E61934E4AEAF0F77C5DCE6F00758D61CF25FF0EB1794777919BC1C`
