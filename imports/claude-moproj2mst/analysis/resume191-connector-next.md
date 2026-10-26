# CP191 connector: copy private identity into R12

## Selected instruction and boundary

Continue only from a validated CP190 terminal at QtCore RVA `0x131429`. Execute exactly `[0x131429,0x13142c)`, bytes `4c 8b e0`, `mov r12,rax`, then stop at `0x13142c` before the stack spill. This is a register copy: R12 receives the already qualified private-allocation identity from RAX. It does not read or write memory, alter flags, or change RAX/RBX/RDI/RSI/RSP.

The retained caller bytes in `analysis/resume163-connector-qobject-671313d0.asm`, qualified by `analysis/resume163-connector-qobject-static.json` as an unwind-region disassembly only, establish this local ordering:

| RVA | Bytes / instruction | Boundary effect |
|---|---|---|
| `0x131425` | `48 89 43 08`, `mov [rbx+8],rax` | CP190 public task+8 publication |
| `0x131429` | `4c 8b e0`, `mov r12,rax` | CP191 private identity copied to R12 |
| `0x13142c` | `48 89 44 24 58`, `mov [rsp+0x58],rax` | Next, not executed |
| `0x131431` | `48 89 58 08`, `mov [rax+8],rbx` | Later reverse private+8 backlink, not executed |

## State transition

The CP190 prefix must retain its public-task+8 value equal to the allocation-bound private identity, its publication marker, and its exact cursor. Add a value-bearing `r12PrivateIdentity` observation (or equivalent) and an explicit CP191 stage/cursor. After complete admission, commit that observation and cursor together with nonthrowing assignments. No new allocation is needed.

The instruction leaves RSP at `S+8`; it does not create a stack-spill observation. Preserve flags, saved slots, allocation/reference counts, shared-null records, sparse private fields, and all other register identities. In particular, `private08` remains zero; `privateTaskBacklinkAvailable` remains false; `complete` and `qobjectBaseCompleted` remain false. Do not infer the concrete pointer from the symbolic token.

## Admission, guards, and refusal

Retain CP190's complete qualified prefix: pending Report ordinal and allocation sizes, QtCore digest, public/private identity and address-point bindings, matching live widget state and shared-null ownership/reference records, sparse zero fields, initializer provenance, task+8 published value/marker, restored RBX/RDI, RAX private identity, zero RSI and XOR flags, `S+8`, and CALL/RET slot lineage. Owner checks must include both stored pointer and shared ownership control block. Reject missing, foreign, expired or inconsistent state before mutation.

The current prefix admission logic must permit the CP190 task+8 publication only at CP190/CP191 phases, while still rejecting premature publication in earlier phases and any reverse private+8 publication or completion in this cut. Extend phase-sensitive register and cursor checks so CP191 reentry validates the copied R12 identity and published task+8 value, then stops without replay even at smaller budgets. A test rewind must consistently restore R12 observation and phase/cursor; phase-only rewinds are invalid.

## Focused acceptance

Extend CP190 full-state snapshots with R12 identity and the CP191 cursor. Test one valid CP190 advance proving R12 equals the qualified private token and cursor becomes `0x13142c`, with every other represented state unchanged. Assert no spill, reverse backlink, allocation/reference delta, or completion is synthesized. Test terminal reentry over all admitted budgets and Report task key, explicit consistent rewind/resume, and refusal snapshots for missing/foreign/corrupt R12 observation, wrong cursor/phase, task+8 marker/value mismatch, owner/control-block and identity mismatches, ordinal/module/provenance/register/flags/stack/return-lineage corruption, premature publication, and reverse-link/completion flags. Invalid-stop checks should admit the new maximum and refuse maximum plus one without mutation.

## Evidence boundary

The retained disassembly establishes the instruction bytes, register transfer, and ordering through the next stack spill and reverse backlink. The static-evidence qualification does not prove a complete logical function or runtime behavior. This note does not establish old R12 contents, concrete pointer values, faults, concurrency semantics, QObject parenting, later counter updates, or final task behavior. No source edits, builds, tests, GUI, REA database, or deployment operations were performed.
