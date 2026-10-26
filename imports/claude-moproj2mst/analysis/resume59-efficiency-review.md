# CP59 efficiency review: retained CP52-58 cycles

The measured bottleneck is build/integration rework, not the full test runner. The retained window contains 621.637 seconds of build-driver occupancy versus 210.562 seconds across eight full launchers. The lowest-risk next change is one root-owned grouped compile/link preflight for new test targets and changed ownership consumers, before the broad build. Keep the final exact full freeze intact and measure whether the preflight pays for itself.

This is a read-only receipt/log analysis; only this report was written. No source, CMake, shared state, executable/build, GUI or mutable REA action occurred. Token usage, total agent-hours, model-comparison throughput and token savings are unavailable. No 10x or other realized speedup is claimed.

## Inventory and scope

The bounded inventory selects analysis/resume52 through resume58 build JSON containing started_unix and steps, plus the full test-run timing receipts in each checkpoint's first-build-to-next-checkpoint-first-build time window. The run directory timestamp supplies association to a window, not proof of a source freeze; accepted validation JSON supplies the actual source/build/test binding. CP58 had two failed build attempts and no accepted validation/full launcher at inspection time. Do not extrapolate a completed CP58 gate.

Build-driver receipts contain configure/build steps, source/artifact hashing and wrapper work. Launcher timing contains inventory, all fixture hashing, CTest and reporting. Do not add nested steps to their wrappers. The twenty build receipt intervals do not overlap; their interval union is 621.637 seconds. Build-plus-launcher component totals exclude diagnosis/editing/coordination/packaging gaps and are not total checkpoint or agent wall time.

| CP | Build attempts | Targeted / broad attempts | All build receipts, s | Configure total, s | Compiler/link failures | Source-unstable only | Full launcher attempts / internal total, s |
|---|---:|---:|---:|---:|---:|---:|---:|
| 52 | 7 | 5 / 2 | 120.478 | 72.748 | 1 | 3 | 1 / 27.469 |
| 53 | 6 | 0 / 6 | 99.556 | 62.828 | 3 | 0 | 2 / 51.812 |
| 54 | 1 | 0 / 1 | 108.351 | 10.313 | 0 | 0 | 1 / 26.781 |
| 55 | 1 | 0 / 1 | 14.120 | 10.281 | 0 | 0 | 1 / 26.313 |
| 56 | 2 | 0 / 2 | 51.016 | 21.079 | 0 | 0 | 2 / 52.328 |
| 57 | 1 | 0 / 1 | 107.428 | 10.531 | 0 | 0 | 1 / 25.859 |
| 58 pending | 2 | 0 / 2 | 120.688 | 21.157 | 2 | 0 | 0 / unavailable |
| Total | 20 | 5 / 15 | 621.637 | 208.937 | 6 | 3 | 8 / 210.562 |

The cmake-build steps total 363.455 seconds; configure totals 208.937 seconds, with the remaining 49.245 seconds in wrapper/source/artifact work. Six failed compiler/link attempts consumed 190.884 seconds of build-driver receipts. Three additional CP52 attempts compiled successfully but failed the whole-source stability contract, consuming 46.564 seconds. These attempts may provide reusable objects or useful local feedback; their full durations are not automatically recoverable savings and their receipts cannot constitute frozen validation.

## Focused feedback versus full freeze

All five explicitly targeted build receipts in the window belong to CP52: save_io 17.548 seconds; operation_info_cleanup 14.104; report_path 14.109; widget_texture_image_binding 14.907; brush_texture_image 14.018. Together they occupied 74.686 seconds, including 51.811 seconds of configure. Three were source-unstable. There are no separately labeled focused build receipts in the selected CP53-58 inventory; an unrecorded direct command is not inferred absent or timed.

The corresponding target bodies in the accepted CP52 full CTest run took 4.44248 seconds for save_io and approximately 0.0133-0.0141 seconds for the four small MDT targets. These are test-body times inside a full run, not measured standalone focused-command startup or complete edit-loop times. A complete focused CTest driver duration is unavailable from these selected receipts.

| Accepted CP | Registered tests | Last accepted build, s | Internal launcher, s | External launcher, s | CTest, s | Pre/post complete fixture SHA, s |
|---|---:|---:|---:|---:|---:|---:|
| 52 | 165 | 19.347 | 27.469 | 27.649 | 13.843 | 12.813 |
| 53 | 173 | 14.708 | 25.515 | 25.701 | 12.031 | 12.641 |
| 54 | 177 | 108.351 | 26.781 | 26.959 | 12.828 | 13.047 |
| 55 | 177 | 14.120 | 26.313 | 26.503 | 12.406 | 13.031 |
| 56 | 179 | 14.724 | 26.281 | 26.459 | 12.578 | 12.813 |
| 57 | 181 | 107.428 | 25.859 | 26.038 | 12.250 | 12.719 |

Internal and external launcher scopes are deliberately separate. Last accepted builds are incremental work sets, not whole-checkpoint totals or paired compiler benchmarks. CP54's cmake-build step was 95.406 seconds and CP57's was 94.422 seconds; receipts establish duration, not whether cache state, source/header breadth, hardware or concurrent agents caused it. No controlled same-workload experiment isolates those causes.

The full runner is stable descriptively around 25.5-27.5 seconds despite registered inventory increasing 165 to 181. Complete pre/post fixture SHA consumes roughly 12.6-13.0 seconds and remains required. All accepted runs retain unchanged fixture hashes, exact folder log SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b, historical 93 SAME / 5 DIFF, six logged-gap passes and one diagnostic-difference test. These are preserved qualifications, not completion evidence.

Two unsuccessful full launchers total 52.344 seconds: CP53 run 1792870514033312400 took 26.297 seconds and returned 8; CP56 run 1792873430196211000 took 26.047 and returned 8. Their required corrected full validations remained necessary. CP56's active PPV flag expectation repair and CP55's earlier compiled-out-assert correction are separate defects; do not conflate them or weaken checks to avoid failures.

## Concrete bottleneck and next change

CP58 supplies direct current failure evidence rather than a speculative build-speed cause:

- resume58-combined-full-build.json occupied 64.552 seconds; its build step failed after 51.453 seconds on the new simctrl_ppv_file_reader test's use of unavailable m2m::qt::cleanPath.
- resume58-reader-test-fix-build.json occupied 56.136 seconds; its build step failed after 43.046 seconds on m2m_p08_emit_a linkage to the new out-of-line m2m::SubProject destructor from project_owned_storage.cpp.

Both source fingerprints were stable, so another source barrier alone would not catch these errors. They are source/API and standalone ownership-link closure failures. The low-risk change is to front-load one grouped root build of the new/changed test targets plus every affected standalone/renamed-namespace ownership consumer before the broad all-target build. For this failure set the new PPV file-reader target and p08 consumer are essential; p09/p11 and other actual consumers must be included when reached by the same out-of-line lifecycle change. Use the existing repeated --target support in admin/build_candidate.py; do not edit CMake or add an experimental transport/mode.

Run affected focused tests with their expected active case counts after that preflight. Then run the normal full build and complete optimized launcher under the same selected-source barrier. This changes failure detection order, not acceptance scope. It preserves all target registration, Release-active assertions, API/architecture checks, fixture hashes, ordered diagnostics/raw differences, source/archive/build binding and original-first boundaries. A focused pass never substitutes for the full freeze.

Measure grouped-preflight configure/build/test wall time, broad retries, source stability and accepted-cycle interval unions in the next comparable ownership change. Grouping itself has setup cost and can compile broad dependencies; the two CP58 failed attempts are not a guaranteed 120.688-second saving. Retain the change only if total accepted work after overhead improves. Do not optimize away full SHA snapshots, introduce hash caching/process sharding, reduce coverage or claim token savings from smaller logs.
