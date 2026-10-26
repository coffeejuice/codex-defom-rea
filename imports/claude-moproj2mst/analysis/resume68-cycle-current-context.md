# Checkpoint 68: Cycle nested writer current context

## Finding

For the retained fresh-open Cycle case, the reachable native call chain makes the first member the expected current item before the Cycle save path. The Cycle writer itself does not rebind member `curItem` (native O-3), but the nested member writer receives that member explicitly, and the candidate's Cycle override assigns it again. For the exact retained case, this is a redundant assignment on the source-derived fresh-open path, not evidence that the native writer starts with a stale item. This does not establish the value for reused/pre-populated engine models or other cycles.

## Native call chain and entry value

The native save writer dispatcher documented at `source/docs/re/moproj2mst/s1-04/spec.md` §0.2 binds `self->curItem = item` around a top-level writer call. The same spec's Cycle body (`0x14134dcc0`, §2) enumerates member IDs and invokes each member class's `vt+0x280` with `it` as the explicit item argument. It explicitly records that Cycle does not set `curItem` for this nested call (O-3).

For the retained `51_MO2_Operation_Cycles2` branch, the active item is Cycle operation 3 and its sole member is Forming2D operation 2. The existing parser/loader test documents the Cycle's active selection and first-member model relationship in `source/apps/moproj2mst/tests/mdt/test_l7_load.cpp`; the Cycle spec independently describes the corpus fixture as count 9, one member. In the native model lookup, Cycle `vt+0xe0` (`0x14134daf0`) asks for the first member's operation model (`0x14005f820`). The spec's vtable inventory in §8 says operation-model lookup may create the model and sets the handler's `curItem`. The candidate mapping is visible in `source/apps/moproj2mst/src/ops/cycle.cpp::CycleClass::classModel` and `source/apps/moproj2mst/src/engine/loader.cpp::opModel`: when the member model is absent, `opModel` binds the member class's `current` to that member immediately before creating the model.

On a fresh open where that member model is absent, the traced sequence is therefore: open active Cycle → Cycle model lookup → first-member model lookup/creation, binding Forming2D current to operation 2 → Cycle load and later save → nested Cycle dispatch with explicit item operation 2 and no native Cycle rebind. This is the best source-supported answer for the current value at entry: operation 2, assuming the fresh-model path. A retained runtime capture of the original member-class field at this exact call boundary is not present, so the report does not elevate that static reconstruction into a fresh original runtime observation.

## Does the nested writer read current?

For this fixture, the member writer is Forming2D's inherited base writer (`0x1405f5340`). `source/apps/moproj2mst/src/ops/base.cpp::OpClass::writeMst` takes the explicit `Operation* op`, passes it to the emission helpers, and does not read `current`. The base property fallback (`OpClass::propsOf`) reads `current` only when its `op` argument is null; the writer's path passes the explicit member. Thus the nested Forming2D writer does not consume the current binding in this fixture's reachable source path.

The general O-3 mismatch remains meaningful for other members. The native spec notes current-sensitive writers such as Convert2Dto3D's task-file path and Distortion's SIMNAM generation; those are not the member class in the retained 51 case. No claim is made that all possible Cycle members ignore `current`.

## Candidate delta decision

Keep the candidate behavior unchanged pending broader evidence. In `CycleClass::writeMst`, candidate code snapshots the member class binding, assigns `it`, calls the member writer with `it`, then restores the snapshot on normal return. For the fresh 51 case, the source-derived pre-entry value is already operation 2 and the Forming2D writer consumes the explicit argument, so the assignment does not change the observed binding or writer output. Removing it now would create a candidate-only behavior change for other current-sensitive member writers without a qualified native entry state.

A useful next candidate check is to activate `51_MO2_Operation_Cycles2` through the actual `M2M_FLAG_WRITE_SAVE` ConversionSession route and record the Forming2D class binding immediately before Cycle's assignment and in the nested writer. This can confirm the candidate realizes the source-derived fresh-open path. Original parity still requires observing the native class-current field at the same boundary; the static evidence above does not replace that capture.

## Scope and limitations

- Existing `cycle51_save_writes_no_task_files` uses a direct `Project`/`Ctx` helper and validates Cycle save effects, not the full ConversionSession route.
- Existing `cycle48_model_is_first_member` checks that Cycle's model is the first member's model, but it does not assert the class-current binding at nested writer entry.
- The retained Oracle B Cycle output is output evidence, not a current-field trace.
- Pre-existing engine models, prior UI selection changes, repeated operations, and Cycle members other than Forming2D can produce different current histories or current-sensitive results.
- No source, build, test, GUI, fixture, or mutable REA database was changed or run for this read-only audit.

## Evidence

- Native Cycle loop and explicit member argument: `source/docs/re/moproj2mst/s1-04/spec.md`, §2 (`0x14134dcc0`), O-3.
- Native model-creation/current-binding inventory: same spec, §8, operation-model row.
- Candidate Cycle first-member model resolver and writer binding: `source/apps/moproj2mst/src/ops/cycle.cpp`, `CycleClass::classModel` and `CycleClass::writeMst`.
- Candidate model current assignment: `source/apps/moproj2mst/src/engine/loader.cpp`, `opModel`.
- Explicit-argument writer and null-only current fallback: `source/apps/moproj2mst/src/ops/base.cpp`, `OpClass::writeMst` / `OpClass::propsOf`.
- Fixture IDs, active Cycle, model relation, and save-only test limits: `source/apps/moproj2mst/tests/mdt/test_l7_load.cpp`, `cycle48_model_is_first_member` and `cycle51_save_writes_no_task_files`.
