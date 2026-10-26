# CP125 folder-sweep timing feasibility and later plan

The admin runner cannot produce accurate per-case folder-sweep times by itself. `m2m.s.save_folder` is one CTest process. The runner measures its aggregate test duration from JUnit and receives the folder rows as test output after the process returns; those rows report only result, case name, file differences, and comparator adjustments. Timing lines added by the runner after CTest returns would measure output parsing or buffering, not case execution, so they would be misleading.

## Read-only dry run

Parsed the retained CP122 `m2m.s.save_folder.log` with the existing admin parser. It returned 98 case rows, two reconciled tallies, and no completeness issues. The case row fields were `case`, `comparator_adjustments`, `differ`, `extra`, `missing`, and `result`; none contain timestamps or elapsed time. Existing CP119–122 receipts record one aggregate `m2m.s.save_folder` duration (about 11.6–11.9 s), not a per-case trace. No runner or test source was changed, and no CTest/build was launched.

## Exact later opt-in C++ timing plan

At a later source-stable checkpoint with explicit ownership of `tests/s/test_save_folder.cpp`, add `M2M_FOLDER_SWEEP_TIMING=1` as a default-off measurement switch. Keep all timings in memory while the existing loop runs; after the loop, write one JSON file under the test's `M2M_TEST_SCRATCH` root, outside the per-case folder directories so the comparator cannot see it. The schema should include a version, source/test run identity if supplied by the runner, case ordinal/name, terminal state (`compared`, `refused`, or `copy_skip`), and durations for:

1. Per-case removal/copy and input validation.
2. The unchanged `m2m::convert` call.
3. The unchanged `compareCase` and its existing cleanup.
4. Total elapsed time from entry to terminal state.

Use `std::chrono::steady_clock`; record durations and case identifiers only. Emit the JSON after all existing case output and summaries have been produced, with no new stdout/stderr lines. Keep the opt-in disabled by default. A later runner-owned change can set the opt-in environment only for an explicitly requested measurement run, validate that JSON covers all 98 executed cases with unique ordinal/order, and attach it to the run receipt. Do not infer timings from CTest's buffered output.

## Required invariants and acceptance check

The instrumentation must not alter discovery order, ordinal allocation, exclusions, copy/remove paths or options, conversion arguments/flags/codepage, comparator logic or normalization, per-case output, tallies, refusal/skip behavior, fixture hashes, or source/binary binding. Timer reads and in-memory records must not participate in test results. The timing file must live outside all compared project folders and must not become part of any input fixture.

At integration, run the same frozen source and fixture set once with timing disabled and once with timing enabled. Compare registered tests and full stdout semantics, case order/names/states/tallies, per-case output bytes, fixture before/after manifests, and source/binary hashes. Require exact equality for every acceptance artifact except the explicitly separate timing JSON and measured durations. Review the resulting per-case data before selecting any optimization; instrumentation itself is not a speedup claim.
