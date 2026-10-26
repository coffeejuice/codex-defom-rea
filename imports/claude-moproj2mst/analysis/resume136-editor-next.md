# CP136: private Environment constant/table radio clicks

The next complete private callback cut is the three Environment constant/table radio pairs: ENVTMP, CNVCOF and ENVATM. Their native bodies change only the two checked states and three enabled arguments. Unlike the recovered ACVCOF callback, they never query SC or touch any live/temporary curve. The existing EnvironmentPage already owns exactly these five projected control values, so no new curve or runtime widget owner is needed for this explicit private event.

Only this new report was written. No source/build/test/GUI/original execution/mutable REA change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Evidence is retained decompilation/constructor metadata plus bounded immutable-PE DUMPBIN; it proves callback code, not an observed native delivered click.

## Exact callback boundary

| Curve index | Body, end exclusive | Radio UI offsets | Constant edit/unit; definition offsets |
| --- | --- | --- | --- |
| 0 ENVTMP | 140eccb50..140eccc80 | cf8/d08 | d00/d18; ce0 |
| 1 CNVCOF | 140eccc90..140eccdc0 | d58/d70 | d60/d68; d80 |
| 2 ENVATM | 140eccdd0..140eccf00 | e08/e28 | e18/e20; de8 |

All three bodies use the same order. Argument0 sets constant checked=true, table checked=false, constant edit enabled=true, unit enabled=true, definition enabled=false. The nonzero branch reverses those five booleans. Each branch contains only two QAbstractButton::setChecked calls (IAT141e07298) followed by three QWidget::setEnabled calls (IAT141e0ba40), then returns. There is no SC getter, current-curve index store, DYNARY mutation, numeric-text read/format, allocation, table dialog, or viewer/file tail.

Constructor clicked() connections and slot-map indices35..40 route through wrappers140ecf6d0/6f0 for ENVTMP,140ecf720/740 for CNVCOF,140ecf770/790 for ENVATM. Bounded DUMPBIN independently confirms each pair supplies constants0/1 to its body. Admit only these explicitly delivered clicked choices. A cached assignment or programmatic setChecked must not automatically invoke this event API. The native body treats other nonzero ints as table, but the wrappers prove only0/1; retain a candidate guard for other values.

## Smallest implementation proposal

Root shared owner.hpp declaration:

```cpp
ApplyResult StagedEditor300::environmentCurveClicked(
    Editor300Binding, int curveIndex, int selection); // index0..2, choice0/1
```

Use the existing live Project/OPI-generation/GuiModel/remesh binding checks, completed private page metadata and same source-model/remesh tokens. Then assign only `pages.environment.curveControls[curveIndex]` as `{constant, !constant, constant, constant, !constant}`, where constant means selection0. Return Completed. No current-SC/sourceControl query and no call to pageState: the native callbacks have none. The method remains valid for retained completed controls when the same live model's step has become absent/replaced; it must not dereference the historical sourceControl, inspect curve payload/aliases or fabricate a model replacement. This is deliberately different from reactionRateClicked, whose later stores depend on actual current SC.

Owner admission is a private candidate prerequisite, not native validation. Missing/partial/private-native-base-pending pages, wrong live binding/OPI generation, out-of-range index or selection refuse before the control projection. Existing EnvironmentCurveControls are value projections only: no physical QWidget enabled bits, automatic signal emissions or reentrancy are certified. Do not impose an enabled-input guard on later explicit numeric writes; native Save's callback checks the selected radio, not physical enablement.

No curve owner, constant text, currentCurveIndex, ACVCOF selection, scalar control, OPI/model/remesh/file value changes at click time. In particular, choosing constant must not clear temporary.ndim or free its table immediately. Those stores belong to later existing applyEnvironment3D. Choosing table preserves the cached invalid constant text and uses the retained typed curve when Apply runs.

Worker exclusive source: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_environment_click.cpp`, new implementation receipt. Root owns the single owner.hpp declaration. No simcontrol300.cpp, shared page fields, lifecycle or CMake edits. Expected target `m2m_mdt_simcontrol300_environment_click` / CTest `m2m.mdt.simcontrol300_environment_click`.

## Focused tests and existing-code reconciliation

- On actual bound completed private pages, all3×2 clicks yield the exact five booleans; only the addressed control tuple changes. Keep unsaved numeric text and live/owned table ndim/counts/pointers/payload unchanged; no I/O or currentCurveIndex change.
- Explicit0→1→0 and repeated same-choice occurrences normalize inconsistent prior projected booleans without a refill or an equality shortcut.
- With actual owned model step absent/replaced, the metadata-valid control event still completes and touches neither old nor replacement curve. This detects an incorrect new SC prerequisite inherited from the ACVCOF method.
- Guard index-1/3, choice-1/2, wrong binding, partial/unconstructed owner and expired OPI generation before any control change. Public constructor remains unsupported.
- Compose one event with the existing private Apply: table choice skips invalid constant text and copies the actual owned table; constant choice defers ndim/value changes until Apply, preserving the existing Invalid-validator behavior. Use separate fixtures for Apply profiles; do not broaden existing post-constant table-shape guards or silently repair retained arrays.

Current fillEnvironment3D already initializes the five booleans using curve.ndim. Existing environment table tests cover initial Fill and Apply directly; they do not expose a bound delivered-click transition. Existing applyEnvironment3D reads constantChecked, changes the actual temporary only there, then copies it to SC; no rewrite of this logic is proposed.

Operation-name still has the unresolved current-main/project/tree tail after SC/OPI writes. Advanced frequency callbacks have model-store prefixes followed by actual QWidget enable setters and no existing requested-enable field; they are not silently treated as fully integrated widget callbacks. Difference-boundary additionally rebuilds TIMEID and invokes the DB-step property path. View-factor dialogs/picking and selected frame navigation retain their existing owner dependencies. Public mode300 remains blocked at the selected palette/private constructor witness/6504abb0; this private control event does not supply it.

## Bounded evidence identities

Complete native body SHA-256: ENVTMP `779a898cb5664b1c6e6869adedb662ad4dad421d8deacc75c41ca121c732d491`; CNVCOF `22fe6134ad274063a826e1ff324047bea083a2aa03cf19b3aa0a3aeeccbdb4e2`; ENVATM `d914f4a2574d3811d397423d336f287b456c93c58c4165f3d3cd310a16c065b5`.

Wrapper half-open range hashes:6d0..6ea `f19af90b96590e3c756ec24a5cf6b72363da2ee72660af88862db1c0fda26525`;6f0..70d `be4f19b6a3334ebd47afc8cfbb7272dc149153672d9ee2216589929a7cce9e29`;720..73a `1f065b09ada2b55e3da2e1a0c432cf9c02d3604c2a471603245b06d79aaba896`;740..75d `e39cb93a5d70cd9676fa9765a34bf2dd9542c80e0412ee7255840b08a2334a58`;770..78a `1645309390c0194b050874631ce3d5db7268e791ba9b7d38c2d25f8b7e3410dd`;790..7ad `8ed9ac4136b91e7b660c53370dde1d2c21596dacfb431137c60c2965308bbc9e` (all have prefix140ecf).

Retained body decompile files140eccb50/140eccc90/140eccdd0 SHA-256 respectively: `d85a6bf505e45c19b9b5aa96dbd29688e7745c3f22d963001f4d8e980a7929b0`, `92508ebf62e3abe2fac4a2aeeb7fc880530b49dd95fe891b90d10bcea6ab972a`, `dc8b8c91dc49207d904ad606f2ed575ec3161a1f7b17b210c1afdc7d8d8268ac`. Native range ends are confirmed by PE unwind bounds and final returns. Tool: pinned BuildTools14.44.35207 DUMPBIN SHA `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, banner14.44.35228.0. No new raw capture outside this report was written, and no reconstructed executable was inspected or produced.
