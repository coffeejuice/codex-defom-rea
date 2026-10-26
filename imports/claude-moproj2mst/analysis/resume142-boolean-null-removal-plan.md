# CP142 plan: classify the null removal leaf without completing a callback

Implement a narrow private projection of native1413ddbc0's argument branch. Its result states which source branch follows explicitly admitted arguments; it must not claim a native normal return, advance CP139's cursor, finish browser clear, store CP136's payload or admit Boolean Apply. This checkpoint writes only this plan; no implementation is authorized yet.

## Source qualification and bound

CP141 inspected1413ddbc0..1413ddc95, code-byte SHA2560a400be5ebde58fc8aedb1cfef1a556de883a7886d5e06a19d984fbdddb33a92. Property argument RDX is tested at13ddbd7. Null takes13ddbdf ->13ddc90 ->RET13ddc94 with no call and no browser dereference. Non-null first reaches the top-level-list copy at13ddbee. The projection stops before that copy; no property search, list erase, browser callback, allocator or temporary cleanup is implemented.

The admitted CP139 source call is13dd9c9 with normal caller continuation13dd9ce. These static addresses identify the branch; they are not evidence of a reached native call. Existing CP141 report SHA256206b5b4e18362b346a5a23cc7444ff0308ca646d8c3b51212759773b79e054e7 remains controlling.

## Proposed API and admission

Use one function, tentatively projectPropertyBrowserNullRemoval, taking the **existing traversal object**, its **existing PropertyBrowserClearState shared owner**, and an explicit removal-entry observation. It returns a new report type. Do not add another queue, owner registry, scheduler or mutable browser model.

The removal-entry observation contains:

- Whether entry at native13ddbc0 was actually observed, plus nonempty qualification. Synthetic tests explicitly label their declared observation instead of calling it original evidence.
- The correlated PropertyBrowserRemovalRequest, including exact owner mapping, parent delivery ordinal, snapshot index, property pointer and invocation weak token.
- Optional receiver/property argument values and optional caller-return RVA. Unknown values stay unknown; zero property is represented by an engaged value0. The admitted caller return must be13dd9ce, receiver argument must equal the request's browser B, and property argument must equal the outstanding request's pointer.

First require the actual traversal report to be RemovalPending with a current removal request. A snapshot request, interrupted removal, empty/exhausted CleanupPending or missing owner remains guarded. Revalidate the existing owner/control block and CP136 parent-request lifetime through traversal.resume(owner, empty snapshot observation, unavailable removal return). In this already-pending state that existing operation validates admission and returns without advancing the cursor or running callbacks. This avoids editing CP139/shared headers to expose private liveness fields. A foreign/released owner is rejected by the existing guard. Do not call it on a not-yet-bound traversal, because that would claim an admission scope while merely projecting a branch.

Then require the entry request to match the outstanding owner/ordinal/index/property and the same still-live invocation pointer **and shared control block**. Numerically identical fields cannot substitute for the token. A stale request whose removal already returned is not current. Missing entry/argument/caller qualification yields AdmissionPending; explicit mismatches reject with M2M_E_UNSUPPORTED before producing a branch result. The capture layer remains responsible for authenticating evidence/module identities; this helper does not authenticate a string or observe native execution.

The known-null branch can be classified only after all admission checks. Its lack of browser dereference is a source fact; validating the surrounding browser-owner request is a diagnostic scope rule, not a fabricated browser read by the leaf. Known non-null arguments stop at the guarded list-copy frontier.

## Result and invariants

Proposed stages are AdmissionPending, NullArgumentLeaf and NonNullContinuationPending. The latter identifies13ddbee as unresolved. NullArgumentLeaf records only the call-free source route to13ddc90/94. Do not add completed, normalReturnObserved, clearComplete or Boolean readiness flags.

The result has **no conversion, accessor or factory producing PropertyBrowserRemovalObservation::Returned or PropertyRefreshClearObservation**. It contains no CP139 invocation-completion object. An actual separately admitted normal removal return remains necessary to advance CP139; final list cleanup/normal clear return remain necessary for CP136. Null classification alone does not cross either barrier.

Repeated projection is read-only with respect to snapshot/cursor, normalRemovalReturns, pending request token and CP136 receiver properties. It must not erase, deduplicate, consume a null snapshot entry, release admission or run Qt. No public Session/Open/Save/PostPending integration is proposed.

## Focused tests after authorization

Use Testkit with the existing CP136 ->CP139 synthetic fixture chain, obtaining a real outstanding diagnostic removal request rather than manufacturing an unrelated token. Planned checks:

1. No pending request, unobserved entry, unknown receiver/property/caller and empty qualification stay guarded rather than becoming null.
2. An admitted request whose exact property is0 produces only NullArgumentLeaf; cursor/count/token/parent payload remain unchanged across repeated projection.
3. An admitted non-null request produces NonNullContinuationPending at13ddbee, without list copying or callbacks.
4. Wrong browser/lifetime, parent ordinal, snapshot index, property or caller-return RVA is rejected.
5. Colliding numeric identities from another frame, foreign/missing invocation control block and expired parent/local admission are rejected.
6. After an actual separately declared return advances CP139, the old entry/request is stale; interruption and CleanupPending cannot be classified as a pending removal.
7. A separate actual return observation can still advance CP139 normally; the projection itself never does. Even an exhausted traversal leaves CP136 BrowserClearPending.
8. Compile-time non-convertibility checks keep the projection report distinct from both completion-observation types; runtime cases use CHECK/CHECK_EQ/REQUIRE, with no C assert.

Tests describe synthetic source-stage observations, not successful original capture or first-Open timing. Root will own actual build/test validation after source release.

## Exclusive write set proposed for the implementation checkpoint

All are new and were verified absent:

- source/apps/moproj2mst/include/m2m/engine/property_browser_null_removal.hpp
- source/apps/moproj2mst/src/engine/property_browser_null_removal.cpp
- source/apps/moproj2mst/tests/mdt/test_property_browser_null_removal.cpp
- A separate new implementation receipt assigned at release.

Only read existing property_browser_clear_prefix.hpp/.cpp and property_refresh_prefix.hpp/.cpp. Their interfaces need no change; CP139 header currently hashes d7672615cb12f23d588d4ce8ed62e6dc55cc9661a0a55950b36c8ca0aaa268ea. Existing recursive configure-dependent globs should register the new files, with root verifying that during grouped configure. No CMake/shared/source edits, code files, test execution, GUI/app control, original process or mutable REA activity occurred in this planning checkpoint. Actual Boolean E/OPI timing remains the separate missing witness in CP141.
