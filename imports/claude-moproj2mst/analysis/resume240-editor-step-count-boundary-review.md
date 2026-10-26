# CP240 StepCount projection boundary review

**PASS — the reference-parameter API is feasible and preserves the bounded NSTART transition when implemented in the stated order.** No source implementation, compilation, or runtime validation was performed by this review.

Reviewed the CP240 proposal against current `engine.hpp`, `simcontrol300.hpp` page/context definitions, `toggleNstart`, the existing three-argument `fillStepCount`, the focused NSTART cases, and retained CP132/CP133 source qualifications already examined for CP238.

## Declaration and ownership

Namespace forward declarations of `simcontrol300::StepCountContext` and `StepCountPage` suffice for the proposed const-reference/output-reference parameters. The member declaration requires no complete editor type or `optional<StepCountPage>` instantiation in `engine.hpp`. The definition must include `m2m/engine/simcontrol300.hpp` before constructing/projecting/assigning the page. These are currently structs in exactly that namespace. A const model method can read its native control and assign a separate output parameter; const qualification is compatible with this operation.

Initialize the caller scratch as `StepCountPage refill{};` rather than reading any default-uninitialized numeric fields. The method must replace the entire output only after a successful existing fill. The output contains scalar projection fields plus the intentional legacy `Operation*` inside optional `MoStepCountContext`; it is not wholly owning. It contains no native control/interior alias, and the proposal correctly limits its ownership claim accordingly.

One wording correction: “Do not expose ... a control value derived from SimControl” cannot literally prohibit derived scalar page values, since producing those values is the method's purpose. Interpret and document that prohibition as no native control object, pointer, reference, span, or pointer-bearing native snapshot returned. The existing page's copied operation token remains separately qualified.

## Exact admission, effects and failures

Retain every caller guard before mutation. Store `operation.nstartGiven`, report stage `OperationFlagStored`, and `operationFlagStored=true` before calling the model method. Within the method call `simCtl()` once, with no prior expected-null or context validation. This retains geometry admission before nullable lookup, identity refusal, and `fillStepCount`'s own context validation.

Require a non-null actual control plus equality with the expected address; null/null must fail here even though the separate identity-only query allows it. Never dereference expected. Then call the existing three-argument fill with the exact supplied prior checkbox boolean. An empty optional returns false with output unchanged; success assigns the full projection before returning true. Do not use `matchesCurrentSimControl` plus a second native getter, which would duplicate admission and split identity from projection.

On false, the caller retains the operation-store prefix and unsupported report, with the cached page untouched. On true, it moves the scratch page into the cached page and only then marks stage complete, pageRefilled, and completed outcome. This extra scratch assignment introduces no native effects; the current page consists of integers, bool and optional pointer-only context. Passing the cached page directly as the output would erase the proposed caller publication boundary and should be avoided.

The three-argument fill preserves delivered checkbox history for key/null-operation cases and current class/operation behavior otherwise, current SC values, original unsigned ADD/NEG and clamps, and current eligibility-list primary selection. No fresh-fill overload, partial start-only refill, equality shortcut or Apply route is equivalent. The current caller already establishes cached checked value equals the delivery, so passing `checked` preserves the prior state used today.

Geometry/fill exceptions propagate; they neither become false nor trigger rollback. After a parked geometry exception the operation flag remains committed, cached page is unchanged, and no report is returned. The method must not be `noexcept`. Refusal for null/replaced control is the existing candidate guard, not proof of graceful native null-control handling.

## Meaningful verification

The proposed tests target distinct risks. Use a fully initialized output sentinel and compare **all** page fields, including optional operation pointer and checkbox, on mismatched expected, missing actual control, and invalid fill context. Include both expected/current null, and live actual/null expected. Keep a replaced old allocation alive when its address is used as expected. Success should verify all projected fields and unchanged SC fields, operation startStepNo, and caller-owned context.

Direct history tests should contrast prior=true/false on key or null-operation contexts against non-key writing/nonwriting branches. Existing callback tests retain the important committed-flag-before-refusal and cached-page publication checks; keep those tests rather than relying solely on the new method tests.

A parked direct call with valid context and expected=null using the existing viewer refusal helper distinguishes barrier-first behavior from premature null rejection. Check output sentinels remain unchanged too. This direct test does not by itself prove the callback flag-store-before-exception prefix; a callback parked case would add that proof if practical. Inspect a page reference retained before parking, since `pageState() noexcept` itself can terminate on the parked query. Do not claim callback exception coverage solely from the direct test.

The existing focused NSTART/viewer targets and Stopping cached regression are appropriate, with no new target or ABI change. Common source ownership and root's build/freeze sequencing still apply.

CP132/CP133 qualify original DEF_GUI_MO_64 v11.0.2 callback/setter/refill/provider evidence and preserve the SC-negation bug and hidden-checkbox history. This review relies on those retained records and current source, not fresh original execution. Actual Qt emissions/reentrancy, native enumeration, complete editor construction, Save closure, and native-control ownership closure remain outside this extraction.

Only this report was written. No source, test, build, shared recovery state, GUI, original process, or REA state was changed.
