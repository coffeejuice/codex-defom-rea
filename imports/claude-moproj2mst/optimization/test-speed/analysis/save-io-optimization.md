# Save I/O test speed analysis

## Profile evidence

`profile-v1-ctest.log` records 72 fixture-copy phases with a mean of 38.3 ms (19.4–77.7 ms), and 33 converter phases with a mean of 59.3 ms (9.1–90.8 ms). The common `133_B_DBSTEP_BOOL3_before` fixture contains 64 files totaling about 10.6 MB. For the slower `m2m.s.save_io` cases, copying the disposable tree commonly costs 40–52 ms before conversion, so fixture setup is a meaningful share of wall time.

## Optimization decision

No source change is made. `Rig` currently copies the complete fixture recursively into a unique scratch directory because test cases mutate project and sidecar files and assert those effects. The profile identifies the cost, but it does not identify which fixture files are safe to omit. Narrowing the copied tree without a per-test dependency inventory could silently change coverage or test setup. Sharing one writable fixture would also violate isolation. Replacing `std::filesystem::copy` with a custom copier has no evidence of being faster and would add platform-specific code.

The next safe optimization is an evidence-backed fixture dependency map for each scenario, followed by a candidate selective-copy implementation and output/coverage comparison. No build, test suite, or fixture mutation was performed for this analysis.

## Detailed copy profiling

`test_save_io.cpp` now supports `M2M_SAVE_IO_COPY_DETAIL=1`. After the normal recursive fixture copy, it makes a disposable diagnostic mirror by copying each regular file separately and emits one `[save-io-copy]` record per file with relative path, byte count, and copy duration. The mirror is removed after profiling. This intentionally leaves the production test setup on the existing `std::filesystem::copy(..., recursive)` path, because the available evidence does not establish that a hand-rolled traversal preserves all recursive-copy metadata and link semantics or improves speed.

Focused measurement plan for the root integrator: build the `m2m.s.save_io` target once; compare at least five normal runs with five detail-enabled runs using identical test selection and scratch volume; retain the existing phase totals and aggregate per-file bytes/time by fixture filename across scenarios. Use the detailed pass only to identify candidates; validate any copy implementation separately by checking file inventory, bytes, permissions, timestamps, symlink status, and converter outputs before claiming equivalence. The diagnostic pass intentionally adds a second copy and its timings are attribution data, not the test's end-to-end setup latency.
