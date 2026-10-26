# CP122: private owned engine-cache null skip after Props

Implemented the next source-closed OPI cleanup prefix in the existing live-Operation progress object: after its Props-null step, an absent **owned** engine cache skips05f980 and stops before0506bb ->061180 object cleanup. Nonnull cache remains untouched before its deleting virtual. This is a private, opt-in continuation; it neither invokes ordinary destruction nor enables public Close or OPI QObject expiry.

Root authorized the existing private cleanup header/source/test exclusively. No Project storage, shared model.hpp, Editor300, Session/main, CMake or plan was edited. No build/test execution, GUI, original launch, REA import/retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Exact native order and owner mapping

Retained050690 disassembly orders0512c0 Props cleanup at0506a1, **05f980 at0506ae**, object cleanup061180 at0506bb, then position cleanup061f70 at0506c8. The new code stops before that first object call.

05f980 reads OPI's guarded+38 at05f992. Null takes05f99a ->05f9e6 ->return. It performs **no deleting virtual and no explicit null assignment**. Nonnull reads the target again and reaches virtual+18(flag1) at05f9d3. Its later actual QObject/guard effects are not implemented here; even the native nonnull body contains no trailing cbc920(null). Do not substitute a shared_ptr reset or an unconditional cache clear.

Current loader.cpp::opModel represents the ownership distinction correctly: classOf/opBase resolves the actual registry class; a nonnull classModel virtual+e0 result returns immediately. Only the fallback reads/creates **Operation.engineModel**, corresponding to OPI+38. Consequently the new cleanup reads this real owned slot directly and never calls lazy opModel, classModel/createModel, editor model getters or GuiModel destruction. A surviving borrowed model can coexist with this null owned cache.

The existing OperationInfoCleanupProgress already binds the actual OperationInfoIdentity through OperationInfoGuard and performs Props-null admission. The Editor300 diagnostic OPI snapshot protects another observer; it supplies no accepted cleanup capability and is not expired by this prefix. Normal Open commonly has nonnull Props and therefore still stops at its earlier virtual-delete guard.

## Implemented continuation

New `attemptOperationInfoEngineCacheCleanup(Operation&, OperationInfoCleanupProgress&)` follows this order:

1.Validate the same live OPI generation before reading Operation fields. Reject foreign/expired identity.
2.Require this progress to have completed its Props-null step; InspectProps/PropsVirtualDeleteBlocked cannot jump forward. Recheck that Props has not appeared.
3.At EngineModelBoundary, read actual Operation.engineModel. Null advances only the diagnostic cursor to ObjectCleanupBoundary and returns NullEngineCacheSkipReached.
4.Nonnull records a weak canonical ownership identity plus an opaque uintptr_t token, enters EngineModelVirtualDeleteBlocked and returns NonNullEngineCacheVirtualDeleteUnavailable. It retains no strong model/Operation allocation lease and mutates neither owner.
5.Retry at the nonnull guard rejects missing/replaced pointers or changed shared ownership blocks, including a different control block at the same address. External deletion does not become a native null-skip receipt. A stable retry remains guarded.
6.Retry after the null skip rejects a newly present cache or Props; otherwise it returns ObjectCleanupUnavailable without calling061180. OPI/registry current remain live and all later object/position/recovery/+160/member/QObject work remains pending.

Existing Props API behavior at EngineModelBoundary is preserved: it still reports EngineModelCleanupUnavailable and does not automatically opt into the new step. Once the explicit new continuation has advanced, its repeated Props-only entry revalidates and reports that same reached cache/object guard. Existing enum entries keep their order; new stages/outcomes are appended. Historical cache diagnostic identity is opaque; no new escaped raw-model pointer API was added.

## Focused tests and static preflight

The existing target `m2m_mdt_operation_info_cleanup` now has14 Release-active Testkit cases, retaining seven and adding seven:

- Engine step cannot bypass unattempted or nonnull Props cleanup.
- Null owned cache advances only to object guard, retaining a real object entry/recovery/current/OPI identity; retries perform no later cleanup.
- Nonnull cache refuses without reset or extra strong ownership; external deletion does not unlock continuation.
- Cache/Props appearance after the null step is rejected without advancing.
- Foreign and explicitly expired OPI identities refuse before cache inspection.
- A real registry-backed classModel borrowed return leaves the owned cache null; cleanup does not invoke its provider again or create a model. The fixture follows opBase's registry lookup rather than relying on the separate cached opClass token.
- A changed shared control block at the same cache address is rejected at the nonnull guard.

Explicit QObject expiry and external field changes in these cases are declared fixtures, not native cleanup/Close observations. No tests were executed in this lane. Static preflight checked actual include resolution, Testkit entry/macros, no C assert/REQUIRE_EQ, source/type signatures, registry lookup path, enum compatibility and whitespace. Root owns integration build/focused run/full source-stable suite:

```text
ctest --test-dir <build> -R '^m2m_mdt_operation_info_cleanup$' --output-on-failure
```

## Remaining actual Close prerequisite

This progress object is an existing private helper bound to an actual Operation; no new inert application owner was created. It still does not prove that root0299a0 reached operation destruction. The real application/main/manager, accepted can-close/modal/nested Save, feature cleanup, diagram/SubProject/Report and operation-manager ordering from CP110/120 remain required before any normal-Close caller may drive it. Source-closed null leaves cannot be invoked from adapter catch/finally to certify that ordering. Nonnull Props/model cleanup and the next061180 call remain guarded; native bugs and Save failure/partial-I/O behavior are unchanged.

## Frozen receipt

Source paths are under source/apps/moproj2mst/:

| Edited file | SHA-256 |
| --- | --- |
|include/m2m/engine/operation_info_cleanup.hpp|`f0b0b3a1df17b1ea603499483c3cb76c433a850a399a1c68e3774821c07997f3`|
|src/model/operation_info_cleanup.cpp|`e42aae0bf7bc61ad856749c4e9d3dcbbb0f010d0ba5a02ce5143834531262816`|
|tests/mdt/test_operation_info_cleanup.cpp|`6aa31adc7ece9b9118bd96a08fc7dd61926f045046866c1d9119c21b44a463d2`|

New bounded offline code reads, DUMPBIN14.44.35228.0, section-aware mapping, end-exclusive:14005f980..14005f9eb SHA-256 `702a4bc5073e6cc0fe27a325d5c9136f65f4d977c392f02d5ba1823e06c1f7b7`;140050690..1400506bb caller prefix before object call `21ec989e1d10b2dcf89c646e2b2c3d028fe7db16b98319d4acf3695246c5764e`. Read-only loader.cpp SHA-256 `746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c`; registry_rescue.cpp/opBase and operation_rescue.cpp creation order were also inspected. Retained resume52-opi-expiry-seam.md, resume54-opi-expiry-owner.md, resume57-close-staged-owner.md, resume56-opi-cache-140050690.asm and CP110/120/121 qualifications remain attached.

Source ownership is released with these three files frozen for root. This is static preflight, not a tested candidate archive or accepted native Close witness.
