# CP244 reaction-rate coupling review

**PASS on the corrected finite-coordinator investigation contract. The prior HOLD is resolved.** The revised note uses `EnvironmentCurveTemporaries`, removes the categorical type/ownership blocker, and proposes one synchronous admission/collision/store suffix while retaining caller-side control projection and refusal prefixes. This approves the behavioral contract and next investigation, not an unselected API or untested implementation.

Reviewed CP244 against current `reactionRateClicked`, Environment page/temporary/report declarations, the focused reaction-rate test cases, CP135 implementation/review qualifications, and the finite-owner-operation principles in CP222. No source or runtime state was changed.

## Re-review of corrected proposal

The corrected note addresses the original HOLD: finite named coordination can accept the existing editor owner state without a generic native callback or an escaping control. It no longer attributes a hypothetical scheduling hazard to the function boundary, and it correctly distinguishes final-state regression evidence from successful store-order proof.

The precise remaining gap is **API placement and its finite authority**, not missing behavioral evidence: choose the private editor-domain coordinator versus named model member, settle forward declarations/includes and the exact state/reference parameters, and document no pointer/reference retention. Keep shared-owner/page identity and other-temporary separation checks before cached control projection; perform the fresh geometry/identity and live collision checks after it. Do not add a second admission or a new failing operation between current-index and live/temporary stores. This is bounded implementation design work that can proceed under the corrected contract.

One result-shape clarification: missing/replaced control and live collision currently produce identical `ControlsProjected`/unsupported reports. A boolean success result is sufficient if the caller retains that prefix and only marks completion on true. A richer enum may distinguish causes diagnostically but is not required for behavioral fidelity. Geometry exceptions remain exceptions. Do not let the note's phrase “distinguish ... sufficiently” force a new externally observable failure taxonomy.

The coordinator need not receive the page's curve-3 owner identity merely to repeat a check already performed before projection. If it does receive that identity, preserve the original early check and do not replace it with a late-only check. Define any direct-call preconditions/selection admission in the selected API, then test them without changing valid callback order.

The analysis below retains the original review's verified source order and rationale; references to categorical blockers describe the superseded wording, not a remaining HOLD.

## Verified order and failures

The existing callback first validates binding, selection 0–3, ready/complete page state and source model/remesh. It then requires temporary 3 to exist, match the page curve's raw address **and shared ownership**, and differ by raw address from temporary curves 0–2. These are pre-projection refusals.

Next it assigns `{selection, selection == 0, selection == 0, selection != 0}` to cached ACVCOF controls and commits report stage/flag `ControlsProjected`. Only then does `simCtl()` run geometry admission and nullable control lookup, followed by non-null/current-source identity refusal. Parked geometry propagates an exception with cached controls already changed; missing/replaced control returns unsupported at `ControlsProjected`. Neither path changes current index or live/temporary curve fields.

The callback compares temporary 3 against the four live curve addresses in order ENVTMP, CNVCOF, ENVATM, ACVCOF. Collision refuses at the same partial stage. It then assigns optional currentCurveIndex=3, clears **live** ACVCOF ndim only for selection zero, assigns live var[0] from `[0,3,12,1]`, and finally assigns temporary var[0]. It leaves temporary ndim, live ndim for nonzero selections, both curves' storage/counts/payload, constant text and other metadata untouched. Completion reporting follows these stores. Repeated delivered clicks still execute them.

A mutation command accepting only expected-control token and selection cannot reproduce the current-index/live/temp order. Updating the index in the caller before admission introduces a wrong refusal prefix; updating it after live writes reorders the transition. A collision check after native mutation is also too late. The note correctly rejects those proposals.

## Type/dependency correction and smaller safe work

The actual owner type is **`EnvironmentCurveTemporaries`**, declared in `simcontrol300.hpp`, and stored as `InitialEditorSession::environmentCurves`. There is no `EnvironmentCurveOwners` declaration in the inspected headers. It contains four shared DYNARY owners and optional current index. The page retains shared const aliases to those curves; these are not scalar-only result types.

This is a cross-domain operation requiring a finite reviewed boundary, but passing editor state to a named synchronous operation is not inherently the same as exposing an unrestricted callback or returning native aliases. The newly reviewed StepCount boundary already distinguishes those cases. CP222 permits finite private domain adapters while migration is staged. Therefore “not safe under current ownership types” is too strong without evaluating a specific coordinator/API and its trusted implementation.

The smallest useful next investigation is a **single finite editor-domain operation** that receives/validates the existing editor owner state and expected-control identity, internally acquires the model's current control once, performs the live collision check, and executes index/live/temp stores in their current order. Leave pre-admission page projection in the caller. A private named domain adapter or narrowly declared member can coordinate existing owner references synchronously; decide that exact authority and non-retention contract before implementation. It must not become a public generic native-access seam. No new generation/transaction framework or C ABI redesign is required merely to evaluate this option.

An even smaller behavior-preserving prefix exists as a read-only model query for current-control identity plus temporary/live-curve collision, treating the temporary address as a comparison token. However, this alone would leave the subsequent raw-control acquisition/writes or require another admission, so it is not a useful completed alias migration and must not be presented as one. Extracting cached widget projection alone is likewise safe ordinary refactoring but removes no model alias. There is no justification for splitting the ordered store suffix into independently admitted commands.

The assertion that adding a function boundary inherently lets the barrier/model change is also unproven: a synchronous call does not itself schedule callbacks. The actual defect in the suggested split designs is their changed mutation/admission order. Preserve existing supported execution assumptions; do not invent a concurrency or reentrancy guarantee to justify either direction.

## Tests and original qualification

Current tests meaningfully cover choices, sequence/repetition, live-zero-only dimension change, pointer/table retention, partial projection on absent/replaced control, pre-projection metadata refusal, and post-projection live collision refusal. A finite coordinator should retain these and directly verify its refusal prefix with nondefault index/live/temp sentinels. A parked callback test should catch the error and inspect an already-retained page reference to verify controls changed but index/data did not; avoid `pageState() noexcept` after parking.

Final-state tests cannot prove the order of distinct index/live/temp stores or the absence of every intermediate state. Establish the simple synchronous store order by source review; add runtime instrumentation only if there is a real callback/observation seam, rather than introducing one that changes production behavior. Do not promise a test can establish general atomicity. These stores were not previously a transaction.

CP135 retains original DEF_GUI_MO_64 v11.0.2 identity, callback range/hash and bounded static trace for controls-before-current-SC and index/live/temp order. It explicitly distinguishes candidate shared-owner/identity/collision constraints from native validation. Native null SC skipping the suffix is recorded, whereas current replaced-control/shared-alias refusal is adapter qualification. This review read retained reports, not fresh original bytes or process results. No claim about actual Qt delivery, physical enabled bits, reentrancy, complete public editor or Save is justified.

Only this report was written. No source, tests, builds, shared state, GUI, original process, or REA state was changed.
