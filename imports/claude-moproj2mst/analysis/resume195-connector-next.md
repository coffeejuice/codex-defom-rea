# CP195 connector: resolve the caller JE only for qualified parent refinements

## Decision and evidence

Select one bounded modeled increment from the fully admitted CP194 terminal at QtCore RVA `0x131438`: execute only `[0x131438,0x13143a)`, bytes `74 0f`, `je 0x67131449`. With image base `0x67000000`, the taken target is RVA `0x131449`; the fallthrough cursor is RVA `0x13143a`. Stop at the selected successor. Do not include the parent `+8` load at `0x13143a`, the subsequent load at `0x13143e`, or the call at `0x131449`.

Read `analysis/resume163-connector-qobject-static.json` before interpreting `analysis/resume163-connector-qobject-671313d0.asm`. The qualification calls this an unwind-region disassembly of the read-only original, not a complete logical function or runtime proof. Rechecked hashes match the qualification: QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; listing `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The listing decodes `0x67131438` as `je 0x67131449`; fallthrough begins `mov rcx,[rdi+8]` at `0x6713143a`. `analysis/resume194-connector-plan-review.md` independently confirms these boundaries and corrected target/fallthrough distinction.

`analysis/resume194-connector-next.md` and `analysis/resume194-connector-plan-review.md` establish the CP194 predecessor: exact CP193 reverse backlink and prior task/private identity, owner/control-block, stack/call-return and register provenance, with `TEST RDI,RDI` flags at cursor `0x131438`. TEST's ZF is set iff symbolic parent pointer `P` is null; CF/OF clear; SF/PF follow the zero result; AF is undefined. JE consumes ZF. This is a static instruction model, not evidence of native runtime branch execution, fault behavior or complete function meaning.

## Refinement policy and default Session

Treat parent nullness as three-valued in CP195: known-null, known-nonnull, or unknown. Existing CP193/CP194 provenance can carry an unknown symbolic parent expression; it must not be coerced to nonnull merely because `savedEntryRdi.parentIsNull` is false. The retained test `doe_task_factory_cp194_default_session_retains_unknown_parent_expression` explicitly admits the default Session path through CP194, leaves `parentIsNull` unknown, retains the branch outcome with no predicted successor, and confirms one retained prototype and zero tasks.

Preserve that default Session admission. At CP195, validate the full CP194 terminal first, then for an unknown parent return the existing guarded refusal at `0x131438` with a full state/count snapshot unchanged. Do not select either successor, rewrite nullness, or lose the admitted prototype. This is a deliberate unresolved branch boundary, not a failed Session admission. Known-null and known-nonnull refinements may be supplied only through existing test/model qualification seams that bind to the exact CP194 restored-RDI and `[S-8]` provenance; do not invent a concrete pointer or infer a refinement from the native listing.

For known-null, ZF=1 selects taken cursor `0x131449`. For known-nonnull, ZF=0 selects fallthrough cursor `0x13143a`. Record distinct path/branch state so terminal validation cannot confuse the two successors. JE changes only modeled control-flow phase/cursor and path outcome; preserve all registers (including RDI), flags, stack slots, sparse memory observations, QObject/task links, owners, counts, and completion flags. Keep `complete` and `qobjectBaseCompleted` false.

## Focused CP195 obligations

- Extend complete state snapshots with nullness knowledge/refinement, branch decision/path marker, phase-derived cursor, CP194 flag outcome, all inherited identities/provenance, counts, and completion flags.
- For both explicit refinements, exercise one CP194-to-CP195 transition. Assert known-null reaches `0x131449`, known-nonnull reaches `0x13143a`; all non-control-flow state is byte-for-byte/model-equal and counts unchanged. Reentry at either successor must validate and stop without replay, including with a smaller admitted diagnostic budget.
- Exercise the default Session integration path: factory call still returns its existing guarded refusal at `0x131438`; verify the retained prototype and zero task count, unknown nullness, no successor prediction, and full snapshot equality. This locks default admission semantics and ensures the new branch cut does not guess.
- Test consistent rewind/resume for each known branch, restoring CP194 flags/phase/cursor and branch metadata together. Phase-only or branch-marker-only rewinds must refuse.
- Add refusal snapshots for missing/foreign/expired restored-RDI provenance, `[S-8]` mismatch, CP194 flag/ZF disagreement, unknown nullness with a forged successor, wrong branch marker/cursor, phase-only rewind, and any state showing parent-load/call effects or either completion flag. Preserve all CP193/CP194 task-key and corruption refusals and require unchanged allocation/reference counts.
- Keep invalid-budget behavior nonmutating. The exact default/max budget extension belongs to the implementation owner; do not combine this control-flow instruction with either successor's native effects.

## Stop cursor and next boundary

This CP195 cut ends on two separately recorded successors: `0x131449` for known-null/taken and `0x13143a` for known-nonnull/fallthrough. An unknown parent remains admitted at `0x131438` under the existing guard. No `mov`, indirect load, call, TLS/current-thread action, parent/thread-data interpretation, task publication or completion belongs here. Any later cut must select one successor path explicitly and independently.

This is a planning note based on qualified static bytes and retained CP193/CP194 model contracts. No implementation, tests, build, GUI control, original execution, mutable REA database access, parent/DMO/Claude evidence edit, or deployment was performed.
