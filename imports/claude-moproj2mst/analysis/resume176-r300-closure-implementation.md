# CP176 R300 L6 scoped SimControl implementation

## Implemented boundary

Replaced only the mst-local ownerSimCtl(Project&, Props&) raw-return acquisition and its four Distortion readers in `source/apps/moproj2mst/src/engine/l6_slots.cpp` with synchronous `withPropsSimCtl`. Its template requires an exact bool callback result; the helper and nested withOpStep callback return bool. No owner-derived step or SimControl address is stored or returned. The nested pointer is passed directly to the reader while withOpStep is active. Explicit nonnull sc invokes the reader immediately before any owner/model lookup; null owner invokes it once with null. Otherwise each reader independently calls the existing withOpStep seam, which invokes opModel once then invokes its callback inline.

Each null reader returns false before dereferencing common or calling setStepBy. The nonnull field-read/store order remains NumSteps(+0x16c), StepIncr(+0x164), TimePerStep(+0x1d0); EnvTemp(+0x3d0); StepBy(+0x240) before the existing dtMode branch and distance/time value; Convection(+0x4c8). Unit formatting is unchanged. Distortion retains its four calls in that order followed by fixture and pass stores. No geometry gate or common-block allocation was introduced. No exported header, C ABI, CP163 cache0/A/after-A refusal or cache1 code changed.

## Focused test authored

Exact file: `source/apps/moproj2mst/tests/mdt/test_l6_slots.cpp`.

Added `distortion_props_null_step_preserves_common_and_refreshes_counts`, which admits the corpus Distortion owner, obtains its actual model, detaches the real step only around the direct existing `g_l6Slots.distortionPropsLoad` call, and restores it before assertions/teardown, including the exception path. It asserts every common destination (numSteps, stepIncr, dtMode, envTemp, convection, dsmax, dtmax) remains at a distinct sentinel while the later fixture/pass stores refresh. It uses the existing corpus reader and creates no test files. Existing `distortion_props_from_sim_control` remains. The explicit-sc branch is internal and unused by production call sites; its zero-lookup behavior is established by source order only, without adding an exposed testing entry.

## Static verification and pending validation

A bounded Python source assertion check passed: no ownerSimCtl name remains in this file; four scoped readers and null-before-write checks; one helper withOpStep site; exact bool constraint; explicit-sc/owner/step acquisition order; one direct nested SimControl address expression; no GuiModelAccess or simCtl() in the changed cut; unchanged Distortion call/count-store order. Manual review of engine_internal.hpp confirms withOpStep invokes the callback synchronously via std::invoke. Callback captures hold references only for inline execution; none are retained or queued. The test's `opBase` type is mst::OpClass*, and GuiModel::step remains unique_ptr<DB_STEP_DATA>.

No build, CTest, full-suite run, freeze, executable production/deployment, GUI session, live mutable REA operation, shared ledger/state update or cache1 publication was performed by this owner. Root still needs the planned source-bound focused CTest group, then complete source-stable suite/freeze. Static checks do not establish compilation or runtime success.

## Hash receipt

| File | Before (plan) | After |
| --- | --- | --- |
| l6_slots.cpp | 8e6e21c84e60d63ef6a804941c3928cdf603feeb9402400de5c968a54d15424c | 42b36efbffcfbf997fed53416459670d58db31ce0c235026baacc9bd8ee37e58 |
| test_l6_slots.cpp | 9b9101aa1ee7499119818b41313e91f512d3e710c46336f237707c07e77f173b | 5b69d84666a508581a307cc28a28edbdc55f3f619b1d6ad9a2a1d305f0c24bad |
| engine_internal.hpp (untouched) | f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d | f4e6627b808eebf31dbcf8aae0280fe1221bc50abbaa6f77639156ea704bcb0d |

## Risks, residual scope and release

This closes only the four readers' nested raw-return acquisition. The local simCtl helper and its users, dimOf/is2D/is3D, dieStressAddInteraction's inter_obj accesses, dieStressFromDbStep's dimension/material/phase copies and model SimControl uses, and dieStressPrologue/model-step alias at the existing later site remain outside this cut. Public GuiModel::step and other existing aliases are not closed. There is no claim of whole-model synchronization, original reader-tail reachability or readiness for zero-length cache1. Existing nonnull common-block assumptions remain intentionally unchanged.

Production and paired test source ownership released to root after writing this receipt. No GUI control was acquired, so no release API claim is made. Rollback is the paired helper/readers and test edit to the recorded pre-cut hashes; validation remains root-owned.
