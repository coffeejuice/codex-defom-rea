# CP149 design review: two fresh geometry recipients

## Result

The proposed two-recipient cut is consistent with the retained caller loop and the CP148 fixed-row helper contract. The second eligible object is reached through the same original loop backedge, with a fresh actual object lookup and MAR preload; no node Stop, new query, A allocation, or arrays checkpoint occurs between the first and second helper calls. A second per-recipient progress record is therefore the right way to extend the current single-copy frame. The proposal correctly retains first-copy bytes on later failure, guards a third positive recipient, and keeps one Stop/retirement after loop exhaustion.

I found no design blocker. One safety detail should be explicit in implementation: when revalidating a saved earlier recipient after a callback-capable second-recipient effect, first recover the current object pointer from the live step's current object table/ordinal and compare identity before dereferencing any saved pointer. A saved `LLOBJ_DATA*` by itself is not a lifetime proof. If live table membership cannot be re-established, retain the prior copy and stop without following that alias.

## Contract checks

- **Same frame and source:** the plan keeps the installed high-version frame, same initial borrowed step/model/DB/directory receipt, and actual A allocation/count across both recipients. The second copy uses offset0 from the same proved n1/d3/first1 source row; it does not infer row2 or broaden pointer arithmetic.
- **Distinct owners and receipts:** each recipient needs a distinct current same-step object identity, fresh primary allocation, six BC attempt records, node n1, canonical 28-entry registry/body/receiver receipts, leaf progress, and its own post-copy source/destination/24-byte/ordinal receipt. A duplicate object alias must fail before second reset; the first receipt must not be overwritten or replayed.
- **Ordering and backedge:** retained assembly establishes increment/current-count, object lookup, MAR preload, skip checks and helper call within each iteration, then the backedge; Stop follows loop exhaustion. Thus a successful first copy resumes at the native loop, and a second failure leaves the loop incomplete with timer2 and no Stop. The plan keeps MAR preload before later-object skips and does not resume a parked frame on a later refresh.
- **Failure retention:** second fresh-admission refusal leaves the second object untouched; refusal after reset/allocation/sizing retains only reached second effects plus the completed first copy. The frame remains parked. Repairing raw inputs cannot restart either entry. Existing owner/destructor lifetimes keep both arrays; no synthetic rollback or copied snapshot is needed.
- **Caller tail:** successful exhaustion with zero/skip objects performs the existing single Stop, then retires only the installed initial frame if every started entry has a committed receipt. Zero-main and low-version completers stay independent. The populated case uses the already-qualified cached second getter; no third getter or reader replay is proposed.
- **Test discrimination:** seeding A with distinct rowA before first copy and rowB before second copy, while giving both destinations distinct sentinels, is a good discriminator for copy source, destination reuse, prior-copy replay and receipt overwrite. Separate second-admission refusal and post-second-partial-effect failure tests are needed to prove first-write persistence and no rollback. The existing CP148 unqualified second-object refusal should stay a negative case.

## Implementation caution

Keep only the initial/global reader state in the frame and use a fixed-capacity two-entry recipient progress store. During later callbacks/virtual leaves, revalidate the prior record using live step ownership first: fetch the current object at the recorded ordinal, compare it with the saved identity, and only then inspect its current fields. Likewise compare current primary/BC/registry bindings to their receipts without dereferencing any pointer whose owner membership was not just re-established. If a callback invalidates membership, refuse and preserve the already-written first copy.

The report's two-positive-recipient profile remains bounded to fresh n1/d3/first1 objects, zero-width ordinary variables, and exact allocations from the actual producers. Third positives, wider rows, nonempty resize/unwind, preexisting spans, and source-offset variation remain guarded. No original runtime evidence is claimed beyond retained static qualification.

This is a read-only design review. No source/build/test, GUI, original execution, or mutable REA activity was performed.
