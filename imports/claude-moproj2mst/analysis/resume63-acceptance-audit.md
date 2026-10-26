# CP63 independent acceptance audit of frozen checkpoint62

Checkpoint62 has a sound retained source/build/test binding and two active public PPV candidate regressions. It remains incomplete original acceptance. The highest-priority missing dimensions are fresh original comparisons, public DOE/Close/editor/Boolean continuation and physical failure/lifetime behavior. The reported 200-test pass must retain its six gap tests and five historical folder differences.

Only this report was written. The audit read frozen source.zip, build/validation/test receipts, selected source qualifications and retained artifacts. No source/build/fixture/shared edit, executable, GUI or mutable REA operation occurred. Prioritized actions below are proposals for root assignment, not executed work or new acceptance claims.

## Binding proof independently checked

Frozen candidate: build/wave3-checkpoint62-verified/receipt.json. Source archive SHA-256: bb47d4f30c6bf15e6aae17cb20608d7364c045a179e9744cf48e4357f1d13194. Rehashed all 3252 source members in canonical WindowsPath/source-fingerprint order and obtained c7b042d0a2e0c016a997acc45452aa4af9fd16c357abbabb9dfd02308d097013, matching archive/build/test/freeze records. Sorting ZIP strings in case-sensitive lexical order produces a different hash; that is an ordering difference, not an archive mismatch. The archive byte hash itself matched too.

The stable full build is analysis/resume62-integrated-full-build.json. Full run is build/test-runs/1792877980706167600. Its receipt SHA-256 is 5090322cfc23a23d484976f7e23f9ee13d152a9d3b21e8c0dd4a2b47626bc5dc. execution_binding independently reports the same 3252-file source hash before/after CTest, 197 equal binary before/after entries and stable_during_test=true. Compared the entries with the selected build: 195 built outputs match size/hash, with cmake.exe and python.exe the two external tool entries; no mismatches. All eight hashed qualification/contract reports in validation still match their recorded hashes.

Rehashed the three preserved production artifacts:

| Artifact | SHA-256 |
|---|---|
| moproj2mst.exe | 716d5ac4489e48b573f7f8377358ef2e23d21cdd751a4a77eb43917f5e0a496d |
| moproj2mst.dll | 3cab68b6fc5a9252eff0818f5aa1b1a54d7c523bcbee8a683eb5fd568a0191a8 |
| deformdb.dll | c89256fd4a9b226adc9e1eeb57dcaf36a99ab1dfae66b1fc1f419077e9577aa3 |

The before/after fixture JSON files are exactly equal over 16,728 files, each SHA-256 ddcc3d21078789fc77fa292c110796e6fe70a15692103339436e8462b2d050b7. Source/binary snapshots prove stability at their sampled boundaries and bind this run to its build; they do not observe arbitrary external DLLs, the entire OS environment, or every intermediate native state. No such broader claim is needed for this binding result.

The checkpoint61 source-advance problem is not carried into checkpoint62's archive proof. Current CP63 working files may advance independently; use the immutable checkpoint62 archive for claims about this run.

## Active assertions and qualified passes

The frozen-source scan found no raw C assert calls in test_*.cpp files. The two new prefix tests use active Testkit cases: DBLIST manager 6, Close status 8. The public PPV file has two active cases and no raw C assert; retained log SHA-256 704a5f4b92b065e0555d8714714f5ba818387113ed404c78fa6c67336658a786 contains both local rewrite and root-only preservation success, ending 0 of 2 cases failed. Thus the second public case is now covered by checkpoint62, unlike the earlier one-case checkpoint61 receipt.

The run reports 193 clean passes, six passed-with-logged-gaps, one passed-with-logged-differences, no failures/skipped/incomplete/unreported tests. An output scan found no 0-of-0 success; the empty Python keyword-table log is legitimate --check exit-status behavior, not a disabled C++ test. Custom non-Testkit harnesses should remain judged by their actual exit/check logic, not rejected merely for lacking TEST_CASE macros.

The six gap tests remain mdt.mdt_write, mdt.save_l6, moproj.deflate, moproj.props_l2, p07.corpus and s.save_current. Their recorded omissions include active-cogging refusal, absent optional PACK/deflate fixtures, unsupported class inputs, excluded/missing corpus projects and unperformed original-edit comparisons. The folder diagnostic additionally records four exclusion/noncomparison notes and five actual differences. These are explicit qualifications; do not collapse them into 200 positive original passes.

A permissive assertion remains in frozen tests/g0/test_abi.cpp: rc may be M2M_OK or M2M_E_UNSUPPORTED for its real-project smoke case. This is meaningful ABI/refusal behavior but cannot establish successful conversion of that case. The public PPV tests do require exact M2M_OK, so there is no hidden refusal allowance in those two positive candidate regressions.

## Missing original acceptance dimensions

| Dimension | Established by checkpoint62 | Still missing |
|---|---|---|
| PPV local public Open/Save | Exported C ABI returns M2M_OK; local input is rewritten to exact source-derived bytes | Fresh original same-input output/state comparison, more encodings/profiles and physical read/write/close failure effects |
| PPV root-only public effect | M2M_OK and existing PPV bytes preserved | Hidden shared-list population/native helper invocation cannot be inferred from unchanged bytes; the contract explicitly says current outer local-empty guard can produce that effect |
| DOE DBLIST manager | Six-case actual-owner exact-path lookup/last-match prefix | Absent-child constructor, path/.DS load/registration/backend continuation and B120/B121/B122/B127 public behavior |
| Close | Eight-case fresh qualified null-main status skip and nonnull guard | Actual nonnull QMainWindow/status/timer effects, QString temporary release, public return and teardown/global-reset ownership |
| Editor/PaintDevice | Source-derived bounded owner/cleanup/control branches and active regressions | Actual original runtime canonical owners and remaining constructor/copy/destructor/event timing before public editor completion |
| Geometry | Populated synthetic getter/COW/rectangle behavior and malformed guards | A real original geometry-list mesh fixture proving relevant storage, state transitions and output parity |
| ABI/lifecycle | Selected strict candidate successes and existing ABI/refusal tests | Broad positive held-out Open-Save-Close acceptance and private cleanup/global-state observations |
| Historical full Save | Same exact diagnostic log, 93 SAME / 5 DIFF | Explained/resolved raw differences and independent fresh-session acceptance; declared normalization is not raw equality |

Public PPV copies intentionally change the retained project's Active index and inject synthetic rows; they are candidate contract fixtures, not untouched held-out original evidence. Full-copy/source manifests preserve input fidelity but do not supply an original answer for those edits. The root-only contract already disclaims hidden-state inference; preserve that limitation rather than adding a claim from unchanged output.

## Prioritized parallel repair actions

Root can assign these disjoint evidence/fixture lanes concurrently with exclusive outputs. Shared public interfaces, final integration/builds and original runtime/REA ownership remain serialized. No lane should widen a guard before its required effects are qualified.

| Priority / lane | Smallest actionable deliverable | Acceptance gate / dependency |
|---|---|---|
| P1 PPV independent acceptance | Prepare one exact short-path copied input for local row and one root-only row, byte/state expectations, expected statuses and error-stage plan; add a paired lower-level actual-owner observation if root population is the claim | Candidate effects and fresh original evidence must be compared separately. Actual original capture waits for the existing supported control-release/status gate and one root owner; fixture/schema preparation can run now |
| P1 DOE absent-child | Recover only DBLIST miss constructor/path/.DS effects with ownership/registration order; retain B120/B121/B122 missing-file map and B127 dependency | Do not equate lookup 6/6 with public DOE completion. Integrate the minimal proven continuation, then strict candidate and independent original comparison |
| P1 Close status/teardown | Separate nonnull status/timer side effects from caller temporary release and destructor/global-reset obligations; produce exact state/lifetime contract | Disjoint static evidence work can run in parallel with DOE/PPV. Root composes once owners/interfaces are established; null-prefix 8/8 is not Close success |
| P1 editor/Boolean runtime origin | Resolve actual canonical runtime/dispatcher ownership and timing occurrence with evidence provenance, not caller-supplied strings alone | Keep unknown state guarded. Static receiver/timing inventories can proceed without a new GUI session; runtime capture remains a serialized dependency |
| P2 strict positive ABI / failure matrix | Inventory where successful real-project conversion is actually required, add exact expected success for qualified cases and preserve separate negative/refusal tests; map partial writes/statuses/cleanup by failure stage | Do not globally replace OK-or-unsupported smoke semantics blindly or manufacture an original output. The strict positives need source/oracle qualification and live Release assertions |
| P2 five-difference closure | In parallel, one owner each for A55 timing, DOE output family and B98 raw bytes/encoding; retain raw artifacts and source-qualified adjustments | Existing gaps are A:RS_55 (1 differing file), B120 (1 different, 7 missing), B121 (1 different, 8 missing), B122 (1 different, 10 missing), B98 (1 different). No normalization-only success claim |
| P2 fixture/path/replay fidelity | Reconcile parent C:\PROBLEM/<100-character project guidance with subtree-local scratch policy; persist per-case scratch/run association and enforce the chosen authorized path constraint | Frozen public test still uses nested build scratch and does not enforce short paths. Do not silently move outside authorized scratch or claim current long paths conform; revalidate any changed setup |

These lanes repair different acceptance dimensions and can prepare evidence independently. They are not a reason to spawn twenty speculative tasks or parallel original-app sessions. The existing full source/binary snapshot binding, archive rehash and exact final regression must remain mandatory after each integrated successor.

The smallest immediate reporting correction is to keep checkpoint62 labeled frozen verified candidate with incomplete objective, not complete original acceptance. Its integration record already does so. The next substantive acceptance gain should be one independently evidenced public lifecycle/failure continuation, rather than another unbound helper count or a broader pass total. Token use is unavailable and is not estimated from this audit's output size.
