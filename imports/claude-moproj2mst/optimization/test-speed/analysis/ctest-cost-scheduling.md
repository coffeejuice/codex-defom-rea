# Schedule the folder sweep early

The isolated experiment now gives `m2m.s.save_folder` an explicit CTest `COST`
of `25.0`. The property changes parallel scheduling priority only. The test
command, environment, membership, labels, assertions and fixture handling remain
unchanged. The edit is confined to the experiment's converter `CMakeLists.txt`.

## Evidence

Installed CMake documentation at
`C:/Program Files/CMake/share/cmake-4.4/Help/prop_test/COST.rst` states that parallel
tests run in descending cost order. An explicit floating-point cost overrides
the normal weighted estimate derived from previous executions. Cost is not a
timeout, resource reservation, process count or test-duration limit.

The inspected `build/test-speed-candidate/Testing/Temporary/CTestCostData.txt`
entry was `m2m.s.save_folder 19 1.29055`. That scheduling estimate is much shorter
than its observed full diagnostic sweep. Focused experiment executions have
shared the build directory, so the historical estimate does not represent the
full-suite workload reliably.

The completed pipeline run `build/test-runs/1792860215872606500/ctest.log`
records the folder sweep as the **13th Start entry**, followed by a 12.49-second
test duration. Ordinal counting must allow padded test numbers (`Start\s+\d+`),
otherwise one-space matching incorrectly counts it eighth. CTest reports 15.44
seconds total; `analysis/pipeline-full-launcher-console.json` records 15.468
seconds around the CTest invocation. The approximately 2.98-second difference
between launcher elapsed time and folder duration is not an isolated measure of
queue delay: it can include launch/reporting overhead and other work.

## Expected effect and validation boundary

An explicit cost of 25.0 should move this long test ahead of lower-cost tests,
allowing its work to overlap more of the remaining suite at the existing `-j 8`.
It does not guarantee a particular total duration or that this is the optimal
priority under CPU/disk contention. No speedup is claimed before a serially
scheduled full validation run measures it. No configure, build or test was run
in this edit lane; the existing generated CTest inventory remains unchanged until
the orchestrator regenerates it.

The comparator must explicitly qualify one inventory metadata change:
`m2m.s.save_folder` gains `COST = 25.0` (possibly serialized numerically as `25`).
Do not broadly ignore all property differences: commands, environment,
membership, dependencies, skip behavior and required output checks must retain
their existing comparisons. Scheduling order and duration can differ under the
existing timing qualification, while per-test diagnostic evidence must still
match. Coordination of that comparator qualification is owned by the orchestrator.
