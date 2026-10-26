# CP152 design review: first nonempty leaf dimension prefix

## Result

The proposed cut is source-consistent as a partial state advance: one actual first normal receiver lookup, then `set_dim0(1)` and `set_dim1(D[actual id])` in native order, followed by an explicit stop before `resize(false)`. It correctly leaves the receiver's value count/body and bound slot untouched, retains earlier effects, does not count the whole leaf as completed, and does not continue to another receiver/copy/Stop/getter/retirement.

No design blocker found. The critical boundary is to preserve exact eligibility and return guards before any setter, and then never enter `resize_values`, `value_data`, temporary operator-new request0, or value-vector operator-new request8. The candidate must not claim a native allocation/OOM path or leaf return from the dimensions-only prefix.

## Evidence and order checked

- Retained native leaf for the first canonical receiver is actual id1, ordinal0, normal `StateVarNode` vtable. The disassembly binds `set_dim0` then `set_dim1` in that order before the resize call. Existing candidate setters are plain previous/current field stores; they do not themselves allocate.
- Wanted count is `D[id] * cur2`; D1=1 is nonempty only with `cur2=1`. D1=1 with cur2=0 still requests zero values and belongs to a distinct path. Require all other ordinary widths zero and keep six BC widths as their separately qualified nonnegative values.
- The first `value_data`/temporary allocator is after both setters. Native invokes an operator-new path with request0 for the temporary, followed later by request8 for the one-double value body. Both are distinct; successful body/slot publication happens before freeing the temporary and completing the ordinary leaf. The proposal stops before all these operations.
- CP119's private heap/new-handler evidence is a separate actual process/runtime owner. A host malloc, `FeatureCrtRuntime` test profile, or old HeapAlloc observation cannot authorize either allocation. No allocator ownership, failed allocation continuation, or cleanup can be inferred from reaching the setters.

## Admission and retained effects

Require the existing same initial high frame/source/primary/six-BC/node n1/actual owner profile, current receiver lookup result0, dynamic receiver type/id/registry body/order/slot binding, unique values body with exact refcount1/empty-capacity state, actual null bound slot, `D[1]=1`, `cur2=1`, and no prior completed recipient whose shared dimension facts conflict with this prefix. Recheck frame, current object/step table, source/primary, body and bound slot after the mutable lookup before setting either dimension.

The two real setters preserve `prev0/prev1` from the current values and set current dim0/dim1; dim2/prev2, value-body/capacity/count, and bound slot remain unchanged. Record a dedicated dimensions-committed/allocation-pending stage; keep the full-leaf completion count unchanged. On any pre-setter guard failure, preserve the actual lookup/COW effects but do not invent dimension changes. After the setters, the same frame remains parked with those dimensions retained; repairing D or owner state on a later refresh must not replay the lookup or setters.

## Existing test correction

`geometry_main_sizing_nonzero_width_preflight_keeps_actual_BC_effects_without_lookup` currently iterates widths `{-1, 1}` and expects neither lookup nor dimension effect for both. The width `1` profile has the exact first-node setting (`freshMain` initializes normal variables with dim2=1), so it should now exercise the new prefix: exactly the actual ordinal0 lookup and the two setter effects, then an unsupported parked outcome before allocation. Preserve the width `-1` expectation as the true pre-leaf guard. Recheck its `cur`/`prev` expectations and avoid asserting that dim0/1 remain zero in the width1 branch.

The test should be discriminating: start with dim0=0 and dim1=7, keep dim2=1 and prior values known, then assert `prev0`/`prev1` and current values after the prefix. Confirm values/body capacity/count and bound slot remain unchanged, no later receivers are looked up, BC effects stay present, arrays checkpoint remains1, node Stop remains absent/timer2, scene remains unsupported, and later D repair does not replay. Also retain pre-leaf refusal for invalid dimensions, foreign owner/body/binding, noncanonical type, nonempty/shared values, wrong cur2, and non-first positive ordinary widths.

All-zero recipients/general 0..8 copy behavior remains unchanged. A non-first D-positive ID, nonempty variable allocation/unwind, actual process heap/handler/errno binding, and post-publication free-success behavior remain outside this cut. No public geometry/editor guard is lifted.

Read-only design review; no source/build/test, GUI, original execution or REA state mutation occurred.
