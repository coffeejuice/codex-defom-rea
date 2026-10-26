# CP173–175 converter efficiency review

## Scope and frozen evidence

This read-only review covers the parallel connector/editor300 lanes, preflight/build receipts, focused gates and full source-stable suites for CP173–175. Frozen snapshots are 3,554 files each; their source-tree hashes are recorded in the checkpoint validation receipts. Final checkpoint validations report 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failures/skips/incomplete outputs/fixture changes, and no changed logged diagnostics. Historical oracle differences remain qualified; the freeze is not fresh original-session acceptance.

## Build and launcher timing

| Checkpoint / candidate | Build workflow | Focused compile | Focused tests in build workflow | Full build phase | Full CTest | Full launcher |
|---|---:|---:|---:|---:|---:|---:|
| CP173 dual-lane | 71.812 s | 13.640 s | 2.501 s across 3 targets | 53.406 s | 15.641 s | 32.375 s |
| CP174 first dual-lane attempt | 11.016 s (stopped at failed focused test) | 6.454 s | 2.265 s across 3 targets, last failed | — | — | — |
| CP174 props-fix | 38.251 s | 2.734 s | 3.172 s across 4 targets | 30.063 s | 15.562 s | 32.719 s |
| CP175 dual-lane | 52.046 s | 5.735 s | 14.249 s across 7 targets | 29.750 s | — | — |
| CP175 null-step fixture/final incremental | 13.313 s | 4.063 s | 6.203 s across 2 targets | 0.797 s incremental | 16.063 s | 32.875 s |

Successful final full launchers total 97.969 seconds across CP173–175; their raw CTest portions total 47.266 seconds. The remaining recorded launcher phases cover inventory, fixture hashes, source/binary binding, JUnit parsing and report output. The three full launcher times are close (32.375–32.875 s). They do not measure total task wall time or the work in parallel lanes.

Build workflow totals including the CP174 failed attempt are 71.812 s for CP173, 49.267 s for CP174, and 65.359 s for CP175. Their sum, 186.438 s, is a ledger sum across sequential receipts and source snapshots; it is not wall-clock duration because work was organized into dual lanes and incremental builds. CP173’s 53.406 s full build was unusually high relative to CP174/175 full build phases (30.063/29.750 s); the receipts do not identify the cause. CP175’s 0.797 s final build is incremental and cannot be compared to a full build.

## Rework and fixture qualification

CP174’s first focused run ended with one failing focused target after 0.812 s; a props-fix build and a new four-target focused run followed, then the full suite passed. The failed test output does not identify the specific assertion in the compact receipt, so this review does not infer a defect class from it.

CP175’s lane notes record a coverage preflight finding: the focused Save tests had not established the admitted nullable-step branch. A narrow internal fixture was added, without inventing a public Save path. That fixture’s focused test took 5.468 s and passed; the separate null-step focused build workflow totaled 13.313 s before the frozen full suite. This is a test-coverage/rework cost, not a failed build or failed fixture execution. Final suite receipts report no fixture changes during the run. No standalone failed-fixture execution receipt appears in the CP173–175 receipts inspected; the observable failure in CP174 is a focused runtime test failure.

This distinction matters for comparing rework against tests: the full suite launcher costs about 33 seconds, whereas the CP174 failing focused target itself took under one second (plus its prior compile and other focused checks), and the CP175 added fixture’s own test took about 5.5 seconds. The receipts do not capture review time or lane overlap, so they cannot establish whether a fixture-preflight-first process would reduce total elapsed wall time.

## One improvement to measure

Before implementation is frozen, add an explicit focused coverage checklist mapping each new early-return or nullable branch to an executable test and its observable no-effect assertions. For CP175, this would surface the missing admitted-null-step fixture during review before the first focused pass, reducing the chance of a later fixture addendum and another compile/focused cycle. Preserve the source review, exact branch qualification, no-effect snapshots, original/fidelity constraints, focused gates, full regression and freeze sequence. This is a plausible reduction in rework, not a measured time or token saving; the available receipts cannot quantify the review work avoided.

## Conclusion

All three source-stable final suites passed with the same qualified 354/6/1 classification. Full launcher cost remained stable around 32.4–32.9 seconds. Build times varied more, and the receipts do not support a total wall-time comparison for parallel lanes. The CP174 focused failure and CP175 coverage finding were followed by repair and successful frozen gates. The proposed branch-to-test checklist is the one concrete process change to measure next; it does not remove any fidelity or freeze gate. No token or end-to-end speedup claim is supported.
