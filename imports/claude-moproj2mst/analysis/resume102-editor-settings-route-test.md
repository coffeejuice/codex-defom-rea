# CP102 heat-mode control-route regression

Added one focused Release-active test in
`source/apps/moproj2mst/tests/mdt/test_editor_setup_heat_route.cpp`. The
existing `EditorSetupState` exposes the required model without shared changes:
the test binds its default combo model, applies the qualified no-active-
translator setup prefix, and projects the existing internal connection.

The test covers original `modeHeatChckBx` `0xa8` → `heatCmBx` `0xb8` toggled to
setEnabled route. It verifies the receiver's explicit requested false state,
unknown effective enabled state and combo state; confirms no delivery before
connection and no connection-time synchronization; and supplies true then false
events, requiring exactly one delivery per event and only the requested local
enable value to change. It leaves the sender's checked value unknown and
preserves the combo items/current index. This is the bounded projection API's
event input; it does not claim a Qt click, selected-editor runtime event,
effective parent-tree enablement, or public editor Fill reachability.

Static preflight: the CMake `CONFIGURE_DEPENDS` test glob includes
`tests/mdt/test_*.cpp`, so this new file produces target
`m2m_mdt_editor_setup_heat_route` and CTest `m2m.mdt.editor_setup_heat_route`
without a CMake edit. It includes the resolved Testkit header and existing
`editor_setup.hpp`; uses only supported `TEST_CASE`, `REQUIRE`, and `CHECK`;
contains exactly one Testkit case and balanced braces; and contains no C
`assert`. No build or test was run.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_editor_setup_heat_route.cpp` | `a1db9384eddae41e5c6c0534538b94bfe89bae8e850d4edd1c83da05952d262` |
| `source/apps/moproj2mst/src/engine/editor_setup.cpp` | `f6b0075090d77c64fef400bc9cb72c63f7558d6734e95b60184b7028dbacab22` |
| `source/apps/moproj2mst/include/m2m/engine/editor_setup.hpp` | `38c82c5784d7df56744f9e3df420f9e2078aeef21c2fe061cd84b65e217cda38` |
| `analysis/wave2-save-diffs-300-setupui.c` | `1217ddfefe72fc52f4a963f01cb4f7d10bc3a468d00e7ef1139736979483b8a7` |
| `analysis/wave3-editor-constructor-prefix-contract.json` | `df9c9a07f9d11072208148c534b92d6619de9fd339ce7338507bfea02d6fb670` |
| `source/apps/moproj2mst/CMakeLists.txt` | `83af75a9e7ca7912e9076a4cdde113262b6497e2353b0a353010ea9c252985da` |

No production, shared source, CMake, GUI or mutable REA file was changed.
