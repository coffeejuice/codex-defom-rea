# CP212 converter efficiency review

## Scope and evidence limits

Read-only review of the CP207 efficiency assessment, CP209 and CP211 build/test receipts, and original-probe records available at this checkpoint. The recorded artifacts are insufficient for a complete CP207/CP210/CP212 original-probe interval comparison or agent active-effort accounting. Token telemetry is unavailable; no token-speed or model-effort claim is made.

## Measured build and test intervals

CP209's build receipt records configure at 2.453 s (command 1.687 s). Its paired 361-test launcher took 34.906 s end to end: CTest itself was 17.782 s; inventory, before/after fixture hashes, source/binary binding and reporting account for the remainder. Return code was 0.

CP211's first typed-gate build recorded configure 2.55 s, focused preflight 9.38 s, focused CTest 0.80 s, and build 58.17 s. The follow-up fix build recorded 2.45 s, 4.11 s, 0.80 s, and 0.83 s respectively. This is consistent with an incremental rebuild after a first full build, not a comparable pair of clean builds. The CP211 complete 361-test launcher took 33.906 s, including 16.953 s CTest and 16.953 s of inventory, hash/binding and report overhead. The suite returned 0.

The CP209 and CP211 suite launcher totals differ by 1.000 s (about 2.9%); CTest differs by 0.829 s (about 4.7%). These are single runs with different checkpoint/source states, so they do not establish a stable speedup. Integrity bindings and fixture checks are deliberate validation costs and should remain.

## Original-probe chronology and effort

The CP207 report documents two CP206 original probe receipt intervals: 5.278 s and approximately 4.3 s, both incomplete. It recommends stopping repetition of the same four-hook capture and using a single discriminating lower-impact observation only after static review. CP212's release note identifies one original launch and raw receipt directory `analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z`; directory timestamps span approximately 12:41:01 to 12:41:06 UTC. That five-second filesystem envelope is rounded and includes fixture snapshots, and is not an exact process or capture interval. The release record explicitly disallows an automatic second run.

No CP210/CP212 exact process start/end timing receipt, nor per-agent start/finish telemetry, was located in these review inputs. File modification timestamps establish chronology only; they cannot measure active work. CP207's 1 h 46 min implementation-to-retry wall span likewise was already qualified as elapsed time, not labor.

## Avoidable repeat work and next workflow

The efficiency issue with strongest evidence remains CP207's repeated broad original capture: multiple hook runs consumed about 9.6 seconds of measured run-window time yet did not reach the intended CP205 fault path. CP212 should therefore use the already authorized single result for analysis and not repeat the original launch absent a new release. Resolve the capture outcome from the retained bounded receipt, correlate the observed path with static evidence, and proceed on converter-side analysis independently.

For candidate verification, retain the full-suite fixture and execution-binding gates. Where code has not changed, reuse the matching source/build-bound receipt rather than rerunning solely to recreate the same evidence. After a source change, run the focused target group, then one source-stable full suite and freeze before releasing edits. This preserves fidelity and validation while avoiding redundant full-suite passes. Maintain separate timing fields for clean build, incremental build, focused tests, full CTest, integrity overhead, and original capture.

## Sources

`analysis/resume207-efficiency-review.md`; `analysis/cp209-project-resolver-build.json`; `analysis/cp209-project-resolver-tests.json` and its `timing.json`; `analysis/cp211-typed-gate-build.json`; `analysis/cp211-typed-gate-fix-build.json`; `analysis/cp211-typed-gate-fix-tests.json` and its `timing.json`; `analysis/resume212-original-trial-release.md`; `analysis/resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z/`.
