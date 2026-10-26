# CP98 — independent review of the scoped geometry borrow implementation

## Result

The released `viewer.cpp` block matches the bounded CP97 design. The low-record branch enters `GuiModelAccess::withStep` at the former install point. Inside its void callback, it installs once, returns immediately on install failure, calls `step_dir_sizes` once with the callback's `nativeStep`, stores the existing result and four output fields in the same frame, reads post-version from that same pointer, and sets `QueryCommitted`. There is no `withStep` call or guarded public accessor after installation in that callback.

I found no owner-lifetime, reentrancy, barrier-order, or new pointer-escape discrepancy. The source release correctly retains the serialized-call limitation and does not claim synchronization against concurrent direct writes to public `GuiModel::step`.

## Checks against the architectural review

- **Model/step lifetime:** `model` is captured as a `shared_ptr` in the callback and remains live until synchronous return. The callback's borrowed pointer is the model's actual `unique_ptr` pointee. No lock prevents a separate thread from directly replacing the public unique_ptr; that remains outside the serialized-call assumption.
- **DB/reader lifetime:** the enclosing branch's strong `dbOwner` local remains in scope across `withStep`; `reader` was derived and validated against this owner before entering. The lambda uses it only synchronously. The frame records `dbOwner` weakly, so parking does not invent a reader lease for later continuation.
- **No reentrant callback:** `GeometrySyncAccess::install` only checks/stores the private barrier. The scoped `step_dir_sizes` implementation is direct field/record access with its local stored-version/setter operation; it does not pump events or dispatch user callbacks. No owner reload or unknown call was added.
- **Barrier/order:** `withStep` preflights `requireGeometrySyncAvailable` before retrieving `model.step.get()`. The callback installs the barrier before native version mutation and continues its already-admitted query without attempting a second public admission. Install failure skips all query writes. Query exception unwinds with the installed frame and committed prefix left intact.
- **Pointer/frame boundary:** the callback neither returns nor stores `nativeStep`; it passes it directly to the query and uses it for the post-version read. `frame->step` remains the preexisting historical snapshot outside the callback and has no new read or authorization role. The callback's reference capture of `frame` is safe for this call because both the local shared pointer and the installed barrier own the frame during execution.
- **Regression scope:** `checkGeometryDirectoryPending` now captures the step identity before refresh and requires equal `model.step.get()` after each refresh, before later native-reference checks. This proves endpoint identity on exercised cases, not the query argument or absence of transient ABA. Static callsite inspection supplies the latter argument-identity evidence, as CP97 already qualified. The test correctly does not overclaim interval synchronization.

The unchanged ordinary high-record query remains a separate `withStep` after its early barrier/record-version gates; the new low-record callback is the only scoped crossing of its own install. Both query routes preserve their pending result. No later continuation is enabled.

## Scope and static receipts

This is a source-only review; no build or test was run. Final source hashes match the implementation receipt: `viewer.cpp` SHA-256 `e2287cac017f17035e7c6f172d6f2765300a7c56ff0207b489e7b07142cdbede`; `test_viewer_box.cpp` SHA-256 `2be69b66f3e1e06585fbe608f650381dc3bb2b57269a974716da44bb6731dee9`. `gui_model_access.hpp` remains `e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26`; `engine.hpp` remains `77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f`; `w2_objarrays.cpp` remains `c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359`.

The same-instance test remains an endpoint regression and cannot independently prove the exact query pointer or rule out same-address replacement. Static source review verifies the query receives `nativeStep` directly and no code in the scope replaces the step. The implementation retains the explicit no-concurrency guarantee boundary; acceptance still depends on root's source-stable build and focused/full test run.
