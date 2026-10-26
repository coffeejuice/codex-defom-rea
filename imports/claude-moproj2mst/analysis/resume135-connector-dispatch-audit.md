# CP135 connector and operation/property dispatch audit

## Finding

I found no source-closed production connector or operation/property dispatch omission to integrate. The requested-connector parser now has a real complete-project candidate check and focused graph tests. The remaining narrow, source-closed increment is a **test-only registry flag inventory**: the current tests exercise the connector roles and selected class flags, but do not pin every one of the 28 registered classes' `dimFlags` values in the real registry. This would detect a future class-init/constructor regression without changing production dispatch or claiming original whole-project parity.

## Current connector dispatch

`src/model/subproject.cpp` implements the source path in three relevant stages:

1. `connectorType` derives input/output connector names from the registered class's `dimFlags`; it handles the asymmetric `0x10000000` case before the ordinary dimension bits.
2. `connectorAccepts` allows equal connector names and the native combined `Control2d3d`/single-dimension pairs.
3. `parseSubProject` tests type admission before target-input occupancy, then calls `connectItems` for each requested source edge once.

`tests/p03/test_connector_contract.cpp` covers the 4×4 legacy class compatibility matrix, occupancy and parse order, Convert bridge, the combined Cycle/DoeOutput roles, Cogging’s 3D role, the retained Cogging→DoeOutput relation, and self-edge admission. `tests/mdt/test_connector_active_project.cpp` parses the retained full `120_B_DOE` project, explicitly activates its second subproject, and checks the real registry flags and graph relation for operation 3 Cogging→operation 6 DoeOutput. Its own source comment correctly limits it to candidate-only parsing/activation: it does not invoke operation loading, Save, or original GUI behavior.

The retained 16-case original probe is still admission-helper evidence only: the probe substitutes the class-type predicate and owner-index callbacks. `analysis/resume53-connector-acceptance.md` correctly keeps full original project parity false and calls for a fresh original/candidate graph capture. Nothing in the current source or test coverage justifies widening that claim.

## Smallest disjoint source-closed test cut

Add one table-driven assertion to `tests/p03/test_connector_contract.cpp` that constructs the real registry with `makeRegistry` and checks each registered key’s exact `dimFlags`. Current source contains 23 explicit entries in `src/model/classinit.cpp`; the remaining registered values are constructor-owned in `ops/multiblow.cpp`, `ops/convert23.cpp`, and `ops/boolean.cpp`. The expected mapping, in the registry's current 28-key inventory (`tests/p02r/test_registry.cpp`), is:

| Flags | Registered classes |
|---|---|
| `0x1a` | Forming, HeatTransfer, HTFurnace, Cogging, Distortion3D |
| `0x19` | Forming2D, HeatTransfer2D, Distortion2D |
| `0x16` | HeatTransferExpress, FormingExpress |
| `0x15` | HeatTransferExpress2D, FormingExpress2D |
| `0x2a` | HeatTreat, MultiBlow3D |
| `0x29` | HeatTreat2D, MultiBlow2D |
| `0x5a` / `0x59` | Pre3D / Pre2D |
| `0x40097` | ShapeOptim |
| `0x4000009d` / `0x4000009e` | DiestressBM2D / DiestressBM3D |
| `0xdf` | Diestress |
| `0x9b` | Report |
| `0x301b` | Cycle |
| `0x20097` | DoeOutput |
| `0x1100009d` | Convert2Dto3D |
| `0x1000099` / `0x100009a` | Boolean2D / Boolean3D |

The test should assert the exact map size and every key/value; then derive the resolved input/output role for each value using the same rules already covered in P03 and pin those expected names, especially asymmetric flags and `Control2d3d`. Keep `SfOprGroup`/`SfOprCopyMirror` excluded because they are unregistered. This is source-closed against the current evidence and non-overlapping with parser implementation; no CMake change is needed if added to the existing target.

## Why no production branch is selected

The property-write inventory in `resume74-operation-dispatch-next.md` found no source-closed missing step-4 family after Cogging, Boolean, L6, and Express. Active DoeOutput Save remains behind the `save_current.cpp` guard because the same-occurrence refresh callback and process-global DBLIST `+0x1c` state are unbound; Report remains dependent on its viewer/document owners. Those are lifecycle dependencies, not omitted connector-admission branches. The separate native connector-vs-active-operation question also remains: the retained Cogging→DoeOutput edge lives in Sub_Project 1 and is only reached in the candidate integration test by explicit activation; current public conversion starts with subproject 0. Do not add an ABI selector or relabel the internal activation test as public parity.

## Recommendation

If a useful immediate RCONNECT action is desired, pin the full 28-key registry flags and decoded connector roles in the existing P03 test file, then have the root build/test that focused target. For a public behavior advancement, the exact blocker is a fresh original/candidate Open graph capture for the requested Cogging→DoeOutput project edge, plus active-subproject selection evidence. Save/Close claims require their separately documented DoeOutput receiver/DBLIST and Report owner dependencies.

Read-only source/report audit. No source, build, test, fixture, GUI, or mutable REA state was changed or executed.
