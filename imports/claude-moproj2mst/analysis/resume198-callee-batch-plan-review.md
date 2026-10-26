# CP198 callee/initializer prefix batch: independent plan review

**PASS for the bounded implementation assignment. BLOCK starting source edits until CP197 is frozen, source-reviewed, and the actual frozen predecessor has been reconciled with this plan.** No concrete byte, stack-effect, flags, or boundary error was found in the reviewed plan. This is a plan review, not acceptance of an implementation or evidence that the original process executed this path.

Reviewed `resume198-callee-prefix-batch.md` SHA-256 `e737419b875c80d86bfe008dece6150640bd85521d0cce86f4091fa8bf80a7eb`. Read the original qualifications in `resume163-connector-qobject-static.json` and `wave2-doe-task-public-qobject-ownership-contract.json`, the CP196 callee investigation, and CP197 plan/review before interpreting the listings. The CP197 planning documents' old CP196 status does not supersede the supplied frozen CP196 predecessor. Current header/implementation inspection is contextual only: CP197 remained owned and unfrozen during this review.

## Independent byte and stack checks

Read-only PE-header/section mapping found preferred image base `0x67000000`. The original `QtCore4_64.dll` independently hashes to `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. All ten instruction byte strings in the plan match the file at their stated RVAs. The first range ends exactly after the CALL, `[0x2aca0,0x2acb6)`; its signed rel32 resolves to `0x29600`, with return `0x2acb6`. The second range is `[0x29600,0x29614)`. No bytes between those image ranges are implicitly executed.

Retained listing hashes also independently match the plan and, for wave2, the ownership contract:

| Listing | SHA-256 |
|---|---|
| `wave2-doe-task-qobject-current-thread.asm` | `bb4f6c61aa4879b3a4b7b9a7463f690e2f7a78fadf08dd0e7b60c39c45b29151` |
| `wave2-doe-task-qobject-thread-tls-initializer.asm` | `c06368be2e4a56cb27af21608f344fa868c383eca125e3ffd1838cd1373014b3` |
| `wave3-closure-next-6702aca0.asm` | `bf048393fe25dfe285f2470cd818b25fd279a395a875debfd6a5535d7713b411` |

With CP197 entry RSP = S, the exact boundary sequence is:

| Executed instruction | Next cursor | RSP | New qword write |
|---|---|---|---|
| `2aca0 push rbx` | `2aca2` | `S-08h` | `[S-08h] = B` |
| `2aca2 push rsi` | `2aca3` | `S-10h` | `[S-10h] = I` |
| `2aca3 push rdi` | `2aca4` | `S-18h` | `[S-18h] = P` |
| `2aca4 sub rsp,50h` | `2aca8` | `S-68h` | none |
| `2aca8 mov [rsp+40h],-2` | `2acb1` | `S-68h` | `[S-28h] = fffffffffffffffe` |
| `2acb1 call 29600` | `29600` | `S-70h` | `[S-70h] = 2acb6` |
| `29600 push rdi` | `29602` | `S-78h` | `[S-78h] = P` |
| `29602 sub rsp,30h` | `29606` | `S-a8h` | none |
| `29606 mov [rsp+20h],-2` | `2960f` | `S-a8h` | `[S-88h] = fffffffffffffffe` |
| `2960f mov [rsp+40h],rbx` | `29614` | `S-a8h` | `[S-68h] = B` |

Both marker immediates sign-extend to 64 bits. The eight written qwords are distinct within this batch; the inner RBX spill is at `S-68h`, not the outer saved-RBX slot `S-8`. The plan correctly retains consumed older stack records as history when physical slots are reused and forbids inventing initialized reservation bytes.

Generation 2 remains active at `[S] = 0x13144e` throughout. Generation 3 is nested at `[S-70h] = 0x2acb6`; generation 1 is consumed history. The planned separate enclosing/innermost representation prevents a single-slot API from silently hiding or replacing a live frame. New occurrence identities, ownership, widths, producers and liveness must be validated at intermediate boundaries as specified.

## Flags, guards and tests

The ten instructions preserve every represented general register except RSP. In particular, saved/restored task RBX, zero RSI, qualified parent RDI, private-token RAX and R12 are not call-clobbered merely because an architectural CALL was entered: no opaque callee summary is being applied.

The first SUB consumes `S-18h` and produces `S-68h`; the second consumes `S-78h` and produces `S-a8h`. Both replace current CF/PF/AF/ZF/SF/OF. PUSH, MOV and CALL preserve those flags. The plan correctly requires symbolic 64-bit SUB provenance rather than deriving flags from signed displacements or carrying the old parent TEST forward. Parent TEST/JE and first-SUB records remain history; the second SUB supplies terminal current arithmetic flags. Existing predecessor validators must become phase-aware without weakening historical branch/operand checks.

The plan requires exact Session-issued production admission and preserves standalone synthetic/unknown/nonnull guards. It includes complete nonmutating corruption refusals, every instruction-boundary diagnostic stop, all split/resume comparisons, terminal reentry without replay, coherent rewind and partial-rewind rejection. Snapshots explicitly include the newly introduced call, slot, owner/control-block and current/historical flags identities. Its per-boundary allocation-before-commit requirement addresses interrupted-prefix consistency. These requirements are sufficient for planning; implementation review must verify them in source and tests rather than infer coverage from their presence here.

## Exact semantic stop

Independent PE bytes at `0x29614` are `83 3d cd 72 2c 00 ff`. RIP-relative decoding resolves the dword comparison to RVA `0x2f08e8`. Stopping with cursor `0x29614` excludes that global read and the following JNE. The plan makes no initialized-index, selected-path, TLS getter/owner, initializer/helper return, private `+0x40`, thread-refcount, QObject counter, TaskInfo, publication or completion claim. Its descriptions of possible later routes remain qualified static possibilities.

No new runtime evidence or GUI acquisition is needed for this structural prefix. No live REA session, mutable database, original execution, build or GUI control was used. Only this review report was written; source, tests, parent application, DMO, Claude, original artifacts and deployment were untouched.
