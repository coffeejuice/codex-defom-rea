# CP135 Report `.DS` reconciliation

CP134 is the latest full validation inspected (319 tests; 312 ordinary passes, six with logged gaps, one with logged differences; 93 SAME / five DIFF). Its scope is editor, SPRING, FeatureMap, and geometry work; it adds no Report document owner or Report callsite evidence.

| Edge | Current status |
|---|---|
| `reportDsPath(Project.path)` | Pure helper, covered by path tests; production search still finds no callsite. It derives only the sibling `.DS` name. |
| Public Report Open | `ReportClass::loadOperation` calls `loadClassData`; current `src/ops/report.cpp:78-85` refuses before document access/read. The refusal is reached after Project/SubProject checks and names native accessor `0x1400b5ad0` / constructor `0x140643100`. |
| Original Report Open/Save | Retained L7 evidence: `vt+0x78` `0x1412ae070` and Save's `vt+0x80` `0x1412aeb30` use the active SubProject `+0x30` lazy Report document; Save calls `0x140645030(document,path,1)` before distinct viewer callback `0x1412af0c0` / `0x140d0d6f0`. |
| Public Report Save | `src/engine/save_current.cpp:761-764` refuses before these callbacks. Later Report property serialization is downstream and cannot be enabled independently. |
| Close/release | Candidate `ReportDocumentSlot` in `model.hpp:293` stores diagnostic knowledge/opaque identity only. It is not a native `QPointer`, QObject, or parented document. Original SubProject cleanup `0x1400b5bb0` has a known-null return and a nonnull deleting-virtual/clear path, but no same-instance accepted-Close occurrence or order against main-window QObject child teardown is bound. |

**Decision:** no source-closed Report `.DS` integration cut is available. Keep Open and Save guarded. The exact missing witness is one same-instance chain binding the active Project/SubProject, actual main-window parent, actual Report document returned or reused in `SP+0x30`, its `.DS` read and Save calls, and the same QPointer/document through accepted Close and either SubProject deletion or QObject-parent teardown. Save also requires the viewer callback's returned owner and per-entry effects before properties. Until those identities/effects are established, adding a production consumer for `reportDsPath` would bypass required native work.

Read-only source and retained-report reconciliation only. No source, build, test, GUI, original runtime, or mutable REA state was changed or used.
