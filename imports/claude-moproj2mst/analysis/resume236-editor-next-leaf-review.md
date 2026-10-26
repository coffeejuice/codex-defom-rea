# CP236 lifecycle control identity review

**PASS — reuse the CP235 query at exactly the two proposed predicates.** The current in-progress CP235 implementation of `matchesCurrentSimControl` performs one geometry check followed by nullable control-address resolution and equality. It preserves both current lifecycle comparisons without returning a native alias. This is source/design approval, not a build or runtime result.

Reviewed `resume236-editor-next-leaf.md`, current `simcontrol300_lifecycle.cpp::applyInitialEditorPageStateBound` (lines 272–350), `gui_model.cpp::matchesCurrentSimControl`, its declaration, the CP235 owner-query tests, the lifecycle replacement/mismatch/prefix tests, and CP222's identity-token/ownership qualifications.

## Preflight order

Retain the exact short-circuit chain: incomplete page state, wrong source model, null cached control, failed current-control query, wrong remesh. Thus incomplete/source-model/null-token refusal precedes geometry admission, whereas a wrong remesh does **not** suppress geometry admission. Do not normalize all guards into a separate preflight or move the query after remesh validation.

After that chain, preserve actual-operation project/model/OPI checks, then the four environment-curve identity checks, then the cached-control dereference and page applications. The proposal calls the actual-operation mismatch regression an “earlier guard”; clarify that it is earlier than page/I/O effects, but **later than** the initial current-control/geometry query. Moving OPI checks ahead of the query would change exception precedence.

Both old and new query paths throw the existing `Error(M2M_E_UNSUPPORTED, ...)` if geometry is parked at the query point. Unlike `pageState() noexcept`, this Apply function and its public forwarding overloads are not `noexcept`; preserve propagation rather than returning an unsupported report or terminating. A non-null stale token remains a comparison value only. CP235's null/null acceptance does not weaken this initial preflight because its existing `!session.sourceControl` guard remains.

## Post-Extra3D order and partial effects

Keep all page operations and `record` calls in place. `ValidationStopped` can permit later pages/I/O, while `UnsupportedSnapshot` returns at the reached page. Only after Extra3D returns a non-unsupported result does the code clear `pages.meshing.objects`; only then evaluate `actualOperation && !model.matchesCurrentSimControl(session.sourceControl)`.

The `actualOperation == nullptr` route must not acquire this second query. On a true operation route, identity mismatch records unsupported recovery tail/outcome and returns without invoking the tail. Earlier page stores, Extra3D effects, recorded page results, and the meshing clear remain committed; do not roll them back or reset the report. If the second query throws because a callback parked geometry, the meshing clear and prior effects likewise remain, and no report is returned. The query must not dereference the expected control or reuse `sc` to obtain the current address.

The retained cached `sc` reference and other page-native uses are unchanged. The check is not a proof that aliases are safe across every callback, nor a generation identity: same-address replacement can still compare equal. This leaf must not be described as lifecycle ownership closure.

## Tests and evidence limits

The proposed preflight replacement test is appropriate. Keep the old allocation alive, give old/replacement controls distinct sentinels, and verify unsupported outcome, absent page/tail results, no Apply I/O, and unchanged controls. CP235's current direct-query cases cover live match/mismatch, live/null and missing-step/null behavior; they are not a lifecycle ordering test.

Extend the existing post-Extra3D replacement case to retain the old allocation as well, so prefix stores can be inspected safely. Assert Extra3D was reached, meshing temporaries were cleared, the prefix stores remain, the tail is unsupported, and replacement recovery storage stays untouched. The existing case replaces/frees the old step and checks replacement recovery storage, but does not establish all those prefix invariants.

The proposed focused targets are suitable. Existing `viewer_box` barrier tests support the guard implementation; without a parked Apply test they do not independently runtime-prove either new call site's position. Source inspection establishes preserved guard placement for this two-expression migration. A targeted parked preflight with wrong remesh would be a useful discriminator if added; it must propagate the existing error before the remesh refusal. Do not claim this coverage unless actually run.

Current source comments qualify the native page ordering and temporary-array clear, but the two identity predicates and synthetic step-replacement callback are adapter evidence. The proposal correctly makes no claim of a corresponding recovered native identity routine, new original execution, or process-level lifetime proof. Original frequency-handler evidence cannot establish these adapter predicates.

Only this report was written. No source, tests, build artifacts, shared recovery state, GUI, original process, or mutable REA state was changed.
