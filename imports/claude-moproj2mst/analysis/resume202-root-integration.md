# CP202 owned original Save-route investigation

## Verified candidate and unresolved scope

CP201 remains the last frozen C++ candidate (`build/wave3-checkpoint201-verified/`): 361 registered tests, 354 clean, six logged gaps, one logged difference, zero failures, skips, incomplete outputs or fixture changes; historical folder comparison remains 93 SAME / five DIFF. No C++ converter behavior was promoted from CP202's incomplete original captures. `admin/check_continuation.py` still reports 15 unfinished workstreams and eight pending completion gates.

## Owned process evidence

Attempt 1 (`analysis/resume202-passive-oracle-run-2026-10-26T04-28-23-604Z/`) failed installing the original executable hook at RVA `0x154a4` before resuming. Its target PID 33884 was terminated through the retained handle and independently found absent. It proves no Open, Save or Close occurrence. The runner's absence aggregate was false because its old Frida detached-state check treated a method as a property; that defect was repaired without changing the retained raw receipt.

The replacement visibility hook at RVA `0x1547e` passed copied-PE instruction/stack review, a synthetic x64 interception/layout/restoration test, and an owned synthetic target-exit/detached lifecycle test with strict successful Windows PID absence plus independent Frida absence. The synthetic cleanup explicitly records script unload as not applicable after target exit, not successful explicit unload. Independent source and lifecycle reviews then accepted one guarded original attempt.

Attempt 2 (`analysis/resume202-passive-oracle-run-2026-10-26T04-45-03-155Z/`) installed all six hooks and resumed original PID 20712 once. Frida recorded 262 dock events, one prepared visibility request (`requestedRaw=0`), and one slot entry (`value=0`) before process detachment. No busy return, auto-save call, Show signal, Save or accepted Close was observed. Windows Application Error 1000 and WER 1001 bind PID 20712 to an access violation in QtCore4_64.dll at RVA `0x32c0` at 04:45:05 UTC; static inspection maps that address to `QListData::size() const`, outside the six hook sites. Whether instrumentation caused the crash remains unknown. The original's 27-file disposable project tree was independently rehashed after the run with no added, deleted or changed files; this does not prove no Save attempt or exclude external settings writes.

Attempt 2 cleanup was incomplete: the held-handle helper emitted `PermissionError(13, Access is denied)` without a verified handle-exit/close receipt. The runner's same-device Frida query still matched PID 20712 at its query time, while its later strict Windows query found the PID absent. A subsequent independent elevated Windows CIM/Get-Process query and REA local Frida listing also found it absent. These later checks establish later absence, not a retroactive successful held-handle receipt. The guard in `admin/frida_original_save_probe.mjs` was returned to false after this attempt. No GUI-control API was acquired, and process termination is not an accepted Close.

The helper's crash/termination race path was repaired to wait on the retained process handle for up to five seconds after `TerminateProcess` fails. It accepts cleanup only if that handle signals, retaining the failed API result and wait receipt; otherwise it fails closed. Nine harmless-child helper tests and independent source review passed. The original probe's pinned helper hash and opaque Frida presence receipt are assigned a separate receipt-only repair, with its execution guard remaining false.

## Next executable actions

1. Investigate the now-reproduced original QtCore fault. Prepare and independently review an owned exception-stack capture with no Interceptor hooks, then run one bounded original process if the reviewed gates pass. Preserve raw fault context and exact cleanup.
2. Use the recovered call stack and input/environment evidence to distinguish an original startup defect from a disposable-fixture or launch-context mismatch. Repair the oracle route only when the same-project behavior is evidenced; do not interpret either crash as Save or accepted Close.
3. Continue independent static/source work for R300, RDOE, RBOOL, RIO, RCLOSE and other queued work while runtime evidence is unresolved. No unsupported public converter path is enabled merely to reduce queue count.

This is a progress checkpoint, not objective completion.

## No-Frida baseline result (later CP202 addition)

The guarded no-Frida baseline launched original PID 37824 with the same executable hash, cwd, B98 project and CLI arguments. The helper created it suspended, resumed once, then observed natural exit code `0xC0000005`; its retained handle reported verified exit, wait result zero and closed process/thread handles. The 27 disposable files and source files remained unchanged. Windows Application Error 1000 and WER 1001 independently identify the same QtCore4_64.dll RVA `0x32c0` access violation as the instrumented PID 20712. This shows Frida hooks are not necessary to reproduce that fault under the recorded baseline conditions, without proving its full cause or all environment equivalence.

The baseline runner's inline Windows absence parser rejected its own `ok absent` output, so its retained `processAbsence` field is null and its result is formally incomplete. A separate elevated Windows CIM/Get-Process query and REA local Frida listing later found exact PID 37824 absent. The parser was repaired and mock-tested without repeating the original launch; the raw baseline receipt was not upgraded. See `analysis/resume202-uninstrumented-baseline-2026-10-26T04-59-10-181Z/`, `analysis/resume202-baseline-crash-evidence.md`, and `analysis/resume202-uninstrumented-baseline-cim-serialization-fix.md`.

The next runtime investigation is a guarded exception-stack capture with no Interceptor hooks, using an owned disposable process. It must pass source/lifecycle review before a new original run. In parallel, continue the other recovery workstreams. Save and accepted Close remain unobserved.
