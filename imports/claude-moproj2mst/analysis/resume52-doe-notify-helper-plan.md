# Isolated SetOprHasDoe notifier prefix

Build a source-qualified dispatcher prefix that uses the **existing shared notifier owner and actual ordered flow-chart receiver bindings**. It may finish suppression, expired-receiver, null-backend and missing-logical-item branches. It must stop before QIcon allocation for a matched live backend (`hasDoe=true`), or before the backend icon setter (`hasDoe=false`). Passing these bounded branches is not permission to enable public active DOE parsing without the actual binding.

This is a design only. No source, test, build, CMake, shared plan, GUI or mutable REA state was changed. The controlling native evidence is `analysis/resume51-doe-variable-notify.md` and its hashed captures/receipt; the current B121 scalar prefix remains stopped at notification and `readInputTable` remains guarded.

## Proposed files and shared seam

After the checkpoint freeze signal, assign one lane these **new files only**:

- `source/apps/moproj2mst/src/doe/variable_notify.hpp`
- `source/apps/moproj2mst/src/doe/variable_notify.cpp`
- `source/apps/moproj2mst/tests/doe/test_variable_notify.cpp`

The parent owns a minimal shared-header hook in `include/m2m/doe_session_notifications.hpp`: forward-declare `SetOprHasDoeBindings` and add `std::shared_ptr<SetOprHasDoeBindings> setOprHasDoeBindings` to `SessionNotificationState`. Its null default means **unavailable**, not an empty receiver list. No `StudyIo`, public ABI or CMake change is required. Existing source/test globs discover the files when the parent reconfigures/builds.

Do not change `variable_prefix.cpp` or public activation/readInputTable in this isolated increment. A later, separately reviewed continuation can call this helper and only then re-query the keyword before the recovered scalar setters. The current prefix owner must continue retaining its published variable through any notifier refusal.

## Concrete internal contract

Use this entry point, preserving the native argument order:

```cpp
void dispatchSetOprHasDoePrefix(StudyIo& io, int operationId, bool hasDoe);
```

The original call takes **int first, bool second**. It constructs bool before int, but final R8/R9 setup reverses that construction order. The function returns normally only when the selected bounded dispatcher path has returned. It throws `Error(M2M_E_UNSUPPORTED, ...)` at the first unresolved boundary; it never converts an unresolved operation into native Boolean false or full parse success. Do not add a positive-ID filter here: the dispatcher forwards the signed int unchanged. The B121 parser's narrower valid-data guard remains separate.

`SetOprHasDoeBindings` is the typed projection of this one key on the same `SessionNotificationState`, with:

- a weak reference to that exact shared notifier owner;
- an explicit notifier-owner qualification/binding supplied by the actual context;
- an **optional qualified ordered list**; missing population remains unavailable;
- the list's `mapKeyPresent` fact and ordered registrations;
- diagnostic attempt records, retaining partial progress on refusal.

Each registration retains a weak receiver and the normalized bare method name. A receiver is a lifecycle adapter for one **actual** `SubProject::flow`, with access to its native QObject-alive state and a three-state backend witness: unknown / known null / actual non-null backend identity. It exposes the actual model by reference or pointer; never copy its logical items into a fake model for dispatch.

The receiver binding must validate the actual sub-project/model identity against its owning live project/context. An extra diagnostic shared lease does not establish that its native QObject is alive. Follow the existing `ViewManagerRegistration`/`TaskFactoryRegistration` convention: weak lease resolution plus `nativeReceiverAlive()` checks. The backend witness defaults to **unknown**, not null. A textual qualification alone does not establish ownership, current population or native completion.

The actual factory/lifecycle owner supplies and retains these adapters and appends registrations in native constructor order. Do not fabricate the list by selecting only the active sub-project, by scanning current operation IDs, or by assuming every modern `FlowChart` implies a null native backend. Native constructors register every flow-chart model; the copy-constructor registration is a separate qualified lifecycle. Future registration/removal changes must update the same list, preserving weak lifetimes and order. This increment supplies no automatic production population hook or invented constructor completion.

## Native ordering to implement

1. **Bind the owner before native effects.** Use `io.observedSessionNotificationState()` to require an existing owner, then validate that its `setOprHasDoeBindings` refers back to the same shared object. Do not call the lazy state getter first and silently create an isolated notifier. A missing/mismatched owner refuses without native flag or map effects.
2. **Suppression first.** If actual `state.suppressed` (`+0x141`) is true, record a diagnostic suppressed return and return. Do not require/read receiver population, reset `flag90`, create a map key or call a receiver in this branch.
3. **Reset before lookup.** Otherwise write `state.flag90=false` (`0x1402cf408`). If the population/key lookup is unavailable, refuse **after this store**, retaining it. Do not undo it on error.
4. **Preserve map lookup/default insertion.** With a qualified known-absent key and empty registration list, set `mapKeyPresent=true` and preserve the shared owner's key insertion fact. A known-absent key plus nonempty entries is inconsistent and must refuse; it is not permission to erase entries. Known-present empty is distinct from unavailable.
5. **Snapshot the ordered registrations.** Copy the list for this delivery. Walk it in native order, resolving the weak QObject guard. Expired/dead entries are skipped/removed from this local traversal only. Do **not** prune the original shared registration vector. Preserve diagnostics for each skipped entry.
6. **Require the known slot/receiver contract.** A live entry whose bare method is not `slotSetOprHasDoe`, whose actual model/lifecycle identity is unavailable, or whose guard becomes inconsistent at the native live-receiver boundary refuses after prior dispatcher/receiver effects. Do not turn this into a fake empty binding or a successful callback.
7. **Read backend witness before items.** A qualified known-null model `+0x18` completes this receiver immediately, without model-item lookup or QIcon work. Unknown backend refuses at this first unresolved receiver read. Model constructor writes initial null, but later publication must be tracked to keep the witness current.
8. **Find the first logical item only when backend is non-null.** Walk the actual `flow.items` in order and compare each item's operation ID, matching native `0x140086820`/virtual `+0x28`. A missing logical item completes this receiver without allocation. An unavailable/null item pointer or unqualified item-ID mapping remains a guard rather than an invented skip. Duplicate operation IDs select the first match.
9. **Stop at the icon branch.** A non-null backend and matching logical item reaches unrecovered icon/backend ownership. For true, refuse immediately before native allocation at `0x140086d0d`; for false, refuse before the actual backend `vt+0x238` clear call. Retain the pending model/item/backend identities and ordered arguments. Do not allocate a replacement icon, set an Operation Boolean, alter graphics maps, deliver a queued event, or clean up an allocation that this prefix never made.
10. **Finish only after all snapshot entries complete.** Normal return means this bounded dispatch returned, including suppression as a distinct reason; it does not mean a callback was delivered when none was reached. Native invokeMethod is DirectConnection 1 and ignores its Boolean result, so this helper must not introduce an aggregate false-return protocol. Unknown Qt invocation paths stay guarded.

Only diagnostics may be added beyond these effects. An attempt should retain arguments, suppressed/reset/key-inserted status, visited indices and outcomes, completion/stop stage, and pending live receiver identity. Record it on the existing binding owner so an exception leaves the prefix reviewable. Do not replace or roll back earlier attempts, native flags, map facts, committed B121 records or earlier receiver outcomes.

## Why the live icon branch stays guarded

For a true signal, the identified native receiver allocates a QIcon and adds the embedded DOE resource **before** backend graphic-map lookup. A missing graphic item therefore occurs after allocation and can retain an orphan icon; it is not equivalent to a missing logical item. The known SfDiagEditor/SfDiagCellEditor setter deletes the old primary icon before assigning the new pointer. These paths need actual backend/map/icon ownership and Qt construction/resource qualification, which this increment does not supply.

The new helper must not complete this branch by marking `hasDoe=true`, assuming a graphics item is absent, or substituting a private headless backend. The only completing receiver branches in this scope are qualified null backend and qualified missing logical item.

## Focused tests and acceptance

Use actual test-owned Project/SubProject flow objects with explicit diagnostic lifecycle/backend witnesses and one shared notifier bound through StudyIo. Such fixtures test the projection; they must not claim native startup or runtime observations. Proposed target: `m2m_doe_variable_notify`; CTest name: `m2m.doe.variable_notify`.

1. Missing shared owner, mismatched owner and unavailable binding refuse without creating a new notifier or changing flags.
2. Suppressed actual owner returns while population is unavailable; `flag90`, key presence and receiver diagnostics remain untouched apart from the attempt receipt.
3. Unsuppressed unavailable population resets `flag90` before refusal; known-empty absent population inserts the key; known-present empty returns; contradictory absent/nonempty refuses.
4. Two distinct live actual flow models with known-null backends complete in registration order, including a model outside the active sub-project. No item lookup, icon allocation or scalar model change occurs.
5. Expired weak and native-dead retained leases are skipped locally; shared vector order/count are unchanged. A later live receiver is still reached.
6. Non-null backend with no matching logical item returns; first-match behavior and unchanged signed operation ID are checked independently from the active operation.
7. Earlier null-backend/missing-item receivers finish, then a later matching live backend refuses at the icon stage. Preserve flag reset, key fact, visited order and pending identities; no later receiver or backend/icon effect runs.
8. Unknown backend/method/model lifetime refuses at its reached boundary. A false argument with a matched backend stops before backend clear and performs no icon allocation.
9. Repeated calls use current shared registrations, with a new ordered snapshot per call; they do not erase expired persistent registrations or overwrite prior attempt receipts.
10. Existing B121 public/prefix tests continue to show the active DOE guard and retained variable/keyword state. No `temp.KWD`, source-success or full parse claim is introduced.

The parent performs shared-header integration, reconfigure/build, focused CTest and required regression. No build/test execution is part of this design task. Before a later scalar continuation, require a normally returned helper for every operation message, then perform the native **post-delivery keyword lookup**; keep later unknown receiver, function/enum, geometry, mode2 and ownership guards.
