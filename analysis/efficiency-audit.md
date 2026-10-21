# Intermediate efficiency audit â€” 2026-10-21

No current external blocker was found. The application recovery remains active;
archived worker coordination remains paused. The next production task is TNOW.

Four completed recovery windows were measured from bounded local timing metadata:
27m28s, 7m43s, 18m21s and 13m53s. These are different tasks, not a speed comparison.
For the latest UNIT implementation, tool activity occupied 92.9s of 833.3s
(11.1%). Reasoning items occupied 384.9s; 364.7s remained unattributed to measured
items. Intervals can overlap. Unattributed time is not assigned to a cause.
The full regression took 29.19s. Some native REA requests took 85â€“99s in earlier
windows. Faster test execution is therefore not the largest present opportunity.

| Consumer | Adopted improvement |
|---|---|
| Repeated inspection, model activity and context growth | Compact start status, bounded body selection, one concrete uncertainty per investigation; stop redundant re-verification after required checks pass. |
| Incompatible retained-evidence schemas | Source-bound reader for standalone, worker and bundle records; identity and qualification pointers retained. |
| Verbose capture receipts | Optional compact stdout; complete PID/run records and digests stay on disk. |
| Probe retries from field names and string decoding | Review required schema before capture; explicit bounded NUL decoding; available syntax check; no extra native smoke run by default. |
| Audit overhead itself | Three-checkpoint/hourly gate; subsequent reviews bounded to five minutes and one improvement. |

The 13-file prepare-only benchmark reduced capture stdout from 9,348 to 221 bytes
(97.64%). Normalized records, transcripts, limits and hashes were identical. No
original process was launched. This establishes output reduction, not token or
end-to-end time savings. A selected evidence record shrank from 6,624 to 1,431
output bytes while preserving its complete selected body. Three real schemas,
scoped misses, output-budget refusal and overlapping interval accounting passed.

Recurring reviews are enabled in this primary chat. They remain quiet without
new actionable evidence, honor goal pauses, and do not revive workers, weaken
tests, change models/permissions or launch the original solely for benchmarking.
Measure the effect on comparable future work before claiming a total speedup.

Evidence: `analysis/efficiency-baseline.json`,
`analysis/efficiency-tooling-validation.json`, `analysis/efficiency-state.json`.
Operating rules: `analysis/efficiency-policy.md` and project `AGENTS.md`.


## Safe-checkpoint review after TRANS implementation, 2026-10-21

The last completed TRANS research turn took17m58s. Its tool interval union was
280.5s, observed reasoning189.7s and unattributed time626.3s; no cause is assigned
to the unattributed portion. Its largest measured tool item was a123.7s native
analysis request. These are different work units from UNIT, not a speed benchmark.
The bounded report separates100,933 uncached input,7,881,856 cached input and
20,337 output tokens for that research window; these are ledger deltas, not a
billing estimate. Current implementation-turn timing remains outside that report.

The current checkpoint exposed a more actionable retry cause: two new runtime
cohorts reproduced matching error/zero states but did not exercise positive grain
sizing. Their harness elapsed totals58.719s include gate waits; they are retained
as error cases. Corrected cohorts use the recovered MTLGRP grammar, explicit
wrapper success and expected nonzero component widths. No C++ candidate change
was needed. The one new workflow improvement is an explicit positive-effect gate.
A0.306s negative check against the retained zero-width capture confirms the new
guard rejects a false positive claim without launching the original again.

All103 regression tests passed in53.95s. Earlier runs took29.19s and76.79s with
different counts/load. No total wall-time or token reduction is established yet.
Keep warm-session dependency questions bounded and reuse saved source bodies;
new speed proposals remain subordinate to evidence and fidelity.

Evidence: `analysis/efficiency-review-20261021-trans.json`,
`analysis/trans-coverage-guard-validation.json`, `analysis/trans-import-contract.json`.
