# Resume 46 ShapeOptim dispatch implementation receipt

## Result and correction to prior audit

The initial dispatch audit missed an existing class-specific implementation. `save_current.cpp::propsToEngine` delegates to `opBase(op)->propsToEngine(p, op)` before generic class-key handling. `ShapeOptimClass::propsToEngine` in `source/apps/moproj2mst/src/ops/report.cpp` calls the source-qualified `ShapeOptimClass::propsToClass`; that copies `ShapeOptimData::maxSims` and `minObjFuncVal` into class fields `fcc_` (+0xcc) and `fe0_` (+0xe0), respectively. The original native behavior is at 0x140142d00. The dispatch gap alleged in `analysis/resume46-dispatch-audit.md` is therefore not present in current source, and no implementation of that mapping was needed.

The source change is a comment correction in `save_current.cpp` documenting ShapeOptim alongside the already delegated class-specific property bodies. The focused test file now covers the ShapeOptim Save override on both sides of `updateDataInWidgetArea`: false must invoke the property-to-model callback before the ShapeOptim after-files/files slots; true must invoke propsLoad and must not invoke the direct copy. The test callback records the provided ShapeOptim values (37 and 0.125) to establish that the false branch delivered the typed properties object. Existing true-branch expectations are retained.

## Changed files

- `source/apps/moproj2mst/src/engine/save_current.cpp` — comment documents the already-existing ShapeOptim dispatch route.
- `source/apps/moproj2mst/tests/s/test_save_dispatch.cpp` — typed ShapeOptim fixture and false/true editor-update cases.

No headers, CMake files, other source, or analysis state were changed. No build or test was run, per instruction; root owns integration and verification.

## Acceptance expectation

Root should build and run `m2m.s.save_dispatch`. The false case should record `editor-index, props-to-model, after-files, files` and values `37`, `0.125`. The true case should record `editor-index, apply, props-load, after-files, files`, leaving the direct-copy observations at their sentinel values. Native mapping itself remains qualified by `source/apps/moproj2mst/src/ops/report.cpp` and `source/docs/re/moproj2mst/mdt/L4/spec.md`; this mock dispatch test does not independently exercise private `ShapeOptimClass` fields.
