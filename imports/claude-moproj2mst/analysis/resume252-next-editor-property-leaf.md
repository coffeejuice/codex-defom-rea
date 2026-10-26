# CP252 next finite editor/property leaf

The next dependency-ready editor leaf is the initial model-dimension admission in `openInitialEditorInto`. Replace its direct public-step scalar read with the already implemented unguarded `GuiModel::dimension()` value query. This needs no new model API or shared model-file edit and is separate from CP243's Environment Apply fallback migration.

Only this report is written. No source/test/build, shared recovery state, original process, GUI or mutable REA state changed.

## Reconciled current state

Current source already contains CP245's private reaction-rate coordinator, CP248's meshing writeback command, CP249's heating unit migration and CP251's MultiBlow control command. Do not reassign them. The reaction coordinator's local simCtl remains intentional; it is not an untouched callback alias. The frequency/title/StepCount/page-state identity migrations are likewise present.

The lifecycle source still has two direct dimension accesses in initial admission (`model.step` presence and `model.step->dimension`) and a separate backup owner setter later. The former is the selected finite leaf. The latter belongs to copying/reparenting the native backup and should not be folded into this change. Spring combo object/name traversal and cached control references also remain separate. CP165 broader meshing, cache1 live guard and native Report accepted-Close witness gates are not advanced.

## Exact production change

In `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp::openInitialEditorInto`, current sequence:

```cpp
report = {};
if (session.constructionStarted) {
    report.issue = InitialOpenIssue::AlreadyStarted;
    return;
}
session.constructionStarted = true;
auto* control = model.simCtl();
if (!control || !model.step || model.step->dimension != 3) {
    report.issue = InitialOpenIssue::InvalidModel;
    return;
}
```

Replace only the conditional with:

```cpp
if (!control || model.dimension() != 3) {
```

Keep the report reset, AlreadyStarted short circuit, constructionStarted assignment and the existing single guarded simCtl lookup at their original positions. Leave numeric-locale admission after this condition and all source metadata publication/page fills after locale admission.

The existing `dimension() const noexcept` body is `return step ? step->dimension : 0;`, with no geometry guard. That is the exact useful contract here. Present dimensions 2, 0, negative or other non-3 values still refuse; only 3 passes. Missing step makes simCtl return null, so `!control` still suppresses the dimension query. In supported synchronous execution there is no intervening callback between simCtl and this conditional; no valid execution has a non-null current control with an absent current step at this point. Do not claim semantics for concurrent mutation/undefined lifetime states.

This differs from CP243's Apply assembly, where missing step had to produce fallback 3 and live zero had to stay zero; there dimension() alone was insufficient. At this initial equality-to-3 admission, absent and zero are both invalid, so the existing zero-without-step getter is sufficient. No dimensionOr extension or additional identity API is needed.

Do not replace the condition with a geometry getter, add another guard, hoist dimension before simCtl, move locale admission, add a range-normalizing helper, create a default step or change the function's exception policy. A parked simCtl still throws after constructionStarted is committed and before dimension/locale/source metadata. This extraction only removes a direct scalar storage access; the local control and later cached aliases remain.

## Actual caller and tests

`openInitialEditorInto` is the actual caller seam. It is reached by `openInitialEditor` and by `StagedEditor300::constructPageState`; the lifecycle tests invoke it directly and through the wrapper. The ordinary successful 3D Open/page/Environment cases remain relevant regressions.

Reserve only:

- `source/apps/moproj2mst/src/engine/simcontrol300_lifecycle.cpp`: one conditional expression.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_lifecycle.cpp`: focused initial-admission cases.

No engine.hpp/gui_model.cpp, CP248 meshing source, CP245 owner source, property dispatch, loader, ABI/layout or new target edit is required. Root must still reserve the lifecycle test/source pair and preserve source-stable build/freeze boundaries.

Add a small direct `openInitialEditorInto` matrix using fresh session/report/model each time:

1. Live dimensions 2 and 0: expect Preflight/InvalidModel, constructionStarted true, pageStateComplete false, no reached page/page flags, and null sourceModel/sourceControl/sourceRemesh. To make live zero, assign the fixture field directly and restore it before unrelated native work/cleanup if needed; do not invoke dimension-changing initialization just for this check.
2. Absent step: same InvalidModel result and no storage recreation.
3. Give those invalid models an Unrecovered numeric locale too, proving InvalidModel still wins before the locale check.
4. Live 3 with Unrecovered numeric locale: expect UnrecoveredNumericLocale rather than InvalidModel, still before source metadata/page publication. This establishes acceptance of actual 3 without relying solely on complete page construction.
5. A second call on one refused session: expect AlreadyStarted and no retry/reconstruction. Do not reset constructionStarted merely to force the matrix through one reused session.

Use the existing Testkit entry point and CHECK/CHECK_EQ/REQUIRE; no C assert. Existing Io records effectful writes/removes, but does not count all read calls. For claims of no I/O in the new preflight test, use a small local Io whose every read/write/remove method counts or fails; an empty effects vector alone is insufficient to prove no reads. A fresh preflight should reach none of them. Avoid reusing post-Open counters without resetting a baseline.

Existing `m2m_mdt_gui_model_dimension` directly covers the scalar getter/setter environment and CP243 fallback cases. Root's focused group should be `m2m.mdt.simcontrol300_lifecycle` and `m2m.mdt.gui_model_dimension`, with the normal owner/viewer/stopping_cached integration before complete source-stable test/freeze. CMake derives the existing lifecycle target `m2m_mdt_simcontrol300_lifecycle`; confirm configured names. No new parked fixture is necessary for this one-expression migration; source review establishes query position, and current simCtl/viewer tests exercise the preexisting barrier. Do not report a parked lifecycle callback test unless actually added/executed.

## Evidence and limits

Current source and initial-session/report definitions establish the adapter admission contract above. CP226 and the existing getter declaration retain original GUI twin attribution `0x140c7c0e0/0x140c7c0a0` for unguarded dimension/zero-without-step behavior. CP242's independent review and CP243's implementation review explicitly distinguish adapter fallback/evaluation timing from original live-query timing. Those qualifications apply here too: this initial InvalidModel preflight is a reconstructed adapter admission, not proof of the original editor's graceful handling of dimension zero or null control.

This is not a fresh original observation or recovered native new routine. It does not prove public construction/Save completion, ownership closure, same-incarnation identity, physical Qt behavior or reentrancy. RINHERIT remains blocked on the joined actual Report document/accepted-Close witness identified in CP203/CP249; no attempt to substitute DOE SessionDocument or constructor flags follows from this ready editor leaf.
