# CP103–104 measured efficiency review

No measured configuration change approaches 10× improvement. The accepted CP103 and CP104 full-suite CTest runs differ by only 0.484 s (12.031 s to 11.547 s, 1.04×), while retaining the same four-worker SHA-256 fixture hashing and source/binary binding. The receipts do not justify reducing coverage or changing configuration.

| Checkpoint / build | Configure | Focused preflight | Full build | Receipt elapsed | Full-suite CTest | Full launcher |
|---|---:|---:|---:|---:|---:|---:|
| CP103 repair 3 (full success) | 2.172 s | 0.796 s | 28.641 s | 37.679 s | — | — |
| CP103 repair 4 (incremental success) | 2.140 s | 0.797 s | 3.328 s | 12.288 s | 12.031 s | 30.907 s |
| CP104 grouped full build | 2.172 s | 5.547 s | 41.344 s | 55.049 s | — | — |
| CP104 geometry repair 3 (success) | 2.140 s | 0.781 s | 3.219 s | 12.008 s | — | — |
| CP104 geometry repair 4 (incremental success) | 2.156 s | 0.796 s | 3.140 s | 11.950 s | 11.547 s | 28.656 s |

Failed attempts retained separately: CP103 had preflight failures (12.122 s, 8.964 s), then a full-build failure after 13.468 s of build work (23.888 s total) before repair 3 completed. CP104 had a grouped full build completing in 55.049 s; geometry repair attempts 1 and 2 failed in the full-build phase after 3.500 s and 5.110 s respectively, with total receipt times 12.587 s and 13.708 s. Repair 3 and repair 4 completed incrementally. Do not treat failed or incremental receipts as full clean-build comparisons.

CP103’s full-suite receipt reports 279 registered tests: 272 clean passes, six with logged gaps, one with a logged difference, and zero failed, skipped, or incomplete-output tests. CP104 reports 280: 273 clean passes and the same six gaps, one logged difference, and zero failed/skipped/incomplete. Both have stable source/binary binding, no fixture changes, and validation `objective_complete: false`; the historical corpus remains 93 SAME / 5 DIFF, with no changed diagnostics. Each increment added one clean passing check without changing those recorded qualifications. Token measurement is unavailable.

The measured launcher breakdown shows why CTest time alone understates verification cost:

| Phase | CP103 | CP104 |
|---|---:|---:|
| Inventory | 0.875 s | 0.844 s |
| Fixture SHA before / after | 7.110 / 7.140 s | 6.250 / 6.328 s |
| Source/binary binding before / after | 1.828 / 1.813 s | 1.796 / 1.766 s |
| CTest | 12.031 s | 11.547 s |
| Parse/report | 0.110 s | 0.094 s |
| Launcher total | 30.907 s | 28.656 s |

Both passes hash the same 16,728 fixture files totaling 1,867,669,827 bytes, with four threaded workers, SHA-256, parent-cache path resolution, and cache disabled. Fixture hashing costs 14.250 s in CP103 and 12.578 s in CP104; source/binary binding costs another 3.641 s and 3.562 s. CP104’s lower hash times are observed, not a controlled configuration comparison. The reports preserve complete before/after manifests and stable execution bindings. There is no basis to claim that a worker-count change caused the difference.

The next useful experiment is a controlled three-run comparison of the current four-worker fixture hashing against one other worker count, using one frozen CP104 source/binary snapshot and identical fixtures. Keep the full manifests, both source/binary binding passes, all CTest cases, and output checks in every run. Compare median launcher phase times and verify byte-for-byte identical fixture manifests before recommending a setting. Do not change the current configuration based on this two-checkpoint evidence alone.
