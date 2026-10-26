# CP59 preflight test audit

## Scope and result

Read-only review of the recently added/updated `moproj2mst` tests, Testkit macros, CMake discovery, filesystem paths, and fixture handling. No build, test execution, GUI control, or mutable REA interaction was performed. No source or CMake files were changed.

**No current blocking defect found.** The earlier observation that `m2m.mdt.simctrl_ppv_open_sequence` was absent from `ctest -N` was a stale generated-test snapshot: after the concurrent configure refresh, the current listing includes it as test #97. CMake's `CONFIGURE_DEPENDS` glob registers `tests/<packet>/test_*.cpp`, and the current generated tree lists the audited additions.

## Test effectiveness

Testkit is available through `source/port/testkit/include/testkit/check.hpp`. Its `CHECK`/`REQUIRE` macros record failures in the active runner and are not C `assert` statements, so Release/NDEBUG does not compile them out. The audited C++ files include Testkit and call `testkit::run_all()` in `main`, except `tests/p11/test_xcheck.cpp`, which intentionally uses a custom comparator runner. That runner compares `cases.txt` and returns failure for either a mismatch or zero cases (`bad == 0 && n > 0` is required for success), so it does not silently pass on an empty corpus.

There are 207 registered `TEST_CASE` declarations across the 18 recently touched Testkit C++ files audited here. `test_xcheck.cpp` contributes its custom nonzero-case comparator instead of Testkit cases. The new PPV open-sequence test has four active cases; the file-reader test has three. Current `ctest -N` lists both, as well as the other recent additions including conversion session, PPV binding/save/read/write, editor setup, paint, close, DOE receiver/backend and active DOE output tests.

## Filesystem and fixture safety

- `m2m_test_environment()` gives each CTest entry a unique working directory beneath the configured `M2M_TEST_SCRATCH`, sets `M2M_TEST_SCRATCH`, and redirects `TEMP`, `TMP`, and `TMPDIR` there. The root is checked by CMake to be a strict child of the takeover build directory.
- `test_simctrl_ppv_open_sequence.cpp`'s `OpenFailed` case now chooses `scratchRoot()/ppv-open-missing/SimCtrl.PPV` and checks that it does not exist before calling the reader. Its failed disk fallback is therefore confined to the assigned test scratch tree. Other `C:/scratch/...` PPV reader/open cases supply matching in-memory `Project::overlay` entries; the raw-looking path is not read from disk.
- Tests that materialize project trees use `scratchOutput`/`scratchRoot`; the DOE active-output guard copies the retained oracle input to scratch and compares source and copied manifests, then checks the source manifest again afterward. Its expected `FileEffects` are inspected rather than applied.
- The PPV root-only save guard reads its retained `.moproj`, changes only in-memory recovery lists, and inspects recorded write effects; it does not apply effects or mutate the fixture. The B98 path test is a candidate-only model/path regression and does not modify the fixture.
- `C:/PROBLEM/...` paths in the close/backend/receiver characterization tests are model inputs only; those tests do not create output at those paths.

## Discovery and execution status

Current `ctest --test-dir build/candidate -N` shows the audited test registrations, including `m2m.mdt.simctrl_ppv_open_sequence` (#97), `m2m.mdt.simctrl_ppv_file_reader` (#96), `m2m.mdt.doeoutput_active_guard` (#51), and `m2m.p11.xcheck` (#148). The Python helper directory is excluded from automatic `test_*.py` discovery, avoiding an import-only zero-case test entry.

This is a preflight/source-registration audit only. It does not establish that the current CP59 tests compiled or passed; no current CP59 test-run receipt was present/created during this audit.
