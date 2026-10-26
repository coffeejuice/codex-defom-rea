# CP240 remesh-v2 preparation timeout flake audit

## Finding

The failure is a test scheduling race, not evidence of a production regression. In `remesh_v2_preparation_failure_is_before_launch_and_inspection_failure_is_sticky`, the last subcase configures a 50 ms end-to-end deadline, enters the preparation callback, sleeps 60 ms, then expects `childCreated == false`, `stopReason == Timeout`, and no inspection. That intention is valid, but it has almost no scheduling margin: the supervisor starts its deadline before image hashing and scratch setup, then later checks the elapsed time after the callback. Under a parallel Windows suite, scheduler pauses can let the deadline expire before the callback; the supervisor then continues, and its post-callback timeout check prevents launch. More importantly, an overrun callback may return after the deadline, but the supervisor's failed/prepared state behavior and inspection rules can produce an already-created child or inspection under races if the deadline is crossed after the check but before creation. The two failed assertions (`!childCreated`, `!state->inspected`) are consistent with the run taking the boundary path into child/inspection rather than the expected pre-launch timeout.

## Evidence

- Failing run: `build/test-runs/1793037803632603400`. `m2m.mdt.terminal_harness_remesh_v2.log` reports only lines 310 and 311 failed; the other nine cases in that executable pass. The full suite ran 369 tests with CTest parallelism; this target took 2.24 seconds. The launcher phases show 18.078 seconds in CTest overall.
- Source: `test_terminal_harness_remesh_v2.cpp` lines 302–312 sets a 50 ms deadline, sleeps 60 ms in preparation, then asserts the child was never created and the lease was not inspected.
- Supervisor source: `terminal_harness_supervisor.cpp` starts `began` before setup, executes `prepareOwnedCase`, creates/binds the nonce, then checks `GetTickCount64() - began >= deadlineMs` immediately before preparing/creating the child. Inspection is intentionally called after a stopped child.
- Prior receipts: the same case passed in 100+ retained `build/test-runs/*/m2m.mdt.terminal_harness_remesh_v2.log` receipts, including 1793037124841593300 immediately before this failure. The latest prior failure before this one was also 179291125; all subsequent retained receipts through 179303712 pass.

## Minimal correction

Keep this test-only and preserve the assertion that an actual preparation overrun cannot launch or inspect a child. Give the subcase a comfortably larger deadline and sleep beyond it, e.g. deadline 500 ms and callback sleep 600 ms. This retains the same end-to-end deadline contract and avoids a 50 ms threshold being consumed by unrelated setup or parallel scheduling. If a long sleep is undesirable, a deterministic test seam around the clock would be stronger but is a wider change. Do not weaken the assertions or alter supervisor behavior based on this single timing failure.

## Qualification

The failure establishes nondeterminism in the test's timing setup, not proof that every production timeout ordering is correct. The supervisor has an unavoidable small interval between its final deadline check and child creation; if the contract requires strict no-launch after deadline under arbitrary scheduler suspension, that would require a production-side atomic/deadline guard at the launch boundary. Current evidence does not isolate such a production defect: the test's callback deliberately overshoots by only 10 ms against a 50 ms budget, and the case passes consistently in many prior parallel-suite receipts.

## Review of implemented correction — PASS

The current diff changes only this test subcase from a 50 ms deadline / 60 ms sleep to a 500 ms deadline / 600 ms sleep. It preserves `!result.childCreated`, `StopReason::Timeout`, timeout disposition, lease destruction, and `!state->inspected`. The callback therefore still deliberately runs past the configured end-to-end deadline, while a 100 ms overrun gives ample margin over Windows timer granularity and ordinary parallel scheduling variation. The supervisor's deadline check following callback/nonce binding and its second check just before `CreateProcessW` remain unchanged; this correction adds no production behavior change. Static review passes. No build or rerun was performed as requested, so runtime confirmation remains for the owning agent.
