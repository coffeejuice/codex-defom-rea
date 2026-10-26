# CP249: no independent operation/property/Close leaf ready

## Result

No new finite dependency-ready source increment was established in the requested operation/property/inheritance/Close scope after excluding completed CP245–247 and CP248 meshing writeback. The nearest property candidate is the HeatTreat propsFromSimCtl projection in source/apps/moproj2mst/src/ops/heattreat_htwz.cpp:536–551, but moving it behind a model-owned snapshot command would change the established guard/default/read order unless the finite operation also coordinates HeatTreat schedule defaults and property-block writes. That owner boundary is not currently defined. Do not replace its void* with a thin wrapper and call that alias closure.

The remaining candidates reconcile as follows:

- **Operation cleanup:** CP239 already migrated the four cache-reappearance predicates to Operation::hasEngineModel(). The remaining raw engineModel copy/identity observation at operation_info_cleanup.cpp:78 participates in the non-null virtual-delete refusal and same-owner resume check. Replacing it with a boolean loses the strong observed owner and owner-before identity evidence; returning another shared_ptr merely republishes the cache owner. A finite private operation/progress token protocol would need a separately reviewed Operation cache ownership API and changes to those progress types/tests. It is not a one-site boolean leaf.
- **HeatTreat property dispatch:** CP245 STEPDEF and CP246 Mode commands are completed. CP247's Forming success-order increment is completed; its unresolved manager/copy failure prefix is evidence work, not another success-path source patch.
- **Loader/inheritance:** CP244's loader audit identifies formingPostLoad, inherited object loading/copy, and equipmentSync as spanning Project, Operation, object slots and/or FileEffects. It specifically lacks an implementation-ready complete effect/failure contract. The CP242 HeatExpress primary-object command is already present. A narrow accessor here would not own the full reached effects.
- **Close/destructor:** CP228 and CP233 agree that no further Close/destructor source change is admissible without a same-owner original Open → accepted Close occurrence, including nested Save result and live owner generations. Existing guarded paths should remain guarded.

## Nearest property candidate and exact dependency

propsRefresh(Project&, HeatTreatProps&) validates its property data/block, then resolves the owner model. Only if model->simCtl() returns nonnull does it call propsFromSimCtl. That function checks the control, conditionally applies engineDefaults(p, hp.data->schedule), then writes these property block fields in order:

1. b0, b1, b2 from control words 0x108, 0x110, 0x10c;
2. stpdef from word 0x1c8 (1 maps to property value 0, otherwise 1);
3. dtmax, dtpmax1, dtpmax2, f28 from doubles 0x1d0, 0x270, 0x2a0, 0x2a8;
4. stepIncr from word 0x164.

If the control is absent, propsFromSimCtl returns false before engineDefaults and before any property-block update. If geometry is parked, simCtl() throws before that null branch. A command that first returns an optional scalar snapshot makes control-field reads happen before engineDefaults; a two-call “check then copy” performs two model admissions and adds a gap; applying defaults before asking the model changes the absent-control prefix. A command that writes the block itself would make the model layer depend on HeatTreatProps, HTPropsBlock, and schedule-default behavior. Thus a sound migration requires a reviewed finite operation that owns this precise conditional sequence and resolves the layering/type ownership, not merely an alias-free value API.

The original source attribution is retained at source/docs/re/moproj2mst/ledger.tsv:1455, source/docs/re/moproj2mst/owners.tsv:674, and source/docs/re/moproj2mst/mdt/L6/spec.md:37,110; the source annotation identifies 0x1411c0730. These are ported/decompilation qualifications, not a fresh original execution. Existing tests/mdt/test_l6_slots.cpp covers the reached HeatTreat property slot mappings and block/native state, but no dedicated propsRefresh owner-projection case was located in the reviewed test paths. A future finite-operation proposal should first add a focused Testkit seam for missing control, parked geometry, defaults-once, all mapped fields and unchanged unrelated state, while preserving the exact order above. It must not claim those tests prove native runtime behavior.

The HeatTreat files are independent from CP248's active editor/model/viewer files, but that alone does not resolve the ownership contract. If a later design selects a GuiModel snapshot method, serialize the shared engine.hpp/gui_model.cpp ownership after CP248; do not edit those files under this task.

## Audit reconciliation and limits

CP222's property/cache ledger distinguished property payload, the native GuiModel step, and Operation's shared engine cache; this selection preserves those distinctions. CP102 found no additional recovered public operation/property dispatch arm beyond the already implemented Save/reader paths and warned against inventing malformed shared-Boolean graph behavior. CP239/244/245/246/247 and CP248's reviewed contract remove or constrain the apparent smaller candidates listed above. The Close reports retain the accepted-close witness blocker. No source or original behavior is inferred from an unobserved branch.

This is a read-only candidate-selection report. No source, tests, builds, shared state, GUI, original process, or REA state was changed.

