# Resume 205: static crash-chain analysis

## Scope and identity

This is a static-only interpretation of the same-run capture at `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`. The capture records an access violation at `QtCore4_64.dll!QListData::size` (`RVA 0x32c0` in QtCore), with `RCX=0x58` and faulting read address `0x58`. Its target hash is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; the copied `evidence/original-binaries/DEF_GUI_MO_64.exe` has the same SHA-256. No application run, GUI action, REA session, or mutable database was used for this analysis.

The captured app frames, in unwind order, are `0x4af4, 0xa7377, 0xa7510, 0xeb1ec, 0xeaa7a, 0xdafad, 0x2cf6cc, 0x2a725, 0x1cae`. Return addresses matter: `0xa7377` follows a call at `0xa7372`; `0xa7510` follows a call at `0xa750b`. They are not themselves the faulting instructions.

## Where the null-derived pointer enters QtCore

Bounded DUMPBIN of the copied original gives this exact chain:

```asm
1400A74F0  mov  dword ptr [rsp+10h],edx
1400A74F4  mov  qword ptr [rsp+8],rcx
1400A74F9  sub  rsp,38h
1400A74FD  mov  qword ptr [rsp+28h],0
1400A7506  mov  rcx,qword ptr [rsp+40h]
1400A750B  call 1400A7360
1400A7510  mov  dword ptr [rsp+20h],eax
...
1400A7525  mov  rcx,qword ptr [rsp+40h]
1400A752A  add  rcx,58h
1400A7532  call 14014B5B0
```

(The exact bounded excerpt is at VA `0x1400A74F0` through `0x1400A7550`.)

(These are full VAs with image base `0x140000000`; the relevant caller call/return are RVA `0xA750B/0xA7510`.)

The callee at RVA `0xA7360` disassembles:

```asm
1400A7360  mov  [rsp+8],rcx
1400A7365  sub  rsp,28h
1400A7369  mov  rcx,[rsp+30h]
1400A736E  add  rcx,58h
1400A7372  call 140004AE0
1400A7377  add  rsp,28h
1400A737B  ret

140004AE0  mov  [rsp+8],rcx
140004AE5  sub  rsp,28h
140004AE9  mov  rcx,[rsp+30h]
140004AEE  call qword ptr [141E04210h]
140004AF4  add  rsp,28h
140004AF8  ret
```

`0xA7360` saves and reloads its incoming `RCX` unchanged, then adds `0x58`. The capture's frame return `0xA7377` proves execution returned from the call at `0xA7372` (the wrapper at `0x4AE0`) along this route. In the captured state, QtCore `QListData::size` receives `RCX=0x58`; therefore the wrapper's incoming base `RCX` at `0xA7360` was zero. The base was already null on entry to `0xA7360`; the `+0x58` adjustment at `0xA736E` converted that null owner into the invalid address `0x58`.

The `0xA7510` frame is the return site after `call 0x1400A7360` at `0xA750B`. Its caller has loaded the argument from its own saved parameter slot at `0xA7506`. Thus the null owner is passed into `0xA7360` by the caller's parameter; static code here does not identify the earlier producer or establish why that parameter was null. The return value from `0xA7360` is stored as a 32-bit value at `0xA7510` and then compared with an index. It is not the null pointer and does not establish pointer corruption at the return site.

The first frame, `0x4af4`, is also a return site: it follows an indirect QtCore call at `0x4AEE`. This is consistent with the captured stack crossing the `QListData::size` wrapper, but by itself does not recover a source-level container name.

## Open / notifier / postOpen attribution

The higher captured frames include `0x2cf6cc` and `0x2a725`. At `0x2cf6cc`, the enclosing function has just called through an indirect function pointer at `0x2cf6c6` with a pointer to stack state in `RDX`, `R8D=1`, and a second stack pointer in `R9`; a later indirect call at `0x2cf6db` follows. At `0x2a725`, the disassembly shows the preceding direct call to `0x2cf3c0` returning, then initializes zero arguments and invokes an indirect function pointer at `0x2a732`. These are compatible with an event/notifier route, but these instructions alone do not label the calls as Qt signal delivery or prove that the observed handler is specifically an Open notification's `postOpen` slot.

The same-run launch arguments (`-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`) establish that the process was launched in Open mode. They do not establish that this was the first Open call, nor that the fault occurred in a `notifier postOpen` callback rather than some other post-load path. Static attribution to “first Open/notifier postOpen” is therefore **not proven** by the evidence examined here. The capture's backtrace plus launch mode support only the narrower statement: the process faulted during its Open-mode startup path, in a list-size query using a null owner plus offset `0x58`.

## Evidence and limits

- Capture facts and register/context values: `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`.
- Exact executable identity: SHA-256 in that summary, independently matched by `evidence/original-binaries/DEF_GUI_MO_64.exe`.
- Static instruction evidence: bounded Microsoft DUMPBIN disassembly of the copied executable at VAs `0x1400A7360–0x1400A7390`, `0x1400A74F0–0x1400A7550`, and `0x140004AE0–0x140004B00`; nearby retained selections are `analysis/wave3-closure-next-1400a74f0.asm` and `analysis/wave2-doe-object-writer-slice-list-size.asm`.
- Inference: `RCX=0x58` at `QListData::size` and the wrapper's `add rcx,58h` establish a null base at `0xA7360` entry for this observed stack path.
- Not established: the earlier state transition that made/provided that null, a source-level type/name for the owner, uniqueness/firstness of the Open operation, or exact notifier/slot identity.



