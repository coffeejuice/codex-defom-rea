# CP142: isolated DOE Output Regions mkdir prefix

## Implementation

Added a private `attemptOutputRegionsMkdirPrefix` helper. It requires immediate-directory mode and an exact active Type2 chain (`Project` → active `SubProject` → owned `Study`, including `sub.doe` and `study.sub` identity). It derives `<Study Control>/Regions` through the existing `mst::doeControlDir`, requires an absolute path for this candidate adapter, calls `qt::nativeMkpathAbsolute` immediately, and records exactly one `FileEffects::directoryAttempts` entry.

The result distinguishes `AttemptSucceeded` from `WarningModalRequired`. A failed attempt is retained and returned as the native one-OK modal boundary; the helper does not display the dialog or continue into cleanup. It is not called by public DoeOutput Save, does not inspect a viewer model, and does not enable the current-operation guard.

## Focused test and preflight

The new `m2m_doe_output_regions_mkpath_prefix` test covers:

- Successful immediate creation for a real Type2 Project/SubProject/Study owner, exact path, one attempt receipt, and no unrelated planned writes/deletes.
- A file blocking the `Regions` leaf: retained failed attempt, `WarningModalRequired`, earlier directory components preserved, and no later effects. The QMessageBox is deliberately not driven.
- Foreign Study/SubProject, Type3, and non-immediate directory profile refusal before any new filesystem attempt, preserving a seeded planned effect.
- An empty Project path yielding a relative Control/Regions candidate is refused before the native directory call.
- Null Study backlink, null SubProject DOE backlink, and empty Study name each refuse before any directory attempt.

All physical test paths use `M2M_TEST_SCRATCH`. `python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_output_regions_mkpath_prefix.cpp` passed. The test has a Testkit `main`, supported assertions, resolvable private helper include, no C `assert`, and is covered by CMake's existing `tests/*/test_*.cpp` `CONFIGURE_DEPENDS` glob. Direct trailing-whitespace inspection found none. No build or test executable/CTest was run; root owns validation.

## Frozen write set

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/output_regions_mkpath_prefix.hpp` | `C4E28E59C83BC59EF95E6C8EC102F09615EEE7486C1CAC3E1F2DF523EBB54DAE` |
| `source/apps/moproj2mst/src/doe/output_regions_mkpath_prefix.cpp` | `B468B1BFFD8294C4595AF7096DEA09B2050A838BE8E2CCD079EE49350AD4C582` |
| `source/apps/moproj2mst/tests/doe/test_output_regions_mkpath_prefix.cpp` | `ACC0AD8C29B3B23C0C49824EF808C43D3796AA908DF6AE2545BA7C82082F1B75` |

The explicit absolute-path check is an adapter precondition, not a claim about native QDir's relative-path behavior. The relative-path test reaches it through an empty Project path: `qt::absolutePath` returns empty for an empty filename, and `doeControlDir` then constructs a rooted-but-not-absolute path. This is private first-effect coverage only: no native warning-modal delivery, subsequent model-dependent cleanup, Common_Mesh deletion, geometry, `.DS` update, or public Save continuation is claimed.
