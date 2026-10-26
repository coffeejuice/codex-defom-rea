# CP132: bounded main-object zero-gate assignment prefix

The initial low-record geometry invocation now walks the actual main-object list through native caller skips and same-owner zero-+0x87c node-helper returns, then records one node-timer Stop. It remains parked before whole-reader completion. No assignment allocation/copy, optional RGEOM mesh write, resume, requery or barrier retirement is enabled.

Root authorized only `src/engine/viewer.cpp`, `tests/mdt/test_viewer_box.cpp` and this receipt. No other files were edited. No build/test execution, GUI, original launch or mutable REA operation occurred. Root owns the grouped build and source archive.

## One initial borrow and exact decision order

The existing prepared `withStep` callback now spans install/query, CP131's arrays checkpoint and the bounded object decisions. It still installs before version mutation, records the query and post-version, and requires nonzero query result plus signed postVersion<6000. Negative arrays count preserves the query/barrier and refuses before timer writes. No pointer escapes the callback and no blocked step borrow is reacquired.

After the arrays checkpoint, the code computes native assignment-state from actual `g_a_index[27]`, `[32]`, `[35]` in that priority and the corresponding actual step fields `u15f8[0x1a/1b/1c]`. It does not hardcode the canonical 3D false result. It traverses actual `u1378` main-object ordinals with existing `step_object_at`. After each lookup and before any null/type/flag skip, it reads actual `g_mar_index[27]`, admits only indices in `[0,0x8c)`, then loads the actual step's `u1b90[index]`. Ordinal/index/stride are retained as private frame diagnostics. Null, type7 and nonzero byte+0x3610 skips then follow in native order.

Only after those skips does it require state=false, flag1, `object->owner == nativeStep` and exact `object->u87c[0] == 0`. This discriminator is not the separate row-count field. The existing `obj_read_node_arrays` is then called with the same actual step's MAR/A/table arguments and id11. Its source-qualified prefix selects the same owner's dimension tables and returns before allocation or copying. Foreign/null owner and nonzero +0x87c refuse before invoking that helper, preserving preceding query/COW/timer effects. No detached owner or duplicate model is created.

If all objects take caller skips or the admitted return, the actual node timer must still have its two entry checkpoints. The code explicitly retains a guard if that count differs from 2. It calls existing Stop with report byte1, then records `NodeSkipLoopCommitted`. Native report is empty; the existing timer records Stop as its third checkpoint. The method still returns object-assignment pending with its model barrier installed; this stage does not admit the native reader return or outer getter/builder.

## MAR arithmetic review

Independent review highlighted native 32-bit IMUL low product followed by sign-extension and LEA. This implementation introduces **no C++ MAR record product or pointer arithmetic**. On the admitted state=false/flag1 node-only path, the caller's computed per-object record address has no consumer; the node helper receives the original MAR base and returns before any MAR/A pointee access. Its pure unused record-address calculation is omitted, rather than expressed as overflowing signed C++ arithmetic or null/out-of-range pointer addition.

The first freeze omitted the distinct native pre-skip stride-table access. `analysis/resume132-geometry-node-skip-review.md` identified this material admission gap; that source/receipt freeze is superseded by the repaired hashes below. The repaired per-iteration guard/load occurs exactly after object lookup and before caller skips. The 0x8c extent is the actual typed step MAR address table. The already reached 3D selector and same-owner callee selection establish the table-selection interval; no later callback/foreign selector is admitted. A malformed selector refuses before its step-table read, preserving preceding query/COW/arrays-checkpoint effects and the barrier. Every raw int32 value at an in-range entry is safe to record; only its unused address product is omitted. No C++ out-of-bounds read is generalized as native success.

This omission applies only to this recovered no-copy prefix. Element assignment and nonzero node work still refuse. A later implementation that consumes the record address must recover its modulo32/sign-extension arithmetic and physical extent separately. Directory sizes are not used to manufacture a MAR/A allocation or capacity.

## Focused tests

The retained low-version and CP131 count-boundary tests now expect node Stop only when the initial nonnegative arrays checkpoint reaches the admitted loop. Negative arrays count keeps two node checkpoints. Both timer snapshots are compared byte-for-byte on the parked second refresh; no second coordinate getter, COW, timer or query is allowed. The broad parked-model/owner-bypass test expects the new initial Stop and retains all its later refusal checks.

One new Release-active case has nineteen explicit profiles over three actual main objects and a populated optional geometry mesh:

- Normal zero +0x87c objects complete the loop; first/later nonzero gates refuse before allocation/copy.
- Foreign and null main-object owners refuse. Type7 or byte+0x3610 skips do not demand an owner, even with a nonzero node gate.
- Each special assignment-state comparison can select the guarded true branch. First-match zero takes priority over a later matching positive field.
- INT_MIN/INT_MAX IIOBJ start/stride values across multiple objects retain null MAR/A bases and complete this no-address-consumer profile, without a signed product or pointer formation.
- MAR selectors -1, 0x8c, INT_MIN and INT_MAX refuse after the arrays checkpoint with no node Stop; in-range boundary selectors 0 and 0x8b with extreme stride values complete the safe load/no-address-consumer profile. Actual selector and complete step MAR table bytes remain unchanged, including on refusal.

The test checks unchanged main node-storage bytes and optional coordinate identity, actual owner values, low version/cache/array/object state, timer labels/null subs/counts, unsupported scene box and byte-exact no-replay behavior. Scoped restorers restore deliberately varied object owner/type/gate/skip fields before destruction, including Testkit early exit. Clock values are not fixed.

## Focused-run fixture repair

Root reported the first focused runtime run failed only the new node-skip case with uncaught `geometry query is parked before unrecovered reader continuation`; the other 46 cases passed. That result is not a passing candidate. Source diagnosis found the fixture constructed objects 2/3 after its low-directory lazy-geometry owners were already armed. `GuiModel::objectWrap` calls viewerSync for a created object even when its withViewer argument is false; the viewer can refresh existing RGEOM and install the low-query barrier during setup, before the explicit traced refresh. The next object admission then correctly refuses.

The repaired test sets actual step.u1b58=1 while constructing the three main objects, then restores 0 before configuring profiles and the explicit refresh. This matches the existing broad parked-model fixture's setup ordering and the recovered nonzero-flag bypass. It neither catches an unexpected setup exception nor weakens public continuation. Production viewer.cpp is unchanged from the reviewed MAR-load repair. Static preflight was repeated; root must rerun the focused/full build tests. No successful rerun is claimed by this receipt.

## Static preflight and frozen files

Static preflight passed for both edited files: quoted include resolution against actual source/include roots, balanced delimiters after comment/literal stripping, and trailing whitespace checks. The test has a Testkit `main`, 47 TEST_CASE declarations (one added), supported CHECK/CHECK_EQ/REQUIRE assertions, no C assert and no REQUIRE_EQ. No compilation/test success is claimed.

| Frozen file under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| `src/engine/viewer.cpp` | `3ea1b91dcf933def29aca5ab253971b8aee3a4670c2dfef57a77732ca6c6e6fa` |
| `tests/mdt/test_viewer_box.cpp` | `07d2343b77a5d683d26a77c2bbaa7b049922add3ba3e76cca3b8dd5a30a4b814` |

Focused target: `m2m_mdt_viewer_box`; CTest: `m2m.mdt.viewer_box`. Native bounds/hashes, object-field qualification, Stop/report and caller order are in `analysis/resume132-geometry-next.md` SHA-256 `f581b63e471b846841a69b16d1ab8e79bbb4c93d94ae554ad53bbc4b68d639f5`. Source is frozen for root's grouped build/archive. Later step-generation, saved-frame continuation and full public geometry completion remain unresolved.
