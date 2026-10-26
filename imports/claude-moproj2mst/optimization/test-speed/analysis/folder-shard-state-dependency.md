# B:107_B_HTFF scheduling dependency investigation

Process sharding is not semantically accepted. The focused test still exits successfully because the historical sweep reports scientific differences instead of asserting them. A faster passing CTest does not establish equivalent converter behavior.

## Observed boundary

The root's current serial focused run reports `SAME B:107_B_HTFF`; the two-worker focused log changes only this case to `DIFF`, naming `operations/task00001/simctrl.key`. Oracle B changes from 45 to 44 adjusted-identical cases and 29 to 30 differing files. The historical profile also reports SAME for this case. Root additionally observed the same adjusted mismatch with eight workers, while four and ten workers preserved adjusted rows. These are adjusted diagnostic observations, not raw-byte inventories.

The case is eligible global ordinal 60. Fixed modulo assignment gives these immediate predecessors:

| Schedule | Immediate predecessor in the same process | Root's adjusted result |
| --- | --- | --- |
| Serial | 59, B:106_B_F2ACT | SAME |
| 2 workers | 58, B:105_B_COG7 | DIFF |
| 4 workers | 56, B:103_B_HT3F | SAME |
| 8 workers | 52, B:82_R52_BEND_2D | DIFF |
| 10 workers | 50, B:R11 | SAME |

COG7 contamination alone cannot explain the eight-worker observation: ordinal58 is not in ordinal60's modulo-eight lane. The four-worker lane contains R52, then HT3F, then HTFF; the eight-worker lane omits that intervening HT3F conversion. Immediate predecessor is only a discriminating probe candidate, not proof that earlier prefix history is irrelevant.

## Bounded source trace

All paths below refer to the isolated `optimization/test-speed/source/` tree. These source observations describe the reconstruction; no original application was exercised in this investigation.

- `apps/moproj2mst/src/core/convert.cpp:153-157` explicitly identifies the engine's Fortran units, KEY reader and model globals as process-wide and serializes conversion. A fresh worker changes process predecessor history. The settings and initial current directory remain inherited; each worker also deliberately changes its TEMP/TMP/TMPDIR roots. A schedule-sensitive mismatch is therefore consistent with history, allocator, or temp/path effects but does not identify one of them.
- `apps/moproj2mst/src/engine/save_current.cpp:277-300` writes SimCtrl.KEY through the native target-file opener. After successful truncation it discards the imported overlay and calls `def::key::write_simctrl_key_stream` with flags 0xf.
- `port/defkey/src/item_writers/task_key_writers.cpp:65-69` assigns the model flags, writes the banner and invokes `write_group1`; `kw_g1a/group1_shell.cpp:33-40` dispatches the g1a and g1b sections. The output depends on the current model, not just copied input bytes. The banner-only exception already exists, so an adjusted DIFF cannot be explained solely by a recognized version banner difference.
- `port/defkey/src/kw_g1b/write_g1b.cpp:82-114` writes ENVTMP as constant/table or furnace curve list depending on `furnace_on`. The expected HTFF file uses a furnace list (`ENVTMP 10 22`). `apps/moproj2mst/src/engine/loader_l2.cpp:116-145` changes heat-transfer defaults and `:194-205` derives an environment value from furnace curve8. This is a relevant field route if the captured diff lands there; there is currently no evidence that it does.
- `port/defmodel/src/step/sim_ctrl.cpp:24-29` intentionally constructs members and writes only owner plus the scoped GUI startup unit before initialization. The later initialization writes defined subsets. This does not itself establish an uninitialized-field defect, but rules out assuming the entire simulation-control object is zeroed.
- `apps/moproj2mst/src/model/units.cpp:190-198` scopes the GUI startup-unit global around each conversion. Its existence is established; a startup-unit leak is not established. `port/defkey/src/line_io.cpp` also retains global token buffers, while `read_g1a.cpp:36-39` retains model hooks. No reset or workaround should be added without a field-level trace.

The source qualification for native SimCtrl saving is `analysis/wave3-native-simctrl-contract.json`: it records the original writer/open ordering and expressly qualifies locale/startup and allocation exhaustion behavior. Those historical notes support source provenance, not a new runtime equivalence claim.

## Coordinated targeted capture

At root's explicit instruction, the isolated folder test now supports `M2M_FOLDER_CAPTURE_107=1`. It affects only B:107_B_HTFF and only when that variable is present. Immediately after a successful conversion and before comparison/cleanup, it copies the generated and expected SimCtrl.KEY bytes to:

`M2M_TEST_SCRATCH/m2m_folder_capture_<process PID>_<steady-clock timestamp>/`

The directory is unique, checked through the existing scratch-containment helper and outside sweep/IPC cleanup. `generated-simctrl.key`, `expected-simctrl.key`, and `capture.tsv` survive ordinary cleanup. The receipt records case name, worker (`serial` or `wN`), process PID, global ordinal, input/expected/work paths and original relative file name. Missing copies or failed receipt writes fail the investigation. No comparator masks, conversion settings or state resets were added. Capture overhead is not suitable for a claimed speedup measurement.

No suite was run and no existing fixture was mutated by this worker. Root must rebuild and collect captures; this report does not assign a changed keyword before those bytes exist.

## Smallest useful next probes

1. Use the same newly rebuilt binary for serial and two-worker runs with targeted capture enabled. Hash both generated files and both expected copies; compare raw bytes and produce the smallest differing line/keyword view. Expected copies must hash identically. Retain paths and TEMP roots as possible explanatory variables.
2. Once the keyword is known, run an owned process-sequence probe that retains outputs at the same explicit work and temp paths across separate fresh-process trials: fresh HTFF; COG7 then HTFF; COG7 then F2ACT then HTFF. If needed, add R52 then HTFF and R52 then HT3F then HTFF to explain the eight/four-worker distinction. Every conversion must use a fresh copy of its input, and sequence prefixes must remain unchanged within each trial.
3. If immediate-predecessor probes do not reproduce the mismatch, use the recorded serial and modulo-lane prefixes and minimize the failing prefix while retaining the known target, paths and settings. Do not assume one-process-per-case behavior is the serial oracle.
4. Trace only the differing keyword's model field through initialization, DB/KEY loading, property activation and saving. Investigate a baseline defect separately from an optimization change. A mask or extra global reset would change the research target and cannot establish shard equivalence.

The targeted capture is a diagnostic aid. It does not replace the design's full raw inventories, complete vectors, source/executable/fixture binding, same-path replay and repeated timing gates. Serial remains the accepted default.
