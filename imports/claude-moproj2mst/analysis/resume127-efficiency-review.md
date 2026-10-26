# CP127: CP124–126 efficiency review

Public-header invalidation is controlled in this window: model.hpp is byte-identical across CP124–126, and initial grouped builds compile only7–10 objects. Remaining rework is runtime/fixture repair and repeated broad links. The useful next bounded trial is to run existing focused runtime checks after building their targets but before the broad build. Retain the final full build, launcher, integrity manifests and archive.

This review reads existing receipts, logs and frozen archives only. No benchmark, active source/build change, compiler, test or GUI operation ran.

## Final accepted receipts versus total checkpoint activity

| Checkpoint | Configure | Compile preflight | Full-build command | Final build total | CTest | Full launcher | Final build + launcher |
| --- | ---: | ---: | ---: | ---: | ---: | ---: | ---: |
| CP124 |2.219s |3.203s |0.828s |12.441s |14.843s |34.671s |47.112s |
| CP125 |2.969s |5.407s |31.313s |47.123s |16.281s |39.344s |86.467s |
| CP126 |2.156s |2.875s |0.797s |11.673s |13.250s |30.687s |42.360s |

Build/test selections follow each `build/wave3-checkpoint<N>-verified/receipt.json` and its validation pointer: CP124 `resume124-final-*`, CP125 `resume125-three-lanes-*`, CP126 `resume126-fixture-repair-*`, under analysis. CTest is included within launcher, not an additional stage. Build total includes runner activity beyond the listed commands. These different final-source scopes are not paired speed benchmarks.

Final repair rows alone hide substantial work:

| Checkpoint | Retained build receipts | Sum of their build elapsed times | Earlier build work beyond final row |
| --- | ---: | ---: | ---: |
| CP124 |3 |62.526s |50.085s |
| CP125 |1 |47.123s |0s |
| CP126 |4 |120.576s |108.903s |

CP124 receipts are four-lanes37.502s, shared-test-repair12.583s, final12.441s. CP126 receipts are three-lanes38.689s, guard-repair36.711s, final33.503s, fixture-repair11.673s. All these build receipts completed; there is no failed compiler-stage receipt in this window. Do not confuse source/runtime repairs with compile failures.

CP126 also retains a failed full launcher at `build/test-runs/1792923148070235500`:29.734s overall,13.484s CTest, exit8, one failed suite. Its `m2m.mdt.boolean_postload_creation.log` records both cases failing the fixture's `!fs::exists(root)` assertion. The accepted launcher at `1792923362154226700` took30.687s. Adding known build and both launcher receipts gives180.997s of retained CP126 build/launcher work; this excludes diagnosis, edits and unrecorded focused checks. The final42.360s row is therefore not the full checkpoint cost.

## Actions, grouping and header boundaries

| Retained build | C++ compiles | Logged link actions |
| --- | ---: | ---: |
| CP124 four lanes |10 |293 |
| CP124 shared-test repair |1 |1 |
| CP124 final |1 |1 |
| CP125 three lanes |7 |294 |
| CP126 three lanes |7 |294 |
| CP126 guard repair |1 |294 |
| CP126 final |1 |294 |
| CP126 fixture repair |1 |1 |

Target-qualified objects compiled during preflight are not compiled again in the same receipt's full-build phase. Preflight is not causing duplicated object work. Grouping three independent ready lanes still amortizes configure/full acceptance; there is no same-workload comparison supporting a different lane count.

Independently reading each frozen source.zip confirms model.hpp SHA-256 `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6` throughout CP124–126. This differs from CP121's one-time449-compile extraction cost. CP123's actual dependency experiment already showed private-header13 versus public-header451 compile edges; that measured fan-out remains a reason to retain the implemented boundary, not a wall-time-saving claim.

CP126 guard-repair and final each compile only one object but trigger294 links. Their full-build commands take25.422s and23.250s respectively. Together with the initial26.578s full-build phase, these three broad phases cost75.250s; the later fixture repair's no-action full-build step adds0.797s. This locates a concrete stage where rejecting a bad focused candidate earlier could matter. Not all that time is avoidable: one broad full build remains required for the accepted source, and source repair may require fresh focused links too.

## Classifier repair: correctness cost, not performance improvement

The CP126 admin patch ignores only an exact empty active-cogging summary, preserving nonempty refusals, optional fixture skips and failure precedence. Owner receipt `analysis/resume126-empty-refusal-classifier.md` records four focused unittest passes after one fixture-text correction (`1 case skipped`→`2 cases skipped`). It records no elapsed time for those unit runs, so repair wall time is unavailable. No C++ rebuild is inherently required by this Python classification change; the grouped C++ build costs must not be attributed to it.

The accepted CP125 test receipt contains25 total `gap_events` entries; accepted CP126 contains24. The mdt_write row changes2→1 by removing only the empty summary, and remains passed-with-logged-gaps due to the real optional PACK skip. Six gap-bearing suites remain. The classifier owner's predicted21→20 total is not the count in those accepted receipts; the independently recomputed25→24 should be used in closeout. This is a count correction, not evidence that native gaps closed or tests ran faster.

CP126's static audit also caught the OPI receipt's underscore CTest selector; the actual name is dotted. No retained evidence here establishes that root ran a zero-test command. It remains a reason to require matched-test counts and reject zero matches in an early runtime gate.

## Test and integrity scopes

Accepted inventory is312/313/313, clean passes305/306/306. Every accepted run retains six gap-bearing suites and one difference-bearing suite, with zero final failures/skips/incomplete outputs and zero fixture changes. Objective completion remains false.

Launcher fixture before/after SHA phases total13.610/16.515/11.890s. CTest spans13.250–16.281s. These source/workload/host differences do not prove a scheduler regression or improvement, and no host-load benchmark is available. Do not reduce hashes, fixture coverage, source/binary bindings or full test inventory to lower those times.

## One bounded next trial

For one next source-frozen grouped checkpoint, use the existing sequence's focused-target build, then run the **same already required focused CTest group immediately**, validate nonzero expected test count and success, and only then execute the broad full-build command. Keep exact target/test dependency prerequisites, including needed DLL/CLI targets. If a focused case fails, repair/rebuild/rerun that group before spending a broad relink on the rejected snapshot. Move the gate rather than duplicating tests solely for timing.

Record focused build/runtime elapsed and pass/fail, whether a broad build was deferred, its later compile/link count and final complete-launcher time. The measured25.422/23.250s broad-repair phases justify this small trial, but no saving is yet established. The unrelated boolean fixture failure also shows that focused changed tests cannot replace the full suite. Accepted source still receives a normal full build, complete source/binary/fixture-bound launcher and source-stable archive. Preserve original-first/coverage qualifications and candidate safety.

Only this report was written. No timing experiment or mutable project operation occurred. Token usage and token savings were unavailable.
