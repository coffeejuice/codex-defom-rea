# Checkpoint 46: folder pipeline extraction boundary

## Outcome

Stopped without editing `source/apps/moproj2mst/tests/s/test_save_folder.cpp`. The frozen accepted pipeline cannot be extracted as a small, confidently isolated patch from the provided reference. The only authorized edit target remains unchanged. This follows the instruction to stop rather than guess when extraction dependencies are uncertain.

## Verified file identities

- Active source: 324 lines, SHA-256 `50ea7cd32691de0c180565b5146fde801495ecf0336100c4a3abdac0b4b56f53`.
- Isolated accepted candidate: 1,563 lines, SHA-256 `8a27894ee6a289d844914bfaff1984b20b922cf8a15ef44d3287827002176dbb`.
- The isolated file has 1,239 more lines than active; it adds extensive event capture, process IPC, worker handlers and receipt code beyond the pipeline loop itself. No test/build was run.

## Exact code boundaries in the isolated file

- New event/outcome and raw-capture state: lines 160–185 (`CaseOutcome`, `FolderEvent`, discovery/event vectors, raw-receipt state).
- Diagnostic buffering for ordered replay: `diagnostic`, lines 425–449.
- Pipeline posting hooks and case adaptation: `runCase` plus posting state, lines 511–674. `runCase` changes behavior according to global discovery/posting flags and accepts ordinal, selected project and outcome parameters.
- Summary buffering: lines 676–691.
- Shared IPC serialization and mailbox/process support: approximately lines 696–1175, including `publishPost` (810), `folderPostWorker` (918), environment setup (1075) and `FolderChild` (1104).
- Accepted coordinator: `runPipeline` at lines 1186–1342. Even with its `prefetch` argument false, it launches `--folder-post-worker`, captures converter stdout/stderr, queues post-comparison work, then replays results in order. It is not just a loop around existing `runCase`.
- Rejected conversion sharding: `runShards`, lines 1344–1445. This region must not be imported.
- Mode selection and discovery integration: `TEST_CASE(save_folder_sweep)`, lines 1448–1548. It invokes `folderWorkers()` before deciding pipeline mode; sets `discovering` based on workers or pipeline; starts raw capture; buffers cases through `runCase`; and calls `runPipeline(scratch, prefetch)` or `runShards(...)`. The same function parses `M2M_FOLDER_PREFETCH` and `M2M_FOLDER_WORKERS` and therefore needs a deliberate rewrite, not a blind transplant.
- Command-line helper dispatch: `wmain`, lines 1550 onward, serves both accepted post-comparison and rejected prefetch/shard helpers.

The copy-prefetch worker implementation and its receipt protocol also occupy shared portions of the helper/IPC layer. Although `runPipeline(..., false)` does not execute copy prefetch, importing its dependency ranges wholesale would carry dormant prefetch code. Conversely, deleting those shared pieces without a dependency-level rewrite risks breaking the accepted post helper or serial fallback.

## Why a minimal patch is not safe from this reference alone

The accepted path requires a discovery pass that records each case while keeping converter invocation in the original single process/order, then a helper process for comparisons and ordered output. The current serial source prints each result directly and has no event/IPC abstraction. On the isolated side, these concepts are interleaved with raw receipt state and experiment-only execution modes. The coordinator also uses shared `FolderChild`, manifest, mailbox, CRT stream capture, `runCase` outcome fields and `wmain` dispatch. Removing prefetch and shard paths safely would require restructuring this shared transport and rechecking all call/cleanup/error paths, not merely copying the accepted coordinator.

The acceptance record says raw-receipt environment was unset, but raw-run setup and event recording still appear in the shared test entry point and `runCase`. The frozen source alone does not demonstrate that those branches can be omitted without changing accepted diagnostics, comparator behavior, or cleanup. As no independently minimized, accepted-only source patch or function-level proof is available, I did not guess at the cut lines.

## Exact next safe action

Prepare a dedicated minimized version of the pipeline from the frozen source in a separate review artifact, keeping only: event buffering, serial `runCase` execution in discovery order, the required post-comparison IPC worker, deterministic ordered replay, opt-in mode selection, and the corresponding helper dispatch. Remove prefetch, shard support, raw receipt and profiling/capture branches only after their references and side effects are traced. Compare the resulting call graph and output/error ordering against the frozen accepted implementation. Then review/apply that patch to the active file after checkpoint 46 ownership is released. Build and equivalence validation remain the root orchestrator's responsibility; none were performed here.
