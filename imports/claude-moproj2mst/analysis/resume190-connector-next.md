# CP190 connector: publish the public task's private pointer

Execute exactly QtCore RVA `[0x131425,0x131429)`, `48 89 43 08`, `mov qword ptr [rbx+8],rax`, after a validated CP189 terminal. It stores the allocation-bound private identity in the public task's field at offset 8. Stop at RVA `0x131429`, before copying RAX to R12. This is the first public task-to-private publication, distinct from the later private-to-task store.

## Evidence and qualification

Paths below are relative to `imports/claude-moproj2mst`. Retained `analysis/resume163-connector-qobject-static.json` qualifies `analysis/resume163-connector-qobject-671313d0.asm` as an unwind-region disassembly, not complete logical-function or runtime proof. The listing hash was rechecked as `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`.

Read-only PE inspection rechecked `evidence/original-binaries/QtCore4_64.dll`: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`. RVA `0x131425` maps into `.text` at file offset `0x130825`. Its 16 bytes through exclusive RVA `0x131435` are `48 89 43 08 4c 8b e0 48 89 44 24 58 48 89 58 08`, matching the listing.

| RVA | Instruction | Ordering significance |
|---|---|---|
| `0x131425` | `mov [rbx+8],rax` | Public task+8 receives private token; CP190's sole instruction |
| `0x131429` | `mov r12,rax` | Next boundary; not executed |
| `0x13142c` | `mov [rsp+0x58],rax` | Later caller stack spill; not executed |
| `0x131431` | `mov [rax+8],rbx` | Separate private+8 receives task token; not executed |
| `0x131435` | `test rdi,rdi` | Later parent-dependent selection begins |

CP189 took the unconditional `EB 05` at `0x13141e` and skipped the null-allocation arm at `0x131420..0x131425`. The selected predecessor therefore has qualified non-null RAX/private and restored RBX/task identities; CP190 does not add a new native null test or branch.

## Symbolic state and transition

Current `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` ends in `QObjectPrefix::Phase::BeforeTaskPrivateBacklink`, with derived RIP `0x131425`. `returnPrivateIdentity` is the stable `allocatedPrivateIdentity`; `restoredTaskRbxIdentity` is the pending Report allocation. RSP remains `S+8`, with S the private initializer entry stack origin. RSI is zero; XOR flags remain CF=0, OF=0, ZF=1, SF=0, PF=1, AF unavailable. The restored incoming RDI token stays opaque.

Record the task+8 value as the qualified private identity, not a concrete native pointer or an allocation address inferred from host memory. Existing `taskPrivateBacklinkAvailable` may report publication but should have a value-bearing identity observation so a wrong or missing published token cannot pass terminal reentry. Commit this observation, publication marker and successor phase/derived RIP together using nonthrowing assignments after validation. No new allocation is needed.

The only memory effect is public task+8. Preserve RAX, RBX, RDI, RSI, flags, RSP, saved slots and provenance, all sparse private fields and shared-null references. In particular `private08` remains the qualified qword zero from CP179. Keep `privateTaskBacklinkAvailable=false`, `complete=false`, and `qobjectBaseCompleted=false`. Do not overwrite private08 with the task token; that belongs to `0x131431`.

## Guards and refusal behavior

Require the complete CP189 retained prefix before mutation: exact phase/derived RIP; pending Report ordinal and 0xc0 public allocation; original 0x88 private pointee; task/private address points; live matching WidgetRuntimeState and shared-null bindings/reference records; sparse zero fields and historical flags expression; initializer EDX/module provenance; restored RBX/RDI, `S+8`, and CALL/RET slot lineage. Saved owners must match both allocation-store pointer and shared ownership control block; pointer equality alone does not admit an alias owner. Keep QtCore digest and task/private token identities bound to the same allocation and ordinal.

The existing `qualifyRetainedZeroPrefix` unconditionally refuses either publication flag. Its CP189 admission behavior should remain strict; the new terminal needs an explicit phase-sensitive check allowing exactly the qualified task+8 publication while still refusing private+8 publication or completion. Earlier phases must reject any premature publication observation. Reject missing, expired, foreign or inconsistent state before any write; preserve failed-input snapshots. Terminal reentry validates the published value and stops at `0x131429` without replay, even with smaller budgets. An explicit test rewind must restore both publication value/marker and phase consistently; changing phase alone is an inconsistent predecessor.

## Focused acceptance and remaining unknowns

Use the current CP189 tests and full-state snapshots in `tests/mdt/test_doe_task_factory.cpp` as the predecessor contract. Add one valid CP189 advance proving exactly task+8 publication and cursor change; compare all other observations with the predecessor. Assert private08 stays zero, both allocation counts and shared-null reference counts stay unchanged, no R12/stack-spill observation is manufactured, and completion/cache/task execution remain unavailable. Exercise terminal reentry across all budgets and task keys; explicit consistent rewind/resume; corruption of published value/marker; wrong phase, owner/control block, ordinal, module, private identity, saved registers, flags, stack/return lineage and premature reverse-link/completion, all without additional mutation. Retain invalid-stop checks at the new maximum plus one.

The selected static evidence establishes this instruction's effect and local ordering. It does not establish successful runtime construction, concrete addresses or old task+8 contents, fault behavior, concurrent visibility guarantees, QObject parent behavior, TLS/thread selection, later counter updates or final TaskInfo behavior. No source/build/test execution, GUI, REA database or deployment operation was performed; only this note was written.
