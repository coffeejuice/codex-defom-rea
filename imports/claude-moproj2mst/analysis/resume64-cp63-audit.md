# CP64 independent audit of frozen checkpoint63

No source/archive/artifact/test-binding defect was found in checkpoint63. Focused assertions are active and the historical diagnostic differences are byte-for-byte stable against checkpoint62. The remaining completion-evidence risk is overstating those results as raw whole-tree equality or fresh original lifecycle acceptance. Both remain unproved; no extra optional rerun is needed merely to reconfirm the already-bound candidate checks.

Only this report was written. The audit read immutable source archives, receipts, logs and qualification records; it changed no source/build/shared state and used no GUI or mutable REA. No executable or test was run. Token use is unavailable and is not estimated.

## Independent identity and binding checks

| Evidence | Verified identity |
|---|---|
| Frozen receipt | build/wave3-checkpoint63-verified/receipt.json; SHA-256 8b429b2fb80bab60bd6584e572a044a805fa7eac56e1668b73c2c58e36bd069f |
| Source ZIP bytes | SHA-256 06be69951cf078fef04bb5d98f7094c7825f64a6164c84f11e6d72aaef60db87 |
| Canonical source tree | 3264 members; SHA-256 53187aae9b860f1b2a95b40a4481fe2b299c95f7a8c2393e560483805622bde8 |
| Selected build | analysis/resume63-integrated-full3-build.json; stable before/after source hash matches the archive |
| Validation | analysis/wave3-checkpoint63-validation.json; SHA-256 e60cd70fd04408dc2078d16f4c80738bddd4003b77ffbe4356e9beafc5f68c06 |
| Full run | build/test-runs/1792878753102640300 |
| Run receipt / exact analysis copy | SHA-256 1b5fb64e7b4e1ba46c558ddcdbbc1887a83d6d95241bfc65af29daaa809fc699 |

Recomputed ZIP byte hash and every member's source fingerprint in canonical WindowsPath order. The source hash matches the freeze, stable build and test execution source boundaries. Rehashed all three preserved production artifacts:

- moproj2mst.exe: 6488f47c095ef546620cf5a0f7898d20083e3d6e6d7b30350d6376aef318a385.
- moproj2mst.dll: 184d2957756881c37e54ac019788e6d7c01c4db925f5ef8d9d4b499ebe9a224d.
- deformdb.dll: c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3.

execution_binding reports stable_during_test=true, equal 3264-file source snapshots and 201 equal binary snapshots. Independently matched 199 built outputs against the selected build's size/SHA-256 artifact records; the other two entries are cmake.exe and python.exe. There are no mismatches. CTest exit and launcher exit are both 0. All eight contract hashes and all eight focused log hashes match their validation records. This establishes the tested candidate's retained binding; it does not measure unrecorded native/OS state or grant current successor source the same result.

## Focused assertion and refusal audit

The eight focused logs execute 140 Testkit cases: DBLIST warning receipt 3; object color values 9; editor setup 58; PaintDevice active copy 40; public PPV 3; PPV text writer 4; viewer box 17; Save event prefix 6. Each count matches TEST_CASE declarations in the frozen source. The source scan found no raw C assert calls in test_*.cpp files. No 0-of-0 result is present. The only empty successful log is the known Python PE keyword-table checker, whose --check mode communicates comparison through exit status; it is not an ineffective C++ test.

Case-name matches for skip/reject/guard occur in successful negative-branch tests, not actual skipped/refused focused execution. The public PPV log contains local rewrite, root-only file preservation and read-only Save-open-failure/KEY-success cases, all with exact M2M_OK expectations. There is no OK-or-unsupported escape in those positives. Its retained log SHA-256 is 8a641d8ae56a8bcd8afea1ce003b841684045cb0c73c394434d9ace902a03483, independently matched to the validation record.

The read-only PPV case verifies returned success, MST existence, unchanged PPV bytes/attributes and a KEY modification time advanced from an old sentinel. Those are meaningful candidate filesystem effects. They do not independently record the attempted physical open, exact KEY bytes, stream failure timing or private Close state. Static ordering evidence can support the declared branch, but the runtime assertions should not be promoted beyond their observables. No extra optional test is required to keep its current narrow candidate contract.

The full suite remains 204 registered tests: 197 clean passes, six logged-gap passes and one diagnostic-difference pass. The six gap targets and the permissive g0 ABI smoke predicate remain qualified as before. Thus 204 passing registrations are not 204 successful original comparisons. Frozen test_abi.cpp's real-project smoke still permits M2M_OK or M2M_E_UNSUPPORTED; retain it as ABI/refusal evidence, not a strict positive acceptance case.

## Historical difference stability

The complete folder log is exactly equal to checkpoint62, SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b. The frozen folder comparator source is also exactly equal, SHA-256 38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83. Validation historical-difference records are exactly equal and changed_diagnostics is empty:

| Case | Differing | Missing | Extra |
|---|---:|---:|---:|
| A:RS_55_Multiple_Operations_Example1 | 1 | 0 | 0 |
| B:120_B_DOE | 1 | 7 | 0 |
| B:121_B_DOE_VAR | 1 | 8 | 0 |
| B:122_B_DOE_GEO | 1 | 10 | 0 |
| B:98_B_DS3F | 1 | 0 | 0 |

The comparator still reports 93 SAME / 5 DIFF under its declared path/view/banner rules. All 16,728 fixture records are equal before/after, both snapshot-file hashes ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7. This proves historical diagnostic/fixture stability. It does not prove every regenerated raw output byte is unchanged: successful work trees are removed, diagnostics cap detailed listings, and equality uses declared normalizations. Do not relabel identical diagnostics as a fresh 98-case raw-tree byte comparison. A stronger raw-output claim needs the corresponding retained per-file output bytes/manifests when that required acceptance work is performed; this audit does not recommend an unneeded reconfirmation run now.

## Priority completion-evidence corrections

1. **Keep the result label precise.** Freeze metadata already says objective_complete=false. Preserve the six gap targets, five differences, public guards and synthetic-candidate nature of the three PPV cases. The candidate binding is verified; no evidence repair is needed there. A missing convenience integration-summary JSON at initial inspection does not invalidate the complete freeze/build/test chain above.
2. **Do not claim fresh original or raw-byte completion.** No original executable ran in this validation. Independent original positive/failure/lifetime acceptance remains required, along with evidence-supported closure of editor runtime, DOE registration/backend, Boolean timing and Close teardown. Resolve those actual missing dimensions rather than adding more optional helper tests for already-passing prefixes. Original runtime remains subject to the existing supported control-release/status and single-owner capture constraints.
3. **Preserve raw differences when their lane next changes.** The five unresolved rows are stable diagnostic evidence, not explained away. When root advances the corresponding implementation/acceptance lane, retain the relevant generated/expected file hashes and raw deltas alongside declared normalization. Do not weaken the diagnostic test or count normalized SAME as raw parity.
4. **Keep failure/cleanup claims within observations.** The new read-only PPV case expands public candidate failure-effect evidence, but no private Close/global-reset or precise physical write-attempt ordering is directly observed. Use the existing source-qualified contracts and exact retained outputs; do not invent acceptance for unobserved effects. The previously documented long-scratch-path and DLL-lifetime qualifications remain separate setup/coverage items, not reasons to discard these candidate results.

These are completion-evidence priorities, not a request to broaden this checkpoint with optional tests or another identical full run. The next integrated candidate must still retain stable source/binary execution binding, exact source archive, production artifact hashes, full fixture snapshots and the complete required suite. No fidelity, failure behavior, interface or acceptance boundary is relaxed.
