# B:107_B_HTFF serial versus two-worker mapping

## Ordinal and predecessor mapping

`B:107_B_HTFF` is the 60th eligible Save-folder case in the `profile-v1-ctest.log` order. Its measured profile-v1 components were copy 20.827 ms, convert 44.157 ms, compare 37.251 ms and cleanup 7.916 ms, totaling 110.151 ms. The profiling census recorded 27 copied files / 2,719,651 bytes and 27 compared pairs / 5,454,532 bytes read.

The sharder uses static assignment `(ordinal - 1) % workers`, with 1-based ordinals and workers. At two workers, ordinal 60 goes to worker 2 (the even-ordinal lane). Its immediately preceding assigned ordinal in that worker is 58, `B:105_B_COG7`; ordinal 59, `B:106_B_F2ACT`, is assigned to worker 1 and is the immediate predecessor only in the serial order. The next worker-2 case is ordinal 62, `B:109_B_FE7`; ordinal 61 (`B:108_B_SOPTA`) is on worker 1.

| Ordinal | Case | Serial position | Two-worker lane |
|---:|---|---|---|
| 58 | B:105_B_COG7 | Before B:106 | Worker 2, predecessor of B:107 |
| 59 | B:106_B_F2ACT | Immediate serial predecessor | Worker 1 |
| 60 | B:107_B_HTFF | Target | Worker 2 |
| 61 | B:108_B_SOPTA | After B:107 | Worker 1 |
| 62 | B:109_B_FE7 | After B:108 | Worker 2, next after B:107 |

## Observed output change

In `folder-serial-focused.log`, B:107 is `SAME` (0 differing, missing or extra files). In the successful `folder-shard2-focused-v2.log`, B:107 is `DIFF` with one differing file, `operations/task00001/simctrl.key`. The parser comparison of case rows finds exactly one status/count change among the 98 eligible cases: B:107. `A:RS_55_Multiple_Operations_Example1` remains `DIFF` with one file difference in both logs, so it is not a new sharding delta.

The aggregate summary correspondingly changes from 45 identical / 29 file differences in the B oracle group to 44 identical / 30 file differences. The A fresh re-save summary remains at 48 identical / 1 file difference. Both focused tests report pass because these ordinary comparison differences are result data, not test failures. The serial run took 14.40 s; the successful two-worker run took 7.79 s. These are single, not paired, runs and the B:107 result differs, so the runtime reduction is not an accepted equivalent speed comparison.

The predecessor mapping makes serial-sequence dependence a concrete hypothesis to investigate: in the serial case stream B:107 follows B:106, whereas its two-worker lane follows B:105. This is correlation only. The logs do not show the internal cause, and unrelated environment/path/timing sensitivity cannot be excluded. Keep the serial path as the acceptance oracle until the same case is replayed against controlled identical work paths with captured raw file inventories and decision evidence. Do not classify B:107 as newly validated behavior from the changed output.

## Evidence boundary

Compared only the retained profile and focused CTest logs plus the sharder’s documented ordinal rule. The earlier `folder-shard2-focused.log` is an initial failed attempt (0.13 s, with no case result stream); the valid comparison is against the later `folder-shard2-focused-v2.log`. No source, build, tests, or fixture files were accessed or changed for this analysis.
