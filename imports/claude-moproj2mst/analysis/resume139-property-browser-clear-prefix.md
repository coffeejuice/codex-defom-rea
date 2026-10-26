# CP139: reverse browser snapshot prefix frozen

Implemented CP138's private clear traversal in three new exclusive files. It reuses CP136's identity and PropertyRefreshBrowserClearRequest declarations by include only. Root confirmed existing recursive CONFIGURE_DEPENDS globs register new engine/test files; no CMake or shared header was edited. Root owns grouped configure/build and validation.

## Exact private contract

PropertyBrowserClearState requires an explicitly qualified browser-private identity and a still-live CP136 clear-request invocation, with the matching slot/browser identities and parent delivery ordinal. These are declared source observations, not authenticated runtime evidence or newly allocated Qt objects. A traversal binds the same state/control block and claims that scope once. Local admission expiry, parent request expiry, a foreign state or a second traversal of that scope cannot advance it.

SnapshotPending requires a qualified owner-matching observation that native list copies returned and an explicitly known snapshot. Unknown and empty are different. Once retained, the snapshot is immutable; later current-list changes cannot replace it. No constructor-produced emptiness is assumed. Raw property pointers, including null and duplicates, remain in their original order and are never dereferenced here.

For a nonempty snapshot the helper requests index n-1, then n-2, through0, matching native13dd997/end and13df312/predecrement. Each removal request has its own invocation token and retains the owner, parent ordinal, snapshot index and exact property pointer. A separate qualified matching normal return is required before advancing. Foreign/early/repeated returns cannot consume the next request. An interrupted removal stays guarded and cannot later be relabeled a normal return in that frame.

normalRemovalReturns counts normally returned native removal invocations, not properties found/erased/destroyed. The actual13ddbc0 null/not-found branches and recursive removal callbacks are outside this model; no current-browser-list mutation or callback success is invented. Earlier callback changes may therefore coexist with later requests for pointers in the frozen snapshot.

Empty or exhausted snapshot enters **CleanupPending**, with no remaining removal request. It does not generate PropertyRefreshClearObservation, mark clear complete, perform temporary destruction, or allow CP136's payload store. Native13dd9d5/13dd9e0 and normal clear return remain separate guarded work. No Session, Qt delivery, Boolean E/OPI/Apply or public PostPending path is activated.

## Focused tests and static preflight

Twelve new Release-active Testkit cases cover unknown copy/snapshot input; empty snapshot with unchanged pending CP136 parent; exact reverse order and per-return barrier; null/duplicate entries; foreign browser-private owner; snapshot replacement; early/foreign/wrong-entry/repeated returns; interruption; parent/local scope expiry; colliding state identities and claimed-scope replay; current-list changes without snapshot replacement; exhausted cleanup boundary; and missing owner/parent/snapshot qualification.

The tests obtain their parent request through the actual isolated CP136 helper, leave its browser-clear barrier pending, and verify that traversal never stores the parent payload. They are synthetic declared observations, not an original first-Open witness.

Static preflight checked all five quoted includes against the target include tree/source directory, balanced header/cpp/test braces, twelve distinct TEST_CASE names, testkit::run_all entry, and supported CHECK/CHECK_EQ/REQUIRE only. No C assert or REQUIRE_EQ is present. API review checked token/control-block matching, snapshot retention, exact cursor bounds, one normal return per request, interrupted no-advance behavior and the absent normal-clear-completion API. This is not a compilation/test-pass claim: no build, executable/test launch, original process, GUI, mutable REA/import or shared file change occurred.

## Frozen source identities

Paths below are relative to source/apps/moproj2mst.

| File | SHA256 |
| --- | --- |
|include/m2m/engine/property_browser_clear_prefix.hpp|d7672615cb12f23d588d4ce8ed62e6dc55cc9661a0a55950b36c8ca0aaa268ea|
|src/engine/property_browser_clear_prefix.cpp|b26529d02908aa4bc146ad1399b27317876745bd4cd003a48e20c210d950b881|
|tests/mdt/test_property_browser_clear_prefix.cpp|db933f7fcad089046123c4b1b5d950f37e9de8bcacfa4d29e5c832d42fd1b8aa|

Reused CP136 header/cpp remain e2b10d1b7bba9119b9d86dc4cdc26f0a6f8d1407497aa74b306ef964800dc97e and e4b4387699afc1f0be8f36c1d275a3f660e1ba68f4c5f26b070763837ca21e18. Only the three new source/test files and this receipt were written. Source is frozen/released for root review. The next native dependency is actual owner-bound snapshot/removal/cleanup delivery; Boolean timing still needs the distinct current E/OPI/add-return frame identified by CP138.
