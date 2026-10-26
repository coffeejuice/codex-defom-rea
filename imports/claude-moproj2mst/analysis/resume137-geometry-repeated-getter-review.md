# CP137 independent review: populated null-companion repeated getter

## Result

No blocking defect found in the frozen CP137 source/test pair. The readiness round completes one qualified high-version/cache-zero/null-companion reader and only requests the second round when connectivity is populated. After the builder's count/count/width reads, the builder round revalidates the live owner/profile, performs one more COW/table/reader/arrays/main-object/Stop sequence, and returns its coordinate/connectivity pointers for direct consumption. The repeated path has no third legacy coordinate/connectivity getter. Connectivity-null edge-only and CP134 low-record retirement remain distinct.

Static review only; root owns the build and runtime test result. This does not claim original-process differential parity.

## Frozen hashes

The current files match `analysis/resume137-geometry-repeated-getter-implementation.md`:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `1f46c1731f92968fb178ad1d0e9018af6be3fd3cdb14743c1115e2bbdb6cf872` |
| `source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp` | `d8fd76ae0a63d030aa4cd99e98465dad9a17bb73117ec4a3c99b89362cdb03ee` |
| `analysis/resume137-geometry-repeated-getter-implementation.md` | `55c0bc494a45fae46ce11d7d1542410c81e0713b4f54183ce00cfe5b6b31fddd` |

## Source review

- The readiness qualifier's initial admission binds both the nonnull node owner and any element owner to `view.ownerModel` and the model's current step. For the builder round it additionally requires a returned-value sink, same live step, and the exact null companion/dimension-3/+0x1b58-zero/high-version/cache-zero profile before effects. After the actual key0 COW and table selection it rechecks the null profile before timer/reader work.
- Each admitted null round starts the node timer, records its entry checkpoints, takes the actual null-companion branch, appends the arrays checkpoint only when count permits, and uses the shared ordered main-object loop. That loop checks the actual MAR selector and stride before caller skips, then requires the same-step object owner and zero +0x87c for the only admitted no-copy object read; Stop occurs only at count 2. A first-round refusal cannot produce `RepeatedGetterRequired`, so no second round is attempted.
- First readiness round uses the qualified coordinate-prefix/reader behavior and returns the current N+8 coordinate pointer plus the result of the connectivity getter. Connectivity null returns Skip before the builder; populated connectivity yields `RepeatedGetterRequired`.
- `geometryMeshRect` reads node count, element count and width before entering the BuilderNullCompanion round. That round returns the current coordinate pointer and performs the second actual connectivity access. `geometryMeshRect` consumes both returned pointers and does not call the legacy getters in this branch. Legacy getter calls remain in the separate `Populated` branch for previously qualified fast/cached paths.
- No step pointer is carried in the returned-value structure. Each round uses a fresh synchronous `withStep` callback. CP134's `completeInitialCoordinateRead` remains called once only by the low-record frame path; the null companion path does not create or retire that barrier. CP135's edge-only null case still has one completed round and returns Skip. Existing incomplete and low-record barriers remain guarded.

## Test coverage and assumptions

The new matrix covers four high-version boundaries, seven arrays timer capacities/counts, and both connectivity states. It checks one round for edge-only or first-round refusal, two rounds for populated/nonnegative profiles, COW dispatch ordering (second dispatch after the first Stop), arrays checkpoint capacity, scene outcome, pointer/storage invariants, and the repeated behavior on a fresh refresh. The additional test keeps first-round effects when the second reader refuses at the main-object gate, and checks that a later valid refresh completes without replaying or retaining a barrier. The source receipt's count of 50 `TEST_CASE` declarations matches the file.

The second-gate refusal uses a test-only observer at the second COW's dynamic dispatch to alter the main object's +0x87c gate. This is an explicit reentrant fault injection, not an original UI timing witness. The check demonstrates the second reader's refusal ordering and retained effects under that injected state; it does not qualify real concurrent mutation or synchronization.

## Validation boundary

No blocker found in requested order, guards, pointer consumption, or prior-path preservation. No source/test edits, build/test execution, GUI/original application interaction, or mutable REA operation occurred in this review.
