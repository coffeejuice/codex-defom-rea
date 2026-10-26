# CP139: bound view-factor state change and deferred Apply effects

The smallest complete next private callback is the view-factor checkbox stateChanged handler. It only projects the definition button's enabled argument; live VFACTR mode/data and the editor's owned curve/definition are changed or consumed later by Apply. Adding the explicit bound event closes that distinction without implementing the definition dialog or crossing the public palette gate.

Only this report was written. No source/build/GUI/original execution/REA database change occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Evidence is retained native assembly/decompilation and immutable section-mapped PE bytes, not observed native stateChanged delivery.

## Exact callback and existing Apply order

Slot-map index66 is `on_m_chbxViewFactorCalculation_stateChanged(int)`; dispatch1410dbd2d calls140ed0820 at1410dbd40. Complete native `[140ed0820,140ed0871)` SHA-256 is `e054a1e246c227966b09bc8bccdee08c5679b41b7b79f55f6b55e057ad82f91a`.

At140ed082d it tests argument0. Zero reaches QWidget::setEnabled(UI+da0,false) atd0849; any nonzero reaches setEnabled(UI+da0,true) atd0866. IAT141e0ba40 is the same exact imported QWidget setter used in the previous recovered callbacks. The function returns atd0870. There is no SC/model/dimension query, checkbox write, curve/current-index access, memory allocation, dialog, numeric conversion, viewer refresh or file effect.

Environment Fill140ec3fa0 explicitly sets UI+d98 checked from SC.vfactr_mode nonzero, then sets the definition button enabled to the same boolean. Existing fillEnvironment3D already projects these two values. This new event must not replay Fill or infer delivery from programmatic assignments.

Existing source-backed Apply (`140ec9b30` then`140ec50f0`) copies the four Environment temporaries first, applies the seven scalar controls, then writes `SC.vfactr_mode = checkbox ? 1 : 0`. Unchecked skips dimension and curve/definition work, retaining live VFACTR data/definition. Checked then requires a view-factor context; if absent, current applyEnvironment3D returns UnsupportedSnapshot **after** the earlier curve/scalar/mode stores. With context it checks current dimension: non3D skips VFACTR copy/definition,3D requires a nonnull distinct source and copies the actual retained curve/definition. Do not move any of these model effects into the checkbox event or roll back the reached mode store on refusal.

## Minimal owner API

Root owner.hpp declaration:

```cpp
ApplyResult StagedEditor300::viewFactorStateChanged(Editor300Binding, int state);
```

Admit only the ordinary two-state delivered values0/2 in this bounded cut. The native body accepts all nonzero ints, but partial/other check-state delivery remains excluded rather than inferred from a bool cache. Require the same live Project/OPI generation/GuiModel/remesh, completed private page metadata/source-model/remesh tokens, and cached Environment.viewFactorChecked equal to state!=0. The caller represents the changed checkbox before declaring delivery, as with the existing title/NSTART/combo event contracts.

Assign only `viewFactorDefinitionEnabled = state != 0` and return Completed. Do not overwrite viewFactorChecked, access simCtl/pageState/sourceControl, require a current step/dimension or viewFactorCurve owner, set mode, mutate/fix/copy any DYNARY or definition, update other controls/index, or perform I/O. A retained completed control can still consume this source-independent event when the same live model's step/curve payload is absent or replaced. Wrong identity/state/cache/partial owner refuses before projection. These are explicit private admissions, not native validation.

This models the existing control value only. Physical QWidget disabled/hidden bits, signal emissions/reentrancy, dialog/button invocation and native constructor completion remain unclaimed. An explicit repeated event reasserts the argument without a same-value shortcut. The public mode300 palette/6504abb0 guard and completeForPublicSave=false remain unchanged.

## Exclusive files and focused tests

Worker source set: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_viewfactor_state.cpp`, implementation receipt. Root adds only the owner.hpp declaration; no new page field, simcontrol300.cpp/Apply rewrite, lifecycle or CMake change. Expected target `m2m_mdt_simcontrol300_viewfactor_state`, CTest `m2m.mdt.simcontrol300_viewfactor_state`.

Focused cases:

- Ordinary delivered0/2 states project only button enabled, preserving already changed checkbox, SC mode/data, actual editor curve/definition/currentCurveIndex, other controls/pages/OPI/remesh and I/O. A cached checkbox assignment alone is not delivery.
- Same-state occurrence reasserts the button projection while retaining unsaved numeric text. Missing/replaced actual step or absent view-factor payload adds no SC/curve prerequisite, because the complete callback has none.
- Uncheck event leaves mode/data unchanged until existing private Apply; then mode becomes0 but then-current live curve and definition remain unchanged.
- Check event leaves model unchanged until Apply; with the actual retained session curve/definition and explicit current dimension3, existing Apply copies them only then.
- Check event plus existing Apply with missing view-factor context returns UnsupportedSnapshot after prior Environment scalar and mode1 stores, preserving those effects and unchanged live VFACTR curve/definition. This is the concrete partial-Apply boundary; do not substitute a fake dialog-produced curve.
- State-1/1/3, mismatched cached checked state, wrong binding, partial/unconstructed owner and expired OPI generation guard before button projection; public factory is still incomplete.

Existing snapshot/lifecycle tests already verify initial view-factor projection, retained actual curve ownership and unchecked/checked Apply semantics. They do not provide a bound post-Open stateChanged entry. The new cases compose that new event with those same existing paths; no old behavior is reimplemented. Partial-state1 and the definition dialog at140ed0880 remain separate guards. TIMEID/DB-step property tail, operation-name/tree refresh and selected constructor palette history remain independent dependencies.

## Evidence identities

- Retained `wave3-editor-viewfactor-dimension-140ed0820.asm`: `2be9286de7b5c1bdf883cd6d377bb9bfb84ae3925d2db75994c37e7902aa0f10`.
- Retained `wave2-save-diffs-300-support-140ed0820.c`: `cc427084d30e7699c4674936797de168e32196300bc1798282a9ae4d1e896247`.
- Complete callback's section-mapped raw bytes and PE unwind bounds confirm the span/return above; no new raw artifact was written outside this report.

No reconstructed executable/deployment was inspected or produced. Source implementation requires root assignment/shared declaration and source-freeze release; this lane made no source edit.
