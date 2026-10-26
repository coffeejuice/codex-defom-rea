# CP192 connector independent plan review

## Decision

Pass for implementation of the bounded caller spill, subject to the focused tests and source review described below. The slot arithmetic, cursor and partial-state boundary are coherent. No blocking plan defect found.

## Evidence and instruction boundary

Read `analysis/resume163-connector-qobject-static.json` before interpreting the retained listing. Its qualification says this is an unwind-region disassembly, not a complete logical-function or runtime proof. The metadata records QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and listing SHA-256 `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The retained listing confirms preferred addresses `0x67131429`, `0x6713142c`, `0x67131431`, and `0x67131435` contain respectively `mov r12,rax`, `mov [rsp+0x58],rax`, `mov [rax+8],rbx`, and `test rdi,rdi`; their RVAs match the plan's `0x131429` through `0x131435`.

CP191 is terminal at `0x13142c` with live RSP `S+8`. Thus the selected instruction's destination is `[S+0x60]`, and the exclusive successor cursor is `0x131431`. This instruction stores qualified RAX into a caller slot; it does not alter registers, flags or RSP. It precedes, and excludes, the reverse store at `0x131431`. The next instruction's local effect is consistent with writing qualified RBX/task into private+8, while the spill and earlier task+8 continue to identify private. These are separate directional relationships.

## State and admission

The proposed spill must be a value-bearing symbolic private-allocation token with provenance, not native pointer bits. Qualifying its owner/control block, Report ordinal, allocation identities, module/instruction and both stack offsets is appropriate. Existing saved RBX `[S+0x10]`, saved RDI `[S-8]`, return lineage `[S]`, and the new spill `[S+0x60]` are distinct observations. Full CP191 admission and task+8 value/marker must remain intact. Phase and derived cursor should commit with the spill only after complete validation, using nonthrowing operations.

The partial state is correctly constrained: public task+8 remains published; private+8 remains qualified zero; reverse-backlink availability and both completion flags remain false; allocation/reference counts, registers, flags and stack remain unchanged. The spill phase should require its exact observation on reentry even at a smaller diagnostic budget. Earlier phases must reject premature spill values. A rewind must clear the spill and restore phase/cursor together; phase-only rewind must refuse.

## Acceptance coverage

The proposed positive and refusal coverage is adequate in scope. Ensure full-state snapshots include the spill observation and derived cursor, and compare refusal snapshots including allocation/reference counts. The positive advance should prove the exact token at symbolic `[S+0x60]`, cursor `0x131431`, and preservation of all other observations. Exercise every retained terminal budget/task-key combination, intermediate budgets, historical cut behavior, consistent rewind/resume, and maximum-plus-one/maximum-size invalid budgets.

The corruption matrix should retain every CP191 identity and provenance guard and include missing/foreign/expired spill owner, mismatched control block, wrong ordinal/module/instruction/slot offsets, phase/cursor mismatch, premature spill at earlier phases, and phase-only rewind. For the later reverse cut, the plan correctly requires a symbolic task token and phase-exclusive treatment of private+8 zero versus publication; the current `optional<uint64_t>` zero cannot encode both states. Keep that later cut separately admitted and tested.

## Review boundary

No source edits, builds, tests, GUI operations, REA database changes, or deployment were performed. This review is limited to the plan and static retained bytes. Fault behavior, concurrency visibility, concrete pointer values, and successful full QObject/task construction remain unestablished.
