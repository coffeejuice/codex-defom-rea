# CP185–187 converter efficiency review

## Scope and outcome

Read-only review of CP185–187 final validation/build receipts, failed dual focused attempts and CP184 efficiency review. The three successful final receipts each report 354 passes, six passed-with-logged-gaps, one passed-with-logged-differences, and zero failed/skipped/incomplete-output cases. These remain synthetic/historical regression evidence; no fresh original-session acceptance is claimed. CP187 dual focused attempt failed at `m2m.s.save_current`; its `source_stable_during_build` flag is false. The final CP187 meshfix receipt subsequently reports a stable successful build/test sequence.

## Measured timings

| Checkpoint/attempt | Configure | Focused preflight | Focused CTests | Build | Full suite | Recorded build workflow |
|---|---:|---:|---:|---:|---:|---:|
| CP185 stopfix successful | 2.234 s | 5.891 s | 13.796 s | 29.766 s | 16.594 s | 65.883 s |
| CP185 dual failed | 2.329 s | 7.750 s | first CTest 0.750 s, exit 8 | — | — | 16.513 s |
| CP186 guardfix successful | 2.234 s | 6.203 s | 13.655 s | 29.859 s | 16.469 s | 65.795 s |
| CP186 dual failed | 2.500 s | 8.171 s | first CTest 0.734 s, exit 8 | — | — | 17.217 s |
| CP187 meshfix successful | 2.250 s | 3.610 s | 13.796 s | 29.891 s | 16.469 s | 63.744 s |
| CP187 dual failed | 2.328 s | 8.922 s | first two CTests 1.656 s; third 5.500 s, exit 8 | — | — | 27.222 s |

Successful workflow figures sum receipt step elapsed values and are not end-to-end task wall time. Asterisk: full-suite receipts are detailed separately; review only claims the recorded suite result, not independently timed final launcher duration where the build receipt does not expose it. CP185/186 dual failures each stop at the first focused test; CP187 runs two tests successfully before the save-current test fails. CP187's failed attempt also records 17 source-fingerprint calls consuming 12.420 s and `source_stable_during_build: false`; these are explicit measured receipt costs, not an attribution of all failed-attempt time.

The repeated failure pattern costs extra configure/preflight plus failed focused execution before a successful final attempt: CP185/186 each have roughly 16.5–17.2 s recorded failed workflow, and CP187 27.222 s. Across these checkpoints, the receipts show repeated failed `m2m.s.save_current` focused gates followed by successful final validations. This suggests stale test executables or fixture state may be involved, but the receipts alone do not prove that diagnosis or identify the exact state transition. Do not treat the sum as total recovery time.

## One safe optimization

At the failed `m2m.s.save_current` gate, retain a compact failure classification receipt that records the test case, expected/observed result, executable/source hashes, fixture input hash, and whether the focused target was rebuilt immediately before execution. Rebuild or reset only the implicated target/fixture when that evidence shows stale state, then rerun that focused gate before the broader suite. This adds no bypass to the acceptance gates and avoids blind whole-phase retries. Keep all original-first comparisons, source/file stability checks, frozen fixtures and full regression. Measure this targeted triage on CP188 before claiming a time improvement.

Next efficiency review: after CP190 or one hour, at the next safe checkpoint. No total-speed or token-saving claim is supported by these receipts; output compaction is not a proxy for either.

## Evidence

`analysis/resume185-stopfix-build.json`, `resume185-stopfix-tests.json`, `resume185-dual-build.json`, `resume186-guardfix-build.json`, `resume186-guardfix-tests.json`, `resume186-dual-build.json`, `resume187-meshfix-build.json`, `resume187-meshfix-tests.json`, `resume187-dual-build.json`, `wave3-checkpoint185-validation.json`, `wave3-checkpoint186-validation.json`, `wave3-checkpoint187-validation.json`, and `resume184-efficiency-review.md`. No source, plan, state, build, GUI or REA database files were changed.

