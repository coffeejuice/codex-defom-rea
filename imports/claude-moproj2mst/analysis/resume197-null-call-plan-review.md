# CP197 null-parent CALL plan review

## Decision

**PASS for the post-CP196 implementation assignment; BLOCK starting implementation until CP196 is actually frozen and source-reviewed.** The revised plan correctly makes call-generation history and the live symbolic `[S]` slot part of CP197 itself. It correctly keeps the existing initializer `ReturnToCaller` record as historical provenance, makes production admission depend on the exact Session-issued known-null Report token, and limits the new transition to the architectural CALL.

The plan explicitly states that CP196 has only a proposal/review at present and that the default Session remains guarded at `0x131438`. I found no basis in the plan documents to treat the CP196 freeze gate as satisfied. Once it is satisfied and checked against source, the CP197 assignment is sufficiently bounded to begin.

## Boundary and source-readiness conditions

The recorded listing bytes at QtCore RVA `0x131449` are `E8 52 98 EF FF`, decoding to `call 0x2aca0`; the following instruction is `mov [r12+40h],rax` at `0x13144e`. The plan correctly models the CALL's return address as `0x13144e` without claiming native reachability, callee behavior, or runtime helper semantics.

The admission contract is appropriately exact: the Session-only CP196 source capability and known-null token must be bound to the first Report occurrence, owner/control-block identities, frame/token occurrence, saved/restored RDI, `[S-8]` lineage, CP194 TEST flags, CP195 taken-null JE, cursor `0x131449`, stack lineage, and incomplete state. Synthetic standalone refinements cannot admit production. Unknown or nonnull states, missing/foreign/stale/copied/replaced capability, and inconsistent inherited state refuse without mutation. The default Session unknown guard remains at `0x131438` until CP196 is verified.

The architectural transition is correctly specified as atomic: make a distinct call generation for caller `0x131449`, target `0x2aca0`, return `0x13144e`; write that return address into live `[S]`; move RSP from `S+8` to `S`; and stop at a distinct callee-entry terminal before `push rbx`. The earlier initializer call record (`0x131417 -> 0x13141c`) remains historical; both return addresses must not appear as simultaneously live. The plan forbids changes to registers, flags, private fields, links, counts, and other observations beyond defined CALL effects, and correctly excludes TLS, helper execution/result, private `+0x40`, and all callee work.

## Required gates and implementation checks

Before CP197 begins, verify the actual CP196 source freeze against its reviewed proposal, including the private Session issuer, exact first pending Report binding, owner/token identity checks, downgrade and replacement refusals, and complete snapshots. Do not infer this gate from the plan or prior PASS review.

During CP197, implement and validate distinct call-generation history and a live slot record bound to the exact call and stack origin. Include the new fields in complete snapshots and validators. Refuse forged tuples, stale/wrong slot generation, wrong caller occurrence, RSP/lineage mismatch, phase/cursor disagreement, partial rewind, invalid budget, and terminal replay without mutation. Rewind must restore the CP195 terminal, prior live slot generation, call history, RSP, phase, and cursor coherently.

Focused acceptance should prove the exact cursor/RSP/slot changes and stop before the first callee instruction; compare complete before/after state to show no unrelated change. Preserve the standalone CP194/CP195 test meanings and all guards. Test production admission only with a real Session-issued capability, and test refusal for nullness mismatch, nonnull, synthetic-only, capability/frame/owner mismatch, malformed stack/call state, premature successor effects, and partial rewind.

## Evidence limit

The plan appropriately separates the CALL-only structural step from any claim about executing or returning from the callee. The bounded callee listing does not qualify TLS-index initialization, same-thread `TlsGetValue` result, owner identity/lifetime, or complete effects/return behavior. Those remain prerequisites for a separate callee/return increment and are not prerequisites for this isolated CALL transition.

This review covers the revised plan and its stated evidence. It does not independently rehash or execute original images and does not verify CP196's implementation or freeze. No source, tests, build, GUI, mutable REA database, parent/DMO/Claude/original artifact, or deployment was changed.
