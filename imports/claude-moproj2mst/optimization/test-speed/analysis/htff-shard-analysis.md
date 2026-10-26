# HTFF serial/shard difference: bounded source analysis

The two-worker candidate changes an existing comparison result: B:107_B_HTFF gains a difference in `operations/task00001/simctrl.key`. Until the raw differing keyword is captured, case history is the leading explanation, not an established root cause. Four-worker adjusted-log agreement does not eliminate the defect: it changes the target's predecessor history again.

This is read-only source/log analysis. No source/build/fixture edits, executions, original applications or tests were performed. Only this report was created. Recovery remains paused. The optimization source was changing during review; test_save_folder.cpp SHA-256 at report capture is `16ac6ff8a6847d17b46203239f49f6c4ce764f597b2826adc36e4d98830535b9`. Rebind any experiment to a frozen binary/source receipt. Source comments cite recovered-function addresses and prior qualification; this review does not independently validate those addresses against an original binary.

## Observed schedule and limits

Parsing the retained serial focused log yields 98 eligible case rows and HTFF at ordinal 60. The static sharder assigns `(ordinal - 1) % workers`, preserving order within a child. Using zero-based worker indices:

| Mode | Target worker | Last predecessor sequence before HTFF |
| --- | ---: | --- |
| Serial | 0 | B:103_B_HT3F (56), B:104_B_HT2F (57), B:105_B_COG7 (58), B:106_B_F2ACT (59) |
| Two workers | 1 | B:82_R52_BEND_2D (52), B:101_B_DIS3F (54), B:103_B_HT3F (56), B:105_B_COG7 (58) |
| Four workers | 3 | A:RS_53_Cogging_Multiple_Ops_Lab1 (44), A:RS_57_Gear_blank (48), B:82_R52_BEND_2D (52), B:103_B_HT3F (56) |

The serial and two-worker logs both retain the existing A:RS_55 SimCtrl difference; only HTFF changes among their 98 ordinary result rows. The parent reports four-worker adjusted rows exactly match serial; that result was provided by the coordinator, not independently rerun here. The target is a furnace operation: existing `tests/mdt/test_save_l6.cpp` exercises 107_B_HTFF as SfOprHTFurnace.

The immediate clue is that two-worker HTFF follows COG7, serial HTFF follows F2ACT after COG7, and four-worker HTFF follows HT3F without COG7 in that lane's recent suffix. This fits a COG7 residual state overwritten by F2ACT, but the earlier lane prefixes also differ. It does not prove a one-predecessor cause. Logs contain no raw bytes or differing keyword; the original runCase removes its work leaf, so the prior mismatch cannot now be localized from these logs.

## Relevant source path

1. `src/core/convert.cpp::run` serializes the entire conversion with a process-local engineMutex. Its comment explicitly identifies process-wide DB Fortran units, KEY reader and model globals. Each call constructs a ConversionSession and opens the active operation; full Save enables nativeSimCtrl and calls saveCurrent.
2. `src/engine/loader.cpp::loadSimCtrl` imports the active operation's key through key_import_sim_ctrl when oprFlag is nonzero. If no key exists, inheritSimCtrl searches previous operations within the current project, using a temporary DB_STEP_DATA and copying PDIE and (in 2D) GEOTYP. These are intra-project predecessors, distinct from the cross-project schedule above.
3. `src/engine/loader_l2.cpp` implements the furnace property route. furnacePropsLoad reads furnace curves/name and simulation controls into properties. furnacePropsToEngine calls furnaceToEngine (name and furnace curve 8/environment value), then furnaceDataToSimCtl. The latter writes VFACTR mode, STPDEF, DTMAX, DTPMAX, its two limits and STPINC. These are first inspection candidates if the raw delta is a furnace/step-control keyword.
4. `src/engine/save_current.cpp::saveSimCtrl` selects the operation's step, uses flags 0xf, and passes it to def::key::write_simctrl_key_stream. `port/defkey/src/item_writers/task_key_writers.cpp` sets model flags, writes a banner, then write_group1. No persistent writer state is introduced in that wrapper.
5. `port/defkey/src/kw_g1a/group1_shell.cpp` dispatches write_group1 into g1a then g1b. Most emitted data comes directly from DB_STEP_DATA.sim_ctrl. `kw_g1b/write_g1b.cpp` serializes furnace curves and FRNNAM, environmental values, iteration settings and user variables. A single keyword delta would narrow this source search substantially.

## State inventory and ranked explanations

**1. Cross-project residual engine or heap state (highest priority).** The changed predecessor pattern and isolated model-owned file support this hypothesis. ConversionSession reconstructs many owners per call, but its destructor explicitly describes adapter exception/final cleanup, not an original normal-Close replay. GuiModel resets its step and GUI controls, but this is not evidence that every global or every model byte is initialized before use. `port/defmodel/src/step/sim_ctrl.cpp` deliberately preserves native partial initialization (for example, u680 is initialized only for 2D); constructors and selective KEY imports must be traced for the actual differing field before blaming a global. Heap reuse can track case history without any intentional global variable. Do not zero extra fields or reset globals as an optimization fix.

Concrete persistent-state sites exist, but none is proven causal: `defmodel/step/db_step.hpp` holds g_gui_engine_variant and thread_local g_gui_engine_startup_unit; `step/index_tables.cpp` retains dimension-selected index arrays and g_index_tables_dim; `program_info.hpp` holds version/dimension/product globals. GuiStartupUnitScope in `src/model/units.cpp` sets startup units for each conversion and restores them afterward, and GuiModel's constructor sets GUI defaults. This makes an unscoped startup-unit leak less likely than an unknown field/import/heap interaction. index_tables are selected during step dimension setup; capture their dimension only if the differing keyword implicates dimensional routing.

**2. Shared filesystem/timing effect across workers (possible, lower priority).** Processes do not share C++ globals, so a data race on those globals between children is not a mechanism. Fixed engine temp basenames exist, but workerEnvironment now isolates TEMP/TMP/TMPDIR and each ordinal has its own project leaf. An overlooked absolute/shared file, library temp path not respecting those variables, or environmental path sensitivity remains possible. The same two-worker schedule run sequentially is the useful discriminator; adding more workers is not.

**3. Allocation/startup nondeterminism unrelated to predecessor (possible).** PID, address, timing, and heap layout differ across launches. Repeated fresh HTFF at the same absolute target path/TEMP is needed before assigning causality to COG7. A raw byte delta that varies between identical repetitions would move this explanation upward.

**4. Comparator or banner-only issue (lower priority).** runCase allocates got/want maps and difference vectors locally per case and uses the same comparator in all modes. Process-local keyword_name uses one overwritten static buffer, but each call rewrites it and conversions are serialized within each process; no cross-process keyword-buffer race follows. The reported simctrl file difference survives existing banner-only classification, so a simple banner-only change is a poor first explanation. Raw capture remains required to exclude comparator/read failures or a banner-plus-data difference.

## Smallest discriminating capture

First freeze the candidate and enable the new investigation-only `M2M_FOLDER_CAPTURE_107` helper observed in the live source. It copies generated/expected simctrl.key before normal leaf cleanup and records input/expected/work paths, ordinal, PID and worker. Run one serial and one two-worker focused sweep, preserve both raw pairs and SHA-256 plus the first complete differing keyword record. This localizes the field with the existing proven-reproducer schedules; different PID/work paths still prevent a general raw-parity claim. Ensure the helper is disabled for performance measurements unless its cost is included identically.

Then use a validation-only manifest schedule and identical target absolute path/TEMP for three fresh-process sequences: (a) HTFF alone, (b) COG7 -> HTFF, (c) COG7 -> F2ACT -> HTFF. Keep each predecessor's owned project path fixed as well; copy from immutable fixtures each time. Archive target raw files before cleanup and repeat each sequence at least twice. This tests the most specific cheap hypothesis using one/two/three conversions. If (b) alone reproduces and (c) restores serial bytes, the predecessor effect is established, while globals versus heap state remains open. If none reproduces, replay the full 30-case even-ordinal prefix versus the 60-case serial prefix truncated at HTFF; prefix-bisect without changing target paths.

Finally run the exact even-ordinal prefix in its own process without a concurrent sibling, with the same child environment. Persistence without concurrency favors process-history/heap causes; disappearance only with concurrent execution makes shared filesystem/timing a stronger suspect. If it reproduces, instrument only the localized field at construction, post-import, post-furnace-properties and pre-write. Compare raw value bits, relevant table shape/contents, unit/dimension and source KEY record. Do not change initialization or faithful converter semantics in this optimization lane.

The acceptance decision remains to retain serial by default until the mismatch is explained and the same-path raw parity gate passes. Choosing four workers because its adjusted log happens to agree does not resolve the demonstrated two-worker behavior change.
