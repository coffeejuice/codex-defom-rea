# CP79: Report document release and Close ownership

## Result

No source-closed Report document destructor/release edge is available. The retained original evidence shows that `SubProject+0x30` starts cleared, is read as a pointer, and is assigned only after the 0x70-byte Report document constructor returns. It does not show slot clearing or destruction at Close. The QObject main-window parent introduces a second possible lifetime mechanism, but no retained native Close trace establishes whether parent teardown deletes the document before/after the SubProject slot releases it. Keep the Report Open/Save guards and do not add a candidate owner field or destructor action.

## What the original evidence closes

`evidence/gui_mo/s1/mdt/L7/d3.c`, native `FUN_1400b5470` (SubProject constructor), initializes the `+0x30` slot by calling `0x140cbc920(param_1 + 0x30, 0)`. The exact helper semantics are not defined by this excerpt. The neighboring accessor `FUN_1400b5ad0` then:

1. Calls `0x140c1aef0(SubProject+0x30)` and branches on the returned pointer.
2. If null, reads mode from `SubProject+0x18`, allocates `0x70` bytes, fetches the main window and calls `0x140643100(raw, mainWindow, mode, mode != 2, 0)`.
3. After constructor return, calls `0x140cbc920(SubProject+0x30, result)`.
4. Calls `0x140c1aef0(SubProject+0x30)` again to return/reuse the slot.

A separately retained body in `evidence/gui_mo/s1/mdt/V/v3.c` establishes `0x140c1aef0(p)` as a direct `return *p`. That confirms the accessor's pointer-read behavior. It does not establish `0x140cbc920`'s copy/refcount/destruction behavior. The existing report therefore should call this a SubProject pointer slot; the specific wrapper type and ownership semantics remain unqualified.

The Report constructor capture establishes a non-null main-window parent argument and ordered QObject/document initialization, then seven main-window connection registrations, before slot publication. Static QtCore disassembly shows the parented QObject constructor's possible thread/parent path and counter ordering, but there is no actual Report-instance observation of branch outcome or parent-child effects. None of this yields Close ordering.

## Missing Close/release witness

The retained accessor body has no clear/reset path, destructor call, or final-release operation. The inspected native L7 excerpts identify `FUN_1400b5470` as the SubProject constructor and `FUN_1400b5ad0` as document access, but do not provide the native SubProject destructor or normal Project Close destruction chain. Candidate `SubProject::~SubProject()` only records a flow-receiver witness death; candidate `ConversionSession::releaseProjectStorage()` is adapter cleanup, not native Close, and cannot stand in for either original event.

The minimum next producer is a bounded immutable-PE xref/release trace covering (a) the concrete native SubProject destructor, (b) normal Project Close destruction order, (c) all writes/clears of `SubProject+0x30`, and (d) the last-reference target and its ordering against main-window QObject child teardown. If static xrefs do not reveal the actual last-release/parent-deletion order, the missing witness is one original application lifecycle capture that binds this same document and SubProject through Open, Save path assignment, Close, slot clear, document destructor (if any), and parent destruction. Do not treat `QObject(parent)` alone as proof that the slot is non-owning or that Close automatically completes correctly.

## Smallest bounded prefix

There is no destructor edge to port. The only source-closed Report prefix remains pure `reportDsPath(Project.path)` value derivation, already implemented and tested. It neither reads/creates the native slot nor mutates a document. Adding a null slot field or calling `reset()` during candidate destruction would encode unproven ownership/order, even if public Report activation remains guarded.

## Evidence

- `evidence/gui_mo/s1/mdt/L7/d3.c` — SubProject constructor slot initialization and Report accessor; SHA-256 `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`.
- `evidence/gui_mo/s1/mdt/V/v3.c` — `0x140c1aef0` direct getter body; SHA-256 `b0a5b9df8fa1dc5f65704f32496098e8341b331aa0e27d8a9e32917a46b6a028`.
- `evidence/gui_mo/s1/mdt/L7/d2.c` — Report accessor caller and active SubProject resolution; SHA-256 `256e16f538c67d7c88509d2fdff6b36057539a160e18337f677ee3f904007175`.
- `analysis/resume67-report-document-owner.md` — prior bounded search and unresolved Close chain; SHA-256 `a4a1078903c4d944c556b48cd65c23b426b05924e5cc01ee6acbd11403471285`.
- `analysis/resume71-report-owner-edge.md` — no source owner edge; SHA-256 `51ab8bfb2917607fb386d19be7ead3e42a8b1e4b2eaaa0e295e8f5ff963111b8`.
- `source/apps/moproj2mst/src/model/subproject_lifetime.cpp` — current adapter-only destructor effect; SHA-256 `849873b5f1f4c139bb815bc3a92b897c104a9fc38af016178aa853f89a131f64`.

Read-only retained-evidence/source audit. No source, build, test, GUI, runtime capture, or mutable REA action was performed.
