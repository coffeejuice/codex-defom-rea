# CP124 — GUI-control release lifecycle recheck

## Result

The release gate remains unavailable. The current supported Windows Computer Use `sky` API lists window discovery, launch, capture, click, keyboard, text, scroll, value, drag, secondary action, and activation methods; it exposes neither a release/cancel-control method nor a release-status/receipt method. The available `mcp__cua_repl` interface likewise exposes JavaScript session reset, but no release operation or verifiable release state. Resetting a JS runtime is not a release receipt. Do not start a GUI-control session or run a fresh original capture until a supported release operation and verifiable status are provided.

## Evidence checked

- Existing `analysis/gui-control-lifecycle-assessment.json` (checked 2026-10-23) already records `unsupported_release_api`, `release_method: null`, `release_status_method: null`, and explicitly limits the assessment to the supported surface; it does not assert the current overlay state.
- Re-read the installed Computer Use skill and its current `docs/api.md` and `docs/guidance.md` under `C:\Users\alext\.codex\plugins\cache\openai-bundled\computer-use\26.1021.31345`. The API's complete `Window2ComputerUseClient` method list contains no control-release or release-status operation. The guidance says to use `node_repl` and `@oai/sky` for UI actions, but describes no teardown API. No UI session was initialized or acquired for this check.
- Inspected the enabled-tool inventory for names containing release/control-stop terms. Matches are Linear release-management tools only; none is a Windows or computer-use release control. The provided CUA tool surface lists `getState`, tab/app operations, and reset, but no release receipt/status method.
- Re-ran `python tools/gui_validation_status.py` from the repository root. It reports `overall: blocked`, `gui_control: release_unverified`, `release_ready: false`; `control_release` is blocked because the supported surface has no release operation plus receipt/status. Stability, GUI-child observation, and downstream GUI acceptance cases remain dependent on that gate.

## Exact missing capability

The tool surface needs both (1) an explicit operation that relinquishes the relevant Windows UI-control lease/session and (2) a query or returned receipt that verifies release completed. An API that only resets JavaScript bindings, detaches a debugger, or closes an app does not satisfy this requirement. Until both are supported and independently observable, retain the current blocked status for fresh original captures. This audit did not acquire control and cannot infer whether any overlay is currently active or inactive.
