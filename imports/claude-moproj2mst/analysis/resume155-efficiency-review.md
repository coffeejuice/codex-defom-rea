# CP155 safe-boundary efficiency review

CP152â€“154 each completed one recorded build and full launcher. CP155 required a second production-source build to close a callback lifetime defect that passing focused tests had not exercised. The concrete next workflow adjustment is to complete independent review of callback-spanning borrows before broad linking for that class of change. Preserve all existing evidence, tests and freeze checks. This report is the only write; no build, source, GUI or REA operation was run.

## Recorded build and validation time

| Checkpoint | Build invocations | All build envelopes | Accepted full-build command | Full CTest | Full launcher | Recorded build+launcher subtotal |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
|152|1|39.818s|25.110s|14.235s|31.578s|71.396s|
|153|1|36.065s|25.375s|14.781s|32.421s|68.486s|
|154|1|35.735s|25.031s|14.188s|31.063s|66.798s|
|155|2|100.511s|26.656s|14.031s|30.578s|131.089s|

The CP155 envelopes are 60.468s for the first passing build and 40.043s for the repaired accepted build. Their full-build commands are 37.453s and 26.656s. The first focused stage compiled 15 objects/linked seven outputs and its full stage compiled 23/linked 346; the repair focused stage compiled three/linked three, then the full stage linked 332 outputs without compilation. This is substantive repeated work following a required source fix, not a failed compiler or full-suite attempt.

CP152â€“154's full stages compiled no objects and linked 330/331/331 outputs. Their roughly 25s command cost remains despite little focused compilation. Source changes to the shared core require dependent linking; these action counts do not establish that those links can be skipped. Different sources and workload sizes prevent a controlled speedup interpretation of the checkpoint totals.

Command fields exclude subsequent fingerprint/cache work. Legacy step elapsed includes some of that work and is not added again. The full-launcher interval includes inventory, before/after fixture/source/executable bindings, CTest and report generation, excluding its timing-receipt write and process exit. Subtotals above include each recorded build plus its one selected full launcher; no additional failed full-launcher receipt was found for CP152â€“155. They exclude untimed source review, native evidence work and archive/freeze operations. A full checkpoint duration or work-hour cost is therefore unavailable rather than equal to the subtotal.

## Early check and fingerprint costs

| Build | Fingerprints | Static source gate | Early focused command time | Runtime-associated fingerprints |
| --- | ---: | ---: | ---: | ---: |
|152|9.155s /18 calls|0.547s|0.687s /2 tests|3.000s /6 calls|
|153|6.281s /12 calls|0.516s|Not selected|None|
|154|6.251s /12 calls|0.563s|Not selected|None|
|155 first|13.968s /27 calls|0.609s|1.126s /5 tests|7.702s /15 calls|
|155 repair|9.141s /18 calls|0.562s|0.094s /2 tests|3.031s /6 calls|

Early CTest command time alone understates gate cost. Commands plus their associated before/boundary/after snapshots total 3.687s for CP152 and 8.828+3.125=11.953s across the two CP155 builds, before small cache/log overhead. These durations are already included in build envelopes and total fingerprint time. Static source-gate duration also overlaps a post-check fingerprint. No binding boundary is proposed for removal.

CP153/154 did not select the optional early-runtime feature; their changed focused CTests still passed after building, and their mandatory full source/binary/fixture-bound regressions remained. Those separate focused invocations have no selected elapsed receipt here, so their cost is not silently treated as zero. There is no basis to attribute the modest build differences entirely to the optional gate.

Full launcher variation is 1.843s across CP152â€“155; CTest variation is 0.750s. Every final source tree/test run remains bound: CP152â€“154 have 3,541 source files/354 registered tests; CP155 has 3,547/356. Ordinary passes increase 347â†’349, with the same six gap-qualified and one difference-qualified pass, zero final failures/skips/incomplete results, no fixture changes and historical 93 SAME/five DIFF. Public lifecycle and fresh-original qualifications remain.

## Review, freeze and usage limits

CP152 batched two already source-qualified SPRING2 families before one shared build. CP153/154 each integrated one further family while geometry allocator/owner evidence remained guarded. CP155's new editor tail, geometry temporary prefix and binary-token family are larger changes than a single parser family; their elapsed totals are not normalized productivity comparisons.

The CP155 source review identifies a real stale-SC risk: an Extra3D callback can replace the live step/control, while the new tail originally accessed the earlier borrowed SC before checking current identity. The repair moves current-control checks before dereference, retains already completed page/IO effects and adds an actual callback-replacement regression. Both original and repaired focused suites passed, demonstrating why source review supplies distinct evidence. The second build is measured at 40.043s; it is not a proven 40.043s saving from any proposed review schedule because the necessary compile work and review duration would move in time.

CP152's integration records all ten contract paths checked locally before freeze; CP153/154 and CP155 have frozen validation/source archives. Freeze receipts contain identities, artifacts and qualifications but no elapsed field. Do not infer compression/archive duration from file timestamps or allocate an unmeasured review/tool wait to model reasoning. Test receipts explicitly report token measurement unavailable; cached/uncached/generated token counts, worker elapsed/occupancy and work-hour savings are not recorded. No10Ã— or token-efficiency claim follows.

## One actionable adjustment

For changes that carry a raw owner/control reference across a callback-capable effect, wait for the independent lifetime review and its required adversarial owner-replacement fixture before starting the broad full-build phase. Focused compilation may still proceed in parallel with review, but a reported blocker requires source repair, renewed source binding and the affected focused tests before broad linking. Apply this narrowly to such lifetime changes rather than serializing every analysis lane.

This targets the observed CP155 rebuild while retaining source-qualified behavior and all full-build/regression/freeze checks. It adds no new runtime occurrence to the faithful converter and removes no required test. Measure the next similar change's review/build ordering and repair outcome; no guaranteed saved seconds or speedup is asserted in advance. No runner rewrite, hashing cache or worker-count change is bundled into this recommendation.

Evidence: frozen CP152â€“155 build/test/validation and source/archive receipts, referenced Ninja/CTest/timing/full stdout logs; CP152â€“154 root integration reports; CP155 editor source/repair reviews and implementation receipts; and `analysis/efficiency-policy.md` in the parent checkout. Accepted CP155 source SHA-256 is `af5f0b7ceb406844b1b073e143d62694ddc7ecad736582b5f60885ceecb77c38`. Objective completion remains false.
