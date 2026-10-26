# CP269 four-slot debugger schedule: full CP264 contract is not qualified

A four-execution-breakpoint schedule can collect the expected positive chronology by multiplexing a deepest normal-return breakpoint. It does not, on the retained evidence, preserve CP214's overlapping-entry and order checks without blind intervals. The full CP264 contract, including the retained normal postOpen endpoint and no missed nested events, is therefore **no-go for a four-slot-only runner**. This is an offline resource/coverage finding, not a proposal to retry the prohibited Frida lane or permission to execute another observer.

The target in these retained sources is original installation v11.0.2 DEF_GUI_MO_64.exe, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, with reported null PE version resource. No reconstructed DEF_PRE instance is examined or changed. No original, debugger, surrogate, GUI, REA, source, test, fixture or build was executed or modified. Only this report is written. The analysis uses local retained sources; native API behavior below is a design obligation inherited from CP268, not a newly qualified implementation.

## Contract and retained chronology

CP214's independent original review qualifies fourteen primary-thread events in order:

`outer enter -> duplicate leave -> factory leave -> cast leave -> Project leave -> acceptance enter/leave -> outer leave -> postOpen enter -> current enter/leave -> activation enter/leave -> postOpen leave`.

The CP264 addition belongs **inside acceptance**, before acceptance leave: creator entry, selected fifth directory-helper entry/leave, creator leave. CP214 never recorded these added events, so their positions are a CP264 required nesting condition supported by retained static call evidence, not a new runtime observation.

All RVAs below are relative to the verified loaded image base B. Entry return addresses must be read from the exact entry RSP; caller PCs alone do not identify a frame.

| Function | Entry RVA | Selected caller return RVA | Required record |
| --- | --- | --- | --- |
| outer | 1ca30 | 1c94 | main, path QString, entry RSP; normal nonzero P leave; stable path reread |
| duplicate | c78c10 | 1cf11 | selected entry/frame association; normal zero return |
| factory | c79390 | 1d08a | selected entry/frame association; normal nonzero return |
| cast | 146f8fc | 1d0bd | selected entry/frame association; normal nonzero return |
| Project accessor | 44a90 | 1d0d8 | selected entry/frame association; normal nonzero P return |
| acceptance | 8a9b0 | 1d0fb | RCX=P, path QString, entry RSP; matching normal AL=1 leave |
| creator | 8c630 | 511d0 | RCX=P, enclosing acceptance/outer identity; matching normal leave, raw return only |
| directory helper | 81afb0 | 8c77a | fifth selected call, RCX QString pointer, DL=1, exact clone Lubricants path; matching normal leave |
| postOpen | 2a0e0 | 1cae | same main/P; matching normal void leave |
| current | 45780 | 2aa72 | same post frame; matching normal return P |
| activation | a7250 | 2aaa1 | RCX=P, EDX=0; matching normal void leave |

The five outer prerequisite/acceptance entries have RSP = outer-entry RSP minus 0x1e0. The selected current/activation entries have RSP = postOpen-entry RSP minus 0x4e0. These are retained CP214 qualifications. Creator/helper stack relationships require their own validation; this report does not invent offsets. Retain entry/return registers, bounded validated memory reads and exact child creation FILETIME as text. Neither creator RAX nor void-return RAX is a success boolean. A normal return-site hit requires RIP=expected return PC **and RSP=recorded entry RSP+8**, matching TID, span stack and predecessor state; an instruction pointer hit alone is insufficient.

The contract also requires rejecting or poisoning overlap, wrong order, multiple qualifying occurrences and mismatched ownership. This is material: the retained agent observes more than its fourteen emitted positive records. It counts rejected calls, detects outer reentry before caller filtering, and detects postOpen/helper overlaps before some caller checks. Removing those guards changes the contract.

## Strongest plausible four-slot positive-path machine

The following is an analysis of an insufficient design, not an executable release specification. It explains why merely listing positive records makes four slots look sufficient.

Maintain immutable frame records in the host: `(child identity, TID, kind, occurrence, entry RSP, return PC, parent, expected-next, poisoned)`. Each selected entry pushes a record; a verified normal return pops it. A return address stored in a host frame consumes no hardware slot until armed. A single frontier slot can follow the deepest selected frame. Ancestor return sites may be unarmed while a deeper normal-return obligation is pending, provided exceptions/unwinds and unsupported transfers immediately invalidate the run. Thus **four nested spans do not imply four simultaneously armed return addresses**.

During the outer phase, a tempting allocation is:

- slot 0: outer entry, retained as an overlap guard;
- slot 1: creator entry;
- slot 2: directory-helper entry;
- slot 3: frontier, initially duplicate entry, then duplicate return, factory entry/return, cast entry/return, Project entry/return, acceptance entry/return in turn.

At each expected prerequisite entry, validate the selected caller, primary TID, unique active outer and RSP relationship before replacing frontier entry with its recorded return PC. At its return validate zero/nonzero and order, then advance frontier to the next expected entry. At acceptance entry retain its path/P/frame and arm acceptance return. At eligible creator entry while acceptance is active, push creator and replace frontier with creator return. At each helper entry keep its real caller and frame; only caller 8c77a, DL=1 and the required exact QString within that creator can satisfy the fifth-helper predicate. If tracing rejected helper spans as well, use nested frontier return records rather than forgetting their extent. At selected helper return restore creator return; at creator return restore acceptance return; at acceptance return validate AL=1 and restore outer return; at outer return retain P and prepare postOpen.

Normal selected nesting can reuse frontier safely only after checking a pending debug event and the entire thread context before continuation. If two independent semantic obligations share one physical address, maintain a set/list of frame subscribers and dispatch by TID/RSP/kind/phase, never overwrite one obligation with another. A hit with no unique eligible subscriber is incomplete. Equal return PCs from recursion need distinct RSP records. A return PC equal to an armed entry needs a single physical slot with both logical subscribers. The concrete outer/acceptance/creator/fifth-helper return PCs listed above are distinct; no alias saves a slot here. ASLR preserves these within-image distinctions.

At postOpen entry, the optimistic schedule switches to postOpen-entry, current-entry, activation-entry and frontier-return. Current return advances state from 0 to 1 with P equality; activation entry validates P/EDX/frame, activation return advances to 2; postOpen normal leave then ends the trace. It stores the ancestor postOpen return in the host while current/activation is active.

This machine has two unproved assumptions: a disabled non-frontier hook cannot produce a selected out-of-order event, and an entry guard that has been retired cannot contribute a required overlap/unique-occurrence failure. The retained call windows and one positive trace do not prove either assumption over all callback, indirect-call, exception and thread paths.

## Concrete coverage conflict

The postOpen phase alone exposes a five-address obligation at a valid selected activation entry. Until activation returns normally, preserving the retained guards requires:

| Armed address | Why it is still needed |
| --- | --- |
| B+1ca30, outer entry | A second qualifying outer entry changes invocationSerial/active owner state even after the first outer returned. A nested occurrence that finishes before the postOpen endpoint cannot be assumed absent. |
| B+2a0e0, postOpen entry | Any nested postOpen entry poisons the active post frame **before caller filtering**. |
| B+45780, current entry | A selected-caller current entry while activation is pending violates pending/order/frame checks and poisons postOpen. A nested 2a9b0 call need not itself enter 2a0e0. |
| B+a7250, activation entry | Any activation entry while activation is pending poisons overlap **before caller filtering**. |
| B+2aaa1, activation return | Required matching normal activation leave and its exact frame must be witnessed. |

These five distinct execution addresses cannot occupy DR0–DR3 simultaneously. The counterexample is an observational ambiguity: retire any one of the four guards, execute its qualifying/poisoning event and complete the nested work before the remaining observed return, and the reduced observer can see the same positive suffix as a valid trace. Retire activation return instead, and normal activation leave is no longer directly witnessed. The retained evidence does not establish that all such nested control flow is unreachable in the original. The conflict is therefore a proof that this schedule cannot **guarantee the required monitoring contract**, rather than a claim that a particular unobserved reentry actually occurs.

The earlier frontier prerequisite schedule has the same evidence gap: while waiting on one return it no longer monitors every other prerequisite entry. CP214's exact selected-caller/frame filters can sometimes make a guard redundant, but such redundancy requires a complete static control-flow/nonreentry proof for the relevant dynamic interval. CP214's successful fourteen-record trace and copied call windows are not that proof. Merely declaring unexpected nesting unsupported does not help if its trigger is not observed.

Dropping postOpen is explicitly a possible narrower CP264 endpoint choice, but the requested assessment includes it. Even for an outer-only endpoint, the frontier design above remains conditional on proving selected out-of-order calls impossible and defining creator/helper reentry coverage. This report does not approve that reduced contract by omission. Exact CP214 rejected-caller counts across all hooks are also unattainable from the displayed four-slot frontier design; they cannot be fabricated as zeros.

A target-specific complete proof of unreachable omitted events could change this conclusion. No such proof is present. Exhaustive trap-flag single stepping could observe omitted instructions without code patching in principle, but it is a different instrumentation plan with enormous timing/event-volume and exception qualification requirements; it is not a repair to the bounded four-execution-breakpoint schedule. No INT3, page-protection exception scheme, debug-register rotation timer, injected trampoline or prohibited Frida retry is authorized here. Polling/rotation leaves gaps and cannot certify absence.

## Thread and debugger-event obligations

Hardware debug registers are per thread. At process creation retain primary identity and initialize its frozen context while execution is stopped. For each CREATE_THREAD event, record exact owned thread identity and establish/debug-context coverage before continuation; observing only primary registers cannot prove other threads did not enter selected functions. Four slots on an unrelated thread cannot lend a fifth comparator to the executing primary thread. Resuming other threads only at convenient milestones does not erase the primary-thread conflict and changes scheduling.

A future policy must choose between fully monitoring all owned threads with per-thread records and a qualified rule that any unsupported thread participation fails closed. Failing on every new thread avoids false success but might preclude any useful original trace; silently allowing an uninstrumented thread does not establish primary-only occurrence. Thread creation/exit, loaded modules and debug exceptions must be consumed on the creating debugger thread. Keep finite waits, the CP264 12-second observation/30-second total bounds, overflow/error receipts and explicit pause overhead.

At each exception inspect DR6 and the owned DR7 configuration; one event can report several fired slots. Process all unique logical obligations before a single continuation. Execution-breakpoint continue semantics, RF/TF behavior, DR6 status handling and context preservation need native surrogate qualification. Never advance RIP or replace application arguments/results to bypass a retrigger. Unexpected application exceptions/unwinds invalidate occurrence; pass through using the appropriate not-handled disposition while entering owned cleanup, rather than claiming a normal leave. The expected OS initial debugger breakpoint must be identified by independently qualified startup state, not swallowed merely because its exception code is a breakpoint. These are design constraints, not tested API claims.

## Native ownership and cleanup state machine

The runner remains unimplemented. CP268's debug-at-creation route is conditional and distinct from attach/Frida. Its resource states must be explicit before any future surrogate implementation:

1. **Before creation:** no child identity or target authority; no name/PID-based cleanup. A failure closes only local resources.
2. **Created suspended / creation event pending:** retain CreateProcessW handles, duplicate the exact process handle into a private wait/termination handle, bind PID/TID plus decimal creation FILETIME and verified image identity, and account for aliases between returned and debug-event handles. CREATE_PROCESS/LOAD_DLL image-file handles require explicit closure after required identity reads. Keep a separate ledger for OS-managed debug-event process/thread handles versus private duplicates; a numeric alias must never be double-closed.
3. **Validated / ready:** exact image/module/ABI/context and source/fixture gates have passed, DebugSetProcessKillOnExit(TRUE) returned success, initial event state and explicit suspend count are recorded. ResumeThread's single decrement and ContinueDebugEvent are separate transitions whose ordering and receipts require surrogate proof. A failed/ambiguous transition enters cleanup, not another resume attempt.
4. **Running / stopped at event:** retain one pending-event token `(PID,TID,event serial)` until exactly one successful continuation; all slot edits and raw records belong to that token. Continue failure is unknown execution state. Natural process exit wins over an endpoint/timeout race. Every bound, overflow, mismatched identity or unexpected exception transitions once to incomplete cleanup.
5. **Cleanup requested:** use only the retained exact-child process handle. If unsignaled, request TerminateProcess(handle,0xe201) once and retain its result/error plus natural-exit race; if signaled, query the existing exit result. No second PID lookup becomes termination authority. Continue pending/remaining debug events according to their real disposition until EXIT_PROCESS is received and continued, subject to a bounded drain. Preserve a failed continuation or missing exit receipt as uncertainty.
6. **Exit continued / wait:** Windows debug-event process/thread handle lifetimes must follow their documented exit-continuation rules; do not close them again as private duplicates. The private duplicate must remain valid across that transition for a bounded signaled wait/GetExitCodeProcess. Close every private process/thread duplicate, event-file handle, watchdog/output handle exactly once, with API results/late errors retained. An independent identity-specific absence check supplements the signaled exact handle; PID absence alone is weaker and PID reuse must not authorize action.
7. **Closed / unknown:** only a complete resource ledger, known continuation state, exact-handle signaled result, exit result and independent absence can satisfy cleanup. Deadline, blocked event draining, abrupt debugger death or unknown closure remains failed/unknown. Kill-on-exit is not proof of descendant or pre-debug-creation-race cleanup; neither a Job Object nor watchdog is qualified merely by this report.

For API details, implementation must use the CP268 primary-reference qualifications and local Windows SDK declarations, then prove actual handle/continuation behavior on a harmless native surrogate under separate authorization. Do not infer that handles returned through different API surfaces are necessarily independently owned. No detach is proposed as successful cleanup; terminating a debuggee also does not supply application Close or GUI-control release. This route creates no Frida device/session, and must record that fact rather than demand or invent a Frida detach receipt.

## Decision and remaining work

Keep original execution no-go. Do not implement a four-slot observer and label missing guards as impossible paths without proof. A future offline change could either supply a rigorous target-specific redundancy proof, explicitly review a narrower contract, or specify and qualify a different observation mechanism. None is approved by this report. Source-frozen native implementation, harmless surrogate qualification, cleanup/neutrality review and independent policy review remain separate prerequisites even if the scheduling conflict is resolved.

Sources read: `resume268-original-debugger-route.md`, `resume264-original-cli-next-probe.md`, its independent review, `resume214-original-postopen-review.md`, `resume214-postopen-probe-plan.md`, the retained CP214 generated `outer-loader-observers.js`, `admin/cp214_postopen_agent.inc.js`, and retained creator/postOpen disassembly. The retained agent was read only for its prior semantics; its prohibited instrumentation route was neither revived nor redesigned.

