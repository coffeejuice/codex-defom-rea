# Checkpoint 46 Cycle/DoeOutput connector regression design

## Inspection result

The specifically assigned `tests/mdt/test_connector_type.cpp` does not exist. The existing regression is in `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp`, in `cycle_and_doe_output_have_original_combined_connectors`.

That test already exercises the needed focused source-level regression: its helper calls `makeRegistry(p)` before `parseSubProject`, then tests both `SfOprCycle` and `SfOprDoeOutput` as bridges between 2D and 3D Forming items. It checks incoming/outgoing links and confirms a later compatible incoming edge cannot take the occupied input. The behavior depends on the registry initializers, so this is stronger than directly constructing classes or manually linking graph nodes. The code comments bind the flags to original initializers: Cycle `0x301b` at `0x14134d2e0`; DoeOutput `0x20097` at `0x140e2df30`. `classinit.cpp` contains those exact entries.

No additional test should be added solely to duplicate this case. The assigned MDT path is absent, and creating it would not wire into CMake or the existing test harness under this task's ownership boundary. The test also properly stops at parser/link state; it does not traverse the resulting graph or imply DOE study serialization.

## Root-owned next action

Run the existing `m2m.p03.connector_contract` test and integrated regression against the final source tree, recording the source-tree hash and logs. Earlier v2 build receipts predate this two-case addition, so they do not bind the current test to a completed build. Keep any subsequent production Open/Save/Close comparison and DOE side-file acceptance as separate lifecycle evidence.

## Evidence consulted

- `source/apps/moproj2mst/tests/p03/test_connector_contract.cpp`
- `source/apps/moproj2mst/src/model/classinit.cpp`
- `analysis/connector-implementation-review.md`
- `analysis/connector-admission-fixed-v2-build.json`

Read-only inspection only; no source, CMake, build, original application, or REA database changes were made.
