# Converter test-time audit, 2026-10-24

## Boundary and measurements

The latest `analysis/test-relocation-results.json` reports 148 CTests, `-j 8`,
17.594 seconds of CTest subprocess wall time, 141 clean passes, six passes with
logged gaps, one diagnostic pass with differences, and no failed tests or changed
fixtures. Its 17.594-second timer excludes inventory, before/after fixture
snapshots, JUnit classification and receipt writing in
`admin/run_converter_tests.py`. Therefore it is **not** the full validation time.

JUnit gives `m2m.s.save_folder` 17.5439 seconds, 99.7% of the recorded CTest
wall time. That executable serially processes 98 project Save cases. The next
longest individual tests take 5.57049 seconds (`save_io`), 4.99317 seconds
(`save_current`), 3.3654 seconds (`release.unchanged`), 2.41815 seconds
(`mdt_write`), and 1.94049 seconds (`runner_self_test`). The sum of all JUnit
test times is 50.2774 seconds under concurrent execution. More CTest jobs alone
cannot shrink the single folder-sweep critical path.

The four fixture roots contain 16,728 files totaling 1,867,669,827 bytes. A
read-only call to the launcher's exact `snapshot()` function on these roots
completed in 20.681 seconds with the current filesystem cache. The launcher
performs this operation twice per full run, outside its reported CTest timer.
This is one warm-cache sample, not a claimed repeated-run median or a measure
of the complete launcher wall time.

The folder sweep is diagnostic: 93 of 98 cases are adjusted matches and five
have logged differences, 30 differing/missing files in total, with no refusals.
It uses path relocation, view/Expanded masks, banner handling and three older
window exceptions. It is historical evidence, not fresh original acceptance.
Do not remove cases, weaken comparisons or suppress gaps to meet a time target.

After candidate 45 integration, a complete externally timed launcher run took
58.598 seconds; its included CTest subprocess took 18.563 seconds. The run
registered 150 tests: 143 clean passes, six passes with logged gaps and one
diagnostic pass with differences; no failures, incomplete outputs or fixture
changes. The sweep took 18.5023 seconds. The observed non-CTest remainder is
40.035 seconds, including both fixture hashes and launcher overhead. This is
one measured full run, not a stable performance distribution. The equivalent
10x full-launcher target for this run is 5.8598 seconds, while the CTest-only
target is 1.8563 seconds. Its receipt is `analysis/test-relocation-results.json`
and run directory `build/test-runs/1792854086489696400`.

## Tenfold target and constraints

A 10x reduction of the **recorded CTest metric** requires at most 1.7594
seconds. Speeding or deleting only the folder sweep leaves a 5.57049-second
test, so the other long tests also need work. For the **complete validation
launcher**, define the baseline by timing entry to exit; the two full SHA-256
fixture passes make that target distinct and potentially harder. No 10x gain is
established by the current receipts or this audit.

## Proposed experiment, in dependency order

1. Freeze candidate and fixture SHA-256, registered test list, environment,
   raw diagnostic rows and current adjusted comparator results. Time the full
   launcher externally over repeated warm and cold runs. Record CTest time,
   fixture snapshot time, parsing/report time, median and spread separately.
2. Add per-case, per-phase timers to the folder sweep without changing output
   decisions: copy, converter, enumerate/read, compare/normalize and cleanup.
   Count bytes/files and process startup, CPU and filesystem I/O. Instrument
   `save_io`, `save_current`, `release.unchanged` and `mdt_write` sufficiently to
   locate their own serial floors.
3. Optimize the measured dominant phase. Likely candidates are isolated
   process sharding of the 98 cases with deterministic aggregation; faster
   disposable copies and raw comparisons; and reuse of *immutable expected*
   metadata keyed by content. Do not cache conversion outputs, hardlink writable
   inputs or reuse scratch trees. Keep real Windows failure/locking tests.
4. Investigate fixture-integrity cost separately. Preserve before/after
   content-integrity guarantees and equivalent root coverage; metadata-only
   change detection is insufficient. Test any faster hashing implementation
   against the exact existing 16,728-file manifest, including mutations that
   keep size and write time. Include cache construction/invalidation cost in
   complete-launcher timing.
5. Benchmark one change at a time at 1, 2, 4, 8 and where supported 10+ isolated
   case workers, using repeated paired runs. Require identical case membership,
   status, raw file hashes/differences, adjusted comparison decisions, notes,
   skip/refusal counts, fixture manifests and ABI/regression outcomes. Preserve
   original-order reporting even when cases finish out of order. Keep the full
   acceptance suite; an incremental mode may exist only with a distinct name.

Accept 10x only when the complete equivalent workload reaches the externally
timed target in repeated runs. If measured per-case conversion or I/O floors
prevent it, report the actual ceiling rather than reducing evidence.

## Agent-capacity investigation

Official Codex documentation lists `[agents].max_concurrent_threads_per_session`
as the cap on spawned agents, excluding the primary; the legacy alias is
`agents.max_threads`. This session exposes four concurrent slots including the
orchestrator, so it currently runs at most three workers. A configuration value
of 9 would request ten total agents in a *new* session, subject to host or
managed limits. This audit has not changed global/project configuration or
verified that the desktop host will honor nine worker slots. At that scale,
retain exclusive file ownership, a single build/integration owner, serialized
original captures, bounded evidence, and task-level acceptance; ten agents do
not imply tenfold speedup.

Source: https://learn.chatgpt.com/docs/agent-configuration/subagents#global-settings
