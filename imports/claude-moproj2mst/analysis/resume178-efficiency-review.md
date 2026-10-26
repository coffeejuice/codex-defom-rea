# CP176–178 converter efficiency review

## Scope and recorded outcomes

Read-only review of `resume176/177/178-dual-build.json`, their full test/timing receipts, and the matching root integration/freeze notes. Each checkpoint has a source-stable frozen build over 3,554 files. Each full suite reports 354 clean passes, six logged-gap suites and one diagnostic-difference suite, with zero failed, skipped, incomplete-output, fixture-change, or changed-diagnostic cases. The historical 93 SAME / five DIFF comparison remains qualified; none of these freezes is fresh original-session acceptance.

## Command timings

| Checkpoint | Build workflow sum | Configure | Focused preflight | Focused CTests | Full build phase | Full CTest | Full launcher |
|---|---:|---:|---:|---:|---:|---:|---:|
| CP176 | 47.595 s | 2.297 s | 5.953 s | 9.626 s across 6 targets | 29.719 s | 16.296 s | 33.141 s |
| CP177 | 48.124 s | 2.328 s | 6.187 s | 9.781 s across 6 targets | 29.828 s | 16.344 s | 33.109 s |
| CP178 | 48.251 s | 2.297 s | 6.469 s | 9.673 s across 6 targets | 29.812 s | 16.578 s | 33.469 s |

Across the three full suites, raw CTest totals 49.218 s and full launcher timing totals 99.719 s. Launcher timing includes inventory, fixture hashes, source/binary binding, JUnit parsing and report output. The roughly 50.5 s difference is accounted for by those wrapper phases; it is not unattributed idle time. Full-suite launcher times vary only 0.360 s, and raw CTest varies 0.282 s.

The build workflow sums total 143.970 s across three recorded workflows. These are sums of per-command elapsed values, not an end-to-end or wall-clock duration. The work is described as disjoint dual lanes, but receipts do not break out per-lane elapsed time or overlap. Full build phases are tightly grouped at 29.719–29.828 s. The recorded focused test targets include one roughly 5.4-second target in each checkpoint; other focused targets are mostly below 1.1 seconds. These are individual command durations, not the duration of an entire lane or review cycle.

## Dual-lane work and rework context

The integration notes pair each constructor cut with independent editor300 work: CP176’s private `+0x78` zero store and L6 readers; CP177’s `+0x80` zero store and HeatTreat access closure; CP178’s qualified version decision and 13 L6 SimControl accesses. All three have successful dual-build, focused, full-suite, and freeze receipts. No failed build or full-suite attempt appears in these CP176–178 receipts. CP178 implementation authors marked their source-only work unbuilt; root integration subsequently ran the successful gates, so those statements describe ownership boundaries, not a missing final test.

The persistent six logged-gap suites and one diagnostic-difference suite are unchanged checkpoint classifications. Successful CTest exit does not remove those qualifications or establish new original-binary parity. No token telemetry, total task wall time, per-lane duration, reviewer active time, or serial baseline is available. Therefore these command timings cannot quantify concurrency benefit, review/rework cost, or overall recovery speed.

## One safe improvement to measure

Batch independent focused CTest targets for each already compiled source snapshot into one CTest invocation using an explicit test-name regex/list, while retaining per-test JUnit/results and the same source binding. The current receipts show six focused invocations per checkpoint, with several taking under one second; one grouped invocation may reduce repeated process/harness setup. Pilot this on a future checkpoint and compare summed command elapsed time and grouped invocation elapsed time, while still running every focused case, the full suite, original/fidelity comparisons, source review and freeze. If the runner cannot preserve individual receipts or the exact same test set, keep the present calls. This is a test-orchestration candidate only; no savings are measured by the current data.

## Conclusion

CP176–178 show stable build and full-suite command timings and the same qualified gap/DIFF status. The available receipts support measured command-cost comparisons, not dual-lane wall-time or token savings. Grouping focused test invocation is a bounded improvement to benchmark without weakening source-stable freeze or original-fidelity gates.
