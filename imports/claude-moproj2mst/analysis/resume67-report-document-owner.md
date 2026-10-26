# CP67 — Native Report document adoption and Close ownership

## Result

The original Report Open path reaches a lazy document accessor owned by the active subproject, but the retained evidence does not establish a complete Close ownership contract. In particular, it does not show when the `SubProject+0x30` slot is released or whether the main-window QObject parent is the sole lifetime owner. The candidate `.DS` helper is only filename derivation; it does not perform this adoption or cleanup. Keep the current Open/Save refusals.

## Original Open dispatch and adoption sequence

The retained L7 decompilation in `evidence/gui_mo/s1/mdt/L7/d1.c` establishes this chain:

1. `FUN_1412add20` (Report `vt+0x58`) obtains the Report class's active subproject (`FUN_1412b1010`), current project (`FUN_1412b0fc0`) and operation table. If these are present, it calls the Report object's virtual `+0x78` slot before viewer/UI work.
2. `FUN_1412ae070` (Report `vt+0x78`) gets the project filename, derives a sibling `<base>.DS` path, then calls `FUN_1412b0f70`. It passes the resulting document to `FUN_140644fd0` (reset), `FUN_140645dc0` (path/read), and `FUN_140644440` (document traversal), in that order.
3. `FUN_1412b0f70` in `evidence/gui_mo/s1/mdt/L7/d2.c` resolves the active subproject with `FUN_1412b1010`, then calls `FUN_1400b5ad0`. A null subproject or null accessor result returns null.
4. `FUN_1400b5ad0` in `evidence/gui_mo/s1/mdt/L7/d3.c` checks the existing slot at `SP+0x30`. If empty, it reads the subproject mode, allocates `0x70` bytes, obtains the main-window pointer through `0x1402c8090(0x143d766b8)`, calls `0x140643100(document, mainWindow, mode, mode != 2, 0)`, and stores the returned pointer into `SP+0x30` through `0x140cbc920`. It then reads/returns the slot. If already populated, it reuses the existing slot rather than allocating again.
5. The retained constructor capture for `0x140643100` shows it calls the QObject constructor with the supplied parent, installs its vtable, initializes document members and registers seven main-window connections. This is an ordered GUI-object construction path, not a value-only document initialization. The current CP65 capture does not identify the connection semantics or unwind behavior.

The adoption point is therefore the native subproject slot itself: Report Open asks the active subproject for a lazily created, main-window-parented document object and stores that pointer in `SP+0x30`. The CP65 `reportDsPath(Project.path)` helper implements only the path expression. The current source `ReportClass::loadClassData` refuses before calling an equivalent accessor, and the path helper has no production call sites; no current `Project`/`SubProject` field adopts the native object.

## What Close ownership remains unproved

The available `FUN_1400b5ad0` body shows get, conditional allocation/construction, store, and get/return. It does not show a clear/reset operation, destructor invocation, or the owner's final release. `0x140cbc920` and `0x140c1aef0` are retained as opaque shared-slot operations in this evidence; their exact pointer wrapper/refcount semantics are not qualified here. The QObject parent argument proves the constructor receives the main window as parent, but the available excerpt does not establish whether that parent deletes the document on the relevant Close path, when the subproject slot drops its reference, or the order between those events.

Current adapter teardown is not a substitute: `src/model/subproject_lifetime.cpp` only records the adapter flow-receiver death, and `ConversionSession::releaseProjectStorage()` is explicitly described in source as exception/final adapter cleanup rather than native Close replay. No Report-document destructor body or native subproject/Close destruction sequence is present in the retained receipts inspected for CP65/66.

## Lifecycle boundary and next evidence query

For current-source reachability, `engine::openActiveOperation` invokes the active class's `loadOperation`; Report `loadOperation` enters the guard in `src/ops/report.cpp` before native document adoption. Thus current public Open cannot reach the native slot sequence, and the subsequent public Save branch is not reached for an active Report. Independently, public Save retains its Report guard in `src/engine/save_current.cpp`.

The smallest next static query is a bounded immutable-PE xref/release trace for the `SP+0x30` slot: identify the concrete native SubProject destructor and normal Project Close callchain, then follow the slot's shared-pointer release operation and the main-window QObject child teardown far enough to determine which event releases the document and in what order. Include the exact slot nulling/reassignment sites and the destructor target reached on last-reference release. Until that is qualified, do not add a source owner field or claim that QObject parenting alone completes Close semantics.

## Evidence scope

- `evidence/gui_mo/s1/mdt/L7/d1.c`: `FUN_1412add20` and `FUN_1412ae070`.
- `evidence/gui_mo/s1/mdt/L7/d2.c`: `FUN_1412b0f70`.
- `evidence/gui_mo/s1/mdt/L7/d3.c`: `FUN_1400b5ad0`.
- `analysis/resume63-report-viewer-bounded-capture.txt`: bounded `0x140643100` constructor body through its return.
- `analysis/resume65-report-ownership.md`, `analysis/resume66-report-owner.md`: prior owner and reachability limits.
- `src/ops/report.cpp`, `src/ops/report_ds_path.cpp`, `src/model/subproject_lifetime.cpp`, and `src/engine/conversion_session.cpp`: current-source guards, helper isolation, and adapter teardown boundary.

Read-only source/retained-static-evidence audit. No source edits, build/tests, GUI control, or mutable REA operation.
