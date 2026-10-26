# CP70 grouped configure-reuse implementation

Added an opt-in mode to `admin/build_candidate.py`:

```text
python admin/build_candidate.py --label cp70 --preflight-target target_a --preflight-target target_b --then-full
```

The mode configures the existing candidate build directory once, builds all supplied targets in one focused Release preflight, then performs the full build using that same build directory and CMake cache. Legacy `--target` remains a focused-only build; it cannot be combined with `--then-full`. `--max-reconfigure` defaults to two retries and is available only in grouped mode.

Before and after configure, focused build, full build, and final artifact hashing, the runner records source fingerprints. It hashes `CMakeCache.txt` at configuration and build-phase boundaries. Source or cache drift triggers reconfiguration and a rerun of the entire focused group before the full build; retries stop at the configured bound. A stable compile/link failure stops immediately for source repair. An exhausted retry or failed step returns nonzero and records `source_stable_during_build: false`.

Successful grouped receipts retain the freeze-consumed `m2m-candidate-build-v1` fields: top-level `targets: null`, source fingerprint/file count before and after, successful command steps, final artifact manifest, and source stability. `preflight_targets` and `configure_reuse` are additive metadata. The full-build `build` step and final artifacts refer to the same source/cache snapshot as the last successful focused preflight. No freeze-checker changes were made.

Focused validation: `python -m unittest discover -s admin/tests -p test_build_candidate_grouped.py -v` passed 10 tests. They use a mocked command runner and disposable temporary directories to cover one-configure success, source drift during configure and focused/full builds, cache drift during focused/full builds, retry exhaustion, focused compile failure, grouped receipt compatibility, and unchanged legacy target behavior. `python -m py_compile admin/build_candidate.py admin/tests/test_build_candidate_grouped.py` passed. No real configure, compile, build, CTest, source, or fixture operation was run.

The continuation checker still reports `CONTINUE`, objective incomplete, 15 remaining items, and no errors. Root owns CP70 integration and should verify the produced receipt against the existing freeze checker before relying on the mode. Hash checkpoints cannot detect an edit-and-restore race between measurements or lock out another process using the shared build directory; stable source/build ownership remains required.
