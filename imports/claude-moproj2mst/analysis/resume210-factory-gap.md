# CP210: original c79390 factory versus candidate model preparation

## Question and evidence boundary

This read-only audit compares the original `.moproj` loader's child factory at `0x140c79390` with the imported candidate's `ConversionSession::prepareProjectModel` path. It uses the retained original DEFORM v11.0.2 PE analysis and disassembly (`analysis/resume210-loader-null-static.md`, `evidence/gui_mo/s1/s1-01/dc/path1.c`), the qualified outer-loader/RTTI sequence in that report, and current candidate source (`source/apps/moproj2mst/src/engine/conversion_session.cpp`, `model_manager.cpp`, and `include/m2m/engine/model_manager.hpp`). No code, original process, GUI, build, or shared evidence was changed.

## Original ordered factory behavior

The retained decompile of `FUN_140c79390` shows this order:

1. Invoke the manager's virtual `+0x68` slot with the database-path argument and retain the returned child.
2. Immediately call `0x140cdcdc0(child, path, false)` to assign the path. There is no intervening child-null test.
3. Obtain the next manager ID with `FUN_140c79470`, assign it through `0x140cdd740(child, id)`.
4. Append the child address to the manager's owning QList through `FUN_14172d7e0(manager+0x18, &child)`.
5. Install two QObject connections from the child to `manager+0x10` (the main-window receiver): `signalModelChanged(SfStepData *)` to `slotModelChanged(SfStepData *)`, and `signalModelUpdated()` to `slotModelUpdated()`. Both are passed raw connection type `3`.
6. Return the same child pointer.

The outer `.moproj` loader then checks the factory return at `1d08f..1d095`. If nonnull, it requests the RTTI conversion at `0x140146f8fc` (call return `1d0bd`) and branches to the common zero-return tail if that cast is null. Only a successful cast reaches the Project accessor/construction helper at `0x140044a90`. This is a sequential factory-return then cast decision, not a single generic “factory succeeded” condition.

Important qualification: the factory body dereferences the returned child before returning it. Therefore the outer null-factory branch is a syntactic caller branch; ordinary null from virtual `+0x68` may fault in the path setter before reaching the caller check. A source model must not turn that into a safe null result without evidence. The loader report also says the exact RTTI target is a specific type descriptor, but the candidate map has no evidence that its `DbHandle` runtime type models that native concrete class.

## Candidate correspondence and mismatch

`prepareProjectModel` requires an empty candidate manager, creates `std::make_shared<DbHandle>(Kind::Mo)`, binds the model-slot owner, sets its path, calls `appendCreatedModel`, calls `registerModelSignals`, and assigns the same handle to `Project::dataDb`. `appendCreatedModel` assigns the next ID before vector append; `registerModelSignals` records the same two signal/slot signatures with raw type `3`. The header correctly labels this as a prefix and disclaims native factory construction, QObject construction, and native main readiness.

This is a useful structural correspondence for the mutations after successful native child creation. It is not the original factory decision:

- Candidate allocation is unconditional C++ allocation rather than the manager's virtual `+0x68` factory result.
- Candidate combines native steps 2–5 in `prepareProjectModel` without retaining separate completion/failure boundaries for each step.
- The connections store a `ConversionSession*` receiver projection, whereas the original connects to the actual `manager+0x10` QObject receiver. The candidate records registration only; it explicitly refuses to simulate dispatch.
- Candidate `commitProjectModelConstruction` binds `moProject` after Project construction. It is not the outer loader RTTI cast at `1d0b8` and does not establish the factory child's native dynamic type.
- Candidate `commitProjectModelSelection` is a later identity-to-ordinal selection projection; it does not determine factory success or RTTI success.

Consequently the source supports a bounded candidate architecture step, but not a claim that the native outer-loader factory/RTTI gate is implemented or that candidate-open success implies nonnull original loader return.

## Source-ready next step

The next implementable slice is to split the existing candidate prefix into explicit factory-stage receipts, preserving order and existing refusal semantics:

- `factory-created child` (the adapter allocation event, explicitly candidate-originated);
- `path assigned`;
- `native-ID assigned`;
- `manager append completed`;
- `two connection registrations completed`;
- `outer typed-child gate` as a separately guarded, not-yet-implemented stage.

Keep the first five as a candidate execution trace/receipt only; do not label the allocation as native factory `+0x68`. Keep the typed-child gate blocked until source evidence qualifies the RTTI target descriptor and maps the native factory return's concrete class to the candidate type model. Then model the gate independently from Project construction/backlink and selection. Preserve the original's unguarded child dereference as a compatibility constraint; do not add a graceful factory-null outcome based solely on `1d095`.

The best narrow implementation seam is `prepareProjectModel` plus `ModelManager` append/connection APIs, because those already own the corresponding candidate mutations. This does not require changing the public ABI or introducing receiver signal effects. If the current task requires behaviorally closing the *original* return decision rather than recording a candidate architectural seam, the exact missing prerequisite is an original same-invocation observation of the `c79390` return and `146f8fc` RTTI return (or equivalent complete static type proof plus qualified candidate class mapping). The existing CP207/CP209 observation only derives a null outer result; it did not capture these prerequisite values.

## Limits

The report does not claim an original runtime observation of the c79390 factory, a failed factory, or a failed RTTI cast. It does not qualify the Qt receiver's runtime effects or connection lifetime. The cited pseudocode names are decompiler interpretations; the ordered calls, arguments and branch sites are supported by the retained instruction analysis in `resume210-loader-null-static.md`. Candidate source locations are current working files and have not been built or tested for this audit.
