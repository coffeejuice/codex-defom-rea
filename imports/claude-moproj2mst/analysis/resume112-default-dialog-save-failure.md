# CP112: default-Yes Save warning and later Close failure path

## Finding

The most specific reachable default-dialog decision in the one-project lifecycle is the Save validation warning for a must-be-last operation placed before another flow item. Under the retained fresh-dialog default qualification, the warning's only button is Yes and its requested default is also Yes. Original Save returns that Qt integer immediately, before current-operation Save and project/property/MST writers. A later normal FileCloseAll does not reuse the earlier Save result: it performs its own can-close checks and, on the qualified current-operation question's default Yes, can invoke nested Save again. If the last-operation warning is reached again, its Yes is remapped to Close Cancel, leaving the project attached and skipping the accepted-Close callback/removal path.

This is source-closed for the answer mapping and control-flow consequences, but not for the actual modal event/result producer. The dialogs use nested `QDialog::exec`; pending events may run before the default result is returned. The candidate therefore remains fail-closed at that point.

## Ordered original path

1. After successful Open and activation, explicit Save enters `0x1400980d0`; the retained timer contract places `processEvents(flags=0)` before subproject validation and the warning check.
2. Per-subproject validation reaches `0x1400b8fb0`. For a non-last `SfOprShapeOptim`, `SfOprDoeOutput`, or `SfOprReport` flow item, it presents “Operation \"%1\" must be the last operation. Do you wish to correct this?” with only Yes. The retained shipped-style/unchanged-focus static default is Yes (`0x4000`).
3. That result returns from Save before current-operation Save and all project, property, and MST writers. In the selected explicit main-window Save wrapper (`0x140028520`), a result other than No (`0x10000`) produces “Current project save has been canceled”; the wrapper returns false after clearing busy.
4. If the user then independently requests FileCloseAll, the Close root (`0x1400299a0`) performs fresh busy/current-main and manager can-close reads. The ordinary current-operation question's default is statically qualified as Yes. Its Yes path invokes nested Save while project and diagram remain attached.
5. If nested Save reaches the same warning, returned Yes (`0x4000`) is mapped exactly to Cancel (`0x400000`). Close rejects; the accepted-Close virtual callback and RemoveGraphItems are skipped. No earlier adapter error, false result, or writer result substitutes for this nested result.

Normal writer failure is a distinct trap: after at least one nonnull subproject passes validation, Save retains answer No (`0x10000`) even if project/MST output returned false. The selected explicit wrapper can consequently report “Current project has been saved.” That is not the must-be-last warning path and cannot be used to infer Close refusal or acceptance.

## Candidate status and isolated test boundary

`src/engine/save_project.cpp:24-39` recognizes the must-be-last classes, sets `lastOperationModalPending=true` and `pendingDefault=Yes`, then returns `M2M_E_UNSUPPORTED` with a warning-modal-delivery-not-recovered message. It does not fabricate a modal answer or write output after the warning. `src/core/convert.cpp` maps this error through the adapter; conversion-session destruction is not native FileCloseAll.

`close_dialog_answer` plus its isolated tests already cover the source-qualified integer rule: when a caller supplies nested Save's answer, only exact Yes maps to Close Cancel; No, Cancel and other integers retain their values under the guard's `answer != Cancel` condition. That unit seam does not produce the last-operation warning answer, execute its nested event loop, or qualify a public Close call. No further detached mapping test would close the remaining boundary.

A useful candidate-only regression may assert the current fail-closed warning behavior and absence of subsequent candidate writer effects, but it would only preserve the adapter guard, not add native dialog parity. The existing CP105 failed-Cogging-open test already checks one separate output-continuation branch and does not represent this dialog.

## Exact missing evidence and smallest next capture

The missing input is a same-process modal interval/result trace, not the button's static default mapping. The next controlled original capture, once the GUI-release gate permits it, should use a fresh application and disposable one-project fixture whose flow has a must-be-last operation before a later operation. From immediately before the Save validation warning through its return, record: dispatcher/timer registrations and receiver identities at entry; every delivered timer or reentrant callback (including an observed empty set); visible buttons/default/focus; actual dialog result; model/flow identity and state before/after; and whether any project/property/MST writer ran. Then record explicit Save wrapper message/Boolean and, only after a separate user Close request, the fresh Close guard inputs, its question answer, any nested warning interval/result, and whether accepted-Close callbacks ran. Keep the Save warning and Close question as distinct modal intervals.

The current GUI release assessment and `gui_validation_status.py` still report release unverified; no capture was attempted. Until this producer is observed or implemented behind an explicit supplied modal schedule/event list, retain the unsupported Save warning branch and do not bind public Close to a guessed result.

## Evidence basis and limits

- `analysis/wave3-close-save-return-contract.json` and `analysis/wave3-normal-close-guard-contract.json`: native Save return, current-operation question mapping, and style/focus-qualified fresh default.
- `analysis/wave3-boolean-timer-contract.json`: Save-entry `processEvents(flags=0)` ordering and timer dispatcher behavior; event readiness at the warning remains unknown.
- `analysis/resume50-close-dialog-answer.md`, `analysis/resume57-close-failure-order.md`, `analysis/resume110-close-after-save-failure.md`, `analysis/resume108-db-open-modal-event.md`: answer mapping, native failure distinctions, later Close revalidation, and nested-loop limitations.
- Candidate source qualifications: `source/apps/moproj2mst/src/engine/save_project.cpp` and `source/apps/moproj2mst/src/core/convert.cpp`.

All native behavior above is bounded retained static evidence, not fresh original execution or complete behavioral acceptance. No source, tests, build, GUI, original application, or mutable REA database was changed or used.
