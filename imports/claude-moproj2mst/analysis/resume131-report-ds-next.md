# CP131 Report `.DS` helper and public lifecycle boundary

## Decision

The pure `reportDsPath` helper remains correctly isolated. No source-closed call can be integrated into public Report Open or Save from the current owners. The next required dependency is one actual, same-instance Report document owner bound to the active `Project`, `SubProject`, and main-window parent across lazy construction/reuse and accepted Close. Until that owner and its cleanup ordering are qualified, retain the Report Open and Save guards; do not add a speculative file read/write around the helper.

## Candidate reachability

- `src/ops/report_ds_path.cpp::reportDsPath` is called only by its declaration-level consumers/tests; a production callsite search finds no caller. It computes a sibling `.DS` path from a project path, but it does not own a Report document, perform I/O, or model a Report lifecycle callback.
- Public candidate Open dispatch reaches `ReportClass::loadOperation`, whose `vt+0x78` path enters `loadClassData`. At `src/ops/report.cpp:78-85`, `loadClassData` refuses before obtaining/constructing the document or reading `.DS`; the outer `loadOperation` also retains its post-vt+0x78 boundary. Therefore the converter cannot reach Save for an active Report through this path.
- `src/engine/save_current.cpp:761-764` refuses `SfOprReport` before its `vt+0x68` save body. The later Report-specific branch around lines 778 onward also marks the class unported. Even a helper call from that location would skip required earlier native work.

## Original callbacks and owner

Retained L7 evidence (summarized in `analysis/resume80-report-ds-integration.md`, `resume87-report-ds-owner.md`, and `resume102-report-ds-project-blob-audit.md`) qualifies this order:

1. Report Open `vt+0x78` (`0x1412ae070`) obtains the active SubProject's lazy Report document via `0x1400b5ad0`, then reads the project-sibling `.DS` through the document. The accessor reuses SubProject `+0x30` when nonnull; otherwise it allocates/constructs the Report document at `0x140643100` with the current main window as QObject parent and publishes it to that slot.
2. Report Save `vt+0x68` (`0x1412adf90`) first calls `vt+0x80` (`0x1412aeb30`). That callback derives `.DS`, resolves the same active SubProject document, and calls `0x140645030(document, path, 1)`. It then invokes the distinct viewer-model callback `0x1412af0c0`, which calls `0x140d0d6f0(display, model)` and iterates a returned collection before later Report property serialization. The document call or viewer callback cannot be omitted or replaced with the path-only helper.
3. Report document cleanup is tied to the SubProject `+0x30` QPointer via `0x1400b5bb0`. The known-null first-read branch returns before deletion or guard assignment. A nonnull branch reaches a deleting virtual and later clears the current guard; main-window QObject child teardown may also invalidate the guard.

These are source-qualified functions and order. They do not yet bind one live Project/SubProject/document/main-window generation through Open, Save, and accepted Close.

## Current model is not the document owner

`include/m2m/model.hpp` embeds `engine::ReportDocumentSlot` in `SubProject`, but `ReportDocumentSlot` stores only `Unknown/KnownNull/KnownNonNull` diagnostic knowledge and, for nonnull, an opaque integer identity. `ReportDocumentReleaseProgress` binds a candidate adapter slot scope and represents the known-null no-delete return or stops before the unknown deleting virtual. It does not allocate a document, own a QObject, publish a QPointer, implement destructor effects, or enable Close. Thus the existence of a SubProject member does not satisfy native `SP+0x30` ownership.

The earlier CP65 path helper is a complete pure path computation with path tests. CP80/87/98/102 reviews found no additional source-qualified production callsite or closed owner contract. CP129 changes elsewhere in OPI, Type3, and SPRING handling do not close this Report dependency. The separately integrated Project object-property KEY branch is unrelated and cannot stand in for Report `.DS`.

## Exact blocker and next evidence

Before a source integration cut can be chosen, a retained same-instance owner receipt must establish:

- which accepted Project and active SubProject reach Report Open;
- the actual main-window parent identity and whether `SP+0x30` is null or reused at first access;
- if constructed, the returned Report document identity, parent relationship, publication into the same SubProject QPointer, and constructor failure/unwind effects;
- the same document being passed to the `.DS` reader and later Save method `0x140645030`, with path, mode, return/partial effects;
- the separate `0x140d0d6f0` viewer-model return owner and per-entry effects before Report property Save; and
- on accepted Close, whether SubProject QPointer cleanup or QObject child teardown invalidates/destroys that same document first, including the branch and destructor order.

The precise missing witness is not another `.DS` path calculation. It is the same constructed-or-reused document instance and QPointer cell tied to the main-window owner through public lifecycle, plus the required viewer callback owner for Save. No source-only production change can truthfully bridge that gap. Keep the narrow path and diagnostic slot helpers as-is; after the owner witness exists, the first reconsideration point is a private owner-bound Report document read/write helper that preserves `Open → Save document callback → viewer callback → properties` ordering without enabling public Report until Close ownership is also closed.

This was a read-only current-source/retained-evidence audit. No source, build, test, GUI, original runtime, or mutable REA state was changed or used.
