# CP70 — Report document parent owner and `SubProject+0x30` effects

## Finding

The immutable QtCore constructor disassembly makes more of the parented QObject branch statically explicit than the parent-null Report-QObject contract: it selects/reuses thread data from the proposed parent, increments that thread reference, checks parent/thread compatibility, calls `QObjectPrivate::setParent_helper`, then increments the process QObject counter. This is enough to specify an observation contract, but not enough to admit a production helper or enable Report Open. The current model lacks the actual main-window QObject identity and QThreadData owner/reference seam required to commit those effects, and the `SubProject+0x30` shared-slot release behavior is not qualified.

## Original Report-specific allocation and adoption

The retained `evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`, gives the exact slot prefix:

1. Read the existing shared slot at `SP+0x30` via `0x140c1aef0`.
2. If null, read subproject mode (`0x140010330`), allocate `0x70` bytes, obtain the main window via `0x1402c8090(0x143d766b8)`, call `0x140643100(document, mainWindow, mode, mode != 2, 0)`, and store its result to `SP+0x30` via `0x140cbc920`.
3. Read the slot again and return it. A non-null slot is reused; the constructor is not repeated on that path.

The Report Open class-data function `0x1412ae070` calls the accessor through `0x1412b0f70`, then resets and reads the document (`0x140644fd0`, `0x140645dc0`) and traverses it (`0x140644440`). Current `ReportClass::loadClassData` still refuses before the accessor; `reportDsPath(Project.path)` remains only a pure filename helper and does not reproduce allocation, adoption, or read effects.

## Parent constructor branch from immutable QtCore

`analysis/wave2-doe-task-base-public-qobject-constructor.asm` is a bounded DUMPBIN window for `QtCore4_64.dll` at `0x671313d0`, through its RET. It is the public `QObject(QObject*)` constructor used by the Report document constructor `0x140643100`; do not conflate it with protected `0x6712e630`.

For a non-null parent argument, the visible native instruction order is:

1. `0x671313f1-0x67131435`: install the QObject vtable; allocate/init private state; publish `object+8 -> private` and `private+8 -> object`.
2. `0x67131435-0x67131449`: test the parent. When non-null, read the parent's `QObjectPrivate+0x40` thread-data pointer and inspect its `+8` field; the branch calls `currentThreadData` (`0x6702aca0`) when that field is nonzero. When the test is false, execution uses the already-loaded parent thread-data pointer.
3. `0x6713144e-0x67131456`: store the selected thread-data pointer in child private `+0x40` and call `0x67027630` to increment its reference count.
4. `0x6713145b-0x67131482`: for non-null parent, call `0x67129a50` with parent and parent/child thread-data pointers. If the compatibility result is false, clear the parent argument; call `0x6712d3f0` (`QObjectPrivate::setParent_helper`) with the resulting parent (original parent on the compatible path, null on the other path).
5. `0x67131488`: increment the process-global QObject counter.

The static code therefore exposes both the parent-compatible branch and the branch that clears the proposed parent before `setParent_helper`. It does not supply the actual Report process's parent thread-data pointer, branch outcome, warning/callback effects, or the complete child-list/event result. Also, the actual Report parent is the main-window object fetched from application state, not the null parent used by the existing DOE public-QObject capture contract. The subsequent Report document constructor initializes members and registers seven connections only after the QObject base returns (`analysis/resume63-report-viewer-bounded-capture.txt`).

## Current owner APIs and exact admission boundary

- `QObjectConstructionRecord` in `include/m2m/engine/view_manager.hpp` records only the final QObject counter for an already-owned native identity; its API expressly does not represent construction, thread binding, parenthood or destruction. `WidgetRuntimeState::commitQObjectConstruction` only advances that counter.
- `WidgetThreadState` does not own the actual `QThreadData*`, its TLS lookup identity, or its native reference count. `WidgetRuntimeState` has no owner for the four Qt shared-null increments/backlinks that precede thread selection.
- `GridLayoutState::beginParentedConstruction` accepts an actual `WidgetState` as a proposed parent, but leaves assignment pending. The later `ViewManagerState` path is a type-specific QWidget layout/parent transfer and refuses at the unresolved child-event/parent tail. It is not a general QObject-parent constructor for the Report document.
- `ReportQObjectSharedRefLedger` records externally supplied shared-ref, backlink, TLS, thread-ref and counter observations; it has no parent observation or source-state commit and is deliberately inert.
- `Project`/`SubProject` have no live Report document owner in current source. No retained body found for `SP+0x30` slot destruction/nulling, so even an otherwise complete constructor would not establish normal Close.

No executable helper admission is supported. The narrowest safely representable extension is an inert evidence ledger that records the actual parent identity, parent/child thread-data identities, compatibility decision, `setParent_helper` outcome and resulting child collection under one constructor ordinal. It must not be called as if it constructs or adopts the Report document. A public Open helper requires a root-owned generic QObject runtime API, an actual main-window owner binding, source-qualified effects through parent publication, and the subsequent Report constructor/slot-store effects.

## Missing evidence / next action

The next evidence must bind one actual Report document constructor in a fresh original process to the exact main-window pointer and retain the static branch addresses above with: parent private/thread-data pointer; actual `currentThreadData` return if taken; result of `0x67129a50`; `setParent_helper` entry/return and parent child-list delta; and counter value/order through constructor return and `SP+0x30` store. Separately recover the native SubProject destructor/Project Close path and the last-reference release/destructor behind the shared slot operations `0x140c1aef0`/`0x140cbc920`. Until these are available, preserve both public Report guards and do not infer ownership from the QObject parent argument alone.

## Scope

Read-only immutable-PE/retained-source review. No source edits, builds/tests, GUI control, runtime capture, or mutable REA action.
