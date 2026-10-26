# Checkpoint 51 Report `.DS` load trace

## Conclusion and current public boundary

The active Report Open path reaches `ReportClass::loadOperation` (`0x1412add20`), which calls its `vt+0x78` class-data slot (`0x1412ae070`). Current source deliberately stops at that slot's guard before any Report-document construction or file read. The retained original decompile supports a narrower next trace: lazily obtain the sub-project's GUI document at `SP+0x30`, derive `<project folder>/<project base name>.DS`, then open it read-only and parse its DOM. The static path shows no write/delete side effect before or during this read helper. It does not establish the later viewer/UI effects as part of that file-load prefix.

Keep the existing `l7::refuse` until the GUI document object, its constructor/ownership state, and the document read/parse behavior are qualified. The existing QObject capture contract for the DOE Report task object must not be assumed to qualify this separate `SP+0x30` document object.

## Original call sequence

1. `0x1412add20` checks that the Report class has an active sub-project, current project, and operation table. It calls `vt+0x78` before coupon geometry display, Report UI fill, and notification work (`evidence/gui_mo/s1/mdt/L7/d1.c`, `FUN_1412add20`).
2. `0x1412ae070` checks project and active sub-project. It obtains the project filename from `0x14008a4b0`, derives `QFileInfo::baseName`, obtains the absolute project folder, cleans the base name, appends `QDir::separator()` and the literal suffix `".DS"`, and passes the resulting path to the document object (`evidence/gui_mo/s1/mdt/L7/d1.c`, `FUN_1412ae070`; mirrored in `source/apps/moproj2mst/src/ops/report.cpp` comments).
3. `0x1412b0f70` obtains the `SubProject` and calls `0x1400b5ad0` for its `+0x30` shared document (`d1.c`, `FUN_1412b0f70`).
4. `0x1400b5ad0` first reads the existing shared pointer at `SP+0x30`. If null, it checks the sub-project type, allocates `0x70` bytes, obtains the main window via `0x1402c8090(0x143d766b8)`, calls `0x140643100(document, mainWindow, typeFlag, cVar1 == 0, 0)`, then stores the returned object in `SP+0x30`. It returns the shared document pointer (`evidence/gui_mo/s1/mdt/L7/d3.c`, `FUN_1400b5ad0`).
5. The Report loader calls `0x140644fd0(document)` to reset its stored filename/document state, then `0x140645dc0(document, path)`. The latter stores the path and constructs `QFile(path)`, calling `QFile::open` with mode enum value `1` (`d2.c`, `FUN_140644fd0` and `FUN_140645dc0`). Under Qt's `QIODevice` mode values, `1` is `ReadOnly`; no write/create/truncate/delete flag is present. If open fails or the file is zero length, this setter closes/returns without parsing. For a nonempty readable file it resets the existing DOM, calls `QDomDocument::setContent` on the open `QFile`, closes it, and updates the DOM element views on success.
6. `0x140644440(document, 1, 1, 0)` traverses the parsed root (`d2.c`, `FUN_140644440`). On the observed ordinary-document path, it allocates child records (`0x140658690`), appends them to the document's owned list, and parses DOM nodes via `0x140659d90`; this is after the file open/read and is not evidence of a filesystem write. Return behavior on malformed or empty documents and allocations is not fully qualified by this bounded trace.

## First side effects and failure ordering

For a first Report activation where `SP+0x30` is empty, the earliest side effect is in-memory: allocate and parent the 0x70 document object to the main window, initialize it through `0x140643100`, and publish/store it in `SP+0x30`. The path calculations are string/QFileInfo operations. The first file-system operation in this path is `QFile::open(path, 1)` on the computed `.DS` path. Static mode evidence identifies a read-only open; no file write, create, truncation or removal is shown in this prefix.

Ordering matters on failures: the document allocation and `SP+0x30` attachment precede the attempted file open. A failed open does not roll back this in-memory object. A zero-byte file is closed without `setContent`. A nonempty file is parsed after a DOM reset; a failed parse closes the file without taking the success branch that assigns a new root element. No claims are made here about the parser's exact return/status propagation into Report `loadOperation` beyond the shown call sequence.

The document loader is reached before Report's later coupon geometry/view work. Source says `0x1412add20` continues to `0x1412ae3d0`, `0x1412b0660`, report selection/notifications and subsequent virtuals only after `vt+0x78` returns. The present converter refuses before any of those effects, so the `.DS` read must be treated as the first unimplemented reachable Report branch.

## QObject and ownership prerequisite

The `SP+0x30` object is not the same allocation as the DOE TaskFactory's pending `Report` task prototype at caller `0x141403222` documented in `analysis/wave2-doe-task-public-qobject-ownership-contract.json`. Here, `0x1400b5ad0` explicitly allocates a `0x70` object and supplies the main window as constructor argument to `0x140643100`; the task factory contract covers a different stable `0xc0` descriptor and a parentless `QObject` base call at QtCore `0x671313d0`.

Consequently, the TaskFactory QObject qualification cannot be transferred by class name or shared Qt runtime. Before modeling the document allocation, establish from immutable evidence the exact `0x140643100` constructor/vtable chain, its QObject base call and parent semantics, shared Qt container references, current-thread/TLS branch, QThreadData ownership and destructor/parent cleanup. If those calls share the public QObject implementation, bind them to the same runtime-global shared owners while preserving the distinct parented object identity and ordering. The existing capture plan's rule remains applicable: do not infer TLS/QThreadData from notify depth, and do not apply a counter-only QObject commit before private and thread-reference effects. No such constructor trace for `0x140643100` was found in the consulted static records; it remains a prerequisite, not a claimed fact.

The project-relative file behavior can be qualified separately through static QFile/DOM code, but successful parsing/attachment must not be implemented ahead of the constructor/owner contract. Keep unsupported return behavior for missing TLS, null TLS value, allocation failure, constructor unwind, parent destruction and DOM cleanup until separately source-qualified.

## Evidence and qualifications

- `evidence/gui_mo/s1/mdt/L7/d1.c`: `FUN_1412add20` and `FUN_1412ae070` call bodies and path construction.
- `evidence/gui_mo/s1/mdt/L7/d3.c`: `FUN_1400b5ad0` lazy `SP+0x30` allocation and main-window parent.
- `evidence/gui_mo/s1/mdt/L7/d2.c`: `FUN_140644fd0`, `FUN_140645dc0` (QFile mode 1/open, size, DOM setContent/close), `FUN_140644440` (DOM item traversal).
- `source/apps/moproj2mst/src/ops/report.cpp`: current unsupported guards and corresponding source-qualified path summary.
- `source/docs/re/moproj2mst/mdt/L7/spec.md`: L7 slot map and Report document boundary.
- `analysis/wave3-doe-report-qobject-prerequisite-contract.json`, `analysis/wave3-doe-report-qobject-capture-plan.json`, and `analysis/wave2-doe-task-public-qobject-ownership-contract.json`: distinct task-object construction requirements and limits.

Static retained-evidence trace only. No source or build changes, original process, GUI control, or mutable REA database operation was performed.
