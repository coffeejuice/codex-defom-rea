# CP174 independent source review

## Decision

**Accepted for source validation; no source blocker found in the released snapshot.** This is a static review of the factory, sparse header and authored tests against `resume174-connector-next.md` and `resume174-connector-plan-review.md`. It does not establish compilation, executed tests, a frozen candidate or original-comparison acceptance. The implementation report says the parent supplied CP173 frozen/passed status; that prerequisite was not independently rerun here.

## Contract and implementation

`doe_task_runtime.hpp:35-37` supplies a distinct weak private `+0x70` binding and initially false second-reference marker. It does not add a root QList record. `task_factory.cpp:314-334` admits only `BeforeSecondQListBinding`, uses the same `qualified->list` whose pointee and control block match private `+0x18`, and writes only the second weak binding and `BeforeSecondQListIncrement` cursor. The unconditional refusal then stops at RVA `0x1311de`, before the second atomic. There is no CP174 reference-commit call or root delta mutation.

The shared qualifier (`task_factory.cpp:60-83`) checks the retained Report ordinal/pointer, allocation-time private identity, task/private sizes, written address points, first QList/QString root identity and committed markers, canonical module/RVA qualification, incomplete QObject/base and unavailable backlinks. CP174 additionally checks the root-qualified committed QVector, its optional RVA qualification and all four zero observations. `TaskAllocationStore::ownsWidgetRuntime` requires both ownership identity and pointee equality, so either aliasing direction is rejected.

The second-binding admission compares ownership against an empty weak pointer in both directions. An expired weak pointer retains a control block and therefore refuses; `.expired()` alone would incorrectly accept that state. A genuinely empty weak pointer passes. The second marker must be false and is never set here.

The plan's malformed optional QList qualification case is inapplicable to the released type: QList module SHA and RVA are compile-time constants and there is no mutable optional override. The implementation checks the actual canonical fields. This is a justified representation adjustment, not a missing runtime check. The malformed optional QVector qualifier remains checked.

## Authored test assessment

The CP174 matrix at `test_doe_task_factory.cpp:1914` rewinds to the precise CP173 predecessor, clears the second binding/marker and exercises missing/expired/foreign/wrong-owner prior bindings, uncommitted markers, expired root, malformed QVector qualifier, premature live and expired second binding, premature second marker, bad identity/size/address/phase/zeros and completion/backlinks. It snapshots second-binding ownership and pointee, phase, fields, identities, markers, three deltas, observations, counts and cache sizes around refusal. The replaced-private fixture snapshots through the replacement rather than dereferencing the destroyed original afterward.

The actual `ConversionSession` positive case at line 2078 replays CP174 with independent QList delta 17 and supplied absolute observation 42 at delta 11. It verifies pointer and control-block identity with the first binding, unchanged deltas/observation/history, false second marker, exact new phase, retained identities and maps/counts, and same-key plus different-key reentry. The unpaired-root test at line 2137 covers absent pairing, foreign root and both aliasing directions at an independently supplied retained predecessor.

Earlier-cut fixtures inspected in the test source clear the new second binding/marker before replay. The CP173 wraparound/history test still requires its single QVector delta change and follows through CP174 without a second QList change. Tests express the required behavior; execution remains the parent's validation gate.

## SHA-256 receipts

Paths are relative to this converter checkout. The three released source/test digests match the implementation release report.

| File | SHA-256 |
|---|---|
| `analysis/resume174-connector-next.md` | `0c043931e20ca42f950437a5bac425d5b967524642be9ad3b9ebb7f7900cef56` |
| `analysis/resume174-connector-plan-review.md` | `0d458f39eca04b2c7350b98f76146e31ec71c01d5db8b65fa54e22b4d960e813` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `7703139852f2eec98d7da3485ffa0ea2df4b27047876ea3d56c9a9697110a7bb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `3d9cf855f0c06db319657b682b3442def787ffbedbaf39caf9354892355994a0` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `e21a97c15fefc135b0e498272fd4b3e9beb8c476d019bd33943e97552eff1cc0` |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp` | `51849bde4e923f2af7eaa7c5dd8d758dde4d27e32c97b3c210be621cefa4dc7f` |

Only this review report was written. No build, tests, GUI control, live REA, deployment, source edit or shared recovery-state change occurred.
