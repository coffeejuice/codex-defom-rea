# CP208 RCLOSE: Report default Save reconciliation

## Decision

No Close-owned source increment is admitted. CP169 closes the selected default
Save branch statically, but does not bind its callback effects or current
document owner to the later accepted Close. The existing
`test_report_document_slot.cpp` exercises a diagnostic initial slot observation;
`attemptReportDocumentReleasePrefix` consumes the first guard observation and
stops before the second slot read and deleting virtual. Extending that fixture
to delete an opaque identity, or adding another synthetic Save/Close helper,
would assert an owner and lifetime not established by the evidence.

## Reconciliation

For original `DEF_GUI_MO_64.exe` v11.0.2 (SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), CP169
qualifies the following static order:

* Report Open resolves an active SubProject and invokes the lazy `S+0x30`
  document accessor. It uses the returned document for reset/read/traversal.
* Report Save resolves its own current active SubProject/document, sets the
  path, calls `0x14064a830(D, 1)`, then conditionally performs payload/parts and
  `.DS` file/DOM work. The new document constructor initializes `D+0x68=1`,
  selecting `0x14064ab80` only if that byte remains current at the branch. That
  branch can emit a signal and makes later fresh owner/list/task reads. Its
  selected body has no direct `D+0x68` store or direct `0x140645320` call, but
  virtuals, callbacks, helpers, and Qt work are not transitively closed. File,
  DOM append, and callback outcomes are unobserved.
* Accepted Close reads `S+0x30`; on nonnull it reads the slot again and invokes
  deleting virtual `+0x18` on the second value, then clears the guard. Static
  code does not establish that either value is the document used by Open or
  Save, that the object remains live with the expected vtable/payload, or how
  QObject parent teardown interleaves.

The current fixture covers known-null return, known-nonnull refusal before the
deleting virtual, foreign/detached slot refusal, and progress invalidation. It
does not construct/publish a parented native document or exercise public Report
Close. `report.cpp` still refuses before document access. These boundaries
remain correct; no source or test edit is justified by CP169.

## Smallest witness needed

One natural original Report **Open → Save → accepted Close** occurrence, on a
fresh reference application state, must bind one concrete `P/S/D` history:

1. At Open and Save, record current Project/SubProject/document identities and
   generations, both lazy `S+0x30` reads, and any intervening slot rebind or
   QObject-parent destruction. Record proposed parent, accepted parent/child
   membership, and the document's relevant fields (`+0x20`, `+0x30`, `+0x40`,
   `+0x48`, `+0x58`, `+0x68`, `+0x2a`).
2. For Save, record the current `D+0x68` branch, delivered signal/virtual
   receivers and their owner/slot effects, current list/index/task/image,
   serializer and DOM append outcomes, and `.DS` open/write/close/error result.
   Record any `0x140645320`/`0x140646370` call if it occurs.
3. At accepted Close, record admission, both `S+0x30` values, the second
   value's live table and payload, deleting-virtual entry/return, guard clear,
   payload destruction order, and QObject parent/child teardown. Retain enough
   owner identities to show whether this is the Open/Save document or a rebind.

Without that occurrence, further static callback qualification may narrow the
trace but cannot establish dynamic delivery, slot values, or accepted lifetime.
Keep public Report Open/Save/Close guarded and do not advance Close cleanup.

## Scope and changes

Only this resume note is written. No source, test, fixture, build, original run,
GUI/app-control session, REA database, shared plan/state, parent DMO, Claude
artifact, or deployment was changed. No tests were run.
