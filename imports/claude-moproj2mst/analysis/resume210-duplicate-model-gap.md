# CP210: outer duplicate-model branch versus candidate preparation

## Finding

The candidate does **not** currently implement the original outer `.moproj` duplicate-model decision at `0x14001cf0c -> 0x1400c78c10`. Candidate code requires an empty `ModelManager` and proceeds to create/append a fresh Mo child. Static evidence supports an implementation of the *decision predicate* (matching resolved database path in the existing manager), but does not support making public fresh single-project conversion behave as though an observed native duplicate always returns null. Keep the public fresh path unchanged until the original manager state and branch outcome are witnessed for the target invocation.

## Original decision and evidence

`analysis/resume210-loader-null-static.md` records exact PE instruction evidence for the B98 `.moproj` branch: outer loader `0x14001ca30` obtains the main-owned manager, calls `0x1400c78c10` at `0x14001cf0c`, checks its return at `0x14001cf16`, and branches at `0x14001cf1c`. A nonzero matching-child result takes the duplicate-ID information path and explicitly returns a null Project (`0x14001d04c..0x14001d06b`); zero continues to model creation. The report explicitly qualifies that the inspected condition concerns manager children, not `.DB` file existence, and that a fresh process does not prove an empty manager.

The available original evidence establishes the branch logic, not that it fired in B98. CP207/CP209 did not capture `c78c10`'s return or startup manager population; the recorded fault producer is a later call at `0x14001d510`. Consequently no behavioral claim that B98 took the duplicate branch is warranted.

## Candidate path and nearby helper

In `source/apps/moproj2mst/src/core/convert.cpp` (`run()`, around lines 209–248), the full-save path derives `db`, calls `session.prepareProjectModel(db)`, constructs and binds the Project, opens the project, then commits selection. `source/apps/moproj2mst/src/engine/conversion_session.cpp` (`prepareProjectModel`, lines 277 onward) refuses unless manager size is zero and `p.dataDb` is unset, then allocates a Mo `DbHandle`, sets its path, appends/registers it, and binds `p.dataDb`. Thus it cannot represent a pre-existing matching child or the original duplicate-null return.

`source/apps/moproj2mst/src/doe/dblist_manager_prefix.cpp` has a superficially similar last-matching-path lookup, but it is specifically the isolated DOE `DBLIST` existing-file branch. Its header documents a different caller contract: caller-qualified mode/gate, resolved path and `QFileInfo::exists()==true`; a miss deliberately stops before native construction. It is not called by `prepareProjectModel` and is not evidence for the outer loader lookup. Reusing it directly would conflate an inner DOE import with the application manager's `.moproj` open path.

`source/apps/moproj2mst/src/engine/model_manager.cpp::modelWithId` is an ID lookup (`c78ee0` qualification), not a path lookup. It rejects nonpositive IDs and searches `modelId`; it cannot substitute for `c78c10`'s path-based duplicate test. The candidate's path representation is available on `DbHandle` through `pathSet/path`, but equivalence to the exact native `c78c10` comparison/normalization requires the referenced DBLIST path qualification rather than assumption.

## Source action and limits

Recommended action now: **no behavioral source change**. Preserve the empty-manager precondition for the agreed fresh-session public conversion profile. If future source work is authorized within the same recovery scope, add a narrowly named outer-open lookup seam only after qualifying the exact `c78c10` path construction/comparison, traversal order (the retained DOE prefix says last match), and input path normalization. It should return a borrowed existing child identity without mutation; the caller can then model the native duplicate result as null. Do not route this through the DOE DBLIST helper or alter the C ABI/fresh-project result without an evidence-backed session-state contract.

The missing witness for any public behavior change is a complete same-invocation original record tying: outer call identity and selected `.moproj` path; manager identity/population at `c78c10`; lookup argument; `c78c10` raw return; and outer return. CP210's proposed ordered capture record is suitable in principle but remains unqualified and unreleased. A nonzero lookup with the matching outer null return would validate the causal branch. A zero lookup would eliminate this branch for that captured occurrence but would not establish general manager-state behavior.

## Scope and qualification

Offline source/evidence audit only. No code, tests, shared state, original process, GUI, build, or fixture was changed. Evidence is limited to the retained CP210 static report and the candidate source named above; it establishes source structure and documented qualifications, not runtime equivalence or whole-project acceptance.
