# CP101–103 efficiency review

CP103’s final full suite completed faster than the two earlier checkpoints, with all 279 registered tests accounted for and source/binary plus fixture bindings retained. The build history includes failed attempts; those are listed separately so their shorter partial runtimes are not mistaken for improvements.

| Checkpoint / attempt | Configure | Focused preflight | Full build | Build-receipt elapsed | Full-suite CTest | Launcher total |
|---|---:|---:|---:|---:|---:|---:|
| CP101 final | 2.313 s | 6.375 s | 46.312 s | 61.327 s | 14.219 s | 35.922 s |
| CP102 first attempt (failed) | 2.156 s | 6.437 s (exit 4294967295) | — | 12.682 s | — | — |
| CP102 repair 1 (completed) | 2.078 s | 2.234 s | 38.891 s | 49.221 s | 13.109 s | 32.859 s |
| CP103 first attempt (failed) | 2.203 s | 5.765 s (exit 2) | — | 12.122 s | — | — |
| CP103 repair 1 (failed) | 2.125 s | 2.703 s (exit 4294967295) | — | 8.964 s | — | — |
| CP103 repair 2 (failed) | 2.156 s | 2.812 s | 13.468 s (exit 2) | 23.888 s | — | — |
| CP103 repair 3 (completed) | 2.172 s | 0.796 s | 28.641 s | 37.679 s | — | — |
| CP103 repair 4 (completed, incremental) | 2.140 s | 0.797 s | 3.328 s | 12.288 s | 12.031 s | 30.907 s |

The CP103 full suite receipt is `build/test-runs/1792904675837235500/receipt.json`; the paired timing receipt reports 12.031 s for CTest and 30.907 s for the launcher from inventory through final report, including source/binary binding and fixture snapshots. CP103 passed 272 tests cleanly, with six logged-gap tests and one logged-difference test; zero tests failed, skipped, or had incomplete output. Source/binary binding was stable and no fixtures changed. The folder sweep covers 98 projects: 49 fresh corpus re-saves (48 identical, one file difference) and 49 oracle cases (45 identical, three view profiles). Its comparator tally reports one corpus file difference and 29 oracle-B file differences; those are file counts, not counts of differing cases. Treat the full folder comparator as diagnostic per its established qualification. Five diagnostic differences occur in `m2m.s.save_folder`; four gap events there are retained. Other logged gaps are in the established MDT, deflate, property, corpus, and save-current cases. These logs are not failures and should remain visible when comparing throughput.

The folder fixture SHA pass used the recorded threaded engine with four workers, SHA-256, parent-cache path resolution, and caching disabled. Each before/after manifest contains 16,728 files totaling 1,867,669,827 bytes. CP103’s measured before and after passes took 7.110 s and 7.140 s, respectively: about 2,353 and 2,343 files/s, or 263 and 262.7 MB/s. CP101 measured about 2,001/2,189 files/s; CP102 about 2,268/2,406 files/s. CP103 is faster in these receipts, but the observations are not a controlled worker-count experiment. The SHA manifests, full source/binary binding, and exact test inventory were preserved; there is no evidence to reduce fidelity checks or change the worker configuration.

From CP101 to CP103, full-suite CTest time decreased 2.188 s (15.4%) while registered tests grew from 270 to 279 and clean passes grew from 263 to 272. CP103’s launcher time still includes 14.25 s of fixture SHA work, about 3.64 s of execution-binding snapshots, and 12.031 s of CTest. This breakdown points to fixture hashing as a substantial measured cost, but the CP101–103 run-to-run measurements do not prove that changing workers would help. Keep four workers and all hashes/tests unless a controlled same-snapshot worker-count comparison demonstrates a repeatable gain.

Token measurement is recorded as unavailable in the test receipts. No token throughput claim is supported. No configuration speedup is claimed: CP103’s `repair4` timing is an incremental build following the repaired full build, not an equivalent cold/full-build comparison. The failed attempts above are retained as failure evidence and excluded from successful build-time comparisons.
