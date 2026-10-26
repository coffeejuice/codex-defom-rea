# Resume200 GUI control release capability

## Finding

No currently supported Windows control-release operation with a verifiable release receipt or status is exposed in the inspected tool surface. GUI interaction must remain gated under the repository's recovery rule.

## Evidence

- `analysis/gui-control-lifecycle-assessment.json` records `status: unsupported_release_api`, `release_method: null`, and `release_status_method: null`. Its qualification says this reflects the supported surface only and does not establish the current overlay state. It also says no GUI control was acquired to investigate lifecycle.
- `python tools/gui_validation_status.py` returned `overall: blocked`, `gui_control: release_unverified`, `release_ready: false`, `next_task: null`, and `available: []`. The `control_release` blocker specifically requires a supported release operation plus release receipt/status. All GUI-dependent work remains blocked.
- The installed Computer Use skill documents `@oai/sky` methods including `list_windows`, `get_window`, `get_window_state`, `click`, `press_key`, `type_text`, `scroll`, `set_value`, `drag`, `perform_secondary_action`, and `activate_window`. The API reference contains no release operation and no release-status method. `activate_window` targets/foregrounds a window; it is not documented as releasing control.
- The skill is scoped to initiating and operating computer use. It provides no release API. The inspected tool inventory is therefore insufficient to verify that app control has been released.

## Next action

Keep the original-comparison GUI task blocked. Resume GUI work only after a supported release operation is exposed and a separate supported status/receipt confirms release. Re-run `python tools/gui_validation_status.py` after updating the lifecycle evidence; it should report `release_ready: true` before GUI-dependent tasks become available. Do not infer release from a JS reset, debugger detach, Escape, or closing DEFORM.
