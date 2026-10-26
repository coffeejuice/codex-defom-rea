# Checkpoint 71: acceptance boundary after CP70

## CP70 receipt status

The frozen grouped run at `build/test-runs/1792884515322017700` is source-bound: the recorded source-tree SHA-256 is `b35d9d9561f437def99edb3f24c022c8cbb369d32537ac1d1b0de45ff36487a6` both before and after the run. It completed 216 registered tests: 209 clean passes, 6 passes with logged gaps, 1 pass with logged differences, and no failures, skips, or incomplete tests. The receipt itself warns that the historical Save-folder comparator is diagnostic and that CTest success may include permissive assertions or expected refusals.

The CP70 additions now establish two useful candidate-internal boundaries:

- `m2m.g0.detailed_outcome` passes its single test. It proves detail-object reset on an early no-path error and subsequent binding of an actual Native StudyIo with an empty journal for a missing WRITE_SAVE project; the returned detail code matches the returned result. This is not a successful project conversion or C ABI test.
- `m2m.doe.type2_native_open_guard` passes two tests. The lifecycle case proves a native Type2 XML write and journal retention through actual candidate Session/Project teardown, reaching the refresh boundary before the qualified owner guard returns unsupported. It does not prove refresh delivery, later PAR generation, or final `.moproj` serialization.

The run remains qualified by five historical Save-folder DIFF rows (A55, B120, B121, B122, B98), six logged gaps and one logged difference. The count of green tests is not an acceptance percentage and does not close those comparisons. In particular, historical oracles are not fresh original-session evidence.

## Highest-priority next executable boundary

The next high-value lifecycle check is the **public conversion's post-open native project-write failure ordering**, including its returned status. This is the next stage after CP70's successful native XML writer and teardown observation, and it checks that an actual Save can continue through later side effects when the project writer returns false.

Existing coverage is not equivalent:

- `save_project_open_failure_preserves_project_but_continues_boolean_and_mst` covers a project-file open failure and proves prior project bytes are preserved while Boolean sidecars and MST continue.
- `project_native_locked_direct_write_uses_post_close_qfile_error` parses before locking and exercises a large native direct write, but the observed initial QFile flush succeeds and clears the earlier error; the writer returns true. This covers the distinction between QTextStream failure and post-close QFile error, not a persistent false writer result.
- `file_device_error_after_close_uses_saved_initial_flush_result` covers the lower-level state rule, not the project writer or top-level Save continuation.

The existing analysis `analysis/resume70-save-failure-next.md` identifies the exact missing condition: a deterministic failure must be armed **after input Open/read and before native project output opens**. A lock installed before public conversion is invalid because it can prevent input reading. The smallest executable dependency is a narrowly scoped test-only fault hook/device factory or phase barrier on the native project output path. With that seam, use a disposable project and assert only qualified effects: project output was attempted and returned the persistent QFile failure; later expected Boolean sidecars and MST were attempted/written; and the public result maps the project-writer failure to the existing `M2M_E_WRITE` status. Do not assert guessed partial project bytes. Keep lower-level direct-writer state assertions separate from public lifecycle assertions.

This test should be added only after the owner supplies the seam and root has integrated it; no current CP70 receipt demonstrates that behavior. If a production seam is not acceptable, the next evidence action is to find an existing callback/barrier after input Open and before project output; current inspected tests do not expose one.

## Separate architecture gate

CP70 also leaves a more upstream DOE boundary unresolved: actual delivery to the `DoeTableRefresh` receiver. The native XML write reaches `RefreshBoundaryReached`, then the candidate correctly stops at the qualified missing-owner guard (`main-window +458` unavailable). The journal observer is not proof of a timer stop, UI refresh, PAR side effects, or completed Type2 Save. Closure requires a source-qualified real owner/thread/widget setup or retained original lifecycle evidence that establishes that receiver and its effects. Do not synthesize PAR writes before that owner is qualified.

## Scope

This audit reads the CP70 receipt, focused logs, and existing failure-gap reports only. It made no source, test, build, GUI, or mutable REA changes.
