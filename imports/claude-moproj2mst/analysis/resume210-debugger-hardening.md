# CP210 debugger fixture hardening

The child-only prototype now passes two natural fixtures and seven injected cleanup cases with saved/restored debug state, qualified loader/hardware events, checked handle generations, and independent PID absence after the debugger exits. An extra disabled-register seed attempt is an **explicit negative-control rejection**, not evidence of preserving nonzero registers. No original or GUI was launched. Original adaptation remains HOLD.

Final source SHA-256: `0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee` (`admin/child_debug_event_fixture.py`). Runtime: existing Python 3.12.10 x64. No compiler, SDK or installation was required. Only this source and this report changed for CP210; shared plans and CP209 reports remain untouched.

## Before and after

| Concern | CP209 | CP210 |
| --- | --- | --- |
| Debug state | Overwrote DR0/DR6/DR7 | Saves DR0-3/DR6/DR7 and resume flag; preserves unrelated controls; reads back exact restoration before continuation and during post-arm failure cleanup. |
| Initial breakpoint | Accepted first breakpoint unconditionally | Requires primary TID, first chance, pre-marker state, zero prior loader hits, pinned ntdll hash/RVA/bytes, exact exception address and RIP after int3. |
| Exception classification | Unknown exceptions did not block PASS | Explicit loader/hardware/AV-first/AV-last/unknown counters and per-exception disposition. Natural PASS requires exact counts and unknown=0. Hardware also requires primary TID, first chance, armed state, exact address and DR6.B0-only. |
| AV pairing | Chance sequence and operands | Requires identical captured fields apart from firstChance, expected native thread/start, exact RIP, parameter count=2, RCX/address=0x58 and read operation. |
| Failure cleanup | Unchecked drain calls and missing new handles | Immediate resource registration in both loops, checked continuation/termination, per-generation handle ledger with immediate invalid-handle checks, exit accepted only after successful continuation. |
| Independent PID state | Exited PID still openable and signaled | Separate parent checks after debugger subprocess exits; all final PIDs return OpenProcess error 87. No arbitrary PID or target input. |
| Durable evidence | Transcribed receipt | Complete decoded stdout JSON embedded below, including failed iterations and final-source run. |

The change in process-object visibility after debugger exit is observed. Attributing it specifically to a retained Windows debug-object reference is an inference, not a kernel reference-count measurement. PID absence applies at the recorded independent check, not forever.

## Focused final-source results

Command: `python -I -S imports/claude-moproj2mst/admin/child_debug_event_fixture.py` from this checkout. Final exit 0; suite pass true; debugger subprocess exit 0 and empty stderr. AST parsing passed. `--exe invalid` exited 1 with `No target/attach arguments supported` before fixture launch.

| Case | PID | Events / continued | Outcome | Exit |
| --- | --- | --- | --- | --- |
| Normal | 36660 | 42 / 42 | Natural PASS, one hardware hit, restored state | 0 |
| Native AV | 38748 | 45 / 45 | Natural PASS, one hardware hit, paired AV, restored state | 0xc0000005 |
| Disabled-register seed | 9340 | 40 / 40 | Explicit rejection; verified baseline restoration | 0xe210 |
| Before create event | 31660 | 3 / 3 | Injected cleanup PASS | 0xe210 |
| After create event | 21332 | 3 / 3 | Injected cleanup PASS | 0xe210 |
| After new-thread event | 37252 | 7 / 7 | Injected cleanup PASS | 0xe210 |
| Marker before context | 25508 | 40 / 40 | Injected cleanup PASS | 0xe210 |
| Context before set | 42060 | 40 / 40 | Injected cleanup PASS | 0xe210 |
| After arm | 20988 | 40 / 40 | Injected cleanup PASS, restored state | 0xe210 |
| Before exit continuation | 28692 | 42 / 42 | Injected cleanup PASS, already naturally exiting | 0 |

All ten closed every acquired handle generation with immediate invalid-handle checks, recorded zero cleanup errors, signaled the exact owned process, and had independent PID-absence checks return error 87. The seven injected cases are failure tests, not natural captures. Before-exit injection retains the attempted TerminateProcess result even though the process had already exited; no success is inferred from that call alone.

Natural counters: loader=1, hardware=1, unknown=0 in both cases; normal AV=0/0, native AV=1/1. Both hardware hits have RAX `0x1122334455667788`. The native AV thread is TID 20680, primary TID 23152. Both AV chances have RIP `0x1d4a8b20012`, RCX/address `0x58`, operation 0, RSP `0xaa953af7d8`, and equal captured fields; both use disposition `0x80010001`.

## Failed iterations and limitations

The first strict loader gate expected exported ntdll DbgBreakPoint and correctly rejected the actual private initial-loader site. Unknown=1 blocked both natural captures; owned cleanup and independent absence still passed. Inspection of this host's ntdll established RVA `0x1226c9`, file SHA-256 `651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c`, and surrounding bytes `0a807c2440007503cceb004883c438c3cc`. The final runner pins these and fails closed on change. This is host-specific fixture qualification, not a portable loader-symbol resolver.

The extra nonzero disabled-slot test initially lacked a strict seed readback check. Its intermediate receipt says seeded but actually shows zeros, and is invalid as nonzero-preservation evidence. After adding readback validation, Windows still returned zeros, including when a disabled-slot write-control field was set. The final source explicitly records this rejection, restores and verifies the baseline, and requires full cleanup/absence. It does not count the rejection as a natural hardware capture. Successful normal restoration is from the actual all-zero baseline. Preservation of nonzero active preexisting debug state remains untested; occupied DR0 is rejected rather than overwritten.

The focused suite does not inject an unrelated native unknown exception or real kernel/API return failures. Named injections occur before operations, including before exit continuation. They do not simulate every API failure, handle reuse race or debugger death. The outer debugger suite has a 55-second bound; abrupt debugger timeout/crash behavior remains a limitation. These are evidence-backed fixture improvements, not a universal cleanup guarantee.

Owned-original work remains HOLD. It needs separate original identity/version/hash and copied-fixture gates, original module/RVA/byte validation, per-thread breakpoint ownership, invocation/frame correlation, bounded object reads, CP205 exception/stack parity and broader failure qualification. Synthetic observations establish no original causal behavior or converter acceptance. No GUI-control session or deployed executable was produced. No speed/token-saving claim is made.

Primary contracts: [ContinueDebugEvent](https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-continuedebugevent), [x64 CONTEXT](https://learn.microsoft.com/en-us/windows/win32/api/winnt/ns-winnt-context), [CREATE_PROCESS_DEBUG_INFO](https://learn.microsoft.com/en-us/windows/win32/api/minwinbase/ns-minwinbase-create_process_debug_info). Review: `analysis/resume209-debugger-review.md`.

## Raw receipts

The blocks below preserve complete JSON text decoded from captured stdout for each iteration. Each binds its source hash and includes process identity, exception/continuation records, handle generations, cleanup results and independent PID checks. Only the last block binds the final source above. CRLF-to-LF decoding is not byte-for-byte preservation of stdout framing. Earlier failed runs are retained and excluded from final PASS claims.

# CP210 debugger hardening â€” pending assessment

Exact raw final-run JSON:

```json
{
  "schema": "child-debug-event-fixture-v2",
  "python": "3.12.10 (tags/v3.12.10:0cc8128, Apr  8 2025, 12:21:36) [MSC v.1943 64 bit (AMD64)]",
  "scriptSha256": "be3cd6911e6260860780881350761808e0cd6fa145c3359405268e334769ec63",
  "results": [
    {
      "mode": "normal",
      "injection": null,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x658d5ef200",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 34984,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x23c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x174",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x248",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x24c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x240",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x250",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x164",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x25c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x250",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x20c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 29288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 34984,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 29288,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 2216,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 37648,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 34984,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 26100,
      "primaryTid": 34984,
      "creationFiletime": "134374900088846828",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "error": "fixture marker lifecycle mismatch",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.109,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    },
    {
      "mode": "av",
      "injection": null,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x6065bef060",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 35288,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x164",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x250",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x264",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x26c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x25c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x270",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x208",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x270",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x24c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x208",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x248",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x248",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x248",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x204",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 31300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 35288,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 31300,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33832,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 24888,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 35712,
      "primaryTid": 35288,
      "creationFiletime": "134374900090025548",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "error": "fixture marker lifecycle mismatch",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.109,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    },
    {
      "mode": "normal",
      "injection": "before-create-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x278",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x274",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x280",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x284",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x27c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x288",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 30592,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30592,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 30592,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 27936,
      "primaryTid": 30592,
      "creationFiletime": "134374900091214192",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:before-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-create-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x28c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x288",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x294",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x298",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x290",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x29c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 31080,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31080,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 31080,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 8900,
      "primaryTid": 31080,
      "creationFiletime": "134374900091267300",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:after-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-thread-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 7,
      "continues": 7,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2a0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x29c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2a8",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2ac",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2a4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2b0",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2b0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 29544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33780,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 29544,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 33780,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 37476,
      "primaryTid": 29544,
      "creationFiletime": "134374900091313466",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 1,
      "injectionReached": true,
      "error": "fixture-injected:after-thread-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.015,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "marker-before-context",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xb5587eedb0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 11500,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2b0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x2ac",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2b4",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2b8",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2a4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2bc",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2bc",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2c0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 25176,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33060,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25176,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25176,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25176,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33060,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 11500,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33060,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33060,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33060,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 4220,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 25176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 11500,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 8200,
      "primaryTid": 11500,
      "creationFiletime": "134374900091457448",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "injectionReached": true,
      "error": "fixture-injected:marker-before-context",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.11,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "context-before-set",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xe110fef000",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 35160,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2ac",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x2b4",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2c0",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2bc",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2b0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x298",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2dc",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2a8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x290",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 34448,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 8584,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34448,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8584,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8584,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34448,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 35160,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 34448,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33508,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 34448,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 8584,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 32868,
      "primaryTid": 35160,
      "creationFiletime": "134374900092628063",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "error": "fixture marker lifecycle mismatch",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    },
    {
      "mode": "normal",
      "injection": "after-arm",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xc8fe5eed50",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 29180,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2b0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x2bc",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2c4",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2c8",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2cc",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2c0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2cc",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x290",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2dc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2dc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 29180,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33556,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23240,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 42792,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 29180,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 25052,
      "primaryTid": 29180,
      "creationFiletime": "134374900093820665",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "error": "fixture marker lifecycle mismatch",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.109,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    },
    {
      "mode": "normal",
      "injection": "before-exit-continuation",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x454dbef250",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 30656,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "unknown-pass-through",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2d0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x280",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2d8",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2a8",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2d4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2dc",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2dc",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2e0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2e4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x2ec",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2e8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2e8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2f0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2f0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2e8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23228,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23228,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41636,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41636,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41636,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23228,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 30656,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23228,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41636,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41636,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23228,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 36592,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 30656,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 1
      },
      "cleanupErrors": [],
      "pid": 35760,
      "primaryTid": 30656,
      "creationFiletime": "134374900095010351",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderBreakpointExpected": "0x7ffb92963f40",
      "additionalThreadCount": 3,
      "error": "fixture marker lifecycle mismatch",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.109,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    }
  ],
  "debuggerExited": true,
  "debuggerExitCode": 0,
  "debuggerStderr": "",
  "pass": false
}

```

Second hardening run, exact raw JSON:

```json
{
  "schema": "child-debug-event-fixture-v2",
  "python": "3.12.10 (tags/v3.12.10:0cc8128, Apr  8 2025, 12:21:36) [MSC v.1943 64 bit (AMD64)]",
  "scriptSha256": "4bcce54f17afd0ec625559446a8b7e07e23c4c8d64156b1887b435193b718e9d",
  "results": [
    {
      "mode": "normal",
      "injection": null,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 44,
      "continues": 44,
      "breakpointHits": [
        {
          "Rip": "0x2036a6b000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x20369ca45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x3bdc7ef0f0",
          "Rsp": "0x3bdc7ef0e8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 39976,
          "code": "0x80000004",
          "exceptionAddress": "0x2036a6b000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x3bdc7ef1f0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 39976,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x2036a6b000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x20369ca45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x3bdc7ef0f0",
          "Rsp": "0x3bdc7ef0e8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 39976,
          "code": "0x80000004",
          "exceptionAddress": "0x2036a6b000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x16c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x164",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x214",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x23c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x210",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x240",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x244",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x218",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 40,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 29256,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 39328,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29256,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39328,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39328,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29256,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29256,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 29256,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 39328,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 36224,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 39976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 37404,
      "primaryTid": 39976,
      "creationFiletime": "134374900556029726",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x2036a6b000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "exitCode": "0x0",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "av",
      "injection": null,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 45,
      "continues": 45,
      "breakpointHits": [
        {
          "Rip": "0x1c21fbb000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1c21f2545e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x14e79eefa0",
          "Rsp": "0x14e79eef98",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 33344,
          "code": "0x80000004",
          "exceptionAddress": "0x1c21fbb000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [
        {
          "Rip": "0x1c21fbb0012",
          "Rax": "0x1c21fbb000b",
          "Rcx": "0x58",
          "Rdx": "0x1c21fbb000b",
          "R11": "0x0",
          "Rsp": "0x14e81afcc8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 32944,
          "code": "0xc0000005",
          "exceptionAddress": "0x1c21fbb0012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        },
        {
          "Rip": "0x1c21fbb0012",
          "Rax": "0x1c21fbb000b",
          "Rcx": "0x58",
          "Rdx": "0x1c21fbb000b",
          "R11": "0x0",
          "Rsp": "0x14e81afcc8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 32944,
          "code": "0xc0000005",
          "exceptionAddress": "0x1c21fbb0012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        }
      ],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x14e79ef0a0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 33344,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1c21fbb000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1c21f2545e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x14e79eefa0",
          "Rsp": "0x14e79eef98",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 33344,
          "code": "0x80000004",
          "exceptionAddress": "0x1c21fbb000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1c21fbb0012",
          "Rax": "0x1c21fbb000b",
          "Rcx": "0x58",
          "Rdx": "0x1c21fbb000b",
          "R11": "0x0",
          "Rsp": "0x14e81afcc8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 32944,
          "code": "0xc0000005",
          "exceptionAddress": "0x1c21fbb0012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        },
        {
          "Rip": "0x1c21fbb0012",
          "Rax": "0x1c21fbb000b",
          "Rcx": "0x58",
          "Rdx": "0x1c21fbb000b",
          "R11": "0x0",
          "Rsp": "0x14e81afcc8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 32944,
          "code": "0xc0000005",
          "exceptionAddress": "0x1c21fbb0012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x164",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x214",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x278",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x21c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x16c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x230",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x284",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x27c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x23c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x244",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 27008,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 27008,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 27008,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 32944,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 32944,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 32944,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 27008,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33344,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 6688,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 32944,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 9188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 1,
        "avLast": 1,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 8480,
      "primaryTid": 33344,
      "creationFiletime": "134374900557361694",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 4,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1c21fbb000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "expectedAvTid": 32944,
      "exitCode": "0xc0000005",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.156,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-create-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x22c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x230",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x234",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x228",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x238",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x224",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 26176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 38740,
      "primaryTid": 26176,
      "creationFiletime": "134374900559031595",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:before-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.016,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-create-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x240",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x224",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x278",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x214",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x18c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x164",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 33176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 33176,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 13096,
      "primaryTid": 33176,
      "creationFiletime": "134374900559100156",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:after-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-thread-event",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 7,
      "continues": 7,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x224",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x278",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x214",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x18c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x240",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x164",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x258",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 30124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 39200,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 30124,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 39200,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 20296,
      "primaryTid": 30124,
      "creationFiletime": "134374900559166044",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 1,
      "injectionReached": true,
      "error": "fixture-injected:after-thread-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.015,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "marker-before-context",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x981cfef330",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 26404,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x278",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x214",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x18c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x258",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x224",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x274",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x288",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x218",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 28300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28300,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23268,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 28300,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23000,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 26404,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 20932,
      "primaryTid": 26404,
      "creationFiletime": "134374900559339686",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "injectionReached": true,
      "error": "fixture-injected:marker-before-context",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "context-before-set",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x405b1ef490",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 39648,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x164",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x278",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x234",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x29c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2a0",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x290",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x294",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 26952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38416,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38416,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38416,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38416,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38416,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 26952,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 32016,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 39648,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 23656,
      "primaryTid": 39648,
      "creationFiletime": "134374900560630887",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "injectionReached": true,
      "error": "fixture-injected:context-before-set",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.109,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-arm",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xd35e7ef110",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 16624,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0xac",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x250",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x214",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x298",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x18c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2a8",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x240",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x280",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2a4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 5016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 19424,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5016,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19424,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19424,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 19424,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 19424,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 5016,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 19036,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 16624,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 40176,
      "primaryTid": 16624,
      "creationFiletime": "134374900561890038",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x25a03a7000a",
      "injectionReached": true,
      "error": "fixture-injected:after-arm",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-exit-continuation",
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 44,
      "continues": 44,
      "breakpointHits": [
        {
          "Rip": "0x262556c000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x26254c945e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xc07c9eedb0",
          "Rsp": "0xc07c9eeda8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 39748,
          "code": "0x80000004",
          "exceptionAddress": "0x262556c000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xc07c9eeeb0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 39748,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x262556c000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x26254c945e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xc07c9eedb0",
          "Rsp": "0xc07c9eeda8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 39748,
          "code": "0x80000004",
          "exceptionAddress": "0x262556c000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x250",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x214",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2b4",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x18c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2bc",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2c8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2a0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x24c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0xac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0xac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2dc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 40,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 13188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 13188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 13188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 20628,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 21120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 39748,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 26396,
      "primaryTid": 39748,
      "creationFiletime": "134374900563248774",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x262556c000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "injectionReached": true,
      "error": "fixture-injected:before-exit-continuation",
      "termination": {
        "ok": false,
        "error": 5
      },
      "exitCode": "0x0",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.188,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    }
  ],
  "debuggerExited": true,
  "debuggerExitCode": 0,
  "debuggerStderr": "",
  "pass": true
}

```

Final-source run including nonzero disabled debug-slot preservation, exact raw JSON:

```json
{
  "schema": "child-debug-event-fixture-v2",
  "python": "3.12.10 (tags/v3.12.10:0cc8128, Apr  8 2025, 12:21:36) [MSC v.1943 64 bit (AMD64)]",
  "scriptSha256": "95dd21190818ad3d71baeb6aa610dfb0a6b96404345f55334e6bc5cbe8f59cf5",
  "results": [
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x1d0536e000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1d0535845e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x11f49ef160",
          "Rsp": "0x11f49ef158",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 26124,
          "code": "0x80000004",
          "exceptionAddress": "0x1d0536e000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x11f49ef260",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 26124,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1d0536e000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1d0535845e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x11f49ef160",
          "Rsp": "0x11f49ef158",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 26124,
          "code": "0x80000004",
          "exceptionAddress": "0x1d0536e000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x188",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x17c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x174",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x22c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x264",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x228",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x278",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x1a0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35648,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38560,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 26124,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 26068,
      "primaryTid": 26124,
      "creationFiletime": "134374901092459832",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1d0536e000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "exitCode": "0x0",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.219,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "av",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 45,
      "continues": 45,
      "breakpointHits": [
        {
          "Rip": "0x252b6c9000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x252b62745e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xfc6b5ef0b0",
          "Rsp": "0xfc6b5ef0a8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 4976,
          "code": "0x80000004",
          "exceptionAddress": "0x252b6c9000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [
        {
          "Rip": "0x252b6c90012",
          "Rax": "0x252b6c9000b",
          "Rcx": "0x58",
          "Rdx": "0x252b6c9000b",
          "R11": "0x0",
          "Rsp": "0xfc6bdafc38",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 32420,
          "code": "0xc0000005",
          "exceptionAddress": "0x252b6c90012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        },
        {
          "Rip": "0x252b6c90012",
          "Rax": "0x252b6c9000b",
          "Rcx": "0x58",
          "Rdx": "0x252b6c9000b",
          "R11": "0x0",
          "Rsp": "0xfc6bdafc38",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 32420,
          "code": "0xc0000005",
          "exceptionAddress": "0x252b6c90012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        }
      ],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xfc6b5ef1b0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 4976,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x252b6c9000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x252b62745e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xfc6b5ef0b0",
          "Rsp": "0xfc6b5ef0a8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 4976,
          "code": "0x80000004",
          "exceptionAddress": "0x252b6c9000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x252b6c90012",
          "Rax": "0x252b6c9000b",
          "Rcx": "0x58",
          "Rdx": "0x252b6c9000b",
          "R11": "0x0",
          "Rsp": "0xfc6bdafc38",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 32420,
          "code": "0xc0000005",
          "exceptionAddress": "0x252b6c90012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        },
        {
          "Rip": "0x252b6c90012",
          "Rax": "0x252b6c9000b",
          "Rcx": "0x58",
          "Rdx": "0x252b6c9000b",
          "R11": "0x0",
          "Rsp": "0xfc6bdafc38",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 32420,
          "code": "0xc0000005",
          "exceptionAddress": "0x252b6c90012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x17c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x174",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x264",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x258",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x250",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x22c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x188",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x240",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x230",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x22c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x188",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x188",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x234",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 28408,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28408,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28408,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28408,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 32420,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 32420,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 32420,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 36212,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 28408,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 4976,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 32420,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 1,
        "avLast": 1,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 40840,
      "primaryTid": 4976,
      "creationFiletime": "134374901094681406",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 4,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x252b6c9000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "expectedAvTid": 32420,
      "exitCode": "0xc0000005",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.234,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": true,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x25f30b5000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x25f30b845e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xeb831ef3a0",
          "Rsp": "0xeb831ef398",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 18136,
          "code": "0x80000004",
          "exceptionAddress": "0x25f30b5000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xeb831ef4a0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 18136,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x25f30b5000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x25f30b845e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xeb831ef3a0",
          "Rsp": "0xeb831ef398",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 18136,
          "code": "0x80000004",
          "exceptionAddress": "0x25f30b5000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x174",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x264",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x238",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x188",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x18c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x19c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x25c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x190",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x258",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x22c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x19c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 22276,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 32168,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 22276,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32168,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32168,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 22276,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 22276,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 22276,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 37668,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 32168,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 18136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 35860,
      "primaryTid": 18136,
      "creationFiletime": "134374901097059452",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeFixtureSeed": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x25f30b5000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "fixtureSeedRestored": true,
      "exitCode": "0x0",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.203,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x230",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x270",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x238",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x264",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x250",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x174",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 5008,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5008,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 5008,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 38892,
      "primaryTid": 5008,
      "creationFiletime": "134374901099228928",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:before-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x27c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x26c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x228",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x284",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x280",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x288",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26356,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26356,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 26356,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 26568,
      "primaryTid": 26356,
      "creationFiletime": "134374901099299473",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:after-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-thread-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 7,
      "continues": 7,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x188",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x288",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x258",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x25c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x18c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x190",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x190",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x190",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26268,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 30908,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 26268,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 30908,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 5056,
      "primaryTid": 26268,
      "creationFiletime": "134374901099364887",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 1,
      "injectionReached": true,
      "error": "fixture-injected:after-thread-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.016,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "marker-before-context",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xbba87eed90",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 20456,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x288",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x258",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x294",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x290",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x188",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x298",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2a0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x260",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38468,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38468,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38468,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33120,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33120,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38468,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 31092,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 20456,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 26948,
      "primaryTid": 20456,
      "creationFiletime": "134374901099542399",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "injectionReached": true,
      "error": "fixture-injected:marker-before-context",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "context-before-set",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x314b7eefc0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 31056,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2a0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x298",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x184",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x18c",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x260",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x28c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x24c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x294",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2a8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2a8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 24048,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24048,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36032,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36032,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36032,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24048,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36032,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24048,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 36032,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 24048,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 6704,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 31056,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 25936,
      "primaryTid": 31056,
      "creationFiletime": "134374901100880407",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "injectionReached": true,
      "error": "fixture-injected:context-before-set",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.203,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-arm",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x92a89ef4e0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 26332,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2a4",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x29c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2b0",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2ac",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x234",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2b4",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x18c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x298",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x18c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x24c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x2c0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2c0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35480,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 9356,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35480,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35480,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35480,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9356,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9356,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 9356,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 39196,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35480,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 26332,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 32960,
      "primaryTid": 26332,
      "creationFiletime": "134374901102979676",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1cab684000a",
      "injectionReached": true,
      "error": "fixture-injected:after-arm",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-exit-continuation",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 46,
      "continues": 46,
      "breakpointHits": [
        {
          "Rip": "0x1b2228f000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1b2229445e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x38bb9ef1b0",
          "Rsp": "0x38bb9ef1a8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 20264,
          "code": "0x80000004",
          "exceptionAddress": "0x1b2228f000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x38bb9ef2b0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 20264,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1b2228f000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1b2229445e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x38bb9ef1b0",
          "Rsp": "0x38bb9ef1a8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 20264,
          "code": "0x80000004",
          "exceptionAddress": "0x1b2228f000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2b4",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x238",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2bc",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2c4",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2c0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2c8",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x184",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x184",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x260",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2b0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x230",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 40,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 41,
          "value": "0x2a0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 36556,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33388,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 40700,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 20264,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 5668,
      "primaryTid": 20264,
      "creationFiletime": "134374901104411933",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1b2228f000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "injectionReached": true,
      "error": "fixture-injected:before-exit-continuation",
      "termination": {
        "ok": false,
        "error": 5
      },
      "exitCode": "0x0",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    }
  ],
  "debuggerExited": true,
  "debuggerExitCode": 0,
  "debuggerStderr": "",
  "pass": true
}

```

Final-source run requiring nonzero seed readback, exact raw JSON:

```json
{
  "schema": "child-debug-event-fixture-v2",
  "python": "3.12.10 (tags/v3.12.10:0cc8128, Apr  8 2025, 12:21:36) [MSC v.1943 64 bit (AMD64)]",
  "scriptSha256": "30a3b30a9accf3b5260e2a6f299f189321beff2644c09d905e54b723acfe0cd2",
  "results": [
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x205ec09000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x205ebf345e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x565e1eee20",
          "Rsp": "0x565e1eee18",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 10888,
          "code": "0x80000004",
          "exceptionAddress": "0x205ec09000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x565e1eef20",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 10888,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x205ec09000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x205ebf345e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x565e1eee20",
          "Rsp": "0x565e1eee18",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 10888,
          "code": "0x80000004",
          "exceptionAddress": "0x205ec09000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x264",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x260",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x26c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x274",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x268",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x278",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x174",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x218",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x278",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 6488,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6488,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6488,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 6488,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 25620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 37024,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 10888,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 5528,
      "primaryTid": 10888,
      "creationFiletime": "134374901334227155",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x205ec09000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "exitCode": "0x0",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "av",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 45,
      "continues": 45,
      "breakpointHits": [
        {
          "Rip": "0x2195819000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x219577c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xe1dffef2f0",
          "Rsp": "0xe1dffef2e8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 29812,
          "code": "0x80000004",
          "exceptionAddress": "0x2195819000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [
        {
          "Rip": "0x21958190012",
          "Rax": "0x2195819000b",
          "Rcx": "0x58",
          "Rdx": "0x2195819000b",
          "R11": "0x0",
          "Rsp": "0xe1e07afa28",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 1568,
          "code": "0xc0000005",
          "exceptionAddress": "0x21958190012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        },
        {
          "Rip": "0x21958190012",
          "Rax": "0x2195819000b",
          "Rcx": "0x58",
          "Rdx": "0x2195819000b",
          "R11": "0x0",
          "Rsp": "0xe1e07afa28",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 1568,
          "code": "0xc0000005",
          "exceptionAddress": "0x21958190012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        }
      ],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xe1dffef3f0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 29812,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x2195819000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x219577c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xe1dffef2f0",
          "Rsp": "0xe1dffef2e8",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 29812,
          "code": "0x80000004",
          "exceptionAddress": "0x2195819000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x21958190012",
          "Rax": "0x2195819000b",
          "Rcx": "0x58",
          "Rdx": "0x2195819000b",
          "R11": "0x0",
          "Rsp": "0xe1e07afa28",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 1568,
          "code": "0xc0000005",
          "exceptionAddress": "0x21958190012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        },
        {
          "Rip": "0x21958190012",
          "Rax": "0x2195819000b",
          "Rcx": "0x58",
          "Rdx": "0x2195819000b",
          "R11": "0x0",
          "Rsp": "0xe1e07afa28",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 1568,
          "code": "0xc0000005",
          "exceptionAddress": "0x21958190012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x260",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x26c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x274",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x268",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x264",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x288",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x218",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x174",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x244",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x168",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x29c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x16c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x298",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x184",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 36088,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36088,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 36088,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 1568,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 1568,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 1568,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35220,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 1568,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35592,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 29812,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 36088,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 1,
        "avLast": 1,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 35720,
      "primaryTid": 29812,
      "creationFiletime": "134374901335661216",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 4,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x2195819000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "expectedAvTid": 1568,
      "exitCode": "0xc0000005",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": true,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x3f61deeff0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 16428,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x29c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x298",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x264",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x218",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2a0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x184",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x184",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x214",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2a8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x17c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 14208,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 14208,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 14208,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 14208,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 14208,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 16428,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 38748,
      "primaryTid": 16428,
      "creationFiletime": "134374901337216407",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "error": "fixture seed was not retained by Windows",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": false,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": false
    },
    {
      "mode": "normal",
      "injection": "before-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x298",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x264",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2a4",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2ac",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x29c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2b0",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 5900,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5900,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 5900,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 29548,
      "primaryTid": 5900,
      "creationFiletime": "134374901338584461",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:before-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.016,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2a0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x218",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x26c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x260",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x278",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2a4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 30040,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30040,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 30040,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 34844,
      "primaryTid": 30040,
      "creationFiletime": "134374901338655980",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:after-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-thread-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 7,
      "continues": 7,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2ac",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x230",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x174",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x260",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x29c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x274",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 6804,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35160,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 6804,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 22476,
      "primaryTid": 35160,
      "creationFiletime": "134374901338728190",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 1,
      "injectionReached": true,
      "error": "fixture-injected:after-thread-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.016,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "marker-before-context",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xb3805ff3c0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 6484,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x218",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x26c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x278",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x294",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2a0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2b0",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2b0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x224",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2b4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2c0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x228",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2ac",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x238",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 13728,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 13728,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 13728,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 13728,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 42844,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 6484,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 25396,
      "primaryTid": 6484,
      "creationFiletime": "134374901338903814",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "injectionReached": true,
      "error": "fixture-injected:marker-before-context",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "context-before-set",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xb6c2bef360",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 8828,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x268",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x228",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x244",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x178",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x16c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x220",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x294",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x278",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x294",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x220",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2c0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x2b0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x2b8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2bc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 31620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38432,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38432,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38432,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38432,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31620,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38432,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 9180,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 31620,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 35868,
      "primaryTid": 8828,
      "creationFiletime": "134374901340257659",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "injectionReached": true,
      "error": "fixture-injected:context-before-set",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-arm",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xddab1ef160",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 28680,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2c0",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x2bc",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2c8",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2cc",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x2c4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2d0",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2d4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2b0",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x2d8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x224",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2e0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2d0",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x2b4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x164",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 26464,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26464,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 26464,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 26464,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33308,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38672,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 12808,
      "primaryTid": 28680,
      "creationFiletime": "134374901341613456",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x2bb1013000a",
      "injectionReached": true,
      "error": "fixture-injected:after-arm",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-exit-continuation",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x2d81532000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x2d8148c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xebb03ef010",
          "Rsp": "0xebb03ef008",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 21916,
          "code": "0x80000004",
          "exceptionAddress": "0x2d81532000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xebb03ef110",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 21916,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x2d81532000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x2d8148c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xebb03ef010",
          "Rsp": "0xebb03ef008",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 21916,
          "code": "0x80000004",
          "exceptionAddress": "0x2d81532000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x2dc",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x2d0",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x2b4",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x2e0",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x164",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x2e4",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x2d4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x2cc",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x2d4",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x2cc",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x284",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x2c4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x2c8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x2e4",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x2d8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 25508,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 33680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 21916,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 25552,
      "primaryTid": 21916,
      "creationFiletime": "134374901342964204",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x2d81532000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "injectionReached": true,
      "error": "fixture-injected:before-exit-continuation",
      "termination": {
        "ok": false,
        "error": 5
      },
      "exitCode": "0x0",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.125,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    }
  ],
  "debuggerExited": true,
  "debuggerExitCode": 0,
  "debuggerStderr": "",
  "pass": false
}

```

FINAL SOURCE: core fixtures, failure cleanup, and explicit seed-rejection control, exact raw JSON:

```json
{
  "schema": "child-debug-event-fixture-v2",
  "python": "3.12.10 (tags/v3.12.10:0cc8128, Apr  8 2025, 12:21:36) [MSC v.1943 64 bit (AMD64)]",
  "scriptSha256": "0aea50992e9e6749f9d5f4db7092710d61ca159e1622f7fd65c275e273ed31ee",
  "results": [
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x1e157d5000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1e157db45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x2e7fdeef30",
          "Rsp": "0x2e7fdeef28",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 35720,
          "code": "0x80000004",
          "exceptionAddress": "0x1e157d5000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x2e7fdef030",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 35720,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1e157d5000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1e157db45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x2e7fdeef30",
          "Rsp": "0x2e7fdeef28",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 35720,
          "code": "0x80000004",
          "exceptionAddress": "0x1e157d5000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x210",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x214",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x218",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x220",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x23c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x21c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0xa8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x21c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0xc8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x250",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x234",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0xa8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0xa8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23284,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 4240,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 38452,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 35720,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 36660,
      "primaryTid": 35720,
      "creationFiletime": "134374901783870060",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1e157d5000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "exitCode": "0x0",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "natural-fixture",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.14,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "av",
      "injection": null,
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 45,
      "continues": 45,
      "breakpointHits": [
        {
          "Rip": "0x1d4a8b2000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1d4a89c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xaa94bef310",
          "Rsp": "0xaa94bef308",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 23152,
          "code": "0x80000004",
          "exceptionAddress": "0x1d4a8b2000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [
        {
          "Rip": "0x1d4a8b20012",
          "Rax": "0x1d4a8b2000b",
          "Rcx": "0x58",
          "Rdx": "0x1d4a8b2000b",
          "R11": "0x0",
          "Rsp": "0xaa953af7d8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 20680,
          "code": "0xc0000005",
          "exceptionAddress": "0x1d4a8b20012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        },
        {
          "Rip": "0x1d4a8b20012",
          "Rax": "0x1d4a8b2000b",
          "Rcx": "0x58",
          "Rdx": "0x1d4a8b2000b",
          "R11": "0x0",
          "Rsp": "0xaa953af7d8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 20680,
          "code": "0xc0000005",
          "exceptionAddress": "0x1d4a8b20012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58"
        }
      ],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xaa94bef410",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 23152,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1d4a8b2000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1d4a89c45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0xaa94bef310",
          "Rsp": "0xaa94bef308",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 23152,
          "code": "0x80000004",
          "exceptionAddress": "0x1d4a8b2000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1d4a8b20012",
          "Rax": "0x1d4a8b2000b",
          "Rcx": "0x58",
          "Rdx": "0x1d4a8b2000b",
          "R11": "0x0",
          "Rsp": "0xaa953af7d8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 20680,
          "code": "0xc0000005",
          "exceptionAddress": "0x1d4a8b20012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        },
        {
          "Rip": "0x1d4a8b20012",
          "Rax": "0x1d4a8b2000b",
          "Rcx": "0x58",
          "Rdx": "0x1d4a8b2000b",
          "R11": "0x0",
          "Rsp": "0xaa953af7d8",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": false,
          "tid": 20680,
          "code": "0xc0000005",
          "exceptionAddress": "0x1d4a8b20012",
          "classification": "application-av",
          "parameterCount": 2,
          "operation": 0,
          "address": "0x58",
          "disposition": "0x80010001"
        }
      ],
      "forcedCleanup": false,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x234",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x254",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0xa8",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x244",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x174",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x220",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x24c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x258",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x210",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x264",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x25c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x26c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41716,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41716,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41716,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 20680,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 20680,
          "disposition": "0x80010001",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 11144,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 23152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41716,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 20680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 35288,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 1,
        "avLast": 1,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 38748,
      "primaryTid": 23152,
      "creationFiletime": "134374901785336751",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 4,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1d4a8b2000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "expectedAvTid": 20680,
      "exitCode": "0xc0000005",
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "natural-fixture",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.156,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": null,
      "seededDebugState": true,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xe51d7ef0c0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 4952,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x220",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x25c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x21c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x254",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0xa8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x234",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x23c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x23c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x234",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x244",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x244",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x270",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x260",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 18876,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41072,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41876,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 4952,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 9340,
      "primaryTid": 4952,
      "creationFiletime": "134374901786978816",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "seedReadback": {
        "Dr0": "0x0",
        "Dr1": "0x0",
        "Dr2": "0x0",
        "Dr7": "0x0"
      },
      "fixtureSeedRestored": true,
      "error": "fixture seed was not retained by Windows",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "seed-preservation-or-explicit-rejection",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x26c",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0xc8",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x258",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x174",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x210",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x24c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 25112,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25112,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 25112,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 31660,
      "primaryTid": 25112,
      "creationFiletime": "134374901788422125",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:before-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.015,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-create-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 3,
      "continues": 3,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x258",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x250",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x26c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x174",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0xc8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x210",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 8828,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 21332,
      "primaryTid": 8828,
      "creationFiletime": "134374901788491955",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "injectionReached": true,
      "error": "fixture-injected:after-create-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.0,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-thread-event",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 7,
      "continues": 7,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x278",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x274",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x280",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x284",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x27c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x288",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x210",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 39152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39152,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 23372,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 39152,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 23372,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 0,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 37252,
      "primaryTid": 39152,
      "creationFiletime": "134374901788557512",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 1,
      "injectionReached": true,
      "error": "fixture-injected:after-thread-event",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.016,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "marker-before-context",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x7bd9eee10",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 30348,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x248",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x27c",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x250",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x210",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x284",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x258",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x258",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x174",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x280",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0xc8",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x290",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x274",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 37988,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37988,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 37988,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 35428,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 37988,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 40544,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 30348,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 25508,
      "primaryTid": 30348,
      "creationFiletime": "134374901788734766",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "injectionReached": true,
      "error": "fixture-injected:marker-before-context",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.156,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "context-before-set",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x66d2beeff0",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 32996,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x284",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x210",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x278",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x268",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x274",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x270",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x21c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x240",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x270",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x220",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x254",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x26c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x174",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x250",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x248",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 39188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 39188,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 39188,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 28680,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 21884,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 32996,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 42060,
      "primaryTid": 32996,
      "creationFiletime": "134374901790198698",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "injectionReached": true,
      "error": "fixture-injected:context-before-set",
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.14,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "after-arm",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 40,
      "continues": 40,
      "breakpointHits": [],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0xebe7eef60",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 31052,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x274",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x268",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x218",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x248",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x26c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x214",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x278",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x278",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x174",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x27c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x288",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x28c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0x214",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0x210",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 25860,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25860,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 25860,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 24136,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 25860,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 41092,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 31052,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 0,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 20988,
      "primaryTid": 31052,
      "creationFiletime": "134374901791788452",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1daf3b5000a",
      "injectionReached": true,
      "error": "fixture-injected:after-arm",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "termination": {
        "ok": true,
        "error": null
      },
      "exitCode": "0xe210",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    },
    {
      "mode": "normal",
      "injection": "before-exit-continuation",
      "seededDebugState": false,
      "originalLaunched": false,
      "guiControlAcquired": false,
      "events": 42,
      "continues": 42,
      "breakpointHits": [
        {
          "Rip": "0x1df8d8e000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1df8dae45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x2e0fdeed90",
          "Rsp": "0x2e0fdeed88",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 6264,
          "code": "0x80000004",
          "exceptionAddress": "0x1df8d8e000a",
          "classification": "owned-hardware-breakpoint"
        }
      ],
      "accessViolations": [],
      "exceptions": [
        {
          "Rip": "0x7ffb929626ca",
          "Rax": "0x0",
          "Rcx": "0x7ffb929a21f4",
          "Rdx": "0x0",
          "R11": "0x246",
          "Rsp": "0x2e0fdeee90",
          "Dr6": "0x0",
          "Dr7": "0x0",
          "firstChance": true,
          "tid": 6264,
          "code": "0x80000003",
          "exceptionAddress": "0x7ffb929626c9",
          "classification": "qualified-loader-breakpoint",
          "disposition": "0x10002"
        },
        {
          "Rip": "0x1df8d8e000a",
          "Rax": "0x1122334455667788",
          "Rcx": "0x1df8dae45e0",
          "Rdx": "0x7ffb555e8bc9",
          "R11": "0x2e0fdeed90",
          "Rsp": "0x2e0fdeed88",
          "Dr6": "0xffff0ff1",
          "Dr7": "0x401",
          "firstChance": true,
          "tid": 6264,
          "code": "0x80000004",
          "exceptionAddress": "0x1df8d8e000a",
          "classification": "owned-hardware-breakpoint",
          "disposition": "0x10002"
        }
      ],
      "forcedCleanup": true,
      "exitObserved": true,
      "handleLedger": [
        {
          "generation": 1,
          "value": "0x258",
          "kind": "caller-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 2,
          "value": "0x214",
          "kind": "caller-thread",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 3,
          "value": "0x26c",
          "kind": "retained-process",
          "owner": "caller",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 4,
          "value": "0x278",
          "kind": "event-process",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 5,
          "value": "0x248",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:process-exit",
          "invalidImmediately": true
        },
        {
          "generation": 6,
          "value": "0x27c",
          "kind": "image-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 7,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 8,
          "value": "0x284",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 9,
          "value": "0x218",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 10,
          "value": "0x284",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 11,
          "value": "0x28c",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 12,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 13,
          "value": "0x218",
          "kind": "event-thread",
          "owner": "event",
          "closed": true,
          "closure": "ContinueDebugEvent:thread-exit",
          "invalidImmediately": true
        },
        {
          "generation": 14,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 15,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 16,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 17,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 18,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 19,
          "value": "0x220",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 20,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 21,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 22,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 23,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 24,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 25,
          "value": "0x280",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 26,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 27,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 28,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 29,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 30,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 31,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 32,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 33,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 34,
          "value": "0x268",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 35,
          "value": "0x27c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 36,
          "value": "0x240",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 37,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 38,
          "value": "0xc8",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        },
        {
          "generation": 39,
          "value": "0x24c",
          "kind": "dll-file",
          "owner": "manual",
          "closed": true,
          "closure": "CloseHandle",
          "invalidImmediately": true
        }
      ],
      "continuationLedger": [
        {
          "code": 3,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 27000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 2,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 27000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 27000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 7,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 8,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 1,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 6,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 27000,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 21156,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 4,
          "tid": 5772,
          "disposition": "0x10002",
          "cleanup": false,
          "ok": true,
          "error": null
        },
        {
          "code": 5,
          "tid": 6264,
          "disposition": "0x10002",
          "cleanup": true,
          "ok": true,
          "error": null
        }
      ],
      "counters": {
        "loader": 1,
        "hardware": 1,
        "avFirst": 0,
        "avLast": 0,
        "unknown": 0
      },
      "cleanupErrors": [],
      "pid": 28692,
      "primaryTid": 6264,
      "creationFiletime": "134374901793185744",
      "image": "C:\\Program Files\\Python312\\python.exe",
      "loaderModule": {
        "path": "\\\\?\\C:\\Windows\\System32\\ntdll.dll",
        "sha256": "651ac9041af8e6b5ebd331d53163244c0a27d36d1433f0f15cac1f28c5f2e82c",
        "base": "0x7ffb92840000",
        "rva": "0x1226c9",
        "bytes": "0a807c2440007503cceb004883c438c3cc"
      },
      "loaderBreakpointExpected": "0x7ffb929626c9",
      "additionalThreadCount": 3,
      "debugStateBeforeArm": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "breakpointAddress": "0x1df8d8e000a",
      "debugStateAfterRestore": {
        "Dr0": 0,
        "Dr1": 0,
        "Dr2": 0,
        "Dr3": 0,
        "Dr6": 0,
        "Dr7": 0,
        "resumeFlag": 0
      },
      "debugStateRestored": true,
      "injectionReached": true,
      "error": "fixture-injected:before-exit-continuation",
      "termination": {
        "ok": false,
        "error": 5
      },
      "exitCode": "0x0",
      "cleanupExitObserved": true,
      "ownedProcessSignaled": true,
      "handlesClosed": true,
      "liveHandleGenerations": [],
      "expectedOutcome": "injected-cleanup",
      "casePassedBeforeIndependentAbsence": true,
      "elapsedSeconds": 0.141,
      "independentPidAbsence": {
        "absent": true,
        "attempts": [
          {
            "openError": 87
          }
        ]
      },
      "pass": true
    }
  ],
  "debuggerExited": true,
  "debuggerExitCode": 0,
  "debuggerStderr": "",
  "pass": true
}

```
