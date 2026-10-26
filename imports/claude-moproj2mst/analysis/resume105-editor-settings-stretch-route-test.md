# CP105 STRETCH route regression receipt

Added one Release-active focused test in
`source/apps/moproj2mst/tests/mdt/test_editor_setup_stretch_route.cpp` for
`m_chkbxSTRETCH (0x1538) -> groupBox72_2_2_2 (0x1540)`.

The test uses the current `EditorSetupState` projection, qualified default
combo models and no-active-translator prefix. It verifies the receiver's
explicit `requestedEnabled=false` initialization, leaves effective enabled
and sender checked states unknown, confirms no delivery before connection and
no connection-time synchronization, then requires one ordered delivery per
supplied true/false event. Only the receiver's requested enable value changes.
The test does not claim a physical event, public setupUi reachability, or public
Fill.

Static preflight: the target's `CONFIGURE_DEPENDS` glob includes this
`test_*.cpp` file, yielding `m2m_mdt_editor_setup_stretch_route` and
`m2m.mdt.editor_setup_stretch_route` without a CMake edit. Testkit and editor
setup includes resolve from the target's existing include directories. The
file has one test case and one Testkit `main`, balanced braces, supported
`CHECK`/`REQUIRE` macros, no C `assert`, and passes `git diff --check`. No build
or test execution was performed.

Original evidence: setupUi explicitly calls receiver `setEnabled(false)` at
`0x140f18418` and connects sender to receiver at `0x140f21bcb`; the retained
constructor contract identifies the route as `toggled(bool)` to
`setEnabled(bool)`, AutoConnection. The candidate route table already contains
this one edge. No production code changed.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/tests/mdt/test_editor_setup_stretch_route.cpp` | `f4f0a463f86c52661e7c949783855bcce0debd2928fa7df5aa696686726a92f7` |
| `source/apps/moproj2mst/src/engine/editor_setup.cpp` | `f6b0075090d77c64fef400bc9cb72c63f7558d6734e95b60184b7028dbacab22` |
| `source/apps/moproj2mst/include/m2m/engine/editor_setup.hpp` | `38c82c5784d7df56744f9e3df420f9e2078aeef21c2fe061cd84b65e217cda38` |
| `analysis/wave2-save-diffs-300-setupui.c` | `1217ddfefe72fc52f4a963f01cb4f7d10bc3a468d00e7ef1139736979483b8a7` |
| `analysis/wave3-editor-constructor-prefix-contract.json` | `df9c9a07f9d11072208148c534b92d6619de9fd339ce7338507bfea02d6fb670` |
| `source/apps/moproj2mst/CMakeLists.txt` | `83af75a9e7ca7912e9076a4cdde113262b6497e2353b0a353010ea9c252985da` |

No shared source, existing test file, CMake, GUI or mutable REA state was
changed.
