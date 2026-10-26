# CP229 editor frequency-index command implementation

## Implemented behavior

Added `GuiModel::applyEditorFrequencyIndexIfControlMatches(expected, row, selection)`. It first rejects invalid row/selection arguments, then performs exactly one geometry availability check. After the guard it resolves the nullable current control, refuses null even when `expected` is null, and compares the current control address to the expected pointer as the existing identity token without dereferencing that token. It reads `frqncy_cols` and requires stride 1 before testing for the custom selection.

Custom selection returns success without reading table rows/components/value storage and without writing the model. Non-custom selection checks rows equal 6, components equal 6, and non-null values in that order, then writes exactly `values[row * stride]`: `-1.0` for row 0 selection 1, otherwise positive `0.0`. It returns no raw control or table pointer.

`StagedEditor300::frequencyIndexChanged` retains its existing binding, range, lifecycle, source model/remesh, and cached-combo checks before calling the command. It maps ordinary false results to `UnsupportedSnapshot`, then enables the widget after a successful custom command or disables it after the non-custom model store. Repeated same-index events still execute the command. A geometry-barrier exception is not caught or translated, so it retains precedence over null/mismatched identity and stride for valid inputs.

The C ABI, `simcontrol300.cpp`, Stopping predicates, cache1 behavior, and widget construction were not changed. The command uses the same staged raw-address equality semantics as the former callback; it does not claim a durable or opaque control identity.

## Tests and verification

Extended `tests/mdt/test_simcontrol300_frequency_index.cpp` with direct command coverage for matching and mismatched control tokens, live-control/null-expected refusal, missing-current-control/null-expected refusal, invalid arguments, wrong stride, custom selection despite malformed/missing table storage, malformed non-custom table refusal, and one-cell-only writes with the row-zero `-1.0` sentinel and positive zero.

The focused target built successfully with the Visual Studio 2022 developer environment:

```text
cmake --build build/candidate --target m2m_mdt_simcontrol300_frequency_index
```

Focused CTest passed:

```text
ctest --test-dir build/candidate -R '^m2m\.mdt\.simcontrol300_frequency_index$' --output-on-failure
1/1 passed
```

`git diff --check` passed. The direct frequency test fixture cannot install a parked geometry barrier; barrier exception precedence is established by the command's first guard after input validation and remains for root to cover with the existing geometry-barrier regression. The `test_simcontrol300_stopping_cached` regression was not run here; root owns the broader focused integration set and freeze.

## Source state

The working source is uncommitted and stable after the focused build/test. The four authorized files also contain pre-existing accumulated changes from prior CP225–CP228 work in shared model files; those were preserved. Current SHA-256 values:

- `source/apps/moproj2mst/include/m2m/engine.hpp`: `C1CD02921A03F96394892DA266A2463CFC265149B3597442F4D3EACDC0F25DFD`
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: `B3460A4BF2F3874688FBE43326AE87CDB325C60CA35EE09A7B85AD26CC1DF002`
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: `15CA48461D6F48B2F2CBD085C971E45AAC43C076CAC4C182EECD0133B373FEB6`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_frequency_index.cpp`: `197CA0CF660A360A9198EB4AD100E9B12D5FBF61D94D33BA64BA48ACDFFF1A8F`


## Added parked-geometry precedence regression

Before root's full freeze, added one assertion to the existing `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypass` fixture in `tests/mdt/test_viewer_box.cpp`. It invokes the new command with valid row 0/custom selection 2 and a null expected-control token while the model is parked, and uses the fixture's `refused` helper to require `M2M_E_UNSUPPORTED`. This specifically exercises barrier refusal before nullable/current-control identity comparison. The test remains in the existing fixture, which continues to assert the parked model and surrounding state are unchanged; existing cached-Stopping/cache1 assertions elsewhere remain untouched.

Root requested no build while holding source for integration. Thus the frequency-index target build/CTest reported above passed before this latest test-only addition; `viewer_box` and the resulting combined source state have not yet been rebuilt or tested. Root should rebuild the focused frequency-index and viewer-box targets and run their CTest cases before the full suite/freeze.

## Constness correction after review

Root's focused `viewer_box` build found that the first assertion used the fixture's `const auto& pendingModel` alias, while the new model command is non-const. Corrected the assertion to invoke the same command on the existing mutable `model` reference. The parked state and `refused` expectation are unchanged. No build was run after this correction; root should rerun the focused targets.
