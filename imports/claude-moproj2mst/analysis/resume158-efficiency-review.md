# CP158 measured efficiency boundary: CP156â€“158

CP156 had one compile retry; CP157 and CP158 each completed one recorded build and full launcher. CP158's semantic cursor repair was incorporated before building. The practical next adjustment is a direct declaring-header check for newly introduced helper calls before source release, addressing CP156's actual missing declaration. All compile, runtime, binding, evidence and freeze requirements remain. Only this report was written; no source, build, GUI or mutable REA operation was performed.

## Frozen phase measurements

| Checkpoint | Accepted build envelope | Configure/focused/full commands | All fingerprints | Full CTest | Full launcher |
| --- | ---: | --- | ---: | ---: | ---: |
|156 repair|82.024s|1.235 /1.922 /67.062s|10.529s /21 calls|14.031s|30.547s|
|157|43.024s|1.234 /3.531 /25.563s|10.813s /21 calls|14.172s|31.015s|
|158|45.410s|1.328 /2.906 /27.079s|12.625s /24 calls|14.688s|32.453s|

These are the builds/tests selected by the immutable `wave3-checkpoint156/157/158-validation.json` receipts. Command fields end before post-command source/cache work; fingerprint timings overlap legacy step elapsed. Total command time is 70.845/31.516/32.078s; envelope remainder after command plus fingerprints is 0.650/0.695/0.707s. Do not add overlapping fields twice.

CP156's accepted full stage compiled 93 objects and linked 333 outputs after its focused stage compiled one/linked two. CP157's focused/full stages compiled seven/zero objects and linked five/349 outputs. CP158's stages compiled four/zero and linked five/332. CP156 therefore contains substantially more actual recompilation; its larger duration cannot be assigned to review overhead, machine contention or a particular header without a matched invalidation experiment. CP157/158's roughly26â€“27s full command still performs broad dependent linking, not a no-op.

Full-launcher time outside CTest is 16.516/16.843/17.765s. Paired fixture hashes cost 11.485/11.734/12.594s; source/executable bindings cost 3.875/3.906/3.968s. Full launcher variation is 1.906s, with CTest variation 0.657s. Different source snapshots and action sets do not establish a speedup.

## Opt-in gate and rework

Early exact-test commands cost 0.626/1.188/0.765s for three/three/four tests. Their associated complete source snapshots cost 4.467/4.577/6.063s. Directly attributed runtime-gate work is therefore 5.093/5.765/6.828s before small cache/log overhead. These samples are already inside the build envelopes and fingerprint totals. Static source checks took 0.563/0.578/0.578s, including their own post-check fingerprint. All accepted early runtime tests passed; no defect-catching speed benefit is measured for this interval.

| Checkpoint | Recorded builds | All build envelopes | Build + selected full launcher |
| --- | ---: | ---: | ---: |
|156|2|97.503s|128.050s|
|157|1|43.024s|74.039s|
|158|1|45.410s|77.863s|

CP156's first focused compile failed after a 15.479s build envelope. The failing phase took 10.093s and reported `def::model::dynary_copy` undeclared in the new StepSize test; root added the required header. The successful second build reused earlier products and still performed the broad 93-object compilation. The entire 15.479s is measured failed-attempt time, not a guaranteed recoverable saving because some concurrent compilation was useful.

CP156 also needed an unchanged graph-evidence report moved from parent analysis into the converter subtree before continuation passed. Its duration is unavailable. CP158's independent review found that a whole-file q/? whitelist did not represent the intended unread-marker class; the source and tests were corrected before the sole recorded build. That is a concrete review result, not proof of a prevented runtime failure or saved build duration.

For comparison, the preceding CP155 review records two source builds totaling 100.511s and a 30.578s launcher after the stale-SC repair. The current three-checkpoint sums are 185.937s builds plus 94.015s launchers, or 279.952s of selected recorded work. These exclude review, separate evidence/tool activity and untimed archive/freeze stages. They are not complete checkpoint wall time, worker-hours or a normalized throughput comparison.

## Accepted output and measurement limits

Root receipts bind CP156's owned StepSize table lifecycle and batched positive-decimal/BOM parser increments; CP157's candidate-only unpublished B8 prefix and negative wrapping parser; CP158's private cached Stopping Fill and corrected q/? cursor class. These are seven bounded increments, with their individual public/native qualifications retained. Evidence preparation and independent reviews are supporting outputs, not additional production features or distinct-worker counts.

Source files/tests grow 3,548/357 to 3,549/358. Final ordinary passes are 350/350/351 plus six gap-qualified and one difference-qualified pass each; zero failures/skips/incomplete results, unchanged fixtures and historical 93 SAME/five DIFF remain. No original raw-byte acceptance or objective completion follows from these passes.

Freeze receipts contain source/archive/artifact identity but no elapsed timing. No freeze duration is inferred from file timestamps. Test receipts explicitly report token measurement unavailable; no cached/uncached/generated token counters, worker occupancy, review elapsed or work-hour totals are available. No10Ã— or token-saving claim is supported.

## One practical adjustment

Before releasing a new helper call in a test or implementation, identify its actual declaring header and confirm that header is explicitly included or deliberately provided by the target's documented interface. Apply this narrowly to newly introduced calls rather than expanding a repository-wide audit. CP156's `dynary_copy` declaration was absent despite resolvable include paths; the lexical binding checker does not perform C++ symbol lookup, so a passing path scan cannot establish declaration visibility.

Keep the grouped focused compiler as the authoritative check, followed by all selected runtime tests, complete build, source/executable/fixture-bound regression and archive rehash. Record whether the next similar release avoids a missing-declaration retry; no fixed saved-seconds estimate is claimed. Retain the CP155 recommendation to finish independent callback-lifetime review before broad linking for that risk class, but do not bundle another runner rewrite or hash-cache experiment into this adjustment.

Evidence: immutable CP156â€“158 build/test/validation/freeze receipts and referenced Ninja, early CTest, timing and full stdout logs; CP156/157 root integration; CP158 frozen contracts and source/repaired-cursor reviews; `analysis/resume155-efficiency-review.md`. Accepted CP158 source SHA-256: `fc44dbf3c5677ec2743505ea4f1a3e776dee3989cae82f8db56181f109bb025e`.
