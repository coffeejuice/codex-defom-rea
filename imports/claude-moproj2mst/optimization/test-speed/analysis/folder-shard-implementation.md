# Folder sweep process sharding candidate

The optimization source now has an opt-in process coordinator. No speedup or scientific equivalence is claimed. The default remains the existing in-process serial sweep, including when `M2M_FOLDER_WORKERS=1`. The original converter source, fixtures, installed applications, CMake and administration files were not edited by this lane. No build or test run was performed in this lane.

## Implemented contract

- The existing folder-sweep/corpus gates run before parsing the new setting. `M2M_FOLDER_WORKERS` accepts decimal values 1 through 16; malformed settings produce a testkit infrastructure failure. Absence selects 1.
- For values above 1, the coordinator discovers cases once in the existing filesystem iterator order. It preserves the corpus saved-log predicate, explicit re-save ordering, exclusions and NOTE event positions. Preflight still selects the last exactly `.moproj` top-level entry. Ineligible cases consume no work ordinal or case tally.
- Each eligible case receives its original global 1-based scratch ordinal. Static `(ordinal - 1) % workers` assignment preserves predecessor order inside each process. No duration-based scheduling was added.
- The private Unicode entry point is `--folder-worker <absolute-job-manifest>`. Workers bypass testkit registration and cannot recursively coordinate. Windows `CreateProcessW` launches the same executable with a quoted argument and a separately constructed environment block; no shell is involved. The initial working directory and all converter settings are inherited.
- Each worker has separate existing `TEMP`, `TMP` and `TMPDIR` directories beneath the test scratch root. The parent environment is unchanged. IPC and captures are outside compared case leaves. Workers retain existing per-case copy, conversion, comparison and cleanup behavior; the parent retains sweep-root cleanup ownership.
- The comparator and existing diagnostic classification stay in `runCase`. Case stdout, including converter writes through standard CRT/Win32 handles, is captured privately. The coordinator replays it at the discovered event position and keeps summaries and NOTES at their original positions. Captured Windows newline translation is not applied a second time during replay. Profiling rows are part of case captures when profiling is enabled.
- Results carry schema, exact job-manifest identity, expected result count, ordered ordinal, all tally fields, explicit copy-skip/refused/compared state, raw converter refusal code/full message, and rendered stdout. Schema/identity, count, ordinal, tally/state bounds, truncation and unexpected tails are checked before any partial result is aggregated or replayed.
- Harness capture/identity reads are bounded and checked for open/read/length errors independently of the unchanged historical comparator helper. C++20 `char8_t` path strings are copied explicitly as UTF-8 bytes and reconstructed through `fs::u8path` in the worker.
- Worker stderr is private. Any successful worker that emitted stderr causes a hard infrastructure failure naming its capture path. Exact stderr replay with case boundaries is deliberately not claimed, and diagnostics are never silently discarded in an accepted sharded run.
- Spawn failure, unsuccessful child exit, malformed result or a worker wait above ten minutes fails the harness. Owned child handles terminate/reap outstanding workers during exception unwinding. Failure IPC stays in scratch for diagnosis; successful IPC is removed. Ordinary adjusted differences/refusals remain report data rather than new scientific assertions.

## Required root validation and acceptance limits

Read-only checks used the design and `profile-v1-ctest.log` (whose folder test passed in 19.50 seconds). That historical serial profile does not measure this candidate's overhead or parallel performance.

The root's first incremental build found the C++20 `std::u8string`/`std::string` mismatch in manifest writing. This lane corrected the byte conversion and notified root to rebuild. Successful compilation and runtime outcomes must be taken from the root's subsequent validation receipts.

The orchestrator must compile the candidate and run targeted disposable probes for missing input/expected preflight, copy failure, converter refusal, malformed worker manifest, child crash/incomplete output and nonempty stderr. Invalid worker settings should fail only after the existing sweep gates are enabled; disabled gates and absent/1 worker settings must preserve the serial path. All fixture mutation for these probes must be coordinated and confined to owned scratch; existing evidence fixtures must remain immutable.

The following portions of the design are **not implemented** and block acceptance of sharded mode as an equivalent replacement:

1. Per-case full raw relative-file inventories, sizes and SHA-256 hashes before cleanup, complete untruncated adjusted vectors and per-file decision stages. Current results retain the existing rendered diagnostics and tally semantics; these do not establish raw equivalence.
2. A receipt binding those inventories to executable/source/fixture hashes. Exact manifest echo binds the coordinator's scheduled paths and names, not fixture bytes or source identity. Launcher fixture snapshots remain required.
3. A validation-only replay against identical explicit case work paths in serial and sharded schedules. Different coordinator PIDs/work paths and worker temp paths may affect raw output, and predecessor engine state may differ between shards. Matching adjusted counts alone cannot resolve this.
4. The paired repeated worker-count/full-launcher benchmark matrix and synthetic failure coverage requested by the design. No worker count has been selected.

Retain the serial default while completing these gates. The implementation does not alter engine state, add masks, weaken fixture hashing or remove cases to hide any mismatch.
