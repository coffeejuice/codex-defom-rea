# CP109 terminal failure matrix: harness/protocol review

No new supervisor or protocol implementation support is required for the proposed worker cases19/20/23/24. The current role validator already permits all four under RemeshConversion+RemeshV2. The collector accepts legal earlier-writer false values, failure before Boundary and failure after Boundary; the supervisor's sticky backend-failure path stops only its owned child while retaining the prepared fixture lease through inspection. This is a review of the planned candidate cases, not runtime acceptance.

## Expected matrix

| Case | Actual candidate edge | Wire/result contract |
| --- | --- | --- |
|19 ConversionDeniedProjectFalse|Delegated project device declares error7 after its real close; MST returns before denied remesh open|Started+Boundary; project attempted/returned true, succeeded false; MST attempted/returned/succeeded true; natural childC0000417; no supervisor stop/Returned|
|20 ConversionDeniedMstFalse|Late project-aboutToClose callback seeds and read-locks the main MST before write-open|Started+Boundary; project A/R/succeeded true; MST A/R true, succeeded false; natural childC0000417; no supervisor stop/Returned|
|23 ConversionPublicationFailure|Actual dispatch intentionally returns PublicationFailed before sending Boundary; failClosed sends tag1 and writer parks|Started+BackendFailure1, two frames; no wire snapshot/Boundary/Returned; sticky BackendFailure, own supervisor stopE1050001, HarnessFailure|
|24 ConversionEndpointReturned|Actual dispatch publishes Boundary then intentionally returns EndpointReturned; failClosed sends tag2 and writer parks|Started+Boundary+BackendFailure2, three frames; prior all-success snapshot retained; sticky BackendFailure, own supervisor stopE1050001, HarnessFailure|

Cases19/20 retain opened/validated, checkedSubprojects1, raw answerNo10000h and pendingDefaultNoButton0 for the selected fixture. Only the respective succeeded bit changes; attempted/returned must not be cleared or normalized. Code7 is the declared project QFile error in19, not an adapter return or process status. Terminal dispatch dominates earlier returned writer false values before the adapter mapping. Case20 must compare the deliberately retained MST sentinel instead of the normal MST oracle; unaffected pre-MST prefix files remain independently compared with their exact raw expectations.

Case23's published failure is a deliberate backend result, not proof of a physical pipe write failure. Its actual live dispatch/failClosed Save state must be retained privately because the valid failure frame contains no snapshot. Do not send a fabricated Boundary to recover that data. Case24 must preserve the already captured Boundary snapshot through failure and stop, never replace it with empty failure metadata. The ledger/counter sequence must account for successive append writes; reading the first repeated failure counter would misstate the final failClosed call. Post-call adapter/unwind markers stay absent, with helper-scope versus Session/normal-Close qualifications intact.

## Existing support checked

The current v2 collector requires started/nonce/site/version/checksum, a complete legal snapshot for Boundary, known profile/proof, EINVAL22 and ordinal1. It deliberately permits succeeded=false when A/R are true. BackendFailure is permitted before or after Boundary, maps explicit wire tags1..3, rejects further outcomes and leaves an existing optional snapshot intact. classify checks backend/supervisor failure before any terminal-status path. V2 returned adapter0..7 remains separate from worker OS exit0 after the CP108 correction; v1 retains exit/payload equality.

The supervisor consumes the completed failure frame, selects StopReason::BackendFailure and requests owned-job stop with E1050001. Inspection runs only after confirmed stop, with the parent lease still held. If failure publication is unavailable, the external deadline remains a failure rather than terminal success. Unknown/custom handler and public C ABI behavior are unrelated guarded routes and must stay unchanged.

## Bounded test increment

The worker owner supplies the actual conversion/partial-output matrix. This harness lane will add only two pure protocol regressions to `tests/mdt/test_terminal_harness_remesh_v2.cpp`: (1) each earlier-writer false snapshot remains legal and is preserved after tag2, with sticky failure outranking a terminal-looking status; (2) tag1 before Boundary is legal, carries no snapshot and cannot be followed by Returned success. These cover distinct protocol states not explicitly pinned by the existing matrix. No production implementation/header/supervisor fix or role expansion is needed, and no worker/core source will be edited.

Required root validation remains focused v1 safety/v2 protocol/role/actual worker targets, required regressions, complete source-stable launcher and freeze. All terminal claims remain candidate-fixed-endpoint; native-default-handler-prelude-unimplemented. No fresh original failure, full accepted Close, C ABI terminal behavior or complete lifecycle parity is established by these cases.

This plan was written before source changes. It used the CP108 frozen worker archive and current released protocol/supervisor plus the worker owner's CP109 case contract; mutable worker implementation is not treated as accepted source. No build/test, GUI, original/native target execution or REA mutation occurred. Source-test receipt/hash will be appended after static release.

## Static implementation release

The planned two pure protocol regressions are now implemented in `source/apps/moproj2mst/tests/mdt/test_terminal_harness_remesh_v2.cpp`. The target remains `m2m_mdt_terminal_harness_remesh_v2` / CTest `m2m.mdt.terminal_harness_remesh_v2`, now10 cases. No child mode/process count was added. One case checks each earlier writer false bit through a valid Boundary and subsequent EndpointReturned failure, preserving the exact prior snapshot and sticky classification. The other checks PublicationFailed before Boundary with no snapshot, two valid records and rejection of any following Returned success.

Source SHA-256: `e205a034e9729da69cfda62d9856ee66107505f9a2b399515e32f373e2a4df0a`. Existing protocol.cpp remains `1d35912181a38ed2f08be1fb4f9736bc84551d42799a791fd9fd3ef20c481db2`; no protocol/header/supervisor/core/worker implementation was changed in this lane. Read-only Testkit source preflight passed with no issues; supported macros/includes/main and whitespace checked. No build, tests, child execution, GUI, original/native target or REA activity occurred. This source is frozen for root focused/full validation and retains the earlier stated qualification limits.
