# CP102 — Report and DOE StepData owner seam

## Finding

No source-closed public Report or StepData increment is available independently of the pending viewer/refresh path. The Report `.DS` owner and DOE output `StepData` owner are separate objects with different lifetimes and native call sequences. Reusing the DOE output document for Report would cross those owner boundaries.

## Report `.DS`

`src/ops/report.cpp::ReportClass::loadOperation` reaches `loadClassData` before coupon geometry, report UI population, or signals. `ReportClass::loadClassData` currently stops at the documented refusal before calling the native SubProject `+0x30` accessor. Retained evidence (`analysis/resume80-report-ds-integration.md`, `analysis/resume87-report-ds-owner.md`, and `analysis/resume98-report-ds-project-blob-seam.md`) qualifies that accessor as lazy construction/reuse of a Report document, constructed with the actual main-window QObject parent and then published to the same SubProject. The document's complete Qt/TLS construction, same-instance lifetime, and accepted-Close teardown are not bound to candidate runtime identities. The path helper alone does not supply that owner. Keep Report Open/Save guarded.

## DOE output `StepData`

The nearby implemented path is separate. In `src/doe/session.cpp::prepareCurrentOutputSession`, the active Type2 `SubProject` must own the `Study`; the callback resets/reads `Study::outputDocument` at `DoeStudies/<study>/Control/<project-base>.DS`, binds the current `ConversionSession`, then calls `readSessionRecords`. This is a DOE Output document, not the Report document stored at SubProject `+0x30`.

`src/doe/session_records.cpp::importSession` invokes the StepData callback only when the imported record has a task and no `LayerName`. That callback resolves the actual project's main `GuiModel` and calls `readStepData`. `src/doe/stepdata.cpp::readStepData` processes StateVar/Object children, lazily creates `StepDataState` and its property table, then always calls the finalizer. `src/engine/viewer.cpp::finalizeStepDataPropertyTable` calls `step_current_object(nativeStep, true)` before its view loop, even when there are no imported children. StateVar construction still requires an explicit resolved constructor profile (`src/doe/statevar.cpp`), and populated viewer/property-table behavior remains guarded. Thus an empty StepData is not an inert, refresh-independent import seam.

The DOE Output caller also stops after `readDoeDocument`/`finishDoeOutputClassData` at the selected session/global-task application guard in `src/ops/noop.cpp::DoeOutputClass::loadOperation`. These source paths do not establish Report document construction or Report Save effects.

## Smallest next dependency

For Report, first bind one same-instance lifecycle receipt across the active `Project`/`SubProject`, actual main-window parent, returned and published Report document, and its accepted-Close release relative to QObject parent teardown. For StepData, a safe increment needs the concrete actual `GuiModel` owner and a bounded StateVar constructor profile plus the native finalizer's view/settings inputs and effects. Neither dependency is supplied by the Report path helper or current `Study::outputDocument` ownership.

No source, test, build, GUI, original-runtime, or mutable REA action was performed.
