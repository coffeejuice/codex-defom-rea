# CP105 independent challenge of the isolated terminal design

CP104's isolation boundary is appropriate: retain ABI1/product CLI behavior and give only an explicitly owned candidate worker the private endpoint. Its current design is not ready for terminal activation until deadline, cleanup and backend-return rules are concrete. These are future activation blockers, not findings against the currently guarded frozen converter. No new original-behavior claim follows from this audit.

Reviewed CP104 design SHA-256 `400e282d2d5f7cdab1c8421b21c53d0c7f38eeb0dd5646b46c483f2cb71388b2`, CP103 architecture SHA-256 `cc474b1ab6203d2196ae5e79010bb84a6785485125c60a3773c09ec683a53f24`, and editor300's CP105 implementation-scope report SHA-256 `2f92c1302e8100d2ad7a7634ca609407564c4e607c3cc99a29ea0a3d19fdd663`. The latter correctly keeps public report/signature unchanged, keeps SaveCallState out of the leaf descriptor and rejects a returning fake as a Testkit shortcut.

## Activation blockers

**P1: the deadline must cover every operation after launch.** A20-second WaitForSingleObject after blocking ReadFile is not a20-second harness. A child that writes half a record and stalls can block the parent before the wait. Redirect stdout/stderr to bounded-owned files or drain them concurrently; do not let a full capture pipe block the child before its terminal event. Use a single monotonic deadline over start acknowledgement, framed protocol reads and process exit. Overlapped/event-driven reads or a cancellable reader with bounded shutdown are acceptable; a reader thread with an unbounded join is not. Bound receipt/file reads too.

**P1: supervisor failure must outrank a terminal-looking exit.** Once timeout, protocol violation, backend failure or parent-initiated termination occurs, that run cannot become CandidateProcessTerminal even if its final code isC0000417. Record who requested stopping, the reason and the observed status separately. Use a distinct harness cleanup status and retain a sticky supervisor-failed state. Reject a valid boundary followed by ordinary exit, terminal status without a boundary, conflicting Returned/TerminalBoundary, and any child killed by the parent. The native status alone is insufficient.

**P1: a nonreturning interface needs defined unexpected-return behavior.** Marking an arbitrary virtual callback [[noreturn]] does not make it nonreturning; returning through that promise risks undefined behavior. A thrown marker instead invokes unwinding/catches. Agree a fail-closed worker state before wiring dispatch: a backend that returns or cannot publish must report HarnessFailure where possible, must not continue Save, and must remain confined for supervisor stop. It must not turn into the same expected terminal status. This mechanism needs an actual adverse-backend test in an owned child, not a returning endpoint inside an ordinary Testkit process.

## Claims that need narrower wording

A profile enum/proof label selects a source-qualified assumption; it does not observe the original module's handler global or prove exclusive process ownership. Keep FreshDefault admission fixed to the agreed candidate experiment, require the owned-child launch/handshake, and reject Unknown/Custom. Do not consult or mutate the embedding host CRT handler as a proxy for the original module. The descriptor's first-invalid-printf occurrence is a modeled source boundary; the probe does not execute the original fprintf/dispatcher.

Matching C0000417 and no normal return proves the candidate fixed endpoint under its test contract. It does not prove native context capture, debugger/filter side effects, WER behavior or full default-handler architecture. Keep `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented` in every terminal receipt. A self-test process that exits with that code must use a distinct SelfTestEndpoint site and must never satisfy the real remesh-source occurrence assertion.

An absent post-call Close marker is not an empirical Session-destructor observation, and current run does not implement complete accepted Close. Preserve those limitations. Likewise, statically linked wide/UTF8 wrapper checks do not alone establish DLL binary compatibility. Either explicitly load the verified candidate DLL/exports in a child, or limit the statement to static-wrapper behavior and unchanged public declarations while root retains its actual DLL ABI regression. Pin the DLL and other relevant loaded candidate artifacts, not only the helper executable.

The sharing-denial fixture is plausible but must be qualified with an unblocked control and observed enabled owner. It must be a valid KEYWORD_DRIVEN input, readable under the parent lock, with no earlier writer replacing that path. Assert unchanged remesh sentinel only for this pre-truncation denial; do not generalize all fopen-null residues. Read-sharing locks on the project/MST need their own read compatibility checks before using the combined-failure case.

## Minimal first integration slice

First integrate **harness safety only**, after CP104 archive release: a dedicated worker target, private protocol/backend, parent supervisor and pure process-outcome classifier. The worker offers an explicitly opted-in SelfTestEndpoint mode that has no remesh conversion or original-process execution. Keep the remesh writer, convert call services, old C ABI and product CLI untouched in this slice. This isolates the dangerous mechanism before shared production wiring and can be reviewed independently. Its acceptance is harness infrastructure, not recovered public behavior.

Required safety tests are bounded and falsifiable:

1. Normal self-test Returned record and matching ordinary exit passes the Returned classifier; no terminal classification occurs.
2. Explicit self-test endpoint sends its unique site then terminatesC0000417; the parent recognizes only SelfTestEndpoint success, never a remesh occurrence. A worker-local RAII marker/control can check no-unwind for this helper scope without implying Session destruction behavior.
3. Half-record stall and child exit with an incomplete record both finish within the deadline as failures. Full stdout/stderr output cannot block the parent indefinitely.
4. Missing/stale/version-mismatched nonce, excess/conflicting frames, wrong source site and boundary-plus-normal-exit are rejected. Complete boundary followed by a stall remains Timeout/HarnessFailure after parent cleanup, even if a terminal-looking status is seen.
5. Forced publication/backend failure or unexpected backend return cannot continue to a Returned conversion event and cannot pass terminal classification. Parent remains alive, stops only its child and preserves the failure receipt.
6. Parent fixture handles are not inherited; unrelated parent handles are not inherited; job-assignment/start failure cannot leave a running unowned child. Launch suspended, assign ownership and restricted handles, then resume. Use explicit application path/argv without a shell and verify artifact hash before/after.

Use one short deadline per deliberately stalled case; tests may select a smaller test deadline than the proposed20-second production probe ceiling. Measure elapsed time and retain every failure disposition. Parent exit0 means its expected assertion passed, not conversion M2M_OK.

Then reserve the real vertical slice together: editor300's private writer sibling/service, root's live after-MST forwarder, conversion worker mode, deterministic physical denial and all CP104 conversion/compatibility cases. The writer's default/null and Unknown/Custom paths must remain guarded. Append the nullable core service after projectOutput so existing aggregate initializers keep their meaning. Leave public Result/ConversionDetails/RemeshSaveReport and exported ABI unchanged.

Before claiming that vertical slice accepted, retain **all** CP104 required cases: normal conversion, real denied-remesh terminal observation, earlier returned writer failures dominated by the terminal boundary, wide/UTF8 legacy7 compatibility, and pure profile rejection. The safety-first slice does not waive or replace them. Existing31-write/ignored-close tests, CP98–102 numeric/errno coverage, full source-stable launcher and freeze remain required. Root must supply trusted pretest executable/DLL hashes when registering the harness; no worker should patch the shared runner or weaken binding.

## Verdict and next action

Do not activate a terminal writer callback during CP104 validation. Root can agree the sticky outcome precedence, end-to-end deadline and defined backend-failure state now, then reserve the isolated safety slice after freeze. Actual source-bound terminal dispatch remains dependent on those results and the complete CP104 test cut. This keeps the next increment useful and measurable without labeling an isolated helper, modeled prediction or same-code crash as original behavioral acceptance.

Only this uniquely assigned report was written. No source/test edits, builds/tests, GUI, original/native target execution or REA mutation occurred.
