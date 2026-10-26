# Case 005 fresh original capture feasibility (read-only)

## Finding

The source inputs and case004 candidate artifacts still exist with their recorded hashes, and no `DEF_GUI_MO_64` process was visible during this inspection. The interactive `alext` console session is active (Windows session 1). This does **not** establish that the desktop is otherwise idle, that no other GUI control owns it, or that a supported app-control release receipt exists. I did not acquire app control or launch anything. Under the checkout recovery rule, a new GUI session is not feasible until a supported release operation and verifiable release status are established.

There is also a concrete path constraint: if case005 follows the harness's hardcoded `build/reference-harness/<case>` store beneath this checkout, planned original project and cwd paths are 151 and 133 characters; candidate paths are 152 and 134 characters. The governing recovery instruction says DEFORM test projects/generated paths must be under `C:\PROBLEM` and shorter than 100 characters. The current harness rejects paths outside its own store, so its standard case005 layout cannot meet that requirement. No case005 directory exists. Do not launch with the long path or silently alias/relocate it; first resolve the harness/store-vs-short-path contract using an approved implementation path, then rerun preflight.

## Read-only identity and isolation inventory

- Current checkout: `C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst`.
- Case005 target: `build\reference-harness\billet-training-005` — absent at inspection time.
- Fixture: `evidence\gui_mo\s1\corpus\06_BILLET\BILLET.moproj`, present; SHA-256 `650b448be5e765c0eaf1e32ef70bf4f7280173b10c8767b023b12e11762a29c1` (5,201 bytes).
- Original executable: `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe`, present; SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes).
- Case004 frozen candidate remains present: EXE `dea5f846e2117e02ab8917ddb08b2331feca10b644b73232502de270334fa89d`; DLL `facb8b34cc8e2d5f94cd41b0307c6f9f9f93788e786bbd14f8209c055ecaf980`; `deformdb.dll` `c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3`. Build receipt `analysis\wave2-geometry-v1-build.json` remains present, SHA-256 `811266d91e917af74733de0b5cde73806ae29838f96a565ca1a1f40cb50f6354`. These prove current file identity only; runtime dependency closure/license readiness is not established.
- Process inventory: `Get-Process -Name DEF_GUI_MO_64` returned none; `qwinsta` showed `console / alext / session 1 / Active`. This was a point-in-time read. Recheck immediately before any future launch; never bind to an uncertain PID.
- Case004 historical qualification remains: original PID 13316 crashed after visible Open at QtCore4_64.dll+0x12a0e4, before verified Save; current user settings were retained. Its candidate PID 31248 later exited 0 in a separate process. Neither event is a current process nor proof of current app-control ownership.

## Feasibility gates before any case005 runtime

1. Resolve the path contract so every native test project/runtime path is under `C:\PROBLEM` and under 100 characters, while evidence remains in this checkout. The present harness's `STORE` containment and `no_links` checks disallow an unreviewed external case root or junction alias. No filesystem link, copied project, or path workaround was created here.
2. Resolve the recovery app-control lifecycle gate: name the supported release operation and establish its verifiable receipt/status before acquiring any new GUI-control session. Current process absence is not a release receipt.
3. In the resolved layout, create a new append-only case only after gates are met; copy fixture and frozen binaries; run source/build/input/binary hash checks and preflight; verify original identity, loaded modules, license/startup state, locale/codepage, current settings, active process absence and exact cwd/path.
4. If feasible, assign one owned PID, capture original stages with observed default dialogs and partial writes, wait for that PID's writes to quiesce, then use the supported release operation and retain its status. Record original before running candidate in its distinct disposable directory. If any ownership or process state is ambiguous, stop before input.

**Current disposition:** file inventory is ready for future preparation, but case005 capture is not presently launch-ready because the required short external sandbox conflicts with harness containment and release status is unestablished. No app launch/control, case preparation, test-project mutation, build, candidate execution, or REA database mutation occurred; this report is the only file written.

