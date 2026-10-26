# Checkpoint 50: efficiency review of checkpoints 47–49

The largest measured integration cost was checkpoint49's linkage rework, not the regression suite. Keep the accepted full-hash/folder validation and move the complete standalone-linkage dependency check ahead of the next shared-storage integration. Twenty assigned workers produced useful source/helper work and evidence, but no session-wide worker-time or token measurement supports a scaling-speedup claim.

This bounded review read existing receipts and a live agent-status snapshot only. It changed no source, build, fixture, GUI or mutable REA state. Token usage is unavailable. The parent efficiency policy's single-chat/retired-worker language predates the converter subtree's explicit 20-worker implementation contract; that newer scoped contract governs staffing here. The shared measurement rules still apply: bounded review, interval unions, one proposed improvement and unchanged validation gates.

## Measured gates

Times below use each run's `timing.json` launcher scope, so the externally timed process values in checkpoint48/49 integration summaries are not mixed with internal measurements.

| Checkpoint | Registered tests | Successful build receipt, s | Launcher, s | CTest, s | Two complete SHA snapshots, s | Folder test, s |
|---|---:|---:|---:|---:|---:|---:|
| 47 | 150 | 27.434 | 26.672 | 12.657 | 13.171 | 12.425 |
| 48 | 151 | 27.353 | 26.218 | 12.625 | 12.813 | 11.820 |
| 49 | 154 | 16.932 | 28.422 | 13.484 | 14.110 | 12.660 |

Sources: `resume47-combined-full-build.json`, `resume48-combined-full-build.json`, `resume49-combined-full3-build.json`; corresponding full-test JSON files and their bound `build/test-runs/.../timing.json` records; `wave3-checkpoint47/48/49-integration.json` and validation receipts. The successful build receipts include configure, build, source hashing and artifact work; their nested steps must not be added again. CTest is nested in the launcher and must not be added to it.

The two fixture SHA passes consume roughly half the launcher (49.4%, 48.9%, 49.6%). They hash all 16,728 fixture files before and after and remain required. The slowest individual CTest is the serial-conversion folder sweep, close to the CTest critical path. These are descriptive single-run values on growing suites, not a controlled regression or speedup experiment. The three-run launcher range is 26.218–28.422 seconds.

Checkpoint49 had two failed broad build attempts before the successful third attempt:

| Attempt | Configure, s | Build step, s | Whole receipt, s | Result |
|---|---:|---:|---:|---|
| combined-full | 12.031 | 61.969 | 76.814 | Standalone packets lacked new out-of-line Project constructor/destructor linkage |
| combined-full2 | 10.438 | 2.359 | 15.601 | p11 xcheck lacked renamed-namespace Project constructor/destructor linkage |
| combined-full3 | 10.406 | 3.688 | 16.932 | Passed |

The receipt intervals do not overlap: their interval union is 109.347 seconds, including 92.415 seconds in the two failed attempts. First-start to last-finish spans 230.138 seconds, including diagnosis/edits/gaps that these build receipts do not attribute. Do not label that unattributed interval model reasoning or tool time. Configure steps total 32.875 seconds across the attempts. This build/rework gate is the largest directly measured gate in the reviewed window. The logs identify `m2m_p08_emit_a`'s `m2m::Project` symbols and `m2m_p11_xcheck`'s `m2m_xc::Project` symbols, rather than a runtime-test failure.

## Useful integrated work versus analysis-only work

Counting unit: a distinct item explicitly named in an accepted checkpoint's integration/scope receipt, not files, reports, CTest registrations, native functions or complete public features. Checkpoint47's efficiency receipt lists three accepted changes; checkpoint48 and checkpoint49 scope receipts each enumerate six. This yields **15 integrated items**:

| Checkpoint | Domain/runtime or isolated helper increments | Harness/regression increments | Integration repair | Total |
|---|---|---|---|---:|
| 47 | Pattern-brush helper; alpha-consumed editor dispatch | Minimal serial-conversion folder pipeline | — | 3 |
| 48 | Boolean queue helper; DOE typed preference projection; editor constant brushes; internal style24 image helper | Save writer-failure ordering coverage; connector-edge coverage | — | 6 |
| 49 | Distortion contact pairs; Mo slot/session ownership capsule; DOE TaskInfo font projection; isolated empty-gradient helper | Connector-dimension regression | Standalone/namespace linkage repair | 6 |

Totals are **10 runtime/helper items, 4 harness/regression items and 1 linkage repair**. Several are isolated helpers or guarded intermediate architecture, not public feature completion. The standalone 11/11 worker-fault suite and one-case parent-failure test are two additional useful validation deliverables outside this 15-item integration-scope count; they should not be quietly recounted as converter features.

At least **8 distinct evidence-only dependency lanes** in this same window still ended without an accepted implementation of their specific missing behavior by checkpoint49: DOE QThread/TLS runtime qualification (`resume47-doe-qthread-capture-contract.md`), populated geometry-mesh fixture qualification (`resume47-geometry-mesh-fixtures.md`), original locale/default encoding (`resume47-original-locale-static.md`), multi-reheat array behavior (`resume47-cogging-reheat-boundary.md`), active DOE variable/output-body mapping (`resume48-b121-variable-output-map.md`), fresh original case005 feasibility (`resume48-case005-capture-script-plan.md` / `resume49-case005-feasibility.md`), timer delivery schedule (`resume49-dialog-timer-schedule.md`) and graph manager/scene final ownership (`resume49-graph-ownership.md`). These findings narrow real dependencies; they are not accepted source increments or evidence that the full application gap is closed.

There are 76 Markdown artifacts with resume47–49 prefixes (19/42/15), but this is a document count, not 76 independent tasks or 76 analysis-only outcomes. Some support integrated items, several revisit the same dependency, and others report experiments. The retained state lacks a complete historical per-assignment ledger with integration timestamps, worker elapsed intervals and accepted work-unit IDs, so a complete integrated-versus-analysis-only assignment denominator cannot be reconstructed honestly. The 15 and at-least-8 counts above are explicit, auditable categories, not an invented worker conversion rate.

Scientific acceptance did not improve in these three checkpoints' historical folder metric: all retained the exact same folder-log SHA-256, 93 SAME / 5 DIFF, six logged-gap passes and one diagnostic-difference test. Fixtures were unchanged. Test inventory increased 150 to 154, but that increase alone is not evidence of new original parity. Fresh original/runtime qualification remains a separate blocker and has no comparable measured gate duration in these receipts.

## Twenty-worker orchestration

The takeover state records a target of 20 workers, 21 host slots including root, exclusive write sets and root-only shared interfaces/builds/integration/captures. Its initial staffing record is 16 Luna Low and four Sol High; no host/model change or worker-speed benefit is inferred from that record. A live snapshot during this review showed 20 worker identities: 11 running and nine completed, plus the running root. That is an instant queue observation, not an occupancy average or proof of wasted time; root was also processing source/freezes and successors.

Continue independent evidence work during serialized gates, and reassign a finished worker to the next dependency-ready owned file. Do not create additional speculative reports merely to keep twenty slots busy, revive archived workers, or run competing builds/REA mutations. Measure accepted increments and review/rework, not visible occupancy. No total agent-hours, model-comparison timing, cached/uncached/generated token counts or coordination-cost measurement is available, so none is estimated.

## One concrete process improvement

For the next change that moves shared Project/storage lifecycle methods out of line, have root identify and build the **complete directly affected standalone target closure, including renamed-namespace clones**, before starting the broad integration build. The checkpoint49 p08-only follow-up did not cover p11's namespace clone, causing the second broad failure. Keep this check narrow and triggered by shared-linkage changes; do not add it routinely to unrelated edits.

Measure the next comparable attempt as preflight + broad-build receipt interval union + retry count, and compare with checkpoint49's 109.347-second build union/two retries. The preflight has a cost and no saving is claimed yet. Retain the required successful full build, all registered regressions, exact ordered folder diagnostics, all fixture hashes, raw differences, source freeze, API/architecture checks and original-first acceptance boundary. Keep the current serial-conversion pipeline and four-job launcher configuration; this review authorizes no hash caching, process sharding or coverage reduction.
