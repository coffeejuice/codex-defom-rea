# CP71 — Report document owner edge

## Result

There is no source-closed state mutation to integrate for Report Open. The earliest native transition is inside `QObject(QObject*)`, before the Report document constructor initializes its fields. Current source has neither an owner for the object nor a model of the actual parented QObject effects. The `.DS` path helper is the only independently closed source piece, and it is pure string derivation.

## Native lazy creation and publication edge

Retained `evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`, shows:

1. Read `SP+0x30` through `0x140c1aef0`.
2. If empty, read the subproject mode; allocate `0x70` bytes; get the application main window; call `0x140643100(raw, mainWindow, mode, mode != 2, 0)`.
3. Only after that constructor returns, write its result into `SP+0x30` through `0x140cbc920`, then read/return the slot.

Therefore the first Report-specific publication into the owner slot is **after** successful QObject and Report-document construction. The earlier constructor already mutates QObject identity/private state, selects and references thread data, performs parent compatibility/parenting, advances the global QObject counter, initializes document members, and registers seven main-window connections. The immutable QtCore instructions and retained Report constructor capture specify order, but the actual parent/thread branch and constructor failure/unwind effects have not been observed for this object.

`FUN_1412ae070` reaches the accessor through `FUN_1412b0f70`, then calls the reset/read/traversal functions. Current `ReportClass::loadClassData` refuses before the accessor; `reportDsPath(Project.path)` does not call it or mutate owner state.

## Candidate source owner edge

The obvious candidate is a distinct Report-document owner on `SubProject`, because native access is scoped to the active subproject and the native slot is `SP+0x30`. That field cannot safely be backed by an ordinary owning `shared_ptr` yet: the retained getter/setter calls at `0x140c1aef0` and `0x140cbc920` have not been qualified as an exact source ownership protocol, and normal Close has no recovered slot clearing/destructor order. Main-window QObject parenting adds a second lifetime relationship, so an adapter `shared_ptr` may incorrectly keep the object alive after native parent deletion or delete it before the native slot releases it. Reusing `ownedDoeStudy`/`doe` would also conflate unrelated object types and destruction contracts.

Available APIs do not close this edge:

- `QObjectConstructionRecord` records only the final process-counter effect for an already-bound native identity.
- `ReportQObjectSharedRefLedger` records externally observed shared refs, TLS/thread ref and counter but is inert and does not create, parent, or own the object.
- `GridLayoutState`/`ViewManagerState` are tied to actual WidgetState and a specific QWidget layout transfer; their pending parent transition still stops before the unresolved tail.
- `WidgetThreadState` and `WidgetRuntimeState` have no actual `QThreadData` identity/refcount and no Qt shared-null owner.
- `SubProject` currently has the DOE study, DB, recovery-list and flow-receiver adapter owners, but no Report document. Its current destructor only records the adapter flow-receiver death.

## Earliest source-closed boundary and missing dependency

The earliest currently closed piece is `reportDsPath(Project.path)`: a pure path helper with focused tests. It performs no allocation, slot read/write, QObject construction, document reset, or I/O. There is no stateful Report prefix between this path expression and the native constructor that could be integrated without skipping a native state transition.

Before a source owner can be added or wired, the minimum missing dependency is a root-owned generic QObject runtime/owner seam, backed by a fresh original-process observation of this actual Report document and actual main-window parent. It must bind one constructor identity through shared-null refs/backlinks, actual TLS/QThreadData selection/refcount, parent compatibility and child-list publication, QObject counter, Report member/connection initialization, and post-return `SP+0x30` store. Separately recover the SubProject/Project Close path and the slot's last-reference release relative to main-window QObject child teardown. Without both constructor and Close edges, an owner field would assert lifetime semantics not in evidence.

## Scope

Read-only source/static-evidence audit. No source edits, builds/tests, GUI control, runtime capture, or mutable REA action.
