# CP233 page-state identity query review

**PASS — the proposed value query preserves the current reconstructed predicate and geometry admission point.** This approves the bounded design, not an implemented or tested change. Keep the corrections and validation qualifications below attached to implementation.

Reviewed `resume233-editor-next-leaf.md`, current `simcontrol300_owner.cpp::pageState`, its header declaration, `gui_model.cpp::simCtl`/`requireGeometrySyncAvailable`, `simcontrol300_lifecycle.cpp::openInitialEditorInto`, the owner tests, the parked-model refusal tests in `test_viewer_box.cpp`, and CP222's alias/closure notes. No original process or mutable REA state was accessed.

## Exact behavior to retain

The current `pageState()` first checks the original operation identity and returns null if expired. It then executes `auto* model = model_.get();` **before** evaluating the state/page conditions. The following `||` chain evaluates constructor state, page existence, page completeness, non-null model, source-model equality, and finally control equality. The proposal's prose compresses the model guard into the later condition order; leave the actual `model_.get()` statement where it is and replace only the final operand.

The replacement `!model->matchesCurrentSimControl(pages_->sourceControl)` must call the geometry guard once, derive `step ? &step->sim_ctrl : nullptr`, and return direct address equality with `expected`. Expected null, stale, or mismatched tokens must not be dereferenced or rejected before the guard. Unlike frequency mutation commands, this query permits null/null equality. `openInitialEditorInto` normally records a non-null source control after its null/dimension check, but that normal construction fact does not authorize strengthening the query's predicate.

Keep the existing OPI/model/page short circuits and `noexcept` on `pageState()`. The query itself should have the proposed potentially throwing signature. Do not route through `setDimension`, reset, page construction, a snapshot, or another getter that duplicates admission or adds null policy. There are no control-field reads, stores, widget changes, or geometry continuation effects in the accepted operation.

## Parked geometry and exception qualification

`requireGeometrySyncAvailable()` throws `Error(M2M_E_UNSUPPORTED, "geometry query is parked before unrecovered reader continuation")` whenever this model has a barrier, before current-step/null/address resolution. Direct calls to the new query must retain that same throw even with null expected or a missing step. A mismatch cannot bypass admission.

When reached through `pageState() noexcept`, the barrier exception cannot be caught by a caller around `pageState()`; crossing that function's exception boundary invokes `std::terminate`. This is established from the current C++ declarations and throwing guard, not a newly observed original-binary behavior. The proposal correctly retains this existing termination behavior. Catching the exception and returning null, removing `noexcept`, or marking the new query `noexcept` would be a separate behavior change. Calling the condition only an exception that “propagates” through `pageState()` would be inaccurate.

## Evidence and regression qualifications

CP222's alias ledger explicitly lists this cached-control identity ingress; its broader ownership plan does not establish original provenance for the adapter's `pageState()` method. Retained frequency-handler evidence does not establish this method's semantics. The proposal correctly limits the claim to source-preserving adapter extraction. It also correctly makes no ownership-closure claim while the raw expected token, public native step, and other consumers remain.

The retained-step replacement test is appropriate: keep the old allocation alive, install a distinct fresh step, and verify null `pageState()` with unchanged `partialPageState()`. Also directly test the new query's live match, live mismatch, null expected with a live step, missing current step with non-null expected, and missing current step with null expected (true). Direct parked-query cases should verify the existing error, including expected-null and missing-step variants where practical. Existing `viewer_box` parked `simCtl()` coverage alone will not exercise the new query's body.

Do not run a parked `pageState()` as an ordinary exception test in the shared test process. If runtime proof of its termination is wanted, use a separately contained death-test process and require evidence from it; this design review requires no such new execution. Existing earlier-guard tests support short-circuit behavior, but without parking they do not independently prove that those guards suppress a throwing geometry lookup. Source inspection supports that ordering here.

Only this report was written. No source, tests, build artifacts, shared recovery state, original application, GUI, or REA state was changed.
