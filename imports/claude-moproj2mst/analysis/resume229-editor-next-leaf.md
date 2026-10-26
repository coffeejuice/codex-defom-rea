# CP229: next editor owner-command leaf

## Recommendation

The next bounded editor leaf is `StagedEditor300::frequencyIndexChanged` in `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`. It is a concrete raw `SimControl` consumer with finite inputs and effects: it reads `frqncy_cols`, and on non-custom selections validates the six-by-one frequency table and writes exactly one `double`. The editor currently retains no control pointer beyond the callback. Move that native read/write into a named `GuiModel` command, for example `applyEditorFrequencyIndexIfControlMatches(expected, row, selection)`, returning success/failure without returning a control or values pointer.

Do not edit `engine.hpp` or `gui_model.cpp` as part of this read-only task. The proposed command needs those shared files for declaration/implementation, so implementation must be sequenced with the CP228 owner of those files or handed off after CP228 lands. This is an integration dependency, not a reason to alter the command’s behavior or the C ABI.

## Exact order and contract

Current source order in `simcontrol300_owner.cpp::frequencyIndexChanged`:

1. Validate editor binding, row/selection range, ready page/source model/remesh, and cached combo selection.
2. Call `binding.model.simCtl()`. This calls `requireGeometrySyncAvailable()` before nullable control lookup.
3. Refuse null or a control address different from `pages_->sourceControl`.
4. Read `frqncy_cols` and require stride `1` for every valid row/selection, including custom selection.
5. For custom selection (`row 0: 2`, other rows: 1), set the editor’s input-enabled bit to true and return. This branch deliberately does not require table rows/components/value storage.
6. Otherwise require `frqncy_rows == 6`, `frqncy.ncomp == 6`, and non-null `frqncy.values`; write only `values[row * stride]` to `-1.0` for row 0 selection 1, otherwise `0.0`; then disable the editor input.

The named model command should revalidate row/selection before the geometry check, then call `requireGeometrySyncAvailable()` exactly once, resolve the current control, compare it with the expected pointer as the existing identity token (without dereferencing that expected pointer), and check stride before branching on custom selection. For custom selection it returns success without touching table shape or values. For non-custom selection it checks shape and writes the one double, then returns success. The caller updates the widget enabled bit only after success: custom enables after the command returns; non-custom disables after the command has completed its model store. Map failure to the existing `UnsupportedSnapshot`. This retains the present model-store-before-disable order and every custom-path exception.

Keep the existing pre-command editor/cache checks in place so invalid widget deliveries refuse before any model call. Preserve nullable-control behavior and the geometry barrier’s current location after event validation and before control/stride access. Do not route through `setDimension`, `reset`, a generic callback, or a table snapshot that reads shape before the stride.

## Exact files and regression coverage

- `source/apps/moproj2mst/include/m2m/engine.hpp`: eventual declaration; avoid changes while CP228 owns this file.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: eventual operation body; avoid changes while CP228 owns this file.
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: replace only current-control lookup and frequency table reads/write with the named command; keep editor guards and the following widget-state update.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_frequency_index.cpp`: extend existing cases to exercise the owner command’s direct valid, missing/replaced expected-control, stride mismatch, custom path with missing table storage, malformed table on non-custom path, and one-cell-only write behavior. Existing cases already cover all six row semantics, the row-zero `-1.0` sentinel, custom selection’s table-shape independence, stale/missing control refusal, stride refusal before widget projection, repeated same-index events, and preservation of cached text. Confirm Testkit macros/target metadata before implementation.
- Keep `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` in root’s integration regression set. No changes to that test or to `simcontrol300.cpp` are needed for this isolated editor leaf.

## Safety and scope

This operation is limited to the editor frequency index event and has no Stopping continuation, cache1 read/write, coordinate query, or publication effect. The unchanged `simcontrol300.cpp` and `test_simcontrol300_stopping_cached.cpp` remain the relevant R300 guard. Do not use this leaf to claim ownership closure: public `step` and other raw control/object routes remain.

Evidence is the current `simcontrol300_owner.cpp` implementation and `tests/mdt/test_simcontrol300_frequency_index.cpp`, read against `analysis/resume222-editor-alias-ledger.md` and `analysis/resume222-editor-closure-architecture.md`. This is a read-only design proposal. No source, tests, build, shared recovery state, original process, GUI, or REA state was changed.
