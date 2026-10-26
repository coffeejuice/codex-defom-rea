# Checkpoint 59: read-only registration and test audit

## Registration and validation state

The post-checkpoint58 CMake configure discovers both new targets:

- `m2m.mdt.simctrl_ppv_file_reader` → `tests/mdt/test_simctrl_ppv_file_reader.cpp`
- `m2m.mdt.ppv_root_only_save_guard` → `tests/mdt/test_ppv_root_only_save_guard.cpp`

The earlier false-positive `m2m.support.paths` is now excluded by `CMakeLists.txt:196-197`; the support helper remains importable by the Python generator scripts. Read-only `ctest -N` lists 186 tests with label `m2m` (205 total tests) and includes both new targets, but no CTest run for checkpoint58 is present. The latest `build/test-runs` directory remains checkpoint57 (`1792874039557650200`). The recorded checkpoint58 Release build stopped at `m2m_p08_emit_a.exe` with LNK2019 unresolved `m2m::SubProject::~SubProject()` referenced from `project_owned_storage.cpp.obj` (LNK1120). The two new PPV targets were linked earlier in that build log, but this does not establish that they ran or that the complete suite builds.

## New-test effectiveness

Both new C++ tests use `testkit::TEST_CASE` and Release-active `REQUIRE` / `CHECK` macros; neither uses C `assert`.

- `test_simctrl_ppv_file_reader.cpp` defines three cases for ASCII LF/missing, UTF-16LE+CRLF, and guarded unsupported encodings. Its content paths are injected through `Project::overlay`.
- `test_ppv_root_only_save_guard.cpp` defines one case. It loads a read-only Oracle-B project, makes local recovery empty and shared recovery nonempty in memory, invokes `saveCurrentOperation`, and asserts completion plus `SimCtrl.KEY` effect and no `SimCtrl.PPV` effect. These `FileEffects` are inspected in memory; the test does not apply them to the corpus. This is a candidate characterization of the local-empty Save gate, not full public PPV writer or original parity.

The new CMake registration is active, and I found no compiled-out assertions or permissive result check in these two files. However, there is no checkpoint58 test log to establish actual case execution due to the recorded build stop.

## Concrete fixture-isolation defect

In `test_simctrl_ppv_file_reader.cpp:24-26`, the missing-file case requests the hard-coded absolute path `C:/scratch/missing.PPV`. `readPpvFileLines` calls `datafiles::readFileBytes`; after a `Project::overlay` miss, `src/datafiles/textfile.cpp:12-23` falls through to `std::ifstream(win32Path(path))`. Therefore the test probes `C:\scratch\missing.PPV` outside `M2M_TEST_SCRATCH` and outside the fixture overlay. The path is absent on this host, but if another file exists there the test can unexpectedly read it and cease to exercise `OpenFailed`. This is read-only, but it is a concrete non-hermetic path escape/nondeterminism. Use a missing absolute path beneath the configured test scratch root, or inject the byte-source seam, so no unrelated filesystem path is consulted.

## Source and fixture identities

Read-only source hashes:

- `test_simctrl_ppv_file_reader.cpp`: `d5fc3729194111057bdbc8a28291c44f0af1540bb9ab00dbb674195a853b3767`
- `test_ppv_root_only_save_guard.cpp`: `0974c242f43ca5751232ec0e04cc864c4f890655d77318b7291aa75e2bb3d5e8`

The root-only guard test reads `evidence/gui_mo/oracle_b/93_B_MB3ACT_before/Gear_blank.moproj`, SHA-256 `0e3bbd030ae3eda05898a6138a223ff560bd6611631ae276a69ac87ca9694b78`. No CP58 before/after fixture snapshot or test-run receipt exists, so fixture non-mutation for this candidate attempt is not independently established by a run receipt.

No source, CMake, build, GUI, or REA mutation was performed for this audit.
