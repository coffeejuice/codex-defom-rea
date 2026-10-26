# Tenfold speedup feasibility and current full-run result

## Conclusion

The best measured full launcher run is **25.25 s**, with the serial-conversion folder pipeline, CTest `COST=25`, uncached SHA-256 hashing using four workers plus parent-cache path resolution, and either CTest `-j 4` or `-j 16`. Both jobs settings returned 25.25 s at the launcher timer's 10 ms precision; the observed CTest phases were 12.031 s and 12.187 s respectively, so there is no measured advantage to raising CTest jobs to 16. The `-j 4` run is the conservative choice.

This is a complete 150-test launcher run with full before/after fixture manifests, not a fast-feedback subset. Its full-run evidence comparison to checkpoint-45 is equivalent with no blockers. The raw 98-case folder comparison also passes with one narrow, retained work-path qualification described below. These are historical-suite and implementation-equivalence results, not fresh original-application acceptance.

Tenfold full-launcher acceleration has **not** been achieved. Against the warm paired serial baseline of 47.371 s, 25.25 s is a 1.88x speedup (46.7% less elapsed time). Against the older 58.598 s historical launcher timing, it is 2.32x (56.9% less). More decisively, the measured serial conversion phase alone sums to 8.456 s for the 98 folder cases. The serial-conversion pipeline retains that work as a sequential phase. A 10x target from the warm paired baseline would be 4.737 s, already below this conversion-only floor. Even if every other launcher phase vanished, that measured conversion workload caps speedup at about 5.60x versus the warm baseline (6.93x versus the historical timing), before accounting for any other test, hashing, or reporting work.

## Full-equivalent result and raw folder qualification

The 25.25 s runs use all 150 registered CTest tests, CTest jobs 4 or 16, and complete uncached SHA-256 fixture snapshots. Both receipts report 143 passed, 6 passed with logged gaps, 1 passed with logged differences, zero failed, zero skipped, and no unreported or incomplete-output tests. The full-run comparators `pipeline-parent-cache-j4-equivalence.json` and `pipeline-parent-cache-j16-equivalence.json` each report equivalent with zero blockers against the checkpoint-45 run. Permitted differences are explicit: path/run locations, timing fields, hash execution metadata, CTest job scheduling, and the folder test's `COST=25` scheduling property. The fixture before/after manifests remain exact; the parent-cache is snapshot-local directory-resolution reuse, not cached content digests.

Independent serial-versus-pipeline raw folder receipts cover all 98 ordered cases. Their semantic statuses, refusal data, complete comparison vectors and decision stages, plus generated-file paths/sizes/SHA-256 values, match with no blockers after the specific capture qualification. One generated file, `B:98_B_DS3F/Gear_blank.MST`, is 2,821 bytes in the serial capture and 2,820 bytes in the pipeline capture because the owned work-root path has a different length. Same-run retained captures show that replacing exactly one validated work-root prefix on each side yields byte-identical content with matching normalized SHA-256; every other generated output is equal. This is a qualified path-only difference, not raw byte identity across all files. The other receipt hash changes record execution provenance. The pipeline keeps converter calls on the coordinator in serial order; this parity result is separate from the rejected two-process sharding experiment that changed B:107.

The full-suite comparison establishes unchanged recorded behavior against the historical checkpoint within those rules. It does not turn the historical oracles into fresh-session original-application evidence, and the recorded gaps/difference remain present and visible.

## Paired serial-hash versus two-worker runs

The paired full-launcher matrix used the same isolated build, all 150 tests, CTest `-j 8`, one folder worker, and complete fixture snapshots; only hash execution changed from serial to the threaded helper with two workers. Every run completed with recorded gaps and compared equivalent to the checkpoint-45 evidence.

| Pair | Serial control | Threaded hash, 2 workers | Change |
|---:|---:|---:|---:|
| 1 | 47.727 s | 32.473 s | -15.254 s (-31.96%) |
| 2 | 47.014 s | 34.950 s | -12.064 s (-25.66%) |

The two-run medians were 47.371 s for the serial controls and 33.712 s for the two-worker treatment, a 28.8% median reduction. CTest itself was effectively unchanged (control median 16.250 s; treatment median 16.383 s), locating the observed reduction in the full fixture-snapshot/launcher path rather than the test execution. These paired runs predate parent-cache resolution and the `COST=25` scheduling property, so they establish the hash-worker contribution under that earlier complete configuration; do not add their savings to the later 25.25 s result.

The historical full-launcher timing remains a separate observation: 58.598 s overall, with CTest 18.563 s and other launcher phases 40.035 s. It is not a paired warm control. Warm paired controls were 47.014–47.727 s. The 25.25 s current best was measured later under changed hash/path and CTest scheduling settings, with full equivalence checked independently.

## Phase floor and limits on further gains

The serial profile reported 8.456 s summed across the 98 conversion calls. This is a measured sequential-work floor for the serial-conversion pipeline, not a full-launcher phase timer and not proof that every future run will spend exactly this duration. It was measured in `profile-v1-ctest.log`; it should be used conservatively as a lower-bound constraint when evaluating designs that preserve the ordered serial conversion calls.

The full launcher also retains inventory, two full fixture snapshots, CTest work outside folder conversion, and report generation. The 25.25 s receipt breaks down as inventory 0.687 s, before snapshot 6.157 s, CTest 12.187 s, after snapshot 6.156 s, and report parsing 0.063 s for the `-j 16` run. With `-j 4`, the corresponding figures are 0.703, 6.266, 12.031, 6.203, and 0.047 s. The phase sums differ from launcher total by timer boundary/rounding. Both runs still use complete snapshots.

The older ideal schedule model put 10-worker LPT folder execution at 1.97 s, but that prediction assumed unchanged per-case costs, no worker overhead, and no contention. It is not a replacement for the measured 8.456 s sequential conversion phase in the accepted pipeline, nor a full-launcher runtime. The current 25.25 s result shows meaningful progress while retaining semantic checks; it does not approach a tenfold full-run goal.

## Fast feedback versus full validation

Use the fast-feedback path to compare targeted phases and settings during iteration, but do not label its elapsed time a full-launcher speedup. A CTest-only duration omits inventory, both fixture hashes, comparison reporting and their timer overhead. The 25.25 s results above are full-equivalent validation runs: they include all registered tests and both full fixture snapshots. The two-worker hash matrix also ran the full suite and both snapshots, but under an earlier configuration, with paired controls.

The defensible current statement is: full equivalent validation completed at 25.25 s with the 150-test suite and full uncached fixture SHA-256 checks; its evidence retains six logged gaps and one logged-difference test, matching checkpoint-45; raw serial-conversion-pipeline folder receipts match across all 98 cases with one verified work-path-only byte qualification; and the tenfold full-launcher target remains unachieved and is bounded below 10x by the preserved serial conversion work.
