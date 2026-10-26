# CP203 RDIALOG: default Report/property dispatch static audit

## Finding

No new source-ready internal prefix is established. CP201's frozen sources are byte-identical to the inspected working files for Report load, the property/Save dispatcher, and the diagnostic document-slot model. Public Report load still stops before its document accessor; public Report Save stops before `propsToEngine`; the generic property dispatcher must remain guarded for unsupported/default classes. The existing shared Report property projection (`0x1405fb720`, `0x1405fbae0`, `0x1405fc180`, mapped to the `F80_90_B0` helper family) is conditional code, not an admitted public Report prefix: its native caller/owner sequence has not been reached in the converter.

The exact unresolved dependency is one fresh original Report **Open → Save → accepted Close** occurrence binding the live document owner and all relevant modal intervals. Static evidence describes a lazy document under active `SubProject S + 0x30`; it does not prove constructor-parent acceptance, stable `S/D` across callbacks and commands, Report Save's branch values or effects, or Close's destruction order. No evidence establishes that Report Save invokes the must-be-last warning; do not transfer that warning's Yes/default-button analysis to Report.

## CP201 source reconciliation

The selected baseline is CP201 frozen source archive `build/wave3-checkpoint201-verified/source.zip`, identified in `analysis/resume201-root-integration.md` by source SHA-256 `2e90ced71ce9e116259a45104df0ceffb2bdd8b94d2a0167f10c0b39636df1de`. I compared archive members directly with the working source; each pair is byte-identical:

| Source | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/ops/report.cpp` | `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a` |
| `source/apps/moproj2mst/src/engine/save_current.cpp` | `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455` |
| `source/apps/moproj2mst/src/engine/report_document_slot.cpp` | `1f004f5f797fc0abeed34b0a90a0a58059c7763e9ea61576eaa0a7c4fb7251b4` |
| `source/apps/moproj2mst/tests/mdt/test_report_document_slot.cpp` | `1d25149f4d85cf6c09e145d055a73452dba1c2460eee4c1ebb1c4d153cfdf63a` |

The source guards agree with this boundary:

- `ReportClass::loadOperation` reaches `loadClassData` then refuses the post-`vt+0x78` continuation. `loadClassData` refuses at the Report document (`S+0x30`) accessor. `propsLoad` refuses the three common-block readers.
- `propsToEngine` maps `SfOprReport` to the shared `F80_90_B0` family, but `saveCurrentOperation` has an earlier explicit Report refusal at `vt+0x68`, before either that dispatcher or the later Report Save callbacks can run.
- `ReportDocumentSlot` records only a diagnostic initial null/non-null identity. Its release-prefix helper requires that token and still stops at the deleting virtual; it is not a live native slot, document constructor, QObject parent, or owner-lifetime model.

Therefore neither a direct property-helper invocation nor relaxing `properties vt+0x60` would be a source-backed advance. The only existing source-ready Report prefix is the pure sibling `.DS` path calculation. CP201 itself adds no Report owner or default-dialog result producer.

## Original static evidence and qualified limits

The preserved original is `DEF_GUI_MO_64.exe` v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The following call sequence and effects are qualified by the immutable PE selections/decompiles summarized in CP165/169, not by a live occurrence:

1. Report Open `0x1412ae070` calculates the sibling `.DS` path, resolves the then-current active SubProject via `0x1412b1010` and document accessor `0x1412b0f70`, then passes its returned `D` through reset/read/traversal (`0x140644fd0`, `0x140645dc0`, `0x140644440`).
2. Lazy accessor `0x1400b5ad0` reads `S+0x30`; on null it allocates and constructs `SfSessionManager D` with the current main window as a *proposed* QObject parent, publishes the constructor result to the slot, and rereads the slot. Static call order does not establish that Qt accepted the parent or that `D` survives later callbacks.
3. Report Save callback `0x1412aeb30` obtains a fresh active `S/D`, then enters document Save `0x140645030`. A newly constructed document initializes `D+0x68=1`, selecting default branch `0x14064ab80` only if that field remains 1 at the selector. That branch includes signal/virtual and task/list/serialization effects; their recipients and results are not closed. Save also rereads other document fields and conditionally handles the interactive payload, parts and `.DS` DOM/file effects. None establishes the result or identity of a same-Open document occurrence.
4. Accepted Close release `0x1400b5bb0` reads `S+0x30` twice, conditionally calls deleting virtual `+0x18` on the second value, then clears the slot. Static code does not bind either read to Open's `D` or establish whether QObject-parent destruction invalidated it first.

The source reports `analysis/resume165-report-owner-next.md` and `analysis/resume169-report-owner-next.md` contain bounded original VA ranges, hashes, and qualifications. In particular, the complete default branch selection `0x14064ab80..0x14064b14f` has SHA-256 `bbfdcd500aa6585078f8914046bdad1b0fb08d6909035c6dcf72642701e87c8c`. These are static conditional contracts, not constructor/Qt/file outcomes or proof of actual branch values.

Default-dialog evidence is separate. CP121/141 close a conditional button-activation-to-result mapping for the native must-be-last warning. CP156/160 show that the answer and event schedule are not produced by static focus: timer readiness/order and callbacks remain unknown, `QDialog::exec` shows before its nested loop, and completion/unwind effects can continue after `done(int)`. No retained evidence binds that warning to the Report Save path. A Report capture must record a warning only if its invocation is actually reached; for every reached modal it must bind warning call, exec entry, show, nested-loop entry/exit, actual activation and `done(int)`, completion/unwind callbacks, exec/Save returns, and relevant timer/receiver delivery and state. Do not infer an empty event interval from default focus or missing callback records.

## Exact missing witness and disposition

One natural fresh-original Report Open → Save → accepted Close trace must bind, by generation and occurrence:

- Open's `P`, active `S`, `S+0x30` before/after both accessor reads, allocated/constructed `D`, proposed main parent and accepted QObject parent/child-list delta, constructor connections, and any rebind/death;
- Open reset/read/traversal results and current `D` fields relevant to model, interactive payload, parts, DOM, branch byte `+0x68`, and later `+0x2a`;
- Save's newly resolved `P/S/D`, actual `+0x68` selection, signal/virtual receivers, task index/list/image, serializers, DOM append/write, file open/close/error and callback effects;
- accepted Close admission, both current `S+0x30` reads, deleting virtual/table and payload state, slot clear, and parent-child destruction order;
- for every modal actually invoked, the complete event/activation/result/unwind interval described above, with coverage/loss evidence sufficient to support any claimed zero-delivery interval.

Until that evidence and owner representation exist, keep Report Open/Save/Close refusals, the generic default-class refusal, and modal-result guards. Do not add a detached Report property helper, synthetic `SessionRoot`, or inferred default answer. No source/test/build/original/GUI/mutable REA action was performed; this report is the only artifact written.

## Supporting resume reports

- `analysis/resume201-root-integration.md` — CP201 freeze identity and validation boundary.
- `analysis/resume164-dialog-next.md` — property-dispatch reachability and Report guard.
- `analysis/resume165-report-owner-next.md` — lazy owner and Open/Close lifecycle evidence.
- `analysis/resume169-report-owner-next.md` — full default Save branch and exact remaining owner boundary.
- `analysis/resume121-default-dialog-done-bridge.md`, `analysis/resume141-default-dialog-next.md`, `analysis/resume156-dialog-timer-next.md`, `analysis/resume160-dialog-default-next.md` — conditional modal result and unresolved delivery/order.
