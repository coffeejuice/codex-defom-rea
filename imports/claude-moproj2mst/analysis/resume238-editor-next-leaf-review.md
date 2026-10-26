# CP238 NSTART value-boundary review

**PASS — NSTART is a bounded next consumer and its stated ordering/history obligations match current source.** The header/type dependency needs an API decision, but it is not evidence that implementation must wait for a broad ownership redesign. The corrections below define a smaller next step.

Reviewed the selection note, current `toggleNstart`, `fillStepCount` overloads and page/context declarations, all eight NSTART test cases, and retained CP132/CP133 evidence qualifications. No source or build operation was performed.

## Actual order and partial results

The callback validates bound Project/Operation generation/model/remesh, constructor readiness, complete page/source-model/remesh metadata, mode 5, current-context MO operation, cached-page MO operation, and cached checkbox equal to the delivered boolean. Only then does it store `operation.nstartGiven`, set report stage `OperationFlagStored`, and set `operationFlagStored=true`.

Only after that prefix does `simCtl()` enforce the geometry barrier and nullable lookup. Null or different current control returns the default unsupported report with that stage/flag retained and cached page untouched. A geometry exception propagates with the operation flag retained; it does not return a report. Neither `pageState()` nor `matchesCurrentSimControl()` may be added before the operation store. Repeated explicitly delivered booleans still refill.

The successful path calls the **three-argument** `fillStepCount` with `checked` as prior checkbox state. The earlier cache guard establishes that this is the current delivered cached state. It replaces the whole step-count page only after the optional projection succeeds, then marks complete/pageRefilled/completed outcome. An empty projection retains the earlier prefix without page assignment.

`fillStepCount` initializes its source from current `sc.nstart`, retains prior checked state on the key/null-operation branch, and conditionally derives checked/start source from the actual operation on the non-key branch. The positivity test uses the selected source; unsigned ADD/NEG still uses `sc.nstart`. Keep the original wrap behavior, three numeric clamps, eligibility-list lookup with fallback zero, copied MO context, and checkbox history. This is a refill, so unsaved cached numeric/primary selections are overwritten. No SC field or `operation.startStepNo` is written by the toggle.

## Type ownership correction and smallest next step

`StepCountContext` and `StepCountPage` are declared in `m2m/engine/simcontrol300.hpp`. The context owns its eligibility vector. **The page is not wholly owning or pointer-free:** its optional `MoStepCountContext` contains `Operation*`, which is retained for later Apply semantics. Calling it a “copied value result” is accurate only with that explicit existing borrowed-operation qualification. Returning it would eliminate the native SimControl alias at this leaf, not close Operation ownership.

The smallest next design check is a concrete named model operation, for example an optional page projection accepting expected-control token, current context and prior checked state. It should perform one geometry admission, resolve/check non-null current control against the expected token, then invoke the existing three-argument fill synchronously inside the model-owned method. Keep the operation flag store and final page assignment in the editor caller at their current points. Do not expose a raw-control result, generic native callback or duplicate refill algorithm.

Investigate a narrow namespace forward declaration of the page/context types in `engine.hpp`, the necessary optional declaration/include, and including the complete editor-domain header in the implementation. A public declaration referring to editor-domain types is a layering choice, not by itself an impossible include cycle. Confirm the exact declaration and all consumers compile during the authorized implementation; this read-only review makes no compilation claim. If that dependency is rejected by the architecture owner, use a separately declared finite editor-domain owner operation with equivalent confinement. Do not expand into extracting all page types or redesigning Operation identities merely to migrate this leaf.

A scalar-only native observation plus a shared projection overload is another possible design, but it introduces extra types/refactoring and must preserve native-read/operation-query order. It is not required merely because the current page contains a legacy Operation token. Resolve the finite model-operation declaration first.

## Tests and provenance

The named focused target and existing coverage described in the note are accurate: prefix retention on missing/replaced current control, complete refill, repeat delivery, wrap/clamps, changed eligibility profile, key history, missing/nonwriting class, invalid event/context/binding, and construction/OPI gates. Add direct owner-operation identity/projection tests after the API is selected. Keep caller checks for no page assignment on failure, and add a parked callback test if practical: catch the propagated error and inspect the previously retained page reference plus the committed operation flag. Do not use `pageState()` to inspect a parked model, since its `noexcept` query path can terminate.

CP132/CP133 retain the original DEF_GUI_MO_64 v11.0.2 identity, callback/setter/refill/provider spans and hashes. They support store-before-current-SC/refill order and checkbox history, while explicitly qualifying primary-die input as supplied eligibility results. This review read those retained qualifications, not newly disassembled bytes. The null/replaced-control unsupported return is a candidate guard, not observed graceful native failure handling. Physical Qt signals, visibility, labels, reentrancy, native enumeration and public editor/Save completion remain unestablished by this leaf.

Only this report was written. No source, tests, builds, shared state, GUI, original process, or REA state was changed.
