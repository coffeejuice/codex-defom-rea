# CP245 private reaction-rate coordinator review

**PASS — the selected private member can enclose the finite ordered suffix without changing callback behavior or introducing an editor-type dependency into GuiModel.** Approval is for the concrete contract; implementation, source review and runtime tests remain required.

Compared CP245 with current `reactionRateClicked`, the private/public `StagedEditor300` declaration, `EnvironmentCurveTemporaries`/page/report types, previously inspected focused tests, and CP135 retained original qualifications. No source/build/runtime state was changed.

## Boundary and declaration

The nested enum and method belong in `StagedEditor300`'s existing private section. `GuiModel` is already available to this declaration and implementation; `pages_` owns the session and its exact Environment temporaries. No new namespace API, forward-declaration cycle, public test hook, raw callback or C ABI is needed. This is a finite editor-domain coordinator with a local native borrow, not a claim that GuiModel storage is now private or that editor/lifecycle aliases are closed.

The coordinator requires completed existing preflight and selection 0–3, and must remain callable only from the reviewed event path. Do not repeat validation late or add a second native admission. Its references/pointers may exist as local variables but must not persist beyond the synchronous call or escape through results, callbacks, queued work or other fields. Interpret “no derived pointer/reference is stored” as no **retained** alias, since ordinary locals are necessary and intended.

The algorithm's `expectedControl` is shorthand for `pages_->sourceControl`; spell that comparison explicitly or define the local token in implementation. No such parameter exists in the selected signature. Neither expected token nor cached source control may be dereferenced.

## Exact prefix and suffix

Keep every public-entry short circuit as currently ordered: binding/selection/readiness/source metadata, then temporary presence, page raw-address and shared-owner agreement, then separation from temporaries 0–2. Only after these pass assign cached ACVCOF controls and commit `ControlsProjected` stage/flag.

The private member then calls `model.simCtl()` exactly once. Geometry admission must occur before nullable/current-control testing, including with a null cached token or missing step. Require non-null current control and equality with the cached token. A miss returns `MissingOrReplacedControl` without any index or curve mutation. The caller preserves its already-projected controls and default unsupported report.

After a match, compare temporary 3's raw address against ENVTMP, CNVCOF, ENVATM, ACVCOF in that order. Collision returns `LiveCurveAlias` at the identical partial prefix. Shared ownership of native interior curves is irrelevant here and must not replace address comparison.

Then, with no additional failing operation or native lookup, assign currentCurveIndex=3; clear live ACVCOF ndim only for selection zero; assign live var[0] from `[0,3,12,1]`; assign temporary var[0] to the same value; return Complete. The optional integer assignment and plain native integer stores do not allocate or invoke callbacks. Do not set temporary ndim, reset nonzero live ndim, clone/copy a DYNARY, change table ownership/counts/payload, or refresh constant text. Repeated clicks must still execute the suffix.

Only after Complete does the caller mark complete/modelUpdated/completed outcome. Both failure enum values map identically to the existing report; the private distinction adds no public failure taxonomy. A boolean would also have sufficed, but the selected private enum is compatible.

Geometry exceptions propagate through both methods, with controls projected and index/live/temp unchanged. Neither method may be `noexcept`, catch-and-return, or roll back the control projection. The original store suffix is not a transaction; the extraction does not establish atomicity or a new concurrency/reentrancy guarantee.

## Tests and retained evidence

Existing reaction-rate cases cover the important success outputs, constant/variable and repeated sequences, table/metadata identity retention, invalid early delivery/owners, missing/replaced control prefix, and live collision prefix. Strengthening distinct index/live/temp/table sentinels is useful; do not expose the private method merely to test enum values when event behavior already covers them.

The proposed parked callback case is a meaningful additional discriminator: retain the page reference before parking, deliver a valid selection, catch the existing geometry error, and verify the projected controls plus unchanged index/live/temp state. Avoid both `pageState() noexcept` and admitted model getters when inspecting parked data; retain only owned fixture references established before parking and respect their lifetimes. The reaction-rate test currently does not itself establish this new exception coverage. If its parking fixture must be added elsewhere, preserve the same callback-level assertions rather than substituting only a direct simCtl refusal.

Review successful assignment order in the implementation; final-state tests cannot prove it or rule out all intermediate states. No production observer is warranted solely for that purpose. Root should run focused reaction-rate plus parked-viewer and Stopping cached regressions under normal grouped build/freeze sequencing; this report records no execution.

CP135 records original DEF_GUI_MO_64 v11.0.2 identity and bounded callback trace/hash for controls-before-current-SC, native null skipping the suffix, then index/live-zero-dimension/live-var/temp-var order. The present same-source identity, shared-owner preflight and live collision refusal are adapter qualifications. The new private coordinator name and enum are reconstruction structure, not recovered original symbol/interface evidence. Retained static evidence is not fresh original-process differential validation or proof of physical Qt delivery, reentrancy, public construction or Save completion.

Only this report was written. No source, tests, build artifacts, shared recovery state, GUI, original process, or REA state was changed.
