# CP105 Cogging failure test independent static review

## Blocking compile defect

`source/apps/moproj2mst/tests/s/test_save_cogging_open_failure.cpp:112-113` declares `OutputFactory(Observation&, const fs::path&)`, but line 145 constructs it with only `factory(observation)`. No default argument or one-argument overload exists in the test. The target cannot compile until the call supplies the project path or the unused constructor parameter is removed. This is a source-level finding; no build was run.

## Denial timing and data checks

The intended timing is sound by inspection: the private project output factory opens the candidate project writer; `mst::FileDevice::close()` flushes and emits `aboutToClose` while its handle is still open; the test callback then opens the existing Cogging file using `CreateFileW(GENERIC_READ, FILE_SHARE_READ)`. The handle is retained through conversion return. Candidate `CoggingFile` uses `CreateFileA(GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS)` (`src/datafiles/cogging_file.cpp`), so its later write-open is incompatible with the earlier handle and must fail on Windows. The fixture's Cogging.MDT exists and is nonempty. The comparison against saved original bytes catches truncation or replacement.

The source checks that the lock was acquired only by `REQUIRE` in the handle constructor; it does not independently record the failed Cogging CreateFile call. Given valid loaded Cogging model data this is a strong deterministic interference setup, but the final unchanged-byte assertion alone would not distinguish an attempted denied open from a path where the writer was skipped. The candidate-source chain at `src/core/convert.cpp` line ~379 calls `saveCoggingMdt` in operation order; `src/datafiles/mdt_write.cpp` skips only when props/model/owner/path are absent, and otherwise constructs `CoggingFile` before writing. The reviewed artifact includes the Cogging.MDT file and Task00003 operation. No native/original runtime was executed.

The test's pre/post observations are appropriately separated: prior Cogging bytes are captured before conversion; BWZ/INI are required absent before conversion and checked for expected markers afterward; `native-output.MST` is required absent before and checked for its end marker afterward. The fixture order is Boolean operation #2 then Cogging #3, so the Boolean files prove earlier Save effects only. The MST end marker is the output that supports continuation after the Cogging property callback, because the main MST write follows the operation-property loop. The current comment at lines 159-160 correctly calls BWZ/INI already-written earlier effects. `M2M_OK` describes the candidate adapter only.

## Paths and cleanup

The test derives one unique leaf from `M2M_TEST_SCRATCH`, process ID, tick count, and an in-process serial. `scratchOutput` resolves ancestors and rejects outputs escaping the canonical scratch root. It checks absolute UTF-16 native path lengths for project, Cogging.MDT, BWZ, INI, and MST; each asserted output is below 100 code units. The fixture copy goes to the unique leaf, and the destructor calls `remove_all` only on that leaf. Cleanup is best-effort: the error code is discarded, so deletion success is not verified. Also, if construction throws after copying but before `Rig` construction completes (for example, a path-length REQUIRE), the `Rig` destructor will not run and the unique leaf may remain. These are containment/cleanup limitations, not evidence deletion escapes the owned leaf.

## Disposition

Fix the constructor-call mismatch before accepting or building this test. Once fixed, the post-failure continuation witness is MST, not Boolean BWZ/INI. No edits, build, GUI, original execution or mutable REA action occurred.

Reviewed test SHA-256: 99CAD09D94B8A9A9791FF613D7B5F6379F68CEA4C9CCE6292497F0FA5901F25C
## Superseding verification

Root reports that the test owner corrected the constructor after the reviewed snapshot. I verified the current source: `OutputFactory` now has `explicit OutputFactory(Observation&)`, matching `factory(observation)` at the call site. Its current SHA-256 is `99CAD09D94B8A9A9791FF613D7B5F6379F68CEA4C9CCE6292497F0FA5901F25C`. The compile-blocker finding above is therefore superseded for this current hash; no build was run.
