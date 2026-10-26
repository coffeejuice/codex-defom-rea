# CP87 — Report .DS owner: isolated edge and remaining binding

## Finding

Retained PE evidence identifies one narrow conditional Report-document cleanup edge: the first null read of the actual SubProject+0x30 QPointer returns without deleting a document or assigning the guard. CP82 already represents and tests this as an isolated candidate prefix through ReportDocumentSlot/attemptReportDocumentReleasePrefix. It is not a public Close integration: current slot knowledge begins Unknown, and no same-instance native observation binds the original QPointer value at the accepted Close occurrence.

No document Save or viewer mutation is independently callable from current candidate owners. Keep Report Open and Save guarded.

## Original owner and release evidence

The immutable original is DEF_GUI_MO_64 v11.0.2, SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc.

- Report document accessor 0x1400b5ad0 reads SubProject+0x30. On empty it allocates 0x70, constructs through 0x140643100 with the main-window parent argument, publishes the returned pointer through 0x140cbc920, and rereads the slot. This establishes lazy construction/reuse and parent argument; it does not by itself establish a complete shared lifetime.
- CP81 bounded disassembly identifies SubProject+0x30 as QPointer and helper 0x1400b5bb0 as its cleanup path. At 0x1400b5bc2 the helper reads the guard; a null first value branches to 0x1400b5c26 and returns without document virtual dispatch or 0x140cbc920. For nonnull it rereads, conditionally invokes the current target's deleting virtual +0x18 with flag 1, then assigns null to the then-current guard cell. The latter work needs an actual document deletion producer and QPointer effects.
- SubProject cleanup calls the helper after its earlier +0x48 conditional cleanup and before deleting its raw model and destroying fields. Project cleanup iterates its SubProject list. This is a static destructor chain, not proof that accepted normal Close reaches it for the live project.
- CP83 maps the deleting virtual to the Report document class and expands its destructor. The destructor has payload cleanup and QObject teardown before the wrapper frees allocation; that complete state machine remains unimplemented. Its QObject parent can independently invalidate the same QPointer.

The required binding remains a single lifecycle identity across: accepted Close entry, actual manager child encountered, Project, SubProject, actual document returned/published by the parented constructor, main-window parent/child lifetime, and QPointer value when 0x1400b5bb0 executes. Static caller paths do not prove those identities or their runtime ordering.

## Candidate edge already represented

source/apps/moproj2mst/include/m2m/engine/report_document_slot.hpp and src/engine/report_document_slot.cpp model the actual candidate SubProject member as Unknown/KnownNull/KnownNonNull, bind progress to that member's adapter scope, and preserve the immutable consumed branch. Unknown stops. Explicit KnownNull reaches only the no-delete/no-clear return. Nonnull retains an opaque identity and stops before its vtable. The helper does not create a document, invoke QObject, delete or clear, mark SubProject cleanup complete, or authorize public Close.

tests/mdt/test_report_document_slot.cpp already includes Release-active coverage for Unknown, explicit null first-read return and retry, nonnull stop, foreign/detached slot refusal, expired/reused scope, and storage preservation. These cases test candidate state-machine behavior with declared observations. They are not an original capture or proof that a production Report occurrence is null. No additional source-only change is justified by the current PE facts.

## .DS Save and viewer boundary

The native Report Save override 0x1412adf90 calls vt+0x80 0x1412aeb30, then 0x1412af0c0 on the same Report operation. The first callback computes the sibling .DS path and calls 0x140645030(document, path, 1) using the active SubProject document. reportDsPath(Project.path) represents only the filename expression and has no production consumer. The document argument is not available from the current model as a constructed Report object with qualified lifetime.

After that call, 0x1412af0c0 resolves the operation model/display and calls 0x140d0d6f0(display, model), then iterates the returned collection through further helpers. The candidate's ROI model path 0x140d0d670 is a different target. No source-bound owner or effect contract for this returned viewer model is available. Therefore neither the document call nor the viewer call can be skipped, mocked as completed, or replaced by the pure path helper.

## Smallest next evidence and test

The next useful evidence is not another isolated path test. Bind an accepted one-project Close root to the conditional static deletion chain for the same actual Project/SubProject and main window, then establish which side invalidates the same Report QPointer first. The minimal static/runtime receipt must show the accepted Close predecessor guards and manager child occurrence, the actual Project/SubProject/document identities, and their order relative to main-window QObject child teardown. If a runtime binding cannot be captured, keep the branch source-qualified but conditionally unintegrated.

Until then, the existing helper test is the smallest relevant test. A future same-instance binding test should supply an observed actual guard value to that helper and assert exactly the selected null or nonnull phase; it must not infer native null from empty C++ metadata. Nonnull continuation additionally requires the Report destructor's ordered payload/guard/QObject effects and a live then-current QPointer cell.

## Scope

Read-only PE/source/test review. No source, build, test execution, GUI, original runtime capture, or mutable REA action occurred.

