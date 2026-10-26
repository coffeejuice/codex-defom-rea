# CP216 RINHERIT: payload producers, adapter ownership, and next admissible step

## Decision

No source-closed helper/test increment advances faithful Report Open/Save/Close. Preserve the public Report guard and the current diagnostic-only `ReportDocumentSlot` prefix. A producer unit test would test a projection, not the original's stateful object lifecycle. The next behaviorally useful evidence remains a same-instance accepted-Close witness binding the Report document through Open, Save, and Close; current app-control release constraints still prevent acquiring that witness.

## Payload reconciliation

The retained CP161/CP164 static qualifications establish distinct conditional producer roles on the original `SfSessionManager` document `D`:

| Field | Static producer/effect | What remains occurrence-dependent |
|---|---|---|
| `D+0x20` | Guarded `SfItemModel`, reached through Report model access (`0x1412b0d08`) using the current SubProject document lookup. | Whether the accessor returns this same `D`, model identity, guard changes, and callbacks for the accepted instance. |
| `D+0x30` | Raw `SfInteractivePost`. Lazy `0x140645320` can construct from `D+0x48`, publish, call `0x1406453c0`, then reread; `0x140646370` is a separate slot writer. Publication can append a `SessionRoot` element beneath current `D+0x58`, created from the interactive object's DOM. | Which route ran, slot replacement, current `+0x68/+0x2a` at Save, indirect callback effects, actual DOM append and whether this `D` survives to Close. Fresh constructor defaults do not bind later mutable state. |
| `D+0x40` | `SfPartInfo` pointers associated with the same document, DOM and inner session object. Removal/cleanup has ordered helper effects. | Exact part records and cleanup order for the accepted instance; cannot equate list cleanup with deleting retained session/task records. |
| `D+0x48` | Separately allocated `QDomDocument` established by the Report document constructor and used by payload producers. | Its content and lifetime at Save/Close. Constructor existence does not establish an empty document at cleanup. |

CP164 narrows Save: `0x140645030` calls `0x14064a830` before checking `D+0x2a`; the earlier conditional route can reach the lazy `+0x30` accessor when `+0x68!=1`. The constructor initializes `+0x68=1`, selecting the other branch initially, but this does not prove its value at Save or close the selected branch's callback boundaries. The later direct lazy call is gated separately by `+0x2a`. Thus the +30 producers cannot be reduced to a single constructor/default-based rule.

## Shared-engine owner mismatch

Current converter source has `ConversionSession::project_` (`unique_ptr<Project>`), project-owned `SubProject` records, DOE `Study::outputDocument` / `doe::SessionDocument`, and `SubProject::reportDocumentSlot`. These are converter/shared-engine or DOE storage owners. They do not instantiate the original GUI's QObject parent graph, register a native `SUB+0x30` QPointer, dispatch native FileCloseAll, or reproduce callbacks and virtual destruction. `releaseProjectAfterCloseEffects()` releases adapter storage after its precondition; it is not native `SfSessionManager` destruction. The Report slot is explicitly knowledge/identity metadata, not an allocation or liveness source.

Accordingly no helper can safely advance past the prefix without fabricating native lifetime state. Existing tests of `attemptReportDocumentReleasePrefix` remain valid as guard/projection tests but cannot be promoted to end-to-end Close evidence. Do not use DOE XML ownership as a substitute.

## Minimum next evidence

If/when a supported release operation and verifiable receipt/status permit a fresh GUI session, observe one accepted Open A / Save A / Close A. Bind active `S`, returned `D`, parent `H`, and any rebind/destruction; bind the `+0x20` model use to that same `D`; at `0x140645030`, capture `+0x68,+0x2a,+0x20,+0x30,+0x40,+0x48,+0x58`, selected branch and callback returns, producer invocations and DOM changes; then bind Close's fresh `S+0x30` reads and deleting virtual/destructor effects to `D`, including parent-child teardown order. If the guard is null, capture the earlier destruction/invalidation. Static call paths do not establish these dynamic identities or values.

## Evidence boundary

This is static reconciliation of retained qualifications: CP161 `analysis/resume161-inherit-payload-next.md`, CP164 `analysis/resume164-inherit-next.md`, CP203 `analysis/resume203-inheritance-owner-static.md`, and CP208 `analysis/resume208-rinherit-next.md`; original image is `evidence/original-binaries/DEF_GUI_MO_64.exe` v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It claims no new runtime observation, REA Evidence ID, or behavioral validation. Only this analysis file was added; no source/test/shared state, build, GUI, original execution, mutable REA or deployment action occurred.
