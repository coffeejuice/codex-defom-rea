# CP215 efficiency review

## Scope and limits

This review uses retained CP213–CP215 preparation, build, and full-suite receipts plus the CP212 baseline. It does not run builds/tests or interact with the original application. Token telemetry and agent active-effort timings are unavailable. Build receipts are incremental checkpoint builds; they do not support a clean-build comparison. Original-probe preparation records provide chronology and checks, but not a comparable measured preparation interval, so no probe-preparation speed claim is made.

## Recorded build and suite timing

CP214's B98 build completed in 4.555 s total (configure 1.704 s, build 0.156 s; remaining receipt overhead included source fingerprints). The paired 362-test launcher completed in 34.750 s, including 17.359 s CTest and 17.391 s inventory, source/binary binding, fixture hashing, and report work. It passed with 355 clean passes, six passes with logged gaps, one logged difference, zero failures/skips/incomplete outputs.

CP215's two focused build iterations took 8.416 s and 7.047 s total, with build commands at 4.094 s and 2.687 s. The final full candidate build took 33.042 s total (configure 1.687 s, build 28.672 s). Its 362-test launcher took 34.562 s, including 17.203 s CTest and 17.359 s integrity/report work. It had the same pass/gap/difference counts and zero failures, skips, or incomplete outputs. The full-suite launcher was 0.188 s faster than CP214 (about 0.5%), while CTest was 0.156 s faster (about 0.9%). These single runs and differing source/build states do not establish a repeatable speedup.

CP212's prior comparable suite receipt was 361 tests in 34.906 s total, 17.782 s CTest, and 17.124 s overhead. CP214/215 each add one registered case; their CTest intervals are 0.423–0.579 s lower, but these are not controlled repeats. Integrity work remains roughly half the launcher duration and should stay because it binds fixtures, source, and binaries.

## Preparation and rework evidence

CP213 and CP215 both record copying the preserved 27-file B98 tree into a fresh absent `C:\PROBLEM` destination, followed by hash equality and path/link checks. Their records describe the procedure but provide no start/end timing, so savings from reusing a fixture or changing preparation are unmeasured. CP213 records a separate command-channel repair after a failed native cleanup; the focused host regression then passed seven cases. This is real rework, but no elapsed duration is recorded. CP215's fixture receipt explicitly requires immediate prelaunch revalidation; retaining that gate is important and it was not counted as a completed original trial.

## One next improvement

Add compact phase timing to the fresh-fixture preparation receipt: record monotonic elapsed time for existence check/copy, source-vs-clone manifest comparison, and link/path audit, with file count and final hash. Keep the same complete byte and link checks. The paired CP213/CP215 method can then be benchmarked on equivalent fresh copies without weakening evidence; until that comparison exists, do not claim that fixture reuse or an alternate copy method is faster. This is a small measurement improvement and addresses the present inability to assess probe setup cost.

## Evidence

`analysis/resume212-efficiency-review.md`; `analysis/cp214-b98-open-build.json`; `analysis/cp214-b98-open-tests.json` and its `timing.json`; `analysis/cp215-doe-io-focus-build.json`; `analysis/cp215-doe-io-focus2-build.json`; `analysis/cp215-doe-io-build.json`; `analysis/cp215-doe-io-tests.json` and its `timing.json`; `analysis/resume213-fixture-preparation.md`; `analysis/resume215-fixture-preparation.md`; `analysis/resume213-cleanup-fix-review.md`.
