# Independent audit of frozen checkpoint66

Checkpoint66's frozen source/archive, production artifacts and test execution binding verify. Its three changed targets execute 100 active cases, and the 212-test counts, six gap suites, five historical differences, folder log and fixture snapshots remain stable. The B98 headless comparison is correctly qualified: raw bytes differ, while one exact KEY-path substitution in a separate buffer matches the retained MST. It is not fresh original or whole-B98 acceptance.

Only this report was written. The audit read frozen archives, receipts/logs and the already-retained B98 scratch/anchor bytes. No source/build/shared-state edit, executable/test, GUI or REA action occurred. Normalization was performed in memory only. Tokens and broader acceptance/performance benefits are unavailable.

## Source/archive/artifact binding

| Evidence | Verified identity |
|---|---|
| Frozen receipt | build/wave3-checkpoint66-verified/receipt.json; SHA-256 f3c0da614661a185e29024cea864169e55c7072c1cea282ed9b665b55dbeb171 |
| Source ZIP byte hash | e6672a7b48ba454c074db940e64d6286fbb3fbe4931578c94845a40fecd02f81 |
| Canonical source tree | 3286 members; SHA-256 866af960cea94c8187cf87fb2a1c11c8b077276ae636d830f0a4aeb1ab2ce6cc |
| Build | analysis/resume66-integrated-full-build.json; stable matching source before/after |
| Validation | analysis/wave3-checkpoint66-validation.json; SHA-256 b77fb9105b7d446b88a5583872ede8b3be4d6528d6a1976a64bb0c261dda9e77 |
| Full run | build/test-runs/1792881138705653600 |
| Selected test receipt | analysis/resume66-integrated-full-tests.json; SHA-256 ba29b3dcd6e859bbc6e29c017178416194af7290d78d0fff1800ff7b3cf3908f |

Recomputed ZIP byte hash and every source-member fingerprint in canonical WindowsPath order. They agree with the selected build, archive, test execution boundaries and freeze. The analysis test receipt is byte-identical to the run receipt. All five hashed contract reports and all three focused log hashes match validation.

execution_binding has equal 3286-file source boundaries, 209 equal binary snapshots and stable_during_test=true. Compared 207 built-output size/SHA records with the selected build, finding no mismatches. cmake.exe and python.exe are the two external tool entries. Both CTest and effective launcher exit 0.

Rehashed all three preserved production artifacts:

| Artifact | SHA-256 |
|---|---|
| moproj2mst.exe | f1da163972c32f7af62768f22c219c4264839509062a984bc9d244367c05e8b9 |
| moproj2mst.dll | 346e432ed062b440906c904ec466a8ff77816528bb669dbc17c7a93294e5bcc2 |
| deformdb.dll | c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3 |

No binding/integrity defect was found. This proof applies to the frozen candidate, not arbitrary successor working files or unobserved native state.

## Assertions and stable suite qualifications

The three changed targets have matching frozen declarations and runtime summaries: editor_base 72, ppv_public_lifecycle 4 and viewer_box 24, 100 cases total. No raw C assert calls were found in scanned frozen test_*.cpp files. The focused logs contain no emitted SKIP/REFUSED rows. No zero-case success is logged; the one empty successful log is the known Python keyword-table exit-status checker.

The full run registers 212 tests: 205 clean passes, six passed-with-logged-gaps and one passed-with-logged-differences; no failed/skipped/incomplete/unreported tests. The six unchanged gap targets are mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current. Their expected refusals/missing fixtures/comparison omissions still count as qualifications. Existing g0 ABI OK-or-unsupported smoke behavior is not a strict positive conversion proof.

The public PPV target now has four active candidate cases, including continuation after KEY Save-open failure. Candidate-only output/status assertions do not establish original runtime, all file-failure timing or private Close/global cleanup. The integration record correctly remains frozen_incomplete and keeps actual DOE refresh/Boolean adoption guarded.

## Historical/fixture stability

All 16,728 fixture records are equal before/after. Both snapshot-file hashes are ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7. Against checkpoint65, the frozen folder comparator source is unchanged, SHA-256 38c6fb80a3bc30a9cf15b49233a88e83f33a8dd4c4b1da5570254dd33679ad83, and the full folder-log bytes are identical, SHA-256 58dd03cef8bfc3a4f22ce0ca8b8296fd63fa5bcda50aa8811de986c58002f77b.

Historical result/difference records are exactly equal: 93 SAME / 5 DIFF, changed_diagnostics empty. A55 still has one differing file; B120/B121/B122 have one differing file and 7/8/10 missing; B98 has one differing file. The four folder exclusions/noncomparison notes remain. Identical normalized diagnostics do not establish identical raw bytes of every regenerated tree; successful work trees are removed and listings are capped.

## Independent B98 byte check

Reviewed analysis/resume66-b98-headless-comparison.md and directly read its retained short-path output at C:/PROBLEM/cp66b98/b98r-32196-22540500/candidate.MST plus the immutable historical 98_B_DS3F/Gear_blank.MST anchor:

| Buffer | Bytes | SHA-256 |
|---|---:|---|
| Raw candidate | 2729 | 4b9cd7c91bd648a42fae7e0384521866d3185637d4d164ca4d34f155d2504e00 |
| Retained MST | 2756 | e69980305fc2ad704a1f44c57b723592af7494a5e97ab61826867a193efa27c1 |
| Separate normalized buffer | 2756 | Same exact retained MST hash |

Reproduced first raw difference at byte 1341 and found exactly one occurrence of `C:/PROBLEM/cp66b98/b98r-32196-22540500/Gear_blank.KEY`. Replacing only that occurrence in memory with `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY` yields byte-for-byte retained equality. The raw scratch file was not changed. No other content, banner, whitespace or model value was normalized.

The B98 test source is unchanged between frozen CP65 and 66, SHA-256 350a1ebcc463bf9d9772e6ab3a5f1283eedc3f341fa551c729a04fcaf39ca4fb. The CP66 full run's m2m.s.b98_kfread target passes in its own bound execution. The separately documented direct headless invocation used the already-built CP65-era test; its process exit and invocation history are documentary evidence in that report, not borrowed from CP66's execution-binding interval. This audit independently verifies its retained raw/normalized bytes, not a fresh process run.

The qualification is accurate and necessary: path-qualified candidate-to-retained-output equality explains this bounded writer-path comparison, while raw candidate equality is false. The anchor is historical, not a fresh original session. Do not decrement the five-DIFF historical lifecycle denominator, claim full B98/raw parity, or widen the folder comparator merely to turn this result into a pass. The report/integration record explicitly preserve those limits.

## Completion conclusion

No source/archive/binary/test binding defect, newly hidden focused refusal or historical/fixture drift was found. Freeze/validation retain objective_complete=false; the integration record's incomplete and fresh-original-blocked qualifications are consistent with the evidence. No duplicate optional run is required to validate the already-retained bytes. Required original/state/failure/lifetime acceptance remains separate work under the existing control-release and single-owner constraints, and the next successor must retain the full exact freeze discipline.
