# CP104 opt-in isolated remesh terminal harness

Implement a separate candidate-only supervisor/worker probe CLI. Leave the existing moproj2mst CLI, ABI1 exports, M2M enum values and legacy failed-remesh-open refusal unchanged. Only an explicitly launched owned worker receives a private terminal execution service. This is the smallest executable experiment that tests process status and no-unwind placement without pretending to recover the original default-handler prelude.

This design continues `resume103-remesh-terminal-outcome-architecture.md`, SHA-256 `cc474b1ab6203d2196ae5e79010bb84a6785485125c60a3773c09ec683a53f24`. Native default-handler/status evidence remains the CP103 retained startup contract; no further native execution or GUI work is required for this candidate experiment.

## Chosen outward contract

Use a dedicated worker target outside the automatic ordinary Testkit source glob, for example `tests/tools/remesh_terminal_worker.cpp`, plus a parent script `admin/test_remesh_terminal_route.py`. A proposed explicit invocation is:

    python admin/test_remesh_terminal_route.py --worker <absolute-candidate-worker.exe> --expected-sha256 <build-receipt-hash> --scratch <absolute-build-scratch> --allow-owned-worker-termination

These are proposed new tool arguments, not commands executed during this task. There is no new product CLI flag, environment switch or public C ABI setting. The parent refuses to start terminal cases without the explicit opt-in, verified candidate executable hash, separate worker role and contained scratch. It must never discover or launch the original executable.

Define the parent result as a tagged process observation:

| Disposition | Required evidence |
| --- | --- |
| Returned | Complete Returned record plus matching ordinary M2M process exit0..7 |
| GuardedTerminalPrediction | Returned7 plus a separately labeled prediction; no endpoint executed |
| CandidateProcessTerminal | Valid reached-boundary record plus exact child status0xC0000417 and no Returned record |
| HarnessFailure | Launch/protocol/backend/profile/status mismatch |
| Timeout | Bounded wait expired; parent stopped its own child |

For the CLI harness, use parent exit0 for a matched expected case/suite and nonzero for failed validation. Store the observed child disposition/status in the receipt. Parent exit0 means the harness assertion passed, never “conversion returned M2M_OK.” The receipt retains unsigned child status3221226519 and normalized hexC0000417 (signed wrappers may expose -1073740777). Do not relay this as an ordinary M2M return or silently replace it with7.

The legacy wide/UTF8 entry points continue calling convert with no private services. On the same denied-write fixture they return7 and normal message behavior; neither is terminal-capable in this increment. The new worker calls the existing source-private detailed conversion entry with services. Its terminal result is therefore a private-core/isolated-CLI experiment, not native termination through the public C ABI.

## Smallest production-facing seam

Add a source-private value contract and borrowed execution service; do not change public Result, ConversionDetails or RemeshSaveReport layouts merely to transport an outcome that cannot return on the terminal branch. The private service is nullable in ConversionCallServices. Default/null remains today's behavior.

The private descriptor contains source site0abcf0:first-null-FILE-fprintf, fresh-default-handler qualification identifier, EINVAL22/0x16, expected process statusC0000417, format attempt ordinal1 and the completed project/MST SaveCallState snapshot. It distinguishes reached intent from observed process death. It never marks remesh/Save returned, normal Close entered, or returnedCode.

Root supplies the snapshot from the live saved.call at the existing after-MST remesh call. A synchronous private wrapper can capture that reference and forward the descriptor to the worker endpoint; no pointer escapes the conversion. The worker service owns protocol publication and the fixed process endpoint. Keep IPC and Win32 child supervision out of the remesh writer and public headers.

At failed enabled write-open, the private writer route sets modeled errnoEINVAL and dispatches the service at the first invalid-parameter boundary while writer/session scopes remain live. Dispatch must occur inside the writer, before returning its report or throwing. Successful open keeps the existing stack CoggingFile and31-format-call body. A null/default service still returns the current guarded report. The ordinary converter's writer-false mapping remains after this point and cannot supersede an admitted terminal dispatch.

The endpoint sends its bounded receipt and then performs the chosen fixed no-unwind process endpoint. It never returns into Save. Do not implement it by throwing, exit(), abort(), std::terminate(), null-FILE host fprintf or normal API return. If endpoint execution unexpectedly fails/returns, publish a distinct harness-failure observation if possible and never continue the writer. The parent watchdog owns bounded child shutdown; that failure is not accepted as source-terminal behavior.

This endpoint intentionally omits native context capture, debugger/filter calls, SetUnhandledExceptionFilter(NULL), UnhandledExceptionFilter/WER behavior and the recorded module debug-flag write. The receipt must say `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`. The native default route remains a source prediction. Fixed status/no-unwind agreement is not full handler reconstruction.

## Ownership and protocol

Create the owned child suspended, assign its job/process lifetime, restrict inherited handles to the dedicated protocol/control handles, then resume. Do not inherit parent fixture-denial handles. Parent holds explicit process/job handles and never kills by image name. All project copies, controls, logs and receipts stay under a unique build scratch run. Preserve failed-run evidence and release parent locks after the child has stopped.

Use a dedicated pipe, separate from stdout/stderr. Fixed bounded records need magic/version, per-run nonce, record kind, source-site enum, expected status, errno and SaveCallState flags. Permit only the expected small transcript: Started followed by either Returned or TerminalBoundary. Reject incomplete records, excessive records, stale nonce/version, conflicting terminal/returned records or wrong source site. A TerminalBoundary message proves reach only; statusC0000417 without that message is an unclassified crash. Require both for CandidateProcessTerminal.

The parent waits at most20 seconds per child and records measured wall time, all binary hashes, command, source/build receipt identity, protocol, normalized exit status and output manifests. Timeout cleanup must stay limited to its owned child. Never rely on buffered stdio or postmortem ConversionDetails. Hash the executable before/after; root supplies the expected build-receipt hash and binds the final source freeze. Unknown/custom-handler profiles remain guarded and cannot receive the fixed endpoint.

## Deterministic fixture and bounded tests

Use a disposable known project whose primary LOCAL_REMESH.DAT is not rewritten earlier in Open/Save. Seed a valid KEYWORD_DRIVEN file with supported fields and DONE so Open establishes enabled=true. Parent opens it for read with read sharing but denies write sharing; native Open can read it, while later write-open fails before truncation. This avoids a new timing barrier. Validate that assumption against the chosen copied project; do not seed an invalid sentinel that changes the Open route.

The first focused increment should contain these six bounded cases:

1. Pure descriptor/profile test: fresh-default failed enabled open classifies terminal intent; unknown/custom profiles remain guarded. No process termination or host handler installation.
2. Owned-worker unblocked control: conversion returns normally, with ordinary result/status and expected output artifacts. Existing31-attempt post-open-failure tests remain part of required regression.
3. Owned-worker denied remesh: actual Open/Save reaches the first invalid printf boundary, receipt records earlier project/MST returned attempts, and child stopsC0000417. Remesh sentinel is unchanged under this particular sharing denial; prior completed files remain. No Returned or post-call/normal-Close marker may appear.
4. Same denied remesh with earlier deterministic project/MST returned failures: require the terminal endpoint rather than adapter write-error mapping, and retain the corresponding earlier-state/output assertions. Use physical locks only where read compatibility is proven; otherwise reuse the existing private project-output service and label that case core-only.
5. Legacy public wide C ABI on denied remesh: normal returned7/message, preserving today's API behavior.
6. Legacy public UTF8 C ABI on denied remesh: same compatibility boundary, independently exercised.

Each process case runs in a child so an accidental endpoint cannot kill the supervisor/Testkit runner. A fresh private descriptor-classifier test is sufficient for unknown handler rejection; do not execute a fabricated returning handler. Tests of the parent classifier also need synthetic malformed/truncated/stale records, missing boundary with terminal status, and boundary with ordinary exit; these are protocol tests with no conversion execution.

The no-unwind claim combines dispatch placement before scope exit with actual endpoint/status and absent post-call markers. An absent marker alone cannot prove Session destructor absence. If empirical Session-destruction observation is required, reserve a separate reviewed private lifetime probe; do not add public hooks or claim complete normal Close parity. Current converter still lacks full accepted Close replay.

## Increment boundary and exclusive files

Root first agrees the private descriptor/service signature, profile admission and protocol. One worker may then own a new private terminal contract/backend and dedicated worker source; editor300 owns the remesh private failed-open dispatch. Root owns convert.cpp, conversion_call_services.hpp, parent harness, CMake registration and integration. The old C ABI header/implementation and product main require no functional change. Preserve their export/API-version/status behavior with focused compatibility checks.

The smallest accepted result is one source-qualified failed-open candidate endpoint, its controls/compatibility cases, focused receipt, complete existing suite and source-stable freeze. Register the parent harness with a trusted pretest build-receipt hash supplied by root; do not weaken hash binding or patch the shared launcher ad hoc. It is not a product isolated-conversion API, default-handler-prelude port, custom-handler recovery or global lifecycle completion. Leave ordinary callers on their explicit guard throughout.

Only this report was written. No source/test edit, build/test execution, original/native target execution, GUI or REA mutation occurred.
