# CP65 original-capture gate

## Decision

A fresh original A55 or PPV capture cannot be scheduled under the current app-control gate. The existing assessment and independent queue check both report that release is unverified because the supported Windows control surface has no release operation plus a verifiable release receipt/status. No GUI control was acquired during this check, and this is not evidence that a prior overlay is currently active or inactive.

## Receipts checked

- Parent `analysis/gui-control-lifecycle-assessment.json` reports `status: unsupported_release_api`, `release_method: null`, and `release_status_method: null`. Its recorded supported methods are list/get/launch/window-state, click, key, text, scroll, set-value, drag, secondary action, and activate-window. It explicitly says the assessment did not acquire GUI control and does not prove current overlay state. The assessment is timestamped `2026-10-23T14:52:37Z` and records hashes for its source docs.
- `python tools/gui_validation_status.py` was run read-only. It reports overall `blocked`, `gui_control: release_unverified`, `release_ready: false`, with the sole available work queue empty. The `control_release` blocker states that no supported release operation plus receipt/status is exposed; all GUI-dependent tasks are blocked on that prerequisite.
- The assessment's two source paths under `.codex/plugins/cache/openai-bundled/computer-use/26.1016.62342/docs/` are no longer present at those exact paths. Their content therefore could not be freshly re-read; the assessment's recorded hashes and summarized method inventory are the available retained qualification. This does not supply a release mechanism.
- The currently exposed tool inventory has no dedicated Windows control-release/status operation. A UI-tool reset or closing the target application is not a verified release receipt and is excluded by the standing instructions.

## Scheduling implication

Do not schedule or begin either original capture yet. A55 requires the process's actual `DEFORM-MO.ini` path/hash and `MoInputMode` plus the interaction/state receipts described in `analysis/resume63-a55-mode-qualification.md`. PPV requires the disposable valid-row and control fixtures and byte/hash receipts described in `analysis/resume64-original-binding-options.md`. Those capture plans are ready, but GUI input is gated until an officially supported release operation and independently verifiable release status/receipt exist.

No apps were inspected or controlled, no app was launched or closed, and no project or REA state was changed. The current status supports only the conclusion that the gate is not satisfied; it does not establish the state of any GUI-control overlay.
