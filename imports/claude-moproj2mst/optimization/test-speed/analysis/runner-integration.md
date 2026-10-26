# Exact fixture-hash runner integration

The launcher can now select the bounded threaded SHA-256 helper without changing its default execution. `admin/run_converter_tests.py` accepts `--hash-engine serial|threaded` and `--hash-workers 1..32`. The default engine remains the original serial `snapshot(roots)` implementation. The threaded engine defaults to four workers; serial always uses one worker and ignores the worker option. Argument errors occur before creating a run directory.

Example command for a separately authorized paired full-run measurement:

```powershell
python admin/run_converter_tests.py --build build/baseline --hash-engine threaded --hash-workers 4
```

Both fixture passes use the same selected engine. Each result still contains the complete relative-path mapping to `[size, mtime_ns, SHA256]`, with legacy insertion order. No cache, timestamp shortcut, sampled content, pruned nested roots or reduced fixture set is introduced. The original serial callable and the helper were left unchanged to allow independent benchmarking. The helper is imported only when the threaded engine is selected.

The existing receipt gains `fixture_hash` with `engine`, effective `workers`, `algorithm: sha256` and `cache: false`. Existing fixture snapshot filenames, per-run/root receipts, gap and refusal classification, diagnostic rows, output completeness checks, CTest command and return-code decisions are retained. A failed CTest return still reaches the after snapshot; a changed fixture still makes an otherwise successful run fail. As before, an exception that prevents reaching the after snapshot aborts the launcher; it does not invent a complete after receipt.

The previously added phase timers use monotonic wall time, preserve a phase duration when its body raises, and leave the CTest-only `elapsed_seconds` field at its existing boundary and precision. `timing.json` remains separate from `receipt.json`, with the existing pointer in each receipt. Timing serialization and process exit remain outside the launcher timer. No changes to these timing semantics were required by this integration.

## Focused validation

Command run from the parent checkout:

```powershell
python imports/claude-moproj2mst/optimization/test-speed/tests/test_runner_hash.py
```

Result: **10 tests passed**, 0.244 seconds reported by unittest on 2026-10-24. This is validation runtime, not an optimization throughput measurement. `--help` also completed and displayed the new options without launching CTest.

The synthetic tests check:

- Complete mapping and serialized key-order equivalence at 1, 2, 4 and 8 workers, including empty and multi-block files, nested roots, overlapping roots and absent roots.
- Default dispatch uses the unchanged serial callable.
- A same-size content mutation with its original nanosecond mtime restored changes SHA-256 for both engines.
- Outside-evidence file roots are refused; read denial and disappearance/stat failures propagate instead of returning partial snapshots.
- The helper rejects invalid worker counts, and the launcher rejects invalid CLI counts/engines before run creation.
- Mocked launcher runs preserve logged gaps, failed CTest classification and return code, after hashing on failed CTest, fixture-mutation failure, five phase names, and equality of per-run/root result receipts.

All temporary synthetic files were placed under the test directory and removed by `TemporaryDirectory`. Subprocess calls were mocked in launcher tests. No full suite, build, real evidence-root hash, original application, GUI control, or recovery-state mutation was performed.

## Limits and measurement gate

Worker count four is a configurable candidate, not a changed launcher default or a full-tree speed claim. The independent hash benchmark owns measured throughput conclusions. Real-corpus default changes should wait for quiescent full-root mapping and serialized-order comparisons and paired launcher timing. The current integration permits that experiment while retaining serial as the default.

Both engines require quiescent fixtures; neither creates an atomic filesystem snapshot. Concurrent read timing can change which concurrent mutation or simultaneous failure is observed. Symlink/junction behavior remains inherited from the helper's matching traversal/containment implementation and was not separately exercised in these tests. No wall-time or token-saving claim follows from the synthetic self-tests.

## Parent-path cache selector integration

The runner now additionally accepts `--hash-path-engine exact|parent-cache`, defaulting to `exact`. This option is delegated to the threaded helper on both complete fixture snapshot passes. The serial engine continues to call the original `snapshot(roots)` without using the selector, including if `parent-cache` was requested. `fixture_hash.path_engine` records the effective choice (`exact` for serial), and `fixture_hash.requested_path_engine` records the CLI selection. Existing fields and failure/gap decisions remain unchanged. The historical `cache:false` field describes content/hash caching; `parent-cache` does cache canonical parent paths within each individual snapshot call. No file contents or hashes are cached.

Example for a separately authorized paired measurement:

```powershell
python admin/run_converter_tests.py --build build/baseline --hash-engine threaded --hash-workers 2 --hash-path-engine parent-cache
```

The CLI help states the additional requirement that directory/reparse topology remain stable during capture. The runner neither changes the helper nor promotes the experimental selector to a default. No speed claim is made here; root owns full-root and launcher measurement.

The expanded synthetic test run on 2026-10-24 completed **15 test methods: 14 passed, 1 skipped**, in 0.396 seconds. The skipped actual file-symlink test was checked separately and reported Windows error 1314 (required privilege unavailable). No escalation or system setting change was attempted. CLI help also completed successfully.

New/expanded coverage includes:

- Full mapping and serialized insertion-order agreement for both path engines and 1/2/4/8 workers, with nested/overlapping roots, empty and multi-block files.
- Actual disposable Windows junctions: an internal target preserves mapping/order; an outside-evidence target is refused by serial, threaded exact, and threaded parent-cache. Junctions were removed before the synthetic temporary directory was cleaned.
- Mocked reparse-file metadata and absent Windows attribute metadata invoke full per-file resolution, rather than the ordinary-parent shortcut. Actual file-symlink behavior remains unverified because its test was skipped.
- Parent-cache `lstat` and canonical-parent resolution failures propagate. Existing open/stat/outside/missing-root checks and same-size/restored-mtime mutation checks now include parent-cache.
- Mocked launcher passes for both threaded path selectors preserve gaps, failed-CTest return codes, complete after snapshots and changed-fixture failure. Receipts distinguish requested from effective serial selection; invalid selector values are rejected before run creation.

All test data was newly created under `optimization/test-speed/tests` and removed by cleanup. No real evidence files, full suite, build, original application or recovery state was changed. The earlier ten-test result and junction limitation above describe the first integration revision; this section records the later expanded coverage.
