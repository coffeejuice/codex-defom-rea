# CP80 — Report .DS helper integration boundary

## Finding

The existing reportDsPath(Project.path) helper is a tested pure value computation, but it is not called by production Report Open or Save code. It represents the native filename expression only. Native Report Save's first Report-specific stage consumes both that path and the active SubProject's Report document object; it is followed by a separate viewer-model routine. The public Report Open and Save guards therefore remain necessary.

## Exact native Save stage

The retained L7 evidence establishes the following order:

1. Generic Save dispatch calls the active operation's vt+0x68 (0x140052ec0). Report's override is 0x1412adf90.
2. 0x1412adf90 invokes Report vt+0x80 (0x1412aeb30) first, then calls 0x1412af0c0 on the same Report operation receiver.
3. The body of 0x1412aeb30 derives an absolute sibling path from the current project path: absolute parent directory, native separator, cleaned QFileInfo baseName, and .DS. It resolves the active SubProject's Report document and calls 0x140645030(document, path, 1).
4. Only after that document call does 0x1412adf90 call 0x1412af0c0. Its bounded body resolves the current operation model, Project and Display, calls 0x140d0d6f0(display, model), obtains a collection count and iterates entries with later helper calls. This is distinct from the already-portable ROI helper 0x140d0d670.
5. The common Report property path (F80/F90/FB0 via ReportProperties vt+0x60, 0x1412b1f50) is later in the operation Save route. A path-only helper cannot stand in for the document and viewer calls that precede it.

The retained body establishes the call sequence and arguments. This review does not assign a stronger meaning to mode 1, claim a physical .DS write at the call, or infer complete per-entry side effects from the opaque downstream helpers.

## Candidate integration boundary

src/ops/report_ds_path.cpp derives the path from Project.path; tests cover normal paths, a multi-dot filename and a dotted parent directory. Search of production sources finds no consumer. src/ops/report.cpp::ReportClass::loadClassData still refuses before .DS document access, so a normal public Open of active Report terminates before Save. If Report reaches saveCurrentOperation through another caller, src/engine/save_current.cpp also refuses it before either native Report Save callback. These guards correctly prevent the path helper from being mistaken for a reconstructed document Save.

The native document is lazily obtained through the active SubProject's +0x30 slot (0x1400b5ad0). If empty, the accessor allocates 0x70 bytes, constructs with the actual main window as QObject parent (0x140643100), then publishes the returned object in the slot; if populated, it reuses that object. The candidate has no equivalent SubProject document owner. Getter/setter calls 0x140c1aef0 and 0x140cbc920 do not by themselves prove the slot wrapper, reference-count semantics, or destruction order. The captured QObject constructor includes thread/private/parent effects and seven main-window connections. No normal Close trace establishes when the slot releases its reference relative to main-window child teardown.

Even if this document owner were closed, the next required Save callback 0x1412af0c0 still has no source-bound candidate owner for the object returned by 0x140d0d6f0, or a complete contract for its per-entry helpers, file/path effects and failures. Do not substitute Project.display, displayRoiModel, or a fabricated .DS document.

## Minimum contract to reopen public integration

A public Report Save increment needs all of the following, in native order:

- a current active SubProject and its same reusable Report document owner;
- constructor/adoption semantics for the parented SP+0x30 object, including failure/unwind and normal Close release;
- invocation of the qualified document method with the Project-derived .DS path and mode 1, preserving its order before the viewer callback;
- actual viewer-model return owner and required per-entry effects/failure behavior from 0x1412af0c0;
- only then, the existing later Report property serialization order.

Until these edges are source-qualified, leave Report Open and Save guarded. The only safe source boundary remains the already-implemented path helper; no additional production code is justified by this review.

## Evidence and limits

- evidence/gui_mo/s1/mdt/L7/s0.c: retained Report vt+0x68 caller and vt+0x80 body; the latter derives .DS and calls 0x140645030 with the subproject document.
- analysis/resume63-report-viewer-bounded-capture.txt: bounded native constructor and 0x1412af0c0 body/caller evidence.
- analysis/resume76-report-doc-owner-trace.md and analysis/resume71-report-owner-edge.md: owner reuse/publication and unresolved Close ownership boundary.
- source/apps/moproj2mst/src/ops/report.cpp: current Open refusal; source/apps/moproj2mst/src/engine/save_current.cpp: current Report Save refusal.
- source/apps/moproj2mst/src/ops/report_ds_path.cpp and tests/s/test_report_ds_path.cpp: isolated path computation and tests only.

Read-only static/source audit. No source, test, build, GUI, original execution, or mutable REA action occurred.

