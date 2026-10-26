# CP208 RINHERIT: payload reconciliation and next evidence cut

## Decision

No static source-ready transition is justified in the inheritance-owned converter code. Keep `ReportDocumentSlot` diagnostic and retain the current refusal before nonnull Report deletion/public Close. The smallest useful next evidence is one same-instance accepted-Close observation, correlated back to that instance's Report model and Save calls. This report only records that boundary; it does not claim a runtime observation.

## Producer reconciliation

The retained CP161/CP164 qualifications distinguish four payload roles on the Report `SfSessionManager` document `D`:

- `D+0x20` is a guarded `SfItemModel`. Report's model path can call the guarded accessor on the document returned from its current SubProject `+0x30` lookup. That producer is conditional and does not bind its result to a later Save or Close occurrence.
- `D+0x30` is a raw `SfInteractivePost`. The lazy accessor constructs it from the current `D+0x48`, publishes it, attaches a `SessionRoot` using the current `D+0x58`, and rereads the slot. A separate `0x140646370` route can write this slot without the lazy accessor's null guard. Save can reach the lazy accessor through an earlier `+0x68`-dependent route and a later `+0x2a`-dependent route; the fresh constructor defaults (`+0x68=1`, `+0x2a=0`) do not prove their values at Save, and callback/indirect boundaries remain.
- `D+0x40` holds `SfPartInfo` pointers. The producer couples each part to the same document, DOM and inner session object; list removal/cleanup has ordered helper effects and is not equivalent to deleting retained task/session records.
- `D+0x48` is a separately allocated `QDomDocument` established by the Report document constructor. It supplies the other payload producers; constructor initialization alone does not imply it is empty at cleanup.

These static paths constrain possible behavior but do not establish which producers ran for a particular document, the current slot values after callbacks, or whether the document survives until accepted Close. In particular, a cached `D` cannot stand in for the fresh `S+0x30` reads at `0x1400b5bb0`; its nonnull path invokes a deleting virtual and clears the then-current QPointer, while parent-child QObject teardown is another lifetime edge.

## Smallest same-instance accepted-Close observation

Capture one fresh original session and one Report document from construction through accepted Close, recording only the identity and return values needed to correlate these boundaries:

1. At publication, bind active SubProject `S`, returned document `D`, and actual QObject parent `H`; record any rebind, guard invalidation, or earlier destruction of `S+0x30`.
2. At Report model use (`0x1412b0d08`), record that the current accessor returns this `D`, the `D+0x20` guard before/after and returned model identity, and intervening callback returns.
3. At Report Save (`0x140645030`), record current `S/D` and `D+0x68`, `+0x2a`, `+0x20`, `+0x30`, `+0x40`, `+0x48`, `+0x58`; selected Save branch and callback returns; any `0x140645320`/`0x140646370` invocation, slot publication/replacement, `SessionRoot` append result, or other mutation of those fields.
4. At accepted Close, bind the manager child and current `P/S`; record both fresh `S+0x30` reads in `0x1400b5bb0`, whether the target is the same `D`, its payload values and deleting-virtual/destructor effects, then the order of `H`'s child teardown relative to document deletion. If the guard is null, identify the earlier destruction/invalidation that made it null.

This is the minimum observation that could choose between the already-known null-return and conditional-delete paths without inventing payload state. Until it exists, no inheritance-owned implementation or test can faithfully advance public Report Open/Save/Close; adding a standalone producer/helper test would only restate static control flow.

## Scope and qualifications

Read-only analysis of retained material in this converter subtree. No source, test, shared plan/state, GUI, original execution, live REA, or deployment action occurred. Producer qualifications are `analysis/resume161-inherit-payload-next.md` and `analysis/resume164-inherit-next.md`; the existing accepted-Close boundary is `analysis/resume171-rinherit-next.md`. They rely on immutable static evidence for original `DEF_GUI_MO_64.exe` v11.0.2 (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), not a new runtime trace or Evidence ID.
