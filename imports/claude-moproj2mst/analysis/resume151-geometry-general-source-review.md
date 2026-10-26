# CP151 independent source/test review

## Conclusion

The generalized geometry loop is consistent with the reviewed design. The fixed capacity is gone; `vector::size()` is the sole recipient count, and each eligible current object receives a separate receipt. Receipt insertion is inside a narrow `bad_alloc`/`length_error` guard before the object's native reset. The record has compile-time trivial-copy, nothrow-move and nothrow-default-construction checks, so growth cannot transfer native ownership or throw while relocating prior records. An active `back()` reference is taken only after append and is used only until that recipient helper returns before the next possible append.

No blocker found. The focused tests cover 0/1/2/3/4/8 recipients, actual owned-list growth from 2 to 8 recipients, injected append-resource failure before the next reset, source/destination discrimination, and later live-identity refusals.

## Source checks

- **One count/current loop bound:** the frame holds only `std::vector<GeometryMainRecipientProgress>`. Admission compares vector size to current signed `step.u1378`, validates current ordinal and `max_size`, and uses that size for active/completed range. No fixed capacity or redundant started counter remains. `completeGeometryNodeSkip` continues looping against current `step.u1378` and reloads object/MAR on every iteration.
- **Append/resource boundary:** eligibility/current source/prior receipts/fresh current owner are checked before append. The optional private test observer and `emplace_back` are the only operations in the local catch. On either caught exception, the function returns false before `free_node_arrays`; prior native effects remain, and the outer frame does not reach Stop or retirement. Native primary/BC allocator calls are outside this catch.
- **No stale vector reference:** the active reference is bound only after successful append from `mainRecipients.back()`. All callbacks, copies and helper return finish before the loop can perform another append. Prior entries are reached by index/value; movement of their plain observational fields does not move the referenced native allocations.
- **Postappend/liveness:** `geometryMainActiveMatches` runs after append and before reset; it verifies the current frame/source, current recipient by live step ordinal and every prior completed receipt. The completed validator obtains `step_object_at(step, savedOrdinal + 1)`, compares identity, then accesses object fields. The same validation is used after callback-sensitive leaves and before copy/Stop/retirement.
- **Current-list growth:** the actual loop condition rereads `step.u1378` at each backedge. The test appends six real owned objects via `get_or_create` from the first recipient's final sizing callback and observes the actual bound grow 3→9; it does not alter the count alone. Newly appended recipients each receive one fresh progress record and are reached once without new query/arrays work.

## Test review

- The generalized normal matrix exercises counts 0,1,2,3,4,8 at versions 6000/11000 with null/populated connectivity, mixed BC profiles and exact rows seeded at each recipient's last actual sizing lookup. It checks 28 actual ordered lookups per recipient, distinct primary/positive BC allocations, independent registry bodies, exact 24-byte destination rows through vector relocation and later source changes, one arrays checkpoint/Stop and cached no-replay.
- The actual-list-growth test proves the current-bound backedge rather than a captured initial count. It checks eight admissions, 224 lookup observations, row bytes and one reader/checkpoint/Stop; the later refresh performs no additional admissions.
- The metadata guard test injects `bad_alloc` and `length_error` at a recorded vector growth boundary after at least four committed recipients. The injected exception is raised by the private observer before `emplace_back`, so it exercises the same narrow catch path but is explicitly not a real allocator failure. It verifies earlier rows persist, the next/later nodes remain unreset, Stop/retirement do not occur, and removing the fault does not resume the parked frame.
- The later partial test covers current-width/owner refusal and independent displacement of an early and nearer prior identity at the sixth recipient. It uses actual spare nodes and RAII link restoration, verifies five earlier copied rows, current partial effects, live step-table validation, no Stop and no replay.
- CP150's old fourth-positive refusal is renamed and made explicitly unqualified (logical n0); fully qualified fourth and eighth recipients are successful normal cases. Malformed fourth MAR and other prior guards remain genuine negatives.

Candidate-side hooks/fault injection are not original runtime observations. No build or tests were run for this read-only review.

## Frozen files

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `3AD0117FDC715F4DBB8F0495D5D09213987519E4209A330EA07A1C1E0905ABC2` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `6F2BB9273D9D02A847ACCA6ED49AD5DD9EA19B730A09BF93A8498EBD5228FCFA` |
| `analysis/resume151-geometry-general-loop-implementation.md` | `904B13154FB841DA6CEC665E4C021B4C68E2D8D48CD025665CC09E9BDEA521F4` |

This was a read-only source/test review. No source/build/GUI/REA mutation or original execution occurred.
