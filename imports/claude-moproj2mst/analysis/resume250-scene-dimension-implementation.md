# CP250 named-display dimension implementation

## Change

`src/engine/scene.cpp::displayNamedModel` now reads the main model's dimension through `b->dimension()` after the existing `!b || !b->step` refusal. Existing-name lookup still precedes main-model resolution; construction, role assignment, display backlink, copied step number/title, GEOTYP handling, ordered insertion and returned ownership are unchanged.

The focused caller test in `tests/mdt/test_display_selection.cpp` covers live dimensions 2, 3, and 0, display backlink, repeated same-name lookup without duplicate holders, and the existing-name short-circuit after removing the main step. A new-name lookup in that missing-step state checks the existing `M2M_E_UNSUPPORTED` refusal.

## Validation

- Confirmed configured target `m2m_mdt_display_selection` and CTest `m2m.mdt.display_selection` in candidate metadata.
- `python admin/build_candidate.py --label cp250-scene-dimension-focused-v2 --target m2m_mdt_display_selection`: configure/build passed. No `--then-full` was used.
- `ctest --test-dir build/candidate -R '^m2m\\.mdt\\.display_selection$' --output-on-failure`: passed, 1/1.
- `python admin/check_new_cpp_bindings.py tests/mdt/test_display_selection.cpp`: passed; Testkit entry point and includes resolved.
- `git diff --check`: passed.
- First build attempt failed because the test reset a `unique_ptr<DB_STEP_DATA>` while its type was incomplete; adding the defining `db_step_data.hpp` include fixed the compile. A second attempt using the original build label was refused because the label receipt already existed; the successful run used the distinct `-v2` label.

## Limits and evidence

This proves focused converter behavior only. The retained native getter defaults to 3 without a step, while this caller continues to refuse missing-step new-name creation; this change does not claim original no-step parity. No fresh native execution, full-suite validation, candidate freeze, or broader ShapeOptim equivalence is claimed.

Successful build receipt: `analysis/cp250-scene-dimension-focused-v2-build.json` (source tree SHA-256 at build start: `6d10d93bc44bdf7e143ba4416f37b1b5c3571ff10430e68f27f0eceb02a288e2`). Current source file SHA-256 values after validation:

- `src/engine/scene.cpp`: `AFD5B1CF049CA59A6C4F88D75853AFBA39D108F81A3634CE878D880C0D665275`
- `tests/mdt/test_display_selection.cpp`: `D3D6C1568FDC8F472D2783B016FEDE77F90FFEFA06271E2DC934FB222E60596E`

The checkout contains unrelated concurrent edits in CP249 and root-owned recovery files. They were not edited or included in this implementation report. The first build attempt also produced `analysis/cp250-scene-dimension-focused-build.json`; it records a failed test-source compile before the complete-type include was added.
