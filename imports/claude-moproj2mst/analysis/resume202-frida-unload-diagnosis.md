# CP202 Frida unload/detach diagnosis

Scope: read-only converter-side diagnosis of the synthetic CP202 fixture lifecycle. No process was launched, no GUI was used, and no original executable was touched. This note supplements `analysis/resume202-fixture-lifecycle-acceptance.md`; it does not upgrade either retained attempt.

## Evidence checked

- The retained acceptance note records unload timeout at 2 seconds in attempt 1 and detach timeout at 5 seconds in attempt 2. Attempt 2 requested detach before the independent helper terminated the deliberately held child. The promises' eventual outcomes were not recorded. Windows and Frida process-absence observations and retained-handle child-exit evidence are useful, but do not establish unload/detach completion.
- The same note identifies a synthetic-runner guard defect: `!session.isDetached` tests the truthy method object rather than calling `session.isDetached()`. That can skip cleanup guards. The original runner's recorded order is kill via retained helper handle, settle attach, validate exit, unload if still attached, bounded detach with before/after `isDetached()` checks, then independent absence checks.
- `tools/capture_startup.py` confirms the general helper pattern: `CreateProcessW` with `CREATE_SUSPENDED` and a retained primary-thread/process handle; resume only after a gate; cleanup must be done against the retained creation handle. This establishes the ownership model, not Frida behavior.
- A local search found no installed `frida`/`frida-tools` Python package, no global `frida` CLI, and no Frida package/types/source in the searched local project and user plugin paths. Therefore this checkout cannot substantiate a Frida 17.23.2 implementation-level claim about why either API call blocks. The method-call contract and observed timeouts above come from retained project evidence, not a fresh local SDK source inspection.

## What can explain the hang (hypotheses, not findings)

The observed order makes the externally suspended target state a plausible contributor, but does not prove it is the cause. `script.unload()` and `session.detach()` are asynchronous teardown operations; depending on the Windows agent/injector path, they may need target-side agent work, thread coordination, or completion of pending attach/load activity. A child whose only primary thread remains suspended may not make progress through any target-side work that depends on that thread. Detach may also wait on teardown work left outstanding by the earlier script operation. These are plausible mechanisms only: without the matching Frida 17.23.2 source or a controlled runtime trace, do not attribute the timeouts to a particular Frida internal or claim that a suspended thread necessarily blocks either operation.

The evidence already shows two concrete confounders independent of that hypothesis: detach was requested before target termination in the synthetic attempt, and method-object truthiness could bypass state-aware cleanup. The timeouts also arose from bounded wrapper waits. A `Promise.race` timeout only bounds how long the caller waits; unless the operation itself supports cancellation and cancellation is confirmed, it says nothing about whether the Frida promise later fulfilled or rejected. Process absence is not an operation-settlement receipt.

## Supported bounded cleanup sequence

For a future synthetic run, preserve the original runner's kill-first ownership discipline and keep the fixture acceptance criteria:

1. Stop fixture activity and detach the fixture listener; record its result separately from script unload.
2. Ask the helper that owns the retained `CreateProcessW` handles to terminate the child. Require termination API success, `wait_result == 0`, verified exit, closed handles, and helper exit code 0. Do not replace this with PID-only termination.
3. Settle any pending or late Frida attach/load operation with a bound and retain its settlement receipt. A timeout remains unresolved even if independent process checks later show absence.
4. Check `typeof session.isDetached === 'function'`, call it, and require a boolean. If the session is already detached after verified target exit, record the explicit `not_applicable_session_already_detached_after_target_exit` script-cleanup basis. Do not call unload on a detached session or count `Script is destroyed` as successful unload.
5. If still attached, attempt bounded `script.unload()` and record fulfillment/rejection. A timeout leaves the underlying result unknown. Then recheck `session.isDetached()`.
6. If still attached after unload handling, attempt bounded `session.detach()`, then call `session.isDetached()` and require `true`. A detach call that fulfilled without this verified state is insufficient. Also accept an already-detached state as its own explicit basis.
7. Only after teardown attempts, query the same PID independently through Windows and the same Frida device. Require successful queries that report no PID; record query errors as errors.
8. Report full lifecycle PASS only if fixture checks pass, helper exit is verified, attach is settled, script cleanup has one valid basis, detachment is verified, no teardown errors/timeouts remain unresolved, and both absence observations succeed. If a timed-out operation later settles, retain both the timeout and later settlement as separate events; do not erase the original uncertain receipt.

The runner should attach rejection handlers immediately to each operation and retain the operation's eventual result when available, while applying a separate deadline to the orchestration. Do not start a second unload/detach concurrently merely because the first caller-side wait timed out; first preserve/observe the original operation's settlement or leave the run incomplete. This avoids turning a timeout into overlapping, hard-to-interpret teardown attempts.

## Limitation

No fresh Frida 17.23.2 SDK documentation, type declarations, source, or runtime reproduction was available in the searched local installation paths. The causal explanation above is explicitly a hypothesis. The actionable conclusion is narrower: kill-first cleanup is supported by the existing runner and retained-handle contract, fixes the known ordering issue, and allows Frida teardown to be assessed after target exit without claiming that it guarantees those APIs will settle.
