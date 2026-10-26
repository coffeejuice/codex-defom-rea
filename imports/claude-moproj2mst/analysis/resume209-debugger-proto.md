# CP209 disposable-child Windows debugger prototype

The existing 64-bit Python 3.12.10 and `ctypes` are sufficient to retrieve x64 register context, service a hardware execution breakpoint, and preserve an unhandled native access violation through first and last chance. No compiler, Windows SDK, debugger installation, REA connection, GUI control, installed-program access, converter build, or original launch was needed.

`admin/child_debug_event_fixture.py` is exclusively a self-fixture runner. It has no target executable, command, working-directory, PID, or attach parameter. Its only `CreateProcessW` call launches the running Python interpreter with `-I -S`, the script's own resolved path, and a fresh token. `DEBUG_ONLY_THIS_PROCESS` limits debugging to that child. Arbitrary CLI arguments fail before launch; `--exe invalid` returned `No target/attach arguments supported`. The internal child mode only executes the disposable fixture and cannot launch another target. The fixture allocates its own private page, writes two small known instruction sequences while writable, then changes the page to execute/read before executing it. No pre-existing executable bytes or other process data are patched.

## Implemented boundary

The event loop runs on the process-creating thread and is bounded to 15 seconds/256 events. It checks the exact created process handle's resolved interpreter path, records PID, primary TID and creation FILETIME, duplicates that handle for post-exit verification, and explicitly sets debugger kill-on-exit to true. A token-qualified debug string supplies the private fixture address. At that stopped event it arms primary-thread DR0 at the fixture's `ret` instruction, captures the hit using a 16-byte-aligned x64 CONTEXT, validates the sentinel RAX value, and disables DR0/DR6/DR7 before continuing.

It consumes only the initial Windows breakpoint and its own address/DR6-qualified single-step. Application AV events use `DBG_EXCEPTION_NOT_HANDLED` at both chances. Register snapshots include RIP, RAX, RCX, RDX, R11, RSP, DR6 and DR7; AV snapshots also retain read/write/execute operation and address. The AV fixture uses a native child thread so Python's ctypes exception wrapper cannot turn the fault into a handled Python exception.

Process/thread event handles are tracked, image/DLL file handles are closed, exit events are continued, and the retained process handle must become signaled. CreateProcess caller handles were observed to differ from event handles on this host; the runner closes the distinct caller handles separately. Successful receipts verify all known process/thread/duplicate handles are invalid using `GetHandleInformation`. A separate read-only `OpenProcess` check found the exited PID still openable and signaled; **PID absence was not established**. Signaled exact creation handles establish process termination; they are not a claim that the PID has disappeared from every enumeration. No GUI session or app-control lifecycle was acquired.

## Actual final-source fixture result

Command: `python -I -S admin/child_debug_event_fixture.py`, executed from the converter subtree's parent checkout using the full relative script path. Exit status was 0. Source SHA-256: `27d494508a1eaafe1f350a3630e2afb777d933cf0bab12eb7e4ca7edcc6ab409`. Runtime: Python 3.12.10, MSC v.1943, 64-bit AMD64. Final receipt values:

| Field | Normal fixture | Unhandled-AV fixture |
| --- | --- | --- |
| PID / primary TID | 29856 / 37668 | 24372 / 3864 |
| Creation FILETIME | 134374893146584594 | 134374893147624888 |
| Events / successful continuations | 42 / 42 | 45 / 45 |
| Additional thread count | 3 | 4 |
| DR0 execution hits | 1 | 1 |
| Hit RIP | 0x2abd3db000a | 0x15fd446000a |
| Hit RAX | 0x1122334455667788 | 0x1122334455667788 |
| Hit DR6 / DR7 | 0xffff0ff1 / 0x401 | 0xffff0ff1 / 0x401 |
| Exit event / exit code | observed / 0 | observed / 0xc0000005 |
| Forced cleanup | false | false |
| Exact owned process signaled | true | true |
| Caller handles closed / all known handles invalid | true / true | true / true |
| Independent PID check | signaled | signaled |
| Elapsed child-loop seconds | 0.094 | 0.125 |
| Pass | true | true |

The AV thread was TID 3800. First-chance and last-chance records had the same RIP `0x15fd4460012`, RAX `0x15fd446000b`, RCX `0x58`, RDX `0x15fd446000b`, R11 `0`, RSP `0x59ec3af7f8`, DR6/DR7 `0`, operation `0` (read), and address `0x58`. First-chance flags were `true`, then `false`. These are synthetic fixture observations, not original-application evidence.

## Development failures retained

The first revision rejected unexpected create-thread events before reaching the fixture: Python/Windows created three auxiliary threads. Both test children (PIDs 12796 and 41868) were terminated through their exact owned handles; cleanup exit events and signaled state were observed. The revision was corrected to track all event thread handles while keeping the one hardware breakpoint primary-thread-only.

The second revision passed the normal fixture but saw only a first-chance AV followed by exit 1: ctypes caught a direct native call's access violation and Python converted it into an exception. That AV fixture correctly failed its required last-chance/natural-exit gate. Moving the AV sequence to an explicitly created native fixture thread produced matching first/last-chance snapshots and natural `0xc0000005` exit. The final source then added handle-invalid and independent PID-state receipts; both final-source cases passed. No failed run is counted as a passing fixture or original capture.

## Scope and remaining limitations

This is an engineering feasibility fixture, not an original-ready debugger. Only the primary fixture thread receives DR0; auxiliary threads are tracked for ownership and exceptions but are not claimed as breakpoint-covered. It has no original identity/hash gate, ASLR RVA mapping, original instruction validation, bounded object reads, invocation/frame correlation, module qualification, multi-thread breakpoint propagation, recursion handling, or original parity comparison. It has no detach/resume-existing-process mode. The forced-cleanup path is bounded and marks failure, but debugger crashes, unexpected event variants and every cleanup failure have not been exhaustively injected. The final passing cases establish natural cleanup for these two fixture paths only.

The initial Windows breakpoint is classified by the first breakpoint exception rather than validating its ntdll address; this is sufficient for the fixed self-fixture but requires strengthening before any original runner. A hardware breakpoint still changes debug-register context and event timing. The fixture cannot establish observational neutrality or causality in the original. It must not replace the CP205 parity gate, and the CP207 same-fault-path producer-return capture remains separately qualified evidence in `resume207-original-function-owner-2026-10-26T11-51-11-398Z/summary.json`.

The prototype used only this new admin file and this report. Shared plans, implementation source, original project data, installed applications and deployed binaries were not modified. No claim of speed/token savings is made; the table measures these fixture loops only.

Microsoft references: [ContinueDebugEvent exception disposition and exit-handle semantics](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-continuedebugevent), [CREATE_PROCESS_DEBUG_INFO ownership fields](https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-create_process_debug_info), and [native x64 CONTEXT layout](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-context). Repository sources read: subtree `AGENTS.md`, `TAKEOVER.md`, `RESEARCH_CHARTER.md`, bounded current-state/plan/import records, CP207 fallback/implementation/fixture/capture records, and parent recovery status/efficiency policy.
