# Save-folder static schedule simulation

## Input and assumptions

The input is the 98 `M2M_FOLDER_PROFILE` rows in `profile-v1-ctest.log`. Case duration is the sum of its recorded copy, convert, compare and cleanup microseconds. Total measured case work is 19.450 s. The simulations perform no tests and predict no measured runtime: they only calculate worker load from this one instrumented profile.

Ordinal round-robin assigns input case `i` to worker `i mod W`, preserving the log order. Longest-processing-time (LPT) sorts cases by descending measured duration and repeatedly assigns the next case to the worker with the lowest accumulated load. Equal-duration ties use the log's stable order. Both are static assignments with no overhead or resource contention.

## Makespan and load balance

The average column is total recorded work divided evenly by worker count. “Spread” is maximum worker load minus average load, shown as a percentage of average; it represents scheduling imbalance above the ideal average, not additional measured time.

| Workers | Mean load | Round-robin loads (s) | RR makespan | RR spread | LPT loads (s) | LPT makespan | LPT spread | LPT reduction vs RR |
|---:|---:|---|---:|---:|---|---:|---:|---:|
| 2 | 9.725 s | 9.503, 9.947 | 9.947 s | 2.3% | 9.721, 9.729 | 9.729 s | 0.0% | 2.2% |
| 4 | 4.863 s | 4.411, 5.471, 5.092, 4.477 | 5.471 s | 12.5% | 4.860, 4.860, 4.869, 4.861 | 4.869 s | 0.1% | 11.0% |
| 8 | 2.431 s | 2.493, 3.619, 2.042, 2.056, 1.918, 1.851, 3.050, 2.421 | 3.619 s | 48.9% | 2.446, 2.415, 2.448, 2.420, 2.442, 2.444, 2.418, 2.417 | 2.448 s | 0.7% | 32.4% |
| 10 | 1.945 s | 2.008, 1.679, 1.663, 1.972, 1.822, 2.719, 2.453, 2.085, 1.556, 1.492 | 2.719 s | 39.8% | 1.935, 1.955, 1.935, 1.942, 1.966, 1.947, 1.943, 1.947, 1.938, 1.941 | 1.966 s | 1.1% | 27.7% |
| 16 | 1.216 s | 1.525, 2.265, 1.318, 1.172, 0.985, 1.130, 2.257, 1.383, 0.968, 1.354, 0.724, 0.884, 0.932, 0.721, 0.792, 1.038 | 2.265 s | 86.4% | 1.226, 1.225, 1.209, 1.207, 1.210, 1.225, 1.225, 1.226, 1.201, 1.216, 1.207, 1.203, 1.201, 1.226, 1.229, 1.215 | 1.229 s | 1.1% | 45.8% |

RR worker case counts are nearly even (2: 49/49; 4: 25/25/24/24; 8: 13/13/12/12/12/12/12/12; 10: 10/10/10/10/10/10/10/10/9/9; 16: ten workers with 6 cases and six workers with 7). Similar case counts do not imply similar time: at 16 workers the RR heaviest worker receives 2.265 s while the lightest receives 0.721 s.

LPT puts the long cases early and balances cumulative time. It gives approximately equal counts as well (at 4 workers, 24/24/25/25 cases). Its four-worker busiest assignment begins with A:RS_25_Inc718_D420_GFM_V_DIES_Angle2_18 (0.543 s), A:RS_22_YZ_H01 (0.404 s), A:RS_24_ZY_H01 (0.396 s), B:112_V_R47 (0.389 s), and A:RS_42_vdie (0.365 s), then fills with shorter cases.

## Cases that drive the difference

The slowest observed profile cases are A:RS_07_CHAMF1 (1.006 s), B:113_V_CHAMF1 (0.873 s), A:RS_25_Inc718_D420_GFM_V_DIES_Angle2_18 (0.543 s), A:RS_26_Inc718_D420_GFM_V_DIES_Angle2_19 (0.541 s), and A:RS_28_Inc718_D420_GFM_V_DIES_Angle2_18 (0.537 s). Round-robin can put multiple heavyweight cases on the same worker based on their positions in the fixed list; LPT spreads these first. The gain is minor at two workers, grows to 11% at four, and is larger for eight to sixteen workers because the serial list order is uneven at those modulo strides.

## Recommendation

For a bounded static sharder using this known, stable 98-case manifest, use LPT ordering/assignment instead of ordinal round-robin. Keep case identity and result aggregation independent of execution order, and preserve per-case scratch isolation. If the implementation can use a shared queue of pending cases, a dynamic worker pool can also naturally place available workers on long remaining work; benchmark that against static LPT rather than assuming the simulated figures transfer. Start validation at four workers: this profile predicts a 0.60 s (11%) scheduling-only reduction against RR at four, while avoiding the larger storage and CPU pressure likely at higher concurrency. Eight and above only merit trial if full-suite timing and output equivalence remain intact.

Every makespan here is a modeled schedule over measured single-run durations. Actual parallel times can be slower because copy and compare phases share filesystem/cache/antivirus resources, and case times may vary by cache state. Re-profile repeated runs and report observed full-test wall times before making performance claims.
