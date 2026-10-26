# CP154 independent review: geometry allocator bridge and temp-only boundary

## Decision

The temporary-only cut is technically implementable without a fresh same-run original witness, but only as a **candidate-owned, incomplete allocation occurrence**. Static evidence already binds the call shape: the first scalar receiver has dimensions 1x1x1, the resize requests temporary size 0, the source malloc wrapper normalizes only the HeapAlloc length to 1, and the temporary is retained across a zero-byte copy before the separate value allocation request8. On this cut, the candidate can create/use its own reconstructed module-role heap and stop at the actual HeapAlloc return. That would demonstrate candidate kernel effects; it would not prove original runtime heap identity, original live input values, or parity.

The full three-lifetime bridge is not yet a source-ready full-lifecycle allocator. CP154 correctly identifies that CowVector bodies/payloads can be shared, cloned, cleared and destroyed outside the Viewer frame, with separate body-storage and payload ownership. That requires provenance-aware freeing and failure continuations. The proposed temp-only experiment must remain outside CowVector and deliberately retain the returned temporary without automatic cleanup. Treat that as an incomplete pending occurrence with an explicit bounded retention policy, not as a completed source allocation call or an ordinary safe reclamation lifecycle.

## Exact admission and stop boundary

Use the CP152 positive prefix only: current installed high geometry frame; actual current step/object and owner; one fresh main recipient; actual ordinal0/id1 normal StateVar receiver; D[1]=1 and all other ordinary widths zero; cur0=0 and cur2=1; unique actual empty value body, null first/last/end and null actual u30 slot; real primary/BC owners and the existing post-setter stage. After the actual mutable lookup, revalidate frame, table/order, receiver identity/type/id, dimensions, body/refcount/capacity and slot before any allocation. Retain the actual setter effects exactly once.

Then establish the candidate module-role lease and prepare all host bookkeeping before HeapAlloc. Perform only the size-0 temporary occurrence (kernel length1), retain the actual nonnull T with its role and occurrence identity, and stop before request8/value allocation. Do not call resize/value_data, publish body or slot, run a free, advance whole-leaf count, copy coordinates, call Stop, retire the frame, process another receiver, or replay on repaired input. If HeapAlloc returns null, park immediately at that raw return: do not continue into new-mode, handler, errno, operator-new throw or fabricate host bad_alloc. A successful raw HeapAlloc result is not by itself a claim that the native malloc/operator-new wrapper returned successfully.

The source does not need a same-run original pointer witness to execute this candidate-owned kernel operation. It does need a fresh original witness before claiming that this candidate allocation corresponds to the original run’s actual heap slot, request, return, or free. Original pointers/handles must never become candidate memory authority.

## Minimal exclusive implementation scope

Avoid touching CowVector, StateVar layout/resize, payload publication, or broad converter allocation behavior for this prefix. A bounded source lane should own only:

- A lower-layer typed module-role heap interface/implementation, separate from `FeatureCrtRuntime` map/proxy ownership. It must own the candidate heap handle and expose an occurrence that records original requested size0 separately from normalized HeapAlloc size1. The handle is process/module scoped and is not destroyed at project or frame release.
- A narrowly scoped app bridge that binds that role to the actual current geometry frame/receiver occurrence and records the retained temporary in out-of-line or barrier-owned pending state. The state must not free T in its destructor. It must prevent repeat allocation for the same parked occurrence and retain the owner for as long as T may exist.
- Focused tests for actual candidate HeapCreate/HeapAlloc success and observed length1, exact CP152 setter/lookup effects, null and nonnull outcomes, no request8/body/slot/copy/Stop, and repeated refresh/repaired input refusing to replay. Add declared failure/order tests only as separate models; do not label declared outcomes as kernel effects.

CMake/test registration is needed only for those focused units. Do not add `CowVector` deallocator hooks or register a source HeapAlloc block in a body that still uses host `free`/`delete`.

## Concrete remaining risks

1. **Retention is a deliberate leak at this cut.** The pending record must make that explicit and prevent cleanup code from applying the wrong allocator. A plan that drops T when the frame dies, or creates a per-frame heap that disappears with it, is not safe. Conversely, an unbounded registry without a clear process-lifetime retention bound is not a deployable full lifecycle design. The cut is suitable as a bounded candidate experiment while the public operation remains parked, not as final application behavior.
2. **The module owner is reconstructed.** Static evidence supports the native producer and role lifetime, but no present GuiModel or FeatureCrtRuntime owner is that process/module CRT. Do not reuse FeatureCrtRuntime’s private test heap, label a candidate-created handle as the original slot value, or expose arbitrary caller heap handles.
3. **HeapAlloc-null is only an observed boundary.** Continuing the actual native wrapper after null requires occurrence-specific new-mode, encoded handler, errno, retry and exception state. The temp-only cut must park before those effects.
4. **No safe payload free is established.** CP153/CP154 show that source-backed payload/body storage cannot enter the current host-owned CowVector. A later publication cut needs payload and body provenance through clone, clear, destruction and error continuations; a successful temporary probe does not reduce that obligation.

## Qualification

The independent CP153 review already found no existing GuiModel-to-runtime binding and no compatible source-heap CowVector lifecycle. CP154’s stricter ownership analysis supports a candidate-only temp-prefix experiment if it is implemented with the exact guard and leak/retention semantics above. It does not authorize full leaf completion or fresh original parity. Static review only; no source/build/test, GUI/original execution, or mutable REA access was performed.
