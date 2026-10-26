# CP185 connector stack-adjustment plan independent review

## Verdict

**PASS — the proposed CP185 cut is consistent with retained static bytes and the CP184 symbolic frame contract.** It covers only `[0x13123a,0x13123e)`, `add rsp,0x30`, and stops before `pop rdi`. The stack arithmetic, cursor, predecessor requirement, and separation of task RBX from private RAX are sound. This is a read-only plan review; no source, build, GUI, or REA database operation was performed.

## Bytes, stack arithmetic, and cursor

The retained QtCore binary qualification is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, image base `0x67000000`. The initializer listing shows `48 83 C4 30` at RVA `0x13123a`, followed by `5f` at `0x13123e` and `c3` at `0x13123f`. The retained caller listing places the call at `0x131417` and return target at `0x13141c`. The static artifacts are bounded unwind-region listings, not proof of full logical-function or runtime behavior.

Using CP184's entry RSP origin S: after `push rdi` and `sub rsp,0x30`, RSP is `S-0x38`. Adding `0x30` yields `S-8`, so the saved entry RDI slot remains at `[S-8]`; the return target remains at `[S]`. The next cursor is exactly `0x13123e`. The plan's later ledger rows are also arithmetically consistent: pop RDI advances RSP to S, and RET consumes `[S]`, leaving RSP at `S+8` and returning to `0x13141c`.

The ADD updates arithmetic flags as a 64-bit addition. Its result and flags may be represented symbolically from the old RSP expression and constant, but no concrete flag values or stack address can be inferred. If flags are absent from the current frame, documenting the effect without creating a new register model is appropriate.

## Predecessor and symbolic identities

CP184's terminal phase is `BeforePrivateStackAdjustment` at `0x13123a`, with restored RBX qualified as the pending task identity and RAX/private identity preserved separately. CP185 correctly requires that exact genuine predecessor and RSP displacement `-0x38`, then retains the task RBX and RAX identities while advancing only modeled RSP displacement and phase/cursor (plus represented symbolic flags).

The source plan properly requires owner/root, ordinal, allocation/private identity, saved-slot and caller/module/initializer provenance to remain linked rather than accepting caller-supplied stack values. It also correctly treats malformed retained provenance as refusal, not something to normalize. The saved-RDI and return-slot relations follow from the CP184 frame contract; no concrete saved RDI value or native stack bytes are established.

## Guards and acceptance coverage

The proposed return-private guard extension permits the established CP184/CP185 successor phases while keeping the result unavailable at earlier cursors. The plan preserves CP183/CP184 diagnostics and rewind semantics, clears only stage-dependent restored-RBX state when rewinding before its MOV, and avoids replaying ADD when rewinding from CP185 to CP184. Validating the terminal frame before same-key or different-key reentry refusal is consistent with the existing state-integrity requirement.

Acceptance checks cover the successful displacement/cursor transition, complete state/reference snapshot preservation, malformed predecessor and saved-slot provenance, foreign identities and controls, both reentry forms, prior boundaries/rewinds, no duplicate ADD, and explicit unavailability of RDI restore, RET, caller continuation, and backlinks. That is sufficient for this one-instruction cut, assuming the implementation's existing snapshot helpers cover all represented fields as stated.

## Qualification

The conclusion is limited to instruction bytes, symbolic stack relations, and the CP184 planned source contract. It does not establish live stack contents, concrete arithmetic flags, runtime constructor behavior, or successful integration tests. The original comparison, focused checks, full regression, source-stable freeze, and retained receipts remain pending per the plan.
