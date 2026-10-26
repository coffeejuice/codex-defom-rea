# Folder sweep process-sharding design

## Purpose and evidence boundary

Reduce the serial folder-sweep critical path while preserving all 98 currently exercised cases and every existing diagnostic decision. This is a read-only design audit, not an implementation or measured speedup. No build, suite execution, original application, or REA session was used.

The audited production test is `source/apps/moproj2mst/tests/s/test_save_folder.cpp`, SHA-256 `50EA7CD32691DE0C180565B5146FDE801495ECF0336100C4A3ABDAC0B4B56F53`. The optimization copy was already different during this audit and is being edited independently; do not treat its live hash as a frozen baseline. Source qualifications remain those of the converter takeover: historical GUI oracles and adjusted comparisons are diagnostic evidence, not new original-application acceptance.

`analysis/test-relocation-results.json` binds the historical run to `build/test-runs/1792854086489696400`; it records 18.5023 seconds for `m2m.s.save_folder`. `analysis/test-speed-audit-2026-10-24.md` reports 98 cases, 93 adjusted matches, five cases with differences, 30 differing/missing files, and zero refusals. The receipt includes seven corpus projects without finished re-saves and the three explicit NOTE exclusions. These are baseline observations, not constants to enforce on arbitrary future fixtures.

## Actual isolation constraints

| State or resource | Source evidence | Required treatment |
| --- | --- | --- |
| Embedded engine process globals | `src/core/convert.cpp:153-157` documents DB Fortran units, KEY reader/model globals and guards the entire conversion with static `engineMutex` | Use processes; threads cannot parallelize conversions without changing engine semantics. |
| Engine version initialization | `src/core/convert.cpp:127-138`: static mutex, `set`, and `last`; `engine::setEngineVersion` sets engine banner state | Inherit identical conversion settings; accept fresh initialization per child only after serial parity validation. |
| Conversion temporary output sequence | `src/core/convert.cpp:69-72`: atomic sequence plus PID and TID in atomic output names | Preserve implementation; never share writable project paths. |
| Logical current directory | `src/qt4/path.cpp:302-303`: thread-local current path; conversion uses `CurrentDirScope` | Inherit the same initial process working directory; do not chdir to a worker directory. |
| Folder scratch sequence | Test `runCase`: static `seq` incremented only after a top-level `.moproj` is found and expected folder exists | Replace assignment with coordinator-provided global sequence, not a child-local sequence. |
| Fixed temporary name | `src/doe/doe_study.cpp:475`: `temp_directory_path()/m2m_temp_kwd.tmp` | Child-specific temp directory is mandatory even with unique project scratch. |
| Address-derived temporary name | `src/engine/l5_slots.cpp:428`: `m2m_l5_<address>.KEY` | Addresses can repeat across processes; isolate temp directories. |
| Other temporary files | `db_load.cpp:75`, `loader.cpp:274`, `save_current.cpp:73`, `save_l6.cpp:68`, `ht_save.cpp:102`, `postload_views.cpp:110`, `heattreat_htwz.cpp:462` use PID/TID names | Preserve names, still isolate all children from each other and concurrent CTests. |
| CTest environment | `source/apps/moproj2mst/CMakeLists.txt:39-47` already gives each test its own scratch and TEMP/TMP/TMPDIR | That isolates CTests, not child processes within this test. Derive child temp roots beneath the assigned test scratch. |

This bounded audit does not prove every hidden engine global resets equivalently between cases. The existing test runs many conversions in one process; N children change their predecessor histories. Identical public settings and process isolation alone do not prove unchanged raw bytes. Serial-versus-sharded parity is a release gate. If predecessor-sensitive differences appear, retain the serial path and investigate; never reset additional engine state, normalize differences away, or silently use fresh-process semantics as the new oracle.

## Minimal interface and orchestration

Keep the existing no-argument serial executable and its environment gate intact. Add one public environment setting, `M2M_FOLDER_WORKERS`, absent or `1` meaning existing serial behavior. Accept positive integers 1 through 16 for this experiment; malformed values fail as harness configuration errors rather than silently reducing coverage. Leave `M2M_FOLDER_SWEEP`, `M2M_CORPUS`, `M2M_ORACLE_B`, and `M2M_TEST_SCRATCH` semantics unchanged. The existing return-without-work when the first two gates are absent happens before sharding setup.

The executable acts as coordinator for N > 1 and launches itself with one private CLI mode, `--folder-worker <absolute-job-manifest>`. A job manifest identifies schema version, parent run identity, fixture/source identity, ordered case records, exact work paths, result destination and worker number. Private mode bypasses `testkit::run_all()` so children do not print duplicate test wrappers or recursively spawn. The coordinator owns the one existing testkit wrapper. Use Unicode Windows process creation with properly quoted executable/manifest arguments; no shell command composition.

For the simplest first implementation, make N fixed job lists with global eligible-case ordinal modulo N, preserving ordinal order within each child. This bounds startup overhead to N processes and makes assignment replayable. Only after equivalence and timing are established should duration-balanced static assignment be evaluated; emit the complete schedule into the receipt. Dynamic queues are unnecessary for the first candidate and make history dependence harder to diagnose.

Create TEMP, TMP and TMPDIR as distinct existing child roots, for example `<test scratch>/fs_<parent pid>/w<id>/tmp`, before process creation. Override these variables only in each child's environment block, not in the parent process. Child stdout/stderr go to private capture files; only the coordinator prints aggregate output. Ensure each worker sees the same fixture paths, codepage 1252, empty output argument and `M2M_FLAG_WRITE_SAVE`. No converter ABI change is needed.

## Freeze discovery, order and work paths

The current filesystem iteration order is unspecified by C++, but it is the existing printed order. Sorting case names would change that order. Enumerate once in the coordinator, exactly through the current corpus loop, then the three explicit B re-saves, then the oracle-B directory loop. Record a stream of events, not merely a sorted set of case names:

1. A-case records in current corpus iterator order, using only saved entries from `_tools/rs_log.tsv` with both RS folders present. Increment `noResave` on exactly the existing predicate.
2. The corpus summary marker and missing-re-save notice marker.
3. B:R11, B:R19, B:82_R52_BEND_2D in that fixed order.
4. The oracle-B iterator events, preserving the three NOTE positions and both silent exclusions. Never promote RS_*_before folders into B cases.
5. The oracle-B summary marker.

Preserve `runCase` preflight: enumerate top-level input entries and retain the last entry whose extension is exactly `.moproj`, just as now. Do not sort, choose the first, search recursively or case-fold that extension. Record the chosen filename. Missing `.moproj` or missing expected folder produces no case count, scratch ordinal or printed line. An input enumeration exception remains an error, not a silent missing-input skip. Freeze fixture manifests so discovery cannot race changing inputs.

For an eligible attempt, allocate a global 1-based work ordinal before dispatch. Keep its path exactly `<M2M_TEST_SCRATCH>/m2m_folder_sweep_<coordinator PID>/<ordinal>`. Disjoint case leaves allow concurrent writes without changing the existing work-path shape or ordinal meaning. Workers must never remove the parent sweep root; only the coordinator performs final root cleanup after all children terminate. Children retain the existing per-case cleanup timing, including copy-failure residue until root cleanup. Worker temp directories live outside the compared case leaf, so they cannot appear as extra files.

Retain recursive copy with the existing flags/error-code handling. Do not hardlink mutable fixtures, reuse previous output trees, or rewrite inputs. A junction-aware containment preflight of owned scratch roots is appropriate for the new harness, but must not alter the converter's faithful failure semantics.

## Exact comparison and tally contract

Run the existing comparator unchanged inside each worker. In particular preserve:

- case-sensitive extension exclusions `.png` and `.oracleA`, exact `view_state.json`, lowercased relative keys in ordered maps;
- raw equality before expected-path relocation, whole-path-component checks, both slash forms, and expected-only relocation;
- `.moproj` group/cycle Expanded masking and DOE-only view masking;
- banner-only classification and all three older-window exceptions;
- complete differ/missing/extra vectors internally, printed first-eight truncation and remainder counts;
- all existing text, punctuation, notes and summary positions.

Each result contains ordinal, section, name, exact rendered case output bytes, raw return status/message, and delta for every Tally field. It also contains an explicit terminal state: silent-ineligible, copy-skip, refused, or compared. `cases` increments before the copy; copy failure contributes one case but neither refusal nor comparison and prints SKIP. Refusal contributes one case plus one refused and prints the same 200-character message prefix. SAME includes adjusted matches: banner files alone do not count as file diffs; the older-window case clears its one reported difference and increments `viewProfile`. Do not reinterpret these as strict raw equality.

The coordinator validates exactly one complete result for each assigned record, rejects duplicates, unknown ordinals, wrong source/fixture identity or malformed/truncated files, sums integer deltas by A/B section, and replays the original event stream. Print all case output at its discovery position, then the original summaries and notes. Never sum child summary strings or reorder results by completion time. Missing results, crashes, timeouts or spawn failures are harness failures; they must not become REFUSED, SKIP or a passing partial sweep. Existing diagnostic differences/refusals still do not fail assertions. Keep infrastructure errors separate from scientific differences.

## Raw parity and fixture integrity

The existing human log does not contain raw output hashes or complete untruncated raw difference vectors. Add a separate equivalence receipt, leaving human output unchanged, for each case's full raw relative file inventory, sizes and SHA-256 values, raw/relocated/masked decision stage and full adjusted vectors. Bind the receipt to executable and source hashes. This instrumentation must be identical in serial and parallel experiments and included in timing if retained in the optimized validation path.

A raw cross-run hash mismatch can arise from the existing PID/work-path relocation in saved files. Do not promise byte-identical cross-run hashes while using different absolute scratch locations. For strict raw validation, replay serial and sharded schedules sequentially against the same explicit coordinator-owned case paths in a validation-only manifest mode; remove/recreate each leaf using existing copy logic between runs. Archive raw inventories before cleanup. Changing worker TEMP paths can also expose actual path-sensitive output; inspect it rather than adding new masks. If exact same-path replay cannot be implemented, retain raw hashes and report their differences explicitly, but do not claim raw-byte equivalence based only on adjusted tallies.

Keep the launcher's full before/after SHA-256 fixture snapshots over the same four roots and exact file membership. Immutable manifests must match, including detection of same-size/time-preserving mutations. Sharding does not justify skipping either snapshot or changing integrity coverage. Per-case project copies and all IPC/temp artifacts remain under takeover scratch; do not execute or modify original applications/projects.

## Benchmark matrix and acceptance

Freeze a single binary, comparator, fixture inventory and manifest first. Run the old serial path and new one-worker path separately to quantify harness/receipt cost and verify the serial reference. Then measure N = 1, 2, 4, 8, 10, 16 on the same host. At least five paired measured repetitions per N, after one separately recorded warm-up, with rotated N order to reduce cache/drift bias. Do not call runs cold-cache unless an explicit reproducible cache method is used. No cache flushing or antivirus exclusions are required by this proposal.

| Workers | Purpose | Additional concern |
| --- | --- | --- |
| 1 | serial equivalence and new harness overhead | No claimed process-parallel speedup. |
| 2 | first proof that isolation and aggregation work | fixed-name temp collision must be prevented. |
| 4 | moderate CPU/IO overlap | identify copy/read saturation. |
| 8 | likely useful ceiling candidate | quantify contention with other CTests. |
| 10 | requested host-scale point | agent capacity is not CPU capacity. |
| 16 | oversubscription experiment | report memory/IO pressure and failures honestly. |

Record external sweep wall time including spawn, manifest generation, IPC, comparison, receipts and cleanup; per-case copy, conversion, inventory/read, comparison and cleanup times; process start/exit times; peak concurrent processes and memory; CPU time and available IO counters. Report unavailable counters as unavailable. Publish median and range or IQR, speedup versus the paired serial median, and all failure/skip/refusal counts. Sum of child elapsed times is not wall time. A lower bound is the largest measured individual case, plus unavoidable coordinator work; IO contention can prevent linear scaling.

First benchmark the sweep alone with no competing builds/captures. Then run the unchanged full CTest membership with the launcher's normal job setting, recording nested concurrency: eight CTest jobs plus N sweep workers can oversubscribe substantially. Finally measure full launcher entry-to-exit, preserving both integrity snapshots. Report sweep, CTest and complete-launcher speedups separately. The historical 58.598-second full launcher and 18.5023-second sweep do not establish any current speedup. If fixture hashing remains approximately 40 seconds, sweep sharding alone cannot approach a tenfold full-launcher improvement.

Accept a worker count only if all repetitions preserve the complete ordered case/event manifest, every raw difference with same-path qualification, all adjusted vectors and statuses, exact summaries/notes/skip/refusal behavior, unchanged fixture manifests, and complete nontruncated output. Exercise synthetic copy failure, converter refusal, missing-moproj/expected preflight, worker crash and incomplete result separately so normal zero-refusal fixtures cannot conceal broken aggregation. Select the smallest count within measured noise of the best complete-validation median; do not select 16 merely because it is available.
