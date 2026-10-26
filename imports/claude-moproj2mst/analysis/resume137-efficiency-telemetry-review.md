# CP137: first production timing interpretation

CP136 establishes that repeated source fingerprints cost 5.562s, but cannot by themselves deliver the requested 10x end-to-end improvement. Preserve every build, source/count/binary binding, fixture check, regression and freeze gate. This is a read-only review of completed receipts, with one proposed timing experiment; no build script, source, CMake, tests or shared state was changed and no benchmark was run during ongoing recovery. Token measurements are unavailable.

## Comparable receipt scopes

| Accepted checkpoint | Source files | Final grouped envelope | Configure/focused/full step | CTest | Full launcher |
| --- | ---: | ---: | --- | ---: | ---: |
|134|3,475|38.129s|2.156 /0.766 /29.313s|13.640s|30.203s|
|135 final4|3,480|10.569s|1.688 /3.766 /0.609s|13.641s|29.937s|
|136|3,485|41.492s|1.734 /5.109 /30.031s|14.046s|31.313s|

Bindings are respectively `e52d0f8aÃ¢â‚¬Â¦26da911`, `b276306dÃ¢â‚¬Â¦2cfd6c`, and `c000b41bÃ¢â‚¬Â¦886bb3e`, as recorded in each frozen receipt. The CP136 build is `analysis/resume136-preflight-build.json`; despite its filename, it is a grouped focused-then-full build. Legacy step elapsed includes the grouped post-command fingerprint/cache work. The launcher is separately timed from inventory through binding, CTest and report output; it excludes timing-receipt writing/process exit. These are different source snapshots, not a controlled speed comparison.

CP135's short accepted build followed three earlier grouped attempts: 9.706s and 8.122s stopped on focused compile failures, and 37.133s built the prior candidate before focused fixture/assertion repairs. All four envelopes total 65.530s. Root's integration receipt identifies the override-signature error, unsupported `REQUIRE_EQ`, stale geometry assertions and incorrect connector role expectations. The final 0.609s full step had no compile/link actions because prior work had already built the broad set; it is not a comparable from-change build speedup. CP136's completed focused/full group has no recorded build retry.

Actual full-build log classifications are CP134 10 compiles/294 links, CP135-final4 zero/zero, CP136 12 compiles/300 links. CP136's focused group first performed 11 compiles/six links. Necessary shared-core dependent relinks remain a large real cost. Action counts do not prove per-action duration or a safe way to suppress them.

Accepted test registration grows 319Ã¢â€ â€™322Ã¢â€ â€™325, with 312Ã¢â€ â€™315Ã¢â€ â€™318 ordinary passes, six gap-qualified and one difference-qualified pass each. All three final receipts retain zero failed/skipped/incomplete results, unchanged fixtures and historical 93 SAME/five DIFF. These qualifications are unchanged by timing telemetry.

## What the first telemetry actually measures

CP136's 11 completed fingerprint samples are 0.515,0.516,0.500,0.515,0.500,0.516,0.500,0.516,0.484,0.500,0.500s, summing 5.562s. Each still hashes and counts all 3,485 files in one traversal; the source archive contains 26,636,225 uncompressed bytes. Do not infer a separate counting pass or remove a boundary because counts matched.

The command timings are 1.219s configure, 4.609s focused and 29.516s full, totaling 35.344s. The 41.492s envelope minus that total and 5.562s fingerprint total leaves 0.586s for remaining wrapper/artifact/cache/report overhead. This subtraction treats command/fingerprint timings as disjoint; it must not also add legacy step durations, whose post-command samples overlap the fingerprint total. Command timing covers the log-open/runner-completion boundary before later source/cache measurement. No individual native build action timing is available.

CP136's 31.313s full launcher consists of 14.046s CTest, 11.984s fixture hashes, 4.188s source/binary execution bindings, 0.969s inventory and 0.110s parsing/report work, plus rounding/unattributed overhead. That leaves 17.267s outside CTest. The before/after fixture and execution-binding costs remain required; unchanged content is not permission to replace their SHA checks with timestamps.

For scale, the serial build-envelope plus launcher sum is 72.805s. It excludes earlier focused runtime calls, environment setup outside the grouped timer, review and archive work, so it is not whole-checkpoint elapsed time. A 10x target for just this measured sum would be 7.2805s. Perfectly eliminating source-fingerprint time, which is not proposed, could improve the build envelope by at most 1.155x and this two-stage sum by at most 1.083x. Even the fixture hashing stage alone exceeds 7.2805s. No single measured hashing shortcut establishes 10x full-cycle recovery; it would require large independently demonstrated improvements across build/link and validation stages while retaining their work.

## One bounded next timing change to evaluate

At a safe frozen boundary, prototype a bounded parallel content-hash implementation on a disposable frozen source copy. Retain the exact existing sorted inventory, file-admission rule, path/NUL/per-file-SHA reduction order and count; parallelize only independent file-content digests, using at most four workers. Every original snapshot occurrence still runs and rereads every file. No mtime/content cache, reused prior count or skipped fingerprint is admitted. This targets the measured 5.562s cost without changing the scientific acceptance contract.

Do not modify the active build script yet. First compare the proposed function against the current function on identical frozen bytes with interleaved runs and exact hash/count parity; record its own overhead and reject a slowdown. The modest 26.6MB spread across 3,485 files could make task scheduling slower, so benefit is unproven. Exercise addition/deletion/content drift and provider failure behavior before adopting it. Preserve existing drift retries and compile/freeze gates, then use the existing telemetry for a real source-bound checkpoint only after review. This experiment supplies a measured decision, not a promised speedup. Broad library/dependency restructuring and concurrent fixture hashing are separate unmeasured changes and are not bundled into this recommendation.

Evidence: `analysis/resume134-final-build.json`, `resume135-final-build.json` and its `final2`, `final3`, `final4` successors, `resume136-preflight-build.json`; the final tests/timing paths referenced by `wave3-checkpoint134/135/136-validation.json`; retained build logs; and `resume135-root-integration.md`. The earlier CP136 telemetry report wording has been narrowed without editing the script; its final report SHA-256 is `3c1d23075d7f8df9babcce2ed8b2741688a055724126efe6b1a53614857ccf10`.
