# CP118: actual Project feature-context anchor and pending constructor phase

The actual Project now owns one passive feature-context identity tied to its existing storage epoch. The actual `initProject` path records the logical089917 constructor phase only after a prepared Mo/slot binding exists, then remains pending before the unknown8-byte proxy allocation. Existing supported adapter Open initialization/backlink behavior is preserved. No feature byte defaults, heap/provider effects, registration, accepted Close or native constructor completion are enabled.

Frozen exclusive write set (paths under `source/apps/moproj2mst`):

| File | SHA-256 |
| --- | --- |
|`include/m2m/model.hpp`|`5653c60d568ee530be41aa35813d3f87cca418f5b9c94e9f7f34cf7ddefc99c6`|
|`src/model/project.cpp`|`00778dfa9e2d05acf091aad5d459d46b7f54f11720a8cd41e6057910497e5868`|
|`src/model/project_owned_storage.cpp`|`d8a98bcba68e52c8c1197a74df4a56a43874a91594be25f7aedb3dd7f24f69ac`|
|NEW `tests/model/test_project_feature_context_anchor.cpp`|`cb84c0926c0a6ed7b3290bf5fd906793201324c37a96bd10ce37afdcfbe947ee`|

Target `m2m_model_project_feature_context_anchor`; CTest `m2m.model.project_feature_context_anchor`. The new tests/model packet requires root's configure pass to discover its directory; existing CMake test glob supplies target linkage. No CMake or other shared source was edited.

## Source contract

Root explicitly selected passive Project metadata plus an explicit pending attempt while retaining supported Open behavior. The implementation therefore separates three facts:

- C++ Project shell construction creates only an adapter anchor. It borrows the exact existing `ProjectStorageLifetime` epoch used by the Mo slots, plus a weak slot owner and identity-only Project pointer. It has no provider/container/flag/cache state and does not execute native1412f10d0 or allocate native8/48-byte objects.
- `initProject`, after its existing tokMode reset and **before its f10=5 assignment**, calls the private pending-attempt hook. Root source sequence206..208 already prepares the actual Mo before `initProject` and commits its Project backlink afterward. Admission requires actual nonnull Mo dataDb with assigned positive modelId/pathSet; the same slot pointer **and shared control block**; slot's same Project/model identity and live epoch; and a null model.moProject at first source entry. Unprepared legacy fixtures retain their existing adapter initialization and leave this anchor passive.
- Once entered, the anchor stores only the actual prepared model weak identity/control block, observed null Project backlink, and pre-write adapter f10 value as a timing observation. It reaches `ProxyAllocationPending` at source `0x140089917:0x1412f10d0`, next `0x1412f107b:8-byte proxy allocation`, with `HeapProfileUnavailable`. No successful native allocation or later constructor field write is recorded. The observed adapter f10 is not native feature data or a fabricated initial feature value.

CP114/116 establish that the native Project+60 constructor precedes later Project native fields and that its first map allocation is unresolved. The stale late “class-name registry” comment is replaced with the correct distinction between this pending feature context, later0aa960 registration, operation class registry and process remesh state. The anchor is an identity projection, not literal C++ byte-layout equivalence to native Project+60. No Qt/base/member constructor predecessor is claimed completed merely because this candidate source phase was reached.

The public read-only APIs are `Project::featureContextAnchor()` and `featureContextAttemptReport()`. Anchor stage, diagnostic identities, weak-epoch liveness, same-epoch comparison and optional timing observation are readable; callers cannot mutate or copy-construct the anchor or start/reset its source phase. All completion/admission traits are statically false: nativeConstructorCompleted, nativeHeapEffectsCommitted, nativeFeatureValuesAvailable, nativeRegistrationCompleted and completeForPublicClose. There is no map, heap callback, feature availability setter, provider deletion or Close entry point.

## Lifetime and retry invariants

The holder is Project-owned, not Session-global or Mo-slot-owned. An escaped shared diagnostic anchor holds only weak epoch/model/slot references and never retains actual Project or model storage. `Project::~Project` now invalidates the source scope before ordinary member destruction; the existing storage epoch still expires at its original later member boundary. This small destructor body performs no native feature cleanup, free, callback or global reset.

Passive shell and prepared-but-not-entered report separately as PreparedMoUnavailable versus ConstructorPhaseNotEntered. After a pending start, repeated supported adapter initialization cannot replay/rebind the occurrence or overwrite its timing/model observation. Adapter backlink commit to the same Project is allowed as existing adapter behavior; it does not change the pending stage or any native completion trait. A changed model/slot/epoch/control block reports OwnerBindingChanged. If an initialization retry observes such a changed binding, rejection is sticky even after caller restores the original pointer. Public raw fields/pre-held pointers remain externally mutable; this anchor does not intercept invisible mutations, and its permanently unavailable native heap/admission traits never authorize native continuation based on restoration.

The scalar modelId/pathSet/Kind checks qualify actual candidate prepared ownership, not native QObject factory/class/manager-current readiness. No manager or GUI owner is inferred. Native registration and accepted Close still require their actual source producers and same-owner callbacks from CP117. `releaseProjectAfterCloseEffects`, ModelManager.clear and Session/DbHandle adapter destruction remain unchanged and do not execute feature release. The feature state's unknown PST bytes remain entirely unrepresented/unknown; no zero-filled context or owning STL map is introduced.

## Eight focused Release-active tests

1. Shell construction is passive; legacy init without a prepared Mo still writes its prior adapter fields/SUB0 and does not start native context work.
2. Actual Session.prepareProjectModel yields a real registered Mo/slot identity but no source attempt. Actual initProject observes adapter f10=77 before setting it5 and before model Project backlink; the attempt stays pending through ordinary adapter backlink commit and a later retry (new f10=88 does not replace77).
3. Two live actual Projects own distinct anchors and weak storage epochs; each matches its own Mo-slot Project identity.
4. Base models and a foreign Project's adopted Mo slots cannot start this occurrence, while legacy adapter init remains supported.
5. The exact same model pointer wrapped in a different shared control block is rejected after pending start; an attempted rebind does not restart and remains rejected after restoring the original control block.
6. An intentionally early adapter backlink commit prevents later init from inventing the missed source-constructor phase.
7. Actual Session storage release expires source admission/epoch despite a strong escaped anchor and retained child/slot owners; Mo slots remain at their existing Operations boundary and manager population survives as before. Expiry does not fake constructor completion/reset.
8. Placement reconstruction at the identical C++ Project address creates a new epoch/anchor; the escaped old anchor remains invalid and cannot be confused with the new live shell.

All tests use Testkit CHECK/REQUIRE and one main, with no C assert. Missing DB/color probes from actual prepare-path calls remain below configured M2M_TEST_SCRATCH, not arbitrary C:/PROBLEM/user files. These are candidate identity/timing/lifetime tests; no actual native feature constructor/provider/Close observation is claimed. Existing historical conversion output and registered-geometry lifetime/empty-Close-tail tests remain root's required regression controls.

## Evidence, static preflight and release

Controlling CP117 plan SHA-256 `a0bcdbdfde77a93268044ebd08fb80cfbc90ac0f4e310bdfedb23699606b6a10`; CP116 owner/lifetime report `8a24844a96af8d6897c96c4720635a3a646081548b376e7890520d8486013871` and metadata `215c68d512628362459b92755baf4c71c664a78683acc55e16d605867f768a94`; CP114 constructor report `80f0f002073a09c172cde12be1df7c436135b8c6ac15ea6d56362e1e2a811504`. Original retained DEF_GUI_MO_64 v11.0.2 remains SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; no original/reconstructed instance or deployment was inspected or changed.

All quoted includes resolve against the actual app/source/Testkit include roots. The new `<optional>` dependency is solely for the Project anchor's unknown timing observation; the additional engine.hpp include supplies actual DbHandle fields to its existing storage source. Eight test cases/one main; supported macros; conservative balanced delimiter scan and zero trailing whitespace across all four files. The logical hook precedes the existing f10 assignment by static source inspection. Project copy/move remains deleted. C ABI headers/exports/version, CLI files and native byte-layout interfaces are untouched. Root must compile the new target (and the standalone tests that also compile project_owned_storage.cpp), execute focused regressions and the required full source-stable suite; no build/CTest/executable was run by this owner.

Only the three assigned existing Project files, the new focused test and this unique receipt were written. Source is frozen for root review/build. The next native-effect dependency is still the qualified actual proxy-allocation/handler profile, followed by sentinel allocation and exact failure cleanup; this anchor neither supplies those inputs nor widens public Close admission.
