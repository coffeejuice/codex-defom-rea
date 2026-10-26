# CP143 efficiency review: CP140â€“142

The useful next process change is to bring all affected standalone packet targets into the first grouped preflight when ownership code gains an out-of-line dependency. CP142's repeated include/assertion repairs and late P09 link failure cost more than its final fingerprint pass. Keep every full build, regression and freeze gate. This review changes only this report; no source, runner, build, GUI or mutable REA operation was performed. Token measurements are unavailable.

## Comparable accepted timings

| Checkpoint | Source files | Accepted build envelope | Command total | Eleven fingerprints | Focused command | Full-build command | CTest | Full launcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
|140|3,502|81.772s|75.703s|5.469s|11.469s|63.015s|14.469s|31.782s|
|141|3,507|83.797s|75.687s|7.501s|10.375s|63.812s|13.641s|30.656s|
|142 repair4|3,516|15.855s|7.827s|7.422s|2.265s|4.047s|13.828s|30.938s|

The accepted builds are `resume140-preflight-build.json`, `resume141-build.json`, and `resume142-repair4-build.json`, selected by their frozen validation receipts. Command totals exclude later fingerprint/cache work; legacy step durations overlap some fingerprint timing and are not added again. Accepted full-build actions are 87 compiles/313 links, 88/315, and zero/47 respectively. The small CP142 accepted envelope follows earlier broad failed attempts; it is not a from-change build speedup.

Fingerprints remain eleven complete hash/count calls. The CP140â†’141 increase is 2.032s, about 37.2%, while file count rises five. The receipts do not isolate the cause; file count alone cannot explain or authorize caching away content checks. The disposable CP141 parallel-hash result remains a microbenchmark, not a production improvement.

Accepted full launchers vary by 1.126s, while CTest varies by 0.828s. Their time outside CTest is 17.313/17.015/17.110s. These scopes include complete before/after fixture and execution bindings. Registration grows 335â†’338â†’343, with 328â†’331â†’336 ordinary passes plus six gap-qualified and one difference-qualified pass each. Final failed/skipped/incomplete counts and fixture changes remain zero; historical 93 SAME/five DIFF is unchanged.

## Output from worker lanes

Root integration receipts identify the following accepted source outputs. They do not provide per-worker elapsed time, tokens or an occupancy denominator, so a numerical output-per-20-workers or productivity rate would be invented.

| Checkpoint | Accepted implementing-lane outputs | Count | Focused CTest targets |
| --- | --- | ---: | ---: |
|140|Geometry width-zero rectangle; SPRING2 uppercase profile; editor solver visibility|3|3|
|141|Geometry width-mismatch reader; SPRING2 uppercase EOF; Type3 REGIONS rewrite; editor finite solver6 Fill|4|5|
|142|Editor solver6 Apply; CopyMirror property mutation; public ABI sequence; canonical DOE Item ownership; DOE Output directory prefix|5|8|

Thus twelve bounded source increments were integrated across these three checkpoints. Registered tests grew by nine from CP139's 334 to CP142's 343; that is executable registration, not a case or scientific-acceptance count. Independent reviews, evidence preparation and the disposable fingerprint benchmark are supporting deliverables and are not counted as extra production increments. The receipts establish successful outputs for the named lanes, not the distinct worker count or total contribution of every active worker.

## Repair overhead retained

CP140 has one recorded 81.772s build and one 31.782s full launcher, with no recorded retry.

CP141 has one 83.797s build. Its first full launcher cost 33.609s and failed with eleven sandbox access-denied cases, including owned `C:\PROBLEM` paths. The same source/binaries passed with required filesystem access in 30.656s. Total full-launcher cost was 64.265s; this environmental retry is not a source repair or evidence of a faster test implementation.

CP142 build attempts cost 11.427,20.110,8.954,86.143 and15.855s, totaling 142.489s. The first three stopped in focused preflight: wrong helper include; missing declarations and Testkit String-printing misuse; another missing declaration include. Their combined envelopes were 40.491s. The fourth reached a full command lasting 74.531s before `m2m_p09_keywords` failed to link the new Item snapshot functions. Its 86.143s envelope includes necessary compilation and cannot all be called avoidable waste. The final test/dependency repair build cost 15.855s. Earlier attempts total 126.634s, but their reusable build products account for the short final run. Only one full launcher, 30.938s, is recorded for CP142.

These sums exclude separate focused runtime invocations, review and freeze/archive time. No whole-checkpoint or model-reasoning duration is inferred from them.

## One next process improvement

For each new out-of-line owner/helper dependency, have its release receipt enumerate every `ctest-standalone` and textual packet consumer of the affected emitter/model source. Resolve declarations and source-list additions together, then place all affected packet doubles in the **first** grouped focused build. CP142's actual examples are `tests/p09/test_emit_b.cpp` and `test_keywords.cpp`, both of whose standalone lists now include `src/doe/item_owner.cpp`. The include/assertion preflight must cover the complete released group, including later DOE/CopyMirror test arrivals.

This change moves dependency/link detection earlier; it does not remove any build or test. It targets the observed late P09 failure and the three focused compile failures. No fixed saved duration is claimed: the 74.531s failed full command also built necessary objects, and the earlier focused group would itself cost time. Measure whether the next ownership migration reaches broad build with zero avoidable include/link repairs, while retaining its full suite and exact source/executable/archive binding. Do not bundle a runner rewrite or fingerprint cache into this process change.

Evidence: CP140â€“142 frozen build/test/validation receipts and their referenced timing/log files; `resume140-root-integration.md`, `resume141-root-integration.md`, `resume142-root-integration.md`; current CMake standalone routing and P09 source lists. The original public lifecycle and fresh-original acceptance limits remain unchanged.
