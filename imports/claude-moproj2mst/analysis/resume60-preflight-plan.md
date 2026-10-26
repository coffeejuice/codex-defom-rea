# CP60: bounded CP59 grouped preflight selection

The CP59 direct preflight is eleven targets: eight new test files and three changed regressions, with 131 declared Testkit cases. The root's newly retained grouped preflight already selected this exact set and exposed a missing internal include before broad integration. Reuse this selection after repair rather than inventing additional CMake wiring or rerunning an equivalent unchanged check.

This report reads CMake, agent handoffs, current test sources, the checkpoint58 source archive and retained timing/log receipts only. No source/admin/CMake/shared-state edits, executable/build, GUI or mutable REA actions were performed. Only this report was written. Token measurements and a realized speedup are unavailable.

## Source-to-target mapping

CMakeLists.txt uses CONFIGURE_DEPENDS GLOB_RECURSE for src/*.cpp and excludes only dll/cli from m2m_core. It discovers tests/<packet>/test_<topic>.cpp as executable m2m_<packet>_<topic> and CTest m2m.<packet>.<topic>. The source glob already includes these new core files; test discovery already includes these new test files. Some worker handoffs say root must wire them, but the current CMake rules require no manual source/target additions.

Current CMake SHA-256 at inspection: 83af75a9e7ca7912e9076a4cdde113262b6497e2353b0a353010ea9c252985da. The checkpoint58 archive receipt binds source.zip SHA-256 c0533bc1c36fd63161a83fefd8e165ede0531227f1b43d27254a0e8def94f35d. Comparing test source members in that archive with the current tests independently confirmed the following complete test-file delta at inspection time:

| Test executable target | CTest name | Delta | Declared cases |
|---|---|---|---:|
| m2m_doe_backend_commit_prefix | m2m.doe.backend_commit_prefix | New | 11 |
| m2m_doe_flow_receiver_population_prefix | m2m.doe.flow_receiver_population_prefix | New | 7 |
| m2m_mdt_boolean_serial_gate | m2m.mdt.boolean_serial_gate | New | 10 |
| m2m_mdt_close_busy_prefix | m2m.mdt.close_busy_prefix | New | 8 |
| m2m_mdt_paint_device_active_copy | m2m.mdt.paint_device_active_copy | New | 21 |
| m2m_mdt_simctrl_ppv_open_sequence | m2m.mdt.simctrl_ppv_open_sequence | New | 4 |
| m2m_mdt_simctrl_ppv_save_file | m2m.mdt.simctrl_ppv_save_file | New | 7 |
| m2m_mdt_doeoutput_active_guard | m2m.mdt.doeoutput_active_guard | New | 1 |
| m2m_mdt_simctrl_ppv_file_reader | m2m.mdt.simctrl_ppv_file_reader | Changed | 3 |
| m2m_mdt_editor_setup | m2m.mdt.editor_setup | Changed | 55 |
| m2m_mdt_simctrl_ppv_writer | m2m.mdt.simctrl_ppv_writer | Changed | 4 |

The PPV Save file adapter receipt appeared during this inspection and was included rather than silently omitted. Handoff sources are the CP59 backend-commit, DOE-projection, Boolean-serial, Close-busy, paint, PPV Open-sequence, PPV Save-file, UTF8-BOM, active-DoeOutput, editor-R1 and PPV Save-gate implementation/regression reports. All eleven files have Testkit main; the source scan found no raw C assert calls in this selected set. Declared case counts are review expectations, not executed results; verify actual registered summaries after compilation.

The bounded core linkage preflight is these eleven targets in one build_candidate.py invocation with repeated --target arguments. This compiles the common m2m_core dependency once as needed and links each new/changed test. Keep all owners frozen beneath source/ for source fingerprint consistency. New source/test files released after this snapshot require explicit selection reconciliation, not an assumption that the list remains complete.

## Current measured attempt

analysis/resume59-focused-preflight-build.json now records exactly these eleven targets. It is source-stable and took 16.891 seconds total: configure 11.765, cmake-build 2.500, remaining wrapper/source/artifact work 2.626. Configure exited 0; build exited 2. The retained build log reports:

```
src/engine/simctrl_ppv_save_file.cpp(3): fatal error C1083:
Cannot open include file: 'm2m/engine/engine_internal.hpp': No such file or directory
```

This is a compile/include failure, not missing CMake test registration. The internal header lives under src/engine and is already reached by existing source-private include conventions. Root owns the actual source repair and subsequent stable revalidation. A failed preflight is not validation of all eleven targets, and this report does not prescribe a guessed production edit.

After repair, the same grouped target set must compile/link successfully before focused execution. Do not rerun an equivalent unchanged preflight merely to create another receipt; do rerun after a source change/failure or unresolved target. Keep distinct labels and preserve failed logs/source hashes.

## Bounded execution set after successful build

Run the eleven selected CTest names as one root-owned command, with the normal configured per-test scratch/environment. This anchored regex selects only the table above:

```powershell
ctest --test-dir build/candidate --output-on-failure -R '^(m2m\.doe\.(backend_commit_prefix|flow_receiver_population_prefix)|m2m\.mdt\.(boolean_serial_gate|close_busy_prefix|paint_device_active_copy|simctrl_ppv_open_sequence|simctrl_ppv_save_file|simctrl_ppv_file_reader|doeoutput_active_guard|editor_setup|simctrl_ppv_writer))$'
```

Verify eleven registered CTest entries, nonempty retained outputs, expected active case counts, no failure and no unexpected skips/refusals. The active DoeOutput case is a candidate-only guard/dispatch regression and remains qualified; it is not independent original parity. Existing constructor/witness/notifier, PPV binding/count, PaintDevice prefix/runtime, Close entry and folder regressions remain required in the final full suite; they are not silently removed because they are outside this direct new/changed preflight.

Do not add the old standalone p08/p09/p11 closure to every preflight routinely. Checkpoint58 fixed and preflighted that closure. Reinclude its affected targets only if the selected CP59 source actually changes out-of-line model ownership/destructors, standalone source lists or renamed-namespace dependencies. That trigger preserves linkage coverage without replaying unrelated configurations. The final full build still reaches all standalone targets.

## Available cost baseline and final gate

Checkpoint58's now-completed receipts contain six builds: 196.568 seconds total, configure 63.687. Its two standalone preflight driver calls totaled 29.563 seconds, including configure 21.187 and compiler/link work 3.219. These are separate costs, not proof of the time a grouped call would save. The successful final full build receipt took 17.709 seconds; its cmake-build step took 4.438. The final internal launcher took 27.312 seconds: inventory 0.844, pre-SHA 6.953, CTest 12.984, post-SHA 6.453, reporting 0.078. Checkpoint58 integration's 27.312 launcher field matches that internal receipt; do not label it a separately measured external duration.

The new CP59 preflight's 16.891 seconds is an actual early-failure measurement. There is no corrected grouped-pass, focused-command elapsed time, complete CP59 broad build or full-launcher timing available at the report's inspected boundary. Do not use the 2.500-second failing build as a prediction for a successful group or infer a speedup from checkpoint58's different workload.

Once the preflight is green, root performs the normal stable full build and complete optimized launcher, retains every registered test and complete before/after fixture SHA, compares exact ordered folder diagnostics and raw differences, and binds the freeze to that same source/build/archive. Focused success never replaces that gate. Measure total preflight+full-build+launcher accepted-cycle cost and retries after overhead before claiming improvement. No validation, coverage, public guard, API or original-first boundary is weakened by this plan.
