# CP126: distinguish Boolean test reruns after PID reuse

The Boolean postLoad lifecycle test now includes a process-run steady-clock tick in its disposable leaf: `b102_<pid>_<runTick>_<sequence>`. A static runTick is sampled once for the fixture constructor's process run; the existing sequence distinguishes its two fixtures. PID reuse alone no longer reconstructs the prior run's directory name. The existing REQUIRE(!exists(root)) remains, so an unexpected collision still fails visibly rather than reusing or deleting prior artifacts.

Exclusive source changed: source/apps/moproj2mst/tests/mdt/test_boolean_postload_creation.cpp. The patch adds the standard <chrono> include, one run-tick initialization and the leaf-name component/comment. Fixture setup, zero/nonzero OPI-slot assertions, Open/lifecycle calls and Boolean timing guards are unchanged. No existing artifacts were removed or modified.

Root corrected its initial C:\PROBLEM wording for this converter CTest and confirmed preservation of M2M_TEST_SCRATCH. The verified candidate cache sets the base to C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/build/candidate/t; CMake hashes test name m2m.mdt.boolean_postload_creation to106e3096. The test retains that configured per-test root and scratchOutput containment. A conservative positive64-bit tick/maxPID/maxsequence example plus test.moproj is152 characters under this checkout (leaf46). No under100-character or C:\PROBLEM claim is made; no CMake/environment redirection occurred.

Static preflight passed: all eight quoted includes resolve through the source directory or target include tree; the new include is a standard C++ header; Testkit main remains testkit::run_all; checks use supported CHECK/CHECK_EQ/REQUIRE with no C assert. The clock count has an integer representation supported by std::to_string on the Windows/MSVC target. No compilation, executable/test launch, full suite, original process or GUI action was run by this worker. Root owns rebuild and revalidation.

Source is frozen for grouped validation. SHA256 **9b46c9963dab43c0a465d43ab8426300bf409f1c791e1f688a7a7de54cddcd04**. Only the exclusive test and this receipt were written.
