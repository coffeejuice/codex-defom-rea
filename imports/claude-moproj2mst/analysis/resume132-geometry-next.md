# Geometry successor: main-object assignment decisions

The next recovered native state is assignment dispatch over the actual step's **main LLOBJ objects**, after the arrays-timer checkpoint. A narrow zero-`object+0x87c` profile closes the selected node-copy callee's early return before allocation/copy. If every eligible main object takes that return or a caller skip, the loop can reach the existing node-timer Stop checkpoint. Keep the parked reader/public geometry guard after that prefix; no later step-generation or frame-resume authority was recovered.

Only this report was written. No source edits, builds/tests, GUI, original execution or mutable REA action occurred. The investigation used retained caller ASM, existing port source and bounded original PE disassembly/byte reads.

## Immediate state and object order

Native `14082d8db` initializes a local assignment-state byte false. It compares the requested region, here id `0x0b`, in this exact priority:

| Compare site / GUI global | On equality |
| --- | --- |
| `14082d8ee`, `143dbdc1c` = A-index slot 27 | state = signed step+0x1660 > 0 |
| `14082d931`, `143dbdc30` = slot 32 | state = signed step+0x1664 > 0 |
| `14082d971`, `143dbdc3c` = slot 35 | state = signed step+0x1668 > 0 |

Only the first matching comparison is used. If none matches, state remains false. Native 3D initializer `140820a60` writes those globals to 29, 34 and 38 at `140820b6e/bA0/bbe`; canonical selected 3D tables therefore leave id11's state false without reading those step fields. Existing port `g_a_index[27/32/35]` has the same values. An integration must still compute the ordered decision from actual tables/step or explicitly qualify that table profile; a cached dimension flag alone does not repair externally changed table entries.

The loop starts local i=0, uses signed i<step+0x1378, and calls `1408302a0(step,i+1)` to obtain the main object. It also computes the MAR per-object record address using the already selected IIOBJ start and `g_mar_index[27]` stride. This address arithmetic precedes the caller skip tests, but does not dereference the MAR pointee on the zero-node callee path below.

Caller decision order is fixed:

1. Null object skips to increment.
2. Nonnull object type at +0x0c equal to 7 skips.
3. Otherwise `1408350f0` reads byte object+0x3610; nonzero skips.
4. Otherwise state true calls both node and element assignment (`1408421d0`, then `1408450f0`). Leave this branch guarded.
5. State false reads the actual elem/flag byte. Zero selects element-only assignment. Nonzero, including the present flag1 profile, selects node-only `1408421d0` at `14082dbe9`.

Do not replace the native step object count/list with the Viewer list, geometry-item count, optional RGEOM mesh, or current selected object. Existing `step_object_at` is a one-based main-object list lookup, with no new owner allocation. Loop count and lookup ordinal are separate from object IDs.

## A source-closed node-callee early return

The original node-copy prefix is independently verified:

| Site | Effect |
| --- | --- |
| `1408421f0` | Load the actual selected object's owning step at object+0x3a98. |
| `1408421f7` | Read that owner's dimension via `140100980`. |
| `140842205` | Select the owner's process-global dimension tables via `140820fe0`. |
| `14084222b` | Test signed/int storage discriminator object+0x87c against zero. |
| `140842234` | Zero jumps to `140843aef`, stack restoration and RET. |
| `140842241/225b` | Only nonzero reaches missing-coordinate test/allocation; do not enter it in this cut. |

This is specifically the **zero +0x87c** branch. Do not rename it to a generic zero node-count condition: row count is a different field, object+0x880. Existing `LLOBJ_DATA::u87c[0]` represents the consumed value. The existing port `obj_read_node_arrays` has this same owner-table-selection-before-early-return order.

For an admitted eligible main object, require its actual owner to be the same live borrowed step before calling that prefix. The selected step is dimension3, and its actual global table selector has already selected dimension3, so the normal same-owner selection is the native no-op. A foreign/unknown owner must not be manufactured or dereferenced to gain this admission. Null/type7/flagged caller skips do not read the object owner and need no fabricated owner.

The RGEOM `OBJ_MESH2D` is independent of these main arrays. `geometryQuad`'s test fixture constructs an optional geometry mesh and its own nodes/connectivity; those values are not destinations for this reader. Never copy A into that optional mesh merely because it triggered lazy synchronization. A populated optional mesh can coexist with a main object taking zero +0x87c, which makes this a potentially useful bounded continuation profile rather than a fake zero-object list.

## Useful next prefix and integration constraint

A source-ready design can admit only the current initial low-version invocation with completed CP131 timer, state=false, flag1 and the exact caller skips/eligible same-owner zero +0x87c outcomes above. Walk all actual main objects. Refuse before any nonzero +0x87c allocation/copy or element-assignment branch, retaining query/version/COW/arrays-timer effects. A local decision record alone would add no native effect; the useful bounded endpoint is the node timer's Stop after a fully skipped/zero-return loop.

At `14082dc6b` the caller sets report byte1, then calls timer stop `140832430` at `14082dc7c` on the actual node timer `143dbe000`. Stop calls checkpoint("Stop",null), calls report when requested, then calculates its ignored elapsed result. The original report `140cac130` is an empty normal RET body. In this same invocation the node timer was started and has its two entry checkpoints; the new Stop would be its third checkpoint. Use the existing actual StepTimer, preserve its bytes/order and do not invent a replacement timer or fixed clock value.

After Stop, native reader returns true at `14082dc81`. This report recommends retaining the reconstruction barrier **before admitting that whole-reader/outer-getter completion**. A later separate review must qualify the outer which0/version tail, coordinate return, optional-geometry builder and safe barrier retirement together. No timer-only endpoint supplies that completion or future resume permission.

CP131 ends its step borrow before the arrays checkpoint. A new ordinary withStep call after installation would self-refuse. The next implementation therefore needs a deliberately reviewed extension of the **one initial prepared borrow** through arrays timer and the bounded object-loop/Stop prefix, preserving native order. Do not capture a step pointer out of the old callback, use frame->step, requery on a later call, weaken general requireAvailable, or clear the barrier. A pointer/generation check cannot substitute for this scope.

Proposed exclusive files would again be viewer.cpp and test_viewer_box.cpp, only after root release. Focused acceptance should cover actual main objects with zero +0x87c and populated optional RGEOM, caller skips in mixed lists, first nonzero +0x87c refusing before allocation/copy, actual table/state and flag distinctions, and node timer Stop once with byte-exact effect-free parked retries. Prior query/version/arrays timer must remain committed on a later refusal. Keep scene-box/public geometry unsupported at this endpoint.

## Later generation remains unresolved

No native asynchronous consumer or source resume/retirement operation was found. The parked frame's step/View/mesh/object addresses remain historical observations. GuiModelGuard keeps the model generation distinct but does not lease its publicly replaceable unique_ptr step. A setter-only counter would miss direct public replacement and same-address reuse. The new profile above concerns one synchronous invocation, not a justification for resuming a saved frame later.

## Evidence qualification

Original EXE SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained caller ASM `wave3-editor-mds-gui-coordinate-lazy-read-14082d7d0.asm` hashes to `de5cdda35ab04b415ea6d094f9d2915f7b33d6deb5511bca70586c233502cd1f`. Current frozen CP131 viewer.cpp is `8e9483a5bc116e043ed7a7ef1df44d38e1be6653d3c72ab99aeb32cb039e9a3f`; read-only port reference w2_objarrays.cpp is `c567ec0c3391507f438bfe66f5121aa25dab577f40592d5a84164d4133152359`.

New bounded PE intervals, end exclusive:

| Range | SHA-256 |
| --- | --- |
| assignment-state/caller decision `[14082d8db,14082da54)` | `a28dd8c0d368ce4767a1951d81473c0a1f982ac6c36000163ead2b6713cc9001` |
| state-false flag/node-only dispatch `[14082db55,14082dbf0)` | `c727d29c6d6d9c6c171b12c183b6d8d4814d2381b216b7f8a47fb0ac94fbcaa9` |
| node prefix/zero branch `[1408421d0,140842239)` | `33e4e4f152bbc8994b47165cf7fb0dfc5e4bea77b45523249d23b11a0f0ef502` |
| node zero-return epilogue `[140843aef,140843af4)` | `c6f24503f485623e15834e7d64329e7f954b1edff51033aee48aa7274a3ace45` |
| object flag getter `[1408350f0,140835101)` | `068ea2efe40e42575cec76b4a51afd51ee695ebe588d2afe4ba45cab8168d742` |
| node Stop caller `[14082dc6b,14082dc83)` | `3fa9a4ec95a023b6688a3a13638aeb7f12be2651649cce2b430e3e0d4a0030e6` |
| native Stop `[140832430,140832498)` | `127e2473399fb1d43d8ac4703ac65d1faf833bf7a3dfa934af6795cf0b8e6307` |
| empty report `[140cac130,140cac136)` | `3eb681664cfcf7bf64d203fed036de23839edf6bba01ad098335d40d05a86738` |
| 3D A-index initializer `[140820a60,140820f80)` | `48f466ebf787981413f0190b9807e3dcb6e18132a4c76d50d6c0e594953254f5` |

These slices close the stated normal branches only. Nonzero node allocation/copy, element assignment, arbitrary foreign owners, exception cleanup and full reader completion are not established here. No new runtime witness or measurement was acquired.
