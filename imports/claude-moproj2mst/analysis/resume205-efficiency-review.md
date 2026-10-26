# CP205 converter efficiency review

## Measured work

CP205 produced a useful original-app crash-stack capture after one failed guarded launch. The two launch directories timestamp their starts at 08:56:25.199Z and 08:58:00.325Z, a 95.126-second interval. The first child stopped before attach/resume due to a case-sensitive executable-path comparison against a helper-canonicalized lowercase argv[0]. It was safely terminated while suspended; integrity and absence checks passed. One identity mismatch was preventable by normalizing the executable path before the first run. The review/revision then enabled the second run, which reached handler-ready and resumed at 08:58:01.310Z, observed the AV at 08:58:01.858Z, and captured natural exit/cleanup by 08:58:05.579Z. Thus the active process observation from resume through completed natural-exit window was 4.269 seconds; this excludes preparation and independent review time.

The separate owned Python AV mechanism preflight began at 08:51:11.981Z and its stage records end at 08:51:12.870Z, about 0.889 seconds of recorded stages. It ended INCOMPLETE because Python exited with code 1 rather than the required raw access-violation status, despite useful callback and cleanup evidence. The overall first-to-second original launch interval is measured, but it is not the amount of human editing/review time: timestamps include an unknown mix of work and idle time. No build or converter test-suite timing was collected in this checkpoint, and the available project-root recovery plan is not present in this imported checkout.

## Parallelism and validation cost

The CP205 records show several independent static/review work products around the probe: AV callback semantics, source adjudication, release review, crash-chain/static impact analysis, and reference-case correlation. Their file modification timestamps span roughly 08:51–09:04 UTC, but those timestamps do not identify agent start/end times or establish overlap. No defensible worker-parallelism count or serial counterfactual can be calculated from the retained reports. They do show validation overhead: the initial release review, first attempt and its failure analysis, corrected runner and second release review, then second attempt and capture qualification. This was necessary validation for a high-risk original process capture, and no data supports removing a gate.

Token counts are unavailable in these artifacts. No token-savings estimate is supportable. The earlier CP203 checkpoint likewise advised source/mock contract checks ahead of owned-process runs; CP205's concrete mismatch validates that recommendation.

## Next efficiency adjustment

Add a bounded, side-effect-free preflight contract check that constructs the expected helper argv and compares it using the exact same path/case normalization as the runtime identity predicate. Include both helper-canonicalized lowercase and normal Windows path spellings, plus exact comparison of the four project arguments. Run this check before requesting release review for any future owned launch. Keep all process identity predicates, independent review, fixture hashes, raw receipts, natural-exit window, absence checks, and cleanup requirements unchanged. This directly targets the single observed preventable first-run failure while preserving capture fidelity.

Do not interpret the 95-second inter-launch interval as measured savings or active work. Reassess after the next checkpoint with explicit per-stage wall-clock receipts and worker start/end timestamps; continue to report token counts as unavailable unless tooling supplies them.

## Evidence

`analysis/resume203-efficiency-review.md`; `analysis/resume205-exception-handler-av-child-preflight.md`; `analysis/resume205-original-probe-release-review.md`; `analysis/resume205-original-crash-stack-2026-10-26T08-56-25-199Z/summary.json`; `analysis/resume205-original-probe-release-review-2.md`; `analysis/resume205-original-crash-stack-2026-10-26T08-58-00-325Z/summary.json`; `analysis/resume205-original-crash-capture-qualification.md`; `analysis/resume205-av-child-adjudication.md`; `analysis/resume205-python-av-semantics.md`; `analysis/resume205-static-crash-chain.md`; `analysis/resume205-cpp-crash-impact.md`; and `analysis/resume205-reference-case-correlation.md`.
