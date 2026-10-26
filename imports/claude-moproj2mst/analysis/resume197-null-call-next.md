# CP197 null-parent CALL boundary: conditional structural increment

## Decision

The architectural CALL at QtCore RVA `0x131449` is a useful, precisely bounded next model increment, but it is **not source-ready today**. Its eventual scope is one CALL transition from caller RVA `0x131449` to callee entry RVA `0x2aca0`, with return RVA `0x13144e`. Adding explicit call-generation history and a live symbolic return-slot representation is part of implementing this increment; it is not an external prerequisite. No callee instruction or whole-call behavior belongs in CP197.

CP197 may begin only after CP196's actual Session-only Report-null issuer and associated tests have been implemented, frozen, and source-reviewed. The CP196 plan/review currently describes and approves a proposal, not a verified source freeze. The current default Session therefore remains at its existing unknown-parent guard at RVA `0x131438` until CP196 is complete. After CP196 is verified, that specific Session route should carry an issued known-null token through CP195 JE and stop at `BeforeCallerNullParentCall`, RVA `0x131449`. Standalone synthetic null/nonnull diagnostics remain distinct and cannot authorize production admission.

## Qualified original boundary

The retained `resume163-connector-qobject-static.json` qualifies `resume163-connector-qobject-671313d0.asm` as a read-only original QtCore unwind-region listing, not a complete logical function or runtime execution witness. The qualified QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the caller listing SHA-256 is `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The relevant bytes are:

```text
VA 0x67131449 / RVA 0x131449  E8 52 98 EF FF  call 0x6702aca0
VA 0x6713144e / RVA 0x13144e  49 89 44 24 40  mov [r12+40h],rax
```

CP195 maps known-null TEST/JE provenance to `TakenNullParent`, phase `BeforeCallerNullParentCall`, and cursor `0x131449`. Under the CP196 proposed source qualification, the first Session-owned Report token is expected to be known-null and this becomes the production predecessor after that plan is implemented and verified. Until then, the default Session's current unknown state remains stopped at `0x131438`. The original disassembly establishes instruction bytes, target, and architectural return address, not native reachability, a live stack instance, or helper semantics.

The existing `wave3-closure-next-6702aca0.asm` is a bounded listing for the callee. Its entry starts with `push rbx; push rsi; push rdi; sub rsp,50h`; later it reads the module TLS index at `0x672f08e8`, calls the imported `TlsGetValue` slot at `0x671a80a8`, and has distinct paths according to the returned value. This static listing does not establish index initialization, the same-thread result, a thread-data owner, dynamic path selection, or a complete effect/return contract.

## CP197 transition to implement after CP196 freeze

Admission must validate the exact CP196 source-issued known-null Report token on CP195's `TakenNullParent` terminal: Session/factory/store/widget ownership and control blocks, first Report occurrence and task/private identities, capability and frame/token occurrence, saved/restored RDI identity and nullness, `[S-8]` lineage, CP194 TEST flags/ZF, CP195 JE marker, phase-derived cursor `0x131449`, stack lineage, counts, and incomplete flags. It must refuse unknown or nonnull state, synthetic-only qualification, retained unqualified frames, missing/replaced/foreign/stale/copied capability, or any inconsistent inherited state without mutation.

Implement the architectural transition `[0x131449,0x13144e)` as one atomic model step:

- Add a distinct call-generation record bound to this exact caller frame and occurrence. It records call-site RVA `0x131449`, target RVA `0x2aca0`, expected return RVA `0x13144e`, and the symbolic stack origin/slot identity. Do not overwrite the existing `QObjectPrefix::ReturnToCaller` initializer record (`callRva 0x131417`, target `0x13141c`) or mistake it for the new call.
- Add a live symbolic return-slot record for `[S]`, including its value `0x13144e`, generation identity, and binding to the new call record. Before this CALL, the symbolic SP is `S+8`; the earlier initializer return in `[S]` has already been consumed. Preserve that earlier tuple as historical provenance, while the live slot advances to the new generation. Both values must not appear simultaneously live.
- Perform only the CALL effects: write the symbolic return address in the live `[S]` slot, change RSP from `S+8` to `S`, and change the cursor/phase to a distinct callee-entry terminal at RVA `0x2aca0`. Stop before executing `push rbx`.
- Preserve flags, all registers, private fields, QObject/task links, owner/reference counts, other stack/memory observations, and completion markers. Do not clear RAX, invent RCX/RDX values, set private `+0x40`, change reference counts, query TLS, claim a helper result, execute `push rbx`, or advance to `0x13144e`.

The call-generation history and live-slot generation/value are new model state in this increment. Add them to all complete snapshots and validators. A stale initializer record, forged callsite/target/return tuple, wrong stack slot/generation, mismatched caller occurrence, wrong RSP/lineage, phase/cursor disagreement, or partial rewind must refuse with the full state and counts unchanged. Rewind must restore the CP195 terminal, prior live-slot generation, call history state, RSP, phase, and cursor coherently. Reentry at the callee-entry terminal validates and stops without replay.

## CP196 freeze gate and independent evidence limit

Before starting CP197 implementation, verify CP196's actual source changes against its PASS plan review, freeze the resulting source, and obtain focused source review confirming the private Session issuer, exact first pending Report binding, owner/token identity checks, downgrade/replacement refusals, and complete snapshots. Do not infer that freeze or tests exist from the planning documents. This is a prerequisite for valid production admission, not a demand for new original-runtime evidence.

There is no separate TLS/owner prerequisite for the isolated CALL-only state transition. TLS and owner evidence remains an independent hard boundary on any claim about callee execution or completion: do not model a TLS query/result, choose the helper path, claim a return, assign the value subsequently stored at `[r12+0x40]`, or claim QObject construction. Any future callee/return increment must qualify actual module TLS index initialization, same-thread `TlsGetValue(index)` and the returned owner identity/lifetime, as well as relevant effects and paths. The null-result slow path, initialization/synchronization, allocation/adoption, TLS writes, failure/unwind, and external effects remain separately guarded absent their own evidence.

## Acceptance checks

After CP196 is frozen and reviewed:

1. Test actual Session-issued known-null admission through the existing CP195 branch to the CALL predecessor; retain standalone synthetic tests as diagnostics only. Until then, assert the existing Session unknown guard at `0x131438` remains intact.
2. Test the exact CALL transition: cursor `0x131449 -> 0x2aca0`, live `[S] = 0x13144e`, RSP `S+8 -> S`, distinct call generation, preserved initializer history, and stop before the first callee instruction. Snapshot all other state to prove only the defined CALL effects changed.
3. Add complete snapshot fields for caller/frame occurrence, CP196 capability identity, call-generation history, live slot value/generation, stack lineage/RSP, phase/cursor, registers, flags, links, owner identities, counts, and completion state.
4. Test nonmutating refusals for unknown/nullness mismatch, nonnull parent, synthetic-only binding, missing/foreign/stale/copied/replaced CP196 capability, wrong Report/task/private/frame/owner, forged callsite/target/return, stale or wrong slot generation, wrong stack lineage/RSP, inconsistent TEST/JE provenance, premature private `+0x40`/refcount/TLS/result state, invalid budget, partial rewind, and terminal replay.
5. Keep CP194/CP195 guards and all separate branch meanings. A source-qualified Session known-null terminal may advance only through this CALL instruction; no fallthrough load or callee instruction is included.

## Scope and evidence statement

This is a planning report only. It consumes the recorded original-image qualifications and bounded listings; it does not independently rehash or execute the images. No source, tests, build, GUI, mutable REA database, parent/DMO/Claude/original evidence, or deployment was changed.
