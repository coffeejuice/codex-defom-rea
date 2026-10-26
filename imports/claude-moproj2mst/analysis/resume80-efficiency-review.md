# Efficiency review after checkpoints 77–79

Checkpoint79's final grouped build receipt (`analysis/resume79-integrated-grouped3-build.json`) records 89.848 seconds: 1.954 configure, 17.532 focused preflight and 64.640 full build, with source stable through each step. The exact full test launcher (`analysis/test-relocation-results.json`) records 15.563 seconds for 238 registered tests and no fixture changes. These are measured durations, not a 10× improvement claim.

The controlled six-scan benchmark in `analysis/resume80-hash-chunk-benchmark.md` hashed the same 16,728 files and produced identical complete manifests. The 1 MiB read buffer had median 6.757 seconds; 4 MiB had median 14.126 seconds. Keep the accepted 1 MiB configuration. Build, source-fingerprint, full-fixture manifest, comparison and ABI gates stay in place.

The next efficiency opportunity is compilation invalidation in the shared engine source graph. Measure it only at a stable checkpoint with identical source, build targets and validation binding. Do not defer an available reconstruction step for a speculative speed change. Workers use independent file ownership; root serializes shared builds and mutable original evidence.
