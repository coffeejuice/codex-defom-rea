# CP69 — Report document parented QObject constructor boundary

## Decision

The native Report document constructor has a statically described prefix, but that prefix cannot be represented as an executable Report Open effect with the current engine ownership APIs. Those APIs either record a final QObject counter for an already-bound widget/object, model a specific QWidget/ViewManager parent relationship, or collect inert external observations. None owns the Report document's shared Qt state, native thread identity, non-null parent transition, or dual subproject/main-window lifetime.

Keep `ReportClass::loadClassData` guarded before native document construction. Do not add a Report constructor helper that allocates only the C++ document model or commits only the QObject counter.

## Native earliest effects and ordering

Retained `evidence/gui_mo/s1/mdt/L7/d3.c` (`FUN_1400b5ad0`) shows the first Report-specific state transition. It reads `SP+0x30`; if null, it checks the subproject mode, allocates `0x70` bytes, obtains the main-window object, invokes `0x140643100(document, mainWindow, mode, mode != 2, 0)`, then stores the returned pointer in the `SP+0x30` slot and reads it back. A populated slot is reused. This is a lazy document allocation and publication in the subproject slot, before `.DS` reset/read.

The bounded CP63 constructor capture (`analysis/resume63-report-viewer-bounded-capture.txt`) establishes that `0x140643100` first calls the Qt `QObject` constructor with its parent argument, then installs the document vtable, initializes document members, calls `0x1406433a0`, registers seven connections to the main-window object at `0x143d76700`, and returns. The Qt base therefore precedes all document-specific member initialization and connection registration. The parent passed by the Report accessor is non-null.

Static QtCore evidence describes public QObject base setup before returning: vtable/private allocation and initialization, shared empty-container reference increments and reciprocal object/private links, current-thread selection and QThreadData reference increment, then the process QObject counter. The Report-specific parent is not the null-parent DOE task path in `analysis/wave2-doe-task-public-qobject-ownership-contract.json`; its non-null parent branch adds parent validation and child publication/callback behavior. The static Report constructor capture does not show that QtCore branch's actual runtime parent/thread profile or all side-effect return/failure outcomes.

After the base call succeeds, constructor member writes and seven connection registrations are visible. Their connection metadata addresses are retained in the constructor capture, but the connection signals/slots' behavior, partial-constructor unwind, and destructor disconnection are not established. The accessor's slot store happens after the constructor returns, so a failure in base/member/connection construction cannot simply be modeled as an already-published slot.

## Existing ownership API comparison

- `QObjectConstructionRecord` in `include/m2m/engine/view_manager.hpp` explicitly represents only a counter effect for one actual native QObject; its comment disclaims construction, thread binding, parenthood, and destruction. `WidgetRuntimeState::commitQObjectConstruction` in `src/engine/view_manager.cpp` increments `qobjectDelta_` only after the caller has already established the actual owner and thread binding. It cannot stand in for the prior native sequence.
- `WidgetRuntimeState` has no shared QList/QString/QVector refcount owner and no actual QThreadData identity/reference-count owner. `analysis/wave3-doe-report-qobject-prerequisite-contract.json` records these as unclosed; its Report QObject path is static-only and uses a null parent. `report_qobject_ledger.hpp` validates externally supplied observations but is explicitly inert and performs no native/source state commit.
- `GridLayoutState::beginParentedConstruction` and `ViewManagerState` model a specific proposed `WidgetState` and a source-bound QWidget layout/parent transfer. The grid path itself leaves parent assignment pending and refuses at the uncovered child-event/parent hooks. It is not a general parented QObject factory and does not provide Report document ownership in `SubProject+0x30`.
- `MainViewState`/`ViewManagerState` provide source-specific owners and child collections for known GUI types. The Report document has no corresponding model slot/owner, and a matching main-window pointer alone would not close Qt QObject private state, thread reference ownership, Report slot publication, constructor connections, or destruction order.

So no existing API spans even the first complete constructor prefix: shared Qt ref effects → private backlinks → TLS/QThreadData binding/ref → process counter → parent validation/child link/events → Report member initialization → seven connections → post-return slot publication.

## Exact missing evidence and integration requirement

The minimum new evidence is a bounded fresh-process capture of the actual Report document's `QObject(parent=mainWindow)` call, with the exact parent object identity and one constructor ordinal bound across:

1. Shared empty-container reference deltas and object/private backlinks.
2. TLS index/value and selected QThreadData identity/refcount on the same actual thread.
3. The non-null parent validation, parent assignment, child-list publication and callbacks, with the main-window child collection state before/after.
4. Process QObject counter ordering, then document-specific member initialization and all seven connection registrations through return.
5. Any failure/unwind branch needed to determine whether the partially built object remains allocated or is released before `SP+0x30` publication.

Separately, a native SubProject/Project Close trace must establish how the shared `SP+0x30` slot is cleared/released relative to Qt parent-child deletion and QObjectPrivate/QThreadData teardown. Until these effects share a root-owned Qt runtime API and an actual Report document owner, the Report Open refusal is the correct integration boundary.

## Scope

Read-only analysis of retained source and static evidence. No source edits, builds/tests, GUI control, runtime capture, or mutable REA action.
