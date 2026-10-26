# CP70: isolated immediate DOE PAR sample helper

## Change

Added `doe::saveSampleTableImmediate(const Study&, int, const String&, StudyIo&)`. It checks the explicit mode is in 0..2 and that the requested table is not invalid, serializes through the same shared byte formatter as the existing `FileEffects` helper, then calls the actual supplied `StudyIo::writeTextFile` immediately. It returns that method's open-success result unchanged. Therefore an ordinary false remains the caller's continuation decision; this helper neither cancels subsequent modes nor inserts itself into public Save.

The existing planned-output `writeSampleTable` now uses the shared serializer, keeping its existing output behavior. The new path does not mutate sample tables, counts, flags, or directory state and does not force a parent directory. The `StudyIo` implementation retains the recovered PAR distinction: false on open failure; after successful open, write/close diagnostics remain observations and do not convert the helper result into an XML-style post-close status.

Added a focused test covering exact successful bytes for a populated mode-0 file and a later missing-parent open failure while preserving the earlier file and recording the failed attempt. A second test covers invalid mode/table skips before any I/O. The test uses `NativeStudyIo` against disposable test scratch only; it does not simulate public Type2 orchestration or DoeTableRefresh.

## Static preflight

- The test's `../support/test_paths.hpp` resolves to the existing `source/apps/moproj2mst/tests/support/test_paths.hpp`.
- The existing target CMake uses a test-directory glob (`tests/*`); no CMake change was made.
- All three owned files have final newlines and no trailing whitespace.
- No compiler, build, CTest, executable, GUI, mutable REA, or public dispatch path was used or changed. Runtime verification is pending root's build/test decision.

## Owned files and SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/doe_study.hpp` | `b423e9bbb821f3ec2011f0e238f9323a35077b1afac02d35a7d02ec29b3ca43f` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `9ca44d9c6bd4e7eacb1e53e13288fab50e1850ab02bd8739542903afc7b2def5` |
| `source/apps/moproj2mst/tests/doe/test_doe_sample_immediate.cpp` | `233352f481d333099e6b806541bca22dec5a0ae217360b7be7be33b4df8eb231` |

The helper is only a single-table effect primitive. It does not close the active Type2 refresh receiver, attach the writer to the public ordered `SaveEffects.project` result, call the current-sample or three-mode sequence, or authorize removal of the existing public guard.
