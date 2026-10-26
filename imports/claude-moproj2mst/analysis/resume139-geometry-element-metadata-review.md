# CP139 independent review: packed-XY mesh with E metadata 3

## Result

No blocker found in the frozen source/test slice. It preserves the supported node storage profile `N.u138 == 2`, admits element metadata values 2 and 3, and tightens the signed triple-stride bound to `INT32_MAX/3` before coordinate traversal. The changed layout/count checks remain after the caller's count/count/width reads and second getter round. Other builder/data refusals still leave only the edge prefix and mark the mesh item unknown.

This is static source/test review. No build/test was run here; no public loader or original runtime parity claim follows from this private allocated-mesh projection.

## Frozen hashes

Hashes match `analysis/resume139-geometry-element-metadata.md`:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `b35f4e69d675e6029a30ccad2eba010b7418cf6c0bd01c27e6141c3eb7af247d` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `b4d35808ab915c836d76a4caa30df47601bffa8ab7e24bc3b96378ed78dafd02` |
| `analysis/resume139-geometry-element-metadata.md` | `01c912ca7c7f84b20ad1f50a09d8d8af66fe6479d59cbf30298aa65d9f60833e` |

## Source review

- `geometryMeshRect` keeps the positive width and checked `elementCount * width` rule, but now accepts only `E.u0f8 == 2 || E.u0f8 == 3`. It still requires packed node data `N.u138 == 2`. The builder's source-qualified flat-array path consumes width/counts and 2D coordinate pairs; the actual rectangle virtual target walks triple-stride output and does not inspect E metadata. This is enough for the bounded already-allocated representation, not proof of a native loader producer.
- The code snapshots node count, element count and width first, then runs the builder-round getter for `RepeatedGetterRequired` (or the legacy getter pair for existing `Populated` paths). Empty-contribution handling and layout/data guards follow those getter effects. The new `nodeCount > INT32_MAX/3` check prevents the native initializer/finalizer's signed 3N stride from wrapping and precedes all coordinate/index traversal. `elementCount * width` is independently checked before multiplication.
- The repeated null-companion path still performs exactly two rounds: readiness produces `RepeatedGetterRequired` only after first-round completion and nonnull connectivity; the builder reads counts/width, calls one BuilderNullCompanion qualifier, and consumes its returned pointers. It does not fall into the legacy getter branch, so there is no third coordinate/connectivity getter. The builder qualifier retains same current model/step owner and null DB/dimension-3/+0x1b58-zero/high-version/cache-zero checks before and after the second COW/table selection.
- CP134 low-record completion remains in its initial borrowed callback and the low/query barrier path is not otherwise changed. CP135 null-companion edge-only still completes one reader round and returns Skip when connectivity is null. First-/second-round failures and the low-record parked profiles remain separate from this metadata/layout guard.

## Test review and assumptions

The three new Testkit cases match the receipt's 12 valid profiles, 16 later-layout/data refusals, and four triple-count refusals. Valid cases compare metadata 2/3 and widths 3/4 across ownerless, cached and actual-null profiles; they check equal scene bounds, current ownership, two dispatches, reader/timer order where active, and unchanged coordinate/connectivity bytes and metadata. The invalid-profile test checks N metadata 3, E metadata 0/-1, zero width, bad connectivity, NaN, infinity and float overflow, then restores valid fields and expects a fresh two-round contribution. The triple-bound test uses `INT32_MAX/3+1`, proves it is still under the prior `/2` limit, retains tiny allocated arrays, checks both getter effects and scene refusal, then restores the true count before fixture cleanup.

The huge-count safety claim depends on the canonical `Cls_14017ef20` registration containing no ID 0 and on key-0 `set_all` not resizing the other registered IDs; source confirms the 28-entry registration skips zero and the base/node-post slot-2 methods resize only matching IDs. The test's actual step main-object zero gate also prevents unrelated node-array reads. This supports a bounded arithmetic refusal without attempting enormous allocations or traversal.

## Validation boundary

No blocker found in metadata/layout qualification, arithmetic ordering, getter sequencing, or prior edge/low-path guards. No source/test changes, build/test execution, GUI/original application interaction, or mutable REA operation occurred in this review.
