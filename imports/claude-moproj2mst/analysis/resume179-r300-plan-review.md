# CP179 R300 plan review: Diestress KEY Save step borrow

## Decision

**Accept the proposed one-function boundary and the stated behavioral constraints. The production change and focused null-step test are already present in the current source tree.** The `optimization/test-speed` snapshot has the exact production baseline hash named in the plan, and still has the original raw step alias. The active source has converted that alias to an immediately invoked callback. Do not repeat the production edit. The test-source hash in the plan does not match either inspected snapshot, so its claimed paired baseline hash needs reconciliation before treating it as a frozen CP178 test baseline.

No tests, builds, GUI/app-control, live REA, or shared state were used for this read-only review.

## Evidence

- Frozen comparison source `optimization/test-speed/source/apps/moproj2mst/src/engine/save_l6.cpp` is SHA-256 `900a1e46a36a3925443b2d5937a234686b6708fe0aa54a066a159366c7da266e`, matching the plan. Its `diestressSaveKey` performs one `opModel(p, op)`, returns if null, computes the active subproject DB path and `.KEY` suffix, checks `m->step`, then holds `DB_STEP_DATA* step` across `captureKey` and its writer lambda.
- Current `source/apps/moproj2mst/src/engine/save_l6.cpp` is SHA-256 `3d0d2bc3c5ffc9e86b9504d46f3c9cd8273dc74a711c1c3c4e629c060410f9e4`. It invokes a local `void` lambda with `m->step.get()` at the same point; null returns before `captureKey`. The writer calls `write_key_file(step, ...)` inline. There remains one model lookup and one step acquisition. Neither callback nor pointer is stored, returned, or queued.
- The path calculation remains before the step-null check, preserving the prior effect/exception order. The code reads the public `step` field directly and adds no geometry synchronization guard, so it avoids the additional refusal behavior of `GuiModel::simCtl()` / `GuiModelAccess::withStep`.
- `optimization/test-speed/source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` is SHA-256 `9b9101aa1ee7499119818b41313e91f512d3e710c46336f237707c07e77f173b`; current source test file is `a9d53b00ec11a9d17a883c7ded8f541df9d373c41f6924496a0136af514ee262f`. Neither equals the plan's claimed `9e1f3f5d...`. The current test file already includes `diestress_save_writes_db_key_only` and `diestress_direct_save_null_step_has_no_effects_then_restores_writer`.
- The null-step fixture loads a copied-corpus Diestress owner, verifies the cached model and step, temporarily moves the step out around the direct helper call, restores it on normal and exceptional exits, checks writes/deletes/mkdirs are empty, and verifies the restored normal direct Save emits one nonempty expected `.KEY` write. It does not establish a public Save route with a null step, consistent with the plan's evidence limit.
- `include/moproj2mst.h` has SHA-256 `34e55cdd58280b056d21e94986c11ac32ccea6f9a6c956d5dad32dbb9864fc40`, matching the plan. The described change is confined to an internal implementation and test; no exported declaration, model layout, or C ABI change is indicated.

## Acceptance and remaining work

Static review finds no production behavior blocker. The planned order, null-before-capture behavior, inline write, and no-geometry-guard constraint are satisfied in the current source. The null test and positive KEY test are present. The source-stable CTest set and complete suite remain unverified here; this review makes no claim that they pass.

Before implementation/freeze bookkeeping, reconcile the test baseline hash: the plan's named value does not match either the CP178-like `optimization/test-speed` snapshot or current test file. This does not require re-adding the test, which is already present, but affects the plan's rollback/frozen-owner hash assertion. No C ABI change is visible or required.
