# CP176 R300 plan review: L6 SimControl readers

## Verdict

The proposed cut matches the checked-in CP175 L6 source boundary. The production target is `source/apps/moproj2mst/src/engine/l6_slots.cpp`; the paired focused test is `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp`. No source edits, builds, GUI actions, or REA operations were performed for this review.

## Source checks

- `ownerSimCtl(Project&, Props&)` is the local raw-return helper at the existing helper position. It checks `pr.owner`, calls `engine::opModel`, and returns the nested `model->step->sim_ctrl` address when present.
- The four readers are exactly `propsReadSteps`, `propsReadEnvTemp`, `propsReadStepBy`, and `propsReadConvection`. Their explicit `sc` parameter short-circuits the owner lookup today; when null, they each independently call `ownerSimCtl`. The reached `distortionPropsLoad` calls each once with `nullptr`, in that order, then copies fixture and pass counts.
- The proposed scoped behavior is compatible with `engine::withOpStep` in `engine_internal.hpp`: it is a direct synchronous `std::invoke`, obtains the `opModel` once, and passes `model ? model->step.get() : nullptr`. It does not queue work or add a geometry check. A local wrapper must preserve the explicit nonnull path before any owner inspection, and on null control with null owner must invoke the reader callback once with null without entering `withOpStep` (since `withOpStep` requires an `Operation&`). With an owner, it can use `withOpStep` and pass `step ? &step->sim_ctrl : nullptr` during the callback.
- Null SimControl checks precede every access to `pr.common` and the `setStepBy` call in the current four readers. Thus the described no-property-write behavior follows, provided each migrated reader returns the callback's bool and does not move the null check inside a later write sequence.
- Nonnull read/write order to retain: steps reads `SC+0x16c`, then `SC+0x164`, then formats `SC+0x1d0`; environment reads/formats `SC+0x3d0`; StepBy first calls `base::setStepBy` with `SC+0x240`, then reads/formats `SC+0x1e0` for `dtMode == 0` or `SC+0x1d0` otherwise; convection reads/formats `SC+0x4c8`. These map to CommonBlock `numSteps`, `stepIncr`, `dtmax`, `envTemp`, `dtMode` via setter, `dsmax`/`dtmax`, and `convection` respectively. Unit categories are 0, 9, 1/0, and 0x20.

## Focused coverage and test command

The plan's proposed null-step test is already present in `test_l6_slots.cpp` (the `distortion_props_without_model_step` case around lines 195-245, as located in this checkout). It validates all common-block sentinel values remain unchanged while fixture/pass count stores still run, and restores the moved `model->step` on both normal and exceptional paths. It invokes `mst::g_l6Slots.distortionPropsLoad` directly with valid Distortion props. The plan should say to preserve/rely on this existing case rather than conditionally add a duplicate. It does not test explicit nonnull `sc`; source-order qualification remains appropriate because there is no production call site passing explicit control.

CTest names are dotted, based on test packet/topic naming in `source/apps/moproj2mst/CMakeLists.txt`. The focused test is `m2m.mdt.l6_slots` (executable target `m2m_mdt_l6_slots`), and the specified regression names are consistent with the convention: `m2m.mdt.save_l6`, `m2m.s.save_current`, `m2m.mdt.simcontrol300_stopping_cached`, and `m2m.g0.abi`. The exact test source path is `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp`.

## Scope qualifications

The distinct `ownerSimCtl(HeatTreatProps&)` is outside this reader cut. Other `m.step` uses in `l6_slots.cpp` remain, including dimension and model-copy/save paths. The plan correctly avoids claiming all L6 raw step access is gone. Its only correction is the already-existing null-step test; retain it and extend only if the refactor invalidates its coverage.
