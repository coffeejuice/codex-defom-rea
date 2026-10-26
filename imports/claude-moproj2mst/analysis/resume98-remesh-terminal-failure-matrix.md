# CP98: one remaining remesh terminal-order case

## Established behavior

The retained original contract `analysis/wave3-remesh-save-lifecycle-contract.json` places remesh after current-operation Save, project write, property writes and MST. In the **disabled** remesh branch, the caller invokes Qt `QFile::remove("LOCAL_REMESH.DAT")` and ignores its Boolean return. Project/MST writer false values do not prevent reaching this tail. The original GUI Save returns its earlier check/status local; that return is not the converter adapter's mapped error. No claim here assigns an original M2M status.

Current `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp::saveRemeshAtProjectSave` performs `DeleteFileW` in the disabled branch, stores the result in `removalSucceeded`, and returns `ApplyResult::Completed` regardless. `convert.cpp` therefore continues to its existing MST-before-project adapter error mapping. Existing coverage separates these facts: `native_full_save_disabled_remesh_removes_unmarked_primary_and_ignores_denial` covers a denied delete with otherwise successful Save; `save_mst_failure_precedes_project_postclose_failure_after_remesh` covers simultaneous MST/project false with successful enabled remesh. The tests do not combine denied disabled-remesh removal with earlier returned Save failures.

## Smallest distinct matrix increment

Add a candidate-only paired public Save case combining those existing fixtures:

1. Use a disposable project whose `LOCAL_REMESH.DAT` contains the existing unmarked `DONE\n` profile, with no enabled fallback. Hold a Windows handle that permits the Open read (`FILE_SHARE_READ | FILE_SHARE_WRITE`) but denies delete sharing. This makes remesh disabled while preserving the sentinel through the later remove attempt.
2. In the project-output `aboutToClose` callback, arm the existing deterministic MST write-open denial after project Open has completed. Declare the injected post-close project error through the existing output service.
3. Assert ordered candidate effects: Boolean outputs still occur; project writer returns false; MST write is attempted and fails; disabled-remesh removal is attempted and fails without changing the sentinel; final adapter result remains the MST `M2M_E_WRITE` with its MST-path message; and `SaveCallState` records both project and MST failures.

This is a useful independent matrix cell because it proves a failed, ignored remesh cleanup cannot replace an already selected Save error or accidentally enter the enabled-remesh unsupported guard. It requires no production seam if the existing project-output callback can arm the MST and retain the delete-denial handle. Never seed or parse the editor300 decimal-overflow profiles for this case.

## Evidence and limits

This is a proposed candidate integration test, not fresh original fault execution. The original evidence establishes operation order and ignored removal result; the precise returned GUI status under simultaneous project/MST failure is still the earlier native status local and is not inferred from `M2M_E_WRITE`. Candidate status precedence is its existing adapter policy. Assert the sentinel only because the test uses a deterministic Windows delete-sharing denial; do not generalize exact bytes to other removal failures.

No source, test, build, GUI, original execution, or mutable REA changes were made for CP98.
