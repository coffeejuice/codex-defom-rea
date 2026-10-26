# CP215 candidate model/view ownership comparison

## Finding

The candidate source has a coherent **Project/model ownership chain** through Open, but its diagram adapter is a separate, partial timer model. It does not currently connect the selected Project/model to a concrete view during `activateSubProject`, and its diagram-binding helpers have no production call sites. The original static contract places a synchronous view setter after current-Project reread, active-subproject lookup, and model-base projection. Therefore no source-only helper increment can faithfully close this gap yet: the missing decision is whether the candidate's active `SubProject` is the exact original setter argument, and whether the original path reaches that setter with a nonnull model.

## Candidate source sequence

`core/convert.cpp:205-214` creates the session Project, prepares a `DbHandle::Kind::Mo`, runs `initProject`, and commits its `moProject` backlink. `conversion_session.cpp:282-304` adopts the Project's slot owner, sets the database path, appends/registers the child with `ModelManager`, checks its outer-loader type, and stores it in `p.dataDb`. `conversion_session.cpp:306-315` requires that same registered child and binds it back to `Project`. After `openProject`, `core/convert.cpp:241-249` commits model selection and calls `activateSubProject(p, 0, ctx)`.

That establishes candidate identity and order for Project/model construction and selection. It does not establish the diagram-view object, view ownership, or the callback from activation into view binding. In particular, `rg` over the candidate source finds definitions of `commitNonNullDiagramBinding` and `commitNullDiagramBinding` only in `conversion_session.cpp`; neither is called by production code. `commitNonNullDiagramBinding` (`:339-348`) checks that the supplied `SubProject` belongs to the current Project, stores a raw pointer, and schedules `events_.modelAttached(receiver_, true)`. The null helper (`:350-354`) clears that pointer without canceling timers. `modelUpdated` (`:404-420`) first calls `fitAndNotify`, then rereads the current raw `SubProject` binding and selects its active flow item for `clickAndUpdateMainView`. This is a deferred adapter callback, not the original synchronous setter's attach/rebuild/refresh body.

## Original static contract and mismatch

The retained CP215 map, based on copied original v11.0.2 PE `fd8ae1f8…25acc`, describes `postOpen`'s selected path: caller `2a9b0` calls `38380(M,true)` at `2aaa8`; `38380` rereads current Project through `45780` (`383ac`), applies a guard at `383c3`, obtains/lazily constructs the view with `12fa0` (`383dc`), obtains the active subproject through `a7390` (`383eb`), projects its model base through `b5a70`, then calls the view's virtual `+0x1b8` slot at `38625`. The static SfDiagCellEditor slot points to concrete setter `170430`, but the map requires checking the loaded receiver's slot. A nonnull RDX at this setter is the actual nonnull model attachment; a matching normal return at `3862b` establishes synchronous setter completion only.

The candidate `activateSubProject` call is an approximate location for that original post-open work, but source ordering alone does not show that it executes the equivalent current reread, lazy view access, active-subproject lookup, and synchronous attachment. Candidate `SubProject` ownership validation is weaker than proving that it equals the model pointer passed by the original setter: the original passes a model base plus `0x10`, not a `SubProject*`. The original setter also detaches old model state, rebuilds view/render/selection state, runs refresh virtuals, and only then requests a one-second timer for nonnull models. Candidate `modelAttached` directly schedules a logical event; its current call site is absent, and it does not model those synchronous setter effects. Its later `modelUpdated` work therefore cannot stand in for original setter completion.

## Evidence boundary and next increment

The source-closed next step is **not** to wire a candidate binding call based on name similarity. First obtain the separately reviewed, same-occurrence original witness specified by `analysis/resume215-view-binding-map.md`: within the selected `38380` frame, prove fresh current accessor RAX equals that occurrence's loader Project; view accessor returns V; the loaded V slot `+0x1b8` targets `170430`; setter RCX equals V; setter RDX is recorded (nonnull required for a positive attachment); and the setter's normal leave returns to `3862b`. Preserve null/guard/alternate-target paths as negative or inconclusive. This is runtime evidence, deliberately outside this source-only task.

Once that evidence identifies the actual model argument and confirms its correspondence to candidate state, an isolated C++ increment may be possible. Likely ownership seam: a small typed binding receipt that names the Project, its owned active SubProject, and the view-binding result, with a separate explicit null transition; however, do not implement that shape until original evidence establishes the corresponding values and ordering. If original state proves setter model identity cannot be represented by existing `SubProject` ownership, the missing model-base projection/owner becomes the prerequisite instead. No exact test target can be responsibly named before choosing the evidence-backed seam.

## Retained evidence used

- `analysis/resume215-view-binding-map.md`: bounded static call/return map, exact receiver and argument conditions, and minimal future observer contract.
- `analysis/resume215-view-xml-static-review.md`: independently checked copied-PE call destinations, setter slot and callback timing; release hold remains in force.
- `analysis/wave3-normal-close-diagram-lifetime-contract.json`: retained setter and Project-destruction null-rebind ownership order; distinguishes Project teardown from receiver destruction.
- `source/apps/moproj2mst/src/core/convert.cpp:205-249`, `source/apps/moproj2mst/src/engine/conversion_session.cpp:282-355,404-420`: candidate Project/model lifecycle and uncalled diagram binding adapter.

No original process, GUI, Frida, build, test, fixture, or shared state was touched for this comparison.
