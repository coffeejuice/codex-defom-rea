# CP64 efficiency review: CP62 versus CP63

## Measured checkpoint costs

CP62’s successful full build receipt totals 35.057 s (10.765 s configure, 21.579 s build). Its 200-test launcher totals 29.375 s, including 12.187 s CTest, 12.718 s for the unchanged before/after fixture SHA passes, and 3.501 s for source/binary binding.

CP63’s successful full build is `resume63-integrated-full3-build.json`: 13.669 s (10.844 s configure, 0.140 s incremental build). The CP63 integrated build/test cycle also records five build invocations: the initial full attempt (24.343 s, failed at compile), a repair attempt (15.444 s, failed at compile), a second repair full build (32.848 s, passed), a focused PPV test-fix build (15.526 s, passed), and the final integrated full build (13.669 s, passed). These receipts sum to **101.830 s** of build-command elapsed time, 66.773 s above CP62’s single successful build receipt. This is the sum of recorded command elapsed times, not an asserted end-to-end wall interval.

The CP63 source fingerprints differ between each recorded full build: `80779f13…0903`, `dc96a136…dfe8`, `5eaea22a…c249`, and `53187aae…bde8`. Each build was stable during its own configure/build window, but the successive builds did not all represent one unchanged candidate. The first two logs both fail in `tests/mdt/test_simctrl_ppv_text_writer.cpp` / its `CHECK_EQ` diagnostic path with MSVC C2679. The later focused PPV build passed on the final source fingerprint; the final full build then passed. This is direct evidence that the failed attempts were avoidable compile-feedback work before the final integrated build, while the changing fingerprints show a shared mutable-source coordination boundary.

CP63’s final 204-test launcher totals 31.047 s, 1.672 s above CP62. CTest is 12.438 s versus 12.187 s (+0.251 s). Fixture hashing is 13.671 s versus 12.718 s (+0.953 s), and source/binary binding is 3.906 s versus 3.501 s (+0.405 s). Four additional tests were registered. The test receipts report 0 CTest failures and retain recorded gaps/differences, so the efficiency comparison does not imply broader acceptance.

## One concrete improvement

For new or changed C++ test files, give one worker exclusive ownership through compilation and require that worker to compile its exact CMake test target in Release mode before the patch enters the full-build queue. Keep that file stable from target preflight through the selected integration build. If the focused target fails, repair the compile diagnostics and rerun only that target until it passes; then use the full build and unchanged 200+ test launcher for integration.

This is supported by CP63’s evidence: both early failures stopped at a compile error in the PPV text-writer test path, while the later focused PPV target receipt completed in 2.000 s of build time (15.526 s including its configure) and the final integrated build passed with only 0.140 s incremental build work. A preflight against the same configured build tree could avoid repeating full-tree compile work and reduce time spent building unstable snapshots. The observed receipts do not isolate the exact counterfactual saving, so treat savings as likely larger than the approximately 0.92 s estimated benefit from concurrent source hashing, not as a measured guarantee. Do not skip the full build or CTest suite after preflight.

This recommendation improves source ownership and compile feedback only. It preserves fixture coverage, both source/binary binding boundaries, the folder sweep, all registered CTest cases, and root’s final stable-source integration build. Token usage is unavailable in the inspected build/test receipts.

Evidence: `analysis/resume62-integrated-full-build.json`, `analysis/resume62-integrated-full-tests.json`, `build/test-runs/1792877980706167600/timing.json`, `analysis/resume63-integrated-full-build.json`, `analysis/resume63-integrated-full-repair-build.json`, `analysis/resume63-integrated-full-repair2-build.json`, `analysis/resume63-ppv-test-fix-build.json`, `analysis/resume63-integrated-full3-build.json`, and `analysis/resume63-integrated-full3-tests.json`.
