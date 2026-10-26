# CP161: Stopping directory-query plan review

Source-ready for the exact nonnull-owned, high-record-version query-only prefix; no blocker was found. CP74/75/77 supply the required source memory-owner, physical-allocation receipt and retry constraint. They do not supply public Stopping reachability or transfer viewer continuation barriers. Only this report was written; no source/test/build/fixture, GUI/original execution or REA operation occurred.

## Actual owner and physical bounds

The proposed order is correct: retain CP159 allocation/key0 COW/module selection/cache0/two node-timer samples first. Only then seek the query owner. In the existing serialized **live** ProjectScope, copy currentProject()->dataDb to a strong local lease; compare its identity to model.db before dereferencing the raw model pointer. Obtain the unique reader only through the lease and compare it to actual S.db before reading that reader. A missing scope/owner, foreign handle, opaque model.db/S.db or unregistered replacement reader must remain CoordinateSync with earlier prefix retained. Do not call projectDb to manufacture an owner, construct shared_ptr from raw identities or substitute ownDb.

This is a valid conditional memory-owner join: real newModel uses projectDb, Project owns dataDb and DbHandle owns reader. ProjectScope itself is a thread-local borrow, not a Project lifetime lease. The contract appropriately limits the whole read/query to the existing live serialized scope with no reload/event/callback/concurrent replacement. It is not a new native OPI/selected-Session owner proof.

`directory_receipts::current(lease.get(), *reader)` must be ready/current and match the real allocation/generations. Reject signed S.u740<0 before size_t conversion; then compare against physical elementCount. Recheck isCurrent immediately before the bounded version read/query and retain the lease through it. Actual int32 index*40 fits the x64 offset domain. Logical uac8, stored count uacc, next-record uad0 and loaded are not capacities.

CP75's receipt is suitable here: it is published at the actual full-load/multipart new[] operand, becomes ready after initialization, tracks allocation/owner generations, survives failed/header-only reload that retains storage, and retires actual deletion/reset/reader/owner lifetime. `isCurrent` looks up registered identities/generations before dereferencing a reader. It supplies no ownership by itself; the strong lease and identity checks must precede it. Tests must use these real producers, not inject a receipt or infer extent from editable logical counts.

## Query semantics and retry-safe admission

The retained GUI query and existing `step_dir_sizes` agree: actual S.u740 selects a0x28-byte record; the explicit step argument is ignored. Read +8/+c/+10/+20 into initialized local hsize/MAR-size/A-size/version ints, then conditionally set actual S.step_version. No S header length, MAR/A body, cache or object field is written by this helper. The explicit nonnull reader makes its normal return true; no extra nonnull failure behavior should be invented.

The safely bounded record-version>=6000 preflight is necessary. A query lowering11000 to5999 followed only by a stateless refusal could let a later fresh call use the existing low-version getter fast path. The plan correctly excludes that record before query/version mutation. For6000/11000/INT32_MAX, execute the actual helper and retain any conditional version update, then immediately refuse CoordinateSync. Do not roll the version back, reuse stale receipts across calls or call the whole shared reader whose predicate differs from the GUI.

A lower-version query would require a Stopping-owned durable barrier or closed same-call tail; the current viewer frame is not that owner. Keeping the high-version subset is a real bounded state effect without crossing this dependency. Null companion remains the old post-prefix refusal: a loader's null exit is not whole-reader completion because arrays checkpoint/assignment still follow it natively.

No arrays timer, loader re-gate, header diagnostic, A allocation/cache1 shortcut, nonempty decode, object assignment, Stop or positive coordinate result belongs to this cut. Owner/index/version admission failure preserves CP159's timer/COW/table/widget prefix. Invalid MAR/id bounds and cached1/-1 remain their earlier branches without the query/timer widening.

## Meaningful focused tests and source split

Existing simcontrol300.cpp plus its Stopping cached test is a sufficient disjoint reservation; add direct w2.hpp/receipt declarations, not a new public API, receipt hook or lifecycle barrier. Public6504abb0/140eb5180 and false completeness flags remain.

The decisive positive test must create a live ProjectScope and actual projectDb owner, load a disposable COG copy through the real loader, bind model.db/S.db to that reader and use its real ready32-record receipt. First/last physical records, high-version changes and same-version fresh retries must remain CoordinateSync with one getter/dispatch, count2 node-timer prefix, unchanged headers/MAR/A/cache/coordinates and untouched arrays timer. A version change distinguishes a real query from merely checking receipt metadata; source inspection must additionally confirm all four actual helper outputs are used as locals rather than replacing the helper with a version-only shortcut.

Required failures are missing/foreign/expired scope-owner, opaque identities, actual reader replacement, stale/pending/retired/pointer-mismatched receipt, negative and physical-out-of-range indices. Restore borrowed fixture sentinels via RAII before owner destruction. Logical33/physical32 must reject32 while logical1/physical32 may admit31. Real reload/prepend tests should obtain fresh receipts; use supporting receipt tests for internal generation mechanics and Stopping tests for this consumer join.

Repeated record versions5999/0/-1/INT32_MIN must preserve the old high S version, proving no future fast-path escape. Repeated calls must use fresh delivered pages as CP159's repair does, preserving same-value spin suppression. Existing owner/canonical/parked/no-node/cache-hit/invalid-table controls must remain. No public Open or original runtime claim follows from controlled flag0/cache0 fixtures; examined managed Open normally sets the flag1 route.

## Independent binding

Proposal SHA-256: `6bd6653b8b075def659c3d6e6a7ec55246187fcdda096b2f9ecc6ca45d0d7bb2`. Original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All six published native range hashes were independently recomputed and match: complete828060 loader,825440 query,825400 setter, actual companion null gate, signed-index/directory first-read slice and four-read/setter slice.

Read-time source hashes match the proposal: receipt header `24bf727760f8d21240c7da181662331fe867dbca2c4805eab9dc1b0b1f863c15`; w2_objarrays.cpp `c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359`. CP74 owner audit, CP75 receipt implementation and CP77 query-only admission agree with this source-bound plan. No reconstructed executable/deployment was inspected. A future patch still requires independent source/test review and root build binding.
