# CP171 RINHERIT: producer audit result and accepted-Close witness gate

## Decision

No additional source-ready transition follows from the inherited `SfSessionManager` payload producer audit. The useful next candidate action is an **occurrence witness**, not a new native owner or a standalone `SessionRoot` helper: bind one Report document through its actual current SubProject slot across Report model use, Save, and accepted Close, including mutable payload slots and QObject parent teardown. Until then the current diagnostic `ReportDocumentSlot` stopping before the deleting virtual is the justified boundary.

This is a read-only audit limited to `imports/claude-moproj2mst`. I wrote this report only. No source/build/test/GUI/live mutable REA/shared-state action occurred. Claims below rest on retained immutable static qualifications, not a new original runtime observation or REA Evidence ID.

## Inherited payload producer audit

CP161 identifies the four payload roles and actual conditional producers: guarded `D+0x20` `SfItemModel`, raw `D+0x30` `SfInteractivePost`, list `D+0x40` `SfPartInfo` entries, and separately allocated DOM owner `D+0x48`. CP164 further bounds the `+0x30` lazy path's `SessionRoot` create/append call and both Save routes to that accessor. These local paths are not a source-owned Report lifecycle or proof that a given occurrence took a producer.

The decisive instance gap remains: Report model use and Report Save perform current document lookups independently, callbacks and slots may change between them, and accepted Close rereads `SUB+0x30`. Constructor defaults (`D+0x68=1`, `D+0x2a=0`) constrain only the fresh constructor state; they do not establish state at Save. The earlier lazy path requires `D+0x68!=1`, the later direct Save path requires `D+0x2a!=0`; the selected default Save branch is only partially closed and includes indirect/callback boundaries. `D+0x30` can also be written by the distinct `0x140646370` route. Thus there is no justified fixed value or producer admission to add.

## Exact witness blocker

A single same-instance trace must bind:

1. selected Project `P`, active SubProject `S`, accessor result `D`, and actual accepted QObject parent `H`; record every rebind, invalidation, or earlier destruction of `S+0x30`.
2. Report model call `0x1412b0d08`: current `S`, returned `D`, `D+0x20` guard/current model identity and callback returns.
3. Report Save call `0x140645030`: current `S/D`, `D+0x68`, `+0x2a`, `+0x30`, `+0x40`, `+0x48`, `+0x58`; selected `0x14064a830` branch, main/singleton and indirect callback returns; any `0x140645320` or `0x140646370` invocation, publish/replace, `SessionRoot` creation and append result.
4. accepted Close: current manager child and `P/S`; both fresh `S+0x30` reads at `0x1400b5bb0`; if nonnull, same `D` deleting-virtual dispatch and `0x140643400` payload cleanup order; actual `H` child deletion relative to that destructor. If null, the earlier destruction/invalidation occurrence must account for it.

Without these identities and call/return boundaries, the constructor, static call graph, opaque slot token, and DOE `SessionDocument` cannot supply the missing state. The exact Close blocker is **current `S+0x30` and its `D` identity/payload values at each read, plus same-object parent/child destruction order**. The exact Save blocker is **whether an admitted Report occurrence modifies `D+0x68/+0x30` through either route or callback before accepted Close**.

## Acceptance tests once the witness/source owner exists

- Preserve existing unknown-slot refusal: no public Report Open/Save/Close completion may be reported from a null/default token or DOE projection.
- Producer sequence test: existing nonnull `D+0x30` skips lazy construction/attachment; null path constructs from the current `D+0x48`, publishes, calls attachment using current `D+0x58`, records append result, and rereads current slot. Do not make publish+append atomic or assume rollback.
- Distinct writer test: `0x140646370` nonnull-node arm may replace `D+0x30` without the lazy null guard; later cleanup must use the replacement slot, not a cached old pointer.
- Save branch tests independently cover constructor-state defaults and intervening `+0x68`/`+0x2a` changes, preserving callback returns and short-circuit guards. Do not infer direct lazy-call absence from `+0x2a=0` alone.
- Close integration test uses captured same-instance `P/S/D/H`, validates both fresh guard reads and conditional virtual deletion, payload order `+0x20 -> +0x30 -> +0x40 -> +0x48 -> QObject`, and compares document deletion with parent child teardown. Null-slot case requires a captured prior death/invalidation. Verify no duplicated deletion and no assumed rollback on callback failure.

## Qualifications

Primary retained reports: `analysis/resume161-inherit-payload-next.md` (payload producers), `analysis/resume164-inherit-next.md` and `analysis/resume164-inherit-plan-review.md` (lazy attachment and Save branch limits), `analysis/resume160-inherit-close-next.md` and `analysis/resume164-close-next.md` (conditional Close chain and owner witness). Original binary identity throughout: `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. These remain static qualifications; no fresh capture is claimed.
