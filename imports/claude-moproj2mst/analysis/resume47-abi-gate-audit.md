# Resume 47 ABI and lifecycle-gate audit

## Finding

The C ABI has meaningful focused coverage for argument validation, message buffers, configuration setters, Open failures and selected adapter-owned state. There is **not enough evidence to mark the objective-wide `abi_and_regression_verified` or `failure_and_close_verified` gates complete**. The current candidate46 validation ran the full registered suite (150 tests, 0 failures), but explicitly records an ABI qualification that the real-project `g0.abi` assertion accepts either `M2M_OK` or `M2M_E_UNSUPPORTED`. It also records six gap-passing tests, one test with logged differences, and `objective_complete: false`.

## Public entrypoints and their bounded evidence

The ABI is version 1 in `source/apps/moproj2mst/include/moproj2mst.h`; implementations are in `source/apps/moproj2mst/src/dll/abi.cpp`.

| Entry point / contract | Existing coverage | Acceptance implication |
|---|---|---|
| `m2m_api_version()` | `tests/g0/test_abi.cpp::version` | Confirms exported version constant, not binary compatibility across client/toolchain combinations. |
| `m2m_set_codepage()` | `test_abi.cpp::codepage`; `test_abi_original_settings.cpp::adapter_settings_parse_each_call` | Valid and invalid values plus call-time use are covered. Global-setting concurrency and parity of all supported code pages are broader gates. |
| `m2m_set_gui_unit()` | `test_abi.cpp::gui_unit` | Range validation is covered; conversion behavior under both unit settings is not established by this test. |
| `m2m_set_deform_dir_w()` | No direct test located in `tests/g0`; Cogging/MDT tests exercise writer behavior and pack/version inputs elsewhere | Setter null/empty/path behavior and subsequent ABI-call snapshot use are not directly established by the located ABI tests. This is a candidate test gap, not proof of a defect. |
| `m2m_convert_w()` | `test_abi.cpp` covers null/empty paths, missing input, unknown flags, NUL-preserving truncation, and a real-project result; `test_abi_open_failure.cpp` covers Open/Save failure status split and current-directory restoration | Direct wide-path status and buffer behavior have good bounded coverage. The permissive real-project `OK || UNSUPPORTED` assertion does not establish successful end-to-end ABI conversion. |
| `m2m_convert_utf8()` | Invalid UTF-8, missing-file message, truncation, Open-failure/save rejection on a rejected suffix | These check conversion of arguments and error reporting. A successful non-ASCII UTF-8 input/output path conversion is not established by these tests. |

The public lifecycle flags are `M2M_FLAG_STRICT` (warnings become `M2M_E_INPUT`), `M2M_FLAG_WRITE_MDT` (Cogging side files), and `M2M_FLAG_WRITE_SAVE` (full Save effects). `M2M_FLAG_DOE_TEMP` is explicitly unsupported. Unknown bits return `M2M_E_ARG`. Existing focused coverage exercises many flag-specific writers and paths in `tests/s`, `tests/mdt`, and the folder comparator, but those do not replace a successful independent end-to-end ABI lifecycle run. In particular, the full-save header itself qualifies the ABI status mapping as adapter behavior and states that earlier Open effects, remaining property writers, and filesystem/encoding edges still need recovery.

## Status mapping: observed checks versus remaining uncertainty

There are explicit assertions for `M2M_E_ARG` (invalid paths/flags/settings), `M2M_E_OPEN` (legacy preflight failures), `M2M_E_PARSE` (failed `openedOk` at Save), and `M2M_E_UNSUPPORTED` (an unrecovered Open-tail failure). `test_abi_open_failure.cpp` also verifies that these outcomes retain or avoid particular file effects and that adapter current-directory restoration occurs for its exercised cases.

That is not a complete validation of status mapping. The header distinguishes the legacy `E_OPEN` preflight from `WRITE_SAVE`'s post-Open `E_PARSE`; tests cover representative cases, not every source-derived failure site. `abi.cpp` maps caught `m2m::Error` to its code and any other exception to `M2M_E_INPUT`; no located ABI test deliberately exercises the catch-all mapping. `M2M_E_WRITE`, `M2M_E_NO_OPERATIONS`, strict-mode warning promotion, and successful `M2M_OK` through the exported DLL entrypoints need to be traceable to focused assertions or independent accepted cases before claiming complete status coverage. A unit-level test of internal conversion logic is useful evidence, but should not be mislabeled ABI-boundary coverage.

## Repeated-call cleanup and scope correction

Existing coverage should be credited before describing this as wholly untested:

- `tests/mdt/test_conversion_session.cpp::fresh_session_does_not_reuse_previous_session_notification_state` checks fresh `ConversionSession` state after a preceding session.
- The same file contains many ownership, release, exception-cleanup, and teardown tests for objects inside one session.
- `tests/g0/test_abi_original_settings.cpp::adapter_settings_parse_each_call` changes `M2M_MO_INPUT_MODE` across consecutive calls and verifies each call rereads it.
- `tests/g0/test_abi_open_failure.cpp` makes two consecutive calls with different flags against a missing project, checks distinct statuses and side effects, and checks current-directory restoration.
- `analysis/closure-audit.md` already qualifies the stage boundary: compare independent single-project calls from fresh original-equivalent state. Historical cross-project A,B,A accumulated-state tests are outside the current scope.

These tests do not yet prove repeated **successful public ABI** conversions clean all process-wide state. The session unit tests construct/use internal session objects and generally inspect a single session or its teardown; the settings test exits during input preflight; the Open-failure pair does not reach a completed Save/Close cycle. The open-failure tests prove only adapter current-directory restoration, not original Close behavior. No located test in `tests/g0` invokes two successful `m2m_convert_w`/`m2m_convert_utf8` conversions on distinct copied projects and compares each result and side effects to its fresh single-project expectation. Treat that as a concrete validation obligation under one-project-per-fresh-session semantics, not a request for historical cross-project parity.

## Reconciliation with current acceptance gate

`analysis/takeover-state.json` keeps `abi_and_regression_verified` and `failure_and_close_verified` false with empty evidence. This matches the receipts. `analysis/wave3-checkpoint46-validation.json` reports all 150 test processes completed with zero failing assertions, but calls out the permissive `g0.abi` real-project assertion and labels its scope “no full lifecycle completion.” The receipt therefore supports “the registered candidate regression passed with qualifications,” not “the public ABI and full lifecycle are independently verified.”

The all-gates checker currently reports 15 remaining work items and all eight completion gates false. There is no contradictory green objective-level status in the inspected acceptance state.

## Next executable gate work

For ABI-specific closure, first trace each public entrypoint/flag/status contract to existing tests and classify any tests that invoke only internal functions. Then add ABI-boundary focused cases for successful wide and UTF-8 conversion, strict warning/status behavior, representative write/no-operation statuses, and two sequential successful calls on distinct copied projects with output/effect checks. Compare each call against its own fresh-session expectation; do not use A,B,A history as the oracle. Exercise cleanup after both success and a controlled failure, and retain the existing failure-order tests.

Before those tests can be treated as objective acceptance, candidate47 changes must be integrated and frozen; the existing checkpoint46 suite is tied to its own source/build receipt. Run the required integrated regression against that frozen candidate, then satisfy the separate original-first comparison, failure/Close, and reproducible-delivery gates. Do not loosen the permissive corpus assertion and call that parity; replace or supplement it with a source-qualified successful-path acceptance case.

## Evidence inspected

- `source/apps/moproj2mst/include/moproj2mst.h`, `source/apps/moproj2mst/src/dll/abi.cpp`.
- `source/apps/moproj2mst/tests/g0/test_abi.cpp`, `test_abi_open_failure.cpp`, `test_abi_original_settings.cpp`.
- Relevant repeated-session and teardown cases in `source/apps/moproj2mst/tests/mdt/test_conversion_session.cpp`.
- `analysis/closure-audit.md`, `analysis/wave3-checkpoint46-validation.json`, `analysis/resume46-combined-full-tests.json`, `analysis/takeover-state.json`.
- Read-only `python admin/check_continuation.py`: `CONTINUE`, 15 remaining items, eight objective gates pending.
