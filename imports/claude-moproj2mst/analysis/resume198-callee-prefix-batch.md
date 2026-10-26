# CP198 deterministic callee/initializer prefix batch

## Decision and implementation gate

One coherent batch can execute ten qualified instructions from QtCore RVA `0x2aca0` through the internal initializer prologue, stopping at RVA `0x29614` **before** the first TLS-index read/comparison. Its executed ranges are `[0x2aca0,0x2acb6)` and `[0x29600,0x29614)` in control-flow order, not one contiguous image interval. This is a structural prefix, not an initialized-TLS fast path or a completed helper call. Implement the ten instructions together after CP197 is frozen and source-reviewed, with intermediate diagnostic checkpoints within that implementation. Separate recovery checkpoints for each push/store are unnecessary.

CP197 source was changing under its owner while this report was prepared. The inspected source already contains `BeforeNullParentHelperEntry`, generation-2 `NullParentCall`, `helperReturnSlot`, and CP197 tests; their presence is not a freeze or source-review receipt. Rebase the plan against the actual frozen CP197 source/test pair before editing. CP196 is the supplied frozen predecessor. The historical CP197 planning text saying CP196 is not implemented is not the current integration status.

## Original qualification and limits

Read `resume163-connector-qobject-static.json` and `wave2-doe-task-public-qobject-ownership-contract.json` before interpreting the caller and helper listings; also consulted `resume196-nullpath-callee.md` and the CP197 plan/review. These qualify retained static original-image evidence, not live execution, complete logical-function boundaries, exhaustive references, or an observed same-thread TLS identity. Preferred QtCore image base is `0x67000000`; all instruction addresses below are RVAs.

Read-only SHA-256 checks during this task matched:

| Input | SHA-256 |
|---|---|
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| `analysis/wave2-doe-task-qobject-current-thread.asm` | `bb4f6c61aa4879b3a4b7b9a7463f690e2f7a78fadf08dd0e7b60c39c45b29151` |
| `analysis/wave2-doe-task-qobject-thread-tls-initializer.asm` | `c06368be2e4a56cb27af21608f344fa868c383eca125e3ffd1838cd1373014b3` |
| `analysis/wave3-closure-next-6702aca0.asm` | `bf048393fe25dfe285f2470cd818b25fd279a395a875debfd6a5535d7713b411` |

The two wave2 listing hashes match the ownership contract. The wave3 listing independently agrees on the selected entry bytes, but its new hash alone is not an additional runtime qualification. No new disassembly tool, mutable REA database, debugger, native execution, or GUI session was used.

## Exact instruction sequence and stack effects

Let S remain the existing private-initializer-entry symbolic stack origin. CP197 terminal RSP is S; live `[S]` is generation-2 return RVA `0x13144e`. B is the already restored task RBX token, I is the already established caller RSI value (zero on this route), and P is the restored saved-RDI parent token with source-issued known-null qualification. These are model expressions, not concrete native addresses. The selected code does not change RBX/RSI/RDI values.

| RVA | Bytes | Instruction/effect | RSP after |
|---|---|---|---|
| `2aca0` | `40 53` | `push rbx`: `[S-8] = B` | `S-8` |
| `2aca2` | `56` | `push rsi`: `[S-10h] = I` | `S-10h` |
| `2aca3` | `57` | `push rdi`: `[S-18h] = P` | `S-18h` |
| `2aca4` | `48 83 EC 50` | `sub rsp,50h` | `S-68h` |
| `2aca8` | `48 C7 44 24 40 FE FF FF FF` | qword `[S-28h] = 0xfffffffffffffffe` | `S-68h` |
| `2acb1` | `E8 4A E9 FF FF` | `call 29600`: `[S-70h] = 2acb6`, enter initializer | `S-70h` |
| `29600` | `40 57` | `push rdi`: `[S-78h] = P` | `S-78h` |
| `29602` | `48 83 EC 30` | `sub rsp,30h` | `S-a8h` |
| `29606` | `48 C7 44 24 20 FE FF FF FF` | qword `[S-88h] = 0xfffffffffffffffe` | `S-a8h` |
| `2960f` | `48 89 5C 24 40` | qword `[S-68h] = B` | `S-a8h` |

Both immediate stores sign-extend the encoded imm32 -2 to a qword. In particular, the initializer's RBX spill is **S-68h**, not S-8, and its marker is **S-88h**, not the outer marker S-28h. Allocating stack space does not initialize other bytes; do not synthesize zeroed locals or fill the complete reservation.

CALL at `2acb1` is allowed because the exact target and the bounded entered instructions are qualified. It is not an opaque initializer summary: no initializer return is executed. Add a third call occurrence/generation linked to generation 2, callsite `2acb1`, target `29600`, return `2acb6`, caller RSP `S-68h`, callee RSP/slot `S-70h`. Generation 2 remains an active enclosing return at `[S]`; generation 3 is the active innermost return. Generation 1 remains consumed history. Do not replace the enclosing live `[S]` return or misrepresent both active frames as a single live slot. Either extend the live-slot API to return a stack of active call slots or keep explicit outer/inner accessors with a clear innermost selection.

Each newly written stack slot needs source frame/call occurrence, width, instruction RVA, symbolic address, and value provenance. `[S-8]` overwrites the physical slot of the old private initializer's consumed saved RDI; retain that old record as history, not simultaneous live P. Other reused stack-region observations likewise require liveness separate from historical records. No host pointer arithmetic or fabricated numeric stack origin is needed.

## Register and flags model

All general registers except RSP retain their entry expressions; RAX remains its existing private-allocation token, RCX/RDX remain whatever the predecessor establishes, R12 remains the qualified private token. Neither PUSH, MOV nor CALL changes arithmetic flags. Each SUB changes CF/PF/AF/ZF/SF/OF; unrelated flags remain unchanged to the degree represented.

The current source has concrete `CallerArithmeticFlags` for XOR and symbolic `ParentTestOutcome` for TEST, but neither expresses stack SUB flags. Add an explicit current-flags provenance for 64-bit SUB: first operand `S-18h`, immediate `0x50`, result `S-68h`, producer `2aca4`; then operand `S-78h`, immediate `0x30`, result `S-a8h`, producer `29602`. Flag bits are symbolic functions of the same stack-origin expression. Do not manufacture CF/AF/etc. from signed displacement arithmetic or an assumed native stack address. The second SUB replaces the current arithmetic flags; the first remains checkpoint history. Preserve CP194 TEST/CP195 JE records as historical branch provenance, never as current flags after SUB. Existing validators that require parent TEST flags as current must be adjusted for these successor phases while continuing to validate historical operand/branch identity.

This requires model/schema work, not new native evidence. If symbolic SUB flags and nested live-stack representation are not added, the full batch is not source-ready. Do not quietly preserve TEST flags or flatten the call chain. A five-instruction outer prologue fallback stops before `2acb1`, but still requires correct stack liveness and SUB provenance; it does not remove those requirements.

## Terminal guard and no-TLS boundary

The new terminal derives cursor `0x29614` from its phase and has RSP `S-a8h`. The very next instruction is:

```text
29614  83 3D CD 72 2C 00 FF  cmp dword [2f08e8],ffffffffh
2961b  0F 85 9F 00 00 00     jne 296c0
```

Stop before CMP: no TLS-index load, initialization-state conclusion, branch choice, global modification, synchronization or allocation. The initialized-index route would restore/return via `296c0..296ca`; the other route has synchronization/global initialization calls. Neither is selected here. After a hypothetical initializer return, `2acb6` reloads that actual index and `2acbc` calls imported `TlsGetValue`; a nonnull result can reach `2ae42`, while the null result has adoption/allocation and TLS-write paths. Those remain unavailable without separately qualified inputs, effects and owner identity. This plan performs no TLS query, getter result, helper return, private+0x40 assignment, thread reference, QObject counter, TaskInfo work, cache publication, or completion.

Admission must rerun all frozen CP197 owner, source-capability, first Report/task/private occurrence, module, restored-register, TEST/JE-history, call-generation, enclosing return-slot, stack-lineage, counts and incomplete-state checks before mutation. Only exact Session-owned generation-2 entry qualifies production. Standalone synthetic null and nonnull paths keep their CP197 guards. Missing/foreign/copied/stale owner/capability/occurrence, malformed slots, wrong phase/cursor/RSP, premature initializer/global/TLS/result/completion state, and impossible budget/checkpoint combinations refuse without mutation.

## Source/test pair and batch acceptance

After CP197 freeze/review, implement together in `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`, `source/apps/moproj2mst/src/doe/task_factory.cpp`, and focused `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`. Extend existing complete-snapshot/rewind helpers; review the final header/implementation/test pair as one CP198 increment. Update only existing Session expectations affected by the farther terminal, if required by the frozen tests; do not broaden admission.

Use one production batch to the terminal and an internal diagnostic stop ordinal for all ten instruction boundaries. Preconstruct any allocating occurrence/history state before nonthrowing mutations at each committed boundary. Admission/refusal must be nonmutating; a diagnostic interrupted prefix retains exactly its selected completed instruction count. Make subsequent resume and full-batch results identical.

Required tests:

1. Actual Session-issued Report reaches CP197 entry, then full CP198 terminal `29614`, RSP `S-a8h`; exact eight newly written qword slots match the table (three outer saves, outer marker, nested return, inner save, inner marker, inner RBX spill). All other modeled state/counts remain unchanged except defined phase/stack/current-flags/history effects.
2. At every intermediate ordinal assert cursor, RSP, defined slots/liveness, current flags producer, enclosing/inner call tuple and unchanged registers. In particular, check `2acb1` before nested CALL, `29600` after CALL, and `29614` before CMP. No implicit nested-call return or TLS read occurs.
3. Compare direct batch against every split/resume combination; validate terminal reentry without replay. Snapshot newly added weak-owner/control-block identities, occurrence identities, stack widths/values/generations/liveness, nested calls and current/historical flags. Include prior private fields, links, all counts and completion/cache state.
4. Corrupt each nested call tuple/occurrence, enclosing generation-2 return, new stack slot/value/width/producer, saved register source, RSP/phase/current-flags relation, or source capability and assert complete nonmutating refusal. Exercise coherent rewind to each accepted boundary; reject partial rewind and copied frame/slot occurrences.
5. Assert no current TLS-index observation exists at `29614`; forged initialized-index, chosen JNE path, getter result/owner or early private+0x40/thread-ref/counter/completion state refuses. Keep unknown/nonnull/synthetic-only CP197 branch behavior unchanged.

These are structural-model tests, not native execution witnesses. The evidence supports exact instruction mechanics over the qualified predecessor, not proof that the original running process traversed this instance. No GUI-control prerequisite is introduced for this batch; the existing release-lifecycle block still forbids fresh GUI acquisition.

Only this analysis report was written. No source, tests, builds, mutable REA database, parent application, DMO, Claude, original artifact, deployment, or GUI state was changed.
