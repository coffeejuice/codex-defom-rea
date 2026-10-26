# CP69 design: configure once for focused preflight plus full build

## Goal and compatible entry point

Add an opt-in configure-reuse mode to `admin/build_candidate.py` that configures `build/candidate` once, builds a grouped set of focused targets, then performs the full build in the same build directory and configured CMake cache. Keep the existing default behavior and the existing focused-only `--target` behavior unchanged.

Suggested invocation:

```text
python admin/build_candidate.py --label cp69-integrated --preflight-target target_a --preflight-target target_b --then-full
```

Require one or more `--preflight-target` values with `--then-full`. Reject mixing these with the legacy `--target` option. The full-build mode records top-level `targets: null`, matching the freeze checker’s existing `assert not build['targets']`; the focused target list is recorded separately as `preflight_targets`. A caller that wants only the focused build continues using legacy `--target` and does not receive a freeze-eligible full-build receipt.

## Snapshot and cache state machine

1. Fingerprint the complete source tree and file count, run the existing Release/Ninja configure in `build/candidate`, then fingerprint source again. Record the configure input and output fingerprints. If source changed during configure, mark that configuration superseded, reconfigure for the new fingerprint, and recheck before building.
2. Before focused targets, verify the live source fingerprint still matches the fingerprint for the current CMake configure. Run the whole grouped target list in one `cmake --build ... --target ...` call. Record the target set, source fingerprint, build directory, command, exit code, elapsed time, and log on the step.
3. After focused build and immediately before full build, fingerprint source again. If it changed, reconfigure and rerun the entire focused group against the new snapshot before any full build. Do not combine a preflight pass from one snapshot with a full build from another.
4. Run the full `cmake --build build/candidate --parallel 4` command using that same configured cache. After artifact collection, fingerprint source again. If source changed during the full build, the run is not freeze-eligible; reconfigure and repeat the focused and full build sequence only while within a small explicit retry cap. On cap exhaustion, preserve the receipt, return nonzero, and mark the candidate unstable.

Record a SHA-256 of `build/candidate/CMakeCache.txt` after configuration and before each build phase. The focused and full phases must have the same cache hash. If it changes (including CMake/Ninja regeneration), rerun configure and the focused group before full build. Also record the CMake source/build paths, generator, build type, and parallelism so the reused cache is inspectable. Source reconfiguration/retry events should be explicit receipt records; do not silently overwrite a failed or superseded attempt.

The retry cap should be explicit (for example, two reconfigurations after the initial configure). A live worker that keeps changing source must eventually cause a nonzero, unstable receipt rather than an unbounded loop. A stable-source ownership window remains the preferred operating condition.

## Receipt contract for freeze compatibility

Preserve the existing `m2m-candidate-build-v1` top-level fields consumed by `admin/freeze_checkpoint.py`:

- `source_tree_sha256` and `source_file_count` identify the exact final configured/preflight/full-build snapshot.
- `source_tree_after_sha256` and `source_file_count_after` identify the source tree after full build and artifact hashing.
- `source_stable_during_build` is true only when selected configure, focused build, full build, and post-build fingerprints all match and all required commands succeeded.
- `targets` is `null` for the overall full-build receipt; all command steps have exit code 0 for freeze eligibility.
- `artifacts` are hashed after the successful full build, not after the focused phase. Keep the required executable names and hashes unchanged.
- `steps` remains a list of command/exit/time/log records. Add the focused build as a step with phase metadata and the target list; preserve the configure step and final unqualified full `build` step so logs and existing summaries remain readable.

Add only optional metadata: `preflight_targets`, `configure_reuse` (cache path/hash, reconfigure count, and retry limit), and per-step `phase`, `source_tree_sha256`, and `source_file_count`. The existing freeze checker can continue consuming successful receipts without understanding optional metadata. Its final source fingerprint check, test-to-build source binding, and executable manifest comparisons remain authoritative.

On any configure, focused build, or full build command failure, stop and write a nonzero receipt. If drift forces a reconfigure, retain all prior successful steps as superseded records and run all required phases on the replacement snapshot. Do not let a successful later build erase the fact that any step failed, because freeze currently requires every step to pass. In the simpler supported policy, stop on an actual compile/link failure and let the owner repair then start a new label; auto-reconfiguration is for source/cache drift, not for hiding build failures.

## Safety limits

Content fingerprints detect persistent tree changes between boundaries; they cannot prove that a worker did not edit and restore identical bytes between two measurements. The tool does not lock `source/`, and a same-directory concurrent process could still mutate `build/candidate`. Therefore use the mode only after workers release the source/build ownership window, or add an external build-directory lock in a separate reviewed change. Never assert concurrent mutation is prevented by hashing alone.

Do not change `admin/freeze_checkpoint.py` as part of this design. Before implementation, keep the source/build receipt schema frozen through CP69 archive creation. Afterward, validate with unit tests for: one configure plus two build commands; preflight/full fingerprint equality; drift after configure and after preflight causing reconfigure plus preflight rerun; cache-hash mismatch; retry-cap exhaustion; failed target/full commands; final `targets: null`; final artifact hashing; and compatibility with the existing freeze assertions. Build and full CTest remain root-owned integration steps.

## Expected efficiency, without extrapolating

Recent checkpoints record about 10.7–11.0 s per configure invocation. A single configure reused for focused targets plus the full build has an approximately 11 s measured configure-phase opportunity per stable checkpoint. The CP66–68 receipts do not establish the full end-to-end gain because build work varied substantially; CP68 also had 89 C++ object compilations versus 7–8 in CP66–67. Keep the optimization limited to the redundant configure and preserve full build/test coverage.
