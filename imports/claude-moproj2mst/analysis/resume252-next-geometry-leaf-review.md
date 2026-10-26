# CP252 next geometry leaf independent review

## Verdict: PASS within the stated normal-result boundary

The proposed `displaySecond` leaf is well scoped. Current source reuses an existing second first, returns null when no main wrapper exists, then constructs with `guiModelDim(*b)`. That helper ultimately uses `GuiModel::dimension()`, which returns `0` without a step. Replacing only that argument with `b->dimensionOr(3)` returns the original getter's default `3` for a live main wrapper with no step, while preserving any live step value exactly. It does not confuse a present dimension `0` with an absent step and adds no geometry admission.

The retained original full function confirms this is a distinct safe no-step path from `displayNamedModel`: after constructing with the dimension getter's result, `displaySecond` publishes the new secondary, sets its display backlink, and queries controls. The main-control getter is nullable: it resolves the wrapper's step and returns null if absent, so the main GEOTYP copy is skipped. There is no main-step `+0x744` or title dereference in this function. Under the report's normal allocation/constructor and stable-wrapper assumptions, static code supports a completed dimension-3 secondary with no main GEOTYP copy. It does not prove natural fresh-project reachability or every exceptional path.

The planned caller fixture is meaningful: actual `displaySecond` calls after removing the main step; asserts dimension, backlink, and display ownership; verifies repeat-call identity; exercises missing-main behavior after main destruction; then drops the second and checks no recreation. Separate 2D/3D live-step cases guard the existing result. The document properly excludes named-model behavior and does not claim that these converter tests prove physical GUI delivery or original no-step occurrence.

The publication-order discrepancy is explicitly kept out of scope: original code publishes the secondary before backlink/control work, while current converter keeps it local until after those operations. The one-expression dimension change does not repair or claim parity for that prefix. The document also acknowledges current `simCtl()` geometry admission differences, so the no-step result is limited to an unparked/no-barrier fixture. Static DUMPBIN/decompile identity, byte-range hashes, and no-runtime-execution qualification are stated; no actual original occurrence is claimed.

No source, build, GUI, or mutable REA state was changed or run for this review.
