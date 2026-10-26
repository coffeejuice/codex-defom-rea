# CP126: actual OPI+20 recovery-wrapper null skip

Implemented the explicit private `attemptOperationInfoRecoveryWrapperCleanup` continuation on the actual Operation.recoveryB owner. A null pointer takes0506da ->05072e without deletion, reset or owner assignment and advances only to RecoveryFlagsCleanupBoundary, before050736's +28 condition. Allocated empty and nonempty wrappers both remain guarded before050709's direct deleting wrapper065440. No public Close/destructor wiring or native cleanup-completion claim was added.

Only the reserved operation_info_cleanup header/source/test and this new receipt were edited. No shared model/Project/Session/main/editor/lifecycle/CMake/plan edits, build/test execution, GUI control, original launch or mutable REA database. Root owns compilation, focused tests and the grouped source-stable freeze.

## Source qualification and effect order

The preceding CP125 report `analysis/resume125-close-next-prefix.md` SHA-256 `274b9f0f71cc8305126b191160c112c62d56e91c29103cf970a09ebb38b6727f` qualifies the immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained `resume56-opi-cache-140050690.asm` and bounded PE bytes show: position cleanup0506c8 ->061f70, wrapper-pointer test0506d5, null branch0506da ->05072e, and next+28 test050736. On the nonnull path,050709 calls065440(pointer,1); only after its destructor/free returns does050726 null the owning field. The implemented null skip bypasses that assignment too.

The current recoveryB shared vector is the existing semantic+20 owner. The native pointer condition is tested through that actual pointer; vector size and strong count are never admission criteria. Ordinary default Operation and operationDefaults allocate recoveryB, so this increment does not assert that normal Open reaches its null branch. A shared_ptr reset would publish null earlier and could retain aliased storage; that unqualified nonnull deletion remains unavailable.

## Private continuation and retries

The new function first requires the same live OPI generation, then the explicit progress already at RecoveryCleanupBoundary. Props/cache-null and empty-object/position steps cannot be bypassed by making current fields empty. It revalidates those earlier actual owners/counts before the recovery pointer read.

For a null pointer, the function appends the next diagnostic stage and returns NullRecoveryWrapperSkipReached. It does not read recoveryFlags, touch any owner/list storage, inspect recovery entries, commit QObject expiry or invalidate registry/editor current. Stable retries return RecoveryFlagsCleanupUnavailable. Reappearing recoveryB or an earlier owner/list refuses.

For nonnull, it records an opaque uintptr_t pointer token and weak canonical owner, latches RecoveryWrapperDeleteBlocked, and returns NonNullRecoveryWrapperDeleteUnavailable. No allocation is strongly retained. Stable retry stays blocked; external reset/replacement or another control block at the same pointer refuses instead of fabricating a null skip. No entry-content identity is certified and there is no bypass beyond the deletion boundary.

All earlier entry points preserve their existing opt-in stops. Merely retrying at RecoveryCleanupBoundary still reports RecoveryCleanupUnavailable. After the explicit new function reaches a new stage, earlier APIs revalidate and report that reached guard without replay. The new enum entries are appended, retaining prior values. Metadata is the only new ownership-related storage.

## Focused tests and static preflight

The existing `m2m_mdt_operation_info_cleanup` target now has32 Testkit cases (24 retained, eight added). New cases cover explicit predecessor/opt-in order even with a declared preexisting-null fixture; null skip preserving flags/reserved list storage/current/live OPI; default allocated-empty and populated wrappers refusing intact without increased strong ownership; reset/replacement refusal; same address with another control block; progress retaining no target/payload allocation; recovery/earlier-owner reappearance; and foreign/explicitly expired OPI rejection before wrapper reads. Fixture resets and expiry are declared external candidate changes, never proof that native accepted Close ran.

Static preflight passed include-path resolution, Testkit entry point, supported Release-active CHECK/CHECK_EQ/REQUIRE checks, no C assert or REQUIRE_EQ, matching function declarations/definitions, stage routing and `git diff --check`. Weak ownership comparisons use shared_ptr/weak_ptr owner_before, including same-pointer/different-control-block detection. No compilation or tests ran in this lane.

Root's focused validation commands:

```text
cmake --build <build> --config Release --target m2m_mdt_operation_info_cleanup
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

The remaining guard is050736 (+28), not+160 or full OPI cleanup. Nonnull recovery deletion, flags deletion, item-model cleanup, member destruction, QObject-boundary expiry and registry/editor effects remain unimplemented here. The accepted normal Close application's live current-main/busy/modal/nested Save and Project teardown predecessors remain independently required; this diagnostic step does not manufacture their witness or enter adapter failure unwinding.

## Frozen source receipt

Paths below are relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_cleanup.hpp|`4231a58f2bbffa7f435860e835b6e6c425c7c067349986a8b98c513659c79bbc`|
|src/model/operation_info_cleanup.cpp|`52f811446f517211889cbd6d83697567b20286b9a876354b0858057b7ee941bf`|
|tests/mdt/test_operation_info_cleanup.cpp|`ac79d6206b2cee955597aa7f3393d8bd1609a97897acb09755f5c1cae92fdcee`|

Source ownership is released to root at these hashes. This is a static implementation receipt, not a tested candidate archive. No reconstructed executable/deployment was inspected or changed.
