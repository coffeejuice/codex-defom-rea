# CP193 connector plan review

## Decision

Pass for the proposed bounded CP193 transition, subject to the stated prerequisite that CP192 is implemented and validated first. The plan correctly scopes the next instruction and models the reverse link as a value-bearing transition. This review is read-only; it does not establish implementation or runtime behavior.

## Evidence checked

The retained qualification JSON identifies the QtCore capture as an unwind-region disassembly from a read-only original binary and explicitly limits its evidentiary scope. Rechecked SHA-256 values match: QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; listing `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`.

The listing shows the expected sequence at preferred VAs: `0x67131425` stores RAX to public-task `[RBX+8]`; `0x67131429` copies RAX to R12; `0x6713142c` spills RAX to `[RSP+0x58]`; `0x67131431` stores RBX to private `[RAX+8]`; `0x67131435` begins `test rdi,rdi`. Thus `[0x131431,0x131435)` is exactly `48 89 58 08`, and stops before TEST. Given CP192's live `RSP=S+8`, its spill is `[S+0x60]`. The two +8 links have opposite directions: public+8 contains the private token; private+8 receives the task token.

## Contract review

The plan correctly keeps CP192's caller spill as inherited state and makes CP193 only the reverse store. Its tagged/current-slot requirement prevents the historical zero observation from coexisting as the current value with a published task token. The explicit successor phase/cursor at `0x131435`, atomic modeled commit after admission, paired value and marker, and consistent rewind/resume are coherent. Retaining XOR flags and leaving completion flags false is correct because TEST is excluded.

The admission list appropriately carries forward CP192's identity, owner/control-block, ordinal, token, registers, stack/CALL-RET, sparse-state, shared-null and task+8 checks, and qualifies the new task token and private+8 destination. The early/terminal guards, smaller-budget terminal revalidation, refusal snapshots, and rewind checks address partial and inconsistent states. The proposed acceptance matrix includes the key forged-token, ownership, provenance, premature-publication, phase/cursor, corruption, budget, and count-preservation cases.

## Conditions and limits

No plan blocker found. CP193 should remain gated on validated CP192 spill state, as the note already requires. Implementation should ensure every earlier admitted phase rejects both the reverse marker and a task-valued current private+8 slot, while CP193 requires both; terminal reentry should validate those facts independently of diagnostic budget. This follows the plan's requirements and is worth preserving as a direct invariant in tests.

The static listing does not establish native pointer bits, fault/concurrency behavior, parent choice, or subsequent initialization. The plan appropriately leaves those claims open. No source, build, GUI, REA database, or deployment action was performed.
