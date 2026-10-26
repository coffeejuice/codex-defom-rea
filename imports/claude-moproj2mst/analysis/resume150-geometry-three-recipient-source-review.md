# CP150 independent source/test review

## Conclusion

The CP150 implementation matches the bounded three-recipient plan. One named `mainRecipientCapacity = 3` controls the fixed receipt array and all completion, active-entry, and admission bounds. Every prior record is resolved again through the current live step object table and identity-compared before dereference; this validation runs during later recipient callbacks/lookups, before copy, at loop completion, and before high-frame retirement. Each successful recipient has its own distinct primary/BC/registry/dimension/copy receipt, and the source stays the same actual step-owned A allocation.

The fourth-positive guard and stale CP149 expectation were both corrected as designed. I found no source or test blocker. This is static source/test review only; root's build and runtime test results remain authoritative.

## Source review

- **Capacity:** `GeometrySyncBarrier::mainRecipientCapacity` is the sole capacity value, used for the array extent and the completed validator, active validator, and pre-start refusal. No stale `2` capacity bound or growable allocation path remains.
- **Prior identity/lifetime:** `geometryCompletedMainRecipientsMatch` first checks ordinal range, then calls `step_object_at(step, ordinal + 1)`, compares the result with the recorded object identity, and only after a match reads descriptor/node/registry fields. `geometryMainActiveMatches` and `publishGeometryMainCoordinates` revalidate every previously committed entry. The third-recipient sizing callback therefore cannot use a stale first/second raw object pointer to authorize further writes.
- **Per-recipient separation:** each entry tracks its object/ordinal, primary pointer/count, six BC attempts and pointers/counts, registry body, 28 receivers and dimension snapshots, and post-copy source/destination/24-byte/ordinal receipt. Pairwise checks require prior recipient identities, destination coordinates, and ordinals to be distinct/increasing. The same live A source is required; each new destination must be fresh before reset and distinct from A and prior destinations after publication.
- **Stage order and partial effects:** starting a recipient requires all earlier entries complete and current live-table/owner/descriptor/source facts to match. The one-way global arrays-to-recipient transition is preserved. Reset, primary allocation, six BC attempts, `n=1`, 28 actual lookups/virtual leaves, and the post-copy receipt are tracked per entry. Failure after a later start retains all earlier copied bytes and only reached later effects; parked refresh cannot replay. The actual object loop still loads MAR before its skip predicates and invokes the same helper at each eligible ordinal. One Stop and exact initial-frame retirement occur only after successful loop exhaustion and receipt validation.
- **Fourth positive:** the capacity check occurs before new receipt selection/reset, so a fourth eligible positive object refuses before its destination changes. Later malformed-MAR refusal still occurs at the next caller preload with all three earlier copies retained.

## Test review

- The normal matrix exercises two and three recipients, versions 6000/11000, populated/null connectivity and zero/mixed BC counts. For three recipients it captures three distinct registry bodies and actual receivers, seeds different A rows after each recipient's last lookup while leaving separate destination sentinels, and checks exact copied bytes, pairwise distinct primary/positive BC allocations, the 84 lookup sequence, one arrays checkpoint/Stop, getter behavior and cached no-replay.
- Third-recipient partial tests cover ordinary-width refusal, foreign owner, and independent live identity replacement at prior ordinals two and three while the current third stays at ordinal four. They restore intrusive links/owner via RAII before cleanup and assert first/second copied rows persist, third's five reached leaves/sentinel remain, no Stop occurs, and later raw repair does not replay.
- The fourth-positive/later-MAR test checks all three rows persist and fourth primary/node fields remain untouched; the malformed-MAR variant is injected after the fourth object's already-loaded MAR input. Third fresh-admission negatives check preexisting primary, first-node mismatch and foreign owner before third reset while both earlier copies remain.
- The stale CP149 case is renamed to `geometry_two_recipient_copies_remain_before_unqualified_third_or_later_MAR_guard`; the third positive descriptor now has logical n0 and remains a deliberate negative. A qualified third is exercised by the normal matrix, and a qualified fourth is exercised by the new capacity guard.

Test values are explicitly adversarial candidate-side instrumentation; they do not claim fresh original parity. No test/build execution was performed during this review.

## Frozen files and hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `A75D80335EB22CD47812E175FD755E84635F10422A5701A9762FC96668DC06C8` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `1AC7E183C684530AEDE83847EE3170B2B47C842E60AC903CD90E286FE1827294` |
| `analysis/resume150-geometry-three-recipient-implementation.md` | `7A1EE139DDF89F203A85659395B8996FBAD5F490E59EBFCC112B2C62CEC760FA` |

No source/build/GUI/REA mutation or original execution occurred.
