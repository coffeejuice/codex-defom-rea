# CP196 connector: investigate the nonnull parent slot before modeling its load

## Decision

Keep the single fallthrough instruction `[0x13143a,0x13143e)`, bytes `48 8b 4f 08`, `mov rcx,qword ptr [rdi+8]`, guarded and unmodeled for now. The next work item is the smallest preparatory investigation: determine whether the converter's existing symbolic parent input has a source-backed, live owner that can issue and admit a parent `+8` slot observation. Do not add a synthetic memory read or select an arbitrary symbolic `T` until that issuer/admission contract is established.

The candidate instruction would start only from CP195's known-nonnull `BeforeCallerParentLoad` terminal at RVA `0x13143a` and would stop at `0x13143e`, before `mov rax,[rcx+40h]`. The null-parent/taken terminal at `0x131449` remains a separate guarded call successor. CP194's unknown-parent guard at `0x131438` remains unchanged; unknown cannot be converted to nonnull or advanced to either successor.

## Qualified original evidence and limits

Read `analysis/resume163-connector-qobject-static.json` before interpreting the retained listing. It qualifies the captured region as a read-only original unwind-region disassembly, not a complete logical function or runtime proof. The original QtCore SHA-256 is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the caller listing `analysis/resume163-connector-qobject-671313d0.asm` SHA-256 is `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`, matching the qualification. At preferred VA `0x6713143a`, bytes `48 8b 4f 08` load eight bytes from `[rdi+8]` to RCX; the next instruction starts at VA `0x6713143e`. Subtract image base `0x67000000` for RVAs `0x13143a` and `0x13143e`.

These bytes do not establish that nonnull P is mapped or readable, the value at `P+8`, or a model owner for that memory. They do not justify a native value or a fault/result claim. A test-created opaque value alone would be a model assumption, not evidence that the exact source slot has an admitted value.

## Current admission boundary

CP195's `SavedEntryRdi` is an allocation-owned symbolic token for incoming RDX, with report/task/private identities, module digest, initializer/save/call/return lineage, and optional three-valued nullness. POP restores that same token and its nullness; TEST and JE bind their results/path to it. The current frame has no parent-object memory owner, slot map, read observation, read issuer, or admitted value for `[P+8]`.

`detail::TaskFactoryTestAccess` exists in `doe_task_factory.hpp` as a friend seam, but its current implementation in `tests/mdt/test_doe_task_factory.cpp` only sets diagnostic stop ordinals and replaces the pending allocation ordinal. Those operations do not supply a parent slot or validate a read. Neither that test helper nor the CP195 nullness refinement seam is presently a justified issuer for a slot value. Do not describe either as one.

## Smallest preparatory investigation

1. Trace only the symbolic incoming-RDX/P boundary through the converter's existing runtime/model types and fixtures. Determine whether any existing source-backed object or input record owns the parent object and can represent its `+8` slot with lifetime and identity tied to the CP195 live frame.
2. If such an owner exists, document its exact issuer and admission path before planning the load: who creates the slot observation, what live CP195 state it validates, how it binds to the saved/restored RDI token and owner/task/private identities, and how stale/foreign/aliased observations are rejected. The acceptance contract must be executable and testable, not a caller-populated record that self-asserts its own provenance. It may establish model-input consistency only; it must not claim native readability or native slot contents without separate evidence.
3. If no existing source-backed owner/issuer exists, retain the guard at `0x13143a` and identify what additional evidence or model boundary is required before the read can be admitted. Do not invent one in this planning increment.

No source/test/build change is authorized by this plan. If a later implementation is separately selected after the investigation, it must revalidate the full CP195 live frame before mutation, including allocation/control-block owner, report ordinal, task and private identities, exact saved/restored RDI token, `[S-8]` lineage, module digest, `parentIsNull == false`, `ParentTestOutcome` and JE path marker, phase-derived cursor `0x13143a`, counts and incomplete flags. It must preserve the CP194 unknown guard and CP195 null/taken terminal. Any unadmitted, missing, stale, foreign, aliased, or mismatched observation must refuse at `0x13143a` with complete snapshot and counts unchanged.

## Conditional future boundary and tests

Only after an issuer/admission contract is established should a follow-on plan reconsider executing `[0x13143a,0x13143e)`. Its potential modeled effect is RCX receiving the admitted eight-byte model observation; it must preserve RDI, flags (including undefined AF), stack, QObject/task state, links, owners, counts, and incomplete flags, then stop at `0x13143e` before the indirect load. Treat this effect as model-state transition only, not proof that a native read occurred or that native memory was readable.

At that point, tests would need to cover valid issuer output from an exact CP195 known-nonnull live frame; complete-state equality apart from RCX/read-observation/phase; terminal reentry and atomic rewind/resume; and unchanged-snapshot refusals for unissued/self-forged, missing, stale, foreign, aliased, token/owner/task/private/module/source/offset/width mismatches, unknown/null branch, wrong phase/cursor/marker, invalid budgets, or any effect from `0x13143e` or later. Existing default Session assertions must continue to retain one prototype, zero tasks, unknown parent, and the CP194 refusal at `0x131438`; the taken null-parent terminal at `0x131449` must remain unchanged.

## Stop and limits

Until the investigation identifies a justified issuer, CP196 stops before RVA `0x13143a`. It does not execute the load, choose RCX, model a fault, read `[rcx+0x40]`, execute the taken-path call, or infer QObject/task semantics. This is a planning decision based on qualified static bytes and current CP195 contracts, not implementation, build, test, GUI, runtime-capture, or deployment evidence.
