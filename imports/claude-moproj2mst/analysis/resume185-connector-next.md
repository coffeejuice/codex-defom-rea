# CP185 connector: private stack adjustment contract

## Decision

The next exact cut after CP184 is the single instruction `[0x13123a,0x13123e)`, bytes `48 83 c4 30`, `add rsp,0x30`. It starts from the CP184 terminal state where the saved caller RBX has been symbolically restored, and stops at RVA `0x13123e`, immediately before `pop rdi`. Do not include the RDI restore or RET.

## Static qualification and stack contract

Use the existing retained QtCore4_64.dll qualification: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`; initializer and caller listings and their scope qualifications are recorded in `analysis/resume163-connector-qobject-static.json` and `analysis/resume163-connector-qobject-67131160.asm` / `...671313d0.asm`. The CP184 independent plan review verifies the five-byte MOV at `0x131235`, this ADD at `0x13123a`, `pop rdi` at `0x13123e`, and `ret` at `0x13123f`. These are static instruction facts, not live stack observations or proof of full logical-function/runtime behavior.

Keep the frame-relative symbolic entry RSP origin `S` from CP184. Before this ADD, RSP is `S-0x38`; after it, RSP is `S-8`. The stack slots remain described symbolically: saved entry RBX at `[S+0x10]` was consumed by CP184, saved entry RDI remains at `[S-8]`, and the return target remains at `[S]` for the later RET. No numeric stack address is modeled. Preserve the independently qualified saved-RBX/task identity, CP183 RAX/private identity, and CP184 phase/result.

ADD changes arithmetic flags based on the RSP addition. The immediate is positive `0x30`; model the resulting arithmetic-flag expression symbolically from the old RSP expression and constant only if the existing frame represents flags. Do not assign concrete flag bits. If no flag state exists, document the flag effect without inventing a register model. This instruction changes no memory, ownership, allocation, private fields, shared references, or backlinks.

## Proposed transition and guard

Require a genuine CP184 terminal frame at `BeforePrivateStackAdjustment` / RVA `0x13123a`; validate its retained owner/root control blocks, pending Report ordinal, private allocation identity and RAX identity, restored RBX identity, saved-RDI slot relation, stack origin/provenance, and prior qualified caller/module/initializer/version provenance without normalizing corrupt state. Require symbolic RSP displacement `-0x38` at entry. Reject a missing/foreign restored RBX or malformed frame before mutation.

On success, atomically record RSP displacement `-8` and advance phase/cursor to `BeforePrivateRdiRestore` / `0x13123e`. Keep the RBX restore result, RAX/private identity, saved RDI relation, return slot, historical `flags20Eax` and `private20`, sparse private state, ownership/reference deltas, allocation identities, maps, completion flags, and backlink availability unchanged. Do not expose stack memory as captured native contents.

The CP183 guard that limits `returnPrivateIdentity` to `BeforePrivateEpilogue` must permit the established CP184 and CP185 successor phases while continuing to reject that result at earlier phases. Retain CP183 and CP184 diagnostic stops and rewind semantics. A rewind before CP184's MOV clears only stage-dependent restored-RBX state; a rewind from CP185 to CP184 preserves valid CP184 state and must not replay the ADD. Validate terminal state before reentry refusal; both same-key and different-key reentry must refuse mutation-free.

## Cursor ledger

| Cursor | Instruction | Post-state / next cursor |
|---|---|---|
| `0x13123a` | `48 83 c4 30` — `add rsp,0x30` | RSP=`S-8` (displacement `-8`); cursor `0x13123e`; arithmetic flags updated symbolically if represented. CP185 stop. |
| `0x13123e` | `5f` — `pop rdi` | RDI becomes saved entry RDI; RSP=`S`; cursor `0x13123f`. Not part of CP185. |
| `0x13123f` | `c3` — `ret` | RIP becomes caller RVA `0x13141c`; RSP=`S+8`. Caller continuation, not fallthrough at `0x131240`. Not part of CP185. |

## Acceptance before integration

1. From a valid CP184 predecessor, one ADD transition reaches `0x13123e`, updates symbolic RSP displacement from `-0x38` to `-8`, and preserves restored task RBX and private RAX identities.
2. Compare a full frame/owner/reference snapshot: only the modeled RSP stage, phase/cursor, and represented symbolic flags may change; all private expressions, sparse stores, reference counts, owner bindings, allocations, and backlinks remain identical.
3. Refuse unchanged for wrong/earlier cursor, absent or foreign CP184 restored RBX, wrong stack-origin/frame provenance, wrong input RSP displacement, corrupted saved-RDI/return-slot relation when represented, foreign ledger/control block/ordinal/module/caller/initializer/version/allocation/private identity, and every prior malformed predecessor case.
4. At CP185, refuse missing/foreign restored identity or corrupted provenance unchanged. Preserve both CP183 and CP184 boundary tests, all earlier cursor refusals, and rewinds; ensure no second ADD on reentry and no allocation/reference/map side effects.
5. Assert that RDI restoration, RET, caller continuation at `0x13141c`, and both backlinks remain unavailable. Do not infer saved RDI's value or publish either backlink.
6. Keep the original comparison, focused checks, full regression, source-stable freeze, and retained receipts required by the current implementation plan. No such validation is claimed by this read-only preparation.

