# CP65 independent audit of frozen checkpoint64

Checkpoint64's archive, production artifacts and source/build/test binding verify. Focused assertions are active, and the historical diagnostic output and fixture manifests are unchanged from checkpoint63. No false objective-completion claim was found in the inspected freeze/validation records: both retain objective_complete=false. The important limit is that qualified candidate passes are not fresh original acceptance or raw whole-tree equality.

Only this report was written. The audit read frozen archives, retained receipts/logs and selected assertion bodies; it ran no executable/build, changed no source/shared state and used no GUI or mutable REA. Token use is unavailable. No extra acceptance is inferred from current successor files.

## Binding and archive verification

| Evidence | Verified value |
|---|---|
| Frozen receipt | build/wave3-checkpoint64-verified/receipt.json; SHA-256 9e335c72153f41814bd9ab76d9f4abcc08438da97a34227e7e4d89c844597d8a |
| ZIP bytes | SHA-256 5c243ee5983c49d91c8cbdde2977aaadba94d31e82358412562992d2b5985839 |
| Source members/tree | 3274 files; SHA-256 576df3adb853098b3e53a202aa6a66177195adc91b9ebf77ac258cb5c8957e97 |
| Build | analysis/resume64-integrated-full-build.json; stable matching source before/after |
| Validation | analysis/wave3-checkpoint64-validation.json; SHA-256 7bfed83521be906690e22777a57cdf04f3e89f747913fda1280660aa1edd4d10 |
| Full run | build/test-runs/1792879860790610300 |
| Test receipt / exact selected analysis record | analysis/resume64-integrated-full-tests.json; SHA-256 2a3bb751c440961ed87392478b6fb0d9731848eb6dcbd091e12ff4262061349b |

Recomputed archive byte hash and every member fingerprint in canonical WindowsPath order. Source tree agrees with the selected stable build, test execution boundaries and freeze. All eight hashed contract reports and all eight focused log hashes match validation.

The execution binding has identical source before/after snapshots and 205 identical binary before/after entries. Independently compared 203 built-output size/SHA records with the selected build; there are no mismatches. The other two entries are external cmake.exe/python.exe. Both raw CTest exit and effective launcher exit are 0. This proves sampled source/binary stability for this run and its selected build, not unobserved native state or original runtime behavior.

Rehashed all three preserved production artifacts:

| Artifact | SHA-256 |
|---|---|
| moproj2mst.exe | 0a2a1be382e0b17e0f86ac73fc9821176ddc0a3affa6ebf0a334c829a0a08bd3 |
| moproj2mst.dll | 42d6d0b5fda866b3333159198744ad7bba0660aa38304a98a9078e8396ef9a75 |
| deformdb.dll | c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3 |

A convenience integration-summary JSON was absent at initial inspection; the complete freeze/build/validation/run chain above is sufficient for this audit and is not invalidated by that omission.

## Active assertions and logged refusal analysis

The focused subset runs 82 Testkit cases, with source declarations matching retained summaries: DBLIST manager 8, Type2 Save node 7, Boolean editor 15, Close status owner 10, message cache 9, viewer box 19, texture-copy origin 12 and moproj writer 2. Scanning frozen test_*.cpp files found no raw C assert calls. No zero-case success was logged. The only empty successful log is the known Python PE keyword-table --check target with meaningful exit-status validation.

There are no focused CHECK/REQUIRE predicates that permit M2M_OK-or-unsupported as a substitute for a positive result. Most negative-path helpers explicitly require M2M_E_UNSUPPORTED, often with the expected message substring. Skip/reject/guard words in their successful case names are negative-branch coverage, not an actual skipped CTest.

One existing focused helper remains permissive: frozen tests/moproj/test_moproj_write.cpp catches M2M_E_UNSUPPORTED around conversion/writing, prints SKIP and returns true. This is a potential non-positive oracle case, not a generic swallowed exception; other errors rethrow. No actual SKIP/refusal was printed by that target in this run, so it is not a newly hidden refusal. Keep it qualified if used to support completion rather than treating its helper-return true alone as successful conversion.

The full suite retains 208 registered tests: 201 clean passes, six passed-with-logged-gaps, one passed-with-logged-differences and no failed/skipped/incomplete/unreported tests. The six gap targets remain mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current, with the same explicit omissions/refusals. The separate g0 ABI smoke still allows M2M_OK or M2M_E_UNSUPPORTED. These registrations cannot be counted as 208 strict original successes.

The public PPV source/log is unchanged from checkpoint63: three active exact-M2M_OK cases for local rewrite, root-only preservation and read-only PPV Save-open failure with KEY success. Log SHA-256 is 8a641d8ae56a8bcd8afea1ce003b841684045cb0c73c394434d9ace902a03483. Those are synthetic candidate contracts under retained static ordering evidence, not fresh original output/state or complete Close acceptance.

## Historical raw-difference and fixture stability

Independent comparisons with checkpoint63 establish:

- Frozen folder comparator source is identical, SHA-256 38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83.
- Full folder-log bytes are identical, SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b.
- The five historical difference records are exactly equal; changed_diagnostics is empty.
- Full fixtures-before/after records are exactly equal over 16,728 files; both snapshot-file SHA-256 values are ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7.

| Stable difference | Differ | Missing | Extra |
|---|---:|---:|---:|
| A:RS_55_Multiple_Operations_Example1 | 1 | 0 | 0 |
| B:120_B_DOE | 1 | 7 | 0 |
| B:121_B_DOE_VAR | 1 | 8 | 0 |
| B:122_B_DOE_GEO | 1 | 10 | 0 |
| B:98_B_DS3F | 1 | 0 | 0 |

The diagnostic remains 93 SAME / 5 DIFF under declared path/view/banner rules. Its work trees are deleted and detailed listings are capped; identical diagnostics do not independently establish unchanged raw bytes for every regenerated output tree. Retain the narrower claim, not fresh 98-case raw-tree parity.

The focused moproj writer log also remains byte-identical to checkpoint63. It prints six raw difference excerpts while accepting equality outside view-derived parts: R11/R19 and their duplicated resaves/82_R52 CenterX profiles, plus 106_B_F2ACT group Expanded. Thus even a clean test classification can contain declared normalization; 201 clean registrations are not 201 raw-byte comparisons. This is stable known qualification, not a new regression.

## Completion-evidence priorities

1. **Preserve the incomplete-objective label.** The inspected freeze and validation do this correctly. Binding is sound; no identical rerun or optional test expansion is required to repair it.
2. **Do not promote prefixes into public closure.** Validation explicitly says Type2 shared Save stops after the pending study node at InputTable, while public C ABI still selects subproject zero. Geometry now guards unqualified admission rather than using unsupported raw arithmetic. The source-qualified helper/guard tests and unchanged corpus do not complete DOE/backend or every real 3D case. Boolean, editor and Close owners/effects still require their missing actual-runtime/teardown evidence.
3. **Separate normalized diagnostics from raw/original acceptance.** Keep all five differences, view/group/banner qualifications, absent optional vectors/fixtures and permissive smoke/refusal behavior visible. Required raw deltas and independent original lifecycle/failure evidence should be acquired in the relevant recovery lane, not invented from pass counts or unchanged logs.
4. **Keep PPV success bounded.** The three candidate cases add public effects but do not directly prove hidden root-list state, actual native helper invocation, physical attempt timing or private global cleanup. Do not claim those dimensions closed without their evidence.

No completion claim exceeding those limits was found in the inspected qualification-bearing records. A statement such as full original parity, no refusals, all 208 cases raw-identical, all Type2 Save complete or general geometry support would exceed the evidence. The next successor must retain this exact archive/build/test binding discipline and full regression; these findings authorize no weakened assertion, omitted hash or guard bypass.
