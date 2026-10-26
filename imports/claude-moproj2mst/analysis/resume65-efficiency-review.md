# CP65 efficiency review: grouped preflight and full-run costs

## CP63 to CP64 receipt comparison

| Checkpoint | Build invocations | Sum of recorded build elapsed | Full launcher | CTest cases |
|---|---:|---:|---:|---:|
| CP63 | 5 | 101.830 s | 31.047 s | 204 |
| CP64 | 3 | 74.139 s | 29.531 s | 208 |

The build elapsed sums are receipt totals, not a claim about non-overlapping end-to-end wall time. CP64’s three invocations were two successful focused Release target groups and one successful full build. This is 27.691 s less recorded build work than CP63 while registering four additional tests and avoiding CP63’s two failed compile attempts. The comparison is not a controlled timing experiment because source snapshots, targets, and build-cache state differ.

CP64’s focused groups were:

- 3 targets: configure 10.890 s, build 4.688 s, total 18.314 s, fingerprint `7b0e218d…7252`.
- 8 targets: configure 10.828 s, build 6.688 s, total 20.256 s, fingerprint `576df3ad…e97`.
- Full build on the latter fingerprint: configure 9.390 s, build 23.453 s, total 35.569 s.

The eight-target group repeats all three target names from the earlier group. The two preflight fingerprints differ, so the repeated targets were checked against distinct source snapshots; the final full build used the eight-target group’s fingerprint and reported stable source during build. Every receipt passed. CP63 instead accumulated two full-build compile failures in the PPV text-writer test path before later repair/focused/full builds succeeded. CP64’s grouped preflight approach therefore has encouraging payoff: compared with CP63’s receipt sum, CP64 records 27.691 fewer build seconds despite the extra four CTest cases. It establishes clean compilation of the selected targets before the full build, but the receipts do not isolate the counterfactual savings attributable to grouping alone.

## Full launcher phases

| Phase | CP63 | CP64 | Change |
|---|---:|---:|---:|
| Inventory | 0.953 s | 1.000 s | +0.047 s |
| Fixture SHA before/after | 13.671 s | 12.719 s | −0.952 s |
| Source/binary binding before/after | 3.906 s | 3.531 s | −0.375 s |
| CTest | 12.438 s | 12.203 s | −0.235 s |
| JUnit/report | 0.079 s | 0.063 s | −0.016 s |
| Full launcher | 31.047 s | 29.531 s | −1.516 s |

CP64 registered 208 tests versus 204. Its receipt has zero skipped or failed tests, no fixture changes, stable source fingerprint, and matching before/after hashes for all 205 captured CTest/CLI binaries. The six logged-gap and one logged-difference statuses remain recorded. The launcher reduction is not a demonstrated optimization because run-to-run fixture hashing and cache conditions differ.

## One next optimization

Before starting focused builds, hold source edits until the current ready targets can be compiled against one stable source fingerprint. Then run one grouped Release preflight containing the union of ready target names, followed by the full integration build on that same fingerprint. CP64 shows why: the later group already contained the earlier three targets, but the source hash had changed between groups. An ownership/fingerprint barrier would let the group cover both sets in one configure/build invocation. It can avoid at least one observed CMake configure phase (10.890 s); any duplicated compilation avoided is additional but not quantified here. Keep the full integration build and all CTest, fixture-hash, and source/binary-binding phases unchanged.

This next step is more promising than concurrent source hashing, whose CP63 microbenchmark suggested only about 0.92 s across two source scans before contention. Token usage was unavailable in the inspected receipts.

Evidence: CP63 build receipts `resume63-integrated-full-build.json`, `resume63-integrated-full-repair-build.json`, `resume63-integrated-full-repair2-build.json`, `resume63-ppv-test-fix-build.json`, and `resume63-integrated-full3-build.json`; CP63 timing `build/test-runs/1792878753102640300/timing.json`; CP64 receipts `resume64-focused-preflight-build.json`, `resume64-focused-preflight2-build.json`, `resume64-integrated-full-build.json`, `resume64-integrated-full-tests.json`; CP64 timing `build/test-runs/1792879860790610300/timing.json`.
