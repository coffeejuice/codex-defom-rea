# Checkpoint 52 Report document owner and safe helper boundary

## Refined order at `SP+0x30`

The Report document object at `SP+0x30` is lazily created and shared by the sub-project. `FUN_1400b5ad0` first reads that slot through `0x140c1aef0`. If it is null, the original allocates `0x70` bytes, obtains the main window, calls `0x140643100(raw, mainWindow, subprojectType, subprojectType != 2, 0)`, then assigns the returned pointer through `0x140cbc920` to `SP+0x30`; it finally reads the shared slot again for return (`evidence/gui_mo/s1/mdt/L7/d3.c`). Therefore:

- repeated access reuses the current shared object rather than constructing a new document;
- first-use ownership/parenting happens before path calculation or file access;
- the newly created pointer is attached to the sub-project before `ReportClass::loadClassData` continues;
- allocation failure stores null and leaves the getter result null.

The exact implementation and ownership semantics of `0x140643100`, its base QObject constructor, parent-child callbacks, and object destruction were not found in the consulted immutable decompiles. The two Qt shared-pointer helpers identify a shared slot/get-and-assign path, but no refcount arithmetic for this specific document slot was established here. Do not substitute the separate parentless TaskFactory Report-object QObject contract.

## Reset, path, and first read order

After obtaining the document, `FUN_1412ae070` builds an absolute sibling path from the project path: the project directory, `QDir::separator()`, cleaned `QFileInfo::baseName(projectPath)`, and `".DS"`. It then calls, in order:

1. `FUN_140644fd0(document)`: assigns a null QString to document `+0x18`, then calls `FUN_140646e90(document)` and `FUN_14064bc80(document)`.
2. `FUN_140645dc0(document, dsPath)`: assigns `dsPath` into `+0x18`, constructs `QFile(dsPath)`, and opens with QIODevice mode value `1` (ReadOnly). If open fails, it returns after QFile cleanup. If size is zero, it closes/returns without QDom `setContent`. If nonzero, it calls `FUN_14064b950(document)` before `QDomDocument::setContent(QFile*, ...)`, closes the file, and on successful parse updates DOM element state.
3. `FUN_140644440(document, 1, 1, 0)`: traverses loaded DOM and constructs document record objects. This is after the first read and is outside a bounded path/read helper.

Correction to the prior resume51 wording: `FUN_140644fd0` is demonstrably a wrapper that nulls the path and invokes two helpers; this decompile alone does not justify claiming exactly which DOM/list fields those helpers clear. The nonempty-file branch separately calls `FUN_14064b950` immediately before `setContent`. Preserve this distinction until those helper bodies are independently identified.

On a reused document, reset and path assignment occur before the next open. Thus a missing/empty/unreadable `.DS` leaves the shared object alive with the new path set and whatever state the reset helpers establish; there is no rollback to the prior document. On first use, object construction and `SP+0x30` attachment likewise precede an open failure. Native behavior at constructor failure/unwind, shared-pointer assignment failure, helper exceptions, malformed DOM cleanup, and parent destruction remains unqualified.

## Safe bounded helper stage

The smallest safe helper is **pure path derivation only**: from an already resolved project filename, derive the absolute project directory plus separator, cleaned base name, and `.DS` suffix using the recovered QFileInfo/QDir rules. Keep it side-effect-free and test the path mapping with representative path spellings if/when root assigns implementation ownership. This stage must not call `0x1400b5ad0`, allocate or attach a QObject, reset the live document, set its path, open the file, parse the DOM, or commit Report success.

The actual loader cannot safely advance to reset/read just because path derivation is understood. Executable behavior beyond the helper still depends on the separate parented `0x140643100` constructor and shared-owner contract. Until that is closed, retain the public `M2M_E_UNSUPPORTED` guard at the first document-object use in Report `vt+0x78`. This helper-only boundary is preparatory; it does not reduce the converter's Open refusal or claim any `.DS` lifecycle parity.

## Evidence and next qualification

- `evidence/gui_mo/s1/mdt/L7/d3.c`: `FUN_1400b5ad0`, getter, allocation, main-window parent argument, setter, and re-get ordering.
- `evidence/gui_mo/s1/mdt/L7/d1.c`: `FUN_1412ae070` path/reset/load call order; `FUN_1412b0f70` obtains sub-project report document.
- `evidence/gui_mo/s1/mdt/L7/d2.c`: `FUN_140644fd0`, `FUN_140645dc0`, and `FUN_140644440` file/DOM order.
- `source/apps/moproj2mst/src/ops/report.cpp`: current guard before document effects.
- `analysis/wave3-doe-report-qobject-prerequisite-contract.json` and `analysis/wave2-doe-task-public-qobject-ownership-contract.json`: TaskFactory QObject evidence is a different allocation and requires shared global/thread ownership not present in current model.

Next evidence task: resolve `0x140643100` constructor and destructor/parent ownership, then the two `FUN_140644fd0` callees and `FUN_14064b950`, before proposing any source integration past the pure path helper.

Static trace only. No source, build, GUI, project fixture, or mutable REA database was changed.
