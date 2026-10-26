# CP200 system-lubricant test plan review

## Verdict: PASS

The revised plan cleanly separates the reconstructed in-memory list from the original serialized `InterObject.KEY` oracle. It now gives a literal, source-derived four-record list conditional on explicit loaded-object and slot-candidate preconditions, and a separate two-block file expectation derived from the qualified writer filter. The prior ambiguity about the 3D skip rule is resolved: position 1 is skipped as the candidate itself, while the pairs with slave 1 remain in memory; serialization then filters by orientation.

## Evidence and scope

- The RS39 fixture is a real positive-import case: active sub-project 2, operation 3 (`SfOprForming`), System lubricant ID 1, a real `Lubricant00001.KEY`, and a retained original `Task00003/InterObject.KEY`. Template values and the output byte count/hash were verified against the retained files.
- The historical producer records and CP198 folder sweep establish provenance, successful original save, and reconstructed reachability. The plan correctly limits their meaning: they do not establish every original intermediate state or prove the original four in-memory records.
- The new four-record assertion is conditional on the actual loaded model having exactly three defined engine objects with literal IDs/positions 1–3 and exactly one candidate at position 1, with the underlying kind and slot fields checked. If those checks fail, the plan retains the independently justified file/owner assertions and reports the unresolved mapping instead of adapting expected values to production behavior.
- The two serialized `CNTACT` records and exact 631-byte fixture match are distinct from list count and use literal retained bytes, not the reconstructed serializer as an oracle. The plan also bounds object-state assertions to qualified `bccdef_lubricant` effects and disclaims full state equivalence or a complete native trace.
- Current rebuild/writer paths are already scoped; the gap is real loaded-operation coverage of the positive template branch. Test-only ownership in `source/apps/moproj2mst/tests/s/test_save_current.cpp` is appropriate. Production `save_current.cpp` remains outside this cut unless an independent assertion exposes a concrete defect.

The synthetic stale-list sentinel is explicitly separated from corpus state; fallback behavior is deferred pending a qualified input. No test-only production seam, fabricated admission fields, or broad parity claim is proposed. Existing test framework conventions and scratch confinement are appropriate.

No source, test, build, GUI, REA database, parent, DMO, Claude, or original-executable state was changed in this review. No tests were run. Exclusive edit ownership for the proposed implementation remains `source/apps/moproj2mst/tests/s/test_save_current.cpp`.
