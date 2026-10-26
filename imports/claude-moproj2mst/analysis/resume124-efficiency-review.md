# CP124: CP120–123 build and validation efficiency

Keep three independent ready lanes grouped, followed by targeted preflight and one full frozen acceptance run. The receipts show no duplicate target-qualified compilation between preflight and full build. Shared-header invalidation explains the large CP121 build; the implemented private boundary now limits private-header exposure to13 compile edges. The avoidable rework in these four checkpoints is CP122's integration defects, not demonstrated excess test coverage or a need to increase lane count.

This review uses retained receipts/logs only. No timing benchmark, compile, test, graph mutation or production edit occurred during active work.

## Consistent final-receipt scopes

The table selects the build and validation referenced by each frozen checkpoint receipt. Build total includes the build runner's work beyond its three listed commands. CTest is nested inside the full launcher; the columns must not be added together as independent stages.

| Checkpoint | Configure | Focused preflight | Full-build command | Final build total | CTest | Full launcher | Final build + launcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| CP120 |2.110s |2.734s |27.579s |38.432s |13.485s |29.782s |68.214s |
| CP121 |2.344s |57.140s |126.735s |193.375s |13.953s |32.219s |225.594s |
| CP122 final repair |2.110s |2.766s |0.797s |11.494s |13.656s |30.907s |42.401s |
| CP123, three lanes |2.156s |3.625s |22.859s |34.596s |13.390s |30.172s |64.768s |

Build sources: `resume120-editor-opi-build.json`, `resume121-feature-anchor-build.json`, `resume122-registry-repair-build.json`, `resume123-three-lanes-build.json`, all under `analysis/`. Corresponding test receipts use the same stems ending `-tests.json`. The selected launcher timing receipts are `build/test-runs/{1792918969543980900,1792919431390907100,1792920838223793400,1792921617442315900}/timing.json` in checkpoint order.

These are different source snapshots and scopes, not paired configuration benchmarks. In particular, CP122's final repair row does not describe its complete integration cost.

## CP122 rework cost

Five retained sequential build receipts sum to77.310s:

| Receipt | Build runner elapsed | Outcome |
| --- | ---: | --- |
| `resume122-four-lanes-build.json` |9.592s |Focused preflight failed |
| `resume122-four-lanes-repaired-build.json` |9.206s |Focused preflight failed |
| `resume122-four-lanes-final-build.json` |35.407s |Completed |
| `resume122-fixture-repair-build.json` |11.611s |Completed repair |
| `resume122-registry-repair-build.json` |11.494s |Final accepted build |

The two failed builds consumed18.798s before the full-build stage. The first exposed a nonexistent `ops/shapeoptim.hpp` include and a call to private `ConversionSession::releaseProjectStorage`; the second exposed an unsupported `qt::String`→filesystem::path conversion. These exact compiler diagnostics are retained in their focused-preflight logs. The later repair receipts add work beyond that initial compile rework; their labels alone do not quantify time spent diagnosing or running earlier failing tests.

Using all five build receipts plus the final accepted launcher gives a measured lower-bound108.217s for CP122 build/validation activity, not42.401s. It still excludes any earlier test-launcher time, diagnosis, editing and unrecorded focused runs. Relative to the final build row,65.816s of retained earlier build work would be hidden by reporting only the last repair.

## Action volume and dependency boundary

Counts below are logged C++ compile/link actions from the selected final builds, not compiler CPU seconds:

| Checkpoint | Preflight compiles / links | Full-build compiles / links | Same target-qualified object compiled in both stages |
| --- | ---: | ---: | ---: |
| CP120 |4 /2 |8 /286 |0 |
| CP121 |156 /4 |293 /295 |0 |
| CP122 final repair |1 /1 |0 /0 |0 |
| CP123 |6 /4 |1 /287 |0 |

CP121's one-time boundary extraction edited model.hpp, so broad recompilation was expected:449 compiles across both stages. CP123's three disjoint lanes required seven compiles. The timings align with action-volume differences, but no paired unchanged-workload run separates source scope, compile cost and machine contention.

CP123's independent safe-boundary experiment (`analysis/resume123-feature-anchor-invalidation-measure.md`, SHA `1dd5c8e8a8611b2c6b2ace3584f04dd0ca96f0bfd7a407170605ac4d919e87e6`) measures actual compiler-recorded dependencies after extraction: private-header touch13 actions, model.hpp touch451, clean baseline0. The private set is the earlier12-edge projection plus the added storage-epoch test; the opaque model-header test remains excluded. That establishes narrower compile fan-out. It does not establish a real elapsed-time saving or eliminate ordinary core/static-library/test relinks.

The correct use of this boundary is to keep future private anchor evolution out of model.hpp unless its public shell/layout actually changes. Do not hide header dependencies or reuse stale objects to manufacture lower action counts. No new boundary refactor is warranted by this review; the measured one is already in place.

## Grouping, preflight and test cost

CP123 integrates three source lanes in one34.596s build and one30.172s launcher with no repair receipt. This is useful evidence that the existing grouping can carry independent work through a single full acceptance cycle. It is not a controlled measurement of grouping three versus one or four lanes. There is no evidence for adding lanes, delaying ready work solely to fill slots, or dropping the preflight/full split.

Preflight did not compile the same object again in the full build. It also stopped both CP122 compiler failures before full linking. Removing it would discard this early failure gate without a demonstrated duplicate-action saving. Keep it as one focused target group matching each source snapshot.

CTest stayed13.390–13.953s across the four accepted runs. Launcher integrity work remains material: fixture before/after SHA phases total11.593/12.672/12.063/11.672s respectively. CP123's launcher exceeds CTest by16.782s, including source/binary bindings, inventory and report work as well as fixture hashing. Those are distinct required evidence stages, not unexplained test slowness. No caching, reduced fixture hashing, skipped tests or weakened binding is justified here.

Accepted inventory grows307→308→310→310. Clean passes are300/301/303/303; every run retains six logged-gap tests and one logged-difference test, with zero final failures, skips or incomplete outputs and zero fixture changes. Objective completion remains false.

## One process adjustment

Require each owner to resolve the exact **new test-facing include, access and path-conversion contract before releasing its source**: verify every include exists in actual target roots; read the declaration/access section for lifecycle methods the test calls; and use the project's supported Qt-string/path conversion rather than an assumed cast. Record this compactly in the existing static release receipt. This directly addresses all three observed CP122 compile defects without a new approval stage, extra full build, altered production semantics or lower coverage.

The18.798s failed-build cost is measured; avoidance by this adjustment is prospective, not guaranteed savings. Root should retain the existing grouped focused compile gate because source checks cannot prove compilation. Continue grouping three truly independent ready lanes, keep the private-header boundary, then run the complete source-stable suite/archive once the grouped candidate passes. Evaluate this one adjustment on the next comparable checkpoints before claiming time benefit.

Only this report was written. No benchmark or active source/build/test mutation occurred. Token usage and token savings are unavailable.
