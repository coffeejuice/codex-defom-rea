# CP127: actual OPI+28 flags-pointer null skip

Implemented opt-in `attemptOperationInfoRecoveryFlagsCleanup` on the actual Operation.recoveryFlags owner after every previous explicit null/empty step, including CP126's null recoveryB skip. Null05073b ->05078f advances only to ItemModelCacheBoundary, before05079e's +160 guard read. Every nonnull flags wrapper, including allocated empty, remains guarded before05076a's direct deleting wrapper065400. No reset/delete, entry inspection, +160 knowledge inference, public Close/destructor wiring or OPI expiry was added.

Only the reserved cleanup header/source/test and this new receipt were edited. No shared model/Project/Session/main/editor/lifecycle/CMake/plan edits, compilation/test execution, GUI, original launch or mutable REA database. Root owns focused and grouped source-stable validation.

## Source qualification and ordering

`analysis/resume127-close-flags-next.md` SHA-256 `2040af1fc4d93e9b54ede0be5266b10360da204e40a2753d838a6792e1a4431e` qualifies immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained050690 assembly and bounded PE/DUMPBIN reads. Native050736 tests qword[OPI+28]. Null05073b bypasses both065400 deletion and050787 owner-null assignment. Nonnull05076a calls065400(pointer,1), whose2081a0 destructor performs Qt QBasicAtomicInt::deref/conditional backing cleanup before deallocation; only afterward does050787 clear the published field. Shared_ptr reset would not reproduce that ordering or storage contract and remains unavailable.

The actual candidate+28 owner is recoveryFlags, a shared vector<bool>. Bare Operation leaves it null; ordinary operationDefaults allocates it. RecoveryB is ordinarily allocated too. New tests declare conditional null fixtures and do not claim normal Open supplies this entire prefix. Vector emptiness and unique ownership are never branch admission criteria.

## Private continuation and retry contract

The function validates the same live OPI generation before fields and requires this progress's RecoveryFlagsCleanupBoundary. It rechecks null Props/cache/recoveryB plus empty objects/positions before the flags pointer read. Null advances only the diagnostic cursor and returns NullRecoveryFlagsSkipReached. Stable retry reports ItemModelCacheCleanupUnavailable, preserving all storage and live current/OPI. It performs no+160 lookup/default commit and reaches no QObject destruction boundary.

Nonnull records a weak canonical owner and opaque uintptr_t token, latches RecoveryFlagsDeleteBlocked, and returns NonNullRecoveryFlagsDeleteUnavailable. Progress retains no strong payload lease. Stable retries stay blocked; reset/replacement or another control block at the same pointer refuses. Reappearing flags after a null skip or any earlier owner/list refuses. Entry contents are neither read nor certified.

Enums are appended. Earlier APIs preserve their existing opt-in stops: replay at RecoveryFlagsCleanupBoundary still returns RecoveryFlagsCleanupUnavailable. Once the explicit flags function has advanced, earlier entries revalidate/report the reached guard without replay. No public behavior is expanded.

## Focused validation and static preflight

Target `m2m_mdt_operation_info_cleanup` now has40 Release-active Testkit cases (32 retained, eight added). Added coverage: every explicit prerequisite including null recoveryB; null flags preserving reserved object/position storage/current/live OPI and an independent Unknown item-cache diagnostic; allocated empty/populated flags remaining intact without strong-count changes; reset/replacement refusal; same pointer with another control block; no strong payload lease; flags/recoveryB/earlier-owner reappearance; and foreign/explicitly expired OPI rejection before owner reads. Declared external reset/expiry fixtures do not certify native cleanup.

Static preflight passed real quoted-include resolution (including operation_info_model_cache.hpp), Testkit main, CHECK/CHECK_EQ/REQUIRE macros, no C assert/REQUIRE_EQ, API signatures, appended-stage routing and `git diff --check`. The only operation.recoveryFlags read is in the explicit new entry point. No compilation or test execution ran here.

Root validation:

```text
cmake --build <build> --config Release --target m2m_mdt_operation_info_cleanup
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

## Frozen source receipt

Paths are relative to source/apps/moproj2mst/:

| File | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_cleanup.hpp|`ece8f5c18a2b10a9a778e0596349560957cb543371bfd09efea811bef5685eaa`|
|src/model/operation_info_cleanup.cpp|`d684d985865a795bc3000a7eff78d36e58db3cb457d0831a63c160e3521a2909`|
|tests/mdt/test_operation_info_cleanup.cpp|`86abbad75ba04fe696aaaa6dbe221d847407186e53a1a8ff3d0bdae862cf6097`|

Source ownership is released at these hashes. The next boundary is the+160 guard read, whose same-OPI owner/producer cannot be inferred from null flags or null engineModel(+38). Nonnull wrapper deletion, item-model cleanup, member destruction, QObject expiry and registry/editor effects remain guarded. Actual accepted Close also retains its application/current-main/busy/modal/nested Save and Project teardown prerequisites. This static receipt is not a tested candidate archive or original accepted-Close witness. No reconstructed executable/deployment was inspected or changed.
