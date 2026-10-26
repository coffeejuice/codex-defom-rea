# CP74: acceptance gate after the CP73 status-injection pair

## CP73 validation is now available

The CP73 design has been implemented and exercised in the integrated grouped2 run. The receipt `analysis/resume73-integrated-grouped2-tests.json` binds source SHA-256 `25f0af4c32432a0dc082ca1469f64ee8194fafa80a6e69b4331ec4652640ee5f` before and after the run. It records 219 tests: 212 clean passes, 6 passes with logged gaps, 1 pass with logged differences, zero failures, and `completed-with-recorded-gaps`.

Focused evidence:

- `m2m.s.save_io` passed all 71 cases; its log includes `[ ok ] save_project_reported_postclose_error_continues_properties_mst_and_remesh` and ends `0 of 71 cases failed`.
- `m2m.moproj.project_output_device` passed its declared post-close status test (`0 of 1 cases failed`).
- The integrated case uses two fresh disposable fixture copies. Both execute real FileDevice open/serialization/close with real post-close error 0. In the injected run, the private adapter reports scripted error 7 at the same query. It verifies Boolean BWZ/INI, MST, and remesh output are generated after the query and match the control bytes; `SaveCallState` shows project attempted/returned but failed, MST attempted/returned/succeeded; `Result` is `M2M_E_WRITE` with project failure selected after later stages, and `details.returnedCode` agrees.

This closes the **candidate orchestration/status-mapping** test designed in CP73. It does not establish a physical QFile post-close error: the real delegated FileDevice reports zero in both runs. Nor does it establish fresh original fault behavior. The six logged gaps, one diagnostic difference, and historical Save-folder DIFFs remain; a green grouped run is not an overall parity claim.

## Highest-priority next non-GUI gate: a real persistent post-close QFile error

The nearest unresolved lifecycle boundary is physical production of the nonzero QFile error that the scripted CP73 adapter supplied. The retained `analysis/wave3-project-close-status-contract.json` says the original native project writer samples QFile.error after Close and ignores QTextStream.status. It also explicitly leaves concrete OS partial-byte and error-reset outcomes unqualified. Existing `project_native_locked_direct_write_uses_post_close_qfile_error` is not sufficient: its selected lock run reaches the close-recovery behavior and returns true, while the CP73 injected run intentionally keeps the actual FileDevice error at zero.

A bounded next experiment can stay headless and use disposable scratch:

1. Reuse the direct native project-writer setup that reads/parses the project before taking an output lock, so the lock cannot block input Open.
2. Sweep a small, recorded set of payload sizes around the existing 16 KiB/`0x4000` stream/device buffering boundary and lock ranges/offsets. Change one parameter per run; do not claim a physical failure from the scripted factory case.
3. Capture actual lower FileDevice `errorAfterClose`, QTextStream failure state, returned native writer Boolean, and before/after project hash/size for each run. A successful result with file error zero is a calibration observation, not a failure of the test.
4. Once one deterministic setup yields nonzero actual QFile error after Close, run it through the public detailed conversion at the after-input/pre-project-output phase (using the now-existing private project-output factory/service as a delegating device). Assert the same downstream Boolean/MST/remesh continuation and project `M2M_E_WRITE` mapping, but keep physical partial project bytes as measured output rather than a predicted prefix.

If no stable physical condition is found, preserve that as the unresolved boundary. Do not relabel error 7 injection as a physical flush failure and do not weaken the test to assert only QTextStream failure.

## Separate unresolved owner gate

The Type2 DOE path still reaches `RefreshBoundaryReached` and then stops at the source-qualified missing native `DoeTableRefresh` owner. CP70's journal/lifetime test and CP73's project output tests do not establish timer stop, receiver delivery, UI refresh, subsequent PAR generation, or full DOE Save completion. That broader gate requires source-qualified live owner/thread/widget lifecycle evidence; it is not the next local headless writer check and remains independent of the physical project-close experiment.

## Scope and provenance

This audit read the CP73 design, implementation/static-review reports, grouped2 build/test receipts, and focused test logs. It made no source, test, build, GUI, or mutable REA changes.
