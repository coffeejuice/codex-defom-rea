# CP203 RINHERIT: owner reconciliation and source-ready boundary

## Decision

The +20/+30/+40/+48 producer set is now concrete, but it does not supply a source-owned Report document or an accepted-Close instance. The converter has two distinct ownership systems that must not be merged: its shared-engine `Project`/`SubProject` and DOE `SessionDocument` owners, and the original GUI's QObject-parented `SfSessionManager` plus its Qt payloads. Existing shared-engine owners cannot stand in for that GUI object. No new native cleanup transition is source-ready.

The narrow candidate prefix remains the existing diagnostic `attemptReportDocumentReleasePrefix`: an unavailable slot stops before the first guard read; an explicitly qualified known-null observation can model the first-null return at `0x1400b5bc2 -> 0x1400b5c26`; known-nonnull stops before the deleting virtual. This is not accepted-Close execution evidence, does not mutate or own a Report object, and cannot enable public Report Close. Keep the public guard.

## Evidence and scope

Read-only converter analysis in `imports/claude-moproj2mst`. Only this report was written. No source/test/shared-state edits, builds, original-process execution, GUI/app-control, mutable REA, database changes, or deployment occurred. Parent recovery status was checked; `python tools/gui_validation_status.py` reports GUI work blocked because there is no supported release operation plus verifiable receipt/status. This independent static pass did not acquire app control or a REA session.

Original image throughout the retained static qualifications: `evidence/original-binaries/DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP160 and CP161 are the controlling conditional-byte qualifications for Close and producers; CP164 adds the SessionRoot/Save branch. These reports use section-mapped original bytes and bounded range hashes, and explicitly do not claim an accepted-Close runtime occurrence.

## Ownership reconciliation

| Converter owner | What source establishes | Why it is not native Report ownership |
| --- | --- | --- |
| `ConversionSession::project_` | `unique_ptr<Project>` owned by one conversion; project storage is explicitly released by `releaseProjectAfterCloseEffects()` after its adapter precondition. | It is the converter's project lifecycle. The method does not dispatch native FileCloseAll, create the original main window/QObject parent graph, or invoke `SfSessionManager` destruction. |
| `Project::subs` | `vector<unique_ptr<SubProject>>`; each adapter SubProject is project-owned. | This gives an adapter owner for the SubProject record, not ownership of a native `SUB+0x30` QObject. |
| `SubProject::reportDocumentSlot` | A no-allocation-by-default diagnostic projection with `Unknown/KnownNull/KnownNonNull`, an optional metadata scope, and opaque identity. Its release prefix validates the adapter SubProject/scope and preserves an initial observation. | The scope is explicitly metadata, not a Report allocation, QObject registration, mutable native slot, or liveness source. It cannot reproduce native callbacks, rereads, destructor dispatch, or guard invalidation. |
| `Study::outputDocument` / `doe::SessionDocument` | A shared DOE Type2 `.DS` XML document with `dom`, `partRoot`, and DOE part/record metadata; lazily created/reset/read under `Study`. | It is the DOE study projection. Despite also involving `SP+0x30` in its DOE route and `SessionRoot` XML, it does not provide `SfSessionManager`, `SfInteractivePost`, `SfItemModel`, `SfPartInfo`, QObject parentage, or native QtDOM ref/lifetime behavior. |

The original `SfSessionManager D` is created through `0x1400b5ad0(S)` / constructor `0x140643100`, with the constructor accepting a main-window QObject parent and the returned object published to the current `SUB+0x30` guard. CP160/161 identify this native class and its payload; they do not bind it to any converter owner. The same offset or the word “session” does not establish shared ownership or equivalence.

## Producer facts and their Close consequence

- `D+0x20`: conditional guard producer `0x140643540` creates an `SfItemModel` (with its selection model); the qualified Report model path reaches it at `0x1412b0d08` on the document returned from its current SUB lookup. This proves a possible producer route, not that the tested Report occurrence took it or that the model remained current until Close.
- `D+0x30`: lazy `0x140645320` constructs an `SfInteractivePost` from current `D+0x48`, publishes the result, calls `0x1406453c0` using current `D+0x58`, then rereads `D+0x30`. `0x140646370` is a separate writer whose nonnull-node route can replace this slot without the lazy accessor's null check. The attachment helper calls `createElement("SessionRoot")` and `appendChild`; the selected PE establishes the calls, not Qt's successful append result or the resulting runtime tree.
- `D+0x40`: `0x140646a60` constructs `SfPartInfo` entries tied to the same `D`, its current `+0x48`, and fields `+0x28/+0x29`, then inserts/appends entries. The destructor loop removes an entry before its bridge/conditional deleting virtual. Entry internals and transitive Qt/container release are not fully established; DOE record ownership is not a substitute.
- `D+0x48`: constructor initialization calls `0x14064b890`, which allocates and publishes a QDomDocument before D is published to SUB. `D+0x58` is a separate QDomElement used by the attachment path. `+0x48` is therefore not justified as null from constructor initialization, but later replacement/current identity is still unbound.

The native `D` destructor's qualified order is: `+0x18` QString null assignment; current guarded `+0x20`; raw `+0x30`; `+0x40` list cleanup; `+0x48` QDomDocument cleanup; QDomElement/guard/string member destruction; QObject base destruction; deleting-wrapper free after destructor return. These typed payloads and predecessors prevent an empty-document shortcut or direct jump from the slot token to QObject cleanup.

## Save flags versus accepted Close state

Keep `D+0x2a` and `D+0x68` separate. Constructor `0x1400b5ad0 -> 0x140643100` passes fifth argument zero, stored as fresh `D+0x2a=0`; constructor initialization sets fresh `D+0x68=1`.

`0x140645030` first calls `0x14064a830`. Inside that helper, `D+0x68==1` selects `0x14064ab80`; any other value selects `0x140646550`. The latter has a conditional route to the lazy +30 accessor, but requires current main/singleton and passes further callback/indirect boundaries. The selected `0x14064ab80` evidence is only a prefix, so fresh `+0x68=1` selects that branch at the split but does not establish its complete effects or rule out later writes/rebinds. After the helper, Save separately tests `D+0x2a`; fresh zero skips the later direct `0x140645320` call. This does not prove `D+0x30` stays null: `+0x68` may have changed, the earlier route and callbacks are not closed, `0x140646370` is a distinct writer, and the other lazy-caller chain is not bound to this Report D.

At accepted Close, the cleanup body does not use `D+0x2a` to choose its +20/+30/+40/+48 cleanup branches. The required Close state is the *current* `S+0x30` at both fresh guard reads, its current D/vtable if nonnull, and D's current payload pointers/guards; the prior Save-time `+0x2a` and `+0x68` values plus writes/callback history are needed to explain producer admission and payload state. Do not infer either from constructor defaults. If Close's slot is null, identify the earlier QObject invalidation/destruction that produced that current state.

CP160 already qualifies the conditional accepted-Close path through manager child, `SfMoModel`, Project and SubProject to `0x1400b5bb0`; this is static reachability, not a delivered/accepted occurrence. That helper's first null read returns without delete/clear; otherwise it rereads the actual slot, conditionally dispatches deleting virtual `+0x18`, then requests `changeGuard(SUB+0x30,null)` after normal return. Parent-driven QObject child deletion can instead invalidate the guard earlier, but no retained evidence binds the actual Report D to the accepted QObject parent/child traversal or establishes their order in one occurrence.

## Narrow source-ready prefix and exact missing evidence

No new source-ready production helper follows. The one safe source prefix already represented in the candidate is the diagnostic first guard read/null return, and its limited outcomes are explicit in `ReportDocumentReleaseResult`: no native delete, no guard assignment, no native SubProject cleanup completion, and no public Close completion. For nonnull, it intentionally stops before virtual dispatch. Promoting it would need, at minimum, a source-owned current slot that can change across producer/callback/destructor events; the current immutable `ReportDocumentSlot` cannot supply that.

The exact missing same-owner evidence is a single original Open→Save→accepted-Close occurrence binding:

1. Project `P`, active `SUB S`, `D=0x1400b5ad0(S)`, and actual accepted QObject parent `H`; retain every current-slot rebind, invalidation, or earlier destruction.
2. The Report model use's fresh `S/D`, `D+0x20` return/model and selection-model identity, and callback returns.
3. The actual Report Save call's fresh `S/D`, `D+0x2a/+0x68/+0x30/+0x40/+0x48/+0x58`; selected Save branch; current-main/singleton and indirect/callback returns; either +30 producer, replacement, SessionRoot create/append result and subsequent DOM/slot state.
4. Accepted Close's actual manager child and `P/S`; both fresh `S+0x30` values at `0x1400b5bb0`; if nonnull, same D's current vtable and payloads through ordered destructor effects; if null, its earlier death/invalidation; and D's QObject child destruction order relative to H.

Until this joined instance witness exists, keep Report Open/Save/Close guarded, retain the current diagnostic slot boundary, and do not model a standalone `SessionRoot`, reuse DOE `SessionDocument`, or add an independent `unique_ptr`/shared owner for D.

## Qualification pointers

- `analysis/resume160-inherit-close-next.md`: conditional accepted-Close chain, SUB rereads/guard setter, `SfSessionManager` identity/deleting target/destructor ordering and payload-cleanup qualification.
- `analysis/resume161-inherit-payload-next.md`: bounded +20/+30/+40/+48 producers and exact PE range hashes.
- `analysis/resume164-inherit-next.md`: SessionRoot attachment calls, Save `+0x68/+0x2a` routes and source limits.
- `analysis/resume164-inherit-plan-review.md`: independent review distinguishing call attempt from proven append effect and actual Report occurrence.
- `analysis/resume171-rinherit-next.md`: same-instance accepted-Close witness checklist; no later source-ready transition.
- Current source hashes inspected: `include/m2m/model.hpp` `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d`; `include/m2m/doe_session.hpp` `583ea14ccc3160ce40437ac4d80a74cc14fcfa78aa2e40217de0dedd7c7a8f4c`; `include/m2m/engine/conversion_session.hpp` `bb11c4582745b706feb3f502a65940f96dae327c9e3783960d30ac618c154d93`; `include/m2m/engine/report_document_slot.hpp` `3deb74db8676602d9e242a7d6cb8c679efd0be8dd8a14fb301f855b014f8832d`; `src/engine/report_document_slot.cpp` `1f004f5f797fc0abeed34b0a90a0a58059c7763e9ea61576eaa0a7c4fb7251b4`; `src/ops/report.cpp` `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a`; `src/engine/save_current.cpp` `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`.
- No new original runtime observation, REA Evidence ID, or executable validation is claimed.


