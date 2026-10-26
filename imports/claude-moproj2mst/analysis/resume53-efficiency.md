# Checkpoint 53: shortest defensible full-freeze path

Use one all-source write barrier, then one full build and the complete optimized launcher. The measured waste risk at the checkpoint51/52 boundary is repeated configure work and successful compiler runs invalidated by concurrent source writes. There is no measured basis for a 10x claim. This review proposes scheduling changes only and preserves source binding and all validation.

Read-only review of existing receipts and admin/build_candidate.py; only this report was written. No source, build, test process, GUI, mutable REA or shared-state mutation occurred. Checkpoint52 has focused receipts but no combined-full build/test receipt at this snapshot, so its completed gate time is unavailable. Token usage and total worker/coordination time are unavailable. An initial report write failed on the Windows default encoding; it was rewritten explicitly as UTF-8, with no execution or source impact.

## Measured gates

| Accepted gate | Registered | Stable full-build receipt, s | Configure / compiler step, s | Full launcher, s | CTest, s | Full pre/post fixture SHA, s |
|---|---:|---:|---:|---:|---:|---:|
| Checkpoint50 | 158 | 21.514 | 1.203 / 16.672 | 27.422 | 13.891 | 12.750 |
| Checkpoint51 | 160 | 73.103 | 10.329 / 60.406 | 25.859 | 12.328 | 12.751 |

Sources: respective resume50/51-combined-full-build.json, full-test receipts, bound run timing.json and wave3-checkpoint50/51-integration.json. Use internal launcher scope consistently: external process times were 27.615/26.050 seconds and include different boundaries. Build/compiler and launcher/CTest are nested; do not double-count them.

Checkpoint51's longest measured freeze phase was the full build, particularly its 60.406-second compiler step. The launcher stayed approximately 26 seconds, including all 16,728 fixture files hashed before and after. The slowest CTest was folder sweep at 11.796 seconds, near the 12.328-second CTest critical path. Checkpoint51 retained 153 clean passes, six logged-gap passes, one diagnostic-difference test, no skipped/incomplete/failed tests, unchanged fixtures and the same historical folder log: 93 SAME / 5 DIFF. Faster gating does not close those scientific gaps.

## Source-change concurrency failures

| Receipt set | Focused attempts | Whole-source unstable | Receipt interval union, s | Configure-step total, s |
|---|---:|---:|---:|---:|
| Checkpoint51 focused | 7 | 1 | 123.461 | 72.467 |
| Checkpoint52 focused so far | 5 | 3 | 74.686 | 51.811 |
| Combined | 12 | 4 | 198.147 | 124.278 |

The twelve receipt intervals do not overlap, so the union equals the sum. This measures build-driver occupancy, not total checkpoint wall time or worker time. Every configure/compiler command exited 0. The four failures are whole-source stability failures, not observed compiler errors:

| Unstable focused receipt | Whole receipt, s | Source file counts before / after |
|---|---:|---:|
| resume51-geometry-focused-build.json | 36.012 | 3159 / 3159, differing whole-tree hash |
| resume52-cogging-fault-focused-build.json | 17.548 | 3159 / 3159, differing whole-tree hash |
| resume52-report-path-focused-build.json | 14.109 | 3164 / 3165 |
| resume52-texture-binding-focused-build.json | 14.907 | 3165 / 3168 |

These four attempts total 82.576 seconds. They provide local compiler feedback but cannot serve as accepted source freezes. Fingerprints cover all source/, including other workers' tests; receipts do not identify changed files or prove overlap with the compiled target. Do not call these corrupt binaries or overlook the failed freeze contract because compiler exit was 0. build_candidate.py explicitly returns failure when its pre/post source fingerprint/count differ.

The standalone linkage preflight avoided the checkpoint49 unresolved-symbol failures at checkpoint51, but ran as three driver invocations: p08 emit_a, p09 keywords and p11 xcheck. Their receipt union was 44.070 seconds, including 31.031 seconds configuring and 6.047 compiling. The existing driver supports repeated --target flags, so these targets can share one configure without helper edits. This is an identified opportunity; no grouped-run saving has been measured.

## Recommended shortest path at the safe boundary

1. Root selects the source set and gets every owner to stop writes beneath source/, including new tests. Continue independent analysis outside source/. Keep the barrier through build, full launcher and freeze packaging; queue later source work for the successor. Even unrelated source writes invalidate the full-tree binding.
2. Confirm every new/changed target is registered in the full suite and required focused checks passed. If only an earlier unstable build exists, a stable full build plus that target's execution in the full suite supplies bound validation; an extra standalone rebuild is not necessary merely to obtain another receipt. A genuine unresolved failure still needs repair/testing.
3. With no unresolved shared-linkage concern, run the normal full build directly once. If new out-of-line/shared-storage linkage uncertainty remains, run one grouped preflight for the complete affected standalone/namespace closure first, then the full build under the same barrier. Do not replay three configure cycles just because the earlier review named three targets.
4. After successful stable build, run the complete optimized launcher once. Preserve four CTest jobs, accepted serial-conversion folder pipeline, exact ordered diagnostics, all registered tests, full pre/post hashes, skips/refusals/raw differences and retained logs. Focused checks do not replace the launcher, and source-stability failures stay failures.
5. Bind validation/artifacts to that same source/build hash, release the barrier and start the successor. If source changes during the gate, retain the failed receipt, identify the change and rebuild the selected successor source; do not freeze mixed fingerprints.

For an unresolved Project-storage linkage case, the existing grouped form is:

```powershell
python admin/build_candidate.py --label <new-unique-label> --target m2m_p08_emit_a --target m2m_p09_keywords --target m2m_p11_xcheck
```

The full build remains a separate normal no-target invocation. The grouped command is a risk-triggered preflight, not an always-required extra gate. Include new namespace clones/dependents when the next change reaches them.

Measure the next comparable gate as stable preflight/full-build receipt interval union plus launcher scope, retries and source-stability failures. Keep worker/tool times separate. Checkpoint51's full-build-plus-launcher components total 98.962 seconds; its optional three-part standalone preflight adds 44.070 seconds of driver occupancy. These are descriptive planning baselines, not a guaranteed duration. Retain the scheduling change only if complete accepted-gate cost falls after setup/preflight overhead. Do not promise speedup from the observed 124.278 seconds of configure work alone.
