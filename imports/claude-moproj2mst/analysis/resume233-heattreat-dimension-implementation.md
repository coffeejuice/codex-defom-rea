# CP233 HeatTreat dimension predicate implementation

## Change

Migrated the two anonymous HeatTreat dimension predicates in `source/apps/moproj2mst/src/ops/heattreat_htwz.cpp` to call `GuiModel::dimension() const noexcept`:

- `is2D`: `m && m->dimension() == 2`
- `is3D`: `m && m->dimension() == 3`

All helper names and call sites remain in place. No guard, classification normalization, cached snapshot, model/header change, or test change was made. The existing P16 fixture already exercises the relevant null-model, 2D/3D and parser-dispatch behaviors, so no additional assertion was justified.

## Validation

- Focused candidate build: `python admin/build_candidate.py --label cp233-heattreat-dimension --target m2m_p16_htwz` — passed (configure and target build both exit 0).
- Focused CTest: `ctest --test-dir build/candidate -R '^m2m\\.p16\\.htwz$' --output-on-failure` — passed, 1/1.
- The candidate runner emitted `analysis/cp233-heattreat-dimension-build.json` and build logs under `build/logs/`.
- No full build, grouped run, freeze, original application, GUI control or REA activity was performed.

## SHA-256

- `source/apps/moproj2mst/src/ops/heattreat_htwz.cpp`: `BCC5A28C8AED0B8868B4B3B085D93E581E5EB3496B67F72DB15CC1B22BB6E27D`
- `source/apps/moproj2mst/tests/p16/test_htwz.cpp` (unchanged): `35119E5FC8D32FB6CCE84EBFE20AF283EFD419E4E844DE6FEFBEFEB1426BA033`

The focused result establishes compilation and current fixture behavior for this candidate. It is not a full source-stable regression or frozen-candidate result and adds no original-runtime witness.
