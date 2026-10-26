# Recovery workflow

## App-control cleanup (user instruction)

Stop app-control input when each GUI interaction session finishes. Release control
through a supported tool API where available, and verify the release before
claiming it. If the current tool has no release API, state that limitation rather
than claiming app control is off. Do not leave a GUI-control session active while
performing unrelated analysis. Do not close user applications as a substitute for
releasing control.

For the adaptive GUI validation plan, do not acquire another GUI-control session
until a supported release operation and verifiable receipt/status are established.
Read analysis/gui-control-lifecycle-assessment.json and run
`python tools/gui_validation_status.py` for the independent work queue. A reported
physical Escape interrupts GUI control, not the overall task. Respect enforced
tool stops, retain uncertain action outcomes, and continue available independent
work. Never claim JS reset, debugger detach or closing DEFORM releases app control.

## Standing deployment rule (user instruction, 2026-10-23)

Every report mentioning a reconstructed DEF_PRE_64 instance must identify its
verified version (and distinguish source/build/deployed versions when different).
Version allocation follows DMO: tools/bump_def_pre_version.ps1 selects one patch
above the highest DEF_PRE_VERSION in src/app/Version.h.in across every Git worktree
working file and every local/remote-tracking branch; preserve major/minor. The
CMake def_pre_version dependency runs it before each executable build. Never skip
the allocation. Resolve version-file merge conflicts with the higher version,
then allocate again before the next build. Older unversioned artifacts must be
identified by legacy banner plus SHA-256, never assigned an invented version.

Keep the latest successfully built reconstructed DEF_PRE_64.exe installed at all
three paths under C:\Program Files\SFTC\DEFORM\v11.0.2 (root, 2D, 3D).
Preserve each original in its own folder as DEF_PRE_64.exe.original; never overwrite
those backups. After each rebuild, deploy the new executable to all three paths,
verify matching SHA-256 hashes, and report the deployment and any test failures.
Use tools/deploy_latest_def_pre.ps1. Do not restore originals after tests, on a
failure, or during cleanup unless the user explicitly asks to return originals.
The prior temporary-trial automatic restoration policy is superseded.
Keep DEFORM test projects and generated paths under C:\PROBLEM, with full paths
shorter than 100 characters. Keep diagnostic reports in this checkout.

Start recovery work with `python tools/recovery_status.py`. Follow the current
work in `analysis/recovery-plan.json` and the efficiency rules in
`analysis/efficiency-policy.md`. The full application goal remains unchanged.

Use bounded evidence views and compact capture receipts. Never print complete
REA session capability objects, dossiers, plans, or ledgers just to find a field.
Read the referenced source qualifications before making behavioral claims.

Review efficiency after three recovery checkpoints or one hour at the next safe
checkpoint. Do not interrupt a live capture, frozen comparison or build for it.
Measure before claiming speed or token savings. Preserve evidence, candidate
freezes, API compatibility and required tests. Archived workers stay archived.

Git transport must use the configured SSH remote and Windows OpenSSH at
`C:/Windows/System32/OpenSSH/ssh.exe`; never fall back to HTTPS Git transport.
