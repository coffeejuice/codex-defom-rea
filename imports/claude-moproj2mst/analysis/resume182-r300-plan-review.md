# CP182 R300 plan independent review

The proposed local callback cut is acceptable against frozen CP181 source. No blocking plan defect was found. This is approval of the proposed transformation and its evidence limits, not acceptance of an implementation or a test run.

Reviewed `analysis/resume182-r300-next-cut.md` against `save_l3.cpp` SHA-256 `1a7c6de08abcb97906518549dbdaf0dc1568d444c2167dc70eb69a2f1f1befe0` and `test_engine_l3.cpp` SHA-256 `a9a151d214960df4bfeeb4153ce673651642c3553b3290dfe8cbba0086f8931b`. Both match the plan's pre-cut hashes.

## Required implementation acceptance

- Preserve the empty output vector followed by exactly one `opModel(p, op)` call (`save_l3.cpp:89-90`). Invoke the void callback with `m ? m->step.get() : nullptr` at the old null-step admission point. A null callback argument returns before dereferencing `m`, clearing a list, counting objects or reading props.
- Preserve `inter_obj.clear()` before `objectCount()` (`save_l3.cpp:93-94`). Keep the 1-through-n traversal, nullable object check, kind-7 filter, then dimension-3/object-1 filter in the existing order. Do not cache dimension ahead of the loop or move property evaluation ahead of the filters.
- Preserve a fresh `ContactPair` for each admitted object, master i, slave 1, lub 0, friction then IHTC evaluation per pair, default f19=1, and push order. `ContactPair` is scalar-only (`engine_internal.hpp:129`); the callback must not retain or export the step, object or nested-list pointer through another capture, queue or output.
- Keep direct nullable step acquisition. `withOpStep` performs its own model lookup (`engine_internal.hpp:68`); `GuiModelAccess::withStep` calls `requireGeometrySyncAvailable()` before acquisition (`gui_model_access.hpp:24`). Either substitution would depart from this plan's intended lookup/guard behavior.

## Test and claim acceptance

The existing `air_convect_2d_active` fixture opens operation 1 and explicitly checks `SfOprHeatTransferExpress2D` (`test_engine_l3.cpp:70-100`). It is suitable for a direct helper mechanics test after requiring the real step, first eligible object, valid common props and 2D dimension. Check all pair scalar fields, including friction/IHTC and f19, not only pair count. Preserve and restore the actual moved step on both ordinary and exceptional exits; verify the returned null-step vector, saved list count, model identity and restored step identity. Any fixture-created nonzero inter-object content would strengthen the clear assertion; a zero-count postcondition alone does not prove that clear ran. Clear-before-count and property-call order remain static review obligations.

`classDefaultPairs` dispatches this helper only for `SfOprFormingExpress` and `SfOprFormingExpress2D` (`save_current.cpp:438-446`). The HeatTransferExpress direct test therefore cannot establish reached FormingExpress public Save behavior or public null-step Save admission. The plan explicitly makes this distinction correctly. The FileEffects check is a local regression observation, not a general proof of absence of all filesystem effects.

Before freezing, review the actual paired-file diff and exact built hashes, confirm the new assertions executed, run the five named focused CTests and the complete source-stable suite, and retain test failures if any. No barrier, cache1, public API closure, same-occurrence reader-tail or full application-completion claim follows from this cut.

## Scope receipt

Only this review report was written. No production source, tests, build, GUI/app-control, original execution, live REA, shared coordination state or deployment was changed. No reconstructed DEF_PRE_64 instance was inspected or used.
