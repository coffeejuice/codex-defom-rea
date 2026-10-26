# Checkpoint 46: minimal folder post-comparison pipeline

Implemented and source-reviewed; frozen for the root orchestrator's combined build and receipt comparison. No worker build, test execution, original-app activity, REA action, shared CMake/admin edit or external-path mutation was performed.

## Inputs and ownership

Owned files are `source/apps/moproj2mst/tests/s/test_save_folder.cpp` and this report only. Reviewed the subtree AGENTS.md, IMPLEMENTATION_PLAN.md, takeover inputs, `analysis/resume46-folder-pipeline-migration.md` and `optimization/test-speed/analysis/accepted-candidate-freeze.json`.

The pre-edit active test was 324 lines, SHA-256 `50ea7cd32691de0c180565b5146fde801495ecf0336100c4a3abdac0b4b56f53`. The accepted frozen source was 1,563 lines, SHA-256 `8a27894ee6a289d844914bfaff1984b20b922cf8a15ef44d3287827002176dbb`. Both hashes were asserted before extraction. The edited test is 849 lines, SHA-256 `38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83`.

## Dependency proof and cut

The earlier migration boundary was resolved by extracting the required call graph rather than transplanting its broad line ranges:

- `compareCase` (209) is the active comparator body mechanically moved from `runCase`, with its error-code local moved too. Its file enumeration, raw/relocated/masked comparisons, banner/view qualifications, ordered diagnostic strings, tally updates and leaf cleanup were preserved verbatim. The generation step asserted exact body equality against the pre-edit active source.
- `runCase` (268) retains the active copy/convert/refusal code and the sole `m2m::convert` call (305). Optional discovery records the selected project and assigns an ordinal; optional posting returns only after conversion succeeds. Unset pipeline and `M2M_FOLDER_PIPELINE=0` execute the ordinary serial path. There is no automatic retry of conversion after a helper/infrastructure failure.
- Event buffering and `diagnostic` (181) retain the frozen accepted ordering model. Summaries retain pointers to the live section tallies and are evaluated during ordered replay, after preceding cases contribute their deltas. Corpus/oracle directory traversal, project selection, exclusions and conversion arguments remain the active ones.
- The private transport (339 onward) carries only job identity, ordinal, compare/no-compare operation, tally and output. A distinct `*_MINIMAL_1` schema prevents accidental mixing with the larger experimental protocol. Profiling and raw-receipt fields were removed from both sender and reader. Their accepted environment was unset, and the frozen source guards their file output and measurement work with those settings; they are not dependencies of ordinary comparison or diagnostic text.
- `folderPostWorker` (447) is the frozen post-only worker with those optional payload fields removed. It never calls conversion or model initialization. Each successful case's completed work tree is compared and removed by that helper; copy failures and converter refusals remain owned by the parent path. No successful output is posted until its serial conversion returns.
- `quotedArgument`, `workerEnvironment`, `FolderChild` and `FolderCrtCapture` (518 onward) are byte-for-byte identical to the frozen accepted implementations. The stable parent stdout/stderr descriptors preserve converter output capture across conversions. The worker has its own temporary directories; the child wrapper stops/reaps only the child it launched on failure.
- `runPipeline` (639) retains the frozen accepted coordinator with the optional copy-helper blocks deleted. One persistent post worker overlaps comparison/cleanup with subsequent serial parent conversions. At most two case results are pending before collecting the oldest. Result identity, ordinal, terminal state, tally, trailing bytes, child exit status and unexpected stderr checks remain. Captured per-case bytes replay in binary mode to preserve their existing CRLF bytes; buffered discovery notes replay normally in their original event positions.
- `wmain` (840) has only the post-helper dispatch and normal test runner. The sole opt-in environment mode is `M2M_FOLDER_PIPELINE`, validated as 0 or 1. No process-sharded conversion, copy-prefetch code, rejected mode flag, profiling flag, raw-receipt flag or capture experiment was imported.

## Cleanup and failure limits

On normal completion each compared case leaf is removed by the unchanged comparator, refusals are removed by the unchanged parent code, copy skips survive until sweep cleanup, the helper is checked for completion, IPC is removed and the sweep scratch root is removed. Pipeline global state and discovery state reset with local destructors.

On infrastructure failure the child destructor terminates/reaps the owned helper and CRT-capture destructors attempt restoration. Diagnostic scratch is retained because the outer cleanup is not reached, as in the accepted coordinator. Failures are surfaced, not counted as successful comparisons. The frozen restoration implementation may retain its saved descriptor after a failed restoration to permit destructor retry; this extraction does not strengthen that inherited behavior.

The asynchronous comparator can alter physical resource timing; this is the already accepted pipeline behavior, not a new proof of resource-failure ordering. The rejected copy-prefetch mode remains entirely absent. This source review establishes extraction boundaries; it does not establish runtime equivalence or newly measure speed.

## Source-only checks and integration gate

All checks passed: exactly one conversion call site; exactly one post-helper command dispatch; no forbidden mode/profile/raw names; no conversion in the post-worker body; exact frozen identity for the diagnostic, capture, argument/environment and child-lifetime helpers. The active comparator body equality was asserted while producing the file.

Root owns compilation, focused folder execution, full optimized launcher, pre/post fixture hashes and ordered historical receipt equivalence. These remain pending at this worker freeze. The prior accepted checkpoint45 results qualify only the frozen reference, not this active candidate. No parity or objective-completion claim is made here.
