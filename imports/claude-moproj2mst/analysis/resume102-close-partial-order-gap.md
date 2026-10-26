# CP102: accepted Close after Save remains the highest-impact uncovered boundary

## Finding

The remaining high-impact partial-effect ordering is the transition from a completed or failed explicit Save into accepted normal Close, including the Close-time question's nested Save and subsequent Project/SubProject teardown. This is distinct from CP99's source-closed Type-0 Save writer matrix. CP99 proves a candidate adapter's project/MST/remesh ordering, but its test returns from `convertDetailedWithServices`; it does not execute the original normal-Close root or any later Close effects.

Original evidence in `analysis/resume89-close-caller-root.md` orders the native Close prefix: read actual busy/main state; walk the actual manager's can-close children; for a successfully opened project, process5 selects the Close question and the source-qualified default Yes branch performs another Save; only an allowed path reaches the busy setters/notifier, followed by null Tree/Property/Scene refresh and manager cleanup. That nested Save means the explicit Save result and Close-time Save are separate effect sequences. An earlier writer error can leave files and project state changed before the Close decision, and a default-Yes nested Save can attempt them again.

The local SubProject/Project cleanup effect is independently source-qualified by `analysis/resume81-report-close-pe.md`: if SubProject `+0x30` still holds the Report document QPointer, its cleanup calls the document deleting virtual before model/member teardown. The Project destructor iterates its SubProjects and invokes their deleting wrappers. The retained evidence does **not** establish that accepted one-project normal Close reaches this Project destructor before main-window parent destruction; if the QPointer has already cleared, the explicit delete is skipped.

## Why no focused test is safe yet

Current `source/apps/moproj2mst/src/core/convert.cpp::run` has no native normal-Close call/coordinator. `ConversionSession::~ConversionSession` explicitly labels its release path “NOT an original normal-Close replay.” The process busy/main-state producer, same-attempt can-close results, Close modal/nested-Save completion, and accepted caller into Project teardown are not bound to the same public Session. Existing close-prefix tests are deliberately isolated and do not supply those producers. A new test that called the leaf helpers or treated Session destruction as Close would fabricate the order that needs proof.

The exact dependency is a source-qualified same-instance path from accepted Close entry `0x1400299a0` through busy/main and manager can-close checks, process5 modal answer/nested Save, and cleanup into either Project/SubProject deletion or parent-driven QObject destruction. Until that path is bound, do not add an end-to-end test or assert a Report-document deletion order. Once the dependency is closed, the focused acceptance should inject a project/MST failure in the explicit Save, record all durable earlier effects, then verify whether the default-Yes Close branch performs a second Save before the same Project's teardown; it must retain original GUI status separately from adapter `M2M_E_WRITE` and must not invent rollback.

This audit is read-only. No source, test, build, GUI/original execution, or mutable REA action was performed.
