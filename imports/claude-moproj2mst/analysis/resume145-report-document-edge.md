# CP145 — Report viewer-to-document edge and accepted Close

## Result

No source-closed integrated Report document edge is established from the viewer callback through accepted Close. CP143's complete `0x1412af0c0` body is a second, post-document-save callback: it receives the same Report operation receiver as `0x1412aeb30`, resolves model/Project/viewer state, and iterates a returned collection. Its bounded body contains no call to the Report document accessor `0x1400b5ad0` or document methods `0x140645030`, `0x140645dc0`, or `0x140644fd0`. Thus the earliest document-sensitive Save action is the preceding vt+0x80 callback, not a downstream consumer of the viewer iteration.

## Source-qualified order and owner

Original `evidence/gui_mo/s1/mdt/L7/s0.c` establishes `FUN_1412adf90` order:

1. Invoke Report operation `vt+0x80` (`FUN_1412aeb30`).
2. Invoke `FUN_1412af0c0(param_1)` with the same operation receiver.

Within `FUN_1412aeb30`, the project path is used to derive the `.DS` sibling name. It then calls `FUN_1412b0f70(param_1)` to resolve the active Report SubProject and, if present, calls `FUN_1400b5ad0(subproject)` to obtain its `+0x30` document. The function passes that returned pointer, the derived path, and mode `1` to `FUN_140645030` (caller in `s0.c`, around the Report Save implementation). Therefore the first document-specific Save call occurs **before** viewer iteration. The caller evidence establishes pointer/path/flag arguments and order; the retained material inspected here does not close `0x140645030`'s body-level file, return, or partial-mutation semantics.

The subsequent `0x1412af0c0` body obtains a model through `0x1405ef2b0`/`0x14005f770`, resolves the operation's Project with `0x1412b0fc0`, gets a handle through `0x140c7ae00`, and invokes `0x140d0d6f0(handle,model)`. It treats that return as a collection: `0x140c7c1a0` supplies a count and `0x140c7ae80(return,index+1)` supplies entries. Each entry's `+0x828` string is transformed and passed through QString/helper calls. This establishes the returned-collection iteration shape. It does not establish that the collection is the SubProject document, that entry processing mutates that document, or even the field-sensitive effects of helpers `0x1400048f0`, `0x1408f37d0`, `0x1408f4760`, and `0x1408f3820`.

Same `param_1` proves sequencing on one Report operation object, but not persistent identity of the Project/model, active SubProject, document, and main-window parent. The document callback re-resolves the active SubProject; the viewer callback independently resolves model/Project/viewer values.

## Accepted-Close boundary

`FUN_1400b5ad0` establishes that `SubProject+0x30` is read and reused, and lazily populated by allocating `0x70`, calling `0x140643100(raw, mainWindow, mode, mode!=2, 0)`, then publishing the returned pointer with `0x140cbc920`. Retained Close evidence establishes only manager predecessor gates and manager clear/current reset behavior in its bounded branches. It does not bind accepted Close to the same Project/SubProject/document pointer, resolve writes/clears of `+0x30`, reveal `0x140cbc920` reference-release semantics, or order document destruction against QObject main-window-parent teardown.

The exact missing witness is a source-qualified native SubProject destructor and accepted-Project-Close path that tracks this instance's `+0x30` slot through clear/release/deleting destructor and QObject-parent teardown; if the PE cannot provide that dataflow, an original same-instance Open→Save→accepted-Close trace binding Project, active SubProject, document pointer, parent, and destruction order is required. No such witness appears in the retained evidence examined here.

## Implementation boundary / next query

The pure `.DS` path helper cannot stand in for the document call. Do not add a candidate Report document owner, invoke `0x140645030` without its state/failure contract, or map viewer iteration into document mutations. Keep public Report Open/Save guards. The smallest evidence increment is two bounded native queries:

- `0x140645030` body and its first document-state/file effects, to close the initial Save mutation contract;
- SubProject `+0x30` destructor/accepted-Close release path and `0x140cbc920` implementation, to close same-instance lifetime.

The viewer helper query remains independent: resolve `0x140d0d6f0` return ownership and the first non-string effect among the indexed-entry helpers before considering viewer integration. A viewer-only helper is not an integration cut for the preceding document mutation or Close.

## Evidence

- `analysis/resume63-report-viewer-bounded-capture.txt`: complete `0x1412af0c0` machine-code window, entry through RET at `0x1412af6a0`.
- `analysis/resume143-report-viewer-owner.md`: bounded interpretation of its early model/Project checks and iteration.
- `evidence/gui_mo/s1/mdt/L7/s0.c`: `FUN_1412adf90` Save order and `FUN_1412aeb30` document path/accessor/`0x140645030` call.
- `evidence/gui_mo/s1/mdt/L7/d3.c`: `FUN_1400b5ad0` lazy SubProject `+0x30` construction/publication.
- `analysis/resume80-report-close-release.md` and `analysis/resume141-report-document-owner-audit.md`: accepted-Close and same-instance owner limits.
- Candidate `source/apps/moproj2mst/src/engine/save_current.cpp` still refuses Report before the native-specific prefix.

Read-only retained-source audit. No source, build, GUI, runtime, or mutable REA action.
