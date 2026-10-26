# CP140 independent review: zero-width populated geometry

## Result

No blocker found in the frozen zero-width slice. Builder admission still comes only after nonnull connectivity readiness and both getter rounds. Width zero then yields a zero connectivity product and no connectivity-entry reads, while the rectangle path still walks every packed-XY node. Negative width and the existing layout, overflow, pointer, finite-value and owner guards remain active.

This is static review only; root owns build and test validation. It does not claim a public file-loader producer or reconstruct the native display vector/renderer state.

## Frozen hashes

The source and test match `analysis/resume140-geometry-zero-width.md`:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `45107ea75fca2394a3fb50444fd8807825d7ff75e1c8adaf4c486186774975df` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b1740fd1ce2ea86b37e4b1dbea4974b468652c345578d81cfa13d8f583647d18` |

## Source findings

- Readiness still requires nonnull returned coordinates and connectivity before `geometryMeshRect` is eligible. A null connectivity pointer returns the earlier Skip outcome and does not reach width/layout checks; width zero therefore cannot turn missing connectivity into a populated contribution.
- In the builder, the width rejection is now `width < 0`. The `elementCount > INT32_MAX / width` guard is evaluated only for positive width, avoiding division by zero. With width zero and positive element count, `connectivityCount = elementCount * width` is defined as zero and the validation loop performs no loads. The coordinate loop remains independent and contributes all `nodeCount` packed XY pairs. The `INT32_MAX/3` node bound remains before traversal.
- Existing required order is preserved: readiness first; builder count/count/width snapshot; second getter round; then width/layout/product checks and array traversal. The repeated high-version/cache-zero/null-companion path still executes only its two explicit rounds and consumes returned values without a third getter. CP134 exact-frame and CP135 edge-only behavior remain distinct.
- The native qualification in the receipt supports the same result: the initializer sizes the private connectivity vector to zero for width zero, the element loop has an immediate width/count exit before connectivity loads, and the actual virtual rectangle finalizer still walks the node vertices. The candidate only projects that rectangle; it does not claim to own or recreate the private vector.

## Test review

The new cases cover metadata 2/3 over ownerless, cached and actual-null owner profiles. They verify nonnull but deliberately invalid connectivity contents are ignored at width zero; the same contents with positive width remain refused until restored; a changed XY coordinate affects the fresh rectangle; and a later width-zero pass ignores an `INT32_MIN` sentinel. The actual-null profiles verify two getter rounds, second dispatch after first Stop, arrays checkpoint totals, and unchanged owner/version/cache/array data. A separate test verifies null connectivity still yields one round and edge-only bounds. Guard/restoration profiles retain refusals for negative width, N dimension3, unsupported element metadata, nonfinite/float-overflow coordinates and excessive 3N count. The two older unsupported-width tests now use -1 so they continue to test refusal rather than width-zero behavior.

## Validation boundary

No blocker found in readiness requirements, zero-width arithmetic, vertex rectangle handling, guard ordering, getter effects or test fixture assumptions. No source/test edits, build/test execution, GUI/original application interaction, or mutable REA operation occurred in this review.
