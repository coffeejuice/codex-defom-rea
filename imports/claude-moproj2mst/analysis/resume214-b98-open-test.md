# CP214 B98 candidate Open-only test

## Result

Added `source/apps/moproj2mst/tests/s/test_b98_open_only.cpp`, which directly exercises candidate `openProject` on a disposable copy of the retained B98 project. It does not invoke the conversion driver, Save, an original executable, GUI control, or REA. No public guard or candidate behavior was changed.

Focused Release build and CTest run passed:

```text
cmake --build build/candidate --target m2m_s_b98_open_only --config Release: passed
ctest --test-dir build/candidate -R m2m.s.b98_open_only -V: 1/1 passed
fixture=Gear_blank.moproj sha256=e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd files=27 acceptedProject=true openedOk=true treeUnchanged=true
```

The test pins `Gear_blank.moproj` to the known SHA-256, copies all 27 fixture files to `M2M_TEST_SCRATCH`, checks the complete copied file-and-directory manifest against the source before Open, and compares the same manifest after Open. Both `acceptedProject` and `openedOk` are captured independently. The observed candidate Open accepted the suffix, completed its project read, and made no file or directory changes in the project copy. This does not identify the source or timing of the original's observed empty `Lubricants` directory; it supplies only the candidate-side Open boundary result.

## Source and artifact identities

SHA-256 values from the tested checkout:

| Artifact | SHA-256 |
|---|---|
| New test source `source/apps/moproj2mst/tests/s/test_b98_open_only.cpp` | `aad8ff29385374dc9d231d76937e643a39e8793a3e6c3f3fa22e5d8194e125d7` |
| Candidate Open implementation `source/apps/moproj2mst/src/moproj/read.cpp` | `da7bb6b87bd70c771c5b0fa25a9eaec2ef6a0139684d00ce503ce0270bc5e145` |
| Retained fixture `evidence/gui_mo/oracle_b/98_B_DS3F_before/Gear_blank.moproj` | `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd` |
| Built test executable `build/candidate/bin/m2m_s_b98_open_only.exe` | `a142fa9fc5500ac1a17e9166dc98ead8e537bc76b1c3ad9bb281926d8e209aef` |

CMake auto-registers `tests/*/test_*.cpp` through its existing `CONFIGURE_DEPENDS` glob, so no registration-file edit was needed. The target was built in the already configured `build/candidate` Release tree. The archived `wave3-checkpoint211-verified` directory is not a CMake build tree and was not used.
