# CP183 R300 plan review

**Decision: BLOCKER — narrow the behavioral claims and test reachability before implementation.**

## Evidence checked

- `source/apps/moproj2mst/src/engine/save_current.cpp:390-435` defines `baseDefaultPairs`. It obtains `opModel` once, reads `m->step`, returns empty before effects if null, clears `inter_obj`, then counts/iterates objects. The two loops are in ascending order. The first selects the last eligible candidate (because each candidate overwrites `master`); the second emits pairs in ascending object index, skipping kind 7/null and the step-type-3 self pair. This order is significant and the proposed callback enclosure can preserve it.
- `classDefaultPairs` at lines 438-445 dispatches FormingExpress to `feDefaultPairs`, Cogging and Distortion to their own helpers, and otherwise to `baseDefaultPairs`. Thus the existing `save_current_oracle_b` cases named in the fixture do not establish base-helper reachability merely because they exercise Save; their class-specific dispatch may bypass it.
- `tests/s/test_save_current.cpp` has oracle/corpus Save sweeps, but no direct assertions for `baseDefaultPairs`. The existing direct helper precedent is `tests/mdt/test_engine_l3.cpp:135+`, which tests `feDefaultPairs` with a real fixture and explicitly labels it helper mechanics rather than public Save. `test_save_current.cpp` is not the sole plausible fixture location if adding an internal-helper case; check the existing `test_engine_l3.cpp` conventions and test registration before reserving only the former.
- `engine_internal.hpp:129` holds the scalar `ContactPair` type. The plan should assert `master`, `slave`, `lub`, friction/IHTC and any relevant mode fields, plus pair ordering, against a fixture whose object kinds actually produce exactly one master candidate and at least one emitted pair. Current listed corpus cases are not shown to meet that predicate.

## Required corrections

1. **Reachability is unresolved.** The plan says “Use the existing … fixture(s) that exercise base default contact pairs,” but the visible `save_current` fixture list provides no proof that a dispatched class reaches the base helper or that its objects satisfy `defined > 1 && cand == 1 && master > 0`. Identify a specific copied corpus fixture and its dispatched class/predicate, or explicitly use a narrowly scoped helper-level case with a valid opened operation and real model. Do not imply a public Save test unless the class dispatch actually reaches the helper.
2. **The null-step test cannot simply move the real step and call the helper unless the model/operation invariants are preserved.** The plan appropriately requires restoration on both normal and exceptional exits, but should require RAII restoration so a failed assertion/exception cannot leave the fixture corrupted. Verify detached-list count and pointer identity after restoration.
3. **“Clear occurred” requires positive evidence.** Seed the attached step list and assert it is empty after the present-step call. The null branch should assert the detached list remains populated and the helper returns empty. These prove the distinction without asserting native equivalence.

## Scope/semantics

The production ownership is appropriately one source file and the transformation can keep all pointer use synchronous. Capture by reference is safe only if the callback is invoked immediately and never stored; confirm that property in the chosen API at implementation review. Keep null return before clear/count, clear-before-count, existing filtering, last-candidate selection, pair order, and all property reads inside the callback. This helper-only change does not establish public Save coverage or a parked-model barrier.
