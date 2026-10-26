# CP90 — BooleanData alias boundary and bounded guard proposal

## Conclusion

The native owner backlink is a single `BooleanProps*` at `BooleanData+0x10`; the candidate currently models only `BooleanProps::data` (a `shared_ptr<BooleanData>`) and the separate base `Props::owner` operation pointer. The ordinary source paths create or lazily allocate data on the owning properties object and do not share it. Thus the CP89 false-positive shape is malformed-model behavior, not a normal-path discrepancy established by current source.

A narrow source-backed guard is available without changing the data layout: during `nativeBooleanSideFileSequence`, scan `project.operations` and require that exactly one live `BooleanProps` in that project references the supplied `BooleanData`, and that it is the already validated `properties`. Refuse a duplicate before either callback. This preserves normal factory/reader/Save behavior and prevents two project-owned properties objects from both claiming one data object. It does not prove absence of aliases held outside the supplied Project; the helper should remain qualified to the represented Project graph. Do not encode an external alias as native `-1`.

Adding a raw `BooleanProps*` field to `BooleanData` is not justified as the first fix. It would be closer to the native layout, but requires binding at both allocation sites, handling rebinding/copying, and defining lifetime behavior for a raw pointer. The current model has no owner-aware data abstraction or weak lifetime guard at that layer. No source evidence establishes those semantics. Recommend a Project-local uniqueness scan and a focused negative test only, subject to root decision; no files were edited and no tests were run here.

## Source evidence

- `BooleanData` contains only scalar values; its comment identifies native `+0x10` as the omitted owner backlink (`include/m2m/data/boolean.hpp`, SHA-256 `6889030046a7ab2838db4b537695576b103824241823634932b13a186893b378`).
- `BooleanProps` contains `shared_ptr<data::BooleanData> data`; base `Props` has `Operation* owner` (`include/m2m/props.hpp`, SHA-256 `b899939c3cd88061e4be75014e475ff73f4c1c562b0450c806a66096cf3c7bf3`). These represent different relations: data-to-properties and properties-to-operation.
- `BooleanClass::makeProps` creates a new `BooleanProps`, then a fresh `BooleanData` (`src/ops/boolean.cpp`, SHA-256 `4d881b968ca13eb9e78fb57114b4e062358f870e2a075f7c9ce7740cecb2ab8e`).
- `readBooleanBwz` allocates `BooleanData` only when `props.data` is null and then fills that same object (`src/datafiles/wizards.cpp`, SHA-256 `ed4dbcd5eaed65f3083758909129fd51b7b1d0d0a45e2692eb0bd5bab95841af`). It does not replace a non-null object or attach it elsewhere.
- `setOprProps` assigns the properties to one operation and calls `setPropsOwner`; `resetOprInfo` clears that operation's properties (`src/model/opinfo.cpp`, SHA-256 `47d74b46837e0462ae9077ea88978424cd049a33af714f80d5ea6f8e30e4342e`). The rescue path calls this setter after `makeProps` (`src/model/operation_rescue.cpp`, inspected read-only).
- The side-file helper validates the expected operation/property/data tuple and Project membership but does not count other project properties that share the same data (`src/engine/boolean_side_file_result.cpp`, SHA-256 `cf8d9eee2ab0b042d7418617554cf4ba7e06dd90132bf0dd8dba50e5e6941330`). This leaves the constructed CP89 alias shape admissible.
- Current side-file tests cover a different data object and ownership mismatches, but not a second `BooleanProps` sharing the same `shared_ptr<BooleanData>` (`tests/mdt/test_boolean_side_file_result.cpp`, SHA-256 `e85c2e7d111177bce8619bc624402062e732669ba6df88452a6c4d04981d4fae`).

Native qualification in `analysis/resume89-boolean-side-file-result-review.md` records the original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and retained `s1-04/spec.md` behavior: `BooleanData+0x10` selects one owning properties object, whose guarded operation is resolved before the BWZ/INI sequence. This report does not extend the native contract to aliases outside a candidate Project.

## Proposed exclusive patch if approved

Target only `source/apps/moproj2mst/src/engine/boolean_side_file_result.cpp` and `source/apps/moproj2mst/tests/mdt/test_boolean_side_file_result.cpp`:

1. After validating Project membership and before resolving callbacks, iterate `project.operations`, dynamically identify `BooleanProps` with non-null `data` whose `data.get() == &data` argument, and count matches.
2. Continue only when the count is exactly one and the matching properties address equals the validated `properties.get()`. Otherwise throw the existing unsupported error before attempts. Keep current `-1` handling for absent owner/expired guard unchanged.
3. Add a test with two project operations, two distinct owner-consistent `BooleanProps`, and one deliberately shared `BooleanData`; call with the second operation and assert unsupported plus zero callbacks. Retain an existing valid fixture assertion to show the ordinary one-to-one path remains admitted.

This is a structural invalid-model guard, not a new native return branch. It requires no production writer changes, no data-layout change, and no CMake change. Root should decide whether the standalone helper's contract warrants rejecting malformed Project-local aliases. No build or test verification was performed for this review.
