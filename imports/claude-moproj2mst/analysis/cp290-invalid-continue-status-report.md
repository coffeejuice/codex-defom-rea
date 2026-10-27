# CP290 fixed native invalid-status continuation

The fixed CP282 disposable child established the actual FALSE-return path of
ContinueDebugEvent at its first held CREATE_PROCESS event. After the image gate
and DR0 arm/readback passed, the worker called `abi.cont(pid, tid, 0)` once. It
returned FALSE, and the immediately captured last error was 87. The worker then
terminated its owned child, continued the same event once using the saved
`0x10002` disposition, and drained event codes `[3, 6, 5]`. The supervisor
independently verified both child and worker exits. Worker cleanup completed,
all ledger handles closed, and no event remained pending.

The invalid attempt has its own receipt. It does not masquerade as the ordinary
continuation attempt. An unexpected TRUE marks the event consumed before
cleanup can run. An exception crossing the native call leaves an unknown outcome
and a no-retry guard; cleanup does not guess whether that event was consumed.
Isolated mocked tests cover these FALSE, TRUE and unknown branches. Only the
FALSE branch was observed against the native API in this checkpoint.

Five focused tests passed in 0.175 seconds. The native supervised receipt took
0.172 seconds. The first sandboxed run executed its disposable child but could
not save the receipt in the checkout; the authorized checkout-write rerun
passed. Source hashes remained stable during the successful capture. No build,
original application, GUI control, REA or Frida activity occurred. The literal
original-run HOLD remains enforced. This test establishes the fixed invalid
status failure and owned cleanup only; it does not establish original readiness,
neutrality or overlap.

Receipt: `analysis/cp290-invalid-continue-status.raw.json` (10,693 bytes), SHA-256
`3571994d768b4a1cc26022f40a1a9f135b81ce4c8ecaf425f7623fb56f4e1431`.

Command: `python -m unittest discover -s admin -p test_color_invalid_continue_cp290.py -q`.

Source SHA-256:

- `admin/color_create_event_probe.py`: `1bbb494c25f8a2647fc6afffc176893d061990a63a7bc3b88118cf0051a1f155`
- `admin/color_entry_supervisor.py`: `4925d6339f6841c09f022b2b776517cf66df7179bb7bee9c597be1adadc42bf9`
- `admin/test_color_invalid_continue_cp290.py`: `7a76e9c6cef3fe95254825048d6f9ab639556219f0bf334d81443ba8327f0af1`

Token usage is unavailable. No speedup or token-saving claim is made.
