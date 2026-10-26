# ShapeOptim Save property copy contract (checkpoint 51)

## Contract and exact fields

The retained L4 specification (`source/docs/re/moproj2mst/mdt/L4/spec.md`, §9) binds original `SfOprShapeOptimProperties` vt+0x60 at `0x140142d00` to two direct assignments when properties hold the originating ShapeOptim class at `+0x40`:

| Property data field | Destination operation-class field | Current named member |
|---|---|---|
| `ShapeOptimData::maxSims` (`data+0x08`, property `MaxSims`) | ShapeOptim `OB+0xcc` | `ShapeOptimClass::fcc_` |
| `ShapeOptimData::minObjFuncVal` (`data+0x10`, property `MinObjFuncVal`) | ShapeOptim `OB+0xe0` | `ShapeOptimClass::fe0_` |

This is a copy, not a swap or numeric conversion in the property slot. The separate vt+0x58 reverse copy (`0x140142ca0`) reads class `+0xcc/+0xe0` back to data `+0x08/+0x10`.

## Current implementation status and guard chain

The copy is already implemented in `src/ops/report.cpp`: `ShapeOptimClass::propsToEngine` requires an operation property object that dynamically casts to `ShapeOptimProps`, then delegates to `propsToClass`. `propsToClass` is a no-op if the class back-pointer `pr.cls` is null; otherwise it refuses null `pr.data`, casts the linked class, and assigns `fcc_=data->maxSims`, `fe0_=data->minObjFuncVal`. `makeProps` establishes the expected link (`p->cls=this`) and retains the weak property owner.

The Save-current route in `src/engine/save_current.cpp::saveCurrentOperation` first returns for absent class, stale/noncurrent operation (`ob->current != &op`), or missing `op.project`. For `SfOprShapeOptim`, the native ordering is:

1. call `updateDataInWidgetArea(p, op, fx)`;
2. if true, call `propsLoad` (properties vt+0x58, class→data);
3. otherwise call `propsToEngine` (properties vt+0x60, data→class);
4. call `saveAfterFiles` (vt+0x80);
5. call `saveOperationFiles` (vt+0xe8).

Generic `propsToEngine` first refuses `op.props == nullptr`; if class-specific `ob->propsToEngine` returns true, it returns before the generic property dispatcher. Therefore the ShapeOptim override handles the false-editor-update path. These are per-call preconditions and ordering. The public editor300 GUI path has separate constructor/wrapper gates and is not made complete by this Save-current helper.

## Existing tests and exact synthetic requirements

The current source already has two useful checks:

- `tests/mdt/test_shapeoptim_props.cpp::shapeoptim_props_class_link` uses the real `ShapeOptimClass`, sets nondefaults `MaxSims=7`, `MinObjFuncVal=0.25`, calls `propsToEngine`, then creates fresh props and calls `propsLoad` to verify the class round-trip. It also checks the null class-link no-op.
- `tests/s/test_save_dispatch.cpp::save_shapeoptim_without_editor_applies_shape_properties_before_save_slots` tests the Save-current false branch and ordering with nondefaults `37` and `0.125`, but uses `ObservedClass`, a test double. The editor-true case similarly proves dispatch order with that test double.

For an end-to-end concrete-class regression, use a real `ShapeOptimClass` from `makeShapeOptim()` and its real `makeProps()` result; set `maxSims=37` and `minObjFuncVal=0.125`. Bind it to an `Operation` with matching class key, project, and `current` pointer. Arrange the harness so `updateDataInWidgetArea` returns false without invoking unrelated geometry/UI paths, then call `saveCurrentOperation`. Create a second props object through the same class and call `propsLoad`; require `37` and exactly `0.125` to confirm the data→`+0xcc/+0xe0` copy happened. Also assert the reached suffix order is `saveAfterFiles` then `saveOperationFiles`; if concrete slot behavior cannot be instrumented without geometry prerequisites, retain the existing dispatch mock for order and use the concrete round-trip solely for the field-copy contract.

The complementary branch test should return true from editor update and verify `propsLoad` copies class values into data while `propsToEngine` is not called. Do not combine it with type-3 optimization-study, display/ROI geometry, or `.GEO` writer behavior; those are separately refused dependencies and do not qualify the two-field copy.

## Corpus trigger and evidence limits

`evidence/gui_mo/oracle_b/108_B_SOPTA_before/Gear_blank.moproj` includes `SfOprShapeOptim`, active Shape Optimization diagram state, and `Properties Number="0"`. The L4 spec identifies Oracle B 108 (the ShapeOptim-appended case) as passing the `save_current` and project writer path. This is an operation-family trigger, but its empty XML properties do not provide nondefault values and its historical comparator is not a fresh original runtime trace of the false-editor-update branch. Use synthetic nondefault properties for the focused value test; do not report the fixture as proof of changed-field parity.

This audit is static and read-only. It resolves the source contract and verifies existing test definitions, not execution. No source/build/GUI/mutable REA action was performed.
