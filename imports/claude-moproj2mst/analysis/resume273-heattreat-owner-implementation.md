# CP273 HeatTreat owner consolidation implementation

Implemented the review-approved private consolidation in the converter subtree.

## Source changes

- `source/apps/moproj2mst/include/m2m/engine.hpp`: declared private `GuiModel::setHeatTreatPropertyModeWord(int, bool)`.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: the three existing named methods remain and delegate to the helper with fixed indices: Deformation 1, Diffusion 3, PhaseTransformation 2. The helper makes one `simCtl()` admission and updates only that TRANS word to 1/0 when control exists.
- `props_heattreat_slots.cpp`, tests, and other files were not modified.

The mode index is private to `GuiModel`; no selector was added to the public interface. The helper preserves the geometry refusal and missing-step behavior through `simCtl()`. Property default, flag, owner dispatch, and refresh ordering remain in the unchanged caller.

## Source hashes

SHA-256 after implementation, before focused build:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `D9BF3FCFBB43F9737D6C2980900AA825732294753F867A371B4DC9688A6FA810`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `0B965B1549761F038AFB4C1955C067941F3AF97BB5EBF7958B3C34D07202BE5A`

## Validation boundary

Retained caller-path coverage is in `m2m.mdt.l6_slots`; direct named-command coverage is in `m2m.mdt.viewer_box`. Run the requested focused build and exact CTests only. This change is a private refactor and establishes no new original-runtime behavior, GUI behavior, lifecycle acceptance, or full-suite/frozen-candidate status.

## Focused validation result

- `python admin/build_candidate.py --label cp273-heattreat-owner --target m2m_mdt_l6_slots --target m2m_mdt_viewer_box`: configure and build both exited 0. Build log: `build/logs/cp273-heattreat-owner-build.log`.
- `ctest --test-dir build/candidate --output-on-failure -R '^(m2m\\.mdt\\.l6_slots|m2m\\.mdt\\.viewer_box)$'`: 2/2 passed (`m2m.mdt.l6_slots`, `m2m.mdt.viewer_box`).

This was the requested focused build and test only. It is not the grouped source-stable suite or a frozen candidate. Root retains integration and freeze ownership.
