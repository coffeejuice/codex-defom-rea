# CP196 R300 next-source audit

## Decision

No source-ready next item is pinned. CP195's selected `dieStressFromDbStep` remains the active owner and is explicitly not treated as a frozen delta here. The next investigation should stay in L5, but the current inventory does not establish a minimal owner with a fixture that actually exercises the step-owned interval. Do not wrap a one-store callback or add a repeat-only test.

## Frozen baseline and L5 candidates

The source baseline is `build/wave3-checkpoint194-verified/source.zip` (SHA-256 `e837f823a179e5039ea70a05bc2820c0b16c081ea8d28e9dbed9ca39435de2b2`), corroborated by `analysis/wave3-checkpoint194-validation.json`. CP195's reviewed L6 pair was confirmed byte-identical to this archive in `analysis/resume195-r300-l6-plan-review.md`. CP173 is a historical direct-step inventory, not proof that its listed owners remain open; later scoped migrations must be reconciled before selecting from it.

The strongest L5-looking candidates are `c23PostLoad` (actual step passed through helper work and object loops), `writeM23Input` (step-derived object bound controls a writer loop), `c23SaveFiles` (actual and work steps span object/file writes), and `c23SyncTail` (step used by per-object revolve setup after viewer/work-model effects). They are genuine multi-statement spans at source level, but the existing `test_l5_loader.cpp` evidence is not yet enough to select one as the next cut: corpus 46 explicitly lacks native Convert2Dto3D and CopyMirror operations, and the current tests retag another operation; the separate chain-head case does exercise a Convert2Dto3D-looking route, but does not independently pin which proposed step-owned branch ran or its ordered effects. Several paths also depend on display/work-model state or unsupported/window conditions. That is weaker than the CP195 requirement for a disjoint source/test pair plus an actual fixture of the intended owner.

`cmSaveFiles` is not a smaller alternative worth choosing: its `m->step.get()` is used only as a null gate and in one KEY writer callback. Wrapping that single consumer would be a redundant callback. The same exclusion applies to setter-only `setSteps`/`setItrmth` and isolated `s2->step` flag stores.

## Next investigation

Before assigning an L5 refactor, bind a specific `c23PostLoad`, `writeM23Input`, `c23SaveFiles`, or `c23SyncTail` owner to the CP194 frozen source and establish an existing fixture that reaches its real multi-statement step use. Trace fixture admission, relevant model/display setup, expected ordered writes/reads, and whether the intended branch is positively reached rather than returning at a null/empty/unsupported gate. Reconcile later closure records against that exact source owner. If no existing fixture provides this, report the missing fixture reachability/expected-effect evidence and seek a genuinely reachable L5 route; do not synthesize a callback-only test seam.

The retained L5 source header cites historical decompiles/assembly; the present audit did not inspect mutable REA state or execute the original. Any eventual claim must preserve that qualification. This audit made no source, test, build, GUI, REA database, parent/DMO/Claude/original, or deployment changes. Only this report was written.
