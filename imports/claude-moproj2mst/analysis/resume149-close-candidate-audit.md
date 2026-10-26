# CP149: Close candidate integration audit

## Result

The candidate still does not integrate the qualified original normal Close path. The narrow, concrete integration gap is that the current `M2M_FLAG_WRITE_SAVE` success path returns after Save/session cleanup without invoking any candidate Close coordinator. This is an intentional boundary in current source comments, but it leaves the requested single-project Open→Save→Close lifecycle incomplete. CP143/144 establish that native accepted Close must be a gated main→manager→Mo→Project path; they do not authorize replaying it from a destructor or treating adapter cleanup as equivalent.

## Source evidence

- `source/apps/moproj2mst/src/core/convert.cpp:203-206` constructs a `ConversionSession` for full Save and obtains its Project.
- `source/apps/moproj2mst/src/core/convert.cpp:207-209` binds the session only for `M2M_FLAG_WRITE_SAVE`.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp:214-227` labels the destructor as adapter final cleanup and explicitly says it is **not** original normal-Close replay; it revokes/releases adapter state and owners.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp:427-434` provides `releaseProjectAfterCloseEffects`, which requires an already-null diagram and then calls storage cleanup. Its comment says receiver/dispatcher remain live; this is a post-effects storage endpoint, not a Close entry/coordinator.
- `source/apps/moproj2mst/src/engine/conversion_session.cpp:436-448` performs storage release, resets the Project, and clears the Mo backlink only if it still points to the released Project.

The current source locations above were read from the working tree for this audit; they are not a frozen candidate identity, and no build or test was run.

## Original qualification and consequence

`analysis/resume144-close-candidate-crosswalk.md` states that native normal Close begins at FileCloseAll, checks fresh current-main/busy/child/modal/nested-Save state, executes accepted callbacks and refresh effects, then clears ModelManager children through the actual deleting virtual at `140c784f2`. It identifies missing candidate-produced main/application state and same-owner Mo/Project occurrence as prerequisites. `analysis/resume143-close-indirect-owner.md` closes the conditional native dispatch chain through `SfMoModel` vt+18, Mo Project-context cleanup, and Project SUB destruction, while explicitly leaving the candidate/live accepted occurrence unproven. Both reports use retained original v11.0.2 binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; they are static native qualification, not a live accepted-Close capture.

Therefore the discrepancy is at integration: Close is not yet wired after full Save, and the current adapter cleanup cannot satisfy the original chain. The evidence does not support adding destructor-triggered cleanup; the precise missing prerequisite is a source-produced application/main entry state and accepted owner/effect coordinator binding to the actual Session child, Mo and Project. If that producer remains unavailable, the candidate must report this lifecycle segment as unsupported rather than claim Open→Save→Close fidelity.

## Qualification

Read-only source/evidence audit only. No edits to C++ source, build, tests, GUI/app-control, mutable REA, original execution, Claude source, installed apps or DMO. No new original behavioral claim beyond CP143/144.
