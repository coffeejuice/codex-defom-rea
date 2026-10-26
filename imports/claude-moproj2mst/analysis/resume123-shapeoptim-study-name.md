# CP123 ShapeOptim study-name owner increment

## Source closure

`ShapeOptimClass::studyFolder` (`0x14013d180`) reads the active sub-project through the current operation's Project. The adapter has one source-qualified nonempty-name owner that matches this field chain:

- `makeSubProject(p, 2, id)` creates `ownedDoeStudy = doe::makeStudy(...)` and aliases `doe` to that exact owner (`src/model/subproject.cpp:293-306`).
- `doe::makeStudy` binds `Study::sub` to that SubProject and initializes `Study::name` to `DoeStudy<sub.id>` (`src/doe/doe_study.cpp:120-128`).
- The Type2 lazy reader later replaces that same name from `<SfDoeStudy name="...">` before using it to form the `Control` input paths (`src/doe/activation.cpp:32-35`; `readStudyElement` in `src/doe/doe_study.cpp:855-858`). Empty imported names therefore are possible and remain outside this increment.
- `SubProject` stores the `doe::Model*` base alias at `+0x20` and shared ownership of the concrete `doe::Study` separately (`include/m2m/model.hpp:273-274`).

The implementation now consumes `ownedDoeStudy->name` only when the caller Project is the current operation's Project, the active sub-project's `project` points back to it, `type == 2`, the `doe` base pointer aliases the retained `ownedDoeStudy`, the Study's `sub` backpointer matches, and the name is nonempty. Any nonnull alias outside that exact shape remains guarded. The null-DOE path still uses the existing empty study-name behavior. Type3 has no `doe` alias and its earlier `loadClassData` guard is unchanged.

The resulting path remains the existing `projectDir / DoeStudies / name / Control / Opt` construction, followed by the CP122 immediate/deferred directory behavior. This is a bounded source-owned value branch; it does not qualify native QString ownership/refcounts, arbitrary malformed imported names, Type3's separate optimization-study object, GEO payload import, Save admission, or overall ShapeOptim support.

## Focused coverage

Added a real `ShapeOptimClass::loadData` case that converts initialized SUB0 into the source's Type2 owner shape with `doe::makeStudy`, retains the exact `doe` alias/backpointer, registers the class in the Project registry, and checks one successful directory attempt at the generated nonempty `DoeStudy0` path. The prior null-DOE test remains and continues to exercise the empty-name CP122 path. Both paths use the PID-specific canonical `C:\PROBLEM\M2M_SO_<pid>` test root, verify resolution before leaf cleanup, and check every generated path is under 100 characters.

No test/build was run in this lane. `git diff --check` passed. Existing public Type3/Report guards, the later GEO guard, and the failed-mkdir modal boundary remain unchanged.

## Frozen source/test hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/shapeoptim_load.cpp` | `1428FD84E341CCBA66D1203243E1836A32A0292FFA44898D927378FAADCB1367` |
| `source/apps/moproj2mst/tests/mdt/test_shapeoptim_study_folder_effect.cpp` | `7C9C298509EB563D3DE65D5C645B0C431163876258AB16077DD13F4F320ED2FD` |
