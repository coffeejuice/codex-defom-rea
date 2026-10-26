# CP128: OPI+160 guarded item-model dependency

The next native branch is a plain read of the current OPI+160 guarded target followed by a null test. Null skips both model deletion and guard assignment. Unlike+20/+28, the converter currently has **no actual Operation-owned+160 slot**, so a private cleanup-only null skip is not ready. The existing standalone OperationInfoModelCache is a diagnostic observation, not a producer of the actual current field. CP127 must retain its stop before this read until that ownership edge is bound.

Only this report was written. No source/test/CMake edits, build/test execution, GUI/original execution or mutable REA database. Qualifications below use the immutable original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained captures and bounded DUMPBIN14.44.35228.0/section-mapped PE bytes. Addresses use the common140 prefix.

## Exact pointer source, branch and ownership

1.After the flags skip,05078f reloads this OPI.050797 computes the address OPI+160;05079e callsC1AEF0 with that address. C1AEF0 is `mov rax,[rcx]` after its local save/reload, then return. It reads exactly one qword. There is no count, refcount modification, allocation, lookup factory, registration or callback in this accessor.
2.0507a3 tests RAX;0507a6 jumps to05080e when null. That branch skips the second getter, deleting virtual call and explicit guard-null assignment.05080e unwinds this050690 payload-cleanup function and returns050812. It does not complete the enclosing OPI destructor or expire incoming OPI guards.
3.For nonnull,0507b7 reads the same guard again, then the local target is null-tested.0507e5 invokes target vtable+18 with flags1. Only after the deleting call returns does050809 callCBC920 on OPI+160 with null. Do not collapse the two reads, dispatch and assignment into a shared_ptr reset or retain an old pre-callback target as proof of the current field.
4.CBC920 compares the stored qword to its new target; equality atCBC93b skips its mutation import. Changed value calls IAT141e058b0 atCBC947. The null branch above calls neither path. Later, the enclosing050440 destructor unconditionally callsCBC900 on the+160 guard at050478, after050466 ->050690 returns and before string/list/member destruction. Guard-registration destruction is a separate phase, followed much later by QObject::~QObject at050531 and incoming OPI guard expiry.

The retained `resume54-opi-expiry-owner.md` and `resume56-opi-cache-lifetime.md` qualify the target as SfMoItemModel: constructor0c4890 installs table14215e7b8, deleting virtual0c8180 ->0c48d0 ->d3d680; its QStandardItemModel base has an owned selection-model+48 with its own deletion before base cleanup. The member QPointer is nonowning registration metadata, while this OPI cleanup explicitly deletes its then-current nonnull target. Absence of+38 engineModel is unrelated to absence of this+160 model; class-model borrowing is unrelated too.

## Producer and current-lifetime trap

Native defaults050550 selects OPI+160 at050672 and callsCBC920(null) at05067b. That is an exact source-null initialization event. Native054f40(create=false) returns the current guarded value without creating; true+null constructs0c4890 and assigns throughCBC920 at054fc1. There is also an inline constructor/assignment route in054950 (constructor054a08, assignment054a3b), so tracking only054f40 calls is incomplete.054c80 can request create=true after its class/engine-model gates.

Class change050c80 requests054f40(false); a nonnull result is passed to QStandardItemModel::clear, preserving the direct caller's guard identity. It does not delete/null the cache. Model target deletion, content clear, guard-null assignment, member deregistration and containing OPI expiry are different events. A defaults-era KnownNull certificate cannot survive unrepresented creation or arbitrary callback boundaries as proof of the current destructor read. Earlier cleanup stages precede this late read, which is why the actual then-current owner matters.

These restrictions were already explicit in CP56; no new exhaustive Open/Save/callback writer closure is claimed here.

## Candidate audit and smallest next source task

Current `model.hpp:230` owns only GuiModel engineModel(+38). Its next comment explicitly leaves+160 unmodelled. `operation_rescue.cpp::operationDefaults` allocates recoveryB/recoveryFlags and ends with only a comment for native+160 null; no actual slot is set. `OperationInfoModelCache` is constructed only in tests. It has Unknown/KnownNull, same-live-OPI metadata and a source-label defaults commit; no Operation member stores it, no production defaults event calls it, and it has no live-model/creation/invalidation representation.

The next cleanup API cannot safely accept an arbitrary caller-supplied OperationInfoModelCache—even a same-OPI KnownNull one—as the actual field. Multiple such diagnostics can disagree, and the existing nonblank receipt is explicitly not proof that a producer ran. Adding a cleanup outcome that consumes that receipt would bypass the missing owner rather than close it.

The smallest ready owner increment, requiring root's shared-header reservation, is an **actual Operation-owned typed+160 slot initially Unknown**, bound to the same OperationInfoIdentity and distinct from engineModel. Reuse the existing cache abstraction only after separating source-owned production events from arbitrary diagnostic receipt commits. Do not allocate another native/model object or default an arbitrary Operation to KnownNull. Ordinary construction should add no make_shared or model allocation. A private accessor can expose this canonical slot to the cleanup helper; external diagnostics must not substitute another instance.

The following initializer step is source-qualified locally: at the existing operationDefaults tail corresponding to05067b, commit the canonical slot's null-default event. However, carrying that state into Open/Save/Close requires explicit invalidation or a guard before every admitted unrepresented creation/callback boundary. `postload_views.cpp::postLoadViews` is a concrete hazard: it corresponds to054950 but projects class objectTreeFill/objectTreeAdd effects without a+160 owner. A barrier would have to precede its first display/model callback, not follow objectTreeAdd. Props postLoad and other earlier callbacks must also be audited before production KnownNull can be trusted. This bounded lane does not certify that complete invalidation set; do not wire a persistent defaults certificate into cleanup yet.

After the canonical owner/current-state producer is qualified, the exact private continuation would be `attemptOperationInfoItemModelCacheCleanup(Operation&, progress)`:

1.Require the same live OPI and explicit ItemModelCacheBoundary from all preceding steps; revalidate their null/empty owners.
2.Read only this Operation's canonical current+160 slot. Unknown refuses at the read with no stage advancement or knowledge promotion.
3.Qualified current null takes0507a6 ->05080e, recording only payload-cleanup-return boundary. Do not call CBC920, destroy the guard registration, expire OPI, or claim full destructor completion.
4.Nonnull must remain guarded before0507e5 until its actual target/selection/base destruction and post-delete guard semantics are closed. No vector/shared_ptr surrogate or null-forgotten identity is allowed.

There is no source-qualified nonzero/scalar shortcut beyond the native pointer test.

## Suggested reservations and meaningful tests

For the first canonical-Unknown owner increment only: root reserves `include/m2m/model.hpp`, `include/m2m/engine/operation_info_model_cache.hpp`, `src/model/operation_info_model_cache.cpp`, and `tests/mdt/test_operation_info_model_cache.cpp` beneath source/apps/moproj2mst. It can test repeated canonical accessor identity, bare/default Operation remaining Unknown, foreign/expired OPI rejection, no duplicate owner substituted, no payload allocation/lease, and retained existing diagnostic behavior. Root must review constructor/destructor declaration order and source compatibility; native member-destruction order is not established by an implicit C++ member.

A later defaults/current-state task additionally needs `src/model/operation_rescue.cpp` and exact callback/creation barrier sites after their audit. The eventual cleanup step uses the existing cleanup header/source/test; its tests need Unknown refusal, a source-bound current-null event advancing only to payload-return boundary, stable retry, invalidation/foreign/expired-state rejection, old API opt-in behavior, and current/editor/OPI guards staying live. A nonnull target branch cannot be tested by inventing a native model owner. Existing targets are m2m_mdt_operation_info_model_cache and m2m_mdt_operation_info_cleanup; no tests ran here.

## Reproducibility receipt

| End-exclusive original byte range | SHA-256 |
| --- | --- |
|140c1aef0..140c1aefe|`5bc1bfe5b72daf957f388c00813c416d4a2b8f719b9667d675ec3db8b1150732`|
|14005078f..140050813|`7519150bc60d3df060877b1c8b1dac76800c4c1c69fe816077c440d41297abc4`|
|140cbc920..140cbc957|`fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8`|
|14005066d..140050685|`95a9c81e2fa28e06b0f85dce99aaa7fce381a7964085a2eb3cc8e5b73ee5ce64`|

Retained CP56 report SHA-256 `81a431642bf1ec3a03ab38893626da67b0950f8b873f5459455600cc5a669039`; standalone cache header/source `80819268703b3af8a9712e637fa4d43bb956bfc26ef2be2e19e198933eb0f059` / `6d4793964bb3dd297fd9e62e2c01eeae28a229c3b414669b58c8a2b95708e0ad`; postload_views.cpp `c157ae9b96a47ba136f838d79aa72ae0880e59e594a1fd5a6305a201ee601651`; setters.hpp `eb5fc98ffe07f08e56bb60c3a44cc87aebe1f5f580332aa84a7a3fa1f1ea799f`.

The public accepted Close application/current-main/busy/modal/nested Save and Project teardown prerequisites remain unchanged. This report closes the pointer-read mechanics, not the missing actual candidate+160 owner/current-state producer or normal Close root. No reconstructed executable/deployment was inspected or changed.
