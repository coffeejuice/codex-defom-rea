# Checkpoint 49 operation registry and connector dispatch audit

## P03 regression static review

`cogging_connects_to_doe_output_with_original_combined_input` is structurally valid for the verified edge. The helper calls `makeRegistry`, whose insertion calls `classInit` before storing each class; the parser then maps XML class names to `FlowItem::className` and checks the registry-backed flags at each requested edge. `Project::activeSub` defaults to 0, matching the single sub-project created by the helper. The assertion checks the actual `next` and `prev` links for requested indices.

There is one narrow assertion limitation: the target DoeOutput input is combined, so it accepts either a 2D or 3D source. Thus this test distinguishes DoeOutput's corrected combined input from its former default-zero/single-2D state, but does not independently establish Cogging's `0x1a` 3D output; a wrongly initialized Cogging as 2D would still be accepted by a combined target. The comment's Cogging flag statement is supported by the initializer table and independent review, but this particular assertion does not test that source-side contract.

## Evidence-backed dispatch coverage gap

The shared dispatch chain is: `makeRegistry` -> `classInit` -> key-indexed registry -> `parseDiagramItem` class mapping -> `connectorType` registry lookup -> `connectorAccepts` -> `connectItems`. Current registry tests (`tests/p02r/test_registry.cpp`) assert membership and case-sensitive `opBase` lookup, not each key's connector flags. Current P17 flag tests assert a subset; P15 covers the FormingExpress pair, and P12 covers Boolean/Convert/CopyMirror constructor flags. The independent connector review explicitly says its audit establishes initialization source coverage but does **not** independently validate all 28 original flag constants. That leaves the class-init-to-connector dispatch table only partially pinned by executable regression assertions.

An exact flag expectation table for every registered key is the smallest next action: assert that calling `classInit` through the real registry preserves the evidence-derived flags, and assert the resulting connector type pair for keys whose flags use special bits. The existing P17 test file is the natural owner for class initialization constants; the current connector contract test should retain focused behavior cases for representative special cases. In particular, add a discriminating Cogging check: `SfOprForming` (3D) -> `SfOprCogging` should admit, while `SfOprForming2D` (2D) -> `SfOprCogging` should reject. This uses the same real registry/parser helper and would fail if Cogging silently inherited zero flags (interpreted as 2D) or was initialized with the wrong dimension.

The explicitly unregistered `SfOprGroup` and `SfOprCopyMirror` boundary is not an unresolved production defect in these sources: `registry_rescue.cpp` documents and implements their absence, and P02 checks both missing keys. Do not add them to `makeRegistry` merely to fill the test matrix; their class dispatch would change from the documented original failure behavior.

## Exact scope limits

The 16-case native probe remains an admission-function test with mocked type and owner-index callbacks. It cannot close the missing class-flag assertions, execute class lookup or GUI parsing, or validate Save. The P03 test is converter source evidence only. A project-level original/candidate comparison for DOE operations remains separately qualified because active DoeOutput loading/Save has retained unsupported branches.

## Evidence consulted

- `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp`
- `source/apps/moproj2mst/tests/p02r/test_registry.cpp`
- `source/apps/moproj2mst/tests/p15/test_classinit.cpp`
- `source/apps/moproj2mst/tests/p17/test_classinit.cpp`
- `source/apps/moproj2mst/tests/p12/test_p12_writers.cpp`
- `source/apps/moproj2mst/src/model/registry_rescue.cpp`
- `source/apps/moproj2mst/src/model/classinit.cpp`
- `source/apps/moproj2mst/src/model/subproject.cpp`
- `analysis/connector-implementation-review.md`
- `analysis/connector-admission-original.json`

Read-only audit. No source, build, GUI, project fixture, or REA database was modified.
