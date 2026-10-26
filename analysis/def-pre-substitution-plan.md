# DEF_PRE_64 substitution and test plan

> Standing user rule, 2026-10-23: keep the latest reconstructed build installed at
> root, 2D and 3D; retain adjacent `.original` backups. Any restoration instructions
> below describe the earlier temporary trial and are superseded. No test, cleanup
> or failure may restore originals without a new explicit user instruction.
> Reference comparisons must use preserved originals/cached results without
> changing the active installed backend. See `def-pre-deployment-policy.json`.

Status: bounded substitution trial completed on 2026-10-23. All three originals
were renamed in place, replaced for testing, then restored and hash-verified.
Results: `analysis/def-pre-substitution-validation.json`. Correctly named x64
candidate built; 217 tier-1, 26 tier-2 and 1,907 tier-3 tests passed; 8 original baseline DB
utility cases and 24 comparisons through the three installed paths passed.
The full test matrix below remains partly unexecuted, especially GUI workflows.

Purpose: test the reconstructed C++ application through every installed
DEF_PRE_64.exe entry point, with an independent original reference and a verified
way to restore the installation. The required output filename is DEF_PRE_64.exe.

## Confirmed inventory

Two recursive searches, including hidden files, found exactly these three files:

| Location | Absolute path |
| --- | --- |
| Root | C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe |
| 2D | C:\Program Files\SFTC\DEFORM\v11.0.2\2D\DEF_PRE_64.exe |
| 3D | C:\Program Files\SFTC\DEFORM\v11.0.2\3D\DEF_PRE_64.exe |

All three are 6,775,296 bytes and have SHA-256
`f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794`.
No reparse points were found under the installation. The machine-readable
inventory is `analysis/def-pre-installation-inventory.json`. Re-scan immediately
before deployment; a changed target list or original hash requires reconciliation.

The import-time candidate (before the naming change) was
`C:\Users\alext\Projects\codex-deform-rea\build\claude-def-pre\Release\def_pre.exe`,
2,200,064 bytes, SHA-256
`29db8c5497b24248a90ccaf0eaa4dce2ca1cffb891c4f41dd48fa91c1d636e8e`.
It passed 217 tier-1 CTest entries. Optional subcases were skipped or filtered;
26 tier-2 and 1,907 tier-3 cases were not rerun during import. This is build/test
evidence, not a finding of complete application equivalence.

## 1. Produce the correctly named candidate

In our imported copy's `port/CMakeLists.txt`, retain the logical target `def_pre`
and set its output name:

```cmake
set_target_properties(def_pre PROPERTIES OUTPUT_NAME "DEF_PRE_64")
```

Update our build wrapper, documentation and any literal executable-path consumers.
Keep target-based references such as `$<TARGET_FILE:def_pre>` unchanged. Do not
alter `C:\Users\alext\Projects\claude-deform-rea`, the existing loader API, or the
frozen MOVCTL candidate. Retain the original import manifest; record our naming
patch and new build separately rather than rewriting import history.

Build x64 Release through `tools/build_claude_def_pre.ps1`. Expected output:
`C:\Users\alext\Projects\codex-deform-rea\build\claude-def-pre\Release\DEF_PRE_64.exe`.
Check PE architecture, runtime DLL dependencies, startup, and `--port-version`.
Run tier-1 once for this candidate and tier-2 with cached original results. Missing
cache/fixture cases must be recorded as untested, never silently counted as passes.
Keep the candidate bytes, source patch, build settings, test results and SHA-256
together as a fixed deployment candidate. Do not use a stale `def_pre.exe`.

## 2. Prepare adjacent backups and capture the baseline

Create a unique local trial directory outside Program Files, proposed under
`C:\Users\alext\Projects\codex-deform-rea\build\substitution-trials\<run-id>`.
Use that directory for the candidate, copied test projects, logs and reports.
Keep each original executable in its own installation folder by renaming it to
`DEF_PRE_64.exe.original` during the substitution step. Do not copy original
executables into the trial directory. The three backup paths are:

- `C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_PRE_64.exe.original`
- `C:\Program Files\SFTC\DEFORM\v11.0.2\2D\DEF_PRE_64.exe.original`
- `C:\Program Files\SFTC\DEFORM\v11.0.2\3D\DEF_PRE_64.exe.original`

Before renaming, record hashes, sizes, attributes, timestamps and access-control
metadata. Never overwrite an existing `.original` file: inspect and reconcile
any earlier trial state first. Verify each renamed original against its recorded
hash. Capture the baseline before these renames, while the installation is intact.

Prepare and rehearse deploy/restore logic on a disposable three-file tree first.
It must reject wrong hashes and changed targets, journal each rename and install,
and recover from failure after any original rename or candidate installation.
Prepare an elevated restoration command usable independently of this chat.
Do not weaken installation permissions to make copying easier.

Capture baseline results while all installed originals are still present. Use
fresh copied projects for original and candidate runs, with the same relative
names, working-directory layout, inputs, arguments, stdin and environment. Record
stdout/stderr, exit codes, output files, timings and child-process paths.

Keep an explicit original reference: the Python oracle supports `DEF_PRE_EXE`
and `--exe`. Before comparisons resume after substitution, point it to the verified
root `DEF_PRE_64.exe.original`, not the replacement `DEF_PRE_64.exe`. Validate that
each required reference tool accepts this explicit filename; do not assume shell
file associations can launch a file with the `.original` suffix. Baseline capture
already precedes renaming; any needed additional original execution must use a
verified explicit process launch or take place after restoration.
Keep corpus comparisons offline after the switch, and validate the oracle hash.
Audit tests that map original code at fixed addresses or use hardcoded installation
paths; `DEF_PRE_EXE` is not assumed to redirect every C++ test. Run such tests before
substitution, redirect them explicitly with a hash guard, or defer them until after
restoration. Never compare the candidate against another copy of itself.

## 3. Test matrix

Use the existing fixture scripts where applicable, verifying their inputs first.
DEFORM project/input/output paths must be shorter than 100 characters. For future
trials, store working projects and generated files under short folders in
`C:\PROBLEM`; keep long-path checkout directories for reports only. Validate
projected generated filenames as well as existing inputs before running DEFORM.
Real project copies must be independent files, not hardlinks or junctions to user
data. An imported fixture currently requests a linked DB; override that for this
trial. Preserve input hashes and keep all generated output inside trial directories.

| Stage | Cases | Required evidence |
| --- | --- | --- |
| All three paths | Startup/version, clean exit, representative `-m` utility | Correct candidate hash and actual executable path at root, 2D and 3D; no launch or DLL failures |
| Small DB | COG_P1_B1.DB: header, stepList, lastStepNo, simStepList; export an existing selected and last step | Same exit status and meaningful output as original; selected steps verified from baseline |
| Large DB | Inc718_D420_GFM_V_DIES_Angle2_18.DB: same utilities and selected/last-step export | Completes within a baseline-derived timeout; preserves step/object counts and exported records |
| KEY/model | Import, edit object/material/simulation values, save KEY and DB, reopen | Intended values persist; unrelated fields and arrays remain equivalent |
| 2D and 3D | At least one representative fixture of each verified dimension | Mesh topology, coordinates, materials, settings and dimension behavior match; folder name alone does not establish dimension |
| DEFORM integration | Repeat baseline GUI/batch workflows using copied projects | Observed child path identifies the substituted executable; expected files are consumed successfully |
| Errors | Missing file, malformed/truncated copied input, invalid step, overwrite cancellation | Compatible failure/exit behavior and no unintended output corruption |
| Helpers | Representative meshing/remeshing/conversion command after core I/O passes | Correct unchanged installed helper, arguments and read-back; distinguish inherited original failure from successful functionality |

Verified source DBs:

- `C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB`: 9,216,000 bytes.
- `C:\PROBLEM\TEST_REA\Inc718_D420_GFM_V_DIES_Angle2_18\Inc718_D420_GFM_V_DIES_Angle2_18.DB`:
  5,015,897,088 bytes. Budget disk space for separate baseline/candidate copies and
  generated outputs; run large cases sequentially rather than multiplying copies.

Compare KEY/text bytes where deterministic. Compare DB bytes and decoded records
with the existing comparator, retaining raw differences and explaining any allowed
volatile fields. Do not invent broad tolerances to turn mismatches into passes.
Cross-reopen candidate output with the original, and original output with the
candidate. Full tier-3 corpus validation remains a broader acceptance gate, preferably
outside the short installation substitution window.

## 4. Replace all three installed instances

Close DEFORM front ends and confirm no solver, preprocessor or helper job is using
the installation. Do not terminate user jobs or discard unsaved work automatically.
Recheck target inventory, original hashes, candidate hash and backup-name availability.

With the necessary Program Files write access, stage the same candidate adjacent
to each destination and verify staged hashes. In each of the three folders,
rename `DEF_PRE_64.exe` to `DEF_PRE_64.exe.original`, verify its original hash,
then install the staged candidate as `DEF_PRE_64.exe` while the installation is
idle. Journal each rename and installation and preserve target access controls.
This is not an atomic three-file operation: if any operation fails, restore every
affected target, including one whose original was renamed but whose candidate was
not installed, before allowing DEFORM to restart.

Re-scan the full tree. Require exactly the expected three DEF_PRE_64.exe paths,
all matching the candidate SHA-256, plus the three adjacent `.original` backups
matching their recorded original hashes. No testing with a mixed original/candidate set.
Only after that check, run stage 3 through the installed paths.

The port defaults to `C:\Program Files\SFTC\DEFORM\v11.0.2` for helper lookup and
accepts `DEF_PRE_PORT_INSTALL_DIR`. Use that exact installed root for this trial;
do not accidentally retain an environment override pointing elsewhere. Other
DEFORM binaries, DLLs, licensing components and configuration remain unchanged.

## 5. Pass criteria, stopping and restoration

Stop on a crash, hang, unexpected helper launch, lost model data, unreadable output,
unexplained numerical difference or any deployed hash mismatch. Preserve logs and
generated files for diagnosis. A successful launch alone is not acceptance.

The initial trial passes only when all three deployed paths are verified, required
2D/3D workflows match the baseline, saved files reopen correctly, and no unexplained
differences remain. Report each case as pass, fail, inherited-original failure,
skipped or untested. Do not label missing coverage as complete reconstruction.

Restore all three originals at the end of the initial trial, including after a
successful trial. With DEFORM idle, verify each `.original` backup and any installed
candidate, clear the candidate from its active pathname, and rename
`DEF_PRE_64.exe.original` back to `DEF_PRE_64.exe`. Do not overwrite or remove an
unexpected file. Restoration consumes the `.original` filename; the original bytes
remain in their original folder under their original name. Verify all three restored
hashes and the inventory, then perform a small original startup/workflow check.
Retain the fixed candidate and reports in the trial directory. Leaving the candidate
installed is a separate deployment decision.

## Execution boundary

The naming/build, rollback rehearsal, original baseline, all-three-path substitution,
DB utility comparison and restoration phases have been executed. Only copied DB
inputs were used. The original Claude source directory was not changed. Detailed
GUI, installed editing/export/reopen and helper/error workflows remain
unexecuted. The full offline tier-3 corpus passed after restoration. Current installation paths contain the restored originals. The staged
candidate and all trial records remain under the recorded trial directory.
