# Resume 48 sequential public ABI test

## Change

Added `sequential_public_calls_keep_single_project_failure_state_isolated` to `source/apps/moproj2mst/tests/g0/test_abi.cpp`. It creates two distinct project directories beneath the configured disposable test scratch root. Each has a different remesh input (`FILTER_DISTANCE 7` versus `19`) and no `.moproj` file, then calls the exported wide-path `m2m_convert_w` entrypoint sequentially with `M2M_FLAG_WRITE_SAVE`.

For both calls, the test requires the already source-qualified adapter result `M2M_E_PARSE`, checks the diagnostic identifies the failed project read, checks the call did not create its MST output, and checks the remesh input bytes remain unchanged. Existing evidence in `tests/g0/test_abi_open_failure.cpp::abi_full_open_missing_project_reaches_later_save_rejection` establishes this failure ordering for one call: Open's remesh read occurs, then Save rejects `openedOk == false`, and the remesh Save effects do not occur. The new test repeats that bounded contract with distinct per-call paths and data.

## Qualification

This test checks observable results and file effects across two sequential public ABI calls. It does not establish that any hidden native process-global state is reset, because the current evidence does not define a cross-project reset contract and this failure path does not complete Save/Close. It also does not prove successful repeated conversions or fresh original parity. Do not use it alone to mark either `abi_and_regression_verified` or `failure_and_close_verified` complete.

No production code, build files, fixtures, app state, or REA data were changed. The test was not built or run, as requested; root owns validation. Compile/runtime behavior therefore remains unverified.

## Existing coverage reconciled

- Existing ABI tests already cover argument checks, wide and UTF-8 entrypoints on error paths, error-message termination/truncation, settings parsing per call, and representative Open failure statuses.
- Existing internal session tests cover ownership and teardown behavior, but are not public ABI repeated-success tests.
- The checkpoint46 suite receipt records 150 tests with zero failed assertions and explicitly qualifies `g0.abi`'s corpus assertion as permitting either `M2M_OK` or `M2M_E_UNSUPPORTED`.

The new case fills only the narrow missing scenario of two sequential, distinct-project calls through the public ABI on the source-supported Open-failure/Save-rejection path.
