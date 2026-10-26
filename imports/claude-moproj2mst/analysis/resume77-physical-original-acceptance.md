# CP77: candidate seeded-error coverage vs original Save contract

## Candidate evidence now recorded

CP75 and CP76 have focused passing logs in the grouped receipts:

- CP75 `m2m.moproj.project_output_physical_lock` passed 1/1 cases in `analysis/resume75-integrated-grouped4-tests.json`; source tree before/after was `1cf00d671c09b0119e3897bdf171ea231852cb80409e2c314a4bac72550b6317` (3,323 files). The log contains `[ ok ] project_output_seeded_buffer_lock_preserves_physical_postclose_file_error` and `0 of 1 cases failed`.
- CP76 `m2m.s.physical_postclose` passed 1/1 cases in `analysis/resume76-integrated-grouped-tests.json`; source tree before/after was `da5b45846b1384219108894a5309238e7a6abf2c32b256b97ac42d4722710e49` (3,327 files). The log contains `[ ok ] save_seeded_real_project_postclose_error_continues_boolean_mst_and_remesh` and `0 of 1 cases failed`. It has no lock-setup diagnostic, so the log is consistent with the lock-acquired path and its active assertions.
- CP76's whole grouped suite had 230 registered tests: 223 clean passes, 6 passes with logged gaps, 1 pass with logged differences, and no failures, skips, or incomplete output. Receipt status is `completed-with-recorded-gaps`; it also explicitly states no original executable was launched.

The candidate-only conclusions from the focused cases are:

1. With the test adapter's explicitly **artificial** initial one-byte FileDevice buffer and real byte-zero Windows lock, candidate FileDevice reports nonzero error 2 after close; the native project writer returns false.
2. Through the existing private per-call `convertDetailedWithServices` path, that injected actual candidate FileDevice error is observed after project close; Boolean BWZ/INI, MST, and remesh are subsequently produced with bytes matching a fresh control run.
3. `SaveCallState` reports project Attempted/Returned true and Succeeded false, while MST Attempted/Returned/Succeeded are true. Candidate `Result.code` and `ConversionDetails.returnedCode` are both `M2M_E_WRITE` with the project-write diagnostic.
4. The final project bytes are measured in the test but no particular residue is asserted or exported as an independent artifact. The test has no direct read of `mst::Stream::writeFailed()`; it records output-device write attempts/rejections separately from FileDevice error.

This is not a test of ordinary `m2m::convert`, the C ABI, or an unmodified native project-output buffer state. The factory exists only on the private C++ core path. The seed byte is not emitted by the original project serializer and is not evidence that the same precondition is reachable in an ordinary original Save. The earlier CP75 no-seed attempt is preserved in `build/candidate/Testing/Temporary/LastTest.log` and records that normal serialization did not call the adapter with an eligible small write; the later seeded setup intentionally changes that starting state.

## Original contract established by retained static evidence

The existing original-byte qualification is for `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`analysis/wave3-project-close-status-contract.json` and its bounded PE captures establish that original project writer `0x1400983c0` closes QFile at `0x140098dc8`, queries `QFile.error()` at `0x140098dd3`, and returns false on failed open or nonzero post-close QFile error. It does not query QTextStream status. The Qt close-state capture establishes the saved-initial-flush distinction: when the initial device-buffer flush fails, the nonzero file error is preserved through close; when that flush succeeds and native close succeeds, error can be cleared even after a later aboutToClose/text flush failure.

`analysis/save-semantics.md` statically binds original top Save `0x1400980d0`: it calls project Save at `0x1400981f0`, operation property writers at `0x140098201`, and MST at `0x14009820e` without branching on the project-writer Boolean between those stages. It returns the prior check/status local, not the combined project/MST Boolean. Thus the candidate's later Boolean/MST continuation is compatible with the recovered **order**, while candidate `M2M_E_WRITE` is an adapter status and must not be attributed to the original GUI Save.

Those facts are static source-qualified claims. The captures explicitly say there was no original process or injected original failure and do not determine concrete original partial bytes.

## Fresh original evidence still missing

No fresh original execution has shown that the original process can enter the CP75/76 seeded-buffer condition, what actual Win32/Qt error is sampled on that path, what project bytes remain, or the visible Save/action/message result. CP75/76 do not close those questions. The artificial seed means even a perfect candidate error reproduction is a device-state exercise, not an original parity vector.

The app-control release gate still bars acquiring another GUI-control session until a supported release operation and verifiable receipt/status exist. No GUI was used for CP77. Under that gate, the smallest independent original comparison available now is an **offline, hash-bound PE recheck**: read the retained original executable and QtCore DLL, compare their current SHA-256 values with the hashes above, and independently verify only the narrow instruction windows for the project writer's post-close error query plus the three caller sites (project, property dispatch, MST). This can confirm that the written static contract matches the retained shipped bytes without app control. It cannot demonstrate a physical original failure or GUI status.

If the goal requires original runtime parity, the remaining dependency is not another candidate test: first establish the app-control release receipt, then separately qualify an original-safe disposable-project failure condition that the unmodified original writer can naturally reach. If the only candidate condition still requires an artificial initial buffer, retain that distinction rather than claiming a matched original fault.

## Scope

This audit inspected the CP75/76 test reports, focused/grouped receipts and logs, the original project-close static receipt, and the retained Save sequence. It made no source/test/build/GUI/mutable REA changes.
