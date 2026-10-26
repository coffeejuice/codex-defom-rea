# CP252 named-display no-step contract independent review

## Verdict: PASS

The report correctly treats the native getter's default `3` as a conditional scalar result, not proof that named-model creation safely completes without a main step. On the named-map miss and assuming earlier allocations/initialization return normally, the decompile gets a valid main wrapper, reads dimension `3` when its step guard is null, and constructs a separate new model. Name-specific role/metadata calls and the display backlink precede the later main-step relookup.

The critical copy is correctly identified: after re-fetching the current main wrapper and step, native code unconditionally loads from source `step + 0x744` at `0x140cfa4be`, with no null test between the step getter and load. Under the report's stable-null-step and normal-prefix assumptions, this predicts an access violation at effective address `0x744`; the destination `+0x744` store and later title/control/holder insertion/normal return are not reached along that ordinary path. The report does not convert this prediction into a claimed observed crash, process exit, exception disposition, unwind outcome, leak, or cleanup result.

The input cases remain appropriately separate: existing-name return precedes creation/main checks; a live wrapper with null step is distinct from no wrapper; and allocation/constructor failure is not folded into the ordinary prefix. The role/metadata differences for ROI/Coupons versus an ordinary name and the dependence on successful earlier calls are stated. Current converter behavior—existing holder first, then explicit missing-step refusal before allocation—remains distinct from native behavior. The report appropriately rejects changing to `dimensionOr(3)` and returning a successful model, since later step-number and title reads remain unchecked.

The proposed smallest witness is precise and evidence-aware: a fresh owned-original named-map miss, valid main wrapper, absent step, producer/reachability provenance, then observation of the second step lookup and fault site plus actual handler/caller/process result. It permits the state to remain unresolved if no legitimate occurrence can be established and labels a forcibly nulled step as an intervention. It does not request this review to open GUI/REA or alter production behavior.

Retained `a.c`, `c.c`, and `g.c` support the described static control flow and `+0x1c` dimension read; the report binds original identity and byte/decompile hashes and qualifies the DUMPBIN ranges as static inspection. No fresh original runtime occurrence is claimed.

No source, build, GUI, or mutable REA changes were made or run for this review.
