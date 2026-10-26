# CP87: current deferred/atomic Save-failure audit

## Result

I did not find a presently actionable public Save failure-order regression matching the requested “deferred/atomic, but original order established” criteria. The older `analysis/resume46-failure-matrix.md` and the `remaining` list in `analysis/wave3-native-save-validation.json` predate later direct-writer integration. In current `source/apps/moproj2mst/src/core/convert.cpp`, native Save applies Save-current effects before project output, executes per-operation property effects in operation order before MST, and reaches remesh after the MST attempt. The old Boolean BWZ/INI deferred-failure claim is explicitly settled by the CP46 current-Boolean test and integration receipt. It must not be reopened.

## Candidate checked and closed

The most plausible old deferred case was failure to create a Save-current directory. Original evidence is `analysis/wave3-earlier-save-mkdir-contract.json` and the instruction captures it names. However, current `save_current.cpp:263–272` already takes the native branch at the original helper call: it invokes `qt::nativeMkpathAbsolute(target)` immediately and records the result in `fx.directoryAttempts`. The native caller sets `saved.step2.nativeDirectories = true` before `saveCurrent`; `applyEffects(saved.step2)` therefore does not perform the directory creation later or throw for that failed attempt. The old mismatch description “directory creation is deferred and throws” is stale. The original helper's path-failure message box remains a UI/event-pump qualification, not evidence for a deferred filesystem attempt or a reason to create a physical-lock test.

The neighboring source-closed post-open remesh writer branch is also already covered by `tests/mdt/test_simcontrol300_remesh.cpp::remesh_save_ignores_post_open_write_and_close_errors_and_continues` (around line 503). Its private fake records writes after the selected failure and Close; `analysis/resume75-remesh-writer-failure.md` qualifies that no physical residue is asserted. This is isolated writer coverage, so it is not a public integration test, but another leaf test would duplicate it. The public failed-open remesh boundary is separately qualified as a default-handler termination path in `analysis/wave3-remesh-invalid-parameter-startup-contract.json`; the candidate's `M2M_E_UNSUPPORTED` guard is not an original returned status.

## Dependency for another useful increment

To find a distinct public case, the next audit needs the current integrated receipts/source map for the remaining native property writers, not the pre-integration failure matrix. A test should only be proposed where a writer still queues executable `FileEffects` in native mode despite an original-qualified per-writer open gate or continuation rule. The remaining `fx.writes.push_back` sites in `save_l3.cpp`, `save_l6.cpp`, and `ht_save.cpp` are guarded by non-native branches in the inspected current source; their existence alone does not establish that native Save is deferred. For any unresolved modal failure, retain the original GUI/message-loop qualification and do not launch a real dialog in the candidate test.

No source, test, build, GUI, or mutable REA changes were made for CP87.
