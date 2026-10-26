# CP102 independent review: deferred DOE LinkTo

**Result: no blocker found in the frozen source/test change.** Queue scope, borrowed source lifetime, post-reindex target resolution, duplicate-source selection, and container/sibling ordering match the reviewed contract.

## Lifetime and selection

The request vector is local to one exact top-level `DoeVariables` branch in `readInputTable`. It cannot survive a later sibling or a later `readInputTable` call. Each request owns its target `String`, while its source is a borrowed `HeaderVariableRecord*`; the `Study`'s `headerVariableOwners` vector owns each record through `unique_ptr`. Vector growth can move the `unique_ptr` objects but does not move their pointees. The admitted row path does not erase records or invoke an external callback before resolution. It also checks that the current `Study::vars` entries and concrete owners align before processing admitted rows.

On a duplicate-name row, the parser still selects `headerVariableOwners.back()` as the source for the subsequent native setters and queued LinkTo, not the earlier record returned by name matching. The duplicate test verifies the last record receives the ID/Description/link while the earlier matching record is unchanged. This is the subtle pointer-ownership rule most likely to regress, and it is directly covered.

## Ordering and behavior

The caller processes direct DoeVariable children in order. If a row refuses before the container finishes, the local queue unwinds without resolution; earlier record/header/Description effects remain. On normal exhaustion, `reindexExistingVariables(s)` runs before `resolveDeferredVariableLinks`, and resolution completes before later top-level siblings are visited. The later-row-guard case checks queued links are not committed while reached row mutations remain. The later-sibling guard case checks links already resolved at container completion remain applied when a later Constraints sibling refuses.

Resolution walks queued requests in encounter order and names in Study list order. The retained original name-index routine at `0x14091b5d0` scans from index 0 and stops at the first content-equal name; candidate forward iteration is consistent. The retained setter at `0x140100a60` performs only the raw source `+0x40` pointer store. A missing target skips that store, preserving any prior link; a later successful request overwrites the earlier pointer, and an unresolved later request leaves it intact. Empty/absent LinkTo queues nothing and does not clear an old link. The test covers forward/backward/self/cycles, ordered found/missing requests, case mismatch, absent/empty values, and per-container lifetime.

## Ownership and coverage boundary

Links remain raw, nonowning pointers into `Study`-owned concrete records; self/cycle cases do not create ownership cycles. The actual lazy-owner test verifies the link on the same Session-owned Study and confirms the already-known Save guard still occurs before XML effects. No shared session API, owner field, record layout, or C ABI was changed.

I found no defect requiring a correction. The implementation receipt explicitly leaves native Qt queue allocation/failure timing outside the claim, and keeps mixed payload/sample/locale/Save/refresh guards. Those remain qualification limits rather than defects in this bounded link branch. The four source/test hashes match the implementation receipt. Root owns build and test acceptance; I did not run them.
