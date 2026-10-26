# CP171 RIO failure-order next cut: source bound audit

## Decision

There is no new source-bound executable cut that advances original RIO parity beyond the retained CP164 candidate-only denial conjunction. The next independent acceptance item is the missing same-run original witness described below. A second test combining already covered failures would only repeat candidate precedence and cannot establish the original GUI outcome. Keep the public candidate refusal and the terminal behavior guarded until the witness exists.

## Original Save effects in order

For original v11.0.2 `DEF_GUI_MO_64.exe` (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), retained static evidence gives this conditional sequence for a validated Type-0 Save:

1. Save active operation (`0x140052ec0`). Its effects may already write operation side files.
2. Write the `.moproj` (`0x1400983c0`). A returned false does not branch around following stages.
3. Visit operation property-save methods (`0x1400982e0`); the loop ignores each individual result. Cogging uses `fopen("wt")`; on null, its writer returns `1` without entering the write body.
4. Write MST and relevant following study output (`0x1400a20d0`). The top-level Save combines project/MST Booleans only after this call; it returns its earlier validation/status local, not that combined Boolean.
5. Run local remesh handling. With remesh enabled and its primary `fopen` failing, the unchecked null stream reaches the first `fprintf`. The CRT stores `errno=22`, invokes this module's invalid-parameter dispatcher with null arguments, and—if the startup-installed null handler slot is still null and the default handler/filter follow the ordinary returning path—requests process termination with `0xC0000417`.

Items 1–4 and the static branch above are source/disassembly claims, not a dynamic trace of the B133-derived scenario. If the conditional default-handler route terminates the process, there is no returned Save status, subsequent remesh formatting/close, or normal Close. The current handler-slot value, remesh-enabled state, resolved Save-time path, actual failed `fopen`, filter result, and child status are not bound to one original run.

## Existing executable coverage and candidate acceptance

CP164 already has the smallest source-closed candidate test in `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp`: one actual `convertDetailedWithServices` call combines a scripted post-close project result of 7, physical MST write-open denial, and enabled-remesh write-open denial. Its acceptance is limited to candidate behavior: prove the pre-truncation remesh lock; preserve MST/remesh sentinels and earlier Boolean BWZ/INI effects; observe the project-output sequence `open → aboutToClose → close returned → error query`; require project/MST attempt-return-false flags; and receive the candidate's `M2M_E_UNSUPPORTED` guard. The event vector does not observe input Open or Save-current. This test does not assert original return status, native termination, native errno, or normal Close.

No distinct source-bound test follows from adding another pair/triple denial or exercising the already modeled worker endpoint: the endpoint is candidate-fixed, and the native default-handler prelude is not executed by it. The earlier writer pairs and worker cases are already separately covered. Further candidate conjunctions therefore fail the independence/parity criterion for this task.

## Exact remaining original witness and acceptance test

The next executable parity cut requires a fresh owned original process on a disposable short-path B133-derived project, with no live mutable REA involvement. A single run must bind all of these in temporal order:

- original executable/module identity and SHA-256;
- successful Open and the loaded remesh-enabled state;
- Save-time current directory and resolved primary `LOCAL_REMESH.DAT` path;
- a write denial proven before the original open attempt to avoid mistaking truncation for no effect;
- actual null `fopen`, first `fprintf`, and module `errno=22`;
- decoded `0x1446FFB40` handler slot at dispatcher entry and selected custom/default branch;
- if default: handler entry, exception record/pointer values, captured RIP/RSP and lookup/unwind branch, debugger state, filter result, and whether `TerminateProcess(0xC0000417)` was reached/returned;
- externally observed child status and before/after raw file manifest.

Accept the original terminal prediction only if these are bound to that one attempt and the trace distinguishes instrumentation effects on debugger/filter behavior. A matching exit status alone is insufficient. If the slot is custom, the filter does not return, the API returns/fails, or the write denial does not establish a pre-truncation failure, retain that profile as unresolved rather than treating it as the default-handler result. Prior project/property/MST outputs must be captured so the observed prefix effects can be compared with the statically established order.

The current GUI release lifecycle blocker means this process experiment is unavailable now. Static retained binary/REA snapshots remain usable; this report does not authorize or perform GUI control, original process execution, or mutable analysis.

## Evidence hashes

| Evidence | SHA-256 |
|---|---|
| `analysis/save-semantics.md` | `02127f105f51de89f9568baaa3241e1da0a42c886a865f3b4e28d47623619c267` |
| `analysis/resume164-rio-next.md` | `6250b6db12a70536d5f4fc3fa6d740ab34216d3d76b652cfa3ebbb3f7fb4fa97` |
| `analysis/resume164-rio-test-review.md` | `435043d2709cfb17d4083cf62ef5edb21b305c25f3e05a5ccd89c5e06c8c64c4` |
| `analysis/resume109-terminal-remesh-worker-failures.md` | `b9f8e276e6294b1cbf80043242d7e5e0bcf3dddddc08382b97d1dd46667650b` |
| `analysis/resume151-remesh-handler-next.md` | `448a6e4effef3a6e7d00a92ec35208cd34fd7f05a06ae3bc8eac9fd6d993f9f5` |
| `analysis/resume103-remesh-terminal-outcome-architecture.md` | `cc474b1ab6203d2196ae5e79010bb84a6785485125c60a3773c09ec683a53f24` |
| `analysis/wave3-remesh-invalid-parameter-startup-contract.json` | `d53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `f3fbc077fc700df96782eef7e621eaf1cba5010712a9b245279b9642c1ec14ce` |
| `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp` | `a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228` |
| `source/apps/moproj2mst/tests/s/test_save_remesh_failure_matrix.cpp` | `3e76588bf5397f6fd9f448408601ca1bd66d0401856bc80aa52d393d8b4b823e` |

The listed source/test hashes identify the inspected files at this audit. Only this report was written; no source, test, build, shared state, original process, GUI or mutable REA action was performed.
