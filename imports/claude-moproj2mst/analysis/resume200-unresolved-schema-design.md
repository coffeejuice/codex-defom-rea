# CP200 inert unresolved TLS JNE schema

CP200 can record the qualified JNE alternatives without executing the branch. Keep CP199's exact issued CMP/read and append one immutable factory-issued record. The new phase means **JNE remains unexecuted and no successor is selected**. Neither target becomes the execution cursor.

This is an implementation design, not source implementation, build verification, native execution evidence, or authorization to select an edge. Only this report was written.

## Baseline and qualifications

CP199 is frozen at `build/wave3-checkpoint199-verified/receipt.json`. Its source-tree SHA-256 is `9c2ab8a0f82ce299c03b89da8daf0f5a895acc1a74c2b719078564321e51885f`; the receipt identifies the verified source archive and built artifacts. `objective_complete=false` concerns the full application objective and does not negate that checkpoint freeze. The freeze objection in `resume200-symbolic-jne-plan-review.md` and the under-validation wording in `resume200-tls-initialization-order.md` are stale. Their unknown-value and scalar-cursor objections still apply.

Source inspected: `doe_task_runtime.hpp`, `doe_task_factory.hpp`, `task_factory.cpp`, the TLS issuer portion of `engine/view_manager.hpp` and `.cpp`, and CP199 fixtures/snapshots in `tests/mdt/test_doe_task_factory.cpp`. The two CP200 reports qualify the selected original instruction bytes; no new original-image or execution evidence was collected here.

JNE is QtCore RVA `0x2961b`, bytes `0f 85 9f 00 00 00`, length 6. Sequential address is `0x29621`; taken target is `0x296c0`. Its predicate is `!ZF`, using CP199's same `TlsIndexCompare::arithmetic[3] == ZeroOfResult32`. CP199 defines `r=(x-0xffffffff) mod 2^32`, so taken means `x!=0xffffffff`, fallthrough means `x==0xffffffff`. These are alternatives with unresolved runtime feasibility, not witnessed outcomes.

The existing root-owned symbol/read chain authenticates the original DLL/evidence identity, root, thread, grant, capability, first Report, frame, private object, and generation-two/three calls. Reuse it transitively through the exact CMP; do not issue a second read, duplicate a grant, or add a thread-local index.

## Exact proposed representation

Append `TlsIndexJneUnresolved` after `BeforeTlsIndexJne` in `QObjectPrefix::Phase`. Do not insert it into the CP198 arithmetic ordinal range.

Add this nested value record after `TlsIndexCompare` (the following is a proposed declaration, not edited source):

```cpp
struct TlsIndexJneUnresolved {
    enum class Status { UnresolvedBeforeExecution };
    enum class Guard { ZeroFlagSet, ZeroFlagClear };
    struct Edge {
        Guard guard;
        std::uint32_t targetRva;
        friend bool operator==(const Edge&, const Edge&) = default;
    };
    std::shared_ptr<const TlsIndexCompare> predecessor;
    std::string_view moduleSha256 = QObjectPrefix::moduleSha256;
    std::uint32_t producerRva = 0x2961b;
    std::array<std::uint8_t, 6> bytes{0x0f,0x85,0x9f,0x00,0x00,0x00};
    std::uint32_t instructionLength = 6;
    std::uint32_t flagsIndex = 3; // CP199 ZeroOfResult32
    Status status = Status::UnresolvedBeforeExecution;
    // Fixed order: sequential/fallthrough, then taken.
    std::array<Edge, 2> edges{{
        {Guard::ZeroFlagSet,0x29621},
        {Guard::ZeroFlagClear,0x296c0}}};
    SourceOccurrence occurrence;
};
std::shared_ptr<const TlsIndexJneUnresolved> tlsIndexJneUnresolved;
```

No selected-edge, concrete-value, concrete-ZF, branch-executed, constraint-solving, or native-loaded-base field exists. `flagsIndex=3` plus exact CMP validation supplies both guards' producer; `ZeroFlagClear` is the JNE predicate. Do not add a second equality expression disconnected from CMP. `instructionLength` and module hash are inspectable qualification fields, not runtime measurements.

Use the existing CP198/199 issuance convention: the factory constructs a value locally and publishes `shared_ptr<const ...>`, retaining its own independent exact handle. Public construction/copying of a lookalike is harmless only because validation rejects every different object or ownership control block. This is issuance authority, not secrecy of a public type. No new private constructor or engine API is needed for the smallest change.

Add private factory members/declarations in `doe_task_factory.hpp`:

```cpp
void advanceTlsIndexJneUnresolved(PendingTaskPrototype::QObjectPrefix&);
void validateTlsIndexCompareHistory(const PendingTaskPrototype::QObjectPrefix&) const;
void validateTlsIndexJneUnresolved(const PendingTaskPrototype::QObjectPrefix&) const;
std::shared_ptr<const PendingTaskPrototype::QObjectPrefix::TlsIndexJneUnresolved>
    issuedTlsIndexJneUnresolved_;
bool tlsIndexJneSchemaEnabled_ = false;
bool failTlsJnePreparation_ = false;
```

The default-false switch preserves the frozen CP199 production stop while the schema is introduced and reviewed. Test access can enable the inert record. A subsequent explicitly adopted CP200 integration may change that default; it must retain a test path that stops at CP199. The switch never selects a branch.

Keep `inCalleePrefix()` unchanged. Add `atTlsIndexBoundary()` returning true for exactly `BeforeTlsIndexJne` and `TlsIndexJneUnresolved`, and `inCalleeOrTlsIndexBoundary()` as `inCalleePrefix() || atTlsIndexBoundary()`. Replace every existing explicit `inCalleePrefix() || phase == BeforeTlsIndexJne` expression with the latter. This covers the saved-register/null-parent/backlink and outer-flags-loop checks currently spread through `task_factory.cpp`, rather than relying on enum comparisons accidentally accepting the new phase.

For both TLS phases, `calleeRspFromEntryS()` returns `-0xa8`; never index the eleven-entry CP198 array with the appended phase. `currentArithmeticFlags()` stays CMP-first: no CP200 flags record is introduced. `activeReturnSlots()` and `liveReturnSlot()` remain unchanged: generation two at `[S]` and generation three at `[S-0x70]` remain live.

`returnBoundaryRva()` returns `0x2961b` for both phases, explicitly documented as the unexecuted instruction boundary, never a selected continuation. The distinct phase and paired record are mandatory; an address alone cannot establish unresolved flow. Neither `0x29621` nor `0x296c0` may be returned there. If a consumer needs a selected-successor query, its CP200 answer must be `nullopt`; do not introduce such an API solely for this change.

## Validators and dispatch order

Split the current CP199 validator into a phase wrapper and a shared body. `validateTlsIndexCompare(frame)` retains its exact `BeforeTlsIndexJne` requirement, then calls `validateTlsIndexCompareHistory(frame)`. The shared body contains the existing `validateCalleePrefixHistory(frame,10)` and every current CMP/read/ownership check unchanged. It is private and must never act as an entry authorization on its own.

Entry through `advancePendingQObjectPrefix` must retain the current validation of adapter scope, allocations, Session capability/binding, sparse fields, saved registers, calls, and public/private backlinks before dispatching CP200. In particular, existing `validateSessionReportNullCaller()` authenticates the live grant before dereferencing a TLS context. Do not move JNE publication above those checks.

Update the current premature-CMP test near line 734 to admit exactly both TLS phases. Add an adjacent JNE-state rule: outside `TlsIndexJneUnresolved`, neither frame nor factory JNE handle may exist; in that phase both must exist. This blocks phase-only and handle-only rewinds. Also include the new factory handle in the pristine/no-Session-capability refusal near line 162.

At the outer flags-loop terminal (ordinal 19):

1. For `TlsIndexJneUnresolved`, call `validateTlsIndexJneUnresolved`, then report the CP200 terminal without publishing anything.
2. For `BeforeTlsIndexJne`, call the CP199 phase wrapper. With schema disabled, preserve the existing CP199 stop. With it enabled, call the JNE preparation/publication routine.
3. Other valid callee phases use the existing CP198 advance path.

The CP200 validator checks, in order: exact new phase and both handles; shared CP199 history; nonempty JNE occurrence; predecessor object and control-block equality to both `frame.tlsIndexCompare` and `issuedTlsIndexCompare_`; module, RVA, six bytes, length, flags index, status, ordered guards and targets against literal expected values; finally exact JNE object and control-block equality to `issuedTlsIndexJneUnresolved_`. As throughout CP199, equal pointers alone are insufficient: compare both `owner_before` directions as well.

All root/grant/thread/frame/context expiry checks remain those of CP199 and the existing caller validator. The record may retain CMP after adapter release, but it cannot make that expired chain acceptable. `releaseAdapterScope()` resets the new factory issuance anchor along with the current CMP/grant anchors; it must not clear the diagnostic frame or invent native cleanup.

## Atomic publication and terminal behavior

The advance routine first requires the CP199 phase, calls the same entry/caller/CMP validation, and requires both JNE handles empty. Construct the complete local record with `predecessor=frame.tlsIndexCompare`; allocate its shared object and occurrence before changing any owner. Put `failTlsJnePreparation_` immediately before publication, after allocation, so the failure test exercises a fully prepared unpublished record.

Publication is only these nonthrowing assignments, in sequence:

```cpp
issuedTlsIndexJneUnresolved_ = prepared;
frame.tlsIndexJneUnresolved = std::move(prepared);
frame.phase = Prefix::Phase::TlsIndexJneUnresolved;
```

No root commit is needed. There is no new read, flags change, stack/register/private/shared write, live-return consumption, edge evaluation, or target instruction. This is exception-atomic in the existing serialized adapter model; do not describe the three assignments as a concurrent transaction.

The CP200 stop should say `DOE Task factory CP200 stops before QtCore RVA 0x2961b (unresolved JNE alternatives; no edge selected)`. Terminal reentry validates and reports that same boundary with exactly the same handles. Constructing an `Error` diagnostic after a successful commit can itself allocate; that does not roll back a committed boundary. Prefer preparing the terminal diagnostic before publication, and treat any exception transport failure separately from record-preparation failure. Do not promise whole-call rollback after successful earlier checkpoints.

The smallest integration leaves CP199's existing throw after CMP publication intact: the next resumed call can publish CP200. Thus direct execution means the normal sequence of factory calls through supported stops, and resumed execution means the same sequence with earlier interruptions. A failure preparing CP200 preserves the accepted CP199 snapshot; it need not undo CP199 or earlier committed prefixes from the same overall operation. No CP198 loop should index its `stops[k]` array using the new phase.

## Tests required when implementing

Extend existing `sourceState`, `frameValues`, `Cp180Snapshot`, and new JNE-value snapshots to include the frame/factory JNE handles, occurrence, predecessor ownership, every qualification field, phase, and current boundary. Keep root symbol/grant/read snapshots complete. Test-control flags are setup controls rather than native state, but compare them where testing failure preservation.

1. Starting from the real Session-bound CP199 fixture, enable the schema and resume. Assert the exact record above, same CMP/read/root/grant/symbol and predecessor handles, full unknown dword domain, both guards, no selected/value API, current CMP flags, boundary `0x2961b`, RSP `-0xa8`, unchanged generations two/three and all saved/history/private/shared fields. Compile-time concepts should establish absence of `selectedEdge`, `concreteValue`, and `concreteZf` fields.
2. Compare uninterrupted-to-CP199 plus CP200 against every currently valid interrupted/resumed callee prefix and representative earlier retained constructor cuts. Compare semantic records across independent fixtures, since distinct runs intentionally have distinct issuance occurrences. Within one run, require exact handle/control-block identity. Repeat terminal entry and require zero new read/record issuance.
3. Parameterize malformed fields: producer/module/bytes/length/flags index/status, swapped/missing guards or targets, copied/lookalike record, same-address foreign control block, foreign CMP/read/root/frame/call generation, missing or replaced predecessor and occurrence, wrong phase or RSP, malformed CP198 history. Compare the complete pre-refusal snapshot after every refusal.
4. Exercise live-grant release, revoked or replaced root read/grant, expired root/thread/capability/allocation scope, and reuse against a second Session/factory. Retaining diagnostic shared pointers must not restore authority.
5. Exercise partial rewinds (phase only; frame JNE handle only; CMP removed with JNE retained) and coherent public-frame rewinds to CP199/CP198. The private issued JNE/CMP/root-read anchors must reject them. Do not claim protection against the test friend deliberately clearing every private authority anchor: that privileged operation can reconstruct setup by design and is outside the public tampering claim. Update CP199's `restoreCmpPredecessor` test helper deliberately for the new state, never silently make it a production rewind path.
6. Trigger preparation failure after local allocation from a valid CP199 predecessor. Require exact root/factory/frame snapshot preservation, no published JNE occurrence and no extra TLS read. Disable failure and retry successfully. Test both an already-retained CP199 frame and the ordinary driver sequence reaching it.
7. Assert no entry of either target, no once-state read, `TlsAlloc`, getter, epilogue, POP/RET, QObject completion, SessionTask construction, cache/list publication, or new branch truth. Test-only predicate examples `x=0xffffffff`, `0`, and `0xfffffffe` may check guard interpretation; they must never populate the production record or issue a branch witness.

Run the focused task-factory tests and checkpoint-required regression suite only during the later implementation task. This report ran no tests or build.

## File ownership and scope

| File | Future owner/change |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | Paired record, appended phase, boundary/range helpers and comments |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | Factory issuance anchor, shared history validator, JNE validator/advance and diagnostic switches |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | Ordered validation/dispatch, local preparation/publication, explicit phase predicates, release reset |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | Test access, full snapshots, new schema/refusal/resumption/failure tests |
| `source/apps/moproj2mst/include/m2m/engine/view_manager.hpp`, `src/engine/view_manager.cpp` | No production change; retain CP199 root issuer/read/grant unchanged |

No CMake, evidence byte file, engine TLS owner, public observation ledger, GUI, native module, mutable REA, parent DMO/Claude, build artifact, or deployment change is needed for this schema. A concrete branch decision remains a separate increment requiring a qualified witness for CP199's same CMP/read occurrence.
