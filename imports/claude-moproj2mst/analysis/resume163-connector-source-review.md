# CP163 connector prototype-one source review

## Decision

**Block the focused build and acceptance on the current source snapshot.** The assignment through a const prototype pointer is a definite compile error. After that correction, the private allocation representation needs to satisfy the approved owned sparse-state contract, and the reentry test needs to observe identity before and after each refusal.

This is a read-only source/test review of the implementation against `resume163-connector-prototype-next.md` and `resume163-connector-plan-review.md`. No build, test execution, source edit, GUI, or live REA operation was performed.

## Findings

1. **Build blocker — const pointer is used for mutation.** `source/apps/moproj2mst/src/doe/task_factory.cpp:117` declares `const auto* pending`; line 132 assigns to `pending->qobjectPrefix`. `retainedPrototypeAt` has a mutable overload, but the local declaration discards that mutability. The optional assignment cannot compile. Keep the pointer mutable for this construction operation.

2. **Contract blocker — the separately owned private state is only a token.** `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp:28-36` declares a `0x88` private allocation descriptor, a `shared_ptr<const unsigned char>` identity, and private address-point/write fields in the surrounding frame. `source/apps/moproj2mst/src/doe/task_factory.cpp:128` allocates one `unsigned char`, not a private-state object carrying its own observed initial address point. The approved contract calls for an actual separately owned sparse private state within the retained pending prototype. It does not require zero-filling 0x88 unknown native bytes; a dedicated sparse private object with its own identity, size descriptor, and initial address-point phase would meet the intent. The present strong `shared_ptr` can also be copied outside the store, so the token survives `releaseAdapterScope` (the test does this at lines 411-415). A nonowning identity observation would preserve adapter-scope ownership.

3. **Test gap — reentry identity is checked only after all refusals.** `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp:310-319` performs three calls, then captures `privateIdentity` and compares it to the same state without another call in between. A regression that replaces the private state on each entry could pass this test. Capture the pending node and private identity after the first refusal; check both after each later refusal, along with the phase and write markers. The focused test also lacks a controlled negative allocation-failure case from the plan; that case needs an injectable failure seam if claimed as tested.

## Accepted source behavior, conditional on those repairs

The native profile still enters `initializeNativePrefix` before requested-key lookup (`task_factory.cpp:141-147`), leaves both maps empty, and refuses before the shared QList cursor (`task_factory.cpp:99-139`). It retains one pending Report node through `TaskAllocationStore` and does not construct a `SessionTask` or TaskInfo. The frame's qualified QtCore RVAs (`0x23cac8`, `0x1a9a30`), false backlink/completion markers, and `BeforeFirstSharedQList` phase match the approved bounded prefix. Publishing the prepared frame only after host allocation succeeds avoids a half-advanced cursor in this adapter model. These are source observations, not a claim that native failure/unwind behavior has been recovered.

## Source snapshot SHA-256

| File under `source/apps/moproj2mst` | SHA-256 |
|---|---|
| `src/doe/task_factory.cpp` | `53a8507d80d36a735f6aeb6713e6d3845ad943de3ff7e76a91d6d3cd995d4ad6` |
| `src/doe/task_runtime.cpp` | `f573396d9d3a7c86ec4817efecbc375e8fbb6f325ceca4d04b272bf8ed137735` |
| `include/m2m/doe_task_runtime.hpp` | `124f48e7d2a606e55b4130c86eb645721d52b18147c041bb61050fafc4a92864` |
| `tests/mdt/test_doe_task_factory.cpp` | `10e11f62a876e3ae701c8abf74523ffffa6c175472452988616b4db51a4390cd` |

## Re-review after repair

**Pass for the three source-review findings on the repaired snapshot; the earlier block verdict applies only to the hashes above.** This is a static review, not a build or test result.

1. `task_factory.cpp:117` now uses `auto* pending`, so the optional assignment at line 132 is through the mutable `TaskAllocationStore` result. The identified const-assignment compile error is removed.
2. `doe_task_runtime.hpp:24-40` now defines `SparseQObjectPrivate` with its own `0x88` native size descriptor, QtCore private address-point RVA, and explicit write marker. The frame uniquely owns it through `std::unique_ptr`; `task_factory.cpp:128-132` creates and marks this sparse state before publishing the optional frame. `privateIdentity()` is a nonowning pointer observation, so an external strong lease no longer extends the private object's lifetime past store release. This represents observed state without inventing the unknown native bytes.
3. `test_doe_task_factory.cpp:310-324` now captures the pending node and private identity after the first refusal and checks them after each of two subsequent refusals, together with cursor and write markers. This detects reallocation or address-point reset on reentry.

The focused test still does not inject host allocation failure. That is a remaining coverage limit, not a blocker for the normal non-null prefix because source publishes the frame only after `make_unique` succeeds. No test execution or build was performed in this re-review.

### Re-reviewed SHA-256

| File under `source/apps/moproj2mst` | SHA-256 |
|---|---|
| `src/doe/task_factory.cpp` | `87cd85e2fd08ac2550f89db01ba9092bf6302bf4166597d6781debe7c1a06520` |
| `src/doe/task_runtime.cpp` | `f573396d9d3a7c86ec4817efecbc375e8fbb6f325ceca4d04b272bf8ed137735` |
| `include/m2m/doe_task_runtime.hpp` | `d708e5a3e204ea574fa775184669bb357c250b2395dfa0127bd15253717ab936` |
| `tests/mdt/test_doe_task_factory.cpp` | `e6a5eb7d43d4e31b2e58caa497457f7597f5ab65ae27afbc134661df39059c14` |

No source, test, build, GUI, or live REA state was changed by this re-review.

