# CP65 three-checkpoint efficiency review

## Build and preflight receipts

| Checkpoint / receipt | Configure | Build | Total | Result | Source fingerprint |
|---|---:|---:|---:|---|---|
| CP63 integrated attempt 1 | 10.844 s | 10.641 s | 24.343 s | Failed compile | `80779f13…` |
| CP63 repair attempt | 10.782 s | 1.968 s | 15.444 s | Failed compile | `dc96a136…` |
| CP63 repair attempt 2 | 10.812 s | 19.329 s | 32.848 s | Passed | `5eaea22a…` |
| CP63 focused PPV target | 10.813 s | 2.000 s | 15.526 s | Passed | `53187aae…` |
| CP63 integrated build | 10.844 s | 0.140 s | 13.669 s | Passed | `53187aae…` |
| CP64 focused group 1, 3 targets | 10.890 s | 4.688 s | 18.314 s | Passed | `7b0e218d…` |
| CP64 focused group 2, 8 targets | 10.828 s | 6.688 s | 20.256 s | Passed | `576df3ad…` |
| CP64 integrated build | 9.390 s | 23.453 s | 35.569 s | Passed | `576df3ad…` |
| CP65 focused group 1, 8 targets | 11.297 s | 12.609 s | 27.355 s | Failed link | `048ea489…` |
| CP65 focused group 2, 8 targets | 10.797 s | 1.219 s | 14.908 s | Passed | `faede925…` |
| CP65 focused group 3, 2 targets | 10.688 s | 2.453 s | 16.001 s | Passed | `3b4b3056…` |
| CP65 integrated build | 10.735 s | 33.719 s | 47.324 s | Passed | `3b4b3056…` |

The recorded build-time sums are 101.830 s for CP63, 74.139 s for CP64, and 105.588 s for CP65. CP64 reduced recorded build work by 27.691 s relative to CP63, with no failed compile receipts. CP65 then rose 31.449 s from CP64. These sums include separate configure calls and are not controlled end-to-end comparisons; target sets, source snapshots, cache state, and changed code differ. There is no evidence for a 10x gain.

CP63’s first two full builds failed in the PPV text-writer test path with C2679. CP64’s two focused target groups both passed, though their fingerprints differed and the first three targets were repeated in the later eight-target group. CP65’s first eight-target preflight failed linking `m2m_s_report_ds_path.exe` because `main` was unresolved; the log reaches the link stage after the preceding target work. This is a testkit-entrypoint contract issue that a pre-build owner check could have caught. The same eight-target group passed in the next receipt on a different fingerprint, followed by a two-target preflight and the full build on a third fingerprint. CP65 thus still had source evolution between preflights, but it ended with a stable candidate boundary: its last focused receipt and integrated full-build receipt share fingerprint `3b4b3056…c9`, and both builds report stable source during their own operation.

The post-freeze test receipts also show stable test-time boundaries. CP63 had 204 tests, a 31.047 s launcher and 12.438 s CTest; CP64 had 208 tests, 29.531 s launcher and 12.203 s CTest; CP65 had 212 tests, 30.109 s launcher and 12.422 s CTest. CP65’s launcher phases were 0.984 s inventory, 13.031 s fixture hashing, 3.563 s source/binary bindings, 12.422 s CTest, and 0.093 s report parsing. It reported no fixture changes, no skips or failures, stable source binding, and equal before/after hashes for 209 captured binaries. Its six logged gaps and one logged difference remain recorded. Launcher differences are small and do not establish a speed trend.

## What improved and what remains

The process repeatedly finishes with a tested source snapshot bound to the full build: CP63’s final focused PPV build matched its integrated build; CP64’s final focused group matched its integrated build; CP65’s final focused two-target build matched its integrated build. CP65’s full build receipt and freeze archive both identify the same full-source fingerprint and 3,286 files. This is evidence that the final integration boundary is stable and the test run remains hash-bound; it does not show that all earlier preflight batches were on one unchanged source revision.

The compile preflights expose errors before full CTest: CP65’s missing test entrypoint failed in the focused compile/link stage, and the full 212-test run proceeded only after later preflights and a successful full build. They do not yet reduce build time consistently: CP65 still required one failed preflight and three successful configure/build invocations, and its full compile phase was 33.719 s. The receipts support earlier detection and snapshot binding, not a universal build-speed reduction.

## Next measured efficiency step

Add a cheap owner-side preflight check for every newly added or modified C++ test target before launching the grouped build: verify the required Testkit entry point (`main`/`testkit::run_all()`), resolve quoted includes against target include paths, and check the supported assertion/diagnostic types. CP65 provides a concrete payoff target: the failed grouped build spent 12.609 s in the build phase before reporting the unresolved `main`; the link failure is detectable from the test file alone. Run this check on each owner’s ready target set, then compile the union once against a stable source fingerprint and follow it with the full build and unchanged full CTest launcher. Do not skip any CTest cases, fixture hashes, or source/binary binding phases.

This next step aims to prevent avoidable compile feedback and repeated preflight work; its savings remain unmeasured. Token usage was not present in the receipts.

Evidence: CP63 receipts `resume63-integrated-full-build.json`, `resume63-integrated-full-repair-build.json`, `resume63-integrated-full-repair2-build.json`, `resume63-ppv-test-fix-build.json`, `resume63-integrated-full3-build.json`, and `resume63-integrated-full3-tests.json`; CP64 receipts `resume64-focused-preflight-build.json`, `resume64-focused-preflight2-build.json`, `resume64-integrated-full-build.json`, and `resume64-integrated-full-tests.json`; CP65 receipts `resume65-focused-preflight-build.json`, `resume65-focused-preflight2-build.json`, `resume65-focused-preflight3-build.json`, `resume65-integrated-full-build.json`, `resume65-integrated-full-tests.json`, `wave3-checkpoint65-validation.json`, and `build/wave3-checkpoint65-verified/receipt.json`.
