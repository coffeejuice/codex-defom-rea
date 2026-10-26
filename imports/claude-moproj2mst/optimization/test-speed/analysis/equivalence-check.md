# Full-run evidence comparison

The comparator is ready for the isolated candidate. No new converter tests, build, fixture changes, or original-app session were performed in this review. Recovery remains paused.

The preserved checkpoint-45 baseline is `analysis/wave3-candidate45-doe-fix-d-tests.json`, bound to `build/test-runs/1792854086489696400` and `build/candidate`. Its self-comparison passes with 150 registered/reported tests and three ABI test names. This checks comparator compatibility with the existing evidence, not candidate equivalence.

From the converter root, compare a completed full candidate run as follows. Set `$candidateRun` to the exact run directory reported by the launcher. The updated launcher also preserves `receipt.json` inside that run; use that bound copy rather than the shared results file.

```powershell
$candidateRun = 'build/test-runs/REPLACE_WITH_CANDIDATE_RUN_ID'
python admin/compare_test_speed_runs.py `
  build/test-runs/1792854086489696400 `
  analysis/wave3-candidate45-doe-fix-d-tests.json `
  $candidateRun "$candidateRun/receipt.json" `
  --baseline-source source `
  --candidate-source optimization/test-speed/source `
  > optimization/test-speed/analysis/full-run-equivalence.json
$LASTEXITCODE
```

Exit 0 means equivalent recorded evidence; 1 means a semantic difference; 2 means evidence is missing/inconsistent. Preserve the report even on failure. The isolated candidate build is `build/test-speed-candidate`; its actual build root comes from its own receipt, never from a hard-coded substitution.

Normalization replaces only declared run/build roots and the explicit paired source roots. Windows path separators and case are accepted, with a suffix boundary so `candidate-other` cannot be mistaken for `candidate`. Source roots are required together. CMake backtrace source line numbers may move after source insertions; the graph edges, referenced files, commands, test bindings, test properties and registration count remain compared. Test-row diagnostics receive the same declared-root substitution as raw case logs. No counts, refusal codes, gap events, folder results, file comparisons, or other numeric text are stripped.

Fixture hash engine/worker metadata may differ only when it declares an allowed uncached SHA-256 implementation; unknown settings are rejected. Legacy checkpoint-45 receipts predate this metadata. Both before/after fixture manifests remain exact comparisons, including file size, modification time and SHA-256. Run failures, modified fixtures, missing/unreported tests, and incomplete output block acceptance even when both runs share the same problem.

Each changed raw artifact is recorded under `raw_differences` with both paths and SHA-256 values: inventory, fixture manifests, preserved receipt, JUnit, aggregate console/CTest logs, and per-test logs. Files are never rewritten. Aggregate logs contain scheduler ordering and timing, so their raw differences are disclosed but semantic comparison uses the validated per-case JUnit logs and receipts. Per-case raw changes still block unless fully explained by declared path roots. Arbitrary timing text inside a case log is not normalized. Declared receipt timing fields and test durations are separately disclosed as permitted differences.

Validation performed: checkpoint-45 self-comparison; path case/separator and sibling-boundary checks; six in-memory mutations confirming that CMake source-line movement and hash execution metadata are permitted, while an extra test argument, changed folder `differ` count, changed ABI diagnostic, and changed fixture hash block equivalence. These checks ran only Python against preserved artifacts.

Full-run candidate equivalence remains pending until the command above completes with exit 0. This establishes unchanged suite evidence and retains all historical oracle/ABI/diagnostic qualifications; it does not establish fresh original-application parity or scientific completion.

## Parent directory resolution metadata follow-up

The full-run comparator accepts the newer exact metadata shape containing both `path_engine` (effective) and `requested_path_engine`. The only values are `exact` and `parent-cache`; threaded execution requires the effective value to match the request. Serial execution requires one worker and effective `exact`, while preserving either recognized request as a recorded ignored option. Both old four-field and new six-field metadata require SHA-256, `cache:false`, a recognized serial/threaded engine and integer workers 1..32. Unknown fields, unknown/mismatched path engines, missing paired fields, content caching and other algorithms are rejected. `parent-cache` records snapshot-local parent directory resolution reuse; it does not authorize reuse of content hashes.

Metadata is not evidence of matching fixture content. Both fixture manifests now require identical raw bytes and identical key order as well as every size/timestamp/SHA-256 entry. Per-case raw logs and all normal semantic checks remain active. The source helper's separately reported twelve full-root matches do not substitute for these preserved full-run artifacts.

The completed 150-test run `1792860623423755500` (threaded four workers, parent-cache resolution, folder COST=25) compares equivalent to checkpoint-45 with zero blockers. Report: `optimization/test-speed/analysis/parent-cache-full-equivalence.json`. Both before/after raw manifest files are byte-identical to checkpoint-45. The COST scheduling qualification and every raw artifact difference remain reported; this is recorded suite evidence equivalence, with the existing historical qualifications.

Follow-up Python validation accepted four supported metadata shapes, rejected thirteen invalid metadata mutations (including unknown engines, unknown path engines, cache changes, other algorithm, worker limits/type, missing request field and extra fields), and confirmed that manifest-byte, manifest-order and raw diagnostic mutations still block. No suite, build, fixture mutation or original process ran for this comparator adjustment.

## CTest parallel scheduling follow-up

The comparator permits a changed value following exactly one separate `-j` argument when each value is a positive ASCII decimal integer string. After replacing that value and declared roots, the argument arrays must otherwise agree exactly, including position/order, test label, output limits and log paths. Combined options, missing/duplicate `-j`, zero/negative/fractional/non-string values and any other changed argument are not qualified. Inventories, environments, membership, fixture manifest bytes/order and raw semantic logs remain independently compared.

Both original command arrays are included as `ctest_commands`. Preserved receipt/aggregate raw artifact SHA-256 values remain in `raw_differences`. A permitted scheduling change reports the actual old/new `-j` values and adds a qualification that different concurrency changes contention and timing; scheduling metadata cannot substitute for matching evidence.

The completed 150-test pipeline+parent-cache-four-worker runs `1792860856922203600` (`-j 4`) and `1792860888921542300` (`-j 16`) each compare equivalent to checkpoint-45 (`-j 8`), with zero blockers. Reports are `pipeline-parent-cache-j4-equivalence.json` and `pipeline-parent-cache-j16-equivalence.json` under this analysis directory. Both preserve the scheduling qualifications and every raw artifact change. Four positive values were accepted in focused Python checks; thirteen malformed/duplicate/other-argument/environment/membership/log mutations remained blockers. No suite, build, fixture mutation or original process ran for this adjustment.
