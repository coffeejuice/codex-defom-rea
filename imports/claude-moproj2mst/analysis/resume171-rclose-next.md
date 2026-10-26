# R171: Report Close owner after default Save

## Decision

No new Report Close or destructor state transition is safe to integrate from the retained static evidence alone. The only source-bound Close branch that can run without a live document owner is the first `SubProject+0x30 == null` return at `0x1400b5c26`; the candidate already models it as a diagnostic branch in `report_document_slot.cpp`. The nonnull route cannot advance through deleting virtual, guard clear, document payload release, or QObject base destruction until the same instance's current document and accepted parent are produced and tracked. Adding another null token, a detached destructor, or a copied owner would repeat the diagnostic prefix without closing that boundary.

This read-only pass concerns original **DEF_GUI_MO_64.exe v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`. All addresses below are original VAs; RVAs subtract that base. It does not identify a reconstructed instance or make a claim about its source, built, or deployed version. I used retained byte-qualified reports and current candidate source; no fresh original occurrence, new REA Evidence ID, build, test, GUI control, or original execution is claimed.

## Bounded ownership trace

| Boundary | VA (RVA) | Conditional fact and unresolved effect |
| --- | --- | --- |
| Report Open lookup | `0x1412ae070` (`0x012ae070`) → `0x1412b0f70` (`0x012b0f70`) → `0x1400b5ad0` (`0x000b5ad0`) | The fresh active SubProject `S` supplies `S+0x30`. A null slot leads to `SfSessionManager D` construction at `0x140643100` (`0x00643100`), proposed current-main QObject parent, guard publication, and a fresh slot reread. Open holds its returned D for reset/read/traversal, not across Save. |
| Report Save document | `0x1412aeb30` (`0x012aeb30`) → `0x140645030` (`0x00645030`) → `0x14064a830` (`0x0064a830`) | Save resolves `S/D` again. A newly constructed D starts with `D+0x68=1`, so current value 1 selects the default branch `0x14064ab80..0x14064b14f` (RVA `0x0064ab80..0x0064b14f`). It can emit `signalChangeActiveStepData()` at `0x14064ae54` (`0x0064ae54`), call virtuals, update task image, serialize, and write DOM/file state. Current `D+0x68`, `D+0x2a`, signal recipients, virtual returns, and file/DOM result are not bound by construction defaults or the direct-call scan. |
| Accepted Close document slot | `0x1400b5bb0` (`0x000b5bb0`) | The first current read at `0x1400b5bc2` (`0x000b5bc2`) returns immediately when null, with no delete or guard assignment. The nonnull route rereads the slot at `0x1400b5bd5` (`0x000b5bd5`), uses that *second* pointer for virtual `+0x18` deletion, then calls `0x140cbc920(S+0x30,null)` to clear the guard. Neither read is automatically Open's or Save's D. |
| Conditional deleting target | `0x14064f060` (`0x0064f060`) → `0x1406432f0` (`0x006432f0`) → `0x140643400` (`0x00643400`) | If the second read is still-live D with its constructor table `0x1424e0098`, deleting virtual invokes the ordinary `SfSessionManager` destructor before scalar free. It releases current `D+0x20` SfItemModel, `D+0x30` SfInteractivePost, `D+0x40` parts, and `D+0x48` QDomDocument in native order, then invokes QObject destruction. These are current payloads, not constructor defaults. |
| QObject parent | constructor `0x140643100` (`0x00643100`); QObject base destructor at `0x14064338e` (`0x0064338e`) | The constructor passes a proposed main-window parent and registers connections. Retained QtCore tail can delete children and unlink a parent. Static constructor input does not prove accepted parent `H`, child-list membership, or whether parent teardown destroyed D before SubProject cleanup. |

CP169's complete default Save branch narrows direct calls: it has no direct `0x140645320` lazy interactive-payload call or `D+0x68` write. That does **not** prove D remains unmodified or alive for Close. The emitted notifier has a qualified candidate MainWindow receiver and a first `W+0x48` guard-bind effect (CP152/CP170), but registration liveness, actual delivery, and the subsequent accessor/callback effects are unobserved for this Save occurrence. The Save path also uses fresh active-S and current-main/list reads. Hence a conditional native destructor target is established, while the controlling current owner is not.

## Exact missing owner and callback

The first missing source owner is a root-owned, parented `SfSessionManager D` produced by the Report Open lazy accessor and published into a **live per-SubProject `S+0x30` guard**, with generation/death tracking. The candidate's `ReportDocumentSlot` is an immutable initial `KnownNull`/`KnownNonNull` diagnostic token. It cannot publish the constructor return, observe later rebind/invalidation, perform Close's two current reads, or certify that the proposed QObject parent accepted D. `report.cpp` still refuses before the Report document accessor, and `save_current.cpp` refuses the Report save callback. `ConversionSession` cleanup is adapter cleanup, not the native accepted-Close occurrence.

The callback gap is specifically between Save's default branch and Close's reads: the signal/virtual/helper/serializer sequence can change current main W, active S, document fields, or owner liveness. Static direct-call absence only excludes a selected direct call. The actual accepted-Close path also needs fresh main M, M+0x50 Session manager, Project/SubProject, busy/can-close/modal results, and the QObject parent destruction order. Before a nonnull Close transition, bind the second `S+0x30` read to a live constructor-table D *after* those callbacks and any parent deletion. Before a null return can be used as public lifecycle evidence, identify why that current guard is null rather than importing an initial diagnostic null.

The smallest decisive original occurrence remains one natural Report Open → Save → accepted Close with identities/generations for each fresh P/S/D lookup, proposed and accepted parent H and child-list change, Save notifier receiver/delivery and branch results, both Close slot reads, D's current table/payloads, guard clear and parent-child deletion order. The parent GUI-control assessment currently reports `unsupported_release_api`; `python tools/gui_validation_status.py` reports `release_unverified` and no available GUI task. I acquired no GUI-control session. Immutable work can refine callbacks, but cannot supply those occurrence values.

## Shortest tests after source ownership is admitted

1. On the actual Session/SubProject owner, construct and publish D once, reread the live slot, and verify same-S reuse. A changed active S between Open and Save must select the newly current slot; failure/parent rejection or death leaves no false live-D authority.
2. Drive accepted Close with controlled fresh slot reads: first-null returns without delete/clear; first-nonnull then changed/null second read follows the native second value; stable live D deletes once, runs payload/QObject order, and clears the same guard only after returned deletion. Exercise parent-first death explicitly.
3. Insert a Save callback that rebinds S/D or invalidates D between default-branch notification and Close. Verify Close rereads current identities and refuses unsupported callback outcomes before mutation. Then run focused Report slot/Session tests and the normal source-stable checkpoint regression and original comparison gate.

These are prospective tests, not executed tests. The current diagnostic slot tests already cover initial-null, opaque nonnull, immutable binding, and refusal before deleting virtual; rerunning them would not prove the missing native owner.

## Retained evidence and scope

Half-open original PE range hashes below are retained static qualifications, not new byte captures. CP160 resolves the Close/deleting target and Qt guard import; CP161 resolves payload producers; CP169 resolves the complete default Save branch. PE range coverage and decompiles do not establish runtime delivery, accepted ownership, or exception/unwind behavior.

| Half-open original VA range | SHA-256 | Qualification |
| --- | --- | --- |
| `0x1400b5ad0..0x1400b5ba9` | `ead50dc1eb421e028926475e360fc1a2c94d4b390a440be271dbf6ea1bb29012` | Lazy document producer (CP165) |
| `0x14064ab80..0x14064b14f` | `bbfdcd500aa6585078f8914046bdad1b0fb08d6909035c6dcf72642701e87c8c` | Complete selected default Save branch (CP169) |
| `0x1400b5bb0..0x1400b5c2b` | `2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9` | SubProject document release (CP160/169) |
| `0x140cbc920..0x140cbc957` | `fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8` | QPointer guard setter; not destructor (CP160) |
| `0x14064f060..0x14064f096` | `18e1627d1092136d445b51dc291a7400527c0fb5c824791765e3ae5b185e0b53` | D deleting wrapper (CP160) |
| `0x1406432f0..0x140643399` | `ae23368312937058d61d2fbc62d89954912196373c865e79260155071aff17cc` | D ordinary destructor (CP160) |
| `0x140643400..0x140643503` | `f011b8599bb96ce2259ef36b927067af8c28f6bce1bb22b8bd2dd477dd7ec650` | D payload cleanup (CP160) |

Key local qualifications: `imports/claude-moproj2mst/analysis/resume169-report-owner-next.md`, `resume165-report-owner-next.md`, `resume164-close-next.md`, `resume160-inherit-close-next.md`, `resume161-inherit-payload-next.md`, and `resume170-high-impact-next.md`. Current candidate boundary: `source/apps/moproj2mst/include/m2m/engine/report_document_slot.hpp`, `src/engine/report_document_slot.cpp`, `src/ops/report.cpp`, and `src/engine/save_current.cpp` under the import. Only this report was written; no source, tests, fixture, shared plan/state, binary, or deployment was changed.
