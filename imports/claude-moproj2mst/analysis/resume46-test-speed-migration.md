# Checkpoint 46: accepted test-speed migration audit

## Decision

Carry forward only the frozen, evidence-equivalent configuration: serial folder conversion with the opt-in `M2M_FOLDER_PIPELINE=1`, full fixture hashing via four hash workers and the `parent-cache` path engine. Keep `M2M_FOLDER_PREFETCH` and `M2M_FOLDER_WORKERS` unset; do not import prefetch or process-sharded conversion. This is a validation-time harness configuration, not a converter behavior change or evidence of original-app acceptance.

## Exact comparison

The freeze at `optimization/test-speed/analysis/accepted-candidate-freeze.json` identifies source archive SHA-256 `87129b8ce7f8ee3df7ff78971eaecc0217924272632b4a2567c6f37904578832`, frozen folder test SHA-256 `8a27894ee6a289d844914bfaff1984b20b922cf8a15ef44d3287827002176dbb`, CMake SHA-256 `864a6a77fae5978201cf5d705022b976e26a1113ccb74e849c21c4efba189a4b`, and the following helper files:

| Active path | SHA-256 | Migration |
|---|---|---|
| `admin/run_converter_tests.py` | `33c2190a21ef4ec7051ec45b6bb5efd89f677371b6fcb7550b206ce3c6249f13` | Already exactly the frozen runner; no edit. |
| `admin/fixture_hash_fast.py` | `68d86366832c4f1c7fc8a42b962e56c565c7f9cfe5d408e93cc0423b4674eba9` | Already exactly the frozen hash helper; no edit. |
| `admin/compare_test_speed_runs.py` | `51adb622ef437343d03c1cb1757a06e0ba9429d558a20ee9c38b7f4857e160c3` | Already exactly the frozen equivalence checker; no edit. |
| `source/harness/defcmp/src/hash.cpp` | `61451b4c0e3a7bf9877c210926cf785f1c7227025e8ff3ca50ed9604ddbfc143` | Active and isolated versions match; no edit. |
| `source/apps/moproj2mst/tests/s/test_save_folder.cpp` | Active `50ea7cd32691de0c180565b5146fde801495ecf0336100c4a3abdac0b4b56f53`; isolated `8a27894ee6a289d844914bfaff1984b20b922cf8a15ef44d3287827002176dbb` | The only substantive code migration. Apply a reviewed, minimal patch to the active file after reconciling it with checkpoint 46. Do not replace it wholesale. |
| `source/apps/moproj2mst/CMakeLists.txt` | Active `0279701314aa0a4d743869e20a87098de79dd24d91696bb6f973094d321435a4`; isolated `864a6a77fae5978201cf5d705022b976e26a1113ccb74e849c21c4efba189a4b` | Active already defines folder-test `COST 25.0`. The other isolated differences are experiment commentary and `M2M_EXPERIMENT_TAKEOVER_ROOT` path plumbing. Do not transplant them into recovery CMake; configure the active build using its existing source/evidence roots. |

The accepted runtime options are `--build build/test-speed-candidate --jobs 4 --hash-engine threaded --hash-workers 4 --hash-path-engine parent-cache`, with `M2M_FOLDER_PIPELINE=1` and all of `M2M_FOLDER_PREFETCH`, `M2M_FOLDER_WORKERS`, and `M2M_FOLDER_RAW_RECEIPT` unset. The build-specific evidence-root define and isolated build paths from `build_isolated.py` do not belong in the active recovery build.

## Test-source patch boundary

The test-source delta contains the accepted coordinator pipeline and serial fallback, plus unrelated experimental branches. Restrict a transplant to the minimum functions and data needed when `M2M_FOLDER_PIPELINE=1` is set: deterministic case discovery, serial conversion in original case order, retained per-case event results, then ordered reporting/summary. Keep the pipeline opt-in so an unset variable follows the current active serial path. Retain the existing comparator and complete case coverage.

Exclude all behavior guarded by `M2M_FOLDER_PREFETCH` (including its future-copy scheduling and prefetch receipts), all worker-count parsing/use guarded by `M2M_FOLDER_WORKERS` (process-sharded conversion), and shard identity/scheduling code. The freeze explicitly rejects both: process sharding changed `B:107_B_HTFF/SimCtrl.KEY` through process-global unit state, while prefetch did not prove original physical copy/resource-failure ordering. Avoid copying telemetry or raw-receipt output branches unless separately shown to be required for accepted pipeline semantics; `M2M_FOLDER_RAW_RECEIPT` was unset for the accepted final run.

No migration is needed for hashing or equivalence tooling: their exact frozen hashes are already present in active `admin/`. Preserve default serial hashing and exact-path behavior as defaults; the faster settings are explicit runner arguments, not changed defaults.

## Checkpoint 46 conflict and safe integration order

Checkpoint 46 is an integration candidate with interrupted/partial palette API/editor work; `analysis/user-stop-checkpoint45-candidate46.json` specifically says the shared palette API source state was not build-validated and directs ownership/freeze inspection before building. The test-speed test source and CMake scheduling property are separate paths, but the checkpoint candidate may have been assembled from a source snapshot. A broad source-zip extraction or directory overlay could silently revert unrelated checkpoint 46 edits. The folder test also sits inside the main source tree and must not be copied while its ownership is assigned or while a build freeze is active.

Safe order:

1. Verify the currently selected checkpoint 46 source candidate and ownership/release state; preserve its exact tree and hash/commit before touching integration files. Do not infer release from the prior stop record.
2. Confirm `admin/run_converter_tests.py`, `admin/fixture_hash_fast.py`, `admin/compare_test_speed_runs.py`, `source/harness/defcmp/src/hash.cpp`, and the active CMake `COST 25.0` setting still match the hashes/state above. If they do, leave them untouched.
3. Diff the current active `test_save_folder.cpp` against the isolated frozen copy and construct a focused patch by function/branch semantics, carrying only the opt-in pipeline and serial coordinator path. Reconcile the active test file's current changes first; never replace it from the isolated tree or frozen zip.
4. Keep `M2M_FOLDER_PREFETCH` and `M2M_FOLDER_WORKERS` absent in implementation and invocation. The pipeline must preserve case conversion order and same-process execution.
5. Only after checkpoint 46 owners release relevant source and the orchestrator establishes a build freeze, integrate the patch and use a distinct recovery build/run receipt. First run focused folder validation, then the complete registered suite with full before/after fixture hashes and the accepted four-worker parent-cache hash settings. Compare outputs using the existing comparator; retain all raw differences and gaps.
6. Record the new source hashes, invocation environment, build identity, test counts, comparator result and fixture hashes in a new checkpoint receipt. Do not re-label the historical checkpoint45 run as a checkpoint46 result.

## Evidence and limits

The accepted freeze records 150 registered tests, best observed complete launcher time 25.250 seconds, final frozen run 25.390 seconds, fixture before/after hash equality, and complete normalized/raw comparisons. The comparison remains historical-oracle evidence with six logged gaps and five folder diagnostic differences; it is not fresh original-app acceptance. The full 10x goal was not achieved. The isolated source/build and its receipts remain preserved as evidence and are not themselves an active-source migration.
