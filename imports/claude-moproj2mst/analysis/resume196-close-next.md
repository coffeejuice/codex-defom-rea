# CP196 RCLOSE: next accepted-Close transition

## Decision

No new accepted-Close or cleanup transition is source-ready. The existing public fixture is `source/apps/moproj2mst/tests/mdt/test_report_document_slot.cpp`; it tests an adapter diagnostic observation and stops before the native deleting virtual. The first-null Close return is the smallest branch, but it is already represented by `attemptReportDocumentReleasePrefix`. Advancing to the second slot read, deleting virtual, guard clear, document payload cleanup, or QObject destruction would invent a current owner and lifetime absent from the public fixture and retained original evidence.

This read-only audit concerns original `DEF_GUI_MO_64.exe` **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`. Addresses below are original VAs. No reconstructed executable version is asserted. Only this report was written; no source, test, fixture, binary, GUI/REA database, parent, DMO, Claude, or original artifact was changed, and no build or GUI session was acquired.

## CP170–194 reconciliation

The recent CP170–194 reports do not add a same-instance Report Open→Save→accepted-Close witness or a new accepted-Close owner. The relevant Report-owner chain remains CP164/165/169/171: the lazy accessor can construct and publish a document into a SubProject slot; Save re-resolves its owner; Close reads the slot twice and uses the second current value for deletion. CP169 closes the selected default Save branch statically, but does not bind its callback effects or the document value later observed by Close. The R300/connector work in CP170–194 addresses independent loader, parser, connector, and source-scope owners; it does not close the Report slot lifetime.

The qualified native Close body at `0x1400b5bb0..0x1400b5c2b` has this relevant order: first `S+0x30` read; immediate return on null; otherwise a second current read; deleting virtual `+0x18` on that second value; then QPointer clear. The retained original qualification hash is `2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9`. The deleting wrapper and payload destructor are qualified in CP160/171, but static code does not prove that the second pointer belongs to the Report document constructed during Open, is still live, or has the expected vtable and payload state.

Current source preserves that limit. `ReportDocumentSlot` accepts only explicit immutable initial null/non-null diagnostic observations. `attemptReportDocumentReleasePrefix` consumes the known first observation, returns the null branch for known-null, and for known-nonnull stops before invoking the deleting virtual. The fixture tests those semantics, foreign/detached slot refusal, and progress invalidation; it does not instantiate the native document, publish it through a live per-SubProject owner, or drive accepted public Close. `report.cpp` still refuses before Report document access. Thus neither adding another null-token case nor making the fixture's opaque identity deletable would advance a source-supported lifecycle transition.

## Exact missing witness and lowest-cost next investigation

The missing original witness is one natural same-instance Report **Open → Save → accepted Close** occurrence that records, at minimum:

* fresh Project/SubProject/document identities and generations at Open, Save, and Close, including any slot rebind or earlier QObject-parent destruction;
* the document constructor's proposed parent, accepted QObject parent/child-list membership, and relevant connection registrations;
* Save's delivered signal/virtual callbacks and any owner or slot changes they cause;
* both Close slot values (including the second value's live table), the deleting call and return, guard clear, current payload destruction order, and parent/child teardown order.

The lowest-cost non-GUI step is a bounded read-only pass over the already retained, byte-qualified Close admission and caller chain plus the CP152/170 signal-receiver qualifications, recording exact call sites and unresolved dynamic edges in a compact evidence table. This can narrow which callback and parent paths must be observed, without starting GUI control or opening/mutating a REA database. It cannot provide the missing same-instance pointer values or demonstrate accepted QObject ownership. If the retained static call chain is already fully covered, there is no further non-GUI investigation that can substitute for the occurrence witness; keep public Close and destructor progression guarded.

## Disjointness and scope

This report does not propose edits to connector or L6 source/test files. It reads the established Report diagnostic boundary only and writes `analysis/resume196-close-next.md`. Existing public Report slot fixture remains diagnostic evidence, not public Close acceptance. No tests were run.
