# CP71 build and test timing comparison: CP66–CP70

## Recorded configure/build work

| Checkpoint / selected receipt sequence | Configure seconds | Build seconds | Recorded build elapsed | Notes |
|---|---:|---:|---:|---|
| CP66 focused (3 targets) + full | 21.750 | 27.234 | 54.699 | Both successful, same source fingerprint |
| CP67 focused (4 targets) + full | 21.953 | 27.516 | 55.373 | Both successful, same source fingerprint |
| CP68 focused (5 targets) + full | 21.905 | 78.157 | 106.015 | Both successful, same fingerprint; 89 C++ object compiles in full build |
| CP69 final focused (6 targets) + full | 2.516 | 32.015 | 39.129 | Successful pair shares a fingerprint; an earlier focused attempt failed on another hash |
| CP70 grouped attempt 1 | 1.906 | 0.719 | 6.385 | Failed focused preflight, exit 1 |
| CP70 grouped attempt 2 | 1.875 | 31.031 | 38.335 | Successful grouped build on hash `c8124869…`; no corresponding full test receipt |
| CP70 grouped attempt 3 | 1.875 | 3.812 | 11.118 | Successful one-target grouped build on hash `b35d9d95…`; the full-build step was incremental (0.750 s) |

CP66–68 used separate configure invocations for focused and full builds. Their configure steps took about 10.9 s each. CP69–70 configure times were much lower—roughly 1.25–1.9 s per invocation for both split and grouped modes. That change affects both workflows and is not attributable to configure reuse from these receipts alone.

The closest available split-versus-grouped comparison is CP69’s final successful focused/full pair (39.129 s total; 2.516 s configure) versus CP70 grouped attempt 2 (38.335 s total; 1.875 s configure). The raw difference is **0.794 s total** and **0.641 s configure**, but it is not a controlled measurement: CP69 used six targets and fingerprint `0bbbc04f…`, CP70 used five targets and fingerprint `c8124869…`, and build-cache state differed. CP70 attempt 3 is also not a fair comparison: it used one target, a later fingerprint, and an already-populated build directory. Therefore the receipts establish that grouped reuse works in the successful path and avoids a second configure within each grouped invocation, but they do not establish a causal end-to-end time saving. The observed timing does not support using the earlier ~10.9-second configure estimate for CP69–70 runs.

CP70 had a first grouped attempt that failed its focused preflight and then two successful grouped receipts across different source fingerprints. The frozen CP70 candidate and full tests bind to attempt 3’s fingerprint `b35d9d9561f437def99edb3f24c022c8cbb369d32537ac1d1b0de45ff36487a6` (3,295 files). Its final preflight and full-build receipt share that hash; all commands in that receipt passed and the tested source/binary binding remained stable.

## Full launcher timing and coverage

| Checkpoint | Registered tests | Full launcher | CTest | Fixture SHA before/after | Source/binary binding before/after | Exit/status |
|---|---:|---:|---:|---:|---:|---|
| CP66 | 212 | 29.781 s | 12.203 s | 13.015 s | 3.516 s | exit 0, recorded gaps/difference |
| CP67 | 212 | 30.187 s | 12.641 s | 12.906 s | 3.547 s | exit 0, recorded gaps/difference |
| CP68 | 214 | 32.078 s | 14.110 s | 13.282 s | 3.608 s | exit 0, recorded gaps/difference |
| CP69 | 214 | 29.500 s | 12.172 s | 12.688 s | 3.562 s | exit 0, recorded gaps/difference |
| CP70 | 216 | 29.594 s | 12.250 s | 12.719 s | 3.562 s | exit 0, recorded gaps/difference |

Across these receipts there were zero CTest skips/failures and no fixture changes. CP70 had 209 plain passes, 6 passed with logged gaps, and 1 passed with logged differences; its source fingerprint and all 213 captured binary hashes matched across the test interval. The extra fixture and binding work remains included in all full-launcher times.

## Ten-times target

The recorded evidence does not achieve a 10× test-launcher reduction. CP70’s 29.594 s is about 0.85× the historical 25.250 s best complete launcher recorded in the accepted test-speed freeze (that is, slower, not ten times faster). A 10× reduction from that historical reference would require about 2.525 s while keeping the same full test inventory, fixture SHA coverage, and source/binary bindings; CP70’s fixture hashing alone took 12.719 s. No 10× claim is supported for grouped builds either: the only close split/grouped receipt comparison is non-controlled and differs by less than one second total.

## Next measurement

To quantify configure reuse, run at least three paired trials on the same frozen source fingerprint, with the same grouped target list, clean separate build directories, CMake/Ninja settings, parallelism, test inventory, fixtures, and binding captures. Alternate order within pairs. Measure the split path (configure + grouped preflight, configure + full build) against the grouped path (configure + grouped preflight + full build). Compare paired configure, build, and full-launcher medians only after source fingerprints, final artifact manifests, CTest outcomes, and full fixture/source/binary hashes match. Current CP69–70 receipts are evidence of functional integration, not this causal speed comparison. Token usage is unavailable in the receipts.

Evidence: `analysis/resume66-focused-preflight-build.json`, `analysis/resume66-integrated-full-build.json`, `analysis/resume66-integrated-full-tests.json`; corresponding CP67 and CP68 receipts; CP69 `resume69-focused-preflight-build.json`, `resume69-focused-preflight2-build.json`, `resume69-integrated-full-build.json`, `resume69-integrated-full-tests.json`; CP70 `resume70-integrated-grouped-build.json`, `resume70-integrated-grouped2-build.json`, `resume70-integrated-grouped3-build.json`, `resume70-integrated-grouped3-tests.json`, and `build/wave3-checkpoint70-verified/receipt.json`; historical accepted timing in `analysis/resume46-test-speed-migration.md`.
