# CP70 grouped builder and freeze-gate review

## Receipt compatibility

The successful grouped-mode receipt preserves the fields consumed by `admin/freeze_checkpoint.py`:

- `schema` remains `m2m-candidate-build-v1`.
- Top-level `targets` is `null`; the focused target set is additive in `preflight_targets`.
- `source_tree_sha256` / `source_file_count` identify the selected final snapshot; `source_tree_after_sha256` / `source_file_count_after` record the post-build tree.
- `source_stable_during_build` is true only for a successful grouped sequence whose selected source and CMakeCache fingerprints remain equal through the full build and artifact hashing.
- The final all-target step is still named `build`, has the full-build command and exit code, and the artifact manifest is collected after that full build.

Freeze requires the build receipt to be stable and targetless, all step exit codes to be zero, and the live source fingerprint to equal the receipt. It then requires the test receipt’s before/after binding to be stable, to match that build source fingerprint, and to match hashes for the production executable/DLLs. The grouped receipt’s success shape satisfies these checks. `admin/freeze_checkpoint.py` does not validate `configure_reuse` or preflight metadata; these are audit fields, not substitutes for its source/test/artifact checks.

No CP70 build/test/validation receipt or `wave3-checkpoint70-verified` directory was present during this read-only review, so the actual freeze path has not yet been exercised with a grouped receipt.

## Drift and failure-path findings

On the successful path, the runner hashes `CMakeCache.txt` after configure, before and after focused preflight, before and after the full build, and after artifact hashing. A persistent cache mismatch triggers configure plus a complete rerun of the focused group before the full build. Source fingerprints are checked at corresponding boundaries. Thus a cache edit that remains visible at a checked boundary is not silently accepted as the selected configured cache.

There is one failure-path inconsistency. In configure, focused-preflight, and full-build handling, source/cache drift is checked before the command’s nonzero exit code. If a command both fails and changes source/cache, the loop retries; a later successful sequence can return process exit code 0 and mark the overall status `completed`, while the receipt still contains the earlier nonzero step. The freeze checker will reject that receipt because it asserts every step has exit code 0, so this does not produce a freeze-acceptable stale candidate. It can, however, make the CLI’s success exit misleading and contradicts the design rule to stop on an actual command failure. The simplest correction is to stop and return nonzero for any nonzero command exit, recording the drift but requiring a new label after repair. Drift retries should apply only when each command itself succeeded.

Hash checks also cannot detect an edit-and-restore between two reads. The builder has no lock for `source/` or the shared `build/candidate` directory. Another process could alter generated build files or object artifacts without changing `CMakeCache.txt`; the cache digest alone does not serialize or fully fingerprint the build directory. Keep source and build ownership exclusive for the grouped run. If concurrent builds must be supported, add a build-directory lock in a separately reviewed change.

## Measured comparison protocol

No real-run speed comparison was performed in this audit. For the first controlled comparison after root approves use:

1. Freeze one source snapshot and record its full-tree fingerprint/file count. Select one fixed grouped preflight target set, the same Release/Ninja toolchain, and parallelism 4.
2. Run three paired trials on separate empty build directories. In each trial, the baseline performs configure + grouped focused build, then configure again + full build. The candidate performs one configure + the same grouped focused build + full build. Alternate which mode runs first across pairs to reduce cache-order bias. Keep fixture paths, full CTest command, hash options, and test inventory identical.
3. Preserve per-command configure, focused-build and full-build elapsed times; source/cache before-and-after fingerprints; all artifact hashes; and the complete test-run timing/fixture/binary-binding receipts. Require both modes to use the same source fingerprint and pass the same full 214-or-current test inventory with identical fixture and stable-binding status before comparing time.
4. Report paired per-phase and total medians, plus spread. Claim only the measured configure-reuse reduction if the focused and full outputs remain bound to the same source and all full-suite/hash checks pass. If test counts, build outputs, or bindings differ, report the comparison as inconclusive rather than a speed gain.

This protocol makes the build-cache condition explicit: every trial begins with an empty, independent build directory, while the baseline and candidate within a pair use the same source and build settings. It also includes the full CTest and its hashing/binding cost, so no apparent improvement can come from omitting validation.
