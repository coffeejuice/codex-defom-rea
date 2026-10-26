# CP63 test-time attribution and next experiment

CP62’s full test-launcher receipt reports **29.375 s** total for 200 registered CTest cases. The measured phases were:

| Phase | Seconds | Share of launcher |
|---|---:|---:|
| Inventory | 0.891 | 3.0% |
| Fixture SHA before CTest | 6.359 | 21.7% |
| Source and binary binding before CTest | 1.766 | 6.0% |
| CTest | 12.187 | 41.5% |
| Source and binary binding after CTest | 1.735 | 5.9% |
| Fixture SHA after CTest | 6.359 | 21.7% |
| JUnit parsing/report | 0.062 | 0.2% |

The two required fixture passes account for 12.718 s; the two source/binary binding passes account for 3.501 s. Together they add 16.219 s around the 12.187 s CTest run. The runner used threaded fixture SHA with four workers, `parent-cache` path resolution, SHA-256, and no content cache. Its execution binding covered 3,252 source files and 197 unique CTest command/`M2M_CLI` binaries; before/after source and binary records are identical and `stable_during_test` is true. CTest exited 0, but results are `completed-with-recorded-gaps`: 193 passed, 6 passed with logged gaps, and 1 passed with logged differences. Timing does not establish a speed improvement: the CP61 receipt measured 26.031 s launcher time before source/binary hashing was added, while CTest itself was 12.265 s; CP62 CTest is 0.078 s lower, within ordinary run variation.

## Next experiment

Run a bounded three-trial timing comparison of the current serial source-tree fingerprint against a bounded thread-pool fingerprint that hashes the exact same sorted source paths and file bytes. Keep the source snapshot and all 197 binary SHA captures at both CTest boundaries, retain both fixture SHA passes unchanged, and require byte-for-byte identical source fingerprint, file count, and binary manifests before accepting any timing result. Record source hashing separately from binary hashing so the result shows whether concurrency helps this 3.5-second binding cost. Do not overlap binding and fixture hashing in this first experiment; that would change contention and make it harder to attribute the result. Only adopt the concurrent implementation if all three trials preserve exact manifests and show repeatable wall-time reduction without changing the fixture or CTest phases.

Evidence: `analysis/resume62-integrated-full-tests.json`, `build/test-runs/1792877980706167600/timing.json`, CP61 `analysis/test-relocation-results.json`, and its `build/test-runs/1792877344044145300/timing.json`. No scripts, tests, source, build outputs, or GUI state were changed for this audit.
