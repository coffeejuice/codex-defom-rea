# CP155 implementation plan: actual geometry temporary request0, raw kernel return

Implement one explicitly enabled, candidate-owned temporary allocation for the real CP152 first-scalar receiver, then keep the operation parked. The stop is **immediately after actual HeapAlloc returns**, before claiming that native malloc/operator-new returned, before zero-copy, and before the separate value request8. The returned block belongs only to an out-of-line pending occurrence. It never enters StateVar or CowVector storage.

This is the reviewed bounded experiment, not a production lifecycle allocator or fresh-original parity result. Normal unbound conversions retain CP152's dimensions-only boundary. The canonical role and its one pending occurrence are retained until process exit; destroying a model/frame must neither discard nor free T. The process/module budget is exactly one temporary attempt and at most one live one-byte T. There is no reset/rebind API that replenishes that budget.

## Controlling evidence and current source

Read `analysis/resume154-geometry-bridge-review.md`, SHA-256 `819c3a468e72ced402a3b3297a7e224775fd6cf311f7ae7b43e8228d3fc0cb35`, and the CP153/154 allocator/lifecycle reports. The review explicitly says: “Do not call resize/value_data” and “A successful raw HeapAlloc result is not by itself a claim that the native malloc/operator-new wrapper returned successfully.” This plan adopts that narrower stop; earlier proposed zero-copy or wrapper-return continuations are not part of this increment.

Original DEF_GUI_MO_64 v11.0.2 is pinned by SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained CP119 native producer ASM establishes HeapCreate(0,4096,0) at`0x141484812`, publication to source heap slot`0x144700810` at`0x141484818` even for a null return, test at481f, then only for nonnull the compatibility call(class0,DWORD2,length4) at483c. Its Boolean is ignored; native producer returns1 afterward. Malloc `0x1414748b4..0x1414748bc` normalizes request0 to kernel length1, reads the current heap slot at48c0 and calls HeapAlloc(heap,0,1) at48f1. Native temp callsite is`0x1409e47e4`; the separate value request8 is`0x140832a1d`. The candidate does not read or write original preferred addresses.

Current exclusive source boundary remains Viewer `8777ad9d05f6f357f34f82e18b1fa366d7cecf057f603a89b6858052d5c09669` and geometry test `0407258d80d8924a2becc6e2a79d4943c049e830d22d036413c6fbb8d5e8eff5`. Source/ABI layouts, host-owned CowVector, FeatureCrtRuntime and its tests remain unchanged by this plan. Only this report was written; no source/build/test/GUI/original execution or mutable REA operation occurred.

## Exact new module-role owner

Add a lower-layer `def::rt::ModuleHeapRole` in new defrt files. The class is final, privately constructed and noncopyable. Its canonical factory prepares one owner in the module containing this implementation; it does not accept a HANDLE, module address, report or success flag. Pin/identify that containing module using the implementation's own function address and record actual process/module identity. This is host qualification for the reconstructed role, not a recovered native startup action. A role in a different test executable is a different process/module owner.

Proposed narrow interface (names may change only as a coordinated edit):

```cpp
class ModuleHeapRole final {
public:
    static std::shared_ptr<ModuleHeapRole> prepareCurrentModule();
    HeapRoleProducerReport attemptProducer() noexcept;
    HeapRoleProducerReport producerReport() const noexcept;
    bool matchesRetainedLease(const std::shared_ptr<ModuleHeapRole>&) const noexcept;
    Temp0PrepareResult prepareTemporaryZero(const std::shared_ptr<ModuleHeapRole>&);
};

class TemporaryZeroCall final {
public:
    Temp0Report attempt(const std::shared_ptr<ModuleHeapRole>&) noexcept;
    Temp0Report report() const noexcept;
};
```

`prepareCurrentModule` allocates all owner/report/retention backing before any native heap producer call. The canonical lease is process retained, with no static destructor that calls HeapDestroy. `attemptProducer` is one-shot: execute exactly one real HeapCreate; publish its actual return to the candidate role slot; on null retain failure and stop before native startup fatal handling. On nonnull call real HeapSetInformation with the native arguments, retain raw BOOL, and mark producer returned regardless of that Boolean. No GetProcessHeap/host malloc/Feature heap fallback or per-model heap is allowed. A missing module scope refuses before HeapCreate. Host preparation failure is a bookkeeping failure before source effects, not native OOM.

`prepareTemporaryZero` reserves the single process/module occurrence and allocates its fixed report/storage before an allocation effect. It checks both pointer and shared_ptr control-block identity of the canonical role; aliasing/foreign-control-block leases refuse. It has no size argument: requested0 and normalized1 are fixed, and request8 is unavailable. Retain the call under the canonical role so dropping every external lease cannot erase the attempt/block. A call retains the owner weakly or by a safe noncyclic relationship; the owner itself is process retained. No public constructor accepts a fabricated receipt.

`attempt` validates the retained owner/producer, reads the **current candidate role slot at this occurrence**, tests its liveness/provenance, consumes a started latch, then performs exactly HeapAlloc(currentHeap,0,1). It records the actual raw returned pointer as an opaque uintptr_t token plus a private retained allocation pointer. It returns an immutable report without executing the native next test/branch/wrapper return. On null it stops before new-mode at`0x1414748ff`. On nonnull it stops at the same raw kernel-return boundary. Neither result invokes handler, errno, DecodePointer, free, HeapDestroy or std::bad_alloc. A null slot takes an explicit unavailable guard before the unqualified native null-heap fatal path; it is not a native allocationnull result.

Use states such as `Prepared`, `HeapSlotObserved`, `HeapAllocEntered`, `HeapAllocReturnedNullPending`, `HeapAllocReturnedNonNullPending`, with a started latch that cannot be cleared. Reports separately expose requestedBytes0, kernelBytes1, flags0, source caller/site, actual module/process/thread/occurrence identity, heap-slot read count, HeapAlloc count and raw return token. `mallocReturned`, `operatorNewReturned`, `zeroCopyEntered`, `sourceTemporaryFreeEntered`, `valueAllocationEntered`, `wholeLeafReturned` remain false. Reading a report never follows an old pointer or performs a heap query.

The owner factory can be reacquired for observation; it cannot manufacture a second occurrence. Preserve the first receipt on foreign/repeated calls. The fixed one-call capacity is a disclosed experiment bound, not a recovered application limit. It prevents repeated model creation from accumulating pending T blocks while full conversion is still unsupported.

## Bind to the actual GuiModel and current frame

Add an app-private `GeometryTempPrefixBinding` in `src/engine/geometry_temp_prefix.hpp/.cpp`. Its explicit preparation function takes the actual `GuiModel&`, before its first pending geometry query; there is no public C ABI or environment flag. Use `GuiModelAccess::withStep` to reject an already parked model and capture its actual nonnull step/DB/reader relationship. Allocate the binding, fixed occurrence metadata and lower call reservation before later HeapAlloc. Establish the one module owner/producer in this explicit preparation phase, before entering the geometry reader. Qualify this as candidate role setup; do not claim native CRT startup occurred inside the geometry leaf.

The app layer retains exactly one prepared binding per module. It contains `GuiModelGuard` (weak), current prepared step/DB identities, canonical role lease and the reserved lower call. Repeated preparation for the same still-live model returns its existing binding/report; another model, changed step/DB, foreign role or expired guard is refused without replacing the first binding. Producer/preparation failure stays recorded. There is no rebind/reset API after preparation. The role resource remains module scoped even though this first bounded call is assigned to one model.

Do not add an owning pointer to `GeometryMainRecipientProgress`: its existing trivially-copyable/nothrow static assertions must remain valid. Add only an enum stage/occurrence token there. `GeometrySyncBarrier` can hold the shared binding (it already contains owning adapter fields), while the module's retained app record owns it independently. The binding must not retain the model, DB, step, receiver or frame strongly; keep their guards/addresses as observations. In particular, retaining the barrier itself forever is unnecessary. No `engine.hpp` field/friend change is needed.

Only the existing `GeometrySyncAccess` defined in viewer.cpp can construct/consume a sealed frame-evidence token for the binding. Make token construction/private effect entry friend-only; tests must not submit raw frame/receiver pointers as authorization. Inside the original initial-high borrow, the Viewer first proves the current installed frame/control block, model/step, directory/reader owner, actual current object/table ordinal, source A/primary/six BC owners, registry body/order and first actual typed receiver. The app bridge verifies that this is its prepared model/step; the lower role never interprets app pointers.

The sealed one-shot evidence binds actual frame identity/generation, recipient ordinal, actual object/node, id1 receiver, registry/value body, u30 slot and original setter observations to this reserved call. Seal it only after the final current recheck and immediately before attempting HeapAlloc. Do not expose a generic `allocateFor(void*)` hook or accept a declared-success report. The current COW body remains host-owned and receives no allocator registration.

## Viewer changes in exact order

1. Preserve CP152 admission: initial high installed frame, one started main recipient, canonical ordinal0/id1 normal receiver, D1=1/all other ordinary widths0, cur0=0/cur2=1, unique registry/value body, all value pointers and actual bound slot null, valid real source/primary/six-BC/node-n1 and actual ownership. Keep the actual mutable lookup0 and its existing after-lookup recheck.
2. Perform the existing real `set_dim0(1)` and `set_dim1(D[actual id])` once. Retain `ScalarDimensionsCommitted` even if there is no enabled binding, producer failed, metadata is unavailable, or later allocation admission fails. Do not rerun the virtual leaf to enter this new cut.
3. If this model has an explicit prepared binding, perform a **post-setter** current-owner/body/slot/dimension check. Existing `geometryMainDimensionsMatch(...,completed)` encodes zero-width dim1=0; do not call it with completed1 for this path. Add a narrow scalar check: first cur=(1,1,1); prev0/prev1 equal captured pre-setter current0/current1; cur2/prev2 unchanged; other27 receivers' current/previous dimensions unchanged; all actual receiver/registry/body/type/id/slot/refcount/capacity facts still match. Use `geometryMainSizingOwnerMatches(...,ScalarDimensionsCommitted,true)` plus the scalar-specific dimension/body check, or equivalent factoring that preserves those facts.
4. Prepare/attach the already-reserved fixed call identity to the current barrier, seal current evidence, consume the attempt latch, and execute the lower request0/HeapAlloc1 occurrence. No new callback is needed between final admission and kernel entry. Host receipt preparation failure preserves setters and parks before HeapAlloc; it cannot be converted to an original allocation failure.
5. Record distinct recipient stages for raw-null versus raw-nonnull HeapAlloc pending (or a shared raw-return stage with an explicit outcome). Keep completed leaves0. Values first/last/end, body identity/refcount, count and u30 slot stay empty/null. Primary coordinates and all reached BC effects remain as CP152; another receiver, main copy, Stop, frame retirement and further getter calls remain blocked.

Do not call `StateVarBase::resize_values`, `value_data`, unrestricted virtual sizing or CowVector.resize. This reviewed cut also does **not** call memcpy0 or claim the original resize stack's T publication. Its source request0 is justified by the actual empty-capacity profile, not null-pointer subtraction through candidate `values.size()`. The direct candidate kernel operation reconstructs this bounded allocation shape; omitted wrapper/accessor transitions remain explicitly incomplete.

The current public barrier already rejects parked refresh/reentry. Keep that behavior. A repeated inspection/refresh after D repair, body repair, Viewer replacement, model version lowering, or lease reacquisition must stop before dereferencing saved receiver/frame tokens, repeating lookup/setters or attempting another allocation. A previously unbound parked CP152 model cannot be enabled later to bypass that rule.

## T retention and teardown

At most one actual T exists per module, owned by the lower process-retained call and reachable through its canonical owner. The model/barrier only observes that call. The app's process-retained binding has a weak model guard, so normal model/step/StateVar destruction can proceed using the unchanged host allocator. T is never placed in a native field, body, slot, host unique_ptr with a freeing deleter, or an RAII buffer.

When the frame/model dies, its address tokens become opaque historical identities. The owner/call/T remain retained, with no HeapFree, HeapDestroy, replay or native cleanup/Close claim. Do not inspect body/receiver/DB through those historical tokens. The retained one-byte block and private heap are deliberate bounded incomplete resources until process exit; OS process reclamation is not proof of the native module's heap-teardown path. A future implementation must explicitly consume the same pending occurrence with qualified continuation/free semantics; this plan provides no such API.

Do not add a teardown hook that opportunistically frees T, retire its allocation receipt as freed, or clear the process budget. Null-return pending has no T but consumes the same one-shot occurrence. Failed producer/preparation states also do not authorize substitute heaps or retries.

## Exclusive write set for the implementation lane

Reserve these files together; no other source owner should edit their shared interfaces during integration:

| File | Change |
| --- | --- |
| New `source/port/defrt/include/defrt/module_heap_role.hpp` | typed canonical role, fixed request0 call, origin-qualified reports |
| New `source/port/defrt/src/module_heap_role.cpp` | actual Win32 producer/one-shot HeapAlloc, bounded process retention; Windows-only implementation with explicit unavailable behavior elsewhere |
| New `source/apps/moproj2mst/src/engine/geometry_temp_prefix.hpp` | app-private preparation/report API and sealed GeometrySyncAccess evidence |
| New `source/apps/moproj2mst/src/engine/geometry_temp_prefix.cpp` | actual model guard/binding, one retained record, lower-role ownership |
| Existing `source/apps/moproj2mst/src/engine/viewer.cpp` | exact enabled post-setter integration and pending stages |
| Existing `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | real fixture integration, scenario children and retained CP152 expectations |
| New `source/apps/moproj2mst/tests/model/test_geometry_temp_module_heap.cpp` | separate-process canonical module owner and declared failure/report checks |
| New implementation receipt | exact source hashes, qualifications and focused target list |

Do not edit CowVector/StateVar/FeatureCrtRuntime, public engine.hpp, ABI headers or broad conversion setup. CMake already uses CONFIGURE_DEPENDS globs for defrt/src, app/src and app/tests: no CMake edit is needed for these filenames. Reconfigure the independent converter project to discover the new files/target. Use explicit Win32 wide APIs and appropriate local Windows include hygiene in the new CPP rather than changing shared build flags.

## Focused tests and integration checks

Keep new real owner state from contaminating existing unbound tests. The one-call process bound cannot be reset for convenience. In the existing geometry test executable, add one Testkit parent case that launches a finite list of **hidden child test processes** of the same frozen executable with a private scenario argument. Child main selects only that registered Testkit case and calls `testkit::run_all`; it executes the selected scenario without spawning again. Existing ordinary main behavior still runs all existing cases. Each child gets one canonical role and at most one T, inherits the existing scratch environment, and exits after checks. Do not modify the Testkit framework, original programs, or registered comparator expectations. This allows versions6000/11000 and independent negative cases without an unbounded/resettable production registry.

Required integrated scenarios:

- Actual enabled valid first scalar at6000 and11000: one lookup, native setters/previous values, actual HeapCreate arguments/slot/compatibility receipt and actual HeapAlloc(current role,0,1) return. REQUIRE actual nonnull in this positive scenario; an environmental null is a recorded pending result and fails the positive prerequisite. Check requested0 distinct from normalized1, exact raw-return stage, preserved body/slot/refcount/dimensions/other27 receivers/primary+BC, no value8/free/copy/Stop/whole-leaf completion.
- Repaired D/version/Viewer and repeated refresh: counters/tokens stable, no extra lookup/setters/kernel call. Dropping all inspection leases then reacquiring the canonical role preserves the same pending T. Destroy the candidate model/frame, then inspect only owner/call reports: model guard expired, T token still retained, no free/destroy and no stale-pointer dereference. A second model/preparation cannot steal/rebind that record or replenish capacity.
- Enabled lookup mutation cases before setters: D1, object owner, u30 binding, cur2 and shared body (existing five modes) keep real lookup effects, refuse allocation, preserve the correct CP152 guard, and remain sticky after repair. Add foreign/aliased runtime-control-block and preexisting parked-model preparation refusals without executing a kernel allocation. Original unbound CP152 tests remain unchanged.

Separate lower-unit declared models cover HeapCreate null, compatibility BOOL0 still-live producer, null current slot before unknown fatal work, actual HeapAlloc rawnull ->pending-before-new-mode, nonnull ->rawreturn-pending, one-shot capacity and no effects beyond the fixed ledger. A declared model has a distinct origin and no actual HANDLE/pointer/model authority. Do not inject a fake null return into an ActualWin32 receipt, force OOM or supply invalid handles. The real module-owner test executable can use its one permitted actual allocation independently of the geometry executable. Declared transition tests supplement actual effects; they do not prove original failure behavior.

Focused targets are `m2m_mdt_geometry_a_producer` and the auto-discovered `m2m_model_geometry_temp_module_heap`; exact CTests `m2m.mdt.geometry_a_producer` and `m2m.model.geometry_temp_module_heap`. Before release verify resolvable includes, Testkit entry point/child filtering, CHECK/REQUIRE macros, no C assert/REQUIRE_EQ, retained progress triviality and no source call to HeapFree/HeapDestroy/value_data/resize in the new prefix. Root runs the focused group, then required full source-stable validation and freeze; no build is authorized to race source owners.

Fresh original same-run heap/receiver/allocation/free validation remains pending CP153's recipe. A passing candidate test proves its actual candidate kernel occurrence and retained boundary only. Full lifecycle payload/body provenance, synchronous failed-free errno/FLS/PTD handling, native allocation failure/exception behavior and original heap termination are still required before any value8 publication or completed Open-to-Save claim.
