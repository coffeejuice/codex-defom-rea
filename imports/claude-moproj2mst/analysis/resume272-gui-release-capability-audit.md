# GUI release capability audit

**Result: gate remains closed.** The currently documented computer-use surface exposes no supported operation that releases Windows app-control input and no independent release receipt/status query. The requested release evidence is therefore unavailable; do not reopen the GUI-validation gate or acquire a GUI-control session.

## Evidence checked

- Read the repository `AGENTS.md` and `analysis/gui-control-lifecycle-assessment.json`.
- Ran `python tools/gui_validation_status.py`: `overall=blocked`, `gui_control=release_unverified`, `release_ready=false`, `available=[]`, and `accepted_backend_cases=0`. Every listed GUI case is blocked on release capability or its dependent prerequisites.
- Read the installed `computer-use` skill and its `docs/api.md` and `docs/guidance.md`. The documented `sky` API includes window/app discovery, launch, observation, input, and activation methods. It has no release method or release-status method. `node_repl` reset, debugger detach, and closing the target app are not documented release mechanisms and cannot serve as proof.
- The current CUA tool instructions expose initialization/state and browser/app selection entry points but no release call or receipt/status interface. No app-control session was acquired during this audit.

## Gate decision and proof sequence

There is no exact documented call or harmless proof sequence to provide on this tool surface. A future gate review requires a supported release operation plus a separately readable, authoritative receipt/status confirming release. Do not infer that an attempted release succeeded from tool reset, debugger detach, app closure, or absence of further input.

## Independent next work

The status script reports no currently available work items. Continue only non-GUI offline analysis that does not depend on GUI acceptance, using the current recovery plan and preserving the existing frozen evidence. Re-run the status script after any tool/API capability change; proceed with GUI validation only if it reports release ready and the release can be independently verified.
