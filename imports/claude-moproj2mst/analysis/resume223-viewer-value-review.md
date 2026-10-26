# CP223 viewer scalar migration review

Verdict: **PASS for the scoped source migration; build/test acceptance remains pending.** No blocking source defect found. This is a named scalar-read staging increment, not ownership closure or cache1 admission.

## Behavior and ordering

The old `viewer.cpp` helper first called `GuiModel::simCtl()`, whose first statement is `requireGeometrySyncAvailable()`. Only after that admitted call did it test control presence, read the step dimension at `+0x1c`, and read signed 32-bit simulation geometry type at control `+0x100` for dimension 2. The new `GuiModel::simGeometryTypeIf2D()` (gui_model.cpp:215-220) performs the same barrier check before inspecting either native field. For an available missing step it returns 0; for an available non-2D step it returns 0; for 2D it returns the identical signed field value as `int`. The simulation control is an embedded member, so with an existing step there is no independent nullable control that this implementation accidentally dereferences.

The typed dimension access is equivalent to the previous offset read: DB_STEP_DATA publicly inherits DB_WRITE_VAR at its primary base, whose signed `std::int32_t dimension` is layout-checked at `0x1c`. The embedded `sim_ctrl` is layout-checked at step `0x750`; `Cls_1401e7b10::geotyp` is signed `std::int32_t` and layout-checked at `0x100`. These existing layout declarations support source equivalence; I did not obtain a new original-runtime witness.

The viewer helper now delegates directly, with its consumer at viewer.cpp:1764 unchanged. `modelDim()` remains because its four other call sites still use it (1179, 1749, 2098, 2110). The migrated helper no longer acquires either raw alias itself. The owner method returns a value and neither accepts nor returns native pointers, references, callbacks, or identities.

## Tests and limits

The existing Testkit case gains three useful available-model checks: forced dimension 3 returns 0, forced dimension 2 returns configured geometry value 37, and an explicitly removed step returns 0. The parked-model call uses the same `refused` helper as `simCtl()` and requires `M2M_E_UNSUPPORTED`; surrounding checks retain the barrier-state/no-effects regressions. It uses Release-active `CHECK_EQ`/`CHECK` and the file already has `testkit::run_all()` entry point. No new include or unsupported assertion macro was introduced. The temporary dimension is subsequently set through the existing loader slot before its dimension-2 assertion, so the new test does not invalidate that existing expectation.

These checks are suitable for this small staging change. They do not prove complete ownership closure, a source-negative API boundary, signed-range exhaustiveness, or original runtime fidelity. The parked missing-step case is not added; barrier-first source order is explicit. No new API test exercises the unchanged downstream viewer branch directly, although that branch remains covered by the existing target suite. These are limitations, not blocking defects for the bounded contract.

## Compatibility, ownership, and integration

The reviewed delta adds one internal C++ declaration/definition and redirects one helper. Existing public `step` and `simCtl()` remain. The same overall diff also contains CP222 StepIdentity work; this review does not approve that independent implementation. The scalar method does not use that observation token as admission authority.

Within the four-file diff, no C ABI declaration/export/layout/error mapping, R300 cache1 store/refusal, CP163 A-only publication, or continuation guard is changed. This is a scoped diff conclusion, not a global audit of other files or a C++ binary ABI guarantee. The source partition is consistent with root owning common engine.hpp/gui_model.cpp and one assigned viewer/test owner; this report edits none of them and cannot independently certify concurrent ownership administration.

A build is required: a new out-of-line C++ method must compile and link with the changed viewer and test. The CMake test-name construction resolves this file to `m2m_mdt_viewer_box`; root should run it in the changed-target group, then the source-stable complete launcher and freeze required by AGENTS. No build, test, GUI/app control, original execution, mutable REA access, deployment, or shared-state change was performed by this review. Only this report was written.
