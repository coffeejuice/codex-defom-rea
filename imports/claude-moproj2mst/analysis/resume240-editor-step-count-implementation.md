# CP240 editor StepCount value boundary

Implemented the reviewed NSTART refill boundary. `GuiModel::refillEditorStepCountIfControlMatches` calls `simCtl()` once, requires a nonnull matching current control, calls the existing three-argument `fillStepCount` with delivered prior checkbox state, and assigns the complete output page only after a successful fill. The editor stores `operation.nstartGiven` first and publishes the page only after the model command succeeds. Geometry and fill exceptions continue to propagate. The returned page contains no native SimControl alias, but its optional MO context retains the existing legacy `Operation*`; this change makes no OPI ownership claim.

Direct tests cover live/null expected, replaced and missing current controls (including null/null), an invalid fill context, unchanged all-field output sentinels on false, complete success projection, unchanged SC and operation values, and prior checkbox history for key, null-operation, writing, and nonwriting branches. The viewer test calls the command on a parked model with a valid context and expected null; it observes `Error(M2M_E_UNSUPPORTED)` before identity refusal and verifies that output remains unchanged. Existing callback tests still cover flag-store-before-refusal and cached-page publication.

Focused build: `python admin/build_candidate.py --label cp240-editor-step-count-r3 --target m2m_mdt_simcontrol300_nstart_toggle --target m2m_mdt_viewer_box` passed without `--then-full`; receipt `analysis/cp240-editor-step-count-r3-build.json`. The receipt's source-tree SHA-256 is `b62049c82d0e3e5e1a2eb890ec68736a7be477f153ee750bf83aafe35fd2a3ec` (3563 source files). Focused CTest command `ctest --test-dir build/candidate -C Release --output-on-failure -R "^m2m\.mdt\.(simcontrol300_nstart_toggle|viewer_box)$"` passed 2/2. `git diff --check` passed for the five owned source files. The first two focused test attempts failed because new assertions expected an operation override while `nstartGiven` was zero and then expected an unchecked checkbox while the writing operation flag was one; the test setup/expectation was corrected before the final passing build.

Owned-file SHA-256:

| File under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine.hpp` | `17e32959813e1dda81812038750141d0bf3d699edc7fe43522f82a840490c45b` |
| `src/engine/gui_model.cpp` | `78f952c019fab6f97b7823121681ca4061ba8ed8140eea54ba06a75bf085c170` |
| `src/engine/simcontrol300_owner.cpp` | `c93ba03b9afa731ea2c2dfc7263a4ff23eec67d52a103ea55e484e85177c4db3` |
| `tests/mdt/test_simcontrol300_nstart_toggle.cpp` | `039a5d1e9fa21105b0741feced5d52dd1f3f59d555835bcfe0222173fd403cde` |
| `tests/mdt/test_viewer_box.cpp` | `80d13019378e9477f11f4802915702648188c6b9bef0b136e760f25e7a21a041` |

Focused executable SHA-256: `m2m_mdt_simcontrol300_nstart_toggle.exe` = `e4cb63afc642067e50dc1aef71fe1a9c7c3afa392a781cc1b2778f090d08ed1a`; `m2m_mdt_viewer_box.exe` = `3c5c42457f6f25a0fabca070ab22afb382745396e7839341004c1179c6968415`.

This validates only the focused reconstructed source. No full source-stable run or freeze was performed here. The retained CP132/CP133 original DEF_GUI_MO_64 v11.0.2 evidence supports the bounded NSTART ordering; no original, GUI, or REA interaction was performed in this implementation. Native Qt emission, reentrancy, whole-editor construction, and Save completion remain unestablished.
