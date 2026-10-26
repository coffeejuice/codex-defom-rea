# CP211 independent typed-child gate review

**PASS for the bounded source change, focused compiled validation, and full regression integration.** The required test robustness fix is verified, and root's grouped Release build, focused CTest and full source-bound regression are recorded below. Freeze remains root integration work. No production defect requiring a source revision was found. This is a review of a candidate partial class projection, not confirmation of the original failing invocation's cast or factory result.

## Review boundary and baseline

Read subtree `AGENTS.md`, `TAKEOVER.md`, `RESEARCH_CHARTER.md`, the import receipt, bounded takeover-state references, `resume211-outer-rtti-map.md`, `resume211-rtti-review.md`, and `resume211-typed-gate-implementation.md`. Compared the three assigned current files directly against the CP209 frozen `build/wave3-checkpoint209-verified/source.zip`, rather than treating Git HEAD as the checkpoint baseline. Its SHA-256 recomputed as `85f106343c802f05e5e5e69b135c18adee508916c9e35a16799d5a073dfa25c6`, matching `receipt.json`.

| File under source/ | CP209 SHA-256 | Reviewed current SHA-256 |
| --- | --- | --- |
| apps/moproj2mst/include/m2m/engine/conversion_session.hpp | bb11c4582745b706feb3f502a65940f96dae327c9e3783960d30ac618c154d93 | 035daee48869f9793106195c3ea3d28d823fda3857a1f4aab05219edd327aaf3 |
| apps/moproj2mst/src/engine/conversion_session.cpp | 4435ed3f888560fa9829616dbf912b8e20d10d6fd8d5e77846f61b72efbceb3b | ae13153d37a482258baae1b8deb491e34adaacca3f5f5a2786c5113bbfa36590 |
| apps/moproj2mst/tests/mdt/test_conversion_session.cpp | a06de6ae89b0c12c703c2b808acea0a16430a8f9e7e3b9be649f2667aa966e0b | 4e077bdad47fcdd9441ad2537bbb21dabafa021e84585830815e047408471a19 |

The actual CP209-to-current changes are the internal helper declaration/definition, the gate/refusal at publication, and one 27-line test. The current-project resolver tests visible in the Git diff already exist in CP209 and are outside this change.

## Production checks

- **Placement:** `prepareProjectModel` still constructs the Mo handle, adopts slots, sets its path, appends it, and registers its signals before calling `outerLoaderMoChild`. The refusal precedes assigning `p.dataDb`. In `src/core/convert.cpp:212-214`, prepare returns before `initProject`, followed by the separate construction commit. This matches the candidate seam justified by the CP211 source/review; it does not claim native instruction timing equivalence.
- **Decision:** the helper reads only `DbHandle::kind`, confirmed `const` in `include/m2m/engine.hpp:108`. Short-circuit evaluation avoids dereferencing null; null/Base return null, Mo returns exactly the input pointer. Backlinks, read/path state, IDs, selection and slots do not influence the result. `noexcept` is appropriate for this operation.
- **Refusal:** unexpected mismatch calls the existing `[[noreturn]] unsupported` helper, which throws `Error(M2M_E_UNSUPPORTED, ...)`. It does not publish a null successful child or emulate an original allocation/cast failure. The existing factory always constructs `Kind::Mo`; no new public failure injector or native-success flag was introduced. Allocation/registration exceptions retain their prior behavior. The later ownership/backlink invariant remains separate and unchanged.
- **Ownership:** the raw pointer is a temporary borrowed discriminator; no new owner, deletion, allocation, or backlink is introduced. On success the same local shared owner is moved into `p.dataDb`, while the manager retains its existing shared owner and signal senders remain weak. No source-level rollback of already performed registration is added on refusal. Under the current immutable-Mo construction path that refusal is unreachable absent a programming change or invalid object state.
- **ABI:** the patch adds an internal C++ function declaration and a forward declaration. It changes no public C header, exported `m2m_*` signature, enum value, object layout, or calling convention. The native RTTI descriptors and ownership model are not presented as fully reconstructed by this helper.

## Test meaningfulness and limits

The new test exercises null rejection, Base rejection despite a populated Project backlink, and identical-pointer Mo acceptance with no backlink and unset load/path state. Its session checks establish a published Mo owner with registrations present, no backlink, and no selected model, then verify the later backlink commit. It uses existing resolvable includes, actual supported Testkit `CHECK`/`REQUIRE` macros, and the existing `testkit::run_all()` entry point; no Release-disabled C `assert` was added. The path probe remains below `C:/PROBLEM` and the operation exercised only sets the path.

Test robustness disposition and remaining evidence qualification:

1. **Required fix completed:** the new test's connection-count assertion at line 125 now uses `REQUIRE` before indexing `[0]`. Thus zero connections produce a controlled test abort. Independently reversing only this exact assertion text reproduces the previously reviewed test hash `9a4f8b402ad5d09950ac302c05198b346ef83fed047c1e93cde1510e62c45433`; both production-file hashes remain unchanged. This confirms the follow-up is exactly the accepted one-line change.
2. The new test does not call `initProject` before its construction commit. Its last assertion therefore establishes the backlink commit, not Project initialization. The adjacent existing test does call `initProject` in the production order. Either insert that call in this test or keep the report's claim explicitly limited to the backlink commit.

The test observes state after `prepareProjectModel` returns; it does not instrument the instant of the gate, and would not independently detect every reordering or deletion of the gate. The source comparison establishes exact placement, while the direct helper cases establish its decision contract. No failed-gate session continuation is exercised because the current construction path is always Mo and has no internal injection seam. That limitation does not justify adding a public injector or claiming original failure reachability.

## Focused validation receipt

Read root's `analysis/cp211-typed-gate-fix-build.json` and `build/logs/cp211-typed-gate-fix-focused-ctest-01-03.log`. Release configuration, focused target build, focused CTest and grouped full build each exited zero. Focused `m2m.mdt.conversion_session` passed 1/1 CTest target (0.05 seconds test time). The receipt binds unchanged before/after source-tree hash `1c39d6d64c766500f332f15f20905d231f1e72aabc31919f63b44bffc66f756a` across those steps. The initial missing-header build failure remains a recorded failed attempt; this later environment-corrected run supplies the passing evidence.

Independently read the completed full regression receipt `build/test-runs/1793017504384316500/receipt.json`: 361 registered, 354 clean passes, six passes with logged gaps, one pass with logged differences, and zero failed, skipped or incomplete tests. Fixture changes and unreported tests are empty; CTest and launcher exit codes are zero. The before/after source-tree hash is the same `1c39d6d64c766500f332f15f20905d231f1e72aabc31919f63b44bffc66f756a`, and execution binding reports stability. Root reports the seven diagnostic outcomes unchanged from CP209. This verifies integration against the existing qualified regression baseline; the logged gaps and difference remain, and do not establish full original fidelity.

No build, test execution, original process, GUI action, or deployment was performed by the reviewer. Root owns build execution and integration validation. The CP211 RTTI evidence supports the fixed-image class mapping only; native factory/constructor side effects and the original null result's cause remain unresolved.
