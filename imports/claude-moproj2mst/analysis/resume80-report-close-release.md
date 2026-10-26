# CP80: Report `SubProject+0x30` Close release trace

## Finding

No retained REA capture or bounded Close disassembly identifies a Report-document release at `SubProject+0x30`. The evidence closes slot initialization, pointer read, lazy constructor call, and post-constructor pointer assignment. It does not close slot reset, destructor dispatch, last-reference behavior, or ordering against main-window QObject child teardown. The smallest source-closed Close/destructor edge is therefore **none**; preserve the Report Open/Save guards and do not add candidate release behavior.

## Bounded owner evidence

`evidence/gui_mo/s1/mdt/L7/d3.c` contains native SubProject constructor `0x1400b5470` and accessor `0x1400b5ad0`:

- The constructor initializes the `+0x30` slot by calling `0x140cbc920(slot, nullptr)`.
- The accessor calls `0x140c1aef0(slot)`; `evidence/gui_mo/s1/mdt/V/v3.c` shows that helper's entire body is `return *slot`.
- For an empty slot, the accessor allocates `0x70`, obtains the main-window object, constructs the Report document through `0x140643100(raw, mainWindow, mode, mode!=2, 0)`, calls `0x140cbc920(slot, constructedPointer)` only after construction returns, then reads/returns the slot.

These facts bound publication and reuse. They do not define the setter `0x140cbc920`'s reference protocol or prove a smart-pointer type. In the retained L7 subset, no slot clear, assignment to null at Close, last-release helper, or Report document destructor callsite is present. The Report constructor also registers seven connections, but teardown/disconnection of those connections is not captured.

## Close evidence boundary

The retained Close investigation in `analysis/resume74-close-next-owner.md` establishes the manager `clear()`/owning-list/current-reset order for a bounded empty-manager branch and explicitly leaves **every nonempty child/Project/OPI/slot/base/QObject teardown** unresolved. It does not trace the Report `SubProject+0x30` field or supply a Report document destructor. `analysis/resume66-close-lifetime.md` investigates status-bar/widget/global-message lifetimes; those are different owners and do not provide a Report document release edge. The candidate `SubProject::~SubProject()` only records death of an adapter flow-receiver witness, so it cannot stand in for the native SubProject destructor or normal Close.

Static QtCore QObject destruction contracts and the constructor's main-window parent argument alone do not determine this case. They do not establish whether Close first drops the SubProject slot, deletes the parent and child, or triggers a final reference release through another owner.

## Exact missing witness and next bounded query

The next static query must start from the native SubProject destructor/its vtable and normal Project Close callchain, then follow the `+0x30` field until the actual release target is known. Required evidence:

1. Native SubProject destructor body and the caller that destroys the active SubProject/Project on accepted Close.
2. Every native write/clear of `SubProject+0x30`, including any reset or reassignment path.
3. The concrete `0x140cbc920` setter implementation and last-reference decrement/destructor target for this field's type.
4. Ordering of that release relative to main-window QObject child deletion and the Report document's virtual/deleting destructor; include connection teardown if it precedes destruction.

If the immutable PE and retained captures do not expose the slot's dynamic last-reference path, the required witness is a same-instance original lifecycle trace from Report Open through Close, binding the SubProject slot, document pointer, main-window parent, slot clear/release and destruction order. No such runtime witness is present here; no new GUI/runtime capture was attempted.

## Candidate boundary

Do not wire `reportDsPath` into a lifecycle: it is pure filename derivation, not document ownership. Do not add a `shared_ptr` or clear it in candidate `SubProject::~SubProject()`. Either would choose ownership and destruction order that the retained original evidence does not establish.

## Evidence and hashes

- `evidence/gui_mo/s1/mdt/L7/d3.c`: `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`
- `evidence/gui_mo/s1/mdt/V/v3.c`: `b0a5b9df8fa1dc5f65704f32496098e8341b331aa0e27d8a9e32917a46b6a028`
- `analysis/resume74-close-next-owner.md`: `1f47984d52dcbff0a7db6bfdc632fe5c268c216a947e1e7a734284546aefa7d8`
- `analysis/resume66-close-lifetime.md`: `2a88a75e263eb65fe376176a7f5ddd5b94778f909e682c16e95e99cb48bda1b0`
- `analysis/resume67-report-document-owner.md`: `a4a1078903c4d944c556b48cd65c23b426b05924e5cc01ee6acbd11403471285`
- Candidate adapter destructor `source/apps/moproj2mst/src/model/subproject_lifetime.cpp` is not original Close evidence; it currently records only the flow-receiver witness.

Read-only retained capture/disassembly review. No source, build, test, GUI, mutable database, or runtime action was performed.
