# CP165 connector independent source and test review

## Decision

**Block CP165 source acceptance.** The normal admitted path performs the narrow QtCore QObjectPrivate address-point overwrite on the existing sparse private object, with no modeled QString or counter effect. Two issues prevent acceptance: the allocation store's ownership-only check can admit an aliased pointer to a different widget root, and the new negative test does not exercise the CP165 guard at its pre-write phase. Focused tests, the source-stable regression gate, and freeze remain unverified in this review.

This is a static review of the three CP165 files and the CP164 store/session guard. I did not edit source or tests, build, run tests, control a GUI, or open a live REA session.

## Exact findings

1. **Root-identity blocker.** `source/apps/moproj2mst/src/doe/task_runtime.cpp:30-35` makes `ownsWidgetRuntime` true when the supplied `shared_ptr` and the store's weak pointer have the same control block, without comparing their stored `WidgetRuntimeState*` values. The public factory accepts any `shared_ptr<WidgetRuntimeState>` (`include/m2m/doe_task_factory.hpp:67-68`). A caller can make `std::shared_ptr<WidgetRuntimeState> alias(owner, foreign.get())`, keep `foreign` live, and pass `alias` to the factory. It shares `owner`'s control block but dereferences `foreign`. At `task_factory.cpp:136-148`, the CP164 guard accepts that alias and binds/increments `foreign->qlistSharedNull()`. At `:151-163`, the CP165 guard again accepts it because it checks the weak binding against the factory's `widgets->qlistSharedNull()`, not against the store's expected root object. This violates the plan's expected allocation/session root identity while leaving the fixed QtCore digest and RVA checks satisfied. Require both shared-owner identity and `widgetRuntime_.lock().get() == widgets.get()` at the store guard; add a foreign-alias negative test that checks both roots' deltas, the binding, and the cursor before the CP164 event. Ordinary unrelated roots and the actual `ConversionSession` pairing are covered by the existing tests, but do not close this alias route.

2. **CP165 negative-test blocker.** `tests/mdt/test_doe_task_factory.cpp:349-375` first calls `createTaskBase` through both CP164 and CP165, requires `BeforeQStringSharedNull` and RVA `0x1a9d10`, and only then clears the committed marker or replaces the private weak binding. The second call bypasses the `BeforePrivateFinalAddressPoint` branch by phase, so its refusal does not prove that an uncommitted, absent, expired, or mismatched binding blocks the address-point write. Add a test that presents each damaged condition while the frame is at `BeforePrivateFinalAddressPoint`, then checks refusal preserves `0x1a9a30`, the cursor, the pending/private identities and the original root delta. The test can explicitly construct that pre-write diagnostic frame; the current public call advances both cuts in one invocation.

3. **Reentry assertion gap.** The same-key and different-key checks at `test_doe_task_factory.cpp:321-330` retain the pending/private pointer, binding, phase, and delta after each refusal, but do not reassert address-point RVA `0x1a9d10` after either refusal. The plan explicitly requires this after each call. Add that assertion alongside the existing state checks. This gap is smaller than findings 1 and 2.

## Supported normal-path effect

`ConversionSession` pairs its actual `taskAllocationStore_` and `widgetRuntime_` before factory use (`src/engine/conversion_session.cpp:163-172`). For that ordinary pair, `initializeNativePrefix` creates one pending Report and one `SparseQObjectPrivate`, binds the CP164 root and commits one symbolic QList reference at `task_factory.cpp:105-148`, then checks the committed marker, live binding, root symbol qualification, original address-point RVA, and incomplete QObject state at `:150-161`. It changes only `privateState->addressPointRva` to `0x1a9d10` and phase to `BeforeQStringSharedNull` at `:162-163`. No new private allocation occurs between CP164 and CP165, so the same `unique_ptr` target is retained. The `privateIdentity()` comparison at `:159` is tautological because `frame` already aliases `*pending->qobjectPrefix`; it is not an independent historical-identity check.

The QtCore slice in `analysis/resume163-connector-qobject-67131160.asm` has the `LEA` at preferred VA `0x67131198` and `[RBX]` write at `0x6713119f`; the next instruction at `0x671311a2` begins the QString shared-null path. The model stores the qualified QtCore digest and RVA, not a host address. The source makes no QString binding or increment and leaves task creation, factory maps, backlinks, thread and QObject completion absent on this path. The normal test checks the phase, final RVA, one retained prototype, root delta `+1`, empty maps and no task after the first call (`test_doe_task_factory.cpp:299-320`). It does not separately observe the pre-CP165 frame because both cuts execute in that call.

The alias finding is a source-level admission counterexample. It does not claim that `ConversionSession` constructs such an alias or that the original application follows that path. The source evidence supports only the selected normal-path instruction slice, not native failure unwinding or an absolute QList count.

## SHA-256 receipts

These current file hashes match the CP165 implementation report for the three assigned files. The CP164 store and Session files match the repaired CP164 receipt.

| File, relative to converter checkout | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0614b3d702f2fd394fd9cdab26c4a22297781483d46245ff3c67719d69fe9a96` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `debe5478a9eb0c89a964f77f8e6c48b61489f1ba335f7694a6d7da6d629c706a` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5f8cb8cdb27189f06155524642e504b544736f6db24e3fe0dda749b6c16810bb` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `e75573b6264abd07afc082472f1d532e9a0467ff262a458e15bc9242f3cd9d7d` |

After the root-identity repair and focused test correction, run the grouped focused factory/root-owner tests and the normal source-stable checkpoint build/regression and freeze gates. This note records no execution result.

## Re-review after root and test repair

**Block the repaired CP165 test set pending one final-RVA assertion after each normal reentry.** The production source guard and the two larger negative-test findings above are repaired in the current hashes. This is a static acceptance decision only; no test execution or source-stable checkpoint result is claimed.

`TaskAllocationStore::ownsWidgetRuntime` now first requires matching shared ownership and then locks the associated weak pointer and requires exact stored-pointee equality (`src/doe/task_runtime.cpp:30-37`). Thus an alias with the expected control block and a foreign `WidgetRuntimeState*` fails both CP164 and CP165 factory checks. The new alias test constructs that case, checks refusal at the pre-QList boundary, and verifies no private binding or reference delta in either root (`tests/mdt/test_doe_task_factory.cpp:457-475`). The actual `ConversionSession` pairing still has a positive test at `:333-346`.

The new pre-write CP165 test at `test_doe_task_factory.cpp:379-405` explicitly restores `BeforePrivateFinalAddressPoint` and RVA `0x1a9a30` on the retained frame, then injects either a foreign weak binding or an uncommitted marker. The factory enters the CP165 branch at `task_factory.cpp:150-163`; the test requires the guard's refusal message and checks that the pending/private identities, phase, initial RVA and one prior root delta remain. This resolves the original post-advance-only test limitation for those two damaged conditions. The source also rejects an absent or expired weak binding through `!boundRoot`, although this repaired test does not separately inject those variants.

**Remaining exact gap:** the normal same-key and different-key reentry assertions at `test_doe_task_factory.cpp:321-330` still check pointer, binding, phase and root delta, but do not check `addressPointRva == 0x1a9d10` after either refusal. The initial value is checked at `:312`, and the damaged post-advance test checks it at `:372`; neither proves retention across the two normal reentries. The implementation report's statement that final RVA is asserted after each reentry is therefore inaccurate for this hash. Add the final-RVA assertion after each call, then rerun the focused and source-stable integration gates. No other source blocker was found in this bounded re-review.

### Re-reviewed SHA-256

| File, relative to converter checkout | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5d9b94001f94f014b03b32d88521aff8ce1c4c751b735123145e91b15d4befef` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0614b3d702f2fd394fd9cdab26c4a22297781483d46245ff3c67719d69fe9a96` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `256606a214ae45605ca939128737144aec36d84229e46a04bc5185f3904addea` |

This re-review edited only this analysis note. No source, tests, build outputs, GUI, or live REA session were changed.

## Final re-review after reentry assertion repair

**Accept the CP165 source and focused-test design for the final four hashes below.** This supersedes the prior static test block. The test now asserts `addressPointRva == 0x1a9d10` immediately after the same-key refusal (`test_doe_task_factory.cpp:321-326`) and the different-key refusal (`:327-332`), alongside retained pending/private identities, binding, phase and root delta. The exact-pointee plus shared-owner store guard, alias negative case, and restored pre-write CP165 tamper test remain as assessed above.

The local `analysis/resume165-final-build.json` receipt records source tree SHA-256 `7aa4d2c20b5ce1c5d8d38a0c8e75bc2c2945d53c3bedb72e24643d5b8219adca` both before and after a completed build, with `source_stable_during_build: true`, and a focused `m2m.mdt.doe_task_factory` result with exit code zero. The integration owner reports the full gate as 361 registered tests and zero failures. This review did not run either gate, inspect every CTest result, freeze the candidate, compare an original-process execution, control a GUI, or use live REA. Its acceptance concerns the bounded normal-path source/test contract, not native failure or unwind behavior, absolute QList count, QString effects, later QObject/TaskInfo completion, or whole-application fidelity.

### Final source/test SHA-256

| File, relative to converter checkout | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `dddec94ca7766ce4637fd5bf0e39475871d866663385f5a0757dd5b788b801ee` |
| `source/apps/moproj2mst/src/doe/task_runtime.cpp` | `5d9b94001f94f014b03b32d88521aff8ce1c4c751b735123145e91b15d4befef` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0614b3d702f2fd394fd9cdab26c4a22297781483d46245ff3c67719d69fe9a96` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `b662d12e393e794d7823525447f9d5d5accc8380fed1f0b6a55b77091577fd8a` |

This final re-review edited only this analysis note.
