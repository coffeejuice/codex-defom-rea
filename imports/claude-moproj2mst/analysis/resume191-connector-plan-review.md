# CP191 connector independent plan review

## Decision

Pass for the proposed bounded `mov r12,rax` transition, with implementation and focused-test results still required. No blocking plan defect found.

## Evidence and instruction boundary

Read `analysis/resume163-connector-qobject-static.json` before interpreting its retained caller listing. It qualifies the capture as an unwind-region disassembly, not a complete logical function or runtime proof. The retained listing SHA-256 rechecks as `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`; the original QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The listing confirms the selected instruction at RVA `0x131429` is bytes `4c 8b e0`, `mov r12,rax`. CP190's preceding store at `0x131425` writes RAX to `[rbx+8]`. The next instruction at `0x13142c` spills RAX to `[rsp+0x58]`, and the later instruction at `0x131431` writes RBX to `[rax+8]`. Thus the proposed exclusive stop at `0x13142c` includes only the R12 register copy. It leaves public task+8 published and does not publish private+8.

## State and admission

CP190's phase/cursor mapping has `BeforeCallerPrivateCopy` at `0x131429`; the planned successor should map to `0x13142c`. Since CP190 qualifies RAX as the allocation-bound private identity, copying that identity into a value-bearing R12 observation is justified. Architectural R12 changes; the other represented registers, flags, stack position, and saved-slot lineage should remain unchanged. RSP remains `S+8`; no spill observation is warranted.

The plan correctly carries forward the public task+8 value and publication marker, while keeping sparse `private08` zero, the private-to-task backlink marker false, and both completion flags false. Admission must continue checking the pending Report ordinal, module digest/provenance, allocation and address-point identities, live widget and shared-null owners including control blocks, saved RBX/RDI, RAX identity, RSI/flags, stack and CALL/RET lineage. At CP191 reentry it must additionally validate the R12 observation against the same private identity, as well as the CP190 task+8 publication, before returning without replay. The proposed paired phase/cursor and R12 update is coherent; a rewind must restore the observation and phase/cursor together.

## Acceptance coverage

The focused plan covers the essential positive transition, complete-state preservation, no synthesized spill or reverse backlink, terminal reentry across admitted budgets, consistent rewind/resume, and refusal snapshots for token, cursor, phase, owner/control-block, identity, provenance, register, flags, stack/return-lineage, premature-publication, and completion corruption. Ensure the snapshot helper includes the new R12 value and the CP191 phase/cursor; otherwise refusal and unchanged-state assertions could miss mutation. Invalid-stop coverage should admit the new maximum and reject maximum plus one before mutation.

## Review boundary

The plan's claim is limited to the local static register transfer and instruction ordering. It does not establish incoming R12 contents, concrete pointer values, fault/concurrency behavior, QObject parenting, or later task behavior. No source changes, builds, tests, GUI, REA database, or deployment operations were performed; only this review note was written.
