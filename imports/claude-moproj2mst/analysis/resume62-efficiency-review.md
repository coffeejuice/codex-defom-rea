# CP62 efficiency review: CP60 through the CP62 safe boundary

The CP61 full launcher took 26.031 seconds; its CTest stage took 12.265 seconds. Reporting the CTest figure as full validation would omit fixture hashing and launcher work. The most consequential current process defect is releasing new source before the tested source archive is frozen: CP61 has a valid completed test run but cannot be named a verified current-source freeze. Complete archive/hash/receipt binding before handing source to the next checkpoint.

Only this report was written. Existing receipts/logs were read; no source/test/build/shared-state edits, executable, GUI or mutable REA operations occurred. The independent validation audit was frozen first at analysis/resume62-validation-audit.md. Tokens, total agent-hours and model/scaling or token savings are unavailable. CP62 had no retained build/full-run receipt at this review boundary, so its accepted duration is not inferred.

## Measured stages

| Checkpoint | Build attempts | Targeted preflight/repair build union, s | Broad build, s | All build interval union, s | Configure steps, s | Accepted/tested launcher, s | CTest, s |
|---|---:|---:|---:|---:|---:|---:|---:|
| 60 | 5 | 61.255 | 28.870 | 90.125 | 53.328 | 25.875 | 12.125 |
| 61 | 3 | 33.506 | 29.176 | 62.682 | 32.140 | 26.031 | 12.265 |
| 62 | None yet | Unavailable | Unavailable | Unavailable | Unavailable | Unavailable | Unavailable |

The build receipts' timestamp intervals do not overlap. The unions measure root build-driver occupancy only, not whole checkpoint or worker wall time. CP60's five stable build receipts are focused-preflight, focused-preflight2, focused-preflight3, text-device-repair and integrated-full. CP61's three stable receipts are focused-preflight, hex-test-repair and integrated-full. All source-stability checks passed in these build attempts. The two compiler failures discussed below are not concurrent-source failures.

Across CP60/61, build occupancy totals 152.807 seconds: configure 85.468, cmake-build 46.424 and wrapper/source/artifact work 20.915. Configuration is material but source/workloads differ; do not attribute its duration to a specific cause or assume it can be safely skipped. Measured build-plus-full-launcher components total 116.000 seconds for CP60 and 88.713 for CP61, 204.713 together. These exclude separately unmeasured focused CTest/review/edits/coordination/archive time and are not total completion wall time. Never double-count nested configure/compiler or CTest/hash stages.

## Full-launcher scope

| Phase | CP60, s | CP61, s |
|---|---:|---:|
| Inventory | 0.891 | 0.922 |
| Complete fixture SHA before | 6.359 | 6.328 |
| CTest | 12.125 | 12.265 |
| Complete fixture SHA after | 6.438 | 6.422 |
| JUnit/reporting | 0.062 | 0.078 |
| Launcher scope total | 25.875 | 26.031 |

CP61's two full fixture snapshots total 12.750 seconds, about 49.0% of launcher time. CTest accounts for about 47.1%. Small wrapper timing not assigned to individual phases explains the remaining fraction after inventory/reporting. This is internal timing.json scope, not a separately measured external process duration. CP61 is 0.156 seconds above CP60 on a suite growing 196 to 198 tests; this unpaired difference is not a performance regression finding.

Bound CP60 run: build/test-runs/1792876747911068400. Bound CP61 run: build/test-runs/1792877344044145300. Both full fixtures-before/after hashes remain equal over 16,728 files, with the exact historical folder-log SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b, 93 SAME / 5 DIFF, six logged-gap passes and one diagnostic-difference test. CP61 reports 191 clean passes, zero failed/skipped/incomplete tests, plus those qualified passes. This is a completed candidate regression run, not 198 independently accepted original behaviors or a complete reconstruction.

## Rework count and cost

- Two CP60 compile failures: resume60-focused-preflight-build.json took 16.704 seconds and focused-preflight2 took 14.326, 31.030 seconds together. Both failed on text-device test comparison types: first an invalid equality comparison, then Testkit's CHECK_EQ diagnostic stream lacking an insertion operator for the compared aggregate. The passing third group took 15.237 seconds. The errors were detected in the focused group before the final full launcher; this is useful detection, not proof their entire attempt time was wasted.
- One CP60 runtime-fixture repair: text-device-repair build took 14.988 seconds after closing the test's own scratch input stream before recursive cleanup. Its focused test/review duration is not retained separately in these receipts.
- One CP61 fixture-expectation repair: hex-test-repair build took 15.397 seconds. No compiler failure occurred in CP61's three selected receipts. Do not invent a separately timed focused test run from that rebuild time.
- Thus four rework build attempts are visible: two failed compiles plus two focused repairs, 61.415 seconds of build-driver occupancy. The repair builds alone total 30.385 seconds. The selected CP60/61 full run receipts both return 0; there is no failed full-launcher repeat in this pair.
- One source-freeze handoff defect remains: CP61 source advanced after the complete run but before archive freeze. Its new integration record explicitly says tested_not_frozen_source_advanced. The stable tested build fingerprint is 60fe2cb33fa7863409e04b811a4cba46e13d647befea3089511a31bee327cca3. Current source and new CP62 tests cannot inherit that result. The completed run still has scientific regression value, so do not call its 26.031 seconds wholly wasted or invent an extra rebuild cost; the actual current-source CP62 gate must be measured when run.

## One concrete CP62 adjustment

Change the source-owner release boundary: after successful full regression, keep every writer beneath source/ paused until root has archived the exact tested source, rehashed the archive contents, matched the build source fingerprint and artifact hashes, and written/verified the freeze receipt. Only then release the next source assignments. Independent reports outside source/ can continue throughout. This is ordering discipline using the existing freeze mechanism, not new source/CMake/tooling work or a broader stop of authorized recovery.

The desired result is one stable selected source, one completed full launcher and one matching immutable freeze, followed immediately by the successor. If source advances first, retain the tested-not-frozen qualification and rebuild/revalidate the actual successor; never package new source as the old tested state. Root's current CP61 status correctly preserves this distinction.

Keep the grouped focused preflight and active-case summaries. A short handoff review should include assertion argument types and file-handle ownership so compile/cleanup assumptions are caught before repeat commands, but it does not replace running those checks. Do not weaken assertions, skip fixture hashes, suppress refusals/differences, reduce the full registered suite, bypass API/architecture guards or relax original-first acceptance to reduce timing.

Measure the next full-run-end to verified-freeze interval, any source-fingerprint drift and retry count. Archive/review overhead and any saved work are unmeasured here. A successful freeze binding is the immediate acceptance improvement; no speed or token saving is claimed until comparable complete-cycle evidence exists.
