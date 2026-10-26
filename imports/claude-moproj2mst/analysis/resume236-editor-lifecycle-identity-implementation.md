# CP236 editor lifecycle identity implementation

Migrated the two identity-only predicates in `applyInitialEditorPageStateBound` to `GuiModel::matchesCurrentSimControl(session.sourceControl)`. The initial query remains in the original short-circuit position after page-state/model/token guards and before remesh identity. The post-Extra3D query remains after `pages.meshing.objects.clear()`, gated by `actualOperation`, and before the owned recovery tail. Cached `sc` use and other model aliases are unchanged.

Added `initial_editor_preflight_rejects_replaced_step_without_effects`. The test retains the original step allocation, installs a replacement, and verifies unsupported outcome, no page results, no recovery tail, no I/O, and unchanged old/replacement `nstep` values. The existing post-Extra3D callback test now retains the replaced allocation inside the callback, sets `nstep = 99` after Open, and asserts Apply overwrites it to `23` before recovery refuses. It seeds a valid `MeshObjectSnapshot` in the meshing temporary list and verifies that list is cleared before the post-I/O identity refusal.

## Validation

- Focused build: `python admin/build_candidate.py --label cp236-editor-lifecycle-identity-r3 --target m2m_mdt_simcontrol300_lifecycle` — passed (configure and target build exit 0). No `--then-full` was used.
- Focused CTest: `ctest --test-dir build/candidate --output-on-failure -R '^m2m\\.mdt\\.simcontrol300_lifecycle$'` — passed, 1/1 target; lifecycle executable reports 33 cases.
- An earlier focused CTest attempt failed because the test retained/moved the old step before Apply, exercising preflight refusal rather than callback replacement. The test was corrected so retention occurs inside the callback; the successful run above is after that correction.

## Source hashes (SHA-256)

- `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: `F9C89697030C287D399543852F645F4246F36DF7F149F10BAAADEB580C8CDF47`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp`: `D1F77F02A3B961EC7A03D082157AAFCD35DA4776718AD472E6401F9E79EFE431`

## Limits

This is an adapter-side identity predicate migration and regression, not lifecycle ownership closure, a generation identity, original-native routine evidence, fresh-original parity, or process-level lifetime proof. Cached control borrows and same-address replacement limitations remain. No full build, source freeze, shared recovery state, original application, GUI, or REA work was performed.


