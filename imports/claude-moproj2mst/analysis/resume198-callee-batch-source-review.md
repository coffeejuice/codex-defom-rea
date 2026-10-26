# CP198 callee/initializer prefix: independent source review

## Decision: PASS for the bounded source implementation

No concrete source defect was found in the exact reviewed four-file increment. The implementation preserves the qualified predecessor, models the ten selected instructions, and stops before the first TLS-index/global read. This is a source review: root still owns compilation, executable regression, and the successor freeze. The inspected tests support the named mechanisms; their corruption matrix is not exhaustive over every nested-call field, as detailed below.

Compared all four complete files line by line with their exact `apps/moproj2mst/...` entries in `build/wave3-checkpoint197-verified/source.zip` (archive paths omit `source/`). The archive SHA-256 independently matches its receipt: `5305bb92fe8a78a0378c0e73e9846d7f2cdfe80de596ed4c6a811009b8a742b1`; the receipt identifies CP197 source tree `c90622460bde430543513cb2e32675006274d3315a7d5d77faeac8ca238ea8f2`. Its four predecessor file hashes match the repaired PASS CP197 review. `conversion_session.cpp` is byte-identical to the archive. Read the CP198 batch plan, PASS plan review, implementation report, CP197 repaired source review, `resume163-connector-qobject-static.json`, and `wave2-doe-task-public-qobject-ownership-contract.json` before interpreting the original evidence.

## Independent mechanics checks

Read-only PE section mapping and direct bytes from the retained QtCore file independently matched every selected instruction. Its SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The two wave2 listing hashes and wave3 callee-entry listing hash also match the plan. The qualification remains bounded static/unwind evidence, not an observed native traversal or complete logical-function contract.

The switch in `task_factory.cpp:308` and header cursor/RSP projections reproduce this control-flow order. S is the existing symbolic initializer stack origin; B/I/P retain the predecessor's task RBX, zero RSI, and source-qualified null parent RDI tokens.

| RVA | Exact bytes | Effect | Next cursor / RSP |
| --- | --- | --- | --- |
| `2aca0` | `40 53` | `[S-8]=B` | `2aca2 / S-8` |
| `2aca2` | `56` | `[S-10h]=I` | `2aca3 / S-10h` |
| `2aca3` | `57` | `[S-18h]=P` | `2aca4 / S-18h` |
| `2aca4` | `48 83 EC 50` | SUB `50h` | `2aca8 / S-68h` |
| `2aca8` | `48 C7 44 24 40 FE FF FF FF` | `[S-28h]=-2` qword | `2acb1 / S-68h` |
| `2acb1` | `E8 4A E9 FF FF` | `[S-70h]=2acb6`, CALL | `29600 / S-70h` |
| `29600` | `40 57` | `[S-78h]=P` | `29602 / S-78h` |
| `29602` | `48 83 EC 30` | SUB `30h` | `29606 / S-a8h` |
| `29606` | `48 C7 44 24 20 FE FF FF FF` | `[S-88h]=-2` qword | `2960f / S-a8h` |
| `2960f` | `48 89 5C 24 40` | `[S-68h]=B` | `29614 / S-a8h` |

The relative CALL resolves exactly to `29600`, return `2acb6`. Both markers use sign-extended `fffffffffffffffe`; the inner RBX spill is `S-68h`. Eight sparse qword observations are created, with width, producer, symbolic address, symbolic/numeric value, source occurrence and call generation. No reservation fill or numeric native stack base is introduced.

Generation 1 stays consumed history. The inherited generation-2 tuple and enclosing live `[S]=13144e` remain validated (`task_factory.cpp:643`). Generation 3 independently records `2acb1 -> 29600 -> 2acb6`, caller RSP `S-68h`, callee/slot `S-70h`, and the enclosing occurrence. `liveReturnSlot()` selects the inner slot after CALL; `activeReturnSlots()` preserves both. `entrySavedRdiSlotLive()` is false after the earlier POP, so the retained old `[S-8]` saved-RDI record is history while the new RBX write is live.

Both SUB records describe six symbolic 64-bit flag functions over the same source-bound S, including auxiliary borrow. The first records `(S-18h)-50h = S-68h` at `2aca4`; the second records `(S-78h)-30h = S-a8h` at `29602`. The current projection selects the first at completed ordinals 4..7 and the second at 8..10. PUSH/MOV/CALL preserve it. Parent TEST/JE remains checked history, rather than current flags after SUB. No concrete CF/AF/etc. is fabricated from displacement signs. Register tokens remain unchanged apart from RSP.

## Admission, authenticity, and commit boundaries

The existing exact Session capability/owner/control-block, first Report/task/private allocation, initializer occurrence, saved/restored register, parent TEST/JE, enclosing CALL/slot, sparse field, refcount-marker and incomplete/cache guards extend through every new phase. Standalone unknown, null and nonnull routes retain their inherited restrictions. Every resume revalidates the complete retained predecessor before entering `advanceCalleePrefix`; each internal instruction boundary then validates the exact Session binding and immutable CP198 state before stopping or advancing.

The private `issuedCalleePrefix_` is independent of the public frame handle. After structural validation, `task_factory.cpp:267` checks both live pointer and ownership control block against it. A coherent copied/rebound boundary cannot appoint itself, even with correctly rebound nested slots/occurrences/current flags. Actual states and children are created as `shared_ptr<const ...>` objects, with no normal mutable alias. Scope release resets the private handle. The explicit friend rewind seam restores the private anchor together with the genuine cached public boundary, phase and RSP; public-only rewind remains rejected.

All potentially allocating copies, new occurrence identities, slots, nested call, SUB flags, and the final immutable boundary are prepared before publication (`task_factory.cpp:285..329`). Publication consists of nonthrowing shared-handle assignments and scalar phase/RSP changes. Allocation failure therefore leaves the preceding committed boundary intact. Invalid budgets are rejected before allocation. A valid smaller diagnostic budget at an already completed boundary stops unchanged; terminal reentry cannot replay.

The last phase derives cursor `29614`; its bytes are `83 3D CD 72 2C 00 FF`, addressing RVA `2f08e8`. The implementation has no case for that CMP, JNE, TLS index/result/owner, initializer/helper return, or later thread/counter/publication/completion. No native/global read is performed. Premature private `+40` and completion corruption remain refused.

## Test inspection and limits

The seven added Testkit cases inspect all eleven boundaries, eight exact slot writes, symbolic current/history flags, active generations, default Session terminal, invalid budgets before allocation/after retention, 66 ordered split pairs, genuine cached-boundary rewinds, smaller-budget terminal reentry, public-only rewind, and copied state/slot/nested-call/flags authenticity. Damaged-state snapshots retain prior fields/counts/cache plus CP198 payloads and ownership checks. Exact provenance/error assertions distinguish refusal from an ordinary terminal stop. The every-slot matrix individually corrupts width, producer, address and value for all eight slots. The old CP197 fixtures explicitly pin the new diagnostic budget to zero; standalone branch behavior remains inspected under that predecessor contract.

One coverage limit should not be described as exhaustive: the 32-case corruption matrix changes several nested-call fields but does not individually corrupt every checked tuple member, such as `precedingGeneration`, nested `sourceBinding`, `callerRsp`, `calleeRsp`, `slotFromEntryS`, or a missing private CP198 anchor. The source explicitly validates those fields and the independent exact immutable anchor covers replacement objects, so this review found no bypass. Additional cases would improve per-field regression coverage. The split tests verify each resulting boundary and restore/check the full unchanged predecessor; they do not compare independently issued occurrence identities across separate executions, which would be inappropriate.

No executable test, compilation, GUI control, mutable REA database action, original execution, deployment, or parent/DMO/Claude edit was performed. Only this report was written.

## Exact reviewed bytes

| File under `source/apps/moproj2mst/` | SHA-256 |
| --- | --- |
| `src/doe/task_factory.cpp` | `39f4e03463eb5c2361bc7ac20267e745e884a4f84ae671f59a401c7877877c43` |
| `include/m2m/doe_task_runtime.hpp` | `6d32dd6e2cc22cd40f3a95fcbec8a2b98af85317efc7d63313088f29fc201985` |
| `include/m2m/doe_task_factory.hpp` | `2f00cd1a5a821f60099c595a68f7b1bef6c04a5bf881d8950bb9626360d25283` |
| `tests/mdt/test_doe_task_factory.cpp` | `463e14661180360f221f2f643ff7798aa543697158f1991538bf9a5c8fc7073f` |
