# Checkpoint 46 graph-removal test design

## Decision

No isolated helper test can currently exercise the source-qualified native shifted-index behavior without first adding production ownership/interface code. The existing implementation has no removal helper corresponding to native `c15db0`, and `PostGraphSceneState::removeGraphItems` deliberately refuses non-text graph items before it reaches any name-based manager-list mutation. A new test that reimplements the loop locally would test the test's own model, not the converter. No production or test source edit is justified at this point.

## Source and evidence trace

- `analysis/wave3-close-graph-manager-cleanup-contract.json` is bounded offline PE/DUMPBIN evidence against original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Under `take_at_and_delete_order`, it records `c170b0` unlinking the list entry while returning a still-guarded graph, then `c15db0` releasing the temporary guard before deleting the pointee. It separately records that the numeric index increments even after removal. This establishes the static algorithm for the captured bounded code, not full runtime consequences: populated destructor callbacks and reentrant effects remain qualified.
- Current `source/apps/moproj2mst/src/engine/close_effects.cpp` has `PostGraphManagerState::~PostGraphManagerState` reset its factory and clear `graphItems_`; that vector has no supported insertion/removal path here. `PostGraphSceneState::removeGraphItems` skips qualified `SceneTextItemState` values and throws for other items as unrecovered concrete graph classification/deletion. There is no native name matching or loop index in this source path.
- Existing `source/apps/moproj2mst/tests/mdt/test_close_effects.cpp` covers receiver dispatch and refusal/empty-population boundaries, not populated graph ownership or list mutation. No `tests/mdt/test_graph_list.cpp` exists. `source/apps/moproj2mst/CMakeLists.txt` globbing is configured for new tests, so a focused file can be added later without changing the shared build interface, once it can call a real production helper.

## Smallest valid future regression

After the production list representation and helper are independently recovered, put the focused test in `tests/mdt/test_graph_list.cpp` and exercise that helper with three live named entries `[A, B, C]`, selecting only `B`. Assert the precise native order of unlink, guard lifetime, destructor call, and loop-index advance; do not encode an expected final membership by intuition alone. First derive the exact expected sequence from the original bounded implementation and determine whether graph destructor reentrancy changes it. Keep this as a helper-level unit test until actual scene graph construction/registration and an original-first runtime capture become available.

The present three-item example is a test-shape proposal only. The static contract says the index advances after a removal; it does not by itself establish that `B` is the only match, that there is a second matching item, or the observable final list for arbitrary destructor callbacks. A future test must therefore capture the relevant native inputs and callbacks before asserting a particular survivor set.
