# CP227–229 efficiency review

## Scope and limits

This checkpoint review uses existing full build, full CTest and launcher timing receipts. All builds are incremental and compile different changed-source sets, so durations and edge counts show observed work rather than a controlled build-speed comparison. Token usage and active effort are unavailable; no token-efficiency claim is made.

## Measured results

| Checkpoint | Configure | Full build | Logged Ninja edges | CTest | Full launcher | Registered tests |
|---|---:|---:|---:|---:|---:|---:|
| CP227 | 1.719 s | 115.969 s | 547 | 18.656 s | 37.468 s | 368 |
| CP228 | 1.672 s | 111.485 s | 551 | 17.843 s | 34.984 s | 369 |
| CP229 | 1.797 s | 110.219 s | 547 | 18.531 s | 37.578 s | 369 |

All three full-suite receipts report exit code zero. The suite used `-j 4` in each case. Fixture SHA-256 phases were 13.171 s, 11.766 s, and 13.282 s; source/binary binding phases were 4.313 s, 4.094 s, and 4.500 s. Launcher variation is mainly consistent with measured hashing/binding phase variation, while CTest itself remains between 17.843 and 18.656 s.

Compared with CP224–226, full builds here are 110.219–115.969 s versus 109.157–150.437 s, and logged edges are 547/551/547 versus 696/662/543. The CP226→227 edge count is nearly flat (543→547) and build time rises 6.812 s; CP227→228 adds four edges while build time falls 4.484 s. CP228→229 loses four edges and falls 1.266 s. These mixed outcomes show why changed-source fan-out and edge count alone do not establish a build optimization. No receipt supplies a matched same-edit comparison or per-header Ninja explanation.

The previously recorded CTest resource benchmark—35.95/22.67/35.57 s for 4/8/4 jobs on the same 151-test workload, all passing—remains the only validated test-time optimization evidence. CP227–229 retain four jobs and do not test or extend that result. It concerns suite execution only; it does not establish a build or total recovery speedup. No new test-time optimization is established in these checkpoints.

## One safe next experiment

At the next appropriate candidate phase, measure a matched incremental dependency-fanout experiment: freeze the source snapshot and build tree, record `ninja -d explain` plus compile/link edge counts, then apply one controlled edit to a suspected shared header and measure. Repeat for the compatibility-preserving split candidate in an equivalent checkout with the same compiler, command and cache conditions. Compare duration and scheduled edges, then retain the full suite and all source/binary, fixture-hash, and historical diagnostic gates. Do not attribute CP227–229's duration changes to a particular edit without this control.

## Evidence

CP227: `analysis/cp227-editor-title-full-build.json`, `cp227-editor-title-full-tests.json`, and the referenced `timing.json`.
CP228: `analysis/cp228-loader-unit-full-build.json`, `cp228-loader-unit-full-tests.json`, and the referenced `timing.json`.
CP229: `analysis/cp229-frequency-full-build.json`, `cp229-frequency-full-tests.json`, and the referenced `timing.json`.
Comparison: `analysis/resume226-efficiency-review.md`; CTest parallelism benchmark: `analysis/efficiency-policy.md`.
