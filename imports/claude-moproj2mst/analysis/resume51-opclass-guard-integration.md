# Integrating the OPI guard into actual OpClass current storage

Replace OpClass's raw current field with one guarded binding; do not maintain raw and guarded fields as competing authorities. Preserve existing pointer reads/assignments through a narrow adapter, and change the three save/restore sites to save guard metadata rather than extract a raw address. Keep native OPI-boundary expiry explicit and accepted Close guarded.

Scope: read-only current source and previously qualified native evidence. This report is the only write; no source/build/GUI/mutable REA action. Native qualification is `resume50-close-destructor-next.md`; isolated guard implementation/limits are `resume51-operation-info-guard.md`.

## Existing caller contract

Current `include/m2m/mst/writer.hpp::OpClass::current` is a public Operation*. The actual source uses these forms:

| Form | Examples | Compatibility requirement |
| --- | --- | --- |
| raw-pointer assignment/null reset | loader.cpp:174,1065,1071,1076; many operation fixtures | Assignment binds that live Operation's one OPI identity; null assignment unbinds. |
| pointer extraction | class_slots.cpp:149; express_sync.cpp:34,61; numerous L5/L6 functions | Implicit/read accessor yields the current live Operation* or null. |
| bool, negation, dereference, arrow | l5_slots.cpp:73; l6_slots.cpp:212-213,346; Cycle::dim | Preserve pointer truthiness and unchecked dereference behavior. Do not add a new throw-on-null policy. |
| pointer comparison | boolean_lifecycle.cpp:19; Boolean/engine tests | Compare the current live target identity to the supplied pointer, including null. |
| conditional expression | base.cpp::propsOf (`op ? op : current`); ht_save.cpp:675 (`c ? c->current : nullptr`) | Wrapper conversion must resolve these to Operation*, without a competing implicit converting constructor. |
| saved temporary binding | master.cpp:109-115; cycle.cpp:152-155; loader.cpp:472-475 | Save/restore the binding metadata, not a possibly dying raw pointer. |

A bounded converter-tree search found no actual OpClass current address-taking, pointer-reference API, pointer arithmetic, swap, pointer-to-member/layout introspection or `auto* x = current` dependence. Matches involving DOE currentPart/currentSession, task runtime identities, ViewManager locals, SaveEffects.current and widget traversal variables are different state and must not be migrated. This lexical audit is not an AST/generated-code proof; root compilation remains required.

There are 42 current-token lines across src/ops, with most broader read use in engine slot files. A wrapper prevents unnecessary mass edits to those readers. No m2m::Operation by-value return/move/container usage was found by the bounded pattern scan, but stable identity remains the rule: manually implemented operation-copy functions must construct a fresh OPI identity, not copy the source identity.

## A safe staged adapter API

Add an internal `CurrentOperationBinding` around the existing `OperationInfoGuard`:

```cpp
class CurrentOperationBinding final {
public:
    CurrentOperationBinding() = default;
    CurrentOperationBinding(const CurrentOperationBinding&) = default;
    CurrentOperationBinding& operator=(const CurrentOperationBinding&) = default;

    CurrentOperationBinding& operator=(Operation*); // out of line: bind op's identity
    CurrentOperationBinding& operator=(std::nullptr_t) noexcept;
    Operation* get() const noexcept;
    operator Operation*() const noexcept;           // existing pointer readers
    explicit operator bool() const noexcept;
    Operation* operator->() const noexcept;
    Operation& operator*() const;                   // same unchecked precondition

    OperationInfoGuard snapshot() const noexcept;
    void restore(const OperationInfoGuard&) noexcept;
private:
    OperationInfoGuard guard_;
};
```

Do NOT add an implicit constructor from Operation*. Assignment is sufficient for detected writers, and avoiding that constructor leaves pointer-vs-binding conditional expressions with one conversion direction. The wrapper's implicit pointer conversion is a compatibility seam, not an invitation to retain raw addresses across callbacks. Dereferencing null remains an unchecked caller precondition, preserving current native-style code. `snapshot()` copies liveness metadata; `restore()` replaces only this binding, without dereferencing the saved Operation. A copied binding is also a valid snapshot, but the explicit methods make the lifetime intent reviewable.

Expose one stable `OperationInfoIdentity` from each Operation through `operationInfoIdentity()` (const/nonconst as needed). Construct it once for that Operation instance; never create a new identity in every current assignment or resolve a freed raw pointer through a global address map. An owned identity declared before the represented members can expire last as adapter fallback; an explicit `commitOperationInfoQObjectBoundary(qualification)` delegates to the identity at the recovered earlier OPI boundary. Identity/Operation copy and move must be deleted unless an audited required use is implemented with fresh identity/backlink repair. Existing copy helpers copy native data fields manually and must retain this separation.

The stable owned identity can be a first-declared direct `OperationInfoIdentity` initialized from *this, or an equivalent first-declared owner with out-of-line Operation construction. Root must check constructor-unwind behavior and standalone link sets. Lazy allocation on every setter is inferior: it can create duplicate native identities and changes same-current rebind semantics. Root's guarded assignment should refuse or retain null for an already-expired OPI according to the chosen adapter contract; it must never resurrect a committed identity.

`OpClass::current` becomes `CurrentOperationBinding current;`. No raw mirror remains. The unchanged source forms above read through `.get()`. Existing old `OperationInfoGuard` has explicit identity binding and no pointer conversions, so it is not by itself a drop-in field replacement; the adapter must be a deliberate additional interface.

## Cycle and the other two saved-current sites

All three should follow this structure at the SAME existing sequence points:

```cpp
const auto saved = h->current.snapshot();
h->current = member;
h->writeMst(...);
h->current.restore(saved);
```

For master.cpp use its actual `op`, `item`, dispatchOperation/dispatchOperationDoe calls. For loader.cpp use `c`, `q`, provideObject. Do not leave `Operation* saved = ...` merely because the wrapper compiles it: restoring a saved raw pointer after its OPI/row expired can dereference freed storage while trying to obtain its identity, or rebind an unrelated object reusing that address. A metadata snapshot restores a null observation if the old OPI has expired and preserves a live old identity otherwise.

Do NOT introduce an RAII restore scope during this migration. The existing code restores only after the call returns; if writeMst/provideObject throws, the restore line is not executed. A scope destructor restoring on exceptions would change adapter error-path state and potentially original failure behavior. Preserve the explicit sequence while separately qualifying any future exception-contract change.

Cycle.cpp's header and line152 explicitly identify its temporary current override as an adapter rule, absent from the original Cycle writer: the original retains whatever the GUI last selected. Guard migration must not make that override look native or silently remove it in the same patch. Keep it qualified and recover the original current-selection behavior separately. The master/loader temporary-binding provenance also needs independent native comparison before changing whether their overrides should exist.

Metadata snapshots improve the intended adapter binding semantics, but do not establish that any hypothetical original destruction/reentrancy during a writer would use the same saved-pointer policy. Such cases remain source-qualified; do not claim a new original Close acceptance case solely from an adapter snapshot test.

## Exact OPI boundary remains separate from row destruction

Native item045cb0 first deletes owned OPI via045dc0, then destroys item diagName+50 and item base. OPI050440 runs050690 cleanup (Props, guarded engine model, object/position cleanup, owned+20/+28, guarded+160), member destructors, and only then QObject base at050531. QtCore clearGuards at6712f8bd nulls matching guard addresses before admitted destroyed delivery at6712f97b.

Therefore:

- Actual Props/model/list/member cleanup callbacks must see current bound until OPI reaches that QObject boundary.
- At that boundary expire only guards bound to this OPI identity; another live Operation's guard remains live.
- Admitted destroyed observers see null, while the outer row may still exist.
- A first-declared identity owner expiring after the whole merged Operation dies is labelled `AdapterIdentityLifetime`; it is later/coarser than native OPI expiry and must not substitute for the explicit commit.
- The existing `commitQObjectBoundary` accepts a receipt string but cannot verify completion of native payloads. Root's staged driver must keep unresolved earlier payloads guarded and must not invoke the commit from the start of Operation destruction.

A storage-only capsule release can now invalidate raw-current exposure through adapter fallback, but accepted Close still requires typed destructor payload/Qt phases and the correct earlier explicit boundary. No broad registry scan/null-reset is justified.

## Root-owned integration files and tests

Suggested exclusive write set for a single root integration lane:

- New `include/m2m/engine/current_operation_binding.hpp` and `src/model/current_operation_binding.cpp`: narrow pointer-compatible binding, no Close coordinator.
- `include/m2m/model.hpp` and, if required, the lightweight model ownership source: one Operation identity plus accessor/explicit boundary seam; copy/move policy.
- `include/m2m/mst/writer.hpp`: replace actual current field type, include the narrow binding header.
- `src/mst/master.cpp`, `src/ops/cycle.cpp`, `src/engine/loader.cpp`: only the three explicit snapshot/restore locals. Preserve calls/exception timing and Cycle adapter qualification.
- New focused tests, plus existing Cycle/P07/current-selection/Boolean/loader tests to catch source compatibility.
- Root-owned CMake standalone linkage: targets bypassing m2m_core may now instantiate Operation identity/CurrentOperationBinding. The new guard/binding definitions must be in their lightweight link set, including standalone p08/p09 configurations if affected. Do not pull the full core into double-based tests to hide unresolved symbols.

Focused tests must exercise the actual OpClass.current field after migration: raw/null assignment, same identity, arrow/bool/comparison/ternary compile forms, two aliases, rebind isolation, explicit boundary expiry while outer row survives, fallback reason after Operation storage death and registry survival across Project release. Add a temporary-binding fixture where the saved old identity expires during the call: explicit restore yields null without accessing its freed row. Add nested Cycle-like calls preserving live old selections. Throw from the inner call and verify the existing no-restore-on-throw sequence is preserved; do not test an unrequested RAII policy.

Run existing P07 current-restoration test (`test_unit.cpp:141-152`), P13 Cycle writer tests, loader inheritance/provideObject tests and Boolean same-current tests. Keep native guard timing and full Close acceptance distinct from source compatibility. No tests were run in this analysis lane.
