# Checkpoint 74: retained scalar-prefix ownership

Retain the prefix's **data**, not its parser driver. One Study-owned shared state can own the concrete ScalarVariablePrefixRecord and pending locals with no reference back to Study, Session or I/O. A stack driver uses that state while the actual Study is bound; its guard unwind leaves the state and Study.vars alias intact. This avoids both a shared Study cycle and deleting a derived record through nonvirtual Var.

This is an implementation-ready design, not source work or native lifecycle completion. Only this report was written; no build, test, GUI, application control or mutable REA was used.

## Ownership graph and minimal types

Current `B121ScalarVariablePrefix` owns a shared Study, shared StudyIo, a concrete unique_ptr<ScalarVariablePrefixRecord> and pending locals. Its destructor removes the Var alias. Storing that driver on Study would cycle; leaving it on the read stack removes the alias during unwind. Existing `Study.scalarPrefixVariableOwners` owns plain Var projections and is not appropriate for the derived native-prefix record because Var has no virtual destructor.

Use this graph instead:

```text
SUB.ownedDoeStudy -> Study
                      -> retained prefix state -> unique_ptr<ScalarVariablePrefixRecord>
                      -> Model.vars ----------- borrowed Var* to that concrete record
stack driver ----------> same retained state
stack driver ----------> actual Study/I/O only for the bounded read call
```

State has no back edge. Its shared ownership is an adapter mechanism for stable interrupted diagnostics, not a native QObject or proof of liveness. No second variable is created by retaining the state.

Suggested declarations:

```cpp
// doe_study.hpp: forward declaration suffices.
class B121ScalarPrefixState;

// Study private storage; friend only the retained-prefix entry/driver.
std::shared_ptr<B121ScalarPrefixState> retainedScalarPrefix_;

// variable_prefix.hpp: complete concrete type is available here.
class B121ScalarPrefixState final {
    // private, mutated only by driver; public const observation accessors.
    std::unique_ptr<ScalarVariablePrefixRecord> variable_;
    std::optional<PendingScalarVariableSignal> pending_;
    std::optional<int> selectedIndex_;
    bool started_ = false;
    // Optional adapter stage enum, explicitly not a native field.
};
```

Expose a `shared_ptr<const B121ScalarPrefixState>` observation accessor on Study if post-release diagnostic retention is needed. State fields stay private: const unique_ptr by itself does not make its pointee const, so expose `const ScalarVariablePrefixRecord*`, const pending/selected accessors, not the owning unique_ptr. An escaped observer may preserve diagnostic data only; it has no resume/dispatch API and no live-owner claim.

The shared_ptr choice is deliberate: Study can contain it while the state is incomplete. It does not require a new out-of-line Study destructor/constructor or pulling internal prefix types into the public Study header. The concrete unique_ptr is constructed/destroyed in the complete state type. No Var virtual destructor, Model layout behavior or unrelated owners need change.

Do not store shared Study, StudyIo, Session, notifier, callback, raw runtime owner or native handle in State. Ordinary scalar variable `link` is null in this admitted prefix; no new foreign pointers are introduced. Pending data is value-only and does not retain XML nodes/stack references.

## Driver refactor and exact entry

Refactor variable/pending/selected/started storage from B121ScalarVariablePrefix into a shared B121ScalarPrefixState. Preserve the current public standalone prefix constructor and accessors for existing tests; that constructor uses detached state and its existing adapter alias cleanup behavior. Do not change the standalone test's native mutation or refusal order merely to share code.

Add a distinct production-ready ownership entry in the same internal prefix module:

```cpp
[[noreturn]] void readOwnedB121ScalarPrefix(
    engine::ConversionSession& session, Project& project,
    SubProject& sub, const xml::Node& doeVariables);
```

It receives actual Session context, rather than dereferencing a saved Project pointer after destruction. It must initially remain an explicitly invoked guarded prefix; wiring readInputTable/readLazyStudy requires root's later shared ownership assignment. A helper/test call is not a successful lazy read.

The entry admits the actual live Project, original owned/active Type2 SUB, exact owned Study/backlink/raw alias, existing same Project StudyIo, actual Session ModelManager owner and already-bound Session notifier using noncreating observation. Check Project/SUB membership before dereferencing saved SUB identity. No new Study/I/O/notifier is made. Refuse foreign/released/replaced owners and incompatible existing scalar/geometry/projection records before installing state.

After nonmutating admission checks, create one state and attach it to this Study **before the first native variable mutation**. Construct a stack driver that uses this state and the actual Study/I/O for the call. Its retained mode must not detach the alias in its destructor; only the standalone detached mode retains that cleanup behavior. Study does not own this driver, so any driver-local strong Study/I/O lease cannot form a cycle.

No retriable parsing policy is inferred: an existing retained state, started attempt or nonempty manager refuses re-entry. State must not be overwritten to retry after a guard, because that would erase already-observed mutations. An admission failure before state attachment leaves no new state or model change. A later allocation/interruption can leave state attached with no variable or a partially constructed prefix; describe it as an interrupted adapter attempt, not native normal completion.

## Mutation and exception order

Retain the existing sourced prefix sequence from CP50:

1. Admit fresh mode0/no existing rows/empty variable state; start the reader.
2. Allocate and construct the **concrete** record. Publish it to State as its owner, then append its borrowed base pointer to Study.vars at the native append phase, before assigning name. Do not publish a raw pointer to an unowned allocation.
3. Assign name, selected index and sample invalid flags in the existing sourced order; then varId/dimension and scalar children.
4. Construct/append the typed keyword, merge operation IDs and populate pending locals.
5. Refuse before unresolved SetOprHasDoe delivery, retaining all preceding state. The record's committed nominalText remains constructor-empty; the collected XML text remains only in pending locals. No selector/parameter/text setter, deferred link resolution, variable reindex or root Operations import has run.

If vector allocation or another host exception occurs between ownership publication and alias append, the state can retain an allocation that was not appended; accessors/tests must distinguish `variable()!=nullptr` from actual `Study.vars` membership. Do not claim native allocation-failure cleanup semantics from this adapter safety choice, and never invent a normal reader return. Once appended, later exceptions leave the alias valid until the Study/state's ordinary adapter teardown.

The initial version should retain the existing unconditional behavioral refusal at the pending notification. It need not dispatch merely to introduce ownership. Future actual dispatcher integration must reuse the existing Session binding, retain partial flag/list/backend effects and stop at its reached unresolved receiver boundary. It must not advance final keyword text just because a diagnostic profile reports return.

## Cleanup without a cycle or stale dereference

Do not put native cleanup in State's destructor. It only destroys its concrete record and value locals. It never locks/dereferences Study, follows sub/project backlinks, emits notifications, rolls back sample flags, reindexes variables, touches files or claims native Close.

While Study is destroyed normally, its retained State member is released before its Model base vector is destroyed. Model.vars is a vector of nonowning pointers whose ordinary destruction does not dereference elements, so no destructor must walk a half-destroyed Study to remove aliases. The same ordering already applies to its plain projection owner vectors. A retained external state observer can keep the record allocated past Study deletion without keeping the Study/Project/Session alive; that is diagnostic allocation retention only.

The smallest slice exposes **no public reset/clear operation** on the retained state. Keep Study's owner private so a client cannot drop it while Model.vars still contains its pointer. If a future adapter-only explicit release is required while Study stays alive, a checked owner method must erase only the exact owned alias before resetting state, and label this adapter retirement, not native variable deletion. Such reset/retirement is unnecessary to establish guard-unwind retention and should not be added as a convenience retry.

Existing standalone driver cleanup remains safe because that external driver still owns the detached concrete state while erasing its alias. Its destructor may use its current strong Study lease for that narrow operation, but must not follow stale Project/SUB pointers. Retained driver mode skips this erase; refactoring must not run both cleanup paths.

## Exclusive source/test ownership proposal

One implementation worker can own this disjoint set:

- `include/m2m/doe_study.hpp`: State forward declaration, private retained owner and const observation accessor/friend entry; no changes to existing plain projection ownership.
- `src/doe/variable_prefix.hpp`: complete concrete State and read-only accessors, driver shared state plus explicit detached/retained mode, internal retained-entry declaration.
- `src/doe/variable_prefix.cpp`: storage refactor, actual Session admission, attach-before-effect sequence, guarded entry and distinct cleanup modes.
- New `tests/doe/test_retained_scalar_prefix.cpp`: real Session owner fixture and retention/cleanup tests.
- Existing `tests/doe/test_variable_prefix.cpp` only if mechanical accessor changes are unavoidable; prefer preserving its interface and tests unchanged.

No shared Session/model destructor/convert/moproj writer/C ABI edit is needed for this isolated retained entry. Root retains CMake/build/shared parser ownership. Linking the entry from actual readInputTable/readLazyStudy is a separate follow-up with assigned `doe_study.cpp`/`activation.cpp` context interfaces; do not silently enable it in this slice. No changes to Var polymorphism or native Report/Task/DOE public guards are part of the owner design.

## Focused tests

1. Actual Session Project, already-bound recording StudyIo/notifier/ModelManager and Type2 SUB/Study. Call retained entry on exact B121 DoeVariables, catch M2M_E_UNSUPPORTED at SetOprHasDoe. After the call stack/driver has unwound, verify State exists, one exact borrowed alias points to its concrete record, selectedIndex0, varId1/dimension3/sampleCount3, all invalid flags set, typed keyword/operations/pending locals retained and final nominalText still empty. Do not invoke Model query helpers that reindex as a side effect before asserting index-1.
2. Save a weak state observation and return from a helper scope to prove the stack driver no longer owns it but Study does. Verify no new file/directory/notification effects beyond the existing prefix and no samples or root Operations finalization.
3. Retry against same Study refuses with identical state/allocation/alias and no duplicated mutations. Wrong/foreign/replaced I/O, Study alias, Session, inactive SUB and nonfresh model refuse before state attachment/effects.
4. Drop the external Study lease, then clear actual SUB/Project ownership. With only weak State observation, it expires: no cycle retains the data. With an explicit shared const state observer, Study's weak owner expires but diagnostic data stays readable. No API permits dispatch/resume on that detached state.
5. Release Project through actual Session storage-release path before a new call and verify entry refuses before dereferencing stale SUB. For the diagnostic observer, release does not execute native Close, table death or notifier callbacks.
6. Preserve existing detached-driver test: its destruction removes its alias while the actual Study remains alive. Retained mode must not remove its alias on guard unwind. Both concrete record destruction paths are typed, with no unique_ptr<Var> adoption.
7. Public readInputTable nonempty guard, public saveInputTable reconciliation guard and Type2 refresh guard remain. Isolated final B121 projection/writer tests stay separate from this incomplete native-prefix record; the latter must not be serialized as a completed scalar row.

Use Testkit active macros/main, resolve new quoted includes and keep any scratch probes under M2M_TEST_SCRATCH. No executable test is needed in this design lane; root runs one grouped preflight after source freeze.

## Qualification boundary

The owner mechanism preserves existing source-qualified early mutations through an adapter guard. It does not qualify native allocator-failure cleanup, QObject ownership, interrupted native destructors, actual SetOprHasDoe receiver effects, scalar setter continuation or a successful XML parse/Save. The first behavioral dependency remains actual notifier population/backend delivery, before collected nominal text becomes committed keyword state.

Sources inspected: current Study owner vectors, Var's nonvirtual destructor shape, B121ScalarVariablePrefix concrete ownership/destructor/read sequence, existing prefix tests and Session project-release boundary. Source hashes for the unchanged relevant prefix/model files are recorded in `analysis/resume74-doe-scalar-study-binding.md`; this follow-up introduces no source patch. Native order remains bound to CP50/51 retained PE evidence for original image SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
