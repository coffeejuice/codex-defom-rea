# CP271 fill-stopping dimension owner query

## Decision

**PASS as a finite, source-ready owner-query substitution**, subject to root's
normal serialization and build/freeze gate. This is a separate fill entry
point from CP270's Apply leaf. Change only the first guard in
`fillStopping3DInto` from
`!model.step || model.step->dimension != 3` to `model.dimension() != 3`.
Keep `model.object(0)` immediately after it and preserve the rest of the
short-circuit order. Do not merge the fill and Apply changes into a broader
guard rewrite.

## Exact guard and effects

Current source at `source/apps/moproj2mst/src/engine/simcontrol300.cpp`
initially tests `!model.step`, then reads `model.step->dimension`, then calls
`model.object(0)`. The first two terms return a default `StoppingFillReport`
with `StoppingFillIssue::InvalidModel`; no locale/profile check, page write,
scalar read, selector work, or model mutation has happened. `object(0)` is
reached only for dimension 3. `GuiModel::dimension() const noexcept` returns
`step ? step->dimension : 0`, unguarded and without admission, so replacing
the first two terms with `model.dimension() != 3` preserves the null-step and
all non-3D refusal cut.

For a parked geometry-sync model, the dimension query itself does not throw.
A parked non-3D or absent-step model still returns `InvalidModel` before
`object(0)`. A parked 3D model reaches `model.object(0)`, whose first action is
`requireGeometrySyncAvailable()`; it throws there before locale/profile
checks or any fill effects. Do not remove, hoist, or replace this object query
with a broader parked-model check. For ordinary live 3D, its null-object
result permits the existing locale and retained-widget-profile checks, then
the ordered scalar/reference/threshold/rotation fill.

## Caller, evidence, and test boundary

The actual reconstructed lifecycle caller is
`simcontrol300_lifecycle.cpp`: `Page::Stopping` invokes the retaining
`fillStopping3DInto(pages.stopping, ...)` and marks the page supported only
when its report is complete. The optional `fillStopping3D` convenience
overloads also reach this entry but discard an incomplete local page. Snapshot
tests call the real fill entry for successful projections, retained widget
state, partial failures, and node callback effects. Lifecycle tests establish
that a reached stopping-page failure is retained and Save stays refused.
These are reconstructed caller/test seams; they do not establish ordinary
public MO UI reachability or fresh original-runtime behavior.

The bounded admission test should call `fillStopping3DInto` itself with a
valid retained page and an ordinary non-3D model, using an independently live
`SimControl` reference. Assert `InvalidModel` and byte/value preservation of
the supplied page and control, with no step recreation. A missing-step case
may be separate; do not retain a `SimControl&` into a destroyed step. A parked
3D case should preserve the existing `object(0)` barrier behavior; parked
non-3D should return before it. Keep existing successful, partial-fill,
Apply, and lifecycle cases unchanged. Use the existing focused snapshot CTest
and include lifecycle regression in root integration if the root gate calls
for it; do not add GUI/original runtime claims.

## Reconciliation

CP270's reviewed Apply change has the same scalar-query equivalence but a
different report type and following Apply admissions/stores. CP269 changed
the distinct `fillMeshing3D` admission and explicitly retained its downstream
object-access order. Earlier stopping-page fixes recovered the retained-page
projection, ordered partial effects, node callbacks, and Save refusal; this
guard substitution neither alters nor reopens those contracts. The native
static capture for `0x140ebd180` supports the stopping fill's widget/control
order, not a claim that this reconstructed owner predicate is a native
predicate. Record this as removal of a direct step alias at an already
reconstructed internal caller, not as a new feature or public reachability
proof.

No source, tests, build, GUI, original process, shared recovery state, or
mutable REA state was changed in this review.
