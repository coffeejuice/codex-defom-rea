# First implementation checkpoint: efficiency review

The useful workflow change is to retain complete test output on disk and return compact receipts. The initial 94-test run truncated six passing JUnit logs at 1,024 bytes, concealing the full Save sweep results. The subsequent 95-test run retained all outputs with zero truncation/completeness errors and reconciled all 98 diagnostic sweep cases with their summaries. This improves evidence completeness; it is not a measured token or execution-time saving.

Three workers handled the closure audit, test relocation and reference harness with separate file ownership. The coordinator alone changed behavior and ran builds/tests. Independent review found two missing registry initializers before final integration. The original source/evidence stores and initial converter baseline were preserved.

The relocated baseline build took 99.828 seconds after 2.844 seconds of configuration. An incremental connector build took 5.766 seconds after 0.938 seconds of configuration; these are different workloads and cannot establish speedup. The first full tests took 20.484 seconds with four jobs; the later corrected-source run took 20.907 seconds with eight jobs. Their tests and instrumentation differ, so no parallelism benefit is claimed. Fixture hashing is additional elapsed work outside those CTest timings.

The bounded metadata audit in `wave1-efficiency-metadata.json` covers the last completed model-advice turn, not this still-active implementation turn. Its token totals must not be attributed to recovery. Current implementation token usage and a fair end-to-end speed comparison remain unavailable.

One compiler retry corrected a namespace/type spelling. Subsequent test reruns were justified by Windows absent-path and relative-path normalization defects in the inherited/relocated test infrastructure. Keep failed-run receipts and the source-bound original connector evidence; do not repeat passing focused checks without a new change or unresolved concern. Next review is due after three additional recovery checkpoints or one hour at a safe boundary.
