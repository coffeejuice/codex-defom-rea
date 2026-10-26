# CP184 connector independent source review

**PASS — no source blocker found for the saved-RBX restoration cut.** The current implementation matches `resume184-connector-next.md`, its independent plan review, and `resume184-connector-implementation.md`. This is a read-only source/test review, with only this report written. No build, test execution, GUI, process capture, or REA database operation was performed; integration and runtime gates remain separate.

## Exact identity and symbolic stack contract

The qualification in `resume163-connector-qobject-static.json` limits the retained listings to unwind regions or bounded candidates, not proof of a complete logical function. The initializer and caller listings support the selected normal-path relation: entry RSP S becomes S-8 after `push rdi`, then S-0x38 after `sub rsp,0x30`. Saving RBX at `[rsp+0x48]` therefore writes `[S+0x10]`. The public caller places its task/this identity in RBX before calling the initializer, while the initializer replaces live RBX with its private allocation identity. Thus the restored RBX must denote the task, independently of the private identity retained in RAX.

`doe_task_runtime.hpp:126-141` represents exactly that contract with `SavedTaskRbx`, symbolic signed displacements, owner/ordinal/task/private/module/entry/call/return provenance, and a separate restored-task result. It introduces no numeric stack address, native stack capture, or assumed saved-RDI value. `task_factory.cpp:175-177` derives the retained contract during frame creation. Lines 193-205 validate it before resumed writes, including both ledger pointee equality and control-block ownership identity. Missing or corrupted retained provenance is refused rather than regenerated. The final instruction path also requires the saved private origin to equal the allocation-time identity after the complete prefix qualification.

## Transition, cursor, and provenance

`task_factory.cpp:562-636` preserves ordinals zero through five and adds ordinal six as `BeforePrivateStackAdjustment`, RVA `0x13123a`. The selected retained bytes at `0x131235` are `48 8b 5c 24 48`, so the half-open range `[0x131235,0x13123a)` is exactly one MOV. The case-five transition publishes only `restoredTaskRbxIdentity = savedTaskRbx->taskIdentity`, then advances phase through nonthrowing pointer/enum assignments. RSP remains symbolically S-0x38.

The CP183 return token remains allocation-bound at both its old terminal phase and the new successor. The early guard allows it only at those phases, so earlier cursors still reject premature RAX publication. `flags20Eax` remains explicitly historical after CP183, and full expression equality preserves its unknown allocation origin, stage, masks, and the private+0x20 expression `(B & 0xfffffcf0) | 0x60`. CP184 neither assigns that expression to live EAX nor changes RAX, private+0x20, flags, or any sparse private field.

## Refusal, reentry, rewinds, and excluded effects

The new terminal path revalidates the complete owner/root/version/address-point/sparse/expression prefix and restored task identity before the diagnostic stop. The default budget is six, and budgets above six refuse before factory or allocation creation. At ordinal six the stop always throws, making the subsequent phase-array indexing unreachable there. Same-key and different-key reentry therefore perform no second transition or requested-key lookup. The native `createTaskBase` path initializes before key dispatch and cannot expose a completed task at this cut.

Earlier diagnostic rewinds clear both stage-dependent identity results and flags/store observations while retaining the symbolic prologue contract. The common validation detects corrupted retained stack provenance before resumed writes. Existing CP183 ordinal-five stops and dedicated tests remain available.

There is no implementation of the following ADD, POP, RET, caller continuation, or either backlink publication in this cut. Full-prefix qualification rejects premature QObject/base completion and backlink flags. The source stops with empty native factory maps and pending construction; no caller cursor, saved-RDI value, or public completion is manufactured.

## Test-source audit and remaining gates

`test_doe_task_factory.cpp:66-145` extends full-frame snapshots with all saved-RBX scalar provenance, the restored result, and independent weak ownership/control-block comparison. CP184 tests at lines 5161-5291 cover the exact predecessor/terminal difference, distinct task/private roles, same/different-key reentry, 34 damage cases at predecessor and terminal, all six earlier flags/return diagnostic stops, and refusal to repair corrupted saved-stack provenance. The predecessor comparison removes only the result and phase before comparing the full snapshot, so it also checks unchanged RAX, historical expression, sparse state, root references, allocation counts, and factory state.

Dedicated CP183 tests at lines 5057-5159 still select ordinal five. Invalid-budget tests now start at seven. Earlier rewind helpers clear the new result; default-terminal expectations point to `0x13123a`. A targeted search of the other active connector/factory tests found no leftover CP182/CP183 cursor expectation. No stale active test expectation was identified by this static review. Historical checkpoint names remain useful labels and do not imply an older default boundary.

No test pass or binary acceptance is claimed here. Compilation, focused and required regression execution, candidate freeze chronology, and source-bound original comparison remain the integrating agent's gates.
