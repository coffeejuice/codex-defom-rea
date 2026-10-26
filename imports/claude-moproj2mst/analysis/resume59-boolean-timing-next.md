# CP59: smallest new Boolean timing evidence and next seam

New bounded immutable PE selections close the posted-wake consumer/serial gate missing from the earlier wake-prefix analysis. They explain where postedWake can reset and why an internal0x401 or fallback timer message can request posted-event dispatch. They do not recover the uncaptured startup HWND/serial/postedWake values, establish Boolean timer readiness or permit nested callbacks to be flattened.

Only this report was written. No source/shared state, build, original runtime, GUI control or mutable REA operation was used. Static discovery used a bounded QtCore text-region displacement search followed by instruction-aligned DUMPBIN selections; matching displacement bytes alone were not used as behavioral evidence.

## Exact new evidence

| Address | Established selected behavior | Limit |
| --- | --- | --- |
| Core67142665..671426b0 | Private initialization zeroes +98 HWND, +a0 hook, +a8 serial, +ac processed-serial comparison field, +b0 fallback timer and +b4 postedWake. | Constructor-stage zeros do not establish their later startup/posting values. Existing startup004 queue19->20 already has intervening application history. |
| Core671441ed..6714423b | Exported createInternalHwnd671441d0 checks/creates HWND, then installs SetWindowsHookExW with raw type3, callback67141e40, null module and current thread ID. Hook result is stored private+a0; null result reaches a diagnostic. | Actual HWND/hook creation success and registration lifetime remain runtime inputs. |
| Core67141e40..67141f58 | The selected thread-hook callback, when its second argument is1, resolves current dispatcher via6710c570/meta-cast6711ddd0, snapshots serial+a8 and calls GetQueueStatus(0x417). On highword0 it optionally kills private+b0 timer, sets+b0=0 and atomically exchanges postedWake+b4 with0. | This is conditional reset during message retrieval, not an unconditional wake-body store. Queue-status/API results and actual callback occurrence are unknown. |
| Core67141ed6..67141f33 | After that reset, serial snapshot differing from+ac can repost0x401 unless the current message already has the same internal HWND/type0x401. On nonzero queue-status highword, no fallback timer and serial mismatch cause SetTimer(HWND,-2,0,null); returned ID is stored+b0. | Do not replace callback/API outcomes with constants or treat every WM_TIMER as a Boolean timeout. |
| Core67142908..6714296e | Internal procedure routes message0x401 to a serial gate. WM_TIMER0x113 routes there only when nonzero private+b0 equals wParam; otherwise it delegates registered-timer handling67141ff0. The gate loads+a8, compares+ac, stores+ac=a8 before invoking sendPostedEvents6711b040(null,0,private+40) on mismatch. Equal serials return0 without that dispatch. | Generic posted-event dispatch is not QSingleShotTimer delivery or the modelUpdated callback. Native filters/userdata/thread/queue/callback effects are still required. |

The internal message procedure67142750 is source-linked by create helper671429c0: it loads the procedure address at671429e3, places it in the window-class procedure slot at67142a79, registers the class at67142ad9, creates a window at67142b18, then stores its actual dispatcher argument in WindowLongPtr index-21 at67142b45. The procedure retrieves that same index at671427ef and loads the dispatcher private+8 at671427ff. The selected branch below requires the actual source-admitted userdata/private owner; a missing or unknown owner is not a safe zero/no-op substitute.

The procedure's application/native-filter prefix6714279d..671427e2 can return a filter result before serial handling. That callback population and application state remain unclosed for a public admission profile. The code regions here are bounded selections, not a complete callback/filter or Win32 API implementation.

## Why this matters for actual timing

The source now distinguishes four phases:

1. wakeUp67141b70 increments+a8, optionally CAS-sets+b4 and posts0x401.
2. A type3 thread hook may reset+b4 and arrange/repost an internal notification, depending on current queue status and serial comparison.
3. The internal0x401/fallback-timer procedure compares+a8/+ac, advances+ac before requesting sendPostedEvents.
4. sendPostedEvents separately clears event/count/tombstone under lock, unlocks, notifies and deletes. A Boolean callback still requires the selected actual QSingleShotTimer timerEvent67134ca0 -> modelUpdated183160 -> fit/current-item/click chain.

In particular, a0x113 message whose wParam matches private+b0 is a fallback notification for generic posted-event processing, not evidence that a registered Boolean1000ms timer fired. Capturing the actual QSingleShotTimer object/generation/native ID and timerEvent context remains the correct readiness boundary. Serial difference is a coalescing generation test, not a count of timers, messages or Boolean applies.

Updating+ac before sendPostedEvents matters on refusal/reentry: a reconstruction must retain that mutation if dispatch reaches an unclosed effect. It must not roll it back merely because a Boolean effect is unsupported. A later wake can change+a8 during callbacks; subsequent message handling can therefore request another pass. This is a concrete possible reentrant path, not proof that a particular captured run nested. The binder's nested/overlapping-frame guard must remain until an explicit hierarchy is represented.

## Values still unknown and minimal future observation

Startup004 never read private HWND/a8/b4; those contents remain unavailable. This new source adds the stage-qualified need for private+ac, private+b0, GetQueueStatus return highword, actual hook registration/result, current-message HWND/type/wParam and application/native-filter admission when studying consumption rather than only wake entry. Native constructor zeros cannot seed them later. Historical process addresses cannot be read from a fresh process.

For the narrow original wake-field gap, the existing corrected probe004 recognizer plus the three fields at its current wake entry/return remains the smallest capture; this report does not expand that prerequisite unnecessarily. For consumption/timing, root may add only the selected internal-gate entry/dispatch sites67142949/67142968 and selected QSingleShotTimer timerEvent context. If the hook-reset branch itself is the uncertainty, add67141e8f/67141ecb with the source-stage queue-status result and same private owner. No broad memory dump, synthetic pump, HWND message injection or GUI interaction is authorized here. Any future capture follows the established owned-session/control-release gate.

## Safe next implementation action

A bounded **internal posted-event serial gate** is now source-ready as an isolated candidate primitive on an explicitly supplied same dispatcher/private/thread owner, after the actual application/filter/userdata preconditions are admitted. Inputs are source-stage serial+a8, processed comparison+ac, message classification and same actual threadData identity. Unknowns remain unknown.

For admitted0x401 (or explicitly matching fallback timer), equal serials return with no queue-dispatch request; unequal serials write processedSerial=serial, then emit a guarded request for sendPostedEvents(null,0,sameThreadData). Stop there if delivery is unimplemented, retaining the processed-serial store. This primitive must not call receiver-removal as a substitute, create a timer, fill a DueObservation or invoke modelUpdated. Ordinary registered-timer0x113 messages and unknown filters/userdata/API state stay guarded/delegated.

Candidate-only tests can cover equal versus unequal32-bit serials, update-before-unavailable-dispatch retention, repeated call idempotency until a new serial, owner substitution rejection and explicit fallback-versus-registered-timer classification. Use declared synthetic inputs; do not assign these values to the public startup profile. A constructor initializer primitive could separately represent the proven zero stores only at the actual private-construction point, never as current wake state.

No implementation was made. Keep PostPending, native readiness, full fit/click effects and nested callback replay guarded. The Session-owned typed binder bridge validates evidence ownership; it does not supply any of these new native state values.

## Retained source identity and bounded selections

UTC: 2026-10-24T20:48:47.581340+00:00. Immutable DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; DUMPBIN SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`. Import/export labels are selected from `analysis/wave3-closure-next-close-widget-post-append-remove-static.json` (SHA-256 `3e848c48661e894390cd82daa4b5d76e367b8ba756c5245d805066eb99161b4e`).

- `0x671a83a0`: `GetCurrentThreadId`.
- `0x671a87e0`: `KillTimer`.
- `0x671a87e8`: `GetQueueStatus`.
- `0x671a87f0`: `DefWindowProcW`.
- `0x671a87f8`: `GetWindowLongPtrW`.
- `0x671a8830`: `SetWindowsHookExW`.
- `0x671a8858`: `PostMessageW`.
- `0x671a8870`: `CallNextHookEx`.
- `0x671a8878`: `SetTimer`.

### 67142665..671426b1 exclusive

Explicit bounded instruction interval; not whole logical function coverage. Code SHA-256 `458b12f123e947bfbf022d5780b3c5c0f0e2131ff8972aa6b381bbf3988fe8bd`. Command: `/disasm:bytes /range:0x67142665,0x671426b0 evidence/original-binaries/QtCore4_64.dll`.

```text
  0000000067142665: 33 FF              xor         edi,edi
  0000000067142667: 48 89 BB 88 00 00  mov         qword ptr [rbx+0000000000000088h],rdi
                    00
  000000006714266E: 48 8D 05 43 D4 0F  lea         rax,[000000006723FAB8h]
                    00
  0000000067142675: 48 89 03           mov         qword ptr [rbx],rax
  0000000067142678: FF 15 22 5D 06 00  call        qword ptr [00000000671A83A0h]
  000000006714267E: 89 83 90 00 00 00  mov         dword ptr [rbx+0000000000000090h],eax
  0000000067142684: 40 88 BB 94 00 00  mov         byte ptr [rbx+0000000000000094h],dil
                    00
  000000006714268B: 48 89 BB 98 00 00  mov         qword ptr [rbx+0000000000000098h],rdi
                    00
  0000000067142692: 48 89 BB A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rdi
                    00
  0000000067142699: 89 BB A8 00 00 00  mov         dword ptr [rbx+00000000000000A8h],edi
  000000006714269F: 89 BB AC 00 00 00  mov         dword ptr [rbx+00000000000000ACh],edi
  00000000671426A5: 89 BB B0 00 00 00  mov         dword ptr [rbx+00000000000000B0h],edi
  00000000671426AB: 89 BB B4 00 00 00  mov         dword ptr [rbx+00000000000000B4h],edi
```

### 671441ed..6714423c exclusive

Explicit bounded instruction interval; not whole logical function coverage. Code SHA-256 `0d8dd9e52ce38da3d15b9a2e08a18bdda5f1dc41af8ed3a405cf696034aeebd5`. Command: `/disasm:bytes /range:0x671441ed,0x6714423b evidence/original-binaries/QtCore4_64.dll`.

```text
  00000000671441ED: 48 83 BE 98 00 00  cmp         qword ptr [rsi+0000000000000098h],0
                    00 00
  00000000671441F5: 0F 85 7F 02 00 00  jne         000000006714447A
  00000000671441FB: E8 C0 E7 FF FF     call        00000000671429C0
  0000000067144200: 48 89 86 98 00 00  mov         qword ptr [rsi+0000000000000098h],rax
                    00
  0000000067144207: FF 15 93 41 06 00  call        qword ptr [00000000671A83A0h]
  000000006714420D: 44 8B C8           mov         r9d,eax
  0000000067144210: 45 33 C0           xor         r8d,r8d
  0000000067144213: 48 8D 15 26 DC FF  lea         rdx,[0000000067141E40h]
                    FF
  000000006714421A: 41 8D 48 03        lea         ecx,[r8+3]
  000000006714421E: FF 15 0C 46 06 00  call        qword ptr [00000000671A8830h]
  0000000067144224: 48 89 86 A0 00 00  mov         qword ptr [rsi+00000000000000A0h],rax
                    00
  000000006714422B: 48 85 C0           test        rax,rax
  000000006714422E: 75 0C              jne         000000006714423C
  0000000067144230: 48 8D 0D 29 BE 0F  lea         rcx,[0000000067240060h]
                    00
  0000000067144237: E8 84 8F ED FF     call        000000006701D1C0
```

### 67141e40..67141f59 exclusive

Explicit bounded instruction interval; not whole logical function coverage. Code SHA-256 `47fea5b855d7755b6f79bbef8a712ac98c77179b0f688bfc070e58736c6ace48`. Command: `/disasm:bytes /range:0x67141e40,0x67141f58 evidence/original-binaries/QtCore4_64.dll`.

```text
  0000000067141E40: 40 56              push        rsi
  0000000067141E42: 41 54              push        r12
  0000000067141E44: 41 55              push        r13
  0000000067141E46: 48 83 EC 20        sub         rsp,20h
  0000000067141E4A: 49 8B F0           mov         rsi,r8
  0000000067141E4D: 4C 8B E2           mov         r12,rdx
  0000000067141E50: 44 8B E9           mov         r13d,ecx
  0000000067141E53: 48 83 FA 01        cmp         rdx,1
  0000000067141E57: 0F 85 E1 00 00 00  jne         0000000067141F3E
  0000000067141E5D: 33 C9              xor         ecx,ecx
  0000000067141E5F: E8 0C A7 FC FF     call        000000006710C570
  0000000067141E64: 48 8D 0D F5 6E 11  lea         rcx,[0000000067258D60h]
                    00
  0000000067141E6B: 48 8B D0           mov         rdx,rax
  0000000067141E6E: E8 5D BF FD FF     call        000000006711DDD0
  0000000067141E73: 48 85 C0           test        rax,rax
  0000000067141E76: 0F 84 C2 00 00 00  je          0000000067141F3E
  0000000067141E7C: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  0000000067141E81: 48 8B 58 08        mov         rbx,qword ptr [rax+8]
  0000000067141E85: B9 17 04 00 00     mov         ecx,417h
  0000000067141E8A: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  0000000067141E8F: 8B AB A8 00 00 00  mov         ebp,dword ptr [rbx+00000000000000A8h]
  0000000067141E95: FF 15 4D 69 06 00  call        qword ptr [00000000671A87E8h]
  0000000067141E9B: C1 E8 10           shr         eax,10h
  0000000067141E9E: 66 85 C0           test        ax,ax
  0000000067141EA1: 75 63              jne         0000000067141F06
  0000000067141EA3: 48 63 83 B0 00 00  movsxd      rax,dword ptr [rbx+00000000000000B0h]
                    00
  0000000067141EAA: 48 89 7C 24 50     mov         qword ptr [rsp+50h],rdi
  0000000067141EAF: 33 FF              xor         edi,edi
  0000000067141EB1: 85 C0              test        eax,eax
  0000000067141EB3: 74 16              je          0000000067141ECB
  0000000067141EB5: 48 8B 8B 98 00 00  mov         rcx,qword ptr [rbx+0000000000000098h]
                    00
  0000000067141EBC: 48 8B D0           mov         rdx,rax
  0000000067141EBF: FF 15 1B 69 06 00  call        qword ptr [00000000671A87E0h]
  0000000067141EC5: 89 BB B0 00 00 00  mov         dword ptr [rbx+00000000000000B0h],edi
  0000000067141ECB: 87 BB B4 00 00 00  xchg        edi,dword ptr [rbx+00000000000000B4h]
  0000000067141ED1: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  0000000067141ED6: 3B AB AC 00 00 00  cmp         ebp,dword ptr [rbx+00000000000000ACh]
  0000000067141EDC: 74 56              je          0000000067141F34
  0000000067141EDE: 48 8B 8B 98 00 00  mov         rcx,qword ptr [rbx+0000000000000098h]
                    00
  0000000067141EE5: 48 39 0E           cmp         qword ptr [rsi],rcx
  0000000067141EE8: 75 09              jne         0000000067141EF3
  0000000067141EEA: 81 7E 08 01 04 00  cmp         dword ptr [rsi+8],401h
                    00
  0000000067141EF1: 74 41              je          0000000067141F34
  0000000067141EF3: 45 33 C9           xor         r9d,r9d
  0000000067141EF6: 45 33 C0           xor         r8d,r8d
  0000000067141EF9: BA 01 04 00 00     mov         edx,401h
  0000000067141EFE: FF 15 54 69 06 00  call        qword ptr [00000000671A8858h]
  0000000067141F04: EB 2E              jmp         0000000067141F34
  0000000067141F06: 83 BB B0 00 00 00  cmp         dword ptr [rbx+00000000000000B0h],0
                    00
  0000000067141F0D: 75 25              jne         0000000067141F34
  0000000067141F0F: 3B AB AC 00 00 00  cmp         ebp,dword ptr [rbx+00000000000000ACh]
  0000000067141F15: 74 1D              je          0000000067141F34
  0000000067141F17: 48 8B 8B 98 00 00  mov         rcx,qword ptr [rbx+0000000000000098h]
                    00
  0000000067141F1E: 45 33 C9           xor         r9d,r9d
  0000000067141F21: 45 33 C0           xor         r8d,r8d
  0000000067141F24: 49 8D 51 FE        lea         rdx,[r9-2]
  0000000067141F28: FF 15 4A 69 06 00  call        qword ptr [00000000671A8878h]
  0000000067141F2E: 89 83 B0 00 00 00  mov         dword ptr [rbx+00000000000000B0h],eax
  0000000067141F34: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067141F39: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  0000000067141F3E: 4C 8B CE           mov         r9,rsi
  0000000067141F41: 4D 8B C4           mov         r8,r12
  0000000067141F44: 41 8B D5           mov         edx,r13d
  0000000067141F47: 33 C9              xor         ecx,ecx
  0000000067141F49: 48 83 C4 20        add         rsp,20h
  0000000067141F4D: 41 5D              pop         r13
  0000000067141F4F: 41 5C              pop         r12
  0000000067141F51: 5E                 pop         rsi
  0000000067141F52: 48 FF 25 17 69 06  jmp         qword ptr [00000000671A8870h]
                    00
```

### 67142908..6714296f exclusive

Explicit bounded instruction interval; not whole logical function coverage. Code SHA-256 `d151f53941e8af55fd9c83243de69024e4e36cfd2168ac12f8095ab6be0038a9`. Command: `/disasm:bytes /range:0x67142908,0x6714296e evidence/original-binaries/QtCore4_64.dll`.

```text
  0000000067142908: 81 FE 01 04 00 00  cmp         esi,401h
  000000006714290E: 74 39              je          0000000067142949
  0000000067142910: 81 FE 13 01 00 00  cmp         esi,113h
  0000000067142916: 75 1E              jne         0000000067142936
  0000000067142918: 41 8B 80 B0 00 00  mov         eax,dword ptr [r8+00000000000000B0h]
                    00
  000000006714291F: 85 C0              test        eax,eax
  0000000067142921: 74 05              je          0000000067142928
  0000000067142923: 48 3B D8           cmp         rbx,rax
  0000000067142926: 74 21              je          0000000067142949
  0000000067142928: 8B D3              mov         edx,ebx
  000000006714292A: 49 8B C8           mov         rcx,r8
  000000006714292D: E8 BE F6 FF FF     call        0000000067141FF0
  0000000067142932: 33 C0              xor         eax,eax
  0000000067142934: EB 39              jmp         000000006714296F
  0000000067142936: 4D 8B CC           mov         r9,r12
  0000000067142939: 4C 8B C3           mov         r8,rbx
  000000006714293C: 8B D6              mov         edx,esi
  000000006714293E: 48 8B CD           mov         rcx,rbp
  0000000067142941: FF 15 A9 5E 06 00  call        qword ptr [00000000671A87F0h]
  0000000067142947: EB 26              jmp         000000006714296F
  0000000067142949: 41 8B 80 A8 00 00  mov         eax,dword ptr [r8+00000000000000A8h]
                    00
  0000000067142950: 41 3B 80 AC 00 00  cmp         eax,dword ptr [r8+00000000000000ACh]
                    00
  0000000067142957: 74 14              je          000000006714296D
  0000000067142959: 41 89 80 AC 00 00  mov         dword ptr [r8+00000000000000ACh],eax
                    00
  0000000067142960: 4D 8B 40 40        mov         r8,qword ptr [r8+40h]
  0000000067142964: 33 D2              xor         edx,edx
  0000000067142966: 33 C9              xor         ecx,ecx
  0000000067142968: E8 D3 86 FD FF     call        000000006711B040
  000000006714296D: 33 C0              xor         eax,eax
```

No tests/builds or runtime evidence were added. Token measurements unavailable.
