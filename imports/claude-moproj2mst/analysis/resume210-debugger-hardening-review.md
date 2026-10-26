# CP210 independent hardening review

**PASS for the bounded child-only hardening suite: two natural fixtures and seven injected cleanup cases. PASS for detecting and safely rejecting the additional disabled-register seed attempt. HOLD for an owned-original outer-loader observer until its separate implementation and identity/parity gates are reviewed.** The seed rejection is not a tenth successful hardware capture or evidence of preserving nonzero debug state.

This review read the actual final `admin/child_debug_event_fixture.py` and parsed the embedded receipts in `analysis/resume210-debugger-hardening.md`. Independently calculated source SHA-256: `0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee`. Five JSON blocks parse; exactly one binds this final source. Earlier blocks were excluded from final acceptance. No fixture replay, original launch, GUI control, implementation edit or external project modification was performed. Findings are static review plus independent examination of saved raw receipt content, not an independent runtime replication.

## Evidence checked beyond pass flags

The matching final receipt records debugger subprocess exit 0, empty stderr, suite pass true and ten result records. I checked the per-generation closure/invalidity fields, continuation successes, counters, raw exception records, process-signaled fields and independent PID probes rather than accepting the top-level pass booleans alone.

| Case | PID | Events / successful continuations | Observed result |
| --- | --- | --- | --- |
| Normal | 36660 | 42 / 42 | One qualified loader event, one owned hardware hit; natural exit 0 |
| Native AV | 38748 | 45 / 45 | One loader, one hardware hit, AV first/last pair; natural exit `0xc0000005` |
| Disabled-register seed | 9340 | 40 / 40 | Seed readback was zero; explicit rejection; baseline restoration; exit `0xe210` |
| Before create event | 31660 | 3 / 3 | Injection reached; exact-child cleanup; exit `0xe210` |
| After create event | 21332 | 3 / 3 | Injection reached; exact-child cleanup; exit `0xe210` |
| After thread event | 37252 | 7 / 7 | Injection reached; new event-thread resource accounted; exit `0xe210` |
| Marker before context | 25508 | 40 / 40 | Injection reached; cleanup; exit `0xe210` |
| Context before set | 42060 | 40 / 40 | Injection reached before register mutation; cleanup; exit `0xe210` |
| After arm | 20988 | 40 / 40 | Injection reached; exact baseline debug state restored; exit `0xe210` |
| Before exit continuation | 28692 | 42 / 42 | Injection reached during natural exit; continued exit 0 |

For every row: all ledger generations have `closed=true` and `invalidImmediately=true`; all continuation entries have `ok=true`; live generations and cleanup errors are empty; exact owned process is signaled; independent parent OpenProcess returns error 87. The final exit-injection TerminateProcess returned false/error 5, which is preserved rather than counted as successful termination. Process termination is independently established by the continued exit event and exact handle signal. Error 87 establishes PID absence at that post-debugger check; it is not a permanent PID identity guarantee.

The normal and AV register baselines and verified restorations contain DR0-3/DR6/DR7=0 and resumeFlag=0. Both hardware records contain the expected sentinel RAX. The AV records have matching captured fields apart from firstChance, native thread 20680, RIP `0x1d4a8b20012`, RCX/address `0x58`, parameterCount=2 and operation=0. Both AV exception disposition entries are `0x80010001`. Unknown counters are zero in the natural cases.

## Source assessment

The x64 ABI and aligned CONTEXT reasoning from the CP209 review remains applicable. The implementation saves debug registers and the resume flag before arm, retains unrelated DR7 controls when using slot 0, and verifies exact restoration by a fresh GetThreadContext. An occupied DR0 enable pair causes rejection. Restoration also runs while stopped during the tested post-arm failure. This fixes the earlier deliberate state clearing for the proven zero-state case.

The loader breakpoint now requires primary TID, first chance, pre-marker lifecycle, one occurrence, pinned ntdll hash and loaded bytes, exact exception address, and RIP one byte after int3. The final receipt binds ntdll hash `651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c`, RVA `0x1226c9`, bytes `0a807c2440007503cceb004883c438c3cc`. This is a host-specific fail-closed fixture gate, not general symbol discovery. Hardware qualification adds primary TID, first chance, armed state, exact address and only B0 among DR6 breakpoint bits. Unknown exceptions pass through and prevent natural PASS. AV parity is now in the acceptance predicate rather than merely present in prose.

Resources are registered immediately in both main and drain loops. Manual image/DLL handles and caller/duplicate handles are explicitly closed; event-owned handles are marked closed only after successful exit-event continuation. The live-generation map avoids the prior accumulation of stale numeric handle values. Immediate invalidity checks and natural/failure receipts demonstrate cleanup for these observed generations. The source retains termination outcomes, records continuation failures and accepts an exit only after successful continuation. These changes address the CP209 review's concrete natural-versus-drain ownership gap.

The no-target interface remains restricted to the current interpreter and this resolved script, isolated Python flags, fresh token and fixed fixture modes; DEBUG_ONLY_THIS_PROCESS excludes descendant debugging. The extra internal suite parameter runs this same fixed suite and does not permit a target executable or PID. Termination remains through the exact created instance handle. The read-only PID probe is confined to PIDs returned by the just-completed internal suite.

## Remaining limits and original readiness

1. **Nonzero state remains unproved.** Windows returned zero for the attempted disabled-slot seed. The final code detects that, restores the prior baseline, and requires cleanup/absence. It does not validate unrelated active DR1-3 preservation, nonzero DR6/DR7 normalization, or restoration from an active prior debug configuration. For an owned-original observer, reject unexpected preexisting debug state unless a separately proven preservation policy is implemented. The current DR0 check alone does not enforce an all-zero configuration.

2. **Injections are control-flow exceptions before named operations, not Win32 failures.** No actual Get/SetThreadContext failure, partial register mutation, failing continuation, CloseHandle failure, unknown native exception, target-creation failure or debugger death was injected. The after-arm case establishes restoration at that stopped marker. It does not prove recovery when a later failure occurs without a pending debug event; the current error path only attempts restoration when armed and pending, then terminates the exact child. A terminating-only policy may be valid for a disposable owned process, but it is not detach/resume support.

3. **Failure reporting is not exhaustive.** CreateProcess failure and outer subprocess timeout/invalid JSON can escape without the normal structured receipt. The outer 55-second timeout can end the debugger; kill-on-debugger-exit is requested, but exact descendant termination and final PID absence on that abrupt path were not tested. A failed termination result is retained but not itself added to cleanupErrors; acceptance relies on subsequent continued exit, exact signal and absence. That is appropriate for the observed already-exiting case, not universal termination success.

4. **Owned-original adaptation needs a new reviewable boundary.** Bind the original image identity/version/hash, exact copied disposable project and environment, loaded module bases/hash/bytes and selected outer-loader instruction boundary. Decide explicitly whether only the primary thread is in scope; reject unplanned thread/invocation behavior or implement per-thread ownership. Capture arguments/object reads with strict bounds and correlate entry/return frames and lifetimes. The synthetic marker supplies its own address; the original runner must establish addresses and invocation identity independently. No blanket breakpoint propagation or recursive-call handling is established here.

5. **Original observation neutrality is a separate gate.** Preserve both exception chances and require CP205 module/RVA, fault operands, stack/path and natural-exit parity on the same owned run. Debug events and hardware context changes can alter timing and path. Synthetic AV parity cannot establish original causality, completion of Open/Save/Close, or converter acceptance. An outer-loader-only observer may narrow its requirements to the states it actually touches, but must not inherit original behavioral acceptance from this child suite.

The child fixture is suitable as evidence that these APIs and lifecycle mechanics work on this host under the tested conditions. It is sufficient to begin preparing a separate owned-original observer implementation and review; it does not authorize or validate running an unreviewed adaptation. No GUI-control session was acquired or released, and no deployed executable was produced.

Primary API contracts used in the preceding CP209 review still apply: [native x64 CONTEXT](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-context) and [ContinueDebugEvent](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-continuedebugevent). The raw receipts provide observed evidence; these contracts explain the register and handle semantics.
