# CP134: complete the initial low-record geometry frame

The fully admitted low-record geometry invocation now completes the reader/outer/first-coordinate return and retires only its exact installed local frame while the original prepared step borrow is active. It then continues into existing connectivity/readiness handling without repeating the first getter COW. Incomplete profiles retain their barrier and effect-free parked retries.

Root authorized only viewer.cpp, test_viewer_box.cpp and this new receipt. No shared headers/CMake/public ABI were edited. No build/test execution, GUI, original launch or mutable REA action occurred. Root owns the focused/full run and source archive.

## Exact completion boundary

The new private `GeometrySyncAccess::completeInitialCoordinateRead` has one callsite, immediately after actual node Stop and NodeSkipLoopCommitted inside the initial withStep callback. It requires:

- The installed frame and supplied local frame have the same pointer **and** shared control block.
- Its live model guard identifies this model, current model.step equals the still-borrowed nonnull step, and stage is exactly NodeSkipLoopCommitted.
- Query succeeded, postVersion is signed below6000, and id/flag/which/dimension remain the qualified 11/1/0/3 profile.
- The actual borrowed step's current version still equals the retained post-query version.

Only then it records CoordinateReturned and resets that model's frame owner. The local frame remains owned until this invocation leaves scope. There is no generic clear/resume entry, no barrier reacquisition, no dereference of historical frame->step and no new step-generation claim. Static search finds the completer definition plus its single callback invocation; frame->step still appears only at its original observational assignment.

Native justification is CP133: after Stop, reader instructions only set AL=1 and return; outer which0 ignores that AL, skips other readers/names and reads current version before returning; the coordinate getter then returns existing N+8. Those normal tails have no additional model effects. The scoped caller keeps its actual model/View/optional geometry alive through this synchronous no-event path, and the strong local DbHandle lease retains the reader. GuiModelGuard itself remains nonowning; this implementation does not convert it into a lifetime lease or promise safety against external concurrent raw replacement.

On success, a stack-local completion flag reaches the enclosing readiness path and marks the first coordinate prefix completed. It skips replay of that getter and asks for connectivity. The existing null-connectivity edge-only Skip and bounded populated builder remain in control. The builder's explicit second coordinate getter is preserved. A later fresh below6000 invocation takes the normal fast sync return and performs its own getter rounds, without replaying the completed query or timers.

## Incomplete paths stay incomplete

All prior admissions remain: actual DB/receipt/index, arrays-timer negative guard, ordered actual assignment-state comparisons, pre-skip actual MAR selector bounds/load, main-object caller skips, same-step owners, zero +0x87c and node count2 before Stop. A refusal keeps the installed frame and prior COW/query/version/timer effects. The dead MAR record product/pointer remains omitted only on this no-address-consumer profile; no signed-overflow or pointee access was added.

High-record loader and null-companion object-assignment paths retain their earlier pending outcomes. Failure of exact completion checks also leaves the frame parked. Changed version/cache/tables/Viewer/DB on a later call cannot retire a parked frame. No nonzero node allocation/copy, element assignment or optional-mesh data copy is admitted.

## Focused tests and fixture separation

The directory-outcome helper now distinguishes completed low profiles from parked profiles. For successful low versions5999/0/-1/INT32_MIN it checks the actual scene result, available guarded step access, exact version/cache/array state, one arrays checkpoint and node Stop/count3. It counts getter dispatches: one per edge-only readiness invocation or two when the existing populated builder requires its own round. The first COW detaches once; later dispatches retain that body. A second fresh refresh must preserve both completed timer snapshots exactly and cannot requery.

The count-boundary and nineteen main-object profiles retain their negative/refusal expectations. For incomplete low profiles, fresh withStep acquisition must throw before invoking its callback, and scene acceptance stays unsupported. The broad parked-owner/mutator test now deliberately uses a nonzero main +0x87c, retaining its original barrier assertions rather than expecting a completed profile to stay parked.

Main-object skip fixtures create secondary objects through the existing typed step-owned get_or_create helper, leaving the Viewer with only the primary geometry View. This deliberately tests the actual main list independently of the Viewer list, and prevents skipped foreign secondary objects from entering unrelated later display consumers. It also avoids setup refreshes before the explicit traced invocation.

One new test supplies a later unsupported optional-node builder layout. The reader/timers complete and step access is available, while the scene remains unsupported at that separate builder guard. Another refresh executes fresh getter rounds without reinstating/replaying the old frame, query or timer. This covers completed-reader effects surviving a later refusal.

## Static preflight and freeze

Static preflight passed for both edited files: quoted include resolution, all defmodel angle includes including llobj_ctor, balanced delimiters after literal/comment stripping, and trailing whitespace. Testkit main and supported CHECK/CHECK_EQ/REQUIRE macros are retained; no C assert or REQUIRE_EQ. The target has48 TEST_CASE declarations (one added). No compilation or runtime pass is claimed.

| Frozen file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `c4897abb365516e9e3056c028b40ac26be4ea3df56c04e0aa0aa70fa4a7b1f61` |
| `tests/mdt/test_viewer_box.cpp` | `888f7e0b80e4a3c57c0316f4bfb46f4567a956239c88b3c658c583459198808b` |

Target `m2m_mdt_viewer_box`; CTest `m2m.mdt.viewer_box`. Controlling native/source report `analysis/resume133-geometry-next.md` SHA-256 `3a8045855263e4db0a2d585bd25d5a6591427b66b1f1d039e1edd5d9b3e561bb` pins reader/outer/getter bytes and the low-version symbolic walk. The source pair is frozen for root's build/archive. This admits only the recovered synchronous no-copy geometry path; unrelated public Editor300 palette and broader reconstruction limits are unchanged.
