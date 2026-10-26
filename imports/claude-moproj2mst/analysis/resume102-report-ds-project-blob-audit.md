# CP102 — Report `.DS` caller and project-blob seam audit

## Conclusion

No retained PE/source receipt beyond CP98 closes a production caller for `reportDsPath` or supplies the missing candidate Report document owner. The only source-qualified Report `.DS` seam is inside the native Report operation callback: its `vt+0x80` routine obtains the active SubProject's `+0x30` document, derives the sibling `.DS` path, and invokes the document method; its `vt+0x68` wrapper calls this routine before a separate viewer-model callback. Candidate `reportDsPath` remains a pure computation without a production consumer. Keep Report Open/Save guarded.

A separately integrated inherited branch does exist: object-property side KEY embedding during Project Save. It is owner-bound to `Project`, ordered `FileEffects`, and the selected operation/object path, and the candidate lifecycle fixture exercises the unreadable-key omission through full conversion. It is not Report `.DS` behavior and creates no Report document.

## Evidence crosswalk

| Effect | Original retained source | Candidate source/status | Disposition |
|---|---|---|---|
| Report `.DS` Save call | `evidence/gui_mo/s1/mdt/L7/s0.c`: `FUN_1412adf90` invokes receiver `vt+0x80`, then `FUN_1412af0c0`; `FUN_1412aeb30` derives `.DS`, calls accessor `FUN_1400b5ad0` for the SubProject document, and calls `FUN_140645030(document,path,1)` | `src/ops/report_ds_path.cpp` has no callsites outside its declaration and path-only tests. `src/engine/save_current.cpp` refuses Report before this pair. | Path expression settled; no public consumer or owner-bound document method call. |
| Report `.DS` Open call | Report `vt+0x78` target `0x1412ae070` reads through the same SubProject accessor; `resume98`/earlier retained owner evidence identifies lazy construction/publication at `0x1400b5ad0`/`0x140643100` | `src/ops/report.cpp::loadClassData` refuses before constructing/accessing the document. `ReportDocumentSlot` represents only declared slot observations. | Keep Open guard: candidate lacks the actual parented, reusable document owner and same-instance Close release contract. |
| Object-property side KEY | `resume80-project-blob-review.md` bounds native read failure to a null Data element with caller continuation; original operation-list parent remains serialized. | `src/moproj/write.cpp::readProjectFile` resolves ordered effects, Project overlay, then disk; `src/moproj/write_ops.cpp::blobElement` emits compressed Data or null under the selected `ObjPropertiesKey`. `tests/s/test_project_blob_lifecycle.cpp` covers locked side KEY, parent retention, Data omission, and subsequent Save effects. | Bounded candidate full-Save seam integrated; CP84 grouped receipt is candidate validation only. Do not generalize to `withBlobs` or Report ownership. |

The PE access path for Report document ownership is concrete but incomplete for candidate integration: `0x1400b5ad0` reuses the SubProject `+0x30` guard or allocates/constructs through `0x140643100` with the current main window from `0x143d766b8`, then publishes to the guard. CP87/CP83 establish a conditional cleanup path and deleting target, not a same-instance accepted-Close binding or transitive QObject/main-window lifetime equivalence. The subsequent Report Save viewer callback also remains a separate unmet owner/effect contract.

## Smallest safe next action

No production edit is supported by the current evidence. For `.DS`, the minimum new evidence is a same-instance owner receipt binding the active Project/SubProject, current main-window parent, constructed or reused document pointer in `SubProject+0x30`, and accepted-Close invalidation/release order. Report Save would additionally need the returned viewer-model owner and per-entry effects of `0x1412af0c0` before the later property stage. A path-helper test cannot substitute for these edges.

The inherited project-blob branch needs no new integration action; retain the focused lifecycle test and existing guard for unrelated archive/blob profiles. No other independently integrated branch was established by this audit.

## Scope

Read-only review of retained L7 PE decompilation, current candidate callsites, and existing receipts. No shared REA database, GUI, original execution, source edit, or build was used.
