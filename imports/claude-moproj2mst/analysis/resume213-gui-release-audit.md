# GUI control release audit (resume213)

Checked: 2026-10-26 (current task tool inventory and checked-in lifecycle assessment)

## Finding

The GUI input gate remains closed. The checked-in lifecycle assessment reports `unsupported_release_api` and records both `release_method` and `release_status_method` as null. Its documented Computer Use surface lists window enumeration, launch/selection, state capture, input, and activation methods; it exposes no release operation and no release receipt/status operation. The current computer-use skill/API documentation inspected for this audit likewise documents only `sky` window2 methods (`list_windows`, `get_window`, `list_apps`, `launch_app`, `get_window_state`, input methods, and `activate_window`). No method to release control or verify release is documented. The current turn's available CUA surface also provides no release/status API.

No CUA/app-control session was initialized or acquired for this audit. This therefore establishes only that the currently documented and exposed supported surface has no release-plus-verification capability; it does not establish the current overlay state, nor prove that a prior GUI session is inactive.

## Independent work queue

`python tools/gui_validation_status.py` returned `overall: blocked`, `gui_control: release_unverified`, `release_ready: false`, and no available tasks. `control_release` is blocked because the supported Windows tool surface lacks both release and verification. GUI stability, small 3D/2D checks, helper GUI verification, preview/export, large selected step, and controller checks remain gated or dependent on that release capability. `accepted_backend_cases` is 0.

## Safe next action

Keep all GUI input tasks gated. Resume only after the supported Windows-control tool surface adds a release operation and a verifiable receipt/status, then independently confirm that release before acquiring another GUI-control session. Continue only independent non-GUI analysis/work that is authorized by the recovery plan. Do not infer release from JS reset, debugger detach, closing DEFORM, or elapsed time.
