# CP97 independent review: prepared geometry query borrow

**Result: no blocker under the documented serialized-call contract.** The released callback is limited to install, the existing query, post-version observation, and stage assignment. It does not weaken later barrier checks or turn the recorded frame pointer into a continuation capability.

## Source and order

The low-record path in `source/apps/moproj2mst/src/engine/viewer.cpp` retains all actual model/step and geometry identity checks, COW/table/timer prefix, DB owner/reader qualification, receipt/index bounds, record-version branch, frame allocation, and frame snapshots before the callback. The `frame->step` observation remains outside it. At the former install point, `GuiModelAccess::withStep` first performs its standard `requireGeometrySyncAvailable()` check while the model is unparked. Inside the callback the order is: `install`; return from the callback if installation fails; one `step_dir_sizes` call using that same `nativeStep`; store the query result; read `nativeStep->step_version` into `postVersion`; set `QueryCommitted`; callback ends. The outer code then returns the original pending result.

`GeometrySyncAccess::install` checks for null/already-pending and stores the frame; it does not dispatch user code or mutate `GuiModel::step`. The native query body is the previously qualified closed reader operation: it reads the actual directory record and conditionally updates `step_version`, with no event/callback or step-owner replacement. Thus the callback does not re-enter `withStep` after installation. The strong local `dbOwner` shared pointer remains in scope and pins the exact reader throughout the query. Query exceptions unwind with the installed frame and partial effects retained, as the source receipt describes; no catch or rollback was added.

## Serialized-path boundary

The callback-free assumption is consistent with this exact source interval: between acquiring the access callback and finishing the query, the only operations are the private barrier assignment and the already-closed `step_dir_sizes` body. There is no event pump, Qt signal, public mutation call, owner setter, or other unknown call in that interval. The source helper itself is a synchronous `std::invoke` with no lock, so this does not establish safety against concurrent external raw writes to public `model.step`; that remains outside the stated serialized GUI-call contract. The report appropriately does not claim a lock or allocation-generation lease.

## Test and limits

The focused fixture helper now captures the nonnull `model.step.get()` before refresh and checks it immediately after each refresh, before dereferencing its saved native reference. This gives endpoint coverage across the high/low, ownership, bounds, reload, and applicable parked cases. It catches an ordinary step replacement, but by itself cannot observe transient replacement/ABA or prove which pointer was passed at the query call. Static binding supplies that missing part: query argument one is the callback's `nativeStep`, and no owner-replacement callback occurs in the bounded scope. Existing low-record first-call mutation/pending checks and the actual two-View parked retry test continue to cover install-before-write and subsequent refusal. No private-frame test hook is needed.

## Conclusion

I found no barrier weakening, frame pointer escape, DB lease gap, argument/result change, or install/query/post-version ordering defect. Current `viewer.cpp` matches the CP97 receipt SHA-256 `e2287cac017f17035e7c6f172d6f2765300a7c56ff0207b489e7b07142cdbede`; the test file matches `2be69b66f3e1e06585fbe608f650381dc3bb2b57269a974716da44bb6731dee9`. The endpoint test's ABA limitation is explicit, not a blocker given the static same-scope binding and serialized-call boundary. Root owns grouped runtime/build acceptance; I did not run it.
