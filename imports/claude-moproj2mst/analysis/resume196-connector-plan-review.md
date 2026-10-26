# CP196 connector plan review

## Decision

**BLOCK pending one admission-contract clarification.** The instruction boundary and intended effect are correctly scoped, and the plan appropriately rejects native claims about readability or the value at `[P+8]`. The remaining gap is that “provide one qualified symbolic `T`” does not define who may mint that qualification or how it becomes an admitted model input. Existing CP195 evidence qualifies the parent token and its nullness, but does not qualify a memory read or the contents at `P+8`.

## Evidence and boundary

I read `analysis/resume163-connector-qobject-static.json` before interpreting the listing. It qualifies a read-only original unwind-region disassembly, not a complete logical function or runtime proof. The cited original binary and listing digests match the qualification. At preferred VA `0x6713143a` the listing has `48 8b 4f 08` (`mov rcx,qword ptr [rdi+8]`); the next instruction starts at `0x6713143e`, and the image-base conversion gives the stated RVAs `0x13143a` and `0x13143e`.

Thus the exact static instruction and stop boundary are supported. The bytes establish an eight-byte read into RCX; they do not establish that nonnull `P` is readable, what value is stored at `P+8`, or what that value means. The plan correctly states these limits and keeps the later `[rcx+0x40]` load outside the cut.

## Contract review

- **Control-flow admission:** Starting only at CP195's nonnull fallthrough terminal, requiring `parentIsNull == false`, and rejecting null/taken, unknown, forged phase, marker, or cursor states preserves CP194/CP195 guards. This is consistent with the current source, where unknown parent nullness refuses before JE and CP195 selects only a path.
- **Effect shape:** RCX receives symbolic `T`; RDI, flags (including undefined AF), stack, sparse state, links, ownership, counts, and completion flags stay unchanged. The read is not modeled as a store or publication. Committing RCX, read provenance, and phase/cursor atomically is the right invariant.
- **No speculative native behavior:** The plan explicitly disclaims readability/fault outcomes and does not assign native bits or semantics to `T`. No unsupported native claim is needed for the bounded symbolic model.
- **Test seam practicality:** The existing `SavedEntryRdi`/restored-RDI token and allocation/report/task/private identities provide plausible anchors for a test seam. The plan's refusal matrix is also concrete. However, current CP195 contracts shown in the source cover saved RDI, TEST outcome, and JE branch provenance; they do not provide a slot-read qualification type or an admission operation for such a record. A record merely supplied by a test is not yet evidence that it is current, non-aliased, and bound to the accepted CP195 frame. Without an explicit issuer/admission rule, a forged record could self-assert the very binding terminal validation is meant to prove.

## Required plan amendment

Define the seam as a test/model-only admitted observation with an explicit acceptance boundary. State which component creates or admits it, and require validation against the live allocation owner, report ordinal, task/private identities, exact saved/restored RDI token, CP195 branch/parent refinement, module digest, source RVA `0x13143a`, displacement `+8`, and width 8 before any state mutation. Make clear that this admission proves only consistency of the symbolic model input; it does not prove a native read occurred or that a native address is readable. Require refusal of unadmitted/self-forged, stale, foreign, or mismatched observations with complete snapshot equality. This makes “qualified `T`” operational without overclaiming native evidence.

After that clarification, the proposed single-instruction scope, reentry/rewind obligations, and inherited CP194/CP195 guards are suitable. No source, test, build, GUI, REA database, parent/DMO/Claude, or original files were changed; only this review report was written.
