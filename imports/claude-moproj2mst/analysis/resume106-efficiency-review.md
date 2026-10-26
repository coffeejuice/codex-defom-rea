# CP104–106 efficiency review

CP106 has the shortest measured CTest and launcher times of these three frozen checkpoints, with one additional clean passing check. The receipts do not establish that high-concurrency agent orchestration caused the decrease: source snapshots and build graphs differ, and none of the receipts records agent overlap or critical-path scheduling.

| Checkpoint | Grouped build receipt | Configure | Focused preflight | Full build | Test CTest | Launcher total | Tests registered / clean |
|---|---|---:|---:|---:|---:|---:|---:|
| CP104 | successful full build 55.049 s; later incremental repair 11.950 s | 2.172 s (full) | 5.547 s | 41.344 s | 11.547 s | 28.656 s | 280 / 273 |
| CP105 | initial full build failed at 57.870 s; repair 1 13.626 s; repair 2 incremental 11.659 s | 2.110 s (repair 2) | 2.906 s | 0.797 s (repair 2) | 11.765 s | 29.485 s | 287 / 280 |
| CP106 | successful grouped full build 32.803 s | 2.188 s | 2.516 s | 22.157 s | 10.765 s | 26.954 s | 288 / 281 |

CP105’s failed initial build spent 44.343 s in the full-build phase and exited 4294967295; its later successful repair receipts are incremental, so they are not full-build timing comparisons. CP104 also has two failed repair build attempts (3.500 s and 5.110 s of full-build work) before its successful incremental repairs. These failures are retained but excluded from successful timing claims. CP106’s completed build provides the comparable successful grouped-build receipt in this three-checkpoint window, while the build candidate and changed source tree still prevent attributing its lower elapsed time to a particular setting.

The launcher breakdown is:

| Phase | CP104 | CP105 | CP106 |
|---|---:|---:|---:|
| Inventory | 0.844 s | 0.875 s | 0.875 s |
| Fixture SHA before / after | 6.250 / 6.328 s | 6.563 / 6.531 s | 5.828 / 5.797 s |
| Source/binary binding before / after | 1.796 / 1.766 s | 1.828 / 1.797 s | 1.813 / 1.766 s |
| CTest | 11.547 s | 11.765 s | 10.765 s |
| JUnit/report | 0.094 s | 0.110 s | 0.094 s |
| Launcher total | 28.656 s | 29.485 s | 26.954 s |

Every run hashes the same 16,728 fixture files totaling 1,867,669,827 bytes, using four threaded workers, SHA-256, parent-cache path resolution, and cache disabled. CP106 takes 11.625 s for before/after fixture hashes, versus 12.578 s at CP104 and 13.094 s at CP105. Source tree hashes and complete binary sets are bound before and after each test: respectively 3,407 files / 277 binaries, 3,419 / 284, and 3,421 / 285; each stayed stable. This maintains full fixture coverage and execution binding.

Across CP104→106, CTest fell by 0.782 s (1.07×) and launcher time by 1.702 s (1.06×), far below 10×. Test coverage rose from 273 to 281 clean passes; each checkpoint retains six logged-gap tests, one logged-difference test, zero failures/skips/incomplete output, no fixture changes, no changed diagnostics, historical 93 SAME / 5 DIFF, and `objective_complete: false`. Token data is unavailable.

These receipts cannot measure orchestration critical path: they contain per-command and launcher timings, but no agent start/end intervals, queue waits, or overlap markers. Build and test phases are sequential within each receipt, while parallel worker execution is recorded only for fixture hashing. Thus the observed reductions cannot be assigned to concurrent agents or counted as an orchestration speedup. A safe next measurement is to capture per-agent start/finish and dependency timestamps for one otherwise unchanged checkpoint, then compare the longest dependency chain with summed worker time; keep the same build, tests, manifests, and bindings. Do not alter test configuration or claim an orchestration gain from current evidence.
