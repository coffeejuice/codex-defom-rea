# CP88 — independent review of the shared Tree notifier prefix

Read-only source and retained static-evidence review. No build, test, GUI, original execution, or mutable REA operation was performed.

## Conclusion

The new prefix is internally consistent for its declared **candidate direct-callee projection**. It binds the actual `ConversionSession` manager and shared notification-state control blocks, checks the actual `suppressed` value at attempt time, and in the unsuppressed branch stores `flag90=false` before stopping at the QString key-construction boundary. The suppressed branch returns without that store. Retries retain the consumed branch. The code does not call the public Close path or claim caller-argument/setter completion.

I found no static ordering or owner-substitution defect in `close_tree_notification_prefix.hpp/.cpp`. The acceptance boundary remains narrow: caller argument construction to the native callee is unimplemented, as are QString/key/map/receiver effects after the flag store. The test has one optional strengthening opportunity around full preservation of notifier registration contents, described below.

## Owner identity and prerequisite order

`CloseTreeNotificationPrefixProgress` first requires the supplied `CloseBusyGlobalOwner` to be at `BeforeTreeEnableSelectionDispatch`, then calls `attemptCloseBusyTruePrefix` only to validate that already-reached state; it refuses if that call reports a newly committed prefix. This order avoids implicitly performing the three busy stores while constructing the notifier progress. The helper also checks the exact declared occurrence string and obtains `session.modelManagerOwner()` plus `session.sessionNotificationStateOwner()` internally.

On each attempt, the helper reacquires both owners and compares pointer identity and shared control-block identity using `get()` plus symmetric `owner_before`. It then checks `manager->ownerSession() == &session` before reading/writing notifier state. A foreign session short-circuits before that final dereference. Weak owner metadata does not keep old Session objects alive or authorize a replacement Session at a reused address. The progress stores only an opaque session-address token and exposes no owner pointer.

Qualification text and occurrence strings remain caller-supplied diagnostic labels. They do not prove a native call occurred or that the source caller produced its arguments; the comments, result constants, and report state this limitation.

## Branch and effect order

The implementation matches the retained PE order in `analysis/resume87-close-next-owner.md`:

1. Read actual shared notifier `suppressed` (receiver `0x143d76700+0x141`) at callee entry.
2. If nonzero, latch `SuppressedCalleeReturn` and return without changing `flag90`, keys, maps, attempts, or callbacks.
3. Otherwise write that same owner's `flag90=false` at `0x1402cf408`.
4. Latch `BeforeKeyQStringConstruction` and stop before `0x1402cf41f` constructs the signal-key QString.

The source does not insert the key, inspect Tree registrations/population, record an attempt, deliver a receiver, or modify project/manager/timer state. It does not use the empty-notification shortcut for Tree, whose positive native registration remains a separate unresolved provider.

`SessionNotificationState` is the actual *shared adapter owner* reached through the current `ConversionSession`, and StudyIo aliases can observe that same object. This source binding does not by itself prove equivalence to the native process-global receiver over multiple Session lifetimes. That broader lifecycle is outside the declared fresh single-project profile and must not be inferred from these tests.

## Test review

`tests/mdt/test_close_tree_notification_prefix.cpp` covers constructor no-effects and attempt-time suppression, unstarted prerequisite refusal, exact occurrence/qualification rejection, suppressed preservation, unsuppressed same-owner `flag90` visibility through a bound StudyIo, the pre-key stop, retry non-replay, foreign Session refusal, expired/replacement identity refusal, and preservation of live Project/current OPI/pending timer state. Assertions are Release-active Testkit `CHECK`/`REQUIRE` calls.

The `NotificationSnapshot` helper checks exact empty-key contents, attempts length, registration optionality, task-registration size/qualification/key presence, graph-registration size/qualification, and view-registration key/count pairs. It does **not** compare every task/view receiver identity, method string, registration order/content, every graph weak identity, or each attempt record. The implementation has no mutation path touching those collections, and the current test asserts the relevant preserved branch state, so this is a coverage strengthening suggestion rather than a production defect. If desired, snapshot/compare the complete retained registration vectors and attempt payloads without adding fake native callbacks or counters.

## Qualification and gates

- Original: DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- The source-effect order is qualified by CP87 `analysis/resume87-close-next-owner.md` and its bounded PE/import receipt. The native callsite still depends on `0x1402caa63` caller argument construction (including QString/QGenericArgument setup); this code does not reproduce or validate that work.
- No assertion here establishes that accepted normal Close reaches this prefix, that `suppressed`/`flag90` values were captured from the original process, that setter return/refresh/model cleanup followed, or that Report/nonempty manager teardown completed.
- Root remains responsible for grouped build/test validation and source freeze. Until caller setup and the accepted-Close root are recovered, keep this helper isolated and do not wire it into public Close.
