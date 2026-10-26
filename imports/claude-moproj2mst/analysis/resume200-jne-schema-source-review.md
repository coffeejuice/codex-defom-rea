# CP200 unresolved JNE source review

**PASS for the production source implementation and revised test-source coverage against the accepted design.** The two prior static-review blockers are resolved by the released test-only additions. No production correctness defect was found. This is an independent static review, not a build or test result, and it does not authorize either JNE successor. Executable acceptance of the latest test hash remains with the coordinating validation lane.

## Evidence and scope

Reviewed the four released files against the archived CP199 versions in `build/wave3-checkpoint199-verified/source.zip`, normalizing line endings only for comparison, and against `analysis/resume200-unresolved-schema-design.md` and the appended PASS review in `analysis/resume200-symbolic-jne-plan-review.md`. The earlier schema-only staging discussion in the latter is qualified by its subsequent explicit review of the detailed design.

The CP199 receipt records source-tree SHA-256 `9c2ab8a0f82ce299c03b89da8daf0f5a895acc1a74c2b719078564321e51885f`. The source archive was rehashed here and matches receipt SHA-256 `6b14e5b0a39889d5115dcfe860295bb0d72f48f85305f2a4544afd57cb9085d3`. `objective_complete=false` does not invalidate that CMP-only freeze. The plan-review opening paragraph abbreviates this archive hash incorrectly by omitting its final `3`; the receipt and this verified hash are authoritative.

Reviewed file SHA-256 values (relative to `source/apps/moproj2mst`):

| File | SHA-256 |
|---|---|
| `include/m2m/doe_task_runtime.hpp` | `087a744bb575123aef2e5192db66a25d213a0e83c43e5d18869e11bb48294956` |
| `include/m2m/doe_task_factory.hpp` | `252bd5d7e3024640f15f827f3c7248b63037935b2b0cb264b38a36828437a7f3` |
| `src/doe/task_factory.cpp` | `037638c5ec91940e5244a16ccdaf6de01687a31eae250314ae473398f549f5ae` |
| `tests/mdt/test_doe_task_factory.cpp` | `973098b04c3b85bf0a0b8ec8c31c6460cd22d2ed62a00361676165047d8fd1af` |

Instruction qualification comes from the supplied design and review, not newly collected original-image evidence. They qualify QtCore RVA `0x2961b`, bytes `0f 85 9f 00 00 00`, fallthrough `0x29621`, taken `0x296c0`. These describe static alternatives, not witnessed outcomes.

## Source checks: PASS

`doe_task_runtime.hpp:343-360` represents exactly that instruction and ordered `(ZeroFlagSet, 0x29621)`, `(ZeroFlagClear, 0x296c0)` pair. The predecessor is the immutable CMP record; flags index 3 refers to its validated `ZeroOfResult32`. No selected edge, concrete value/ZF, or execution witness is added. The tests include compile-time absence checks for these fields. Public copies do not confer issuance authority.

The appended phase leaves `inCalleePrefix()` and CP198 ordinal arrays unchanged. The explicit TLS predicates admit exactly CP199 and CP200 boundaries. Both return RSP `-0xa8` and instruction boundary `0x2961b`; neither successor becomes the cursor. Existing CMP-first current-flags and live-return-slot logic are unchanged. All prior explicit callee-or-CP199 predicates in the factory diff were updated, including sparse-field/backlink checks and ordinal-19 dispatch. The new phase cannot reach CP198 array indexing through the dispatch path.

`task_factory.cpp:381-398` preserves the archived CP199 validator body as the shared history validator. It still checks the complete CP198 history, CMP expression, private root-issued read, and exact issued CMP object and control block. The CP199 wrapper retains its exact phase admission. The new validator checks phase/handles, shared history, nonempty occurrence, exact predecessor object/control block against both frame and factory CMP, literal module/bytes/RVA/length/flags/status/ordered edges, and finally exact JNE object/control block. The new routine does not issue another read or grant.

Existing adapter, allocation, capability/binding and live-grant checks remain ahead of dispatch. In particular, `validateSessionReportNullCaller()` authenticates the grant before the TLS context is used. The pristine/no-capability guard includes the new issuance anchor. The premature-CMP gate admits exactly both TLS phases, and the adjacent JNE rule rejects phase-only, handle-only and coherent public-frame rewinds while private anchors remain retained. Adapter release clears the new anchor alongside CMP/grant authority and leaves diagnostic records available.

`task_factory.cpp:400-415` prepares the complete record, occurrence, shared allocation and terminal diagnostic before failure injection or publication. The three publication assignments are nonthrowing shared-pointer/phase assignments in the serialized adapter model. Preparation failure therefore preserves the accepted CP199 state. Throwing/copying the prepared terminal diagnostic after commit is not a rollback promise. There is no new native field, stack/register/flags update, target instruction, helper return, branch evaluation, once-state read, getter or cache/list publication in this diff. The default-false switch preserves CP199 production stopping behavior; CP200 terminal reentry validates the same handles regardless of subsequent switch changes.

## Revised test review: PASS

Re-reviewed the final test-only release and the appended acceptance-coverage section of `analysis/resume200-jne-schema-implementation.md`. The three production hashes above are unchanged. The test hash above identifies the revised source; the original reviewed test hash was `409d29c1d4a064f2a5e3a0cc15fd2fd691ed1452760c42254e39cfcc12b06c73`.

The existing descriptor, default-CP199 stop, terminal-reentry, preparation-failure, malformed-record, rewind and lifecycle tests remain. The final source now contains seven dedicated CP200 test cases. The two prior coverage blockers are resolved as follows:

1. `cp200SemanticEqual` compares every JNE qualification field and the semantic CMP/read/history state across independent runs. It covers CMP arithmetic, the qualified read identity/domain/context, all eight stack descriptions and token roles, SUB histories, nested/outer calls, live returns, saved-register and initializer-call qualifications, private initializer version, source identity, flags expression, parent TEST/history, phase, RSP, flags kind and current boundary. Per-run opaque identities are deliberately excluded. `cp200ExactAuthority` separately authenticates the factory JNE, CMP predecessor, factory CMP, root read, and read root/thread/grant/symbol/frame/call relationships by object and control block. Every callee interruption from 0 through 10 invokes both checks. Seven earlier constructor cuts (flags ordinals 0, 4, 8, 12, 16, 17 and 18) invoke the same checks. Each cut preserves its complete accepted CP199 snapshot through privileged test-only projection, restores the exact issued JNE handle, and verifies stable terminal reentry. The ordinary Session driver also uses the full semantic comparator. This satisfies the requested cross-cut evidence without equating distinct issuance occurrences or creating a production rewind operation.

2. The new twelve-variant authority test retains the accepted JNE and independently exercises foreign/expired replacement root owners, foreign/missing root threads, missing/foreign factory capabilities, a foreign grant in both root and factory, replacement grants with expired thread/capability links, live-grant revocation, replay through a second actual Session/factory, and root-only grant removal. Both observable Session fixtures, retained record/read values and available authority-owner state are compared after repeated refusals. It includes independent grant revocation without adapter or allocation release, so the outer `released` check is no longer the sole evidence for lost authority.

The implementation report accurately qualifies lifetime feasibility. `ConversionSession::widgetRuntime_` strongly owns its root, `WidgetRuntimeState::thread_` strongly owns its thread, and the retained Session binding strongly owns its capability. The new tests exercise supported substitutions/revocation and expired replacement weak links; they do not claim independent destruction of original owners that remain strongly retained. The original post-Session case remains a joint lifecycle expiry test. This ownership argument plus the targeted variants resolves the second blocker under the correction originally requested.

The released-scope test repair also addresses the reported dangling-frame defect correctly: allocation release destroys retained prototypes, so the repaired branch never accesses `fixture.pending`. It snapshots surviving factory/root authority and retained immutable JNE/CMP/read records and checks store inactivity/zero counts across repeated refusals. Factory-only release retains the diagnostic frame and continues using the full frame snapshot. No production refusal guarantee was removed.

The implementation report's earlier statement that earlier constructor-cut coverage relies on existing fixtures describes the initial release; its appended acceptance-coverage section explicitly supersedes that limitation and accurately describes the final source. No further source/test correction is required by this static review. No new concrete value, branch truth, target execution or production API was introduced.

## Verification boundary

The coordinating agent reported that the initialized-environment build compiled and the focused suite passed 177/177 after the dangling-frame test repair, before these final coverage additions. That result is external evidence for the preceding test revision, not an executable PASS for hash `973098b04c3b85bf0a0b8ec8c31c6460cd22d2ed62a00361676165047d8fd1af`. The coordinating lane must compile/run the final revision and required regressions before freezing it. This remaining execution gate does not reverse the static source-review PASS.

Only this report was written in this review and re-review. No source, build, tests, GUI, mutable REA state, parent DMO/Claude tree, or deployment was changed; no tests were executed. Full application recovery and concrete JNE selection remain outside this review.
