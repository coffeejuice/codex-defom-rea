# CP244 HeatTreat STEPDEF owner command implementation

Implemented the reviewed property-local dispatch migration. `GuiModel::setHeatTreatPropertyStepDefinition(int)` uses the existing guarded nullable `simCtl()` path and writes only `stpdef`, mapping zero to 1 and every nonzero value to 3. `syncOwnerStepDefinition` resolves `engine::opModel` once before reading `block.stpdef`; the setter retains property storage before lookup and the getter retains native synchronization before Variant assignment. Only the two STEPDEF callbacks changed; Mode callbacks remain on `withOwnerSimCtl`.

Tests retain existing setter/getter/null-step coverage, add a negative getter selection case (returned and stored value stay negative while native `stpdef` becomes 3), and assert the command refuses under the parked geometry barrier.

## Validation

- `python admin/check_new_cpp_bindings.py` on all four changed C++ source/test files: passed, no issues.
- `python admin/build_candidate.py --label cp244-heattreat-stepdef --target m2m_mdt_l6_slots --target m2m_mdt_viewer_box`: passed (Release candidate configure/build).
- `ctest --test-dir build/candidate --output-on-failure -R "^(m2m\\.mdt\\.l6_slots|m2m\\.mdt\\.viewer_box)$"`: 2/2 passed.
- `analysis/resume244-operation-next-leaf-review.md`: PASS design review; no blocking defects.

The build script only accepts `--preflight-ctest` together with `--then-full`; per instruction, the build was run without `--then-full`, then the two CTests were invoked directly. No full build/freeze was run.

## Source SHA-256

- `include/m2m/engine.hpp`: `C0485A4E983562C8E67A2E9ABA2F9315A94A44FB70043C746D037B8B23C2E032`
- `src/engine/gui_model.cpp`: `B75487DA8A6F072DFD0FF776598B259CB7CD9A6BF6D87B4B6419902B52189032`
- `src/model/props_heattreat_slots.cpp`: `329145818E8DE598567BFB31137B6853E3CED8C055ADBEE73642555752AC4F19`
- `tests/mdt/test_l6_slots.cpp`: `4CB1F7A8F05D6908BE5864E729ED187BE67AD2B579177AE4D91F5874D66E5D2C`
- `tests/mdt/test_viewer_box.cpp`: `CB6DF1758B66179BD5A82217DCCD5BC308D0008A58B70E15C8AEAACD87B88823`

## Evidence limits

These tests validate reconstructed dispatch and geometry-barrier behavior in the copied-corpus/internal fixtures. They do not establish a public Open occurrence or fresh original execution. Historical source annotations/ledger support attribution of the existing STEPDEF slots; the owner method is a reconstructed C++ boundary. This leaf does not resolve Mode callbacks, aliases, live ObjectKey/currentness, cache1, GUI/original witnesses, or lifecycle acceptance.
