# Profile v1 aggregate and case-cost analysis

## Evidence and method

This is a read-only aggregation of `profile-v1-ctest.log`. CTest ran four tests concurrently; the recorded wall time was 19.53 s. The Save-folder test took 19.50 s and logged 98 per-case rows. The other test durations were 3.10 s (`release.unchanged`), 4.89 s (`save_current`) and 5.44 s (`save_io`); they overlap the folder test and therefore cannot be added to the 19.53 s wall time.

The folder rows report microseconds for copy, conversion, compare, and cleanup. Their summed work is 19.450 s: copy 3.465 s (17.8%), conversion 8.456 s (43.5%), compare 6.370 s (32.7%), cleanup 1.159 s (6.0%). These are component-time sums from a single instrumented run, not separate wall-time phases. Profiling also counts copied files and bytes, which adds filesystem metadata work; treat rankings as indicative and timing as a single-run sample.

## Highest-cost cases

| Case | Total ms | Copy ms | Convert ms | Compare ms | Cleanup ms |
|---|---:|---:|---:|---:|---:|
| A:RS_07_CHAMF1 | 1,006 | 304 | 320 | 295 | 88 |
| B:113_V_CHAMF1 | 873 | 275 | 290 | 231 | 76 |
| A:RS_25_Inc718_D420_GFM_V_DIES_Angle2_18 | 543 | 40 | 288 | 204 | 11 |
| A:RS_26_Inc718_D420_GFM_V_DIES_Angle2_19 | 541 | 39 | 282 | 208 | 12 |
| A:RS_28_Inc718_D420_GFM_V_DIES_Angle2_18 | 537 | 35 | 280 | 209 | 12 |
| A:RS_22_YZ_H01 | 404 | 23 | 281 | 91 | 8 |
| B:R50F | 398 | 102 | 106 | 161 | 29 |
| A:RS_24_ZY_H01 | 396 | 23 | 275 | 91 | 8 |
| A:RS_21_YX_H01 | 395 | 19 | 278 | 90 | 8 |
| A:RS_23_ZX_H01 | 394 | 20 | 277 | 89 | 8 |

The two CHAMF1 cases and three Inc718 variants account for about 3.50 s, or 18% of all measured case work. The two CHAMF1 cases have substantial cost in every phase; reducing conversion alone would leave copy and compare close to 60% of their observed time. The Inc718 variants are conversion- and compare-heavy. B:R50F and the R47 cases are comparatively copy/compare-heavy.

## Ideal worker-count ceiling

For a scheduling-only upper bound, I assigned the 98 measured case durations in descending order to the currently least-loaded worker. This assumes independent cases, no shared-resource slowdown, no orchestration overhead, and timings unchanged by concurrency.

| Workers | Ideal folder-sweep makespan | Ideal speedup vs serial |
|---:|---:|---:|
| 1 | 19.45 s | 1.00x |
| 2 | 9.73 s | 2.00x |
| 4 | 4.87 s | 3.99x |
| 8 | 2.45 s | 7.94x |
| 10 | 1.97 s | 9.89x |

These are not expected observed results. The longest indivisible case alone imposes a 1.01 s lower bound, and concurrent filesystem copies, comparison reads, antivirus scanning, and CPU/cache pressure can reduce scaling. `ctest -j` cannot parallelize cases inside the current single Save-folder test; this estimate requires case-level dispatch while retaining all 98 case checks and their isolation.

## Interaction with the other CTest cases

The log shows `release.unchanged`, `save_current`, `save_folder`, and `save_io` launched together. Thus the existing 19.53 s suite result already hides much of the three shorter tests behind the folder sweep. Accelerating only the folder test will expose those tests as the new wall-time floor: the measured release/current/io durations are 3.10/4.89/5.44 s, though their durations may change under added contention. Their scratch working directories are distinct in the log, but they all read common evidence/corpus locations. More concurrent file copying and comparison reading could therefore lengthen these tests or compete with shared-storage cache/AV and undermine the ideal scheduling bound. The `save_io` test also exercises native Windows handle/share/lock behavior, so preserve that test and its focused assertions while tuning worker count. The release test performs content snapshots and hashing; keep its full integrity checks and measure whether it overlaps the new worker load. `save_current` is shorter than the sweep but also uses corpus/oracle evidence; include it in paired suite-level timing.

Current evidence supports a concurrency experiment, not a speed claim. Compare the exact 98-case membership, result tallies, scratch isolation and full release integrity at worker counts 1/2/4/8/10, and record both case-level phases and full CTest wall time. No source, build, or test was changed or run for this aggregation.
