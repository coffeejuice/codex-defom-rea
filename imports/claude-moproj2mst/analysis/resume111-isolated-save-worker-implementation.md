# CP111: private isolated Save worker

Implemented only the assigned same-image worker and focused test. This route is opt-in candidate execution under the static fresh-default assumption; its fixed own-process endpoint omits the original context/debugger/filter prelude. It is not original-runtime termination acceptance or full native Close recovery. Public C ABI and default CLI parsing/behavior were not edited by this lane.

## Worker contract

NEW `src/cli/isolated_save_worker.cpp` defines `m2m::cli::isolatedSaveWorkerMain(int,wchar_t**)`. It requires exactly7 nonnull arguments: executable, exact `--m2m-isolated-save-worker-v1`, exact termination opt-in, decimal case25, decimal pipe uintptr, nonzero decimal nonce and decimal unrelated-handle0. Unsigned parsing rejects signs, whitespace, suffixes, alternate bases and overflow and is bounded by the target's decimal width. It validates root's ProductIsolatedSave/RemeshV2/ProductIsolated selection, a nonzero non-INVALID pipe and FILE_TYPE_PIPE before publication. Malformed contract exits65 with no protocol bytes.

On the admitted role it sends Started with nonce/site/PID, reads the fixed job via root/gradient-owned `readIsolatedJob(current_path())`, requires job.nonce equality, then restores job.workingDirectory with SetCurrentDirectoryW. Nonce mismatch, invalid job or failed CWD restoration produces no Returned record and setup exit66. Only after those checks does it establish C numeric locale and call `convertDetailedWithServices` using the actual job.project/output/flags/codepage/deformDir, default guiUnit0 and default OriginalSettings. There are no product guiUnit/moInputMode flags in this first cut. The job reader owns the version/size/UTF16/path/flags/codepage validation; this worker does not duplicate a second job format.

The local RemeshTerminalCallService always selects FreshDefault as the explicitly admitted static assumption, not an observed host/original handler. At actual writer failed-open dispatch it validates the exact leaf request and modeled errno22, emits v2 Boundary from the root forwarder's copied live SaveCallState, then requests TerminateProcess(GetCurrentProcess(), C0000417). No native context/unwind/filter APIs or host invalid-parameter handler are invoked. If Boundary publication fails or the endpoint returns, the service reports the appropriate failure to the leaf; failClosed sends BackendFailure once and returns to the leaf's established no-unwind parked state. No false noreturn annotation is placed on that backend callback. Unexpected tags map to InvalidFailureCode.

Ordinary conversion return validates adapter code0..7 and any returnedCode observation, emits Returned with the optional actual saveCall snapshot, then exitsOS0. Result messages go to stderr. Payload7/OS0 therefore remains distinct from terminal status and from setup exit66. Started/Returned publication failure emits best-effort BackendFailure and parks rather than returning false success. The accepted pipe is closed on ordinary function exit; process death has no C++ unwind. Failure parking and missing final records must remain supervisor failures, never relabeled native success.

## Tests and static preflight

NEW `tests/g0/test_cli_isolated_worker.cpp` has seven Release-active cases. Three strict-contract groups use a real pipe plus an empty contained CWD and verify no bytes for wrong arity/null argument, wrong role/opt-in/case, signed/hex/suffixed/overflow numeric inputs, zero nonce or unrelated handle. Four owned-child cases run the same worker function through the test's exact wide role entry: missing-project WRITE_SAVE returns adapterPARSE3 with transportOS0; malformed job exits66 without Returned; missing caller CWD does likewise; a nonzero stale job nonce is rejected before CWD/conversion. Valid test jobs are written in PreparedCase::bindNonce after the supervisor generates its nonce. Missing-file probes and diagnostics remain beneath M2M_TEST_SCRATCH.

Test entry is one `wmain`, required for the exact wchar_t child contract, with ordinary `testkit::run_all()` when not in the explicit role. No endpoint is invoked in the parent Testkit process. The child tests here exercise returned/setup/refusal boundaries; full terminal B133/product parent-receipt acceptance is an integration dependency, not claimed by these seven cases. Target `m2m_g0_cli_isolated_worker`; CTest `m2m.g0.cli_isolated_worker`. Root's CMake links m2m_cli_private; this lane changed no build registration.

Static preflight resolved every quoted include, checked Testkit header/main/macros (CHECK/CHECK_EQ/REQUIRE; no C assert/REQUIRE_EQ), seven cases, balanced delimiters/whitespace and the source order job-read -> nonce-check -> CWD-restore -> conversion. No compiler/build/test executable was invoked. Root must build/run this target, existing worker/protocol/ABI cases, actual opt-in product controls/terminal receipts, and the required complete frozen launcher before acceptance.

## Frozen files

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
|src/cli/isolated_save_worker.cpp|19d3191d1039772fea9e75e49a5042be5781e076cba3714a4ab5bfa47dfb182f|
|tests/g0/test_cli_isolated_worker.cpp|78d88b2e0e6fa47a208ece823f84cb8ac672fd1f354c0c4e786e707fd62ac90f|

Read-only interface hashes at preflight: isolated_save.hpp b505e03bfdb01b2cf53e7395931adb09c896b3aa179cdab8af56aa41d5267be7; isolated_save_job.hpp de28e3e369a34eeff8d12513430eff4aa669e650dd91d4051a66f338b079102d; supervisor.hpp 7c256a901571ddbb786f44033f1e348010096343a6a9ae8375c26536ba2f2c3e; conversion_call_services.hpp 4b0c5f823618a3804dd6f470ee7e263d6aabe5e2519b38605c7162a36a2454bd; root remesh call service0f3f3cdd1ed46ab92521a3d15273983fa3d36b8439dbb43d432afdda7d580e81. These bind this preflight, not another lane's final freeze.

Native boundary evidence remains CP109 `resume109-remesh-handler-prelude.md` SHA387242795bf33d991524879f1c6c6378aeabfabd284c5dd97b6353da331eebb6 and its original binary/range qualifications. CP110's distinction between modeled prefix and actual native prelude remains applicable. Root owns main/parent dispatcher, job serializer/receipt and harness integration; this lane owns none of those files.

Only the two assigned new source/test files and this receipt were written. No root header/CMake/main/harness/public ABI file was edited. No build/test, GUI, original execution or mutable REA operation occurred. Source is frozen/released for root validation; default/public editor and palette guards remain unchanged.
