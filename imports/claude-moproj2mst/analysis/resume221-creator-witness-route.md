# CP221 creator/witness route assessment

## Decision

No supported low-overhead local method was found that can bind the original process occurrence of `0x140050820 -> 0x140051190 -> 0x14008c630` with call identity, arguments, returns, and failure continuation while leaving the existing user PID untouched. Do not retry the CP215 Frida lane: its observer work was interrupted by automated filtering before freeze, and this route is explicitly not to be repeated.

The CP219 no-hook pair remains useful only as filesystem contrast. It observed an added `Lubricants` directory for the control and three root-level `ObjPropertiesNNNNN.KEY` files for the negative-ID input. It did not observe a completed/comparable Open, identify the creator of those paths, or tie any effect to the three native calls. The 5-second process-survival interval is not an Open-return or acceptance witness (`resume219-original-receipt-review.md`).

## Local method comparison

| Method available or considered | Overhead / process impact | What it could establish | Why it cannot satisfy this witness |
|---|---|---|---|
| Windows ETW/WPR (`wpr.exe` and `logman.exe` are present) | Kernel/provider event collection; no code patching or debugger attach is inherently required. | With an applicable file-I/O provider/profile, timestamps, process identity, and observed filesystem operations/statuses may corroborate path creation or failure. | File-I/O events do not establish entry into these three private addresses, their argument values, return registers, caller chain, or that a `mkpath` failure's modal path returned before the next call. Even stack-enabled file events would not recover the private call arguments/returns as a same-occurrence sequence. It is insufficient for the requested binding.
| REA `observe_native_calls` | Breakpoint observation under LLDB; launches the selected executable as an owned process and kills it at the observation bound. | Function-entry registers, thread, module/address, and optional caller frames for resolved breakpoints. | The registered tool supports launching the active Mach-O, not attaching to an already-running Windows PE. It would launch a new process and cannot meet the no-touch-user-PID condition or the original Windows target requirement.
| REA Frida attach/instrumentation | In-process instrumentation and breakpoint hooks; can observe arguments/returns when source and lifecycle are qualified. | Potentially the direct address/argument/return sequence needed. | The prior CP215 observer lane was stopped before source freeze by automated filtering. The task forbids retrying that route, and attaching to the existing process would touch the user PID. No alternate approved Frida path is evidenced here.
| Windows debugger attach (WinDbg/CDB) | Debugger attach suspends/controls the target and breakpointing modifies execution behavior. | Address breakpoints can record registers and returns with a qualified script. | `windbg` and `cdb` are not discoverable on the current PATH, and an attach would violate the no-touch-user-PID constraint. Installing or substituting another debugger is not a low-overhead supported local route established by this audit.

These capabilities were assessed from the local REA tool descriptions and read-only command discovery only. No process inventory, target launch, attach, instrumentation, ETW session, or GUI-control session was started.

## Smallest evidence-producing next action

Continue from the retained source and disassembly without upgrading the runtime claim: preserve the ordered native contract (positive effective ID gate; task path attempt first; five fixed project paths and conditional Type4 `DieStress`; ignored helper returns) and keep candidate Open staging inert. The CP221 design already identifies the minimal candidate representation: an ordered task-plus-project attempt record appended only after operation construction, separate from the frozen project-only vector. A focused source-level review can then verify candidate paths/order/gating against `wave3-earlier-save-140051190.asm`, `wave3-earlier-save-14008c630.asm`, and the existing helper/dialog contract.

This next action can establish candidate modeling parity against the retained static contract. It cannot establish original runtime call occurrence, concrete path arguments for a run, native return values, actual failure/modal ordering, or creator causation. Keep public filesystem writes and any claim of a bound original Open gated until a policy-approved observation route for a separately owned, comparable process is available; the evidence gathered here does not authorize touching the user PID.

## Evidence consulted

- `analysis/resume219-original-receipt-review.md`: CP219 comparison limits, retained ownership receipts, and lack of direct Open witness.
- `analysis/resume215-observer-interruption.md`: CP215 observer lane filter interruption; draft unqualified and not to be run.
- `analysis/resume221-task-dir-static-review.md` and `analysis/resume221-task-directory-stage-design.md`: static call ordering, ignored-return contract, and candidate staging boundary.
- `analysis/wave3-native-directories-contract.json`: helper and native directory semantics/qualifications.
- Read-only local command discovery: `wpr.exe` and `logman.exe` present; `windbg.exe`, `cdb.exe`, and `procmon.exe` not discoverable on PATH.
- Registered REA capability descriptions for `observe_native_calls`, `capture_process_scenario`, and Frida instrumentation/session tools.
