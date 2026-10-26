# XML/property omission audit: next reachable ShapeOptim save branch (checkpoint 50)

## Finding

The remaining C13 XML grammar candidates do not yet identify a concrete corpus-triggered defect. `source/apps/moproj2mst/src/xml/dom.cpp` explicitly implements the supported Qt 4.8 XML subset and lists internal DTD entity declarations/general entity references as unsupported; the checked-in project corpus uses the simple `<!DOCTYPE ProjectFile>` form and predefined/numeric references. No examined corpus `.moproj` requires an internal entity declaration. Keep the grammar boundary documented, but do not widen it based only on general XML-library capability.

The next concrete property branch with an existing project trigger is ShapeOptim `properties vt+0x60` during Save-current. This is already a classified unsupported path in the current converter, so it is not a silent parser defect. It is a reachable property-family omission suitable for a bounded next implementation unit once its active-editor branch is confirmed by test setup.

## Corpus trigger and source path

`evidence/gui_mo/oracle_b/108_B_SOPTA_before/Gear_blank.moproj` contains an operation with `<Type>SfOprShapeOptim</Type>`, `Name=Shape Optimization`, `Id=3`, active subproject diagram state, and `<Properties Number="0"/>`. The paired `108_B_SOPTA` output is included in the historical B folder sweep. This proves the operation family exists in retained input evidence, but it does not by itself prove which callback result was taken in the historical comparator.

Current `src/engine/save_current.cpp::saveCurrentOperation` has an explicit ShapeOptim special path. When `updateDataInWidgetArea(p, op, fx)` is false, it calls `propsToEngine(p, op)` before `saveAfterFiles` and `saveOperationFiles`. The generic `propsToEngine` dispatcher has no ShapeOptim handler and reaches `M2M_E_UNSUPPORTED` for the `properties vt+0x60` family. With a shown editor, the true branch calls `propsLoad` instead; preserve this branch distinction and original ordering.

The retained L4 specification identifies original ShapeOptim property `vt+0x60` at `0x140142d00` as copying `MaxSims` and `MinObjFuncVal` from property data into operation-class fields `+0xcc` and `+0xe0`. This is a direct static contract, not a new runtime observation. The broader historical comparator is diagnostic and can log differences/refusals without failing its CTest.

## Smallest concrete implementation/test branch

The smallest source change should implement only the false-editor-update property copy:

- Input: current `SfOprShapeOptim`, valid parsed `ShapeOptimProps`, and `updateDataInWidgetArea == false`.
- Expected model effect from the L4 spec: `MaxSims` → operation `+0xcc`; `MinObjFuncVal` → operation `+0xe0`.
- Ordering: apply this copy before ShapeOptim `saveAfterFiles` and `saveOperationFiles`; keep true-editor-update behavior on `propsLoad`.
- Focused test: start from the existing `108_B_SOPTA_before` class/property shape, populate nondefault values for those two fields in a copied in-memory project, force the non-editor branch, assert the two class fields, then assert both downstream Save slots were reached in their existing order. Add the complementary editor-update-true case to ensure it uses `propsLoad` and does not take the property-to-engine path.

The fixture currently has `Properties Number="0"`, so it triggers the operation-family dispatch but does **not** provide nondefault XML values for a changed-value assertion. The focused test must use a copied/synthetic property payload consistent with the original L4 schema, while the project-level trigger remains the real `108_B_SOPTA_before` operation. Do not claim the historical case proves changed values or original parity.

## Open/Save and parser qualifications

- The current loader parses properties into class-specific data before the `Save-current` dispatcher. The candidate branch is a writer/operation property mapping, not a claim that XML parsing itself loses these values.
- Existing Convert2Dto3D and CopyMirror call crosswalks are already covered in `resume46-copy2d-property-crosswalk.md` and `resume46-copymirror-first-consumer.md`. Convert2Dto3D's load-time `+0x60` application is already implemented; CopyMirror has no recovered Open `+0x60` call. Do not reopen either as an assumed missing XML hook.
- The historical 108 case and current comparator do not prove the false-editor-update condition. Establish it in a focused source test or future original-qualified evidence before describing the branch as taken by that historical run.

## Evidence basis and limits

Read-only basis: `analysis/closure-audit.md` and `resume46-inherited-audit.md` (C13 candidates); `analysis/resume46-dispatch-audit.md` (ShapeOptim dispatch gap); original L4 `source/docs/re/moproj2mst/mdt/L4/spec.md` section 9; `evidence/gui_mo/oracle_b/108_B_SOPTA_before/Gear_blank.moproj`; current `src/xml/dom.cpp`, `src/engine/save_current.cpp`, `src/engine/shapeoptim_load.cpp`, and `src/engine/save_current.cpp` generic property dispatcher. No source, build, GUI/app-control, converter execution, or mutable REA action was performed.
