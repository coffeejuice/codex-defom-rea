# Next reachable operation-family override after Distortion and ShapeOptim

## Finding

The next clear unsupported operation-family override is **SfOprReport `loadOperation` / `loadClassData`**. Report is in the same retained L7 override family inventory as Cycle, DoeOutput, Group, and ShapeOptim (`src/ops/l7.hpp`). ShapeOptim now has a dedicated integrated loader, while Report's source still stops before the native report-document open and following UI path.

## Public reachability and exact guard

`engine::openActiveOperation` chooses the active diagram item, resolves its operation and class, establishes `current`, builds/resets the model, then dispatches `classOf(*op)->loadOperation(...)` (`src/engine/loader.cpp`, `openActiveOperation`, about lines 1052–1077). Thus a selected `SfOprReport` reaches its override during the public project-open/activation path; this is not an unreachable helper.

In `src/ops/report.cpp`, `ReportClass::loadOperation` validates an active sub-project and project, calls `loadClassData` (`vt+0x78`), then has a second explicit refusal for the steps after that slot. The first public barrier is inside `ReportClass::loadClassData`: after verifying project and sub-project presence, it refuses before constructing/reading the GUI-owned report document:

```text
operation load: SfOprReport 0x1412ae070 reads the report document (SP+0x30, 0x1400b5ad0 -> 0x140643100, <project>.DS), a GUI object that is not reproduced
```

If that boundary is later closed, the following refusal in `loadOperation` is also material:

```text
operation load: SfOprReport 0x1412add20: the steps after vt+0x78 are not reproduced
```

The source comment for the latter identifies the later coupon geometry display (`0x1412ae3d0`), report UI fill (`0x1412b0660`), and main-window signal effects. Do not delete or broaden either guard without source-qualified coverage of the reached branch.

The adjacent `ReportClass::propsLoad` is a later distinct guard for the Report Properties `vt+0x58` common-block readers (`0x1405fb720`, `0x1405fbae0`, `0x1405fc180`). It should not be presented as the first Open blocker. `analysis/takeover-state.json` separately records the root-owned Report QObject TLS/shared-ref prerequisite and instructs that later effects remain guarded until actual TLS/QThreadData identity evidence is available.

## Why this follows the override inventory

- `class_slots.cpp` explicitly documents that Group, Cycle, DoeOutput, ShapeOptim, and Report override `loadOperation`; Group/Cycle forwarding behavior and ShapeOptim have their own source implementations.
- `src/ops/report.cpp` still contains the explicit Report `vt+0x58`, `vt+0x78`, and Properties `vt+0x58` refusals above.
- Root queue item `ROPS` in `analysis/takeover-state.json` says ShapeOptim dispatch is integrated and requests auditing the next family override/public branch. The state separately names actual fresh-process Report TLS index/value, shared-null refs and QThreadData identity/refcount as unresolved prerequisites.

Therefore Report is the next *reachable guard to audit*, not a claim that implementation can immediately continue through the whole Report path. The concrete next action is a narrowly bounded Report load trace: bind the active Report case and `SP+0x30` document creation/read effects, then satisfy the already-recorded TLS/shared-ref prerequisite before implementing QObject or later UI effects. Preserve the current refusal at the first unqualified state transition; once it is qualified, re-evaluate the second guard before moving further.

## Evidence consulted

- `source/apps/moproj2mst/src/engine/loader.cpp` (`openActiveOperation` dispatch)
- `source/apps/moproj2mst/src/engine/class_slots.cpp` (base vs class override boundaries)
- `source/apps/moproj2mst/src/ops/l7.hpp` (L7 family)
- `source/apps/moproj2mst/src/ops/report.cpp` (Report guards and later effects)
- `analysis/takeover-state.json` (`ROPS` next action and Report TLS/QThreadData blocker)
- `analysis/wave3-doe-report-qobject-prerequisite-contract.json`
- `analysis/wave3-doe-report-qobject-capture-plan.json`

Read-only source/receipt audit. No implementation, build, GUI, or mutable REA database was touched.
