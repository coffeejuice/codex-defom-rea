# CP208 RDIALOG next step: Report owner and modal trace required

## Decision

No default-dialog or Report property-dispatch source increment is supported by the retained evidence. Keep the public Report Open/Save guards, the generic `properties vt+0x60` unsupported-class refusal, and the diagnostic-only `ReportDocumentSlot` model. The property callback family is known statically, but the public Report caller cannot reach it until the actual document owner and lifecycle are represented.

The default-button and timer evidence from CP160/203 cannot establish a Report Save answer or delivered modal events. In particular, do not infer that Report Save invokes the must-be-last warning or that a registered/requested timer fired.

## Exact owner blocker

The native owner is the live `SfSessionManager` Report document published in the active `SubProject S + 0x30` QPointer. Open obtains a fresh Project `P`, derives the sibling `.DS` path, resolves active `S`, and accesses or lazily constructs `D`; it then uses `D` for reset/read/traversal. Save independently resolves the active subproject and rereads its document slot. A document identity alone does not establish that Save sees Open's same `S` or `D`, nor constructor parent acceptance, QObject child/lifetime behavior, or Close order.

Current source has no executable Report document producer/lifetime. `ReportDocumentSlot` only records diagnostic null/non-null observations and cannot substitute for constructor publication, active-subproject rereads, payload effects, QObject ownership, or accepted-Close release. The Report property helper mapping is therefore not a public source-ready prefix.

## Smallest discriminating original evidence

One fresh-original, natural Report **Open → Save → accepted Close** occurrence is needed. Bind and record:

1. Fresh Project, active SubProject, Report document pointer/generation, constructor result, QObject parent acceptance and parent-child delta at each lookup; record both Open/Save slot reads and any active-`S` change.
2. Open's document reset/read/traversal results and relevant document fields; Save's selected default branch, property callbacks, signal/virtual receivers, task/index/list reads, serializers, DOM/file effects, and any interactive-payload or SessionRoot append path.
3. Close admission, both native slot reads, guard clearing, and actual QObject/document destruction order.
4. If and only if the observed Report Save path invokes a modal dialog: warning invocation; `QDialog::exec` entry; show and nested-loop entry/exit; actual button activation and `done(int)` result; completion/unwind callbacks; exec and Save returns; plus timer request/registration/due/delivery, receiver liveness and thread affinity. Include capture coverage/loss evidence so an empty event interval is meaningful.

This one bound occurrence resolves both public reachability and the dialog-default question. Static PE/decompile records can refine candidate capture locations, but cannot replace the runtime occurrence or justify guessing a delivered default answer.

## Evidence basis and limits

- `analysis/resume171-rdialog-next.md` records the callback family, document owner/lifetime gap, and the exact need for a Report Open→Save→accepted Close trace. It also explains why the modal transcript from another dialog cannot be transplanted.
- `analysis/resume164-dialog-next.md` records the early public Save refusal at `vt+0x68`, the Report callback mapping in `propsToEngine`, and the guard on generic default-class dispatch.
- `analysis/resume203-dialog-default-static.md` reconciles the CP201 frozen source hashes against working source and confirms there is no newly source-ready Report prefix or evidence that Report Save invokes the must-be-last warning.
- `analysis/resume160-dialog-default-next.md` qualifies static constructor/default-button and conditional timer ordering only; it does not prove an answer or event delivery for Report Save.
- `analysis/resume87-report-ds-owner.md` and `analysis/resume83-report-nonnull-next.md` qualify the static lazy `SubProject+0x30` Report document construction and deleting target, while leaving live publication/lifetime and accepted-Close order unobserved.

No source or tests were changed or run. This note is the only artifact written; no GUI/app-control session, original process, mutable REA state, build, deployment, or shared plan/state was used.
