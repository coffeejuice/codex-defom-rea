# CP202 original Save-route probe attempt 2

## Identity and setup

The passive trace identifies PID 20712 as the owned original `DEF_GUI_MO_64.exe` on Sim2, launched with `-m open -f C:\PROBLEM\p202a\Gear_blank.moproj`. Its loaded module path, image size and SHA-256 matched the recorded original identity (`fd8ae1f8…725acc`, 87,420,928 bytes). The helper created it suspended; Frida attached and reported the target and Qt module identities, then the observer armed at 04:45:03.248Z. The helper resumed it once at 04:45:04.511Z (prior suspend count 1). GUI control was not acquired.

## Event ordering and observed facts

Frida messages are sequenced 1–272. Sequences 1–8 are module identity/readiness for the executable, QtCore, and QtGui, followed by `observer_ready` and `armed`. The first dock event is sequence 9 at 364 ms. There are 262 dock events overall, from 364 to 763 ms; most are classified `other`. Seven are explicitly labeled `Hide` (Qt event type 18). These are dock event observations, not a decoded project Save or accepted Close.

At sequence 185 (521 ms), the trace records `prepared_visibility_request` at candidate RVA `0x1547e`, with `requestedRaw: 0` and `boolQualified: true`. The immediately adjacent observations are dock Hide at sequence 186 and `slot_entry` at sequence 187, also at 521 ms, with `value: 0`, `slotId: 1`, and no parent slot. Dock events continue through sequence 272 at 763 ms. No `busy_return`, `auto_save_call`, or `show_signal_call` message appears. Thus a prepared request and one slot entry were observed; busy return, auto-save call, and show-signal call were not observed in this trace. No event is identified as Save.

## Termination, cleanup, and limits

The runner ended with `endpoint: target_exited_or_detached`, `sessionDetachedReason: process-terminated`, `terminal: null`, `captureComplete: false`, and `coverageComplete: false`. Its final coverage lists all six hooks as missing and `observerContinuity: false`, despite the two callback records above; the trace therefore does not establish complete hook coverage or a continuous observation interval. The module readiness records list the expected hooks, but that is readiness metadata, not proof that every relevant event path was captured.

The helper's cleanup attempt failed with `PermissionError(13, 'Access is denied.', ..., 5)` and `exit_verified: false`; the held-handle exit/close receipt is missing/failed. A separate Windows presence query succeeded and reported PID 20712 absent after the endpoint; the same-device Frida absence record is inconclusive (`present: {}`). These facts establish observed process absence at the Windows query time, not the manner or cause of termination. In particular, do not infer accepted Close or natural Save from detachment, termination, or absence: `acceptedClose` is false, and the trace has no terminal event or Save observation.

The capture itself qualifies that interceptors alter code/timing, no GUI input or target API calls were made, GUI visibility/startup was unobserved, the six-point trace cannot prove writer success, unchanged files cannot prove no Save, and directory manifests do not cover external/registry/settings writes. This attempt is therefore partial passive evidence only, not a successful Save-route or dialog-free-startup validation.
