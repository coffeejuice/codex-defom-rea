# CP183 connector plan independent review

## Verdict

**PASS — source-ready for the proposed one-instruction cut.** The plan selects the correct retained instruction, preserves CP182's terminal state, and stops before the first epilogue instruction. No factual blocker was found. This is a static evidence and source review only; no source, build, test, GUI, or REA database was changed or run.

## Retained bytes and register identity

The retained QtCore DLL hashes to `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, matching the plan. Using its stated `.text` mapping (`raw = RVA - 0xc00`), `[0x131232,0x131235)` is `48 8b c3` and hashes to `88de8531189088e84aea757958f351c870c008322498ae012d5c1e52fc597037`. The retained listing independently decodes it as `mov rax,rbx`.

RBX is loaded with the private allocation in the listing at RVA `0x131178`; the intervening selected path through `0x131231` has no RBX write, branch, or call after the qualified equal-version path. Thus the move establishes identity `RAX = allocatedPrivateIdentity`. It does not establish a portable host pointer or change the private object. A symbolic identity slot is the correct model.

## Cursor and excluded epilogue

The proposed half-open cut ends exactly at `0x131235`, the start of `mov rbx,[rsp+0x48]`. The following bytes independently match the plan and their individual hashes:

- `[0x131235,0x13123a)`: `48 8b 5c 24 48`, restore RBX.
- `[0x13123a,0x13123e)`: `48 83 c4 30`, adjust RSP.
- `[0x13123e,0x13123f)`: `5f`, restore RDI.
- `[0x13123f,0x131240)`: `c3`, return.

These are correctly excluded. The proposed cursor names the point before the epilogue and must not imply that the native function has returned. QObject completion and the caller's task/private backlink stores remain subsequent work.

## CP182 source guards

Current CP182 code has distinct cursors through `BeforePrivateReturnSetup`; its last cursor means the private `+0x20` dword write completed. Before that terminal state, it requalifies the retained allocation/root and bindings, checks the Qt module/version call provenance, checks the exact sparse-zero prefix, and verifies the staged EAX expression and matching stored `private20` expression. At its final ordinal, the expected value is the stable allocation-bound expression `(B & 0xfffffcf0) | 0x60`; the code does not concretize B. This supports the proposed next-step guard.

Implementation should retain those checks at the new transition, additionally require the return slot absent at CP182 entry, and publish only the same allocation identity plus the new phase. It should refuse malformed/foreign identity or corrupted prefix without mutation. `allocatedPrivateIdentity` is currently a pointer to the owned sparse object used as an identity token; the new slot should carry that token without dereferencing it or exposing its numeric address.

## Focused-test review

The proposed acceptance matrix is appropriate. Existing CP182 tests already exercise the five symbolic-flags cursors, expression provenance/stage corruption, sparse-prefix damage, owner/root/binding/version rejection, and complete state/reference-delta preservation. Extend those established patterns for the new terminal transition: successful identity equality and unchanged CP182/root state; refusal snapshots; same-key and different-key reentry; and diagnostic rewinds that clear the new slot. Update all terminal-frame snapshot/equality helpers so the added slot cannot be omitted silently. At the new terminal cursor, a repeated factory call should refuse without changing the slot or replaying the move.

## Blockers

None identified. The review does not claim the cut has been implemented, tested, frozen, or validated against a live constructor occurrence.
