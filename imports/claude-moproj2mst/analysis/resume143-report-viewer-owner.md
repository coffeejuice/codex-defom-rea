# CP143 — Report viewer callback and document ownership through Close

## Finding

The retained bounded DUMPBIN receipt contains the complete body of Report Save's viewer callback `0x1412af0c0` through its `ret` at `0x1412af6a0`. This supersedes the “body absent” statements in the older CP63 and CP141 reports. The body now closes its coarse call/iteration shape, but not the downstream helper semantics or same-instance lifetime through accepted Close. There is still no safe public Report Save integration cut; keep the current guard.

## Save order and callback body

The original `evidence/gui_mo/s1/mdt/L7/s0.c` caller `FUN_1412adf90` invokes its own `vt+0x80` (`0x1412aeb30`) and then directly calls `0x1412af0c0(param_1)`. The first callback derives the `.DS` path, resolves the active SubProject Report document through `0x1400b5ad0`, and calls `0x140645030(document,path,1)`. Thus the viewer callback executes after the Report document save on the same Report operation receiver.

The complete body in `analysis/resume63-report-viewer-bounded-capture.txt` establishes this sequence:

1. Resolve a model-like value from `param_1` using `0x1405ef2b0` then `0x14005f770`; return early if null (`0x1412af0d9–0x1412af0fb`).
2. Resolve a Project-like value with `0x1412b0fc0(param_1)`; return early if null (`0x1412af100–0x1412af11a`). It obtains a Project path via `0x14008a4b0` and performs local path/name/string work.
3. Obtain a viewer/handle value with `0x140c7ae00(model)`; return through cleanup if null (`0x1412af3b5–0x1412af407`).
4. Call `0x140d0d6f0(handle, model)` and overwrite the model local with its return (`0x1412af40c–0x1412af41b`). Pass that return to `0x140c7c1a0` for a count (`0x1412af420–0x1412af42a`).
5. For each zero-based loop index below that count, call `0x140c7ae80(returnedModel,index+1)`, read the object at offset `+0x828` through a string helper, transform the string with `0x140819600`, combine it with a constant at `0x142d63510`, and call `0x1400048f0` twice (`0x1412af477–0x1412af589`). Later helpers at `0x1408f37d0`, `0x1408f4760`, and `0x1408f3820` process the resulting temporaries before the next iteration (`0x1412af5a7–0x1412af64c`).
6. Destroy local temporaries and return (`0x1412af651–0x1412af6a0`).

This proves that the result from `0x140d0d6f0` is used as a collection-like object for count and indexed retrieval, and that every retrieved object's `+0x828` string participates in later processing. It does **not** prove from this caller alone the concrete C++ types, mutation performed by `0x1400048f0`/the later helpers, ownership or validity of the returned model, or the callback's externally observable effects/failure contract. Do not label the two `0x1400048f0` calls as a particular viewer mutation without resolving those callees.

## Same-owner chain and accepted Close

The callbacks share `param_1`, but document ownership is resolved indirectly: `0x1412aeb30` obtains the active SubProject's `+0x30` Report document through `0x1400b5ad0`; `0x1412af0c0` independently resolves the operation model and Project, then obtains the viewer handle from the model. Static caller order does not establish that the Project/model and the SubProject/document belong to the same candidate-equivalent instance across lifecycle transitions.

The retained `L7/d3.c` accessor establishes `SubProject+0x30` read/reuse; on null it allocates `0x70`, obtains the global main window, calls `0x140643100(raw,mainWindow,mode,mode!=2,0)`, then publishes the returned pointer through `0x140cbc920`. It does not establish setter reference semantics, slot clear, final release, or actual QObject parent outcome for this Report document.

Accepted-Close evidence remains bounded to predecessor gates and manager clearing. The inspected empty-manager branch clears its list/current pointer; the nonempty branch dispatches child deletion, but no retained callsite binds that accepted Close to this Project's active SubProject `+0x30`, the document pointer from Open/Save, or teardown relative to the document's main-window QObject parent. CP80's slot-release audit likewise found no native slot-clear/deleting-destructor witness in its retained scope. Therefore the same-instance chain stops at Save dispatch; Close ownership/order is unknown, not established absent from the full PE.

## Integration decision and smallest next query

There is no complete source-closed private Save cut yet. A candidate can invoke isolated helpers for document path or for named viewer operations only after their exact owner and effects are established; the newly recovered caller body does not supply those callee contracts or native document adoption/release behavior. Keep `SfOprReport` guarded before vt+0x80 and `0x1412af0c0` in `save_current.cpp`.

Smallest static follow-up: inspect bounded bodies/xrefs for `0x140d0d6f0` (returned collection ownership), `0x140c7ae00`/`0x140c7ae80` (handle and indexed item contract), `0x1400048f0` and `0x1408f37d0`/`0x1408f4760`/`0x1408f3820` (first field-sensitive effects). Separately trace the native SubProject destructor/accepted-Close caller and every `+0x30` write through the concrete `0x140cbc920` release path. If those static traces cannot bind one same Project/SubProject/document/parent identity, the missing witness is an original same-instance Open→Save→accepted-Close lifecycle receipt.

## Provenance

- Original immutable PE SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC` (as recorded in the bounded evidence reports).
- Complete callback receipt: `analysis/resume63-report-viewer-bounded-capture.txt`, SHA-256 `0A0B5B87C1F034E2F0207F23864222098AEB2577AF6D5F95DEF9D49F536B5559`; it labels the full `0x1412AF0C0` entry-to-RET window.
- Caller: `evidence/gui_mo/s1/mdt/L7/s0.c`, Report `FUN_1412adf90`.
- Document accessor/construction: `evidence/gui_mo/s1/mdt/L7/d3.c`; getter body qualification: `evidence/gui_mo/s1/mdt/V/v3.c`.
- Close limit: `analysis/resume80-report-close-release.md` and `analysis/resume141-report-document-owner-audit.md`.
- Candidate guard: `source/apps/moproj2mst/src/engine/save_current.cpp`, Report branch around line 763.

Read-only retained PE/source audit only. No GUI, build, source edit, or mutable REA action.
