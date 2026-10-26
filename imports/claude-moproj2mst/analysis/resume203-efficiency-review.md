# CP203 converter efficiency review

## Measured comparison

CP203 adds no frozen candidate build or CTest receipt after CP201. Its converter work was source review, static audits, two original-app crash observations carried forward from CP202, and a bounded no-crash Frida registration check. Accordingly there is no new build or full-suite speed point, and no support for a CP202-to-CP203 suite-speed claim.

| Measure | CP202 / prior baseline | CP203 evidence | Interpretation |
|---|---:|---:|---|
| Full CTest | CP200 16.875 s; CP201 17.046 s | No new receipt | Prior full suite remains about 17 s; CP201 delta was +0.171 s. |
| Focused CTest | CP200 3.094 s; CP201 3.234 s (command sums) | No new receipt | Not comparable to a new run; the existing sums are not wall-clock suite measurements. |
| No-crash exception-handler preflight | Not available in CP202 | 1.200 s (05:29:20.582Z–05:29:21.782Z) | Handler registration succeeded; owned-child termination, handle closure, detach and independent Windows/Frida absence all passed. It intentionally triggered no exception, so it says nothing about callback invocation or converter behavior. |

The CP203 no-crash receipt's measured start-to-finish elapsed time is **1.200 s** (05:29:20.582Z–05:29:21.782Z). This is the cost of this one preflight on this host, not a general native-run cost estimate. A separate deliberate-exception preflight stopped before child creation on `EPERM`; the exact elevated command was rejected by automatic review. No runtime cost or result exists for that blocked attempt. CP202's original-app captures likewise lack a comparable total preparation/runtime receipt: attempt 1 stopped pre-resume on hook installation; attempt 2 and the no-Frida baseline both ended in QtCore4_64.dll access violations at RVA/offset `0x32c0`. These are incomplete original-app observations, not successful converter acceptance runs.

## Rework and parallel work

CP202 documents specific preflight/source-review rework: the original attempt-1 runner treated `Session.isDetached` as a property instead of calling the method and did not fail closed on an empty Windows CIM result. The correction required a receipt repair and independent exact-PID checks; the 27 disposable fixture hashes remained unchanged. CP203's original-runner receipt review found that raw helper-receipt write failure did not affect the already-computed `captureComplete`; this was fixed, then reviewed again with the final runner hash. The CP203 no-crash runner also needed a static argv normalization correction before its review passed. These are concrete repeated review/fix cycles, but the artifacts provide no active-work duration per cycle, so elapsed cost or savings cannot be assigned.

Parallel static audits covered multiple separate source questions and the queue reconciliation names their outcomes: they found no source-ready public-behavior increment, and kept the relevant workstreams gated on missing occurrence evidence. This is useful coverage of independent questions, but no serial counterfactual, token telemetry, or comparable wall-time receipt exists. Parallelism therefore cannot be credited with a speedup from these records.

## Next efficiency action

Keep the existing full CTest and all fidelity gates. For the next converter checkpoint, collect a compact, source-bound timing receipt for the unchanged full suite and any focused targets actually run; separately time preflight, build, and test phases. For probe preparation, add a static/mock contract check for receipt-write failure and Windows/Frida absence parsing before any owned-process run, since the CP202/203 rework arose at these API/receipt boundaries. This is a measured process improvement candidate, not a reason to reduce independent review or evidence retention. Follow the already queued static QtCore fault/CLI analysis; do not rerun the crashing original or enable the deliberately crashing child path without the pending authorization/review route.

No x10, token-savings, or end-to-end speed claim is supported. No test, source edit, integrity check, cleanup proof, or review gate should be removed for speed.

## Evidence

`analysis/resume202-efficiency-review.md`; `analysis/resume202-original-probe-attempt1.md`; `analysis/resume202-original-probe-attempt2-crash.md`; `analysis/resume202-original-probe-attempt2-cleanup.md`; `analysis/resume202-uninstrumented-baseline-plan.md`; `analysis/resume202-baseline-crash-evidence.md`; `analysis/resume203-exception-handler-no-crash-preflight.md`; `analysis/resume203-exception-handler-no-crash-review.md`; `analysis/resume203-exception-handler-preflight.md`; `analysis/resume203-preflight-approval-block.md`; `analysis/resume203-original-runner-receipt-repair.md`; `analysis/resume203-original-runner-receipt-review.md`; `analysis/resume203-queue-reconciliation.md`; and `analysis/resume203-original-launch-context-audit.md`.


