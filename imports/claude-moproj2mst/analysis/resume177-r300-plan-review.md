# CP177 R300 independent plan review

The HeatTreat callback cut is suitable to implement within the proposed two-file boundary. No blocking plan defect was found. This is a source-plan assessment, not implementation or runtime acceptance.

## Frozen-source correspondence

Reviewed `analysis/resume177-r300-next-cut.md` against the current CP176 HeatTreat and test files. Their SHA-256 hashes match the plan exactly: production `8dea903f2b4ba68f055d4965891a643fdfbeb31af7cfb51db174dcdf6e88e20a`; test `5b69d84666a508581a307cc28a28edbdc55f3f619b1d6ad9a2a1d305f0c24bad`. `analysis/resume176-root-integration.md` identifies the CP176 source freeze as `abb80badb264d6cc60349d147054040f1821253e6af565066887709ddd0bdaba` and the disjoint HeatTreat cut selected for CP177.

There are exactly four `ownerSimCtl` consumers: `setMode` at line 133, `getMode` at 140, `applyValue` STEPDEF at 210, and `getValue` STEPDEF at 245. The helper is at 42–47. Mode dispatch maps Deformation to `0x108`, Diffusion to `0x110`, and PhaseTransformation to `0x10c`. STEPDEF writes `sc->stpdef` (`0x1c8`) using zero -> 1 and nonzero -> 3. Four callback invocation sites correctly cover all these dispatches without expanding ownership.

## Behavior and lifetime conditions

Keep `m->simCtl()` verbatim in the replacement helper. `src/engine/gui_model.cpp:173–183` shows that it first invokes `requireGeometrySyncAvailable()`, which throws for `geometrySyncBarrier_`, before testing `step`. A callback based on direct `step` access would lose this refusal even when the step is null. The proposal explicitly preserves it.

The missing-owner or missing-project path must call the callback once with null and make no `opModel` call. Otherwise preserve one helper lookup and one guarded accessor evaluation. `src/engine/loader.cpp:168` establishes that `opModel` can initialize loader slots, consult a class model, or create the operation model; caching, prefetching, or repeating it would therefore alter observable sequencing.

The plan correctly preserves Mode setter order: conditional defaults and its owner validation, flag conversion/store, helper lookup/write, then the separate owner validation and refresh. Note that “one owner lookup” applies only to the helper invocation: `ht::propsRefresh` independently calls `opModel` and `simCtl` (`src/ops/heattreat_htwz.cpp:554` onward), and defaults can perform additional work. Do not assert one lookup for the whole setter. A missing owner may already fail during defaults; if defaults are bypassed, the flag changes before the later ownerProject refusal. The getter writes before Variant construction. STEPDEF conversion/store precedes helper lookup; its getter writes before assigning the output. These exception and side-effect boundaries are accurately retained by the plan.

An exact void callback-result constraint blocks returning the borrowed pointer through the helper result, but cannot prevent escape through captures or globals. The proposed static review is therefore necessary: invocation must be inline and synchronous, no retained callable, no pointer/reference publication, and only inline conditional word stores in all four bodies. Do not describe the void constraint alone as proving complete lifetime safety.

## Test-path assessment

The chosen `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp` already supplies `openCorpus`, `reclass`, real owner/model setup, HeatTreat schedule/default cases, and the CP176 Distortion missing-step restoration pattern. Its fixture requires `M2M_CORPUS/42_vdie/vdie.moproj`; it reclasses operation 4 rather than demonstrating an original corpus HeatTreat operation. The proposed evidence qualification is correct.

The focused case should independently dirty the SimControl word before the getter so the assertion proves the getter writes, rather than merely observing the setter result. Likewise assert both STEPDEF mappings (0 and a nonzero value), preferably with deliberately different prior control values. During the moved-step interval, compare saved-step sentinels after exercising Mode getter and STEPDEF setter/getter, and restore on every exit. This null-step test does not exercise the parked-geometry refusal; retention of `simCtl()` supplies source-level coverage of that gate, with existing barrier suites providing regression support. No additional production access seam is justified for this cut.

The proposed focused CTest list and full source-stable freeze are appropriate acceptance work. No tests or builds were run for this review. The tree suite remains a structural regression, not a callback lifetime witness.

Only this report was written. No source, build product, GUI/app-control session, original execution, live REA session, deployment, or shared coordination state was changed.
