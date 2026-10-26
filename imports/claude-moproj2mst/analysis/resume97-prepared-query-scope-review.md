# CP97 review: prepared pre-install `withStep` scope

**Assessment: the proposed narrow scope is source-order compatible, with one explicit assumption to keep visible.** It is suitable for root to consider authorizing as a same-invocation prepared-call scope; it must not be described as a native-step lease or as permission to resume later.

## Order and owner review

The CP96 sketch acquires `withStep` at the current `GeometrySyncAccess::install` statement, after actual model/step and geometry identity qualification, timers, the strong `Project.dataDb` owner check, reader identity, current directory receipt/index/extent checks, record-version check, frame allocation, and frame metadata snapshots. The low-record branch continues to install before the query's conditional native `step_version` mutation. It then performs that same query once, stores its result in the same frame fields, samples post-version from the borrowed `nativeStep`, sets `QueryCommitted`, ends the callback, and returns the same pending result.

This ordering fits the current access helper: `GuiModelAccess::withStep` calls `requireGeometrySyncAvailable()` before obtaining `model.step.get()` and invoking the callback. The callback would be admitted while the model is unparked. `GeometrySyncAccess::install` only rejects a null frame or an already-present barrier, then moves the shared frame into the model. It does not invoke user code or replace the native step. The callback is already active when installation makes later public entries refuse; it does not re-enter `withStep` or any guarded public mutator. Capturing the model for this synchronous callback and capturing `frame` by reference do not themselves escape the borrow. The actual DB owner local remains alive across the scope, keeping the reader valid.

The callback's early `return` on install failure is a void-lambda return: it skips query/post-version/stage, then outer code returns the existing pending result. On exception after successful install, unwinding leaves the installed frame and any query partial effects as the current path does; the sketch adds no retry, catch, or frame retirement. The frame's historical `step` pointer remains an observation stored outside the callback, not an authorization token.

## Exact source risk and test

The key risk is an unrecognized reentrant/concurrent path that can replace `GuiModel::step` between the pre-install observation and callback acquisition, or run another guarded model operation while this callback is in progress. The source sketch correctly states the required callback-free, serialized prefix; implementation review should confirm the caller path does not emit events or dispatch unknown code in that interval. The install check itself protects against a second barrier, but it does not synchronize concurrent raw `step` replacement.

The most focused regression is the proposed assertion in the existing low/high directory-query fixture: capture `candidate.model.step.get()` before refresh, call the refresh, then immediately require the same pointer afterward before using the saved native-step reference. Apply it to low and high record rows. This verifies no step replacement across the admitted query and catches an accidental fixture/source owner swap without exposing private frame fields. Retain the low-version four-row first-call mutation/pending checks and the two-View parked retry assertion to prove install-before-query and continued refusal. The install-failure branch is not normally forceable without artificial interference; no corrupt-receipt or injected callback test is justified merely to reach it.

## Boundary

I found no source-order reason to move installation, weaken general `requireAvailable`, or keep the borrow beyond `QueryCommitted`. Approval should be limited to this exact same-invocation callback and the focused pointer-stability test. Do not infer that it solves replacement/ABA lifetime for a later continuation: the public replaceable `unique_ptr` and raw mutation surface still have no allocation-generation observer or retirement contract. No implementation or build/test action was performed for this review.
