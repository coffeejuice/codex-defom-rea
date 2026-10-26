# CP232 frequency-text model command implementation

## Change

Moved the frequency text edit's canonical spelling/range admission and live table mutation into `GuiModel::applyEditorFrequencyTextIfControlMatches`. `StagedEditor300::frequencyTextEdited` retains its binding, row, locale, page/model/remesh, and populated-text cache gates, then maps the model command's boolean result to the existing `Completed` or `UnsupportedSnapshot` result.

The model command rejects invalid rows and noncanonical/out-of-range text before model access. It then calls `requireGeometrySyncAvailable()` once, resolves the current step's control and checks its expected pointer token, reads the stride and preserves the short-circuit shape predicate order (`rows == 6`, `stride == 1`, `ncomp == 6`), converts with the existing `qt::toInt`, reads the live values pointer, and writes only `values[row * stride]`. Empty text remains admitted and converts to positive zero. No pointer is returned or cached.

Direct command tests cover a matching control, selected-row-only writes, empty-to-positive-zero conversion, mismatched and null expected identities against a live control, absent step, invalid rows and text, table-shape refusal, and null values storage. The parked-viewer test also calls the command with valid row/text and a null expected token, confirming the geometry barrier refuses before the control lookup. Existing owner callback coverage remains intact.

## Validation

- Test source preflight: passed for `test_simcontrol300_frequency_edit.cpp`.
- Focused candidate build: passed with `python admin/build_candidate.py --label cp232-frequency-text-implementation --target m2m_mdt_simcontrol300_frequency_edit`; receipt: `analysis/cp232-frequency-text-implementation-build.json`.
- Focused CTest: `m2m.mdt.simcontrol300_frequency_edit` passed (1/1).
- Follow-up test preflight passed for `test_simcontrol300_frequency_edit.cpp` and `test_viewer_box.cpp`.
- Repair build passed with `python admin/build_candidate.py --label cp232-frequency-text-parked-guard --target m2m_mdt_simcontrol300_frequency_edit --target m2m_mdt_viewer_box`; receipt: `analysis/cp232-frequency-text-parked-guard-build.json`.
- Follow-up focused CTest passed: `m2m.mdt.simcontrol300_frequency_edit` and `m2m.mdt.viewer_box` (2/2).
- The build receipt and test result establish this focused validation only. Root's integration group and source freeze remain pending.

## Source qualification

Current full-file SHA-256 values at this checkpoint:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/engine.hpp` | `58BA04CF72FF6DA7D59DEED6D058F1A1E38F09E7C7A4EF20FB27D510F36F489F` |
| `source/apps/moproj2mst/src/engine/gui_model.cpp` | `2E68432B8147D60C96BBBE745946D302912C0762B475B8EAD83C2CBFA32B298F` |
| `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp` | `935D1C933BE37FDDB18D4D5D1AABA0E29D0A8CEA985902220CC9F0E0B0C54977` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_frequency_edit.cpp` | `3115E05D9D28C400C6C9B11E99308312FA8B682D3A362E43F0F8F458627FCBB0` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `1625FD68654D93946A5A9AF5CE3D0F3B5588F1D0FDD71008CA6F8A870D39FD37` |

The converter source tree has concurrent changes from other checkpoint work; these hashes identify the complete current files, not isolated CP232 hunks. The native comparison is bounded by CP137's retained original DEF_GUI_MO_64 v11.0.2 and QtCore4_64.dll qualifications recorded in `analysis/resume232-frequency-text-contract-review.md`. The source extraction does not establish fresh native execution, widget validator behavior, Qt provider/reentrancy effects, pointer lifetime safety, or instruction-level equivalence.
