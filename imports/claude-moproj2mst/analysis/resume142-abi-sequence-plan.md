# CP142 public ABI success-then-failure test plan

## Scope and write set

When released, add exactly `source/apps/moproj2mst/tests/g0/test_abi_write_save_sequence.cpp` and this plan. Do not change production, CMake, shared fixtures, or existing ABI tests. The existing CMake glob automatically discovers `tests/g0/test_*.cpp`; the `stem MATCHES "^test_abi"` branch links `moproj2mst` and `testkit`, so this file exercises the exported DLL API. No build is authorized in this step.

## Proposed sequence

The test uses a unique child of `m2m_test::scratchRoot()` and validates it with `m2m_test::scratchOutput()`. It recursively copies the retained input tree `evidence/gui_mo/oracle_b/RS_08_HEAT1_before` to `<scratch>/abi-save-sequence/first/`, without writing to the retained evidence. Before conversion, preserve and later restore `M2M_MO_INPUT_MODE`; set it to `0` for both calls, matching the mode-0 retained fixture and bypassing the separately guarded Forming editor path. Set the documented code page to 1252 and GUI unit to 0, checking each setter result. Pass explicit source and destination paths to `m2m_convert_w` with `M2M_FLAG_WRITE_SAVE`.

1. **Successful first call:** source is the copied `HEAT1.moproj`; destination is a new `HEAT1.MST` in the copied project directory. Require exactly `M2M_OK`, not the current permissive `M2M_OK || M2M_E_UNSUPPORTED` contract. Keep the returned message buffer for diagnosis. Verify the generated tree against retained output `evidence/gui_mo/oracle_b/RS_08_HEAT1`, using the exclusions/path rules in `tests/s/test_save_folder.cpp`; report raw path/byte differences first, and keep any separately qualified normalization view distinct. CP140 `m2m.s.save_folder` already records this case as SAME with 0 differing, 0 missing, 0 extra, and no comparator adjustment, but the public ABI call/status is the new assertion.
2. **Failure second call, same process:** create a separate disposable `B` directory, deliberately omit `B.moproj`, and write `LOC_DEN_CTRL.DAT` as the existing ABI test does (`FILTER_DISTANCE 19\nDONE\n`). Call `m2m_convert_w` with `M2M_FLAG_WRITE_SAVE` and explicit `B.MST` destination. Require exactly `M2M_E_PARSE`, the established `reading project` message, absence of `B.MST`, and byte-identical `LOC_DEN_CTRL.DAT`. Recheck that the successful first call's output tree is unchanged.

Record the current working directory before either conversion and assert it is the same after each call; use RAII for environment restoration and any CWD change introduced by test helpers. Do not delete test scratch during the test: CTest owns that disposable root and retained failure artifacts aid diagnosis. Use a process-unique path and fail clearly if the chosen scratch path already exists rather than overwriting it.

## Fixture and evidence binding

- Input project: `evidence/gui_mo/oracle_b/RS_08_HEAT1_before/HEAT1.moproj`, SHA-256 `e758aa107b6d8c849b1417f9f12177f70b282a4ecd6fa96d1231f6f748e30093`.
- Retained output MST: `evidence/gui_mo/oracle_b/RS_08_HEAT1/HEAT1.MST`, SHA-256 `4245304b4cb3941b1a8ff10d21a714d1290a696adf2834f0626ff5ae5896eec7`.
- CP140 candidate freeze: source tree SHA-256 `9a9eb34f0a934ffd5fde3e02b5bb6b48152121ab91f608b215cdeb820b9bf981`; `moproj2mst.dll` SHA-256 `17ad2635f4dde3fbe0fdd56c700117a6278a93b82d5719a3062269b4f109f8f5`.
- Existing sequential ABI test is `sequential_public_calls_keep_single_project_failure_state_isolated` in `tests/g0/test_abi.cpp`; reuse its exact parse-failure expectations, not its two-failure-only setup. `M2M_MO_INPUT_MODE` parsing and `m2m_convert_w` status mapping are in `src/dll/abi.cpp`.

Before implementation, confirm the retained folder comparator's exact comparable-file exclusions and the generated output path convention. The `before` folder includes a sidecar `HEAT1.MST.corpus`; the output folder also contains `.oracleA` metadata and images, so do not compare every file blindly. A file is considered comparable only under the existing comparator's recorded rules; no new normalization or exclusions may be introduced to make this test pass.

## Claim boundary

A passing test would establish candidate exported-ABI status, candidate output bytes/tree for this one retained mode-0 fixture, and same-process success-then-failure isolation. It would not establish a fresh original execution, native GUI/modal behavior, native Close parity, or completion of Forming editor, DOE, Project, or other guarded public paths. Keep the historical 93 SAME / 5 DIFF classification separate.
