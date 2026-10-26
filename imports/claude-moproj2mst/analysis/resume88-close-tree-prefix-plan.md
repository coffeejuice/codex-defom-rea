# CP88 source plan: actual shared Tree notifier entry

Implement only the source-closed dispatcher-entry prefix identified in CP87: read the actual shared notifier's suppression; suppressed returns with no effect; otherwise write its flag90=false and guard before the key QString constructor at2cf41f. No source edit is authorized before root confirms CP87 freeze. This plan changes only this analysis file; no build/test/GUI/REA activity occurred.

The original executable remains immutable DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP87's source/byte/import receipt is `resume87-close-next-owner.md`, SHA-256 `8756b2177a937a2e19e2e1cffc906f7a79c1a2823e1eadc1e53cabd7b8a91718`. No reconstructed executable/deployment was inspected or changed.

## Exclusive write set after freeze

All code paths below are relative to `source/apps/moproj2mst/`:

- NEW `include/m2m/engine/close_tree_notification_prefix.hpp`.
- NEW `src/engine/close_tree_notification_prefix.cpp`.
- NEW `tests/mdt/test_close_tree_notification_prefix.cpp`.
- This plan receives the source receipt/hashes after authorized implementation.

No existing Session, busy helper, DOE notifier, model, Report, C ABI, public Close, CMake or shared plan edit is needed. CMake already globs src/*.cpp recursively and tests/<packet>/test_*.cpp; creating files inside these globs must wait for freeze. The implementation worker does not build; root performs grouped validation/archive.

## Precise interface and ownership

Use namespace `m2m::engine`, forward declarations for Session/ModelManager and `doe::SessionNotificationState`, and the existing `close_busy_prefix.hpp` declaration or forward declaration for `CloseBusyGlobalOwner`. No new notifier allocation or copied suppression/flag90 state is added.

Proposed API shape:

```cpp
enum class CloseTreeNotificationPrefixStage {
    AwaitSuppressionRead, SuppressedCalleeReturn, BeforeKeyQStringConstruction
};
enum class CloseTreeNotificationPrefixStop {
    SuppressedCalleeReturnReached, KeyQStringConstructionUnavailable
};
class CloseTreeNotificationPrefixProgress final {
public:
    CloseTreeNotificationPrefixProgress(const ConversionSession&,
        CloseBusyGlobalOwner& alreadyReachedTrueBusyPrefix,
        std::string sourceOccurrence, std::string qualification);
    // Noncopyable/nonmovable; stage() only. No historical raw-owner accessor.
};
CloseTreeNotificationPrefixResult attemptCloseTreeNotificationPrefix(
    const ConversionSession&, CloseTreeNotificationPrefixProgress&);
```

The exact occurrence constant should identify both the enclosing normal-Close true setter029a54 and direct dispatcher call2cac94 ->2cf3c0(false). The same2cac94 call can occur during a later false setter's still-busy branch; that branch is outside this first API. An occurrence string/qualification describes supplied entry evidence, not capture authentication or original argument completion.

Progress privately retains the Session address as an opaque comparison token, weak ownership identities for its actual manager and actual notifier, qualification, stage and the consumed suppression branch. It must not retain a pointer/reference to the busy-prefix object after construction; that stack object may die before this progress. Validate its prior boundary during construction only, using the existing API. Escaped progress has no dereferenceable historical Session/manager/notifier accessor. No new allocation/failure point is added to ordinary Session, Project or SubProject construction; this explicit diagnostic progress may allocate strings as other staged helpers do.

The result contains stop, suppression-read-this-attempt, flag90-store-this-attempt and nextSourceSite. Compile-time flags keep callerArgumentProductionCompleted, busySetterComplete, modelCleanupComplete and completeForPublicClose false. SuppressedCalleeReturnReached describes only the selected callee's return; do not turn it into setter completion or a public success Boolean.

## Ordered guard and mutation contract

Constructor, in order:

1. Check the supplied busy progress is **already** `BeforeTreeEnableSelectionDispatch`; otherwise refuse without invoking its attempt function. This prevents implicit busy stores from an unstarted prerequisite.
2. Validate the already-reached prefix with `attemptCloseBusyTruePrefix(session, busy)`. Require its pending-dispatch stop and prefixCommittedThisAttempt=false. That call performs the existing same-Session/manager check and cannot replay stores after the stage check.
3. Require exact occurrence/nonblank qualification. Obtain `session.modelManagerOwner()` and `session.sessionNotificationStateOwner()` internally; validate nonnull actual manager/backlink and notifier, then retain their weak control-block identities. Constructor performs no suppression read or flag90 write. Do not accept a caller-supplied notifier reference/value pair or use StudyIo's lazy standalone owner creation.

Attempt, in order:

1. Before every effect, compare the supplied live Session token and internally reacquired manager/notifier to the weak-bound identities using both pointer and control-block equality; validate manager->ownerSession()==&session. Do not dereference a stored Session pointer. Refuse foreign/replaced/expired identity before mutation.
2. On AwaitSuppressionRead only, read this actual notifier's current suppressed member. True latches SuppressedCalleeReturn and returns at source2cf72e/2cf737 with no write, allocation, map, attempt-log or callback effect.
3. False writes **this same notifier's flag90=false** at source2cf408, then latches BeforeKeyQStringConstruction. Return typed unavailable at2cf41f, the imported QString(char const*) provider. Do not guard population before the flag write; do not insert a key after it.
4. Subsequent attempts validate owner identity, then report the consumed stage with both this-attempt flags false. They do not reread suppression or replay flag90. Later changes to these actual fields are preserved rather than rolled back. There is no API for advancing past the QString guard or forcing completion.

Native evidence for the first effect is receiver singleton143d76700, suppression+141 at2cf3f1, nonzero jump2cf3fa ->2cf72e, unsuppressed flag90 store2cf408. Key QString construction2cf41f precedes map lookup2cf454 and snapshot2cf461. This is separate from busy global143d766b8 and remesh143d91238; do not rename one into the other.

## Caller dependency remains explicit

CP59's true busy helper stops before2caa63's Boolean/025080/QGenericArgument setup. The new direct-callee helper does not execute or certify those providers. It must remain isolated and have no callsite in existing busy or public Close. This is the same bounded seam discipline used by other actual-owner prefixes: it implements real effects at a declared callee entry while retaining unavailable original root/producer dependencies.

Do not call `observeEmptySessionNotification`: TreeEnableSelection has positive native registrationd39b9c and is outside that helper's approved empty-key set. No live/expired receiver list is examined here. The native callee's suppressed branch itself touches no key/QString, but caller argument production remains a separate prerequisite even for that branch. After eventual real setter completion, null Tree/Property/Scene refresh and manager teardown remain later guards; Report destruction and accepted normal Close root remain unclaimed.

## Focused Release-active test plan

Use real `ConversionSession`, its actual notifier owner and a declared completed true-prefix fixture. Each fixture seeds nondefault busy bytes/counter before attempting the existing helper, so preserving their later values is observable. Testkit supports CHECK/CHECK_EQ/REQUIRE, not REQUIRE_EQ; enum/pointer assertions should use CHECK. Use `int main() { return testkit::run_all(); }`.

1. **No constructor effects:** construct reached prerequisite, seed notifier suppressed/flag90, construct progress and require AwaitSuppressionRead plus unchanged fields/map/attempt count.
2. **Unstarted/wrong entry refusal:** unstarted busy progress refuses before its stores; wrong enclosing/source occurrence and blank qualification refuse before notifier effects. Keep this test separate from actual callee behavior.
3. **Suppressed actual owner:** suppressed=true and flag90=true reaches callee return, preserves flag90/keys/typed registration collections/attempts, and keeps all public/caller completion flags false.
4. **Unsuppressed actual owner:** suppressed=false and flag90=true changes that exact owner to false and stops at2cf41f. A StudyIo bound to the same shared owner must observe that store; create no second lazy owner. Missing Tree coverage still yields the flag mutation but no key/map/log/callback effect.
5. **Both retry branches:** after first attempt, externally alter suppression and flag90, retry and verify the latched phase and unchanged newly altered fields. No replay or branch switching.
6. **Foreign live Session:** construction/attempt with another actual Session refuses, preserving both actual notifiers and prefix progress.
7. **Escaped/expired identity:** destroy original Session while retaining diagnostic manager/notifier leases and progress; a replacement Session cannot use them. Only opaque metadata escapes, and no native-null inference follows from expired weak metadata. Do not access a destroyed Session.
8. **Actual unrelated owners preserved:** prepare a real registered Mo child/adapter Project, optionally bind a real SubProject pending diagram timer through existing APIs, and retain a live OpClass current guard. New attempt must preserve manager list/current/connections, Project/dataDb/moProject, pending event count/identities, current OPI guard and busy bytes/counter. No Report, tree receiver or GUI construction is requested.

Use existing metadata snapshots rather than adding fake counters or destructor callbacks solely for these tests. A supplied suppression profile tests the reconstructed branch; it does not validate original modal timing, constructor coverage or current native Tree population.

Expected target/CTest name follows existing automatic rules:

```text
cmake --build <root-selected-build> --config Release --target m2m_mdt_close_tree_notification_prefix
ctest --test-dir <root-selected-build> -C Release -R ^m2m\.mdt\.(close_tree_notification_prefix|close_busy_prefix|close_busy_false_prefix|doe_session_notifications)$ --output-on-failure
```

Before release, inspect new quoted include resolution against source/include and source/port/testkit/include, Testkit main/macros, no C assert or NDEBUG-only checks, exact flag-store-before-key-provider guard, no unqualified public wiring or empty-map insertion, and whitespace/static diff. Hash the three files and this receipt, release ownership and hold all source stable for root's build/freeze. No build/test execution is part of this worker plan.
