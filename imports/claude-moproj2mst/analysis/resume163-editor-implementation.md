# CP163 editor allocation-only implementation

Implemented the bounded post-query A producer in `source/apps/moproj2mst/src/engine/simcontrol300.cpp`. The existing ownership, reader, receipt, directory-index and record-version gates remain in force. After the real `step_dir_sizes` query, only dimension 3 / `u1b58 == 0` / post-query version at least 6000 / positive actual headers / cache11 zero / fresh A / safe nonnegative count proceeds. It calls the existing `bc::calloc_doubles`, publishes its normal return directly to `u1b78`, and then returns `CoordinateSync` immediately. The selected length, cache1, arrays timer, DB body read and Stop remain untouched.

Extended `test_simcontrol300_stopping_cached.cpp` with fresh allocations for two, three and six doubles, including zeroed contents; directory versions 6000, 11000 and `INT32_MAX`; zero and positive selected lengths; fresh-page retry pointer retention; and missing, zero, negative, preexisting-A, negative-count and overflow guards. Existing generic query refusal cases now use zero header inputs so they stay on the pre-producer side of the new gate.

## Static preflight and verification

- Both additions remain in the Testkit executable, whose entry point calls `testkit::run_all()`.
- Assertions use Testkit `CHECK`, `CHECK_EQ` and `REQUIRE`; no `REQUIRE_EQ` or C `assert` was added.
- `git diff --check` passed for both owned source files.
- No build, test execution, GUI or live REA interaction was performed, as directed.
- `tools/gui_validation_status.py` reports GUI input blocked because a supported release operation and verifiable status are unavailable. No GUI-control session was acquired.

## File hashes

SHA-256 for `source/apps/moproj2mst/src/engine/simcontrol300.cpp`:
`67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`

SHA-256 for `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp`:
`31dc5b593077a7bace4a22255d58cf722481b9f90542d38f2f07e82b27324373`

After the static source review, corrected the focused tests to check the two
expected node-timer checkpoints rather than whole-timer identity, while keeping
the arrays timer byte-identical. The fresh-A test now uses `REQUIRE` before
buffer reads, traces the single dispatch before the directory query's version
effect, and checks unchanged headers, body metadata and object coordinates.
`git diff --check` passed after these repairs. They have not been built or run.

The subsequent grouped compile preflight identified that `u1b78` is exposed as
`void*` in the layout. Both zero-content loops now read through a `const double*`
cast made immediately after the existing nonnull requirement. The test file's
final hash above includes this correction; `git diff --check` passes, and no
additional build or test run was performed in this lane.

## Ownership handoff

The implementation lane releases exclusive ownership of `simcontrol300.cpp` and `test_simcontrol300_stopping_cached.cpp` to the root integration owner. Root should run the focused target and required DB/query support regressions, then source-stable suite and freeze according to the converter takeover instructions. No claim of strict original allocator failure parity or reader completion is made.
