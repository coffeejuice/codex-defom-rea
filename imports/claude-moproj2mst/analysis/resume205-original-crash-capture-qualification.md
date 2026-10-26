# CP205 same-run original startup crash capture

## Source and boundary

The owned-process probe retained the raw run at `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/`. The installed original `DEF_GUI_MO_64.exe` is v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It was launched once, suspended, with `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`; the helper validated its exact process handle, PID 31656, image, arguments, cwd and project hash before direct Frida attach. The injected script registered only `Process.setExceptionHandler` and sent handler-ready before the one resume. It returned `false` on the first access violation. No GUI input or app-control session was acquired.

## Observed fault

At 2026-10-26T08:58:01.858Z the first access violation occurred at QtCore4_64.dll `QListData::size`, loaded base `0x583e0000`, RIP `0x583e32c0`, RVA `0x32c0`. The fault was a **read of address `0x58`**. The saved `RCX=0x58`, consistent with the instruction `mov rax,[rcx]` proved in the earlier static slice. Thus this invocation called the list-size function with an unreadable wrapper pointer at `0x58`. An interpretation as a null base plus `0x58` requires the caller instruction chain; the register alone does not prove an object identity.

The accurate backtrace records app return RVAs `0x4af4`, `0xa7377`, `0xa7510`, `0xeb1ec`, `0xeaa7a`, `0xdafad`, then Qt meta-invocation, app `0x2cf6cc`, `0x2a725`, and `0x1cae`. These belong to this same PID and occurrence. They distinguish the crash from the previously listed candidate paths; interpretation of the intermediate owners and branch predicates remains subject to disassembly. The fault module/RVA matches the earlier CP202 uninstrumented and instrumented crash reports, which lacked this caller context.

The helper then observed natural process exit `3221225477` (`0xC0000005`), with zero termination attempts, exact held-handle exit and handle closure, and helper exit code `0`. Windows and same-device Frida views found the PID absent; Frida detached. The 27-file disposable fixture manifest was byte-identical before and after (`55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`). The run has no recorded failures and reports `CAPTURE_COMPLETE`. The original one-run guard was restored to `false` after the run.

The first guarded attempt at `analysis/resume205-original-crash-stack-2026-10-26T08-56-25-199Z/` remains `INCOMPLETE`. It stopped before attach/resume due a case-sensitive comparison of the helper's canonicalized executable path, then terminated its still-suspended owned child and verified fixture integrity and process absence. That defect was repaired and independently reviewed before the successful second attempt. Neither attempt supplies Save or accepted Close behavior.

## Next action

Resolve the app call chain and receiver provenance at `0xa7377`/`0xa7510`/`0xeb1ec` against copied-binary disassembly and REA. Only then decide whether this B98 Open crash is an in-scope branch to reconstruct through the public m2m ABI and which internal state transition must precede it. Retain raw summary, JSONL, runtime identities and fixture manifests as separate evidence.
