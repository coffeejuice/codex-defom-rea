# CP168 headless original-oracle route audit

## Decision

No retained, source-qualified route currently performs a fresh original DEF_GUI_MO_64 v11.0.2 Open → Save → Close comparison without GUI input. The original binary inspected here is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (verified again in this audit). This is a bounded negative finding about the reviewed routes, not proof that no undocumented route exists.

The exact blocker is **original Save and Close action delivery**. The native startup command line can Open a supplied project, but its inspected dispatch shows no Save or Close command. Existing passive process instrumentation observes only selected prefixes and cannot turn those observations into a natural full lifecycle. The historical full-lifecycle runners use GUI menus and dialogs. The standing supported app-control release operation plus verifiable release receipt/status is unavailable (`python tools/gui_validation_status.py` reported `release_ready=false` in this audit). Thus a GUI-driven fresh comparison cannot start under the current gate.

## Routes checked

| Route | What is established | Limit for fresh Open → Save → Close |
| --- | --- | --- |
| Historical Oracle-B `launch.py`, `probe.py`, `run.py` | `analysis/resume78-original-headless-access.md` traces a detached installed-app launch followed by File/Open, File/Save, File/Close, modal responses and window closure. | Detached launch is not headless Save. Those steps require GUI input and the old runner has upstream paths and unqualified dialog answers. Do not replay it unchanged. |
| Native startup CLI Open | `analysis/wave3-editor-owned-open-harness-proposal.json` binds `-m open` and encoded `-f` to actual Open, postOpen and recent-project calls before window show/event loop. `analysis/wave3-native-cli-save-feasibility.json` inspected the main argv branch through event-loop entry and found no Save/Close dispatch. | Useful for bounded passive Open observations only; its startup timing differs from interactive File/Open. The negative dispatch finding is bounded, not an exhaustive automation inventory. |
| Retained Frida/REA observation | `analysis/wave3-original-editor-probe-004-receipt.json` records a source-bound, fresh owned process, selected native CLI Open to first editor grid, no GUI input, PID absence and closed Frida session. The probe explicitly says no Save/Close comparison. REA exposes process/Frida capture and native UI scenario tools, but process capture needs an externally supplied action path; UI scenario would take GUI control. | A new passive hook can observe a naturally occurring Save only after one is triggered. Directly invoking a native Save/Close function, posting a command, pumping events, or modifying target state is a separate experimental stimulus. Its equivalence to normal user action and lifecycle state has not been source-bound. It cannot be labeled fresh original acceptance now. |
| `tools/reference_harness` | Its README and `harness.py` prepare, freeze, record and compare a fresh original/candidate case; the harness launches no program and sends no input. | The operator or a supported driver must supply the original Open/Save/Close observations and external effects. The prepared case is not an executed oracle. |
| Retained historical artifacts | `analysis/resume164-historical-next.md` and `analysis/resume156-acceptance-queue.md` distinguish B98 candidate-versus-retained output from fresh original behavior; A55 lacks process-bound mode/event state and B120–B122 lack equivalent pre-action DOE state. | These comparisons cannot substitute for a fresh, same-state original run. |

The installed target, user sessions, registry/settings and license side effects are not isolated merely by starting a disposable process. A valid future run needs an owned fresh PID, verified loaded modules/settings, a copied project and generated paths under `C:\PROBLEM` shorter than 100 characters, before/after manifests and process cleanup. The current audit neither verifies an installed deployment nor supplies a writable `C:\PROBLEM` sandbox; the original executable was not launched.

## Exact safe next action

Continue original-free source and candidate work, or prepare a frozen same-root B98 comparison capsule as described in `analysis/resume156-acceptance-queue.md`; label either result candidate-only or preparation. For a **fresh original** comparison, wait for one of these concrete prerequisites:

1. A supported app-control release operation and authoritative release receipt/status, verified before acquiring GUI control, then run the reference-harness protocol in an owned disposable process; or
2. New static/source and runtime evidence that identifies a non-GUI original Save and Close dispatch, proves its actual entry conditions and normal event/owner state after startup Open, and provides bounded process cleanup and side-effect capture. Review that route before running it; do not guess CLI switches or inject a Save call as an acceptance shortcut.

This audit was read-only except for this report. It launched no original or candidate program, did not access a live REA/Frida session, acquired no GUI control, and changed no source, tests, fixtures, shared plan/state or user files.
