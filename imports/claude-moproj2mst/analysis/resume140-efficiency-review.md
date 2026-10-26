# CP140 efficiency boundary: CP137–139

Broad compile/link work and repair iterations dominate the variable cost. Fingerprint cost and full-launcher time are stable. The next worthwhile experiment is an isolated dependency-invalidation comparison, not removing validation or adding another acceptance shortcut. Only this report was written; no source, build script, build, benchmark, GUI or shared state was changed. Token usage remains unavailable.

## Accepted receipt timings

| Checkpoint | Source files | Grouped envelope | Command total | Fingerprints | Full-stage command | CTest | Full launcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
|137 final3|3,489|10.626s|4.531s|5.563s|0.125s|13.953s|31.000s|
|138|3,496|78.888s|72.687s|5.625s|61.391s|13.890s|30.891s|
|139 final2|3,501|9.674s|3.656s|5.484s|0.125s|13.797s|31.218s|

Sources are the final builds selected by `wave3-checkpoint137/138/139-validation.json`: `resume137-final3-build.json`, `resume138-preflight-build.json`, and `resume139-final2-build.json`. Launcher timings come from the final tests' referenced `timing.json`, not CTest alone. The grouped command field ends before its post-command binding work; fingerprint timing overlaps legacy step elapsed, so those step totals are not added again. Envelope minus command/fingerprint totals is 0.532/0.576/0.534s respectively.

All three accepted builds have eleven completed full hash/count calls, one configure, no internal reconfigure/drift retry, stable source/cache receipts and unchanged cache identity. Configure commands take 1.203/1.218/1.250s. The fingerprint range is only 0.141s across the three runs. Hash and count are already collected together; a matched file count is not a replacement for content binding.

Accepted full launchers vary by 0.327s. Their cost outside CTest is 17.047/17.001/17.421s; paired fixture SHA checks cost 11.797/11.687/12.063s and paired execution bindings cost 4.188/4.188/4.265s. No production timing change was applied to those operations. Registration grows 328→331→334, with 321→324→327 ordinary passes, six gap-qualified and one difference-qualified pass each; no final failures, skips, incomplete results or fixture changes. Historical comparison remains 93 SAME/five DIFF.

## Repairs and build actions

CP137's first grouped preflight failed after 9.712s on a missing viewer-test declaration include. Its next 40.472s build completed, but focused runtime found a stale mode-0 geometry assertion. The final 10.626s build repaired that test. All three build envelopes sum to 60.810s. The final full command's 0.125s reflects already-built broad targets, not a from-change build optimization.

CP138 has one recorded 78.888s grouped build and one full launcher, with no repair build. Its focused stage compiled 39 objects and linked five outputs; its full stage compiled another 84 objects and linked 308 outputs. The two build commands together took 71.469s. CP137's earlier broad completed build had 14 full-stage compiles/301 links and a 29.906s full command. These are different source snapshots, so the 31.485s command difference cannot be assigned to a particular header or machine contention without an isolated comparison.

CP139's first 44.796s grouped build completed with 13 focused compiles/seven links and 17 full compiles/308 links. Focused runtime then exposed the newly admitted SPRING2 case still present in the refusal table; independent CRT review also added a missing lifetime sequence test. The 9.674s final build compiled two tests/linked two outputs, with no full-stage compile/link. Total recorded build envelopes are 54.470s. The runtime repair and added coverage are retained; the earlier broad build was necessary work, not entirely avoidable overhead. Each checkpoint has one recorded final full launcher; no failed full launcher was found in the selected CP137–139 full stdout receipts.

## One next experiment

At a safe freeze, use a disposable copy of the frozen Ninja dependency graph to measure action invalidation separately for `include/m2m/engine/simcontrol300.hpp` and `simcontrol300_owner.hpp`, without compiling or touching the active candidate. CP137→138 archive comparison shows both changed, along with two private DOE/error-map headers. The 123 compiled objects across CP138 focused/full stages make shared-header fanout a larger measured investigation target than the roughly 5.6s fingerprint total, but do not yet prove which header caused which action.

Retain raw dry-run action sets and classify compile/link consumers. Only if that isolates avoidable fanout should a separate source-qualified private boundary be designed and tested for unchanged layout, symbols, macro/textual packet builds and behavior. Do not raise build parallelism, skip dependent relinks or change CMake admission based on the current numbers. This is one bounded measurement, not authorization for a refactor. The earlier parallel-content-hash proposal remains unbenchmarked and is not added as a second experiment here.

For scale, final build-plus-launcher sums are 41.626/109.779/40.892s. Even zero-cost fingerprints would bound improvement of these sums to 1.154×/1.054×/1.155×; this is a mathematical upper bound, not a proposed removal or observed speedup. No 10× end-to-end claim follows. Keep exact content/count checks, every full build/regression, source/binary/fixture bindings and archive read-back gates throughout any experiment.
