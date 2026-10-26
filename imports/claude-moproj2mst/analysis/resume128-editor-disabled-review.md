# CP128 independent review: disabled remesh Apply

## Finding

The frozen private `applyMeshing3D` change matches the retained disabled branch for the admitted 3D page profile. It allows object-scalar writeback when global remesh is disabled despite an unusable method-radio projection, copies those per-object values before setting the shared enabled byte to zero, and leaves method, scaling, and internal-element state untouched. The short-circuit condition does not evaluate the method radio when `remeshEnabled` is false; scaling and the checkbox are only used inside the enabled tail.

No actionable defect was found within that profile. The review does not claim public Editor300 callback, QWidget read timing, selection-transition, or original runtime parity.

## Native sequence

The controlling static qualification is `analysis/resume128-editor300-next.md`, tied to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained instruction view `analysis/wave2-save-diffs-300-save-140ec1350.asm` and decompile `analysis/wave2-save-diffs-300-apply-140ec1350.c` establish:

1. At `0x140ec13a0`, commit current form controls to their temporary row.
2. At `0x140ec14df`, copy the four remembered scalar fields to each still-present model object; a missing object skips its copy and continues.
3. Then test the 3D path and read disabled/enabled radios.
4. On disabled, call the singleton setter with byte zero at `0x140ec15c4` and jump to the common tail at `0x140ec179d`.
5. Method radio reads occur only in the enabled branch (`0x140ec15f8` / `0x140ec1634`); scaling reads follow at `0x140ec167c` and internal-element checkbox at `0x140ec1759`.

The native setter `0x1400ab150` stores only the supplied byte at singleton `+0x29`; getter `0x1402c7ed0` returns the singleton at `0x143d91238`. This matches the candidate's mutation of the same `RemeshState` passed to its private page helper, including the actual `ConversionSession::remeshState()` used by one test.

## Candidate and tests

In `source/apps/moproj2mst/src/engine/simcontrol300.cpp`, `applyMeshing3D` retains dimension/step qualification and makes the method-radio exclusion conditional on `form.remeshEnabled`. The four-field object copy loop remains ahead of `state.enabled = form.remeshEnabled`. The `if (state.enabled)` body still contains all three enabled-only reads/mutations: method assignment, scaling parse/clamp, and inverse internal-element checkbox assignment.

The new `tests/mdt/test_simcontrol300_disabled_remesh.cpp` gives the disabled path invalid method sentinels (`0`, `99`, and `INT_MIN`), malformed scaling text, and a checkbox value that would alter the singleton if consumed. It checks selected-first rounded values, unvisited bit-preserved values, unrelated object fields, and every other relevant RemeshState field. It also exercises a real missing object between present remembered objects and verifies the later object and enabled-byte update still occur. The remaining cases preserve existing enabled-invalid, wrong-dimension, and absent-step guards.

The method projection is tested as a private page input, not as a physical native radio state. Candidate's preflight reads the plain `form.remeshEnabled` value before the object loop to decide whether to validate the radio; on the disabled branch short-circuiting avoids reading the radio. It does not assign the shared enabled byte until after object updates. This slight adapter decision timing does not change state for the qualified disabled profile, but it is not evidence that an original QWidget getter was called at that earlier point. The enabled-invalid and wrong-dimension guards remain pre-effect refusals and are explicitly not claimed to reproduce native partial effects for those excluded profiles.

No build, tests, GUI, original execution, or mutable REA operation was performed for this review.

## Frozen identities checked

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: `260331D646FDC5EDF712278C6E056142996A8B4DF54A59BFD1DB8C96B9B52ABC`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_disabled_remesh.cpp`: `457CD866BF861233FBCB83E0FB346616CC56DD7F5A24BD47E72E47937EB53AB3`
- implementation receipt: `analysis/resume128-editor-disabled-remesh.md`
