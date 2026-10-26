# CP202 synthetic visibility fixture lifecycle acceptance

Scope: independent, converter-only source and retained-report review. No original executable, GUI or GUI-control session was used. This note defines the lifecycle evidence required to call a future synthetic fixture run complete and compares that order with the separate original runner. It does not retroactively upgrade either retained fixture attempt.

## Exact fixture lifecycle acceptance criteria

A synthetic fixture run may report a full lifecycle PASS only when all of the following are recorded for the same owned child and same Frida session:

1. The fixture-specific checks pass: exact synthetic code prefix, baseline/hooked/restored calls for both requested values, matching hook observations, listener detach, and exact restored bytes. These establish the fixture behavior only.
2. Child cleanup is performed through the Python helper's retained creation handle. The helper receipt must establish termination API success, `wait_result == 0`, verified exit, closed handles, and helper exit code 0. A PID-only termination request does not qualify.
3. Final teardown uses the Frida 17.23.2 method contract: test `typeof session.isDetached === 'function'`, call `session.isDetached()`, and validate that its return is boolean. Never branch on the method object as a boolean property.
4. After verified child termination, if the session remains attached, attempt bounded script unload and record its successful completion. If unload fails or times out, preserve the error and uncertain outcome. A `Promise.race` timeout does not cancel or prove failure/success of the underlying operation.
5. If the session is already detached after verified child exit, script cleanup may be classified as `not_applicable_session_already_detached_after_target_exit`. This is a separate terminal basis: it does not claim `script.unload()` succeeded. Do not call unload on an already detached session or reinterpret `Script is destroyed` as unload success. If that call was already attempted and rejected, preserve the error and leave the result incomplete.
6. If the session remains attached after unload handling, attempt bounded detach. A successful detach call alone is insufficient: call `session.isDetached()` afterward and require `true`. Record a verified already-detached state as a distinct valid basis.
7. Only after teardown attempts, query the same PID independently through Windows process inspection and the same Frida device. Require both queries to succeed and report no matching PID. Preserve query errors as errors, not as absence.
8. Compute the full-lifecycle outcome from fixture checks, verified retained-handle exit, attach settlement, a valid script-cleanup basis (successful unload or the explicit target-exit/detached basis), verified session detachment, zero unresolved teardown errors, and both successful absence observations. A timeout, rejected operation, missing state receipt, or absent verification is `FAIL / incomplete`, not PASS. Keep the timeout and eventual evidence distinct if an operation later settles.

The child may be terminated before unload/detach because it is deliberately held suspended and independently owned. Termination is not a substitute for Frida lifecycle evidence: a PASS using the target-exit basis must explicitly establish that the session is detached after verified child exit, and must not claim explicit script unload. Conversely, successful process absence does not prove either Frida operation settled. Listener detach and exact memory restoration are also separate from script unload and session detach.

## Current fixture evidence and repair target

The retained attempt 1 timed out script unload at 2 seconds; attempt 2 timed out session detach at 5 seconds before helper termination. Their underlying promises' eventual states are not established. Both have positive retained-handle child-exit and independent Windows/Frida absence evidence, which remains useful but cannot satisfy the full lifecycle gate.

The current fixture runner has two concrete lifecycle defects: it awaits session detach before terminating the held child, and its final guards test `!session.isDetached` rather than invoke `session.isDetached()`. Since the method value is a truthy function object, those guards can skip final unload/detach. A repaired run must retain both historical records, write new evidence separately, use the method contract, and follow the termination-first order above. A report should not convert old failed outcomes into a pass.

## Original runner cleanup-order comparison

`admin/frida_original_save_probe.mjs` is safely distinct in the relevant cleanup structure. It first asks the independent helper to terminate via its retained handle and collects helper exit; then settles a pending/late attach; validates the held-handle exit receipt; attempts script unload when a session remains attached; calls `settleSessionCleanup`, which checks `session.isDetached()` before and after bounded detach; and only afterward takes fresh same-device and Windows absence observations. Its `processAbsenceVerified` combines successful absence with attach settlement, session cleanup and verified helper exit, rather than treating absence alone as cleanup.

The original runner therefore does not need to copy the synthetic runner's broken property guards or its detach-before-termination ordering. This is a source-order finding, not proof that a future original-runner invocation will settle successfully. Its bounded timeouts still represent uncertain outcomes and must leave its acceptance result false unless later explicit settlement evidence satisfies the gates. No original-runner launch or runtime action was performed for this review.

## Re-review of repaired lifecycle run

The follow-up repair now makes the Windows absence observation auditable rather than treating empty command output as absence. `localPresence()` requires nonempty JSON with schema `owned-pid-presence-v1`, `querySucceeded: true`, the exact owned PID, boolean `present`, a process list of at most one matching row, and agreement between `present` and the row count. PowerShell uses terminating-error behavior; command failure, empty output, invalid JSON, or a contradictory envelope becomes a verification error. The final PASS gate requires the validated `present: false` envelope and an independent same-device Frida result of null.

The latest synthetic run at `imports/claude-moproj2mst/analysis/resume202-visibility-fixture-lifecycle-2026-10-26T04-42-53.880Z.json` records `{schema: "owned-pid-presence-v1", querySucceeded: true, pid: 24700, present: false, processes: []}`, Frida absence as null, verified retained-handle exit and closed handles, method-based `isDetached() === true`, script cleanup explicitly not applicable after target exit, and zero failures. It satisfies the bounded synthetic lifecycle gate under the distinct target-exit/detached basis; it does not establish successful explicit `script.unload()` or any original-application/GUI behavior. Reviewed runner SHA-256: `1dea29ea9d12e9024e9095f6f53d1c978eb74547bc8a7894cbd67fb28e4bf40c`. Raw receipt SHA-256: `48ec5f0d1477a7f51469207591b752000fcc33139f50ec079204869306b9c472`.

