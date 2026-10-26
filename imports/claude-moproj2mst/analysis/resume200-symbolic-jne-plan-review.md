# CP200 symbolic TLS-index JNE: independent plan review

**PASS for a first, inert unresolved-successor schema cut; BLOCK for implementing the JNE edge pair with the current scalar-cursor schema.** CP199 is frozen: `build/wave3-checkpoint199-verified/receipt.json` records source tree SHA-256 `9c2ab8a0f82ce299c03b89da8daf0f5a895acc1a74c2b719078564321e51885f` and verified source archive SHA-256 `6b14e5b0a39889d5115dcfe860295bb0d72f48f85305f2a4544afd57cb9085d`. The validation receipt is scoped to CMP-only and has `objective_complete: false` for the broader checkpoint objective; that does not revoke the CP199 freeze. First add and accept an inert schema capable of representing an unresolved branch pair at the current JNE boundary, without issuing a JNE record or changing execution. Then implement the privately issued JNE record against that schema. Do not advance to either successor.

## Evidence and scope

Read `resume200-tls-initialization-order.md`, `resume199-tls-cmp-next.md`, `resume199-tls-cmp-plan-review.md`, the retained aligned initializer listing `wave2-doe-task-qobject-thread-tls-initializer.asm`, CP199 implementation report, `wave3-checkpoint199-validation.json`, and the verified CP199 receipt. CP199's validation reports 361 registered tests, 354 passed, six passed-with-logged-gaps, one passed-with-logged-differences, and zero failed. Its `objective_complete: false` refers to the larger checkpoint objective, not the frozen CMP-only source. The receipt binds this validation to a stable archived source tree. The JNE extension is a new scope and needs its own review/acceptance.

The original QtCore image hash and instruction evidence are retained in the CP199/CP200 reports. The aligned listing shows JNE bytes `0F 85 9F 00 00 00` at RVA `0x2961b`, next RIP `0x29621`, and taken target `0x296c0`. The proposal's two logical outcomes are correct: `ZF` means `x == 0xffffffff`; JNE is taken exactly when `x != 0xffffffff`, to `0x296c0`, and falls through to `0x29621` when equal. Both alternatives must stay guarded by the *same* CMP/read occurrence and retain unresolved feasibility. This is static instruction evidence, not a concrete runtime value or proof either edge has been witnessed.

## Current source boundary and owners

Relevant source is `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`, `source/apps/moproj2mst/src/doe/task_factory.cpp`, `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp`, `source/apps/moproj2mst/src/engine/view_manager.cpp`, and focused tests in `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`.

The existing private source owner is correctly split: `WidgetRuntimeState` owns the symbolic TLS cell/read issuer and grant; `TaskFactoryRuntime` owns the issued CP199 transition and frame. `advanceTlsIndexCompare` commits a full-domain unknown read, a 32-bit CMP result/flag record, and phase `BeforeTlsIndexJne`, then stops. `validateTlsIndexCompare` authenticates the private read and exact transition. Existing focused tests cover symbol/read identity, CMP arithmetic schema, predecessor preservation, terminal reentry and refusal cases. That is the right owner boundary to extend; do not move a concrete index or branch truth into `WidgetThreadState`.

Current `QObjectPrefix::Phase::BeforeTlsIndexJne` maps to cursor `0x2961b`, `currentArithmeticFlags()` returns the CMP flag record, and terminal reentry refuses with “JNE unavailable.” This boundary is safe because it chooses no route. But no distinct record represents the JNE predicate, instruction bytes, taken/fallthrough target pair, predecessor CMP handle, and unresolved edge state. The current `TlsIndexCompare` is a CMP arithmetic record, not a JNE decision receipt. A public cursor or matching addresses cannot authenticate either edge.

## Smallest schema-only cut: PASS

The first cut should be inert: add a private noncopyable `UnresolvedSuccessorPair` (or equivalent discriminated branch-state type) and a distinct `BeforeTlsIndexJne` pending-instruction state that can carry it, while preserving the existing CP199 frozen prefix and current CMP terminal behavior. The schema may encode the static JNE identity/bytes and two guarded target descriptors, but it must not issue/populate a production pair, mutate phase on reaching CP199, or claim either edge was evaluated. Keep the current `0x2961b` pre-execution stop and `JNE unavailable` behavior until the subsequent implementation cut.

For schema-only acceptance, test type constraints (private construction, noncopyable or otherwise unforgeable through public fields), exact immutable descriptor constants and target arithmetic, and that a default/unissued pair has no selected edge/value and cannot validate or advance a frame. Run the existing CP199 acceptance unchanged against the frozen baseline; any new tests should assert this cut adds no production issuance, no cursor change, and no effects. This is the concrete next implementation approval.

## Required JNE record invariant after schema cut

The successor must retain the exact issued CMP/read and CP198 history and add one privately issued immutable JNE-control-flow record. Its invariant should be:

* producer identity is the same QtCore image/evidence plus RVA `0x2961b`, exact six bytes, length, sequential target `0x29621`, and taken target `0x296c0`;
* predicate is `!ZF`, with ZF derived from the exact unknown `x - 0xffffffff` CMP result; equivalently taken iff `x != 0xffffffff`, fallthrough iff `x == 0xffffffff`;
* both edges are present as guarded alternatives, while selected edge, concrete `x`, concrete ZF, and branch execution are absent;
* the record authenticates by object/control-block identity to the same frame, session grant, root-issued read and factory-issued CMP; it cannot be copied, synthesized, replayed for another frame, or consumed after grant/scope expiry;
* the instruction is modeled at the pre-execution boundary. No target instruction, stack/register/flag mutation, helper return, or CMP/read replay occurs. Terminal reentry returns the same edge-pair handle without a fresh read or record.

Use an explicit unresolved-successor/pending-branch schema or equivalent discriminated state. A single phase-to-RIP cursor can remain the instruction address, but it cannot stand in for a selected successor. Keep the CP198 ten-stage validator scoped to its immutable prefix; add a CP200 successor validator and deliberately update phase ranges, current-flags precedence, boundary helpers and refusal snapshots.

## Minimal tests before implementation can pass

1. Actual Session-bound first Report reaches CP199's accepted CMP terminal, then produces exactly one JNE edge-pair record with verified bytes/targets and same read/CMP identity. Assert both guarded alternatives and no selected edge/value.
2. Across direct and every valid interrupted/resumed path, assert edge-pair equality, preserved registers/stack/live returns/CMP flags and predecessor history. Reentry preserves the same handles and counts and performs no second read.
3. Refuse copied, foreign, stale, wrong-CMP, wrong-read, wrong-frame/call-generation, wrong-byte/target, wrong-phase/cursor, malformed predecessor, expired grant/root, and coherent or partial rewind attempts without mutation. Preparation failure must preserve complete root/factory/frame state.
4. Explicitly assert neither `0x29621` nor `0x296c0` is entered, no JNE outcome is evaluated, and no initialization, return, getter, private-field write or publication follows. Predicate arithmetic examples may validate `x==-1` and representative other values but remain test-only and must not become production branch observations.

## Decision and next gate

The exact structural edge pair is justified by the qualified bytes and unknown CMP flags; no new native observation is needed to represent both alternatives. The freeze receipt permits building on CP199. Proceed first with the inert unresolved-successor schema cut above, preserving the frozen CMP-only behavior. After that cut is reviewed and accepted, implement one root/factory-authenticated edge-pair record tied to the same CMP/read, with failure-atomic publication and stable terminal reentry. Concrete JNE selection remains blocked on a qualified value/ZF for this same read occurrence. No source, tests, build, GUI, mutable REA state, parent DMO/Claude tree, or deployment was changed; this report is the only written artifact.

## Review of `resume200-unresolved-schema-design.md`

**PASS for the proposed non-executing unresolved-JNE record and phase, subject to the implementation checks below.** The design correctly appends a distinct phase after CP199's `BeforeTlsIndexJne`, leaves `inCalleePrefix()` and its ten/eleven-entry CP198 ordinal arrays scoped to CP198, and explicitly maps both TLS phases to RSP `-0xa8` and the unexecuted instruction boundary `0x2961b`. The ordered `(ZF set, fallthrough)` and `(ZF clear, taken)` guards match `JNE` semantics and CP199's `ZeroOfResult32` expression; neither target is made the cursor. Retaining the exact CMP predecessor and factory-issued shared handle preserves the existing root/read/grant ownership transitively.

The proposed shared CMP-history validator is implementable if it contains the current CMP checks but no hard-coded phase admission; keep the public/private wrapper responsible for the exact phase. Then CP199 validation can require `BeforeTlsIndexJne`, while CP200 validates the same history under `TlsIndexJneUnresolved`. The prescribed global audit/update of every explicit `inCalleePrefix() || phase == BeforeTlsIndexJne` and every CMP premature-state gate is necessary: current `task_factory.cpp` has these checks in saved-register/backlink validation, branch/helper derivation, and ordinal-19 dispatch. The new phase must be admitted only at the terminal ordinal, with both frame and factory JNE handles present there and both absent everywhere else. This catches phase-only, frame-only, and factory-only rewinds; keep the exact CP199 predecessor checks and private issued anchors in snapshots.

Refusal and rewind handling is adequately specified: the frame record's predecessor must match both the frame CMP and factory CMP by object and control block; the JNE record must likewise match the factory anchor; release clears the new anchor, leaving retained diagnostic records non-authoritative. Tests should cover phase-only, frame-record-only, and CMP-removed/JNE-retained rewinds, plus coherent public frame rewinds. A test friend that deliberately erases every private anchor is privileged fixture reconstruction and is correctly outside the public tampering guarantee. Update the existing CP199 rewind fixture/helper deliberately so it cannot leave a new JNE authority anchor behind.

The commit is failure-atomic for the existing serialized adapter model: allocate the occurrence and immutable record locally, inject preparation failure before publication, then publish only nonthrowing shared-pointer/phase assignments. Do not call it a concurrent transaction. A failure before commit must preserve complete frame/factory/root/read state and issue no second TLS read. An exception while constructing or transporting the terminal `Error` after a successful commit does not roll back the committed phase; the design correctly distinguishes that from preparation failure. Constructing the diagnostic before commit is a useful hardening, but tests and reporting must not promise whole-call rollback once publication has happened.

Implementation acceptance remains conditional on these concrete checks:

1. Update all exact-phase sites found in `task_factory.cpp`, not just the main loop: in particular the premature-CMP guard and helper/backlink/flags predicates must recognize the new phase as a TLS boundary while keeping CP198 ordinal arithmetic unchanged.
2. Snapshot the new frame and factory handles and all record fields; refusal assertions must show no mutation for malformed fields, foreign/copy handles, expired scope, and partial/coherent rewinds. Include `issuedTlsIndexJneUnresolved_` in the pristine/no-capability guard and reset it at adapter-scope release.
3. Inject failure after record allocation but before any publication, assert exact CP199 snapshot preservation/no new read, then retry. Separately assert terminal diagnostic failure semantics if that failure point is testable; do not mistake a post-commit exception for a failed/rolled-back transition.
4. Keep the schema switch default false in this first integration so the frozen CP199 stop remains available; explicitly test both CP199-only and CP200-resumed routes. Verify no edge is selected and no successor instruction runs.

The detailed design includes these requirements; they are implementation audit points, not blockers in its schema. The CP199 receipt is verified and its CMP-only freeze stands despite `objective_complete: false` for the larger recovery objective. No source/test/build was changed in this review.
