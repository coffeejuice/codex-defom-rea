# CP216 RDIALOG continuation: Report dispatch and modal evidence

## Disposition

No isolated Report property-dispatch increment is supported by the current source and retained evidence. Preserve the public Report Open/Save/Close refusals, the `SfOprReportProperties` reader refusal, the generic unsupported/default-class refusal, and the diagnostic-only `ReportDocumentSlot` model. The next Report advance still requires one fresh-original, same-process Open → Save → accepted Close witness that binds the live owner and the actual Save/modal path.

## Reconciliation

The current Report source remains guarded at three distinct boundaries:

- `ReportClass::loadOperation` reaches its class-data slot, then refuses the unimplemented continuation; `loadClassData` refuses before reading the `.DS` document through the active SubProject's `+0x30` slot.
- `ReportClass::propsLoad` refuses the three common-block readers (`0x1405fb720`, `0x1405fbae0`, `0x1405fc180`). Their shared helper mapping in `propsToEngine` is conditional static dispatch, not proof that a public Report caller reaches them.
- The public Save path refuses `SfOprReport` at `vt+0x68` before `propsToEngine` and later Report callbacks. Removing that guard or calling the helpers directly would fabricate reachability.

`ReportDocumentSlot` remains diagnostic state only. It does not model lazy construction/publication, QObject parent acceptance, document identity across separate Open and Save lookups, parent destruction, or accepted-Close slot clearing/deletion.

The default-dialog record is not transferable to Report. CP121/141 establish a conditional button-activation/result mapping for the must-be-last warning. CP156/160 qualify static default-button and timer ordering but do not prove that a timer was delivered or establish a Report Save answer. CP203's hash reconciliation found the relevant CP201 frozen sources identical to working source and no new Report caller prefix. CP208 states the precise same-process witness requirement. No later retained evidence identified in the current queue/source changes that conclusion.

## Smallest missing witness

Capture one natural fresh-original Report Open → Save → accepted Close occurrence. Correlate fresh Project `P`, active SubProject `S`, the `S+0x30` slot, constructed document `D`, proposed and accepted QObject parent, child-list delta, and generations at both Open and Save lookups. Record Open reset/read/traversal results and the Save selector byte (`D+0x68`), actual property callbacks, signal/virtual receivers, serialization and DOM/file effects. On accepted Close, record both slot reads, deleting virtual, slot clear, and QObject/document destruction order.

Only if that Save occurrence actually invokes a modal, retain its complete warning-to-result interval: warning invocation, `QDialog::exec`, show, nested-loop entry/exit, actual button activation, `done(int)`, completion/unwind callbacks, exec and Save returns, plus timer request/registration/due/delivery, receiver liveness, thread affinity, and capture coverage/loss. A lack of modal records without coverage evidence is not evidence that no modal event occurred.

## Safe source boundary

There is no new source-ready internal prefix from default-dialog evidence or the Report callback map. The only established Report source behavior remains the pure sibling `.DS` path calculation. Keep the refusals until the occurrence above establishes public reachability and owner effects; then implement only the observed prefix with a regression contract tied to that witness. Do not synthesize a `SessionRoot`, detach the property callbacks from their owner, infer a default answer, or claim Report Save invokes the must-be-last warning.

## Evidence and limits

This is a static reconciliation of `analysis/resume208-rdialog-next.md`, `analysis/resume203-dialog-default-static.md`, `analysis/resume203-queue-reconciliation.md`, and the current `source/apps/moproj2mst/src/ops/report.cpp`. It adds no runtime observation. No source, tests, build, original process, GUI control, or mutable REA state was changed or used. The original-process crash and GUI-control release blockers recorded elsewhere remain external constraints; they do not justify weakening these guards.
