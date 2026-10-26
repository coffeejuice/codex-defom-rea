# DEF_PRE_64 v0.2.1 validation checkpoint

Parallel preparation and native helper diagnosis are integrated. The requested
GUI/controller acceptance is incomplete: **zero backend cases are accepted**.
The external prerequisite is a supported Windows app-control release operation
with a release receipt or observable status. The installed interface exposes
neither. No new GUI control was acquired during this stage; the earlier control
state remains unverified. This is not a successful end-to-end test result.

## Invocation evidence

The independently reviewed [route map](gui-invocation-route-map.md) corrects the
earlier inference that all four MO callers necessarily launch DEF_PRE_64.

| MO command / recovered handler | Established static selection | Runtime qualification |
| --- | --- | --- |
| Run Simulation | Flags (1,0) select the command-interpreter path requesting DEF_PRE | Actual resolved executable still requires process capture |
| Copy/Mirror generation | Flags (1,0), same plain DEF_PRE path | Not an established DEF_PRE_64 invocation |
| 2D-to-3D generation | Fifth flag is 1; marker-selected external route | With the 64-bit marker this is the strongest mapped direct route; exact live GUI action and child remain unobserved |
| Generate Database | Fifth flag is 0; virtual conditions choose the branch or bypass the helper | Prior instrumented DB generation without a child is GUI-only evidence |

The controller's master-file rebuild requests DEF_PRE.EXE. Its ARM driver has
marker-selected DEF_PRE_64 branches for next-run/remeshing processing. A normal
one-step, no-remesh solver run may not invoke DEF_PRE_64 v0.2.1. Do not add
remeshing or replace a 32-bit binary merely to create coverage. The solver itself
has only a preliminary string inventory; full solver code analysis is not claimed.

## Execution blocker investigated

In native helper trial H1, DEF_CFG_DIR was absent, the application's resolved
configuration path was empty, file access failed, and its warning branch ran.
H2 supplied the stock configuration directory to that new child before startup:
the application opened the file, exchanged its required headers and returned a
valid configuration flag. The checkout working directory did not change.

H2 also corrected a duplicated executable argument, so the trials are not a
strict one-variable comparison. They do not prove the cause of the earlier GUI
warning or completion of Run Options. Both owned helper processes were stopped
at their 45-second diagnostic deadlines; no solver was run. The actual GUI child
must still be observed with its fresh embedding handle and launch context.
See [findings](helper-launch-context-findings.json) and the
[GUI resume contract](gui-helper-resume-contract.json).

## Integration and verification

- Three subagents prepared routes, the helper probe and fixtures, then reviewed
  each other's work. The orchestrator alone ran native captures and integrated.
- Independent review found and repaired a retained MPI control file in trial
  preparation, duplicate XML control ambiguity, and an incorrect Frida callback
  form at an instruction probe. Exact MO callsite flags corrected route attribution.
- The combined offline suite passed **27 tests**. After the final canonical
  comparison-file path repair, all **16 fixture tests** passed again. Comparison
  masks are byte-for-byte unchanged; no C++ rebuild or full scientific regression
  is claimed for this tooling-only stage.
- R4 holds separate small 3D, one-step controller and static 2D pairs under
  C:\PROBLEM. Current known full paths are at most 60 characters. Original source
  and all six fixture manifests remained unchanged through the helper diagnostics.
- Large-project preparation is metadata-only; its two approximately 5 GB copies
  remain deferred until preceding GUI cases can execute.
- Independent audit confirms reconstructed **DEF_PRE_64 v0.2.1** at root, 2D and
  3D, all SHA-256
  `7e9d043f19d453d9f1d3cae217b17eaaf0bd70f55a29979741729ab2a1f7c9c5`.
  All adjacent original backups remain intact. No rebuild or restoration occurred.

## Remaining acceptance queue

All GUI cases depend on the external release capability: stability isolation,
actual Run Options helper verification, small 3D unchanged generation and object,
material and control edits, confirmed preview/export routes, verified 2D cases
and conversion, large selected-step generation, and the bounded controller run.
Each backend case still requires observed invocation and version/hash, successful
processing, independent original replay/comparison, and correct GUI reopen.

The persisted [validation state](def-pre-gui-validation-state.json) keeps analysis
completion separate from acceptance. The [parallel state](gui-validation-parallel-state.json)
records ownership, frozen handoffs, blockers and the resume cursor. Worker overlap
and stage times are recorded separately; no serial comparison or token-savings
measurement exists, so no overall speedup is claimed. Executors should remain
idle once this preparation is integrated until an executable dependency changes.
