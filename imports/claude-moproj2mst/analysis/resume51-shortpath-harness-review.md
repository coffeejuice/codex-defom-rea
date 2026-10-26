# Independent review: short-path runtime-root harness

Review scope: read-only source inspection of `tools/reference_harness/harness.py` and its tests. No harness code, build, runtime path, original app, or REA database was changed.

## Findings

### [P1] Candidate observation can bind the wrong project path

At `tools/reference_harness/harness.py:483-491`, validation checks only the disposable cwd, argv executable path, and executable digest. For a Moproj2mst candidate it checks that `--write-save` appears at lines 481-482, but it never checks `argv[1]` against `cfg["runtime_sandboxes"]["candidate"]["project_file"]`. The recorded project-file mapping is therefore unused for candidate invocation. A candidate can be declared completed while it was pointed at another project; harness then snapshots the prepared short-path case tree, potentially unchanged, and reports that as the candidate result. This undermines the raw comparison's input/output binding.

**Correction:** in runtime-root mode, require candidate `argv[1]` to resolve to the recorded candidate `project_file` (and confirm the same file is contained in that role's project tree). Record that exact argument. For the original GUI, add a required `session.project_path` field in runtime-root mode and validate it against the original mapped project file; the GUI ignores project command-line arguments, so the path must come from the observed File>Open action evidence, not `argv`.

### [P2] Runtime length cap is enforced only before execution

`runtime_layout` checks the project fixture's existing paths at lines 151-161, and `verify_runtime_layout` checks the four stored anchors at lines 198-207. However, `record` manifests the post-run sandbox at lines 543-549 without checking every resulting path against `MAX_RUNTIME_PATH_CHARS`. The application can create a deeper generated path longer than 99 characters, and the harness will accept and retain it despite the stated rule for generated paths.

**Correction:** when recording a runtime-root case, reject any `after` manifest entry whose resolved absolute path (`sandbox / relative_entry`) is 100 characters or longer. Keep the raw observation/effect evidence append-only and surface the violation as a failed record/preflight diagnostic; do not silently discard output.

## Reviewed protections that appear sound

- Runtime root is strict-descendant of `PROBLEM_ROOT`, must be new, is checked for links/reparse points, path length, and overlap with fixture/source/case/store/original/candidate binary paths (`runtime_layout`, lines 137-164; rechecked by `verify_runtime_layout`, lines 181-211).
- Original and candidate trees are separate and link-free, and preflight compares both against their frozen before-manifests (lines 424-433).
- Original recording checks the candidate tree is still unchanged (lines 532-538); candidate chronology continues to require an original receipt.
- Runtime output snapshots and comparison receipts remain in the case store; the external runtime directories are the sources for recorded manifests/copies.
- Build/source identity remains tied to the copied source, candidate binaries, and supplied build receipt via existing freeze checks.

Symlink-creation-specific runtime test coverage remains skipped on this host due Windows privilege denial, matching the existing harness limitation; static no-link checks remain present.

No app launch or app-control acquisition occurred during this review.
