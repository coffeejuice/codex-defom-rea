# CP196 parent-slot issuer investigation

## Decision

No existing inspected Session, Report, task, widget, or model owner can issue an observation of `[P+8]` for the exact CP195 nonnull frame. Keep `BeforeCallerParentLoad` at QtCore RVA `0x13143a` guarded. A read receipt carrying the current saved-RDI token and an arbitrary `T` would still invent its underlying slot observation: identity validation cannot supply missing memory ownership or a value.

There is a further source distinction: the retained original **Report prototype caller supplies a null parent**. CP195's nonnull test refinement exercises a generic symbolic branch; it does not identify a production parent object. Recovering an unrelated QObject parent's private pointer would therefore not qualify the exact retained Report frame.

This investigation changes only this report. No source, tests, builds, GUI sessions, original artifacts, or REA databases were changed or executed.

## Evidence qualification and checked identity

I read `resume196-connector-next.md`, its BLOCK review `resume196-connector-plan-review.md`, and the capture qualifications before interpreting their listings. `resume163-connector-qobject-static.json`, `resume163-connector-prototype-static.json`, and `resume160-connector-static.json` qualify bounded original unwind-region listings, not complete logical functions or runtime execution. `wave2-doe-task-prototype-constructors-static.json` qualifies direct prototype-constructor entries with partial unwind regions and validated call lines.

Read-only SHA-256 checks in this investigation matched these retained identities:

| Artifact | SHA-256 |
| --- | --- |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `resume163-connector-prototype-141403210.asm` | `718aabe722a528879498d2238d06722b9a02b0b79a6535da0b43acba6a4fa781` |
| `resume163-connector-prototype-140137610.asm` | `aa72c53b0fe0e723425649f22e3dbd702606c3363b8b872c925e8bb4c09a84c4` |
| `resume160-connector-14157ead0.asm` | `72ec3729516dc420cea9da4f5fa0d0aa57cc448eed193cffc1ef28ff96f8fb4a` |
| `wave2-doe-task-prototype-ctor-1407b89e0.asm` | `e4de224a3d5ee0c73eb726c94f88e4fcc0e7872d0696c116273299d691946980` |

Listing names in this report are relative to `analysis/`. Source paths below are relative to the converter root. Hash equality authenticates the retained bytes, not native execution or any symbolic slot contents.

The QtCore listing gives `0x671313eb: mov rdi,rdx`, followed by the private initializer call at `0x67131417`. At `0x6713143a`, bytes `48 8b 4f 08` load eight bytes from `[rdi+8]` into RCX. Subtracting preferred image base `0x67000000` gives the proposed range `[0x13143a,0x13143e)`. Nothing in those bytes gives P's concrete bits, lifetime, mapped/readable status, or slot contents. The following `[rcx+0x40]` read remains outside the cut.

## Source owner audit

| Existing structure | Actual owned observation | Why it cannot issue this parent-slot read |
| --- | --- | --- |
| `TaskAllocationStore` / pending Report | Stable `unique_ptr` Report node and separately owned `SparseQObjectPrivate`; adapter scope and canonical runtime pairing | Owns the newly allocated task and its private state. It has no parent allocation, parent slot map, parent lifetime, or lookup from incoming RDX/P to an existing object. |
| `QObjectPrefix::SavedEntryRdi` | Opaque incoming-RDX token with allocation/report/task/private/module/call/save/return lineage and optional nullness | `initializeNativePrefix()` creates it from the new allocation at `task_factory.cpp:195`. Its identity identifies the saved register record; it does not identify a separately owned parent object. There is no slot value member or issuer. |
| `taskPrivateIdentity` and `PrivateTaskPublication` | CP190 task `+8 = newly allocated private`; CP193 newly allocated private `+8 = task` | These are two different source destinations from parent `P+8`. Neither establishes that P aliases the task, its private, or another retained Report. Copying either value into a parent observation would manufacture an alias. |
| `SessionTask`, `SessionRecord`, `TaskObserver` | Parsed DOE task lifetime, Session record identity, QPointer-style task observation and raw Session backlink | `SessionRecord::task` corresponds to native `+0x58`; the task backlink corresponds to `+0x30`. Neither models a parent QObject/private slot. Parsed tasks are a separate explicit projection, not completed pending Report prototypes. |
| `ConversionSession`, `WidgetRuntimeState`, `WidgetState` | Actual adapter roots, runtime pairing, shared container state, widget components/parent links/thread state | Session creates and pairs the allocation/runtime roots (`conversion_session.cpp:162..172`). Widget parent relations and component identities do not expose a generic public QObject `+8` slot. No relation connects any widget to this frame's incoming P. `QObjectConstructionRecord` is explicitly a counter record, not QObject construction or parenthood. |
| `ReportQObjectSharedRefLedger` | Caller-supplied inert constructor observations, including backlinks and thread observations | Its header explicitly says callers supply every observation. It owns no parent and has no CP195-frame admission path. `observeBacklinks` cannot authenticate a parent-slot value simply because its input fields agree. |
| `ReportDocumentSlot` | Actual SubProject document member's explicit diagnostic nullness/opaque document identity | A different slot/source occurrence. Its own contract disclaims authenticated native execution/QObject lifetime. It provides no parent identity or private slot for this constructor. |
| `ModelManager` / `SessionModelManagerReadReceipt` | Session's canonical manager field/backlink and a privately issued receipt for that specific candidate read | The receipt is an example of owner-derived admission, but its source is `main+0x50`, with no connection to P or `P+8`. Reusing its admission design does not supply this missing source field. |

The inspected implementation sources are `source/apps/moproj2mst/src/doe/{task_factory,task_runtime}.cpp`, their public headers, `doe_session_records.hpp`, and the listed engine owner/receipt headers and Session construction. The negative conclusion is scoped to these current converter ownership paths, not a claim that every original QObject caller or every historical snapshot has been exhaustively searched.

The friend helper at the top of `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` only changes two diagnostic stop ordinals and the pending ordinal. CP195's test at approximately line 7843 directly changes `savedEntryRdi->parentIsNull` and validates JE path/cursor behavior. That assignment neither creates a parent nor observes memory. The production Session tests continue to require one prototype, zero tasks, unknown parent, and refusal at `0x131438`. No tests were run here; these are inspected assertions.

## Smallest next static investigation

Start with the **existing exact Report caller chain**, rather than searching arbitrary `+8` fields:

1. Original `0x140137644` calls `0x1407b89e0` on the first nonnull `0xc0` allocation. Its retained entry calls `0x14157ead0` at `0x1407b89f7`, which calls TaskBase `0x141403210` at `0x14157eae7`.
2. The retained TaskBase bytes at `0x141403222` are `33 d2` (`xor edx,edx`), followed by only the RCX reload at `0x141403224` and the IAT call at `0x141403229` through `0x141e041c0`. This is direct static evidence for the null argument on that call, not an observed runtime frame.
3. `wave2-doe-task-public-qobject-ownership-contract.json` qualifies the import/export mapping as `??0QObject@@QEAA@PEAV0@@Z` to QtCore public entry `0x671313d0`; `resume46-doe-input-imports.json` independently records that IAT identity with an immutable-PE-only qualification. The protected constructor `0x6712e630` is a different boundary.

This already resolves the immediate ambiguity for the selected Report path: no nonnull parent owner is identified because that source call passes zero. The smallest additional original-byte check, if stronger import-resolution binding is required, is a read-only PE import/export lookup for just IAT `0x141e041c0` and that QObject symbol against the two verified images, retaining a compact mapping receipt. It needs no GUI, runtime memory probe, database attachment, or new broad disassembly.

The next planning choice for production progress is therefore a separate, reviewed **caller-nullness qualification** proposal based on that exact callsite, followed by investigation of the guarded null successor at QtCore RVA `0x131449` / target `0x2aca0`. This report does not authorize changing the existing default-unknown contract or executing that helper. Retained current-thread evidence may inform that successor only after its own qualifications and exact same-thread/model admission boundary are reviewed.

If generic nonnull QObject support remains separately desired, the smallest useful new xref investigation is incoming calls to the same public constructor/import, selecting one caller whose RDX is demonstrably derived from a live existing model owner. Trace only that caller's parent construction/publication of `parent+8`, lifetime and invalidation. Do not substitute another constructor or attach an unrelated caller's parent to this Report token. A successful such investigation would define a new source-qualified input boundary; it would not retroactively qualify CP195's test-created nonnull Report refinement.

## Admission requirements if a future owner is actually recovered

There is no implementation-ready issuer contract to approve now. A future proposal must first name the actual owner-held slot and the original caller that binds P to it. The issuer must read that live model-owned slot internally; it must not accept a caller-populated `T` plus provenance strings as evidence. Private receipt construction and equality checks are insufficient without this underlying ownership relation.

Only then can admission require the exact live allocation/control-block owner, report ordinal, task/private identities, saved and restored RDI token, `[S-8]` lineage, module digest, known-nonnull refinement, TEST outcome, fallthrough marker, phase-derived `0x13143a`, offset `+8`, width 8, counts and incomplete flags before any mutation. Revocation must cover parent/slot owner death, scope release, slot replacement/version change, frame replacement or rewind before issuance, token/refinement changes, and reuse across another task/report/runtime/control block. Any alias must be source-established, never inferred from equal opaque values.

Future tests must obtain their valid receipt from that owner, then prove complete-state equality on missing, self-forged, stale, foreign, aliased or mismatched receipts and unchanged terminal reentry. They must preserve the unknown/null guards and verify that any admitted transition changes only RCX, its read provenance, and phase to `0x13143e`; all subsequent effects remain unavailable. Such tests would establish model-input consistency only, not native readability or native memory values. Until the owner exists, the existing nonnull refusal is the supported outcome.
