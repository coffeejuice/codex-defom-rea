# CP146 GUI-control release capability audit

## Result

Fresh original GUI comparisons remain blocked under the repository's app-control cleanup rule. The latest independent status command, run from the repository root on 2026-10-25, returned `overall: blocked`, `gui_control: release_unverified`, `release_ready: false`, and no available GUI validation tasks. It lists `control_release` as blocked because the supported Windows tool surface has no release operation plus a release receipt/status; the dependent stability, small 2D/3D, helper-verification, object/material/control edit, preview/export, and controller checks remain blocked for that reason.

The retained `analysis/gui-control-lifecycle-assessment.json` was checked at `2026-10-23T14:52:37Z`. It also records `status: unsupported_release_api`, `release_method: null`, `release_status_method: null`, and that no GUI control was acquired during that assessment. It explicitly does not prove current overlay state.

## Current supported API review

The installed Computer Use skill is `computer-use` version `26.1021.31345`. Its `docs/api.md` describes `sky.list_windows`, `get_window`, `list_apps`, `launch_app`, `get_window_state`, input operations (`click`, `press_key`, `type_text`, `scroll`, `set_value`, `drag`, `perform_secondary_action`), and `activate_window`. It documents no stop/release method and no receipt/status query for release. `SKILL.md` initializes a persistent `@oai/sky` session and has no app-control release step. The exposed `mcp__cua_repl` surface offers `js_reset`, but resetting its JavaScript session is not a supported app-control release, nor does it return a verifiable release receipt. I did not initialize the UI automation runtime or call any app/window API.

Therefore, neither a completed UI session nor a tool reset can satisfy the required release-and-verify condition. The exact missing capability is an API operation that relinquishes the injected Windows input/control session, paired with an authoritative status/receipt that confirms release. Until both are exposed and that receipt is checked, the AGENTS instruction bars acquiring another GUI-control session.

## Independent work that can proceed

The release gap only blocks GUI-dependent work. The converter can continue headless source qualification, static PE analysis, disposable fixture tests and frozen candidate builds; it can also run candidate-versus-retained-output comparisons when their states are demonstrably equivalent. CP142 and CP143 ABI test changes remain pending source-bound build/test validation and can proceed independently. Historical A55 still needs a fresh original settings/mode and editor callback trace; B120/B121/B122 still need matched post-action DOE states. Candidate-only tests or more static disassembly cannot replace those missing original witnesses.

No app control was acquired, no user application was inspected or changed, and no converter source, build, fixture or REA database was touched for this capability audit.

## Evidence

- Repository-root `python tools/gui_validation_status.py` output: `release_unverified`, `release_ready=false`, `control_release` blocked, no available tasks.
- `analysis/gui-control-lifecycle-assessment.json` (assessment timestamp, documented methods, null release/status method, no-acquisition qualification).
- Installed `C:/Users/alext/.codex/plugins/cache/openai-bundled/computer-use/26.1021.31345/skills/computer-use/SKILL.md`, `docs/api.md`, and `docs/guidance.md` (current tool methods and absence of a release operation/status).
- Current turn's `mcp__cua_repl` tool surface exposes UI observation/input and JS reset; no release/status API is documented in its available methods.
